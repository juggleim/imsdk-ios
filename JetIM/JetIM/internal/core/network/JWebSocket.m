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

#define jWebSocketPrefix @"ws://"
#define jWebSocketSuffix @"/im"
#define jMaxConcurrentCount 5

@interface JBlockObj : NSObject
@end

@implementation JBlockObj
@end

@interface JSendMessageObj : JBlockObj
@property (nonatomic, assign) long long clientMsgNo;
@property (nonatomic, copy) void (^successBlock)(long long clientMsgNo, NSString *msgId, long long timestamp, long long seqNo);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode, long long clientMsgNo);
@end

@implementation JSendMessageObj
@end

@interface JQryHisMsgsObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(NSArray * _Nonnull msgs, BOOL isFinished);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@implementation JQryHisMsgsObj
@end

@interface JSyncConvsObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(NSArray * _Nonnull convs, NSArray * _Nonnull deletedConvs, BOOL isFinished);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@implementation JSyncConvsObj
@end

@interface JTimestampBlockObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(long long timestamp);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@implementation JTimestampBlockObj
@end

@interface JQryReadDetailObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(NSArray<JUserInfo *> *readMembers, NSArray<JUserInfo *> *unreadMembers);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@implementation JQryReadDetailObj
@end

@interface JSimpleBlockObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(void);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@implementation JSimpleBlockObj
@end

@interface JWebSocket () <SRWebSocketDelegate>
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
@property (nonatomic, strong) NSMutableDictionary<NSNumber *, JBlockObj *> *cmdBlockDic;
@property (nonatomic, strong) JPBData *pbData;
@property (nonatomic, strong) NSOperationQueue *competeQueue;
@property (nonatomic, assign) BOOL isCompeteFinish;
@property (nonatomic, strong) NSMutableArray <SRWebSocket *> *competeSwsList;
@end

@implementation JWebSocket

- (instancetype)initWithSendQueque:(dispatch_queue_t)sendQueue receiveQueue:(dispatch_queue_t)receiveQueue {
    if (self = [super init]) {
        self.sendQueue = sendQueue;
        self.receiveQueue = receiveQueue;
        self.pbData = [[JPBData alloc] init];
        self.cmdBlockDic = [[NSMutableDictionary alloc] init];
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
        self.appKey = appKey;
        self.token = token;
        self.pushToken = pushToken;
        
        [self resetSws];
        for (NSString *url in servers) {
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
        [self sendDisconnectMsgByWebSocket:needPush];
    });
}

- (void)setConnectDelegate:(id<JWebSocketConnectDelegate>)delegate {
    _connectDelegate = delegate;
}

- (void)setMessageDelegate:(id<JWebSocketMessageDelegate>)delegate {
    _messageDelegate = delegate;
}

#pragma mark - send pb
- (void)sendIMMessage:(JMessageContent *)content
       inConversation:(nonnull JConversation *)conversation
          clientMsgNo:(long long)clientMsgNo
            clientUid:(NSString *)clientUid
           mergedMsgs:(NSArray <JConcreteMessage *> *)mergedMsgs
          isBroadcast:(BOOL)isBroadcast
               userId:(NSString *)userId
              success:(void (^)(long long clientMsgNo, NSString *msgId, long long timestamp, long long seqNo))successBlock
                error:(void (^)(JErrorCodeInternal errorCode, long long clientMsgNo))errorBlock{
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData sendMessageDataWithType:[[content class] contentType]
                                                 msgData:[content encode]
                                                   flags:[[content class] flags]
                                               clientUid:clientUid
                                              mergedMsgs:mergedMsgs
                                             isBroadcast:isBroadcast
                                                  userId:userId
                                                   index:self.cmdIndex++
                                        conversationType:conversation.conversationType
                                          conversationId:conversation.conversationId
                                             mentionInfo:content.mentionInfo];

        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            NSLog(@"WebSocket send IM message error, msg is %@", err.description);
            if (errorBlock) {
                errorBlock(JErrorCodeInternalWebSocketFailure, clientMsgNo);
            }
        } else {
            JSendMessageObj *obj = [[JSendMessageObj alloc] init];
            obj.clientMsgNo = clientMsgNo;
            obj.successBlock = successBlock;
            obj.errorBlock = errorBlock;
            [self setBlockObject:obj forKey:key];
        }
    });
}

- (void)recallMessage:(NSString *)messageId
         conversation:(JConversation *)conversation
            timestamp:(long long)timestamp
              success:(void (^)(long long timestamp))successBlock
                error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData recallMessageData:messageId
                                      conversation:conversation
                                         timestamp:timestamp
                                             index:self.cmdIndex++];
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)sendReadReceipt:(NSArray <NSString *> *)messageIds
         inConversation:(JConversation *)conversation
                success:(void (^)(void))successBlock
                  error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData sendReadReceiptData:messageIds
                                      inConversation:conversation
                                               index:self.cmdIndex++];
        [self simpleSendData:d
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
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            NSLog(@"WebSocket sync messages error, msg is %@", err.description);
        }
    });
}

- (void)queryHisMsgsFrom:(JConversation *)conversation
               startTime:(long long)startTime
                   count:(int)count
               direction:(JPullDirection)direction
                 success:(void (^)(NSArray * _Nonnull, BOOL))successBlock
                   error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData queryHisMsgsDataFrom:conversation
                                            startTime:startTime
                                                count:count
                                            direction:direction
                                                index:self.cmdIndex++];
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
                    error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData queryHisMsgsDataByIds:messageIds
                                        inConversation:conversation
                                                 index:self.cmdIndex++];
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
                    error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData syncConversationsData:startTime
                                                 count:count
                                                userId:userId
                                                 index:self.cmdIndex++];
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
                       success:(void (^)(void))successBlock
                         error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData deleteConversationData:conversation
                                                 userId:userId
                                                  index:self.cmdIndex++];
        [self simpleSendData:d
                         key:key
                     success:successBlock
                       error:errorBlock];
    });
}

- (void)clearUnreadCount:(JConversation *)conversation
                  userId:(NSString *)userId
                msgIndex:(long long)msgIndex
                 success:(void (^)(void))successBlock
                   error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData clearUnreadCountData:conversation
                                               userId:userId
                                             msgIndex:msgIndex
                                                index:self.cmdIndex++];
        [self simpleSendData:d
                         key:key
                     success:successBlock
                       error:errorBlock];
    });
}

- (void)setMute:(BOOL)isMute
 inConversation:(JConversation *)conversation
         userId:(NSString *)userId
        success:(void (^)(void))successBlock
          error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData undisturbData:conversation
                                        userId:userId
                                        isMute:isMute
                                         index:self.cmdIndex++];
        [self simpleSendData:d
                         key:key
                     success:successBlock
                       error:errorBlock];
    });
}

- (void)setTop:(BOOL)isTop
inConversation:(JConversation *)conversation
        userId:(NSString *)userId
       success:(void (^)(long long timestamp))successBlock
         error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData topConversationData:conversation
                                              userId:userId
                                               isTop:isTop
                                               index:self.cmdIndex++];
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
                       error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData getMergedMessageList:messageId
                                                 time:timestamp
                                                count:count
                                            direction:direction
                                                index:self.cmdIndex++];
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
                     error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData getMentionMessages:conversation
                                               time:time
                                              count:count
                                          direction:direction
                                              index:self.cmdIndex++];
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
                    error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData registerPushToken:token
                                          deviceId:[JUtility getDeviceId]
                                       packageName:[[NSBundle mainBundle] bundleIdentifier]
                                            userId:userId
                                             index:self.cmdIndex++];
        [self simpleSendData:d
                         key:key
                     success:successBlock
                       error:errorBlock];
    });
}

- (void)clearTotalUnreadCount:(NSString *)userId
                         time:(long long)time
                      success:(void (^)(void))successBlock
                        error:(void (^)(JErrorCodeInternal code))errorBlock{
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData clearTotalUnreadCountMessages:userId
                                                          time:time
                                                         index:self.cmdIndex++];
        [self simpleSendData:d
                         key:key
                     success:successBlock
                       error:errorBlock];
    });
    
}

- (void)sendPing {
    dispatch_async(self.sendQueue, ^{
        NSData *d = [self.pbData pingData];
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            NSLog(@"WebSocket ping error, msg is %@", err.description);
        }
    });
}

- (void)sendPublishAck:(int)index {
    dispatch_async(self.sendQueue, ^{
        NSData *d = [self.pbData publishAckData:index];
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            NSLog(@"WebSocket send publishAck error, msg is %@", err.description);
        }
    });
}

#pragma mark - SRWebSocketDelegate
- (void)webSocketDidOpen:(SRWebSocket *)webSocket {
    dispatch_async(self.sendQueue, ^{
        if (self.isCompeteFinish) {
            return;
        }
        //防止上一批竞速的 webSocket 被选中
        for (SRWebSocket *sws in self.competeSwsList) {
            if (webSocket == sws) {
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
    NSLog(@"[JetIM] websocket did fail with error, %@", error.description);
    if ([self.connectDelegate respondsToSelector:@selector(webSocketDidFail)]) {
        [self.connectDelegate webSocketDidFail];
    }
}

- (void)webSocket:(SRWebSocket *)webSocket didCloseWithCode:(NSInteger)code reason:(NSString *)reason wasClean:(BOOL)wasClean {
    if (webSocket != self.sws) {
        return;
    }
    NSLog(@"[JetIM] websocket did close with code(%ld), reason(%@)", (long)code, reason);
    if ([self.connectDelegate respondsToSelector:@selector(webSocketDidClose)]) {
        [self.connectDelegate webSocketDidClose];
    }
}

- (void)webSocket:(SRWebSocket *)webSocket didReceiveMessageWithData:(NSData *)data {
    if (webSocket != self.sws) {
        return;
    }
    JPBRcvObj *obj = [self.pbData rcvObjWithData:data];
    switch (obj.rcvType) {
        case JPBRcvTypeParseError:
            break;
        case JPBRcvTypeConnectAck:
            [self handleConnectAckMsg:obj.connectAck];
            break;
        case JPBRcvTypePublishMsgAck:
            [self handlePublishAckMsg:obj.publishMsgAck];
            break;
        case JPBRcvTypeQryHisMsgsAck:
            [self handleQryHisMsgs:obj.qryHisMsgsAck];
            break;
        case JPBRcvTypeSyncConvsAck:
            [self handleSyncConvsAck:obj.syncConvsAck];
            break;
        case JPBRcvTypePublishMsg:
            [self handleReceiveMessage:obj.publishMsgBody];
            break;
        case JPBRcvTypePublishMsgNtf:
            [self handlePublishMsgNtf:obj.publishMsgNtf];
            break;
        case JPBRcvTypeSyncMsgsAck:
            [self handleSyncMsgsAck:obj.qryHisMsgsAck];
            break;
        case JPBRcvTypePong:
            [self handlePong];
            break;
        case JPBRcvTypeDisconnectMsg:
            [self handleDisconnectMsg:obj.disconnectMsg];
            break;
        case JPBRcvTypeSimpleQryAck:
            [self handleSimpleAck:obj.simpleQryAck];
            break;
        case JPBRcvTypeQryReadDetailAck:
            [self handleQryReadDetailAck:obj.qryReadDetailAck];
            break;
        case JPBRcvTypeSimpleQryAckCallbackTimestamp:
            [self handleSimpleQryAckWithTimeCallback:obj.simpleQryAck];
            break;
        case JPBRcvTypeConversationSetTopAck:
            [self handleTimestampCallback:obj.timestampQryAck];
            break;
        default:
            break;
    }
}

#pragma mark - inner
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
        NSLog(@"WebSocket send connect error, msg is %@", err.description);
    }
}

- (void)sendDisconnectMsgByWebSocket:(BOOL)needPush {
    NSData *d = [self.pbData disconnectData:needPush];
    NSError *err = nil;
    [self.sws sendData:d error:&err];
    if (err != nil) {
        NSLog(@"WebSocket send disconnect error, msg is %@", err.description);
    }
}

- (void)handleConnectAckMsg:(JConnectAck *)connectAck {
    NSLog(@"connect userId is %@", connectAck.userId);
    if ([self.connectDelegate respondsToSelector:@selector(connectCompleteWithCode:userId:session:extra:)]) {
        [self.connectDelegate connectCompleteWithCode:connectAck.code
                                               userId:connectAck.userId
                                              session:connectAck.session
                                                extra:connectAck.extra];
    }
}

- (void)handlePublishAckMsg:(JPublishMsgAck *)ack {
    NSLog(@"handlePublishAckMsg, msgId is %@, code is %d", ack.msgId, ack.code);
    JBlockObj *obj = [self.cmdBlockDic objectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JSendMessageObj class]]) {
        JSendMessageObj *sendMessageObj = (JSendMessageObj *)obj;
        if (ack.code != 0) {
            sendMessageObj.errorBlock(ack.code, sendMessageObj.clientMsgNo);
        } else {
            sendMessageObj.successBlock(sendMessageObj.clientMsgNo, ack.msgId, ack.timestamp, ack.seqNo);
        }
    }
    [self removeBlockObjectForKey:@(ack.index)];
}

- (void)handleQryHisMsgs:(JQryHisMsgsAck *)ack {
    NSLog(@"handleQryMsg");
    JBlockObj *obj = [self.cmdBlockDic objectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JQryHisMsgsObj class]]) {
        JQryHisMsgsObj *qryHisMsgsObj = (JQryHisMsgsObj *)obj;
        if (ack.code != 0) {
            qryHisMsgsObj.errorBlock(ack.code);
        } else {
            qryHisMsgsObj.successBlock(ack.msgs, ack.isFinished);
        }
    }
    [self removeBlockObjectForKey:@(ack.index)];
}

- (void)handleSyncConvsAck:(JSyncConvsAck *)ack {
    NSLog(@"handleSyncConvsAck");
    JBlockObj *obj = [self.cmdBlockDic objectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JSyncConvsObj class]]) {
        JSyncConvsObj *syncConvsObj = (JSyncConvsObj *)obj;
        if (ack.code != 0) {
            syncConvsObj.errorBlock(ack.code);
        } else {
            syncConvsObj.successBlock(ack.convs, ack.deletedConvs, ack.isFinished);
        }
    }
    [self removeBlockObjectForKey:@(ack.index)];
}

//sync 和 queryHisMsgs 共用一个 ack
- (void)handleSyncMsgsAck:(JQryHisMsgsAck *)ack {
    NSLog(@"handleSyncMsgsAck");
    if ([self.messageDelegate respondsToSelector:@selector(messagesDidReceive:isFinished:)]) {
        [self.messageDelegate messagesDidReceive:ack.msgs isFinished:ack.isFinished];
    }
}

- (void)handleReceiveMessage:(JPublishMsgBody *)publishMsgBody {
    NSLog(@"handleReceiveMessage");
    if ([self.messageDelegate respondsToSelector:@selector(messageDidReceive:)]) {
        [self.messageDelegate messageDidReceive:publishMsgBody.rcvMessage];
    }
    if (publishMsgBody.qos == 1) {
        [self sendPublishAck:publishMsgBody.index];
    }
}

- (void)handlePublishMsgNtf:(JPublishMsgNtf *)ntf {
    NSLog(@"handlePublishMsgNtf");
    if ([self.messageDelegate respondsToSelector:@selector(syncNotify:)]) {
        [self.messageDelegate syncNotify:ntf.syncTime];
    }
}

- (void)handlePong {
    NSLog(@"handlePong");
}

- (void)handleDisconnectMsg:(JDisconnectMsg *)msg {
    NSLog(@"handleDisconnectMsg");
    dispatch_async(self.sendQueue, ^{
        [self resetSws];
        //TODO: 处理 cmdBlockDic
        if ([self.connectDelegate respondsToSelector:@selector(disconnectWithCode:extra:)]) {
            [self.connectDelegate disconnectWithCode:msg.code extra:msg.extra];
        }
    });
}

- (void)handleSimpleAck:(JSimpleQryAck *)ack {
    NSLog(@"handleSimpleAck, code is %d", ack.code);
    JBlockObj *obj = [self.cmdBlockDic objectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JSimpleBlockObj class]]) {
        JSimpleBlockObj *simpleObj = (JSimpleBlockObj *)obj;
        if (ack.code != 0) {
            simpleObj.errorBlock(ack.code);
        } else {
            simpleObj.successBlock();
        }
    }
    [self removeBlockObjectForKey:@(ack.index)];
}

- (void)handleSimpleQryAckWithTimeCallback:(JSimpleQryAck *)ack {
    NSLog(@"handleSimpleQryAckWithtimeCallback, code is %d", ack.code);
    JBlockObj *obj = [self.cmdBlockDic objectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JTimestampBlockObj class]]) {
        JTimestampBlockObj *simpleObj = (JTimestampBlockObj *)obj;
        if (ack.code != 0) {
            simpleObj.errorBlock(ack.code);
        } else {
            simpleObj.successBlock(ack.timestamp);
        }
    }
    [self removeBlockObjectForKey:@(ack.index)];
}

- (void)handleTimestampCallback:(JTimestampQryAck *)ack {
    NSLog(@"handleTimestampCallback, code is %d", ack.code);
    JBlockObj *obj = [self.cmdBlockDic objectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JTimestampBlockObj class]]) {
        JTimestampBlockObj *timestampObj = (JTimestampBlockObj *)obj;
        if (ack.code != 0) {
            timestampObj.errorBlock(ack.code);
        } else {
            timestampObj.successBlock(ack.operationTime);
        }
    }
    [self removeBlockObjectForKey:@(ack.index)];
}

- (void)handleQryReadDetailAck:(JQryReadDetailAck *)ack {
    NSLog(@"handleQryReadDetailAck, code is %d", ack.code);
    JBlockObj *obj = [self.cmdBlockDic objectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JQryReadDetailObj class]]) {
        JQryReadDetailObj *qryReadDetailObj = (JQryReadDetailObj *)obj;
        if (ack.code != 0) {
            qryReadDetailObj.errorBlock(ack.code);
        } else {
            qryReadDetailObj.successBlock(ack.readMembers, ack.unreadMembers);
        }
    }
    [self removeBlockObjectForKey:@(ack.index)];
}

- (void)simpleSendData:(NSData *)data
                   key:(NSNumber *)key
               success:(void (^)(void))successBlock
                 error:(void (^)(JErrorCodeInternal))errorBlock {
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
                    error:(void (^)(JErrorCodeInternal))errorBlock {
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
           error:(void (^)(JErrorCodeInternal))errorBlock {
    NSError *err = nil;
    [self.sws sendData:data error:&err];
    if (err != nil) {
        NSLog(@"WebSocket send data error, description is %@", err.description);
        if (errorBlock) {
            errorBlock(JErrorCodeInternalWebSocketFailure);
        }
    } else {
        [self setBlockObject:obj forKey:key];
    }
}

- (void)setBlockObject:(JBlockObj *)obj
               forKey:(NSNumber *)index {
    dispatch_async(self.receiveQueue, ^{
        [self.cmdBlockDic setObject:obj forKey:index];
    });
}

- (void)removeBlockObjectForKey:(NSNumber *)index {
    dispatch_async(self.receiveQueue, ^{
        [self.cmdBlockDic removeObjectForKey:index];
    });
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
