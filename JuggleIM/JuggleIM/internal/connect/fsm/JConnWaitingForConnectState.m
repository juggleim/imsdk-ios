//
//  JConnWaitingForConnectState.m
//  JuggleIM
//
//  Created by Fei Li on 2024/11/12.
//

#import "JConnWaitingForConnectState.h"
#import "JConnEvent.h"

@interface JConnWaitingForConnectState ()
@property (nonatomic, strong) NSTimer *reconnectTimer;
@end

@implementation JConnWaitingForConnectState

- (BOOL)stateDidEnter {
    [self.connectionManager setConnectionStatus:JConnectionStatusInternalWaitingForConnecting];
    [self startTimer];
    return YES;
}

- (BOOL)stateDidLeave {
    [self stopTimer];
    return YES;
}

- (BOOL)event:(NSInteger)event userInfo:(NSDictionary *)userInfo {
    BOOL result = NO;
    switch (event) {
        case JConnEventUserConnect:
        {
            NSString *token = userInfo[@"token"];
            BOOL isSame = [self.connectionManager isSameToken:token];
            if (isSame) {
                // same token, do nothing
            } else {
                [self.connectionManager notifyDisconnected:JErrorCodeNone extra:nil];
                [self.connectionManager transitionToIdleState];
                [self.connectionManager event:event userInfo:userInfo];
            }
            result = YES;
            break;
        }
            
        case JConnEventUserDisconnect:
            [self.connectionManager disconnectWithoutWS];
            [self.connectionManager notifyDisconnected:JErrorCodeNone extra:nil];
            [self.connectionManager transitionToIdleState];
            result = YES;
            break;
            
        case JConnEventReconnectTimerFire:
            [self.connectionManager transitionToConnectingState];
            result = YES;
            break;
            
        case JConnEventEnterForground:
            [self.connectionManager transitionToConnectingState];
            result = YES;
            break;
            
        case JConnEventNetworkAvailable:
            [self.connectionManager transitionToConnectingState];
            result = YES;
            break;
            
        default:
            break;
    }
    return result;
}

- (void)startTimer {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.reconnectTimer) {
            return;
        }
        self.reconnectTimer = [NSTimer scheduledTimerWithTimeInterval:[self.connectionManager getReconnectInterval] target:self selector:@selector(reconnectTimerFire) userInfo:nil repeats:NO];
    });
}

- (void)stopTimer {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.reconnectTimer) {
            [self.reconnectTimer invalidate];
            self.reconnectTimer = nil;
        }
    });
}

- (void)reconnectTimerFire {
    [self.connectionManager event:JConnEventReconnectTimerFire userInfo:nil];
}

@end
