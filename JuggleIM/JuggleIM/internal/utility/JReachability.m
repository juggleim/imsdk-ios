
#import <arpa/inet.h>

#import "JReachability.h"

#pragma mark IPv6 Support
// Reachability fully support IPv6.  For full details, see ReadMe.md.

NSString *kJReachabilityChangedNotification = @"kJNetworkReachabilityChangedNotification";

#pragma mark - Supporting functions

#define kShouldPrintReachabilityFlags 1

static JNetworkStatus gs_JStatus = JNetworkStatusNotReachable;

static void JPrintReachabilityFlags(SCNetworkReachabilityFlags flags, const char *comment) {
#if kShouldPrintReachabilityFlags
//
//    LogW(@"Reachability Flag Status: %c%c %c%c%c%c%c%c%c %s\n",
//           (flags & kSCNetworkReachabilityFlagsIsWWAN) ? 'W' : '-',
//           (flags & kSCNetworkReachabilityFlagsReachable) ? 'R' : '-',
//
//           (flags & kSCNetworkReachabilityFlagsTransientConnection) ? 't' : '-',
//           (flags & kSCNetworkReachabilityFlagsConnectionRequired) ? 'c' : '-',
//           (flags & kSCNetworkReachabilityFlagsConnectionOnTraffic) ? 'C' : '-',
//           (flags & kSCNetworkReachabilityFlagsInterventionRequired) ? 'i' : '-',
//           (flags & kSCNetworkReachabilityFlagsConnectionOnDemand) ? 'D' : '-',
//           (flags & kSCNetworkReachabilityFlagsIsLocalAddress) ? 'l' : '-',
//           (flags & kSCNetworkReachabilityFlagsIsDirect) ? 'd' : '-', comment);
#endif
}

static void JReachabilityCallback(SCNetworkReachabilityRef target, SCNetworkReachabilityFlags flags, void *info) {
#pragma unused(target, flags)
    NSCAssert(info != NULL, @"info was NULL in ReachabilityCallback");
    NSCAssert([(__bridge NSObject *)info isKindOfClass:[JReachability class]],
              @"info was wrong class in ReachabilityCallback");

    JReachability *noteObject = (__bridge JReachability *)info;
    gs_JStatus = [noteObject currentReachabilityStatus];
    // Post a notification to notify the client that the network reachability
    // changed.
    [[NSNotificationCenter defaultCenter] postNotificationName:kJReachabilityChangedNotification object:noteObject];
}

#pragma mark - Reachability implementation

@implementation JReachability {
    SCNetworkReachabilityRef _reachabilityRef;
}

+ (instancetype)reachabilityWithHostName:(NSString *)hostName {
    JReachability *returnValue = NULL;
    SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithName(NULL, [hostName UTF8String]);
    if (reachability != NULL) {
        returnValue = [[self alloc] init];
        if (returnValue != NULL) {
            returnValue->_reachabilityRef = reachability;
        } else {
            CFRelease(reachability);
        }
    }
    return returnValue;
}

+ (instancetype)reachabilityWithAddress:(const struct sockaddr *)hostAddress {
    SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithAddress(kCFAllocatorDefault, hostAddress);

    JReachability *returnValue = NULL;

    if (reachability != NULL) {
        returnValue = [[self alloc] init];
        if (returnValue != NULL) {
            returnValue->_reachabilityRef = reachability;
        } else {
            CFRelease(reachability);
        }
    }
    return returnValue;
}

+ (instancetype)reachabilityForInternetConnection {
    struct sockaddr_in zeroAddress;
    bzero(&zeroAddress, sizeof(zeroAddress));
    zeroAddress.sin_len = sizeof(zeroAddress);
    zeroAddress.sin_family = AF_INET;

    return [self reachabilityWithAddress:(const struct sockaddr *)&zeroAddress];
}

+ (JNetworkStatus)getCacheReachabilityStatus:(BOOL)flash {
    if (!flash) {
        return gs_JStatus;
    }
    gs_JStatus = [[self reachabilityForInternetConnection] currentReachabilityStatus];
    return gs_JStatus;
}

#pragma mark reachabilityForLocalWiFi
// reachabilityForLocalWiFi has been removed from the sample.  See ReadMe.md for
// more information.
//+ (instancetype)reachabilityForLocalWiFi

#pragma mark - Start and stop notifier

- (BOOL)startNotifier {
    BOOL returnValue = NO;
    SCNetworkReachabilityContext context = {0, (__bridge void *)(self), NULL, NULL, NULL};

    if (SCNetworkReachabilitySetCallback(_reachabilityRef, JReachabilityCallback, &context)) {
        if (SCNetworkReachabilityScheduleWithRunLoop(_reachabilityRef, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode)) {
            returnValue = YES;
        }
    }

    return returnValue;
}

- (void)stopNotifier {
    if (_reachabilityRef != NULL) {
        SCNetworkReachabilityUnscheduleFromRunLoop(_reachabilityRef, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    }
}

- (void)dealloc {
    [self stopNotifier];
    if (_reachabilityRef != NULL) {
        CFRelease(_reachabilityRef);
    }
}

#pragma mark - Network Flag Handling

- (JNetworkStatus)networkStatusForFlags:(SCNetworkReachabilityFlags)flags {
    JPrintReachabilityFlags(flags, "networkStatusForFlags");
    if ((flags & kSCNetworkReachabilityFlagsReachable) == 0) {
        // The target host is not reachable.
        return JNetworkStatusNotReachable;
    }

    JNetworkStatus returnValue = JNetworkStatusNotReachable;

    if ((flags & kSCNetworkReachabilityFlagsConnectionRequired) == 0) {
        /*
         If the target host is reachable and no connection is required then we'll
         assume (for now) that you're on Wi-Fi...
         */
        returnValue = JNetworkStatusReachableViaWiFi;
    }

    if ((((flags & kSCNetworkReachabilityFlagsConnectionOnDemand) != 0) ||
         (flags & kSCNetworkReachabilityFlagsConnectionOnTraffic) != 0)) {
        /*
         ... and the connection is on-demand (or on-traffic) if the calling
         application is using the CFSocketStream or higher APIs...
         */

        if ((flags & kSCNetworkReachabilityFlagsInterventionRequired) == 0) {
            /*
             ... and no [user] intervention is needed...
             */
            returnValue = JNetworkStatusReachableViaWiFi;
        }
    }

    if ((flags & kSCNetworkReachabilityFlagsIsWWAN) == kSCNetworkReachabilityFlagsIsWWAN) {
        /*
         ... but WWAN connections are OK if the calling application is using the
         CFNetwork APIs.
         */
        returnValue = JNetworkStatusReachableViaWWAN;
    }

    return returnValue;
}

- (BOOL)connectionRequired {
    NSAssert(_reachabilityRef != NULL, @"connectionRequired called with NULL reachabilityRef");
    SCNetworkReachabilityFlags flags;

    if (SCNetworkReachabilityGetFlags(_reachabilityRef, &flags)) {
        return (flags & kSCNetworkReachabilityFlagsConnectionRequired);
    }

    return NO;
}

- (JNetworkStatus)currentReachabilityStatus {
    NSAssert(_reachabilityRef != NULL, @"currentNetworkStatus called with NULL SCNetworkReachabilityRef");
    JNetworkStatus returnValue = JNetworkStatusNotReachable;
    SCNetworkReachabilityFlags flags;

    if (SCNetworkReachabilityGetFlags(_reachabilityRef, &flags)) {
        returnValue = [self networkStatusForFlags:flags];
    }

    return returnValue;
}

@end
