//
//  JMoment.m
//  JuggleIM
//
//  Created by Fei Li on 2025/11/11.
//

#import "JMoment.h"

@implementation JMoment

+ (JMoment *)momentWith:(NSDictionary *)dictionary {
    if (!dictionary || dictionary.count == 0) {
        return nil;
    }
    JMoment *moment = [[JMoment alloc] init];
    
    moment.momentId = dictionary[@"moment_id"];
    NSDictionary *contentDict = dictionary[@"content"];
    moment.content = contentDict[@"text"];
    NSArray *mediasDictArray = contentDict[@"medias"];
    if ([mediasDictArray isKindOfClass:[NSArray class]]) {
        NSMutableArray *mediaModels = [NSMutableArray arrayWithCapacity:mediasDictArray.count];
        for (NSDictionary *mediaDict in mediasDictArray) {
            JMomentMedia *media = [JMomentMedia mediaWith:mediaDict];
            if (media) [mediaModels addObject:media];
        }
        moment.mediaArray = mediaModels.copy;
    }
    
    NSDictionary *userInfoDict = dictionary[@"user_info"];
    moment.userInfo = [JUserInfo userInfoWith:userInfoDict];
    
    NSArray *reactionsDictArray = dictionary[@"reactions"];
    if ([reactionsDictArray isKindOfClass:[NSArray class]]) {
        moment.reactionArray = [JMomentReaction mergeReactionListWithJson:reactionsDictArray];
    }
    
    NSArray *commentsDictArray = dictionary[@"top_comments"];
    if ([commentsDictArray isKindOfClass:[NSArray class]]) {
        NSMutableArray *commentModels = [NSMutableArray arrayWithCapacity:commentsDictArray.count];
        for (NSDictionary *commentDict in commentsDictArray) {
            JMomentComment *comment = [JMomentComment commentWith:commentDict];
            if (comment) [commentModels addObject:comment];
        }
        moment.commentArray = commentModels.copy;
    }
    moment.createTime = [dictionary[@"moment_time"] longLongValue];
    
    return moment;
}

@end
