//
//  JUtility.h
//  Juggle
//
//  Created by Nathan on 2023/12/6.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JUtility : NSObject

+ (NSString *)getDeviceId;
+ (NSString *)currentDeviceModel;
+ (NSString *)currentSystemVersion;
+ (NSString *)currentNetWork;
+ (NSString *)currentCarrier;
@end

NS_ASSUME_NONNULL_END
