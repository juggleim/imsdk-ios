//
//  JConnectionManager.m
// JuggleIM
//
//  Created by Nathan on 2023/11/29.
//

#import "JConnectionManager.h"
#import "JWebSocket.h"
#import "JNaviTask.h"
#import "JuggleIMConstInternal.h"
#import <UIKit/UIKit.h>
#import "JLogger.h"
#import "JIntervalGenerator.h"
#import "JReachability.h"
#import "JStateMachine.h"
#import "JConnIdleState.h"
#import "JConnConnectedState.h"
#import "JConnConnectingState.h"
#import "JConnWaitingForConnectState.h"
#import "JConnEventUtil.h"
#import "JConnEvent.h"
#import "JConnSuperState.h"

@interface JConnectionManager () <JWebSocketConnectDelegate>
@property (nonatomic, strong) JIMCore *core;
@property (nonatomic, strong) JConversationManager *conversationManager;
@property (nonatomic, strong) JMessageManager *messageManager;
@property (nonatomic, strong) JChatroomManager *chatroomManager;
@property (nonatomic, strong) JCallManager *callManager;
@property (nonatomic, strong) NSHashTable <id<JConnectionDelegate>> *delegates;
@property (nonatomic, strong) NSTimer *reconnectTimer;
@property (nonatomic, copy) NSString *pushToken;
@property (nonatomic, assign) BOOL isBackground;
@property (nonatomic, strong) JIntervalGenerator *intervalGenerator;
@property (nonatomic, strong) JReachability *reachability;

@property (nonatomic, strong) JStateMachine *stateMachine;
@property (nonatomic, strong) JConnSuperState *superState;
@property (nonatomic, strong) JConnIdleState *idleState;
@property (nonatomic, strong) JConnConnectedState *connectedState;
@property (nonatomic, strong) JConnConnectingState *connectingState;
@property (nonatomic, strong) JConnWaitingForConnectState *waitingState;
@end

@implementation JConnectionManager

- (instancetype)initWithCore:(JIMCore *)core
         conversationManager:(nonnull JConversationManager *)conversationManager
              messageManager:(nonnull JMessageManager *)messageManager
             chatroomManager:(nonnull JChatroomManager *)chatroomManager
                 callManager:(nonnull JCallManager *)callManager {
    self = [super init];
    if (self) {
        [core.webSocket setConnectDelegate:self];
        core.connectionStatus = JConnectionStatusInternalIdle;
        self.core = core;
        self.conversationManager = conversationManager;
        self.messageManager = messageManager;
        self.chatroomManager = chatroomManager;
        self.callManager = callManager;
        [self addObserver];
        [self stateMachine];
    }
    return self;
}

- (void)connectWithToken:(NSString *)token {
    JLogI(@"CON-Connect", @"token is %@", token);
    if (token.length == 0) {
        token = @"";
    }
    [self event:JConnEventUserConnect userInfo:@{@"token":token}];
}

- (void)disconnect:(BOOL)receivePush {
    JLogI(@"CON-Disconnect", @"receivePush is %d", receivePush);
    
    [self event:JConnEventUserDisconnect userInfo:@{@"receivePush":@(receivePush)}];
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

- (JConnectionStatus)getConnectionStatus {
    return (JConnectionStatus)self.core.connectionStatus;
}

- (void)addDelegate:(id<JConnectionDelegate>)delegate {
    dispatch_async(self.core.delegateQueue, ^{
        if (!delegate) {
            return;
        }
        [self.delegates addObject:delegate];
    });
}

#pragma mark - JWebSocketConnectDelegate
- (void)connectCompleteWithCode:(JErrorCodeInternal)error
                         userId:(NSString *)userId
                        session:(NSString *)session
                          extra:(NSString *)extra {
    if (error == JErrorCodeInternalNone) {
        [self.intervalGenerator reset];
        self.core.userId = userId;
        if (!self.core.dbManager.isOpen) {
            if ([self.core.dbManager openIMDB:self.core.appKey userId:userId]) {
                [self dbOpenNotice:YES];
            } else {
                JLogE(@"CON-Db", @"open fail");
            }
        }
        [self.messageManager connectSuccess];
        [self.conversationManager connectSuccess];
        [self.chatroomManager connectSuccess];
        [self.callManager connectSuccess];
        [self event:JConnEventConnectDone userInfo:@{@"extra":extra}];
        [self.conversationManager syncConversations:^{
            [self.messageManager syncMessages];
        }];
    } else {
        if ([self checkConnectionFailure:error]) {
            [self event:JConnEventConnectFailure userInfo:@{@"code":@(error), @"extra":extra}];
        } else {
            [self event:JConnEventWebsocketFail userInfo:nil];
        }
    }
}

- (void)disconnectWithCode:(JErrorCodeInternal)error
                     extra:(NSString *)extra {
    [self event:JConnEventRemoteDisconnect userInfo:@{@"code":@(error), @"extra":extra}];
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

#pragma mark - called from fsm
- (void)setConnectionStatus:(JConnectionStatusInternal)status {
    self.core.connectionStatus = status;
}

- (BOOL)isSameToken:(NSString *)token {
    return [self.core.token isEqualToString:token];
}

- (BOOL)updateToken:(NSString *)token {
    BOOL isUpdate = NO;
    if (![self.core.token isEqualToString:token]) {
        self.core.token = token;
        self.core.userId = @"";
        isUpdate = YES;
    }
    return isUpdate;
}

- (void)connect {
    if (![self.core.dbManager isOpen]) {
        if (self.core.userId.length > 0) {
            if ([self.core.dbManager openIMDB:self.core.appKey userId:self.core.userId]) {
                [self dbOpenNotice:YES];
            }
        }
    }
    
    [self.core.webSocket connect:self.core.appKey
                           token:self.core.token
                       pushToken:self.pushToken
                         servers:self.core.servers];
    
//    JNaviTask *task = [JNaviTask taskWithUrls:self.core.naviUrls
//                                       appKey:self.core.appKey
//                                        token:token
//                                      success:^(NSString * _Nonnull userId, NSArray<NSString *> * _Nonnull servers) {
//        JLogI(@"CON-Navi", @"success");
//        self.core.servers = servers;
//        [self.core.webSocket connect:self.core.appKey
//                               token:token
//                           pushToken:self.pushToken
//                             servers:self.core.servers];
//    } failure:^(JErrorCodeInternal errorCode) {
//        JLogI(@"CON-Navi", @"error code is %lu", (long unsigned)errorCode);
//        if ([self checkConnectionFailure:errorCode]) {
//            [self changeStatus:JConnectionStatusInternalFailure errorCode:errorCode extra:@""];
//        } else {
//            [self changeStatus:JConnectionStatusInternalWaitingForConnecting errorCode:errorCode extra:@""];
//        }
//    }];
//    [task start];
}

- (void)enterConnected {
    [[JLogger shared] removeExpiredLogs];
    [self.core.webSocket startHeartbeat];
}

- (void)leaveConnected {
    [self.core.webSocket stopHeartbeat];
    [self.core.webSocket pushRemainCmdAndCallbackError];
}

- (void)disconnectExist:(BOOL)receivePush {
    [self.chatroomManager userDisconnect];
    [self.core.webSocket disconnect:receivePush];
    [self closeDB];
}

- (void)disconnectWithoutWS {
    [self.chatroomManager userDisconnect];
    [self closeDB];
}

- (void)handleRemoteDisconnect {
    [self closeDB];
}

- (float)getReconnectInterval {
    return [self.intervalGenerator getNextInterval];
}

- (void)notifyConnecting {
    [self notify:JConnectionStatusConnecting
            code:JErrorCodeNone
           extra:@""];
}

- (void)notifyConnected:(NSString *)extra {
    [self notify:JConnectionStatusConnected
            code:JErrorCodeNone
           extra:extra];
}

- (void)notifyDisconnected:(NSInteger)code extra:(NSString *)extra {
    [self notify:JConnectionStatusDisconnected
            code:code
           extra:extra];
}

- (void)notifyFailure:(NSInteger)code extra:(NSString *)extra {
    [self notify:JConnectionStatusFailure
            code:code
           extra:extra];
}

- (void)notify:(JConnectionStatus)status
          code:(JErrorCode)code
         extra:(NSString *)extra {
    if (code == JErrorCodeUserKickedByOtherClient) {
        [self.callManager imKick];
    }
    dispatch_async(self.core.delegateQueue, ^{
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConnectionDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj respondsToSelector:@selector(connectionStatusDidChange:errorCode:extra:)]) {
                [obj connectionStatusDidChange:status
                                     errorCode:code
                                         extra:extra];
            }
        }];
    });
}

- (void)transitionToIdleState {
    [self.stateMachine transitionTo:self.idleState];
}

- (void)transitionToConnectingState {
    [self.stateMachine transitionTo:self.connectingState];
}

- (void)transitionToConnectedState {
    [self.stateMachine transitionTo:self.connectedState];
}

- (void)transitionToWaitingForConnectState {
    [self.stateMachine transitionTo:self.waitingState];
}

#pragma mark - fsm
- (void)event:(NSInteger)event
     userInfo:(NSDictionary *)userInfo {
    [self.stateMachine event:event
                        name:[JConnEventUtil nameOfEvent:event]
                    userInfo:userInfo];
}

#pragma mark - internal
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
                [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConnectionDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                    if ([obj respondsToSelector:@selector(dbDidOpen)]) {
                        [obj dbDidOpen];
                    }
                }];
            } else {
                [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConnectionDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                    if ([obj respondsToSelector:@selector(dbDidClose)]) {
                        [obj dbDidClose];
                    }
                }];
            }
        });
    });
}

- (void)handleWebSocketFail {
    [self event:JConnEventWebsocketFail userInfo:nil];
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
    self.reachability = [JReachability reachabilityForInternetConnection];
    [self.reachability startNotifier];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(networkChanged:)
                                                 name:kJReachabilityChangedNotification
                                               object:nil];
}

- (void)enterBackground {
    self.isBackground = YES;
    [self.core.webSocket pushSwitch:YES userId:self.core.userId];
}

- (void)enterForeground {
    [self.intervalGenerator reset];
    self.isBackground = NO;
    [self.core.webSocket pushSwitch:NO userId:self.core.userId];
    [self event:JConnEventEnterForground userInfo:nil];
}

- (void)appTerminate {
    
}

- (void)networkChanged:(NSNotification *)ntf {
    JReachability *curReachability = [ntf object];
    JNetworkStatus status = JNetworkStatusReachableViaWiFi;
    if (![curReachability respondsToSelector:@selector(currentReachabilityStatus)]) {
        JLogV(@"CON-Reach", @"networkChanged: unknown reachability obj");
    } else {
        status = [curReachability currentReachabilityStatus];
    }
    JLogI(@"CON-Network", @"network:%ld", status);
    
    if (status != JNetworkStatusNotReachable) {
        [self.intervalGenerator reset];
        [self event:JConnEventNetworkAvailable userInfo:nil];
    }
}

- (NSHashTable<id<JConnectionDelegate>> *)delegates {
    if (!_delegates) {
        _delegates = [NSHashTable weakObjectsHashTable];
    }
    return _delegates;
}

- (JIntervalGenerator *)intervalGenerator {
    if (!_intervalGenerator) {
        _intervalGenerator = [[JIntervalGenerator alloc] init];
    }
    return _intervalGenerator;
}

- (JStateMachine *)stateMachine {
    if (!_stateMachine) {
        _stateMachine = [[JStateMachine alloc] initWithName:@"j_connect"];
        [_stateMachine setInitialState:self.idleState];
        [_stateMachine start];
    }
    return _stateMachine;
}

- (JConnSuperState *)superState {
    if (!_superState) {
        _superState = [[JConnSuperState alloc] init];
        _superState.connectionManager = self;
    }
    return _superState;
}

- (JConnIdleState *)idleState {
    if (!_idleState) {
        _idleState = [[JConnIdleState alloc] initWithName:@"idle" superState:self.superState];
        _idleState.connectionManager = self;
    }
    return _idleState;
}

- (JConnConnectingState *)connectingState {
    if (!_connectingState) {
        _connectingState = [[JConnConnectingState alloc] initWithName:@"connecting" superState:self.superState];
        _connectingState.connectionManager = self;
    }
    return _connectingState;
}

- (JConnConnectedState *)connectedState {
    if (!_connectedState) {
        _connectedState = [[JConnConnectedState alloc] initWithName:@"connected" superState:self.superState];
        _connectedState.connectionManager = self;
    }
    return _connectedState;
}

- (JConnWaitingForConnectState *)waitingState {
    if (!_waitingState) {
        _waitingState = [[JConnWaitingForConnectState alloc] initWithName:@"waiting" superState:self.superState];
        _waitingState.connectionManager = self;
    }
    return _waitingState;
}

@end
