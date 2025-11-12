//
//  JPost.h
//  JuggleIM
//
//  Created by Fei Li on 2025/11/11.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JUserInfo.h>
#import <JuggleIM/JPostMedia.h>
#import <JuggleIM/JPostReaction.h>

NS_ASSUME_NONNULL_BEGIN

@interface JPost : NSObject

/// 帖子 id
@property (nonatomic, copy) NSString *postId;
/// 帖子的文本内容
@property (nonatomic, copy) NSString *content;
/// 帖子的媒体内容列表（图片或者视频）
@property (nonatomic, copy) NSArray <JPostMedia *> *mediaArray;
/// 帖子的发送者
@property (nonatomic, strong) JUserInfo *userInfo;
/// 帖子的点赞列表（支持多种类型的点赞，此处最多给出最新的 20 个）
@property (nonatomic, copy) NSArray <JPostReaction *> *reactionArray;
/// 帖子的评论列表（此处最多给出最新的 10 条）
@property (nonatomic, copy) NSArray *commentArray;
/// 帖子的创建时间
@property (nonatomic, assign) long long createTime;
/// 帖子的更新时间
@property (nonatomic, assign) long long updateTime;
@end

NS_ASSUME_NONNULL_END
