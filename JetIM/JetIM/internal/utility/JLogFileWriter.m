//
//  JLogFileWriter.m
//  JetIM
//
//  Created by Nathan on 2024/5/21.
//

#import "JLogFileWriter.h"
#import "JUtility.h"

#define jLogFolder @"jlog"
#define jLogFileFormate @"yyyyMMddHH"
#define jLogExpireInterval 7 * 24 * 60 *60

@interface JLogFileWriter ()
@property (nonatomic, copy) NSString *logFolder;
@property (nonatomic, copy) NSString *cachedTimeString;
@property (nonatomic, strong) NSFileHandle *cachedFileHandle;
@property (nonatomic, strong) NSDateFormatter *dateFormatter;
@end

//本身不控制线程，但是 write 方法是在单独的线程中运行
@implementation JLogFileWriter

- (void)removeExpiredLogs {
    NSArray *files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:[self logFolder] error:nil];
    NSDate *currentDate = [NSDate date];
    for (NSString *fileName in files) {
        NSString *timeString = [fileName substringToIndex:jLogFileFormate.length];
        NSDate *date = [self.dateFormatter dateFromString:timeString];
        NSTimeInterval interval = [currentDate timeIntervalSinceDate:date];
        if (interval > jLogExpireInterval) {
            [[NSFileManager defaultManager] removeItemAtPath:fileName error:nil];
        }
    }
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
