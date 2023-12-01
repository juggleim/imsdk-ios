//
//  JConnectionManager.m
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

#import "JConnectionManager.h"
#import "SRWebSocket.h"
#import "JuggleConstInternal.h"
#import "ImWebSocket.pbobjc.h"

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


@interface JConnectionManager () <SRWebSocketDelegate>
@property (nonatomic, strong) JuggleCore *core;
@property (nonatomic, weak) id<JConnectionDelegate> delegate;
@end

@implementation JConnectionManager

- (instancetype)initWithCore:(JuggleCore *)core {
    JConnectionManager *m = [[JConnectionManager alloc] init];
    m.core = core;
    return m;
}

- (void)connectWithToken:(NSString *)token {
    self.core.token = token;
    
    //TODO: navi
    
    SRWebSocket *sws = [[SRWebSocket alloc] initWithURLRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:JWebSocketURL]]];
    sws.delegate = self;
    [sws open];
}

- (void)disconnect:(BOOL)receivePush {
    
}

- (void)setDelegate:(id<JConnectionDelegate>)delegate {
    _delegate = delegate;
}

#pragma mark - SRWebSocketDelegate
- (void)webSocketDidOpen:(SRWebSocket *)webSocket {
    [self sendConnectMsgByWebSocket:webSocket];
}

- (void)webSocket:(SRWebSocket *)webSocket didReceiveMessageWithData:(NSData *)data {
    NSError *err = nil;
    ImWebsocketMsg *msg = [[ImWebsocketMsg alloc] initWithData:data error:&err];
    if (err != nil) {
        NSLog(@"Web socket receive message error, msg is %@", err.description);
    }
}

#pragma mark - inner
- (void)sendConnectMsgByWebSocket:(SRWebSocket *)sws {
    ConnectMsgBody *connectMsg = [[ConnectMsgBody alloc] init];
    connectMsg.protoId = JProtoId;
    connectMsg.sdkVersion = JSDKVersion;
    connectMsg.appkey = self.core.appKey;
    connectMsg.token = self.core.token;
    
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
