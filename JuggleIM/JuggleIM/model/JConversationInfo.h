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
/// 会话标识
@property (nonatomic, strong) JConversation *conversation;
/// 当前会话的消息未读数
@property (nonatomic, assign) int unreadCount;
/// 会话是否未读状态
@property (nonatomic, assign) BOOL hasUnread;
/// 会话排序时间（服务端时间）
@property (nonatomic, assign) long long sortTime;
/// 会话中的最后一条消息
@property (nonatomic, strong) JMessage *lastMessage;
/// 是否置顶
@property (nonatomic, assign) BOOL isTop;
/// 会话置顶时间
@property (nonatomic, assign) long long topTime;
/// 是否免打扰
@property (nonatomic, assign) BOOL mute;
/// 会话草稿
@property (nonatomic, copy) NSString *draft;
/// @信息
@property (nonatomic, strong) JConversationMentionInfo * mentionInfo;

@end

