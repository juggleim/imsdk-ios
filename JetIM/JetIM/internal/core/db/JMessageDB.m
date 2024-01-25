//
//  JMessageDB.m
//  JetIM
//
//  Created by Nathan on 2023/12/8.
//

#import "JMessageDB.h"
#import "JMessageContent+internal.h"
#import "JContentTypeCenter.h"

NSString *const kCreateMessageTable = @"CREATE TABLE IF NOT EXISTS message ("
                                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                        "conversation_type SMALLINT,"
                                        "conversation_id VARCHAR (64),"
                                        "type VARCHAR (64),"
                                        "message_uid VARCHAR (64),"
                                        "client_uid VARCHAR (64),"
                                        "direction BOOLEAN,"
                                        "state SMALLINT,"
                                        "has_read BOOLEAN,"
                                        "timestamp INTEGER,"
                                        "sender VARCHAR (64),"
                                        "content TEXT,"
                                        "extra TEXT,"
                                        "message_index INTEGER,"
                                        "is_deleted BOOLEAN DEFAULT 0"
                                        ")";
NSString *const kCreateMessageIndex = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_message ON message(message_uid)";
NSString *const kGetMessageWithMessageId = @"SELECT * FROM message WHERE message_uid = ? AND is_deleted = 0";
NSString *const jGetMessagesInConversation = @"SELECT * FROM message WHERE conversation_type = ? AND conversation_id = ? AND is_deleted = 0";
NSString *const jAndGreaterThan = @" AND timestamp > ?";
NSString *const jAndLessThan = @" AND timestamp < ?";
NSString *const jAndTypeIn = @" AND type in ";
NSString *const jOrderByTimestamp = @" ORDER BY timestamp";
NSString *const jASC = @" ASC";
NSString *const jDESC = @" DESC";
NSString *const jLimit = @" LIMIT ?";
NSString *const jInsertMessage = @"INSERT INTO message (conversation_type, conversation_id, type, message_uid, client_uid, direction, state, has_read, timestamp, sender, content, message_index) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
NSString *const jUpdateMessageAfterSend = @"UPDATE message SET message_uid = ?, state = ?, timestamp = ?, message_index = ? WHERE id = ?";
NSString *const jDeleteMessage = @"UPDATE message SET is_deleted = 1 WHERE";
NSString *const jClearMessages = @"UPDATE message SET is_deleted = 1 WHERE conversation_type = ? AND conversation_id = ?";
NSString *const jClientMsgNoIs = @" id = ?";
NSString *const jMessageIdIs = @" message_uid = ?";
NSString *const jGetMessagesByMessageIds = @"SELECT * FROM message WHERE message_uid in ";
NSString *const jGetMessagesByClientMsgNos = @"SELECT * FROM message WHERE id in ";

NSString *const jMessageConversationType = @"conversation_type";
NSString *const jMessageConversationId = @"conversation_id";
NSString *const jMessageId = @"id";
NSString *const jContentType = @"type";
NSString *const jMessageUId = @"message_uid";
NSString *const jMessageClientUid = @"client_uid";
NSString *const jDirection = @"direction";
NSString *const jState = @"state";
NSString *const jHasRead = @"has_read";
NSString *const jTimestamp = @"timestamp";
NSString *const jSender = @"sender";
NSString *const jContent = @"content";
NSString *const jExtra = @"extra";
NSString *const jMessageIndex = @"message_index";
NSString *const jIsDeleted = @"is_deleted";

@interface JMessageDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JMessageDB

- (JConcreteMessage *)getMessageWithMessageId:(NSString *)messageId {
    if (messageId.length == 0) {
        return nil;
    }
    __block JConcreteMessage *message;
    [self.dbHelper executeQuery:kGetMessageWithMessageId
           withArgumentsInArray:@[messageId]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            message = [self messageWith:resultSet];
        }
    }];
    return message;
}

- (void)insertMessages:(NSArray<JConcreteMessage *> *)messages {
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        [messages enumerateObjectsUsingBlock:^(JConcreteMessage * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            [self insertMessage:obj inDb:db];
            obj.clientMsgNo = db.lastInsertRowId;
        }];
    }];
}

- (void)updateMessageAfterSend:(long long)clientMsgNo
                     messageId:(NSString *)messageId
                     timestamp:(long long)timestamp
                  messageIndex:(long long)messageIndex {
    [self.dbHelper executeUpdate:jUpdateMessageAfterSend
            withArgumentsInArray:@[messageId, @(JMessageStateSent), @(timestamp), @(messageIndex), @(clientMsgNo)]];
}

- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction
                            contentTypes:(NSArray<NSString *> *)contentTypes {
    if (time == 0) {
        time = INT64_MAX;
    }
    NSString *sql = jGetMessagesInConversation;
    if (direction == JPullDirectionNewer) {
        sql = [sql stringByAppendingString:jAndGreaterThan];
    } else {
        sql = [sql stringByAppendingString:jAndLessThan];
    }
    if (contentTypes.count > 0) {
        sql = [sql stringByAppendingString:jAndTypeIn];
        sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:contentTypes.count]];
    }
    sql = [sql stringByAppendingString:jOrderByTimestamp];
    if (direction == JPullDirectionNewer) {
        sql = [sql stringByAppendingString:jASC];
    } else {
        sql = [sql stringByAppendingString:jDESC];
    }
    sql = [sql stringByAppendingString:jLimit];
    
    NSMutableArray *messages = [[NSMutableArray alloc] init];
    NSMutableArray *args = [[NSMutableArray alloc] initWithArray:@[@(conversation.conversationType), conversation.conversationId, @(time)]];
    [args addObjectsFromArray:contentTypes];
    [args addObject:@(count)];
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:args
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        while ([resultSet next]) {
            JConcreteMessage *m = [self messageWith:resultSet];
            [messages addObject:m];
        }
    }];
    NSArray *result;
    if (direction == JPullDirectionOlder) {
        result = [[messages reverseObjectEnumerator] allObjects];
    } else {
        result = [messages copy];
    }
    return result;
}

- (void)deleteMessageByClientId:(long long)clientMsgNo {
    NSString *sql = jDeleteMessage;
    sql = [sql stringByAppendingString:jClientMsgNoIs];
    [self.dbHelper executeUpdate:sql
            withArgumentsInArray:@[@(clientMsgNo)]];
}

- (void)deleteMessageByMessageId:(NSString *)messageId {
    NSString *sql = jDeleteMessage;
    sql = [sql stringByAppendingString:jMessageIdIs];
    [self.dbHelper executeUpdate:sql
            withArgumentsInArray:@[messageId]];
}

- (void)clearMessagesIn:(JConversation *)conversation {
    [self.dbHelper executeUpdate:jClearMessages
            withArgumentsInArray:@[@(conversation.conversationType), conversation.conversationId]];
}

//被删除的消息也能查出来
- (NSArray<JMessage *> *)getMessagesByMessageIds:(NSArray<NSString *> *)messageIds {
    NSMutableArray<JMessage *> *result = [[NSMutableArray alloc] init];
    if (messageIds.count == 0) {
        return result;
    }
    NSString *sql = [jGetMessagesByMessageIds stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:messageIds.count]];
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:messageIds
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        while ([resultSet next]) {
            JConcreteMessage *m = [self messageWith:resultSet];
            [result addObject:m];
        }
    }];
    NSMutableArray *messages = [[NSMutableArray alloc] init];
    for (NSString *messageId in messageIds) {
        for (JMessage *message in result) {
            if ([messageId isEqualToString:message.messageId]) {
                [messages addObject:message];
                break;
            }
        }
    }
    return [messages copy];
}

//被删除的消息也能查出来
- (NSArray<JMessage *> *)getMessagesByClientMsgNos:(NSArray<NSNumber *> *)clientMsgNos {
    NSMutableArray<JMessage *> *result = [[NSMutableArray alloc] init];
    if (clientMsgNos.count == 0) {
        return result;
    }
    NSString *sql = [jGetMessagesByClientMsgNos stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:clientMsgNos.count]];
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:clientMsgNos
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        while ([resultSet next]) {
            JConcreteMessage *m = [self messageWith:resultSet];
            [result addObject:m];
        }
    }];
    NSMutableArray *messages = [[NSMutableArray alloc] init];
    for (NSNumber *clientMsgNo in clientMsgNos) {
        for (JMessage *message in result) {
            if (message.clientMsgNo == clientMsgNo.longLongValue) {
                [messages addObject:message];
                break;
            }
        }
    }
    return [messages copy];
}

- (void)createTables {
    [self.dbHelper executeUpdate:kCreateMessageTable withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:kCreateMessageIndex withArgumentsInArray:nil];
}

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper {
    JMessageDB *db = [[JMessageDB alloc] init];
    db.dbHelper = dbHelper;
    return db;
}

#pragma mark - operation with db
- (void)insertMessage:(JMessage *)message inDb:(JFMDatabase *)db {
    long long msgIndex = 0;
    NSString *clientUid = @"";
    if ([message isKindOfClass:[JConcreteMessage class]]) {
        msgIndex = ((JConcreteMessage *)message).msgIndex;
        clientUid = ((JConcreteMessage *)message).clientUid;
    }
    NSData *data = [message.content encode];
    NSString *content = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    [db executeUpdate:jInsertMessage, @(message.conversation.conversationType), message.conversation.conversationId, message.contentType, message.messageId, clientUid, @(message.direction), @(message.messageState), @(message.hasRead), @(message.timestamp), message.senderUserId, content, @(msgIndex)];
}

#pragma mark - internal
- (JConcreteMessage *)messageWith:(JFMResultSet *)rs {
    JConcreteMessage *message = [[JConcreteMessage alloc] init];
    JConversation *c = [[JConversation alloc] init];
    c.conversationType = [rs intForColumn:jMessageConversationType];
    c.conversationId = [rs stringForColumn:jMessageConversationId];
    message.conversation = c;
    message.contentType = [rs stringForColumn:jContentType];
    message.clientMsgNo = [rs longLongIntForColumn:jMessageId];
    message.messageId = [rs stringForColumn:jMessageUId];
    message.clientUid = [rs stringForColumn:jMessageClientUid];
    message.direction = [rs intForColumn:jDirection];
    message.messageState = [rs intForColumn:jState];
    message.hasRead = [rs boolForColumn:jHasRead];
    message.timestamp = [rs longLongIntForColumn:jTimestamp];
    message.senderUserId = [rs stringForColumn:jSender];
    NSString *content = [rs stringForColumn:jContent];
    NSData *data = [content dataUsingEncoding:NSUTF8StringEncoding];
    message.content = [[JContentTypeCenter shared] contentWithData:data
                                                       contentType:message.contentType];
    message.msgIndex = [rs longLongIntForColumn:jMessageIndex];
    return message;
}

@end
