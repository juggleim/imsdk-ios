//
//  JIdleState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/9.
//

#import "JIdleState.h"
#import "JCallEvent.h"

@implementation JIdleState

- (BOOL)stateDidEnter {
    self.callSessionImpl.callStatus = JCallStatusIdle;
    return YES;
}

- (BOOL)stateDidLeave {
    return YES;
}

- (BOOL)event:(NSInteger)event
     userInfo:(id)userInfo {
    BOOL result = NO;
    switch (event) {
        case JCallEventInvite:
            [self.callSessionImpl transitionToOutgoingState];
            result = YES;
            break;
            
        default:
            break;
    }
    return result;
}

@end
