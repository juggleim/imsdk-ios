//
//  JChatroomManager.m
//  JuggleIM
//
//  Created by Fei Li on 2024/8/13.
//

#import "JChatroomManager.h"
#import "JLogger.h"
#import "JIntervalGenerator.h"

@interface JChatroomManager () <JWebSocketChatroomDelegate>
@property (nonatomic, strong) JIMCore *core;
@property (nonatomic, strong) NSHashTable <id<JChatroomDelegate>> *delegates;
@property (nonatomic, strong) NSHashTable <id<JChatroomAttributesDelegate>> *attributesDelegates;
@property (nonatomic, strong) NSMutableDictionary <NSString *, JCachedChatroom *> *cachedChatroomDic;
@property (nonatomic, strong) JIntervalGenerator *intervalGenerator;
@property (nonatomic, strong) NSTimer *attrRetryTimer;
@end

@implementation JChatroomManager

- (instancetype)initWithCore:(JIMCore *)core {
    self = [super init];
    if (self) {
        self.core = core;
        [self.core.webSocket setChatroomDelegate:self];
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

- (void)addAttributesDelegate:(id<JChatroomAttributesDelegate>)delegate {
    dispatch_async(self.core.delegateQueue, ^{
        if (!delegate) {
            return;
        }
        [self.attributesDelegates addObject:delegate];
    });
}

- (void)joinChatroom:(NSString *)chatroomId {
    [self joinChatroom:chatroomId prevMessageCount:-1];
}

- (void)joinChatroom:(NSString *)chatroomId prevMessageCount:(int)count {
    [self joinChatroom:chatroomId prevMessageCount:count isAutoCreate:NO];
}

- (void)joinChatroom:(NSString *)chatroomId prevMessageCount:(int)count isAutoCreate:(BOOL)isAutoCreate {
    [self changeStatus:JChatroomStatusJoining forChatroom:chatroomId];
    [self setPrevMessageCount:count forChatroom:chatroomId];
    [self.core.webSocket joinChatroom:chatroomId
                         isAutoCreate:isAutoCreate
                              success:^(long long timestamp) {
        JLogI(@"CHRM-Join", @"success");
        [self changeStatus:JChatroomStatusJoined forChatroom:chatroomId];
        //count 为 0，timestamp 也为 0，服务端永远同步不下来消息
        long long existedSyncTime = [self getSyncTimeForChatroom:chatroomId];
        if (count == 0 && existedSyncTime == 0) {
            [self setSyncTime:timestamp forChatroom:chatroomId];
        }
        [self syncChatroomAttr:chatroomId time:[self getAttrSyncTimeForChatroom:chatroomId]];
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
}

- (void)getChatroomInfo:(NSString *)chatroomId option:(JChatroomInfoOptions *)option success:(void (^)(JChatroomInfo *))successBlock error:(void (^)(JErrorCode))errorBlock { 
    
}


- (void)quitChatroom:(NSString *)chatroomId { 
    [self.core.webSocket quitChatroom:chatroomId
                              success:^(long long timestamp) {
        JLogI(@"CHRM-Quit", @"success");
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

- (void)setAttributes:(NSDictionary <NSString *, NSString *> *)attributes
          forChatroom:(NSString *)chatroomId
             complete:(void (^)(JErrorCode, NSDictionary<NSString *,NSNumber *> *))completeBlock {
    if (attributes.count == 0) {
        JLogE(@"CHRM-SetAttr", @"count is 0");
        dispatch_async(self.core.delegateQueue, ^{
            if (completeBlock) {
                completeBlock(JErrorCodeInvalidParam, nil);
            }
        });
        return;
    }
    __weak typeof(self) ws = self;
    [self.core.webSocket setAttributes:attributes
                           forChatroom:chatroomId
                              complete:^(JErrorCodeInternal code, NSArray<JChatroomAttributeItem *> * _Nonnull items) {
        NSMutableDictionary *resultDic = [NSMutableDictionary dictionary];
        if (code == JErrorCodeInternalNone) {
            [items enumerateObjectsUsingBlock:^(JChatroomAttributeItem * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if (obj.code != 0)  {
                    [resultDic setObject:@(obj.code) forKey:obj.key];
                }
            }];
            if (resultDic.count == 0) {
                JLogI(@"CHRM-SetAttr", @"success");
                dispatch_async(ws.core.delegateQueue, ^{
                    if (completeBlock) {
                        completeBlock(JErrorCodeNone, nil);
                    }
                });
            } else {
                JLogE(@"CHRM-SetAttr", @"partial fail");
                dispatch_async(ws.core.delegateQueue, ^{
                    if (completeBlock) {
                        completeBlock(JErrorCodeChatroomBatchSetAttributeFail, [resultDic copy]);
                    }
                });
            }
        } else {
            JLogE(@"CHRM-SetAttr", @"fail, code is %ld", code);
            [attributes enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull key, NSString * _Nonnull obj, BOOL * _Nonnull stop) {
                [resultDic setObject:@(code) forKey:key];
            }];
            dispatch_async(ws.core.delegateQueue, ^{
                if (completeBlock) {
                    completeBlock((JErrorCode)code, [resultDic copy]);
                }
            });
        }
    }];
}

- (void)removeAttributes:(NSArray<NSString *> *)keys
             forChatroom:(NSString *)chatroomId
                complete:(void (^)(JErrorCode, NSDictionary<NSString *,NSNumber *> *))completeBlock {
    if (keys.count == 0) {
        JLogE(@"CHRM-RmAttr", @"count is 0");
        dispatch_async(self.core.delegateQueue, ^{
            if (completeBlock) {
                completeBlock(JErrorCodeInvalidParam, nil);
            }
        });
        return;
    }
    __weak typeof(self) ws = self;
    [self.core.webSocket removeAttributes:keys
                              forChatroom:chatroomId
                                 complete:^(JErrorCodeInternal code, NSArray<JChatroomAttributeItem *> * _Nonnull items) {
        NSMutableDictionary *resultDic = [NSMutableDictionary dictionary];
        if (code == JErrorCodeInternalNone) {
            [items enumerateObjectsUsingBlock:^(JChatroomAttributeItem * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if (obj.code != 0)  {
                    [resultDic setObject:@(obj.code) forKey:obj.key];
                }
            }];
            if (resultDic.count == 0) {
                JLogI(@"CHRM-RmAttr", @"success");
                dispatch_async(ws.core.delegateQueue, ^{
                    if (completeBlock) {
                        completeBlock(JErrorCodeNone, nil);
                    }
                });
            } else {
                JLogE(@"CHRM-RmAttr", @"partial fail");
                dispatch_async(ws.core.delegateQueue, ^{
                    if (completeBlock) {
                        completeBlock(JErrorCodeChatroomBatchSetAttributeFail, [resultDic copy]);
                    }
                });
            }
        } else {
            JLogE(@"CHRM-RmAttr", @"fail, code is %ld", code);
            [keys enumerateObjectsUsingBlock:^(NSString * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                [resultDic setObject:@(code) forKey:obj];
            }];
            dispatch_async(ws.core.delegateQueue, ^{
                if (completeBlock) {
                    completeBlock((JErrorCode)code, [resultDic copy]);
                }
            });
        }
    }];
}

- (void)getAllAttributesFromChatroom:(NSString *)chatroomId complete:(void (^)(JErrorCode, NSDictionary<NSString *,NSString *> *))completeBlock { 
    @synchronized (self) {
        JCachedChatroom *cachedChatroom = [self.cachedChatroomDic objectForKey:chatroomId];
        if (!cachedChatroom) {
            dispatch_async(self.core.delegateQueue, ^{
                if (completeBlock) {
                    completeBlock(JErrorCodeNotChatroomMember, nil);
                }
            });
            return;
        }
        NSDictionary <NSString *, NSString *> *attributes = [cachedChatroom.attributes copy];
        dispatch_async(self.core.delegateQueue, ^{
            if (completeBlock) {
                completeBlock(JErrorCodeNone, attributes);
            }
        });
    }
}

- (void)changeStatus:(JChatroomStatus)status forChatroom:(NSString *)chatroomId {
    @synchronized (self) {
        if (status == JChatroomStatusQuit
            || status == JChatroomStatusFailed) {
            [self.cachedChatroomDic removeObjectForKey:chatroomId];
            return;
        }
        JCachedChatroom *cachedChatroom = [self.cachedChatroomDic objectForKey:chatroomId];
        if (!cachedChatroom) {
            cachedChatroom = [[JCachedChatroom alloc] init];
        }
        cachedChatroom.status = status;
        [self.cachedChatroomDic setObject:cachedChatroom forKey:chatroomId];
    }
}

- (long long)getSyncTimeForChatroom:(NSString *)chatroomId {
    @synchronized (self) {
        JCachedChatroom *cachedChatroom = [self.cachedChatroomDic objectForKey:chatroomId];
        return cachedChatroom.syncTime;
    }
}

- (long long)getAttrSyncTimeForChatroom:(NSString *)chatroomId {
    @synchronized (self) {
        JCachedChatroom *cachedChatroom = [self.cachedChatroomDic objectForKey:chatroomId];
        return cachedChatroom.attrSyncTime;
    }
}

- (void)setSyncTime:(long long)syncTime forChatroom:(NSString *)chatroomId {
    @synchronized (self) {
        JCachedChatroom *cachedChatroom = [self.cachedChatroomDic objectForKey:chatroomId];
        if (!cachedChatroom) {
            return;
        }
        if (syncTime > cachedChatroom.syncTime) {
            cachedChatroom.syncTime = syncTime;
            [self.cachedChatroomDic setObject:cachedChatroom forKey:chatroomId];
        }
    }
}

- (void)setAttrSyncTime:(long long)syncTime forChatroom:(NSString *)chatroomId {
    @synchronized (self) {
        JCachedChatroom *cachedChatroom = [self.cachedChatroomDic objectForKey:chatroomId];
        if (!cachedChatroom) {
            return;
        }
        if (syncTime > cachedChatroom.attrSyncTime) {
            cachedChatroom.attrSyncTime = syncTime;
            [self.cachedChatroomDic setObject:cachedChatroom forKey:chatroomId];
        }
    }
}

- (int)getPrevMessageCountForChatroom:(NSString *)chatroomId {
    @synchronized (self) {
        JCachedChatroom *cachedChatroom = [self.cachedChatroomDic objectForKey:chatroomId];
        return cachedChatroom.prevMessageCount;
    }
}

- (void)setPrevMessageCount:(int)count forChatroom:(NSString *)chatroomId {
    @synchronized (self) {
        JCachedChatroom *cachedChatroom = [self.cachedChatroomDic objectForKey:chatroomId];
        if (!cachedChatroom) {
            return;
        }
        cachedChatroom.prevMessageCount = count;
        [self.cachedChatroomDic setObject:cachedChatroom forKey:chatroomId];
    }
}

- (BOOL)isChatroomAvailable:(NSString *)chatroomId {
    @synchronized (self) {
        JCachedChatroom *cachedChatroom = [self.cachedChatroomDic objectForKey:chatroomId];
        if (cachedChatroom) {
            return YES;
        } else {
            return NO;
        }
    }
}

- (void)updateAttributes:(NSDictionary <NSString *, NSString *> *)attributes
             forChatroom:(NSString *)chatroomId
                isRemove:(BOOL)isRemove {
    @synchronized (self) {
        JCachedChatroom *cachedChatroom = [self.cachedChatroomDic objectForKey:chatroomId];
        if (!cachedChatroom) {
            return;
        }
        NSMutableDictionary *cachedAttributes;
        if (cachedChatroom.attributes) {
            cachedAttributes = [cachedChatroom.attributes mutableCopy];
        } else {
            cachedAttributes = [NSMutableDictionary dictionary];
        }
        [attributes enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull key, NSString * _Nonnull obj, BOOL * _Nonnull stop) {
            if (isRemove) {
                [cachedAttributes removeObjectForKey:key];
            } else {
                [cachedAttributes setObject:obj forKey:key];
            }
        }];
        cachedChatroom.attributes = [cachedAttributes copy];
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

#pragma mark -- JWebSocketChatroomDelegate
- (void)syncChatroomAttrNotify:(nonnull NSString *)chatroomId time:(long long)syncTime {
    if (![self isChatroomAvailable:chatroomId]) {
        return;
    }
    
    long long cachedSyncTime = [self getAttrSyncTimeForChatroom:chatroomId];
    if (syncTime > cachedSyncTime) {
        [self syncChatroomAttr:chatroomId
                          time:cachedSyncTime];
    }
}

- (void)attributesDidSync:(NSArray<JChatroomAttributeItem *> *)items
              forChatroom:(nonnull NSString *)chatroomId
                     code:(JErrorCodeInternal)code {
    JLogI(@"CHRM-AttrSync", @"code is %ld, count is %ld", code, items.count);
    if (code != JErrorCodeInternalNone) {
        dispatch_async(dispatch_get_main_queue(), ^{
            if (self.attrRetryTimer) {
                return;
            }
            self.attrRetryTimer = [NSTimer scheduledTimerWithTimeInterval:[self.intervalGenerator getNextInterval] target:self selector:@selector(attrRetryTimerFired) userInfo:chatroomId repeats:NO];
        });
        return;
    }
    
    [self.intervalGenerator reset];
    if (items.count == 0) {
        return;
    }
    NSMutableDictionary *updateDic = [NSMutableDictionary dictionary];
    NSMutableDictionary *deleteDic = [NSMutableDictionary dictionary];
    __block long long syncTime = 0;
    [items enumerateObjectsUsingBlock:^(JChatroomAttributeItem * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if (obj.timestamp > syncTime) {
            syncTime = obj.timestamp;
        }
        if (obj.type == JChatroomAttrOptType_Update) {
            [updateDic setObject:obj.value forKey:obj.key];
        } else if (obj.type == JChatroomAttrOptType_Delete) {
            [deleteDic setObject:obj.value forKey:obj.key];
        }
    }];
    if (syncTime > 0) {
        [self setAttrSyncTime:syncTime forChatroom:chatroomId];
    }
    if (updateDic.count > 0) {
        [self updateAttributes:updateDic
                   forChatroom:chatroomId
                      isRemove:NO];
        dispatch_async(self.core.delegateQueue, ^{
            [self.attributesDelegates.allObjects enumerateObjectsUsingBlock:^(id<JChatroomAttributesDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(attributesDidUpdate:forChatroom:)]) {
                    [obj attributesDidUpdate:[updateDic copy] forChatroom:chatroomId];
                }
            }];
        });
    }
    if (deleteDic.count > 0) {
        [self updateAttributes:updateDic
                   forChatroom:chatroomId
                      isRemove:YES];
        dispatch_async(self.core.delegateQueue, ^{
            [self.attributesDelegates.allObjects enumerateObjectsUsingBlock:^(id<JChatroomAttributesDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(attributesDidDelete:forChatroom:)]) {
                    [obj attributesDidDelete:[deleteDic copy] forChatroom:chatroomId];
                }
            }];
        });
    }
}

- (void)chatroomDidDestroy:(NSString *)chatroomId {
    [self changeStatus:JChatroomStatusQuit forChatroom:chatroomId];
    dispatch_async(self.core.delegateQueue, ^{
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JChatroomDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj respondsToSelector:@selector(chatroomDidDestroy:)]) {
                [obj chatroomDidDestroy:chatroomId];
            }
        }];
    });
}

- (void)chatroomDidQuit:(NSString *)chatroomId {
    [self changeStatus:JChatroomStatusQuit forChatroom:chatroomId];
    dispatch_async(self.core.delegateQueue, ^{
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JChatroomDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj respondsToSelector:@selector(chatroomDidQuit:)]) {
                [obj chatroomDidQuit:chatroomId];
            }
        }];
    });
}

- (void)chatroomDidKick:(NSString *)chatroomId {
    [self changeStatus:JChatroomStatusQuit forChatroom:chatroomId];
    dispatch_async(self.core.delegateQueue, ^{
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JChatroomDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj respondsToSelector:@selector(chatroomDidKick:)]) {
                [obj chatroomDidKick:chatroomId];
            }
        }];
    });
}

#pragma mark -- internal
- (void)attrRetryTimerFired {
    NSString *chatroomId = self.attrRetryTimer.userInfo;
    [self stopAttrRetryTimer];
    [self syncChatroomAttr:chatroomId time:[self getAttrSyncTimeForChatroom:chatroomId]];
}

- (void)stopAttrRetryTimer {
    if (self.attrRetryTimer) {
        [self.attrRetryTimer invalidate];
        self.attrRetryTimer = nil;
    }
}

- (void)syncChatroomAttr:(nonnull NSString *)chatroomId time:(long long)syncTime {
    JLogI(@"MSG-ChrmAttrSync", @"id is %@, time is %lld", chatroomId, syncTime);
    [self.core.webSocket syncChatroomAttributesWithTime:syncTime
                                             chatroomId:chatroomId
                                                 userId:self.core.userId];
}

- (NSHashTable<id<JChatroomDelegate>> *)delegates {
    if (!_delegates) {
        _delegates = [NSHashTable weakObjectsHashTable];
    }
    return _delegates;
}

- (NSHashTable<id<JChatroomAttributesDelegate>> *)attributesDelegates {
    if (!_attributesDelegates) {
        _attributesDelegates = [NSHashTable weakObjectsHashTable];
    }
    return _attributesDelegates;
}

- (NSMutableDictionary<NSString *, JCachedChatroom *> *)cachedChatroomDic {
    if (!_cachedChatroomDic) {
        _cachedChatroomDic = [NSMutableDictionary dictionary];
    }
    return _cachedChatroomDic;
}

- (JIntervalGenerator *)intervalGenerator {
    if (!_intervalGenerator) {
        _intervalGenerator = [[JIntervalGenerator alloc] init];
    }
    return _intervalGenerator;
}

@end
