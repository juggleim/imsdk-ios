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
@property(nonatomic, weak) id<JWebSocketConnectDelegate> connectDelegate;
@property(nonatomic, strong) JConnectInfo *connectInfo;
@property(nonatomic, strong) SRWebSocket *sws;
@end

@implementation JWebSocket

- (void)connect:(JConnectInfo *)info {
    self.sws = [[SRWebSocket alloc] initWithURLRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:JWebSocketURL]]];
    self.sws.delegate = self;
    [self.sws open];
}

- (void)setConnectDelegate:(id<JWebSocketConnectDelegate>)delegate {
    _connectDelegate = delegate;
}

- (void)sendIMMessage:(JMessageContent *)content
       inConversation:(nonnull JConversation *)conversation {
    
    //TODO:
    UpMsg *upMsg = [[UpMsg alloc] init];
    upMsg.msgType = [[content class] contentType];
    upMsg.msgContent = [content encode];
    upMsg.flags = 1;
    upMsg.clientUid = @"11";
    
    PublishMsgBody *publishMsg = [[PublishMsgBody alloc] init];
    publishMsg.index = 1;
    publishMsg.topic = @"topic";
    publishMsg.targetId = @"userid2";
    publishMsg.timestamp = 123456;
    publishMsg.data_p = [upMsg data];
    
    [self.sws sendData:publishMsg.data error:nil];
}

#pragma mark - SRWebSocketDelegate
- (void)webSocketDidOpen:(SRWebSocket *)webSocket {
    [self sendConnectMsgByWebSocket:webSocket];
}

- (void)webSocket:(SRWebSocket *)webSocket didReceiveMessageWithData:(NSData *)data {
    NSError *err = nil;
    ImWebsocketMsg *msg = [[ImWebsocketMsg alloc] initWithData:data error:&err];
    JErrorCode code = JErrorCodeNone;
    if (err != nil) {
        NSLog(@"[Juggle]Web socket receive message error, msg is %@", err.description);
        code = JErrorCodeWebSocketFailure;
    } else {
        
        if (msg.testofOneOfCase == ImWebsocketMsg_Testof_OneOfCase_ConnectAckMsgBody) {
            if (self.connectDelegate) {
                [self.connectDelegate connectCompleteWithCode:msg.connectAckMsgBody.code
                                                       userId:msg.connectAckMsgBody.userId];
            }
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
    
    //TODO:
    connectMsg.deviceId = @"iOS_Simulator1";
    connectMsg.platform = JPlatform;
    connectMsg.deviceCompany = JDeviceCompany;
    connectMsg.deviceModel = @"iOS_Simulator";
    connectMsg.deviceOsVersion = @"iOS9";
    connectMsg.pushToken = @"pushToken";
    connectMsg.networkId = @"4G";
    connectMsg.ispNum = @"ispNum";
    connectMsg.clientIp = @"clientIp";
    
    ImWebsocketMsg *sm = [[ImWebsocketMsg alloc] init];
    sm.version = JuggleProtocolVersion;
    sm.cmd = JCmdTypeConnect;
    sm.qos = JQosYes;
    sm.connectMsgBody = connectMsg;
    NSError *err = nil;
    [sws sendData:sm.data error:&err];
    if (err != nil) {
        NSLog(@"WebSocket send data error, msg is %@", err.description);
    }
}

@end
