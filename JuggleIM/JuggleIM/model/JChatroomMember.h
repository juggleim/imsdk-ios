//
//  JChatroomMember.h
// JuggleIM
//
//  Created by Nathan on 2024/7/8.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JUserInfo.h>

NS_ASSUME_NONNULL_BEGIN

@interface JChatroomMember : NSObject

/// 用户信息
@property (nonatomic, strong) JUserInfo *userInfo;

/// 用户加入聊天室时间戳
@property (nonatomic, assign) long long joinTime;

@end

NS_ASSUME_NONNULL_END
