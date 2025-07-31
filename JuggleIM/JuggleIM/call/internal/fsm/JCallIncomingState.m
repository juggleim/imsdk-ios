//
//  JIncomingState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JCallIncomingState.h"
#import "JCallEvent.h"
#import "JLogger.h"

#define JRtcHangupInterval 0.5
#define JIncomingInterval 60

@interface JCallIncomingState ()
@property (nonatomic, strong) NSTimer *incomingTimer;
@end

@implementation JCallIncomingState

- (BOOL)stateDidEnter {
    self.callSessionImpl.callStatus = JCallStatusIncoming;
    [self startIncomingTimer];
    [self.callSessionImpl notifyReceiveCall];
    return YES;
}

- (BOOL)stateDidLeave {
    [self stopIncomingTimer];
    return YES;
}

- (BOOL)event:(NSInteger)event
     userInfo:(NSDictionary *)userInfo {
    BOOL result = NO;
    switch (event) {
        case JCallEventAccept:
        {
            BOOL needHangupOther = [self.callSessionImpl notifyAcceptCall];
            if (needHangupOther) {
                [self startHangupTimer];
            } else {
                [self.callSessionImpl signalAccept];
            }
            result = YES;
        }
            break;
            
        case JCallEventReceiveAccept:
        {
            NSString *userId = userInfo[@"userId"];
            // 当前用户在其它端 accept，else 走 super
            if ([self.callSessionImpl.core.userId isEqualToString:userId]) {
                self.callSessionImpl.finishReason = JCallFinishReasonAcceptOnOtherClient;
                [self.callSessionImpl transitionToIdleState];
                result = YES;
            }
            break;
        }
            
        case JCallEventReceiveHangup:
        {
            NSString *userId = userInfo[@"userId"];
            // 当前用户在其它端 hangup，else 走 super
            if ([self.callSessionImpl.core.userId isEqualToString:userId]) {
                self.callSessionImpl.finishReason = JCallFinishReasonHangupOnOtherClient;
                [self.callSessionImpl transitionToIdleState];
                result = YES;
            }
            break;
        }
            
        case JCallEventIncomingTimeOut:
            [self incomingTimeOut];
            [self.callSessionImpl transitionToIdleState];
            result = YES;
            break;
            
        case JCallEventAcceptAfterHangupOther:
            [self.callSessionImpl signalAccept];
            result = YES;
            break;
            
        case JCallEventAcceptDone:
            [self.callSessionImpl transitionToConnectingState];
            result = YES;
            break;
            
        case JCallEventAcceptFail:
            [self.callSessionImpl error:JCallErrorCodeAcceptFail];
            [self.callSessionImpl transitionToIdleState];
            result = YES;
            break;
            
        default:
            break;
    }
    return result;
}

- (void)startHangupTimer {
    dispatch_async(dispatch_get_main_queue(), ^{
       [NSTimer scheduledTimerWithTimeInterval:JRtcHangupInterval
                                        target:self
                                      selector:@selector(hangupTimerFire)
                                      userInfo:nil
                                       repeats:NO];
    });
}

- (void)hangupTimerFire {
    JLogI(@"Call-Accept", @"hangupTimerFire");
    [self.callSessionImpl event:JCallEventAcceptAfterHangupOther
                       userInfo:nil];
}

- (void)startIncomingTimer {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.incomingTimer) {
            return;
        }
        JLogI(@"Call-Timer", @"incoming timer start");
        self.incomingTimer = [NSTimer scheduledTimerWithTimeInterval:JIncomingInterval target:self selector:@selector(incomingTimerFire) userInfo:nil repeats:NO];
    });
}

- (void)stopIncomingTimer {
    dispatch_async(dispatch_get_main_queue(), ^{
        JLogI(@"Call-Timer", @"incoming timer stop");
        if (self.incomingTimer) {
            [self.incomingTimer invalidate];
            self.incomingTimer = nil;
        }
    });
}

- (void)incomingTimerFire {
    [self.callSessionImpl event:JCallEventIncomingTimeOut userInfo:nil];
}

- (void)incomingTimeOut {
    self.callSessionImpl.finishTime = [[NSDate date] timeIntervalSince1970] * 1000;
    self.callSessionImpl.finishReason = JCallFinishReasonNoResponse;
}

@end
