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

/// Comment id
@property (nonatomic, copy) NSString *commentId;
/// Moment id
@property (nonatomic, copy) NSString *momentId;
/// Replied comment id
@property (nonatomic, copy) NSString *parentCommentId;
/// Comment content
@property (nonatomic, copy) NSString *content;
/// User info of the commenter
@property (nonatomic, strong) JUserInfo *userInfo;
/// User info of the replied comment
@property (nonatomic, strong) JUserInfo *parentUserInfo;
/// Created time
@property (nonatomic, assign) long long createTime;

- (NSDictionary *)toDictionary;

+ (JMomentComment *)commentWith:(NSDictionary *)dictionary;

@end

NS_ASSUME_NONNULL_END
