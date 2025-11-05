//
//  JUserInfoCache.m
//  JuggleIM
//
//  Created by Fei Li on 2025/5/7.
//

#import "JUserInfoCache.h"
#import "JLRUCache.h"

#define jMaxCacheCount 100
#define jGroupSeparator @"+++"

@interface JUserInfoCache ()
@property (nonatomic, strong) JLRUCache *userInfoCache;
@property (nonatomic, strong) JLRUCache *groupInfoCache;
@property (nonatomic, strong) JLRUCache *groupMemberCache;
@end

@implementation JUserInfoCache
- (void)clearCache {
    [self.userInfoCache clearCache];
    [self.groupInfoCache clearCache];
    [self.groupMemberCache clearCache];
}

- (JUserInfo *)getUserInfo:(NSString *)userId {
    if (userId.length == 0){
        return nil;
    }
    return [self.userInfoCache get:userId];
}

- (void)putUserInfo:(JUserInfo *)userInfo {
    if (userInfo.userId.length > 0) {
        JUserInfo *old = [self.userInfoCache get:userInfo.userId];
        if (!old || userInfo.updatedTime >= old.updatedTime) {
            [self.userInfoCache put:userInfo.userId value:userInfo];
        }
    }
}

- (void)putUserInfoList:(NSArray<JUserInfo *> *)userInfoList {
    for (JUserInfo *userInfo in userInfoList) {
        [self putUserInfo:userInfo];
    }
}

- (JGroupInfo *)getGroupInfo:(NSString *)groupId {
    if (groupId.length == 0) {
        return nil;
    }
    return [self.groupInfoCache get:groupId];
}

- (void)putGroupInfo:(JGroupInfo *)groupInfo {
    if (groupInfo.groupId.length > 0) {
        JGroupInfo *old = [self.groupInfoCache get:groupInfo.groupId];
        if (!old || groupInfo.updatedTime >= old.updatedTime) {
            [self.groupInfoCache put:groupInfo.groupId value:groupInfo];
        }
    }
}

- (void)putGroupInfoList:(NSArray<JGroupInfo *> *)groupInfoList {
    for (JGroupInfo *groupInfo in groupInfoList) {
        [self putGroupInfo:groupInfo];
    }
}

- (JGroupMember *)getGroupMember:(NSString *)groupId userId:(NSString *)userId {
    if (groupId.length == 0 || userId.length == 0) {
        return nil;
    }
    return [self.groupMemberCache get:[self keyForGroupId:groupId userId:userId]];
}

- (void)putGroupMember:(JGroupMember *)groupMember {
    if (groupMember.groupId.length > 0 && groupMember.userId.length > 0) {
        JGroupMember *old = [self.groupMemberCache get:[self keyForGroupId:groupMember.groupId userId:groupMember.userId]];
        if (!old || groupMember.updatedTime >= old.updatedTime) {
            [self.groupMemberCache put:[self keyForGroupId:groupMember.groupId userId:groupMember.userId] value:groupMember];
        }
    }
}

- (void)putGroupMemberList:(NSArray<JGroupMember *> *)groupMemberList {
    for (JGroupMember *groupMember in groupMemberList) {
        [self putGroupMember:groupMember];
    }
}

#pragma mark - private
- (NSString *)keyForGroupId:(NSString *)groupId
                     userId:(NSString *)userId {
    return [NSString stringWithFormat:@"%@%@%@", groupId, jGroupSeparator, userId];
}

#pragma mark - getter
- (JLRUCache *)userInfoCache {
    if (!_userInfoCache) {
        _userInfoCache = [[JLRUCache alloc] initWithCapacity:jMaxCacheCount];
    }
    return _userInfoCache;
}

- (JLRUCache *)groupInfoCache {
    if (!_groupInfoCache) {
        _groupInfoCache = [[JLRUCache alloc] initWithCapacity:jMaxCacheCount];
    }
    return _groupInfoCache;
}

- (JLRUCache *)groupMemberCache {
    if (!_groupMemberCache) {
        _groupMemberCache = [[JLRUCache alloc] initWithCapacity:jMaxCacheCount];
    }
    return _groupMemberCache;
}
@end
