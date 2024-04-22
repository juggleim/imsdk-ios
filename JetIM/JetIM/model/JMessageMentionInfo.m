//
//  JMentionInfo.m
//  JetIM
//
//  Created by Nathan on 2024/4/19.
//

#import "JMessageMentionInfo.h"

#define jMentionType @"mention_type"
#define jTargetUsers @"target_users"
#define jMentionUserId @"id"
#define jMentionUserName @"name"
#define jMentionUserPortrait @"portrait"

@implementation JMessageMentionInfo

- (NSString *)encodeToJson {
    NSMutableDictionary *dic = [NSMutableDictionary dictionary];
    [dic setObject:@(self.type) forKey:jMentionType];
    if (self.targetUsers.count > 0) {
        NSMutableArray *users = [NSMutableArray array];
        for (JUserInfo *user in self.targetUsers) {
            NSMutableDictionary *userDic = [NSMutableDictionary dictionary];
            [userDic setObject:user.userId?:@"" forKey:jMentionUserId];
            [userDic setObject:user.userName?:@"" forKey:jMentionUserName];
            [userDic setObject:user.portrait?:@"" forKey:jMentionUserPortrait];
            [users addObject:userDic];
        }
        [dic setObject:users forKey:jTargetUsers];
    }
    NSData *d = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    NSString *s = [[NSString alloc] initWithData:d encoding:NSUTF8StringEncoding];
    return s;
}

+ (instancetype)decodeFromJson:(NSString *)json {
    if (json.length == 0) {
        return nil;
    }
    NSData *data = [json dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    JMessageMentionInfo *info = [[JMessageMentionInfo alloc] init];
    id typeId = dic[jMentionType];
    if ([typeId isKindOfClass:[NSNumber class]]) {
        info.type = [(NSNumber *)typeId unsignedIntValue];
    }
    NSArray *userJson = dic[jTargetUsers];
    if (userJson.count > 0) {
        NSMutableArray <JUserInfo *> *users = [NSMutableArray array];
        for (NSDictionary *userDic in userJson) {
            JUserInfo *userInfo = [[JUserInfo alloc] init];
            userInfo.userId = userDic[jMentionUserId]?:@"";
            userInfo.userName = userDic[jMentionUserName]?:@"";
            userInfo.portrait = userDic[jMentionUserPortrait]?:@"";
            [users addObject:userInfo];
        }
        info.targetUsers = users;
    }
    return info;
}

@end
