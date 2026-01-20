//
//  JMessage.m
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import "JMessage.h"
#import "JuggleIM/JIM.h"

@interface JMessage ()
@property (nonatomic, strong) JUserInfo *userInfo;
@end

@implementation JMessage

- (BOOL)isEqual:(id)object {
    if ([object isKindOfClass:self.class]) {
        JMessage * it = (JMessage *)object;
        return (self.clientMsgNo == it.clientMsgNo);
    } else {
        return [super isEqual:object];
    }
}

- (NSString *)senderDisplayName {
    NSString *userName;
    if (self.conversation.conversationType == JConversationTypeGroup) {
        JGroupMember *groupMember = [JIM.shared.userInfoManager getGroupMember:self.conversation.conversationId userId:self.senderUserId];
        userName = groupMember.groupDisplayName;
        if (userName.length == 0) {
            userName = self.userInfo.userName;
        }
    } else {
        userName = self.userInfo.userName;
    }
    return userName;
}

- (NSString *)senderPortrait {
    NSString *portrait = self.userInfo.portrait;
    return portrait;
}

- (JUserInfo *)userInfo {
    if (!_userInfo) {
        _userInfo = [JIM.shared.userInfoManager getUserInfo:self.conversation.conversationId];
    }
    return _userInfo;
}

@end
