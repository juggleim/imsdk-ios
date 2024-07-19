//
//  JUserInfoDB.m
// JuggleIM
//
//  Created by Nathan on 2024/3/12.
//

#import "JUserInfoDB.h"

//user 最新版本
#define jUserTableVersion 1
//NSUserDefault 中保存 user 数据库版本的 key
#define jUserTableVersionKey @"UserVersion"
//group 最新版本
#define jGroupTableVersion 1
//NSUserDefault 中保存 group 数据库版本的 key
#define jGroupTableVersionKey @"GroupVersion"

NSString *const jCreateUserTable = @"CREATE TABLE IF NOT EXISTS user ("
                                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                        "user_id VARCHAR (64),"
                                        "name VARCHAR (64),"
                                        "portrait TEXT,"
                                        "extension TEXT"
                                        ")";
NSString *const jCreateGroupTable = @"CREATE TABLE IF NOT EXISTS group_info ("
                                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                        "group_id VARCHAR (64),"
                                        "name VARCHAR (64),"
                                        "portrait TEXT,"
                                        "extension TEXT"
                                        ")";
NSString *const jCreateUserIndex = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_user ON user(user_id)";
NSString *const jCreateGroupIndex = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_group ON group_info(group_id)";
NSString *const jGetUserInfo = @"SELECT * FROM user WHERE user_id = ?";
NSString *const jGetGroupInfo = @"SELECT * FROM group_info WHERE group_id = ?";
NSString *const jInsertUserInfo = @"INSERT OR REPLACE INTO user (user_id, name, portrait, extension) VALUES (?, ?, ?, ?)";
NSString *const jInsertGroupInfo = @"INSERT OR REPLACE INTO group_info (group_id, name, portrait, extension) VALUES (?, ?, ?, ?)";
NSString *const jColUserId = @"user_id";
NSString *const jColGroupId = @"group_id";
NSString *const jColName = @"name";
NSString *const jColPortrait = @"portrait";
NSString *const jColExtension = @"extension";

@interface JUserInfoDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JUserInfoDB

- (void)createTables {
    [self.dbHelper executeUpdate:jCreateUserTable withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:jCreateGroupTable withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:jCreateUserIndex withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:jCreateGroupIndex withArgumentsInArray:nil];
    [[NSUserDefaults standardUserDefaults] setObject:@(jUserTableVersion) forKey:jUserTableVersionKey];
    [[NSUserDefaults standardUserDefaults] setObject:@(jGroupTableVersion) forKey:jGroupTableVersionKey];
}

- (void)updateTables {
    NSNumber *existedVersion = [[NSUserDefaults standardUserDefaults] objectForKey:jUserTableVersionKey];
    if (jUserTableVersion > existedVersion.intValue) {
        //update table
        
        [[NSUserDefaults standardUserDefaults] setObject:@(jUserTableVersion) forKey:jUserTableVersionKey];
    }

    existedVersion = [[NSUserDefaults standardUserDefaults] objectForKey:jGroupTableVersionKey];
    if (jGroupTableVersion > existedVersion.intValue) {
        //update table
        
        [[NSUserDefaults standardUserDefaults] setObject:@(jGroupTableVersion) forKey:jGroupTableVersionKey];
    }
}

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper {
    if (self = [super init]) {
        self.dbHelper = dbHelper;
    }
    return self;
}

- (JUserInfo *)getUserInfo:(NSString *)userId {
    if (userId.length == 0) {
        return nil;
    }
    __block JUserInfo *userInfo = nil;
    [self.dbHelper executeQuery:jGetUserInfo
           withArgumentsInArray:@[userId]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            userInfo = [self userInfoWith:resultSet];
        }
    }];
    return userInfo;
}

- (JGroupInfo *)getGroupInfo:(NSString *)groupId {
    if (groupId.length == 0) {
        return nil;
    }
    __block JGroupInfo *groupInfo = nil;
    [self.dbHelper executeQuery:jGetGroupInfo
           withArgumentsInArray:@[groupId]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            groupInfo = [self groupInfoWith:resultSet];
        }
    }];
    return groupInfo;
}

- (void)insertUserInfos:(NSArray <JUserInfo *> *)userInfos {
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        [userInfos enumerateObjectsUsingBlock:^(JUserInfo * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            NSString *userId = obj.userId?:@"";
            NSString *name = obj.userName?:@"";
            NSString *portrait = obj.portrait?:@"";
            NSString *extension = [self stringFromDic:obj.extraDic];
            [db executeUpdate:jInsertUserInfo withArgumentsInArray:@[userId, name, portrait, extension]];
        }];
    }];
}

- (void)insertGroupInfos:(NSArray <JGroupInfo *> *)groupInfos {
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        [groupInfos enumerateObjectsUsingBlock:^(JGroupInfo * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            NSString *groupId = obj.groupId?:@"";
            NSString *name = obj.groupName?:@"";
            NSString *portrait = obj.portrait?:@"";
            NSString *extension = [self stringFromDic:obj.extraDic];
            [db executeUpdate:jInsertGroupInfo withArgumentsInArray:@[groupId, name, portrait, extension]];
        }];
    }];
}

#pragma mark - internal
- (JUserInfo *)userInfoWith:(JFMResultSet *)rs {
    JUserInfo *userInfo = [[JUserInfo alloc] init];
    userInfo.userId = [rs stringForColumn:jColUserId];
    userInfo.userName = [rs stringForColumn:jColName];
    userInfo.portrait = [rs stringForColumn:jColPortrait];
    NSString *extra = [rs stringForColumn:jColExtension];
    userInfo.extraDic = [self dicFromString:extra];
    return userInfo;
}

- (JGroupInfo *)groupInfoWith:(JFMResultSet *)rs {
    JGroupInfo *groupInfo = [[JGroupInfo alloc] init];
    groupInfo.groupId = [rs stringForColumn:jColGroupId];
    groupInfo.groupName = [rs stringForColumn:jColName];
    groupInfo.portrait = [rs stringForColumn:jColPortrait];
    NSString *extra = [rs stringForColumn:jColExtension];
    groupInfo.extraDic = [self dicFromString:extra];
    return groupInfo;
}

- (NSString *)stringFromDic:(NSDictionary *)dic {
    if (dic.count == 0) {
        return @"";
    }
    NSData *d = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    return [[NSString alloc] initWithData:d encoding:NSUTF8StringEncoding];
}

- (NSDictionary *)dicFromString:(NSString *)string {
    if (string.length == 0) {
        return nil;
    }
    NSData *data = [string dataUsingEncoding:NSUTF8StringEncoding];
    return [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
}

@end
