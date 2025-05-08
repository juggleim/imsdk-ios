//
//  JUserInfoCache.h
//  JuggleIM
//
//  Created by Fei Li on 2025/5/7.
//

#import <Foundation/Foundation.h>
#import "JUserInfo.h"
#import "JGroupInfo.h"
#import "JGroupMember.h"

@interface JUserInfoCache : NSObject
- (void)clearCache;
- (JUserInfo *)getUserInfo:(NSString *)userId;
- (void)putUserInfo:(JUserInfo *)userInfo;
- (void)putUserInfoList:(NSArray <JUserInfo *> *)userInfoList;
- (JGroupInfo *)getGroupInfo:(NSString *)groupId;
- (void)putGroupInfo:(JGroupInfo *)groupInfo;
- (void)putGroupInfoList:(NSArray <JGroupInfo *> *)groupInfoList;
- (JGroupMember *)getGroupMember:(NSString *)groupId
                          userId:(NSString *)userId;
- (void)putGroupMember:(JGroupMember *)groupMember;
- (void)putGroupMemberList:(NSArray <JGroupMember *> *)groupMemberList;
@end

