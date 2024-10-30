//
//  JWebSocket.h
// JuggleIM
//
//  Created by Nathan on 2023/11/29.
//

#import <Foundation/Foundation.h>
#import "JuggleIMConst.h"
#import "JuggleIMConstInternal.h"
#import "JMessageContent.h"
#import "JConversation.h"
#import "JConcreteMessage.h"
#import "JConcreteConversationInfo.h"
#import "JMergeInfo.h"
#import "JUploadEnum.h"
#import "JUploadQiNiuCred.h"
#import "JUploadPreSignCred.h"
#import "JChatroomAttributeItem.h"
#import "JPushData.h"

NS_ASSUME_NONNULL_BEGIN

@protocol JWebSocketConnectDelegate <NSObject>
- (void)connectCompleteWithCode:(JErrorCodeInternal)error
                         userId:(NSString *)userId
                        session:(NSString *)session
                          extra:(NSString *)extra;
- (void)disconnectWithCode:(JErrorCodeInternal)error
                     extra:(NSString *)extra;
- (void)webSocketDidFail;
- (void)webSocketDidClose;
- (void)webSocketDidTimeOut;
@end

@protocol JWebSocketMessageDelegate <NSObject>
- (BOOL)messageDidReceive:(JConcreteMessage *)message;
- (void)messagesDidReceive:(NSArray<JConcreteMessage *> *)messages
                isFinished:(BOOL)isFinished;
- (void)chatroomMessagesDidReceive:(NSArray<JConcreteMessage *> *)messages;
- (void)syncNotify:(long long)syncTime;
- (void)syncChatroomNotify:(NSString *)chatroomId
                      time:(long long)syncTime;
- (void)messageDidSend:(NSString *)messageId
                  time:(long long)timestamp
                 seqNo:(long long)seqNo
             clientUid:(NSString *)clientUid;
@end

@protocol JWebSocketChatroomDelegate <NSObject>
- (void)syncChatroomAttrNotify:(NSString *)chatroomId
                          time:(long long)syncTime;
- (void)attributesDidSync:(NSArray <JChatroomAttributeItem *> *)items
              forChatroom:(NSString *)chatroomId
                     code:(JErrorCodeInternal)code;
- (void)chatroomDidDestroy:(NSString *)chatroomId;
- (void)chatroomDidQuit:(NSString *)chatroomId;
- (void)chatroomDidKick:(NSString *)chatroomId;
@end

@interface JWebSocket : NSObject
- (instancetype)initWithSendQueque:(dispatch_queue_t)sendQueue
                      receiveQueue:(dispatch_queue_t)receiveQueue;
- (void)connect:(NSString *)appKey
          token:(NSString *)token
      pushToken:(NSString *)pushToken
        servers:(NSArray *)servers;

- (void)disconnect:(BOOL)needPush;

- (void)startHeartbeat;

- (void)stopHeartbeat;

- (void)heartbeatTimeOut;

- (void)pushRemainCmdAndCallbackError;

- (void)setConnectDelegate:(id<JWebSocketConnectDelegate>)delegate;

- (void)setMessageDelegate:(id<JWebSocketMessageDelegate>)delegate;

- (void)setChatroomDelegate:(id<JWebSocketChatroomDelegate>)delegate;

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
              success:(void (^)(long long clientMsgNo, NSString *msgId, long long timestamp, long long reqNo))successBlock
                error:(void (^)(JErrorCodeInternal errorCode, long long clientMsgNo))errorBlock;

- (void)recallMessage:(NSString *)messageId
               extras:(NSDictionary *)extras
         conversation:(JConversation *)conversation
            timestamp:(long long)timestamp
              success:(void (^)(long long timestamp))successBlock
                error:(void (^)(JErrorCodeInternal errorCode))errorBlock;

- (void)sendReadReceipt:(NSArray <NSString *> *)messageIds
         inConversation:(JConversation *)conversation
                success:(void (^)(long long timestamp))successBlock
                  error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)syncMessagesWithReceiveTime:(long long)receiveTime
                           sendTime:(long long)sendTime
                             userId:(NSString *)userId;

- (void)queryHisMsgsFrom:(JConversation *)conversation
               startTime:(long long)startTime
                   count:(int)count
               direction:(JPullDirection)direction
            contentTypes:(nullable NSArray <NSString *> *)contentTypes
                 success:(void (^)(NSArray *messages, BOOL isFinished))successBlock
                   error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)queryHisMsgsByIds:(NSArray <NSString *> *)messageIds
           inConversation:(JConversation *)conversation
                  success:(void (^)(NSArray <JConcreteMessage *> *messages, BOOL isFinished))successBlock
                    error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)syncConversations:(long long)startTime
                    count:(int)count
                   userId:(NSString *)userId
                  success:(void (^)(NSArray *conversations, NSArray *deletedConversations, BOOL isFinished))successBlock
                    error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)deleteConversationInfo:(JConversation *)conversation
                        userId:(NSString *)userId
                       success:(void (^)(long long timestamp))successBlock
                         error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)clearUnreadCount:(JConversation *)conversation
                  userId:(NSString *)userId
                msgIndex:(long long)msgIndex
                   msgId:(NSString *)msgId
               timestamp:(long long)timestamp
                 success:(void (^)(long long timestamp))successBlock
                   error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)getGroupMessageReadDetail:(NSString *)messageId
                   inConversation:(JConversation *)conversation
                          success:(void (^)(NSArray<JUserInfo *> *readMembers, NSArray<JUserInfo *> *unreadMembers))successBlock
                            error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)setMute:(BOOL)isMute
 inConversation:(JConversation *)conversation
         userId:(NSString *)userId
        success:(void (^)(long long timestamp))successBlock
          error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)setTop:(BOOL)isTop
inConversation:(JConversation *)conversation
        userId:(NSString *)userId
       success:(void (^)(long long timestamp))successBlock
         error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)createConversationInfo:(JConversation *)conversation
                        userId:(NSString *)userId
                       success:(void (^)(JConcreteConversationInfo * conversationInfo, long long timestamp))successBlock
                         error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)getMergedMessageList:(NSString *)messageId
                        time:(long long)timestamp
                       count:(int)count
                   direction:(JPullDirection)direction
                     success:(void (^)(NSArray<JConcreteMessage *> *messages, BOOL isFinished))successBlock
                       error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)getMentionMessages:(JConversation *)conversation
                      time:(long long)time
                     count:(int)count
                 direction:(JPullDirection)direction
             lastReadIndex:(long long)lastReadIndex
                   success:(void (^)(NSArray<JConcreteMessage *> *messages, BOOL isFinished))successBlock
                     error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)registerPushToken:(NSString *)token
                   userId:(NSString *)userId
                  success:(void (^)(void))successBlock
                    error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)clearTotalUnreadCount:(NSString *)userId
                         time:(long long)time
                      success:(void (^)(long long timestamp))successBlock
                        error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)deleteMessage:(JConversation *)conversation
              msgList:(NSArray <JConcreteMessage *> *)msgList
              success:(void (^)(long long timestamp))successBlock
                error:(void (^)(JErrorCodeInternal code))errorBlock;


- (void)clearHistoryMessage:(JConversation *)conversation
                       time:(long long)time
                    success:(void (^)(long long timestamp))successBlock
                      error:(void (^)(JErrorCodeInternal code))errorBlock;


- (void)getUploadFileCred:(NSString *)userId
                 fileType:(JUploadFileType)fileType
                      ext:(NSString *)ext
                  success:(void (^)(JUploadOssType ossType, JUploadQiNiuCred * qiNiuCred, JUploadPreSignCred * preSignCred))successBlock
                    error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)setGlobalMute:(BOOL)isMute
               userId:(NSString *)userId
             timezone:(NSString *)timezone
              periods:(NSArray <JTimePeriod *> *)periods
              success:(void (^)(long long timestamp))successBlock
                error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)getGlobalMute:(NSString *)userId
              success:(void (^)(BOOL isMute, NSString *timezone, NSArray <JTimePeriod *> *periods))successBlock
                error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)joinChatroom:(NSString *)chatroomId
        isAutoCreate:(BOOL)isAutoCreate
             success:(void (^)(long long timestamp))successBlock
               error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)quitChatroom:(NSString *)chatroomId
             success:(void (^)(long long timestamp))successBlock
               error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)setUnread:(JConversation *)conversation
           userId:(NSString *)userId
          success:(void (^)(long long timestamp))successBlock
            error:(void (^)(JErrorCodeInternal))errorBlock;

- (void)syncChatroomMessagesWithTime:(long long)syncTime
                          chatroomId:(NSString *)chatroomId
                    prevMessageCount:(int)count;

- (void)syncChatroomAttributesWithTime:(long long)syncTime
                            chatroomId:(NSString *)chatroomId;

- (void)getFirstUnreadMessage:(JConversation *)conversation
                      success:(void (^)(NSArray<JConcreteMessage *> *messages, BOOL isFinished))successBlock
                        error:(void (^)(JErrorCodeInternal))errorBlock;

- (void)setAttributes:(NSDictionary <NSString *, NSString *> *)attributes
          forChatroom:(NSString *)chatroomId
             complete:(void (^)(JErrorCodeInternal code, NSArray <JChatroomAttributeItem *> *items))completBlock;

- (void)removeAttributes:(NSArray <NSString *> *)keys
             forChatroom:(NSString *)chatroomId
                complete:(void (^)(JErrorCodeInternal code, NSArray <JChatroomAttributeItem *> *items))completeBlock;

- (void)pushSwitch:(BOOL)enablePush
            userId:(NSString *)userId;

- (void)uploadLogStatus:(int)result
                 userId:(NSString *)userId
              messageId:(NSString *)messageId
                    url:(NSString *)url;

- (void)sendPing;
@end

NS_ASSUME_NONNULL_END
