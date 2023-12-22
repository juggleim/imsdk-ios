//
//  JMessageDB.m
//  Juggle
//
//  Created by Nathan on 2023/12/8.
//

#import "JMessageDB.h"

NSString *const kCreateMessageTable = @"CREATE TABLE IF NOT EXISTS message ("
                                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                        "conversation_type SMALLINT,"
                                        "conversation_id VARCHAR (64),"
                                        "type VARCHAR (64),"
                                        "message_uid VARCHAR (64),"
                                        "direction BOOLEAN,"
                                        "state SMALLINT,"
                                        "has_read BOOLEAN,"
                                        "timestamp INTEGER,"
                                        "sender VARCHAR (64),"
                                        "content TEXT,"
                                        "extra TEXT,"
                                        "message_index INTEGER,"
                                        "is_deleted BOOLEAN"
                                        ")";

@interface JMessageDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JMessageDB

- (void)createTables {
    [self.dbHelper executeUpdate:kCreateMessageTable withArgumentsInArray:nil];
}

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper {
    JMessageDB *db = [[JMessageDB alloc] init];
    db.dbHelper = dbHelper;
    return db;
}
@end
