//
//  JChatroomManager.m
//  JuggleIM
//
//  Created by Fei Li on 2024/8/13.
//

#import "JChatroomManager.h"
#import "JLogger.h"

@interface JChatroomManager ()
@property (nonatomic, strong) JIMCore *core;
@property (nonatomic, strong) NSHashTable <id<JChatroomDelegate>> *delegates;
@property (nonatomic, strong) NSMutableDictionary <NSString *, JCachedChatroomStatus *> *cachedChatroomDic;
@end

@implementation JChatroomManager

- (instancetype)initWithCore:(JIMCore *)core {
    self = [super init];
    if (self) {
        self.core = core;
    }
    return self;
}

- (void)addDelegate:(id<JChatroomDelegate>)delegate {
    dispatch_async(self.core.delegateQueue, ^{
        if (!delegate) {
            return;
        }
        [self.delegates addObject:delegate];
    });
}

- (void)joinChatroom:(NSString *)chatroomId {
    [self.core.webSocket joinChatroom:chatroomId
                              success:^(long long timestamp) {
        JLogI(@"CHRM-Join", @"success");
        [self changeStatus:JChatroomStatusJoined forChatroom:chatroomId];
        [self.core.webSocket syncChatroomMessagesWithTime:[self getSyncTimeForChatroom:chatroomId] chatroomId:chatroomId];
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JChatroomDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(chatroomDidJoin:)]) {
                    [obj chatroomDidJoin:chatroomId];
                }
            }];
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"CHRM-Join", @"error code is %ld", code);
        [self changeStatus:JChatroomStatusFailed forChatroom:chatroomId];
        //不做自动重新加入
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JChatroomDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(chatroomJoinFail:errorCode:)]) {
                    [obj chatroomJoinFail:chatroomId errorCode:(JErrorCode)code];
                }
            }];
        });
    }];
    [self changeStatus:JChatroomStatusJoining forChatroom:chatroomId];
}

- (void)getChatroomInfo:(NSString *)chatroomId option:(JChatroomInfoOptions *)option success:(void (^)(JChatroomInfo *))successBlock error:(void (^)(JErrorCode))errorBlock { 
    
}


- (void)quitChatroom:(NSString *)chatroomId { 
    [self.core.webSocket quitChatroom:chatroomId
                              success:^(long long timestamp) {
        JLogI(@"CHRM-quit", @"success");
        [self changeStatus:JChatroomStatusQuit forChatroom:chatroomId];
        [self.core.dbManager clearChatroomMessage:chatroomId];
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JChatroomDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(chatroomDidQuit:)]) {
                    [obj chatroomDidQuit:chatroomId];
                }
            }];
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"CHRM-quit", @"error code is %ld", code);
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JChatroomDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(chatroomQuitFail:errorCode:)]) {
                    [obj chatroomQuitFail:chatroomId errorCode:(JErrorCode)code];
                }
            }];
        });
    }];
}

- (JChatroomStatus)getStatusForChatroom:(NSString *)chatroomId {
    @synchronized (self) {
        JCachedChatroomStatus *cachedStatus = [self.cachedChatroomDic objectForKey:chatroomId];
        return cachedStatus.status;
    }
}

- (void)changeStatus:(JChatroomStatus)status forChatroom:(NSString *)chatroomId {
    @synchronized (self) {
        if (status == JChatroomStatusQuit
            || status == JChatroomStatusFailed) {
            [self.cachedChatroomDic removeObjectForKey:chatroomId];
            return;
        }
        JCachedChatroomStatus *cachedStatus = [self.cachedChatroomDic objectForKey:chatroomId];
        if (!cachedStatus) {
            cachedStatus = [[JCachedChatroomStatus alloc] init];
        }
        cachedStatus.status = status;
        [self.cachedChatroomDic setObject:cachedStatus forKey:chatroomId];
    }
}

- (long long)getSyncTimeForChatroom:(NSString *)chatroomId {
    @synchronized (self) {
        JCachedChatroomStatus *cachedStatus = [self.cachedChatroomDic objectForKey:chatroomId];
        return cachedStatus.syncTime;
    }
}

- (void)setSyncTime:(long long)syncTime forChatroom:(NSString *)chatroomId {
    @synchronized (self) {
        JCachedChatroomStatus *cachedStatus = [self.cachedChatroomDic objectForKey:chatroomId];
        if (!cachedStatus) {
            cachedStatus = [[JCachedChatroomStatus alloc] init];
        }
        cachedStatus.syncTime = syncTime;
        [self.cachedChatroomDic setObject:cachedStatus forKey:chatroomId];
    }
}

- (void)connectSuccess {
    NSArray *chatroomIds;
    @synchronized (self) {
        chatroomIds = [self.cachedChatroomDic.allKeys copy];
    }
    [self.core.dbManager clearChatroomMessageExclude:chatroomIds];
    if (chatroomIds.count == 0) {
        return;
    }
    [chatroomIds enumerateObjectsUsingBlock:^(NSString * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        [self joinChatroom:obj];
    }];
}

- (void)userDisconnect {
    @synchronized (self) {
        [self.cachedChatroomDic removeAllObjects];
    }
}

#pragma mark -- internal
- (NSHashTable<id<JChatroomDelegate>> *)delegates {
    if (!_delegates) {
        _delegates = [NSHashTable weakObjectsHashTable];
    }
    return _delegates;
}

- (NSMutableDictionary<NSString *, JCachedChatroomStatus *> *)cachedChatroomDic {
    if (!_cachedChatroomDic) {
        _cachedChatroomDic = [NSMutableDictionary dictionary];
    }
    return _cachedChatroomDic;
}

@end
