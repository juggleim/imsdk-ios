//
//  JConversationInfo.m
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import "JConversationInfo.h"
#import "JuggleIM/JIM.h"

@interface JConversationInfo ()
@property (nonatomic, strong) JGroupInfo *groupInfo;
@property (nonatomic, strong) JUserInfo *userInfo;
@end

@implementation JConversationInfo

- (NSString *)displayName {
    NSString *displayName = @"";
    if (self.conversation.conversationType == JConversationTypeGroup
        || self.conversation.conversationType == JConversationTypePublicService) {
        displayName = self.groupInfo.groupName;
    } else if (self.conversation.conversationType == JConversationTypePrivate) {
        JFriendInfo *friendInfo = [JIM.shared.userInfoManager getFriendInfo:self.conversation.conversationId];
        displayName = friendInfo.alias;
        if (displayName.length == 0) {
            displayName = self.userInfo.userName;
        }
    }
    return displayName;
}

- (NSString *)alias {
    NSString *alias = @"";
    if (self.conversation.conversationType == JConversationTypeGroup
        || self.conversation.conversationType == JConversationTypePublicService) {
        alias = self.groupInfo.groupName;
    } else if (self.conversation.conversationType == JConversationTypePrivate) {
        JFriendInfo *friendInfo = [JIM.shared.userInfoManager getFriendInfo:self.conversation.conversationId];
        alias = friendInfo.alias;
    }
    return alias;
}

- (NSString *)name {
    NSString *name = @"";
    if (self.conversation.conversationType == JConversationTypeGroup
        || self.conversation.conversationType == JConversationTypePublicService) {
        name = self.groupInfo.groupName;
    } else if (self.conversation.conversationType == JConversationTypePrivate) {
        name = self.userInfo.userName;
    }
    return name;
}

- (NSString *)portrait {
    NSString *portrait = @"";
    if (self.conversation.conversationType == JConversationTypeGroup
        || self.conversation.conversationType == JConversationTypePublicService) {
        portrait = self.groupInfo.portrait;
    } else if (self.conversation.conversationType == JConversationTypePrivate) {
        portrait = self.userInfo.portrait;
    }
    return portrait;
}

- (JGroupInfo *)groupInfo {
    if (!_groupInfo) {
        _groupInfo = [JIM.shared.userInfoManager getGroupInfo:self.conversation.conversationId];
    }
    return _groupInfo;
}

- (JUserInfo *)userInfo {
    if (!_userInfo) {
        _userInfo = [JIM.shared.userInfoManager getUserInfo:self.conversation.conversationId];
    }
    return _userInfo;
}

@end
