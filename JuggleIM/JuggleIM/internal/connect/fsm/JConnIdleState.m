//
//  JConnIdleState.m
//  JuggleIM
//
//  Created by Fei Li on 2024/11/12.
//

#import "JConnIdleState.h"
#import "JConnEvent.h"

@implementation JConnIdleState

- (BOOL)stateDidEnter {
    [self.connectionManager setConnectionStatus:JConnectionStatusInternalIdle];
    return YES;
}

- (BOOL)stateDidLeave {
    return YES;
}

- (BOOL)event:(NSInteger)event userInfo:(NSDictionary *)userInfo {
    BOOL result = NO;
    switch (event) {
        case JConnEventUserConnect:
        {
            NSString *token = userInfo[@"token"];
            [self.connectionManager updateToken:token];
            [self.connectionManager notifyConnecting];
            [self.connectionManager transitionToConnectingState];
            result = YES;
            break;
        }
            
        case JConnEventUserDisconnect:
            [self.connectionManager disconnectWithoutWS];
            result = YES;
            break;
            
        default:
            break;
    }
    return result;
}

@end
