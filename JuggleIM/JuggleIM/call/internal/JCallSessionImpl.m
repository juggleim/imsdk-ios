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
#import "JCallSignalManager.h"
#import "JLogger.h"
#import "JCallEvent.h"
#import "JCallMediaManager.h"

@interface JCallSessionImpl ()
@property (nonatomic, strong) JCallSignalManager *signalManager;
@property (nonatomic, strong) JCallMediaManager *mediaManager;
@property (nonatomic, strong) JStateMachine *stateMachine;
@property (nonatomic, strong) JCallSuperState *superState;
@property (nonatomic, strong) JConnectedState *connectedState;
@property (nonatomic, strong) JConnectingState *connectingState;
@property (nonatomic, strong) JIdleState *idleState;
@property (nonatomic, strong) JIncomingState *incomingState;
@property (nonatomic, strong) JOutgoingState *outgoingState;
@property (nonatomic, strong) NSHashTable <id<JCallSessionDelegate>> *delegates;
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


- (void)setMicrophoneMute:(BOOL)isMute { 
    
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

#pragma mark - signal
- (void)signalSingleInvite {
    NSMutableArray *targetIds = [NSMutableArray array];
    for (JCallMember *member in self.members) {
        [targetIds addObject:member.userInfo.userId];
    }
    [self.core.webSocket callInvite:self.callId
                        isMultiCall:NO
                       targetIdList:targetIds
                            success:^{
        JLogI(@"Call-Signal", @"send invite success");
    } error:^(JErrorCodeInternal code) {
        JLogE(@"Call-Signal", @"send invite error, code is %ld", code);
        [self.stateMachine event:JCallEventInviteFail userInfo:nil];
    }];
}

- (void)signalHangup {
    [self.core.webSocket callHangup:self.callId
                             userId:self.core.userId
                            success:^{
        JLogI(@"Call-Signal", @"send hangup success");
    } error:^(JErrorCodeInternal code) {
        JLogE(@"Call-Signal", @"send hangup error, code is %ld", code);
    }];
}

- (void)signalAccept {
    //TODO: 
//    [self.core.webSocket callAccept:self.callId]
}

#pragma mark - media
- (void)mediaQuit {
    //TODO: 
}

- (void)mediaJoin {
    [JCallMediaManager.shared joinRoom:self
                              complete:^(int errorCode, NSDictionary *data) {
        int i = 1;
    }];
}

#pragma mark - fsm
- (void)event:(NSInteger)event userInfo:(id)userInfo {
    [self.stateMachine event:event userInfo:userInfo];
}

- (void)transitionToConnectedState {
    [self.stateMachine transitionTo:self.connectedState];
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

- (JCallSignalManager *)signalManager {
    if (!_signalManager) {
        _signalManager = [[JCallSignalManager alloc] initWithCore:self.core];
    }
    return _signalManager;
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
@synthesize members;
@synthesize startTime;

@end
