//
//  JMessageManager.m
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

#import "JMessageManager.h"
#import "JuggleConst.h"
#import "JTextMessage.h"
#import "JImageMessage.h"

@interface JMessageManager () <JWebSocketMessageDelegate>
@property (nonatomic, strong) JuggleCore *core;
@property (nonatomic, weak) id<JMessageDelegate> delegate;
//TODO: 消息收发时间，应该放到 DB 里
//TODO: 消息拉取回来在 messageManager 里做排重
//sendSyncTime 和 receiveSyncTime 都在 receiveQueue 里处理
@property (nonatomic, assign) long long sendSyncTime;
@property (nonatomic, assign) long long receiveSyncTime;
@end

@implementation JMessageManager

- (instancetype)initWithCore:(JuggleCore *)core {
    JMessageManager *m = [[JMessageManager alloc] init];
    m.core = core;
    [m.core.webSocket setMessageDelegate:m];
    [m.core.webSocket registerMessageType:[JTextMessage class]];
    [m.core.webSocket registerMessageType:[JImageMessage class]];
    return m;
}

- (void)setDelegate:(id<JMessageDelegate>)delegate {
    _delegate = delegate;
}

//
//- (void)deleteMessageByClientId:(long)clientMsgNo {
//    <#code#>
//}
//
//- (void)deleteMessageByMessageId:(NSString *)messageId {
//    <#code#>
//}
//
//- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation count:(int)count time:(long long)time direction:(JPullDirection)direction {
//    <#code#>
//}
//

- (JMessage *)sendMessage:(JMessageContent *)content
     inConversation:(JConversation *)conversation
            success:(void (^)(long))successBlock
              error:(void (^)(JErrorCode, long))errorBlock{
    //TODO: save message
    JMessage *message = [[JMessage alloc] init];
    message.content = content;
    message.conversation = conversation;
    message.messageType = [[content class] contentType];
    message.direction = JMessageDirectionSend;
    message.messageState = JMessageStateSending;
    message.senderUserId = self.core.userId;
    
    [self.core.webSocket sendIMMessage:content
                        inConversation:conversation
                           clientMsgNo:message.clientMsgNo
                               success:^(long clientMsgNo, NSString *msgId, long long timestamp) {
        self.sendSyncTime = timestamp;
        //TODO: 更新 DB，msgId 和 消息状态
        if (successBlock) {
            successBlock(clientMsgNo);
        }
    } error:^(JErrorCode errorCode, long clientMsgNo) {
        if (errorBlock) {
            errorBlock(errorCode, clientMsgNo);
        }
    }];
    return message;
}

- (void)registerMessageType:(Class)messageClass {
    [self.core.webSocket registerMessageType:messageClass];
}

#pragma mark - JWebSocketMessageDelegate
- (void)messagesDidReceive:(NSArray<JConcreteMessage *> *)messages
                isFinished:(BOOL)isFinished {
    //TODO: 排重
    //TODO: cmd message 吞掉
    [messages enumerateObjectsUsingBlock:^(JConcreteMessage * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if (obj.direction == JMessageDirectionSend) {
            if (obj.timestamp > self.sendSyncTime) {
                self.sendSyncTime = obj.timestamp;
            }
        } else {
            if (obj.timestamp > self.receiveSyncTime) {
                self.receiveSyncTime = obj.timestamp;
            }
        }
        dispatch_async(self.core.delegateQueue, ^{
            if (self.delegate) {
                [self.delegate messageDidReceive:obj];
            }
        });
    }];
    
    if (!isFinished) {
        [self sync];
    }
    
}

- (void)syncNotify:(long long)syncTime {
    if (syncTime > self.receiveSyncTime) {
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
                                    error:errorBlock];
}

- (void)sync {
    [self.core.webSocket syncMessagesWithReceiveTime:self.receiveSyncTime
                                            sendTime:self.sendSyncTime
                                              userId:self.core.userId];
}

@end
