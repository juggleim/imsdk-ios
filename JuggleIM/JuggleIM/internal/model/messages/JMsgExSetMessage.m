//
//  JMsgExSetMessage.m
//  JuggleIM
//
//  Created by Fei Li on 2025/1/13.
//

#import "JMsgExSetMessage.h"

#define jMsgExSetCmdType @"jg:msgexset"
#define jMsgId @"msg_id"
#define jExts @"exts"
#define jIsDel @"is_del"
#define jKey @"key"
#define jValue @"value"
#define jTimestamp @"timestamp"
#define jUser @"user"
#define jUserId @"user_id"
#define jNickname @"nickname"
#define jUserPortrait @"user_portrait"

@implementation JMsgExSetMessage

+ (NSString *)contentType {
    return jMsgExSetCmdType;
}

- (void)decode:(NSData *)data {
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    self.originalMessageId = json[jMsgId]?:@"";
    NSMutableArray <JMessageReactionItem *> *add = [NSMutableArray array];
    NSMutableArray <JMessageReactionItem *> *remove = [NSMutableArray array];
    NSArray *exts = json[jExts];
    BOOL isUpdate = NO;
    if ([exts isKindOfClass:[NSArray class]]) {
        for (NSDictionary *itemDic in exts) {
            id isDeleteID = itemDic[jIsDel];
            NSString *keyJson = itemDic[jKey];
            JUserInfo *user = [self userInfoFromJson:itemDic[jUser]];
            if ([isDeleteID isKindOfClass:[NSNumber class]]) {
                BOOL isDelete = [(NSNumber *)isDeleteID intValue];
                isUpdate = NO;
                if (isDelete) {
                    for (JMessageReactionItem *loopItem in remove) {
                        if ([loopItem.reactionId isEqualToString:keyJson]) {
                            isUpdate = YES;
                            NSMutableArray <JUserInfo *> *userInfoList = [loopItem.userInfoList mutableCopy];
                            [userInfoList addObject:user];
                            loopItem.userInfoList = [userInfoList copy];
                            break;
                        }
                    }
                    if (!isUpdate) {
                        JMessageReactionItem *reactionItem = [[JMessageReactionItem alloc] init];
                        reactionItem.reactionId = keyJson;
                        reactionItem.userInfoList = @[user];
                        [remove addObject:reactionItem];
                    }
                } else {
                    for (JMessageReactionItem *loopItem in add) {
                        if ([loopItem.reactionId isEqualToString:keyJson]) {
                            isUpdate = YES;
                            NSMutableArray <JUserInfo *> *userInfoList = [loopItem.userInfoList mutableCopy];
                            [userInfoList addObject:user];
                            loopItem.userInfoList = [userInfoList copy];
                            break;
                        }
                    }
                    if (!isUpdate) {
                        JMessageReactionItem *reactionItem = [[JMessageReactionItem alloc] init];
                        reactionItem.reactionId = keyJson;
                        reactionItem.userInfoList = @[user];
                        [add addObject:reactionItem];
                    }
                }
            }
        }
    }
    self.addItemList = add;
    self.removeItemList = remove;
}

+ (JMessageFlag)flags {
    return JMessageFlagIsStatus;
}

- (JUserInfo *)userInfoFromJson:(NSDictionary *)dic {
    JUserInfo *user = [[JUserInfo alloc] init];
    user.userId = dic[jUserId] ?: @"";
    user.userName = dic[jNickname] ?: @"";
    user.portrait = dic[jUserPortrait] ?: @"";
    return user;
}
@end
