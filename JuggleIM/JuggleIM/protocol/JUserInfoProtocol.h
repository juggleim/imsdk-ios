//
//  JUserInfoProtocol.h
// JuggleIM
//
//  Created by Nathan on 2024/3/12.
//

#import <JuggleIM/JUserInfo.h>
#import <JuggleIM/JGroupInfo.h>
#import <JuggleIM/JGroupMember.h>
#import <JuggleIM/JFriendInfo.h>
#import <JuggleIM/JuggleIMConst.h>
#import <JuggleIM/JUserStatus.h>

@protocol JUserStatusDelegate <NSObject>

- (void)userStatusDidChange:(JUserStatus *)userStatus;

@end

@protocol JUserInfoProtocol <NSObject>

/// Gets user information.
/// - Parameter userId: User ID.
- (JUserInfo *)getUserInfo:(NSString *)userId;

/// Gets user information in batches.
/// - Parameter userIdList: User ID list.
- (NSArray <JUserInfo *> *)getUserInfoList:(NSArray <NSString *> *)userIdList;

/// Gets group information.
/// - Parameter groupId: Group ID.
- (JGroupInfo *)getGroupInfo:(NSString *)groupId;

/// Gets group information in batches.
/// - Parameter groupIdList: Group ID list.
- (NSArray <JGroupInfo *> *)getGroupInfoList:(NSArray <NSString *> *)groupIdList;

/// Gets group member information.
/// - Parameters:
///   - groupId: Group ID.
///   - userId: User ID.
- (JGroupMember *)getGroupMember:(NSString *)groupId
                          userId:(NSString *)userId;

/// Gets friend information.
/// - Parameter userId: Friend's user ID.
- (JFriendInfo *)getFriendInfo:(NSString *)userId;

/// Fetches the latest user information from the server.
/// - Parameters:
///   - userId: User ID.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)fetchUserInfo:(NSString *)userId
              success:(void (^)(JUserInfo *userInfo))successBlock
                error:(void (^)(JErrorCode code))errorBlock;

/// Fetches the latest group information from the server.
/// - Parameters:
///   - groupId: Group ID.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)fetchGroupInfo:(NSString *)groupId
               success:(void (^)(JGroupInfo *groupInfo))successBlock
                 error:(void (^)(JErrorCode code))errorBlock;

/// Fetches the latest friend information from the server.
/// - Parameters:
///   - userId: User ID.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)fetchFriendInfo:(NSString *)userId
                success:(void (^)(JFriendInfo *friendInfo))successBlock
                  error:(void (^)(JErrorCode code))errorBlock;

/// Queries user status.
/// - Parameters:
///   - userIdList: User ID list to fetch.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)getUserStatus:(NSArray <NSString *>*)userIdList
              success:(void (^)(NSArray <JUserStatus *> *statusList))successBlock
                error:(void (^)(JErrorCode code))errorBlock;

- (void)addUserStatusDelegate:(id<JUserStatusDelegate>)delegate;
@end
