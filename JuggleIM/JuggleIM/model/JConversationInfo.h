//
//  JConversationInfo.h
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import <Foundation/Foundation.h>

#import <JuggleIM/JConversation.h>
#import <JuggleIM/JMessage.h>
#import <JuggleIM/JConversationMentionInfo.h>


@interface JConversationInfo : NSObject
/// Conversation identifier
@property (nonatomic, strong) JConversation *conversation;
/// Unread message count for the current conversation
@property (nonatomic, assign) int unreadCount;
/// Whether the conversation is unread
@property (nonatomic, assign) BOOL hasUnread;
/// Conversation sort time (server time)
@property (nonatomic, assign) long long sortTime;
/// Last message in the conversation
@property (nonatomic, strong) JMessage *lastMessage;
/// Whether the conversation is pinned
@property (nonatomic, assign) BOOL isTop;
/// Conversation pinned time
@property (nonatomic, assign) long long topTime;
/// Whether Do Not Disturb is enabled
@property (nonatomic, assign) BOOL mute;
/// Conversation draft
@property (nonatomic, copy) NSString *draft;
/// Mention info
@property (nonatomic, strong) JConversationMentionInfo * mentionInfo;
/// Conversation name (displayed according to the default rules)
@property (nonatomic, copy) NSString *displayName;
/// Conversation remark name (group conversations use the group remark name; one-to-one conversations use the friend remark name)
@property (nonatomic, copy) NSString *alias;
/// Conversation real name (group conversations use the group name; one-to-one conversations use the user name)
@property (nonatomic, copy) NSString *name;
/// Conversation avatar
@property (nonatomic, copy) NSString *portrait;

@end

