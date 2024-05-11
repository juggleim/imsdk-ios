//
//  JConversationManager.m
//  JetIM
//
//  Created by Nathan on 2023/11/29.
//

#import "JConversationManager.h"
#import "JMessageSendReceiveDelegate.h"

#define kConversationSyncCount 100

@interface JConversationManager ()
@property (nonatomic, strong) JetIMCore *core;
@property (nonatomic, weak) id<JConversationDelegate> delegate;
@property (nonatomic, weak) id<JConversationSyncDelegate> syncDelegate;
//在 receiveQueue 里处理
@property (nonatomic, assign) BOOL syncProcessing;
@property (nonatomic, assign) long long cachedSyncTime;
@end

@implementation JConversationManager

- (instancetype)initWithCore:(JetIMCore *)core {
    if (self = [super init]) {
        self.core = core;
        self.cachedSyncTime = -1;
    }
    return self;
}

- (void)syncConversations:(void (^)(void))completeBlock {
    self.syncProcessing = YES;
    dispatch_async(self.core.sendQueue, ^{
        [self.core.webSocket syncConversations:self.core.conversationSyncTime
                                         count:kConversationSyncCount
                                        userId:self.core.userId
                                       success:^(NSArray * _Nonnull conversations, NSArray * _Nonnull deletedConversations, BOOL isFinished) {
            long long syncTime = 0;
            if (conversations.lastObject) {
                [self updateUserInfos:conversations];
                JConcreteConversationInfo *last = conversations.lastObject;
                if (last.syncTime > syncTime) {
                    syncTime = last.syncTime;
                }
                [self.core.dbManager insertConversations:conversations
                                              completion:^(NSArray<JConcreteConversationInfo *> * _Nonnull insertConversations, NSArray<JConcreteConversationInfo *> * _Nonnull updateConversations) {
                    if (insertConversations.count > 0) {
                        dispatch_async(self.core.delegateQueue, ^{
                            if ([self.delegate respondsToSelector:@selector(conversationInfoDidAdd:)]) {
                                [self.delegate conversationInfoDidAdd:insertConversations];
                            }
                        });
                    }
                    if (updateConversations.count > 0) {
                        dispatch_async(self.core.delegateQueue, ^{
                            if ([self.delegate respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                                [self.delegate conversationInfoDidUpdate:updateConversations];
                            }
                        });
                    }
                }];
                [self noticeTotalUnreadCountChange];
            }
            if (deletedConversations.lastObject) {
                [self updateUserInfos:deletedConversations];
                JConcreteConversationInfo *last = deletedConversations.lastObject;
                if (last.syncTime > syncTime) {
                    syncTime = last.syncTime;
                }
                [deletedConversations enumerateObjectsUsingBlock:^(JConcreteConversationInfo *  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                    [self.core.dbManager deleteConversationInfoBy:obj.conversation];
                }];
                //本地没有的给 delete 回调也没事
                dispatch_async(self.core.delegateQueue, ^{
                    if ([self.delegate respondsToSelector:@selector(conversationInfoDidDelete:)]) {
                        [self.delegate conversationInfoDidDelete:deletedConversations];
                    }
                });
            }
            if (syncTime > 0) {
                self.core.conversationSyncTime = syncTime;
            }
            if (!isFinished) {
                [self syncConversations:completeBlock];
            } else {
                self.syncProcessing = NO;
                if (self.cachedSyncTime > 0) {
                    self.core.conversationSyncTime = self.cachedSyncTime;
                    self.cachedSyncTime = -1;
                }
                dispatch_async(self.core.delegateQueue, ^{
                    if ([self.syncDelegate respondsToSelector:@selector(conversationSyncDidComplete)]) {
                        [self.syncDelegate conversationSyncDidComplete];
                    }
                });
                if (completeBlock) {
                    completeBlock();
                }
            }
        } error:^(JErrorCodeInternal code) {
            NSLog(@"[JetIM] sync conversation fail, code is %lu", (unsigned long)code);
            if (completeBlock) {
                completeBlock();
            }
        }];
    });
}

- (JConversationInfo *)getConversationInfo:(JConversation *)conversation {
    return [self.core.dbManager getConversationInfo:conversation];
}

- (NSArray<JConversationInfo *> *)getConversationInfoListWithTypes:(NSArray<NSNumber *> *)conversationTypes
                                                             count:(int)count
                                                         timestamp:(long long)ts
                                                         direction:(JPullDirection)direction {
    return [self.core.dbManager getConversationInfoListWithTypes:conversationTypes
                                                           count:count
                                                       timestamp:ts
                                                       direction:direction];
}

- (NSArray<JConversationInfo *> *)getTopConversationInfoListByCount:(int)count
                                                          timestamp:(long long)ts
                                                          direction:(JPullDirection)direction {
    NSArray *array = [[NSArray alloc] init];
    return [self.core.dbManager getTopConversationInfoListWithTypes:array
                                                              count:count
                                                          timestamp:ts
                                                          direction:direction];
}

- (NSArray<JConversationInfo *> *)getConversationInfoListByCount:(int)count
                                                       timestamp:(long long)ts
                                                       direction:(JPullDirection)direction {
    NSArray *array = [[NSArray alloc] init];
    return [self getConversationInfoListWithTypes:array
                                            count:count
                                        timestamp:ts
                                        direction:direction];
}

- (void)deleteConversationInfoBy:(JConversation *)conversation {
    [self.core.dbManager deleteConversationInfoBy:conversation];
    //手动删除不给回调
    [self.core.webSocket deleteConversationInfo:conversation
                                         userId:self.core.userId
                                        success:^(void) {
        //删除会话不更新时间戳，只通过命令消息来更新
        NSLog(@"[JetIM] delete conversation success");
    } error:^(JErrorCodeInternal code) {
        NSLog(@"[JetIM] delete conversation error, code is %lu", code);
    }];
}

- (void)clearUnreadCountByConversation:(JConversation *)conversation {
    JConcreteConversationInfo *info = [self.core.dbManager getConversationInfo:conversation];
    if (!info) {
        return;
    }
    [self.core.dbManager clearUnreadCountBy:conversation
                                   msgIndex:info.lastMessageIndex];
    [self.core.dbManager setMention:NO
                       conversation:conversation];
    [self noticeTotalUnreadCountChange];
    [self.core.webSocket clearUnreadCount:conversation
                                   userId:self.core.userId
                                 msgIndex:info.lastMessageIndex
                                  success:^(void) {
        //清除未读数暂时不用更新 syncTime
        NSLog(@"[JetIM] clear unread success");
    } error:^(JErrorCodeInternal code) {
        NSLog(@"[JetIM] clear unread error, code is %lu", code);
    }];
}

- (NSArray<JConversationInfo *> *)getConversationInfoList {
    return [self.core.dbManager getConversationInfoList];
}

- (int)getTotalUnreadCount {
    return [self.core.dbManager getTotalUnreadCount];
}

- (void)setDraft:(NSString *)draft inConversation:(JConversation *)conversation {
    [self.core.dbManager setDraft:draft inConversation:conversation];
    JConversationInfo *info = [self.core.dbManager getConversationInfo:conversation];
    if (info) {
        dispatch_async(self.core.delegateQueue, ^{
            if ([self.delegate respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                [self.delegate conversationInfoDidUpdate:@[info]];
            }
        });
    }
}

- (void)clearDraftInConversation:(JConversation *)conversation {
    [self.core.dbManager clearDraftInConversation:conversation];
    JConversationInfo *info = [self.core.dbManager getConversationInfo:conversation];
    if (info) {
        dispatch_async(self.core.delegateQueue, ^{
            if ([self.delegate respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                [self.delegate conversationInfoDidUpdate:@[info]];
            }
        });
    }
}

- (void)setMute:(BOOL)isMute
   conversation:(JConversation *)conversation
        success:(void (^)(void))successBlock
          error:(void (^)(JErrorCode))errorBlock {
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket setMute:isMute
                  inConversation:conversation
                          userId:self.core.userId
                         success:^{
        [weakSelf.core.dbManager setMute:isMute conversation:conversation];
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (successBlock) {
                successBlock();
            }
        });
    } error:^(JErrorCodeInternal code) {
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)setTop:(BOOL)isTop conversation:(JConversation *)conversation {
    __weak typeof(self) weakSelf = self;
    [self.core.dbManager setTop:isTop conversation:conversation];
    [self.core.webSocket setTop:isTop
                 inConversation:conversation
                         userId:self.core.userId
                        success:^(long long timestamp) {
        [weakSelf.core.dbManager setTopTime:timestamp conversation:conversation];
        JConversationInfo * conversationInfo = [weakSelf.core.dbManager getConversationInfo:conversation];
        if(conversationInfo){
            dispatch_async(weakSelf.core.delegateQueue, ^{
                if ([weakSelf.delegate respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                    [weakSelf.delegate conversationInfoDidUpdate:@[conversationInfo]];
                }
            });
        }
    } error:^(JErrorCodeInternal code) {
    }];
}

- (void)setDelegate:(id<JConversationDelegate>)delegate {
    _delegate = delegate;
}

- (void)setSyncDelegate:(id<JConversationSyncDelegate>)delegate {
    _syncDelegate = delegate;
}

- (void)clearTotalUnreadCount {
    [self.core.dbManager clearTotalUnreadCount];
    [self noticeTotalUnreadCountChange];
    long long time = MAX(self.core.messageSendSyncTime, self.core.messageReceiveSyncTime);
    
    [self.core.webSocket clearTotalUnreadCount:self.core.userId
                                          time:time
                                       success:^{
        
    } error:^(JErrorCodeInternal code) {
        
    }];
    
}

#pragma mark - JMessageSendReceiveDelegate
- (void)messageDidSave:(JConcreteMessage *)message {
    [self addOrUpdateConversationIfNeed:message];
}

- (void)messageDidSend:(JConcreteMessage *)message {
    [self addOrUpdateConversationIfNeed:message];
    [self updateSyncTime:message.timestamp];
}

- (void)messageDidReceive:(JConcreteMessage *)message {
    [self addOrUpdateConversationIfNeed:message];
    [self updateSyncTime:message.timestamp];
    [self noticeTotalUnreadCountChange];
}

- (void)conversationsDidDelete:(NSArray<JConversation *> *)conversations {
    NSMutableArray *results = [[NSMutableArray alloc] init];
    [conversations enumerateObjectsUsingBlock:^(JConversation * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        JConversationInfo *info = [[JConversationInfo alloc] init];
        info.conversation = obj;
        [results addObject:info];
    }];
    dispatch_async(self.core.delegateQueue, ^{
        if ([self.delegate respondsToSelector:@selector(conversationInfoDidDelete:)]) {
            [self.delegate conversationInfoDidDelete:results];
        }
    });
}

#pragma mark - internal
- (void)updateUserInfos:(NSArray <JConcreteConversationInfo *> *)conversations {
    NSMutableDictionary *groupDic = [[NSMutableDictionary alloc] init];
    NSMutableDictionary *userDic = [[NSMutableDictionary alloc] init];
    [conversations enumerateObjectsUsingBlock:^(JConcreteConversationInfo * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if (obj.groupInfo.groupId.length > 0) {
            [groupDic setObject:obj.groupInfo forKey:obj.groupInfo.groupId];
        }
        if (obj.targetUserInfo.userId.length > 0) {
            [userDic setObject:obj.targetUserInfo forKey:obj.targetUserInfo.userId];
        }
    }];
    [self.core.dbManager insertUserInfos:userDic.allValues];
    [self.core.dbManager insertGroupInfos:groupDic.allValues];
}

- (void)addOrUpdateConversationIfNeed:(JConcreteMessage *)message {
    BOOL hasMention = NO;
    //接收的消息才处理 mention
    if (message.direction == JMessageDirectionReceive
        && message.content.mentionInfo) {
        if (message.content.mentionInfo.type == JMentionTypeAll
            || message.content.mentionInfo.type == JMentionTypeAllAndSomeOne) {
            hasMention = YES;
        } else if (message.content.mentionInfo.type == JMentionTypeSomeOne) {
            for (JUserInfo *userInfo in message.content.mentionInfo.targetUsers) {
                if ([userInfo.userId isEqualToString:self.core.userId]) {
                    hasMention = YES;
                    break;
                }
            }
        }
    }
    BOOL isBroadcast = NO;
    if (message.flags & JMessageFlagIsBroadcast) {
        isBroadcast = YES;
    }
    JConversationInfo *info = [self getConversationInfo:message.conversation];
    if (!info) {
        JConcreteConversationInfo *addInfo = [[JConcreteConversationInfo alloc] init];
        addInfo.conversation = message.conversation;
        if (isBroadcast && message.direction == JMessageDirectionSend) {
            addInfo.sortTime = 0;
        } else {
            addInfo.sortTime = message.timestamp;
        }
        addInfo.lastMessage = message;
        addInfo.lastMessageIndex = message.msgIndex;
        addInfo.lastReadMessageIndex = message.msgIndex - 1;
        addInfo.unreadCount = 1;
        if (hasMention) {
            addInfo.hasMentioned = YES;
        }
        [self.core.dbManager insertConversations:@[addInfo] completion:nil];
        dispatch_async(self.core.delegateQueue, ^{
            if ([self.delegate respondsToSelector:@selector(conversationInfoDidAdd:)]) {
                [self.delegate conversationInfoDidAdd:@[addInfo]];
            }
        });
    } else {
        if (hasMention) {
            [self.core.dbManager setMention:YES conversation:message.conversation];
            info.hasMentioned = YES;
        }
        info.lastMessage = message;
        [self.core.dbManager updateLastMessage:message];
        dispatch_async(self.core.delegateQueue, ^{
            if ([self.delegate respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                [self.delegate conversationInfoDidUpdate:@[info]];
            }
        });
    }
}

- (void)noticeTotalUnreadCountChange {
    int count = [self.core.dbManager getTotalUnreadCount];
    if (count < 0) {
        return;
    }
    dispatch_async(self.core.delegateQueue, ^{
        if ([self.delegate respondsToSelector:@selector(totalUnreadMessageCountDidUpdate:)]) {
            [self.delegate totalUnreadMessageCountDidUpdate:count];
        }
    });
}

- (void)updateSyncTime:(long long)timestamp {
    if (self.syncProcessing) {
        if (timestamp > self.cachedSyncTime) {
            self.cachedSyncTime = timestamp;
        }
    } else {
        self.core.conversationSyncTime = timestamp;
    }
}

@end
