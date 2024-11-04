//
//  JIncomingState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JIncomingState.h"
#import "JCallEvent.h"

@implementation JIncomingState

- (BOOL)stateDidEnter {
    self.callSessionImpl.callStatus = JCallStatusIncoming;
    [self.callSessionImpl notifyReceiveCall];
    return YES;
}

- (BOOL)stateDidLeave {
    return YES;
}

- (BOOL)event:(NSInteger)event
     userInfo:(NSDictionary *)userInfo {
    BOOL result = NO;
    switch (event) {
        case JCallEventAccept:
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

@end
