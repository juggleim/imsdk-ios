//
//  JMessageManager.m
// JuggleIM
//
//  Created by Nathan on 2023/11/29.
//

#import "JMessageManager.h"
#import "JuggleIMConst.h"
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
#import "JAddConvMessage.h"
#import "JClearTotalUnreadMessage.h"
#import "JMarkUnreadMessage.h"
#import "JLogger.h"
#import "JUploadManager.h"
#import "JDownloader.h"
#import "JDownloadManager.h"
#import "JUtility.h"
#import "JMsgModifyMessage.h"
#import "JMsgExSetMessage.h"
#import "JTagAddConvMessage.h"
#import "JTagDelConvMessage.h"
#import "JTopMsgMessage.h"
#import "JCallActiveCallMessage.h"

@interface JMessageManager () <JWebSocketMessageDelegate, JChatroomDelegate>
{
    id<JMessageUploadProvider> _uploadProvider;
}
@property (nonatomic, strong) JIMCore *core;
@property (nonatomic, strong) NSHashTable <id<JMessageDelegate>> *delegates;
@property (nonatomic, strong) NSHashTable <id<JMessageSyncDelegate>> *syncDelegates;
@property (nonatomic, strong) NSHashTable <id<JMessageReadReceiptDelegate>> *readReceiptDelegates;
@property (nonatomic, strong) NSHashTable <id<JMessageDestroyDelegate>> *destroyDelegates;
//@property (nonatomic, weak) id<JMessageUploadProvider> uploadProvider;
@property (nonatomic, strong) JDownloadManager *downloadManager;
@property (nonatomic, strong) JChatroomManager *chatroomManager;
@property (nonatomic, strong) JUserInfoManager *userInfoManager;
@property (nonatomic, strong) JCallManager *callManager;
//在 receiveQueue 里处理
@property (nonatomic, assign) BOOL syncProcessing;
@property (nonatomic, assign) long long cachedReceiveTime;
@property (nonatomic, assign) long long cachedSendTime;
@property (nonatomic, assign) long long syncNotifyTime;//发件箱
@property (nonatomic, assign) BOOL chatroomSyncProcessing;
@property (nonatomic, strong) NSMutableDictionary <NSString *, NSNumber *> *chatroomSyncDic;
@end

@implementation JMessageManager

@synthesize chatroomSyncProcessing = _chatroomSyncProcessing;

- (void)syncMessages {
    self.syncProcessing = YES;
    [self sync];
}

- (instancetype)initWithCore:(JIMCore *)core
             chatroomManager:(nonnull JChatroomManager *)chatroomManager
             userInfoManager:(nonnull JUserInfoManager *)userInfoManager
                 callManager:(nonnull JCallManager *)callManager {
    if (self = [super init]) {
        self.core = core;
        self.chatroomManager = chatroomManager;
        self.userInfoManager = userInfoManager;
        self.callManager = callManager;
        [self.chatroomManager addDelegate:self];
        [self.core.webSocket setMessageDelegate:self];
        [self registerMessages];
        self.cachedSendTime = -1;
        self.cachedReceiveTime = -1;
        self.syncProcessing = YES;
    }
    return self;
}

- (void)addDelegate:(id<JMessageDelegate>)delegate {
    dispatch_async(self.core.delegateQueue, ^{
        if (!delegate) {
            return;
        }
        [self.delegates addObject:delegate];
    });
}

- (void)addSyncDelegate:(id<JMessageSyncDelegate>)delegate {
    dispatch_async(self.core.delegateQueue, ^{
        if (!delegate) {
            return;
        }
        [self.syncDelegates addObject:delegate];
    });
}

- (void)addReadReceiptDelegate:(id<JMessageReadReceiptDelegate>)delegate {
    dispatch_async(self.core.delegateQueue, ^{
        if (!delegate) {
            return;
        }
        [self.readReceiptDelegates addObject:delegate];
    });
}

- (void)addDestroyDelegate:(id<JMessageDestroyDelegate>)delegate {
    dispatch_async(self.core.delegateQueue, ^{
        if (!delegate) {
            return;
        }
        [self.destroyDelegates addObject:delegate];
    });
}

- (void)setMessageUploadProvider:(id<JMessageUploadProvider>)uploadProvider {
    _uploadProvider = uploadProvider;
}

- (void)setPreprocessor:(id<JMessagePreprocessor>)preprocessor {
    [self.core.webSocket setMessagePreprocessor:preprocessor];
}

- (id<JMessageUploadProvider>)uploadProvider{
    if (_uploadProvider == nil) {
        _uploadProvider = [[JUploadManager alloc] initWithCore:self.core];
    }
    return _uploadProvider;
}

- (void)deleteMessagesByClientMsgNoList:(NSArray<NSNumber *> *)clientMsgNos
                           conversation:(JConversation *)conversation
                            forAllUsers:(BOOL)forAllUsers
                                success:(void (^)(void))successBlock
                                  error:(void (^)(JErrorCode))errorBlock {
    if (clientMsgNos == nil || clientMsgNos.count == 0 || conversation == nil) {
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
        if ([message.conversation isEqual:conversation]) {
            if (message.messageId.length > 0) {
                [deleteRemoteList addObject:message];
            }
            [deleteClientMsgNoList addObject:@(message.clientMsgNo)];
        }
    }
    JLogI(@"MSG-Delete", @"by clientMsgNo, local count is %lu, remote count is %lu", (unsigned long)deleteClientMsgNoList.count, (unsigned long)deleteRemoteList.count);
    if (deleteClientMsgNoList.count == 0) {
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeMessageNotExist);
            }
        });
        return;
    }
    //如果没有远端消息 只删除本地后直接回调
    if (deleteRemoteList.count == 0) {
        [self.core.dbManager deleteMessageByClientIds:deleteClientMsgNoList];
        [self notifyMessageRemoved:conversation removedMessages:messages];
        dispatch_async(self.core.delegateQueue, ^{
            if(successBlock){
                successBlock();
            }
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull dlg, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([dlg respondsToSelector:@selector(messageDidDelete:clientMsgNos:)]) {
                    [dlg messageDidDelete:conversation clientMsgNos:deleteClientMsgNoList];
                }
            }];
        });
        return;
    }
    
    //如果有远端消息则删除远端消息，操作本地数据 回调
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket deleteMessage:conversation
                               msgList:deleteRemoteList
                           forAllUsers:forAllUsers
                               success:^(long long timestamp) {
        JLogI(@"MSG-Delete", @"websocket success");
        [weakSelf updateSendSyncTime:timestamp];
        [weakSelf.core.dbManager deleteMessageByClientIds:deleteClientMsgNoList];
        [weakSelf notifyMessageRemoved:conversation removedMessages:deleteRemoteList];
        dispatch_async(self.core.delegateQueue, ^{
            if(successBlock){
                successBlock();
            }
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull dlg, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([dlg respondsToSelector:@selector(messageDidDelete:clientMsgNos:)]) {
                    [dlg messageDidDelete:conversation clientMsgNos:deleteClientMsgNoList];
                }
            }];
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

- (void)deleteMessagesByClientMsgNoList:(NSArray<NSNumber *> *)clientMsgNos
                           conversation:(JConversation *)conversation
                                success:(void (^)(void))successBlock
                                  error:(void (^)(JErrorCode))errorBlock {
    [self deleteMessagesByClientMsgNoList:clientMsgNos
                             conversation:conversation
                              forAllUsers:NO
                                  success:successBlock
                                    error:errorBlock];
}

- (void)deleteMessagesByMessageIds:(NSArray<NSString *> *)messageIds
                      conversation:(JConversation *)conversation
                       forAllUsers:(BOOL)forAllUsers
                           success:(void (^)(void))successBlock
                             error:(void (^)(JErrorCode))errorBlock {
    if (messageIds == nil || messageIds.count == 0 || conversation == nil) {
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam);
            }
        });
        return;
    }
    
    NSArray *messages = [self getMessagesByMessageIds:messageIds];
    NSMutableArray *msgList = [NSMutableArray array];
    NSMutableArray <NSNumber *> *clientMsgNos = [NSMutableArray array];
    for (JMessage * message in messages) {
        if ([message.conversation isEqual:conversation]) {
            [msgList addObject:message];
            [clientMsgNos addObject:@(message.clientMsgNo)];
        }
    }
    JLogI(@"MSG-Delete", @"by messageId, count is %lu", (unsigned long)msgList.count);
    if (msgList.count != 0) {
        __weak typeof(self) weakSelf = self;
        [self.core.webSocket deleteMessage:conversation
                                   msgList:msgList
                               forAllUsers:forAllUsers
                                   success:^(long long timestamp) {
            JLogI(@"MSG-Delete", @"websocket success");
            [weakSelf updateSendSyncTime:timestamp];
            NSMutableArray * ids = [NSMutableArray array];
            for (JMessage * message in msgList) {
                [ids addObject:message.messageId];
            }
            [weakSelf.core.dbManager deleteMessageByMessageIds:ids];
            [weakSelf notifyMessageRemoved:conversation removedMessages:msgList];
            dispatch_async(self.core.delegateQueue, ^{
                if(successBlock){
                    successBlock();
                }
                [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull dlg, NSUInteger idx, BOOL * _Nonnull stop) {
                    if ([dlg respondsToSelector:@selector(messageDidDelete:clientMsgNos:)]) {
                        [dlg messageDidDelete:conversation clientMsgNos:clientMsgNos];
                    }
                }];
            });
        } error:^(JErrorCodeInternal code) {
            JLogE(@"MSG-Delete", @"websocket error code is %lu", (unsigned long)code);
            dispatch_async(self.core.delegateQueue, ^{
                if (errorBlock) {
                    errorBlock((JErrorCode)code);
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

- (void)deleteMessagesByMessageIds:(NSArray<NSString *> *)messageIds
                      conversation:(JConversation *)conversation
                           success:(void (^)(void))successBlock
                             error:(void (^)(JErrorCode))errorBlock {
    [self deleteMessagesByMessageIds:messageIds
                        conversation:conversation
                         forAllUsers:NO
                             success:successBlock
                               error:errorBlock];
}

- (void)recallMessage:(NSString *)messageId
               extras:(NSDictionary *)extras
              success:(void (^)(JMessage *))successBlock
                error:(void (^)(JErrorCode))errorBlock {
    if (messageId.length == 0) {
        JLogE(@"MSG-Recall", @"messageId length is 0");
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam);
            }
        });
        return;
    }

    __block BOOL isAllString = YES;
    [extras enumerateKeysAndObjectsUsingBlock:^(id  _Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
        if (![key isKindOfClass:[NSString class]] || ![obj isKindOfClass:[NSString class]]) {
            isAllString = NO;
            *stop = YES;
        }
    }];
    if (!isAllString) {
        JLogE(@"MSG-Recall", @"recall message extras key/value must be NSString");
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeRecallExtrasTypeNotString);
            }
        });
    }
    
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
            [self updateSendSyncTime:timestamp];
            m.contentType = [JRecallInfoMessage contentType];
            JRecallInfoMessage *recallInfoMsg = [[JRecallInfoMessage alloc] init];
            recallInfoMsg.exts = extras;
            m.content = recallInfoMsg;
            [self.core.dbManager updateMessageContent:recallInfoMsg
                                          contentType:m.contentType
                                        withMessageId:messageId];
            [self notifyMessageRemoved:m.conversation removedMessages:@[(JConcreteMessage *)m]];
            dispatch_async(self.core.delegateQueue, ^{
                if (successBlock) {
                    successBlock(m);
                }
                [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull dlg, NSUInteger idx, BOOL * _Nonnull stop) {
                    if ([dlg respondsToSelector:@selector(messageDidRecall:)]) {
                        [dlg messageDidRecall:m];
                    }
                }];
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
            forAllUsers:(BOOL)forAllUsers
                success:(void (^)(void))successBlock
                  error:(void (^)(JErrorCode))errorBlock {
    if (startTime == 0) {
        long long currentTime =  [[NSDate date] timeIntervalSince1970] * 1000;
        startTime =  MAX(MAX(self.core.messageSendSyncTime, self.core.messageReceiveSyncTime), currentTime);
    }
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket clearHistoryMessage:conversation
                                        time:startTime
                                 forAllUsers:forAllUsers
                                     success:^(long long timestamp) {
        JLogI(@"MSG-Clear", @"success");
        [weakSelf updateSendSyncTime:timestamp];
        [weakSelf.core.dbManager clearMessagesIn:conversation startTime:startTime senderId:@""];
        [weakSelf notifyMessageCleared:conversation startTime:startTime sendUserId:nil];
        
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock();
            }
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull dlg, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([dlg respondsToSelector:@selector(messageDidClear:timestamp:senderId:)]) {
                    [dlg messageDidClear:conversation timestamp:startTime senderId:nil];
                }
            }];
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

- (void)clearMessagesIn:(JConversation *)conversation
              startTime:(long long)startTime
                success:(void (^)(void))successBlock
                  error:(void (^)(JErrorCode errorCode))errorBlock {
    [self clearMessagesIn:conversation
                startTime:startTime
              forAllUsers:NO
                  success:successBlock
                    error:errorBlock];
}

-(void)notifyMessageRemoved:(JConversation *)conversation removedMessages:(NSArray <JConcreteMessage *> *)removedMessages{
    if ([self.sendReceiveDelegate respondsToSelector:@selector(messageDidRemove:removedMessages:lastMessage:)]) {
        long long now = [self.core getCurrentTime];
        JConcreteMessage * lastMessage = [self.core.dbManager getLastMessage:conversation currentTime:now];
        [self.sendReceiveDelegate messageDidRemove:conversation removedMessages:removedMessages lastMessage:lastMessage];
    }
}
-(void)notifyMessageCleared:(JConversation *)conversation startTime:(long long)startTime sendUserId:(NSString *)sendUserId{
    if ([self.sendReceiveDelegate respondsToSelector:@selector(messageDidClear:startTime:sendUserId:lastMessage:)]) {
        long long now = [self.core getCurrentTime];
        JConcreteMessage * lastMessage = [self.core.dbManager getLastMessage:conversation currentTime:now];
        [self.sendReceiveDelegate messageDidClear:conversation startTime:startTime sendUserId:sendUserId lastMessage:lastMessage];

    }
}

- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction {
    if (!conversation) {
        return [NSArray array];
    }
    JQueryMessageOptions *options = [[JQueryMessageOptions alloc] init];
    options.conversations = @[conversation];
    if (count > 100) {
        count = 100;
    }
    return [self getMessages:count
                        time:time
                   direction:direction
                 queryOption:options];
}

- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction
                            contentTypes:(NSArray<NSString *> *)contentTypes {
    if (!conversation) {
        return [NSArray array];
    }
    JQueryMessageOptions *options = [[JQueryMessageOptions alloc] init];
    options.conversations = @[conversation];
    options.contentTypes = contentTypes;
    if (count > 100) {
        count = 100;
    }
    return [self getMessages:count
                        time:time
                   direction:direction
                 queryOption:options];
}

- (NSArray<JMessage *> *)getMessages:(int)count
                                time:(long long)time
                           direction:(JPullDirection)direction
                         queryOption:(JQueryMessageOptions *)option {
    if (count > 100) {
        count = 100;
    }
    long long now = [self.core getCurrentTime];
    return [self.core.dbManager searchMessagesWithContent:option.searchContent
                                                    count:count
                                                     time:time
                                            pullDirection:direction
                                             contentTypes:option.contentTypes
                                                  senders:option.senderUserIds
                                                   states:option.states
                                            conversations:option.conversations
                                        conversationTypes:option.conversationTypes
                                              currentTime:now];
}

- (JMessage *)saveMessage:(JMessageContent *)content
           inConversation:(JConversation *)conversation
                direction:(JMessageDirection)direction{
    return [self saveMessage:content
               messageOption:nil
              inConversation:conversation
                   direction:direction];
}

- (JMessage *)saveMessage:(JMessageContent *)content
            messageOption:(JMessageOptions *)messageOption
           inConversation:(JConversation *)conversation
                direction:(JMessageDirection)direction{
    return [self saveMessageWithContent:content
                          messageOption:messageOption
                         inConversation:conversation
                                  state:JMessageStateUnknown
                              direction:direction
                            isBroadcast:NO];
}

- (void)setMessageState:(JMessageState)state
        withClientMsgNo:(long long)clientMsgNo {
    [self.core.dbManager setMessageState:state
                         withClientMsgNo:clientMsgNo];
    NSArray<JMessage *> * messages = [self getMessagesByClientMsgNos:@[@(clientMsgNo)]];
    if ([self.sendReceiveDelegate respondsToSelector:@selector(messageStateDidChange:conversation:clientMsgNo:)] && messages != nil && messages.count != 0) {
        JMessage * message = messages.firstObject;
        if (message.conversation.conversationType != JConversationTypeChatroom) {
            [self.sendReceiveDelegate messageStateDidChange:state conversation:message.conversation clientMsgNo:clientMsgNo];
        }
    }
}

/// 搜素本地消息 SearchContent内容包含 指定内容
/// - Parameter searchContent: 需要包含的字符串
- (NSArray<JMessage *> *)searchMessagesWithContent:(NSString *)searchContent
                                    inConversation:(JConversation *)conversation
                                             count:(int)count
                                              time:(long long)time
                                         direction:(JPullDirection)direction
                                      contentTypes:(NSArray<NSString *> *)contentTypes {
    if (!conversation || conversation.conversationId.length == 0) {
        return [NSArray array];
    }
    if (count > 100) {
        count = 100;
    }
    long long now = [self.core getCurrentTime];
    return [self.core.dbManager searchMessagesWithContent:searchContent
                                                    count:count
                                                     time:time
                                            pullDirection:direction
                                             contentTypes:contentTypes
                                                  senders:nil
                                                   states:nil
                                            conversations:@[conversation]
                                        conversationTypes:nil
                                              currentTime:now];
}

- (void)searchConversationsWithMessageContent:(JQueryMessageOptions *)option
                                     complete:(void (^)(NSArray<JSearchConversationsResult *> *))completeBlock {
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        long long now = [self.core getCurrentTime];
        NSArray *result = [self.core.dbManager searchMessageInConversations:option currentTime:now];
        dispatch_async(self.core.delegateQueue, ^{
            if (completeBlock) {
                completeBlock(result);
            }
        });
    });
}

- (JMessage *)sendMessage:(JMessageContent *)content
           inConversation:(JConversation *)conversation
                  success:(void (^)(JMessage *message))successBlock
                    error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock{
    return [self sendMessage:content
               messageOption:nil
              inConversation:conversation
                 isBroadcast:NO
                     success:successBlock
                       error:errorBlock];
}

- (JMessage *)sendMessage:(JMessageContent *)content
            messageOption:(JMessageOptions *)messageOption
           inConversation:(JConversation *)conversation
                  success:(void (^)(JMessage *message))successBlock
                    error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock{
    return [self sendMessage:content
               messageOption:messageOption
              inConversation:conversation
                 isBroadcast:NO
                     success:successBlock
                       error:errorBlock];
}

- (JMessage *)sendMediaMessage:(JMediaMessageContent *)content
                inConversation:(JConversation *)conversation
                      progress:(void (^)(int progress, JMessage *message))progressBlock
                       success:(void (^)(JMessage *message))successBlock
                         error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock
                        cancel:(void (^)(JMessage *message))cancelBlock{
    return [self sendMediaMessage:content
                    messageOption:nil
                   inConversation:conversation
                         progress:progressBlock
                          success:successBlock
                            error:errorBlock
                           cancel:cancelBlock];
}

- (JMessage *)sendMediaMessage:(JMediaMessageContent *)content
                 messageOption:(JMessageOptions *)messageOption
                inConversation:(JConversation *)conversation
                      progress:(void (^)(int progress, JMessage *message))progressBlock
                       success:(void (^)(JMessage *message))successBlock
                         error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock
                        cancel:(void (^)(JMessage *message))cancelBlock{
    if (![content isKindOfClass:[JMediaMessageContent class]]) {
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam, nil);
            }
        });
        return nil;
    }
    JConcreteMessage *message = [self saveMessageWithContent:content
                                               messageOption:messageOption
                                              inConversation:conversation
                                                       state:JMessageStateUploading
                                                   direction:JMessageDirectionSend
                                                 isBroadcast:NO];
    return [self sendMediaMessage:message
                         progress:progressBlock
                          success:successBlock
                            error:errorBlock
                           cancel:cancelBlock];
}

-(JMessage *)sendMediaMessage:(JMessage *)message
                     progress:(void (^)(int progress, JMessage *message))progressBlock
                      success:(void (^)(JMessage *message))successBlock
                        error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock
                       cancel:(void (^)(JMessage *message))cancelBlock{
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
                [self setMessageState:JMessageStateFail withClientMsgNo:uploadMessage.clientMsgNo];
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
            [self setMessageState:JMessageStateSending withClientMsgNo:cm.clientMsgNo];
            [self sendWebSocketMessage:cm
                           isBroadcast:NO
                               success:successBlock
                                 error:errorBlock];
            
        } error:^{
            message.messageState = JMessageStateFail;
            [self setMessageState:JMessageStateFail withClientMsgNo:message.clientMsgNo];
            dispatch_async(self.core.delegateQueue, ^{
                if (errorBlock) {
                    errorBlock(JErrorCodeMessageUploadError, message);
                }
            });
        } cancel:^{
            message.messageState = JMessageStateFail;
            [self setMessageState:JMessageStateFail withClientMsgNo:message.clientMsgNo];
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
                                 success:^(long long timestamp) {
        [self updateSendSyncTime:timestamp];
        [self.core.dbManager setMessagesRead:messageIds
                                    readTime:timestamp];
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock();
            }
        });
        if ([self.sendReceiveDelegate respondsToSelector:@selector(messageDidRead:messageIds:)]) {
            [self.sendReceiveDelegate messageDidRead:conversation messageIds:messageIds];
        }
        NSArray <JMessage *> *readMessages = [self.core.dbManager getMessagesByMessageIds:messageIds];
        [readMessages enumerateObjectsUsingBlock:^(JMessage * _Nonnull readMessage, NSUInteger idx, BOOL * _Nonnull stop) {
            [self checkAndNotifyDestroyTime:readMessage time:timestamp];
        }];
    } error:^(JErrorCodeInternal code) {
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)getGroupMessageReadInfoDetail:(NSString *)messageId
                       inConversation:(JConversation *)conversation
                              success:(void (^)(JGroupMessageReadInfoDetail *))successBlock
                                error:(void (^)(JErrorCode))errorBlock {
    [self.core.webSocket getGroupMessageReadInfoDetail:messageId
                                        inConversation:conversation
                                               success:^(JGroupMessageReadInfoDetail * _Nonnull detail) {
        JLogI(@"MSG-GroupReadInfoDetail", @"success");
        JGroupMessageReadInfo *info = [[JGroupMessageReadInfo alloc] init];
        info.readCount = detail.readCount;
        info.memberCount = detail.memberCount;
        [self.core.dbManager setGroupMessageReadInfo:@{messageId:info}];
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(detail);
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-GroupReadInfoDetail", @"error code is %lu", code);
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (long long)getMessageReadTime:(long long)clientMsgNo {
    NSArray <JMessage *> *messages = [self getMessagesByClientMsgNos:@[@(clientMsgNo)]];
    if (messages.count > 0) {
        JConcreteMessage *m = (JConcreteMessage *)messages[0];
        return m.readTime;
    }
    return 0;
}

- (JMessage *)resend:(JMessage *)message
             success:(void (^)(JMessage *))successBlock
               error:(void (^)(JErrorCode, JMessage *))errorBlock {
    if (message.clientMsgNo <= 0
        || !message.content
        || message.conversation.conversationId.length == 0
        || ![message isKindOfClass:[JConcreteMessage class]]) {
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam, message);
            }
        });
        return message;
    }
    if (message.clientMsgNo > 0) {
        if (message.messageState == JMessageStateSent) {
            dispatch_async(self.core.delegateQueue, ^{
                if (successBlock) {
                    successBlock(message);
                }
            });
            return message;
        }
        if (message.messageState != JMessageStateSending) {
            message.messageState = JMessageStateSending;
            [self setMessageState:JMessageStateSending withClientMsgNo:message.clientMsgNo];
        }
        [self updateMessageWithContent:(JConcreteMessage *)message];
        [self sendWebSocketMessage:(JConcreteMessage *)message
                       isBroadcast:NO
                           success:successBlock
                             error:errorBlock];
        return message;
    } else {
        JMessageOptions * messageOptions = [[JMessageOptions alloc] init];
        messageOptions.mentionInfo = message.mentionInfo;
        messageOptions.referredMsgId = message.referredMsg.messageId;
        JConcreteMessage *concreteMessage = (JConcreteMessage *)message;
        messageOptions.lifeTime = concreteMessage.lifeTime;
        messageOptions.lifeTimeAfterRead = concreteMessage.lifeTimeAfterRead;
        return [self sendMessage:message.content
                   messageOption:messageOptions
                  inConversation:message.conversation
                         success:successBlock
                           error:errorBlock];
    }
}

- (JMessage *)resendMediaMessage:(JMessage *)message
                        progress:(void (^)(int progress, JMessage *message))progressBlock
                         success:(void (^)(JMessage *message))successBlock
                           error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock
                          cancel:(void (^)(JMessage *message))cancelBlock{
    if(message.clientMsgNo <= 0 ||
       !(message.messageId == nil || message.messageId.length == 0) ||   //已发送的消息不允许重发
       message.content == nil ||
       ![message.content isKindOfClass:[JMediaMessageContent class]] ||
       message.conversation == nil ||
       (message.conversation.conversationId == nil || message.conversation.conversationId.length == 0)){
        if(errorBlock){
            errorBlock(JErrorCodeInvalidParam,message);
        }
        return message;
    }
    
    if (message.messageState == JMessageStateSent) {
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(message);
            }
        });
        return message;
    }
    
    if(message.messageState != JMessageStateSending){
        message.messageState = JMessageStateSending;
        [self setMessageState:JMessageStateSending withClientMsgNo:message.clientMsgNo];
    }
    [self updateMessageWithContent:(JConcreteMessage *)message];
    
    return [self sendMediaMessage:message
                         progress:progressBlock
                          success:successBlock
                            error:errorBlock
                           cancel:cancelBlock];
    
}

- (void)getRemoteMessagesFrom:(JConversation *)conversation
                    startTime:(long long)startTime
                        count:(int)count
                    direction:(JPullDirection)direction
                      success:(void (^)(NSArray *messages, BOOL isFinished))successBlock
                        error:(void (^)(JErrorCode code))errorBlock {
    [self getRemoteMessagesFrom:conversation
                      startTime:startTime
                          count:count
                      direction:direction
                   contentTypes:nil
                        success:successBlock
                          error:errorBlock];
}

- (void)getRemoteMessagesFrom:(JConversation *)conversation
                    startTime:(long long)startTime
                        count:(int)count
                    direction:(JPullDirection)direction
                 contentTypes:(NSArray <NSString *> *)contentTypes
                      success:(void (^)(NSArray *messages, BOOL isFinished))successBlock
                        error:(void (^)(JErrorCode code))errorBlock {
    if (count > 100) {
        count = 100;
    }
    [self internalGetRemoteMessagesFrom:conversation
                              startTime:startTime
                                  count:count
                              direction:direction
                           contentTypes:contentTypes
                                success:successBlock
                                  error:errorBlock];
}

- (void)internalGetRemoteMessagesFrom:(JConversation *)conversation
                            startTime:(long long)startTime
                                count:(int)count
                            direction:(JPullDirection)direction
                         contentTypes:(NSArray <NSString *> *)contentTypes
                              success:(void (^)(NSArray *messages, BOOL isFinished))successBlock
                                error:(void (^)(JErrorCode code))errorBlock {
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket queryHisMsgsFrom:conversation
                                startTime:startTime
                                    count:count
                                direction:direction
                             contentTypes:contentTypes
                                  success:^(NSArray * _Nonnull messages, BOOL isFinished) {
        JLogI(@"MSG-Get", @"success");
        //TODO: 拉取的历史消息，重复的本地消息直接覆盖，clientMsgNo 不变，MediaMessageContent 的 localPath 不变，其它字段覆盖
        //远端消息中间有断档的情况下，表示远端删了而本地没跟进，需要把本地对应的范围删掉
        [self insertRemoteMessages:messages];
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(messages, isFinished);
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-Get", @"error code is %lu", code);
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)getMessages:(JConversation *)conversation
          direction:(JPullDirection)direction
             option:(JGetMessageOptions *)option
           complete:(void (^)(NSArray<JMessage *> *, long long, BOOL, JErrorCode))completeBlock {
    if (conversation.conversationId.length == 0) {
        dispatch_async(self.core.delegateQueue, ^{
            if (completeBlock) {
                completeBlock(nil, 0, NO, JErrorCodeInvalidParam);
            }
        });
        return;
    }
    if (!option) {
        option = [[JGetMessageOptions alloc] init];
    }
    if (option.count <= 0 || option.count > 100) {
        option.count = 100;
    }
    long long now = [self.core getCurrentTime];
    NSArray *localMessages = [self.core.dbManager searchMessagesWithContent:nil
                                                                      count:option.count+1
                                                                       time:option.startTime
                                                              pullDirection:direction
                                                               contentTypes:option.contentTypes
                                                                    senders:nil
                                                                     states:nil
                                                              conversations:@[conversation]
                                                          conversationTypes:nil
                                                                currentTime:now];
    
    __block BOOL needRemote = NO;
    if (localMessages.count < option.count+1) {
        //本地数据小于需要拉取的数量
        needRemote = YES;
    } else {
        //查询逆向的消息，用于判断是否断档
        NSMutableArray *fullLocalMessages = [localMessages mutableCopy];
        if (option.startTime != 0) {
            JPullDirection reverseDirection;
            long long reverseTimestamp;
            if (direction == JPullDirectionNewer) {
                reverseDirection = JPullDirectionOlder;
                reverseTimestamp = option.startTime + 1;
            } else {
                reverseDirection = JPullDirectionNewer;
                reverseTimestamp = option.startTime - 1;
            }
            NSArray *exMessages = [self.core.dbManager searchMessagesWithContent:nil
                                                                           count:5
                                                                            time:reverseTimestamp
                                                                   pullDirection:reverseDirection
                                                                    contentTypes:option.contentTypes
                                                                         senders:nil
                                                                          states:nil
                                                                   conversations:@[conversation]
                                                               conversationTypes:nil
                                                                     currentTime:now];
            if (exMessages.count > 0) {
                if (direction == JPullDirectionNewer) {
                    fullLocalMessages = [NSMutableArray arrayWithArray:[exMessages arrayByAddingObjectsFromArray:localMessages]];
                } else {
                    fullLocalMessages = [NSMutableArray arrayWithArray:[localMessages arrayByAddingObjectsFromArray:exMessages]];
                }
            }
        }
        
        //判断是否连续
        __block long long seqNo = -1;
        [fullLocalMessages enumerateObjectsUsingBlock:^(JConcreteMessage * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if (obj.seqNo < 0) {
                return;
            }
            if (obj.messageState == JMessageStateSent && obj.seqNo > 0) {
                if (seqNo < 0) {
                    seqNo = obj.seqNo;
                } else {
                    if (obj.seqNo != ++seqNo) {
                        needRemote = YES;
                        *stop = YES;
                        return;
                    }
                }
            }
        }];
        if (!needRemote && option.startTime == 0 && direction == JPullDirectionOlder) {
            JConversationInfo *conversationInfo = [self.core.dbManager getConversationInfo:conversation];
            JConcreteMessage *conversationLastMessage = (JConcreteMessage *)conversationInfo.lastMessage;
            JConcreteMessage *localListLastMessage = localMessages.lastObject;
            if (conversationLastMessage.seqNo > localListLastMessage.seqNo) {
                needRemote = YES;
            }
        }
    }
    
    long long startTime = option.startTime;
    if (needRemote) {
        [self internalGetRemoteMessagesFrom:conversation
                                  startTime:startTime
                                      count:option.count+1
                                  direction:direction
                               contentTypes:option.contentTypes
                                    success:^(NSArray *messages, BOOL isFinished) {
            //合并
            NSMutableArray * messagesArray = [NSMutableArray array];
            [messagesArray addObjectsFromArray:messages];
            for (JMessage *localMessage in localMessages) {
                BOOL isContain = NO;
                for (JMessage *remoteMessage in messagesArray) {
                    if (localMessage.clientMsgNo == remoteMessage.clientMsgNo) {
                        if ([[localMessage.content class] isEqual:[remoteMessage.content class]]) {
                            remoteMessage.localAttribute = localMessage.localAttribute;
                        }
                        if ([localMessage.content isKindOfClass:[JImageMessage class]]
                            && [remoteMessage.content isKindOfClass:[JImageMessage class]]) {
                            JImageMessage *localImage = (JImageMessage *)localMessage.content;
                            JImageMessage *remoteImage = (JImageMessage *)remoteMessage.content;
                            remoteImage.thumbnailLocalPath = localImage.thumbnailLocalPath;
                            remoteImage.localPath = localImage.localPath;
                        } else if ([localMessage.content isKindOfClass:[JVideoMessage class]]
                                   && [remoteMessage.content isKindOfClass:[JVideoMessage class]]) {
                            JVideoMessage *localVideo = (JVideoMessage *)localMessage.content;
                            JVideoMessage *remoteVideo = (JVideoMessage *)remoteMessage.content;
                            remoteVideo.localPath = localVideo.localPath;
                            remoteVideo.snapshotLocalPath = localVideo.snapshotLocalPath;
                       } else if ([localMessage.content isKindOfClass:[JMediaMessageContent class]]
                            && [remoteMessage.content isKindOfClass:[JMediaMessageContent class]]) {
                            JMediaMessageContent *localContent = (JMediaMessageContent *)localMessage.content;
                            JMediaMessageContent *remoteContent = (JMediaMessageContent *)remoteMessage.content;
                            remoteContent.localPath = localContent.localPath;
                        }
                        isContain = YES;
                        break;
                    }
                }
                if (!isContain) {
                    [messagesArray addObject:localMessage];
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
            [self completeCallbackForGetMessages:completeBlock
                                        messages:ascArray
                                           count:option.count
                                       direction:direction
                                         hasMore:!isFinished
                                       errorCode:JErrorCodeNone
                                  getMessageTime:startTime];
        } error:^(JErrorCode code) {
            BOOL hasMore = YES;
            if (localMessages.count < option.count + 1) {
                hasMore = NO;
            }
            [self completeCallbackForGetMessages:completeBlock
                                        messages:localMessages
                                           count:option.count
                                       direction:direction
                                         hasMore:hasMore
                                       errorCode:code
                                  getMessageTime:startTime];
        }];
    } else {
        BOOL hasMore = YES;
        if (localMessages.count < option.count + 1) {
            hasMore = NO;
        }
        [self completeCallbackForGetMessages:completeBlock
                                    messages:localMessages
                                       count:option.count
                                   direction:direction
                                     hasMore:hasMore
                                   errorCode:JErrorCodeNone
                              getMessageTime:startTime];
    }
}

//- (void)getLocalAndRemoteMessagesFrom:(JConversation *)conversation
//                            startTime:(long long)startTime
//                                count:(int)count
//                            direction:(JPullDirection)direction
//                    localMessageBlock:(void (^)(NSArray <JMessage *> *messages,BOOL needRemote))localMessageBlock
//                   remoteMessageBlock:(void (^)(NSArray <JMessage *> *messages))remoteMessageBlock
//                                error:(void (^)(JErrorCode code))errorBlock{
//    if (conversation.conversationId.length == 0) {
//        dispatch_async(self.core.delegateQueue, ^{
//            if (errorBlock) {
//                errorBlock(JErrorCodeInvalidParam);
//            }
//        });
//        return;
//    }
//    if (count <= 0) {
//        dispatch_async(self.core.delegateQueue, ^{
//            if (localMessageBlock) {
//                NSArray *arr = [NSArray array];
//                localMessageBlock(arr, NO);
//            }
//        });
//        return;
//    }
//    if (count > 100) {
//        count = 100;
//    }
//    NSArray *localMessages = [self getMessagesFrom:conversation
//                                             count:count
//                                              time:startTime
//                                         direction:direction];
//    __block BOOL needRemote = NO;
//    //本地数据为空
//    if (localMessages.count == 0) {
//        needRemote = YES;
//    } else {
//        JConcreteMessage *message = localMessages[0];
//        __block long long seqNo = message.seqNo;
//        if(localMessages.count < count){
//            //本地数据小于需要拉取的数量
//            needRemote = YES;
//        } else {
//            //本地数据等于需要拉取的数据
//            //判断是否连续
//            [localMessages enumerateObjectsUsingBlock:^(JConcreteMessage *  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
//                if (idx > 0 && obj.messageState == JMessageStateSent && obj.seqNo != 0) {
//                    if (obj.seqNo > ++seqNo) {
//                        needRemote = YES;
//                        *stop = YES;
//                        return;
//                    }
//                }
//            }];
//        }
//    }
//    
//    dispatch_async(self.core.delegateQueue, ^{
//        if (localMessageBlock) {
//            localMessageBlock(localMessages, needRemote);
//        }
//    });
//    
//    if (needRemote) {
//        [self getRemoteMessagesFrom:conversation
//                          startTime:startTime
//                              count:count
//                          direction:direction
//                            success:^(NSArray *messages, BOOL isFinished) {
//            //合并
//            NSMutableArray * messagesArray = [NSMutableArray array];
//            [messagesArray addObjectsFromArray:localMessages];
//            for (JMessage * message in messages) {
//                if(![messagesArray containsObject:message]){
//                    [messagesArray addObject:message];
//                }
//            }
//            //正序排序
//            NSArray * ascArray = [messagesArray sortedArrayUsingComparator:^NSComparisonResult(JConcreteMessage *  _Nonnull msg1, JConcreteMessage *  _Nonnull msg2) {
//                if(msg1.timestamp < msg2.timestamp){
//                    return NSOrderedAscending;
//                }else if(msg1.timestamp > msg2.timestamp){
//                    return NSOrderedDescending;
//                }else{
//                    return NSOrderedSame;
//                }
//            }];
//            dispatch_async(self.core.delegateQueue, ^{
//                if (remoteMessageBlock) {
//                    remoteMessageBlock(ascArray);
//                }
//            });
//            
//        } error:errorBlock];
//    }
//}

- (void)getMessages:(JConversation *)conversation
          direction:(JPullDirection)direction
             option:(JGetMessageOptions *)option
  localMessageBlock:(void (^)(NSArray<JMessage *> *, JErrorCode))localMessageBlock
 remoteMessageBlock:(void (^)(NSArray<JMessage *> *, long long, BOOL, JErrorCode))remoteMessageBlock {
    if (conversation.conversationId.length == 0) {
        dispatch_async(self.core.delegateQueue, ^{
            if (localMessageBlock) {
                localMessageBlock(nil, JErrorCodeInvalidParam);
            }
            if (remoteMessageBlock) {
                remoteMessageBlock(nil, 0, NO, JErrorCodeInvalidParam);
            }
        });
        return;
    }
    if (!option) {
        option = [[JGetMessageOptions alloc] init];
    }
    if (option.count <= 0 || option.count > 100) {
        option.count = 100;
    }
    NSArray *localMessages = [self getMessagesFrom:conversation
                                             count:option.count
                                              time:option.startTime
                                         direction:direction
                                      contentTypes:option.contentTypes];
    
    dispatch_async(self.core.delegateQueue, ^{
        if (localMessageBlock) {
            localMessageBlock(localMessages, JErrorCodeNone);
        }
    });
    
    [self getRemoteMessagesFrom:conversation
                      startTime:option.startTime
                          count:option.count
                      direction:direction
                   contentTypes:option.contentTypes
                        success:^(NSArray *messages, BOOL isFinished) {
        dispatch_async(self.core.delegateQueue, ^{
            long long timestamp;
            if (direction == JPullDirectionNewer) {
                JMessage *m = messages.lastObject;
                timestamp = m.timestamp;
            } else {
                JMessage *m = messages.firstObject;
                timestamp = m.timestamp;
            }
            if (remoteMessageBlock) {
                remoteMessageBlock(messages, timestamp, !isFinished, JErrorCodeNone);
            }
        });
    } error:^(JErrorCode code) {
        dispatch_async(self.core.delegateQueue, ^{
            if (remoteMessageBlock) {
                remoteMessageBlock(nil, 0, NO, code);
            }
        });
    }];
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
            [self insertRemoteMessages:remoteMessages];
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
        [self updateUserInfos:messages];
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
                   success:(void (^)(NSArray<JMessage *> *, BOOL))successBlock
                     error:(void (^)(JErrorCode))errorBlock {
    if (count > 100) {
        count = 100;
    }
    JConcreteConversationInfo *conversationInfo = [self.core.dbManager getConversationInfo:conversation];
    [self.core.webSocket getMentionMessages:conversation
                                       time:time
                                      count:count
                                  direction:direction
                              lastReadIndex:conversationInfo.lastReadMessageIndex
                                    success:^(NSArray<JConcreteMessage *> * _Nonnull messages, BOOL isFinished) {
        JLogI(@"MSG-GetMention", @"success");
        [self insertRemoteMessages:messages];
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(messages, isFinished);
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

- (void)setTop:(BOOL)isTop
     messageId:(NSString *)messageId
  conversation:(JConversation *)conversation
       success:(void (^)(void))successBlock
         error:(void (^)(JErrorCode))errorBlock {
    if (messageId.length == 0 || conversation.conversationId.length == 0) {
        JLogE(@"MSG-SetTop", @"invalid parameter");
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam);
            }
        });
        return;
    }
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket setMessageTop:isTop
                             messageId:messageId
                          conversation:conversation
                               success:^(long long timestamp) {
        JLogI(@"MSG-SetTop", @"success");
        [weakSelf updateSendSyncTime:timestamp];
        long long now = [weakSelf.core getCurrentTime];
        JMessage *message = [weakSelf.core.dbManager getMessageWithMessageId:messageId currentTime:now];
        JUserInfo *user = [weakSelf.userInfoManager getUserInfo:weakSelf.core.userId];
        if (!user) {
            user = [JUserInfo new];
            user.userId = weakSelf.core.userId;
        }
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (successBlock) {
                successBlock();
            }
            if (message && user) {
                [weakSelf.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                    if ([obj respondsToSelector:@selector(messageDidSetTop:message:user:)]) {
                        [obj messageDidSetTop:isTop message:message user:user];
                    }
                }];
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-SetTop", @"error code is %lu", code);
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)getTopMessage:(JConversation *)conversation
              success:(void (^)(JMessage *message, JUserInfo *userInfo, long long timestamp))successBlock
                error:(void (^)(JErrorCode))errorBlock {
    if (conversation.conversationId.length == 0) {
        JLogE(@"MSG-GetTop", @"invalid parameter");
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam);
            }
        });
        return;
    }
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket getTopMessage:conversation
                               success:^(JConcreteMessage * _Nonnull message, JUserInfo * _Nonnull userInfo, long long timestamp) {
        JLogI(@"MSG-GetTop", @"success");
        [weakSelf insertRemoteMessages:@[message]];
        [weakSelf.userInfoManager insertUserInfoList:@[userInfo]];
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(message, userInfo, timestamp);
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-GetTop", @"error code is %lu", code);
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)addFavorite:(NSArray<NSString *> *)messageIdList
            success:(void (^)(void))successBlock
              error:(void (^)(JErrorCode))errorBlock {
    if (messageIdList.count == 0) {
        JLogE(@"MSG-AddFvr", @"invalid parameter");
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam);
            }
        });
        return;
    }
    NSArray <JMessage *> *messageList = [self getMessagesByMessageIds:messageIdList];
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket addFavoriteMessages:messageList
                                      userId:self.core.userId
                                     success:^(long long timestamp) {
        JLogI(@"MSG-AddFvr", @"success");
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (successBlock) {
                successBlock();
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-AddFvr", @"error, code is %lu", code);
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)removeFavorite:(NSArray<NSString *> *)messageIdList
               success:(void (^)(void))successBlock
                 error:(void (^)(JErrorCode))errorBlock {
    if (messageIdList.count == 0) {
        JLogE(@"MSG-RmFvr", @"invalid parameter");
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam);
            }
        });
        return;
    }
    NSArray <JMessage *> *messageList = [self getMessagesByMessageIds:messageIdList];
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket removeFavoriteMessages:messageList
                                         userId:self.core.userId
                                        success:^(long long timestamp) {
        JLogI(@"MSG-RmFvr", @"success");
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (successBlock) {
                successBlock();
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-RmFvr", @"error, code is %lu", code);
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)getFavorite:(JGetFavoriteMessageOption *)option
            success:(void (^)(NSArray<JFavoriteMessage *> *, NSString *))successBlock
              error:(void (^)(JErrorCode))errorBlock {
    if (option.count <= 0) {
        JLogE(@"MSG-GetFvr", @"invalid parameter");
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam);
            }
        });
        return;
    }
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket getFavoriteMessagesWithOffset:option.offset
                                                 limit:option.count
                                                userId:self.core.userId
                                               success:^(NSArray<JFavoriteMessage *> * _Nonnull messageList, NSString * _Nonnull offset) {
        JLogI(@"MSG-GetFvr", @"success");
        NSMutableArray *messages = [NSMutableArray array];
        for (JFavoriteMessage *favMessage in messageList) {
            [messages addObject:favMessage.message];
        }
        [self updateUserInfos:messages];
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(messageList, offset);
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-GetFvr", @"error, code is %lu", code);
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)downloadMediaMessage:(NSString *)messageId
                    progress:(void (^)(JMessage *, int))progressBlock
                     success:(void (^)(JMessage *))successBlock
                       error:(void (^)(JErrorCode))errorBlock {
    JMessage *message = [self.core.dbManager getMessageWithMessageId:messageId
                                                         currentTime:[self.core getCurrentTime]];
    if (!message) {
        JLogE(@"MSG-Download", @"can't find message with messageId %@", messageId);
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam);
            }
        });
        return;
    }
    JMessageContent *content = message.content;
    if (![content isKindOfClass:[JMediaMessageContent class]]) {
        JLogE(@"MSG-Download", @"content is not a JMediaMessageContent");
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeDownloadNotMediaMessage);
            }
        });
        return;
    }
    JMediaMessageContent *mediaContent = (JMediaMessageContent *)content;
    NSString *localPath = mediaContent.localPath;
    if (localPath.length > 0) {
        NSFileManager *fileManager = [NSFileManager defaultManager];
        if ([fileManager fileExistsAtPath:localPath]) {
            dispatch_async(self.core.delegateQueue, ^{
                if (successBlock) {
                    successBlock(message);
                }
            });
            return;
        }
    }
    
    NSString *appKey = self.core.appKey;
    NSString *userId = self.core.userId;
    if (appKey.length == 0 || userId.length == 0) {
        JLogE(@"MSG-Download", @"appKey or userId length is 0");
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeConnectionUnavailable);
            }
        });
        return;
    }
    JMediaType type = JMediaTypeFile;
    if ([content isKindOfClass:[JImageMessage class]]) {
        type = JMediaTypeImage;
    } else if ([content isKindOfClass:[JVoiceMessage class]]) {
        type = JMediaTypeVoice;
    } else if ([content isKindOfClass:[JVideoMessage class]]) {
        type = JMediaTypeVideo;
    }
    NSString *url = mediaContent.url;
    NSString *name = [messageId stringByAppendingFormat:@"_%@", [self getFileNameWith:url]];
    NSString *path = [self generateLocalPath:type name:name];
    [self.downloadManager downloadWithMessageId:messageId
                                            Url:url
                                           path:path
                                       progress:^(int progress) {
        dispatch_async(self.core.delegateQueue, ^{
            if (progressBlock) {
                progressBlock(message, progress);
            }
        });
    } success:^(NSString * _Nonnull localPath) {
        mediaContent.localPath = localPath;
        [self.core.dbManager updateMessageContent:mediaContent contentType:message.contentType withMessageId:messageId];
        if ([self.sendReceiveDelegate respondsToSelector:@selector(messageDidUpdate:)]) {
            [self.sendReceiveDelegate messageDidUpdate:(JConcreteMessage *)message];
        }
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(message);
            }
        });
    } error:^(JErrorCode errorCode) {
        JLogE(@"MSG-Download", @"download fail, code is %ld", errorCode);
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(errorCode);
            }
        });
    }];
}

- (void)cancelDownloadMediaMessage:(NSString *)messageId {
    [self.downloadManager cancelDownload:messageId];
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

- (void)addMessageReaction:(NSString *)messageId
              conversation:(JConversation *)conversation
                reactionId:(NSString *)reactionId
                   success:(void (^)(void))successBlock
                     error:(void (^)(JErrorCode))errorBlock {
    if (messageId.length == 0 ||
        conversation.conversationId.length == 0 ||
        reactionId.length == 0) {
        JLogE(@"MSG-ReactionAdd", @"invalid parameter");
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam);
            }
        });
        return;
    }
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket addMessageReaction:messageId
                               conversation:conversation
                                 reactionId:reactionId
                                     userId:self.core.userId
                                    success:^(long long timestamp) {
        JLogI(@"MSG-ReactionAdd", @"success");
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (successBlock) {
                successBlock();
            }
            JUserInfo *currentUser = [weakSelf.userInfoManager getUserInfo:weakSelf.core.userId];
            if (!currentUser) {
                currentUser = [[JUserInfo alloc] init];
                currentUser.userId = weakSelf.core.userId;
            }
            
            //callback delegate
            //callback 只有新增的，不用本地做合并，因为本地不全（特别是收到别的用户的 reaction 时，不能返回不全的数据）
            JMessageReaction *reaction = [[JMessageReaction alloc] init];
            reaction.messageId = messageId;
            JMessageReactionItem *item = [[JMessageReactionItem alloc] init];
            item.reactionId = reactionId;
            item.userInfoList = @[currentUser];
            reaction.itemList = @[item];
            
            //update reaction db
            NSArray <JMessageReaction *> *dbReactions = [weakSelf.core.dbManager getMessageReactions:@[messageId]];
            if (dbReactions.count > 0) {
                JMessageReaction *dbReaction = dbReactions[0];
                for (JMessageReactionItem *item in dbReaction.itemList) {
                    if ([reactionId isEqualToString:item.reactionId]) {
                        NSMutableArray *userInfoList = [item.userInfoList mutableCopy];
                        [userInfoList addObject:currentUser];
                        item.userInfoList = [userInfoList copy];
                        break;
                    }
                }
                [weakSelf.core.dbManager setMessageReactions:@[dbReaction]];
            } else {
                [weakSelf.core.dbManager setMessageReactions:@[reaction]];
            }
            
            [weakSelf.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(messageReactionDidAdd:inConversation:)]) {
                    [obj messageReactionDidAdd:reaction inConversation:conversation];
                }
            }];
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-ReactionAdd", @"error, code is %ld", code);
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)removeMessageReaction:(NSString *)messageId
                 conversation:(JConversation *)conversation
                   reactionId:(NSString *)reactionId
                      success:(void (^)(void))successBlock
                        error:(void (^)(JErrorCode))errorBlock {
    if (messageId.length == 0 ||
        conversation.conversationId.length == 0 ||
        reactionId.length == 0) {
        JLogE(@"MSG-ReactionRemove", @"invalid parameter");
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam);
            }
        });
        return;
    }
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket removeMessageReaction:messageId
                                  conversation:conversation
                                    reactionId:reactionId
                                        userId:self.core.userId
                                       success:^(long long timestamp) {
        JLogI(@"MSG-ReactionRemove", @"success");
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (successBlock) {
                successBlock();
            }
            
            // update reaction db
            NSArray <JMessageReaction *> *dbReactions = [weakSelf.core.dbManager getMessageReactions:@[messageId]];
            if (dbReactions.count > 0) {
                JMessageReaction *dbReaction = dbReactions[0];
                for (JMessageReactionItem *item in dbReaction.itemList) {
                    if ([reactionId isEqualToString:item.reactionId]) {
                        NSMutableArray *userInfoList = [NSMutableArray array];
                        for (JUserInfo *userInfo in item.userInfoList) {
                            if (![weakSelf.core.userId isEqualToString:userInfo.userId]) {
                                [userInfoList addObject:userInfo];
                            }
                        }
                        if (userInfoList.count == 0) {
                            NSMutableArray *l = [dbReaction.itemList mutableCopy];
                            [l removeObject:item];
                            dbReaction.itemList = [l copy];
                        } else {
                            item.userInfoList = [userInfoList copy];
                        }
                        break;
                    }
                }
                [weakSelf.core.dbManager setMessageReactions:@[dbReaction]];
            }
            
            //callback delegate
            //callback 只有新增的，不用本地做合并，因为本地不全（特别是收到别的用户的 reaction 时，不能返回不全的数据）
            JMessageReaction *reaction = [[JMessageReaction alloc] init];
            reaction.messageId = messageId;
            JMessageReactionItem *item = [[JMessageReactionItem alloc] init];
            item.reactionId = reactionId;
            JUserInfo *currentUser = [weakSelf.userInfoManager getUserInfo:weakSelf.core.userId];
            if (!currentUser) {
                currentUser = [[JUserInfo alloc] init];
                currentUser.userId = weakSelf.core.userId;
            }
            item.userInfoList = @[currentUser];
            reaction.itemList = @[item];
            [weakSelf.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(messageReactionDidRemove:inConversation:)]) {
                    [obj messageReactionDidRemove:reaction inConversation:conversation];
                }
            }];
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-ReactionRemove", @"error, code is %ld", code);
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)getMessagesReaction:(NSArray<NSString *> *)messageIdList
               conversation:(JConversation *)conversation
                    success:(void (^)(NSArray<JMessageReaction *> *))successBlock
                      error:(void (^)(JErrorCode))errorBlock {
    if (messageIdList.count == 0 ||
        conversation.conversationId.length == 0) {
        JLogE(@"MSG-ReactionGet", @"invalid parameter");
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam);
            }
        });
        return;
    }
    [self.core.webSocket getMessagesReaction:messageIdList
                                conversation:conversation
                                     success:^(NSArray<JMessageReaction *> * _Nonnull reactionList) {
        [self.core.dbManager setMessageReactions:reactionList];
        JLogI(@"MSG-ReactionGet", @"success");
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(reactionList);
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-ReactionGet", @"error, code is %ld", code);
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (NSArray<JMessageReaction *> *)getCachedMessagesReaction:(NSArray<NSString *> *)messageIdList {
    return [self.core.dbManager getMessageReactions:messageIdList];
}

- (void)setMute:(BOOL)isMute
        periods:(NSArray<JTimePeriod *> *)periods
       complete:(void (^)(JErrorCode))completeBlock {
    NSTimeZone *zone = [NSTimeZone systemTimeZone];
    NSString *zoneName = [zone name].length ? [zone name] : @"";
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket setGlobalMute:isMute
                                userId:self.core.userId
                              timezone:zoneName
                               periods:periods
                               success:^(long long timestamp) {
        JLogI(@"MSG-Mute", @"success");
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (completeBlock) {
                completeBlock(JErrorCodeNone);
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-Mute", @"code is %ld", code);
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (completeBlock) {
                completeBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)getFirstUnreadMessage:(JConversation *)conversation
                      success:(void (^)(JMessage *))successBlock
                        error:(void (^)(JErrorCode))errorBlock {
    [self.core.webSocket getFirstUnreadMessage:conversation
                                       success:^(NSArray<JConcreteMessage *> * _Nonnull messages, BOOL isFinished) {
        JLogI(@"MSG-FirstUnread", @"success");
        [self insertRemoteMessages:messages];
        dispatch_async(self.core.delegateQueue, ^{
            if (!successBlock) {
                return;
            }
            if (messages.count == 0) {
                successBlock(nil);
            } else {
                successBlock(messages.firstObject);
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-FirstUnread", @"code is %ld", code);
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)code);
            }
        });
    }];
}

- (void)getMuteStatus:(void (^)(JErrorCode, BOOL, NSString *, NSArray<JTimePeriod *> *))completeBlock {
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket getGlobalMute:self.core.userId
                               success:^(BOOL isMute, NSString * _Nonnull timezone, NSArray<JTimePeriod *> * _Nonnull periods) {
        JLogI(@"MSG-GetMute", @"success");
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (completeBlock) {
                completeBlock(JErrorCodeNone, isMute, timezone, periods);
            }
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-GetMute", @"code is %ld", code);
        dispatch_async(weakSelf.core.delegateQueue, ^{
            if (completeBlock) {
                completeBlock((JErrorCode)code, NO, nil, nil);
            }
        });
    }];
}

- (void)uploadImage:(UIImage *)image success:(void (^)(NSString *))successBlock error:(void (^)(JErrorCode))errorBlock {
    JUploadManager *uploader = [[JUploadManager alloc] initWithCore:self.core];
    [uploader uploadImage:image
                  success:^(NSString *url) {
        JLogI(@"MSG-UpldImg", @"url is %@", url);
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(url);
            }
        });
    } error:^(JErrorCode code) {
        JLogE(@"MSG-UpldImg", @"error, code is %ld", code);
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(code);
            }
        });
    }];
}

- (void)updateMessage:(JMessageContent *)content
            messageId:(NSString *)messageId
       inConversation:(JConversation *)conversation
              success:(void (^)(JMessage *))successBlock
                error:(void (^)(JErrorCode))errorBlock {
    if (messageId.length == 0) {
        JLogE(@"MSG-Update", @"messageId length is 0");
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam);
            }
        });
        return;
    }
    NSArray *messages = [self getMessagesByMessageIds:@[messageId]];
    if (messages.count > 0) {
        JConcreteMessage *m = messages[0];
        [self.core.webSocket updateMessage:messageId
                                   content:content
                              conversation:conversation
                                 timestamp:m.timestamp
                                  msgSeqNo:m.seqNo
                                   success:^(long long timestamp) {
            JLogI(@"MSG-Update", @"success");
            [self updateSendSyncTime:timestamp];
            if ([content isKindOfClass:[JUnknownMessage class]]) {
                JUnknownMessage *unknown = (JUnknownMessage *)content;
                m.contentType = unknown.messageType;
            } else {
                m.contentType = [[content class] contentType];
            }
            m.content = content;
            [self.core.dbManager updateMessageContent:content
                                          contentType:m.contentType
                                        withMessageId:messageId];
            int flags = m.flags | JMessageFlagIsModified;
            m.flags = flags;
            m.isEdit = YES;
            [self.core.dbManager setMessageFlags:flags withMessageId:messageId];
            if ([self.sendReceiveDelegate respondsToSelector:@selector(messageDidUpdate:)]) {
                [self.sendReceiveDelegate messageDidUpdate:m];
            }
            dispatch_async(self.core.delegateQueue, ^{
                if (successBlock) {
                    successBlock(m);
                }
                [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                    if ([obj respondsToSelector:@selector(messageDidUpdate:)]) {
                        [obj messageDidUpdate:m];
                    }
                }];
            });
        } error:^(JErrorCodeInternal errorCode) {
            JLogE(@"MSG-Update", @"error code is %ld", errorCode);
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

- (void)connectSuccess {
    self.syncProcessing = YES;
    self.syncNotifyTime = 0;
    self.chatroomSyncProcessing = NO;
    [self clearChatroomSyncDic];
}

#pragma mark - JChatroomProtocol
- (void)chatroomDidJoin:(NSString *)chatroomId {
    //确保后面会走 sync 逻辑
    long long time = [self.chatroomManager getSyncTimeForChatroom:chatroomId] + 1;
    [self syncChatroomNotify:chatroomId time:time];
}

- (void)chatroomDidDestroy:(NSString *)chatroomId {
}

- (void)chatroomDidKick:(NSString *)chatroomId {
}

- (void)chatroomDidQuit:(NSString *)chatroomId {
}

- (void)chatroomJoinFail:(NSString *)chatroomId errorCode:(JErrorCode)errorCode {
}

- (void)chatroomQuitFail:(NSString *)chatroomId errorCode:(JErrorCode)errorCode {
}

#pragma mark - JWebSocketMessageDelegate
- (BOOL)messageDidReceive:(JConcreteMessage *)message {
    JLogI(@"MSG-Rcv", @"direct message id is %@", message.messageId);
    // 只处理发件箱的消息，收件箱的消息直接抛弃（状态消息直接漏过）
    BOOL isStatusMessage = message.flags&JMessageFlagIsStatus;
    if (self.syncProcessing && !isStatusMessage) {
        if (message.direction == JMessageDirectionSend) {
            self.syncNotifyTime = message.timestamp;
        }
        return NO;
    }
    [self handleReceiveMessages:@[message]
                         isSync:NO];
    return YES;
}

- (void)syncNotify:(long long)syncTime {
    if (self.syncProcessing) {
        self.syncNotifyTime = syncTime;
        return;
    }
    if (syncTime > self.core.messageReceiveSyncTime) {
        self.syncProcessing = YES;
        [self sync];
    }
}

- (void)syncChatroomNotify:(NSString *)chatroomId time:(long long)syncTime {
    if (self.chatroomSyncProcessing) {
        [self setTimeForChatroomSyncDic:syncTime chatroomId:chatroomId];
        return;
    }
    [self syncChatroomMessages:chatroomId time:syncTime];
}

- (void)messageDidSend:(NSString *)messageId
                  time:(long long)timestamp
                 seqNo:(long long)seqNo
             clientUid:(NSString *)clientUid
           contentType:(nullable NSString *)contentType
               content:(nullable JMessageContent *)content
      groupMemberCount:(int)count {
    if (clientUid.length == 0) {
        return;
    }
    [self.core.dbManager updateMessageAfterSendWithClientUid:clientUid
                                                   messageId:messageId
                                                   timestamp:timestamp
                                                       seqNo:seqNo
                                            groupMemberCount:count];
    if (contentType && content) {
        [self.core.dbManager updateMessageContent:content
                                      contentType:contentType
                                    withMessageId:messageId];
    }
}

#pragma mark - internal
- (void)messagesDidReceive:(NSArray<JConcreteMessage *> *)messages
                isFinished:(BOOL)isFinished {
    JLogI(@"MSG-Rcv", @"message count is %ld, isFinish is %d", messages.count, isFinished);
    [self handleReceiveMessages:messages
                         isSync:YES];
    
    if (!isFinished) {
        [self sync];
    } else if (self.syncNotifyTime > self.core.messageSendSyncTime) {
        [self sync];
        self.syncNotifyTime = -1;
    }  else {
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
            [self.syncDelegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageSyncDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(messageSyncDidComplete)]) {
                    [obj messageSyncDidComplete];
                }
            }];
        });
    }
}

- (void)chatroomMessagesDidReceive:(NSArray<JConcreteMessage *> *)messages {
    JLogI(@"MSG-Rcv", @"chatroom message count is %ld", messages.count);
    if (messages.count == 0) {
        [self checkChatroomSyncDic];
        return;
    }
    NSArray <JConcreteMessage *> *messagesToSave = [self messagesToSave:messages];
    [self insertRemoteMessages:messagesToSave];
    
    JConcreteMessage *lastMessage = messages.lastObject;
    [self.chatroomManager setSyncTime:lastMessage.timestamp forChatroom:lastMessage.conversation.conversationId];
    
    [messages enumerateObjectsUsingBlock:^(JConcreteMessage * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        
        //recall message
        if ([obj.contentType isEqualToString:[JRecallCmdMessage contentType]]) {
            JRecallCmdMessage *cmd = (JRecallCmdMessage *)obj.content;
            JMessage *recallMessage = [self handleRecallCmdMessage:cmd.originalMessageId extra:cmd.extra];
            //recallMessage 为空表示被撤回的消息本地不存在，不需要回调
            if (recallMessage) {
                dispatch_async(self.core.delegateQueue, ^{
                    [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull dlg, NSUInteger idx, BOOL * _Nonnull stop) {
                        if ([dlg respondsToSelector:@selector(messageDidRecall:)]) {
                            [dlg messageDidRecall:recallMessage];
                        }
                    }];
                });
            }
            return;
        }
        
        if (obj.flags & JMessageFlagIsCmd) {
            return;
        }
        if (obj.existed) {
            return;
        }
        
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull dlg, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([dlg respondsToSelector:@selector(messageDidReceive:)]) {
                    [dlg messageDidReceive:obj];
                }
            }];
        });
    }];
    [self checkChatroomSyncDic];
}

- (void)checkChatroomSyncDic {
    NSDictionary <NSString *, NSNumber *> *popDic = [self popChatroomSyncDic];
    if (popDic.count > 0) {
        NSArray *keys = [popDic allKeys];
        NSString *chatroomId = [keys objectAtIndex:0];
        long long time = [popDic objectForKey:chatroomId].longLongValue;
        [self syncChatroomMessages:chatroomId time:time];
    } else {
        self.chatroomSyncProcessing = NO;
    }
}

- (void)syncChatroomMessages:(NSString *)chatroomId
                        time:(long long)time {
    if (![self.chatroomManager isChatroomAvailable:chatroomId]) {
        [self checkChatroomSyncDic];
        return;
    }
    long long cachedSyncTime = [self.chatroomManager getSyncTimeForChatroom:chatroomId];
    if (time > cachedSyncTime) {
        int prevMessageCount = [self.chatroomManager getPrevMessageCountForChatroom:chatroomId];
        [self webSocketSyncChatroomMessages:chatroomId
                                       time:cachedSyncTime
                           prevMessageCount:prevMessageCount];
    } else {
        [self checkChatroomSyncDic];
    }
}

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
    [self registerContentType:[JAddConvMessage class]];
    [self registerContentType:[JClearTotalUnreadMessage class]];
    [self registerContentType:[JMarkUnreadMessage class]];
    [self registerContentType:[JCallFinishNotifyMessage class]];
    [self registerContentType:[JMsgModifyMessage class]];
    [self registerContentType:[JMsgExSetMessage class]];
    [self registerContentType:[JTagAddConvMessage class]];
    [self registerContentType:[JTagDelConvMessage class]];
    [self registerContentType:[JTopMsgMessage class]];
    [self registerContentType:[JCallActiveCallMessage class]];
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
        messageOption:nil
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
    JMergeInfo * mergeInfo;
    if ([message.content isKindOfClass:[JMergeMessage class]]) {
        JMergeMessage * mergeMessage = (JMergeMessage *)message.content;
        mergeInfo = [[JMergeInfo alloc]init];
        mergeInfo.conversation = mergeMessage.conversation;
        mergeInfo.containerMsgId = mergeMessage.containerMsgId;
        mergeInfo.messages = [self.core.dbManager getMessagesByMessageIds:mergeMessage.messageIdList];
    }
    JMessageContent *content = message.content;
    if (!content) {
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock(JErrorCodeInvalidParam, message);
            }
        });
        return;
    }
    [self.core.webSocket sendIMMessage:content
                        inConversation:message.conversation
                           clientMsgNo:message.clientMsgNo
                             clientUid:message.clientUid
                             mergeInfo:mergeInfo
                           isBroadcast:isBroadcast
                                userId:self.core.userId
                           mentionInfo:message.mentionInfo
                       referredMessage:(JConcreteMessage *)message.referredMsg
                              pushData:message.pushData
                              lifeTime:message.lifeTime
                     lifeTimeAfterRead:message.lifeTimeAfterRead
                               success:^(long long clientMsgNo, NSString *msgId, long long timestamp, long long seqNo,  NSString * _Nullable contentType, JMessageContent * _Nullable content, int groupMemberCount) {
        JLogI(@"MSG-Send", @"success");
        [self.core.dbManager updateMessageAfterSend:message.clientMsgNo
                                          messageId:msgId
                                          timestamp:timestamp
                                              seqNo:seqNo
                                   groupMemberCount:groupMemberCount];
        message.messageId = msgId;
        message.timestamp = timestamp;
        message.seqNo = seqNo;
        message.messageState = JMessageStateSent;
        message.destroyTime = timestamp + message.lifeTime;
        if (message.conversation.conversationType == JConversationTypeGroup) {
            JGroupMessageReadInfo *info = [JGroupMessageReadInfo new];
            info.readCount = 0;
            info.memberCount = groupMemberCount;
            message.groupReadInfo = info;
        }
        
        if (contentType && content) {
            [self.core.dbManager updateMessageContent:content contentType:contentType withMessageId:msgId];
            message.content = content;
            message.contentType = contentType;
        }
        
        if([message.content isKindOfClass:[JMergeMessage class]]){
            JMergeMessage * mergeMessage = (JMergeMessage *)message.content;
            if(mergeMessage.containerMsgId == nil || mergeMessage.containerMsgId.length == 0){
                mergeMessage.containerMsgId = msgId;
            }
            [self.core.dbManager updateMessageContent:message.content
                                          contentType:message.contentType
                                        withMessageId:message.messageId];
        }
        if (message.conversation.conversationType != JConversationTypeChatroom) {
            if (! (message.flags & JMessageFlagIsStatus)) {
                [self updateSendSyncTime:timestamp];
            }
            if (message.flags & JMessageFlagIsSave) {
                if ([self.sendReceiveDelegate respondsToSelector:@selector(messageDidSend:)]) {
                    [self.sendReceiveDelegate messageDidSend:message];
                }
            }
        }
        
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(message);
            }
        });
    } error:^(JErrorCodeInternal errorCode, long long clientMsgNo) {
        JLogI(@"MSG-Send", @"error, code is %lu", errorCode);
        message.messageState = JMessageStateFail;
        [self setMessageState:JMessageStateFail withClientMsgNo:clientMsgNo];
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)errorCode, message);
            }
        });
    }];
}

- (JMessage *)sendMessage:(JMessageContent *)content
            messageOption:(JMessageOptions *)messageOption
           inConversation:(JConversation *)conversation
              isBroadcast:(BOOL)isBroadcast
                  success:(void (^)(JMessage *message))successBlock
                    error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock {
    JConcreteMessage *message = [self saveMessageWithContent:content
                                               messageOption:messageOption
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

-(void)updateMessageWithContent:(JConcreteMessage *)message{
    if (message.content != nil) {
        if ([message.content isKindOfClass:[JUnknownMessage class]]) {
            JUnknownMessage *unknown = (JUnknownMessage *)message.content;
            message.contentType = unknown.messageType;
        } else {
            message.contentType = [[message.content class] contentType];
        }
    }
    if (message.referredMsg) {
        message.referredMsg = [self.core.dbManager getMessageWithMessageId:message.referredMsg.messageId
                                                               currentTime:[self.core getCurrentTime]];
    }
    [self.core.dbManager updateMessage:message];
}

- (JConcreteMessage *)saveMessageWithContent:(JMessageContent *)content
                               messageOption:(JMessageOptions *)messageOption
                              inConversation:(JConversation *)conversation
                                       state:(JMessageState)state
                                   direction:(JMessageDirection)direction
                                 isBroadcast:(BOOL)isBroadcast {
    JConcreteMessage *message = [[JConcreteMessage alloc] init];
    message.content = content;
    message.conversation = conversation;
    if ([content isKindOfClass:[JUnknownMessage class]]) {
        JUnknownMessage *unknown = (JUnknownMessage *)content;
        message.contentType = unknown.messageType;
        message.flags = unknown.flags;
    } else {
        message.contentType = [[content class] contentType];
        message.flags = [[content class] flags];
    }
    message.direction = direction;
    message.messageState = state;
    message.senderUserId = self.core.userId;
    message.clientUid = [self createClientUid];
    long long now = [[NSDate date] timeIntervalSince1970] * 1000;
    message.timestamp = now;
    if (isBroadcast) {
        message.flags |= JMessageFlagIsBroadcast;
    }
    if(messageOption.mentionInfo) {
        message.mentionInfo = messageOption.mentionInfo;
    }
    if (messageOption.referredMsgId) {
        JConcreteMessage * referredMsg = [self.core.dbManager getMessageWithMessageId:messageOption.referredMsgId currentTime:[self.core getCurrentTime]];
        message.referredMsg = referredMsg;
    }
    if (messageOption.pushData) {
        message.pushData = messageOption.pushData;
    }
    message.lifeTime = messageOption.lifeTime;
    message.lifeTimeAfterRead = messageOption.lifeTimeAfterRead;
    if (message.lifeTime > 0) {
        message.destroyTime = [self.core getCurrentTime] + message.lifeTime;
    }
    
    if (message.flags & JMessageFlagIsSave) {
        [self.core.dbManager insertMessages:@[message]];
        if (conversation.conversationType != JConversationTypeChatroom) {
            if ([self.sendReceiveDelegate respondsToSelector:@selector(messageDidSave:)]) {
                [self.sendReceiveDelegate messageDidSave:message];
            }
        }
    } else {
        message.clientMsgNo = now;
    }
    
    return message;
}


- (NSArray <JConcreteMessage *> *)messagesToSave:(NSArray <JConcreteMessage *> *)messages {
    NSMutableArray <JConcreteMessage *> *arr = [[NSMutableArray alloc] init];
    for (JConcreteMessage *message in messages) {
        if (message.flags & JMessageFlagIsSave) {
            [arr addObject:message];
        }
        [self saveReferMessages:message];
    }
    return arr;
}

-(void)saveReferMessages:(JConcreteMessage *)message {
    if(message.referredMsg == nil){
        return;
    }
    JConcreteMessage * localReferMsg = [self.core.dbManager getMessageWithMessageId:message.referredMsg.messageId currentTime:[self.core getCurrentTime]];
    if(localReferMsg != nil){
        message.referredMsg = localReferMsg;
    }else{
        JConcreteMessage *refer = (JConcreteMessage *)message.referredMsg;
        NSArray * messages = [self messagesToSave:@[refer]];
        [self insertRemoteMessages:messages];
    }
}

- (JMessage *)handleModifyMessage:(NSString *)messageId
                          msgType:(NSString *)msgType
                          content:(JMessageContent *)content {
    if(messageId == nil) {
        return nil;
    }
    [self.core.dbManager updateMessageContent:content
                                  contentType:msgType
                                withMessageId:messageId];
    NSArray <JMessage *> *messages = [self.core.dbManager getMessagesByMessageIds:@[messageId]];
    if (messages.count > 0) {
        JConcreteMessage * message = (JConcreteMessage *)messages.firstObject;
        int flags = message.flags | JMessageFlagIsModified;
        message.flags = flags;
        message.isEdit = YES;
        [self.core.dbManager setMessageFlags:flags withMessageId:message.messageId];
        if ([self.sendReceiveDelegate respondsToSelector:@selector(messageDidUpdate:)]) {
            [self.sendReceiveDelegate messageDidUpdate:message];
        }
        return message;
    }
    return nil;
}

- (JMessage *)handleRecallCmdMessage:(NSString *)messageId extra:(NSDictionary *)extra {
    if(messageId == nil) {
        return nil;
    }
    JRecallInfoMessage *recallInfoMsg = [[JRecallInfoMessage alloc] init];
    recallInfoMsg.exts = extra;
    [self.core.dbManager updateMessageContent:recallInfoMsg
                                  contentType:[JRecallInfoMessage contentType]
                                withMessageId:messageId];
    NSArray <JMessage *> *messages = [self.core.dbManager getMessagesByMessageIds:@[messageId]];
    if (messages.count > 0) {
        JConcreteMessage * message = (JConcreteMessage *)messages.firstObject;
        [self notifyMessageRemoved:message.conversation removedMessages:@[message]];
        return message;
    }
    return nil;
}

- (void)handleDeleteMsgMessageCmdMessage:(JConcreteMessage *)message {
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
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if([obj respondsToSelector:@selector(messageDidDelete:clientMsgNos:)]){
                [obj messageDidDelete:message.conversation clientMsgNos:clientMsgNos];
            }
        }];
    });
    [self notifyMessageRemoved:message.conversation removedMessages:messageList];
}

- (void)handleTopMsgMessage:(JConcreteMessage *)message {
    JTopMsgMessage *topMsg = (JTopMsgMessage *)message.content;
    //延时操作有可能导致乱序，但是针对这个业务，回调延迟也不会造成太大影响
    [self getMessagesByMessageIds:@[topMsg.messageId]
                   inConversation:message.conversation
                          success:^(NSArray<JMessage *> *messages) {
        if (messages.count > 0) {
            JMessage *m = messages[0];
            JUserInfo *userInfo = [self.userInfoManager getUserInfo:message.senderUserId];
            if (!userInfo) {
                userInfo = [[JUserInfo alloc] init];
                userInfo.userId = message.senderUserId;
            }
            dispatch_async(self.core.delegateQueue, ^{
                [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                    if ([obj respondsToSelector:@selector(messageDidSetTop:message:user:)]) {
                        [obj messageDidSetTop:topMsg.isTop
                                      message:m
                                         user:userInfo];
                    }
                }];
            });
        }
    } error:^(JErrorCode errorCode) {
    }];
}

- (void)handleClearHistoryMessageCmdMessage:(JConcreteMessage *)message {
    JCleanMsgMessage * content = (JCleanMsgMessage *)message.content;
    
    long long starTime = content.cleanTime;
    if(starTime == 0){
        starTime = [[NSDate date] timeIntervalSince1970] * 1000;
    }
    
    [self.core.dbManager clearMessagesIn:message.conversation startTime:starTime senderId:content.senderId];
    dispatch_async(self.core.delegateQueue, ^{
        [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if([obj respondsToSelector:@selector(messageDidClear:timestamp:senderId:)]){
                [obj messageDidClear:message.conversation
                                     timestamp:starTime
                                      senderId:content.senderId];
            }
        }];
    });
    [self notifyMessageCleared:message.conversation startTime:starTime sendUserId:message.senderUserId];
}

- (void)handleLogCommandMessage:(JConcreteMessage *)message {
    JLogCommandMessage *content = (JLogCommandMessage *)message.content;
    
    if (![content.platform isEqualToString:@"iOS"]) {
        return;
    }
    [JLogger.shared uploadLog:message.messageId
                    startTime:content.startTime
                      endTime:content.endTime];
}

- (void)handleAddConversationMessage:(JConcreteMessage *)message {
    JAddConvMessage *content = (JAddConvMessage *)message.content;
    if ([self.sendReceiveDelegate respondsToSelector:@selector(conversationsDidAdd:)]) {
        [self.sendReceiveDelegate conversationsDidAdd:content.conversationInfo];
    }
}

- (void)handleMarkUnreadMessage:(JConcreteMessage *)message {
    JMarkUnreadMessage *content = (JMarkUnreadMessage *)message.content;
    [content.conversations enumerateObjectsUsingBlock:^(JConversation * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if ([self.sendReceiveDelegate respondsToSelector:@selector(conversationDidSetUnread:)]) {
            [self.sendReceiveDelegate conversationDidSetUnread:obj];
        }
    }];
}

- (void)handleReceiveMessages:(NSArray<JConcreteMessage *> *)messages
                       isSync:(BOOL)isSync {
    NSArray <JConcreteMessage *> *messagesToSave = [self messagesToSave:messages];
    [self insertRemoteMessages:messagesToSave];
    
    __block long long sendTime = 0;
    __block long long receiveTime = 0;
    [messages enumerateObjectsUsingBlock:^(JConcreteMessage * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        BOOL isStatusMessage = obj.flags&JMessageFlagIsStatus;
        if (obj.direction == JMessageDirectionSend && !isStatusMessage) {
            sendTime = obj.timestamp;
        } else if (obj.direction == JMessageDirectionReceive && !isStatusMessage) {
            receiveTime = obj.timestamp;
        }
        
        // call related
        if ([obj.contentType isEqualToString:[JCallActiveCallMessage contentType]]) {
            [self.callManager handleActiveCallMessage:obj];
            return;
        }
        
        // tag add conversation
        if ([obj.contentType isEqualToString:[JTagAddConvMessage contentType]]) {
            if (obj.timestamp <= self.core.conversationSyncTime) {
                return;
            }
            JTagAddConvMessage *cmd = (JTagAddConvMessage *)obj.content;
            if (cmd.conversations.count == 0 || cmd.tagId == 0) {
                return;
            }
            [self.core.dbManager addConversations:cmd.conversations toTag:cmd.tagId];
            [self.sendReceiveDelegate conversationsDidAddToTag:cmd.tagId
                                                 conversations:cmd.conversations];
            return;
        }
        
        // tag remove conversation
        if ([obj.contentType isEqualToString:[JTagDelConvMessage contentType]]) {
            if (obj.timestamp <= self.core.conversationSyncTime) {
                return;
            }
            JTagDelConvMessage *cmd = (JTagDelConvMessage *)obj.content;
            if (cmd.conversations.count == 0 || cmd.tagId == 0) {
                return;
            }
            [self.core.dbManager removeConversations:cmd.conversations fromTag:cmd.tagId];
            [self.sendReceiveDelegate conversationsDidRemoveFromTag:cmd.tagId
                                                      conversations:cmd.conversations];
            return;
        }
        
        // reaction
        if ([obj.contentType isEqualToString:[JMsgExSetMessage contentType]]) {
            JMsgExSetMessage *cmd = (JMsgExSetMessage *)obj.content;
            if (cmd.addItemList.count > 0) {
                dispatch_async(self.core.delegateQueue, ^{
                    [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull dlg, NSUInteger idx, BOOL * _Nonnull stop) {
                        if ([dlg respondsToSelector:@selector(messageReactionDidAdd:inConversation:)]) {
                            JMessageReaction *reaction = [[JMessageReaction alloc] init];
                            reaction.messageId = cmd.originalMessageId;
                            reaction.itemList = cmd.addItemList;
                            [dlg messageReactionDidAdd:reaction inConversation:obj.conversation];
                        }
                    }];
                });
            }
            if (cmd.removeItemList.count > 0) {
                dispatch_async(self.core.delegateQueue, ^{
                    [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull dlg, NSUInteger idx, BOOL * _Nonnull stop) {
                        if ([dlg respondsToSelector:@selector(messageReactionDidRemove:inConversation:)]) {
                            JMessageReaction *reaction = [[JMessageReaction alloc] init];
                            reaction.messageId = cmd.originalMessageId;
                            reaction.itemList = cmd.removeItemList;
                            [dlg messageReactionDidRemove:reaction inConversation:obj.conversation];
                        }
                    }];
                });
            }
            return;
        }
        
        //modify message
        if ([obj.contentType isEqualToString:[JMsgModifyMessage contentType]]) {
            JMsgModifyMessage *cmd = (JMsgModifyMessage *)obj.content;
            JMessage *updatedMessage = [self handleModifyMessage:cmd.originalMessageId msgType:cmd.messageType content:cmd.messageContent];
            //updatedMessage 为空表示被修改的消息本地不存在，不需要回调
            if (updatedMessage) {
                dispatch_async(self.core.delegateQueue, ^{
                    [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull dlg, NSUInteger idx, BOOL * _Nonnull stop) {
                        if ([dlg respondsToSelector:@selector(messageDidUpdate:)]) {
                            [dlg messageDidUpdate:updatedMessage];
                        }
                    }];
                });
            }
            return;
        }
        
        //recall message
        if ([obj.contentType isEqualToString:[JRecallCmdMessage contentType]]) {
            JRecallCmdMessage *cmd = (JRecallCmdMessage *)obj.content;
            JMessage *recallMessage = [self handleRecallCmdMessage:cmd.originalMessageId extra:cmd.extra];
            //recallMessage 为空表示被撤回的消息本地不存在，不需要回调
            if (recallMessage) {
                dispatch_async(self.core.delegateQueue, ^{
                    [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull dlg, NSUInteger idx, BOOL * _Nonnull stop) {
                        if ([dlg respondsToSelector:@selector(messageDidRecall:)]) {
                            [dlg messageDidRecall:recallMessage];
                        }
                    }];
                });
            }
            return;
        }
        //delete conversation
        if ([obj.contentType isEqualToString:[JDeleteConvMessage contentType]]) {
            JDeleteConvMessage *deleteConvMsg = (JDeleteConvMessage *)obj.content;
            NSMutableArray * deletedList = [NSMutableArray array];
            for (JConversation *deleteConversation in deleteConvMsg.conversations) {
                JConcreteConversationInfo *conversationInfo = [self.core.dbManager getConversationInfo:deleteConversation];
                if (!conversationInfo) {
                    continue;
                }
                JMessage * lastMessage = conversationInfo.lastMessage;
                if (obj.timestamp <= lastMessage.timestamp) {
                    continue;
                } else {
                    [deletedList addObject:deleteConversation];
                }
                [self.core.dbManager deleteConversationInfoBy:deleteConversation];
            }
            if ([self.sendReceiveDelegate respondsToSelector:@selector(conversationsDidDelete:)] && deletedList.count != 0) {
                [self.sendReceiveDelegate conversationsDidDelete:deletedList];
            }
            return;
        }
        
        //read ntf
        if ([obj.contentType isEqualToString:[JReadNtfMessage contentType]]) {
            JReadNtfMessage *readNtfMsg = (JReadNtfMessage *)obj.content;
            [self.core.dbManager setMessagesRead:readNtfMsg.messageIds
                                        readTime:obj.timestamp];
            dispatch_async(self.core.delegateQueue, ^{
                [self.readReceiptDelegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageReadReceiptDelegate>  _Nonnull dlg, NSUInteger idx, BOOL * _Nonnull stop) {
                    if ([dlg respondsToSelector:@selector(messagesDidRead:inConversation:)]) {
                        [dlg messagesDidRead:readNtfMsg.messageIds
                              inConversation:obj.conversation];
                    }
                }];
            });
            if ([self.sendReceiveDelegate respondsToSelector:@selector(messageDidRead:messageIds:)]) {
                [self.sendReceiveDelegate messageDidRead:obj.conversation
                                              messageIds:readNtfMsg.messageIds];
            }
            NSArray <JMessage *> *readMessages = [self.core.dbManager getMessagesByMessageIds:readNtfMsg.messageIds];
            [readMessages enumerateObjectsUsingBlock:^(JMessage * _Nonnull readMessage, NSUInteger idx, BOOL * _Nonnull stop) {
                [self checkAndNotifyDestroyTime:readMessage time:obj.timestamp];
            }];
            return;
        }
        
        //group read ntf
        if ([obj.contentType isEqualToString:[JGroupReadNtfMessage contentType]]) {
            JGroupReadNtfMessage *groupReadNtfMsg = (JGroupReadNtfMessage *)obj.content;
            [self.core.dbManager setGroupMessageReadInfo:groupReadNtfMsg.msgs];
            dispatch_async(self.core.delegateQueue, ^{
                [self.readReceiptDelegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageReadReceiptDelegate>  _Nonnull dlg, NSUInteger idx, BOOL * _Nonnull stop) {
                    if ([dlg respondsToSelector:@selector(groupMessagesDidRead:inConversation:)]) {
                        [dlg groupMessagesDidRead:groupReadNtfMsg.msgs
                                                        inConversation:obj.conversation];
                    }
                }];
            });
            [groupReadNtfMsg.msgs enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull messageId, JGroupMessageReadInfo * _Nonnull readInfo, BOOL * _Nonnull stop) {
                if (readInfo.readCount >= readInfo.memberCount) {
                    NSArray <JMessage *> *readMessages = [self.core.dbManager getMessagesByMessageIds:@[messageId]];
                    if (readMessages.count > 0) {
                        JMessage *readMessage = readMessages[0];
                        [self checkAndNotifyDestroyTime:readMessage time:obj.timestamp];
                    }
                }
            }];
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
        
        //top message
        if ([obj.contentType isEqualToString:[JTopMsgMessage contentType]]) {
            [self handleTopMsgMessage:obj];
            return;
        }
        
        //clear total unread message
        if ([obj.contentType isEqualToString:[JClearTotalUnreadMessage contentType]]) {
            JClearTotalUnreadMessage * message = (JClearTotalUnreadMessage *)obj.content;
            if ([self.sendReceiveDelegate respondsToSelector:@selector(conversationsDidClearTotalUnread:)]) {
                [self.sendReceiveDelegate conversationsDidClearTotalUnread:message.clearTime];
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
        
        //add conversation
        if ([obj.contentType isEqualToString:[JAddConvMessage contentType]]) {
            [self handleAddConversationMessage:obj];
            return;
        }
        
        //mark unread
        if ([obj.contentType isEqualToString:[JMarkUnreadMessage contentType]]) {
            [self handleMarkUnreadMessage:obj];
            return;
        }
        
        if (obj.flags & JMessageFlagIsCmd) {
            return;
        }
        if (obj.existed) {
            return;
        }
        dispatch_async(self.core.delegateQueue, ^{
            JLogI(@"MSG-Rcv", @"receive message is %@", obj.messageId);
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDelegate>  _Nonnull dlg, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([dlg respondsToSelector:@selector(messageDidReceive:)]) {
                    [dlg messageDidReceive:obj];
                }
            }];
        });
    }];
    
    if ([self.sendReceiveDelegate respondsToSelector:@selector(messagesDidReceive:)]) {
        [self.sendReceiveDelegate messagesDidReceive:messagesToSave];
    }

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
                                              userId:self.core.userId
                                             success:^(NSArray * _Nonnull messages, BOOL isFinished) {
        JLogI(@"MSG-Sync", @"success");
        [self messagesDidReceive:messages isFinished:isFinished];
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-Sync", @"error, code is %ld", code);
        [self messagesDidReceive:[NSArray array] isFinished:YES];
    }];
}

- (void)webSocketSyncChatroomMessages:(NSString *)chatroomId
                        time:(long long)syncTime
                     prevMessageCount:(int)count {
    JLogI(@"MSG-ChrmSync", @"id is %@, time is %lld, count is %d", chatroomId, syncTime, count);
    self.chatroomSyncProcessing = YES;
    [self.core.webSocket syncChatroomMessagesWithTime:syncTime
                                           chatroomId:chatroomId
                                               userId:self.core.userId
                                     prevMessageCount:count
                                              success:^(NSArray * _Nonnull messages, BOOL isFinished) {
        JLogI(@"MSG-ChrmSync", @"success");
        [self chatroomMessagesDidReceive:messages];
    } error:^(JErrorCodeInternal code) {
        JLogE(@"MSG-ChrmSync", @"error, code is %ld", code);
        [self chatroomMessagesDidReceive:[NSArray array]];
    }];
}

- (NSString *)createClientUid {
    return [JUtility getUUID];
}

- (void)updateUserInfos:(NSArray <JConcreteMessage *> *)messages {
    NSMutableDictionary *groupDic = [[NSMutableDictionary alloc] init];
    NSMutableDictionary *userDic = [[NSMutableDictionary alloc] init];
    NSMutableDictionary *groupMemberDic = [NSMutableDictionary dictionary];
    [messages enumerateObjectsUsingBlock:^(JConcreteMessage * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if (obj.groupInfo.groupId.length > 0) {
            [groupDic setObject:obj.groupInfo forKey:obj.groupInfo.groupId];
        }
        if (obj.targetUserInfo.userId.length > 0) {
            [userDic setObject:obj.targetUserInfo forKey:obj.targetUserInfo.userId];
        }
        if (obj.groupMemberInfo.userId.length > 0 && obj.groupMemberInfo.groupId.length > 0) {
            NSString *key = [NSString stringWithFormat:@"%@xxx%@", obj.groupMemberInfo.groupId, obj.groupMemberInfo.userId];
            [groupMemberDic setObject:obj.groupMemberInfo forKey:key];
        }
        if (obj.mentionInfo) {
            for (JUserInfo *userInfo in obj.mentionInfo.targetUsers) {
                [userDic setObject:userInfo forKey:userInfo.userId];
            }
        }
    }];
    [self.userInfoManager insertUserInfoList:userDic.allValues];
    [self.userInfoManager insertGroupInfoList:groupDic.allValues];
    [self.userInfoManager insertGroupMemberList:groupMemberDic.allValues];
}

- (void)insertRemoteMessages:(NSArray<JConcreteMessage *> *)messages {
    [self.core.dbManager insertMessages:messages];
    [self updateUserInfos:messages];
}

- (NSString *)generateLocalPath:(JMediaType)mediaType
                           name:(NSString *)fileName {
    NSString *path = [JUtility mediaPath:mediaType];
    if (![[NSFileManager defaultManager] fileExistsAtPath:path]) {
        [[NSFileManager defaultManager] createDirectoryAtPath:path
                                  withIntermediateDirectories:YES
                                                   attributes:nil
                                                        error:nil];
    }
    path = [path stringByAppendingPathComponent:fileName];
    return path;
}

- (NSString *)getFileNameWith:(NSString *)url {
    if (url.length == 0) {
        return @"";
    }
    NSArray *a = [url componentsSeparatedByString:@"/"];
    NSUInteger c = a.count;
    if (c > 0) {
        return a[c-1];
    } else {
        return @"";
    }
}

- (void)updateSendSyncTime:(long long)timestamp {
    if (self.syncProcessing) {
        if (timestamp > self.cachedSendTime) {
            self.cachedSendTime = timestamp;
        }
    } else {
        self.core.messageSendSyncTime = timestamp;
    }
}

- (void)completeCallbackForGetMessages:(void (^)(NSArray<JMessage *> *, long long, BOOL, JErrorCode))completeCallback
                              messages:(NSArray<JMessage *> *)messages
                                 count:(int)count
                             direction:(JPullDirection)direction
                               hasMore:(BOOL)hasMore
                             errorCode:(JErrorCode)code
                        getMessageTime:(long long)getMessageTime {
    if (messages.count > count) {
        if (direction == JPullDirectionNewer) {
            messages = [messages subarrayWithRange:NSMakeRange(0, count)];
        } else {
            messages = [messages subarrayWithRange:NSMakeRange(messages.count - count, count)];
        }
    }
    if (messages.count == 0) {
        dispatch_async(self.core.delegateQueue, ^{
            if (completeCallback) {
                completeCallback(messages, getMessageTime, hasMore, code);
            }
        });
        return;
    }
    JMessage *m;
    if (direction == JPullDirectionNewer) {
        m = messages.lastObject;
    } else {
        m = messages.firstObject;
    }
    long long timestamp = m.timestamp;
    dispatch_async(self.core.delegateQueue, ^{
        if (completeCallback) {
            completeCallback(messages, timestamp, hasMore, code);
        }
    });
}

- (void)checkAndNotifyDestroyTime:(JMessage *)readMessage
                             time:(long long)timestamp {
    if (readMessage.lifeTimeAfterRead > 0 && !readMessage.isDeleted) {
        long long destroyTime = timestamp + readMessage.lifeTimeAfterRead;
        if (destroyTime < readMessage.destroyTime) {
            readMessage.destroyTime = destroyTime;
            [self.core.dbManager updateDestroyTime:destroyTime withMessageId:readMessage.messageId];
            dispatch_async(self.core.delegateQueue, ^{
                [self.destroyDelegates.allObjects enumerateObjectsUsingBlock:^(id<JMessageDestroyDelegate>  _Nonnull destroyDelegate, NSUInteger idx, BOOL * _Nonnull stop) {
                    if ([destroyDelegate respondsToSelector:@selector(messageDestroyTimeDidUpdate:inConversation:destroyTime:)]) {
                        [destroyDelegate messageDestroyTimeDidUpdate:readMessage.messageId
                                                      inConversation:readMessage.conversation
                                                         destroyTime:destroyTime];
                    }
                }];
            });
        }
    }
}

#pragma mark - getter
- (NSHashTable<id<JMessageDelegate>> *)delegates {
    if (!_delegates) {
        _delegates = [NSHashTable weakObjectsHashTable];
    }
    return _delegates;
}

- (NSHashTable<id<JMessageSyncDelegate>> *)syncDelegates {
    if (!_syncDelegates) {
        _syncDelegates = [NSHashTable weakObjectsHashTable];
    }
    return _syncDelegates;

}

- (NSHashTable<id<JMessageReadReceiptDelegate>> *)readReceiptDelegates {
    if (!_readReceiptDelegates) {
        _readReceiptDelegates = [NSHashTable weakObjectsHashTable];
    }
    return _readReceiptDelegates;
}

- (NSHashTable<id<JMessageDestroyDelegate>> *)destroyDelegates {
    if (!_destroyDelegates) {
        _destroyDelegates = [NSHashTable weakObjectsHashTable];
    }
    return _destroyDelegates;
}

- (JDownloadManager *)downloadManager {
    if (!_downloadManager) {
        _downloadManager = [[JDownloadManager alloc] init];
    }
    return _downloadManager;
}

- (void)setTimeForChatroomSyncDic:(long long)timestamp
                       chatroomId:(NSString *)chatroomId {
    @synchronized (self) {
        [self.chatroomSyncDic setObject:@(timestamp) forKey:chatroomId];
    }
}

- (NSDictionary <NSString *, NSNumber *>*)popChatroomSyncDic {
    @synchronized (self) {
        if (self.chatroomSyncDic.count > 0) {
            NSArray *keys = [self.chatroomSyncDic allKeys];
            NSString *chatroomId = [keys objectAtIndex:0];
            long long time = [self.chatroomSyncDic objectForKey:chatroomId].longLongValue;
            [self.chatroomSyncDic removeObjectForKey:chatroomId];
            NSDictionary *result = @{chatroomId:@(time)};
            return result;
        } else {
            return nil;
        }
    }
}

- (void)clearChatroomSyncDic {
    @synchronized (self) {
        [self.chatroomSyncDic removeAllObjects];
    }
}

- (BOOL)chatroomSyncProcessing {
    @synchronized (self) {
        return _chatroomSyncProcessing;
    }
}

- (void)setChatroomSyncProcessing:(BOOL)chatroomSyncProcessing {
    @synchronized (self) {
        if (_chatroomSyncProcessing != chatroomSyncProcessing) {
            _chatroomSyncProcessing = chatroomSyncProcessing;
        }
    }
}

- (NSMutableDictionary<NSString *,NSNumber *> *)chatroomSyncDic {
    if (!_chatroomSyncDic) {
        _chatroomSyncDic = [NSMutableDictionary dictionary];
    }
    return _chatroomSyncDic;
}

@end
