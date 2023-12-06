//
//  JWebSocket.m
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

#import "JWebSocket.h"
#import "SRWebSocket.h"
#import "JuggleConstInternal.h"
#import "ImWebSocket.pbobjc.h"
#import "Appmessages.pbobjc.h"
#import "JMessageContent+internal.h"
#import "JUtility.h"

typedef NS_ENUM(NSUInteger, JCmdType) {
    JCmdTypeConnect = 0,
    JCmdTypeConnectAck = 1,
    JCmdTypeDisconnect = 2,
    JCmdTypePublish = 3,
    JCmdTypePublishAck = 4,
    JCmdTypeQuery = 5,
    JCmdTypeQueryAck = 6,
    JCmdTypeQueryConfirm = 7,
    JCmdTypePing = 8,
    JCmdTypePong = 9
};

typedef NS_ENUM(NSUInteger, JQos) {
    //不需要响应 ack
    JQosNo = 0,
    //需要响应 ack
    JQosYes = 1
};

@implementation JConnectInfo
@end

@interface JWebSocket () <SRWebSocketDelegate>
@property (nonatomic, weak) id<JWebSocketConnectDelegate> connectDelegate;
@property (nonatomic, strong) JConnectInfo *connectInfo;
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

- (void)connect:(JConnectInfo *)info {
    dispatch_async(self.sendQueue, ^{
        self.connectInfo = info;
        [self.sws open];
    });
}

- (void)setConnectDelegate:(id<JWebSocketConnectDelegate>)delegate {
    _connectDelegate = delegate;
}

- (void)sendIMMessage:(JMessageContent *)content
       inConversation:(nonnull JConversation *)conversation {
    dispatch_async(self.sendQueue, ^{
        UpMsg *upMsg = [[UpMsg alloc] init];
        upMsg.msgType = [[content class] contentType];
        upMsg.msgContent = [content encode];
        upMsg.flags = [[content class] flags];
        upMsg.clientUid = [self createClientUid];

        PublishMsgBody *publishMsg = [[PublishMsgBody alloc] init];
        publishMsg.index = self.msgIndex++;
        switch (conversation.conversationType) {
            case JConversationTypePrivate:
                publishMsg.topic = @"p_msg";
                break;
                
            case JConversationTypeGroup:
                publishMsg.topic = @"g_msg";
                break;
                
                //TODO: 聊天室和系统会话还没做
            case JConversationTypeChatroom:
    //            publishMsg.topic
                break;
                
            case JConversationTypeSystem:
    //
                break;
            default:
                break;
        }
        publishMsg.targetId = conversation.conversationId;
        publishMsg.data_p = [upMsg data];

        ImWebsocketMsg *sm = [[ImWebsocketMsg alloc] init];
        sm.version = JuggleProtocolVersion;
        sm.cmd = JCmdTypePublish;
        sm.qos = JQosYes;
        sm.publishMsgBody = publishMsg;

        NSError *err = nil;
        [self.sws sendData:sm.data error:&err];
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

- (void)webSocket:(SRWebSocket *)webSocket didReceiveMessageWithData:(NSData *)data {
    NSError *err = nil;
    ImWebsocketMsg *msg = [[ImWebsocketMsg alloc] initWithData:data error:&err];
    JErrorCode code = JErrorCodeNone;
    if (err != nil) {
        NSLog(@"[Juggle]Web socket receive message parse error, msg is %@", err.description);
        code = JErrorCodeWebSocketParseFailure;
    } else {
        
        if (msg.testofOneOfCase == ImWebsocketMsg_Testof_OneOfCase_ConnectAckMsgBody) {
            [self handleConnectAckMsg:msg];
        }
        
    }
    
    
    
    NSLog(@"connect userId is %@", msg.connectAckMsgBody.userId);
    
    
}

#pragma mark - inner
- (void)sendConnectMsgByWebSocket:(SRWebSocket *)sws {
    ConnectMsgBody *connectMsg = [[ConnectMsgBody alloc] init];
    connectMsg.protoId = JProtoId;
    connectMsg.sdkVersion = JSDKVersion;
    connectMsg.appkey = self.connectInfo.appKey;
    connectMsg.token = self.connectInfo.token;
    
    connectMsg.deviceId = [JUtility getDeviceId];
    connectMsg.platform = JPlatform;
    connectMsg.deviceCompany = JDeviceCompany;
    connectMsg.deviceModel = [JUtility currentDeviceModel];
    connectMsg.deviceOsVersion = [JUtility currentSystemVersion];
    connectMsg.pushToken = @"pushToken";//TODO: 
    connectMsg.networkId = [JUtility currentNetWork];
    connectMsg.ispNum = [JUtility currentCarrier];
    connectMsg.clientIp = @"";
    
    ImWebsocketMsg *sm = [[ImWebsocketMsg alloc] init];
    sm.version = JuggleProtocolVersion;
    sm.cmd = JCmdTypeConnect;
    sm.qos = JQosYes;
    sm.connectMsgBody = connectMsg;
    NSError *err = nil;
    [sws sendData:sm.data error:&err];
    if (err != nil) {
        NSLog(@"WebSocket send connect error, msg is %@", err.description);
    }
}

- (NSString *)createClientUid {
    long long ts = [[NSDate date] timeIntervalSince1970];
    ts = ts % 1000;
    int32_t msgId = self.msgIndex % 1000;
    return [NSString stringWithFormat:@"%04lld%04d", ts, msgId];
}

//TODO: test
- (void)qryHistoryMessages {
    QryHisMsgsReq *req = [[QryHisMsgsReq alloc] init];
    req.targetId = @"userid2:userid1";
    req.channelType = ChannelType_Private;
    req.startTime = [[NSDate date] timeIntervalSince1970];
    req.count = 5;
    req.order = 0;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = 3;
    body.topic = @"topic";
    body.targetId = @"userid1";
    body.timestamp = [[NSDate date] timeIntervalSince1970];
    body.data_p = [req data];
    
    ImWebsocketMsg *sm = [[ImWebsocketMsg alloc] init];
    sm.version = JuggleProtocolVersion;
    sm.cmd = JCmdTypeQuery;
    sm.qos = JQosYes;
    sm.qryMsgBody = body;
    
    NSError *err = nil;
    [self.sws sendData:sm.data error:&err];
    if (err != nil) {
        NSLog(@"WebSocket query history message error, msg is %@", err.description);
    }
}

- (void)handleConnectAckMsg:(ImWebsocketMsg *)msg {
    if (self.connectDelegate) {
        [self.connectDelegate connectCompleteWithCode:msg.connectAckMsgBody.code
                                               userId:msg.connectAckMsgBody.userId];
    }
}

@end
