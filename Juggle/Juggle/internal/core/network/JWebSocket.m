//
//  JWebSocket.m
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

#import "JWebSocket.h"
#import "SRWebSocket.h"
#import "JMessageContent+internal.h"
#import "JUtility.h"
#import "JuggleConstInternal.h"
#import "JPBData.h"

@interface JBlockObj : NSObject
@end

@implementation JBlockObj
@end

@interface JSendMessageObj : JBlockObj
@property (nonatomic, assign) long clientMsgNo;
@property (nonatomic, copy) void (^successBlock)(long clientMsgNo, NSString *msgId, long long timestamp);
@property (nonatomic, copy) void (^errorBlock)(JErrorCode errorCode, long clientMsgNo);
@end

@implementation JSendMessageObj
@end

@interface JQryHisMsgsObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(NSArray * _Nonnull msgs, BOOL isFinished);
@property (nonatomic, copy) void (^errorBlock)(JErrorCode errorCode);
@end

@implementation JQryHisMsgsObj
@end

@interface JSyncConvsObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(NSArray * _Nonnull convs, BOOL isFinished);
@property (nonatomic, copy) void (^errorBlock)(JErrorCode errorCode);
@end

@implementation JSyncConvsObj
@end

@interface JWebSocket () <SRWebSocketDelegate>
@property (nonatomic, weak) id<JWebSocketConnectDelegate> connectDelegate;
@property (nonatomic, weak) id<JWebSocketMessageDelegate> messageDelegate;
@property (nonatomic, copy) NSString *appKey;
@property (nonatomic, copy) NSString *token;
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
    ws.sws = [[SRWebSocket alloc] initWithURLRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:JWebSocketURL]]];
    ws.sws.delegateDispatchQueue = ws.receiveQueue;
    ws.sws.delegate = ws;
    return ws;
}

- (void)connect:(NSString *)appKey token:(NSString *)token {
    dispatch_async(self.sendQueue, ^{
        self.appKey = appKey;
        self.token = token;
        [self.sws open];
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

- (void)registerMessageType:(Class)messageClass {
    [self.pbData registerMessageType:messageClass];
}

#pragma mark - send pb
- (void)sendIMMessage:(JMessageContent *)content
       inConversation:(nonnull JConversation *)conversation
          clientMsgNo:(long)clientMsgNo
              success:(void (^)(long clientMsgNo, NSString *msgId, long long timestamp))successBlock
                error:(void (^)(JErrorCode errorCode, long clientMsgNo))errorBlock{
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.msgIndex);
        NSData *d = [self.pbData sendMessageDataWithType:[[content class] contentType]
                                             msgData:[content encode]
                                               flags:[[content class] flags]
                                           clientUid:[self createClientUid]
                                               index:self.msgIndex++
                                    conversationType:conversation.conversationType
                                      conversationId:conversation.conversationId];

        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            NSLog(@"WebSocket send IM message error, msg is %@", err.description);
            if (errorBlock) {
                errorBlock(JErrorCodeWebSocketParseFailure, clientMsgNo);
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
                   error:(void (^)(JErrorCode))errorBlock {
    dispatch_async(self.sendQueue, ^{
        JQryHisMsgsObj *obj = [[JQryHisMsgsObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self setBlockObject:obj forKey:@(self.msgIndex)];
        NSData *d = [self.pbData queryHisMsgsDataFrom:conversation
                                            startTime:startTime
                                                count:count
                                            direction:direction
                                                index:self.msgIndex++];
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            NSLog(@"WebSocket query history message error, msg is %@", err.description);
            //TODO: callback
        }
    });
}

- (void)syncConversations:(long long)startTime
                    count:(int)count
                   userId:(NSString *)userId
                  success:(void (^)(NSArray * _Nonnull, BOOL))successBlock
                    error:(void (^)(JErrorCode))errorBlock {
    dispatch_async(self.sendQueue, ^{
        JSyncConvsObj *obj = [[JSyncConvsObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self setBlockObject:obj forKey:@(self.msgIndex)];
        NSData *d = [self.pbData syncConversationsData:startTime
                                             count:count
                                            userId:userId
                                             index:self.msgIndex++];
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            NSLog(@"WebSocket sync conversations error, msg is %@", err.description);
            //TODO: callback
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
    NSLog(@"[Juggle] websocket did fail with error, %@", error.description);
}

- (void)webSocket:(SRWebSocket *)webSocket didCloseWithCode:(NSInteger)code reason:(NSString *)reason wasClean:(BOOL)wasClean {
    NSLog(@"[Juggle] websocket did close with code(%ld), reason(%@)", (long)code, reason);
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
            [self handleReceiveMessage:obj.rcvMessage];
            break;
        case JPBRcvTypePublishMsgNtf:
            [self handlePublishMsgNtf:obj.publishMsgNtf];
            break;
        case JPBRcvTypeSyncMsgsAck:
            [self handleSyncMsgsAck:obj.qryHisMsgsAck];
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

- (NSString *)createClientUid {
    long long ts = [[NSDate date] timeIntervalSince1970];
    ts = ts % 1000;
    int32_t msgId = self.msgIndex % 1000;
    return [NSString stringWithFormat:@"%04lld%04d", ts, msgId];
}

- (void)handleConnectAckMsg:(JConnectAck *)connectAck {
    NSLog(@"connect userId is %@", connectAck.userId);
    if (self.connectDelegate) {
        [self.connectDelegate connectCompleteWithCode:connectAck.code
                                               userId:connectAck.userId];
    }
}

- (void)handlePublishAckMsg:(JPublishMsgAck *)ack {
    NSLog(@"handlePublishAckMsg, msgId is %@", ack.msgId);
    JBlockObj *obj = [self.msgBlockDic objectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JSendMessageObj class]]) {
        JSendMessageObj *sendMessageObj = (JSendMessageObj *)obj;
        if (ack.code != 0) {
            sendMessageObj.errorBlock(ack.code, sendMessageObj.clientMsgNo);
        } else {
            sendMessageObj.successBlock(sendMessageObj.clientMsgNo, ack.msgId, ack.timestamp);
        }
    }
    [self removeBlockObjectForKey:@(ack.index)];
}

- (void)handleQryHisMsgs:(JQryHisMsgsAck *)ack {
    NSLog(@"handleQryMsg");
}

- (void)handleSyncConvsAck:(JSyncConvsAck *)ack {
    NSLog(@"handleSyncConvsAck");
}

//sync 和 queryHisMsgs 共用一个 ack
- (void)handleSyncMsgsAck:(JQryHisMsgsAck *)ack {
    NSLog(@"handleSyncMsgsAck");
    if (self.messageDelegate) {
        [self.messageDelegate messagesDidReceive:ack.msgs isFinished:ack.isFinished];
    }
}

- (void)handleReceiveMessage:(JConcreteMessage *)message {
    NSLog(@"handleReceiveMessage");
    if (self.messageDelegate) {
        [self.messageDelegate messagesDidReceive:@[message] isFinished:YES];
    }
}

- (void)handlePublishMsgNtf:(JPublishMsgNtf *)ntf {
    NSLog(@"handlePublishMsgNtf");
    if (self.messageDelegate) {
        [self.messageDelegate syncNotify:ntf.syncTime];
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
