//
//  JUserInfoManager.h
// JuggleIM
//
//  Created by Nathan on 2024/3/12.
//

#import <Foundation/Foundation.h>
#import "JUserInfoProtocol.h"
#import "JIMCore.h"

NS_ASSUME_NONNULL_BEGIN

@interface JUserInfoManager : NSObject <JUserInfoProtocol>

- (instancetype)initWithCore:(JIMCore *)core;
- (void)clearCache;
- (void)insertUserInfoList:(NSArray <JUserInfo *> *)userInfoList;
- (void)insertGroupInfoList:(NSArray <JGroupInfo *> *)groupInfoList;
- (void)insertGroupMemberList:(NSArray <JGroupMember *> *)groupMemberList;

@end

NS_ASSUME_NONNULL_END
