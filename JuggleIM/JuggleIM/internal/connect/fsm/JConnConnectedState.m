//
//  JConnConnectedState.m
//  JuggleIM
//
//  Created by Fei Li on 2024/11/12.
//

#import "JConnConnectedState.h"
#import "JConnEvent.h"
#import "JLogger.h"

@implementation JConnConnectedState

- (BOOL)stateDidEnter {
    [self.connectionManager setConnectionStatus:JConnectionStatusInternalConnected];
    [self.connectionManager enterConnected];
    return YES;
}

- (BOOL)stateDidLeave {
    [self.connectionManager leaveConnected];
    return YES;
}

- (BOOL)event:(NSInteger)event userInfo:(NSDictionary *)userInfo {
    BOOL result = NO;
    switch (event) {
        case JConnEventUserConnect:
        {
            NSString *token = userInfo[@"token"];
            if ([self.connectionManager isSameToken:token]) {
                JLogW(@"CON-Connect", @"connection already exist");
                // do nothing
            } else {
                [self.connectionManager disconnectExist:NO];
                [self.connectionManager notifyDisconnected:JErrorCodeNone extra:nil];
                [self.connectionManager transitionToIdleState];
                [self.connectionManager event:event userInfo:userInfo];
            }
            result = YES;
            break;
        }
            
        case JConnEventUserDisconnect:
        {
            NSNumber *num = userInfo[@"receivePush"];
            BOOL receivePush = num.boolValue;
            [self.connectionManager disconnectExist:receivePush];
            [self.connectionManager notifyDisconnected:JErrorCodeNone extra:nil];
            [self.connectionManager transitionToIdleState];
            result = YES;
            break;
        }
            
        case JConnEventWebsocketFail:
            [self.connectionManager notifyConnecting];
            [self.connectionManager transitionToWaitingForConnectState];
            result = YES;
            break;
            
        case JConnEventRemoteDisconnect:
        {
            NSNumber *num = userInfo[@"code"];
            NSInteger code = num.integerValue;
            NSString *extra = userInfo[@"extra"];
            [self.connectionManager handleRemoteDisconnect];
            [self.connectionManager notifyDisconnected:code extra:extra];
            [self.connectionManager transitionToIdleState];
            result = YES;
            break;
        }
            
        default:
            break;
    }
    return result;
}

@end
