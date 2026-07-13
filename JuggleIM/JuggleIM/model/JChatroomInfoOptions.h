//
//  JChatroomInfoOption.h
// JuggleIM
//
//  Created by Nathan on 2024/7/8.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JuggleIMConst.h>

NS_ASSUME_NONNULL_BEGIN

@interface JChatroomInfoOptions : NSObject

/// Fetch count; member is greater than 0 and less than or equal to 100. Defaults to 20
@property (nonatomic, assign) int memberCount;

/// Member order; defaults to JChatroomMemberOrderDescending
@property (nonatomic, assign) JChatroomMemberOrder memberOrder;

@end

NS_ASSUME_NONNULL_END
