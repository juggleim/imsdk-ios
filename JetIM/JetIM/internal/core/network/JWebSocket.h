//
//  JWebSocket.h
//  JetIM
//
//  Created by Nathan on 2023/11/29.
//

#import <Foundation/Foundation.h>
#import "JetIMConst.h"
#import "JetIMConstInternal.h"
#import "JMessageContent.h"
#import "JConversation.h"
#import "JConcreteMessage.h"

NS_ASSUME_NONNULL_BEGIN

@protocol JWebSocketConnectDelegate <NSObject>
- (void)connectCompleteWithCode:(JErrorCodeInternal)error
                         userId:(NSString *)userId;
- (void)disconnectWithCode:(JErrorCodeInternal)error;
- (void)webSocketDidFail;
- (void)webSocketDidClose;
@end

@protocol JWebSocketMessageDelegate <NSObject>
- (void)messageDidReceive:(JConcreteMessage *)message;
- (void)messagesDidReceive:(NSArray<JConcreteMessage *> *)messages
                isFinished:(BOOL)isFinished;
- (void)syncNotify:(long long)syncTime;
@end

@interface JWebSocket : NSObject
- (instancetype)initWithSendQueque:(dispatch_queue_t)sendQueue
                      receiveQueue:(dispatch_queue_t)receiveQueue;
- (void)connect:(NSString *)appKey
          token:(NSString *)token
      pushToken:(NSString *)pushToken
        servers:(NSArray *)servers;
- (void)disconnect:(BOOL)needPush;
- (void)setConnectDelegate:(id<JWebSocketConnectDelegate>)delegate;

- (void)setMessageDelegate:(id<JWebSocketMessageDelegate>)delegate;

- (void)sendIMMessage:(JMessageContent *)content
       inConversation:(JConversation *)conversation
          clientMsgNo:(long long)clientMsgNo
            clientUid:(NSString *)clientUid
           mergedMsgs:(NSArray <JConcreteMessage *> *)mergedMsgs
               userId:(NSString *)userId
              success:(void (^)(long long clientMsgNo, NSString *msgId, long long timestamp, long long reqNo))successBlock
                error:(void (^)(JErrorCodeInternal errorCode, long long clientMsgNo))errorBlock;

- (void)recallMessage:(NSString *)messageId
         conversation:(JConversation *)conversation
            timestamp:(long long)timestamp
              success:(void (^)(long long timestamp))successBlock
                error:(void (^)(JErrorCodeInternal errorCode))errorBlock;

- (void)sendReadReceipt:(NSArray <NSString *> *)messageIds
         inConversation:(JConversation *)conversation
                success:(void (^)(void))successBlock
                  error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)syncMessagesWithReceiveTime:(long long)receiveTime
                           sendTime:(long long)sendTime
                             userId:(NSString *)userId;

- (void)queryHisMsgsFrom:(JConversation *)conversation
               startTime:(long long)startTime
                   count:(int)count
               direction:(JPullDirection)direction
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
                       success:(void (^)(void))successBlock
                         error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)clearUnreadCount:(JConversation *)conversation
                  userId:(NSString *)userId
                msgIndex:(long long)msgIndex
                 success:(void (^)(void))successBlock
                   error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)getGroupMessageReadDetail:(NSString *)messageId
                   inConversation:(JConversation *)conversation
                          success:(void (^)(NSArray<JUserInfo *> *readMembers, NSArray<JUserInfo *> *unreadMembers))successBlock
                            error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)setMute:(BOOL)isMute
 inConversation:(JConversation *)conversation
         userId:(NSString *)userId
        success:(void (^)(void))successBlock
          error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)setTop:(BOOL)isTop
inConversation:(JConversation *)conversation
        userId:(NSString *)userId
       success:(void (^)(long long timestamp))successBlock
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
                   success:(void (^)(NSArray<JConcreteMessage *> *messages, BOOL isFinished))successBlock
                     error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)registerPushToken:(NSString *)token
                   userId:(NSString *)userId
                  success:(void (^)(void))successBlock
                    error:(void (^)(JErrorCodeInternal code))errorBlock;

- (void)sendPing;
@end



NS_ASSUME_NONNULL_END
