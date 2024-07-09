//
//  JMessageSendReceiveDelegate.h
//  JetIM
//
//  Created by Nathan on 2024/2/27.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol JMessageSendReceiveDelegate <NSObject>
- (void)messageDidSave:(JConcreteMessage *)message;
- (void)messageDidSend:(JConcreteMessage *)message;
- (void)messagesDidReceive:(NSArray <JConcreteMessage *> *)messages;
- (void)conversationsDidAdd:(JConcreteConversationInfo *)conversationInfo;
- (void)conversationsDidDelete:(NSArray <JConversation *> *)conversations;
- (void)conversationsDidUpdate:(JConcreteMessage *)message;
- (void)messageDidRemove:(JConversation *)conversation
         removedMessages:(NSArray <JConcreteMessage *> *)removedMessages
             lastMessage:(JConcreteMessage *)lastMessage;
- (void)messageDidClear:(JConversation *)conversation
              startTime:(long long)startTime
             sendUserId:(NSString *)sendUserId
            lastMessage:(JConcreteMessage *)lastMessage;
-(void)conversationsDidClearTotalUnread:(long long)clearTime;
-(void)messageStateDidChange:(JMessageState)state
                conversation:(JConversation *)conversation
                 clientMsgNo:(long long)clientMsgNo;
-(void)messageDidRead:(JConversation *)conversation
           messageIds:(NSArray <NSString *> *)messageIds;
@end

NS_ASSUME_NONNULL_END
