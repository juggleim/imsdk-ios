//
//  JPBData.m
// JuggleIM
//
//  Created by Nathan on 2023/12/6.
//

#import "JPBData.h"
#import "Connect.pbobjc.h"
#import "Appmessages.pbobjc.h"
#import "Pushtoken.pbobjc.h"
#import "JuggleIMPBConst.h"
#import "JConcreteMessage.h"
#import "JContentTypeCenter.h"
#import "JMessageMentionInfo.h"
#import "JConversationMentionInfo.h"
#import "JLogger.h"

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
#define kCMsg @"c_msg"
#define kRecallMsg @"recall_msg"
#define kQryHisMsgs @"qry_hismsgs"
#define jQryHisMsgsByIds @"qry_hismsg_by_ids"
#define kSyncConvers @"sync_convers"
#define kSyncMsgs @"sync_msgs"
#define jMarkRead @"mark_read"
#define jQryReadDetail @"qry_read_detail"
#define jDelConvers @"del_convers"
#define jClearUnread @"clear_unread"
#define jUndisturb @"undisturb_convers"
#define jTopConvers @"top_convers"
#define jQryMergedMsgs @"qry_merged_msgs"
#define jRegPushToken @"reg_push_token"
#define jQryMentionMsgs @"qry_mention_msgs"
#define jClearTotalUnread @"clear_total_unread"
#define jDelMsg @"del_msg"
#define jCleanHismsg @"clean_hismsg"
#define jAddConver @"add_conver"
#define jFileCred @"file_cred"
#define jSetUserUndisturb @"set_user_undisturb"
#define jGetUserUndisturb @"get_user_undisturb"
#define jJoinChatroom @"c_join"
#define jQuitChatroom @"c_quit"

#define jApns @"Apns"
#define jNtf @"ntf"
#define jMsg @"msg"

@implementation JConnectAck
@end

@implementation JPublishMsgAck
@end

@implementation JPublishMsgBody
@end

@implementation JPublishMsgNtf
@end

@implementation JDisconnectMsg
@end

@implementation JQryReadDetailAck
@end

@implementation JConversationInfoAck
@end

@implementation JSimpleQryAck
@end

@implementation JTimestampQryAck
@end

@implementation JQryFileCredAck
@end

@implementation JGlobalMuteAck
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
                      packageName:(NSString *)packageName
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
    connectMsg.packageName = packageName;
    connectMsg.pushChannel = jApns;
    
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
                          mergeInfo:(JMergeInfo *)mergeInfo
                        isBroadcast:(BOOL)isBroadcast
                             userId:(NSString *)userId
                              index:(int)index
                   conversationType:(JConversationType)conversationType
                     conversationId:(NSString *)conversationId
                        mentionInfo:(JMessageMentionInfo *)mentionInfo
                    referredMessage:(JConcreteMessage *)referredMessage{
    UpMsg *upMsg = [[UpMsg alloc] init];
    upMsg.msgType = contentType;
    upMsg.msgContent = msgData;
    upMsg.flags = flags;
    upMsg.clientUid = clientUid;
    if (mergeInfo != nil && mergeInfo.containerMsgId.length == 0 && mergeInfo.messages.count > 0) {
        upMsg.flags |= JMessageFlagIsMerged;
        MergedMsgs *pbMsgs = [[MergedMsgs alloc] init];
        pbMsgs.channelType = (int32_t)mergeInfo.conversation.conversationType;
        pbMsgs.userId = userId;
        pbMsgs.targetId = mergeInfo.conversation.conversationId;
        NSMutableArray <SimpleMsg *> *pbMsgArr = [NSMutableArray array];
        for (JConcreteMessage *msg in mergeInfo.messages) {
            SimpleMsg *simpleMsg = [[SimpleMsg alloc] init];
            simpleMsg.msgId = msg.messageId;
            simpleMsg.msgTime = msg.timestamp;
            simpleMsg.msgReadIndex = msg.seqNo;
            [pbMsgArr addObject:simpleMsg];
        }
        pbMsgs.msgsArray = pbMsgArr;
        upMsg.mergedMsgs = pbMsgs;
    }
    if (isBroadcast) {
        upMsg.flags |= JMessageFlagIsBroadcast;
    }
    if (mentionInfo != nil) {
        MentionInfo *pbMentionInfo = [[MentionInfo alloc] init];
        pbMentionInfo.mentionType = (int32_t)mentionInfo.type;
        NSMutableArray <UserInfo *> *pbUsers = [NSMutableArray array];
        for (JUserInfo *userInfo in mentionInfo.targetUsers) {
            UserInfo *pbUser = [[UserInfo alloc] init];
            pbUser.userId = userInfo.userId;
            [pbUsers addObject:pbUser];
        }
        pbMentionInfo.targetUsersArray = pbUsers;
        upMsg.mentionInfo = pbMentionInfo;
    }
    
    if(referredMessage != nil){
        DownMsg * referredDownMsg = [self downMsgWithMessage:referredMessage];
        upMsg.referMsg = referredDownMsg;
        
    }

    PublishMsgBody *publishMsg = [[PublishMsgBody alloc] init];
    publishMsg.index = index;
    switch (conversationType) {
        case JConversationTypePrivate:
            publishMsg.topic = kPMsg;
            break;
            
        case JConversationTypeGroup:
            publishMsg.topic = kGMsg;
            break;
            
        case JConversationTypeChatroom:
            publishMsg.topic = kCMsg;
            break;
            
        //TODO: 系统会话还没做
        case JConversationTypeSystem:
//
            break;
        default:
            break;
    }
    publishMsg.targetId = conversationId;
    publishMsg.data_p = [upMsg data];

    @synchronized (self) {
        [self.msgCmdDic setObject:publishMsg.topic forKey:@(publishMsg.index)];
    }
    ImWebsocketMsg *sm = [self createImWebSocketMsgWithPublishMsg:publishMsg];
    return sm.data;
}

-(DownMsg *)downMsgWithMessage:(JConcreteMessage *)message{
    if(message == nil){
        return nil;
    }
    if(message.conversation == nil){
        return nil;
    }
    DownMsg * downMsg = [[DownMsg alloc] init];
    downMsg.targetId = message.conversation.conversationId;
    downMsg.channelType = (int32_t)message.conversation.conversationType;
    downMsg.msgType = message.contentType;
    downMsg.senderId = message.senderUserId;
    downMsg.msgId = message.messageId;
    downMsg.msgSeqNo = message.seqNo;
    downMsg.msgContent = [message.content encode];
    downMsg.msgTime = message.timestamp;
    downMsg.flags = message.flags;
    downMsg.isSend = (message.direction == JMessageDirectionSend);
    downMsg.clientUid = message.clientUid;
    downMsg.isRead = message.hasRead;
    downMsg.unreadIndex = message.msgIndex;
    if(message.groupReadInfo){
        downMsg.readCount = message.groupReadInfo.readCount;
        downMsg.memberCount = message.groupReadInfo.memberCount;
    }
    if(message.groupInfo){
        downMsg.groupInfo = [self pbGroupInfoWithGroupInfo:message.groupInfo];
    }
    if(message.targetUserInfo){
        downMsg.targetUserInfo = [self pbUserInfoWithUserInfo:message.targetUserInfo];
    }
    if(message.mentionInfo){
        MentionInfo * mentionInfo = [[MentionInfo alloc] init];
        mentionInfo.mentionType = (int32_t)message.mentionInfo.type;
        NSMutableArray * targetUsersArray = [NSMutableArray array];
        for (JUserInfo * userInfo in message.mentionInfo.targetUsers) {
            [targetUsersArray addObject:[self pbUserInfoWithUserInfo:userInfo]];
        }
        mentionInfo.targetUsersArray = targetUsersArray;
        downMsg.mentionInfo = mentionInfo;
    }
    if(message.referredMsg){
        downMsg.referMsg = [self downMsgWithMessage:(JConcreteMessage *)message.referredMsg];
    }
    return downMsg;
}

- (NSData *)recallMessageData:(NSString *)messageId
                       extras:(NSDictionary *)extras
                 conversation:(JConversation *)conversation
                    timestamp:(long long)msgTime
                        index:(int)index {
    RecallMsgReq *req = [[RecallMsgReq alloc] init];
    req.msgId = messageId;
    req.targetId = conversation.conversationId;
    req.channelType = (int32_t)conversation.conversationType;
    req.msgTime = msgTime;
    NSMutableArray * extsArray = [NSMutableArray array];
    for (NSString * key in extras) {
        KvItem * item = [[KvItem alloc] init];
        item.key = key;
        item.value = extras[key];
        [extsArray addObject:item];
    }
    req.extsArray = extsArray;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = kRecallMsg;
    body.targetId = conversation.conversationId;
    body.data_p = [req data];
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *sm = [self createImWebSocketMsgWithQueryMsg:body];
    return sm.data;
}

- (NSData *)sendReadReceiptData:(NSArray<NSString *> *)messageIds
                 inConversation:(JConversation *)conversation
                          index:(int)index {
    NSMutableArray *arr = [[NSMutableArray alloc] init];
    [messageIds enumerateObjectsUsingBlock:^(NSString * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        SimpleMsg *simpleMsg = [[SimpleMsg alloc] init];
        simpleMsg.msgId = obj;
        [arr addObject:simpleMsg];
    }];
    
    MarkReadReq *req = [[MarkReadReq alloc] init];
    req.targetId = conversation.conversationId;
    req.channelType = (int32_t)conversation.conversationType;
    req.msgsArray = arr;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jMarkRead;
    body.targetId = conversation.conversationId;
    body.data_p = req.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)getGroupMessageReadDetail:(NSString *)messageId
                       inConversation:(JConversation *)conversation
                                index:(int)index {
    QryReadDetailReq *req = [[QryReadDetailReq alloc] init];
    req.targetId = conversation.conversationId;
    req.channelType = (int32_t)conversation.conversationType;
    req.msgId = messageId;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jQryReadDetail;
    body.targetId = conversation.conversationId;
    body.data_p = req.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
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
    if (direction == JPullDirectionOlder) {
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

- (NSData *)queryHisMsgsDataByIds:(NSArray<NSString *> *)messageIds
                   inConversation:(JConversation *)conversation
                            index:(int)index {
    QryHisMsgByIdsReq *r = [[QryHisMsgByIdsReq alloc] init];
    r.targetId = conversation.conversationId;
    r.channelType = [self channelTypeFromConversationType:conversation.conversationType];
    r.msgIdsArray = [messageIds mutableCopy];
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jQryHisMsgsByIds;
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

- (NSData *)deleteConversationData:(JConversation *)conversation
                            userId:(NSString *)userId
                             index:(int)index {
    ConversationsReq *req = [[ConversationsReq alloc] init];
    Conversation *c = [[Conversation alloc] init];
    c.targetId = conversation.conversationId;
    c.channelType = [self channelTypeFromConversationType:conversation.conversationType];
    NSMutableArray *arr = [NSMutableArray arrayWithObject:c];
    req.conversationsArray = arr;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jDelConvers;
    body.targetId = userId;
    body.data_p = req.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)clearUnreadCountData:(JConversation *)conversation
                          userId:(NSString *)userId
                        msgIndex:(long long)msgIndex
                           index:(int)index {
    ClearUnreadReq *req = [[ClearUnreadReq alloc] init];
    Conversation *c = [[Conversation alloc] init];
    c.targetId = conversation.conversationId;
    c.channelType = [self channelTypeFromConversationType:conversation.conversationType];
    c.latestReadIndex = msgIndex;
    NSMutableArray *arr = [NSMutableArray arrayWithObject:c];
    req.conversationsArray = arr;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jClearUnread;
    body.targetId = userId;
    body.data_p = req.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)undisturbData:(JConversation *)conversation
                   userId:(NSString *)userId
                   isMute:(BOOL)isMute
                    index:(int)index {
    UndisturbConverItem *item = [[UndisturbConverItem alloc] init];
    item.targetId = conversation.conversationId;
    item.channelType = [self channelTypeFromConversationType:conversation.conversationType];
    item.undisturbType = isMute?1:0;
    NSMutableArray *arr = [NSMutableArray arrayWithObject:item];
    
    UndisturbConversReq *req = [[UndisturbConversReq alloc] init];
    req.itemsArray = arr;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jUndisturb;
    body.targetId = userId;
    body.data_p = req.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)topConversationData:(JConversation *)conversation
                         userId:(NSString *)userId
                          isTop:(BOOL)isTop
                          index:(int)index {
    Conversation *pbConversation = [[Conversation alloc] init];
    pbConversation.channelType = [self channelTypeFromConversationType:conversation.conversationType];
    pbConversation.targetId = conversation.conversationId;
    pbConversation.isTop = isTop?1:0;
    NSMutableArray *arr = [NSMutableArray arrayWithObject:pbConversation];
    
    ConversationsReq *req = [[ConversationsReq alloc] init];
    req.conversationsArray = arr;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jTopConvers;
    body.targetId = userId;
    body.data_p = req.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)getMergedMessageList:(NSString *)messageId
                            time:(long long)timestamp
                           count:(int)count
                       direction:(JPullDirection)direction
                           index:(int)index {
    QryMergedMsgsReq *req = [[QryMergedMsgsReq alloc] init];
    req.startTime = timestamp;
    req.count = count;
    if (direction == JPullDirectionOlder) {
        req.order = 0;
    } else {
        req.order = 1;
    }
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jQryMergedMsgs;
    body.targetId = messageId;
    body.data_p = req.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)registerPushToken:(NSString *)token
                     deviceId:(NSString *)deviceId
                  packageName:(NSString *)packageName
                       userId:(NSString *)userId
                        index:(int)index {
    RegPushTokenReq *req = [[RegPushTokenReq alloc] init];
    req.deviceId = deviceId;
    req.platform = Platform_IOs;
    req.pushChannel = PushChannel_Apple;
    req.pushToken = token;
    req.packageName = packageName;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jRegPushToken;
    body.targetId = userId;
    body.data_p = req.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)getMentionMessages:(JConversation *)conversation
                          time:(long long)time
                         count:(int)count
                     direction:(JPullDirection)direction
                         index:(int)index {
    QryMentionMsgsReq *req = [[QryMentionMsgsReq alloc] init];
    req.targetId = conversation.conversationId;
    req.channelType = [self channelTypeFromConversationType:conversation.conversationType];
    req.startTime = time;
    req.count = count;
    if (direction == JPullDirectionOlder) {
        req.order = 0;
    } else {
        req.order = 1;
    }
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jQryMentionMsgs;
    body.targetId = conversation.conversationId;
    body.data_p = req.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)clearTotalUnreadCountMessages:(NSString *)userId
                                     time:(long long)time
                                    index:(int)index{
    
    QryTotalUnreadCountReq * req = [[QryTotalUnreadCountReq alloc] init];
    req.time = time;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jClearTotalUnread;
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

- (NSData *)publishAckData:(int)index {
    PublishAckMsgBody *body = [[PublishAckMsgBody alloc] init];
    body.index = index;
    
    ImWebsocketMsg *m = [self createImWebsocketMsg];
    m.cmd = JCmdTypePublishAck;
    m.qos = JQosNo;
    m.pubAckMsgBody = body;
    
    return m.data;
}

- (NSData *)deleteMessage:(JConversation *)conversation
                  msgList:(NSArray <JConcreteMessage *> *)msgList
                    index:(int)index{
    DelHisMsgsReq * req = [[DelHisMsgsReq alloc] init];
    req.targetId = conversation.conversationId;
    req.channelType = [self channelTypeFromConversationType:conversation.conversationType];
    NSMutableArray <SimpleMsg *> *pbMsgArr = [NSMutableArray array];
    for (JConcreteMessage *msg in msgList) {
        SimpleMsg *simpleMsg = [[SimpleMsg alloc] init];
        simpleMsg.msgId = msg.messageId;
        simpleMsg.msgTime = msg.timestamp;
        [pbMsgArr addObject:simpleMsg];
    }
    req.msgsArray = pbMsgArr;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jDelMsg;
    body.targetId = conversation.conversationId;
    body.data_p = [req data];
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    
    return m.data;
}

- (NSData *)clearHistoryMessage:(JConversation *)conversation
                           time:(long long)time
                          scope:(int)scope
                          index:(int)index{
    CleanHisMsgReq * req = [[CleanHisMsgReq alloc] init];
    req.targetId = conversation.conversationId;
    req.channelType = [self channelTypeFromConversationType:conversation.conversationType];
    req.cleanMsgTime = time;
    req.cleanScope = scope;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jCleanHismsg;
    body.targetId = conversation.conversationId;
    body.data_p = [req data];
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    
    return m.data;
}

- (NSData *)createConversationInfo:(JConversation *)conversation
                            userId:(NSString *)userId
                             index:(int)index {
    Conversation *c = [[Conversation alloc] init];
    c.channelType = [self channelTypeFromConversationType:conversation.conversationType];
    c.targetId = conversation.conversationId;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jAddConver;
    body.targetId = userId;
    body.data_p = [c data];
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)getUploadFileCred:(NSString *)userId
                     fileType:(JUploadFileType)fileType
                          ext:(NSString *)ext
                        index:(int)index{
    QryFileCredReq * req = [[QryFileCredReq alloc] init];
    req.fileType = (FileType)fileType;
    if(ext == nil || ext.length == 0){
        req.ext = @"";
    }else{
        req.ext = ext;
    }
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jFileCred;
    body.targetId = userId;
    body.data_p = [req data];
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)setGlobalMute:(BOOL)isMute
                   userId:(NSString *)userId
                 timezone:(NSString *)timezone
                  periods:(NSArray<JTimePeriod *> *)periods
                    index:(int)index {
    UserUndisturb *req = [[UserUndisturb alloc] init];
    req.switch_p = isMute;
    req.timezone = timezone;
    
    NSMutableArray <UserUndisturbItem *> *arr = [NSMutableArray array];
    for (JTimePeriod *period in periods) {
        UserUndisturbItem *item = [[UserUndisturbItem alloc] init];
        item.start = period.startTime;
        item.end = period.endTime;
        [arr addObject:item];
    }
    req.rulesArray = arr;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jSetUserUndisturb;
    body.targetId = userId;
    body.data_p = [req data];
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)getGlobalMute:(NSString *)userId index:(int)index {
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jGetUserUndisturb;
    body.targetId = userId;
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)joinChatroom:(NSString *)chatroomId
                   index:(int)index {
    ChatroomInfo *info = [[ChatroomInfo alloc] init];
    info.chatId = chatroomId;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jJoinChatroom;
    body.targetId = chatroomId;
    body.data_p = [info data];
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)quitChatroom:(NSString *)chatroomId index:(int)index {
    ChatroomInfo *info = [[ChatroomInfo alloc] init];
    info.chatId = chatroomId;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jQuitChatroom;
    body.targetId = chatroomId;
    body.data_p = [info data];
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}


- (JPBRcvObj *)rcvObjWithData:(NSData *)data {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    
    NSError *err = nil;
    ImWebsocketMsg *msg = [[ImWebsocketMsg alloc] initWithData:data error:&err];
    if (err != nil) {
        JLogE(@"PB-Parse", @"receive message parse error, msg is %@", err.description);
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
            a.session = msg.connectAckMsgBody.session;
            a.extra = msg.connectAckMsgBody.ext;
            obj.connectAck = a;
        }
            break;
            
        case ImWebsocketMsg_Testof_OneOfCase_PubAckMsgBody:
        {
            NSString *cachedCmd;
            @synchronized (self) {
                cachedCmd = self.msgCmdDic[@(msg.pubAckMsgBody.index)];
                [self.msgCmdDic removeObjectForKey:@(msg.pubAckMsgBody.index)];
            }
            if (cachedCmd.length == 0) {
                JLogW(@"PB-Match", @"pub ack can't match a cached command");
                obj.rcvType = JPBRcvTypeCmdMatchError;
                return obj;
            }
            obj.rcvType = [self ackTypeWithCmd:cachedCmd];
            JPublishMsgAck *a = [[JPublishMsgAck alloc] init];
            a.index = msg.pubAckMsgBody.index;
            a.code = msg.pubAckMsgBody.code;
            a.msgId = msg.pubAckMsgBody.msgId;
            a.timestamp = msg.pubAckMsgBody.timestamp;
            a.seqNo = msg.pubAckMsgBody.msgSeqNo;
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
                JLogW(@"PB-Match", @"qry ack can't match a cached command");
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
                    
                case JPBRcvTypeQryReadDetailAck:
                    obj = [self qryReadDetailAckWithImWebsocketMsg:msg];
                    break;
                    
                case JPBRcvTypeSimpleQryAck:
                    obj = [self simpleQryAckWithImWebsocketMsg:msg];
                    break;
                    
                case JPBRcvTypeSimpleQryAckCallbackTimestamp:
                    obj = [self simpleQryAckCallbackTimestampWithImWebsocketMsg:msg];
                    break;
                    
                case JPBRcvTypeConversationSetTopAck:
                    obj = [self conversationSetTopAckWithImWebsocketMsg:msg];
                    break;
                    
                case JPBRcvTypeAddConversation:
                    obj = [self addConversationWithImWebsocketMsg:msg];
                    break;
                case JPBRcvTypeFileCredMsgAck:
                    obj = [self qryFileCredAckWithImWebsocketMsg:msg];
                    break;
                case JPBRcvTypeGlobalMuteAck:
                    obj = [self globalMuteAckWithImWebsocketMsg:msg];
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
                Notify *ntf = [[Notify alloc] initWithData:msg.publishMsgBody.data_p error:&err];
                if (err != nil) {
                    JLogE(@"PB-Parse", @"publish msg notify parse error, msg is %@", err.description);
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
                DownMsg *downMsg = [[DownMsg alloc] initWithData:msg.publishMsgBody.data_p error:&err];
                if (err != nil) {
                    JLogE(@"PB-Parse", @"publish msg parse error, msg is %@", err.description);
                    obj.rcvType = JPBRcvTypeParseError;
                    return obj;
                }
                obj.rcvType = JPBRcvTypePublishMsg;
                JPublishMsgBody *publishMsgBody = [[JPublishMsgBody alloc]init];
                publishMsgBody.rcvMessage = [self messageWithDownMsg:downMsg];
                publishMsgBody.index = msg.publishMsgBody.index;
                publishMsgBody.qos = msg.qos;
                obj.publishMsgBody = publishMsgBody;
            }
        }
            break;
            
        case ImWebsocketMsg_Testof_OneOfCase_DisconnectMsgBody:
        {
            obj.rcvType = JPBRcvTypeDisconnectMsg;
            JDisconnectMsg *m = [[JDisconnectMsg alloc] init];
            m.code = msg.disconnectMsgBody.code;
            m.timestamp = msg.disconnectMsgBody.timestamp;
            m.extra = msg.disconnectMsgBody.ext;
            obj.disconnectMsg = m;
        }
            break;
            
        default:
            break;
    }
    return obj;
}

#pragma mark - internal
- (ImWebsocketMsg *)createImWebSocketMsgWithPublishMsg:(PublishMsgBody *)body {
    ImWebsocketMsg *sm = [self createImWebsocketMsg];
    sm.cmd = JCmdTypePublish;
    sm.qos = JQosYes;
    sm.publishMsgBody = body;
    return sm;
}

- (ImWebsocketMsg *)createImWebSocketMsgWithQueryMsg:(QueryMsgBody *)body {
    ImWebsocketMsg *m = [self createImWebsocketMsg];
    m.cmd = JCmdTypeQuery;
    m.qos = JQosYes;
    m.qryMsgBody = body;
    return m;
}

- (ImWebsocketMsg *)createImWebsocketMsg {
    ImWebsocketMsg *m = [[ImWebsocketMsg alloc] init];
    m.version = JuggleIMProtocolVersion;
    return m;
}

- (JConcreteMessage *)messageWithDownMsg:(DownMsg *)downMsg {
    JConcreteMessage *msg = [[JConcreteMessage alloc] init];
    JConversation *conversation = [[JConversation alloc] initWithConversationType:[self conversationTypeFromChannelType:downMsg.channelType]
                                                                   conversationId:downMsg.targetId];
    msg.conversation = conversation;
    msg.contentType = downMsg.msgType;
    msg.messageId = downMsg.msgId;
    msg.clientUid = downMsg.clientUid;
    msg.direction = downMsg.isSend ? JMessageDirectionSend : JMessageDirectionReceive;
    msg.hasRead = downMsg.isRead;
    msg.timestamp = downMsg.msgTime;
    msg.messageState = JMessageStateSent;
    msg.senderUserId = downMsg.senderId;
    msg.seqNo = downMsg.msgSeqNo;
    msg.msgIndex = downMsg.unreadIndex;
    msg.content = [[JContentTypeCenter shared] contentWithData:downMsg.msgContent
                                                   contentType:downMsg.msgType];
    if([msg.content isKindOfClass:[JMergeMessage class]]){
        JMergeMessage * mergeMessage = (JMergeMessage *)msg.content;
        if(mergeMessage.containerMsgId == nil || mergeMessage.containerMsgId.length == 0){
            mergeMessage.containerMsgId = msg.messageId;
        }
    }
    msg.flags = downMsg.flags;
    JGroupMessageReadInfo *info = [[JGroupMessageReadInfo alloc] init];
    info.readCount = downMsg.readCount;
    info.memberCount = downMsg.memberCount;
    msg.groupReadInfo = info;
    msg.groupInfo = [self groupInfoWithPBGroupInfo:downMsg.groupInfo];
    msg.targetUserInfo = [self userInfoWithPBUserInfo:downMsg.targetUserInfo];
    if (downMsg.hasMentionInfo && downMsg.mentionInfo.mentionType != MentionType_MentionDefault) {
        JMessageMentionInfo *mentionInfo = [[JMessageMentionInfo alloc] init];
        mentionInfo.type = (JMentionType)downMsg.mentionInfo.mentionType;
        NSMutableArray <JUserInfo *> *mentionUserList = [NSMutableArray array];
        for (UserInfo *u in downMsg.mentionInfo.targetUsersArray) {
            JUserInfo *userInfo = [self userInfoWithPBUserInfo:u];
            if (userInfo) {
                [mentionUserList addObject:userInfo];
            }
        }
        mentionInfo.targetUsers = mentionUserList;
        msg.mentionInfo = mentionInfo;
    }
    if(downMsg.hasReferMsg && downMsg.referMsg != nil){
        JConcreteMessage * referMsg = [self messageWithDownMsg:downMsg.referMsg];
        msg.referredMsg = referMsg;
    }
    
    return msg;
}

- (GroupInfo *)pbGroupInfoWithGroupInfo:(JGroupInfo *)groupInfo{
    if(groupInfo == nil){
        return nil;
    }
    GroupInfo * pbGroupInfo = [[GroupInfo alloc] init];
    pbGroupInfo.groupId = groupInfo.groupId;
    pbGroupInfo.groupName = groupInfo.groupName;
    pbGroupInfo.groupPortrait = groupInfo.portrait;
    NSMutableArray * extFieldsArray = [NSMutableArray array];
    for (NSString * key in groupInfo.extraDic.allKeys) {
        KvItem * item = [[KvItem alloc] init];
        item.key = key;
        item.value = [groupInfo.extraDic objectForKey:key];
        [extFieldsArray addObject:item];
    }
    pbGroupInfo.extFieldsArray = extFieldsArray;
    return pbGroupInfo;
}

- (JGroupInfo *)groupInfoWithPBGroupInfo:(GroupInfo *)pbGroupInfo {
    if (pbGroupInfo == nil) {
        return nil;
    }
    JGroupInfo *result = [[JGroupInfo alloc] init];
    result.groupId = pbGroupInfo.groupId;
    result.groupName = pbGroupInfo.groupName;
    result.portrait = pbGroupInfo.groupPortrait;
    if (pbGroupInfo.extFieldsArray_Count > 0) {
        NSMutableDictionary *dic = [NSMutableDictionary dictionary];
        for (KvItem *item in pbGroupInfo.extFieldsArray) {
            [dic setObject:item.value forKey:item.key];
        }
        result.extraDic = [dic copy];
    }
    return result;
}

-(UserInfo *)pbUserInfoWithUserInfo:(JUserInfo *)userInfo{
    if(userInfo == nil){
        return nil;
    }
    UserInfo * pbUserInfo = [[UserInfo alloc] init];
    pbUserInfo.userId = userInfo.userId;
    pbUserInfo.nickname = userInfo.userName;
    pbUserInfo.userPortrait = userInfo.portrait;
    NSMutableArray * extFieldsArray = [NSMutableArray array];
    for (NSString * key in userInfo.extraDic.allKeys) {
        KvItem * item = [[KvItem alloc] init];
        item.key = key;
        item.value = [userInfo.extraDic objectForKey:key];
        [extFieldsArray addObject:item];
    }
    pbUserInfo.extFieldsArray = extFieldsArray;
    return pbUserInfo;
}

- (JUserInfo *)userInfoWithPBUserInfo:(UserInfo *)pbUserInfo {
    if (pbUserInfo == nil) {
        return nil;
    }
    JUserInfo *result = [[JUserInfo alloc] init];
    result.userId = pbUserInfo.userId;
    result.userName = pbUserInfo.nickname;
    result.portrait = pbUserInfo.userPortrait;
    if (pbUserInfo.extFieldsArray_Count > 0) {
        NSMutableDictionary *dic = [NSMutableDictionary dictionary];
        for (KvItem *item in pbUserInfo.extFieldsArray) {
            [dic setObject:item.value forKey:item.key];
        }
        result.extraDic = [dic copy];
    }
    return result;
}

- (JUserInfo *)userInfoWithMemberReadDetailItem:(MemberReadDetailItem *)item {
    JUserInfo *userInfo = [[JUserInfo alloc] init];
    userInfo.userId = item.member.userId;
    userInfo.userName = item.member.nickname;
    userInfo.portrait = item.member.userPortrait;
    if (item.member.extFieldsArray_Count > 0) {
        NSMutableDictionary *dic = [NSMutableDictionary dictionary];
        for (KvItem *it in item.member.extFieldsArray) {
            [dic setObject:it.value forKey:it.key];
        }
        userInfo.extraDic = [dic copy];
    }
    return userInfo;
}

- (JConcreteConversationInfo *)conversationWithPBConversation:(Conversation *)conversation {
    JConcreteConversationInfo *info = [[JConcreteConversationInfo alloc] init];
    JConversation *c = [[JConversation alloc] initWithConversationType:[self conversationTypeFromChannelType:conversation.channelType]
                                                                   conversationId:conversation.targetId];
    info.conversation = c;
    info.sortTime = conversation.sortTime;
    JConcreteMessage *lastMessage = [self messageWithDownMsg:conversation.msg];
    info.lastMessage = lastMessage;
    info.lastReadMessageIndex = conversation.latestReadIndex;
    info.lastMessageIndex = conversation.latestUnreadIndex;
    info.unreadCount = (int)conversation.unreadCount;
    info.syncTime = conversation.syncTime;
    info.mute = (conversation.undisturbType==1)?YES:NO;
    info.isTop = (conversation.isTop==1)?YES:NO;
    info.topTime = conversation.topUpdatedTime;
    info.groupInfo = [self groupInfoWithPBGroupInfo:conversation.groupInfo];
    info.targetUserInfo = [self userInfoWithPBUserInfo:conversation.targetUserInfo];
    if(conversation.mentions != nil && conversation.mentions.isMentioned){
        JConversationMentionInfo * mentionInfo = [[JConversationMentionInfo alloc] init];
        if(conversation.mentions.mentionMsgsArray != nil){
            NSMutableArray<JConversationMentionMessage *> * array = [NSMutableArray array];
            for (MentionMsg * mentionMsg in conversation.mentions.mentionMsgsArray) {
                JConversationMentionMessage * mentionMessage = [self mentionMsgWithPBMentionMsg:mentionMsg];
                if(mentionMessage){
                    [array addObject:mentionMessage];
                }
            }
            mentionInfo.mentionMsgList = array;
        }
        info.mentionInfo = mentionInfo;

        if(conversation.mentions.sendersArray != nil){
            NSMutableArray<JUserInfo *> * array = [NSMutableArray array];
            for (UserInfo * userInfo in conversation.mentions.sendersArray) {
                JUserInfo * jUserInfo = [self userInfoWithPBUserInfo:userInfo];
                if(jUserInfo){
                    [array addObject:jUserInfo];
                }
            }
            info.mentionUserList = array;
        }
    }
    return info;
}

-(JConversationMentionMessage *)mentionMsgWithPBMentionMsg:(MentionMsg *)mentionMsg{
    if(mentionMsg == nil){
        return nil;
    }
    JConversationMentionMessage * mentionMessage = [[JConversationMentionMessage alloc] init];
    mentionMessage.senderId = mentionMsg.senderId;
    mentionMessage.msgId = mentionMsg.msgId;
    mentionMessage.msgTime = mentionMsg.msgTime;
    return mentionMessage;
}

- (JPBRcvObj *)queryHistoryMessagesAckWithImWebsocketMsg:(ImWebsocketMsg *)msg {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    DownMsgSet *set = [[DownMsgSet alloc] initWithData:msg.qryAckMsgBody.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"query history messages parse error, msg is %@", e.description);
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
        JLogE(@"PB-Parse", @"sync conversations parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeSyncConvsAck;
    JSyncConvsAck *a = [[JSyncConvsAck alloc] init];
    [a encodeWithQueryAckMsgBody:msg.qryAckMsgBody];
    a.isFinished = resp.isFinished;
    NSMutableArray *arr = [[NSMutableArray alloc] init];
    NSMutableArray *deletedArr = [[NSMutableArray alloc] init];
    if (resp.conversationsArray_Count > 0) {
        [resp.conversationsArray enumerateObjectsUsingBlock:^(Conversation * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            JConcreteConversationInfo *info = [self conversationWithPBConversation:obj];
            if (obj.isDelete) {
                [deletedArr addObject:info];
            } else {
                [arr addObject:info];
            }
        }];
    }
    a.convs = arr;
    a.deletedConvs = deletedArr;
    obj.syncConvsAck = a;
    return obj;
}

- (JPBRcvObj *)syncMsgsAckWithImWebsocketMsg:(ImWebsocketMsg *)msg {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    DownMsgSet *set = [[DownMsgSet alloc] initWithData:msg.qryAckMsgBody.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"sync messages parse error, msg is %@", e.description);
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

- (JPBRcvObj *)simpleQryAckWithImWebsocketMsg:(ImWebsocketMsg *)msg {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    obj.rcvType = JPBRcvTypeSimpleQryAck;
    JSimpleQryAck *a = [[JSimpleQryAck alloc] init];
    [a encodeWithQueryAckMsgBody:msg.qryAckMsgBody];
    obj.simpleQryAck = a;
    return obj;
}

- (JPBRcvObj *)simpleQryAckCallbackTimestampWithImWebsocketMsg:(ImWebsocketMsg *)msg {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    obj.rcvType = JPBRcvTypeSimpleQryAckCallbackTimestamp;
    JSimpleQryAck *a = [[JSimpleQryAck alloc] init];
    [a encodeWithQueryAckMsgBody:msg.qryAckMsgBody];
    obj.simpleQryAck = a;
    return obj;
}

- (JPBRcvObj *)qryFileCredAckWithImWebsocketMsg:(ImWebsocketMsg *)msg{
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    QryFileCredResp * resp = [[QryFileCredResp alloc] initWithData:msg.qryAckMsgBody.data_p error:&e];
    if(e != nil){
        JLogE(@"PB-Parse", @"file cred ack parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeFileCredMsgAck;
    JQryFileCredAck * a = [[JQryFileCredAck alloc] init];
    [a encodeWithQueryAckMsgBody:msg.qryAckMsgBody];
    a.ossType = (JUploadOssType)resp.ossType;
    if(resp.qiNiuCred != nil){
        JUploadQiNiuCred * qiNiuCred = [[JUploadQiNiuCred alloc] init];
        qiNiuCred.domain = resp.qiNiuCred.domain;
        qiNiuCred.token = resp.qiNiuCred.token;
        a.qiNiuCred = qiNiuCred;
    }
    if(resp.preSignResp != nil){
        JUploadPreSignCred * preSignCred = [[JUploadPreSignCred alloc] init];
        preSignCred.url = resp.preSignResp.URL;
        a.preSignCred = preSignCred;
    }
    obj.qryFileCredAck = a;
    return obj;
    
}

- (JPBRcvObj *)globalMuteAckWithImWebsocketMsg:(ImWebsocketMsg *)msg{
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    UserUndisturb *resp = [[UserUndisturb alloc] initWithData:msg.qryAckMsgBody.data_p error:&e];
    if(e != nil){
        JLogE(@"PB-Parse", @"global mute ack parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeGlobalMuteAck;
    JGlobalMuteAck *a = [[JGlobalMuteAck alloc] init];
    [a encodeWithQueryAckMsgBody:msg.qryAckMsgBody];
    a.isMute = resp.switch_p;
    a.timezone = resp.timezone;
    NSMutableArray <JTimePeriod *> *periods = [NSMutableArray array];
    for (UserUndisturbItem *item in resp.rulesArray) {
        JTimePeriod *p = [[JTimePeriod alloc] init];
        p.startTime = item.start;
        p.endTime = item.end;
        [periods addObject:p];
    }
    a.periods = periods;
    obj.globalMuteAck = a;
    return obj;
}

- (JPBRcvObj *)conversationSetTopAckWithImWebsocketMsg:(ImWebsocketMsg *)msg {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    TopConversResp *resp = [[TopConversResp alloc] initWithData:msg.qryAckMsgBody.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"set top ack parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeConversationSetTopAck;
    JTimestampQryAck *a = [[JTimestampQryAck alloc] init];
    [a encodeWithQueryAckMsgBody:msg.qryAckMsgBody];
    a.operationTime = resp.optTime;
    obj.timestampQryAck = a;
    return obj;
}

- (JPBRcvObj *)addConversationWithImWebsocketMsg:(ImWebsocketMsg *)msg {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    Conversation *pbConv = [[Conversation alloc] initWithData:msg.qryAckMsgBody.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"add conversation error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeAddConversation;
    JConversationInfoAck *a = [[JConversationInfoAck alloc] init];
    [a encodeWithQueryAckMsgBody:msg.qryAckMsgBody];
    a.conversationInfo = [self conversationWithPBConversation:pbConv];
    obj.conversationInfoAck = a;
    return obj;
}

- (JPBRcvObj *)qryReadDetailAckWithImWebsocketMsg:(ImWebsocketMsg *)msg {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    QryReadDetailResp *resp = [[QryReadDetailResp alloc] initWithData:msg.qryAckMsgBody.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"qry read detail ack parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeQryReadDetailAck;
    JQryReadDetailAck *a = [[JQryReadDetailAck alloc] init];
    [a encodeWithQueryAckMsgBody:msg.qryAckMsgBody];
    NSMutableArray *readMembers = [[NSMutableArray alloc] init];
    NSMutableArray *unreadMembers = [[NSMutableArray alloc] init];
    [resp.readMembersArray enumerateObjectsUsingBlock:^(MemberReadDetailItem * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        JUserInfo *userInfo = [self userInfoWithMemberReadDetailItem:obj];
        [readMembers addObject:userInfo];
    }];
    [resp.unreadMembersArray enumerateObjectsUsingBlock:^(MemberReadDetailItem * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        JUserInfo *userInfo = [self userInfoWithMemberReadDetailItem:obj];
        [unreadMembers addObject:userInfo];
    }];
    a.readMembers = readMembers;
    a.unreadMembers = unreadMembers;
    obj.qryReadDetailAck = a;
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
             kSyncMsgs:@(JPBRcvTypeSyncMsgsAck),
             kPMsg:@(JPBRcvTypePublishMsgAck),
             kGMsg:@(JPBRcvTypePublishMsgAck),
             kCMsg:@(JPBRcvTypePublishMsgAck),
             kRecallMsg:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jDelConvers:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jClearUnread:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jMarkRead:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jQryReadDetail:@(JPBRcvTypeQryReadDetailAck),
             jQryHisMsgsByIds:@(JPBRcvTypeQryHisMsgsAck),
             jUndisturb:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jTopConvers:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jQryMergedMsgs:@(JPBRcvTypeQryHisMsgsAck),
             jRegPushToken:@(JPBRcvTypeSimpleQryAck),
             jQryMentionMsgs:@(JPBRcvTypeQryHisMsgsAck),
             jClearTotalUnread:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jDelMsg:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jCleanHismsg:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jAddConver:@(JPBRcvTypeAddConversation),
             jFileCred:@(JPBRcvTypeFileCredMsgAck),
             jSetUserUndisturb:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jGetUserUndisturb:@(JPBRcvTypeGlobalMuteAck),
             jJoinChatroom:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jQuitChatroom:@(JPBRcvTypeSimpleQryAckCallbackTimestamp)
    };
}
@end
