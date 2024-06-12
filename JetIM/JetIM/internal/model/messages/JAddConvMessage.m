//
//  JAddConvMessage.m
//  JetIM
//
//  Created by Nathan on 2024/6/12.
//

#import "JAddConvMessage.h"

#define jAddConvType @"jg:addconver"
#define jConversation @"conversation"
#define jTargetId @"target_id"
#define jChannelType @"channel_type"
#define jSortTime @"sort_time"
#define jSyncTime @"sync_time"
#define jTargetUserInfo @"target_user_info"
#define jUserId @"user_id"
#define jUserName @"nickname"
#define jUserPortrait @"user_portrait"
#define jExtFields @"ext_fields"
#define jGroupInfo @"group_info"
#define jGroupId @"group_id"
#define jGroupName @"group_name"
#define jGroupPortrait @"group_portrait"

@implementation JAddConvMessage

+ (NSString *)contentType {
    return jAddConvType;
}

- (void)decode:(NSData *)data {
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    NSDictionary *conversationDic = json[jConversation];
    JConcreteConversationInfo *conversationInfo = [[JConcreteConversationInfo alloc] init];
    id type = conversationDic[jChannelType];
    JConversationType conversationType =  JConversationTypeUnknown;
    if ([type isKindOfClass:[NSNumber class]]) {
        conversationType = [(NSNumber *)type intValue];
    }
    JConversation *c = [[JConversation alloc] initWithConversationType:conversationType conversationId:conversationDic[jTargetId]];
    conversationInfo.conversation = c;
    id sortTime = conversationDic[jSortTime];
    if ([sortTime isKindOfClass:[NSNumber class]]) {
        conversationInfo.sortTime = [(NSNumber *)sortTime longLongValue];
    }
    id syncTime = conversationDic[jSyncTime];
    if ([syncTime isKindOfClass:[NSNumber class]]) {
        conversationInfo.syncTime = [(NSNumber *)syncTime longLongValue];
    }
    NSDictionary *userInfoDic = conversationDic[jTargetUserInfo];
    if (userInfoDic) {
        conversationInfo.targetUserInfo = [self decodeUserInfo:userInfoDic];
    }
    NSDictionary *groupDic = conversationDic[jGroupInfo];
    if (groupDic) {
        conversationInfo.groupInfo = [self decodeGroupInfo:groupDic];
    }
    self.conversationInfo = conversationInfo;
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

- (JUserInfo *)decodeUserInfo:(NSDictionary *)userInfoDic {
    JUserInfo *userInfo = [[JUserInfo alloc] init];
    userInfo.userId = userInfoDic[jUserId];
    userInfo.userName = userInfoDic[jUserName];
    userInfo.portrait = userInfoDic[jUserPortrait];
    userInfo.extraDic = userInfoDic[jExtFields];
    return userInfo;
}

- (JGroupInfo *)decodeGroupInfo:(NSDictionary *)dic {
    JGroupInfo *info = [[JGroupInfo alloc] init];
    info.groupId = dic[jGroupId];
    info.groupName = dic[jGroupName];
    info.portrait = dic[jGroupPortrait];
    info.extraDic = dic[jExtFields];
    return info;
}

@end
