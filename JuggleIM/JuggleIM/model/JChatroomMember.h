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

/// User info
@property (nonatomic, strong) JUserInfo *userInfo;

/// Timestamp when the user joined the chatroom
@property (nonatomic, assign) long long joinTime;

@end

NS_ASSUME_NONNULL_END
