//
//  JBaseState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JCallSuperState.h"
#import "JCallEvent.h"

@implementation JCallSuperState

- (BOOL)stateDidEnter {
    return YES;
}

- (BOOL)stateDidLeave {
    return YES;
}

- (BOOL)event:(NSInteger)event
     userInfo:(id)userInfo {
    
    switch (event) {
        case JCallEventInvite:
//            [self.callSessionImpl error:JCallErrorCodeCallExist];
            // do nothing
            break;
            
        case JCallEventInviteFail:
            // do nothing
            break;
            
        default:
            break;
    }
    
    return YES;
}

@end
