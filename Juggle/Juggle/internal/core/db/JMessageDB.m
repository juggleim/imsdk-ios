//
//  JMessageDB.m
//  Juggle
//
//  Created by Nathan on 2023/12/8.
//

#import "JMessageDB.h"
#import "JMessageContent+internal.h"
#import "JMessageTypeCenter.h"

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
                                        "is_deleted BOOLEAN"
                                        ")";
NSString *const kCreateMessageIndex = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_message ON message(message_uid)";
NSString *const kGetMessageWithMessageId = @"SELECT * FROM message WHERE message_uid = ? AND is_deleted = false";
NSString *const jInsertMessage = @"INSERT INTO message (conversation_type, conversation_id, type, message_uid, client_uid, direction, state, has_read, timestamp, sender, content, message_index) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
NSString *const jUpdateMessageAfterSend = @"UPDATE message SET message_uid = ?, state = ?, timestamp = ?, message_index = ? WHERE id = ?";

NSString *const jMessageConversationType = @"conversation_type";
NSString *const jMessageConversationId = @"conversation_id";
NSString *const jMessageId = @"id";
NSString *const jMessageType = @"type";
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
    [db executeUpdate:jInsertMessage, @(message.conversation.conversationType), message.conversation.conversationId, message.messageType, message.messageId, clientUid, @(message.direction), @(message.messageState), @(message.hasRead), @(message.timestamp), message.senderUserId, content, @(msgIndex)];
}

#pragma mark - internal


- (JConcreteMessage *)messageWith:(JFMResultSet *)rs {
    JConcreteMessage *message = [[JConcreteMessage alloc] init];
    JConversation *c = [[JConversation alloc] init];
    c.conversationType = [rs intForColumn:jMessageConversationType];
    c.conversationId = [rs stringForColumn:jMessageConversationId];
    message.conversation = c;
    message.messageType = [rs stringForColumn:jMessageType];
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
    message.content = [[JMessageTypeCenter shared] contentWithData:data
                                                       contentType:message.messageType];
    message.msgIndex = [rs longLongIntForColumn:jMessageIndex];
    return message;
}

@end
