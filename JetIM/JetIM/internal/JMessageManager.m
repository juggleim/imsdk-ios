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

@interface JMessageManager () <JWebSocketMessageDelegate>
@property (nonatomic, strong) JetIMCore *core;
@property (nonatomic, weak) id<JMessageDelegate> delegate;
@property (nonatomic, weak) id<JMessageSyncDelegate> syncDelegate;
@property (nonatomic, assign) int increaseId;
//在 receiveQueue 里处理
@property (nonatomic, assign) BOOL syncProcessing;
@property (nonatomic, assign) long long cachedReceiveTime;
@property (nonatomic, assign) long long cachedSendTime;
//TODO: 消息拉取回来在 messageManager 里做排重
@end

@implementation JMessageManager

- (void)syncMessages {
    self.syncProcessing = YES;
    [self sync];
}

- (instancetype)initWithCore:(JetIMCore *)core {
    JMessageManager *m = [[JMessageManager alloc] init];
    m.core = core;
    [m.core.webSocket setMessageDelegate:m];
    [m registerContentType:[JTextMessage class]];
    [m registerContentType:[JImageMessage class]];
    [m registerContentType:[JFileMessage class]];
    [m registerContentType:[JVoiceMessage class]];
    [m registerContentType:[JVideoMessage class]];
    m.cachedSendTime = -1;
    m.cachedReceiveTime = -1;
    return m;
}

- (void)setDelegate:(id<JMessageDelegate>)delegate {
    _delegate = delegate;
}

- (void)setSyncDelegate:(id<JMessageSyncDelegate>)syncDelegate {
    _syncDelegate = syncDelegate;
}

- (void)deleteMessageByClientMsgNo:(long long)clientMsgNo {
    [self.core.dbManager deleteMessageByClientId:clientMsgNo];
}

- (void)deleteMessageByMessageId:(NSString *)messageId {
    [self.core.dbManager deleteMessageByMessageId:messageId];
}

- (void)clearMessagesIn:(JConversation *)conversation {
    [self.core.dbManager clearMessagesIn:conversation];
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


- (JMessage *)sendMessage:(JMessageContent *)content
     inConversation:(JConversation *)conversation
            success:(void (^)(JMessage *))successBlock
              error:(void (^)(JErrorCode, JMessage *))errorBlock{
    JConcreteMessage *message = [[JConcreteMessage alloc] init];
    message.content = content;
    message.conversation = conversation;
    message.contentType = [[content class] contentType];
    message.direction = JMessageDirectionSend;
    message.messageState = JMessageStateSending;
    message.senderUserId = self.core.userId;
    message.clientUid = [self createClientUid];
    
    [self.core.dbManager insertMessages:@[message]];
    [self.core.webSocket sendIMMessage:content
                        inConversation:conversation
                           clientMsgNo:message.clientMsgNo
                             clientUid:message.clientUid
                               success:^(long long clientMsgNo, NSString *msgId, long long timestamp, long long msgIndex) {
        if (self.syncProcessing) {
            self.cachedSendTime = timestamp;
        } else {
            self.core.messageSendSyncTime = timestamp;
        }
        [self.core.dbManager updateMessageAfterSend:message.clientMsgNo
                                          messageId:msgId
                                          timestamp:timestamp
                                       messageIndex:msgIndex];
        message.messageId = msgId;
        message.timestamp = timestamp;
        message.msgIndex = msgIndex;
        message.messageState = JMessageStateSent;
        dispatch_async(self.core.delegateQueue, ^{
            if (successBlock) {
                successBlock(message);
            }
        });
    } error:^(JErrorCodeInternal errorCode, long long clientMsgNo) {
        message.messageState = JMessageStateFail;
        [self.core.dbManager messageSendFail:clientMsgNo];
        dispatch_async(self.core.delegateQueue, ^{
            if (errorBlock) {
                errorBlock((JErrorCode)errorCode, message);
            }
        });
    }];
    return message;
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
    [self deleteMessageByClientMsgNo:messsage.clientMsgNo];
    return [self sendMessage:messsage.content
              inConversation:messsage.conversation
                     success:successBlock
                       error:errorBlock];
}

- (NSArray<JMessage *> *)getMessagesByMessageIds:(NSArray<NSString *> *)messageIds {
    return [self.core.dbManager getMessagesByMessageIds:messageIds];
}

- (NSArray<JMessage *> *)getMessagesByClientMsgNos:(NSArray<NSNumber *> *)clientMsgNos {
    return [self.core.dbManager getMessagesByClientMsgNos:clientMsgNos];
}

- (void)registerContentType:(Class)messageClass {
    [[JContentTypeCenter shared] registerContentType:messageClass];
}

#pragma mark - JWebSocketMessageDelegate
- (void)messageDidReceive:(JConcreteMessage *)message {
    [self handleReceiveMessages:@[message]];
}

- (void)messagesDidReceive:(NSArray<JConcreteMessage *> *)messages
                isFinished:(BOOL)isFinished {
    [self handleReceiveMessages:messages];
    
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
    NSLog(@"[JetIM] syncNotify syncTime is %lld, receiveSyncTime is %lld", syncTime, self.core.messageReceiveSyncTime);
    if (syncTime > self.core.messageReceiveSyncTime) {
        self.syncProcessing = YES;
        [self sync];
    }
}

#pragma mark - internal
- (void)handleReceiveMessages:(NSArray<JConcreteMessage *> *)messages {
    //TODO: 排重
    //TODO: cmd message 吞掉
    
    [self.core.dbManager insertMessages:messages];
    
    __block long long sendTime = 0;
    __block long long receiveTime = 0;
    [messages enumerateObjectsUsingBlock:^(JConcreteMessage * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if (obj.direction == JMessageDirectionSend) {
            sendTime = obj.timestamp;
        } else if (obj.direction == JMessageDirectionReceive) {
            receiveTime = obj.timestamp;
        }
        dispatch_async(self.core.delegateQueue, ^{
            if ([self.delegate respondsToSelector:@selector(messageDidReceive:)]) {
                [self.delegate messageDidReceive:obj];
            }
        });
    }];
    if (self.syncProcessing) {
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

- (void)getRemoteMessagesFrom:(JConversation *)conversation
                    startTime:(long long)startTime
                        count:(int)count
                    direction:(JPullDirection)direction
                      success:(void (^)(NSArray *messages, BOOL isFinished))successBlock
                        error:(void (^)(JErrorCode code))errorBlock {
    [self.core.webSocket queryHisMsgsFrom:conversation
                                startTime:startTime
                                    count:count
                                direction:direction
                                  success:successBlock
                                    error:^(JErrorCodeInternal code) {
        if (errorBlock) {
            errorBlock((JErrorCode)code);
        }
    }];
}

- (void)sync {
    [self.core.webSocket syncMessagesWithReceiveTime:self.core.messageReceiveSyncTime
                                            sendTime:self.core.messageSendSyncTime
                                              userId:self.core.userId];
}

- (NSString *)createClientUid {
    long long ts = [[NSDate date] timeIntervalSince1970];
    ts = ts % 1000000;
    return [NSString stringWithFormat:@"%06lld%03d", ts, self.increaseId++];
}

@end
