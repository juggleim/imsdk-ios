//
//  JVersionDB.m
//  JuggleIM
//
//  Created by Fei Li on 2025/2/21.
//

#import "JVersionDB.h"
#import "JMessageDB.h"
#import "JUserInfoDB.h"
#import "JReactionDB.h"
#import "JConversationDB.h"
#import "JMomentDB.h"

#define jDBVersion 9

NSString *const jCreateVersionTable = @"CREATE TABLE IF NOT EXISTS version (v INTEGER)";
NSString *const jGetVersion = @"SELECT v FROM version";
NSString *const jInsertVersion = @"INSERT INTO version (v) VALUES (?)";
NSString *const jUpdateVersion = @"UPDATE version SET v = ?";

@interface JVersionDB()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JVersionDB

- (void)createTables {
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        [db executeUpdate:jCreateVersionTable];
        [db executeUpdate:jInsertVersion withArgumentsInArray:@[@(jDBVersion)]];
    }];
}

- (void)updateTables {
    [self.dbHelper executeUpdate:[JMessageDB addMessageClientUidIndex] withArgumentsInArray:nil];
    int version = [self getVersion];
    
    if (version < jDBVersion) {
        [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
            if (version < 1) {
                [db executeUpdate:jCreateVersionTable];
                [db executeUpdate:jInsertVersion withArgumentsInArray:@[@(0)]];
                [db executeUpdate:[JMessageDB alterTableAddFlags]];
                [db executeUpdate:[JUserInfoDB alterUserTableAddType]];
                [db executeUpdate:[JReactionDB createReactionTable]];
            }
            if (version < 2) {
                [db executeUpdate:[JUserInfoDB createGroupMemberTable]];
                [db executeUpdate:[JUserInfoDB createGroupMemberIndex]];
                [db executeUpdate:[JConversationDB createConversationTagTable]];
                [db executeUpdate:[JConversationDB createConversationTagIndex]];
            }
            if (version < 3) {
                [db executeUpdate:[JMessageDB addConversationTSIndex]];
            }
            if (version < 4) {
                [db executeUpdate:[JMessageDB alterTableAddLifeTime]];
                [db executeUpdate:[JMessageDB alterTableAddLifeTimeAfterRead]];
                [db executeUpdate:[JMessageDB alterTableAddDestroyTime]];
            }
            if (version < 5) {
                [db executeUpdate:[JUserInfoDB alterUserTableAddUpdatedTime]];
                [db executeUpdate:[JUserInfoDB alterGroupTableAddUpdatedTime]];
                [db executeUpdate:[JUserInfoDB alterGroupMemberTableAddUpdatedTime]];
            }
            if (version < 6) {
                [db executeUpdate:[JMessageDB addDTConversationTSIndex]];
            }
            if (version < 7) {
                [db executeUpdate:[JMessageDB alterTableAddReadTime]];
            }
            if (version < 8) {
                [db executeUpdate:[JMessageDB alterTableAddSubChannel]];
                [db executeUpdate:[JConversationDB alterConversationInfoAddSubChannel]];
                [db executeUpdate:[JConversationDB alterConversationTagAddSubChannel]];
                [db executeUpdate:[JConversationDB dropConversationIndex1]];
                [db executeUpdate:[JConversationDB dropConversationTagIndex1]];
                [db executeUpdate:[JConversationDB addConversationIndex2]];
                [db executeUpdate:[JConversationDB addConversationTagIndex2]];
                [db executeUpdate:[JMessageDB addDTConversationTSIndex2]];
            }
            if (version < 9) {
                [db executeUpdate:[JMomentDB createMomentTable]];
            }
            [db executeUpdate:jUpdateVersion withArgumentsInArray:@[@(jDBVersion)]];
        }];
    }
}

- (int)getVersion {
    __block int version = 0;
    [self.dbHelper executeQuery:jGetVersion
           withArgumentsInArray:nil
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            version = [resultSet intForColumn:@"v"];
        }
    }];
    return version;
}

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper {
    if (self = [super init]) {
        self.dbHelper = dbHelper;
    }
    return self;
}

@end
