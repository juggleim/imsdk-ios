//
//  JConversationDB.m
//  JetIM
//
//  Created by Nathan on 2023/12/8.
//

#import "JConversationDB.h"
#import "JMessageContent+internal.h"
#import "JContentTypeCenter.h"

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
                                        "last_mention_message_id VARCHAR (64),"
                                        "last_message_type VARCHAR (64),"
                                        "last_message_client_uid VARCHAR (64),"
                                        "last_message_direction BOOLEAN,"
                                        "last_message_state SMALLINT,"
                                        "last_message_has_read BOOLEAN,"
                                        "last_message_timestamp INTEGER,"
                                        "last_message_sender VARCHAR (64),"
                                        "last_message_content TEXT,"
                                        "last_message_message_index INTEGER"
                                        ")";
NSString *const kCreateConversationIndex = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_conversation ON conversation_info(conversation_type, conversation_id)";
NSString *const kInsertConversation = @"INSERT OR REPLACE INTO conversation_info"
                                        "(conversation_type, conversation_id, timestamp, last_message_id,"
                                        "last_read_message_index, is_top, top_time, mute, last_mention_message_id,"
                                        "last_message_type, last_message_client_uid, last_message_direction, last_message_state,"
                                        "last_message_has_read, last_message_timestamp, last_message_sender, last_message_content,"
                                        "last_message_message_index)"
                                        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
NSString *const jUpdateConversation = @"UPDATE conversation_info SET timestamp=?, last_message_id=?, last_read_message_index=?, "
                                        "is_top=?, top_time=?, mute=?, last_mention_message_id=?, last_message_type=?,  "
                                        "last_message_client_uid=?, last_message_direction=?, last_message_state=?, "
                                        "last_message_has_read=?, last_message_timestamp=?, last_message_sender=?, "
                                        "last_message_content=?, last_message_message_index=? WHERE conversation_type = ? "
                                        "AND conversation_id = ?";
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
NSString *const jClearUnreadCount = @"UPDATE conversation_info SET last_read_message_index = ? WHERE conversation_type = ? AND conversation_id = ?";
NSString *const jUpdateLastMessage = @"UPDATE conversation_info SET timestamp=?, last_message_id=?, last_message_type=?,"
                                    "last_message_client_uid=?, "
                                    "last_message_direction=?, last_message_state=?, last_message_has_read=?, last_message_timestamp=?, "
                                    "last_message_sender=?, last_message_content=?, last_message_message_index=? WHERE "
                                    "conversation_type = ? AND conversation_id = ?";

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
NSString *const jLastMessageType = @"last_message_type";
NSString *const jLastMessageClientUid = @"last_message_client_uid";
NSString *const jLastMessageDirection = @"last_message_direction";
NSString *const jLastMessageState = @"last_message_state";
NSString *const jLastMessageHasRead = @"last_message_has_read";
NSString *const jLastMessageTimestamp = @"last_message_timestamp";
NSString *const jLastMessageSender = @"last_message_sender";
NSString *const jLastMessageContent = @"last_message_content";
NSString *const jLastMessageIndex = @"last_message_message_index";

@interface JConversationDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JConversationDB

- (void)createTables {
    [self.dbHelper executeUpdate:kCreateConversationTable withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:kCreateConversationIndex withArgumentsInArray:nil];
}

- (void)insertConversations:(NSArray<JConcreteConversationInfo *> *)conversations
                 completion:(nonnull void (^)(NSArray<JConcreteConversationInfo *> * _Nonnull, NSArray<JConcreteConversationInfo *> * _Nonnull))completeBlock {
    NSMutableArray *insertConversations = [[NSMutableArray alloc] init];
    NSMutableArray *updateConversations = [[NSMutableArray alloc] init];
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        [conversations enumerateObjectsUsingBlock:^(JConcreteConversationInfo * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            JConcreteMessage *lastMessage = (JConcreteMessage *)obj.lastMessage;
            NSData *data = [lastMessage.content encode];
            NSString *content = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
            
            JConcreteConversationInfo *info = nil;
            JFMResultSet *resultSet = [db executeQuery:kGetConversation, @(obj.conversation.conversationType), obj.conversation.conversationId];
            if ([resultSet next]) {
                info = [self conversationInfoWith:resultSet];
            }
            if (info) {
                [updateConversations addObject:obj];
                [db executeUpdate:jUpdateConversation, @(obj.updateTime), lastMessage.messageId, @(obj.lastReadMessageIndex), @(obj.isTop), @(obj.topTime), @(obj.mute), @(0), lastMessage.contentType, lastMessage.clientUid, @(lastMessage.direction), @(lastMessage.messageState), @(lastMessage.hasRead), @(lastMessage.timestamp), lastMessage.senderUserId, content, @(lastMessage.msgIndex), @(obj.conversation.conversationType), obj.conversation.conversationId];
            } else {
                [insertConversations addObject:obj];
                [db executeUpdate:kInsertConversation, @(obj.conversation.conversationType), obj.conversation.conversationId, @(obj.updateTime), lastMessage.messageId, @(obj.lastReadMessageIndex), @(obj.isTop), @(obj.topTime), @(obj.mute), @(0), lastMessage.contentType, lastMessage.clientUid, @(lastMessage.direction), @(lastMessage.messageState), @(lastMessage.hasRead), @(lastMessage.timestamp), lastMessage.senderUserId, content, @(lastMessage.msgIndex)];
            }
        }];
    }];
    if (completeBlock) {
        completeBlock(insertConversations, updateConversations);
    }
}

- (JConcreteConversationInfo *)getConversationInfo:(JConversation *)conversation {
    __block JConcreteConversationInfo *info = nil;
    [self.dbHelper executeQuery:kGetConversation
           withArgumentsInArray:@[@(conversation.conversationType), conversation.conversationId]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            info = [self conversationInfoWith:resultSet];
        }
    }];
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
            [array addObject:info];
        }
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
            [array addObject:info];
        }
    }];
    return array;
}

- (void)setDraft:(NSString *)draft inConversation:(JConversation *)conversation {
    [self.dbHelper executeUpdate:jSetDraft withArgumentsInArray:@[draft, @(conversation.conversationType), conversation.conversationId]];
}

- (void)clearDraftInConversation:(JConversation *)conversation {
    [self setDraft:@"" inConversation:conversation];
}

- (void)clearUnreadCountBy:(JConversation *)conversation
                  msgIndex:(long long)msgIndex {
    [self.dbHelper executeUpdate:jClearUnreadCount withArgumentsInArray:@[@(msgIndex), @(conversation.conversationType), conversation.conversationId]];
}

- (void)updateLastMessage:(JConcreteMessage *)message
           inConversation:(JConversation *)conversation {
    NSData *data = [message.content encode];
    NSString *content = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    [self.dbHelper executeUpdate:jUpdateLastMessage withArgumentsInArray:@[@(message.timestamp), message.messageId?:@"", message.contentType, message.clientUid, @(message.direction), @(message.messageState), @(message.hasRead), @(message.timestamp), message.senderUserId, content, @(message.msgIndex), @(conversation.conversationType), conversation.conversationId]];
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
    info.unreadCount = (int)([rs longLongIntForColumn:jLastMessageIndex] - info.lastReadMessageIndex);
    JConcreteMessage *lastMessage = [[JConcreteMessage alloc] init];
    lastMessage.conversation = c;
    lastMessage.contentType = [rs stringForColumn:jLastMessageType];
    lastMessage.messageId = [rs stringForColumn:jLastMessageId];
    lastMessage.clientUid = [rs stringForColumn:jLastMessageClientUid];
    lastMessage.direction = [rs intForColumn:jLastMessageDirection];
    lastMessage.messageState = [rs intForColumn:jLastMessageState];
    lastMessage.hasRead = [rs boolForColumn:jLastMessageHasRead];
    lastMessage.timestamp = [rs longLongIntForColumn:jLastMessageTimestamp];
    lastMessage.senderUserId = [rs stringForColumn:jLastMessageSender];
    NSString *content = [rs stringForColumn:jLastMessageContent];
    NSData *data = [content dataUsingEncoding:NSUTF8StringEncoding];
    lastMessage.content = [[JContentTypeCenter shared] contentWithData:data
                                                           contentType:lastMessage.contentType];
    lastMessage.msgIndex = [rs longLongIntForColumn:jLastMessageIndex];
    info.lastMessage = lastMessage;
    return info;
}
@end
