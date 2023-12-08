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

NS_ASSUME_NONNULL_BEGIN

@protocol JWebSocketConnectDelegate <NSObject>
- (void)connectCompleteWithCode:(JErrorCode)error
                         userId:(NSString *)userId;
@end

@interface JWebSocket : NSObject
- (instancetype)initWithSendQueque:(dispatch_queue_t)sendQueue
                      receiveQueue:(dispatch_queue_t)receiveQueue;
- (void)connect:(NSString *)appKey
          token:(NSString *)token;
- (void)disconnect:(BOOL)needPush;
- (void)setConnectDelegate:(id<JWebSocketConnectDelegate>)delegate;

- (void)registerMessageType:(Class)messageClass;

- (void)sendIMMessage:(JMessageContent *)content
       inConversation:(JConversation *)conversation;
- (void)queryHisMsgsFrom:(JConversation *)conversation
               startTime:(long long)startTime
                   count:(int)count
               direction:(JPullDirection)direction
                 success:(void (^)(NSArray *messages, BOOL isRemaining))successBlock
                   error:(void (^)(JErrorCode code))errorBlock;
- (void)syncConversations:(long long)startTime
                    count:(int)count
                   userId:(NSString *)userId
                  success:(void (^)(NSArray *conversations, BOOL isRemaining))successBlock
                    error:(void (^)(JErrorCode code))errorBlock;
@end



NS_ASSUME_NONNULL_END
