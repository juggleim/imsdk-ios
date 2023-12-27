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
#import "JConcreteMessage.h"
#import "JMessageTypeCenter.h"

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

#define kPMsg @"p_msg"
#define kGMsg @"g_msg"
#define kQryHisMsgs @"qry_hismsgs"
#define kSyncConvers @"sync_convers"
#define kSyncMsgs @"sync_msgs"
#define jNtf @"ntf"
#define jMsg @"msg"

@implementation JConnectAck
@end

@implementation JPublishMsgAck
@end

@implementation JPublishMsgNtf
@end

@implementation JQryAck
- (void)encodeWithQueryAckMsgBody:(QueryAckMsgBody *)body {
    self.index = body.index;
    self.code = body.code;
    self.timestamp = body.timestamp;
}
@end

@implementation JQryHisMsgsAck
@end

@implementation JSyncConvsAck
@end

@implementation JPBRcvObj
@end

@interface JPBData ()
@property (nonatomic, strong) NSMutableDictionary *msgCmdDic;
@property (nonatomic, strong) NSDictionary *cmdAckPair;
@end

@implementation JPBData
- (instancetype)init {
    self = [super init];
    if (self) {
        self.msgCmdDic = [[NSMutableDictionary alloc] init];
    }
    return self;
}

- (NSData *)connectDataWithAppKey:(NSString *)appKey
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

- (NSData *)disconnectData:(BOOL)needPush {
    DisconnectMsgBody *body = [[DisconnectMsgBody alloc] init];
    if (needPush) {
        body.code = 0;
    } else {
        body.code = 1;
    }
    body.timestamp = [[NSDate date] timeIntervalSince1970]*1000;

    ImWebsocketMsg *sm = [self createImWebsocketMsg];
    sm.cmd = JCmdTypeDisconnect;
    sm.qos = JQosNo;
    sm.disconnectMsgBody = body;
    return sm.data;
}

- (NSData *)sendMessageDataWithType:(NSString *)contentType
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
            publishMsg.topic = kPMsg;
            break;
            
        case JConversationTypeGroup:
            publishMsg.topic = kGMsg;
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

- (NSData *)syncMessagesDataWithReceiveTime:(long long)receiveTime
                                   sendTime:(long long)sendTime
                                     userId:(NSString *)userId
                                      index:(int)index {
    SyncMsgReq *r = [[SyncMsgReq alloc] init];
    r.syncTime = receiveTime;
    r.containsSendBox = YES;
    r.sendBoxSyncTime = sendTime;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = kSyncMsgs;
    body.targetId = userId;
    body.data_p = r.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)queryHisMsgsDataFrom:(JConversation *)conversation
                       startTime:(long long)startTime
                           count:(int)count
                       direction:(JPullDirection)direction
                           index:(int)index {
    QryHisMsgsReq *r = [[QryHisMsgsReq alloc] init];
    r.targetId = conversation.conversationId;
    r.channelType = [self channelTypeFromConversationType:conversation.conversationType];
    r.startTime = startTime;
    r.count = count;
    if (direction == JPullDirectionNewToOld) {
        r.order = 0;
    } else {
        r.order = 1;
    }
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = kQryHisMsgs;
    body.targetId = conversation.conversationId;
    body.data_p = r.data;

    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)syncConversationsData:(long long)startTime
                            count:(int)count
                           userId:(NSString *)userId
                            index:(int)index {
    SyncConversationsReq *req = [[SyncConversationsReq alloc] init];
    req.startTime = startTime;
    req.count = count;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = kSyncConvers;
    body.targetId = userId;
    body.data_p = req.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)pingData {
    ImWebsocketMsg *m = [self createImWebsocketMsg];
    m.cmd = JCmdTypePing;
    m.qos = JQosNo;
    return m.data;
}

- (JPBRcvObj *)rcvObjWithData:(NSData *)data {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    
    NSError *err = nil;
    ImWebsocketMsg *msg = [[ImWebsocketMsg alloc] initWithData:data error:&err];
    if (err != nil) {
        NSLog(@"[Juggle]Websocket receive message parse error, msg is %@", err.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    if (msg.cmd == JCmdTypePong) {
        obj.rcvType = JPBRcvTypePong;
        return obj;
    }
    switch (msg.testofOneOfCase) {
        case ImWebsocketMsg_Testof_OneOfCase_ConnectAckMsgBody:
        {
            obj.rcvType = JPBRcvTypeConnectAck;
            JConnectAck *a = [[JConnectAck alloc] init];
            a.userId = msg.connectAckMsgBody.userId;
            a.code = msg.connectAckMsgBody.code;
            obj.connectAck = a;
        }
            break;
            
        case ImWebsocketMsg_Testof_OneOfCase_PubAckMsgBody:
        {
            obj.rcvType = JPBRcvTypePublishMsgAck;
            JPublishMsgAck *a = [[JPublishMsgAck alloc] init];
            a.index = msg.pubAckMsgBody.index;
            a.code = msg.pubAckMsgBody.code;
            a.msgId = msg.pubAckMsgBody.msgId;
            a.timestamp = msg.pubAckMsgBody.timestamp;
            obj.publishMsgAck = a;
        }
            break;
            
        case ImWebsocketMsg_Testof_OneOfCase_QryAckMsgBody:
        {
            NSString *cachedCmd;
            @synchronized (self) {
                cachedCmd = self.msgCmdDic[@(msg.qryAckMsgBody.index)];
                [self.msgCmdDic removeObjectForKey:@(msg.qryAckMsgBody.index)];
            }
            if (cachedCmd.length == 0) {
                NSLog(@"[Juggle]ack can't match a cachedCmd");
                obj.rcvType = JPBRcvTypeCmdMatchError;
                return obj;
            }
            JPBRcvType type = [self ackTypeWithCmd:cachedCmd];
            switch (type) {
                case JPBRcvTypeQryHisMsgsAck:
                    obj = [self queryHistoryMessagesAckWithImWebsocketMsg:msg];
                    break;
                    
                case JPBRcvTypeSyncConvsAck:
                    obj = [self syncConversationsAckWithImWebsocketMsg:msg];
                    break;
                    
                case JPBRcvTypeSyncMsgsAck:
                    obj = [self syncMsgsAckWithImWebsocketMsg:msg];
                    break;
                default:
                    break;
            }
        }
            break;
            
        case ImWebsocketMsg_Testof_OneOfCase_PublishMsgBody:
        {
            NSError *err = nil;
            if ([msg.publishMsgBody.topic isEqualToString:jNtf]) {
                NSLog(@"[Juggle] publish msg notify");
                Notify *ntf = [[Notify alloc] initWithData:msg.publishMsgBody.data_p error:&err];
                if (err != nil) {
                    NSLog(@"[Juggle]Websocket receive publish message notify parse error, msg is %@", err.description);
                    obj.rcvType = JPBRcvTypeParseError;
                    return obj;
                }
                if (ntf.type == NotifyType_Msg) {
                    obj.rcvType = JPBRcvTypePublishMsgNtf;
                    JPublishMsgNtf *n = [[JPublishMsgNtf alloc] init];
                    n.syncTime = ntf.syncTime;
                    obj.publishMsgNtf = n;
                }
            } else if ([msg.publishMsgBody.topic isEqualToString:jMsg]) {
                NSLog(@"[Juggle] publish msg direct");
                DownMsg *downMsg = [[DownMsg alloc] initWithData:msg.publishMsgBody.data_p error:&err];
                if (err != nil) {
                    NSLog(@"[Juggle]Websocket receive publish message parse error, msg is %@", err.description);
                    obj.rcvType = JPBRcvTypeParseError;
                    return obj;
                }
                obj.rcvType = JPBRcvTypePublishMsg;
                JConcreteMessage *message = [self messageWithDownMsg:downMsg];
                obj.rcvMessage = message;
            }
        }
            break;
            
            //TODO: 
        default:
            break;
    }
    return obj;
}

#pragma mark - internal
- (ImWebsocketMsg *)createImWebSocketMsgWithQueryMsg:(QueryMsgBody *)body {
    ImWebsocketMsg *m = [self createImWebsocketMsg];
    m.cmd = JCmdTypeQuery;
    m.qos = JQosYes;
    m.qryMsgBody = body;
    return m;
}

- (ImWebsocketMsg *)createImWebsocketMsg {
    ImWebsocketMsg *m = [[ImWebsocketMsg alloc] init];
    m.version = JuggleProtocolVersion;
    return m;
}

- (JConcreteMessage *)messageWithDownMsg:(DownMsg *)downMsg {
    JConcreteMessage *msg = [[JConcreteMessage alloc] init];
    JConversation *conversation = [[JConversation alloc] initWithConversationType:[self conversationTypeFromChannelType:downMsg.channelType]
                                                                   conversationId:downMsg.targetId];
    msg.conversation = conversation;
    msg.messageType = downMsg.msgType;
    msg.messageId = downMsg.msgId;
    msg.clientUid = downMsg.clientUid;
    msg.direction = downMsg.isSend ? JMessageDirectionSend : JMessageDirectionReceive;
    msg.hasRead = downMsg.isReaded;
    msg.timestamp = downMsg.msgTime;
    msg.senderUserId = downMsg.senderId;
    msg.msgIndex = downMsg.msgIndex;
    msg.content = [[JMessageTypeCenter shared] contentWithData:downMsg.msgContent
                                                   contentType:downMsg.msgType];
    return msg;
}

- (JConcreteConversationInfo *)conversationWithPBConversation:(Conversation *)conversation {
    JConcreteConversationInfo *info = [[JConcreteConversationInfo alloc] init];
    JConversation *c = [[JConversation alloc] initWithConversationType:[self conversationTypeFromChannelType:conversation.channelType]
                                                                   conversationId:conversation.targetId];
    info.conversation = c;
    info.unreadCount = (int)conversation.unreadCount;
    info.updateTime = conversation.updateTime;
    info.lastMessage = [self messageWithDownMsg:conversation.msg];
    info.lastReadMessageIndex = conversation.latestReadedMsgIndex;
    //TODO: mention
//    info.lastMentionMessage = [self messageWithDownMsg:conversation.latestMentionMsg];
    return info;
}

- (JPBRcvObj *)queryHistoryMessagesAckWithImWebsocketMsg:(ImWebsocketMsg *)msg {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    DownMsgSet *set = [[DownMsgSet alloc] initWithData:msg.qryAckMsgBody.data_p error:&e];
    if (e != nil) {
        NSLog(@"[Juggle]Websocket query history messages parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeQryHisMsgsAck;
    JQryHisMsgsAck *a = [[JQryHisMsgsAck alloc] init];
    [a encodeWithQueryAckMsgBody:msg.qryAckMsgBody];
    a.isFinished = set.isFinished;
    NSMutableArray *arr = [[NSMutableArray alloc] init];
    if (set.msgsArray_Count > 0) {
        [set.msgsArray enumerateObjectsUsingBlock:^(DownMsg * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            JConcreteMessage *msg = [self messageWithDownMsg:obj];
            [arr addObject:msg];
        }];
    }
    a.msgs = arr;
    obj.qryHisMsgsAck = a;
    return obj;
}

- (JPBRcvObj *)syncConversationsAckWithImWebsocketMsg:(ImWebsocketMsg *)msg {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    QryConversationsResp *resp = [[QryConversationsResp alloc] initWithData:msg.qryAckMsgBody.data_p error:&e];
    if (e != nil) {
        NSLog(@"[Juggle]Websocket sync conversations parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeSyncConvsAck;
    JSyncConvsAck *a = [[JSyncConvsAck alloc] init];
    [a encodeWithQueryAckMsgBody:msg.qryAckMsgBody];
    a.isFinished = resp.isFinished;
    NSMutableArray *arr = [[NSMutableArray alloc] init];
    if (resp.conversationsArray_Count > 0) {
        [resp.conversationsArray enumerateObjectsUsingBlock:^(Conversation * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            JConcreteConversationInfo *info = [self conversationWithPBConversation:obj];
            [arr addObject:info];
        }];
    }
    a.convs = arr;
    obj.syncConvsAck = a;
    return obj;
}

- (JPBRcvObj *)syncMsgsAckWithImWebsocketMsg:(ImWebsocketMsg *)msg {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    DownMsgSet *set = [[DownMsgSet alloc] initWithData:msg.qryAckMsgBody.data_p error:&e];
    if (e != nil) {
        NSLog(@"[Juggle]Websocket sync messages parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeSyncMsgsAck;
    //sync 和 query history 共用一个 ack
    JQryHisMsgsAck *a = [[JQryHisMsgsAck alloc] init];
    [a encodeWithQueryAckMsgBody:msg.qryAckMsgBody];
    a.isFinished = set.isFinished;
    NSMutableArray *arr = [[NSMutableArray alloc] init];
    if (set.msgsArray_Count > 0) {
        [set.msgsArray enumerateObjectsUsingBlock:^(DownMsg * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            JConcreteMessage *msg = [self messageWithDownMsg:obj];
            [arr addObject:msg];
        }];
    }
    a.msgs = arr;
    obj.qryHisMsgsAck = a;
    return obj;
}

#pragma mark - helper
- (int32_t)channelTypeFromConversationType:(JConversationType)type {
    int32_t result = ChannelType_Unknown;
    switch (type) {
        case JConversationTypePrivate:
            result = ChannelType_Private;
            break;
            
        case JConversationTypeGroup:
            result = ChannelType_Group;
            break;
            
        case JConversationTypeChatroom:
            result = ChannelType_Chatroom;
            break;
            
        case JConversationTypeSystem:
            result = ChannelType_System;
            break;
            
        default:
            break;
    }
    return result;
}

- (JConversationType)conversationTypeFromChannelType:(int32_t)channelType {
    JConversationType result = JConversationTypeUnknown;
    switch (channelType) {
        case ChannelType_Private:
            result = JConversationTypePrivate;
            break;
            
        case ChannelType_Group:
            result = JConversationTypeGroup;
            break;
            
        case ChannelType_Chatroom:
            result = JConversationTypeChatroom;
            break;
            
        case ChannelType_System:
            result = JConversationTypeSystem;
            break;
            
        default:
            break;
    }
    return result;
}

- (JPBRcvType)ackTypeWithCmd:(NSString *)cmd {
    NSNumber *n = self.cmdAckPair[cmd];
    return n.unsignedIntegerValue;
}

#pragma mark - getter&setter
- (NSDictionary *)cmdAckPair {
    return @{kQryHisMsgs:@(JPBRcvTypeQryHisMsgsAck),
             kSyncConvers:@(JPBRcvTypeSyncConvsAck),
             kSyncMsgs:@(JPBRcvTypeSyncMsgsAck)
    };
}
@end
