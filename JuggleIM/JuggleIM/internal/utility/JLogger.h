//
//  JLogger.h
// JuggleIM
//
//  Created by Nathan on 2024/5/21.
//

#import <Foundation/Foundation.h>
#import "JuggleIMConst.h"

NS_ASSUME_NONNULL_BEGIN

#define JLogF(t, k, ...) [[JLogger shared] write:JLogLevelFatal tag:t keys:k, ##__VA_ARGS__]
#define JLogE(t, k, ...) [[JLogger shared] write:JLogLevelError tag:t keys:k, ##__VA_ARGS__]
#define JLogW(t, k, ...) [[JLogger shared] write:JLogLevelWarning tag:t keys:k, ##__VA_ARGS__]
#define JLogI(t, k, ...) [[JLogger shared] write:JLogLevelInfo tag:t keys:k, ##__VA_ARGS__]
#define JLogD(t, k, ...) [[JLogger shared] write:JLogLevelDebug tag:t keys:k, ##__VA_ARGS__]
#define JLogV(t, k, ...) [[JLogger shared] write:JLogLevelVerbose tag:t keys:k, ##__VA_ARGS__]

@interface JLogger : NSObject

@property (nonatomic, assign) JLogLevel fileLogLevel;
@property (nonatomic, assign) JLogLevel consoleLogLevel;

+ (instancetype)shared;

- (void)uploadLog:(long long)startTime
          endTime:(long long)endTime
           appKey:(NSString *)appKey
            token:(NSString *)token;

- (void)removeExpiredLogs;

- (void)write:(JLogLevel)level
          tag:(NSString *)tag
         keys:(NSString *)keys, ... NS_FORMAT_FUNCTION(3, 4);

@end

NS_ASSUME_NONNULL_END
