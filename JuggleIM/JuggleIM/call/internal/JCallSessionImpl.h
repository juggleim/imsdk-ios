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

@interface JCallSessionImpl : NSObject <JCallSession>

@property (nonatomic, strong) JIMCore *core;
@property (nonatomic, weak) id<JCallSessionLifeCycleDelegate> sessionLifeCycleDelegate;

- (void)error:(JCallErrorCode)code;
- (void)startOutgoingTimer;
- (void)inviteFail;
- (void)notifyReceiveCall;

#pragma mark - signal
- (void)signalSingleInvite;
- (void)signalHangup;

#pragma mark - media
- (void)mediaQuit;

#pragma mark - fsm
- (void)event:(NSInteger)event
     userInfo:(id)userInfo;
- (void)transitionToConnectedState;
- (void)transitionToConnectingState;
- (void)transitionToIdleState;
- (void)transitionToIncomingState;
- (void)transitionToOutgoingState;


@end
