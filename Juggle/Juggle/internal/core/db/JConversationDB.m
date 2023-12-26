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
                                        "last_message_id VARCHAR (64),"
                                        "last_read_message_index INTEGER,"
                                        "is_top BOOLEAN,"
                                        "top_time INTEGER,"
                                        "mute BOOLEAN,"
                                        "last_mention_message_id VARCHAR (64)"
                                        ")";
NSString *const kCreateConversationIndex = @"CREATE INDEX IF NOT EXISTS idx_conversation ON conversation_info(conversation_type, conversation_id)";
NSString *const kInsertConversation = @"INSERT OR REPLACE INTO conversation_info"
                                        "(conversation_type, conversation_id, timestamp, last_message_id,"
                                        "last_read_message_index, is_top, mute, last_mention_message_id)"
                                        "values (?, ?, ?, ?, ?, ?, ?, ?)";
NSString *const kGetConversation = @"SELECT * FROM conversation_info WHERE conversation_type = ? AND conversation_id = ?";

NSString *const kConversationType = @"conversation_type";
NSString *const kConversationId = @"conversation_id";
NSString *const kDraft = @"draft";
NSString *const kTimestamp = @"timestamp";
NSString *const kLastMessageId = @"last_message_id";
NSString *const kLastReadMessageIndex = @"last_read_message_index";
NSString *const kIsTop = @"is_top";
NSString *const kTopTime = @"top_time";
NSString *const kMute = @"mute";
NSString *const kLastMentionMessageId = @"last_mention_message_id";

@interface JConversationDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JConversationDB

- (void)createTables {
    [self.dbHelper executeUpdate:kCreateConversationTable withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:kCreateConversationIndex withArgumentsInArray:nil];
}

- (void)insertConversations:(NSArray<JConcreteConversationInfo *> *)conversations {
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        [conversations enumerateObjectsUsingBlock:^(JConcreteConversationInfo * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            [db executeUpdate:kInsertConversation, @(obj.conversation.conversationType), obj.conversation.conversationId, @(obj.updateTime), obj.lastMessage.messageId, @(obj.lastReadMessageIndex), @(obj.isTop), @(obj.mute), @(0)];//TODO: mention
        }];
    }];
}

- (JConcreteConversationInfo *)getConversationInfo:(JConversation *)conversation {
    __block JConcreteConversationInfo *info;
    [self.dbHelper executeQuery:kGetConversation
           withArgumentsInArray:@[@(conversation.conversationType), conversation.conversationId]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            info = [self conversationInfoWith:resultSet];
        }
    }];
    return info;
}

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper {
    JConversationDB *db = [[JConversationDB alloc] init];
    db.dbHelper = dbHelper;
    return db;
}

#pragma mark - internal
- (JConcreteConversationInfo *)conversationInfoWith:(JFMResultSet *)rs {
    JConcreteConversationInfo *info = [[JConcreteConversationInfo alloc] init];
    JConversation *c = [[JConversation alloc] init];
    c.conversationType = [rs intForColumn:kConversationType];
    c.conversationId = [rs stringForColumn:kConversationId];
    info.conversation = c;
    info.draft = [rs stringForColumn:kDraft];
    info.updateTime = [rs longLongIntForColumn:kTimestamp];
    NSString *lastMessageId = [rs stringForColumn:kLastMessageId];
    info.lastMessage = [self.messageDB getMessageWithMessageId:lastMessageId];
    info.lastReadMessageIndex = [rs longLongIntForColumn:kLastReadMessageIndex];
    info.isTop = [rs boolForColumn:kIsTop];
    info.topTime = [rs longLongIntForColumn:kTopTime];
    info.mute = [rs boolForColumn:kMute];
    return info;
}
@end
