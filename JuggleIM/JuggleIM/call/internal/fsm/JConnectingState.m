//
//  JConnectingState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JConnectingState.h"
#import "JCallEvent.h"

@implementation JConnectingState

- (BOOL)stateDidEnter {
    self.callSessionImpl.callStatus = JCallStatusConnecting;
    [self.callSessionImpl mediaJoin];
    return YES;
}

- (BOOL)stateDidLeave {
    return YES;
}

- (BOOL)event:(NSInteger)event
     userInfo:(id)userInfo {
    BOOL result = NO;
    
    switch (event) {
        case JCallEventJoinChannelDone:
            //TODO: 更新 callSession 状态
            //TODO:
            //[self.callSessionImpl signalChangeState];
            [self.callSessionImpl transitionToConnectedState];
            result = YES;
            break;
            
        case JCallEventJoinChannelFail:
            //TODO: 更新 callSession 状态
            //TODO:
            //[self.callSessionImpl signalChangeState];
            self.callSessionImpl.finishReason = JCallFinishReasonNetworkError;
            [self.callSessionImpl transitionToIdleState];
            result = YES;
            break;
            
        default:
            break;
    }
    
    return result;
}

@end
