//
//  JWebSocket.m
//  JetIM
//
//  Created by Nathan on 2023/11/29.
//

#import "JWebSocket.h"
#import "SRWebSocket.h"
#import "JMessageContent+internal.h"
#import "JUtility.h"
#import "JetIMConstInternal.h"
#import "JPBData.h"

#define jWebSocketPrefix @"ws://"
#define jWebSocketSuffix @"/im"

@interface JBlockObj : NSObject
@end

@implementation JBlockObj
@end

@interface JSendMessageObj : JBlockObj
@property (nonatomic, assign) long long clientMsgNo;
@property (nonatomic, copy) void (^successBlock)(long long clientMsgNo, NSString *msgId, long long timestamp, long long msgIndex);
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

@interface JRecallMsgObj : JBlockObj
@property (nonatomic, copy) NSString *messageId;
@property (nonatomic, copy) void (^successBlock)(long long timestamp);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@implementation JRecallMsgObj
@end

@interface JSimpleBlockObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(long long timestamp);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@implementation JSimpleBlockObj
@end

@interface JWebSocket () <SRWebSocketDelegate>
@property (nonatomic, weak) id<JWebSocketConnectDelegate> connectDelegate;
@property (nonatomic, weak) id<JWebSocketMessageDelegate> messageDelegate;
@property (nonatomic, copy) NSString *appKey;
@property (nonatomic, copy) NSString *token;
@property (nonatomic, copy) NSArray *servers;
@property (nonatomic, strong) SRWebSocket *sws;
@property (nonatomic, strong) dispatch_queue_t sendQueue;
@property (nonatomic, strong) dispatch_queue_t receiveQueue;
/// 所有上行数据的自增 index
@property (nonatomic, assign) int32_t msgIndex;
@property (nonatomic, strong) NSMutableDictionary<NSNumber *, JBlockObj *> *msgBlockDic;
@property (nonatomic, strong) JPBData *pbData;
@end

@implementation JWebSocket

- (instancetype)initWithSendQueque:(dispatch_queue_t)sendQueue receiveQueue:(dispatch_queue_t)receiveQueue {
    JWebSocket *ws = [[JWebSocket alloc] init];
    ws.sendQueue = sendQueue;
    ws.receiveQueue = receiveQueue;
    ws.pbData = [[JPBData alloc] init];
    ws.msgBlockDic = [[NSMutableDictionary alloc] init];
    return ws;
}

- (void)connect:(NSString *)appKey
          token:(NSString *)token
        servers:(nonnull NSArray *)servers {
    dispatch_async(self.sendQueue, ^{
        self.appKey = appKey;
        self.token = token;
        self.servers = servers;
        if (servers.count > 0) {
            NSString *u = [NSString stringWithFormat:@"%@%@%@", jWebSocketPrefix, self.servers[0], jWebSocketSuffix];
            self.sws = [[SRWebSocket alloc] initWithURLRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:u]]];
            self.sws.delegateDispatchQueue = self.receiveQueue;
            self.sws.delegate = self;
            [self.sws open];
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
              success:(void (^)(long long clientMsgNo, NSString *msgId, long long timestamp, long long msgIndex))successBlock
                error:(void (^)(JErrorCodeInternal errorCode, long long clientMsgNo))errorBlock{
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.msgIndex);
        NSData *d = [self.pbData sendMessageDataWithType:[[content class] contentType]
                                             msgData:[content encode]
                                               flags:[[content class] flags]
                                           clientUid:clientUid
                                               index:self.msgIndex++
                                    conversationType:conversation.conversationType
                                      conversationId:conversation.conversationId];

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
                error:(void (^)(JErrorCodeInternal))errorBlock{
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.msgIndex);
        NSData *d = [self.pbData recallMessageData:messageId
                                      conversation:conversation
                                         timestamp:timestamp
                                             index:self.msgIndex++];
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            NSLog(@"WebSocket recall message error, description is %@", err.description);
            if (errorBlock) {
                errorBlock(JErrorCodeInternalWebSocketFailure);
            }
        } else {
            JRecallMsgObj *obj = [[JRecallMsgObj alloc] init];
            obj.messageId = messageId;
            obj.successBlock = successBlock;
            obj.errorBlock = errorBlock;
            [self setBlockObject:obj forKey:key];
        }
    });
}

- (void)syncMessagesWithReceiveTime:(long long)receiveTime
                           sendTime:(long long)sendTime
                             userId:(NSString *)userId {
    dispatch_async(self.sendQueue, ^{
        NSData *d = [self.pbData syncMessagesDataWithReceiveTime:receiveTime
                                                        sendTime:sendTime
                                                          userId:userId
                                                           index:self.msgIndex++];
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
        NSNumber *key = @(self.msgIndex);
        NSData *d = [self.pbData queryHisMsgsDataFrom:conversation
                                            startTime:startTime
                                                count:count
                                            direction:direction
                                                index:self.msgIndex++];
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            NSLog(@"WebSocket query history message error, msg is %@", err.description);
            if (errorBlock) {
                errorBlock(JErrorCodeInternalWebSocketFailure);
            }
        } else {
            JQryHisMsgsObj *obj = [[JQryHisMsgsObj alloc] init];
            obj.successBlock = successBlock;
            obj.errorBlock = errorBlock;
            [self setBlockObject:obj forKey:key];
        }
    });
}

- (void)syncConversations:(long long)startTime
                    count:(int)count
                   userId:(NSString *)userId
                  success:(void (^)(NSArray * _Nonnull, NSArray * _Nonnull, BOOL))successBlock
                    error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.msgIndex);
        NSData *d = [self.pbData syncConversationsData:startTime
                                             count:count
                                            userId:userId
                                             index:self.msgIndex++];
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            NSLog(@"WebSocket sync conversations error, msg is %@", err.description);
            if (errorBlock) {
                errorBlock(JErrorCodeInternalWebSocketFailure);
            }
        } else {
            JSyncConvsObj *obj = [[JSyncConvsObj alloc] init];
            obj.successBlock = successBlock;
            obj.errorBlock = errorBlock;
            [self setBlockObject:obj forKey:key];
        }
    });
}

- (void)deleteConversationInfo:(JConversation *)conversation
                        userId:(NSString *)userId
                       success:(void (^)(long long timestamp))successBlock
                         error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.msgIndex);
        NSData *d = [self.pbData deleteConversationData:conversation
                                                 userId:userId
                                                  index:self.msgIndex++];
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            NSLog(@"WebSocket delete conversation error, msg is %@", err.description);
            if (errorBlock) {
                errorBlock(JErrorCodeInternalWebSocketFailure);
            }
        } else {
            JSimpleBlockObj *obj = [[JSimpleBlockObj alloc] init];
            obj.successBlock = successBlock;
            obj.errorBlock = errorBlock;
            [self setBlockObject:obj forKey:key];
        }
    });
}

- (void)clearUnreadCount:(JConversation *)conversation
                  userId:(NSString *)userId
                msgIndex:(long long)msgIndex
                 success:(void (^)(long long timestamp))successBlock
                   error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.msgIndex);
        NSData *d = [self.pbData clearUnreadCountData:conversation
                                               userId:userId
                                             msgIndex:msgIndex
                                                index:self.msgIndex++];
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            NSLog(@"WebSocket clear unread count error, msg is %@", err.description);
            if (errorBlock) {
                errorBlock(JErrorCodeInternalWebSocketFailure);
            }
        } else {
            JSimpleBlockObj *obj = [[JSimpleBlockObj alloc] init];
            obj.successBlock = successBlock;
            obj.errorBlock = errorBlock;
            [self setBlockObject:obj forKey:key];
        }
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
        [self sendConnectMsgByWebSocket:webSocket];
    });
}

- (void)webSocket:(SRWebSocket *)webSocket didFailWithError:(NSError *)error {
    NSLog(@"[JetIM] websocket did fail with error, %@", error.description);
    if ([self.connectDelegate respondsToSelector:@selector(webSocketDidFail)]) {
        [self.connectDelegate webSocketDidFail];
    }
}

- (void)webSocket:(SRWebSocket *)webSocket didCloseWithCode:(NSInteger)code reason:(NSString *)reason wasClean:(BOOL)wasClean {
    NSLog(@"[JetIM] websocket did close with code(%ld), reason(%@)", (long)code, reason);
    if ([self.connectDelegate respondsToSelector:@selector(webSocketDidClose)]) {
        [self.connectDelegate webSocketDidClose];
    }
}

- (void)webSocket:(SRWebSocket *)webSocket didReceiveMessageWithData:(NSData *)data {
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
        case JPBRcvTypeRecall:
            [self handleRecallMessage:obj.publishMsgAck];
            break;
        case JPBRcvTypeDelConvsAck:
            [self handleDelConvs:obj.simpleQryAck];
            break;
        case JPBRcvTypeClearUnreadAck:
            [self handleClearUnread:obj.simpleQryAck];
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
                                     pushToken:@"pushToken"//TODO:
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
    if ([self.connectDelegate respondsToSelector:@selector(connectCompleteWithCode:userId:)]) {
        [self.connectDelegate connectCompleteWithCode:connectAck.code
                                               userId:connectAck.userId];
    }
}

- (void)handlePublishAckMsg:(JPublishMsgAck *)ack {
    NSLog(@"handlePublishAckMsg, msgId is %@, code is %d", ack.msgId, ack.code);
    JBlockObj *obj = [self.msgBlockDic objectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JSendMessageObj class]]) {
        JSendMessageObj *sendMessageObj = (JSendMessageObj *)obj;
        if (ack.code != 0) {
            sendMessageObj.errorBlock(ack.code, sendMessageObj.clientMsgNo);
        } else {
            sendMessageObj.successBlock(sendMessageObj.clientMsgNo, ack.msgId, ack.timestamp, ack.msgIndex);
        }
    }
    [self removeBlockObjectForKey:@(ack.index)];
}

- (void)handleQryHisMsgs:(JQryHisMsgsAck *)ack {
    NSLog(@"handleQryMsg");
    JBlockObj *obj = [self.msgBlockDic objectForKey:@(ack.index)];
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
    JBlockObj *obj = [self.msgBlockDic objectForKey:@(ack.index)];
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
    if ([self.connectDelegate respondsToSelector:@selector(disconnectWithCode:)]) {
        [self.connectDelegate disconnectWithCode:msg.code];
    }
}

- (void)handleRecallMessage:(JPublishMsgAck *)ack {
    NSLog(@"handleRecallMessage, code is %d", ack.code);
    JBlockObj *obj = [self.msgBlockDic objectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JRecallMsgObj class]]) {
        JRecallMsgObj *recallObj = (JRecallMsgObj *)obj;
        if (ack.code != 0) {
            recallObj.errorBlock(ack.code);
        } else {
            recallObj.successBlock(ack.timestamp);
        }
    }
    [self removeBlockObjectForKey:@(ack.index)];
}

- (void)handleDelConvs:(JSimpleQryAck *)ack {
    NSLog(@"handleDelConvs, code is %d", ack.code);
    JBlockObj *obj = [self.msgBlockDic objectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JSimpleBlockObj class]]) {
        JSimpleBlockObj *simpleObj = (JSimpleBlockObj *)obj;
        if (ack.code != 0) {
            simpleObj.errorBlock(ack.code);
        } else {
            simpleObj.successBlock(ack.timestamp);
        }
    }
}

- (void)handleClearUnread:(JSimpleQryAck *)ack {
    NSLog(@"handleClearUnread, code is %d", ack.code);
    JBlockObj *obj = [self.msgBlockDic objectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JSimpleBlockObj class]]) {
        JSimpleBlockObj *simpleObj = (JSimpleBlockObj *)obj;
        if (ack.code != 0) {
            simpleObj.errorBlock(ack.code);
        } else {
            simpleObj.successBlock(ack.timestamp);
        }
    }
}

- (void)setBlockObject:(JBlockObj *)obj
               forKey:(NSNumber *)index {
    dispatch_async(self.receiveQueue, ^{
        [self.msgBlockDic setObject:obj forKey:index];
    });
}



- (void)removeBlockObjectForKey:(NSNumber *)index {
    dispatch_async(self.receiveQueue, ^{
        [self.msgBlockDic removeObjectForKey:index];
    });
}
@end
