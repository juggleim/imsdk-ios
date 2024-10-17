//
//  JLogger.m
// JuggleIM
//
//  Created by Nathan on 2024/5/21.
//

#import "JLogger.h"
#import "JLogFileWriter.h"
#import "JUploadManager.h"

#define jLogQueue "com.JIM.im.logqueue"

@interface JLogger ()
@property (nonatomic, strong) dispatch_queue_t logQueue;
@property (nonatomic, strong) JLogFileWriter *fileWriter;
@property (nonatomic, strong) NSDateFormatter *dateFormatter;
@end

@implementation JLogger

static JLogger *_instance;

+ (instancetype)shared {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [[self alloc] init];
        _instance.fileLogLevel = JLogLevelInfo;
        _instance.consoleLogLevel = JLogLevelNone;
        _instance.logQueue = dispatch_queue_create(jLogQueue, NULL);
        NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
        [formatter setDateFormat:@"yyyy-MM-dd HH:mm:ss.SSS"];
        _instance.dateFormatter = formatter;
        _instance.fileWriter = [[JLogFileWriter alloc] init];
    });
    return _instance;
}

- (void)uploadLog:(NSString *)messageId
        startTime:(long long)startTime
          endTime:(long long)endTime {
    dispatch_async(self.logQueue, ^{
        NSString *fileName = [self.fileWriter generateZipFile:startTime endTime:endTime];
        JUploadManager *uploader = [[JUploadManager alloc] initWithCore:self.core];
        [uploader uploadLog:fileName messageId:messageId complete:^{
            dispatch_async(self.logQueue, ^{
                if (fileName.length > 0 && [[NSFileManager defaultManager] fileExistsAtPath:fileName]) {
                    [self.fileWriter removeZipFile:fileName];
                }
            });
        }];
    });
}

- (void)removeExpiredLogs {
    dispatch_async(self.logQueue, ^{
        [self.fileWriter removeExpiredLogs];
    });
}

- (void)write:(JLogLevel)level tag:(NSString *)tag keys:(NSString *)keys, ... NS_FORMAT_FUNCTION(3, 4) {
    va_list args;
    va_start(args, keys);
    __block NSString *logStr = (keys != nil ? [[NSString alloc] initWithFormat:keys arguments:args] : @"");
    va_end(args);

    //控制台
    if (level <= self.consoleLogLevel) {
        NSLog(@"[JLogger:%@]%@", tag, logStr);
    }

    //写文件
    if (level <= self.fileLogLevel) {
        dispatch_async(self.logQueue, ^{
            NSDate *date = [NSDate date];
            NSString *time = [self.dateFormatter stringFromDate:date];
            logStr = [NSString stringWithFormat:@"%@ [%@] %@\n", time, tag, logStr];
            [self.fileWriter write:logStr date:date];
        });
    }
}
@end
