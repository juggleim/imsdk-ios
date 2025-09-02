//
//  JIdleState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/9.
//

#import "JCallIdleState.h"
#import "JCallEvent.h"

@implementation JCallIdleState

- (BOOL)stateDidEnter {
    self.callSessionImpl.callStatus = JCallStatusIdle;
    return YES;
}

- (BOOL)stateDidLeave {
    return YES;
}

- (BOOL)event:(NSInteger)event
     userInfo:(NSDictionary *)userInfo {
    BOOL result = NO;
    switch (event) {
        case JCallEventInvite:
            [self.callSessionImpl transitionToOutgoingState];
            result = YES;
            break;
            
        case JCallEventReceiveInvite:
            [self.callSessionImpl transitionToIncomingState];
            result = YES;
            break;
            
        case JCallEventReceiveInviteOthers:
            // do nothing
            result = YES;
            break;
            
        case JCallEventJoin:
            [self.callSessionImpl transitionToJoinState];
            result = YES;
            break;
            
        default:
            break;
    }
    return result;
}

@end
