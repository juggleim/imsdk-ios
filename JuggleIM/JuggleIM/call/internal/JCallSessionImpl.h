//
//  JCallSessionImpl.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/23.
//

#import <JuggleIM/JuggleIM.h>
#import "JCallManager.h"
#import "JState.h"
#import "JCallSession.h"
#import "JCallSessionLifeCycleDelegate.h"
#import "JCallInternalConst.h"

@interface JCallSessionImpl : NSObject <JCallSession>

@property (nonatomic, strong) JIMCore *core;
@property (nonatomic, assign) JCallEngineType engineType;
@property (nonatomic, weak) id<JCallSessionLifeCycleDelegate> sessionLifeCycleDelegate;
@property (nonatomic, copy) NSString *zegoToken;

- (void)error:(JCallErrorCode)code;
- (void)startOutgoingTimer;
- (void)inviteFail;
- (void)notifyReceiveCall;
- (void)memberHangup:(NSString *)userId;
- (void)memberAccept:(NSString *)userId;
- (void)addMember:(JCallMember *)member;
- (void)removeMember:(NSString *)userId;

#pragma mark - signal
- (void)signalSingleInvite;
- (void)signalHangup;
- (void)signalAccept;
- (void)signalConnected;
- (void)ping;

#pragma mark - media
- (void)mediaQuit;
- (void)mediaJoin;

#pragma mark - fsm
- (void)event:(NSInteger)event
     userInfo:(NSDictionary *)userInfo;
- (void)transitionToConnectedState;
- (void)transitionToConnectingState;
- (void)transitionToIdleState;
- (void)transitionToIncomingState;
- (void)transitionToOutgoingState;


@end
