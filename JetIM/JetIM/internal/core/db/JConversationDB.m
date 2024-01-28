//
//  JConversationDB.m
//  JetIM
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
NSString *const kCreateConversationIndex = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_conversation ON conversation_info(conversation_type, conversation_id)";
NSString *const kInsertConversation = @"INSERT OR REPLACE INTO conversation_info"
                                        "(conversation_type, conversation_id, timestamp, last_message_id,"
                                        "last_read_message_index, is_top, top_time, mute, last_mention_message_id)"
                                        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
NSString *const kGetConversation = @"SELECT * FROM conversation_info WHERE conversation_type = ? AND conversation_id = ?";
NSString *const jGetConversations = @"SELECT * FROM conversation_info ORDER BY timestamp DESC";
NSString *const jGetConversationsBy = @"SELECT * FROM conversation_info WHERE";
NSString *const jTimestampGreaterThan = @" timestamp > ?";
NSString *const jTimestampLessThan = @" timestamp < ?";
NSString *const jConversationAnd = @" AND ";
NSString *const jConversationTypeIn = @" conversation_type in ";
NSString *const jConversationOrderByTimestamp = @" ORDER BY timestamp DESC";
NSString *const jConversationLimit = @" LIMIT ?";
NSString *const jDeleteConversation = @"DELETE FROM conversation_info WHERE conversation_type = ? AND conversation_id = ?";
NSString *const jSetDraft = @"UPDATE conversation_info SET draft = ? WHERE conversation_type = ? AND conversation_id = ?";

NSString *const jConversationType = @"conversation_type";
NSString *const jConversationId = @"conversation_id";
NSString *const jDraft = @"draft";
NSString *const jConversationTimestamp = @"timestamp";
NSString *const jLastMessageId = @"last_message_id";
NSString *const jLastReadMessageIndex = @"last_read_message_index";
NSString *const jIsTop = @"is_top";
NSString *const jTopTime = @"top_time";
NSString *const jMute = @"mute";
NSString *const jLastMentionMessageId = @"last_mention_message_id";

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
            [db executeUpdate:kInsertConversation, @(obj.conversation.conversationType), obj.conversation.conversationId, @(obj.updateTime), obj.lastMessage.messageId, @(obj.lastReadMessageIndex), @(obj.isTop), @(obj.topTime), @(obj.mute), @(0)];//TODO: mention
            [self.messageDB insertMessage:obj.lastMessage inDb:db];
        }];
    }];
}

- (JConcreteConversationInfo *)getConversationInfo:(JConversation *)conversation {
    __block JConcreteConversationInfo *info;
    __block NSString *lastMessageId;
    [self.dbHelper executeQuery:kGetConversation
           withArgumentsInArray:@[@(conversation.conversationType), conversation.conversationId]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            info = [self conversationInfoWith:resultSet];
            lastMessageId = [resultSet stringForColumn:jLastMessageId];
        }
    }];
    info.lastMessage = [self.messageDB getMessageWithMessageId:lastMessageId];
    return info;
}

- (void)deleteConversationInfoBy:(JConversation *)conversation {
    [self.dbHelper executeUpdate:jDeleteConversation
            withArgumentsInArray:@[@(conversation.conversationType), conversation.conversationId]];
}

- (NSArray<JConcreteConversationInfo *> *)getConversationInfoList {
    NSMutableArray<JConcreteConversationInfo *> *array = [[NSMutableArray alloc] init];
    [self.dbHelper executeQuery:jGetConversations
           withArgumentsInArray:nil
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        while ([resultSet next]) {
            JConcreteConversationInfo *info = [self conversationInfoWith:resultSet];
            JConcreteMessage *message = [[JConcreteMessage alloc] init];
            message.messageId = [resultSet stringForColumn:jLastMessageId];
            info.lastMessage = message;
            [array addObject:info];
        }
    }];
    [array enumerateObjectsUsingBlock:^(JConcreteConversationInfo * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        obj.lastMessage = [self.messageDB getMessageWithMessageId:obj.lastMessage.messageId];
    }];
    return array;
}

- (NSArray<JConversationInfo *> *)getConversationInfoListWithTypes:(NSArray<NSNumber *> *)conversationTypes
                                                             count:(int)count
                                                         timestamp:(long long)ts
                                                         direction:(JPullDirection)direction {
    if (ts == 0) {
        ts = INT64_MAX;
    }
    NSMutableArray *args = [[NSMutableArray alloc] init];
    NSString *sql = jGetConversationsBy;
    if (direction == JPullDirectionOlder) {
        sql = [sql stringByAppendingString:jTimestampLessThan];
    } else {
        sql = [sql stringByAppendingString:jTimestampGreaterThan];
    }
    [args addObject:@(ts)];
    if (conversationTypes.count > 0) {
        sql = [sql stringByAppendingString:jConversationAnd];
        sql = [sql stringByAppendingString:jConversationTypeIn];
        sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:conversationTypes.count]];
        [args addObjectsFromArray:conversationTypes];
    }
    sql = [sql stringByAppendingString:jConversationOrderByTimestamp];
    sql = [sql stringByAppendingString:jConversationLimit];
    [args addObject:@(count)];
    
    NSMutableArray<JConcreteConversationInfo *> *array = [[NSMutableArray alloc] init];
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:args
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        while ([resultSet next]) {
            JConcreteConversationInfo *info = [self conversationInfoWith:resultSet];
            JConcreteMessage *message = [[JConcreteMessage alloc] init];
            message.messageId = [resultSet stringForColumn:jLastMessageId];
            info.lastMessage = message;
            [array addObject:info];
        }
    }];
    [array enumerateObjectsUsingBlock:^(JConcreteConversationInfo * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        obj.lastMessage = [self.messageDB getMessageWithMessageId:obj.lastMessage.messageId];
    }];
    return array;
}

- (void)setDraft:(NSString *)draft inConversation:(JConversation *)conversation {
    [self.dbHelper executeUpdate:jSetDraft withArgumentsInArray:@[draft, @(conversation.conversationType), conversation.conversationId]];
}

- (void)clearDraftInConversation:(JConversation *)conversation {
    [self setDraft:@"" inConversation:conversation];
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
    c.conversationType = [rs intForColumn:jConversationType];
    c.conversationId = [rs stringForColumn:jConversationId];
    info.conversation = c;
    info.draft = [rs stringForColumn:jDraft];
    info.updateTime = [rs longLongIntForColumn:jConversationTimestamp];
    info.lastReadMessageIndex = [rs longLongIntForColumn:jLastReadMessageIndex];
    info.isTop = [rs boolForColumn:jIsTop];
    info.topTime = [rs longLongIntForColumn:jTopTime];
    info.mute = [rs boolForColumn:jMute];
    return info;
}
@end
