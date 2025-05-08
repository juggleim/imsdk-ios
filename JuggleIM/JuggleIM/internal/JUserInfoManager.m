//
//  JUserInfoManager.m
// JuggleIM
//
//  Created by Nathan on 2024/3/12.
//

#import "JUserInfoManager.h"
#import "JUserInfoCache.h"

@interface JUserInfoManager ()
@property (nonatomic, strong) JIMCore *core;
@property (nonatomic, strong) JUserInfoCache *cache;
@end

@implementation JUserInfoManager

- (instancetype)initWithCore:(JIMCore *)core {
    if (self = [super init]) {
        self.core = core;
        self.cache = [[JUserInfoCache alloc] init];
    }
    return self;
}

- (void)clearCache {
    [self.cache clearCache];
}

- (JUserInfo *)getUserInfo:(NSString *)userId {
    JUserInfo *userInfo = [self.cache getUserInfo:userId];
    if (userInfo) {
        return userInfo;
    }
    userInfo = [self.core.dbManager getUserInfo:userId];
    [self.cache putUserInfo:userInfo];
    return userInfo;
}

- (JGroupInfo *)getGroupInfo:(NSString *)groupId {
    JGroupInfo *groupInfo = [self.cache getGroupInfo:groupId];
    if (groupInfo) {
        return groupInfo;
    }
    groupInfo = [self.core.dbManager getGroupInfo:groupId];
    [self.cache putGroupInfo:groupInfo];
    return groupInfo;
}

- (JGroupMember *)getGroupMember:(NSString *)groupId userId:(NSString *)userId {
    JGroupMember *groupMember = [self.cache getGroupMember:groupId userId:userId];
    if (groupMember) {
        return groupMember;
    }
    groupMember = [self.core.dbManager getGroupMember:groupId userId:userId];
    [self.cache putGroupMember:groupMember];
    return groupMember;
}

- (void)insertUserInfoList:(NSArray<JUserInfo *> *)userInfoList {
    if (userInfoList.count == 0) {
        return;
    }
    [self.cache putUserInfoList:userInfoList];
    [self.core.dbManager insertUserInfos:userInfoList];
}

- (void)insertGroupInfoList:(NSArray<JGroupInfo *> *)groupInfoList {
    if (groupInfoList.count == 0) {
        return;
    }
    [self.cache putGroupInfoList:groupInfoList];
    [self.core.dbManager insertGroupInfos:groupInfoList];
}

- (void)insertGroupMemberList:(NSArray<JGroupMember *> *)groupMemberList {
    if (groupMemberList.count == 0) {
        return;
    }
    [self.cache putGroupMemberList:groupMemberList];
    [self.core.dbManager insertGroupMembers:groupMemberList];
}

@end
