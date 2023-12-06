//
//  JUtility.m
//  Juggle
//
//  Created by Nathan on 2023/12/6.
//

#import "JUtility.h"
#import <UIKit/UIKit.h>
#import <sys/sysctl.h>
#import "JReachability.h"

@import CoreTelephony;

#define kUUID @"JUUID"

@implementation JUtility

+ (NSString *)getDeviceId {
    NSString *uuid = [[NSUserDefaults standardUserDefaults] objectForKey:kUUID];
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
        [[NSUserDefaults standardUserDefaults] setObject:uuid forKey:kUUID];
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
@end
