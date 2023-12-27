//
//  JWebSocket.h
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

#import <Foundation/Foundation.h>
#import "JuggleConst.h"
#import "JMessageContent.h"
#import "JConversation.h"
#import "JConcreteMessage.h"

NS_ASSUME_NONNULL_BEGIN

@protocol JWebSocketConnectDelegate <NSObject>
- (void)connectCompleteWithCode:(JErrorCode)error
                         userId:(NSString *)userId;
- (void)webSocketDidFail;
- (void)webSocketDidClose;
@end

@protocol JWebSocketMessageDelegate <NSObject>
- (void)messagesDidReceive:(NSArray<JConcreteMessage *> *)messages
                isFinished:(BOOL)isFinished;
- (void)syncNotify:(long long)syncTime;
@end

@interface JWebSocket : NSObject
- (instancetype)initWithSendQueque:(dispatch_queue_t)sendQueue
                      receiveQueue:(dispatch_queue_t)receiveQueue;
- (void)connect:(NSString *)appKey
          token:(NSString *)token;
- (void)disconnect:(BOOL)needPush;
- (void)setConnectDelegate:(id<JWebSocketConnectDelegate>)delegate;

- (void)setMessageDelegate:(id<JWebSocketMessageDelegate>)delegate;

- (void)sendIMMessage:(JMessageContent *)content
       inConversation:(JConversation *)conversation
          clientMsgNo:(long long)clientMsgNo
            clientUid:(NSString *)clientUid
              success:(void (^)(long long clientMsgNo, NSString *msgId, long long timestamp))successBlock
                error:(void (^)(JErrorCode errorCode, long long clientMsgNo))errorBlock;

- (void)syncMessagesWithReceiveTime:(long long)receiveTime
                           sendTime:(long long)sendTime
                             userId:(NSString *)userId;

- (void)queryHisMsgsFrom:(JConversation *)conversation
               startTime:(long long)startTime
                   count:(int)count
               direction:(JPullDirection)direction
                 success:(void (^)(NSArray *messages, BOOL isFinished))successBlock
                   error:(void (^)(JErrorCode code))errorBlock;
- (void)syncConversations:(long long)startTime
                    count:(int)count
                   userId:(NSString *)userId
                  success:(void (^)(NSArray *conversations, BOOL isFinished))successBlock
                    error:(void (^)(JErrorCode code))errorBlock;

- (void)sendPing;
@end



NS_ASSUME_NONNULL_END
