//
//  JCallSessionImpl.m
//  JuggleIM
//
//  Created by Fei Li on 2024/10/23.
//

#import "JCallSessionImpl.h"
#import "JStateMachine.h"
#import "JCallSuperState.h"
#import "JCallConnectedState.h"
#import "JCallConnectingState.h"
#import "JCallIdleState.h"
#import "JCallIncomingState.h"
#import "JCallOutgoingState.h"
#import "JLogger.h"
#import "JCallEvent.h"
#import "JCallEventUtil.h"
#import "JCallMediaManager.h"

@interface JCallSessionImpl ()
@property (nonatomic, strong) JStateMachine *stateMachine;
@property (nonatomic, strong) JCallSuperState *superState;
@property (nonatomic, strong) JCallConnectedState *connectedState;
@property (nonatomic, strong) JCallConnectingState *connectingState;
@property (nonatomic, strong) JCallIdleState *idleState;
@property (nonatomic, strong) JCallIncomingState *incomingState;
@property (nonatomic, strong) JCallOutgoingState *outgoingState;
@property (nonatomic, strong) NSHashTable <id<JCallSessionDelegate>> *delegates;
@property (nonatomic, copy, readwrite) NSMutableArray <JCallMember *> *members;
//@property (nonatomic, assign) BOOL cameraEnable;
//@property (nonatomic, assign) BOOL microphoneEnable;
@property (nonatomic, copy) NSMutableDictionary <NSString *, UIView *> *viewDic;
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
    [self event:JCallEventAccept userInfo:nil];
}


- (void)hangup { 
    [self event:JCallEventHangup userInfo:nil];
}

- (void)inviteUsers:(NSArray<NSString *> *)userIdList { 
    [self event:JCallEventInvite userInfo:@{@"userIdList":userIdList}];
}

- (void)enableCamera:(BOOL)isEnable {
    [[JCallMediaManager shared] enableCamera:isEnable];
}

- (void)setVideoView:(UIView *)view forUserId:(NSString *)userId {
    if (userId.length == 0) {
        return;
    }
    if (view) {
        [self.viewDic setObject:view forKey:userId];
    } else {
        [self.viewDic removeObjectForKey:userId];
    }
    if ([userId isEqualToString:JIM.shared.currentUserId]) {
        [[JCallMediaManager shared] startPreview:view];
    } else {
        if (self.callStatus == JCallStatusConnected) {
            [[JCallMediaManager shared] setVideoView:view roomId:self.callId userId:userId];
        }
    }
}

- (void)startPreview:(UIView *)view {
    [[JCallMediaManager shared] startPreview:view];
    if (view) {
        [self.viewDic setObject:view forKey:JIM.shared.currentUserId];
    } else {
        [self.viewDic removeObjectForKey:JIM.shared.currentUserId];
    }
}

- (void)muteMicrophone:(BOOL)isMute {
    [[JCallMediaManager shared] muteMicrophone:isMute];
}

- (void)muteSpeaker:(BOOL)isMute {
    [[JCallMediaManager shared] muteSpeaker:isMute];
}

- (void)setSpeakerEnable:(BOOL)isEnable {
    [[JCallMediaManager shared] setSpeakerEnable:isEnable];
}

- (void)useFrontCamera:(BOOL)isEnable {
    [[JCallMediaManager shared] useFrontCamera:isEnable];
}

#pragma mark - JCallSessionImpl
/// 下面方法都在状态机中调用
- (void)error:(JCallErrorCode)code {
    dispatch_async(self.core.delegateQueue, ^{
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JCallSessionDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj respondsToSelector:@selector(errorDidOccur:)]) {
                [obj errorDidOccur:code];
            }
        }];
    });
}

- (void)notifyReceiveCall {
    [self.sessionLifeCycleDelegate callDidReceive:self];
}

- (BOOL)notifyAcceptCall {
    return [self.sessionLifeCycleDelegate callDidAccept:self];
}

- (void)memberHangup:(NSString *)userId {
    [self removeMember:userId];
    if (!self.isMultiCall) {
        self.finishTime = [[NSDate date] timeIntervalSince1970] * 1000;
        if (self.callStatus == JCallStatusOutgoing) {
            self.finishReason = JCallFinishReasonOtherSideDecline;
        } else if (self.callStatus == JCallStatusIncoming) {
            self.finishReason = JCallFinishReasonOtherSideCancel;
        } else {
            self.finishReason = JCallFinishReasonOtherSideHangup;
        }
    } else {
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JCallSessionDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(usersDidLeave:)]) {
                    [obj usersDidLeave:@[userId]];
                }
            }];
        });
    }
}

- (void)membersQuit:(NSArray<NSString *> *)userIdList {
    for (NSString *userId in userIdList) {
        [self removeMember:userId];
    }
    if (!self.isMultiCall) {
        self.finishTime = [[NSDate date] timeIntervalSince1970] * 1000;
        self.finishReason = JCallFinishReasonOtherSideNoResponse;
    } else {
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JCallSessionDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(usersDidLeave:)]) {
                    [obj usersDidLeave:userIdList];
                }
            }];
        });
    }
}

- (void)memberAccept:(NSString *)userId {
    for (JCallMember *member in self.members) {
        if ([member.userInfo.userId isEqualToString:userId]) {
            member.callStatus = JCallStatusConnecting;
        }
    }
}

- (void)addMember:(JCallMember *)member {
    [self.members addObject:member];
}

- (void)membersInviteBySelf:(NSArray<NSString *> *)userIdList {
    NSMutableArray *resultList = [NSMutableArray array];
    for (NSString *userId in userIdList) {
        BOOL isExist = NO;
        for (JCallMember *member in self.members) {
            if ([userId isEqualToString:member.userInfo.userId]) {
                isExist = YES;
                break;
            }
        }
        if (!isExist) {
            JCallMember *newMember = [[JCallMember alloc] init];
            JUserInfo *userInfo = [JIM.shared.userInfoManager getUserInfo:userId];
            if (!userInfo) {
                userInfo = [[JUserInfo alloc] init];
                userInfo.userId = userId;
            }
            newMember.userInfo = userInfo;
            newMember.callStatus = JCallStatusIncoming;
            newMember.startTime = [[NSDate date] timeIntervalSince1970] * 1000;
            newMember.inviter = [JIM.shared.userInfoManager getUserInfo:self.core.userId];
            [self.members addObject:newMember];
            [resultList addObject:userId];
        }
    }
    if (resultList.count > 0) {
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JCallSessionDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(usersDidInvite:inviterId:)]) {
                    [obj usersDidInvite:resultList inviterId:self.core.userId];
                }
            }];
        });
    }
}

- (void)addInviteMembers:(NSArray<JUserInfo *> *)targetUsers
                 inviter:(JUserInfo *)inviter {
    NSMutableArray *userIdList = [NSMutableArray array];
    for (JUserInfo *userInfo in targetUsers) {
        if ([userInfo.userId isEqualToString:self.core.userId]) {
            continue;
        }
        BOOL isExist = NO;
        for (JCallMember *member in self.members) {
            if ([userInfo.userId isEqualToString:member.userInfo.userId]) {
                isExist = YES;
                break;
            }
        }
        if (!isExist) {
            JCallMember *newMember = [[JCallMember alloc] init];
            newMember.userInfo = userInfo;
            newMember.callStatus = JCallStatusIncoming;
            newMember.startTime = [[NSDate date] timeIntervalSince1970] * 1000;
            newMember.inviter = inviter;
            [self.members addObject:newMember];
            [userIdList addObject:userInfo.userId];
        }
    }
    if (userIdList.count > 0) {
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JCallSessionDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(usersDidInvite:inviterId:)]) {
                    [obj usersDidInvite:userIdList inviterId:inviter.userId];
                }
            }];
        });
    }
}

- (void)removeMember:(NSString *)userId {
    for (JCallMember *member in self.members) {
        if ([member.userInfo.userId isEqualToString:userId]) {
            [self.members removeObject:member];
            return;
        }
    }
}

- (void)membersConnected:(NSArray<NSString *> *)userIdList {
    for (NSString *userId in userIdList) {
        for (JCallMember *member in self.members) {
            if ([member.userInfo.userId isEqualToString:userId]) {
                member.callStatus = JCallStatusConnected;
                break;
            }
        }
    }
    dispatch_async(self.core.delegateQueue, ^{
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JCallSessionDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj respondsToSelector:@selector(usersDidConnect:)]) {
                [obj usersDidConnect:userIdList];
            }
        }];
    });
}

- (void)cameraEnable:(BOOL)enable userId:(NSString *)userId {
    dispatch_async(self.core.delegateQueue, ^{
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JCallSessionDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj respondsToSelector:@selector(userCamaraDidChange:userId:)]) {
                [obj userCamaraDidChange:enable userId:userId];
            }
        }];
    });
}

- (void)soundLevelUpdate:(NSDictionary<NSString *,NSNumber *> *)soundLevels {
    dispatch_async(self.core.delegateQueue, ^{
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JCallSessionDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj respondsToSelector:@selector(soundLevelDidUpdate:)]) {
                [obj soundLevelDidUpdate:soundLevels];
            }
        }];
    });
}

- (void)videoFirstFrameRender:(NSString *)userId {
    dispatch_async(self.core.delegateQueue, ^{
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JCallSessionDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj respondsToSelector:@selector(videoFirstFrameDidRender:)]) {
                [obj videoFirstFrameDidRender:userId];
            }
        }];
    });
}

#pragma mark - signal
- (void)signalInvite {
    NSMutableArray *targetIds = [NSMutableArray array];
    for (JCallMember *member in self.members) {
        [targetIds addObject:member.userInfo.userId];
    }
    [self signalInvite:targetIds];
}

- (void)signalInvite:(NSArray<NSString *> *)userIdList {
    [self.core.webSocket callInvite:self.callId
                        isMultiCall:self.isMultiCall
                          mediaType:self.mediaType
                       targetIdList:userIdList
                         engineType:(NSUInteger)self.engineType
                              extra:self.extra
                            success:^(NSString *token, NSString *url){
        JLogI(@"Call-Signal", @"send invite success");
        self.token = token;
        self.url = url;
        [self event:JCallEventInviteDone userInfo:@{@"userIdList":userIdList}];
    } error:^(JErrorCodeInternal code) {
        JLogE(@"Call-Signal", @"send invite error, code is %ld", code);
        [self event:JCallEventInviteFail userInfo:nil];
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
                            success:^(NSString * _Nonnull token, NSString *url) {
        JLogI(@"Call-Signal", @"send accept success");
        self.token = token;
        self.url = url;
        [self event:JCallEventAcceptDone userInfo:nil];
    } error:^(JErrorCodeInternal code) {
        JLogE(@"Call-Signal", @"send accept error, code is %ld", code);
        [self event:JCallEventAcceptFail userInfo:nil];
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

- (void)ping {
    [self.core.webSocket rtcPing:self.callId];
}

#pragma mark - media
- (void)mediaQuit {
    JLogI(@"Call-Media", @"media quit");
    [JCallMediaManager.shared stopPreview];
    [JCallMediaManager.shared leaveRoom:self.callId];
}

- (void)mediaJoin {
    JLogI(@"Call-Media", @"media join");
    [JCallMediaManager.shared joinRoom:self
                              complete:^(int errorCode, NSDictionary *data) {
        if (errorCode == 0) {
            JLogI(@"Call-Media", @"join room success");
            [self event:JCallEventJoinChannelDone userInfo:nil];
        } else {
            JLogE(@"Call-Media", @"join room error, code is %d", errorCode);
            [self event:JCallEventJoinChannelFail userInfo:@{@"code":@(errorCode)}];
        }
    }];
}

#pragma mark - fsm
- (void)event:(NSInteger)event userInfo:(id)userInfo {
    [self.stateMachine event:event
                        name:[JCallEventUtil nameOfEvent:event]
                    userInfo:userInfo];
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

#pragma mark - JCallMediaDelegate
- (UIView *)viewForUserId:(NSString *)userId {
    return self.viewDic[userId];
}

- (UIView *)viewForSelf {
    return self.viewDic[JIM.shared.currentUserId];
}

- (void)usersDidJoin:(NSArray<NSString *> *)userIdList {
    [self event:JCallEventParticipantJoinChannel userInfo:@{@"userIdList":userIdList}];
}

- (void)userCamaraDidChange:(BOOL)enable userId:(NSString *)userId {
    [self event:JCallEventParticipantEnableCamera userInfo:@{@"enable":@(enable), @"userId":userId}];
}

- (void)soundLevelDidUpdate:(NSDictionary<NSString *,NSNumber *> *)soundLevels {
    [self event:JCallEventSoundLevelUpdate userInfo:soundLevels];
}

- (void)videoFirstFrameDidRender:(NSString *)userId {
    [self event:JCallEventVideoFirstFrameRender userInfo:@{@"userId": userId}];
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

- (JCallConnectedState *)connectedState {
    if (!_connectedState) {
        _connectedState = [[JCallConnectedState alloc] initWithName:@"callConnected" superState:self.superState];
        _connectedState.callSessionImpl = self;
    }
    return _connectedState;
}

- (JCallConnectingState *)connectingState {
    if (!_connectingState) {
        _connectingState = [[JCallConnectingState alloc] initWithName:@"callConnecting" superState:self.superState];
        _connectingState.callSessionImpl = self;
    }
    return _connectingState;
}

- (JCallIdleState *)idleState {
    if (!_idleState) {
        _idleState = [[JCallIdleState alloc] initWithName:@"callIdle" superState:self.superState];
        _idleState.callSessionImpl = self;
    }
    return _idleState;
}

- (JCallIncomingState *)incomingState {
    if (!_incomingState) {
        _incomingState = [[JCallIncomingState alloc] initWithName:@"callIncoming" superState:self.superState];
        _incomingState.callSessionImpl = self;
    }
    return _incomingState;
}

- (JCallOutgoingState *)outgoingState {
    if (!_outgoingState) {
        _outgoingState = [[JCallOutgoingState alloc] initWithName:@"callOutgoing" superState:self.superState];
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

- (NSMutableDictionary<NSString *,UIView *> *)viewDic {
    if (!_viewDic) {
        _viewDic = [NSMutableDictionary dictionary];
    }
    return _viewDic;
}

- (JCallMember *)currentCallMember {
    JCallMember *current = [[JCallMember alloc] init];
    JUserInfo *userInfo = [JIM.shared.userInfoManager getUserInfo:JIM.shared.currentUserId];
    current.userInfo = userInfo;
    current.callStatus = self.callStatus;
    current.startTime = self.startTime;
    current.connectTime = self.connectTime;
    current.finishTime = self.finishTime;
    current.inviter = [JIM.shared.userInfoManager getUserInfo:self.inviterId];
    return current;
}

@synthesize callId;
@synthesize callStatus;
@synthesize connectTime;
@synthesize finishReason;
@synthesize finishTime;
@synthesize inviterId;
@synthesize isMultiCall;
@synthesize owner;
@synthesize members = _members;
@synthesize startTime;
@synthesize mediaType;
@synthesize extra;

@end
