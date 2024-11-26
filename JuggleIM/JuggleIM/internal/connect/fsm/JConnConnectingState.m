//
//  JConnConnectingState.m
//  JuggleIM
//
//  Created by Fei Li on 2024/11/12.
//

#import "JConnConnectingState.h"
#import "JConnEvent.h"
#import "JLogger.h"

typedef NS_ENUM(NSInteger, JConnectingStoreStatus) {
    JConnectingStoreStatusNone,
    JConnectingStoreStatusConnect,
    JConnectingStoreStatusDisconnect
};

@interface JConnConnectingState ()
@property (nonatomic, copy) NSString *userToken;
//连接过程中保存的状态
@property (nonatomic, assign) JConnectingStoreStatus storeStatus;
@property (nonatomic, assign) BOOL receivePush;
@end

@implementation JConnConnectingState

- (BOOL)stateDidEnter {
    self.userToken = nil;
    self.storeStatus = JConnectingStoreStatusNone;
    self.receivePush = NO;
    [self.connectionManager setConnectionStatus:JConnectionStatusInternalConnecting];
    [self.connectionManager connect];
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
            BOOL isSame = [self.connectionManager isSameToken:token];
            if (isSame) {
                // same token, do nothing
                JLogW(@"CON-Connect", @"same token is connecting");
                self.userToken = nil;
                self.storeStatus = JConnectingStoreStatusNone;
            } else {
                self.userToken = token;
                self.storeStatus = JConnectingStoreStatusConnect;
            }
            result = YES;
            break;
        }
            
        case JConnEventConnectDone:
        {
            NSString *extra = userInfo[@"extra"];
            [self.connectionManager transitionToConnectedState];
            [self.connectionManager notifyConnected:extra];
            if (self.storeStatus == JConnectingStoreStatusConnect) {
                if (self.userToken.length > 0) {
                    [self.connectionManager event:JConnEventUserConnect userInfo:@{@"token":self.userToken}];
                }
            } else if (self.storeStatus == JConnectingStoreStatusDisconnect) {
                [self.connectionManager event:JConnEventUserDisconnect userInfo:@{@"receivePush":@(self.receivePush)}];
            }
            result = YES;
            break;
        }
            
        case JConnEventConnectFailure:
        {
            NSNumber *num = userInfo[@"code"];
            NSInteger errorCode = num.integerValue;
            NSString *extra = userInfo[@"extra"];
            [self.connectionManager transitionToIdleState];
            [self.connectionManager notifyFailure:errorCode
                                            extra:extra];
            if (self.storeStatus == JConnectingStoreStatusConnect) {
                if (self.userToken.length > 0) {
                    [self.connectionManager event:JConnEventUserConnect userInfo:@{@"token":self.userToken}];
                }
            } else if (self.storeStatus == JConnectingStoreStatusDisconnect) {
                [self.connectionManager event:JConnEventUserDisconnect userInfo:@{@"receivePush":@(self.receivePush)}];
            }
            
            result = YES;
            break;
        }
            
        case JConnEventWebsocketFail:
            [self.connectionManager transitionToWaitingForConnectState];
            if (self.storeStatus == JConnectingStoreStatusConnect) {
                if (self.userToken.length > 0) {
                    [self.connectionManager event:JConnEventUserConnect userInfo:@{@"token":self.userToken}];
                }
            } else if (self.storeStatus == JConnectingStoreStatusDisconnect) {
                [self.connectionManager event:JConnEventUserDisconnect userInfo:@{@"receivePush":@(self.receivePush)}];
            }
            result = YES;
            break;
            
        case JConnEventUserDisconnect:
        {
            self.userToken = nil;
            self.storeStatus = JConnectingStoreStatusDisconnect;
            NSNumber *num = userInfo[@"receivePush"];
            BOOL receivePush = num.boolValue;
            self.receivePush = receivePush;
            result = YES;
            break;
        }
        
        default:
            break;
    }
    return result;
}

@end
