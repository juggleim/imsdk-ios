//
//  JLogFileWriter.h
// JuggleIM
//
//  Created by Nathan on 2024/5/21.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JLogFileWriter : NSObject
- (void)write:(NSString *)log
         date:(NSDate *)date;
- (void)removeExpiredLogs;
- (void)removeZipFile:(NSString *)fileName;
- (NSString *)generateZipFile:(long long)startTime
                      endTime:(long long)endTime;
@end

NS_ASSUME_NONNULL_END
