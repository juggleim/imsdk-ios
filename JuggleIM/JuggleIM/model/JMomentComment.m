//
//  JMomentComment.m
//  JuggleIM
//
//  Created by Fei Li on 2025/11/11.
//

#import "JMomentComment.h"

@implementation JMomentComment
- (NSDictionary *)toDictionary {
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    dict[@"comment_id"] = self.commentId;
    dict[@"moment_id"] = self.momentId;
    if (self.parentCommentId) {
        dict[@"parent_comment_id"] = self.parentCommentId;
    }
    dict[@"content"] = @{@"text": self.content};
    dict[@"user_info"] = [self.userInfo toDictionary];
    if (self.parentUserInfo) {
        dict[@"parent_user_info"] = [self.parentUserInfo toDictionary];
    }
    dict[@"comment_time"] = @(self.createTime);
    return dict.copy;
}

+ (JMomentComment *)commentWith:(NSDictionary *)dictionary {
    if (!dictionary) return nil;
    JMomentComment *comment = [[self alloc] init];
    
    comment.commentId = dictionary[@"comment_id"];
    comment.momentId = dictionary[@"moment_id"];
    comment.parentCommentId = dictionary[@"parent_comment_id"];
    comment.content = dictionary[@"content"][@"text"];
    comment.parentUserInfo = [JUserInfo userInfoWith:dictionary[@"parent_user_info"]];
    comment.userInfo = [JUserInfo userInfoWith:dictionary[@"user_info"]];
    comment.createTime = [dictionary[@"comment_time"] longLongValue];
    
    return comment;
}
@end
