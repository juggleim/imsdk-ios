//
//  JConnectingState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JConnectingState.h"

@implementation JConnectingState

- (BOOL)stateDidEnter {
    self.callSessionImpl.callStatus = JCallStatusConnecting;
    return YES;
}

- (BOOL)stateDidLeave {
    return YES;
}

- (BOOL)event:(NSInteger)event
     userInfo:(id)userInfo {
    BOOL result = NO;
    return result;
}

@end
