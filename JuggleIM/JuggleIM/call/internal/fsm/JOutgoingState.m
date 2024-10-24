//
//  JOutgoingState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JOutgoingState.h"
#import "JCallEvent.h"
#import "JCallSessionImpl.h"

@implementation JOutgoingState

- (BOOL)stateDidEnter {
    self.callSessionImpl.callStatus = JCallStatusOutgoing;
    [self.callSessionImpl startOutgoingTimer];
    [self.callSessionImpl signalSingleInvite];
    return YES;
}

- (BOOL)stateDidLeave {
    return YES;
}

- (BOOL)event:(NSInteger)event
     userInfo:(id)userInfo {
    BOOL result = NO;
    
    switch (event) {
        case JCallEventInviteFail:
            [self.callSessionImpl inviteFail];
            [self.callSessionImpl transitionToIdleState];
            result = YES;
            break;
            
        default:
            break;
    }
    
    return result;
}

@end
