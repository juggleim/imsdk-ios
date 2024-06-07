//
//  JMessageManager.m
//  JetIM
//
//  Created by Nathan on 2023/11/29.
//

#import "JMessageManager.h"
#import "JetIMConst.h"
#import "JTextMessage.h"
#import "JImageMessage.h"
#import "JFileMessage.h"
#import "JVoiceMessage.h"
#import "JContentTypeCenter.h"
#import "JRecallInfoMessage.h"
#import "JRecallCmdMessage.h"
#import "JDeleteConvMessage.h"
#import "JReadNtfMessage.h"
#import "JGroupReadNtfMessage.h"
#import "JMediaMessageContent.h"
#import "JUnDisturbConvMessage.h"
#import "JTopConvMessage.h"
#import "JClearUnreadMessage.h"
#import "JDeleteMsgMessage.h"
#import "JCleanMsgMessage.h"
#import "JLogCommandMessage.h"
#import "JLogger.h"

@interface JMessageManager () <JWebSocketMessageDelegate>
@property (nonatomic, strong) JetIMCore *core;
@property (nonatomic, weak) id<JMessageDelegate> delegate;
@property (nonatomic, weak) id<JMessageSyncDelegate> syncDelegate;
@property (nonatomic, weak) id<JMessageReadReceiptDelegate> readReceiptDelegate;
@property (nonatomic, weak) id<JMessageUploadProvider> uploadProvider;
@property (nonatomic, assign) int increaseId;
//在 receiveQueue 里处理
@property (nonatomic, assign) BOOL syncProcessing;
@property (nonatomic, assign) long long cachedReceiveTime;
@property (nonatomic, assign) long long cachedSendTime;
@end

@implementation JMessageManager

- (void)syncMessages {
    self.syncProcessing = YES;
    [self sync];
}

- (instancetype)initWithCore:(JetIMCore *)core {
    if (self = [super init]) {
        self.core = core;
        [self.core.webSocket setMessageDelegate:self];
        [self registerMessages];
        self.cachedSendTime = -1;
        self.cachedReceiveTime = -1;
    }
    return self;
}

- (void)setDelegate:(id<JMessageDelegate>)delegate {
    _delegate = delegate;
}

- (void)setSyncDelegate:(id<JMessageSyncDelegate>)syncDelegate {
    _syncDelegate = syncDelegate;
}

- (void)setReadReceiptDelegate:(id<JMessageReadReceiptDelegate>)delegate {
    _readReceiptDelegate = delegate;
}

- (void)setMessageUploadProvider:(id<JMessageUploadProvider>)uploadProvider {
    _uploadProvider = uploadProvider;
}

- (void)deleteMessagesByClientMsgNoList:(NSArray<NSNumber *> *)clientMsgNos
                           conversation:(JConversation *)conversation
                                success:(void (^)(void))successBlock
                                  error:(void (^)(JErrorCode))errorBlock{
    if(clientMsgNos == nil || clientMsgNos.count == 0 || conversation == nil){
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam);
            }
        });
        return;
    }
    NSArray * messages = [self getMessagesByClientMsgNos:clientMsgNos];
    NSMutableArray * deleteClientMsgNoList = [NSMutableArray array];
    NSMutableArray * deleteRemoteList = [NSMutableArray array];
    for (JMessage * message in messages) {
        if([message.conversation.conversationId isEqualToString:conversation.conversationId]
           && message.conversation.conversationType == conversation.conversationType){
            if(message.messageId.length > 0){
                [deleteRemoteList addObject:message];
            }
            [deleteClientMsgNoList addObject:@(message.clientMsgNo)];
        }
    }
    JLogI(@"MSG-Delete", @"by clientMsgNo, local count is %lu, remote count is %lu", (unsigned long)deleteClientMsgNoList.count, (unsigned long)deleteRemoteList.count);
    if(deleteClientMsgNoList.count == 0){
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeMessageNotExist);
            }
        });
        return;
    }
    //如果没有远端消息 只删除本地后直接回调
    if(deleteRemoteList.count == 0){
        [self.core.dbManager deleteMessageByClientIds:deleteClientMsgNoList];
#warning TODO 通知会话更新
        dispatch_async(self.core.delegateQueue, ^{
            if(successBlock){
                successBlock();
            }
        });
        return;
    }
    
    //如果有远端消息则删除远端消息，操作本地数据 回调
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket deleteMessage:conversation
                               msgList:deleteRemoteList
                               success:^{
        JLogI(@"MSG-Delete", @"websocket success");
        [weakSelf.core.dbManager deleteMessageByClientIds:deleteClientMsgNoList];

#warning TODO 通知会话更新
        dispatch_async(self.core.delegateQueue, ^{
            if(successBlock){
                successBlock();
            }
        });
        
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-Delete", @"websocket error code is %lu", (unsigned long)code);
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)deleteMessagesByMessageIds:(NSArray<NSString *> *)messageIds
                     conversation:(JConversation *)conversation
                          success:(void (^)(void))successBlock
                            error:(void (^)(JErrorCode))errorBlock{
    if(messageIds == nil || messageIds.count == 0 || conversation == nil){
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam);
            }
        });
        return;
    }
    
    NSArray * messages = [self getMessagesByMessageIds:messageIds];
    NSMutableArray * msgList = [NSMutableArray array];
    for (JMessage * message in messages) {
        if( [message.conversation.conversationId isEqualToString:conversation.conversationId]
           && message.conversation.conversationType == conversation.conversationType ){
            [msgList addObject:message];
        }
    }
    JLogI(@"MSG-Delete", @"by messageId, count is %lu", (unsigned long)msgList.count);
    if(msgList.count != 0){
        __weak typeof(self) weakSelf = self;
        [self.core.webSocket deleteMessage:conversation
                                   msgList:msgList
                                   success:^{
            JLogI(@"MSG-Delete", @"websocket success");
            NSMutableArray * ids = [NSMutableArray array];
            for (JMessage * message in msgList) {
                [ids addObject:message.messageId];
            }
            [weakSelf.core.dbManager deleteMessageByMessageIds:ids];
#warning TODO 通知会话更新
            dispatch_async(self.core.delegateQueue, ^{
                if(successBlock){
                    successBlock();
                }
            });
        } error:^(JErrorCodeInternal code) {
            JLogE(@"MSG-Delete", @"websocket error code is %lu", (unsigned long)code);
            dispatch_async(self.core.delegateQueue, ^{
                if (errorBlock) {
                    errorBlock((JErrorCode)code);
                }
            });
        }];
        
    }else{
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeMessageNotExist);
            }
        });
    }
}

- (void)recallMessage:(NSString *)messageId
               extras:(NSDictionary *)extras
              success:(void (^)(JMessage *))successBlock
                error:(void (^)(JErrorCode))errorBlock {
    NSArray *arr = [self getMessagesByMessageIds:@[messageId]];
    if (arr.count > 0) {
        JMessage *m = arr[0];
        if ([m.contentType isEqualToString:[JRecallInfoMessage contentType]]) {
            dispatch_async(self.core.delegateQueue, ^{
                if (errorBlock) {
                    errorBlock(JErrorCodeMessageAlreadyRecalled);
                }
            });
            return;
        }
        [self.core.webSocket recallMessage:messageId
                                    extras:extras
                              conversation:m.conversation
                                 timestamp:m.timestamp
                                   success:^(long long timestamp) {
            JLogI(@"MSG-Recall", @"success");
            if (self.syncProcessing) {
                self.cachedSendTime = timestamp;
            } else {
                self.core.messageSendSyncTime = timestamp;
            }
            m.contentType = [JRecallInfoMessage contentType];
            JRecallInfoMessage *recallInfoMsg = [[JRecallInfoMessage alloc] init];
            recallInfoMsg.exts = extras;
            m.content = recallInfoMsg;
            [self.core.dbManager updateMessageContent:recallInfoMsg
                                          contentType:m.contentType
                                        withMessageId:messageId];
            dispatch_async(self.core.delegateQueue, ^{
                if (successBlock) {
                    successBlock(m);
                }
            });
        } error:^(JErrorCodeInternal errorCode) {
            JLogE(@"MSG-Recall", @"error code is %lu", (unsigned long)errorCode);
            dispatch_async(self.core.delegateQueue, ^{
                if (errorBlock) {
                    errorBlock((JErrorCode)errorCode);
                }
            });
        }];
    } else {
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeMessageNotExist);
            }
        });
    }
}

- (void)clearMessagesIn:(JConversation *)conversation
              startTime:(long long)startTime
                success:(void (^)(void))successBlock
                  error:(void (^)(JErrorCode errorCode))errorBlock{
    if(startTime == 0){
        startTime =  [[NSDate date] timeIntervalSince1970] * 1000;
    }
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket clearHistoryMessage:conversation
                                        time:startTime
                                     success:^{
        JLogI(@"MSG-Clear", @"success");
        [weakSelf.core.dbManager clearMessagesIn:conversation startTime:startTime senderId:@""];
#warning  TODO 通知会话更新
        
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock();
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-Clear", @"error code is %lu", (unsigned long)code);
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction {
    NSArray *arr = [[NSArray alloc] init];
    return [self getMessagesFrom:conversation
                           count:count
                            time:time
                       direction:direction
                    contentTypes:arr];
}

- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction
                            contentTypes:(NSArray<NSString *> *)contentTypes {
    return [self.core.dbManager getMessagesFrom:conversation
                                          count:count
                                           time:time
                                      direction:direction
                                   contentTypes:contentTypes];
}

- (JMessage *)saveMessage:(JMessageContent *)content
           inConversation:(JConversation *)conversation
                direction:(JMessageDirection)direction {
    return [self saveMessageWithContent:content
                         inConversation:conversation
                                  state:JMessageStateUnknown
                              direction:direction
                            isBroadcast:NO];
}

- (void)setMessageState:(JMessageState)state
        withClientMsgNo:(long long)clientMsgNo {
    [self.core.dbManager setMessageState:state
                         withClientMsgNo:clientMsgNo];
}

/// 搜素本地消息 SearchContent内容包含 指定内容
/// - Parameter searchContent: 需要包含的字符串
- (NSArray<JMessage *> *)searchMessagesWithContent:(NSString *)searchContent
                                    inConversation:(JConversation *)conversation
                                             count:(int)count
                                              time:(long long)time
                                         direction:(JPullDirection)direction
                                      contentTypes:(NSArray<NSString *> *)contentTypes{
    return [self.core.dbManager searchMessagesWithContent:searchContent inConversation:conversation count:count time:time direction:direction contentTypes:contentTypes];
}

- (JMessage *)sendMessage:(JMessageContent *)content
           inConversation:(JConversation *)conversation
                  success:(void (^)(JMessage *))successBlock
                    error:(void (^)(JErrorCode, JMessage *))errorBlock {
    return [self sendMessage:content
              inConversation:conversation
                 isBroadcast:NO
                     success:successBlock
                       error:errorBlock];
}

- (JMessage *)sendMediaMessage:(JMediaMessageContent *)content
                inConversation:(JConversation *)conversation
                      progress:(void (^)(int, JMessage *))progressBlock
                       success:(void (^)(JMessage *))successBlock
                         error:(void (^)(JErrorCode, JMessage *))errorBlock
                        cancel:(void (^)(JMessage *))cancelBlock {
    if (![content isKindOfClass:[JMediaMessageContent class]]) {
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam, nil);
            }
        });
        return nil;
    }
    JConcreteMessage *message = [self saveMessageWithContent:content
                                              inConversation:conversation
                                                       state:JMessageStateUploading
                                                   direction:JMessageDirectionSend
                                                 isBroadcast:NO];
    if ([self.uploadProvider respondsToSelector:@selector(uploadMessage:progress:success:error:cancel:)]) {
        [self.uploadProvider uploadMessage:message
                                  progress:^(int progress) {
            dispatch_async(self.core.delegateQueue, ^{
                if (progressBlock) {
                    progressBlock(progress, message);
                }
            });
        } success:^(JMessage * _Nonnull uploadMessage) {
            if (![uploadMessage isKindOfClass:[JConcreteMessage class]]) {
                uploadMessage.messageState = JMessageStateFail;
                [self.core.dbManager setMessageState:JMessageStateFail withClientMsgNo:uploadMessage.clientMsgNo];
                dispatch_async(self.core.delegateQueue, ^{
                    if (errorBlock) {
                        errorBlock(JErrorCodeMessageUploadError, uploadMessage);
                    }
                });
                return;
            }
            JConcreteMessage *cm = (JConcreteMessage *)uploadMessage;
            [self.core.dbManager updateMessageContent:cm.content
                                          contentType:cm.contentType
                                      withClientMsgNo:cm.clientMsgNo];
            cm.messageState = JMessageStateSending;
            [self.core.dbManager setMessageState:JMessageStateSending withClientMsgNo:cm.clientMsgNo];
            [self sendWebSocketMessage:cm
                           isBroadcast:NO
                               success:successBlock
                                 error:errorBlock];
            
        } error:^{
            message.messageState = JMessageStateFail;
            [self.core.dbManager setMessageState:JMessageStateFail withClientMsgNo:message.clientMsgNo];
            dispatch_async(self.core.delegateQueue, ^{
                if (errorBlock) {
                    errorBlock(JErrorCodeMessageUploadError, message);
                }
            });
        } cancel:^{
            message.messageState = JMessageStateFail;
            [self.core.dbManager setMessageState:JMessageStateFail withClientMsgNo:message.clientMsgNo];
            dispatch_async(self.core.delegateQueue, ^{
                if (cancelBlock) {
                    cancelBlock(message);
                }
            });
        }];
    } else {
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeMessageUploadError, message);
            }
        });
    }
    return message;
}

- (void)broadcastMessage:(JMessageContent *)content
         inConversations:(NSArray<JConversation *> *)conversations
                progress:(void (^)(JMessage *, JErrorCode, int, int))progressBlock
                complete:(void (^)(void))completeBlock {
    if (conversations.count == 0) {
        dispatch_async(self.core.delegateQueue, ^{
            if (completeBlock) {
                completeBlock();
            }
        });
        return;
    }
    [self loopBroadcastMessage:content
               inConversations:conversations
                  processCount:0
                    totalCount:(int)conversations.count
                      progress:progressBlock
                      complete:completeBlock];
}

- (void)sendReadReceipt:(NSArray<NSString *> *)messageIds
         inConversation:(JConversation *)conversation
                success:(void (^)(void))successBlock
                  error:(void (^)(JErrorCode))errorBlock {
    [self.core.webSocket sendReadReceipt:messageIds
                          inConversation:conversation
                                 success:^{
        [self.core.dbManager setMessagesRead:messageIds];
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock();
            }
        });
    } error:^(JErrorCodeInternal code) {
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)getGroupMessageReadDetail:(NSString *)messageId
                   inConversation:(JConversation *)conversation
                          success:(void (^)(NSArray<JUserInfo *> *, NSArray<JUserInfo *> *))successBlock
                            error:(void (^)(JErrorCode))errorBlock {
    [self.core.webSocket getGroupMessageReadDetail:messageId
                                    inConversation:conversation
                                           success:^(NSArray<JUserInfo *> * _Nonnull readMembers, NSArray<JUserInfo *> * _Nonnull unreadMembers) {
        JLogI(@"MSG-GroupReadDetail", @"success");
        JGroupMessageReadInfo *info = [[JGroupMessageReadInfo alloc] init];
        info.readCount = (int)readMembers.count;
        info.memberCount = (int)readMembers.count + (int)unreadMembers.count;
        [self.core.dbManager setGroupMessageReadInfo:@{messageId:info}];
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(readMembers, unreadMembers);
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-GroupReadDetail", @"error code is %lu", code);
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (JMessage *)resend:(JMessage *)messsage
             success:(void (^)(JMessage *))successBlock
               error:(void (^)(JErrorCode, JMessage *))errorBlock {
    if (messsage.clientMsgNo <= 0
        || !messsage.content
        || messsage.conversation.conversationId.length == 0) {
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam, messsage);
            }
        });
        return messsage;
    }
    [self.core.dbManager deleteMessageByClientIds:@[@(messsage.clientMsgNo)]];
    return [self sendMessage:messsage.content
              inConversation:messsage.conversation
                     success:successBlock
                       error:errorBlock];
}

- (void)getRemoteMessagesFrom:(JConversation *)conversation
                    startTime:(long long)startTime
                        count:(int)count
                    direction:(JPullDirection)direction
                      success:(void (^)(NSArray *messages))successBlock
                        error:(void (^)(JErrorCode code))errorBlock {
    if (count > 100) {
        count = 100;
    }
    [self.core.webSocket queryHisMsgsFrom:conversation
                                startTime:startTime
                                    count:count
                                direction:direction
                                  success:^(NSArray * _Nonnull messages, BOOL isFinished) {
        JLogI(@"MSG-Get", @"success");
        [self.core.dbManager insertMessages:messages];
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(messages);
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-Get", @"error code is %lu", code);
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)getLocalAndRemoteMessagesFrom:(JConversation *)conversation
                            startTime:(long long)startTime
                                count:(int)count
                            direction:(JPullDirection)direction
                    localMessageBlock:(void (^)(NSArray *messages,BOOL needRemote))localMessageBlock
                   remoteMessageBlock:(void (^)(NSArray *messages))remoteMessageBlock
                                error:(void (^)(JErrorCode code))errorBlock{
    if (count <= 0) {
        dispatch_async(self.core.delegateQueue, ^{
            if (localMessageBlock) {
                NSArray *arr = [NSArray array];
                localMessageBlock(arr, NO);
            }
        });
        return;
    }
    if (count > 100) {
        count = 100;
    }
    NSArray *localMessages = [self getMessagesFrom:conversation
                                             count:count
                                              time:startTime
                                         direction:direction];
    __block BOOL needRemote = NO;
    //本地数据为空
    if (localMessages.count == 0) {
        needRemote = YES;
    } else {
        JConcreteMessage *message = localMessages[0];
        __block long long seqNo = message.seqNo;
        if(localMessages.count < count && seqNo != 1){
            //本地数据小于需要拉取的数量，并且本地数据第一条 seqNo 不是第一个
            needRemote = YES;
        } else {
            //本地数据等于需要拉取的数据 或 本地数据第一条 seqNo 是第一个
            //判断是否连续
            [localMessages enumerateObjectsUsingBlock:^(JConcreteMessage *  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if (idx > 0 && obj.messageState == JMessageStateSent && obj.seqNo > 0) {
                    if (obj.seqNo > ++seqNo) {
                        needRemote = YES;
                        *stop = YES;
                        return;
                    }
                }
            }];
        }
    }
    
    dispatch_async(self.core.delegateQueue, ^{
        if (localMessageBlock) {
            localMessageBlock(localMessages, needRemote);
        }
    });
    
    if (needRemote) {
        [self getRemoteMessagesFrom:conversation
                          startTime:startTime
                              count:count
                          direction:direction
                            success:^(NSArray *messages) {
            //合并
            NSMutableArray * messagesArray = [NSMutableArray array];
            [messagesArray addObjectsFromArray:localMessages];
            for (JConversation * message in messages) {
                if(![messagesArray containsObject:message]){
                    [messagesArray addObject:message];
                }
            }
            //正序排序
            NSArray * ascArray = [messagesArray sortedArrayUsingComparator:^NSComparisonResult(JConcreteMessage *  _Nonnull msg1, JConcreteMessage *  _Nonnull msg2) {
                if(msg1.timestamp < msg2.timestamp){
                    return NSOrderedAscending;
                }else if(msg1.timestamp > msg2.timestamp){
                    return NSOrderedDescending;
                }else{
                    return NSOrderedSame;
                }
            }];
            dispatch_async(self.core.delegateQueue, ^{
                if (remoteMessageBlock) {
                    remoteMessageBlock(ascArray);
                }
            });
            
        } error:errorBlock];
    }
}

- (NSArray<JMessage *> *)getMessagesByMessageIds:(NSArray<NSString *> *)messageIds {
    return [self.core.dbManager getMessagesByMessageIds:messageIds];
}

- (void)getMessagesByMessageIds:(NSArray<NSString *> *)messageIds
                 inConversation:(JConversation *)conversation
                        success:(void (^)(NSArray<JMessage *> *))successBlock
                          error:(void (^)(JErrorCode))errorBlock {
    if (messageIds.count == 0) {
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam);
            }
        });
        return;
    }
    NSArray <JMessage *> *localMessages = [self.core.dbManager getMessagesByMessageIds:messageIds];
    NSMutableArray *notExistArray = [[NSMutableArray alloc] init];
    if (localMessages.count == 0) {
        notExistArray = [messageIds mutableCopy];
    } else if (localMessages.count < messageIds.count) {
        int localMessageIndex = 0;
        for (int i = 0; i < messageIds.count; i++) {
            if (localMessageIndex == localMessages.count) {
                [notExistArray addObject:messageIds[i]];
                continue;
            }
            if ([messageIds[i] isEqualToString:localMessages[localMessageIndex].messageId]) {
                localMessageIndex++;
            } else {
                [notExistArray addObject:messageIds[i]];
            }
        }
    }
    if (notExistArray.count > 0) {
        [self.core.webSocket queryHisMsgsByIds:notExistArray
                                inConversation:conversation
                                       success:^(NSArray<JConcreteMessage *> * _Nonnull remoteMessages, BOOL isFinished) {
            JLogI(@"MSG-Get", @"by id, success");
            NSMutableArray <JMessage *> *result = [[NSMutableArray alloc] init];
            for (NSString *messageId in messageIds) {
                BOOL isMatch = NO;
                for (JMessage *localMessage in localMessages) {
                    if ([messageId isEqualToString:localMessage.messageId]) {
                        [result addObject:localMessage];
                        isMatch = YES;
                        break;
                    }
                }
                if (isMatch) {
                    continue;
                }
                for (JMessage *remoteMessage in remoteMessages) {
                    if ([messageId isEqualToString:remoteMessage.messageId]) {
                        [result addObject:remoteMessage];
                        break;
                    }
                }
            }
            dispatch_async(self.core.delegateQueue, ^{
                if (successBlock) {
                    successBlock(result);
                }
            });
        } error:^(JErrorCodeInternal code) {
            JLogE(@"MSG-Get", @"by id, error code is %lu", code);
            if (localMessages.count > 0) {
                dispatch_async(self.core.delegateQueue, ^{
                    if (successBlock) {
                        successBlock(localMessages);
                    }
                });
            } else if (errorBlock) {
                dispatch_async(self.core.delegateQueue, ^{
                    errorBlock((JErrorCode)code);
                });
            }
        }];
    } else {
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(localMessages);
            }
        });
    }
}

- (void)getMergedMessageList:(NSString *)messageId
                     success:(void (^)(NSArray<JMessage *> *))successBlock
                       error:(void (^)(JErrorCode))errorBlock {
    [self.core.webSocket getMergedMessageList:messageId
                                         time:0
                                        count:100
                                    direction:JPullDirectionOlder
                                      success:^(NSArray<JConcreteMessage *> * _Nonnull messages, BOOL isFinished) {
        JLogI(@"MSG-GetMerge", @"success");
        [self.core.dbManager insertMessages:messages];
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(messages);
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-GetMerge", @"error code is %lu", code);
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (NSArray<JMessage *> *)getMessagesByClientMsgNos:(NSArray<NSNumber *> *)clientMsgNos {
    return [self.core.dbManager getMessagesByClientMsgNos:clientMsgNos];
}

- (void)getMentionMessages:(JConversation *)conversation
                     count:(int)count
                      time:(long long)time
                 direction:(JPullDirection)direction
                   success:(void (^)(NSArray<JMessage *> *))successBlock
                     error:(void (^)(JErrorCode))errorBlock {
    [self.core.webSocket getMentionMessages:conversation
                                       time:time
                                      count:count
                                  direction:direction
                                    success:^(NSArray<JConcreteMessage *> * _Nonnull messages, BOOL isFinished) {
        JLogI(@"MSG-GetMention", @"success");
        [self.core.dbManager insertMessages:messages];
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(messages);
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-GetMention", @"error code is %lu", code);
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)registerContentType:(Class)messageClass {
    JLogI(@"MSG-Register", @"class is %@", messageClass);
    [[JContentTypeCenter shared] registerContentType:messageClass];
}

- (NSString *)getLocalAttributeByMessageId:(NSString *)messageId{
    return [self.core.dbManager getLocalAttributeByMessageId:messageId];
}
- (void)setLocalAttribute:(NSString *)attribute forMessage:(NSString *)messageId{
    [self.core.dbManager setLocalAttribute:attribute forMessage:messageId];
}
- (NSString *)getLocalAttributeByClientMsgNo:(long long)clientMsgNo{
    return [self.core.dbManager getLocalAttributeByClientMsgNo:clientMsgNo];
}
- (void)setLocalAttribute:(NSString *)attribute forClientMsgNo:(long long)clientMsgNo{
    [self.core.dbManager setLocalAttribute:attribute forClientMsgNo:clientMsgNo];
}

#pragma mark - JWebSocketMessageDelegate
- (void)messageDidReceive:(JConcreteMessage *)message {
    [self handleReceiveMessages:@[message]
                         isSync:NO];
}

- (void)messagesDidReceive:(NSArray<JConcreteMessage *> *)messages
                isFinished:(BOOL)isFinished {
    [self handleReceiveMessages:messages
                         isSync:YES];
    
    if (!isFinished) {
        [self sync];
    } else {
        self.syncProcessing = NO;
        if (self.cachedSendTime > 0) {
            self.core.messageSendSyncTime = self.cachedSendTime;
            self.cachedSendTime = -1;
        }
        if (self.cachedReceiveTime > 0) {
            self.core.messageReceiveSyncTime = self.cachedReceiveTime;
            self.cachedReceiveTime = -1;
        }
        
        dispatch_async(self.core.delegateQueue, ^{
            if ([self.syncDelegate respondsToSelector:@selector(messageSyncDidComplete)]) {
                [self.syncDelegate messageSyncDidComplete];
            }
        });
    }
}

- (void)syncNotify:(long long)syncTime {
    if (syncTime > self.core.messageReceiveSyncTime) {
        self.syncProcessing = YES;
        [self sync];
    }
}

#pragma mark - internal
- (void)registerMessages {
    [self registerContentType:[JTextMessage class]];
    [self registerContentType:[JImageMessage class]];
    [self registerContentType:[JFileMessage class]];
    [self registerContentType:[JVoiceMessage class]];
    [self registerContentType:[JVideoMessage class]];
    [self registerContentType:[JRecallCmdMessage class]];
    [self registerContentType:[JRecallInfoMessage class]];
    [self registerContentType:[JDeleteConvMessage class]];
    [self registerContentType:[JReadNtfMessage class]];
    [self registerContentType:[JGroupReadNtfMessage class]];
    [self registerContentType:[JMergeMessage class]];
    [self registerContentType:[JThumbnailPackedImageMessage class]];
    [self registerContentType:[JSnapshotPackedVideoMessage class]];
    [self registerContentType:[JUnDisturbConvMessage class]];
    [self registerContentType:[JTopConvMessage class]];
    [self registerContentType:[JClearUnreadMessage class]];
    [self registerContentType:[JDeleteMsgMessage class]];
    [self registerContentType:[JCleanMsgMessage class]];
    [self registerContentType:[JLogCommandMessage class]];
}

- (void)loopBroadcastMessage:(JMessageContent *)content
             inConversations:(NSArray<JConversation *> *)conversations
                processCount:(int)processCount
                  totalCount:(int)totalCount
                    progress:(void (^)(JMessage *message, JErrorCode errorCode, int processCount, int totalCount))progressBlock
                    complete:(void (^)(void))completeBlock {
    if (conversations.count == 0) {
        dispatch_async(self.core.delegateQueue, ^{
            if (completeBlock) {
                completeBlock();
            }
        });
        return;
    }
    [self sendMessage:content
       inConversation:conversations[0]
          isBroadcast:YES
              success:^(JMessage *message) {
        [self broadcastCallbackAndLoopNext:message
                                 errorCode:JErrorCodeNone
                             conversations:conversations
                              processCount:processCount
                                totalCount:totalCount
                                  progress:progressBlock
                                  complete:completeBlock];
    } error:^(JErrorCode errorCode, JMessage *message) {
        [self broadcastCallbackAndLoopNext:message
                                 errorCode:errorCode
                             conversations:conversations
                              processCount:processCount
                                totalCount:totalCount
                                  progress:progressBlock
                                  complete:completeBlock];
    }];
}

- (void)broadcastCallbackAndLoopNext:(JMessage *)message
                           errorCode:(JErrorCode)errorCode
                       conversations:(NSArray<JConversation *> *)conversations
                        processCount:(int)processCount
                          totalCount:(int)totalCount
                            progress:(void (^)(JMessage *message, JErrorCode errorCode, int processCount, int totalCount))progressBlock
                            complete:(void (^)(void))completeBlock {
    dispatch_async(self.core.delegateQueue, ^{
        if (progressBlock) {
            progressBlock(message, errorCode, processCount, totalCount);
        }
    });
    if (conversations.count <= 1) {
        dispatch_async(self.core.delegateQueue, ^{
            if (completeBlock) {
                completeBlock();
            }
        });
    } else {
        NSArray <JConversation *> *subConversations = [conversations subarrayWithRange:NSMakeRange(1, conversations.count - 1)];
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 50 * NSEC_PER_MSEC), self.core.sendQueue, ^{
            [self loopBroadcastMessage:message.content
                       inConversations:subConversations
                          processCount:processCount+1
                            totalCount:totalCount
                              progress:progressBlock
                              complete:completeBlock];
        });
    }
}

- (void)sendWebSocketMessage:(JConcreteMessage *)message
                 isBroadcast:(BOOL)isBroadcast
                     success:(void (^)(JMessage *message))successBlock
                       error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock {
    NSArray *mergedMessages = nil;
    if ([message.content isKindOfClass:[JMergeMessage class]]) {
        JMergeMessage *merge = (JMergeMessage *)message.content;
        mergedMessages = [self.core.dbManager getMessagesByMessageIds:merge.messageIdList];
    }
    
    [self.core.webSocket sendIMMessage:message.content
                        inConversation:message.conversation
                           clientMsgNo:message.clientMsgNo
                             clientUid:message.clientUid
                            mergedMsgs:mergedMessages
                           isBroadcast:isBroadcast
                                userId:self.core.userId
                               success:^(long long clientMsgNo, NSString *msgId, long long timestamp, long long seqNo) {
        JLogI(@"MSG-Send", @"success");
        if (self.syncProcessing) {
            self.cachedSendTime = timestamp;
        } else {
            self.core.messageSendSyncTime = timestamp;
        }
        [self.core.dbManager updateMessageAfterSend:message.clientMsgNo
                                          messageId:msgId
                                          timestamp:timestamp
                                              seqNo:seqNo];
        message.messageId = msgId;
        message.timestamp = timestamp;
        message.seqNo = seqNo;
        message.messageState = JMessageStateSent;
        
        if ([self.sendReceiveDelegate respondsToSelector:@selector(messageDidSend:)]) {
            [self.sendReceiveDelegate messageDidSend:message];
        }
        
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(message);
            }
        });
    } error:^(JErrorCodeInternal errorCode, long long clientMsgNo) {
        JLogI(@"MSG-Send", @"error, code is %lu", errorCode);
        message.messageState = JMessageStateFail;
        [self.core.dbManager messageSendFail:clientMsgNo];
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)errorCode, message);
            }
        });
    }];
}

- (JMessage *)sendMessage:(JMessageContent *)content
           inConversation:(JConversation *)conversation
              isBroadcast:(BOOL)isBroadcast
                  success:(void (^)(JMessage *message))successBlock
                    error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock {
    JConcreteMessage *message = [self saveMessageWithContent:content
                                              inConversation:conversation
                                                       state:JMessageStateSending
                                                   direction:JMessageDirectionSend
                                                 isBroadcast:isBroadcast];
    [self sendWebSocketMessage:message
                   isBroadcast:isBroadcast
                       success:successBlock
                         error:errorBlock];
    return message;
}

- (JConcreteMessage *)saveMessageWithContent:(JMessageContent *)content
                              inConversation:(JConversation *)conversation
                                       state:(JMessageState)state
                                   direction:(JMessageDirection)direction
                                 isBroadcast:(BOOL)isBroadcast {
    JConcreteMessage *message = [[JConcreteMessage alloc] init];
    message.content = content;
    message.conversation = conversation;
    message.contentType = [[content class] contentType];
    message.direction = direction;
    message.messageState = state;
    message.senderUserId = self.core.userId;
    message.clientUid = [self createClientUid];
    message.timestamp = [[NSDate date] timeIntervalSince1970] * 1000;
    message.flags = [[content class] flags];
    if (isBroadcast) {
        message.flags |= JMessageFlagIsBroadcast;
    }
    
    [self.core.dbManager insertMessages:@[message]];
    
    if ([self.sendReceiveDelegate respondsToSelector:@selector(messageDidSave:)]) {
        [self.sendReceiveDelegate messageDidSave:message];
    }
    
    return message;
}


- (NSArray <JConcreteMessage *> *)messagesToSave:(NSArray <JConcreteMessage *> *)messages {
    NSMutableArray <JConcreteMessage *> *arr = [[NSMutableArray alloc] init];
    for (JConcreteMessage *message in messages) {
        if (message.flags & JMessageFlagIsSave) {
            [arr addObject:message];
        }
    }
    return arr;
}

- (JMessage *)handleRecallCmdMessage:(NSString *)messageId extra:(NSDictionary *)extra {
    if(messageId == nil){
        return nil;
    }
    JRecallInfoMessage *recallInfoMsg = [[JRecallInfoMessage alloc] init];
    recallInfoMsg.exts = extra;
    [self.core.dbManager updateMessageContent:recallInfoMsg
                                  contentType:[JRecallInfoMessage contentType]
                                withMessageId:messageId];
    NSArray <JMessage *> *messages = [self.core.dbManager getMessagesByMessageIds:@[messageId]];
    if (messages.count > 0) {
        return messages[0];
    }
    return nil;
}

- (void)handleDeleteMsgMessageCmdMessage:(JConcreteMessage *)message{
    JDeleteMsgMessage * content = (JDeleteMsgMessage *)message.content;
    
    NSArray * messageList = [self.core.dbManager getMessagesByMessageIds:content.msgIdList];
    if(messageList == nil || messageList.count == 0){
        return;
    }
    
    NSMutableArray * clientMsgNos = [NSMutableArray array];
    NSMutableArray * messageIds = [NSMutableArray array];
    for (JMessage * message in messageList) {
        [clientMsgNos addObject:@(message.clientMsgNo)];
        [messageIds addObject:message.messageId];
    }
    [self.core.dbManager deleteMessageByMessageIds:messageIds];

    dispatch_async(self.core.delegateQueue, ^{
        if(self.delegate && [self.delegate respondsToSelector:@selector(messageDidDelete:clientMsgNos:)]){
            [self.delegate messageDidDelete:message.conversation clientMsgNos:clientMsgNos];
        }
    });
#warning TODO 通知会话更新
}

- (void)handleClearHistoryMessageCmdMessage:(JConcreteMessage *)message{
    JCleanMsgMessage * content = (JCleanMsgMessage *)message.content;
    
    long long starTime = content.cleanTime;
    if(starTime == 0){
        starTime = [[NSDate date] timeIntervalSince1970] * 1000;
    }
    
    [self.core.dbManager clearMessagesIn:message.conversation startTime:starTime senderId:content.senderId];
    dispatch_async(self.core.delegateQueue, ^{
        if(self.delegate && [self.delegate respondsToSelector:@selector(messageDidClear:timestamp:senderId:)]){
            [self.delegate messageDidClear:message.conversation
                                 timestamp:starTime
                                  senderId:content.senderId];
        }
    });
#warning TODO 通知会话更新
}

- (void)handleLogCommandMessage:(JConcreteMessage *)message {
    JLogCommandMessage *content = (JLogCommandMessage *)message.content;
    
    [JLogger.shared uploadLog:content.startTime
                      endTime:content.endTime
                       appKey:self.core.appKey
                        token:self.core.token];
}

- (void)handleReceiveMessages:(NSArray<JConcreteMessage *> *)messages
                       isSync:(BOOL)isSync {
    NSArray <JConcreteMessage *> *messagesToSave = [self messagesToSave:messages];
    [self.core.dbManager insertMessages:messagesToSave];
    [self updateUserInfos:messagesToSave];
    //TODO: 遍历 messagesToSave，同一个会话的最后一个 message 更新会话的 lastMessage
    
    __block long long sendTime = 0;
    __block long long receiveTime = 0;
    NSMutableDictionary *userDic = [NSMutableDictionary dictionary];
    [messages enumerateObjectsUsingBlock:^(JConcreteMessage * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if (obj.direction == JMessageDirectionSend) {
            sendTime = obj.timestamp;
        } else if (obj.direction == JMessageDirectionReceive) {
            receiveTime = obj.timestamp;
        }
        
        //recall message
        if ([obj.contentType isEqualToString:[JRecallCmdMessage contentType]]) {
            JRecallCmdMessage *cmd = (JRecallCmdMessage *)obj.content;
            JMessage *recallMessage = [self handleRecallCmdMessage:cmd.originalMessageId extra:cmd.extra];
            //recallMessage 为空表示被撤回的消息本地不存在，不需要回调
            if (recallMessage) {
                dispatch_async(self.core.delegateQueue, ^{
                    if ([self.delegate respondsToSelector:@selector(messageDidRecall:)]) {
                        [self.delegate messageDidRecall:recallMessage];
                    }
                });
            }
            return;
        }
        //delete conversation
        if ([obj.contentType isEqualToString:[JDeleteConvMessage contentType]]) {
            JDeleteConvMessage *deleteConvMsg = (JDeleteConvMessage *)obj.content;
            for (JConversation *deleteConversation in deleteConvMsg.conversations) {
                [self.core.dbManager deleteConversationInfoBy:deleteConversation];
            }
            if ([self.sendReceiveDelegate respondsToSelector:@selector(conversationsDidDelete:)]) {
                [self.sendReceiveDelegate conversationsDidDelete:deleteConvMsg.conversations];
            }
            return;
        }
        
        //read ntf
        if ([obj.contentType isEqualToString:[JReadNtfMessage contentType]]) {
            JReadNtfMessage *readNtfMsg = (JReadNtfMessage *)obj.content;
            [self.core.dbManager setMessagesRead:readNtfMsg.messageIds];
            dispatch_async(self.core.delegateQueue, ^{
                if ([self.readReceiptDelegate respondsToSelector:@selector(messagesDidRead:inConversation:)]) {
                    [self.readReceiptDelegate messagesDidRead:readNtfMsg.messageIds
                                               inConversation:obj.conversation];
                }
            });
            return;
        }
        
        //group read ntf
        if ([obj.contentType isEqualToString:[JGroupReadNtfMessage contentType]]) {
            JGroupReadNtfMessage *groupReadNtfMsg = (JGroupReadNtfMessage *)obj.content;
            [self.core.dbManager setGroupMessageReadInfo:groupReadNtfMsg.msgs];
            dispatch_async(self.core.delegateQueue, ^{
                if ([self.readReceiptDelegate respondsToSelector:@selector(groupMessagesDidRead:inConversation:)]) {
                    [self.readReceiptDelegate groupMessagesDidRead:groupReadNtfMsg.msgs
                                                    inConversation:obj.conversation];
                }
            });
            return;
        }
        
        //UnDisturb Top Clear unread
        if ([obj.contentType isEqualToString:[JUnDisturbConvMessage contentType]]||
            [obj.contentType isEqualToString:[JTopConvMessage contentType]]||
            [obj.contentType isEqualToString:[JClearUnreadMessage contentType]]) {
            if ([self.sendReceiveDelegate respondsToSelector:@selector(conversationsDidUpdate:)]) {
                [self.sendReceiveDelegate conversationsDidUpdate:obj];
            }
            return;
        }
        
        //delete Msg
        if ([obj.contentType isEqualToString:[JDeleteMsgMessage contentType]]) {
            [self handleDeleteMsgMessageCmdMessage:obj];
            return;
        }
        
        //clear Msg
        if ([obj.contentType isEqualToString:[JCleanMsgMessage contentType]]) {
            [self handleClearHistoryMessageCmdMessage:obj];
            return;
        }
        
        //log command
        if ([obj.contentType isEqualToString:[JLogCommandMessage contentType]]) {
            [self handleLogCommandMessage:obj];
            return;
        }
        
        if (obj.flags & JMessageFlagIsCmd) {
            return;
        }
        if (obj.existed) {
            return;
        }
        
        if (obj.content.mentionInfo) {
            for (JUserInfo *userInfo in obj.content.mentionInfo.targetUsers) {
                [userDic setObject:userInfo forKey:userInfo.userId];
            }
        }
        
        if ([self.sendReceiveDelegate respondsToSelector:@selector(messageDidReceive:)]) {
            [self.sendReceiveDelegate messageDidReceive:obj];
        }
        
        dispatch_async(self.core.delegateQueue, ^{
            if ([self.delegate respondsToSelector:@selector(messageDidReceive:)]) {
                [self.delegate messageDidReceive:obj];
            }
        });
    }];
    [self.core.dbManager insertUserInfos:userDic.allValues];
    //直发的消息，而且正在同步中，不直接更新 sync time
    if (!isSync && self.syncProcessing) {
        if (sendTime > 0) {
            self.cachedSendTime = sendTime;
        }
        if (receiveTime > 0) {
            self.cachedReceiveTime = receiveTime;
        }
    } else {
        if (sendTime > 0) {
            self.core.messageSendSyncTime = sendTime;
        }
        if (receiveTime > 0) {
            self.core.messageReceiveSyncTime = receiveTime;
        }
    }
}

- (void)sync {
    JLogI(@"MSG-Sync", @"receive time is %lld, send time is %lld", self.core.messageReceiveSyncTime, self.core.messageSendSyncTime);
    [self.core.webSocket syncMessagesWithReceiveTime:self.core.messageReceiveSyncTime
                                            sendTime:self.core.messageSendSyncTime
                                              userId:self.core.userId];
}

- (NSString *)createClientUid {
    long long ts = [[NSDate date] timeIntervalSince1970];
    ts = ts % 1000000;
    return [NSString stringWithFormat:@"%06lld%03d", ts, self.increaseId++];
}

- (void)updateUserInfos:(NSArray <JConcreteMessage *> *)messages {
    NSMutableDictionary *groupDic = [[NSMutableDictionary alloc] init];
    NSMutableDictionary *userDic = [[NSMutableDictionary alloc] init];
    [messages enumerateObjectsUsingBlock:^(JConcreteMessage * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
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

@end
