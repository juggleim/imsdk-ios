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
                                        "extension TEXT,"
                                        "type SMALLINT,"
                                        "updated_time INTEGER"
                                        ")";
NSString *const jCreateGroupTable = @"CREATE TABLE IF NOT EXISTS group_info ("
                                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                        "group_id VARCHAR (64),"
                                        "name VARCHAR (64),"
                                        "portrait TEXT,"
                                        "extension TEXT,"
                                        "updated_time INTEGER"
                                        ")";
NSString *const jCreateGroupMemberTable = @"CREATE TABLE IF NOT EXISTS group_member ("
                                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                        "group_id VARCHAR (64),"
                                        "user_id VARCHAR (64),"
                                        "display_name VARCHAR (64),"
                                        "extension TEXT,"
                                        "updated_time INTERGER"
                                        ")";
NSString *const jCreateUserIndex = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_user ON user(user_id)";
NSString *const jCreateGroupIndex = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_group ON group_info(group_id)";
NSString *const jCreateGroupMemberIndex = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_group_member ON group_member(group_id, user_id)";
NSString *const kAlterAddUserType = @"ALTER TABLE user ADD COLUMN type SMALLINT";
NSString *const jAlterAddUserUpdatedTime = @"ALTER TABLE user ADD COLUMN updated_time INTEGER";
NSString *const jAlterAddGroupUpdatedTime = @"ALTER TABLE group_info ADD COLUMN updated_time INTEGER";
NSString *const jAlterAddGroupMemberUpdatedTime = @"ALTER TABLE group_member ADD COLUMN updated_time INTEGER";

NSString *const jGetUserInfo = @"SELECT * FROM user WHERE user_id = ?";
NSString *const jGetGroupInfo = @"SELECT * FROM group_info WHERE group_id = ?";
NSString *const jGetGroupMember = @"SELECT * FROM group_member WHERE group_id = ? AND user_id = ?";
NSString *const jInsertUserInfo = @"INSERT OR REPLACE INTO user (user_id, name, portrait, extension, type, updated_time) VALUES (?, ?, ?, ?, ?, ?)";
NSString *const jInsertGroupInfo = @"INSERT OR REPLACE INTO group_info (group_id, name, portrait, extension, updated_time) VALUES (?, ?, ?, ?, ?)";
NSString *const jInsertGroupMembers = @"INSERT OR REPLACE INTO group_member (group_id, user_id, display_name, extension, updated_time) VALUES (?, ?, ?, ?, ?)";
NSString *const jGetUserInfoList = @"SELECT * FROM user WHERE user_id IN ";
NSString *const jGetGroupList = @"SELECT * FROM group_info WHERE group_id IN ";
NSString *const jColUserId = @"user_id";
NSString *const jColGroupId = @"group_id";
NSString *const jColName = @"name";
NSString *const jColPortrait = @"portrait";
NSString *const jColExtension = @"extension";
NSString *const jColType = @"type";
NSString *const jColDisplayName = @"display_name";
NSString *const jColUpdatedTime = @"updated_time";

@interface JUserInfoDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JUserInfoDB

- (void)createTables {
    [self.dbHelper executeUpdate:jCreateUserTable withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:jCreateGroupTable withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:jCreateGroupMemberTable withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:jCreateUserIndex withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:jCreateGroupIndex withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:jCreateGroupMemberIndex withArgumentsInArray:nil];
    [[NSUserDefaults standardUserDefaults] setObject:@(jUserTableVersion) forKey:jUserTableVersionKey];
    [[NSUserDefaults standardUserDefaults] setObject:@(jGroupTableVersion) forKey:jGroupTableVersionKey];
}

- (void)updateTables {
    NSNumber *existedVersionNumber = [[NSUserDefaults standardUserDefaults] objectForKey:jUserTableVersionKey];
    int existedVersion = existedVersionNumber.intValue;
    if (jUserTableVersion > existedVersion) {
        //update table
        [[NSUserDefaults standardUserDefaults] setObject:@(jUserTableVersion) forKey:jUserTableVersionKey];
    }

    existedVersionNumber = [[NSUserDefaults standardUserDefaults] objectForKey:jGroupTableVersionKey];
    existedVersion = existedVersionNumber.intValue;
    if (jGroupTableVersion > existedVersion) {
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

- (JGroupMember *)getGroupMemberIn:(NSString *)groupId userId:(NSString *)userId {
    __block JGroupMember *groupMember = nil;
    if (groupId.length == 0 || userId.length == 0) {
        return groupMember;
    }
    [self.dbHelper executeQuery:jGetGroupMember withArgumentsInArray:@[groupId, userId] syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            groupMember = [self groupMemberWith:resultSet];
        }
    }];
    return groupMember;
}

- (void)insertUserInfos:(NSArray <JUserInfo *> *)userInfos {
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        [userInfos enumerateObjectsUsingBlock:^(JUserInfo * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            NSString *userId = obj.userId?:@"";
            NSString *name = obj.userName?:@"";
            NSString *portrait = obj.portrait?:@"";
            NSString *extension = [self stringFromDic:obj.extraDic];
            JUserInfo *userInfo = nil;
            JFMResultSet *resultSet = [db executeQuery:jGetUserInfo, userId];
            if ([resultSet next]) {
                userInfo = [self userInfoWith:resultSet];
            }
            [resultSet close];
            if (!userInfo || obj.updatedTime > userInfo.updatedTime) {
                [db executeUpdate:jInsertUserInfo withArgumentsInArray:@[userId, name, portrait, extension, @(obj.type), @(obj.updatedTime)]];
            }
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
            JGroupInfo *groupInfo = nil;
            JFMResultSet *resultSet = [db executeQuery:jGetGroupInfo, groupId];
            if ([resultSet next]) {
                groupInfo = [self groupInfoWith:resultSet];
            }
            [resultSet close];
            if (!groupInfo || obj.updatedTime > groupInfo.updatedTime) {
                [db executeUpdate:jInsertGroupInfo withArgumentsInArray:@[groupId, name, portrait, extension, @(obj.updatedTime)]];
            }
        }];
    }];
}

- (void)insertGroupMembers:(NSArray<JGroupMember *> *)members {
    if (members.count == 0) {
        return;
    }
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        [members enumerateObjectsUsingBlock:^(JGroupMember * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            NSString *groupId = obj.groupId?:@"";
            NSString *userId = obj.userId?:@"";
            NSString *displayName = obj.groupDisplayName?:@"";
            NSString *extension = [self stringFromDic:obj.extraDic];
            JGroupMember *member = nil;
            JFMResultSet *rs = [db executeQuery:jGetGroupMember, groupId, userId];
            if ([rs next]) {
                member = [self groupMemberWith:rs];
            }
            [rs close];
            if (!member || obj.updatedTime > member.updatedTime) {
                [db executeUpdate:jInsertGroupMembers withArgumentsInArray:@[groupId, userId, displayName, extension, @(obj.updatedTime)]];
            }
        }];
    }];
}

- (NSArray<JUserInfo *> *)getUserInfoList:(NSArray<NSString *> *)userIdList {
    NSMutableArray <JUserInfo *> *userList = [NSMutableArray array];
    if (userIdList.count == 0) {
        return [userList copy];
    }
    NSString *sql = jGetUserInfoList;
    sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:userIdList.count]];
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:userIdList
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        while ([resultSet next]) {
            JUserInfo *userInfo = [self userInfoWith:resultSet];
            [userList addObject:userInfo];
        }
    }];
    return [userList copy];
}

- (NSArray<JGroupInfo *> *)getGroupInfoList:(NSArray<NSString *> *)groupIdList {
    NSMutableArray <JGroupInfo *> *groupList = [NSMutableArray array];
    if (groupIdList.count == 0) {
        return [groupList copy];
    }
    NSString *sql = jGetGroupList;
    sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:groupIdList.count]];
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:groupIdList
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        while ([resultSet next]) {
            JGroupInfo *groupInfo = [self groupInfoWith:resultSet];
            [groupList addObject:groupInfo];
        }
    }];
    return [groupList copy];
}

+ (NSString *)alterUserTableAddType {
    return kAlterAddUserType;
}

+ (NSString *)createGroupMemberTable {
    return jCreateGroupMemberTable;
}

+ (NSString *)createGroupMemberIndex {
    return jCreateGroupMemberIndex;
}

+ (NSString *)alterUserTableAddUpdatedTime {
    return jAlterAddUserUpdatedTime;
}

+ (NSString *)alterGroupTableAddUpdatedTime {
    return jAlterAddGroupUpdatedTime;
}

+ (NSString *)alterGroupMemberTableAddUpdatedTime {
    return jAlterAddGroupMemberUpdatedTime;
}

#pragma mark - internal
- (JUserInfo *)userInfoWith:(JFMResultSet *)rs {
    JUserInfo *userInfo = [[JUserInfo alloc] init];
    userInfo.userId = [rs stringForColumn:jColUserId];
    userInfo.userName = [rs stringForColumn:jColName];
    userInfo.portrait = [rs stringForColumn:jColPortrait];
    NSString *extra = [rs stringForColumn:jColExtension];
    userInfo.extraDic = [self dicFromString:extra];
    userInfo.type = [rs intForColumn:jColType];
    userInfo.updatedTime = [rs longLongIntForColumn:jColUpdatedTime];
    return userInfo;
}

- (JGroupInfo *)groupInfoWith:(JFMResultSet *)rs {
    JGroupInfo *groupInfo = [[JGroupInfo alloc] init];
    groupInfo.groupId = [rs stringForColumn:jColGroupId];
    groupInfo.groupName = [rs stringForColumn:jColName];
    groupInfo.portrait = [rs stringForColumn:jColPortrait];
    NSString *extra = [rs stringForColumn:jColExtension];
    groupInfo.extraDic = [self dicFromString:extra];
    groupInfo.updatedTime = [rs longLongIntForColumn:jColUpdatedTime];
    return groupInfo;
}

- (JGroupMember *)groupMemberWith:(JFMResultSet *)rs {
    JGroupMember *groupMember = [[JGroupMember alloc] init];
    groupMember.groupId = [rs stringForColumn:jColGroupId];
    groupMember.userId = [rs stringForColumn:jColUserId];
    groupMember.groupDisplayName = [rs stringForColumn:jColDisplayName];
    NSString *extra = [rs stringForColumn:jColExtension];
    groupMember.extraDic = [self dicFromString:extra];
    groupMember.updatedTime = [rs longLongIntForColumn:jColUpdatedTime];
    return groupMember;
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
