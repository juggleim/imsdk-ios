//
//  JConversationManager.m
// JuggleIM
//
//  Created by Nathan on 2023/11/29.
//

#import "JConversationManager.h"
#import "JMessageSendReceiveDelegate.h"
#import "JUnDisturbConvMessage.h"
#import "JTopConvMessage.h"
#import "JClearUnreadMessage.h"
#import "JDeleteMsgMessage.h"
#import "JCleanMsgMessage.h"
#import "JLogger.h"
#import "JIntervalGenerator.h"

#define kConversationSyncCount 100

@interface JConversationManager ()
@property (nonatomic, strong) JIMCore *core;
@property (nonatomic, strong) NSHashTable <id<JConversationDelegate>> *delegates;
@property (nonatomic, strong) NSHashTable <id<JConversationSyncDelegate>> *syncDelegates;
//在 receiveQueue 里处理
@property (nonatomic, assign) BOOL syncProcessing;
@property (nonatomic, assign) long long cachedSyncTime;

@property (nonatomic, strong) JMessageManager * messageManager;
@property (nonatomic, strong) JIntervalGenerator *intervalGenerator;
@property (nonatomic, strong) NSTimer *syncRetryTimer;
@end

@implementation JConversationManager

- (instancetype)initWithCore:(JIMCore *)core messageManager:(JMessageManager *)messageManager{
    if (self = [super init]) {
        self.core = core;
        self.messageManager = messageManager;
        self.cachedSyncTime = -1;
        self.syncProcessing = YES;
    }
    return self;
}

- (void)syncConversations:(void (^)(void))completeBlock {
    self.syncProcessing = YES;
    [self internalSyncConversations:completeBlock];
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

- (void)deleteConversationInfoBy:(JConversation *)conversation
                         success:(void (^)(void))successBlock
                           error:(void (^)(JErrorCode code))errorBlock{
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket deleteConversationInfo:conversation
                                         userId:self.core.userId
                                        success:^(long long timestamp) {
        //删除会话不更新时间戳，只通过命令消息来更新
        JLogI(@"CONV-Delete", @"success");
        //更新消息发送时间
        [weakSelf.messageManager updateSendSyncTime:timestamp];
        [weakSelf.core.dbManager deleteConversationInfoBy:conversation];
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if(successBlock){
                successBlock();
            }
            JConversationInfo *info = [[JConversationInfo alloc] init];
            info.conversation = conversation;
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(conversationInfoDidDelete:)]) {
                    [obj conversationInfoDidDelete:@[info]];
                }
            }];
        });
        
    } error:^(JErrorCodeInternal code) {
        JLogE(@"CONV-Delete", @"error code is %lu", code);
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if(errorBlock){
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)clearUnreadCountByConversation:(JConversation *)conversation
                               success:(void (^)(void))successBlock
                                 error:(void (^)(JErrorCode code))errorBlock {
    JConcreteConversationInfo *info = [self.core.dbManager getConversationInfo:conversation];
    if (!info) {
        dispatch_async(self.core.delegateQueue, ^{
            if(errorBlock){
                errorBlock(JErrorCodeInvalidParam);
            }
        });
    }
    
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket clearUnreadCount:conversation
                                   userId:self.core.userId
                                 msgIndex:info.lastMessageIndex
                                    msgId:info.lastMessage.messageId
                                timestamp:info.lastMessage.timestamp
                                  success:^(long long timestamp) {
        JLogI(@"CONV-ClearUnread", @"success");
        [weakSelf.messageManager updateSendSyncTime:timestamp];
        [weakSelf.core.dbManager clearUnreadCountBy:conversation
                                           msgIndex:info.lastMessageIndex];
        [weakSelf.core.dbManager setMentionInfo:conversation mentionInfoJson:@""];
        [weakSelf.core.dbManager setUnread:NO conversation:conversation];
        [weakSelf noticeTotalUnreadCountChange];
        JConversationInfo * info = [self.core.dbManager getConversationInfo:conversation];
        if (info) {
            dispatch_async(self.core.delegateQueue, ^{
                [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                    if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                        [obj conversationInfoDidUpdate:@[info]];
                    }
                }];
            });
        }
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (successBlock) {
                successBlock();
            }
        });
        
    } error:^(JErrorCodeInternal code) {
        JLogE(@"CONV-ClearUnread", @"error code is %lu", code);
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if(errorBlock){
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (NSArray<JConversationInfo *> *)getConversationInfoList {
    return [self.core.dbManager getConversationInfoList];
}

- (int)getTotalUnreadCount {
    return [self.core.dbManager getTotalUnreadCount];
}

- (int)getUnreadCountWithTypes:(NSArray<NSNumber *> *)conversationTypes {
    return [self.core.dbManager getUnreadCountWithTypes:conversationTypes];
}

- (void)setDraft:(NSString *)draft inConversation:(JConversation *)conversation {
    [self.core.dbManager setDraft:draft inConversation:conversation];
    JConversationInfo *info = [self.core.dbManager getConversationInfo:conversation];
    if (info) {
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                    [obj conversationInfoDidUpdate:@[info]];
                }
            }];
        });
    }
}

- (void)clearDraftInConversation:(JConversation *)conversation {
    [self.core.dbManager clearDraftInConversation:conversation];
    JConversationInfo *info = [self.core.dbManager getConversationInfo:conversation];
    if (info) {
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                    [obj conversationInfoDidUpdate:@[info]];
                }
            }];
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
                         success:^(long long timestamp) {
        JLogI(@"CONV-Mute", @"success");
        [weakSelf.messageManager updateSendSyncTime:timestamp];
        [weakSelf.core.dbManager setMute:isMute conversation:conversation];
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (successBlock) {
                successBlock();
            }
            JConversationInfo *conversationInfo = [weakSelf.core.dbManager getConversationInfo:conversation];
            if (conversationInfo) {
                [weakSelf.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                    if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                        [obj conversationInfoDidUpdate:@[conversationInfo]];
                    }
                }];
            }
        });
        [weakSelf noticeTotalUnreadCountChange];
    } error:^(JErrorCodeInternal code) {
        JLogE(@"CONV-Mute", @"error code is %lu", code);
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)setTop:(BOOL)isTop
  conversation:(JConversation *)conversation
       success:(void (^)(void))successBlock
         error:(void (^)(JErrorCode))errorBlock {
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket setTop:isTop
                 inConversation:conversation
                         userId:self.core.userId
                        success:^(long long timestamp) {
        JLogI(@"CONV-Top", @"success");
        [weakSelf.messageManager updateSendSyncTime:timestamp];
        [weakSelf.core.dbManager setTop:isTop time:timestamp conversation:conversation];
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (successBlock) {
                successBlock();
            }
            JConversationInfo * conversationInfo = [weakSelf.core.dbManager getConversationInfo:conversation];
            if(conversationInfo){
                [weakSelf.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                    if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                        [obj conversationInfoDidUpdate:@[conversationInfo]];
                    }
                }];
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"CONV-Top", @"error code is %lu", code);
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)clearTotalUnreadCount:(void (^)(void))successBlock
                        error:(void (^)(JErrorCode code))errorBlock{
    long long time = MAX(self.core.messageSendSyncTime, self.core.messageReceiveSyncTime);
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket clearTotalUnreadCount:self.core.userId
                                          time:time
                                       success:^(long long timestamp) {
        JLogI(@"CONV-ClearTotal", @"success");
        [weakSelf.messageManager updateSendSyncTime:timestamp];
        [weakSelf.core.dbManager clearTotalUnreadCount];
        [weakSelf.core.dbManager clearMentionInfo];
        [weakSelf.core.dbManager clearUnreadTag];
        [weakSelf noticeTotalUnreadCountChange];
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if(successBlock){
                successBlock();
            }
        });
        
    } error:^(JErrorCodeInternal code) {
        JLogE(@"CONV-ClearTotal", @"error code is %lu", code);
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
            
        });
    }];
    
}

- (void)setUnread:(JConversation *)conversation
          success:(void (^)(void))successBlock
            error:(void (^)(JErrorCode))errorBlock {
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket setUnread:conversation
                            userId:self.core.userId
                           success:^(long long timestamp) {
        JLogI(@"CONV-SetUnread", @"success");
        [weakSelf.messageManager updateSendSyncTime:timestamp];
        [weakSelf setDBUnreadAndNotice:conversation];
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (successBlock) {
                successBlock();
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"CONV-SetUnread", @"error code is %lu", code);
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)createConversationInfo:(JConversation *)conversation
                       success:(void (^)(JConversationInfo *))successBlock
                         error:(void (^)(JErrorCode))errorBlock {
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket createConversationInfo:conversation
                                         userId:self.core.userId
                                        success:^(JConcreteConversationInfo * _Nonnull conversationInfo, long long timestamp) {
        JLogI(@"CONV-Create", @"success");
        [weakSelf.messageManager updateSendSyncTime:timestamp];
        JConcreteConversationInfo *info = [self handleConversationAdd:conversationInfo];
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(info);
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"CONV-Create", @"error code is %lu", code);
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((int)code);
            }
        });
    }];
}

- (void)setTopConversationsOrderType:(JTopConversationsOrderType)type {
    [self.core.dbManager setTopConversationsOrderType:type];
}

- (void)addDelegate:(id<JConversationDelegate>)delegate {
    dispatch_async(self.core.delegateQueue, ^{
        if (!delegate) {
            return;
        }
        [self.delegates addObject:delegate];
    });
}

- (void)addSyncDelegate:(id<JConversationSyncDelegate>)delegate {
    dispatch_async(self.core.delegateQueue, ^{
        if (!delegate) {
            return;
        }
        [self.syncDelegates addObject:delegate];
    });
}

- (void)connectSuccess {
    self.syncProcessing = YES;
}

#pragma mark - JMessageSendReceiveDelegate
- (void)messageDidSave:(JConcreteMessage *)message {
    [self addOrUpdateConversationsIfNeed:@[message]];
}

- (void)messageDidSend:(JConcreteMessage *)message {
    [self addOrUpdateConversationsIfNeed:@[message]];
    [self updateSyncTime:message.timestamp];
}

-(void)messagesDidReceive:(NSArray<JConcreteMessage *> *)messages{
    if (messages.count == 0) {
        return;
    }
    [self addOrUpdateConversationsIfNeed:messages];
    JConcreteMessage * message = messages.lastObject;
    [self updateSyncTime:message.timestamp];
    [self noticeTotalUnreadCountChange];
}

- (void)conversationsDidAdd:(JConcreteConversationInfo *)conversationInfo {
    [self handleConversationAdd:conversationInfo];
}

- (void)conversationsDidDelete:(NSArray<JConversation *> *)conversations {
    NSMutableArray *results = [[NSMutableArray alloc] init];
    [conversations enumerateObjectsUsingBlock:^(JConversation * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        JConversationInfo *info = [[JConversationInfo alloc] init];
        info.conversation = obj;
        [results addObject:info];
    }];
    dispatch_async(self.core.delegateQueue, ^{
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj respondsToSelector:@selector(conversationInfoDidDelete:)]) {
                [obj conversationInfoDidDelete:results];
            }
        }];
    });
}
- (void)conversationsDidUpdate:(JConcreteMessage *)message{
    if([message.contentType isEqualToString:[JUnDisturbConvMessage contentType]]){
        JUnDisturbConvMessage * content = (JUnDisturbConvMessage *)message.content;
        NSMutableArray * convs = [NSMutableArray array];
        for (JConcreteConversationInfo * conv in content.conversations) {
            //更新数据库
            [self.core.dbManager setMute:conv.mute conversation:conv.conversation];
            //获取会话对象
            JConversationInfo * conversationInfo = [self.core.dbManager getConversationInfo:conv.conversation];
            if(conversationInfo){
                [convs addObject:conversationInfo];
            }
        }
        //回调
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                    [obj conversationInfoDidUpdate:convs];
                }
            }];
        });
    }else if([message.contentType isEqualToString:[JTopConvMessage contentType]]){
        JTopConvMessage * content = (JTopConvMessage *)message.content;
        NSMutableArray * convs = [NSMutableArray array];
        for (JConcreteConversationInfo * conv in content.conversations) {
            //更新数据库
            [self.core.dbManager setTop:conv.isTop time:conv.topTime conversation:conv.conversation];
            //获取会话对象
            JConversationInfo * conversationInfo = [self.core.dbManager getConversationInfo:conv.conversation];
            if(conversationInfo){
                [convs addObject:conversationInfo];
            }
        }
        //回调
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                    [obj conversationInfoDidUpdate:convs];
                }
            }];
        });
    }else if([message.contentType isEqualToString:[JClearUnreadMessage contentType]]){
        JClearUnreadMessage * content = (JClearUnreadMessage *)message.content;
        NSMutableArray * convs = [NSMutableArray array];
        for (JConcreteConversationInfo * conv in content.conversations) {
            //更新数据库
            [self.core.dbManager clearUnreadCountBy:conv.conversation msgIndex:conv.lastReadMessageIndex];
            [self.core.dbManager setMentionInfo:conv.conversation mentionInfoJson:@""];
            [self.core.dbManager setUnread:NO conversation:conv.conversation];
            
            //获取会话对象
            JConversationInfo * convationInfo = [self.core.dbManager getConversationInfo:conv.conversation];
            if(convationInfo){
                [convs addObject:convationInfo];
            }
        }
        //回调
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                    [obj conversationInfoDidUpdate:convs];
                }
            }];
        });
        [self noticeTotalUnreadCountChange];
    }
}

- (void)messageDidRemove:(JConversation *)conversation
         removedMessages:(NSArray <JConcreteMessage *> *)removedMessages
             lastMessage:(JConcreteMessage *)lastMessage{
    JConcreteConversationInfo * info = [self getConversationAfterCommonResolved:conversation lastMessage:lastMessage];
    if(info == nil) {
        return;
    }
    NSMutableArray <JConversationMentionMessage *> * mentionMessages = [NSMutableArray arrayWithArray:info.mentionInfo.mentionMsgList];
    NSMutableArray <JConversationMentionMessage *> * removeMentionMessage = [NSMutableArray array];
    for (JConcreteMessage * removedMessage in removedMessages) {
        if(removedMessage.messageId == nil) {
            continue;
        }
        JConversationMentionMessage * temp = [[JConversationMentionMessage alloc] init];
        temp.msgId = removedMessage.messageId;
        if([mentionMessages containsObject:temp]){
            [removeMentionMessage addObject:temp];
        }
    }
    BOOL isUpdateMention = NO;
    if(removeMentionMessage.count != 0){
        [mentionMessages removeObjectsInArray:removeMentionMessage];
        info.mentionInfo.mentionMsgList = mentionMessages;
        [self.core.dbManager setMentionInfo:conversation mentionInfoJson:[info.mentionInfo encodeToJson]];
        isUpdateMention = YES;
    }
    
    [self updateConversationLastMessage:info lastMessage:lastMessage isUpdateMention:isUpdateMention];
}

- (void)messageDidClear:(JConversation *)conversation
              startTime:(long long)startTime
             sendUserId:(NSString *)sendUserId
            lastMessage:(JConcreteMessage *)lastMessage{
    JConcreteConversationInfo * info = [self getConversationAfterCommonResolved:conversation lastMessage:lastMessage];
    if(info == nil){
        return;
    }
    NSMutableArray <JConversationMentionMessage *> * mentionMessages = [NSMutableArray arrayWithArray:info.mentionInfo.mentionMsgList];
    NSMutableArray <JConversationMentionMessage *> * removeMentionMessage = [NSMutableArray array];
    for (JConversationMentionMessage * removedMessage in mentionMessages) {
        if (sendUserId.length > 0) {
            if ([removedMessage.senderId isEqualToString:sendUserId] && startTime > 0 && removedMessage.msgTime < startTime) {
                [removeMentionMessage addObject:removedMessage];
            }
        } else {
            if (startTime > 0 && removedMessage.msgTime < startTime) {
                [removeMentionMessage addObject:removedMessage];
            }
        }
    }
    BOOL isUpdateMention = NO;
    if(removeMentionMessage.count != 0){
        [mentionMessages removeObjectsInArray:removeMentionMessage];
        info.mentionInfo.mentionMsgList = mentionMessages;
        [self.core.dbManager setMentionInfo:conversation mentionInfoJson:[info.mentionInfo encodeToJson]];
        isUpdateMention = YES;
    }
    [self updateConversationLastMessage:info lastMessage:lastMessage isUpdateMention:isUpdateMention];
}

- (void)messageDidUpdate:(JConcreteMessage *)message {
    JConcreteMessage * lastMessage = [self.core.dbManager getLastMessage:message.conversation];
    if (lastMessage.clientMsgNo == message.clientMsgNo) {
        [self.core.dbManager updateLastMessageWithoutIndex:lastMessage];
        JConversationInfo *info = [self.core.dbManager getConversationInfo:message.conversation];
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                    [obj conversationInfoDidUpdate:@[info]];
                }
            }];
        });
    }
}

- (void)conversationsDidClearTotalUnread:(long long)clearTime { 
    [self.core.dbManager clearTotalUnreadCount];
    [self.core.dbManager clearMentionInfo];
    [self.core.dbManager clearUnreadTag];
    [self noticeTotalUnreadCountChange];
}

-(void)messageStateDidChange:(JMessageState)state conversation:(JConversation *)conversation clientMsgNo:(long long)clientMsgNo{
    if(conversation == nil){
        return;
    }
    if(clientMsgNo< 0 || state == 0){
        return;
    }
    JConcreteConversationInfo * conversationInfo = (JConcreteConversationInfo *)[self getConversationInfo:conversation];
    if(conversationInfo == nil || conversationInfo.lastMessage == nil || conversationInfo.lastMessage.clientMsgNo < 0){
        return;
    }
    if(clientMsgNo == conversationInfo.lastMessage.clientMsgNo){
        conversationInfo.lastMessage.messageState = state;
        [self.core.dbManager updateLastMessageState:conversation state:state withClientMsgNo:clientMsgNo];
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                    [obj conversationInfoDidUpdate:@[conversationInfo]];
                }
            }];
        });
    }
}

-(void)messageDidRead:(JConversation *)conversation messageIds:(NSArray<NSString *> *)messageIds{
    if(conversation == nil){
        return;
    }
    if(messageIds == nil || messageIds.count == 0){
        return;
    }
    JConcreteConversationInfo * conversationInfo = (JConcreteConversationInfo *)[self getConversationInfo:conversation];
    if(conversationInfo == nil || conversationInfo.lastMessage == nil || conversationInfo.lastMessage.clientMsgNo < 0){
        return;
    }
    if([messageIds containsObject:conversationInfo.lastMessage.messageId]){
        conversationInfo.lastMessage.hasRead = YES;
        [self.core.dbManager setLastMessageHasRead:conversation];
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                    [obj conversationInfoDidUpdate:@[conversationInfo]];
                }
            }];
        });
    }
}

- (void)conversationDidSetUnread:(JConversation *)conversation {
    [self setDBUnreadAndNotice:conversation];
}

#pragma mark - internal
-(JConcreteConversationInfo *)getConversationAfterCommonResolved:(JConversation *)conversation lastMessage:(JConcreteMessage *)lastMessage{
    if(conversation == nil) {
        return nil;
    }
    JConcreteConversationInfo * info = [self.core.dbManager getConversationInfo:conversation];
    if(info == nil) {
        return nil;
    }
    if(lastMessage != nil) {
        return info;
    }
    [self clearConversationLastMessage:info];
    return nil;
}

-(void)clearConversationLastMessage:(JConcreteConversationInfo *)conversationInfo{
    [self.core.dbManager clearLastMessage:conversationInfo.conversation];
    conversationInfo.mentionInfo = nil;
    conversationInfo.lastMessage = nil;
    dispatch_async(self.core.delegateQueue, ^{
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                [obj conversationInfoDidUpdate:@[conversationInfo]];
            }
        }];
    });
}
-(void)updateConversationLastMessage:(JConcreteConversationInfo *)info
                         lastMessage:(JConcreteMessage *)lastMessage
                     isUpdateMention:(BOOL)isUpdateMention {
    BOOL isLastMessageUpdate = (info.lastMessage == nil ||
                                info.lastMessage.clientMsgNo != lastMessage.clientMsgNo ||
                                ![info.lastMessage.contentType isEqualToString:lastMessage.contentType]);
    if (isLastMessageUpdate || isUpdateMention) {
        [self.core.dbManager updateLastMessageWithoutIndex:lastMessage];
        info.lastMessage = lastMessage;
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                    [obj conversationInfoDidUpdate:@[info]];
                }
            }];
        });
    }
}

- (void)internalSyncConversations:(void (^)(void))completeBlock {
    dispatch_async(self.core.sendQueue, ^{
        JLogI(@"CONV-Sync", @"sync time is %lld", self.core.conversationSyncTime);
        [self.core.webSocket syncConversations:self.core.conversationSyncTime
                                         count:kConversationSyncCount
                                        userId:self.core.userId
                                       success:^(NSArray * _Nonnull conversations, NSArray * _Nonnull deletedConversations, BOOL isFinished) {
            JLogI(@"CONV-Sync", @"success conversation count is %lu, delete count is %lu", (unsigned long)conversations.count, (unsigned long)deletedConversations.count);
            [self.intervalGenerator reset];
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
                            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                                if ([obj respondsToSelector:@selector(conversationInfoDidAdd:)]) {
                                    [obj conversationInfoDidAdd:insertConversations];
                                }
                            }];
                        });
                    }
                    if (updateConversations.count > 0) {
                        dispatch_async(self.core.delegateQueue, ^{
                            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                                if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                                    [obj conversationInfoDidUpdate:updateConversations];
                                }
                            }];
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
                    [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                        if ([obj respondsToSelector:@selector(conversationInfoDidDelete:)]) {
                            [obj conversationInfoDidDelete:deletedConversations];
                        }
                    }];
                });
            }
            if (syncTime > 0) {
                self.core.conversationSyncTime = syncTime;
            }
            if (!isFinished) {
                [self internalSyncConversations:completeBlock];
            } else {
                self.syncProcessing = NO;
                if (self.cachedSyncTime > 0) {
                    self.core.conversationSyncTime = self.cachedSyncTime;
                    self.cachedSyncTime = -1;
                }
                dispatch_async(self.core.delegateQueue, ^{
                    [self.syncDelegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationSyncDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                        if ([obj respondsToSelector:@selector(conversationSyncDidComplete)]) {
                            [obj conversationSyncDidComplete];
                        }
                    }];
                });
                if (completeBlock) {
                    completeBlock();
                }
            }
        } error:^(JErrorCodeInternal code) {
            JLogE(@"CONV-Sync", @"error code is %lu", code);
            
            dispatch_async(dispatch_get_main_queue(), ^{
                if (self.syncRetryTimer) {
                    return;
                }
                self.syncRetryTimer = [NSTimer scheduledTimerWithTimeInterval:[self.intervalGenerator getNextInterval] target:self selector:@selector(syncRetryTimerFired) userInfo:completeBlock repeats:NO];
            });
        }];
    });
}

- (void)syncRetryTimerFired {
    void (^block)(void) = self.syncRetryTimer.userInfo;
    [self stopSyncTimer];
    [self internalSyncConversations:block];
}

- (void)stopSyncTimer {
    if (self.syncRetryTimer) {
        [self.syncRetryTimer invalidate];
        self.syncRetryTimer = nil;
    }
}

- (JConcreteConversationInfo *)handleConversationAdd:(JConcreteConversationInfo *)conversationInfo {
    [self updateSyncTime:conversationInfo.syncTime];
    [self updateUserInfos:@[conversationInfo]];
    JConcreteConversationInfo *old = [self.core.dbManager getConversationInfo:conversationInfo.conversation];
    if (old) {
        if (conversationInfo.sortTime > old.sortTime) {
            [self.core.dbManager updateTime:conversationInfo.sortTime forConversation:conversationInfo.conversation];
            old.sortTime = conversationInfo.sortTime;
            old.syncTime = conversationInfo.syncTime;
        }
        conversationInfo = old;
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                    [obj conversationInfoDidUpdate:@[conversationInfo]];
                }
            }];
        });
    } else {
        [self.core.dbManager insertConversations:@[conversationInfo] completion:nil];
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(conversationInfoDidAdd:)]) {
                    [obj conversationInfoDidAdd:@[conversationInfo]];
                }
            }];
        });
    }
    return conversationInfo;
}

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
        if (obj.mentionUserList!= nil){
            for (JUserInfo * userInfo in obj.mentionUserList) {
                [userDic setObject:userInfo forKey:userInfo.userId];
            }
        }
    }];
    [self.core.dbManager insertUserInfos:userDic.allValues];
    [self.core.dbManager insertGroupInfos:groupDic.allValues];
}

-(void)addOrUpdateConversationsIfNeed:(NSArray <JConcreteMessage *> *)messages {
    NSMutableArray * conversations = [NSMutableArray array];
    for (JConcreteMessage * message in messages) {
        if (message.timestamp <= self.core.conversationSyncTime) {
            continue;
        }
        
        BOOL hasMention = NO;
        //接收的消息才处理 mention
        if (message.direction == JMessageDirectionReceive
            && message.mentionInfo != nil) {
            if (message.mentionInfo.type == JMentionTypeAll
                || message.mentionInfo.type == JMentionTypeAllAndSomeOne) {
                hasMention = YES;
            } else if (message.mentionInfo.type == JMentionTypeSomeOne) {
                for (JUserInfo *userInfo in message.mentionInfo.targetUsers) {
                    if ([userInfo.userId isEqualToString:self.core.userId]) {
                        hasMention = YES;
                        break;
                    }
                }
            }
        }
        JConversationMentionInfo * mentionInfo;
        if(hasMention) {
            NSMutableArray <JConversationMentionMessage *> * msgs = [NSMutableArray array];
            JConversationMentionMessage * msg = [[JConversationMentionMessage alloc]init];
            msg.senderId = message.senderUserId;
            msg.msgId = message.messageId;
            msg.msgTime = message.timestamp;
            [msgs addObject:msg];
            mentionInfo = [[JConversationMentionInfo alloc] init];
            mentionInfo.mentionMsgList = msgs;
        }
        BOOL isBroadcast = NO;
        if (message.flags & JMessageFlagIsBroadcast) {
            isBroadcast = YES;
        }
        JConcreteConversationInfo *info;
        for (JConcreteConversationInfo * conversationInfo in conversations) {
            if([conversationInfo.conversation.conversationId isEqualToString:message.conversation.conversationId] &&
               conversationInfo.conversation.conversationType == message.conversation.conversationType){
                info = conversationInfo;
                break;
            }
        }
        if(info == nil) {
            info = (JConcreteConversationInfo *)[self getConversationInfo:message.conversation];
            if(info != nil) {
                [conversations addObject:info];
            }
        }
        if (!info) {
            JConcreteConversationInfo *addInfo = [[JConcreteConversationInfo alloc] init];
            addInfo.conversation = message.conversation;
            if (isBroadcast && message.direction == JMessageDirectionSend) {
                addInfo.sortTime = 0;
            } else {
                addInfo.sortTime = message.timestamp;
            }
            addInfo.lastMessage = message;
            if (message.msgIndex > 0) {
                addInfo.lastMessageIndex = message.msgIndex;
                addInfo.lastReadMessageIndex = message.msgIndex - 1;
                addInfo.unreadCount = 1;
            }
            if (mentionInfo) {
                addInfo.mentionInfo = mentionInfo;
            }
            [conversations addObject:addInfo];
        } else {
            if (mentionInfo) {
                if(info.mentionInfo.mentionMsgList != nil){
                    NSMutableArray * msgs = [NSMutableArray arrayWithArray:info.mentionInfo.mentionMsgList];
                    for (JConversationMentionMessage * msg in mentionInfo.mentionMsgList) {
                        if(![msgs containsObject:msg]){
                            [msgs addObject:msg];
                        }
                    }
                    mentionInfo.mentionMsgList = msgs;
                }
                info.mentionInfo = mentionInfo;
            }
            //更新未读数
            if (message.msgIndex > 0) {
                info.lastMessageIndex = message.msgIndex;
                info.unreadCount = (int)(info.lastMessageIndex - info.lastReadMessageIndex);
            }
            if (!isBroadcast || message.direction != JMessageDirectionSend) {
                info.sortTime = message.timestamp;
            }
            info.lastMessage = message;
        }
    }
    [self.core.dbManager insertConversations:conversations completion:^(NSArray<JConcreteConversationInfo *> * _Nonnull insertConversations, NSArray<JConcreteConversationInfo *> * _Nonnull updateConversations) {
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if(insertConversations.count > 0){
                    if ([obj respondsToSelector:@selector(conversationInfoDidAdd:)]) {
                        [obj conversationInfoDidAdd:insertConversations];
                    }
                }
                if(updateConversations.count > 0){
                    if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                        [obj conversationInfoDidUpdate:updateConversations];
                    }
                }
            }];
        });
    }];
}

- (void)setDBUnreadAndNotice:(JConversation *)conversation {
    [self.core.dbManager setUnread:YES conversation:conversation];
    dispatch_async(self.core.delegateQueue, ^{
        JConversationInfo * conversationInfo = [self.core.dbManager getConversationInfo:conversation];
        if(conversationInfo) {
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(conversationInfoDidUpdate:)]) {
                    [obj conversationInfoDidUpdate:@[conversationInfo]];
                }
            }];
            if (conversationInfo.unreadCount == 0) {
                [self noticeTotalUnreadCountChange];
            }
        }
    });
}

- (void)noticeTotalUnreadCountChange {
    int count = [self.core.dbManager getTotalUnreadCount];
    if (count < 0) {
        return;
    }
    dispatch_async(self.core.delegateQueue, ^{
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JConversationDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj respondsToSelector:@selector(totalUnreadMessageCountDidUpdate:)]) {
                [obj totalUnreadMessageCountDidUpdate:count];
            }
        }];
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

- (NSHashTable<id<JConversationDelegate>> *)delegates {
    if (!_delegates) {
        _delegates = [NSHashTable weakObjectsHashTable];
    }
    return _delegates;
}

- (NSHashTable<id<JConversationSyncDelegate>> *)syncDelegates {
    if (!_syncDelegates) {
        _syncDelegates = [NSHashTable weakObjectsHashTable];
    }
    return _syncDelegates;
}

- (JIntervalGenerator *)intervalGenerator {
    if (!_intervalGenerator) {
        _intervalGenerator = [[JIntervalGenerator alloc] init];
    }
    return _intervalGenerator;
}

@end
