//
//  JReactionDB.m
//  JuggleIM
//
//  Created by Fei Li on 2025/2/7.
//

#import "JReactionDB.h"

NSString *const jCreateReactionTable = @"CREATE TABLE IF NOT EXISTS reaction ("
                                        "messageId VARCHAR (64) PRIMARY KEY,"
                                        "reactions TEXT"
                                        ")";
NSString *const jGetReactions = @"SELECT * FROM reaction WHERE messageId IN ";
NSString *const jSetReactions = @"INSERT OR REPLACE INTO reaction (messageId, reactions) VALUES (?, ?)";

NSString *const jReactionId = @"reactionId";
NSString *const jUserInfoList = @"userInfoList";

@interface JReactionDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JReactionDB

- (void)createTables {
    [self.dbHelper executeUpdate:jCreateReactionTable withArgumentsInArray:nil];
}

- (void)updateTables {
}

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper {
    if (self = [super init]) {
        self.dbHelper = dbHelper;
    }
    return self;
}

- (NSArray<JMessageReaction *> *)getMessageReactions:(NSArray<NSString *> *)messageIds {
    NSMutableArray<JMessageReaction *> *result = [NSMutableArray array];
    if (messageIds.count == 0) {
        return [result copy];
    }
    NSString *sql = jGetReactions;
    sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:messageIds.count]];
    [self.dbHelper executeQuery:sql withArgumentsInArray:messageIds syncResult:^(JFMResultSet * _Nonnull resultSet) {
        while ([resultSet next]) {
            JMessageReaction *reaction = [self reactionWith:resultSet];
            [result addObject:reaction];
        }
    }];
    return [result copy];
}

- (void)setMessageReactions:(NSArray<JMessageReaction *> *)reactions {
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        [reactions enumerateObjectsUsingBlock:^(JMessageReaction * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if (obj.messageId.length > 0 && obj.itemList.count > 0) {
                [db executeUpdate:jSetReactions, obj.messageId, [self jsonWith:obj.itemList]];
            }
        }];
    }];
}

+ (NSString *)createReactionTable {
    return jCreateReactionTable;
}

- (NSString *)jsonWith:(NSArray <JMessageReactionItem *> *)items {
    NSMutableArray *jsonArray = [NSMutableArray array];
    [items enumerateObjectsUsingBlock:^(JMessageReactionItem * _Nonnull item, NSUInteger idx, BOOL * _Nonnull stop) {
        NSMutableDictionary *jsonItem = [NSMutableDictionary dictionary];
        [jsonItem setObject:item.reactionId forKey:jReactionId];
        NSMutableArray *jsonUserList = [NSMutableArray array];
        [item.userInfoList enumerateObjectsUsingBlock:^(JUserInfo * _Nonnull userInfo, NSUInteger idx, BOOL * _Nonnull stop) {
            NSMutableDictionary *jsonUser = [NSMutableDictionary dictionary];
            if (userInfo.userId.length > 0) {
                [jsonUser setObject:userInfo.userId forKey:@"id"];
            }
            if (userInfo.userName.length > 0) {
                [jsonUser setObject:userInfo.userName forKey:@"name"];
            }
            if (userInfo.portrait.length > 0) {
                [jsonUser setObject:userInfo.portrait forKey:@"portrait"];
            }
            [jsonUserList addObject:jsonUser];
        }];
        [jsonItem setObject:jsonUserList forKey:jUserInfoList];
        [jsonArray addObject:jsonItem];
    }];
    NSData *d = [NSJSONSerialization dataWithJSONObject:jsonArray options:kNilOptions error:nil];
    NSString *s = [[NSString alloc] initWithData:d encoding:NSUTF8StringEncoding];
    return s;
}

- (JMessageReaction *)reactionWith:(JFMResultSet *)rs {
    JMessageReaction *reaction = [[JMessageReaction alloc] init];
    reaction.messageId = [rs stringForColumn:@"messageId"];
    NSString *reactionsStr = [rs stringForColumn:@"reactions"];
    
    if (reactionsStr.length == 0) {
        return reaction;
    }
    NSData *data = [reactionsStr dataUsingEncoding:NSUTF8StringEncoding];
    NSArray *reactions = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    NSMutableArray <JMessageReactionItem *> *reactionItemList = [NSMutableArray array];
    for (NSDictionary *reactionItemJson in reactions) {
        JMessageReactionItem *reactionItem = [[JMessageReactionItem alloc] init];
        reactionItem.reactionId = reactionItemJson[jReactionId]?:@"";
        NSArray *userInfoListJson = reactionItemJson[jUserInfoList];
        NSMutableArray *userInfoList = [NSMutableArray array];
        for (NSDictionary *userInfoJson in userInfoListJson) {
            JUserInfo *userInfo = [[JUserInfo alloc] init];
            userInfo.userId = userInfoJson[@"id"]?:@"";
            userInfo.userName = userInfoJson[@"name"]?:@"";
            userInfo.portrait = userInfoJson[@"portrait"]?:@"";
            [userInfoList addObject:userInfo];
        }
        reactionItem.userInfoList = [userInfoList copy];
        [reactionItemList addObject:reactionItem];
    }
    reaction.itemList = reactionItemList;
    return reaction;
}

@end
