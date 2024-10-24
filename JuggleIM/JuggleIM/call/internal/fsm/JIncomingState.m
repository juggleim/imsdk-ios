//
//  JIncomingState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JIncomingState.h"

@implementation JIncomingState

- (BOOL)stateDidEnter {
    self.callSessionImpl.callStatus = JCallStatusIncoming;
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
