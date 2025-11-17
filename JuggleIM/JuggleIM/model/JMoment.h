//
//  JMoment.h
//  JuggleIM
//
//  Created by Fei Li on 2025/11/11.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JUserInfo.h>
#import <JuggleIM/JMomentMedia.h>
#import <JuggleIM/JMomentReaction.h>
#import <JuggleIM/JMomentComment.h>

NS_ASSUME_NONNULL_BEGIN

@interface JMoment : NSObject
/// 朋友圈 id
@property (nonatomic, copy) NSString *momentId;
/// 朋友圈的文本内容
@property (nonatomic, copy) NSString *content;
/// 朋友圈的媒体内容列表（图片或者视频）
@property (nonatomic, copy) NSArray <JMomentMedia *> *mediaArray;
/// 朋友圈的发送者
@property (nonatomic, strong) JUserInfo *userInfo;
/// 朋友圈的点赞列表（支持多种类型的点赞，此处最多给出最新的 20 个）
@property (nonatomic, copy) NSArray <JMomentReaction *> *reactionArray;
/// 朋友圈的评论列表（此处最多给出最新的 10 条）
@property (nonatomic, copy) NSArray <JMomentComment *> *commentArray;
/// 朋友圈的创建时间
@property (nonatomic, assign) long long createTime;
/// 朋友圈的更新时间
@property (nonatomic, assign) long long updateTime;
@end

NS_ASSUME_NONNULL_END
