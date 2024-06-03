//
//  JConnectionManager.m
//  JetIM
//
//  Created by Nathan on 2023/11/29.
//

#import "JConnectionManager.h"
#import "JWebSocket.h"
#import "JNaviTask.h"
#import "JetIMConstInternal.h"
#import <UIKit/UIKit.h>
#import "JLogger.h"

@interface JConnectionManager () <JWebSocketConnectDelegate>
@property (nonatomic, strong) JetIMCore *core;
@property (nonatomic, strong) JConversationManager *conversationManager;
@property (nonatomic, strong) JMessageManager *messageManager;
@property (nonatomic, weak) id<JConnectionDelegate> delegate;
@property (nonatomic, strong) NSTimer *reconnectTimer;
@property (nonatomic, copy) NSString *pushToken;
@property (nonatomic, assign) BOOL isBackground;
@property (nonatomic, assign) UIBackgroundTaskIdentifier bgTask;
@end

@implementation JConnectionManager

- (instancetype)initWithCore:(JetIMCore *)core
         conversationManager:(nonnull JConversationManager *)conversationManager
              messageManager:(nonnull JMessageManager *)messageManager {
    self = [super init];
    if (self) {
        [core.webSocket setConnectDelegate:self];
        core.connectionStatus = JConnectionStatusInternalIdle;
        self.core = core;
        self.conversationManager = conversationManager;
        self.messageManager = messageManager;
        self.bgTask = UIBackgroundTaskInvalid;
        [self addObserver];
    }
    return self;
}

- (void)connectWithToken:(NSString *)token {
    JLogI(@"CON-Connect", @"token is %@", token);
    //TODO: 连接状态判断，如果是连接中而且 token 跟之前的一样的话，直接 return
    //TODO: 连接状态判断，如果连接中或已连接，而且 token 跟之前不一样的话，先 disconnect
    
    if (![self.core.token isEqualToString:token]) {
        //token 更新了，则原来缓存的 userId 不再适用
        self.core.token = token;
        self.core.userId = @"";
    }
    if (![self.core.dbManager isOpen]) {
        if (self.core.userId.length > 0) {
            if ([self.core.dbManager openIMDB:self.core.appKey userId:self.core.userId]) {
                [self dbOpenNotice:YES];
            }
        }
    }
    [self changeStatus:JConnectionStatusInternalConnecting errorCode:JErrorCodeInternalNone extra:@""];
    
    JNaviTask *task = [JNaviTask taskWithUrls:self.core.naviUrls
                                       appKey:self.core.appKey
                                        token:token
                                      success:^(NSString * _Nonnull userId, NSArray<NSString *> * _Nonnull servers) {
        JLogI(@"CON-Navi", @"success");
        self.core.servers = servers;
        [self.core.webSocket connect:self.core.appKey
                               token:token
                           pushToken:self.pushToken
                             servers:self.core.servers];
    } failure:^(JErrorCodeInternal errorCode) {
        JLogI(@"CON-Navi", @"error code is %lu", (long unsigned)errorCode);
        if (errorCode == JErrorCodeInternalTokenIllegal) {
            [self changeStatus:JConnectionStatusInternalFailure errorCode:JErrorCodeInternalTokenIllegal extra:@""];
        } else {
            [self changeStatus:JConnectionStatusInternalWaitingForConnecting errorCode:errorCode extra:@""];
        }
    }];
    [task start];
}

- (void)disconnect:(BOOL)receivePush {
    JLogI(@"CON-Disconnect", @"receivePush is %d", receivePush);
    [self.core.webSocket disconnect:receivePush];
    [self changeStatus:JConnectionStatusInternalDisconnected errorCode:JErrorCodeInternalNone extra:@""];
}

- (void)registerDeviceToken:(NSData *)tokenData {
    if (![tokenData isKindOfClass:[NSData class]]) {
        JLogE(@"CON-Token", @"tokenData 类型错误，请直接将 didRegisterForRemoteNotificationsWithDeviceToken 方法中的 "
              @"deviceToken 传入");
        return;
    }
    JLogI(@"CON-Token", @"");
    NSUInteger len = [tokenData length];
    char *chars = (char *)[tokenData bytes];
    NSMutableString *hexString = [[NSMutableString alloc] init];
    for (NSUInteger i = 0; i < len; i++) {
        [hexString appendString:[NSString stringWithFormat:@"%0.2hhx", chars[i]]];
    }
    self.pushToken = hexString;
    [self.core.webSocket registerPushToken:hexString
                                    userId:self.core.userId
                                   success:^{
        JLogI(@"CON-Token", @"success");
    } error:^(JErrorCodeInternal code) {
        JLogE(@"CON-Token", @"fail, code is %lu", code);
    }];
}

- (void)setDelegate:(id<JConnectionDelegate>)delegate {
    _delegate = delegate;
}

#pragma mark - JWebSocketConnectDelegate
- (void)connectCompleteWithCode:(JErrorCodeInternal)error
                         userId:(NSString *)userId
                        session:(NSString *)session
                          extra:(NSString *)extra {
    if (error == JErrorCodeInternalNone) {
        self.core.userId = userId;
        if (!self.core.dbManager.isOpen) {
            if ([self.core.dbManager openIMDB:self.core.appKey userId:userId]) {
                [self dbOpenNotice:YES];
            } else {
                JLogE(@"CON-Db", @"open fail");
            }
        }
        [self changeStatus:JConnectionStatusInternalConnected errorCode:JErrorCodeInternalNone extra:extra];
        //TODO: operation queue
        [self.conversationManager syncConversations:^{
            [self.messageManager syncMessages];
        }];
    } else {
        if ([self checkConnectionFailure:error]) {
            [self changeStatus:JConnectionStatusInternalFailure errorCode:error extra:extra];
        } else {
            [self changeStatus:JConnectionStatusInternalWaitingForConnecting errorCode:JErrorCodeInternalNone extra:extra];
        }
    }
}

- (void)disconnectWithCode:(JErrorCodeInternal)error
                     extra:(NSString *)extra {
    [self changeStatus:JConnectionStatusInternalDisconnected
             errorCode:error
                 extra:extra];
}

- (void)webSocketDidFail {
    [self handleWebSocketFail];
}

- (void)webSocketDidClose {
    [self handleWebSocketFail];
}

- (void)webSocketDidTimeOut {
    [self handleWebSocketFail];
}

#pragma mark -- internal
- (void)changeStatus:(JConnectionStatusInternal)status
           errorCode:(JErrorCodeInternal)errorCode
               extra:(NSString *)extra {
    dispatch_async(self.core.sendQueue, ^{
        JLogI(@"CON-Status", @"status is %lu, code is %lu", (unsigned long)status, (unsigned long)errorCode);
        if (status == self.core.connectionStatus) {
            return;
        }
        if (status == JConnectionStatusInternalIdle) {
            self.core.connectionStatus = status;
            return;
        }
        if (status == JConnectionStatusInternalConnected && self.core.connectionStatus != JConnectionStatusInternalConnected) {
            [[JLogger shared] removeExpiredLogs];
            [self.core.webSocket startHeartbeat];
        }
        if (self.core.connectionStatus == JConnectionStatusInternalConnected && status != JConnectionStatusInternalConnected) {
            [self.core.webSocket stopHeartbeat];
            [self.core.webSocket pushRemainCmdAndCallbackError];
        }
        JConnectionStatus outStatus = JConnectionStatusIdle;
        switch (status) {
            case JConnectionStatusInternalConnected:
                outStatus = JConnectionStatusConnected;
                break;
                
            case JConnectionStatusInternalDisconnected:
                [self closeDB];
                [self stopReconnectTimer];
                outStatus = JConnectionStatusDisconnected;
                break;

            case JConnectionStatusInternalWaitingForConnecting:
                [self reconnect];
                //无需 break，跟 CONNECTING 一起处理
            case JConnectionStatusInternalConnecting:
                //已经在连接中，不需要再对外抛回调
                if (self.core.connectionStatus == JConnectionStatusInternalConnecting ||
                    self.core.connectionStatus == JConnectionStatusInternalWaitingForConnecting) {
                    self.core.connectionStatus = status;
                    return;
                }
                outStatus = JConnectionStatusConnecting;
                break;
                
            case JConnectionStatusInternalFailure:
                outStatus = JConnectionStatusFailure;
                break;
                
            default:
                break;
        }
        self.core.connectionStatus = status;
        dispatch_async(self.core.delegateQueue, ^{
            if ([self.delegate respondsToSelector:@selector(connectionStatusDidChange:errorCode:extra:)]) {
                [self.delegate connectionStatusDidChange:outStatus errorCode:(JErrorCode)errorCode extra:extra];
            }
        });
    });
}

- (void)closeDB {
    [self.core.dbManager closeIMDB];
    [self dbOpenNotice:NO];
}

- (void)dbOpenNotice:(BOOL)isOpen {
    dispatch_async(self.core.sendQueue, ^{
        if (isOpen) {
            [self.core getSyncTimeFromDB];
        }
        JLogI(@"CON-Db", @"db notice isOpen is %d", isOpen);
        dispatch_async(self.core.delegateQueue, ^{
            if (isOpen) {
                if ([self.delegate respondsToSelector:@selector(dbDidOpen)]) {
                    [self.delegate dbDidOpen];
                }
            } else {
                if ([self.delegate respondsToSelector:@selector(dbDidClose)]) {
                    [self.delegate dbDidClose];
                }
            }
        });
    });
}

- (void)stopReconnectTimer {
    if (self.reconnectTimer) {
        [self.reconnectTimer invalidate];
        self.reconnectTimer = nil;
    }
}

- (void)reconnect {
    //需要在 sendQueue 里
    JLogI(@"CON-Reconnect", @"");
    //TODO: 线程控制，间隔控制
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.reconnectTimer) {
            return;
        }
        self.reconnectTimer = [NSTimer scheduledTimerWithTimeInterval:5
                                                               target:self
                                                             selector:@selector(reconnectTimerFired)
                                                             userInfo:nil
                                                              repeats:NO];
    });
}

- (void)reconnectTimerFired {
    [self stopReconnectTimer];
    if (self.core.connectionStatus == JConnectionStatusInternalWaitingForConnecting) {
        [self connectWithToken:self.core.token];
    }
}

- (void)handleWebSocketFail {
    dispatch_async(self.core.sendQueue, ^{
        if (self.core.connectionStatus == JConnectionStatusInternalDisconnected
            || self.core.connectionStatus == JConnectionStatusInternalFailure) {
            return;
        }
        [self changeStatus:JConnectionStatusInternalWaitingForConnecting errorCode:JErrorCodeInternalNone extra:@""];
    });
}

- (BOOL)checkConnectionFailure:(JErrorCodeInternal)code {
    if (code == JErrorCodeInternalAppKeyEmpty ||
        code == JErrorCodeInternalTokenEmpty ||
        code == JErrorCodeInternalAppKeyInvalid ||
        code == JErrorCodeInternalTokenIllegal ||
        code == JErrorCodeInternalTokenUnauthorized ||
        code == JErrorCodeInternalTokenExpired ||
        code == JErrorCodeInternalAppProhibited ||
        code == JErrorCodeInternalUserProhibited ||
        code == JErrorCodeInternalUserKickedByOtherClient ||
        code == JErrorCodeInternalUserLogOut
        ) {
        return YES;
    }
    return NO;
}

- (void)addObserver {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(enterBackground)
                                                 name:UIApplicationDidEnterBackgroundNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(enterForeground)
                                                 name:UIApplicationWillEnterForegroundNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(appTerminate)
                                                 name:UIApplicationWillTerminateNotification
                                               object:nil];
    if ([UIApplication sharedApplication].applicationState == UIApplicationStateBackground) {
        self.isBackground = YES;
    } else {
        self.isBackground = NO;
    }
}

- (void)enterBackground {
    self.isBackground = YES;
}

- (void)enterForeground {
    self.isBackground = NO;
}

- (void)appTerminate {
    
}

@end
