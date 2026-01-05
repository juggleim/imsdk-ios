//
//  JUserInfoProtocol.h
// JuggleIM
//
//  Created by Nathan on 2024/3/12.
//

#import <JuggleIM/JUserInfo.h>
#import <JuggleIM/JGroupInfo.h>
#import <JuggleIM/JGroupMember.h>
#import <JuggleIM/JuggleIMConst.h>

@protocol JUserInfoProtocol <NSObject>

/// 获取用户信息
/// - Parameter userId: 用户 id
- (JUserInfo *)getUserInfo:(NSString *)userId;

/// 批量获取用户信息
/// - Parameter userIdList: 用户 id 列表
- (NSArray <JUserInfo *> *)getUserInfoList:(NSArray <NSString *> *)userIdList;

/// 获取群组信息
/// - Parameter groupId: 群组 id
- (JGroupInfo *)getGroupInfo:(NSString *)groupId;

/// 批量获取群组信息
/// - Parameter groupIdList: 群组 id 列表
- (NSArray <JGroupInfo *> *)getGroupInfoList:(NSArray <NSString *> *)groupIdList;

/// 获取群成员信息
/// - Parameters:
///   - groupId: 群组 id
///   - userId: 用户 id
- (JGroupMember *)getGroupMember:(NSString *)groupId
                          userId:(NSString *)userId;

/// 从服务端获取最新的用户信息
/// - Parameters:
///   - userId: 用户 id
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)fetchUserInfo:(NSString *)userId
              success:(void (^)(JUserInfo *userInfo))successBlock
                error:(void (^)(JErrorCode code))errorBlock;

@end
