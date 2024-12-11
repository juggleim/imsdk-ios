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
#import "JCallMediaManager.h"
#import "JCallInternalConst.h"
#import "JLogger.h"

@interface JCallManager () <JCallSessionLifeCycleDelegate, JWebSocketCallDelegate>
@property (nonatomic, strong) JIMCore *core;
@property (nonatomic, strong) NSMutableArray <JCallSessionImpl *> *callSessionList;
@property (nonatomic, strong) NSHashTable <id<JCallReceiveDelegate>> *callReceiveDelegates;
@property (nonatomic, assign) JCallEngineType engineType;
@end

@implementation JCallManager
- (void)initZegoEngineWith:(int)appId appSign:(NSString *)appSign {
    [JCallMediaManager.shared initZegoEngineWith:appId appSign:appSign];
    self.engineType = JCallEngineTypeZego;
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
    return [self startSingleCall:userId
                       mediaType:JCallMediaTypeVoice
                        delegate:delegate];
}

- (id<JCallSession>)startSingleCall:(NSString *)userId
                          mediaType:(JCallMediaType)mediaType
                           delegate:(id<JCallSessionDelegate>)delegate {
    if (userId.length == 0) {
        dispatch_async(self.core.delegateQueue, ^{
            if ([delegate respondsToSelector:@selector(errorDidOccur:)]) {
                [delegate errorDidOccur:JCallErrorCodeInvalidParameter];
            }
        });
        return nil;
    }
    return [self startCall:@[userId]
                   isMulti:NO
                 mediaType:mediaType
                  delegate:delegate];
}

- (id<JCallSession>)startMultiCall:(NSArray<NSString *> *)userIdList
                         mediaType:(JCallMediaType)mediaType
                          delegate:(id<JCallSessionDelegate>)delegate {
    if (userIdList.count == 0) {
        dispatch_async(self.core.delegateQueue, ^{
            if ([delegate respondsToSelector:@selector(errorDidOccur:)]) {
                [delegate errorDidOccur:JCallErrorCodeInvalidParameter];
            }
        });
        return nil;
    }
    return [self startCall:userIdList
                   isMulti:YES
                 mediaType:mediaType
                  delegate:delegate];
}

- (instancetype)initWithCore:(JIMCore *)core {
    if (self = [super init]) {
        self.core = core;
        [self.core.webSocket setCallDelegate:self];
    }
    return self;
}

- (void)connectSuccess {
    [self.core.webSocket queryCallRooms:self.core.userId
                                success:^(NSArray<JRtcRoom *> * _Nonnull rooms) {
        JLogI(@"Call-Qry", @"query call rooms, count is %lu", (unsigned long)rooms.count);
        for (JRtcRoom *room in rooms) {
            if (room.deviceId.length == 0
                || [room.deviceId isEqualToString:[JUtility getDeviceId]]) {
                JCallStatus callStatus = room.callStatus;
                [self.core.webSocket queryCallRoom:room.roomId
                                           success:^(NSArray<JRtcRoom *> * _Nonnull singleRooms) {
                    JLogI(@"Call-Qry", @"query call room success");
                    if (singleRooms.count == 0) {
                        JLogW(@"Call-Qry", @"query call room count is 0");
                        return;
                    }
                    JRtcRoom *singleRoom = singleRooms[0];
                    JCallSessionImpl *callSession = [self createCallSessionImpl:singleRoom.roomId isMultiCall:singleRoom.isMultiCall];
                    callSession.owner = singleRoom.owner.userId;
                    NSMutableDictionary *userDic = [NSMutableDictionary dictionary];
                    for (JCallMember *member in singleRoom.members) {
                        [userDic setObject:member.userInfo forKey:member.userInfo.userId];
                        if (member.userInfo.userId != self.core.userId) {
                            [callSession addMember:member];
                        }
                    }
                    [self.core.dbManager insertUserInfos:userDic.allValues];
                    [self initCallSession:callSession
                           withCallStatus:callStatus];
                } error:^(JErrorCodeInternal code) {
                    JLogE(@"Call-Qry", @"query call room error, code is %ld", code);
                }];
                break;
            }
        }
    } error:^(JErrorCodeInternal code) {
        JLogE(@"Call-Qry", @"query call rooms error, code is %ld", code);
    }];
}

#pragma mark - JWebSocketCallDelegate
- (void)callDidInvite:(JRtcRoom *)room
              inviter:(JUserInfo *)inviter
          targetUsers:(NSArray<JUserInfo *> *)targetUsers {    
    NSMutableDictionary *userDic = [NSMutableDictionary dictionary];
    for (JCallMember *member in room.members) {
        [userDic setObject:member.userInfo forKey:member.userInfo.userId];
    }
    [self.core.dbManager insertUserInfos:userDic.allValues];
    
    JCallSessionImpl *callSession = [self getCallSessionImpl:room.roomId];
    if (callSession) {
        [callSession event:JCallEventReceiveInviteOthers userInfo:@{@"inviter":inviter, @"targetUsers":targetUsers}];
    } else {
        BOOL isInvite = NO;
        for (JUserInfo *userInfo in targetUsers) {
            [userDic setObject:userInfo forKey:userInfo.userId];
            if ([userInfo.userId isEqualToString:self.core.userId]) {
                isInvite = YES;
            }
        }
        
        if (isInvite) {
            callSession = [self createCallSessionImpl:room.roomId
                                          isMultiCall:room.isMultiCall];
            callSession.owner = room.owner.userId;
            callSession.inviterId = inviter.userId;
            callSession.mediaType = room.mediaType;
            if (callSession.mediaType == JCallMediaTypeVideo) {
                [[JCallMediaManager shared] enableCamera:YES];
            } else {
                [[JCallMediaManager shared] enableCamera:NO];
            }
            for (JCallMember *member in room.members) {
                if (![member.userInfo.userId isEqualToString:self.core.userId]) {
                    [callSession addMember:member];
                }
            }
            [self addCallSession:callSession];
            [callSession event:JCallEventReceiveInvite userInfo:nil];
        }
    }
}

- (void)callDidHangup:(JRtcRoom *)room
                 user:(JUserInfo *)user {
    NSMutableDictionary *userDic = [NSMutableDictionary dictionary];
    [userDic setObject:user forKey:user.userId];
    [self.core.dbManager insertUserInfos:userDic.allValues];
    
    JCallSessionImpl *callSession = [self getCallSessionImpl:room.roomId];
    if (!callSession) {
        return;
    }
    NSDictionary *userInfo = @{@"userId" : user.userId};
    [callSession event:JCallEventReceiveHangup userInfo:userInfo];
}

- (void)callDidQuit:(JRtcRoom *)room members:(NSArray<JCallMember *> *)members {
    NSMutableDictionary *userDic = [NSMutableDictionary dictionary];
    for (JCallMember *member in members) {
        [userDic setObject:member.userInfo forKey:member.userInfo.userId];
    }
    [self.core.dbManager insertUserInfos:userDic.allValues];
    
    JCallSessionImpl *callSession = [self getCallSessionImpl:room.roomId];
    if (!callSession) {
        return;
    }
    NSDictionary *userInfo = @{@"userIdList" : userDic.allKeys};
    [callSession event:JCallEventReceiveQuit userInfo:userInfo];
}

- (void)callDidAccept:(JRtcRoom *)room
                 user:(JUserInfo *)user {
    NSMutableDictionary *userDic = [NSMutableDictionary dictionary];
    [userDic setObject:user forKey:user.userId];
    [self.core.dbManager insertUserInfos:userDic.allValues];
    
    JCallSessionImpl *callSession = [self getCallSessionImpl:room.roomId];
    if (!callSession) {
        return;
    }
    NSDictionary *userInfo = @{@"userId" : user.userId};
    [callSession event:JCallEventReceiveAccept userInfo:userInfo];
}

- (void)roomDidDestroy:(JRtcRoom *)room {
    JCallSessionImpl *callSession = [self getCallSessionImpl:room.roomId];
    if (!callSession) {
        return;
    }
    [callSession event:JCallEventRoomDestroy userInfo:nil];
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

- (BOOL)callDidAccept:(JCallSessionImpl *)session {
    __block BOOL needHangupOther = NO;
    @synchronized (self) {
        for (JCallSessionImpl *loopSession in self.callSessionList) {
            if (![loopSession.callId isEqualToString:session.callId]) {
                [loopSession event:JCallEventHangup userInfo:nil];
                needHangupOther = YES;
            }
        }
    }
    return needHangupOther;
}

#pragma mark - internal
- (id<JCallSession>)startCall:(NSArray<NSString *> *)userIdList
                      isMulti:(BOOL)isMulti
                    mediaType:(JCallMediaType)mediaType
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
    
    if (userIdList.count == 0) {
        dispatch_async(self.core.delegateQueue, ^{
            if ([delegate respondsToSelector:@selector(errorDidOccur:)]) {
                [delegate errorDidOccur:JCallErrorCodeInvalidParameter];
            }
        });
        return nil;
    }
    
    NSString *callId = [JUtility getUUID];
    JCallSessionImpl *callSession = [self createCallSessionImpl:callId
                                                    isMultiCall:isMulti];
    callSession.owner = JIM.shared.currentUserId;
    callSession.mediaType = mediaType;
    if (callSession.mediaType == JCallMediaTypeVideo) {
        [[JCallMediaManager shared] enableCamera:YES];
    } else {
        [[JCallMediaManager shared] enableCamera:NO];
    }
    [userIdList enumerateObjectsUsingBlock:^(NSString * _Nonnull userId, NSUInteger idx, BOOL * _Nonnull stop) {
        JCallMember *member = [[JCallMember alloc] init];
        JUserInfo *userInfo = [[JUserInfo alloc] init];
        userInfo.userId = userId;
        member.userInfo = userInfo;
        member.callStatus = JCallStatusIncoming;
        member.startTime = [[NSDate date] timeIntervalSince1970] * 1000;
        JUserInfo *inviter = [JIM.shared.userInfoManager getUserInfo:self.core.userId];
        member.inviter = inviter;
        [callSession addMember:member];
    }];
    
    [callSession addDelegate:delegate];
    [callSession event:JCallEventInvite userInfo:nil];
    
    [self addCallSession:callSession];
    return callSession;
}

- (void)initCallSession:(JCallSessionImpl *)callSession
         withCallStatus:(JCallStatus)callStatus {
    if (callStatus == JCallStatusIncoming) {
        [self addCallSession:callSession];
        [callSession event:JCallEventReceiveInvite userInfo:nil];
    }
}

- (JCallSessionImpl *)createCallSessionImpl:(NSString *)callId
                                isMultiCall:(BOOL)isMultiCall {
    JCallSessionImpl *callSession = [[JCallSessionImpl alloc] init];
    callSession.callId = callId;
    callSession.isMultiCall = isMultiCall;
    callSession.engineType = self.engineType;
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
