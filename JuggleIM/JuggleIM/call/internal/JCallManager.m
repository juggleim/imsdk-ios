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

@interface JCallManager () <JCallSessionLifeCycleDelegate>
@property (nonatomic, strong) JIMCore *core;
@property (nonatomic, strong) NSMutableArray <JCallSessionImpl *> *callSessionList;
@property (nonatomic, strong) NSHashTable <id<JCallReceiveDelegate>> *callReceiveDelegates;
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
    JCallSessionImpl *callSession = [[JCallSessionImpl alloc] init];
    callSession.callId = callId;
    callSession.isMultiCall = NO;
    callSession.cameraEnable = NO;
    callSession.microphoneEnable = YES;
    callSession.startTime = [[NSDate date] timeIntervalSince1970] * 1000;
    callSession.owner = JIM.shared.currentUserId;
    callSession.core = self.core;
    NSMutableArray *participants = [NSMutableArray array];
    JCallMember *member = [[JCallMember alloc] init];
    member.userId = userId;
    member.callStatus = JCallStatusIncoming;
    [participants addObject:member];
    callSession.participants = participants;
    [callSession addDelegate:delegate];
    callSession.sessionLifeCycleDelegate = self;
    [callSession event:JCallEventInvite userInfo:nil];
    
    [self addCallSession:callSession];
    return callSession;
}

#pragma mark - JCallSessionLifeCycleDelegate
- (void)sessionDidfinish:(JCallSessionImpl *)session {
    @synchronized (self) {
        if (session) {
            [self.callSessionList removeObject:session];
        }
    }
}

#pragma mark - internal
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
