//
//  JRecallCmdMessage.h
//  JetIM
//
//  Created by Nathan on 2024/2/2.
//

#import <JetIM/JetIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JRecallCmdMessage : JMessageContent

@property (nonatomic, copy) NSString *originalMessageId;
@property (nonatomic, assign) long long originalMessageTime;
@property (nonatomic, copy) NSString *senderId;
@property (nonatomic, copy) NSString *receiverId;//原消息的接收者 id，群聊时为 groupId
@property (nonatomic, assign) JConversationType conversationType;

@end

NS_ASSUME_NONNULL_END
