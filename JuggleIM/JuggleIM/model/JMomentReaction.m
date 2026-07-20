//
//  JMomentReaction.m
//  JuggleIM
//
//  Created by Fei Li on 2025/11/11.
//

#import "JMomentReaction.h"

@implementation JMomentReaction
- (NSDictionary *)toDictionary {
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    dict[@"key"] = self.key;
    NSMutableArray *userDicts = [NSMutableArray array];
    for (JUserInfo *user in self.userArray) {
        [userDicts addObject:[user toDictionary]];
    }
    dict[@"userArray"] = userDicts.copy;
    return dict.copy;
}

+ (NSArray<JMomentReaction *> *)mergeReactionListWithJson:(NSArray *)jsonArray {
    NSMutableArray *reactionModels = [NSMutableArray array];// Actual reaction array
    NSMutableDictionary <NSString *, NSMutableArray <JUserInfo *> *> *keyUserInfosDic = [NSMutableDictionary dictionary];// key/user list
    for (NSDictionary *reactionDict in jsonArray) {
        NSString *key = reactionDict[@"key"];
        if (key.length == 0) {
            continue;;
        }
        NSMutableArray *userInfos = keyUserInfosDic[key];
        if (!userInfos) {
            userInfos = [NSMutableArray array];
        }
        JUserInfo *userInfo = [JUserInfo userInfoWith:reactionDict[@"user_info"]];
        [userInfos addObject:userInfo];
        [keyUserInfosDic setObject:userInfos forKey:key];
    }
    [keyUserInfosDic enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull key, NSMutableArray<JUserInfo *> * _Nonnull userInfoList, BOOL * _Nonnull stop) {
        JMomentReaction *reaction = [JMomentReaction new];
        reaction.key = key;
        reaction.userArray = [userInfoList copy];
        [reactionModels addObject:reaction];
    }];
    return reactionModels.copy;
}
@end
