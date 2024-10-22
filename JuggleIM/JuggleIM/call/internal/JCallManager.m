//
//  JCallManager.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JCallManager.h"
#import "JCallSignalManager.h"
#import "JCallMediaEngine.h"
#import "JStateMachine.h"
#import "JCallSuperState.h"
#import "JConnectedState.h"
#import "JConnectingState.h"
#import "JIdleState.h"
#import "JIncomingState.h"
#import "JOutgoingState.h"

@interface JCallManager ()
@property (nonatomic, strong) JIMCore *core;
@property (nonatomic, strong) JCallSignalManager *signalManager;
@property (nonatomic, strong) JCallMediaEngine *mediaEngine;
@property (nonatomic, strong) NSArray <JCallSession *> *callSessionList;
@property (nonatomic, strong) NSHashTable <id<JCallReceiveDelegate>> *callReceiveDelegates;
@property (nonatomic, strong) JStateMachine *stateMachine;
@property (nonatomic, strong) JCallSuperState *superState;
@property (nonatomic, strong) JConnectedState *connectedState;
@property (nonatomic, strong) JConnectingState *connectingState;
@property (nonatomic, strong) JIdleState *idleState;
@property (nonatomic, strong) JIncomingState *incomingState;
@property (nonatomic, strong) JOutgoingState *outgoingState;
@end

@implementation JCallManager
- (instancetype)initWithCore:(JIMCore *)core {
    if (self = [super init]) {
        self.core = core;
    }
    return self;
}

- (void)addReceiveDelegate:(id<JCallReceiveDelegate>)receiveDelegate {
    dispatch_async(self.core.delegateQueue, ^{
        if (!receiveDelegate) {
            return;
        }
        [self.callReceiveDelegates addObject:receiveDelegate];
    });
}

//- (JCallSession *)startSingleCall:(NSString *)userId delegate:(id<JCallSessionDelegate>)delegate { 
//    <#code#>
//}

#pragma mark - internal
- (NSHashTable<id<JCallReceiveDelegate>> *)callReceiveDelegates {
    if (!_callReceiveDelegates) {
        _callReceiveDelegates = [NSHashTable weakObjectsHashTable];
    }
    return _callReceiveDelegates;
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
    }
    return _stateMachine;
}

- (JCallSuperState *)superState {
    if (!_superState) {
        _superState = [[JCallSuperState alloc] init];
        _superState.callManager = self;
    }
    return _superState;
}

- (JConnectedState *)connectedState {
    if (!_connectedState) {
        _connectedState = [[JConnectedState alloc] initWithName:@"connected" superState:self.superState];
        _connectedState.callManager = self;
    }
    return _connectedState;
}

- (JConnectingState *)connectingState {
    if (!_connectingState) {
        _connectingState = [[JConnectingState alloc] initWithName:@"connecting" superState:self.superState];
        _connectingState.callManager = self;
    }
    return _connectingState;
}

- (JIdleState *)idleState {
    if (!_idleState) {
        _idleState = [[JIdleState alloc] initWithName:@"idle" superState:self.superState];
        _idleState.callManager = self;
    }
    return _idleState;
}

- (JIncomingState *)incomingState {
    if (!_incomingState) {
        _incomingState = [[JIncomingState alloc] initWithName:@"incoming" superState:self.superState];
        _incomingState.callManager = self;
    }
    return _incomingState;
}

- (JOutgoingState *)outgoingState {
    if (!_outgoingState) {
        _outgoingState = [[JOutgoingState alloc] initWithName:@"outgoing" superState:self.superState];
        _outgoingState.callManager = self;
    }
    return _outgoingState;
}

@end
