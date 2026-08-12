//
//  JConcreteConversationInfo.h
// JuggleIM
//
//  Created by Nathan on 2023/12/25.
//

#import <JuggleIM/JuggleIM.h>
#import "JGroupInfo.h"
#import "JUserInfo.h"

NS_ASSUME_NONNULL_BEGIN

@interface JConcreteConversationInfo : JConversationInfo
@property (nonatomic, assign) long long lastReadMessageIndex;//Unread index of the last read message.
@property (nonatomic, assign) long long lastMessageIndex;//Unread index of the last message.
@property (nonatomic, assign) long long syncTime;
@property (nonatomic, strong) JGroupInfo *groupInfo;
@property (nonatomic, strong) JUserInfo *targetUserInfo;
@property (nonatomic, strong) JFriendInfo *friendInfo;
@property (nonatomic, copy) NSArray <JUserInfo *> * mentionUserList;
/// Conversation tag array.
@property (nonatomic, copy) NSArray <JConversationTagInfo *> *tagInfoList;
@end

NS_ASSUME_NONNULL_END
