//
//  JChatroomInfo.h
//  JetIM
//
//  Created by Nathan on 2024/7/8.
//

#import <Foundation/Foundation.h>
#import "JChatroomMember.h"
#import "JetIMConst.h"

NS_ASSUME_NONNULL_BEGIN


@interface JChatroomInfo : NSObject

/// 聊天室 id
@property (nonatomic, copy) NSString *chatroomId;

/// 成员排序
@property (nonatomic, assign) JChatroomMemberOrder memberOrder;

/// 聊天室成员数量
@property (nonatomic, assign) int totalMemberCount;

/// 成员列表，只包含前 100 条（按照 memberOrder 排序规则）
@property (nonatomic, strong) NSArray <JChatroomMember *> *memberList;

@end

NS_ASSUME_NONNULL_END
