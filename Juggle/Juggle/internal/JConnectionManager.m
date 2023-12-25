//
//  JConnectionManager.m
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

#import "JConnectionManager.h"
#import "JWebSocket.h"

@interface JConnectionManager () <JWebSocketConnectDelegate>
@property (nonatomic, strong) JuggleCore *core;
@property (nonatomic, strong) JConversationManager *conversationManager;
@property (nonatomic, strong) JMessageManager *messageManager;
@property (nonatomic, weak) id<JConnectionDelegate> delegate;

@end

@implementation JConnectionManager

- (instancetype)initWithCore:(JuggleCore *)core
         conversationManager:(nonnull JConversationManager *)conversationManager
              messageManager:(nonnull JMessageManager *)messageManager {
    JConnectionManager *m = [[JConnectionManager alloc] init];
    [core.webSocket setConnectDelegate:m];
    core.connectionStatus = JConnectionStatusInternalIdle;
    m.core = core;
    m.conversationManager = conversationManager;
    m.messageManager = messageManager;
    return m;
}

- (void)connectWithToken:(NSString *)token {
    //TODO: 连接状态判断，如果是连接中而且 token 跟之前的一样的话，直接 return
    
    if (![self.core.token isEqualToString:token]) {
        //token 更新了，则原来缓存的 userId 不再适用
        self.core.token = token;
        self.core.userId = @"";
    }
    if (self.core.userId.length > 0) {
        if ([self.core.dbManager openIMDB:self.core.appKey userId:self.core.userId]) {
            [self dbOpenNotice:JDBStatusOpen];
        }
    }
    [self changeStatus:JConnectionStatusInternalConnecting];
    
    
    
    //TODO: navi
//    [self.core.dbManager openIMDB:self.core.appKey userId:];
    [self.core.webSocket connect:self.core.appKey token:token];
}

- (void)disconnect:(BOOL)receivePush {
    NSLog(@"[Juggle] disconnect, receivePush is %d", receivePush);
    [self changeStatus:JConnectionStatusInternalDisconnected];
    [self.core.webSocket disconnect:receivePush];
}

- (void)setDelegate:(id<JConnectionDelegate>)delegate {
    _delegate = delegate;
}

#pragma mark - JWebSocketConnectDelegate
- (void)connectCompleteWithCode:(JErrorCode)error
                         userId:(NSString *)userId {
    if (error == JErrorCodeNone) {
        self.core.userId = userId;
        if (self.core.dbStatus != JDBStatusOpen) {
            if ([self.core.dbManager openIMDB:self.core.appKey userId:userId]) {
                [self dbOpenNotice:JDBStatusOpen];
            } else {
                NSLog(@"[Juggle] db open fail");
            }
        }
        [self changeStatus:JConnectionStatusInternalConnected];
        //TODO: operation queue
        [self.conversationManager syncConversations];
    } else {
        [self changeStatus:JConnectionStatusInternalWaitingForConnecting];
        [self reconnect];
    }
}

- (void)webSocketDidFail {
    [self changeStatus:JConnectionStatusInternalWaitingForConnecting];
    [self reconnect];
}

- (void)webSocketDidClose {
    dispatch_async(self.core.sendQueue, ^{
        if (self.core.connectionStatus == JConnectionStatusInternalDisconnected) {
            return;
        }
        [self changeStatus:JConnectionStatusInternalWaitingForConnecting];
        [self reconnect];
    });
}

#pragma mark -- internal
- (void)changeStatus:(JConnectionStatusInternal)status {
    dispatch_async(self.core.sendQueue, ^{
        if (status == JConnectionStatusInternalIdle) {
            self.core.connectionStatus = status;
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
                break;
                
            case JConnectionStatusInternalWaitingForConnecting:
                //已经在连接中，不需要再对外抛回调
                if (self.core.connectionStatus == JConnectionStatusInternalConnecting ||
                    self.core.connectionStatus == JConnectionStatusInternalWaitingForConnecting) {
                    self.core.connectionStatus = JConnectionStatusInternalWaitingForConnecting;
                    return;
                }
                outStatus = JConnectionStatusConnecting;
                break;
                
            default:
                break;
        }
        self.core.connectionStatus = status;
        dispatch_async(self.core.delegateQueue, ^{
            if (self.delegate) {
                [self.delegate connectionStatusDidChange:outStatus errorCode:JErrorCodeNone];
            }
        });
    });
}

- (void)dbOpenNotice:(JDBStatus)status {
    dispatch_async(self.core.sendQueue, ^{
        self.core.dbStatus = status;
        if (status == JDBStatusOpen) {
            [self.core getSyncTimeFromDB];
        }
        dispatch_async(self.core.delegateQueue, ^{
            if (self.delegate) {
                if (status == JDBStatusOpen) {
                    [self.delegate dbDidOpen];
                } else {
                    [self.delegate dbDidClose];
                }
            }
        });
    });
}

- (void)reconnect {
    //需要在 sendQueue 里
    NSLog(@"[Juggle] reconnect");
}

@end
