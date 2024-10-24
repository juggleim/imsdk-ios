//
//  JConnectedState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JConnectedState.h"

@implementation JConnectedState

- (BOOL)stateDidEnter {
    self.callSessionImpl.callStatus = JCallStatusConnected;
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
