//
//  JLogFileWriter.h
//  JetIM
//
//  Created by Nathan on 2024/5/21.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JLogFileWriter : NSObject
- (void)write:(NSString *)log
         date:(NSDate *)date;
@end

NS_ASSUME_NONNULL_END
