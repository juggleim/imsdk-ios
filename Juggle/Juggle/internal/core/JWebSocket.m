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

@interface JWebSocket () <SRWebSocketDelegate>
@property (nonatomic, weak) id<JWebSocketConnectDelegate> connectDelegate;
@property (nonatomic, copy) NSString *appKey;
@property (nonatomic, copy) NSString *token;
@property (nonatomic, strong) SRWebSocket *sws;
@property (nonatomic, strong) dispatch_queue_t sendQueue;
@property (nonatomic, strong) dispatch_queue_t receiveQueue;
/// 所有上行数据的自增 index
@property (nonatomic, assign) int32_t msgIndex;
@end

@implementation JWebSocket

- (instancetype)initWithSendQueque:(dispatch_queue_t)sendQueue receiveQueue:(dispatch_queue_t)receiveQueue {
    JWebSocket *ws = [[JWebSocket alloc] init];
    ws.sendQueue = sendQueue;
    ws.receiveQueue = receiveQueue;
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
        
    });
}

- (void)setConnectDelegate:(id<JWebSocketConnectDelegate>)delegate {
    _connectDelegate = delegate;
}

- (void)sendIMMessage:(JMessageContent *)content
       inConversation:(nonnull JConversation *)conversation {
    dispatch_async(self.sendQueue, ^{
        NSData *d = [JPBData sendMessageDataWithType:[[content class] contentType]
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
    JAck *ack = [JPBData ackWithData:data];
    switch (ack.ackType) {
        case JAckTypeParseError:
            break;
        case JAckTypeConnect:
            [self handleConnectAckMsg:ack.connectAck];
            break;
            
        case JAckTypePublishMsg:
            [self handlePublishAckMsg:ack.publishMsgAck];
            
        default:
            break;
    }
    
    
}

#pragma mark - inner
- (void)sendConnectMsgByWebSocket:(SRWebSocket *)sws {
    NSData *d = [JPBData connectDataWithAppKey:self.appKey
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
//    DisconnectMsgBody *body = [[DisconnectMsgBody alloc] init];
//    if (needPush) {
//        body.code = 1;
//    } else {
//        body.code = 0;
//    }
//    body.timestamp = [[NSDate date] timeIntervalSince1970];
//
//    ImWebsocketMsg *sm = [[ImWebsocketMsg alloc] init];
//    sm.version = JuggleProtocolVersion;
//    sm.cmd = JCmdTypeDisconnect;
//    sm
}

- (NSString *)createClientUid {
    long long ts = [[NSDate date] timeIntervalSince1970];
    ts = ts % 1000;
    int32_t msgId = self.msgIndex % 1000;
    return [NSString stringWithFormat:@"%04lld%04d", ts, msgId];
}

//TODO: test
- (void)qryHistoryMessages {
//    QryHisMsgsReq *req = [[QryHisMsgsReq alloc] init];
//    req.targetId = @"userid2:userid1";
//    req.channelType = ChannelType_Private;
//    req.startTime = [[NSDate date] timeIntervalSince1970];
//    req.count = 5;
//    req.order = 0;
//    
//    QueryMsgBody *body = [[QueryMsgBody alloc] init];
//    body.index = 3;
//    body.topic = @"topic";
//    body.targetId = @"userid1";
//    body.timestamp = [[NSDate date] timeIntervalSince1970];
//    body.data_p = [req data];
//    
//    ImWebsocketMsg *sm = [[ImWebsocketMsg alloc] init];
//    sm.version = JuggleProtocolVersion;
//    sm.cmd = JCmdTypeQuery;
//    sm.qos = JQosYes;
//    sm.qryMsgBody = body;
//    
//    NSError *err = nil;
//    [self.sws sendData:sm.data error:&err];
//    if (err != nil) {
//        NSLog(@"WebSocket query history message error, msg is %@", err.description);
//    }
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
}

@end
