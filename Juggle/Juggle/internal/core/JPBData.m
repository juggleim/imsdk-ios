//
//  JPBData.m
//  Juggle
//
//  Created by Nathan on 2023/12/6.
//

#import "JPBData.h"
#import "ImWebSocket.pbobjc.h"
#import "Appmessages.pbobjc.h"
#import "JugglePBConst.h"


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

@implementation JConnectAck
@end

@implementation JPublishMsgAck
@end

@implementation JAck
@end

@implementation JPBData

+ (NSData *)connectDataWithAppKey:(NSString *)appKey
                            token:(NSString *)token
                         deviceId:(NSString *)deviceId
                         platform:(NSString *)platform
                    deviceCompany:(NSString *)deviceCompany
                      deviceModel:(NSString *)deviceModel
                  deviceOsVersion:(NSString *)osVersion
                        pushToken:(NSString *)pushToken
                        networkId:(NSString *)networkId
                           ispNum:(NSString *)ispNum
                         clientIp:(NSString *)clientIp {
    ConnectMsgBody *connectMsg = [[ConnectMsgBody alloc] init];
    connectMsg.protoId = JProtoId;
    connectMsg.sdkVersion = JSDKVersion;
    connectMsg.appkey = appKey;
    connectMsg.token = token;
    
    connectMsg.deviceId = deviceId;
    connectMsg.platform = platform;
    connectMsg.deviceCompany = deviceCompany;
    connectMsg.deviceModel = deviceModel;
    connectMsg.deviceOsVersion = osVersion;
    connectMsg.pushToken = pushToken;
    connectMsg.networkId = networkId;
    connectMsg.ispNum = ispNum;
    connectMsg.clientIp = clientIp;
    
    ImWebsocketMsg *sm = [self createImWebsocketMsg];
    sm.cmd = JCmdTypeConnect;
    sm.qos = JQosYes;
    sm.connectMsgBody = connectMsg;
    return sm.data;
}

+ (NSData *)sendMessageDataWithType:(NSString *)contentType
                            msgData:(NSData *)msgData
                              flags:(int)flags
                          clientUid:(NSString *)clientUid
                              index:(int)index
                   conversationType:(JConversationType)conversationType
                     conversationId:(NSString *)conversationId {
    UpMsg *upMsg = [[UpMsg alloc] init];
    upMsg.msgType = contentType;
    upMsg.msgContent = msgData;
    upMsg.flags = flags;
    upMsg.clientUid = clientUid;

    PublishMsgBody *publishMsg = [[PublishMsgBody alloc] init];
    publishMsg.index = index;
    switch (conversationType) {
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
    publishMsg.targetId = conversationId;
    publishMsg.data_p = [upMsg data];

    ImWebsocketMsg *sm = [self createImWebsocketMsg];
    sm.cmd = JCmdTypePublish;
    sm.qos = JQosYes;
    sm.publishMsgBody = publishMsg;
    return sm.data;
}

+ (JAck *)ackWithData:(NSData *)data {
    JAck *ack = [[JAck alloc] init];
    
    NSError *err = nil;
    ImWebsocketMsg *msg = [[ImWebsocketMsg alloc] initWithData:data error:&err];
    if (err != nil) {
        NSLog(@"[Juggle]Websocket receive message parse error, msg is %@", err.description);
        ack.ackType = JAckTypeParseError;
        return ack;
    }
    switch (msg.testofOneOfCase) {
        case ImWebsocketMsg_Testof_OneOfCase_ConnectAckMsgBody:
        {
            ack.ackType = JAckTypeConnect;
            JConnectAck *a = [[JConnectAck alloc] init];
            a.userId = msg.connectAckMsgBody.userId;
            a.code = msg.connectAckMsgBody.code;
            ack.connectAck = a;
        }
            break;
            
        case ImWebsocketMsg_Testof_OneOfCase_PubAckMsgBody:
        {
            ack.ackType = JAckTypePublishMsg;
            JPublishMsgAck *a = [[JPublishMsgAck alloc] init];
            a.index = msg.pubAckMsgBody.index;
            a.code = msg.pubAckMsgBody.code;
            a.msgId = msg.pubAckMsgBody.msgId;
            a.timestamp = msg.pubAckMsgBody.timestamp;
            ack.publishMsgAck = a;
        }
            break;
            
            //TODO: 
        default:
            break;
    }
    return ack;
}

#pragma mark - internal
+ (ImWebsocketMsg *)createImWebsocketMsg {
    ImWebsocketMsg *m = [[ImWebsocketMsg alloc] init];
    m.version = JuggleProtocolVersion;
    return m;
}

@end
