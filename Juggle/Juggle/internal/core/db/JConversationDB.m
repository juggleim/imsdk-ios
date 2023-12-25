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
NSString *const kInsertConversation = @"INSERT OR REPLACE INTO conversation_info"
                                        "(conversation_type, conversation_id, timestamp, last_message_id,"
                                        "last_read_message_index, is_top, mute, last_mention_message_id)"
                                        "values (?, ?, ?, ?, ?, ?, ?, ?)";

@interface JConversationDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JConversationDB

- (void)createTables {
    [self.dbHelper executeUpdate:kCreateConversationTable withArgumentsInArray:nil];
}

- (void)insertConversations:(NSArray<JConcreteConversationInfo *> *)conversations {
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        [conversations enumerateObjectsUsingBlock:^(JConcreteConversationInfo * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            [db executeUpdate:kInsertConversation, @(obj.conversation.conversationType), obj.conversation.conversationId, @(obj.updateTime), obj.lastMessage.messageId, @(obj.lastReadMessageIndex), @(obj.isTop), @(obj.mute), @(0)];//TODO: mention
        }];
    }];
}

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper {
    JConversationDB *db = [[JConversationDB alloc] init];
    db.dbHelper = dbHelper;
    return db;
}
@end
