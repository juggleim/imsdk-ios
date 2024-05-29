//
//  JLogger.m
//  JetIM
//
//  Created by Nathan on 2024/5/21.
//

#import "JLogger.h"
#import "JLogFileWriter.h"

#define jLogQueue "com.JetIM.im.logqueue"
#define jUploadQueue "com.JetIM.im.logqueue"

@interface JLogger ()
@property (nonatomic, strong) dispatch_queue_t logQueue;
@property (nonatomic, strong) dispatch_queue_t uploadQueue;
@property (nonatomic, strong) JLogFileWriter *fileWriter;
@property (nonatomic, strong) NSDateFormatter *dateFormatter;
@end

@implementation JLogger

static JLogger *_instance;

+ (instancetype)shared {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [[self alloc] init];
        _instance.logLevel = JLogLevelVerbose;
        _instance.logQueue = dispatch_queue_create(jLogQueue, NULL);
        _instance.uploadQueue = dispatch_queue_create(jUploadQueue, NULL);
        NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
        [formatter setDateFormat:@"yyyy-MM-dd HH:mm:ss.SSS"];
        _instance.dateFormatter = formatter;
        _instance.fileWriter = [[JLogFileWriter alloc] init];
    });
    return _instance;
}

- (void)uploadLog:(long long)startTime
          endTime:(long long)endTime
       completion:(void (^)(JErrorCodeInternal))completeBlock {
    NSString *fileName = [self.fileWriter generateZipFile:startTime endTime:endTime];
    if (fileName.length == 0) {
        completeBlock(JErrorCodeInternalLogNotExist);
    }
    //TODO: uploader
}

- (void)removeExpiredLogs {
    dispatch_async(self.logQueue, ^{
        [self.fileWriter removeExpiredLogs];
    });
}

- (void)write:(JLogLevel)level tag:(NSString *)tag keys:(NSString *)keys, ... NS_FORMAT_FUNCTION(3, 4) {
    if (level > self.logLevel) {
        return;
    }
    
    va_list args;
    va_start(args, keys);
    __block NSString *logStr = (keys != nil ? [[NSString alloc] initWithFormat:keys arguments:args] : @"");
    va_end(args);
#ifdef DEBUG
    //控制台
    NSLog(@"[Juggle:%@]%@", tag, logStr);
#endif
    
    //写文件
    dispatch_async(self.logQueue, ^{
        NSDate *date = [NSDate date];
        NSString *time = [self.dateFormatter stringFromDate:date];
        logStr = [NSString stringWithFormat:@"%@ [%@] %@\n", time, tag, logStr];
        [self.fileWriter write:logStr date:date];
    });
}
@end
