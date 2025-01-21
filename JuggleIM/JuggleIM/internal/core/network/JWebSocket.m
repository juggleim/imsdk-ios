//
//  JWebSocket.m
// JuggleIM
//
//  Created by Nathan on 2023/11/29.
//

#import "JWebSocket.h"
#import "SRWebSocket.h"
#import "JUtility.h"
#import "JuggleIMConstInternal.h"
#import "JPBData.h"
#import "JLogger.h"
#import "JHeartBeatManager.h"
#import "JBlockObj.h"
#import "JWebSocketCommandManager.h"

#define jProtocolHead @"://"
#define jWSPrefix @"ws://"
#define jWSSPrefix @"wss://"
#define jWebSocketSuffix @"/im"
#define jMaxConcurrentCount 5

typedef NS_ENUM(NSUInteger, JWebSocketStatus) {
    JWebSocketStatusIdle,
    JWebSocketStatusFailure,
    JWebSocketStatusSuccess
};

@interface JWebSocket () <SRWebSocketDelegate, JCommandTimeOutDelegate>
@property (nonatomic, weak) id<JWebSocketConnectDelegate> connectDelegate;
@property (nonatomic, weak) id<JWebSocketMessageDelegate> messageDelegate;
@property (nonatomic, weak) id<JWebSocketChatroomDelegate> chatroomDelegate;
@property (nonatomic, weak) id<JWebSocketCallDelegate> callDelegate;
@property (nonatomic, copy) NSString *appKey;
@property (nonatomic, copy) NSString *token;
@property (nonatomic, copy) NSString *pushToken;
@property (nonatomic, copy) NSString *voipToken;
@property (nonatomic, strong) SRWebSocket *sws;
@property (nonatomic, strong) dispatch_queue_t sendQueue;
@property (nonatomic, strong) dispatch_queue_t receiveQueue;
/// 所有上行数据的自增 index
@property (nonatomic, assign) int32_t cmdIndex;
@property (nonatomic, strong) JPBData *pbData;
@property (nonatomic, strong) NSOperationQueue *competeQueue;
@property (nonatomic, assign) BOOL isCompeteFinish;
@property (nonatomic, strong) NSMutableArray <SRWebSocket *> *competeSwsList;
@property (nonatomic, strong) NSMutableArray <NSNumber *> *competeStatusList;
@property (nonatomic, strong) JHeartBeatManager *heartbeatManager;
@property (nonatomic, strong) JWebSocketCommandManager *commandManager;
@end

@implementation JWebSocket

- (instancetype)initWithSendQueque:(dispatch_queue_t)sendQueue receiveQueue:(dispatch_queue_t)receiveQueue {
    if (self = [super init]) {
        self.heartbeatManager = [[JHeartBeatManager alloc] initWithWebSocket:self];
        self.sendQueue = sendQueue;
        self.receiveQueue = receiveQueue;
        self.commandManager = [[JWebSocketCommandManager alloc] initWithDelegate:self];
        [self.commandManager start];
        self.pbData = [[JPBData alloc] init];
        NSOperationQueue *competeQueue = [[NSOperationQueue alloc] init];
        competeQueue.maxConcurrentOperationCount = jMaxConcurrentCount;
        self.competeQueue = competeQueue;
    }
    return self;
}

- (void)connect:(NSString *)appKey
          token:(NSString *)token
      pushToken:(NSString *)pushToken
      voipToken:(NSString *)voipToken
        servers:(nonnull NSArray *)servers {
    dispatch_async(self.sendQueue, ^{
        JLogI(@"WS-Connect", @"appkey is %@, token is %@", appKey, token);
        self.appKey = appKey;
        self.token = token;
        self.pushToken = pushToken;
        self.voipToken = voipToken;
        
        [self resetSws];
        for (NSString *url in servers) {
            JLogI(@"WS-Connect", @"create web socket url is %@", url);
            SRWebSocket *sws = [self createWebSocket:url];
            [self.competeSwsList addObject:sws];
            [self.competeStatusList addObject:@(JWebSocketStatusIdle)];
            NSBlockOperation *operation = [NSBlockOperation blockOperationWithBlock:^{
                [sws open];
            }];
            [self.competeQueue addOperation:operation];
        }
    });
}

- (void)disconnect:(BOOL)needPush {
    dispatch_async(self.sendQueue, ^{
        JLogI(@"WS-Disconnect", @"need push is %d", needPush);
        [self sendDisconnectMsgByWebSocket:needPush];
    });
}

- (void)startHeartbeat {
    [self.heartbeatManager start];
}

- (void)stopHeartbeat {
    [self.heartbeatManager stop];
}

- (void)heartbeatTimeOut {
    [self.connectDelegate webSocketDidTimeOut];
}

- (void)pushRemainCmdAndCallbackError {
    NSArray <JBlockObj *> *objs = [self.commandManager clearBlockObject];
    for (JBlockObj *obj in objs) {
        [self command:obj error:JErrorCodeInternalConnectionUnavailable];
    }
}

- (void)setConnectDelegate:(id<JWebSocketConnectDelegate>)delegate {
    _connectDelegate = delegate;
}

- (void)setMessageDelegate:(id<JWebSocketMessageDelegate>)delegate {
    _messageDelegate = delegate;
}

- (void)setChatroomDelegate:(id<JWebSocketChatroomDelegate>)delegate {
    _chatroomDelegate = delegate;
}

- (void)setCallDelegate:(id<JWebSocketCallDelegate>)delegate {
    _callDelegate = delegate;
}

#pragma mark - send pb
- (void)sendIMMessage:(JMessageContent *)content
       inConversation:(JConversation *)conversation
          clientMsgNo:(long long)clientMsgNo
            clientUid:(NSString *)clientUid
            mergeInfo:(JMergeInfo *)mergeInfo
          isBroadcast:(BOOL)isBroadcast
               userId:(NSString *)userId
          mentionInfo:(JMessageMentionInfo *)mentionInfo
      referredMessage:(JConcreteMessage *)referredMessage
             pushData:(JPushData *)pushData
              success:(void (^)(long long clientMsgNo, NSString *msgId, long long timestamp, long long seqNo,  NSString * _Nullable contentType,  JMessageContent * _Nullable content))successBlock
                error:(void (^)(JErrorCodeInternal errorCode, long long clientMsgNo))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *encodeData;
        if ([content isKindOfClass:[JMediaMessageContent class]]) {
            JMediaMessageContent *mediaContent = (JMediaMessageContent *)content;
            NSString *local = mediaContent.localPath;
            mediaContent.localPath = nil;
            encodeData = [mediaContent encode];
            mediaContent.localPath = local;
        } else {
            encodeData = [content encode];
        }
        NSData *d = [self.pbData sendMessageDataWithType:[[content class] contentType]
                                                 msgData:encodeData
                                                   flags:[[content class] flags]
                                               clientUid:clientUid
                                               mergeInfo:mergeInfo
                                             isBroadcast:isBroadcast
                                                  userId:userId
                                                   index:self.cmdIndex++
                                        conversationType:conversation.conversationType
                                          conversationId:conversation.conversationId
                                             mentionInfo:mentionInfo
                                         referredMessage:referredMessage
                                                pushData:pushData];
        JLogI(@"WS-Send", @"send message");
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            JLogE(@"WS-Send", @"send message error, msg is %@", err.description);
            if (errorBlock) {
                errorBlock(JErrorCodeInternalWebSocketFailure, clientMsgNo);
            }
        } else {
            JSendMessageObj *obj = [[JSendMessageObj alloc] init];
            obj.clientMsgNo = clientMsgNo;
            obj.successBlock = successBlock;
            obj.errorBlock = errorBlock;
            [self.commandManager setBlockObject:obj forKey:key];
        }
    });
}

- (void)recallMessage:(NSString *)messageId
               extras:(NSDictionary *)extras
         conversation:(JConversation *)conversation
            timestamp:(long long)timestamp
              success:(void (^)(long long timestamp))successBlock
                error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData recallMessageData:messageId
                                            extras:extras
                                      conversation:conversation
                                         timestamp:timestamp
                                             index:self.cmdIndex++];
        JLogI(@"WS-Send", @"recall message, id is %@", messageId);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)sendReadReceipt:(NSArray <NSString *> *)messageIds
         inConversation:(JConversation *)conversation
                success:(void (^)(long long timestamp))successBlock
                  error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData sendReadReceiptData:messageIds
                                      inConversation:conversation
                                               index:self.cmdIndex++];
        JLogI(@"WS-Send", @"send read receipt");
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)getGroupMessageReadDetail:(NSString *)messageId
                   inConversation:(JConversation *)conversation
                          success:(void (^)(NSArray<JUserInfo *> *readMembers, NSArray<JUserInfo *> *unreadMembers))successBlock
                            error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData getGroupMessageReadDetail:messageId
                                            inConversation:conversation
                                                     index:self.cmdIndex++];
        JLogI(@"WS-Send", @"get group message read detail, id is %@", messageId);
        JQryReadDetailObj *obj = [[JQryReadDetailObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)syncMessagesWithReceiveTime:(long long)receiveTime
                           sendTime:(long long)sendTime
                             userId:(NSString *)userId {
    dispatch_async(self.sendQueue, ^{
        NSData *d = [self.pbData syncMessagesDataWithReceiveTime:receiveTime
                                                        sendTime:sendTime
                                                          userId:userId
                                                           index:self.cmdIndex++];
        JLogI(@"WS-Send", @"sync messages, receive is %lld, send is %lld", receiveTime, sendTime);
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            JLogE(@"WS-Send", @"sync message error, msg is %@", err.description);
        }
    });
}

- (void)queryHisMsgsFrom:(JConversation *)conversation
               startTime:(long long)startTime
                   count:(int)count
               direction:(JPullDirection)direction
            contentTypes:(NSArray <NSString *> *)contentTypes
                 success:(void (^)(NSArray * _Nonnull, BOOL))successBlock
                   error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData queryHisMsgsDataFrom:conversation
                                            startTime:startTime
                                                count:count
                                            direction:direction
                                         contentTypes:contentTypes
                                                index:self.cmdIndex++];
        JLogI(@"WS-Send", @"query history messages");
        JQryHisMsgsObj *obj = [[JQryHisMsgsObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)queryHisMsgsByIds:(NSArray<NSString *> *)messageIds
           inConversation:(JConversation *)conversation
                  success:(void (^)(NSArray<JConcreteMessage *> * _Nonnull, BOOL isFinished))successBlock
                    error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData queryHisMsgsDataByIds:messageIds
                                        inConversation:conversation
                                                 index:self.cmdIndex++];
        JLogI(@"WS-Send", @"query history message by id");
        JQryHisMsgsObj *obj = [[JQryHisMsgsObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)syncConversations:(long long)startTime
                    count:(int)count
                   userId:(NSString *)userId
                  success:(void (^)(NSArray * _Nonnull, NSArray * _Nonnull, BOOL))successBlock
                    error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData syncConversationsData:startTime
                                                 count:count
                                                userId:userId
                                                 index:self.cmdIndex++];
        JLogI(@"WS-Send", @"sync conversation, time is %lld", startTime);
        JSyncConvsObj *obj = [[JSyncConvsObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)deleteConversationInfo:(JConversation *)conversation
                        userId:(NSString *)userId
                       success:(void (^)(long long timestamp))successBlock
                         error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData deleteConversationData:conversation
                                                 userId:userId
                                                  index:self.cmdIndex++];
        JLogI(@"WS-Send", @"delete conversation, type is %lu, id is %@", (unsigned long)conversation.conversationType, conversation.conversationId);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)clearUnreadCount:(JConversation *)conversation
                  userId:(NSString *)userId
                msgIndex:(long long)msgIndex
                   msgId:(nonnull NSString *)msgId
               timestamp:(long long)timestamp
                 success:(void (^)(long long timestamp))successBlock
                   error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData clearUnreadCountData:conversation
                                               userId:userId
                                             msgIndex:msgIndex
                                                msgId:msgId
                                            timestamp:timestamp
                                                index:self.cmdIndex++];
        JLogI(@"WS-Send", @"clear unread, type is %lu, id is %@, msgIndex is %lld", (unsigned long)conversation.conversationType, conversation.conversationId, msgIndex);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)setMute:(BOOL)isMute
 inConversation:(JConversation *)conversation
         userId:(NSString *)userId
        success:(void (^)(long long timestamp))successBlock
          error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData undisturbData:conversation
                                        userId:userId
                                        isMute:isMute
                                         index:self.cmdIndex++];
        JLogI(@"WS-Send", @"set mute, mute is %d, type is %lu, id is %@", isMute, (unsigned long)conversation.conversationType, conversation.conversationId);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)setTop:(BOOL)isTop
inConversation:(JConversation *)conversation
        userId:(NSString *)userId
       success:(void (^)(long long timestamp))successBlock
         error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData topConversationData:conversation
                                              userId:userId
                                               isTop:isTop
                                               index:self.cmdIndex++];
        JLogI(@"WS-Send", @"set top, top is %d, type is %lu, id is %@", isTop, (unsigned long)conversation.conversationType, conversation.conversationId);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)setUnread:(JConversation *)conversation
           userId:(NSString *)userId
          success:(void (^)(long long timestamp))successBlock
            error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData markUnread:conversation userId:userId index:self.cmdIndex++];
        JLogI(@"WS-Send", @"set conversation unread, type is %lu, id is %@", (unsigned long)conversation.conversationType, conversation.conversationId);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)getFirstUnreadMessage:(JConversation *)conversation
                      success:(void (^)(NSArray<JConcreteMessage *> *messages, BOOL isFinished))successBlock
                        error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData qryFirstUnreadMessage:conversation index:self.cmdIndex++];
        JLogI(@"WS-Send", @"get first unread message, type is %lu, id is %@", (unsigned long)conversation.conversationType, conversation.conversationId);
        JQryHisMsgsObj *obj = [[JQryHisMsgsObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)setAttributes:(NSDictionary<NSString *,NSString *> *)attributes
          forChatroom:(NSString *)chatroomId
             complete:(void (^)(JErrorCodeInternal, NSArray <JChatroomAttributeItem *> *))completeBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData setAttributes:attributes
                                   forChatroom:chatroomId
                                         index:self.cmdIndex++];
        JLogI(@"WS-Send", @"set attributes for chatroom %@", chatroomId);
        JUpdateChatroomAttrObj *obj = [[JUpdateChatroomAttrObj alloc] init];
        obj.completeBlock = completeBlock;
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            JLogE(@"WS-Send", @"send data error, description is %@", err.description);
            if (completeBlock) {
                completeBlock(JErrorCodeInternalWebSocketFailure, nil);
            }
        } else {
            [self.commandManager setBlockObject:obj forKey:key];
        }
    });
}

- (void)removeAttributes:(NSArray<NSString *> *)keys
             forChatroom:(NSString *)chatroomId
                complete:(void (^)(JErrorCodeInternal, NSArray<JChatroomAttributeItem *> *))completeBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData removeAttributes:keys
                                      forChatroom:chatroomId
                                            index:self.cmdIndex++];
        JLogI(@"WS-Send", @"remove attributes for chatroom %@", chatroomId);
        JUpdateChatroomAttrObj *obj = [[JUpdateChatroomAttrObj alloc] init];
        obj.completeBlock = completeBlock;
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            JLogE(@"WS-Send", @"send data error, description is %@", err.description);
            if (completeBlock) {
                completeBlock(JErrorCodeInternalWebSocketFailure, nil);
            }
        } else {
            [self.commandManager setBlockObject:obj forKey:key];
        }
    });
}

- (void)getMergedMessageList:(NSString *)messageId
                        time:(long long)timestamp
                       count:(int)count
                   direction:(JPullDirection)direction
                     success:(void (^)(NSArray<JConcreteMessage *> * _Nonnull, BOOL isFinished))successBlock
                       error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData getMergedMessageList:messageId
                                                 time:timestamp
                                                count:count
                                            direction:direction
                                                index:self.cmdIndex++];
        JLogI(@"WS-Send", @"get merge message, id is %@", messageId);
        JQryHisMsgsObj *obj = [[JQryHisMsgsObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)getMentionMessages:(JConversation *)conversation
                      time:(long long)time
                     count:(int)count
                 direction:(JPullDirection)direction
             lastReadIndex:(long long)lastReadIndex
                   success:(void (^)(NSArray<JConcreteMessage *> *messages, BOOL isFinished))successBlock
                     error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData getMentionMessages:conversation
                                               time:time
                                              count:count
                                          direction:direction
                                      lastReadIndex:lastReadIndex
                                              index:self.cmdIndex++];
        JLogI(@"WS-Send", @"get mention message");
        JQryHisMsgsObj *obj = [[JQryHisMsgsObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)registerPushToken:(NSString *)token
                   userId:(NSString *)userId
                  success:(void (^)(void))successBlock
                    error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData registerPushToken:token
                                          deviceId:[JUtility getDeviceId]
                                       packageName:[[NSBundle mainBundle] bundleIdentifier]
                                            userId:userId
                                             index:self.cmdIndex++];
        JLogI(@"WS-Send", @"register push token");
        [self simpleSendData:d
                         key:key
                     success:successBlock
                       error:errorBlock];
    });
}

- (void)registerVoIPToken:(NSString *)token
                   userId:(NSString *)userId
                  success:(void (^)(void))successBlock
                    error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData registerVoIPToken:token
                                          deviceId:[JUtility getDeviceId]
                                       packageName:[[NSBundle mainBundle] bundleIdentifier]
                                            userId:userId
                                             index:self.cmdIndex++];
        JLogI(@"WS-Send", @"register voip token");
        [self simpleSendData:d
                         key:key
                     success:successBlock
                       error:errorBlock];
    });
}

- (void)clearTotalUnreadCount:(NSString *)userId
                         time:(long long)time
                      success:(void (^)(long long timestamp))successBlock
                        error:(void (^)(JErrorCodeInternal code))errorBlock{
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData clearTotalUnreadCountMessages:userId
                                                          time:time
                                                         index:self.cmdIndex++];
        JLogI(@"WS-Send", @"clear total unread, time is %lld", time);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
    
}

- (void)deleteMessage:(JConversation *)conversation
              msgList:(NSArray <JConcreteMessage *> *)msgList
              success:(void (^)(long long timestamp))successBlock
                error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData deleteMessage:conversation
                                       msgList:msgList
                                         index:self.cmdIndex++];
        JLogI(@"WS-Send", @"delete message");
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}


- (void)clearHistoryMessage:(JConversation *)conversation
                       time:(long long)time
                    success:(void (^)(long long timestamp))successBlock
                      error:(void (^)(JErrorCodeInternal code))errorBlock{
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData clearHistoryMessage:conversation
                                                time:time
                                               scope:0
                                               index:self.cmdIndex++];
        JLogI(@"WS-Send", @"clear history message, type is %lu, id is %@", (unsigned long)conversation.conversationType, conversation.conversationId);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)createConversationInfo:(JConversation *)conversation
                        userId:(NSString *)userId
                       success:(void (^)(JConcreteConversationInfo * conversationInfo, long long timestamp))successBlock
                         error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData createConversationInfo:conversation userId:userId index:self.cmdIndex++];
        JLogI(@"WS-Send", @"create conversation, type is %lu, id is %@", (unsigned long)conversation.conversationType, conversation.conversationId);
        JConversationObj *obj = [[JConversationObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)setGlobalMute:(BOOL)isMute
               userId:(NSString *)userId
             timezone:(NSString *)timezone
              periods:(NSArray<JTimePeriod *> *)periods
              success:(void (^)(long long))successBlock
                error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData setGlobalMute:isMute
                                        userId:userId
                                      timezone:timezone
                                       periods:periods
                                         index:self.cmdIndex++];
        JLogI(@"WS-Send", @"set global mute, isMute is %d", isMute);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)getGlobalMute:(NSString *)userId
              success:(void (^)(BOOL, NSString * _Nonnull, NSArray<JTimePeriod *> * _Nonnull))successBlock
                error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData getGlobalMute:userId index:self.cmdIndex++];
        JLogI(@"WS-Send", @"get global mute");
        JGlobalMuteObj *obj = [[JGlobalMuteObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)joinChatroom:(NSString *)chatroomId
        isAutoCreate:(BOOL)isAutoCreate
             success:(void (^)(long long))successBlock
               error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData joinChatroom:chatroomId
                                 isAutoCreate:isAutoCreate
                                        index:self.cmdIndex++];
        JLogI(@"WS-Send", @"join chatroom, id is %@", chatroomId);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)quitChatroom:(NSString *)chatroomId
             success:(void (^)(long long))successBlock
               error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData quitChatroom:chatroomId index:self.cmdIndex++];
        JLogI(@"WS-Send", @"quit chatroom, id is %@", chatroomId);
        [self timestampSendData:d
                            key:key
                        success:successBlock
                          error:errorBlock];
    });
}

- (void)pushSwitch:(BOOL)enablePush userId:(NSString *)userId {
    dispatch_async(self.sendQueue, ^{
        NSData *d = [self.pbData pushSwitch:enablePush
                                     userId:userId
                                      index:self.cmdIndex++];
        JLogI(@"WS-Send", @"push switch, enable is %d", enablePush);
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            JLogE(@"WS-Send", @"push switch error, msg is %@", err.description);
        }
    });
}

- (void)uploadLogStatus:(int)result
                 userId:(NSString *)userId
              messageId:(NSString *)messageId
                    url:(NSString *)url {
    dispatch_async(self.sendQueue, ^{
       NSData *d = [self.pbData uploadLogStatus:result
                                         userId:userId
                                      messageId:messageId
                                            url:url
                                          index:self.cmdIndex++];
        JLogI(@"WS-Send", @"upload log status, result is %d", result);
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            JLogE(@"WS-Send", @"upload log status error, msg is %@", err.description);
        }
    });
}

- (void)syncChatroomMessagesWithTime:(long long)syncTime
                          chatroomId:(NSString *)chatroomId
                    prevMessageCount:(int)count {
    dispatch_async(self.sendQueue, ^{
        NSData *d = [self.pbData syncChatroomMessages:syncTime
                                           chatroomId:chatroomId
                                     prevMessageCount:count
                                                index:self.cmdIndex++];
        JLogI(@"WS-Send", @"sync chatroom messages, id is %@, time is %lld, prevMessageCount is %d", chatroomId, syncTime, count);
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            JLogE(@"WS-Send", @"sync chatroom message error, msg is %@", err.description);
        }
    });
}

- (void)syncChatroomAttributesWithTime:(long long)syncTime chatroomId:(NSString *)chatroomId {
    dispatch_async(self.sendQueue, ^{
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData syncChatroomAttributes:syncTime
                                             chatroomId:chatroomId
                                                  index:self.cmdIndex++];
        JLogI(@"WS-Send", @"sync chatroom attributes, id is %@, time is %lld", chatroomId, syncTime);
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            JLogE(@"WS-Send", @"sync chatroom attributes error, msg is %@", err.description);
        } else {
            JChatroomObj *obj = [[JChatroomObj alloc] init];
            obj.chatroomId = chatroomId;
            [self.commandManager setBlockObject:obj forKey:key];
        }
    });
}

- (void)sendPing {
    dispatch_async(self.sendQueue, ^{
        NSData *d = [self.pbData pingData];
        NSError *err = nil;
        JLogV(@"WS-Send", @"ping");
        [self.sws sendData:d error:&err];
        if (err != nil) {
            JLogE(@"WS-Send", @"ping error, msg is %@", err.description);
        }
    });
}

- (void)sendPublishAck:(int)index {
    dispatch_async(self.sendQueue, ^{
        NSData *d = [self.pbData publishAckData:index];
        NSError *err = nil;
        JLogV(@"WS-Send", @"publish ack");
        [self.sws sendData:d error:&err];
        if (err != nil) {
            JLogE(@"WS-Send", @"publish ack error, msg is %@", err.description);
        }
    });
}

- (void)getUploadFileCred:(NSString *)userId
                 fileType:(JUploadFileType)fileType
                      ext:(NSString *)ext
                  success:(void (^)(JUploadOssType ossType, JUploadQiNiuCred * qiNiuCred, JUploadPreSignCred * preSignCred))successBlock
                    error:(void (^)(JErrorCodeInternal code))errorBlock {
    dispatch_async(self.sendQueue, ^{
        JLogI(@"WS-Send", @"get upload cred");
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData getUploadFileCred:userId fileType:fileType ext:ext index:self.cmdIndex++];
        JUploadFileCredBlockObj *obj = [[JUploadFileCredBlockObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)callInvite:(NSString *)callId
       isMultiCall:(BOOL)isMultiCall
         mediaType:(JCallMediaType)mediaType
      targetIdList:(NSArray<NSString *> *)userIdList
        engineType:(NSUInteger)engineType
           success:(nonnull void (^)(NSString *))successBlock
             error:(nonnull void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        JLogI(@"WS-Send", @"call invite, callId is %@, isMultiCall is %d", callId, isMultiCall);
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData callInvite:callId
                                isMultiCall:isMultiCall
                                  mediaType:mediaType
                               targetIdList:userIdList
                                 engineType:engineType
                                      index:self.cmdIndex++];
        JCallAuthObj *obj = [[JCallAuthObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)callHangup:(NSString *)callId
           success:(void (^)(void))successBlock
             error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        JLogI(@"WS-Send", @"call hangup, callId is %@", callId);
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData callHangup:callId
                                      index:self.cmdIndex++];
        [self simpleSendData:d
                         key:key
                     success:successBlock
                       error:errorBlock];
    });
}

- (void)callAccept:(NSString *)callId
           success:(void (^)(NSString * _Nonnull))successBlock
             error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        JLogI(@"WS-Send", @"call accept, callId is %@", callId);
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData callAccept:callId
                                      index:self.cmdIndex++];
        JCallAuthObj *obj = [[JCallAuthObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)callConnected:(NSString *)callId
              success:(void (^)(void))successBlock
                error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        JLogI(@"WS-Send", @"call connected, callId is %@", callId);
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData callConnected:callId index:self.cmdIndex++];
        [self simpleSendData:d
                         key:key
                     success:successBlock
                       error:errorBlock];
    });
}

- (void)queryCallRooms:(NSString *)userId
               success:(void (^)(NSArray <JRtcRoom *> *))successBlock
                 error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        JLogI(@"WS-Send", @"query call rooms");
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData queryCallRooms:userId index:self.cmdIndex++];
        JRtcRoomArrayObj *obj = [[JRtcRoomArrayObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)queryCallRoom:(NSString *)roomId
              success:(void (^)(NSArray<JRtcRoom *> * _Nonnull))successBlock
                error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        JLogI(@"WS-Send", @"query call room");
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData queryCallRoom:roomId index:self.cmdIndex++];
        JRtcRoomArrayObj *obj = [[JRtcRoomArrayObj alloc] init];
        obj.successBlock = successBlock;
        obj.errorBlock = errorBlock;
        [self sendData:d
                   key:key
                   obj:obj
                 error:errorBlock];
    });
}

- (void)setLanguage:(NSString *)language
             userId:(NSString *)userId
            success:(void (^)(void))successBlock
              error:(void (^)(JErrorCodeInternal))errorBlock {
    dispatch_async(self.sendQueue, ^{
        JLogI(@"WS-Send", @"set language");
        NSNumber *key = @(self.cmdIndex);
        NSData *d = [self.pbData setLanguage:language
                                      userId:userId
                                       index:self.cmdIndex++];
        [self simpleSendData:d
                         key:key
                     success:successBlock
                       error:errorBlock];
    });
}

- (void)rtcPing:(NSString *)callId {
    dispatch_async(self.sendQueue, ^{
        JLogV(@"WS-Send", @"rtc ping");
        NSData *d = [self.pbData rtcPingData:callId index:self.cmdIndex++];
        NSError *err = nil;
        [self.sws sendData:d error:&err];
        if (err != nil) {
            JLogE(@"WS-Send", @"rtc ping error, msg is %@", err.description);
        }
    });
}

#pragma mark - SRWebSocketDelegate
- (void)webSocketDidOpen:(SRWebSocket *)webSocket {
    dispatch_async(self.sendQueue, ^{
        if (self.isCompeteFinish) {
            JLogI(@"WS-Connect", @"compete fail, url is %@", webSocket.url);
            [webSocket close];
            return;
        }
        //防止上一批竞速的 webSocket 被选中
        for (int i = 0; i < self.competeSwsList.count; i++) {
            SRWebSocket *sws = self.competeSwsList[i];
            if (webSocket == sws) {
                JLogI(@"WS-Connect", @"compete success, url is %@", webSocket.url);
                self.isCompeteFinish = YES;
                self.competeStatusList[i] = @(JWebSocketStatusSuccess);
                self.sws = webSocket;
                [self sendConnectMsgByWebSocket:webSocket];
                break;
            }
        }
    });
}

- (void)webSocket:(SRWebSocket *)webSocket didFailWithError:(NSError *)error {
    dispatch_async(self.sendQueue, ^{
        if (self.isCompeteFinish) {
            if (webSocket != self.sws) {
                return;
            }
            JLogI(@"WS-Connect", @"isCompeteFinish, fail message is %@", error.description);
            [self resetSws];
            if ([self.connectDelegate respondsToSelector:@selector(webSocketDidFail)]) {
                [self.connectDelegate webSocketDidFail];
            }
        } else {
            for (int i = 0; i < self.competeSwsList.count; i++) {
                SRWebSocket *sws = self.competeSwsList[i];
                if (webSocket == sws) {
                    self.competeStatusList[i] = @(JWebSocketStatusFailure);
                    break;
                }
            }
            BOOL allFailed = YES;
            for (NSNumber *status in self.competeStatusList) {
                if (status.unsignedIntValue != JWebSocketStatusFailure) {
                    allFailed = NO;
                    break;
                }
            }
            if (allFailed && [self.connectDelegate respondsToSelector:@selector(webSocketDidFail)]) {
                JLogI(@"WS-Connect", @"fail message is %@", error.description);
                [self resetSws];
                [self.connectDelegate webSocketDidFail];
            }
        }
    });
}

- (void)webSocket:(SRWebSocket *)webSocket didCloseWithCode:(NSInteger)code reason:(NSString *)reason wasClean:(BOOL)wasClean {
    if (webSocket != self.sws) {
        return;
    }
    JLogI(@"WS-Connect", @"close code is %ld, reason is %@", (long)code, reason);
    dispatch_async(self.sendQueue, ^{
        [self resetSws];
        if ([self.connectDelegate respondsToSelector:@selector(webSocketDidClose)]) {
            [self.connectDelegate webSocketDidClose];
        }
    });
}

- (void)webSocket:(SRWebSocket *)webSocket didReceiveMessageWithData:(NSData *)data {
    if (webSocket != self.sws) {
        return;
    }
    [self.heartbeatManager updateLastMessageReceiveTime];
    JPBRcvObj *obj = [self.pbData rcvObjWithData:data];
    switch (obj.rcvType) {
        case JPBRcvTypeParseError:
            JLogI(@"WS-Receive", @"JPBRcvTypeParseError");
            break;
        case JPBRcvTypeConnectAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeConnectAck");
            [self handleConnectAckMsg:obj.connectAck];
            break;
        case JPBRcvTypePublishMsgAck:
            JLogI(@"WS-Receive", @"JPBRcvTypePublishMsgAck");
            [self handlePublishAckMsg:obj.publishMsgAck];
            break;
        case JPBRcvTypeQryHisMsgsAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeQryHisMsgsAck");
            [self handleQryHisMsgs:obj.qryHisMsgsAck];
            break;
        case JPBRcvTypeSyncConvsAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeSyncConvsAck");
            [self handleSyncConvsAck:obj.syncConvsAck];
            break;
        case JPBRcvTypePublishMsg:
            JLogI(@"WS-Receive", @"JPBRcvTypePublishMsg");
            [self handleReceiveMessage:obj.publishMsgBody];
            break;
        case JPBRcvTypePublishMsgNtf:
            JLogI(@"WS-Receive", @"JPBRcvTypePublishMsgNtf");
            [self handlePublishMsgNtf:obj.publishMsgNtf];
            break;
        case JPBRcvTypePublishChatroomMsgNtf:
            JLogI(@"WS-Receive", @"JPBRcvTypePublishChatroomMsgNtf");
            [self handlePublishChatroomMsgNtf:obj.publishMsgNtf];
            break;
        case JPBRcvTypePublishChatroomAttrNtf:
            JLogI(@"WS-Receive", @"JPBRcvTypePublishChatroomAttrNtf");
            [self handlePublishChatroomAttrNtf:obj.publishMsgNtf];
            break;
        case JPBRcvTypeSyncMsgsAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeSyncMsgsAck");
            [self handleSyncMsgsAck:obj.qryHisMsgsAck];
            break;
        case JPBRcvTypeSyncChatroomMsgsAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeSyncChatroomMsgsAck");
            [self handleSyncChatroomMsgsAck:obj.qryHisMsgsAck];
            break;
        case JPBRcvTypePong:
            JLogV(@"WS-Receive", @"JPBRcvTypePong");
            [self handlePong];
            break;
        case JPBRcvTypeDisconnectMsg:
            JLogI(@"WS-Receive", @"JPBRcvTypeDisconnectMsg");
            [self handleDisconnectMsg:obj.disconnectMsg];
            break;
        case JPBRcvTypeSimpleQryAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeSimpleQryAck");
            [self handleSimpleAck:obj.simpleQryAck];
            break;
        case JPBRcvTypeQryReadDetailAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeQryReadDetailAck");
            [self handleQryReadDetailAck:obj.qryReadDetailAck];
            break;
        case JPBRcvTypeSimpleQryAckCallbackTimestamp:
            JLogI(@"WS-Receive", @"JPBRcvTypeSimpleQryAckCallbackTimestamp");
            [self handleSimpleQryAckWithTimeCallback:obj.simpleQryAck];
            break;
        case JPBRcvTypeAddConversation:
            JLogI(@"WS-Receive", @"JPBRcvTypeAddConversation");
            [self handleConversationAck:obj.conversationInfoAck];
            break;
        case JPBRcvTypeFileCredMsgAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeFileCredMsgAck");
            [self handleUploadFileCredCallback:obj.qryFileCredAck];
            break;
        case JPBRcvTypeGlobalMuteAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeGlobalMuteAck");
            [self handleGlobalMuteAck:obj.globalMuteAck];
            break;
        case JPBRcvTypeQryFirstUnreadMsgAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeQryFirstUnreadMsgAck");
            [self handleFirstUnreadMsgAck:obj.qryHisMsgsAck];
            break;
        case JPBRcvTypeSetChatroomAttrAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeSetChatroomAttrAck");
            [self handleChatroomAttrAck:obj.chatroomAttrsAck];
            break;
        case JPBRcvTypeRemoveChatroomAttrAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeRemoveChatroomAttrAck");
            [self handleChatroomAttrAck:obj.chatroomAttrsAck];
            break;
        case JPBRcvTypeSyncChatroomAttrsAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeSyncChatroomAttrsAck");
            [self handleSyncChatroomAttrAck:obj.chatroomAttrsAck];
            break;
        case JPBRcvTypeChatroomDestroyNtf:
            JLogI(@"WS-Receive", @"JPBRcvTypeChatroomDestroyNtf");
            [self handleChatroomDestroyNtf:obj.publishMsgNtf];
            break;
        case JPBRcvTypeChatroomEventNtf:
            JLogI(@"WS-Receive", @"JPBRcvTypeChatroomEventNtf");
            [self handleChatroomEventNtf:obj.publishMsgNtf];
            break;
        case JPBRcvTypeRtcRoomEventNtf:
            JLogI(@"WS-Receive", @"JPBRcvTypeRtcRoomEventNtf");
            [self handleRtcRoomEventNtf:obj.rtcRoomEventNtf];
            break;
        case JPBRcvTypeRtcInviteEventNtf:
            JLogI(@"WS-Receive", @"JPBRcvTypeRtcInviteEventNtf");
            [self handleRtcInviteEventNtf:obj.rtcInviteEventNtf];
            break;
        case JPBRcvTypeCallAuthAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeCallAuthAck");
            [self handleRtcInviteAck:obj.callInviteAck];
            break;
        case JPBRcvTypeQryCallRoomsAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeQryCallRoomsAck");
            [self handleRtcQryCallRoomsAck:obj.rtcQryCallRoomsAck];
            break;
        case JPBRcvTypeQryCallRoomAck:
            JLogI(@"WS-Receive", @"JPBRcvTypeQryCallRoomAck");
            //复用 rtcQryCallRoomsAck
            [self handleRtcQryCallRoomsAck:obj.rtcQryCallRoomsAck];
            break;
        default:
            JLogI(@"WS-Receive", @"default, type is %lu", (unsigned long)obj.rcvType);
            break;
    }
}

#pragma mark - JCommandTimeOutDelegate
- (void)commandDidTimeOut:(NSArray<JBlockObj *> *)objs {
    for (JBlockObj *obj in objs) {
        [self command:obj error:JErrorCodeInternalOperationTimeOut];
    }
}

#pragma mark - inner
- (void)command:(JBlockObj *)obj
          error:(JErrorCodeInternal)code {
    if ([obj isKindOfClass:[JSendMessageObj class]]) {
        JSendMessageObj *s = (JSendMessageObj *)obj;
        s.errorBlock(code, s.clientMsgNo);
    } else if ([obj isKindOfClass:[JQryHisMsgsObj class]]) {
        JQryHisMsgsObj *s = (JQryHisMsgsObj *)obj;
        s.errorBlock(code);
    } else if ([obj isKindOfClass:[JSyncConvsObj class]]) {
        JSyncConvsObj *s = (JSyncConvsObj *)obj;
        s.errorBlock(code);
    } else if ([obj isKindOfClass:[JTimestampBlockObj class]]) {
        JTimestampBlockObj *s = (JTimestampBlockObj *)obj;
        s.errorBlock(code);
    } else if ([obj isKindOfClass:[JQryReadDetailObj class]]) {
        JQryReadDetailObj *s = (JQryReadDetailObj *)obj;
        s.errorBlock(code);
    } else if ([obj isKindOfClass:[JSimpleBlockObj class]]) {
        JSimpleBlockObj *s = (JSimpleBlockObj *)obj;
        s.errorBlock(code);
    } else if ([obj isKindOfClass:[JConversationObj class]]) {
        JConversationObj *s = (JConversationObj *)obj;
        s.errorBlock(code);
    } else if ([obj isKindOfClass:[JUploadFileCredBlockObj class]]) {
        JUploadFileCredBlockObj *s = (JUploadFileCredBlockObj *)obj;
        s.errorBlock(code);
    } else if ([obj isKindOfClass:[JGlobalMuteObj class]]) {
        JGlobalMuteObj *s = (JGlobalMuteObj *)obj;
        s.errorBlock(code);
    } else if ([obj isKindOfClass:[JUpdateChatroomAttrObj class]]) {
        JUpdateChatroomAttrObj *s = (JUpdateChatroomAttrObj *)obj;
        s.completeBlock(code, nil);
    } else if ([obj isKindOfClass:[JCallAuthObj class]]) {
        JCallAuthObj *s = (JCallAuthObj *)obj;
        s.errorBlock(code);
    } else if ([obj isKindOfClass:[JRtcRoomArrayObj class]]) {
        JRtcRoomArrayObj *s = (JRtcRoomArrayObj *)obj;
        s.errorBlock(code);
    }
}

- (void)sendConnectMsgByWebSocket:(SRWebSocket *)sws {
    [self.pbData resetDataConverter];
    NSData *d = [self.pbData connectDataWithAppKey:self.appKey
                                             token:self.token
                                          deviceId:[JUtility getDeviceId]
                                          platform:JPlatform
                                     deviceCompany:JDeviceCompany
                                       deviceModel:[JUtility currentDeviceModel]
                                   deviceOsVersion:[JUtility currentSystemVersion]
                                       packageName:[[NSBundle mainBundle] bundleIdentifier]
                                         pushToken:self.pushToken
                                         voipToken:self.voipToken
                                         networkId:[JUtility currentNetWork]
                                            ispNum:[JUtility currentCarrier]
                                          clientIp:@""
                                          language:[JUtility getSystemLanguage]];
    NSError *err = nil;
    [sws sendData:d error:&err];
    if (err != nil) {
        JLogE(@"WS-Connect", @"send connect error, msg is %@", err.description);
    }
}

- (void)sendDisconnectMsgByWebSocket:(BOOL)needPush {
    NSData *d = [self.pbData disconnectData:needPush];
    NSError *err = nil;
    [self.sws sendData:d error:&err];
    if (err != nil) {
        JLogE(@"WS-Send", @"send disconnect error, msg is %@", err.description);
    }
    [self resetSws];
}

- (void)handleConnectAckMsg:(JConnectAck *)connectAck {
    if ([self.connectDelegate respondsToSelector:@selector(connectCompleteWithCode:userId:session:extra:)]) {
        [self.connectDelegate connectCompleteWithCode:connectAck.code
                                               userId:connectAck.userId
                                              session:connectAck.session
                                                extra:connectAck.extra];
    }
}

- (void)handlePublishAckMsg:(JPublishMsgAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    //超时回调之后拿不到 obj，通知 message 更新状态
    if (!obj && ack.code == 0) {
        [self.messageDelegate messageDidSend:ack.msgId
                                        time:ack.timestamp
                                       seqNo:ack.seqNo
                                   clientUid:ack.clientUid
                                 contentType:ack.contentType
                                     content:ack.content];
        return;
    }
    if ([obj isKindOfClass:[JSendMessageObj class]]) {
        JSendMessageObj *sendMessageObj = (JSendMessageObj *)obj;
        if (ack.code != 0) {
            sendMessageObj.errorBlock(ack.code, sendMessageObj.clientMsgNo);
        } else {
            sendMessageObj.successBlock(sendMessageObj.clientMsgNo, ack.msgId, ack.timestamp, ack.seqNo, ack.contentType, ack.content);
        }
    }
}

- (void)handleQryHisMsgs:(JQryHisMsgsAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JQryHisMsgsObj class]]) {
        JQryHisMsgsObj *qryHisMsgsObj = (JQryHisMsgsObj *)obj;
        if (ack.code != 0) {
            qryHisMsgsObj.errorBlock(ack.code);
        } else {
            qryHisMsgsObj.successBlock(ack.msgs, ack.isFinished);
        }
    }
}

- (void)handleSyncConvsAck:(JSyncConvsAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JSyncConvsObj class]]) {
        JSyncConvsObj *syncConvsObj = (JSyncConvsObj *)obj;
        if (ack.code != 0) {
            syncConvsObj.errorBlock(ack.code);
        } else {
            syncConvsObj.successBlock(ack.convs, ack.deletedConvs, ack.isFinished);
        }
    }
}

//sync 和 queryHisMsgs 共用一个 ack
- (void)handleSyncMsgsAck:(JQryHisMsgsAck *)ack {
    if ([self.messageDelegate respondsToSelector:@selector(messagesDidReceive:isFinished:)]) {
        [self.messageDelegate messagesDidReceive:ack.msgs isFinished:ack.isFinished];
    }
}

- (void)handleSyncChatroomMsgsAck:(JQryHisMsgsAck *)ack {
    if ([self.messageDelegate respondsToSelector:@selector(chatroomMessagesDidReceive:)]) {
        [self.messageDelegate chatroomMessagesDidReceive:ack.msgs];
    }
}

- (void)handleReceiveMessage:(JPublishMsgBody *)publishMsgBody {
    BOOL needAck = NO;
    if ([self.messageDelegate respondsToSelector:@selector(messageDidReceive:)]) {
        needAck = [self.messageDelegate messageDidReceive:publishMsgBody.rcvMessage];
    }
    if (publishMsgBody.qos == 1 && needAck) {
        [self sendPublishAck:publishMsgBody.index];
    }
}

- (void)handlePublishMsgNtf:(JPublishMsgNtf *)ntf {
    if ([self.messageDelegate respondsToSelector:@selector(syncNotify:)]) {
        [self.messageDelegate syncNotify:ntf.syncTime];
    }
}

- (void)handlePublishChatroomMsgNtf:(JPublishMsgNtf *)ntf {
    if ([self.messageDelegate respondsToSelector:@selector(syncChatroomNotify:time:)]) {
        [self.messageDelegate syncChatroomNotify:ntf.chatroomId
                                            time:ntf.syncTime];
    }
}

- (void)handlePublishChatroomAttrNtf:(JPublishMsgNtf *)ntf {
    if ([self.chatroomDelegate respondsToSelector:@selector(syncChatroomAttrNotify:time:)]) {
        [self.chatroomDelegate syncChatroomAttrNotify:ntf.chatroomId
                                                 time:ntf.syncTime];
    }
}

- (void)handlePong {
}

- (void)handleDisconnectMsg:(JDisconnectMsg *)msg {
    dispatch_async(self.sendQueue, ^{
        [self resetSws];
        if ([self.connectDelegate respondsToSelector:@selector(disconnectWithCode:extra:)]) {
            [self.connectDelegate disconnectWithCode:msg.code extra:msg.extra];
        }
    });
}

- (void)handleSimpleAck:(JSimpleQryAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JSimpleBlockObj class]]) {
        JSimpleBlockObj *simpleObj = (JSimpleBlockObj *)obj;
        if (ack.code != 0) {
            simpleObj.errorBlock(ack.code);
        } else {
            simpleObj.successBlock();
        }
    }
}

- (void)handleSimpleQryAckWithTimeCallback:(JSimpleQryAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JTimestampBlockObj class]]) {
        JTimestampBlockObj *simpleObj = (JTimestampBlockObj *)obj;
        if (ack.code != 0) {
            simpleObj.errorBlock(ack.code);
        } else {
            simpleObj.successBlock(ack.timestamp);
        }
    }
}

- (void)handleConversationAck:(JConversationInfoAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JConversationObj class]]) {
        JConversationObj *conversationObj = (JConversationObj *)obj;
        if (ack.code != 0) {
            conversationObj.errorBlock(ack.code);
        } else {
            conversationObj.successBlock(ack.conversationInfo,ack.timestamp);
        }
    }
}

- (void)handleQryReadDetailAck:(JQryReadDetailAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JQryReadDetailObj class]]) {
        JQryReadDetailObj *qryReadDetailObj = (JQryReadDetailObj *)obj;
        if (ack.code != 0) {
            qryReadDetailObj.errorBlock(ack.code);
        } else {
            qryReadDetailObj.successBlock(ack.readMembers, ack.unreadMembers);
        }
    }
}

-(void)handleUploadFileCredCallback:(JQryFileCredAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JUploadFileCredBlockObj class]]) {
        JUploadFileCredBlockObj *fileCredBlockObj = (JUploadFileCredBlockObj *)obj;
        if (ack.code != 0) {
            fileCredBlockObj.errorBlock(ack.code);
        } else {
            fileCredBlockObj.successBlock(ack.ossType, ack.qiNiuCred, ack.preSignCred);
        }
    }
}

- (void)handleGlobalMuteAck:(JGlobalMuteAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JGlobalMuteObj class]]) {
        JGlobalMuteObj *globalMuteObj = (JGlobalMuteObj *)obj;
        if (ack.code != 0) {
            globalMuteObj.errorBlock(ack.code);
        } else {
            globalMuteObj.successBlock(ack.isMute, ack.timezone, ack.periods);
        }
    }
}

- (void)handleFirstUnreadMsgAck:(JQryHisMsgsAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JQryHisMsgsObj class]]) {
        JQryHisMsgsObj *qryHisMsgsObj = (JQryHisMsgsObj *)obj;
        if (ack.code != 0) {
            qryHisMsgsObj.errorBlock(ack.code);
        } else {
            qryHisMsgsObj.successBlock(ack.msgs, ack.isFinished);
        }
    }
}

- (void)handleChatroomAttrAck:(JChatroomAttrsAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JUpdateChatroomAttrObj class]]) {
        JUpdateChatroomAttrObj *updateChatroomAttrObj = (JUpdateChatroomAttrObj *)obj;
        updateChatroomAttrObj.completeBlock(ack.code, ack.items);
    }
}

- (void)handleSyncChatroomAttrAck:(JChatroomAttrsAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JChatroomObj class]]) {
        JChatroomObj *chatroomObj = (JChatroomObj *)obj;
        if ([self.chatroomDelegate respondsToSelector:@selector(attributesDidSync:forChatroom:code:)])  {
            [self.chatroomDelegate attributesDidSync:ack.items
                                         forChatroom:chatroomObj.chatroomId
                                                code:ack.code];
        }
    }
}

- (void)handleChatroomDestroyNtf:(JPublishMsgNtf *)ntf {
    if ([self.chatroomDelegate respondsToSelector:@selector(chatroomDidDestroy:)]) {
        [self.chatroomDelegate chatroomDidDestroy:ntf.chatroomId];
    }
}

- (void)handleChatroomEventNtf:(JPublishMsgNtf *)ntf {
    if (ntf.type == JPBChrmEventTypeQuit || ntf.type == JPBChrmEventTypeFallout) {
        if ([self.chatroomDelegate respondsToSelector:@selector(chatroomDidQuit:)]) {
            [self.chatroomDelegate chatroomDidQuit:ntf.chatroomId];
        }
    } else if (ntf.type == JPBChrmEventTypeKick) {
        if ([self.chatroomDelegate respondsToSelector:@selector(chatroomDidKick:)]) {
            [self.chatroomDelegate chatroomDidKick:ntf.chatroomId];
        }
    }
}

- (void)handleRtcRoomEventNtf:(JRtcRoomEventNtf *)ntf {
    JLogI(@"Call-RmEvent", @"type is %ld", ntf.eventType);
    switch (ntf.eventType) {
        case JPBRtcRoomEventTypeQuit:
            if ([self.callDelegate respondsToSelector:@selector(callDidQuit:members:)]) {
                [self.callDelegate callDidQuit:ntf.room members:ntf.members];
            }
            break;
            
        case JPBRtcRoomEventTypeDestroy:
            if ([self.callDelegate respondsToSelector:@selector(roomDidDestroy:)]) {
                [self.callDelegate roomDidDestroy:ntf.room];
            }
            break;
            
        //TODO: join 只在用户主动加入房间时触发，暂未开发；statechange 只在用户 connecting 变成 connected 时触发，暂不处理
            
        default:
            break;
    }
}

- (void)handleRtcInviteEventNtf:(JRtcInviteEventNtf *)ntf {
    switch (ntf.type) {
        case JPBRtcInviteTypeInvite:
            if ([self.callDelegate respondsToSelector:@selector(callDidInvite:inviter:targetUsers:)]) {
                [self.callDelegate callDidInvite:ntf.room
                                         inviter:ntf.user
                                     targetUsers:ntf.targetUsers];
            }
            break;
            
        case JPBRtcInviteTypeHangup:
            if ([self.callDelegate respondsToSelector:@selector(callDidHangup:user:)]) {
                [self.callDelegate callDidHangup:ntf.room
                                            user:ntf.user];
            }
            break;
            
        case JPBRtcInviteTypeAccept:
            if ([self.callDelegate respondsToSelector:@selector(callDidAccept:user:)]) {
                [self.callDelegate callDidAccept:ntf.room
                                            user:ntf.user];
            }
            break;
            
        default:
            break;
    }
}

- (void)handleRtcInviteAck:(JCallAuthAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JCallAuthObj class]]) {
        JCallAuthObj *inviteObj = (JCallAuthObj *)obj;
        if (ack.code != 0) {
            inviteObj.errorBlock(ack.code);
        } else {
            inviteObj.successBlock(ack.zegoToken);
        }
    }
}

- (void)handleRtcQryCallRoomsAck:(JRtcQryCallRoomsAck *)ack {
    JBlockObj *obj = [self.commandManager removeBlockObjectForKey:@(ack.index)];
    if ([obj isKindOfClass:[JRtcRoomArrayObj class]]) {
        JRtcRoomArrayObj *roomsObj = (JRtcRoomArrayObj *)obj;
        if (ack.code != 0) {
            roomsObj.errorBlock(ack.code);
        } else {
            roomsObj.successBlock(ack.rooms);
        }
    }
}

- (void)simpleSendData:(NSData *)data
                   key:(NSNumber *)key
               success:(void (^)(void))successBlock
                 error:(void (^)(JErrorCodeInternal code))errorBlock {
    JSimpleBlockObj *obj = [[JSimpleBlockObj alloc] init];
    obj.successBlock = successBlock;
    obj.errorBlock = errorBlock;
    [self sendData:data
               key:key
               obj:obj
             error:errorBlock];
}

- (void)timestampSendData:(NSData *)data
                      key:(NSNumber *)key
                  success:(void (^)(long long timestamp))successBlock
                    error:(void (^)(JErrorCodeInternal code))errorBlock {
    JTimestampBlockObj *obj = [[JTimestampBlockObj alloc] init];
    obj.successBlock = successBlock;
    obj.errorBlock = errorBlock;
    [self sendData:data
               key:key
               obj:obj
             error:errorBlock];
}

- (void)sendData:(NSData *)data
             key:(NSNumber *)key
             obj:(JBlockObj *)obj
           error:(void (^)(JErrorCodeInternal code))errorBlock {
    NSError *err = nil;
    [self.sws sendData:data error:&err];
    if (err != nil) {
        JLogE(@"WS-Send", @"send data error, description is %@", err.description);
        if (errorBlock) {
            errorBlock(JErrorCodeInternalWebSocketFailure);
        }
    } else {
        [self.commandManager setBlockObject:obj forKey:key];
    }
}

- (SRWebSocket *)createWebSocket:(NSString *)url {
    NSString *u;
    if ([url containsString:jProtocolHead]) {
        u = [NSString stringWithFormat:@"%@%@", url, jWebSocketSuffix];
    } else {
        u = [NSString stringWithFormat:@"%@%@%@", jWSPrefix, url, jWebSocketSuffix];
    }
    SRWebSocket *sws = [[SRWebSocket alloc] initWithURLRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:u]]];
    sws.delegateDispatchQueue = self.receiveQueue;
    sws.delegate = self;
    return sws;
}

- (void)resetSws {
    self.sws = nil;
    [self.competeSwsList removeAllObjects];
    [self.competeStatusList removeAllObjects];
    self.isCompeteFinish = NO;
}

#pragma mark - getter
- (NSMutableArray<SRWebSocket *> *)competeSwsList {
    if (!_competeSwsList) {
        _competeSwsList = [NSMutableArray array];
    }
    return _competeSwsList;
}

- (NSMutableArray<NSNumber *> *)competeStatusList {
    if (!_competeStatusList) {
        _competeStatusList = [NSMutableArray array];
    }
    return _competeStatusList;
}
@end
