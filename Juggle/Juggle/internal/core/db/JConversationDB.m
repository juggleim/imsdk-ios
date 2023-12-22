//
//  JConversationDB.m
//  Juggle
//
//  Created by Nathan on 2023/12/8.
//

#import "JConversationDB.h"

NSString *const kCreateConversationTable = @"CREATE TABLE IF NOT EXISTS conversation_info ("
                                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                        "conversation_type SMALLINT,"
                                        "conversation_id VARCHAR (64),"
                                        "draft TEXT,"
                                        "timestamp INTEGER,"
                                        "last_message_id INTEGER,"
                                        "last_read_message_index INTEGER,"
                                        "is_top BOOLEAN,"
                                        "top_time INTEGER,"
                                        "mute BOOLEAN,"
                                        "last_mention_message_id INTEGER"
                                        ")";

@interface JConversationDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JConversationDB

- (void)createTables {
    [self.dbHelper executeUpdate:kCreateConversationTable withArgumentsInArray:nil];
}

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper {
    JConversationDB *db = [[JConversationDB alloc] init];
    db.dbHelper = dbHelper;
    return db;
}
@end
