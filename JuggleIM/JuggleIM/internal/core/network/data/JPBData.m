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
#import "Chatroom.pbobjc.h"
#import "Rtcroom.pbobjc.h"
#import "JuggleIMPBConst.h"
#import "JConcreteMessage.h"
#import "JContentTypeCenter.h"
#import "JMessageMentionInfo.h"
#import "JConversationMentionInfo.h"
#import "JLogger.h"
#import "JDataConverterProtocol.h"
#import "JSimpleDataConverter.h"
#import "JuggleIMConstInternal.h"

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
#define kModifyMsg @"modify_msg"
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
#define jSetTopMsg @"set_top_msg"
#define jDelTopMsg @"del_top_msg"
#define jGetTopMsg @"get_top_msg"
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
#define jMarkUnread @"mark_unread"
#define jPushSwitch @"push_switch"
#define jUploadLogStatus @"upd_log_state"
#define jSyncChatroomMessage @"c_sync_msgs"
#define jSyncChatroomAtts @"c_sync_atts"
#define jQryFirstUnreadMsg @"qry_first_unread_msg"
#define jBatchAddAtt @"c_batch_add_att"
#define jBatchDelAtt @"c_batch_del_att"
#define jMsgExSet @"msg_exset"
#define jDelMsgExSet @"del_msg_exset"
#define jQryMsgExSet @"qry_msg_exset"
#define jTagAddConvers @"tag_add_convers"
#define jTagDelConvers @"tag_del_convers"

#define jRtcInvite @"rtc_invite"
#define jRtcHangUp @"rtc_hangup"
#define jRtcAccept @"rtc_accept"
#define jRtcQuit @"rtc_quit"
#define jRtcUpdState @"rtc_upd_state"
#define jRtcMemberRooms @"rtc_member_rooms"
#define jRtcQry @"rtc_qry"
#define jSetUserSettings @"set_user_settings"
#define jGetUserSettings @"get_user_settings"
#define jLanguage @"language"
#define jRtcPing @"rtc_ping"

#define jApple @"Apple"
#define jNtf @"ntf"
#define jMsg @"msg"
#define jCUserNtf @"c_user_ntf"
#define jRtcRoomEvent @"rtc_room_event"
#define jRtcInviteEvent @"rtc_invite_event"

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

@implementation JChatroomAttrsAck
@end

@implementation JRtcRoomEventNtf
@end

@implementation JRtcInviteEventNtf
@end

@implementation JStringAck
@end

@implementation JRtcQryCallRoomsAck
@end

@implementation JQryMsgExtAck
@end

@implementation JRtcAuthAck
@end

@implementation JGetTopMsgAck
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
@property (nonatomic, strong) id<JDataConverterProtocol> converter;
@end

@implementation JPBData
- (instancetype)init {
    self = [super init];
    if (self) {
        self.msgCmdDic = [[NSMutableDictionary alloc] init];
        self.converter = [JSimpleDataConverter converter];
    }
    return self;
}

- (void)resetDataConverter {
    self.converter = [JSimpleDataConverter converter];
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
                        voipToken:(NSString *)voipToken
                        networkId:(NSString *)networkId
                           ispNum:(NSString *)ispNum
                         clientIp:(NSString *)clientIp
                         language:(nonnull NSString *)language {
    ConnectMsgBody *connectMsg = [[ConnectMsgBody alloc] init];
    connectMsg.protoId = JProtoId;
    connectMsg.sdkVersion = JIMVersion;
    connectMsg.appkey = appKey;
    connectMsg.token = token;
    connectMsg.deviceId = deviceId;
    connectMsg.platform = platform;
    connectMsg.deviceCompany = deviceCompany;
    connectMsg.deviceModel = deviceModel;
    connectMsg.deviceOsVersion = osVersion;
    connectMsg.pushToken = pushToken;
    connectMsg.voipToken = voipToken;
    connectMsg.networkId = networkId;
    connectMsg.ispNum = ispNum;
    connectMsg.clientIp = clientIp;
    connectMsg.packageName = packageName;
    connectMsg.pushChannel = jApple;
//    connectMsg.language = language;
    
    NSData *data = [self.converter encode:connectMsg.data];
    ImWebsocketMsg *sm = [self createImWebsocketMsg];
    sm.cmd = JCmdTypeConnect;
    sm.qos = JQosYes;
    sm.payload = data;
    return sm.data;
}

- (NSData *)disconnectData:(BOOL)needPush {
    DisconnectMsgBody *body = [[DisconnectMsgBody alloc] init];
    if (needPush) {
        body.code = 0;
    } else {
        body.code = 11012;
    }
    body.timestamp = [[NSDate date] timeIntervalSince1970]*1000;
    
    NSData *data = [self.converter encode:body.data];
    ImWebsocketMsg *sm = [self createImWebsocketMsg];
    sm.cmd = JCmdTypeDisconnect;
    sm.qos = JQosNo;
    sm.payload = data;
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
                    referredMessage:(JConcreteMessage *)referredMessage
                           pushData:(nonnull JPushData *)pushData {
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
    if (mentionInfo) {
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
    
    if (referredMessage) {
        DownMsg * referredDownMsg = [self downMsgWithMessage:referredMessage];
        upMsg.referMsg = referredDownMsg;
    }
    if (pushData) {
        PushData *pbPushData = [[PushData alloc] init];
        pbPushData.pushText = pushData.content;
        pbPushData.pushExtraData = pushData.extra;
        upMsg.pushData = pbPushData;
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
            
        case JConversationTypeSystem:
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

- (NSData *)updateMessageData:(NSString *)messageId
                      msgType:(nonnull NSString *)contentType
                      msgData:(nonnull NSData *)msgData
                 conversation:(nonnull JConversation *)conversation
                    timestamp:(long long)timestamp
                     msgSeqNo:(long long)msgSeqNo
                        index:(int)index {
    ModifyMsgReq *req = [[ModifyMsgReq alloc] init];
    req.msgId = messageId;
    req.targetId = conversation.conversationId;
    req.channelType = (int32_t)conversation.conversationType;
    req.msgTime = timestamp;
    req.msgSeqNo = msgSeqNo;
    req.msgContent = msgData;
    req.msgType = contentType;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = kModifyMsg;
    body.targetId = conversation.conversationId;
    body.data_p = [req data];
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *sm = [self createImWebSocketMsgWithQueryMsg:body];
    return sm.data;
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
                    contentTypes:(NSArray <NSString *> *)contentTypes
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
    if (contentTypes.count > 0) {
        r.msgTypesArray = [contentTypes mutableCopy];
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
                           msgId:(NSString *)msgId
                       timestamp:(long long)timestamp
                           index:(int)index {
    ClearUnreadReq *req = [[ClearUnreadReq alloc] init];
    Conversation *c = [[Conversation alloc] init];
    c.targetId = conversation.conversationId;
    c.channelType = [self channelTypeFromConversationType:conversation.conversationType];
    c.latestReadIndex = msgIndex;
    if (msgId.length > 0) {
        c.latestReadMsgId = msgId;
    }
    c.latestReadMsgTime = timestamp;
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

- (NSData *)topMessageData:(NSString *)messageId
              conversation:(JConversation *)conversation
                     isTop:(BOOL)isTop
                     index:(int)index {
    TopMsgReq *req = [[TopMsgReq alloc] init];
    req.channelType = [self channelTypeFromConversationType:conversation.conversationType];
    req.targetId = conversation.conversationId;
    req.msgId = messageId;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    if (isTop) {
        body.topic = jSetTopMsg;
    } else {
        body.topic = jDelTopMsg;
    }
    body.targetId = conversation.conversationId;
    body.data_p = req.data;
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)getTopMessageData:(JConversation *)conversation index:(int)index {
    GetTopMsgReq *req = [[GetTopMsgReq alloc] init];
    req.channelType = [self channelTypeFromConversationType:conversation.conversationType];
    req.targetId = conversation.conversationId;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jGetTopMsg;
    body.targetId = conversation.conversationId;
    body.data_p = req.data;
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)markUnread:(JConversation *)conversation userId:(NSString *)userId index:(int)index {
    Conversation *pbConversation = [[Conversation alloc] init];
    pbConversation.channelType = [self channelTypeFromConversationType:conversation.conversationType];
    pbConversation.targetId = conversation.conversationId;
    pbConversation.unreadTag = 1;
    NSMutableArray *arr = [NSMutableArray arrayWithObject:pbConversation];
    
    ConversationsReq *req = [[ConversationsReq alloc] init];
    req.conversationsArray = arr;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jMarkUnread;
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

- (NSData *)registerVoIPToken:(NSString *)token
                     deviceId:(NSString *)deviceId
                  packageName:(NSString *)packageName
                       userId:(NSString *)userId
                        index:(int)index {
    RegPushTokenReq *req = [[RegPushTokenReq alloc] init];
    req.deviceId = deviceId;
    req.platform = Platform_IOs;
    req.pushChannel = PushChannel_Apple;
    req.voipToken = token;
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
                 lastReadIndex:(long long)lastReadIndex
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
    req.latestReadIndex = lastReadIndex;
    
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
                                    index:(int)index {
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

- (NSData *)pushSwitch:(BOOL)enablePush
                userId:(NSString *)userId
                 index:(int)index {
    PushSwitch *ps = [[PushSwitch alloc] init];
    ps.switch_p = enablePush;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jPushSwitch;
    body.targetId = userId;
    body.data_p = ps.data;
    
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)uploadLogStatus:(int)result
                     userId:(NSString *)userId
                  messageId:(NSString *)messageId
                        url:(NSString *)url
                      index:(int)index {
    UploadLogStatusReq *req = [[UploadLogStatusReq alloc] init];
    req.msgId = messageId;
    if (url.length > 0) {
        req.logURL = url;
    }
    req.state = result;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jUploadLogStatus;
    body.targetId = userId;
    body.data_p = req.data;
    
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)syncChatroomMessages:(long long)syncTime
                      chatroomId:(NSString *)chatroomId
                          userId:(NSString *)userId
                prevMessageCount:(int)count
                           index:(int)index {
    SyncChatroomReq *req = [[SyncChatroomReq alloc] init];
    req.chatroomId = chatroomId;
    req.syncTime = syncTime;
    req.count = count;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jSyncChatroomMessage;
    body.targetId = userId;
    body.data_p = req.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)syncChatroomAttributes:(long long)syncTime
                        chatroomId:(NSString *)chatroomId
                            userId:(NSString *)userId
                             index:(int)index {
    SyncChatroomReq *req = [[SyncChatroomReq alloc] init];
    req.chatroomId = chatroomId;
    req.syncTime = syncTime;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jSyncChatroomAtts;
    body.targetId = userId;
    body.data_p = req.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)qryFirstUnreadMessage:(JConversation *)conversation index:(int)index {
    QryFirstUnreadMsgReq *req = [[QryFirstUnreadMsgReq alloc] init];
    req.targetId = conversation.conversationId;
    req.channelType = [self channelTypeFromConversationType:conversation.conversationType];
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jQryFirstUnreadMsg;
    body.targetId = conversation.conversationId;
    body.data_p = req.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)setAttributes:(NSDictionary<NSString *,NSString *> *)attributes
              forChatroom:(NSString *)chatroomId
                    index:(int)index {
    NSMutableArray *reqList = [NSMutableArray array];
    [attributes enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull key, NSString * _Nonnull obj, BOOL * _Nonnull stop) {
        ChatAttReq *req = [[ChatAttReq alloc] init];
        req.key = key;
        req.value = obj;
        req.isForce = NO;
        [reqList addObject:req];
    }];
    
    ChatAttBatchReq *batchReq = [[ChatAttBatchReq alloc] init];
    batchReq.attsArray = reqList;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jBatchAddAtt;
    body.targetId = chatroomId;
    body.data_p = batchReq.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)removeAttributes:(NSArray<NSString *> *)keys forChatroom:(NSString *)chatroomId index:(int)index {
    NSMutableArray *reqList = [NSMutableArray array];
    [keys enumerateObjectsUsingBlock:^(NSString * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
       ChatAttReq *req = [[ChatAttReq alloc] init];
        req.key = obj;
        req.isForce = NO;
        [reqList addObject:req];
    }];
    
    ChatAttBatchReq *batchReq = [[ChatAttBatchReq alloc] init];
    batchReq.attsArray = reqList;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jBatchDelAtt;
    body.targetId = chatroomId;
    body.data_p = batchReq.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)addMsgSet:(NSString *)messageId
         conversation:(JConversation *)conversation
                  key:(NSString *)key
               userId:(NSString *)userId
                index:(int)index {
    MsgExtItem *item = [[MsgExtItem alloc] init];
    item.key = key;
    item.value = userId;
    
    MsgExt *ext = [[MsgExt alloc] init];
    ext.targetId = conversation.conversationId;
    ext.channelType = (int32_t)conversation.conversationType;
    ext.msgId = messageId;
    ext.ext = item;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jMsgExSet;
    body.targetId = messageId;
    body.data_p = ext.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)removeMsgSet:(NSString *)messageId
            conversation:(JConversation *)conversation
                     key:(NSString *)key
                  userId:(NSString *)userId
                   index:(int)index {
    MsgExtItem *item = [[MsgExtItem alloc] init];
    item.key = key;
    item.value = userId;
    
    MsgExt *ext = [[MsgExt alloc] init];
    ext.targetId = conversation.conversationId;
    ext.channelType = (int32_t)conversation.conversationType;
    ext.msgId = messageId;
    ext.ext = item;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jDelMsgExSet;
    body.targetId = messageId;
    body.data_p = ext.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)queryMsgExSet:(NSArray<NSString *> *)messageIdList
             conversation:(JConversation *)conversation
                    index:(int)index {
    NSMutableArray *array = [NSMutableArray array];
    for (NSString *messageId in messageIdList) {
        [array addObject:messageId];
    }
    
    QryMsgExtReq *req = [[QryMsgExtReq alloc] init];
    req.targetId = conversation.conversationId;
    req.channelType = (int32_t)conversation.conversationType;
    req.msgIdsArray = array;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jQryMsgExSet;
    body.targetId = conversation.conversationId;
    body.data_p = req.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)addConversations:(NSArray<JConversation *> *)conversations
                       toTag:(NSString *)tagId
                      userId:(NSString *)userId
                       index:(int)index {
    NSMutableArray *pbConversations = [NSMutableArray array];
    for (JConversation *conversation in conversations) {
        SimpleConversation *pbConversation = [[SimpleConversation alloc] init];
        pbConversation.targetId = conversation.conversationId;
        pbConversation.channelType = (int32_t)conversation.conversationType;
        [pbConversations addObject:pbConversation];
    }
    
    TagConvers *tagConvers = [[TagConvers alloc] init];
    tagConvers.tag = tagId;
    tagConvers.conversArray = pbConversations;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jTagAddConvers;
    body.targetId = userId;
    body.data_p = tagConvers.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)removeConversations:(NSArray<JConversation *> *)conversations
                        fromTag:(NSString *)tagId
                         userId:(NSString *)userId
                          index:(int)index {
    NSMutableArray *pbConversations = [NSMutableArray array];
    for (JConversation *conversation in conversations) {
        SimpleConversation *pbConversation = [[SimpleConversation alloc] init];
        pbConversation.targetId = conversation.conversationId;
        pbConversation.channelType = (int32_t)conversation.conversationType;
        [pbConversations addObject:pbConversation];
    }
    
    TagConvers *tagConvers = [[TagConvers alloc] init];
    tagConvers.tag = tagId;
    tagConvers.conversArray = pbConversations;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jTagDelConvers;
    body.targetId = userId;
    body.data_p = tagConvers.data;
    
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(index)];
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
    NSData *data = [self.converter encode:body.data];
    
    ImWebsocketMsg *m = [self createImWebsocketMsg];
    m.cmd = JCmdTypePublishAck;
    m.qos = JQosNo;
    m.payload = data;
    
    return m.data;
}

- (NSData *)deleteMessage:(JConversation *)conversation
                  msgList:(NSArray <JConcreteMessage *> *)msgList
              forAllUsers:(BOOL)forAllUsers
                    index:(int)index{
    DelHisMsgsReq * req = [[DelHisMsgsReq alloc] init];
    req.targetId = conversation.conversationId;
    req.channelType = [self channelTypeFromConversationType:conversation.conversationType];
    if (forAllUsers) {
        req.delScope = 1;
    }
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
            isAutoCreate:(BOOL)isAutoCreate
                   index:(int)index {
    
    ChatroomReq *req = [[ChatroomReq alloc] init];
    req.chatId = chatroomId;
    req.isAutoCreate = isAutoCreate;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jJoinChatroom;
    body.targetId = chatroomId;
    body.data_p = [req data];
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

- (NSData *)callInvite:(NSString *)callId
           isMultiCall:(BOOL)isMultiCall
             mediaType:(JCallMediaType)mediaType
          targetIdList:(nonnull NSArray<NSString *> *)userIdList
            engineType:(NSUInteger)engineType
                 index:(int)index {
    RtcInviteReq *req = [[RtcInviteReq alloc] init];
    req.targetIdsArray = [userIdList mutableCopy];
    if (isMultiCall) {
        req.roomType = RtcRoomType_OneMore;
    } else {
        req.roomType = RtcRoomType_OneOne;
    }
    req.roomId = callId;
    if (engineType == 0) {
        req.rtcChannel = RtcChannel_Zego;
    } else if (engineType == 1) {
        req.rtcChannel = RtcChannel_LivekitRtc;
    }
    req.rtcMediaType = (int32_t)mediaType;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jRtcInvite;
    body.targetId = callId;
    body.data_p = [req data];
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)callHangup:(NSString *)callId
                 index:(int)index {
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jRtcHangUp;
    body.targetId = callId;
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)callAccept:(NSString *)callId
                 index:(int)index {
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jRtcAccept;
    body.targetId = callId;
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)callConnected:(NSString *)callId
                    index:(int)index {
    RtcMember *member = [[RtcMember alloc] init];
    member.rtcState = RtcState_RtcConnected;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jRtcUpdState;
    body.targetId = callId;
    body.data_p = [member data];
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)queryCallRooms:(NSString *)userId index:(int)index {
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jRtcMemberRooms;
    body.targetId = userId;
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)queryCallRoom:(NSString *)roomId index:(int)index {
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jRtcQry;
    body.targetId = roomId;
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)setLanguage:(NSString *)language
                 userId:(NSString *)userId
                  index:(int)index {
    NSMutableArray *arr = [NSMutableArray array];
    KvItem *item = [[KvItem alloc] init];
    item.key = jLanguage;
    item.value = language;
    [arr addObject:item];
    UserInfo *userInfo = [[UserInfo alloc] init];
    userInfo.settingsArray = arr;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jSetUserSettings;
    body.targetId = userId;
    body.data_p = [userInfo data];
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)getLanguage:(NSString *)userId index:(int)index {
    NSMutableArray *arr = [NSMutableArray array];
    KvItem *item = [[KvItem alloc] init];
    item.key = jLanguage;
    [arr addObject:item];
    UserInfo *userInfo = [[UserInfo alloc] init];
    userInfo.settingsArray = arr;
    
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jGetUserSettings;
    body.targetId = userId;
    body.data_p = [userInfo data];
    @synchronized (self) {
        [self.msgCmdDic setObject:body.topic forKey:@(body.index)];
    }
    ImWebsocketMsg *m = [self createImWebSocketMsgWithQueryMsg:body];
    return m.data;
}

- (NSData *)rtcPingData:(NSString *)callId
                  index:(int)index {
    QueryMsgBody *body = [[QueryMsgBody alloc] init];
    body.index = index;
    body.topic = jRtcPing;
    body.targetId = callId;
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
    switch (msg.cmd) {
        case JCmdTypeConnectAck:
        {
            obj.rcvType = JPBRcvTypeConnectAck;
            JConnectAck *a = [[JConnectAck alloc] init];
            NSData *decodeData = [self.converter decode:msg.payload];
            ConnectAckMsgBody *body = [[ConnectAckMsgBody alloc] initWithData:decodeData error:&err];
            if (err != nil) {
                JLogE(@"PB-Parse", @"ConnectAck decode message parse error, msg is %@", err.description);
                obj.rcvType = JPBRcvTypeParseError;
                return obj;
            }
            a.userId = body.userId;
            a.code = body.code;
            a.session = body.session;
            a.extra = body.ext;
            obj.connectAck = a;
        }
            break;
            
        case JCmdTypePublishAck:
        {
            NSData *decodeData = [self.converter decode:msg.payload];
            PublishAckMsgBody *body = [[PublishAckMsgBody alloc] initWithData:decodeData error:&err];
            if (err != nil) {
                JLogE(@"PB-Parse", @"PublishAck decode message parse error, msg is %@", err.description);
                obj.rcvType = JPBRcvTypeParseError;
                return obj;
            }
            
            NSString *cachedCmd;
            @synchronized (self) {
                cachedCmd = self.msgCmdDic[@(body.index)];
                [self.msgCmdDic removeObjectForKey:@(body.index)];
            }
            if (cachedCmd.length == 0) {
                JLogW(@"PB-Match", @"pub ack can't match a cached command");
                obj.rcvType = JPBRcvTypeCmdMatchError;
                return obj;
            }
            obj.rcvType = [self ackTypeWithCmd:cachedCmd];
            JPublishMsgAck *a = [[JPublishMsgAck alloc] init];
            a.index = body.index;
            a.code = body.code;
            a.msgId = body.msgId;
            a.timestamp = body.timestamp;
            a.seqNo = body.msgSeqNo;
            a.clientUid = body.clientMsgId;
            a.groupMemberCount = body.memberCount;
            if (body.hasModifiedMsg) {
                JMessage *modifiedMsg = [self messageWithDownMsg:body.modifiedMsg];
                a.contentType = modifiedMsg.contentType;
                a.content = modifiedMsg.content;
            }
            obj.publishMsgAck = a;
        }
            break;
            
        case JCmdTypeQueryAck:
        {
            NSData *decodeData = [self.converter decode:msg.payload];
            QueryAckMsgBody *body = [[QueryAckMsgBody alloc] initWithData:decodeData error:&err];
            if (err != nil) {
                JLogE(@"PB-Parse", @"QueryAck decode message parse error, msg is %@", err.description);
                obj.rcvType = JPBRcvTypeParseError;
                return obj;
            }
            
            NSString *cachedCmd;
            @synchronized (self) {
                cachedCmd = self.msgCmdDic[@(body.index)];
                [self.msgCmdDic removeObjectForKey:@(body.index)];
            }
            if (cachedCmd.length == 0) {
                JLogW(@"PB-Match", @"qry ack can't match a cached command");
                obj.rcvType = JPBRcvTypeCmdMatchError;
                return obj;
            }
            JPBRcvType type = [self ackTypeWithCmd:cachedCmd];
            switch (type) {
                case JPBRcvTypeQryHisMsgsAck:
                    obj = [self queryHistoryMessagesAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeSyncConvsAck:
                    obj = [self syncConversationsAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeSyncMsgsAck:
                    obj = [self syncMsgsAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeQryReadDetailAck:
                    obj = [self qryReadDetailAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeSimpleQryAck:
                    obj = [self simpleQryAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeSimpleQryAckCallbackTimestamp:
                    obj = [self simpleQryAckCallbackTimestampWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeConversationSetTopAck:
                    obj = [self conversationSetTopAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeAddConversation:
                    obj = [self addConversationWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeFileCredMsgAck:
                    obj = [self qryFileCredAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeGlobalMuteAck:
                    obj = [self globalMuteAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeSyncChatroomMsgsAck:
                    obj = [self syncChatroomMsgsAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeQryFirstUnreadMsgAck:
                    obj = [self qryFirstUnreadMsgAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeSetChatroomAttrAck:
                    obj = [self setChatroomAttrAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeRemoveChatroomAttrAck:
                    obj = [self removeChatroomAttrAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeSyncChatroomAttrsAck:
                    obj = [self syncChatroomAttrsAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeCallAuthAck:
                    obj = [self callInviteAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeRtcPingAck:
                    obj.rcvType = JPBRcvTypeRtcPingAck;
                    break;
                case JPBRcvTypeQryCallRoomsAck:
                    obj = [self qryCallRoomsAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeQryCallRoomAck:
                    obj = [self qryCallRoomAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeGetUserInfoAck:
                    obj = [self getUserInfoAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeQryMsgExtAck:
                    obj = [self qryMsgExtAckWithImWebsocketMsg:body];
                    break;
                case JPBRcvTypeGetTopMsgAck:
                    obj = [self getTopMsgAckWithImWebsocketMsg:body];
                    break;
                default:
                    break;
            }
        }
            break;
            
        case JCmdTypePublish:
        {
            NSData *decodeData = [self.converter decode:msg.payload];
            PublishMsgBody *body = [[PublishMsgBody alloc] initWithData:decodeData error:&err];
            if (err != nil) {
                JLogE(@"PB-Parse", @"publish decode message parse error, msg is %@", err.description);
                obj.rcvType = JPBRcvTypeParseError;
                return obj;
            }
            if ([body.topic isEqualToString:jNtf]) {
                Notify *ntf = [[Notify alloc] initWithData:body.data_p error:&err];
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
                } else if (ntf.type == NotifyType_ChatroomMsg) {
                    obj.rcvType = JPBRcvTypePublishChatroomMsgNtf;
                    JPublishMsgNtf *n = [[JPublishMsgNtf alloc] init];
                    n.syncTime = ntf.syncTime;
                    n.chatroomId = ntf.chatroomId;
                    obj.publishMsgNtf = n;
                } else if (ntf.type == NotifyType_ChatroomAtt) {
                    obj.rcvType = JPBRcvTypePublishChatroomAttrNtf;
                    JPublishMsgNtf *n = [[JPublishMsgNtf alloc] init];
                    n.syncTime = ntf.syncTime;
                    n.chatroomId = ntf.chatroomId;
                    obj.publishMsgNtf = n;
                } else if (ntf.type == NotifyType_ChatroomDestroy) {
                    obj.rcvType = JPBRcvTypeChatroomDestroyNtf;
                    JPublishMsgNtf *n = [[JPublishMsgNtf alloc] init];
                    n.syncTime = ntf.syncTime;
                    n.chatroomId = ntf.chatroomId;
                    obj.publishMsgNtf = n;
                }
            } else if ([body.topic isEqualToString:jMsg]) {
                DownMsg *downMsg = [[DownMsg alloc] initWithData:body.data_p error:&err];
                if (err != nil) {
                    JLogE(@"PB-Parse", @"publish msg parse error, msg is %@", err.description);
                    obj.rcvType = JPBRcvTypeParseError;
                    return obj;
                }
                obj.rcvType = JPBRcvTypePublishMsg;
                JPublishMsgBody *publishMsgBody = [[JPublishMsgBody alloc]init];
                publishMsgBody.rcvMessage = [self messageWithDownMsg:downMsg];
                publishMsgBody.index = body.index;
                publishMsgBody.qos = msg.qos;
                obj.publishMsgBody = publishMsgBody;
            } else if ([body.topic isEqualToString:jCUserNtf]) {
                ChrmEvent *event = [[ChrmEvent alloc] initWithData:body.data_p error:&err];
                if (err != nil) {
                    JLogE(@"PB-Parse", @"publish msg jCUserNtf parse error, msg is %@", err.description);
                    obj.rcvType = JPBRcvTypeParseError;
                    return obj;
                }
                obj.rcvType = JPBRcvTypeChatroomEventNtf;
                JPublishMsgNtf *n = [[JPublishMsgNtf alloc] init];
                n.chatroomId = event.chatId;
                n.type = (NSUInteger)event.eventType;
                obj.publishMsgNtf = n;
            } else if ([body.topic isEqualToString:jRtcRoomEvent]) {
                RtcRoomEvent *event = [[RtcRoomEvent alloc] initWithData:body.data_p error:&err];
                if (err != nil) {
                    JLogE(@"PB-Parse", @"publish rtc_room_event parse error, msg is %@",err.description);
                    obj.rcvType = JPBRcvTypeParseError;
                    return obj;
                }
                obj.rcvType = JPBRcvTypeRtcRoomEventNtf;
                JRtcRoomEventNtf *n = [[JRtcRoomEventNtf alloc] init];
                n.eventType = (NSUInteger)event.roomEventType;
                NSMutableArray *membersArray = [NSMutableArray array];
                for (RtcMember *pbMember in event.membersArray) {
                    JCallMember *callMember = [self callMemberWithPBRtcMember:pbMember];
                    [membersArray addObject:callMember];
                }
                n.members = membersArray;
                n.room = [self rtcRoomWithPBRtcRoom:event.room];
                obj.rtcRoomEventNtf = n;
            } else if ([body.topic isEqualToString:jRtcInviteEvent]) {
                RtcInviteEvent *event = [[RtcInviteEvent alloc] initWithData:body.data_p error:&err];
                if (err != nil) {
                    JLogE(@"PB-Parse", @"publish rtc_invite_event parse error, msg is %@",err.description);
                    obj.rcvType = JPBRcvTypeParseError;
                    return obj;
                }
                obj.rcvType = JPBRcvTypeRtcInviteEventNtf;
                JRtcInviteEventNtf *n = [[JRtcInviteEventNtf alloc] init];
                n.type = (NSUInteger)event.inviteType;
                n.user = [self userInfoWithPBUserInfo:event.user];
                n.room = [self rtcRoomWithPBRtcRoom:event.room];
                NSMutableArray *targetUserList = [NSMutableArray array];
                for (UserInfo *pbUserInfo in event.targetUsersArray) {
                    JUserInfo *u = [self userInfoWithPBUserInfo:pbUserInfo];
                    [targetUserList addObject:u];
                }
                n.targetUsers = [targetUserList copy];
                obj.rtcInviteEventNtf = n;
            }
        }
            break;
            
        case JCmdTypeDisconnect:
        {
            NSData *decodeData = [self.converter decode:msg.payload];
            DisconnectMsgBody *body = [[DisconnectMsgBody alloc] initWithData:decodeData error:&err];
            if (err != nil) {
                JLogE(@"PB-Parse", @"disconnect decode message parse error, msg is %@", err.description);
                obj.rcvType = JPBRcvTypeParseError;
                return obj;
            }
            
            obj.rcvType = JPBRcvTypeDisconnectMsg;
            JDisconnectMsg *m = [[JDisconnectMsg alloc] init];
            m.code = body.code;
            m.timestamp = body.timestamp;
            m.extra = body.ext;
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
    NSData *data = [self.converter encode:body.data];
    ImWebsocketMsg *sm = [self createImWebsocketMsg];
    sm.cmd = JCmdTypePublish;
    sm.qos = JQosYes;
    sm.payload = data;
    return sm;
}

- (ImWebsocketMsg *)createImWebSocketMsgWithQueryMsg:(QueryMsgBody *)body {
    NSData *data = [self.converter encode:body.data];
    ImWebsocketMsg *m = [self createImWebsocketMsg];
    m.cmd = JCmdTypeQuery;
    m.qos = JQosYes;
    m.payload = data;
    return m;
}

- (ImWebsocketMsg *)createImWebsocketMsg {
    ImWebsocketMsg *m = [[ImWebsocketMsg alloc] init];
    m.version = JIMProtocolVersion;
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
    msg.isEdit = msg.flags & JMessageFlagIsModified;
    JGroupMessageReadInfo *info = [[JGroupMessageReadInfo alloc] init];
    info.readCount = downMsg.readCount;
    info.memberCount = downMsg.memberCount;
    msg.groupReadInfo = info;
    msg.groupInfo = [self groupInfoWithPBGroupInfo:downMsg.groupInfo];
    msg.targetUserInfo = [self userInfoWithPBUserInfo:downMsg.targetUserInfo];
    msg.groupMemberInfo = [self groupMemberWithPBGroupMember:downMsg.grpMemberInfo
                                                groupId:msg.groupInfo.groupId
                                                 userId:msg.targetUserInfo.userId];
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
    result.type = (int)pbUserInfo.userType;
    if (pbUserInfo.extFieldsArray_Count > 0) {
        NSMutableDictionary *dic = [NSMutableDictionary dictionary];
        for (KvItem *item in pbUserInfo.extFieldsArray) {
            [dic setObject:item.value forKey:item.key];
        }
        result.extraDic = [dic copy];
    }
    return result;
}

- (JGroupMember *)groupMemberWithPBGroupMember:(GrpMemberInfo *)pbGroupMember
                                       groupId:(NSString *)groupId
                                        userId:(NSString *)userId {
    if (pbGroupMember == nil) {
        return nil;
    }
    if (pbGroupMember.updatedTime == 0) {
        return nil;
    }
    JGroupMember *result = [JGroupMember new];
    result.groupId = groupId;
    result.userId = userId;
    result.groupDisplayName = pbGroupMember.grpDisplayName;
    if (pbGroupMember.extFieldsArray_Count > 0) {
        NSMutableDictionary *dic = [NSMutableDictionary dictionary];
        for (KvItem *item in pbGroupMember.extFieldsArray) {
            [dic setObject:item.value forKey:item.key];
        }
        result.extraDic = [dic copy];
    }
    return result;
}

- (JCallMember *)callMemberWithPBRtcMember:(RtcMember *)pbMember {
    if (pbMember == nil) {
        return nil;
    }
    JCallMember *result = [[JCallMember alloc] init];
    result.userInfo = [self userInfoWithPBUserInfo:pbMember.member];
    result.callStatus = (NSInteger)pbMember.rtcState;
    result.startTime = pbMember.callTime;
    result.connectTime = pbMember.connectTime;
    result.finishTime = pbMember.hangupTime;
    result.inviter = [self userInfoWithPBUserInfo:pbMember.inviter];
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

- (JRtcRoom *)rtcRoomWithPBRtcRoom:(RtcRoom *)pbRoom {
    if (pbRoom == nil) {
        return nil;
    }
    JRtcRoom *result = [[JRtcRoom alloc] init];
    result.roomId = pbRoom.roomId;
    result.owner = [self userInfoWithPBUserInfo:pbRoom.owner];
    result.isMultiCall = pbRoom.roomType == RtcRoomType_OneMore;
    result.mediaType = (int)pbRoom.rtcMediaType;
    
    NSMutableArray <JCallMember *> *members = [NSMutableArray array];
    for (RtcMember *member in pbRoom.membersArray) {
        JCallMember *outMember = [self callMemberWithPBRtcMember:member];
        [members addObject:outMember];
    }
    result.members = members;
    return result;
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
    info.hasUnread = conversation.unreadTag;
    if (conversation.converTagsArray_Count > 0) {
        NSMutableArray <NSString *> *tagIdList = [NSMutableArray array];
        for (ConverTag *pbTag in conversation.converTagsArray) {
            [tagIdList addObject:pbTag.tag];
        }
        info.tagIdList = [tagIdList copy];
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
    mentionMessage.type = (JMentionType)mentionMsg.mentionType;
    return mentionMessage;
}

- (JPBRcvObj *)queryHistoryMessagesAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    DownMsgSet *set = [[DownMsgSet alloc] initWithData:body.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"query history messages parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeQryHisMsgsAck;
    JQryHisMsgsAck *a = [[JQryHisMsgsAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
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

- (JPBRcvObj *)syncConversationsAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    QryConversationsResp *resp = [[QryConversationsResp alloc] initWithData:body.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"sync conversations parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeSyncConvsAck;
    JSyncConvsAck *a = [[JSyncConvsAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
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

- (JPBRcvObj *)syncMsgsAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    DownMsgSet *set = [[DownMsgSet alloc] initWithData:body.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"sync messages parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeSyncMsgsAck;
    //sync 和 query history 共用一个 ack
    JQryHisMsgsAck *a = [[JQryHisMsgsAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
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

- (JPBRcvObj *)syncChatroomMsgsAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    SyncChatroomMsgResp *resp = [[SyncChatroomMsgResp alloc] initWithData:body.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"sync chatroom messages parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeSyncChatroomMsgsAck;
    JQryHisMsgsAck *a = [[JQryHisMsgsAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
    a.isFinished = YES;
    NSMutableArray *arr = [[NSMutableArray alloc] init];
    if (resp.msgsArray_Count > 0) {
        [resp.msgsArray enumerateObjectsUsingBlock:^(DownMsg * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            JConcreteMessage *msg = [self messageWithDownMsg:obj];
            [arr addObject:msg];
        }];
    }
    a.msgs = arr;
    obj.qryHisMsgsAck = a;
    return obj;
}

- (JPBRcvObj *)syncChatroomAttrsAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    SyncChatroomAttResp *resp = [[SyncChatroomAttResp alloc] initWithData:body.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"sync chatroom attributes parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeSyncChatroomAttrsAck;
    JChatroomAttrsAck *a = [[JChatroomAttrsAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
    NSMutableArray *arr = [[NSMutableArray alloc] init];
    if (resp.attsArray > 0) {
        [resp.attsArray enumerateObjectsUsingBlock:^(ChatAttItem * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            JChatroomAttributeItem *item = [[JChatroomAttributeItem alloc] init];
            item.key = obj.key;
            item.value = obj.value;
            item.timestamp = obj.attTime;
            item.userId = obj.userId;
            item.type = (NSInteger)obj.optType;
            [arr addObject:item];
        }];
    }
    a.items = arr;
    obj.chatroomAttrsAck = a;
    return obj;
}

- (JPBRcvObj *)callInviteAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    RtcAuth *rtcAuth = [[RtcAuth alloc] initWithData:body.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"call invite ack parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeCallAuthAck;
    JRtcAuthAck *a = [[JRtcAuthAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
    if (rtcAuth.hasZegoAuth) {
        ZegoAuth *zegoAuth = rtcAuth.zegoAuth;
        a.token = zegoAuth.token;
    } else if (rtcAuth.hasLivekitRtcAuth) {
        LivekitRtcAuth *liveKitAuth = rtcAuth.livekitRtcAuth;
        a.token = liveKitAuth.token;
        a.url = liveKitAuth.serviceURL;
    }
    obj.rtcAuthAck = a;
    return obj;
}

- (JPBRcvObj *)qryCallRoomsAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    RtcMemberRooms *rooms = [[RtcMemberRooms alloc] initWithData:body.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"qry call rooms ack parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeQryCallRoomsAck;
    NSMutableArray <JRtcRoom *> *outRooms = [NSMutableArray array];
    for (RtcMemberRoom *room in rooms.roomsArray) {
        JRtcRoom *outRoom = [[JRtcRoom alloc] init];
        outRoom.roomId = room.roomId;
        outRoom.deviceId = room.deviceId;
        outRoom.callStatus = (int)room.rtcState;
        [outRooms addObject:outRoom];
    }
    JRtcQryCallRoomsAck *a = [[JRtcQryCallRoomsAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
    a.rooms = outRooms;
    obj.rtcQryCallRoomsAck = a;
    return obj;
}

- (JPBRcvObj *)qryCallRoomAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    RtcRoom *room = [[RtcRoom alloc] initWithData:body.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"qry call room ack parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeQryCallRoomAck;
    NSMutableArray <JRtcRoom *> *outRooms = [NSMutableArray array];
    JRtcRoom *outRoom = [self rtcRoomWithPBRtcRoom:room];
    [outRooms addObject:outRoom];
    //共用 JRtcQryCallRoomsAck
    JRtcQryCallRoomsAck *a = [[JRtcQryCallRoomsAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
    a.rooms = outRooms;
    obj.rtcQryCallRoomsAck = a;
    return obj;
}

- (JPBRcvObj *)getUserInfoAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    UserInfo *userInfo = [[UserInfo alloc] initWithData:body.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"get userInfo ack parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeGetUserInfoAck;
    NSString *s = @"";
    for (KvItem *item in userInfo.settingsArray) {
        if ([item.key isEqualToString:jLanguage]) {
            s = item.value;
            break;
        }
    }
    JStringAck *a = [[JStringAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
    a.str = s;
    obj.stringAck = a;
    return obj;
}

- (JPBRcvObj *)qryMsgExtAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    MsgExtItemsList *list = [[MsgExtItemsList alloc] initWithData:body.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"qry msg ext ack parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeQryMsgExtAck;
    NSMutableArray <JMessageReaction *> *reactionList = [NSMutableArray array];
    for (MsgExtItems *pbItems in list.itemsArray) {
        JMessageReaction *reaction = [[JMessageReaction alloc] init];
        reaction.messageId = pbItems.msgId;
        NSMutableArray <JMessageReactionItem *> *itemList = [NSMutableArray array];
        BOOL isUpdate = NO;
        for (MsgExtItem *pbItem in pbItems.extsArray) {
            JUserInfo *user = [self userInfoWithPBUserInfo:pbItem.userInfo];
            isUpdate = NO;
            for (JMessageReactionItem *loopItem in itemList) {
                if ([loopItem.reactionId isEqualToString:pbItem.key]) {
                    isUpdate = YES;
                    NSMutableArray <JUserInfo *> *userInfoList = [loopItem.userInfoList mutableCopy];
                    [userInfoList addObject:user];
                    loopItem.userInfoList = [userInfoList copy];
                    break;
                }
            }
            if (!isUpdate) {
                JMessageReactionItem *reactionItem = [[JMessageReactionItem alloc] init];
                reactionItem.reactionId = pbItem.key;
                reactionItem.userInfoList = @[user];
                [itemList addObject:reactionItem];
            }
        }
        reaction.itemList = [itemList copy];
        [reactionList addObject:reaction];
    }
    JQryMsgExtAck *ack = [[JQryMsgExtAck alloc] init];
    [ack encodeWithQueryAckMsgBody:body];
    ack.reactionList = reactionList;
    obj.qryMsgExtAck = ack;
    return obj;
}

- (JPBRcvObj *)getTopMsgAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    TopMsg *topMsg = [[TopMsg alloc] initWithData:body.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"get top msg ack parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeGetTopMsgAck;
    JConcreteMessage *concreteMessage = [self messageWithDownMsg:topMsg.msg];
    JUserInfo *userInfo = [self userInfoWithPBUserInfo:topMsg.operator_p];
    long long timestamp = topMsg.createdTime;
    JGetTopMsgAck *ack = [[JGetTopMsgAck alloc] init];
    [ack encodeWithQueryAckMsgBody:body];
    ack.message = concreteMessage;
    ack.userInfo = userInfo;
    ack.createdTime = timestamp;
    obj.getTopMsgAck = ack;
    return obj;
}

- (JPBRcvObj *)qryFirstUnreadMsgAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    obj.rcvType = JPBRcvTypeQryFirstUnreadMsgAck;
    JQryHisMsgsAck *a = [[JQryHisMsgsAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
    a.isFinished = YES;
    NSMutableArray *arr = [[NSMutableArray alloc] init];
    if (body.data_p.length > 0) {
        NSError *e = nil;
        DownMsg *downMsg = [[DownMsg alloc] initWithData:body.data_p error:&e];
        if (e != nil) {
            JLogE(@"PB-Parse", @"qry first unread message parse error, msg is %@", e.description);
            obj.rcvType = JPBRcvTypeParseError;
            return obj;
        }
        JConcreteMessage *message = [self messageWithDownMsg:downMsg];
        [arr addObject:message];
    }
    a.msgs = arr;
    obj.qryHisMsgsAck = a;
    return obj;
}

- (JPBRcvObj *)setChatroomAttrAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    obj.rcvType = JPBRcvTypeSetChatroomAttrAck;
    
    JChatroomAttrsAck *a = [[JChatroomAttrsAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
    NSMutableArray *arr = [NSMutableArray array];
    if (body.data_p.length > 0) {
        NSError *e = nil;
        ChatAttBatchResp *batchResp = [[ChatAttBatchResp alloc] initWithData:body.data_p error:&e];
        if (e != nil) {
            JLogE(@"PB-Parse", @"set chatroom attribute parse error, msg is %@", e.description);
            obj.rcvType = JPBRcvTypeParseError;
            return obj;
        }
        [batchResp.attRespsArray enumerateObjectsUsingBlock:^(ChatAttResp * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            JChatroomAttributeItem *item = [[JChatroomAttributeItem alloc] init];
            item.key = obj.key;
            item.code = obj.code;
            item.timestamp = obj.attTime;
            [arr addObject:item];
        }];
    }
    a.items = arr;
    obj.chatroomAttrsAck = a;
    return obj;
}

- (JPBRcvObj *)removeChatroomAttrAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    obj.rcvType = JPBRcvTypeRemoveChatroomAttrAck;
    
    JChatroomAttrsAck *a = [[JChatroomAttrsAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
    NSMutableArray *arr = [NSMutableArray array];
    if (body.data_p.length > 0) {
        NSError *e = nil;
        ChatAttBatchResp *batchResp = [[ChatAttBatchResp alloc] initWithData:body.data_p error:&e];
        if (e != nil) {
            JLogE(@"PB-Parse", @"remove chatroom attribute parse error, msg is %@", e.description);
            obj.rcvType = JPBRcvTypeParseError;
            return obj;
        }
        [batchResp.attRespsArray enumerateObjectsUsingBlock:^(ChatAttResp * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            JChatroomAttributeItem *item = [[JChatroomAttributeItem alloc] init];
            item.key = obj.key;
            item.code = obj.code;
            item.timestamp = obj.attTime;
            [arr addObject:item];
        }];
    }
    a.items = arr;
    obj.chatroomAttrsAck = a;
    return obj;
}

- (JPBRcvObj *)simpleQryAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    obj.rcvType = JPBRcvTypeSimpleQryAck;
    JSimpleQryAck *a = [[JSimpleQryAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
    obj.simpleQryAck = a;
    return obj;
}

- (JPBRcvObj *)simpleQryAckCallbackTimestampWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    obj.rcvType = JPBRcvTypeSimpleQryAckCallbackTimestamp;
    JSimpleQryAck *a = [[JSimpleQryAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
    obj.simpleQryAck = a;
    return obj;
}

- (JPBRcvObj *)qryFileCredAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    QryFileCredResp * resp = [[QryFileCredResp alloc] initWithData:body.data_p error:&e];
    if(e != nil){
        JLogE(@"PB-Parse", @"file cred ack parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeFileCredMsgAck;
    JQryFileCredAck * a = [[JQryFileCredAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
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

- (JPBRcvObj *)globalMuteAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    UserUndisturb *resp = [[UserUndisturb alloc] initWithData:body.data_p error:&e];
    if(e != nil){
        JLogE(@"PB-Parse", @"global mute ack parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeGlobalMuteAck;
    JGlobalMuteAck *a = [[JGlobalMuteAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
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

- (JPBRcvObj *)conversationSetTopAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    TopConversResp *resp = [[TopConversResp alloc] initWithData:body.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"set top ack parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeConversationSetTopAck;
    JTimestampQryAck *a = [[JTimestampQryAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
    a.operationTime = resp.optTime;
    obj.timestampQryAck = a;
    return obj;
}

- (JPBRcvObj *)addConversationWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    Conversation *pbConv = [[Conversation alloc] initWithData:body.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"add conversation error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeAddConversation;
    JConversationInfoAck *a = [[JConversationInfoAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
    a.conversationInfo = [self conversationWithPBConversation:pbConv];
    obj.conversationInfoAck = a;
    return obj;
}

- (JPBRcvObj *)qryReadDetailAckWithImWebsocketMsg:(QueryAckMsgBody *)body {
    JPBRcvObj *obj = [[JPBRcvObj alloc] init];
    NSError *e = nil;
    QryReadDetailResp *resp = [[QryReadDetailResp alloc] initWithData:body.data_p error:&e];
    if (e != nil) {
        JLogE(@"PB-Parse", @"qry read detail ack parse error, msg is %@", e.description);
        obj.rcvType = JPBRcvTypeParseError;
        return obj;
    }
    obj.rcvType = JPBRcvTypeQryReadDetailAck;
    JQryReadDetailAck *a = [[JQryReadDetailAck alloc] init];
    [a encodeWithQueryAckMsgBody:body];
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
             jQuitChatroom:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jMarkUnread:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jSyncChatroomMessage:@(JPBRcvTypeSyncChatroomMsgsAck),
             jQryFirstUnreadMsg:@(JPBRcvTypeQryFirstUnreadMsgAck),
             jBatchAddAtt:@(JPBRcvTypeSetChatroomAttrAck),
             jSyncChatroomAtts:@(JPBRcvTypeSyncChatroomAttrsAck),
             jBatchDelAtt:@(JPBRcvTypeRemoveChatroomAttrAck),
             jRtcInvite:@(JPBRcvTypeCallAuthAck),
             jRtcHangUp:@(JPBRcvTypeSimpleQryAck),
             jRtcAccept:@(JPBRcvTypeCallAuthAck),
             jRtcUpdState:@(JPBRcvTypeSimpleQryAck),
             jRtcPing:@(JPBRcvTypeRtcPingAck),
             jRtcMemberRooms:@(JPBRcvTypeQryCallRoomsAck),
             jRtcQry:@(JPBRcvTypeQryCallRoomAck),
             jSetUserSettings:@(JPBRcvTypeSimpleQryAck),
             jGetUserSettings:@(JPBRcvTypeGetUserInfoAck),
             kModifyMsg:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jMsgExSet:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jDelMsgExSet:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jQryMsgExSet:@(JPBRcvTypeQryMsgExtAck),
             jTagAddConvers:@(JPBRcvTypeSimpleQryAck),
             jTagDelConvers:@(JPBRcvTypeSimpleQryAck),
             jSetTopMsg:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jDelTopMsg:@(JPBRcvTypeSimpleQryAckCallbackTimestamp),
             jGetTopMsg:@(JPBRcvTypeGetTopMsgAck)
    };
}
@end
