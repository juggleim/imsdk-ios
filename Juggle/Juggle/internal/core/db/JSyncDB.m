//
//  JSyncDB.m
//  Juggle
//
//  Created by Nathan on 2023/12/22.
//

#import "JSyncDB.h"

NSString *const kCreateSyncTable = @"CREATE TABLE IF NOT EXISTS sync ("
                                        "user_id VARCHAR(64) PRIMARY KEY,"
                                        "conversation_time INTEGER,"
                                        "send_time INTEGER,"
                                        "receive_time INTEGER"
                                        ")";

@interface JSyncDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JSyncDB


- (void)createTables {
    [self.dbHelper executeUpdate:kCreateSyncTable withArgumentsInArray:nil];
}

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper {
    JSyncDB *db = [[JSyncDB alloc] init];
    db.dbHelper = dbHelper;
    return db;
}

@end
