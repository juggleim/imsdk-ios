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
@property (nonatomic, weak) id<JConnectionDelegate> delegate;

@end

@implementation JConnectionManager

- (instancetype)initWithCore:(JuggleCore *)core {
    JConnectionManager *m = [[JConnectionManager alloc] init];
    [core.webSocket setConnectDelegate:m];
    core.status = JConnectionStatusInternalIdle;
    m.core = core;
    return m;
}

- (void)connectWithToken:(NSString *)token {
    self.core.token = token;
    [self changeStatus:JConnectionStatusInternalConnecting];
    
    
    
    //TODO: navi
//    [self.core.dbManager openIMDB:self userId:<#(nonnull NSString *)#>]
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

- (void)connectCompleteWithCode:(JErrorCode)error
                         userId:(NSString *)userId {
    if (error == JErrorCodeNone) {
        self.core.userId = userId;
        [self changeStatus:JConnectionStatusInternalConnected];
    } else {
        [self reconnect];
    }
}


#pragma mark -- internal
- (void)changeStatus:(JConnectionStatusInternal)status {
    dispatch_async(self.core.sendQueue, ^{
        self.core.status = status;
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
        dispatch_async(self.core.delegateQueue, ^{
            if (self.delegate) {
                [self.delegate connectionStatusDidChange:outStatus errorCode:JErrorCodeNone];
            }
        });
    });
}

- (void)reconnect {
    NSLog(@"[Juggle] reconnect");
}

- (void)syncConversations {
    [self.core.webSocket syncConversations:0
                                     count:100
                                    userId:self.core.userId
                                   success:^(NSArray * _Nonnull conversations, BOOL isFinished) {
        
    } error:^(JErrorCode code) {
        
    }];
}

@end
