
#ifndef __JReachability
#define __JReachability

#import <Foundation/Foundation.h>
#import <SystemConfiguration/SystemConfiguration.h>
#import <netinet/in.h>
#import "JuggleConst.h"

#pragma mark IPv6 Support
// Reachability fully support IPv6.  For full details, see ReadMe.md.

extern NSString *kJReachabilityChangedNotification;

@interface JReachability : NSObject

/*!
 * Use to check the reachability of a given host name.
 */
+ (instancetype)reachabilityWithHostName:(NSString *)hostName;

/*!
 * Use to check the reachability of a given IP address.
 */
+ (instancetype)reachabilityWithAddress:(const struct sockaddr *)hostAddress;

/*!
 * Checks whether the default route is available. Should be used by applications
 * that do not connect to a particular host.
 */
+ (instancetype)reachabilityForInternetConnection;

+ (JNetworkStatus)getCacheReachabilityStatus:(BOOL)flash;

#pragma mark reachabilityForLocalWiFi
// reachabilityForLocalWiFi has been removed from the sample.  See ReadMe.md for
// more information.
//+ (instancetype)reachabilityForLocalWiFi;

/*!
 * Start listening for reachability notifications on the current run loop.
 */
- (BOOL)startNotifier;
- (void)stopNotifier;

- (JNetworkStatus)currentReachabilityStatus;

/*!
 * WWAN may be available, but not active until a connection has been
 * established. WiFi may require a connection for VPN on Demand.
 */
- (BOOL)connectionRequired;

@end
#endif
