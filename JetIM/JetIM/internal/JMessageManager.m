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
@property (nonatomic, assign) int increaseId;
//TODO: 消息拉取回来在 messageManager 里做排重
@end

@implementation JMessageManager

- (void)syncMessages {
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
    return m;
}

- (void)setDelegate:(id<JMessageDelegate>)delegate {
    _delegate = delegate;
}


- (void)deleteMessageByClientMsgNo:(long long)clientMsgNo {
    [self.core.dbManager deleteMessageByClientId:clientMsgNo];
}

- (void)deleteMessageByMessageId:(NSString *)messageId {
    [self.core.dbManager deleteMessageByMessageId:messageId];
}

- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction {
    return [self.core.dbManager getMessagesFrom:conversation
                                          count:count
                                           time:time
                                      direction:direction];
}


- (JMessage *)sendMessage:(JMessageContent *)content
     inConversation:(JConversation *)conversation
            success:(void (^)(long long))successBlock
              error:(void (^)(JErrorCode, long long))errorBlock{
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
        self.core.messageSendSyncTime = timestamp;
        [self.core.dbManager updateMessageAfterSend:message.clientMsgNo
                                          messageId:msgId
                                          timestamp:timestamp
                                       messageIndex:msgIndex];
        if (successBlock) {
            successBlock(clientMsgNo);
        }
    } error:^(JErrorCodeInternal errorCode, long long clientMsgNo) {
        if (errorBlock) {
            errorBlock((JErrorCode)errorCode, clientMsgNo);
        }
    }];
    return message;
}

- (void)registerContentType:(Class)messageClass {
    [[JContentTypeCenter shared] registerContentType:messageClass];
}

#pragma mark - JWebSocketMessageDelegate
- (void)messagesDidReceive:(NSArray<JConcreteMessage *> *)messages
                isFinished:(BOOL)isFinished {
    //TODO: 排重
    //TODO: cmd message 吞掉
    
    [self.core.dbManager insertMessages:messages];
    
    //标识是否存在发送的消息
    __block BOOL sendDirection = NO;
    //标识是否存在接收的消息
    __block BOOL receiveDirection = NO;
    [messages enumerateObjectsWithOptions:NSEnumerationReverse
                               usingBlock:^(JConcreteMessage * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if (!sendDirection) {
            if (obj.direction == JMessageDirectionSend) {
                sendDirection = YES;
                if (obj.timestamp > self.core.messageSendSyncTime) {
                    self.core.messageSendSyncTime = obj.timestamp;
                }
            }
        }
        if (!receiveDirection) {
            if (obj.direction == JMessageDirectionReceive) {
                receiveDirection = YES;
                if (obj.timestamp > self.core.messageReceiveSyncTime) {
                    self.core.messageReceiveSyncTime = obj.timestamp;
                }
            }
        }
        dispatch_async(self.core.delegateQueue, ^{
            if ([self.delegate respondsToSelector:@selector(messageDidReceive:)]) {
                [self.delegate messageDidReceive:obj];
            }
        });
    }];
    
    if (!isFinished) {
        [self sync];
    }
    
}

- (void)syncNotify:(long long)syncTime {
    NSLog(@"[JetIM] syncNotify syncTime is %lld, receiveSyncTime is %lld", syncTime, self.core.messageReceiveSyncTime);
    if (syncTime > self.core.messageReceiveSyncTime) {
        [self sync];
    }
}

#pragma mark - internal
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
