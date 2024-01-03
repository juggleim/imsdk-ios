//
//  JSyncDB.m
//  JetIM
//
//  Created by Nathan on 2023/12/22.
//

#import "JProfileDB.h"

NSString *const kCreateSyncTable = @"CREATE TABLE IF NOT EXISTS profile ("
                                        "key VARCHAR (64) PRIMARY KEY,"
                                        "value VARCHAR (64)"
                                        ")";
NSString *const kGetValue = @"SELECT value FROM profile WHERE key = ?";
NSString *const kSetValue = @"INSERT OR REPLACE INTO profile (key, value) values (?, ?)";

NSString *const kConversationTime = @"conversation_time";
NSString *const kSendTime = @"send_time";
NSString *const kReceiveTime = @"receive_time";

NSString *const kValue = @"value";

@interface JProfileDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JProfileDB


- (void)createTables {
    [self.dbHelper executeUpdate:kCreateSyncTable withArgumentsInArray:nil];
}

- (long long)getConversationSyncTime {
    __block long long time = 0;
    [self.dbHelper executeQuery:kGetValue
           withArgumentsInArray:@[kConversationTime]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            time = [[resultSet stringForColumn:kValue] longLongValue];
        }
    }];
    return time;
}

- (long long)getMessageSendSyncTime {
    __block long long time = 0;
    [self.dbHelper executeQuery:kGetValue
           withArgumentsInArray:@[kSendTime]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            time = [[resultSet stringForColumn:kValue] longLongValue];
        }
    }];
    return time;
}

- (long long)getMessageReceiveSyncTime {
    __block long long time = 0;
    [self.dbHelper executeQuery:kGetValue
           withArgumentsInArray:@[kReceiveTime]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            time = [[resultSet stringForColumn:kValue] longLongValue];
        }
    }];
    return time;
}

- (void)setConversationSyncTime:(long long)time {
    [self.dbHelper executeUpdate:kSetValue
            withArgumentsInArray:@[kConversationTime, [NSString stringWithFormat:@"%lld", time]]];
}

- (void)setMessageSendSyncTime:(long long)time {
    [self.dbHelper executeUpdate:kSetValue
            withArgumentsInArray:@[kSendTime, [NSString stringWithFormat:@"%lld", time]]];
}

- (void)setMessageReceiveSyncTime:(long long)time {
    [self.dbHelper executeUpdate:kSetValue
            withArgumentsInArray:@[kReceiveTime, [NSString stringWithFormat:@"%lld", time]]];
}

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper {
    JProfileDB *db = [[JProfileDB alloc] init];
    db.dbHelper = dbHelper;
    return db;
}

@end
