//
//  JCallManager.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JCallManager.h"
#import "JUtility.h"
#import "JCallEvent.h"
#import "JCallSessionImpl.h"

@interface JCallManager () <JCallSessionLifeCycleDelegate, JWebSocketCallDelegate>
@property (nonatomic, strong) JIMCore *core;
@property (nonatomic, strong) NSMutableArray <JCallSessionImpl *> *callSessionList;
@property (nonatomic, strong) NSHashTable <id<JCallReceiveDelegate>> *callReceiveDelegates;
@end

@implementation JCallManager
- (instancetype)initWithCore:(JIMCore *)core {
    if (self = [super init]) {
        self.core = core;
        [self.core.webSocket setCallDelegate:self];
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

- (id<JCallSession>)startSingleCall:(NSString *)userId
                           delegate:(id<JCallSessionDelegate>)delegate {
    @synchronized (self) {
        if (self.callSessionList.count > 0) {
            dispatch_async(self.core.delegateQueue, ^{
                if ([delegate respondsToSelector:@selector(errorDidOccur:)]) {
                    [delegate errorDidOccur:JCallErrorCodeCallExist];
                }
            });
            return nil;
        }
    }
    
    NSString *callId = [JUtility getUUID];
    JCallSessionImpl *callSession = [self createCallSessionImpl:callId
                                                    isMultiCall:NO];
    callSession.owner = JIM.shared.currentUserId;
    NSMutableArray *participants = [NSMutableArray array];
    JCallMember *member = [[JCallMember alloc] init];
    member.userId = userId;
    member.callStatus = JCallStatusIncoming;
    [participants addObject:member];
    callSession.participants = participants;
    [callSession addDelegate:delegate];
    [callSession event:JCallEventInvite userInfo:nil];
    
    [self addCallSession:callSession];
    return callSession;
}

#pragma mark - JWebSocketCallDelegate
- (void)callDidInvite:(JUserInfo *)inviter room:(JRtcRoom *)room {
    JCallSessionImpl *callSession = [self getCallSessionImpl:room.roomId];
    if (!callSession) {
        callSession = [self createCallSessionImpl:room.roomId
                                                        isMultiCall:room.isMultiCall];
        callSession.owner = room.owner.userId;
        callSession.inviter = inviter.userId;
        [self addCallSession:callSession];
    }
    
    [callSession event:JCallEventReceiveInvite userInfo:nil];
    NSMutableDictionary *userDic = [NSMutableDictionary dictionary];
    [userDic setObject:inviter forKey:inviter.userId];
    [userDic setObject:room.owner forKey:room.owner.userId];
    [self.core.dbManager insertUserInfos:userDic.allValues];
}

#pragma mark - JCallSessionLifeCycleDelegate
- (void)sessionDidfinish:(JCallSessionImpl *)session {
    @synchronized (self) {
        if (session) {
            [self.callSessionList removeObject:session];
        }
    }
}

- (void)callDidReceive:(JCallSessionImpl *)session {
    dispatch_async(self.core.delegateQueue, ^{
        [self.callReceiveDelegates.allObjects enumerateObjectsUsingBlock:^(id<JCallReceiveDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj respondsToSelector:@selector(callDidReceive:)]) {
                [obj callDidReceive:session];
            }
        }];
    });
}

#pragma mark - internal
- (JCallSessionImpl *)createCallSessionImpl:(NSString *)callId
                                isMultiCall:(BOOL)isMultiCall {
    JCallSessionImpl *callSession = [[JCallSessionImpl alloc] init];
    callSession.callId = callId;
    callSession.isMultiCall = isMultiCall;
    callSession.cameraEnable = NO;
    callSession.microphoneEnable = YES;
    callSession.startTime = [[NSDate date] timeIntervalSince1970] * 1000;
    callSession.core = self.core;
    callSession.sessionLifeCycleDelegate = self;
    return callSession;
}

- (JCallSessionImpl *)getCallSessionImpl:(NSString *)callId {
    if (callId.length == 0) {
        return nil;
    }
    @synchronized (self) {
        for (JCallSessionImpl *session in self.callSessionList) {
            if ([session.callId isEqualToString:callId]) {
                return session;
            }
        }
    }
    return nil;
}

- (void)addCallSession:(JCallSessionImpl *)callSession {
    @synchronized (self) {
        [self.callSessionList addObject:callSession];
    }
}

- (NSHashTable<id<JCallReceiveDelegate>> *)callReceiveDelegates {
    if (!_callReceiveDelegates) {
        _callReceiveDelegates = [NSHashTable weakObjectsHashTable];
    }
    return _callReceiveDelegates;
}

- (NSMutableArray<JCallSessionImpl *> *)callSessionList {
    if (!_callSessionList) {
        _callSessionList = [NSMutableArray array];
    }
    return _callSessionList;
}

@end
