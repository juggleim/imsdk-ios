//
//  JConnectingState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JCallConnectingState.h"
#import "JCallEvent.h"

@implementation JCallConnectingState

- (BOOL)stateDidEnter {
    self.callSessionImpl.callStatus = JCallStatusConnecting;
    [self.callSessionImpl mediaJoin];
    return YES;
}

- (BOOL)stateDidLeave {
    return YES;
}

- (BOOL)event:(NSInteger)event
     userInfo:(NSDictionary *)userInfo {
    BOOL result = NO;
    
    switch (event) {
        case JCallEventJoinChannelDone:
            self.callSessionImpl.connectTime = [[NSDate date] timeIntervalSince1970] * 1000;
            [self.callSessionImpl transitionToConnectedState];
            result = YES;
            break;
            
        case JCallEventJoinChannelFail:
            self.callSessionImpl.finishTime = [[NSDate date] timeIntervalSince1970] * 1000;
            self.callSessionImpl.finishReason = JCallFinishReasonNetworkError;
            [self.callSessionImpl error:JCallErrorCodeJoinMediaRoomFail];
            [self.callSessionImpl transitionToIdleState];
            result = YES;
            break;
            
        default:
            break;
    }
    
    return result;
}

@end
