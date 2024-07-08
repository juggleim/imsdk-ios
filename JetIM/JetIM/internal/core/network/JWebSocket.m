//
//  JWebSocket.m
//  JetIM
//
//  Created by Nathan on 2023/11/29.
//

#import "JWebSocket.h"
#import "SRWebSocket.h"
#import "JUtility.h"
#import "JetIMConstInternal.h"
#import "JPBData.h"
#import "JLogger.h"
#import "JHeartBeatManager.h"
#import "JBlockObj.h"
#import "JWebSocketCommandManager.h"

#define jWebSocketPrefix @"ws://"
#define jWebSocketSuffix @"/im"
#define jMaxConcurrentCount 5

@interface JWebSocket () <SRWebSocketDelegate, JCommandTimeOutDelegate>
@property (nonatomic, weak) id<JWebSocketConnectDelegate> connectDelegate;
@property (nonatomic, weak) id<JWebSocketMessageDelegate> messageDelegate;
@property (nonatomic, copy) NSString *appKey;
@property (nonatomic, copy) NSString *token;
@property (nonatomic, copy) NSString *pushToken;
@property (nonatomic, strong) SRWebSocket *sws;
@property (nonatomic, strong) dispatch_queue_t sendQueue;
@property (nonatomic, strong) dispatch_queue_t receiveQueue;
/// 所有上行数据的自增 index
@property (nonatomic, assign) int32_t cmdIndex;
@property (nonatomic, strong) JPBData *pbData;
@property (nonatomic, strong) NSOperationQueue *competeQueue;
@property (nonatomic, assign) BOOL isCompeteFinish;
@property (nonatomic, strong) NSMutableArray <SRWebSocket *> *competeSwsList;
@property (nonatomic, strong) JHeartBeatManager *heartbeatManager;
@property (nonatomic, strong) JWebSocketCommandManager *commandManager;
@end

@implementation JWebSocket

- (instancetype)initWithSendQueque:(dispatch_queue_t)sendQueue receiveQueue:(dispatch_queue_t)receiveQueue {
    if (self = [super init]) {
        self.heartbeatManager = [[JHeartBeatManager alloc] initWithWebSocket:self];
        self.sendQueue = sendQueue;
        self.receiveQueue = receiveQueue;
        self.commandManager = [[JWebSocketCommandManager alloc] initWithDelegate:self];
        [self.commandManager start];
        self.pbData = [[JPBData alloc] init];
        NSOperationQueue *competeQueue = [[NSOperationQueue alloc] init];
        competeQueue.maxConcurrentOperationCount = jMaxConcurrentCount;
        self.competeQueue = competeQueue;
    }
    return self;
}

- (void)connect:(NSString *)appKey
          token:(NSString *)token
      pushToken:(NSString *)pushToken
        servers:(nonnull NSArray *)servers {
    dispatch_async(self.sendQueue, ^{
        JLogI(@"WS-Connect", @"appkey is %@, token is %@", appKey, token);
        self.appKey = appKey;
        self.token = token;
        self.pushToken = pushToken;
        
        [self resetSws];
        for (NSString *url in servers) {
            JLogI(@"WS-Connect", @"create web socket url is %@", url);
            SRWebSocket *sws = [self createWebSocket:url];
            [self.competeSwsList addObject:sws];
            NSBlockOperation *operation = [NSBlockOperation blockOperationWithBlock:^{
                [sws open];
            }];
            [self.competeQueue addOperation:operation];
        }
    });
}

- (void)disconnect:(BOOL)needPush {
    dispatch_async(self.sendQueue, ^{
        JLogI(@"WS-Disconnect", @"need push is %d", needPush);
        [self sendDisconnectMsgByWebSocket:needPush];
    });
}

- (void)startHeartbeat {
    [self.heartbeatManager start];
}

- (void)stopHeartbeat {
    [self.heartbeatManager stop];
}

- (void)heartbeatTimeOut {
    [self.connectDelegate webSocketDidTimeOut];
}

- (void)pushRemainCmdAndCallbackError {
    NSArray <JBlockObj *> *objs = [self.commandManager clearBlockObject];
    for (JBlockObj *obj in objs) {
        [self command:obj error:JErrorCodeInternalConnectionUnavailable];
    }
}

- (void)setConnectDelegate:(id<JWebSocketConnectDelegate>)delegate {
    _connectDelegate = delegate;
}

- (void)setMessageDelegate:(id<JWebSocketMessageDelegate>)delegate {
    _messageDelegate = delegate;
}

#pragma mark - send pb
- (void)sendIMMessage:(JMessageContent *)content
       inConversation:(JConversation *)conversation
          clientMsgNo:(long long)clientMsgNo
            clientUid:(NSString *)clientUid
            mergeInfo:(JMergeInfo *)mergeInfo
          isBroadcast:(BOOL)isBroadcast
               userId:(NSString *)userId
          mentionInfo:(JMessageMentionInfo *)mentionInfo
      referredMessage:(JConcreteMessage *)referredMessage
              success:(void (^)(long long clientMsgNo, NSString *msgId, long long timestamp, long long reqNo))successBlock
                error:(void (^)(JErrorCodeInternal errorCode, long long clientMsgNo))errorBlock{
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *encodeData;
        if ([content isKindOfClass:[JMediaMessageContent class]]) {
            JMediaMessageContent *mediaContent = (JMediaMessageContent *)content;
            NSString *local = mediaContent.localPath;
            mediaContent.localPath = nil;
            encodeData = [mediaContent encode];
            mediaContent.localPath = local;
        } else {
            encodeData = [content encode];
        }
        NSData *d = [self.pbData sendMessageDataWithType:[[content class] contentType]
                                                 msgData:encodeData
                                                   flags:[[content class] flags]
                                               clientUid:clientUid
                                               mergeInfo:mergeInfo
                                             isBroadcast:isBroadcast
                                                  userId:userId
                                                   index:self.cmdIndex++
                                        conversationType:conversation.conversationType
                                          conversationId:conversation.conversationId
                                             mentionInfo:mentionInfo
                                         referredMessage:referredMessage];
        JLogI(@"WS-Send", @"send message");
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            JLogE(@"WS-Send", @"send message error, msg is %@", err.description);
            if (errorBlock) {
                errorBlock(JErrorCodeInternalWebSocketFailure, clientMsgNo);
            }
        } else {
            JSendMessageObj *obj = [[JSendMessageObj alloc] init];
            obj.clientMsgNo = clientMsgNo;
            obj.successBlock = successBlock;
            obj.errorBlock = errorBlock;
            [self.commandManager setBlockObject:obj forKey:key];
        }
    });
}

- (void)recallMessage:(NSString *)messageId
               extras:(NSDictionary *)extras
         conversation:(JConversation *)conversation
            timestamp:(long long)timestamp
              success:(void (^)(long long timestamp))successBlock
                error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData recallMessageData:messageId
                                            extras:extras
                                      conversation:conversation
                                         timestamp:timestamp
                                             index:self.cmdIndex++];
        JLogI(@"WS-Send", @"recall message, id is %@", messageId);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)sendReadReceipt:(NSArray <NSString *> *)messageIds
         inConversation:(JConversation *)conversation
                success:(void (^)(long long timestamp))successBlock
                  error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData sendReadReceiptData:messageIds
                                      inConversation:conversation
                                               index:self.cmdIndex++];
        JLogI(@"WS-Send", @"send read receipt");
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)getGroupMessageReadDetail:(NSString *)messageId
                   inConversation:(JConversation *)conversation
                          success:(void (^)(NSArray<JUserInfo *> *readMembers, NSArray<JUserInfo *> *unreadMembers))successBlock
                            error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData getGroupMessageReadDetail:messageId
                                            inConversation:conversation
                                                     index:self.cmdIndex++];
        JLogI(@"WS-Send", @"get group message read detail, id is %@", messageId);
        JQryReadDetailObj *obj = [[JQryReadDetailObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)syncMessagesWithReceiveTime:(long long)receiveTime
                           sendTime:(long long)sendTime
                             userId:(NSString *)userId {
    dispatch_async(self.sendQueue, ^{
        NSData *d = [self.pbData syncMessagesDataWithReceiveTime:receiveTime
                                                        sendTime:sendTime
                                                          userId:userId
                                                           index:self.cmdIndex++];
        JLogI(@"WS-Send", @"sync messages, receive is %lld, send is %lld", receiveTime, sendTime);
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            JLogE(@"WS-Send", @"sync message error, msg is %@", err.description);
        }
    });
}

- (void)queryHisMsgsFrom:(JConversation *)conversation
               startTime:(long long)startTime
                   count:(int)count
               direction:(JPullDirection)direction
                 success:(void (^)(NSArray * _Nonnull, BOOL))successBlock
                   error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData queryHisMsgsDataFrom:conversation
                                            startTime:startTime
                                                count:count
                                            direction:direction
                                                index:self.cmdIndex++];
        JLogI(@"WS-Send", @"query history messages");
        JQryHisMsgsObj *obj = [[JQryHisMsgsObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)queryHisMsgsByIds:(NSArray<NSString *> *)messageIds
           inConversation:(JConversation *)conversation
                  success:(void (^)(NSArray<JConcreteMessage *> * _Nonnull, BOOL isFinished))successBlock
                    error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData queryHisMsgsDataByIds:messageIds
                                        inConversation:conversation
                                                 index:self.cmdIndex++];
        JLogI(@"WS-Send", @"query history message by id");
        JQryHisMsgsObj *obj = [[JQryHisMsgsObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)syncConversations:(long long)startTime
                    count:(int)count
                   userId:(NSString *)userId
                  success:(void (^)(NSArray * _Nonnull, NSArray * _Nonnull, BOOL))successBlock
                    error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData syncConversationsData:startTime
                                                 count:count
                                                userId:userId
                                                 index:self.cmdIndex++];
        JLogI(@"WS-Send", @"sync conversation, time is %lld", startTime);
        JSyncConvsObj *obj = [[JSyncConvsObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)deleteConversationInfo:(JConversation *)conversation
                        userId:(NSString *)userId
                       success:(void (^)(long long timestamp))successBlock
                         error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData deleteConversationData:conversation
                                                 userId:userId
                                                  index:self.cmdIndex++];
        JLogI(@"WS-Send", @"delete conversation, type is %lu, id is %@", (unsigned long)conversation.conversationType, conversation.conversationId);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)clearUnreadCount:(JConversation *)conversation
                  userId:(NSString *)userId
                msgIndex:(long long)msgIndex
                 success:(void (^)(long long timestamp))successBlock
                   error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData clearUnreadCountData:conversation
                                               userId:userId
                                             msgIndex:msgIndex
                                                index:self.cmdIndex++];
        JLogI(@"WS-Send", @"clear unread, type is %lu, id is %@, msgIndex is %lld", (unsigned long)conversation.conversationType, conversation.conversationId, msgIndex);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)setMute:(BOOL)isMute
 inConversation:(JConversation *)conversation
         userId:(NSString *)userId
        success:(void (^)(long long timestamp))successBlock
          error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData undisturbData:conversation
                                        userId:userId
                                        isMute:isMute
                                         index:self.cmdIndex++];
        JLogI(@"WS-Send", @"set mute, mute is %d, type is %lu, id is %@", isMute, (unsigned long)conversation.conversationType, conversation.conversationId);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)setTop:(BOOL)isTop
inConversation:(JConversation *)conversation
        userId:(NSString *)userId
       success:(void (^)(long long timestamp))successBlock
         error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData topConversationData:conversation
                                              userId:userId
                                               isTop:isTop
                                               index:self.cmdIndex++];
        JLogI(@"WS-Send", @"set top, top is %d, type is %lu, id is %@", isTop, (unsigned long)conversation.conversationType, conversation.conversationId);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)getMergedMessageList:(NSString *)messageId
                        time:(long long)timestamp
                       count:(int)count
                   direction:(JPullDirection)direction
                     success:(void (^)(NSArray<JConcreteMessage *> * _Nonnull, BOOL isFinished))successBlock
                       error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData getMergedMessageList:messageId
                                                 time:timestamp
                                                count:count
                                            direction:direction
                                                index:self.cmdIndex++];
        JLogI(@"WS-Send", @"get merge message, id is %@", messageId);
        JQryHisMsgsObj *obj = [[JQryHisMsgsObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)getMentionMessages:(JConversation *)conversation
                      time:(long long)time
                     count:(int)count
                 direction:(JPullDirection)direction
                   success:(void (^)(NSArray<JConcreteMessage *> *messages, BOOL isFinished))successBlock
                     error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData getMentionMessages:conversation
                                               time:time
                                              count:count
                                          direction:direction
                                              index:self.cmdIndex++];
        JLogI(@"WS-Send", @"get mention message");
        JQryHisMsgsObj *obj = [[JQryHisMsgsObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)registerPushToken:(NSString *)token
                   userId:(NSString *)userId
                  success:(void (^)(void))successBlock
                    error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData registerPushToken:token
                                          deviceId:[JUtility getDeviceId]
                                       packageName:[[NSBundle mainBundle] bundleIdentifier]
                                            userId:userId
                                             index:self.cmdIndex++];
        JLogI(@"WS-Send", @"register push token");
        [self simpleSendData:d
                         key:key
                     success:successBlock
                       error:errorBlock];
    });
}

- (void)clearTotalUnreadCount:(NSString *)userId
                         time:(long long)time
                      success:(void (^)(long long timestamp))successBlock
                        error:(void (^)(JErrorCodeInternal code))errorBlock{
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData clearTotalUnreadCountMessages:userId
                                                          time:time
                                                         index:self.cmdIndex++];
        JLogI(@"WS-Send", @"clear total unread, time is %lld", time);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
    
}

- (void)deleteMessage:(JConversation *)conversation
              msgList:(NSArray <JConcreteMessage *> *)msgList
              success:(void (^)(long long timestamp))successBlock
                error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData deleteMessage:conversation
                                       msgList:msgList
                                         index:self.cmdIndex++];
        JLogI(@"WS-Send", @"delete message");
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}


- (void)clearHistoryMessage:(JConversation *)conversation
                       time:(long long)time
                    success:(void (^)(long long timestamp))successBlock
                      error:(void (^)(JErrorCodeInternal code))errorBlock{
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData clearHistoryMessage:conversation
                                                time:time
                                               scope:0
                                               index:self.cmdIndex++];
        JLogI(@"WS-Send", @"clear history message, type is %lu, id is %@", (unsigned long)conversation.conversationType, conversation.conversationId);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)createConversationInfo:(JConversation *)conversation
                        userId:(NSString *)userId
                       success:(void (^)(JConcreteConversationInfo * conversationInfo, long long timestamp))successBlock
                         error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData createConversationInfo:conversation userId:userId index:self.cmdIndex++];
        JLogI(@"WS-Send", @"create conversation, type is %lu, id is %@", (unsigned long)conversation.conversationType, conversation.conversationId);
        JConversationObj *obj = [[JConversationObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)sendPing {
    dispatch_async(self.sendQueue, ^{
        NSData *d = [self.pbData pingData];
        NSError *err = nil;
        JLogV(@"WS-Send", @"ping");
        [self.sws sendData:d error:&err];
        if (err != nil) {
            JLogE(@"WS-Send", @"ping error, msg is %@", err.description);
        }
    });
}

- (void)sendPublishAck:(int)index {
    dispatch_async(self.sendQueue, ^{
        NSData *d = [self.pbData publishAckData:index];
        NSError *err = nil;
        JLogV(@"WS-Send", @"publish ack");
        [self.sws sendData:d error:&err];
        if (err != nil) {
            JLogE(@"WS-Send", @"publish ack error, msg is %@", err.description);
        }
    });
}

#pragma mark - SRWebSocketDelegate
- (void)webSocketDidOpen:(SRWebSocket *)webSocket {
    dispatch_async(self.sendQueue, ^{
        if (self.isCompeteFinish) {
            JLogI(@"WS-Connect", @"compete fail, url is %@", webSocket.url);
            [webSocket close];
            return;
        }
        //防止上一批竞速的 webSocket 被选中
        for (SRWebSocket *sws in self.competeSwsList) {
            if (webSocket == sws) {
                JLogI(@"WS-Connect", @"compete success, url is %@", webSocket.url);
                self.isCompeteFinish = YES;
                self.sws = webSocket;
                [self sendConnectMsgByWebSocket:webSocket];
                break;
            }
        }
    });
}

- (void)webSocket:(SRWebSocket *)webSocket didFailWithError:(NSError *)error {
    if (webSocket != self.sws) {
        return;
    }
    JLogI(@"WS-Connect", @"fail message is %@", error.description);
    dispatch_async(self.sendQueue, ^{
        [self resetSws];
    });
    if ([self.connectDelegate respondsToSelector:@selector(webSocketDidFail)]) {
        [self.connectDelegate webSocketDidFail];
    }
}

- (void)webSocket:(SRWebSocket *)webSocket didCloseWithCode:(NSInteger)code reason:(NSString *)reason wasClean:(BOOL)wasClean {
    if (webSocket != self.sws) {
        return;
    }
    JLogI(@"WS-Connect", @"close code is %ld, reason is %@", (long)code, reason);
    dispatch_async(self.sendQueue, ^{
        [self resetSws];
    });
    if ([self.connectDelegate respondsToSelector:@selector(webSocketDidClose)]) {
        [self.connectDelegate webSocketDidClose];
    }
}

- (void)webSocket:(SRWebSocket *)webSocket didReceiveMessageWithData:(NSData *)data {
    if (webSocket != self.sws) {
        return;
    }
    [self.heartbeatManager updateLastMessageReceiveTime];
    JPBRcvObj *obj = [self.pbData rcvObjWithData:data];
    switch (obj.rcvType) {
        case JPBRcvTypeParseError:
            JLogI(@"WS-Receive", @"JPBRcvTypeParseError");
            break;
        case JPBRcvTypeConnectAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeConnectAck");
            [self handleConnectAckMsg:obj.connectAck];
            break;
        case JPBRcvTypePublishMsgAck:
            JLogI(@"WS-Receive", @"JPBRcvTypePublishMsgAck");
            [self handlePublishAckMsg:obj.publishMsgAck];
            break;
        case JPBRcvTypeQryHisMsgsAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeQryHisMsgsAck");
            [self handleQryHisMsgs:obj.qryHisMsgsAck];
            break;
        case JPBRcvTypeSyncConvsAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeSyncConvsAck");
            [self handleSyncConvsAck:obj.syncConvsAck];
            break;
        case JPBRcvTypePublishMsg:
            JLogI(@"WS-Receive", @"JPBRcvTypePublishMsg");
            [self handleReceiveMessage:obj.publishMsgBody];
            break;
        case JPBRcvTypePublishMsgNtf:
            JLogI(@"WS-Receive", @"JPBRcvTypePublishMsgNtf");
            [self handlePublishMsgNtf:obj.publishMsgNtf];
            break;
        case JPBRcvTypeSyncMsgsAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeSyncMsgsAck");
            [self handleSyncMsgsAck:obj.qryHisMsgsAck];
            break;
        case JPBRcvTypePong:
            JLogV(@"WS-Receive", @"JPBRcvTypePong");
            [self handlePong];
            break;
        case JPBRcvTypeDisconnectMsg:
            JLogI(@"WS-Receive", @"JPBRcvTypeDisconnectMsg");
            [self handleDisconnectMsg:obj.disconnectMsg];
            break;
        case JPBRcvTypeSimpleQryAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeSimpleQryAck");
            [self handleSimpleAck:obj.simpleQryAck];
            break;
        case JPBRcvTypeQryReadDetailAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeQryReadDetailAck");
            [self handleQryReadDetailAck:obj.qryReadDetailAck];
            break;
        case JPBRcvTypeSimpleQryAckCallbackTimestamp:
            JLogI(@"WS-Receive", @"JPBRcvTypeSimpleQryAckCallbackTimestamp");
            [self handleSimpleQryAckWithTimeCallback:obj.simpleQryAck];
            break;
        case JPBRcvTypeAddConversation:
            JLogI(@"WS-Receive", @"JPBRcvTypeAddConversation");
            [self handleConversationAck:obj.conversationInfoAck];
        default:
            JLogI(@"WS-Receive", @"default, type is %lu", (unsigned long)obj.rcvType);
            break;
    }
}

#pragma mark - JCommandTimeOutDelegate
- (void)commandDidTimeOut:(NSArray<JBlockObj *> *)objs {
    for (JBlockObj *obj in objs) {
        [self command:obj error:JErrorCodeInternalOperationTimeOut];
    }
}

#pragma mark - inner
- (void)command:(JBlockObj *)obj
          error:(JErrorCodeInternal)code {
    if ([obj isKindOfClass:[JSendMessageObj class]]) {
        JSendMessageObj *s = (JSendMessageObj *)obj;
        s.errorBlock(code, s.clientMsgNo);
    } else if ([obj isKindOfClass:[JQryHisMsgsObj class]]) {
        JQryHisMsgsObj *s = (JQryHisMsgsObj *)obj;
        s.errorBlock(code);
    } else if ([obj isKindOfClass:[JSyncConvsObj class]]) {
        JSyncConvsObj *s = (JSyncConvsObj *)obj;
        s.errorBlock(code);
    } else if ([obj isKindOfClass:[JTimestampBlockObj class]]) {
        JTimestampBlockObj *s = (JTimestampBlockObj *)obj;
        s.errorBlock(code);
    } else if ([obj isKindOfClass:[JQryReadDetailObj class]]) {
        JQryReadDetailObj *s = (JQryReadDetailObj *)obj;
        s.errorBlock(code);
    } else if ([obj isKindOfClass:[JSimpleBlockObj class]]) {
        JSimpleBlockObj *s = (JSimpleBlockObj *)obj;
        s.errorBlock(code);
    }
}

- (void)sendConnectMsgByWebSocket:(SRWebSocket *)sws {
    NSData *d = [self.pbData connectDataWithAppKey:self.appKey
                                             token:self.token
                                          deviceId:[JUtility getDeviceId]
                                          platform:JPlatform
                                     deviceCompany:JDeviceCompany
                                       deviceModel:[JUtility currentDeviceModel]
                                   deviceOsVersion:[JUtility currentSystemVersion]
                                       packageName:[[NSBundle mainBundle] bundleIdentifier]
                                         pushToken:self.pushToken
                                         networkId:[JUtility currentNetWork]
                                            ispNum:[JUtility currentCarrier]
                                          clientIp:@""];
    NSError *err = nil;
    [sws sendData:d error:&err];
    if (err != nil) {
        JLogE(@"WS-Connect", @"send connect error, msg is %@", err.description);
    }
}

- (void)sendDisconnectMsgByWebSocket:(BOOL)needPush {
    NSData *d = [self.pbData disconnectData:needPush];
    NSError *err = nil;
    [self.sws sendData:d error:&err];
    if (err != nil) {
        JLogE(@"WS-Send", @"send disconnect error, msg is %@", err.description);
    }
    [self resetSws];
}

- (void)handleConnectAckMsg:(JConnectAck *)connectAck {
    if ([self.connectDelegate respondsToSelector:@selector(connectCompleteWithCode:userId:session:extra:)]) {
        [self.connectDelegate connectCompleteWithCode:connectAck.code
                                               userId:connectAck.userId
                                              session:connectAck.session
                                                extra:connectAck.extra];
    }
}

- (void)handlePublishAckMsg:(JPublishMsgAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JSendMessageObj class]]) {
        JSendMessageObj *sendMessageObj = (JSendMessageObj *)obj;
        if (ack.code != 0) {
            sendMessageObj.errorBlock(ack.code, sendMessageObj.clientMsgNo);
        } else {
            sendMessageObj.successBlock(sendMessageObj.clientMsgNo, ack.msgId, ack.timestamp, ack.seqNo);
        }
    }
}

- (void)handleQryHisMsgs:(JQryHisMsgsAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JQryHisMsgsObj class]]) {
        JQryHisMsgsObj *qryHisMsgsObj = (JQryHisMsgsObj *)obj;
        if (ack.code != 0) {
            qryHisMsgsObj.errorBlock(ack.code);
        } else {
            qryHisMsgsObj.successBlock(ack.msgs, ack.isFinished);
        }
    }
}

- (void)handleSyncConvsAck:(JSyncConvsAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JSyncConvsObj class]]) {
        JSyncConvsObj *syncConvsObj = (JSyncConvsObj *)obj;
        if (ack.code != 0) {
            syncConvsObj.errorBlock(ack.code);
        } else {
            syncConvsObj.successBlock(ack.convs, ack.deletedConvs, ack.isFinished);
        }
    }
}

//sync 和 queryHisMsgs 共用一个 ack
- (void)handleSyncMsgsAck:(JQryHisMsgsAck *)ack {
    if ([self.messageDelegate respondsToSelector:@selector(messagesDidReceive:isFinished:)]) {
        [self.messageDelegate messagesDidReceive:ack.msgs isFinished:ack.isFinished];
    }
}

- (void)handleReceiveMessage:(JPublishMsgBody *)publishMsgBody {
    BOOL needAck = NO;
    if ([self.messageDelegate respondsToSelector:@selector(messageDidReceive:)]) {
        needAck = [self.messageDelegate messageDidReceive:publishMsgBody.rcvMessage];
    }
    if (publishMsgBody.qos == 1 && needAck) {
        [self sendPublishAck:publishMsgBody.index];
    }
}

- (void)handlePublishMsgNtf:(JPublishMsgNtf *)ntf {
    if ([self.messageDelegate respondsToSelector:@selector(syncNotify:)]) {
        [self.messageDelegate syncNotify:ntf.syncTime];
    }
}

- (void)handlePong {
}

- (void)handleDisconnectMsg:(JDisconnectMsg *)msg {
    dispatch_async(self.sendQueue, ^{
        [self resetSws];
        if ([self.connectDelegate respondsToSelector:@selector(disconnectWithCode:extra:)]) {
            [self.connectDelegate disconnectWithCode:msg.code extra:msg.extra];
        }
    });
}

- (void)handleSimpleAck:(JSimpleQryAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JSimpleBlockObj class]]) {
        JSimpleBlockObj *simpleObj = (JSimpleBlockObj *)obj;
        if (ack.code != 0) {
            simpleObj.errorBlock(ack.code);
        } else {
            simpleObj.successBlock();
        }
    }
}

- (void)handleSimpleQryAckWithTimeCallback:(JSimpleQryAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JTimestampBlockObj class]]) {
        JTimestampBlockObj *simpleObj = (JTimestampBlockObj *)obj;
        if (ack.code != 0) {
            simpleObj.errorBlock(ack.code);
        } else {
            simpleObj.successBlock(ack.timestamp);
        }
    }
}

- (void)handleConversationAck:(JConversationInfoAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JConversationObj class]]) {
        JConversationObj *conversationObj = (JConversationObj *)obj;
        if (ack.code != 0) {
            conversationObj.errorBlock(ack.code);
        } else {
            conversationObj.successBlock(ack.conversationInfo,ack.timestamp);
        }
    }
}

- (void)handleQryReadDetailAck:(JQryReadDetailAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JQryReadDetailObj class]]) {
        JQryReadDetailObj *qryReadDetailObj = (JQryReadDetailObj *)obj;
        if (ack.code != 0) {
            qryReadDetailObj.errorBlock(ack.code);
        } else {
            qryReadDetailObj.successBlock(ack.readMembers, ack.unreadMembers);
        }
    }
}

- (void)simpleSendData:(NSData *)data
                   key:(NSNumber *)key
               success:(void (^)(void))successBlock
                 error:(void (^)(JErrorCodeInternal code))errorBlock {
    JSimpleBlockObj *obj = [[JSimpleBlockObj alloc] init];
    obj.successBlock = successBlock;
    obj.errorBlock = errorBlock;
    [self sendData:data
               key:key
               obj:obj
             error:errorBlock];
}

- (void)timestampSendData:(NSData *)data
                      key:(NSNumber *)key
                  success:(void (^)(long long timestamp))successBlock
                    error:(void (^)(JErrorCodeInternal code))errorBlock {
    JTimestampBlockObj *obj = [[JTimestampBlockObj alloc] init];
    obj.successBlock = successBlock;
    obj.errorBlock = errorBlock;
    [self sendData:data
               key:key
               obj:obj
             error:errorBlock];
}

- (void)sendData:(NSData *)data
             key:(NSNumber *)key
             obj:(JBlockObj *)obj
           error:(void (^)(JErrorCodeInternal code))errorBlock {
    NSError *err = nil;
    [self.sws sendData:data error:&err];
    if (err != nil) {
        JLogE(@"WS-Send", @"send data error, description is %@", err.description);
        if (errorBlock) {
            errorBlock(JErrorCodeInternalWebSocketFailure);
        }
    } else {
        [self.commandManager setBlockObject:obj forKey:key];
    }
}

- (SRWebSocket *)createWebSocket:(NSString *)url {
    NSString *u = [NSString stringWithFormat:@"%@%@%@", jWebSocketPrefix, url, jWebSocketSuffix];
    SRWebSocket *sws = [[SRWebSocket alloc] initWithURLRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:u]]];
    sws.delegateDispatchQueue = self.receiveQueue;
    sws.delegate = self;
    return sws;
}

- (void)resetSws {
    self.sws = nil;
    [self.competeSwsList removeAllObjects];
    self.isCompeteFinish = NO;
}

#pragma mark - getter
- (NSMutableArray<SRWebSocket *> *)competeSwsList {
    if (!_competeSwsList) {
        _competeSwsList = [NSMutableArray array];
    }
    return _competeSwsList;
}
@end
