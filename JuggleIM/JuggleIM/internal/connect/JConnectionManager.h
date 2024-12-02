//
//  JConnectionManager.h
// JuggleIM
//
//  Created by Nathan on 2023/11/29.
//

#import <Foundation/Foundation.h>
#import <JConnectionProtocol.h>
#import "JIMCore.h"
#import "JConversationManager.h"
#import "JMessageManager.h"
#import "JChatroomManager.h"
#import "JCallManager.h"

@interface JConnectionManager : NSObject <JConnectionProtocol>
- (instancetype)initWithCore:(JIMCore *)core
         conversationManager:(JConversationManager *)conversationManager
              messageManager:(JMessageManager *)messageManager
             chatroomManager:(JChatroomManager *)chatroomManager
                 callManager:(JCallManager *)callManager;

#pragma mark - called from fsm
- (void)setConnectionStatus:(JConnectionStatusInternal)status;
- (BOOL)isSameToken:(NSString *)token;
- (BOOL)updateToken:(NSString *)token;
- (void)connect;
- (void)enterConnected;
- (void)leaveConnected;
- (void)disconnectExist:(BOOL)receivePush;
- (void)disconnectWithoutWS;
- (float)getReconnectInterval;
- (void)handleRemoteDisconnect;
- (void)pushSwitch:(BOOL)isBackground;

- (void)notifyConnecting;
- (void)notifyConnected:(NSString *)extra;
- (void)notifyDisconnected:(NSInteger)code
                     extra:(NSString *)extra;
- (void)notifyFailure:(NSInteger)code
                extra:(NSString *)extra;

- (void)event:(NSInteger)event
     userInfo:(NSDictionary *)userInfo;
- (void)transitionToIdleState;
- (void)transitionToConnectingState;
- (void)transitionToConnectedState;
- (void)transitionToWaitingForConnectState;

@end
