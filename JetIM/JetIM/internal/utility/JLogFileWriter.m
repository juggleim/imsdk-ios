//
//  JLogFileWriter.m
//  JetIM
//
//  Created by Nathan on 2024/5/21.
//

#import "JLogFileWriter.h"
#import "JUtility.h"
#import <zlib.h>
#import "JLogger.h"

#define jLogFolder @"jlog"
#define jLogFileFormate @"yyyyMMddHH"
#define jLogExpireInterval (7 * 24 * 60 *60)
#define jOneHour (60 * 60)

@interface JLogFileWriter ()
@property (nonatomic, copy) NSString *logFolder;
@property (nonatomic, copy) NSString *cachedTimeString;
@property (nonatomic, strong) NSFileHandle *cachedFileHandle;
@property (nonatomic, strong) NSDateFormatter *dateFormatter;
@property (nonatomic, assign) int zipIndex;
@end

//本身不控制线程，但是 write 方法是在单独的线程中运行
@implementation JLogFileWriter

- (NSString *)generateZipFile:(long long)startTime endTime:(long long)endTime {
    NSArray *files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:[self logFolder] error:nil];
    NSMutableArray *resultFiles = [NSMutableArray array];
    for (NSString *fileName in files) {
        NSString *timeString = [fileName substringToIndex:jLogFileFormate.length];
        NSDate *date = [self.dateFormatter dateFromString:timeString];
        NSTimeInterval interval = [date timeIntervalSince1970];
        //拉取3:05~4:00 的日志需要把文件名中小时数为 3 的日志包含进去
        if ((interval+jOneHour)*1000-1 > startTime && interval*1000 < endTime) {
            NSString *fullPath = [[self logFolder] stringByAppendingPathComponent:fileName];
            [resultFiles addObject:fullPath];
        }
    }
    NSString *gzipFile = @"";
    if (resultFiles.count > 0) {
        long long now = [[NSDate date] timeIntervalSince1970];
        gzipFile = [NSString stringWithFormat:@"%lld-%lld-%d.gzip", startTime, now, self.zipIndex++];
        gzipFile = [self.logFolder stringByAppendingPathComponent:gzipFile];
        if ([[NSFileManager defaultManager] fileExistsAtPath:gzipFile]) {
            [[NSFileManager defaultManager] removeItemAtPath:gzipFile error:nil];
        }
        [[NSFileManager defaultManager] createFileAtPath:gzipFile contents:nil attributes:nil];
        [self createGzipArchiveFromFiles:resultFiles outputPath:gzipFile];
    } else {
        JLogI(@"J-Logger", @"no log file between %lld and %lld", startTime, endTime);
    }
    return gzipFile;
}

- (void)removeExpiredLogs {
    NSArray *files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:[self logFolder] error:nil];
    NSDate *currentDate = [NSDate date];
    for (NSString *fileName in files) {
        NSString *timeString = [fileName substringToIndex:jLogFileFormate.length];
        NSDate *date = [self.dateFormatter dateFromString:timeString];
        NSTimeInterval interval = [currentDate timeIntervalSinceDate:date];
        if (interval > jLogExpireInterval) {
            NSError *err = nil;
            NSString *fullPath = [[self logFolder] stringByAppendingPathComponent:fileName];
            [[NSFileManager defaultManager] removeItemAtPath:fullPath error:&err];
        }
    }
}

- (void)removeZipFile:(NSString *)fileName {
    [[NSFileManager defaultManager] removeItemAtPath:fileName error:nil];
}

- (void)write:(NSString *)log
         date:(nonnull NSDate *)date {
    NSFileHandle *fileHandle = [self getFileHandle:date];
    [fileHandle seekToEndOfFile];
    [fileHandle writeData:[log dataUsingEncoding:NSUTF8StringEncoding]];
}

#pragma mark - private
- (NSFileHandle *)getFileHandle:(NSDate *)date {
    NSString *timeString = [self.dateFormatter stringFromDate:date];
    if ([timeString isEqualToString:self.cachedTimeString]) {
        return self.cachedFileHandle;
    }
    
    [self.cachedFileHandle closeFile];
    self.cachedFileHandle = nil;
    
    self.cachedTimeString = timeString;
    NSString *fullPath = [self getPathWith:timeString];
    
    if (![[NSFileManager defaultManager] fileExistsAtPath:[self logFolder]]) {
        [[NSFileManager defaultManager] createDirectoryAtPath:[self logFolder] withIntermediateDirectories:YES attributes:nil error:NULL];
    }
    if (![[NSFileManager defaultManager] fileExistsAtPath:fullPath]) {
        [[NSFileManager defaultManager] createFileAtPath:fullPath contents:nil attributes:nil];
    }
    NSFileHandle *handle = [NSFileHandle fileHandleForUpdatingAtPath:fullPath];
    self.cachedFileHandle = handle;
    return handle;
}

- (BOOL)createGzipArchiveFromFiles:(NSArray<NSString*>*)filePaths outputPath:(NSString*)outputPath {
    gzFile gzFile = gzopen([outputPath cStringUsingEncoding:NSUTF8StringEncoding], "wb");
    if (!gzFile) {
        return NO;
    }
    
    for (NSString *filePath in filePaths) {
        NSData *fileData = [NSData dataWithContentsOfFile:filePath];
        if (!fileData) {
            gzclose(gzFile);
            return NO;
        }
        
        if (gzwrite(gzFile, [fileData bytes], (unsigned)fileData.length) != (int)[fileData length]) {
            gzclose(gzFile);
            return NO;
        }
    }
    
    if (gzclose(gzFile)) {
        return NO;
    }
    
    return YES;
}

- (NSString *)getPathWith:(NSString *)timeString {
    NSString *path = [NSString stringWithFormat:@"%@.jlog", timeString];
    path = [self.logFolder stringByAppendingPathComponent:path];
    return path;
}

- (NSString *)logFolder {
    if (!_logFolder) {
        _logFolder = [JUtility rootPath];
        _logFolder = [_logFolder stringByAppendingPathComponent:jLogFolder];
    }
    return _logFolder;
}

- (NSDateFormatter *)dateFormatter {
    if (!_dateFormatter) {
        _dateFormatter = [[NSDateFormatter alloc] init];
        [_dateFormatter setDateFormat:jLogFileFormate];
    }
    return _dateFormatter;
}
@end
