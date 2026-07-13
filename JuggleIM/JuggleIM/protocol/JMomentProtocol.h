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

/// Publishes a Moment.
/// - Parameters:
///   - content: Text content of the Moment.
///   - mediaList: Media content list for the Moment, including images or videos.
///   - completeBlock: Result callback.
- (void)addMoment:(nonnull NSString *)content
        mediaList:(nullable NSArray <JMomentMedia *> *)mediaList
         complete:(nullable void (^)(JErrorCode errorCode, JMoment * _Nullable moment))completeBlock;

/// Deletes a Moment.
/// - Parameters:
///   - momentId: Moment ID.
///   - completeBlock: Result callback.
- (void)removeMoment:(nonnull NSString *)momentId
            complete:(nullable void (^)(JErrorCode errorCode))completeBlock;

/// Gets the cached Moment list. Cached data may not be the latest version and can be used for immediate UI rendering to optimize user experience.
/// - Parameter option: Fetch options.
- (NSArray <JMoment *> *_Nonnull)getCachedMomentList:(nonnull JGetMomentOption *)option;

/// Gets the Moment list.
/// - Parameters:
///   - option: Fetch options.
///   - completeBlock: Result callback.
- (void)getMomentList:(nonnull JGetMomentOption *)option
             complete:(nullable void (^)(JErrorCode errorCode, NSArray <JMoment *> * _Nullable momentList, BOOL isFinish))completeBlock;

/// Gets Moment details.
/// - Parameters:
///   - momentId: Moment ID.
///   - completeBlock: Result callback.
- (void)getMoment:(nonnull NSString *)momentId
         complete:(nullable void (^)(JErrorCode errorCode, JMoment * _Nullable moment))completeBlock;

/// Publishes a comment.
/// - Parameters:
///   - momentId: Moment ID for the comment.
///   - parentCommentId: Parent comment ID.
///   - content: Comment content.
///   - completeBlock: Result callback.
- (void)addComment:(nonnull NSString *)momentId
   parentCommentId:(nullable NSString *)parentCommentId
           content:(nonnull NSString *)content
          complete:(nullable void (^)(JErrorCode errorCode, JMomentComment * _Nullable comment))completeBlock;

/// Deletes a comment.
/// - Parameters:
///   - commentId: Comment ID.
///   - momentId: Moment ID.
///   - completeBlock: Result callback.
- (void)removeComment:(nonnull NSString *)commentId
             momentId:(nonnull NSString *)momentId
             complete:(nullable void (^)(JErrorCode errorCode))completeBlock;

/// Gets the comment list.
/// - Parameters:
///   - option: Fetch options.
///   - completeBlock: Result callback.
- (void)getCommentList:(nonnull JGetMomentCommentOption *)option
              complete:(nullable void (^)(JErrorCode errorCode, NSArray <JMomentComment *> * _Nullable commentList, BOOL isFinish))completeBlock;

/// Adds a reaction.
/// - Parameters:
///   - momentId: Moment ID.
///   - key: Reaction type.
///   - completeBlock: Result callback.
- (void)addReaction:(nonnull NSString *)momentId
                key:(nonnull NSString *)key
           complete:(nullable void (^)(JErrorCode errorCode))completeBlock;

/// Removes a reaction.
/// - Parameters:
///   - momentId: Moment ID.
///   - key: Reaction type.
///   - completeBlock: Result callback.
- (void)removeReaction:(nonnull NSString *)momentId
                   key:(nonnull NSString *)key
              complete:(nullable void (^)(JErrorCode errorCode))completeBlock;

/// Gets the reaction list.
/// - Parameters:
///   - momentId: Moment ID.
///   - completeBlock: Result callback.
- (void)getReactionList:(nonnull NSString *)momentId
               complete:(nullable void (^)(JErrorCode errorCode, NSArray <JMomentReaction *> * _Nullable reactionList))completeBlock;

@end
