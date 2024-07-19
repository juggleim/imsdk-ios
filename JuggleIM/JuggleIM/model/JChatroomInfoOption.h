//
//  JChatroomInfoOption.h
// JuggleIM
//
//  Created by Nathan on 2024/7/8.
//

#import <Foundation/Foundation.h>
#import "JIMCore.h"

NS_ASSUME_NONNULL_BEGIN

@interface JChatroomInfoOption : NSObject

/// 拉取数量，member 大于 0 小于等于 100， 默认为 20
@property (nonatomic, assign) int memberCount;

/// 成员排序，默认为 JChatroomMemberOrderDescending
@property (nonatomic, assign) JChatroomMemberOrder memberOrder;

@end

NS_ASSUME_NONNULL_END
