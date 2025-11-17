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
@end
