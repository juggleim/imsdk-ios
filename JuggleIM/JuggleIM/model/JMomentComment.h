//
//  JMomentComment.h
//  JuggleIM
//
//  Created by Fei Li on 2025/11/11.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JUserInfo.h>

NS_ASSUME_NONNULL_BEGIN

@interface JMomentComment : NSObject

/// 评论 id
@property (nonatomic, copy) NSString *commentId;
/// 帖子 id
@property (nonatomic, copy) NSString *postId;
/// 被回复的评论 id
@property (nonatomic, copy) NSString *parentCommentId;
/// 评论内容
@property (nonatomic, copy) NSString *content;
/// 评论人的用户信息
@property (nonatomic, strong) JUserInfo *userInfo;
/// 被回复的评论用户信息
@property (nonatomic, strong) JUserInfo *parentUserInfo;
/// 创建时间
@property (nonatomic, assign) long long createTime;
/// 更新时间
@property (nonatomic, assign) long long updateTime;

- (NSDictionary *)toDictionary;

@end

NS_ASSUME_NONNULL_END
