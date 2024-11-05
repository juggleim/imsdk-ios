//
//  JCallSessionImpl.m
//  JuggleIM
//
//  Created by Fei Li on 2024/10/23.
//

#import "JCallSessionImpl.h"
#import "JStateMachine.h"
#import "JCallSuperState.h"
#import "JConnectedState.h"
#import "JConnectingState.h"
#import "JIdleState.h"
#import "JIncomingState.h"
#import "JOutgoingState.h"
#import "JLogger.h"
#import "JCallEvent.h"
#import "JCallMediaManager.h"

@interface JCallSessionImpl ()
@property (nonatomic, strong) JStateMachine *stateMachine;
@property (nonatomic, strong) JCallSuperState *superState;
@property (nonatomic, strong) JConnectedState *connectedState;
@property (nonatomic, strong) JConnectingState *connectingState;
@property (nonatomic, strong) JIdleState *idleState;
@property (nonatomic, strong) JIncomingState *incomingState;
@property (nonatomic, strong) JOutgoingState *outgoingState;
@property (nonatomic, strong) NSHashTable <id<JCallSessionDelegate>> *delegates;
@property (nonatomic, copy, readwrite) NSMutableArray <JCallMember *> *members;
@end

@implementation JCallSessionImpl

#pragma mark - JCallSession
- (void)addDelegate:(id<JCallSessionDelegate>)delegate {
    dispatch_async(self.core.delegateQueue, ^{
        if (!delegate) {
            return;
        }
        [self.delegates addObject:delegate];
    });
}

- (void)accept { 
    [self.stateMachine event:JCallEventAccept userInfo:nil];
}


- (void)hangup { 
    [self.stateMachine event:JCallEventHangup userInfo:nil];
}


- (void)inviteUsers:(NSArray<NSString *> *)userIdList { 
    
}

- (void)muteMicrophone:(BOOL)isMute {
    [[JCallMediaManager shared] muteMicrophone:isMute];
}

- (void)muteSpeaker:(BOOL)isMute {
    [[JCallMediaManager shared] muteSpeaker:isMute];
}


- (void)setVideoView:(UIView *)view forUserId:(NSString *)userId { 
    
}


#pragma mark - JCallSessionImpl
- (void)error:(JCallErrorCode)code {
    dispatch_async(self.core.delegateQueue, ^{
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JCallSessionDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj respondsToSelector:@selector(errorDidOccur:)]) {
                [obj errorDidOccur:code];
            }
        }];
    });
}

- (void)inviteFail {
    self.callStatus = JCallStatusIdle;
    self.finishTime = [[NSDate date] timeIntervalSince1970];
    self.finishReason = JCallFinishReasonNetworkError;
}

- (void)startOutgoingTimer {
    //TODO:
}

- (void)notifyReceiveCall {
    [self.sessionLifeCycleDelegate callDidReceive:self];
}

- (void)memberHangup:(NSString *)userId {
    [self removeMember:userId];
    if (!self.isMultiCall) {
        self.finishTime = [[NSDate date] timeIntervalSince1970];
        if (self.callStatus == JCallStatusOutgoing) {
            self.finishReason = JCallFinishReasonOtherSideDecline;
        } else {
            self.finishReason = JCallFinishReasonOtherSideHangup;
        }
    }
}

- (void)memberAccept:(NSString *)userId {
    if (!self.isMultiCall) {
        for (JCallMember *member in self.members) {
            if ([member.userInfo.userId isEqualToString:userId]) {
                member.callStatus = JCallStatusConnecting;
            }
        }
    } else {
        
    }
}

- (void)addMember:(JCallMember *)member {
    [self.members addObject:member];
}

- (void)removeMember:(NSString *)userId {
    for (JCallMember *member in self.members) {
        if ([member.userInfo.userId isEqualToString:userId]) {
            [self.members removeObject:member];
            return;
        }
    }
}

#pragma mark - signal
- (void)signalSingleInvite {
    NSMutableArray *targetIds = [NSMutableArray array];
    for (JCallMember *member in self.members) {
        [targetIds addObject:member.userInfo.userId];
    }
    [self.core.webSocket callInvite:self.callId
                        isMultiCall:NO
                       targetIdList:targetIds
                         engineType:(NSUInteger)self.engineType
                            success:^(NSString *zegoToken){
        JLogI(@"Call-Signal", @"send invite success");
        self.zegoToken = zegoToken;
    } error:^(JErrorCodeInternal code) {
        JLogE(@"Call-Signal", @"send invite error, code is %ld", code);
        [self.stateMachine event:JCallEventInviteFail userInfo:nil];
    }];
}

- (void)signalHangup {
    [self.core.webSocket callHangup:self.callId
                            success:^{
        JLogI(@"Call-Signal", @"send hangup success");
    } error:^(JErrorCodeInternal code) {
        JLogE(@"Call-Signal", @"send hangup error, code is %ld", code);
    }];
}

- (void)signalAccept {
    [self.core.webSocket callAccept:self.callId
                            success:^(NSString * _Nonnull zegoToken) {
        JLogI(@"Call-Signal", @"send accept success");
        self.zegoToken = zegoToken;
        [self.stateMachine event:JCallEventAcceptDone userInfo:nil];
    } error:^(JErrorCodeInternal code) {
        JLogE(@"Call-Signal", @"send invite error, code is %ld", code);
        [self.stateMachine event:JCallEventAcceptFail userInfo:nil];
    }];
}

- (void)signalConnected {
    [self.core.webSocket callConnected:self.callId
                               success:^{
        JLogI(@"Call-Signal", @"call connected success");
    } error:^(JErrorCodeInternal code) {
        JLogE(@"Call-Signal", @"call connected error, code is %ld", code);
    }];
}

#pragma mark - media
- (void)mediaQuit {
    JLogI(@"Call-Media", @"media quit");
    [JCallMediaManager.shared leaveRoom:self.callId];
}

- (void)mediaJoin {
    JLogI(@"Call-Media", @"media join");
    [JCallMediaManager.shared joinRoom:self
                              complete:^(int errorCode, NSDictionary *data) {
        if (errorCode == 0) {
            JLogI(@"Call-Media", @"join room success");
            [self.stateMachine event:JCallEventJoinChannelDone userInfo:nil];
        } else {
            JLogE(@"Call-Media", @"join room error, code is %d", errorCode);
            [self.stateMachine event:JCallEventJoinChannelFail userInfo:@{@"code":@(errorCode)}];
        }
    }];
}

#pragma mark - fsm
- (void)event:(NSInteger)event userInfo:(id)userInfo {
    [self.stateMachine event:event userInfo:userInfo];
}

- (void)transitionToConnectedState {
    [self.stateMachine transitionTo:self.connectedState];
    [self signalConnected];
    dispatch_async(self.core.delegateQueue, ^{
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JCallSessionDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj respondsToSelector:@selector(callDidConnect)]) {
                [obj callDidConnect];
            }
        }];
    });
}

- (void)transitionToConnectingState {
    [self.stateMachine transitionTo:self.connectingState];
}

- (void)transitionToIdleState {
    [self.stateMachine transitionTo:self.idleState];
    [self mediaQuit];
    dispatch_async(self.core.delegateQueue, ^{
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JCallSessionDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj respondsToSelector:@selector(callDidFinish:)]) {
                [obj callDidFinish:self.finishReason];
            }
        }];
        [self destroy];
    });
}

- (void)transitionToIncomingState {
    [self.stateMachine transitionTo:self.incomingState];
}

- (void)transitionToOutgoingState {
    [self.stateMachine transitionTo:self.outgoingState];
}



#pragma mark - private
- (void)destroy {
    [self.sessionLifeCycleDelegate sessionDidfinish:self];
}

#pragma mark - getter
- (NSHashTable<id<JCallSessionDelegate>> *)delegates {
    if (!_delegates) {
        _delegates = [NSHashTable weakObjectsHashTable];
    }
    return _delegates;
}

- (JStateMachine *)stateMachine {
    if (!_stateMachine) {
        _stateMachine = [[JStateMachine alloc] initWithName:@"j_call"];
        [_stateMachine setInitialState:self.idleState];
        [_stateMachine start];
    }
    return _stateMachine;
}

- (JCallSuperState *)superState {
    if (!_superState) {
        _superState = [[JCallSuperState alloc] init];
        _superState.callSessionImpl = self;
    }
    return _superState;
}

- (JConnectedState *)connectedState {
    if (!_connectedState) {
        _connectedState = [[JConnectedState alloc] initWithName:@"connected" superState:self.superState];
        _connectedState.callSessionImpl = self;
    }
    return _connectedState;
}

- (JConnectingState *)connectingState {
    if (!_connectingState) {
        _connectingState = [[JConnectingState alloc] initWithName:@"connecting" superState:self.superState];
        _connectingState.callSessionImpl = self;
    }
    return _connectingState;
}

- (JIdleState *)idleState {
    if (!_idleState) {
        _idleState = [[JIdleState alloc] initWithName:@"idle" superState:self.superState];
        _idleState.callSessionImpl = self;
    }
    return _idleState;
}

- (JIncomingState *)incomingState {
    if (!_incomingState) {
        _incomingState = [[JIncomingState alloc] initWithName:@"incoming" superState:self.superState];
        _incomingState.callSessionImpl = self;
    }
    return _incomingState;
}

- (JOutgoingState *)outgoingState {
    if (!_outgoingState) {
        _outgoingState = [[JOutgoingState alloc] initWithName:@"outgoing" superState:self.superState];
        _outgoingState.callSessionImpl = self;
    }
    return _outgoingState;
}

- (NSMutableArray<JCallMember *> *)members {
    if (!_members) {
        _members = [NSMutableArray array];
    }
    return _members;
}

@synthesize callId;
@synthesize callStatus;
@synthesize cameraEnable;
@synthesize connectTime;
@synthesize finishReason;
@synthesize finishTime;
@synthesize inviter;
@synthesize isMultiCall;
@synthesize microphoneEnable;
@synthesize owner;
@synthesize members = _members;
@synthesize startTime;

@end
