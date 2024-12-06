//
//  JOutgoingState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JCallOutgoingState.h"
#import "JCallEvent.h"
#import "JCallSessionImpl.h"
#import "JLogger.h"

#define JOutgoingInterval 60

@interface JCallOutgoingState ()
@property (nonatomic, strong) NSTimer *outgoingTimer;
@end

@implementation JCallOutgoingState

- (BOOL)stateDidEnter {
    self.callSessionImpl.callStatus = JCallStatusOutgoing;
    [self startOutgoingTimer];
    [self.callSessionImpl signalInvite];
    return YES;
}

- (BOOL)stateDidLeave {
    [self stopOutgoingTimer];
    return YES;
}

- (BOOL)event:(NSInteger)event
     userInfo:(NSDictionary *)userInfo {
    BOOL result = NO;
    
    switch (event) {
        case JCallEventInviteDone:
            if (self.callSessionImpl.isMultiCall) {
                [self.callSessionImpl transitionToConnectingState];
            }
            result = YES;
            break;
            
        case JCallEventInviteFail:
            [self inviteFail];
            [self.callSessionImpl transitionToIdleState];
            result = YES;
            break;
            
        case JCallEventInviteTimeOut:
            [self inviteTimeOut];
            [self.callSessionImpl transitionToIdleState];
            result = YES;
            break;
            
        case JCallEventReceiveAccept:
        {
            NSString *userId = userInfo[@"userId"];
            [self.callSessionImpl memberAccept:userId];
            if (!self.callSessionImpl.isMultiCall) {
                [self.callSessionImpl transitionToConnectingState];
            }
            result = YES;
            break;
        }
            
        default:
            break;
    }
    
    return result;
}

- (void)startOutgoingTimer {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.outgoingTimer) {
            return;
        }
        JLogI(@"Call-Timer", @"outgoing timer start");
        self.outgoingTimer = [NSTimer scheduledTimerWithTimeInterval:JOutgoingInterval target:self selector:@selector(outgoingTimerFire) userInfo:nil repeats:NO];
    });
}

- (void)stopOutgoingTimer {
    dispatch_async(dispatch_get_main_queue(), ^{
        JLogI(@"Call-Timer", @"outgoing timer stop");
        if (self.outgoingTimer) {
            [self.outgoingTimer invalidate];
            self.outgoingTimer = nil;
        }
    });
}

- (void)outgoingTimerFire {
    [self.callSessionImpl event:JCallEventInviteTimeOut userInfo:nil];
}

- (void)inviteFail {
    self.callSessionImpl.finishTime = [[NSDate date] timeIntervalSince1970];
    self.callSessionImpl.finishReason = JCallFinishReasonNetworkError;
}

- (void)inviteTimeOut {
    self.callSessionImpl.finishTime = [[NSDate date] timeIntervalSince1970];
    self.callSessionImpl.finishReason = JCallFinishReasonOtherSideNoResponse;
}

@end
