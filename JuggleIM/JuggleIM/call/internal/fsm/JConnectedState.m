//
//  JConnectedState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JConnectedState.h"
#import "JLogger.h"

#define JRtcPingInterval 5

@interface JConnectedState ()
@property (nonatomic, strong) NSTimer *pingTimer;
@end

@implementation JConnectedState

- (BOOL)stateDidEnter {
    self.callSessionImpl.callStatus = JCallStatusConnected;
    [self startPing];
    return YES;
}

- (BOOL)stateDidLeave {
    [self stopPing];
    return YES;
}

- (BOOL)event:(NSInteger)event
     userInfo:(NSDictionary *)userInfo {
    BOOL result = NO;
    return result;
}

- (void)startPing {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.pingTimer) {
            return;
        }
        JLogI(@"Call-Ping", @"start");
        self.pingTimer = [NSTimer scheduledTimerWithTimeInterval:JRtcPingInterval
                                                          target:self
                                                        selector:@selector(sendPing)
                                                        userInfo:nil
                                                         repeats:YES];
    });
}

- (void)stopPing {
    dispatch_async(dispatch_get_main_queue(), ^{
        JLogI(@"Call-Ping", @"stop");
        if (self.pingTimer) {
            [self.pingTimer invalidate];
            self.pingTimer = nil;
        }
    });
}

- (void)sendPing {
    [self.callSessionImpl ping];
}

@end
