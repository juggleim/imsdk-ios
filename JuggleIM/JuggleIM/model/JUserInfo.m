//
//  JUserInfo.m
// JuggleIM
//
//  Created by Nathan on 2024/3/9.
//

#import "JUserInfo.h"

@implementation JUserInfo
- (NSDictionary *)toDictionary {
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    if (self.userId.length > 0) {
        dict[@"user_id"] = self.userId;
    }
    if (self.userName.length > 0) {
        dict[@"nickname"] = self.userName;
    }
    if (self.portrait.length > 0) {
        dict[@"avatar"] = self.portrait;
    }
    if (self.extraDic.count > 0) {
        dict[@"ext_fields"] = self.extraDic;
    }
    dict[@"user_type"] = @(self.type);
    dict[@"updated_time"] = @(self.updatedTime);
    return dict.copy;
}

+ (JUserInfo *)userInfoWith:(NSDictionary *)dictionary {
    if (!dictionary || [dictionary isKindOfClass:[NSNull class]] || dictionary.count == 0) {
        return nil;
    }
    JUserInfo *userInfo = [JUserInfo new];
    if (dictionary) {
        userInfo.userId = dictionary[@"user_id"];
        userInfo.userName = dictionary[@"nickname"];
        userInfo.portrait = dictionary[@"avatar"];
        userInfo.extraDic = dictionary[@"ext_fields"];
        userInfo.type = [dictionary[@"user_type"] integerValue];
        userInfo.updatedTime = [dictionary[@"updated_time"] longLongValue];
    }
    return userInfo;
}

@end
