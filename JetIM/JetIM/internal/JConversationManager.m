//
//  JConversationManager.m
//  JetIM
//
//  Created by Nathan on 2023/11/29.
//

#import "JConversationManager.h"

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
    JConversationManager *m = [[JConversationManager alloc] init];
    m.core = core;
    m.cachedSyncTime = -1;
    //TODO: change implement
    [[NSNotificationCenter defaultCenter] addObserver:m
                                             selector:@selector(noticeConversationUpdate:)
                                                 name:@"updateLastMessage"
                                               object:nil];
    return m;
}

- (void)syncConversations:(void (^)(void))completeBlock {
    self.syncProcessing = YES;
    [self.core.webSocket syncConversations:self.core.conversationSyncTime
                                     count:kConversationSyncCount
                                    userId:self.core.userId
                                   success:^(NSArray * _Nonnull conversations, NSArray * _Nonnull deletedConversations, BOOL isFinished) {
        long long syncTime = 0;
        if (conversations.lastObject) {
            JConcreteConversationInfo *last = conversations.lastObject;
            if (last.syncTime > 0) {
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
        }
        if (deletedConversations.lastObject) {
            JConcreteConversationInfo *last = deletedConversations.lastObject;
            if (last.syncTime > syncTime) {
                syncTime = last.syncTime;
            }
            [deletedConversations enumerateObjectsUsingBlock:^(JConcreteConversationInfo *  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                [self.core.dbManager deleteConversationInfoBy:obj.conversation];
            }];
            //TODO: 本地没有的不做回调？给回调也没事
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
}

//- (void)clearUnreadCountByConversation:(JConversation *)conversation { 
//    <#code#>
//}

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
                                        success:^(long long timestamp) {
        //删除会话不更新时间戳，只通过命令消息来更新
//        if (self.syncProcessing) {
//            if (timestamp > self.cachedSyncTime) {
//                self.cachedSyncTime = timestamp;
//            }
//        } else {
//            self.core.conversationSyncTime = timestamp;
//        }
        NSLog(@"[JetIM] delete conversation success");
    } error:^(JErrorCodeInternal code) {
        NSLog(@"[JetIM] delete conversation error, code is %lu", code);
    }];
}

- (void)clearUnreadCountByConversation:(JConversation *)conversation {
    JConversationInfo *info = [self getConversationInfo:conversation];
    JConcreteMessage *lastMessage = (JConcreteMessage *)info.lastMessage;
    long long lastMessageIndex = lastMessage.msgIndex;
    [self.core.dbManager clearUnreadCountBy:conversation
                                   msgIndex:lastMessageIndex];
    [self.core.webSocket clearUnreadCount:conversation
                                   userId:self.core.userId
                                 msgIndex:lastMessageIndex
                                  success:^(long long timestamp) {
        //清除未读数暂时不用更新 syncTime
//        if (self.syncProcessing) {
//            if (timestamp > self.cachedSyncTime) {
//                self.cachedSyncTime = timestamp;
//            }
//        } else {
//            self.core.conversationSyncTime = timestamp;
//        }
        NSLog(@"[JetIM] clear unread success");
    } error:^(JErrorCodeInternal code) {
        NSLog(@"[JetIM] clear unread error, code is %lu", code);
    }];
}

- (NSArray<JConversationInfo *> *)getConversationInfoList {
    return [self.core.dbManager getConversationInfoList];
}
//
//- (int)getTotalUnreadCount { 
//    <#code#>
//}
//
//- (int)getUnreadCountByConversation:(JConversation *)conversation { 
//    <#code#>
//}

- (void)setDraft:(NSString *)draft inConversation:(JConversation *)conversation {
    [self.core.dbManager setDraft:draft inConversation:conversation];
}

- (void)clearDraftInConversation:(JConversation *)conversation {
    [self.core.dbManager clearDraftInConversation:conversation];
}

- (void)setDelegate:(id<JConversationDelegate>)delegate {
    _delegate = delegate;
}

- (void)setSyncDelegate:(id<JConversationSyncDelegate>)delegate {
    _syncDelegate = delegate;
}

#pragma mark - internal
- (void)noticeConversationUpdate:(NSNotification *)notification {
    //TODO: 只有send成功的回调里才更新 syncTime，插入消息时不更新
    JConversation *conversation = notification.object;
    JConversationInfo *info = [self getConversationInfo:conversation];
    //TODO: info 为空时表示是新的会话发送消息，此时要生成会话并给一个 conversationAdd 回调
    if (!info) {
        return;
    }
    if (self.syncProcessing) {
        if (info.lastMessage.timestamp > self.cachedSyncTime) {
            self.cachedSyncTime = info.lastMessage.timestamp;
        }
    } else {
        self.core.conversationSyncTime = info.lastMessage.timestamp;
    }
    dispatch_async(self.core.delegateQueue, ^{
        if ([self.delegate respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
            [self.delegate conversationInfoDidUpdate:@[info]];
        }
    });
    
}

@end
