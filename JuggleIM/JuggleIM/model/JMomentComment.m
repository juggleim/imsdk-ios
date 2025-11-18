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
    dict[@"commentId"] = self.commentId;
    dict[@"momentId"] = self.momentId;
    dict[@"parentCommentId"] = self.parentCommentId ?: [NSNull null];
    dict[@"content"] = self.content;
    dict[@"userInfo"] = [self.userInfo toDictionary];
    dict[@"parentUserInfo"] = [self.parentUserInfo toDictionary] ?: [NSNull null];
    dict[@"createTime"] = @(self.createTime);
    dict[@"updateTime"] = @(self.updateTime);
    return dict.copy;
}
@end
