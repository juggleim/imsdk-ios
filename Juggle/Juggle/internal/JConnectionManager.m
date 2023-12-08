//
//  JConnectionManager.m
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

#import "JConnectionManager.h"
#import "JWebSocket.h"

typedef NS_ENUM(NSUInteger, JConnectionStatusInternal) {
    //未连接
    JConnectionStatusInternalIdle = 0,
    //已连接
    JConnectionStatusInternalConnected = 1,
    //连接断开（用户主动断开）
    JConnectionStatusInternalDisconnected = 2,
    //连接中
    JConnectionStatusInternalConnecting = 3,
    //连接 token 错误
    JConnectionStatusInternalTokenIncorrect = 4
};

@interface JConnectionManager () <JWebSocketConnectDelegate>
@property (nonatomic, strong) JuggleCore *core;
@property (nonatomic, weak) id<JConnectionDelegate> delegate;
@property (nonatomic, assign) JConnectionStatusInternal status;
@end

@implementation JConnectionManager

- (instancetype)initWithCore:(JuggleCore *)core {
    JConnectionManager *m = [[JConnectionManager alloc] init];
    [core.webSocket setConnectDelegate:m];
    m.core = core;
    m.status = JConnectionStatusInternalIdle;
    return m;
}

- (void)connectWithToken:(NSString *)token {
    self.core.token = token;
    [self changeStatus:JConnectionStatusInternalConnecting];
    
    //TODO: navi
    [self.core.webSocket connect:self.core.appKey token:token];
}

- (void)disconnect:(BOOL)receivePush {
    NSLog(@"[Juggle] disconnect, receivePush is %d", receivePush);
    [self.core.webSocket disconnect:receivePush];
}

- (void)setDelegate:(id<JConnectionDelegate>)delegate {
    _delegate = delegate;
}

- (void)connectCompleteWithCode:(JErrorCode)error
                         userId:(NSString *)userId {
    dispatch_async(self.core.delegateQueue, ^{
        NSLog(@"[Juggle] connect complete, error code is %lu", (unsigned long)error);
        if (self.delegate) {
            if (error == JErrorCodeNone) {
                self.core.userId = userId;
                [self.delegate connectionStatusDidChange:JConnectionStatusConnected errorCode:JErrorCodeNone];
            } else {
                [self reconnect];
            }
        }
    });
}


#pragma mark -- internal
- (void)changeStatus:(JConnectionStatusInternal)status {
    dispatch_async(self.core.sendQueue, ^{
        self.status = status;
        if (status == JConnectionStatusInternalIdle) {
            return;
        }
        JConnectionStatus outStatus = JConnectionStatusIdle;
        switch (status) {
            case JConnectionStatusInternalConnected:
                outStatus = JConnectionStatusConnected;
                break;
                
            case JConnectionStatusInternalDisconnected:
                outStatus = JConnectionStatusConnected;
                break;
                
            case JConnectionStatusInternalConnecting:
                outStatus = JConnectionStatusConnecting;
                break;
                
            case JConnectionStatusInternalTokenIncorrect:
                outStatus = JConnectionStatusTokenIncorrect;
                
            default:
                break;
        }
        if (self.delegate) {
            [self.delegate connectionStatusDidChange:outStatus errorCode:JErrorCodeNone];
        }
    });
}

- (void)reconnect {
    NSLog(@"[Juggle] reconnect");
}

- (void)syncConversations {
    [self.core.webSocket syncConversations:0
                                     count:100
                                    userId:self.core.userId
                                   success:^(NSArray * _Nonnull conversations, BOOL isRemaining) {
        
    } error:^(JErrorCode code) {
        
    }];
}

@end
