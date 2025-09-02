//
//  JCallJoinState.m
//  JuggleIM
//
//  Created by Fei Li on 2025/8/28.
//

#import "JCallJoinState.h"
#import "JLogger.h"
#import "JCallSessionImpl.h"
#import "JCallEvent.h"

@interface JCallJoinState ()
@end

@implementation JCallJoinState

- (BOOL)stateDidEnter {
    self.callSessionImpl.callStatus = JCallStatusJoin;
    [self.callSessionImpl signalJoin];
    return YES;
}

- (BOOL)stateDidLeave {
    return YES;
}

- (BOOL)event:(NSInteger)event
     userInfo:(NSDictionary *)userInfo {
    BOOL result = NO;
    
    switch (event) {
        case JCallEventJoinDone:
            [self.callSessionImpl transitionToConnectingState];
            result = YES;
            break;
            
        case JCallEventJoinFail:
            [self.callSessionImpl error:JCallErrorCodeJoinRoomFail];
            [self.callSessionImpl transitionToIdleState];
            result = YES;
            break;
            
        default:
            break;
    }
    
    return result;
}

@end
