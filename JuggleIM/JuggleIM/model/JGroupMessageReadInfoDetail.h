//
//  JGroupMessageReadInfoDetail.h
//  JuggleIM
//
//  Created by Fei Li on 2025/10/30.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JGroupMessageMemberReadDetail.h>

NS_ASSUME_NONNULL_BEGIN

@interface JGroupMessageReadInfoDetail : NSObject

/// 群消息已读数量
@property (nonatomic, assign) int readCount;
/// 群成员总数
@property (nonatomic, assign) int memberCount;
/// 已读用户列表
@property (nonatomic, copy) NSArray <JGroupMessageMemberReadDetail *> *readMembers;
/// 未读用户列表
@property (nonatomic, copy) NSArray <JGroupMessageMemberReadDetail *> *unreadMembers;

@end

NS_ASSUME_NONNULL_END
