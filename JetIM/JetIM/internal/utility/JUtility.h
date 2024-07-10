//
//  JUtility.h
//  JetIM
//
//  Created by Nathan on 2023/12/6.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface JUtility : NSObject

+ (NSString *)getDeviceId;
+ (NSString *)currentDeviceModel;
+ (NSString *)currentSystemVersion;
+ (NSString *)currentNetWork;
+ (NSString *)currentCarrier;
+ (UIImage *)generateThumbnail:(UIImage *)image targetSize:(CGSize)targetSize;
+ (NSString *)base64EncodedStringFrom:(NSData *)data;
+ (NSData *)dataWithBase64EncodedString:(NSString *)string;
+ (NSString *)rootPath;

+ (NSString *)absolutePathRelativePath;
+ (NSString *)absolutePathRelativePath;

@end

NS_ASSUME_NONNULL_END
