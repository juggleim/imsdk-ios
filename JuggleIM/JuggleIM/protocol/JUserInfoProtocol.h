//
//  JUserInfoProtocol.h
// JuggleIM
//
//  Created by Nathan on 2024/3/12.
//

#import <JuggleIM/JUserInfo.h>
#import <JuggleIM/JGroupInfo.h>

@protocol JUserInfoProtocol <NSObject>

/// 获取用户信息
/// - Parameter userId: 用户 id
- (JUserInfo *)getUserInfo:(NSString *)userId;

/// 获取群组信息
/// - Parameter groupId: 群组 id
- (JGroupInfo *)getGroupInfo:(NSString *)groupId;

@end
