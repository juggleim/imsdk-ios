//
//  JUtility.m
//  JetIM
//
//  Created by Nathan on 2023/12/6.
//

#import "JUtility.h"
#import <UIKit/UIKit.h>
#import <sys/sysctl.h>
#import "JReachability.h"

@import CoreTelephony;

#define jUUID @"JUUID"
#define jJetIM @"jetim"

static const char encodingTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

@implementation JUtility

+ (NSString *)getDeviceId {
    NSString *uuid = [[NSUserDefaults standardUserDefaults] objectForKey:jUUID];
    if (uuid.length == 0) {
        CFUUIDRef uuidRef = CFUUIDCreate(kCFAllocatorDefault);
        NSString *UUID = (NSString *)CFBridgingRelease(CFUUIDCreateString(kCFAllocatorDefault, uuidRef));
        CFRelease(uuidRef);
        NSMutableString *UUID_temp = [UUID mutableCopy];
        [UUID_temp replaceOccurrencesOfString:@"-"
                                   withString:@""
                                      options:NSLiteralSearch
                                        range:NSMakeRange(0, UUID.length)];

        uuid = [UUID_temp copy];
        [[NSUserDefaults standardUserDefaults] setObject:uuid forKey:jUUID];
    }
    return uuid;
}

+ (NSString *)currentDeviceModel {
    int mib[2];
    size_t len;
    char *machine;

    mib[0] = CTL_HW;
    mib[1] = HW_MACHINE;
    sysctl(mib, 2, NULL, &len, NULL, 0);
    machine = malloc(len);
    sysctl(mib, 2, machine, &len, NULL, 0);

    NSString *platform = [NSString stringWithCString:machine encoding:NSASCIIStringEncoding];
    free(machine);

    if ([platform isEqualToString:@"i386"])
        return @"iPhone Simulator";
    if ([platform isEqualToString:@"x86_64"])
        return @"iPhone Simulator";
    return platform;
}

+ (NSString *)currentSystemVersion {
    return [[UIDevice currentDevice] systemVersion];
}

/**
 *  获取当前使用的网络类型
 *
 *  @return 当前使用的网络类型
 */
+ (NSString *)currentNetWork {
    CTTelephonyNetworkInfo *netInfo = [[CTTelephonyNetworkInfo alloc] init];
    JReachability *_reachability = [JReachability reachabilityForInternetConnection];
    JNetworkStatus _currentStatus = [_reachability currentReachabilityStatus];
    NSString *carrierName = @"";
    switch (_currentStatus) {
    case JNetworkStatusNotReachable: {
        carrierName = @"NotReachable";
        break;
    }

    case JNetworkStatusReachableViaWWAN: {
        carrierName = @"WWAN";
        if ([netInfo respondsToSelector:@selector(currentRadioAccessTechnology)])
            carrierName = netInfo.currentRadioAccessTechnology;
        break;
    }
    case JNetworkStatusReachableViaWiFi: {
        carrierName = @"WIFI";
        break;
    }
    }
    return carrierName;
}

/**
 *  获取当前运营商名称
 *
 *  @return 当前运营商名称，iPad等无SIM卡设备返回nil
 */
+ (NSString *)currentCarrier {

    CTTelephonyNetworkInfo *netInfo = [[CTTelephonyNetworkInfo alloc] init];

    CTCarrier *carrier = [netInfo subscriberCellularProvider];

    NSString *_mobileCountryCode = [carrier mobileCountryCode];
    NSString *_mobileNetworkCode = [carrier mobileNetworkCode];

    if (nil == _mobileCountryCode) {
        _mobileCountryCode = @"";
    }
    if (nil == _mobileNetworkCode) {
        _mobileNetworkCode = @"";
    }

    NSString *carrierName = [NSString stringWithFormat:@"%@%@", _mobileCountryCode, _mobileNetworkCode];
    return carrierName;
}

+ (UIImage *)generateThumbnail:(UIImage *)image targetSize:(CGSize)targetSize {
    UIImage *sourceImage = image;
    UIImage *newImage = nil;
    CGSize imageSize = sourceImage.size;
    CGFloat imageWidth = imageSize.width;
    CGFloat imageHeight = imageSize.height;
    CGFloat targetWidth = targetSize.width;
    CGFloat targetHeight = targetSize.height;
    CGFloat scaleFactor = 0.0;
    CGFloat scaledWidth = 0.0;
    CGFloat scaledHeight = 0.0;
    CGPoint thumbnailPoint = CGPointMake(0.0, 0.0);

    if (imageWidth / imageHeight < 2.4 && imageHeight / imageWidth < 2.4) {
        CGFloat widthFactor = targetWidth / imageWidth;
        CGFloat heightFactor = targetHeight / imageHeight;

        if (widthFactor < heightFactor)
            scaleFactor = widthFactor; // scale to fit height
        else
            scaleFactor = heightFactor; // scale to fit width
        scaledWidth = imageWidth * scaleFactor;
        scaledHeight = imageHeight * scaleFactor;

        // center the image
        if (widthFactor > heightFactor) {
            thumbnailPoint.y = (targetHeight - scaledHeight) * 0.5;
        } else if (widthFactor < heightFactor) {
            thumbnailPoint.x = (targetWidth - scaledWidth) * 0.5;
        }
    } else {
        if (imageWidth / imageHeight > 2.4) {
            scaleFactor = 100 * targetHeight / imageHeight / 240;
        } else {
            scaleFactor = 100 * targetWidth / imageWidth / 240;
        }
        scaledWidth = imageWidth * scaleFactor;
        scaledHeight = imageHeight * scaleFactor;
    }

    UIGraphicsBeginImageContext(CGSizeMake(scaledWidth, scaledHeight)); // this will crop

    CGRect thumbnailRect = CGRectZero;
    thumbnailRect.origin = thumbnailPoint;
    thumbnailRect.size.width = scaledWidth;
    thumbnailRect.size.height = scaledHeight;
    [sourceImage drawInRect:thumbnailRect];

    newImage = UIGraphicsGetImageFromCurrentImageContext();

    if (imageWidth / imageHeight > 2.4) {
        CGRect rect = CGRectZero;
        rect.origin.x = (newImage.size.width - 240) / 2;
        rect.size.width = 240;
        rect.origin.y = 0;
        rect.size.height = newImage.size.height;

        CGImageRef imageRef = CGImageCreateWithImageInRect([newImage CGImage], rect);
        // or use the UIImage wherever you like
        newImage = [UIImage imageWithCGImage:imageRef];
        CGImageRelease(imageRef);
    } else if (imageHeight / imageWidth > 2.4) {
        CGRect rect = CGRectZero;
        rect.origin.y = (newImage.size.height - 240) / 2;
        rect.size.height = 240;
        rect.origin.x = 0;
        rect.size.width = newImage.size.width;

        CGImageRef imageRef = CGImageCreateWithImageInRect([newImage CGImage], rect);
        // or use the UIImage wherever you like
        newImage = [UIImage imageWithCGImage:imageRef];
        CGImageRelease(imageRef);
    }

    // pop the context to get back to the default
    UIGraphicsEndImageContext();
    return newImage;
}

+ (NSString *)base64EncodedStringFrom:(NSData *)data {
    if ([data length] == 0)
        return @"";

    char *characters = malloc((([data length] + 2) / 3) * 4);
    if (characters == NULL)
        return nil;
    NSUInteger length = 0;

    NSUInteger i = 0;
    while (i < [data length]) {
        char buffer[3] = {0, 0, 0};
        short bufferLength = 0;
        while (bufferLength < 3 && i < [data length])
            buffer[bufferLength++] = ((char *)[data bytes])[i++];

        //  Encode the bytes in the buffer to four characters, including padding "="
        //  characters if necessary.
        characters[length++] = encodingTable[(buffer[0] & 0xFC) >> 2];
        characters[length++] = encodingTable[((buffer[0] & 0x03) << 4) | ((buffer[1] & 0xF0) >> 4)];
        if (bufferLength > 1)
            characters[length++] = encodingTable[((buffer[1] & 0x0F) << 2) | ((buffer[2] & 0xC0) >> 6)];
        else
            characters[length++] = '=';
        if (bufferLength > 2)
            characters[length++] = encodingTable[buffer[2] & 0x3F];
        else
            characters[length++] = '=';
    }
    NSString *base64Str =
        [[NSString alloc] initWithBytesNoCopy:characters length:length encoding:NSASCIIStringEncoding freeWhenDone:YES];
    return base64Str;
}

+ (NSData *)dataWithBase64EncodedString:(NSString *)string {
    if (string == nil)
        [NSException raise:NSInvalidArgumentException format:@""];
    if ([string length] == 0)
        return [NSData data];

    static char *decodingTable = NULL;
    if (decodingTable == NULL) {
        decodingTable = malloc(256);
        if (decodingTable == NULL)
            return nil;
        memset(decodingTable, CHAR_MAX, 256);
        NSUInteger i;
        for (i = 0; i < 64; i++)
            decodingTable[(short)encodingTable[i]] = i;
    }

    const char *characters = [string cStringUsingEncoding:NSASCIIStringEncoding];
    if (characters == NULL) //  Not an ASCII string!
    {
        return nil;
    }
    char *bytes = malloc((([string length] + 3) / 4) * 3);
    if (bytes == NULL) {
        return nil;
    }
    NSUInteger length = 0;

    NSUInteger i = 0;
    while (YES) {
        char buffer[4];
        short bufferLength;
        for (bufferLength = 0; bufferLength < 4; i++) {
            if (characters[i] == '\0')
                break;
            if (isspace(characters[i]) || characters[i] == '=')
                continue;
            buffer[bufferLength] = decodingTable[(short)characters[i]];
            if (buffer[bufferLength++] == CHAR_MAX) //  Illegal character!
            {
                free(bytes);
                return nil;
            }
        }

        if (bufferLength == 0)
            break;
        if (bufferLength == 1) //  At least two characters are needed to produce one byte!
        {
            free(bytes);
            return nil;
        }

        //  Decode the characters in the buffer to bytes.
        bytes[length++] = (buffer[0] << 2) | (buffer[1] >> 4);
        if (bufferLength > 2)
            bytes[length++] = (buffer[1] << 4) | (buffer[2] >> 2);
        if (bufferLength > 3)
            bytes[length++] = (buffer[2] << 6) | buffer[3];
    }

    bytes = realloc(bytes, length);
    return [NSData dataWithBytesNoCopy:bytes length:length];
}

+ (NSString *)rootPath {
    NSString *path =
            NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES)[0];
    path = [path stringByAppendingPathComponent:jJetIM];
    return path;
}
@end
