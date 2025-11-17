//
//  JPostProtocol.h
//  JuggleIM
//
//  Created by Fei Li on 2025/11/11.
//

#import <JuggleIM/JMomentMedia.h>
#import <JuggleIM/JMoment.h>
#import <JuggleIM/JGetMomentOption.h>
#import <JuggleIM/JGetMomentCommentOption.h>
#import <JuggleIM/JMomentReaction.h>

@protocol JMomentProtocol <NSObject>

/// 发布朋友圈
/// - Parameters:
///   - content: 朋友圈的文本内容
///   - mediaList: 朋友圈的媒体内容列表（图片或者视频）
///   - completeBlock: 结果回调
- (void)addMoment:(nonnull NSString *)content
        mediaList:(nullable NSArray <JMomentMedia *> *)mediaList
         complete:(nullable void (^)(JErrorCode errorCode, JMoment * _Nullable moment))completeBlock;

/// 删除朋友圈
/// - Parameters:
///   - momentId: 朋友圈 id
///   - completeBlock: 结果回调
- (void)removeMoment:(nonnull NSString *)momentId
            complete:(nullable void (^)(JErrorCode errorCode))completeBlock;

/// 获取缓存的朋友圈列表（缓存的数据不一定是最新版本，可用于第一时间渲染界面，优化用户体验）
/// - Parameter option: 获取参数
- (NSArray <JMoment *> *_Nonnull)getCachedMomentList:(nonnull JGetMomentOption *)option;

/// 获取朋友圈列表
/// - Parameters:
///   - option: 获取参数
///   - completeBlock: 结果回调
- (void)getMomentList:(nonnull JGetMomentOption *)option
             complete:(nullable void (^)(JErrorCode errorCode, NSArray <JMoment *> * _Nullable momentList))completeBlock;

/// 获取朋友圈详情
/// - Parameters:
///   - momentId: 朋友圈 id
///   - completeBlock: 结果回调
- (void)getMoment:(nonnull NSString *)momentId
         complete:(nullable void (^)(JErrorCode errorCode, JMoment * _Nullable moment))completeBlock;

/// 发布评论
/// - Parameters:
///   - momentId: 评论的朋友圈 id
///   - parentCommentId: 父级评论 id
///   - content: 评论内容
///   - completeBlock: 结果回调
- (void)addComment:(nonnull NSString *)momentId
   parentCommentId:(nullable NSString *)parentCommentId
           content:(nonnull NSString *)content
          complete:(nullable void (^)(JErrorCode errorCode, JMomentComment * _Nullable comment))completeBlock;

/// 删除评论
/// - Parameters:
///   - commentId: 评论 id
///   - completeBlock: 结果回调
- (void)removeComment:(nonnull NSString *)commentId
             complete:(nullable void (^)(JErrorCode errorCode))completeBlock;

/// 获取评论列表
/// - Parameters:
///   - option: 获取参数
///   - completeBlock: 结果回调
- (void)getCommentList:(nonnull JGetMomentCommentOption *)option
              complete:(nullable void (^)(JErrorCode errorCode, NSArray <JMomentComment *> * _Nullable commentList))completeBlock;

/// 添加点赞
/// - Parameters:
///   - momentId: 朋友圈 id
///   - key: 点赞类型
///   - completeBlock: 结果回调
- (void)addReaction:(nonnull NSString *)momentId
                key:(nonnull NSString *)key
           complete:(nullable void (^)(JErrorCode errorCode))completeBlock;

/// 取消点赞
/// - Parameters:
///   - momentId: 朋友圈 id
///   - key: 点赞类型
///   - completeBlock: 结果回调
- (void)removeReaction:(nonnull NSString *)momentId
                   key:(nonnull NSString *)key
              complete:(nullable void (^)(JErrorCode errorCode))completeBlock;

/// 获取点赞列表
/// - Parameters:
///   - momentId: 朋友圈 id
///   - completeBlock: 结果回调
- (void)getReactionList:(nonnull NSString *)momentId
               complete:(nullable void (^)(JErrorCode errorCode, NSArray <JMomentReaction *> * _Nullable reactionList))completeBlock;

@end
