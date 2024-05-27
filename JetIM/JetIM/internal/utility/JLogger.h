//
//  JLogger.h
//  JetIM
//
//  Created by Nathan on 2024/5/21.
//

#import <Foundation/Foundation.h>
#import "JetIMConst.h"

NS_ASSUME_NONNULL_BEGIN

#define JLogF(t, k, ...) [[JLogger shared] write:JLogLevelFatal tag:t keys:k, ##__VA_ARGS__]
#define JLogE(t, k, ...) [[JLogger shared] write:JLogLevelError tag:t keys:k, ##__VA_ARGS__]
#define JLogW(t, k, ...) [[JLogger shared] write:JLogLevelWarning tag:t keys:k, ##__VA_ARGS__]
#define JLogI(t, k, ...) [[JLogger shared] write:JLogLevelInfo tag:t keys:k, ##__VA_ARGS__]
#define JLogD(t, k, ...) [[JLogger shared] write:JLogLevelDebug tag:t keys:k, ##__VA_ARGS__]
#define JLogV(t, k, ...) [[JLogger shared] write:JLogLevelVerbose tag:t keys:k, ##__VA_ARGS__]

typedef NS_ENUM(NSUInteger, JLogLevel) {
    JLogLevelNone = 0,
    JLogLevelFatal = 1,
    JLogLevelError = 2,
    JLogLevelWarning = 3,
    JLogLevelInfo = 4,
    JLogLevelDebug = 5,
    JLogLevelVerbose = 6
};


@interface JLogger : NSObject

@property (nonatomic, assign) JLogLevel logLevel;

+ (instancetype)shared;

- (void)uploadLog:(long long)startTime
          endTime:(long long)endTime
       completion:(void (^)(JErrorCode code))completeBlock;

- (void)removeExpiredLogs;

- (void)write:(JLogLevel)level
          tag:(NSString *)tag
         keys:(NSString *)keys, ... NS_FORMAT_FUNCTION(3, 4);

@end

NS_ASSUME_NONNULL_END
