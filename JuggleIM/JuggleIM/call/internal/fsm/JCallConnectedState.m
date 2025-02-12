//
//  JConnectedState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JCallConnectedState.h"
#import "JLogger.h"
#import "JCallEvent.h"

#define JRtcPingInterval 5

@interface JCallConnectedState ()
@property (nonatomic, strong) NSTimer *pingTimer;
@end

@implementation JCallConnectedState

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
    switch (event) {
        case JCallEventInvite:
        {
            NSArray <NSString *> *userIdList = userInfo[@"userIdList"];
            [self.callSessionImpl signalInvite:userIdList];
            result = YES;
            break;
        }
            
        case JCallEventInviteDone:
        {
            NSArray <NSString *> *userIdList = userInfo[@"userIdList"];
            [self.callSessionImpl membersInviteBySelf:userIdList];
            result = YES;
            break;
        }
            
        case JCallEventInviteFail:
        {
            [self.callSessionImpl error:JCallErrorCodeInviteFail];
            result = YES;
            break;
        }
            
        default:
            break;
    }
    
    
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
