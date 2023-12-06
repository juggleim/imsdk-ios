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

- (void)sendIMMessage:(JMessageContent *)content
       inConversation:(JConversation *)conversation;

@end



NS_ASSUME_NONNULL_END
