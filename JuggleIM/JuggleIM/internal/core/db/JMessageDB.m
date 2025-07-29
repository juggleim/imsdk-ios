//
//  JMessageDB.m
// JuggleIM
//
//  Created by Nathan on 2023/12/8.
//

#import "JMessageDB.h"
#import "JContentTypeCenter.h"

//message 最新版本
#define jMessageTableVersion 3
//NSUserDefault 中保存 message 数据库版本的 key
#define jMessageTableVersionKey @"MessageVersion"

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
                                        "seq_no INTEGER,"
                                        "message_index INTEGER,"
                                        "read_count INTEGER DEFAULT 0,"
                                        "member_count INTEGER DEFAULT 0,"
                                        "is_deleted BOOLEAN DEFAULT 0,"
                                        "search_content TEXT,"
                                        "local_attribute TEXT,"
                                        "mention_info TEXT,"
                                        "refer_msg_id VARCHAR (64),"
                                        "flags INTEGER,"
                                        "life_time INTEGER DEFAULT 0,"
                                        "life_time_after_read INTEGER DEFAULT 0,"
                                        "destroy_time INTEGER DEFAULT 0"
                                        ")";
NSString *const kCreateMessageIndex = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_message ON message(message_uid)";
NSString *const kCreateClientUidIndex = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_message_client_uid ON message(client_uid)";
NSString *const kCreateMessageConversationIndex = @"CREATE INDEX IF NOT EXISTS idx_message_conversation ON message(conversation_type, conversation_id)";
NSString *const jCreateMessageConversationTSIndex = @"CREATE INDEX IF NOT EXISTS idx_message_conversation_ts ON message(conversation_type, conversation_id, timestamp)";
NSString *const kAlterAddFlags = @"ALTER TABLE message ADD COLUMN flags INTEGER";
NSString *const kAlterAddLifeTime = @"ALTER TABLE message ADD COLUMN life_time INTEGER DEFAULT 0";
NSString *const kAlterAddLifeTimeAfterRead = @"ALTER TABLE message ADD COLUMN life_time_after_read INTEGER DEFAULT 0";
NSString *const kAlterAddDestroyTime = @"ALTER TABLE message ADD COLUMN destroy_time INTEGER DEFAULT 0";
NSString *const kGetMessageWithMessageId = @"SELECT * FROM message WHERE message_uid = ? AND is_deleted = 0 AND destroy_time > ?";
NSString *const kGetMessageWithMessageIdEvenDelete = @"SELECT * FROM message WHERE message_uid = ?";
NSString *const kGetMessageWithClientUid = @"SELECT * FROM message WHERE client_uid = ?";
NSString *const jGetMessagesInConversation = @"SELECT * FROM message WHERE is_deleted = 0 AND destroy_time > ? AND conversation_type = ? AND conversation_id = ? ";
NSString *const jAndGreaterThan = @" AND timestamp > ?";
NSString *const jAndLessThan = @" AND timestamp < ?";
NSString *const jAndTypeIn = @" AND type IN ";
NSString *const jAndSenderIn = @" AND sender IN ";
NSString *const jAndStateIn = @" AND state IN ";
NSString *const jAndConversationTypeIn = @" AND conversation_type IN ";
NSString *const jOrderByTimestamp = @" ORDER BY timestamp";
NSString *const jAnd = @" AND";
NSString *const jLeftBracket = @" (";
NSString *const jRightBracket = @")";
NSString *const jConversationIs = @" conversation_type = ? AND conversation_id = ?";
NSString *const jOr = @" OR";
NSString *const jASC = @" ASC";
NSString *const jDESC = @" DESC";
NSString *const jLimit = @" LIMIT ?";
NSString *const jInsertMessage = @"INSERT INTO message (conversation_type, conversation_id, type, message_uid, client_uid, direction, state, has_read, timestamp, sender, content, seq_no, message_index, read_count, member_count, search_content, mention_info, refer_msg_id, flags, is_deleted, life_time, life_time_after_read, destroy_time) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
NSString *const jUpdateMessageAfterSend = @"UPDATE message SET message_uid = ?, state = ?, timestamp = ?, seq_no = ?, member_count = ?, destroy_time = CASE WHEN life_time != 0 THEN ? + life_time ELSE destroy_time END WHERE id = ?";
NSString *const jUpdateMessageAfterSendWithClientUid = @"UPDATE message SET message_uid = ?, state = ?, timestamp = ?, seq_no = ?, member_count = ? WHERE client_uid = ?";
NSString *const jUpdateMessageContent = @"UPDATE message SET content = ?, type = ?, search_content = ? WHERE ";
NSString *const jSetMessageFlags = @"UPDATE message SET flags = ? WHERE message_uid = ?";
NSString *const jMessageSendFail = @"UPDATE message SET state = ? WHERE id = ?";
NSString *const jDeleteMessage = @"UPDATE message SET is_deleted = 1 WHERE";
NSString *const jClearMessages = @"UPDATE message SET is_deleted = 1 WHERE conversation_type = ? AND conversation_id = ? AND timestamp <= ?";
NSString *const jAndSenderIs = @" AND sender = ?";
NSString *const jUpdateMessage = @"UPDATE message SET type = ?, content = ?, search_content = ?, mention_info = ?,refer_msg_id = ? WHERE id = ?";

NSString *const jUpdateMessageState = @"UPDATE message SET state = ? WHERE id = ?";
NSString *const jSetMessagesRead = @"UPDATE message SET has_read = 1 WHERE message_uid IN ";
NSString *const jSetGroupReadInfo = @"UPDATE message SET read_count = ?, member_count = ? WHERE message_uid = ?";
NSString *const jClientMsgNoIs = @" id = ?";
NSString *const jClientMsgNoIn = @" id in ";
NSString *const jMessageIdIs = @" message_uid = ?";
NSString *const jMessageIdIn = @" message_uid in ";
NSString *const jGetMessagesByMessageIds = @"SELECT * FROM message WHERE message_uid in ";
NSString *const jGetMessagesByClientMsgNos = @"SELECT * FROM message WHERE id in ";
//NSString *const jGetMessagesBySearchContent = @"SELECT * FROM message WHERE search_content LIKE ? AND is_deleted = 0";
NSString *const jGetMessagesNotDeleted = @"SELECT * FROM message WHERE is_deleted = 0 AND destroy_time > ?";
NSString *const jAndSearchContentIs = @" AND search_content LIKE ?";
NSString *const jAndInConversation = @" AND conversation_id = ?";
NSString *const jGetMessageLocalAttribute = @"SELECT local_attribute FROM message WHERE";
NSString *const jUpdateMessageLocalAttribute = @"UPDATE message SET local_attribute = ? WHERE";
NSString *const jClearChatroomMessagesExclude = @"DELETE FROM message WHERE conversation_type = 3 AND conversation_id NOT IN ";
NSString *const jClearChatroomMessagesIn = @"DELETE FROM message WHERE conversation_type = 3 AND conversation_id = ?";
NSString *const jSearchMessageInConversations = @"SELECT conversation_type, conversation_id, count(*) AS match_count FROM message WHERE is_deleted = 0 AND destroy_time > ?";
NSString *const jGroupByConversationTypeAndId = @" GROUP BY conversation_type, conversation_id";
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
NSString *const jSeqNo = @"seq_no";
NSString *const jMessageIndex = @"message_index";
NSString *const jReadCount = @"read_count";
NSString *const jMemberCount = @"member_count";
NSString *const jIsDeleted = @"is_deleted";
NSString *const jLocalAttribute = @"local_attribute";
NSString *const jMessageMentionInfo = @"mention_info";
NSString *const jReferMsgId = @"refer_msg_id";
NSString *const jMatchCount = @"match_count";
NSString *const jFlags = @"flags";
NSString *const jLifeTime = @"life_time";
NSString *const jLifeTimeAfterRead = @"life_time_after_read";
NSString *const jDestroyTime = @"destroy_time";

@interface JMessageDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JMessageDB

- (JConcreteMessage *)getMessageWithMessageId:(NSString *)messageId
                                  currentTime:(long long)now {
    if (messageId.length == 0) {
        return nil;
    }
    __block JConcreteMessage *message;
    [self.dbHelper executeQuery:kGetMessageWithMessageId
           withArgumentsInArray:@[messageId, @(now)]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            message = [self messageWith:resultSet];
        }
    }];
    if(message.referMsgId.length > 0){
        message.referredMsg = [self getMessageWithMessageIdEvenDelete:message.referMsgId];
    }
    return message;
}

- (JConcreteMessage *)getMessageWithMessageIdEvenDelete:(NSString *)messageId {
    if (messageId.length == 0) {
        return nil;
    }
    __block JConcreteMessage *message;
    [self.dbHelper executeQuery:kGetMessageWithMessageIdEvenDelete
           withArgumentsInArray:@[messageId]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            message = [self messageWith:resultSet];
        }
    }];
    if(message.referMsgId.length > 0){
        message.referredMsg = [self getMessageWithMessageIdEvenDelete:message.referMsgId];
    }
    return message;
}

- (void)insertMessages:(NSArray<JConcreteMessage *> *)messages {
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        [messages enumerateObjectsUsingBlock:^(JConcreteMessage * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            JConcreteMessage *m = nil;
            //messageId 排重
            if (obj.messageId.length > 0) {
                m = [self getMessageWithMessageId:obj.messageId currentTime:0 inDb:db];
            }
            //clientUid 排重
            if (!m && obj.clientUid.length > 0) {
                m = [self getMessageWithClientUid:obj.clientUid inDb:db];
            }
            if (m) {
                obj.clientMsgNo = m.clientMsgNo;
                obj.existed = YES;
            } else {
                [self insertMessage:obj inDb:db];
                obj.clientMsgNo = db.lastInsertRowId;
            }
            
            if(obj.referredMsg != nil) {
                JConcreteMessage * ref = [self getMessageWithMessageId:obj.referredMsg.messageId currentTime:0 inDb:db];
                if (ref == nil) {
                    [self insertMessage:obj.referredMsg inDb:db];
                }
            }
        }];
    }];
}

- (void)updateMessageAfterSend:(long long)clientMsgNo
                     messageId:(NSString *)messageId
                     timestamp:(long long)timestamp
                         seqNo:(long long)seqNo
              groupMemberCount:(int)count {
    if (messageId.length == 0) {
        return;
    }
    [self.dbHelper executeUpdate:jUpdateMessageAfterSend
            withArgumentsInArray:@[messageId, @(JMessageStateSent), @(timestamp), @(seqNo), @(count), @(timestamp), @(clientMsgNo)]]; 
}

- (void)updateMessageAfterSendWithClientUid:(NSString *)clientUid
                                  messageId:(NSString *)messageId
                                  timestamp:(long long)timestamp
                                      seqNo:(long long)seqNo
                           groupMemberCount:(int)count {
    if (messageId.length == 0) {
        return;
    }
    [self.dbHelper executeUpdate:jUpdateMessageAfterSendWithClientUid
            withArgumentsInArray:@[messageId, @(JMessageStateSent), @(timestamp), @(seqNo), @(count), clientUid]];
}

- (void)updateMessageContent:(JMessageContent *)content
                 contentType:(nonnull NSString *)type
               withMessageId:(NSString *)messageId {
    NSData *data = [content encode];
    NSString *s = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    if (s.length == 0 || messageId.length == 0) {
        return;
    }
    NSString *sql = [jUpdateMessageContent stringByAppendingString:jMessageIdIs];
    [self.dbHelper executeUpdate:sql
            withArgumentsInArray:@[s, type, content.searchContent, messageId]];
}

- (void)updateMessageContent:(JMessageContent *)content
                 contentType:(NSString *)type
             withClientMsgNo:(long long)clientMsgNo {
    NSData *data = [content encode];
    NSString *s = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    if (s.length == 0) {
        return;
    }
    NSString *sql = [jUpdateMessageContent stringByAppendingString:jClientMsgNoIs];
    [self.dbHelper executeUpdate:sql withArgumentsInArray:@[s, type, content.searchContent, @(clientMsgNo)]];
}

- (void)setMessageFlags:(int)flags withMessageId:(NSString *)messageId {
    NSString *sql = jSetMessageFlags;
    [self.dbHelper executeUpdate:sql withArgumentsInArray:@[@(flags), messageId]];
}

-(void)updateMessage:(JConcreteMessage *)message{
    NSMutableArray *args = [NSMutableArray array];
    [args addObject:message.contentType];
    NSData *data = [message.content encode];
    NSString * content = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    [args addObject:content];
    [args addObject:message.content.searchContent];
    NSString * mentionInfo;
    if(message.mentionInfo){
        mentionInfo = [message.mentionInfo encodeToJson];
    }else{
        mentionInfo = @"";
    }
    [args addObject:mentionInfo];
    NSString * refer_msg_id;
    if(message.referredMsg){
        refer_msg_id = message.referredMsg.messageId;
    }else{
        refer_msg_id = @"";
    }
    [args addObject:refer_msg_id];
    [args addObject:@(message.clientMsgNo)];
    [self.dbHelper executeUpdate:jUpdateMessage
            withArgumentsInArray:args];
}

//- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
//                                   count:(int)count
//                                    time:(long long)time
//                               direction:(JPullDirection)direction
//                            contentTypes:(NSArray<NSString *> *)contentTypes
//                             currentTime:(long long)now {
//    if (conversation.conversationId.length == 0) {
//        return [NSArray array];
//    }
//    if (time == 0) {
//        time = INT64_MAX;
//    }
//    NSString *sql = jGetMessagesInConversation;
//    if (direction == JPullDirectionNewer) {
//        sql = [sql stringByAppendingString:jAndGreaterThan];
//    } else {
//        sql = [sql stringByAppendingString:jAndLessThan];
//    }
//    if (contentTypes.count > 0) {
//        sql = [sql stringByAppendingString:jAndTypeIn];
//        sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:contentTypes.count]];
//    }
//    sql = [sql stringByAppendingString:jOrderByTimestamp];
//    if (direction == JPullDirectionNewer) {
//        sql = [sql stringByAppendingString:jASC];
//    } else {
//        sql = [sql stringByAppendingString:jDESC];
//    }
//    sql = [sql stringByAppendingString:jLimit];
//    
//    NSMutableArray *messages = [[NSMutableArray alloc] init];
//    NSMutableArray *args = [[NSMutableArray alloc] initWithArray:@[@(now), @(conversation.conversationType), conversation.conversationId, @(time)]];
//    [args addObjectsFromArray:contentTypes];
//    [args addObject:@(count)];
//    [self.dbHelper executeQuery:sql
//           withArgumentsInArray:args
//                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
//        while ([resultSet next]) {
//            JConcreteMessage *m = [self messageWith:resultSet];
//            [messages addObject:m];
//        }
//    }];
//    for (JConcreteMessage * message in messages) {
//        if(message.referMsgId.length > 0){
//            message.referredMsg = [self getMessageWithMessageIdEvenDelete:message.referMsgId];
//        }
//    }
//    
//    NSArray *result;
//    if (direction == JPullDirectionOlder) {
//        result = [[messages reverseObjectEnumerator] allObjects];
//    } else {
//        result = [messages copy];
//    }
//    return result;
//}

- (void)deleteMessageByClientIds:(NSArray <NSNumber *> *)clientMsgNos{
    if (clientMsgNos == nil || clientMsgNos.count == 0) {
        return;
    }
    NSString *sql = jDeleteMessage;
    sql = [sql stringByAppendingString:jClientMsgNoIn];
    sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:clientMsgNos.count]];

    [self.dbHelper executeUpdate:sql
            withArgumentsInArray:clientMsgNos];
}

- (void)deleteMessageByMessageIds:(NSArray <NSString *> *)messageIds{
    if (messageIds == nil || messageIds.count == 0) {
        return;
    }
    NSString *sql = jDeleteMessage;
    sql = [sql stringByAppendingString:jMessageIdIn];
    sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:messageIds.count]];
    [self.dbHelper executeUpdate:sql
            withArgumentsInArray:messageIds];
}

- (void)clearMessagesIn:(JConversation *)conversation startTime:(long long)startTime senderId:(NSString *)senderId{
    if (conversation.conversationId.length == 0) {
        return;
    }
    NSString *sql = jClearMessages;
    NSMutableArray *args = [[NSMutableArray alloc] initWithArray:@[@(conversation.conversationType), conversation.conversationId, @(startTime)]];
    if(senderId.length > 0) {
        sql = [sql stringByAppendingString:jAndSenderIs];
        [args addObject:senderId];
    }
    [self.dbHelper executeUpdate:sql
            withArgumentsInArray:args];
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
    for (JConcreteMessage * message in result) {
        if(message.referMsgId.length > 0){
            message.referredMsg = [self getMessageWithMessageIdEvenDelete:message.referMsgId];
        }
    }
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
    for (JConcreteMessage * message in result) {
        if(message.referMsgId.length > 0){
            message.referredMsg = [self getMessageWithMessageIdEvenDelete:message.referMsgId];
        }
    }
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

- (NSArray <JSearchConversationsResult *> *)searchMessageInConversations:(JQueryMessageOptions *)option
                                                             currentTime:(long long)now {
    __block NSString *sql = jSearchMessageInConversations;
    NSMutableArray *args = [NSMutableArray array];
    [args addObject:@(now)];
    if (option) {
        if (option.searchContent.length > 0) {
            sql = [sql stringByAppendingString:jAndSearchContentIs];
            NSString *searchString = [NSString stringWithFormat:@"%%%@%%", option.searchContent];
            [args addObject:searchString];
        }
        if (option.senderUserIds.count > 0) {
            sql = [sql stringByAppendingString:jAndSenderIn];
            sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:option.senderUserIds.count]];
            [args addObjectsFromArray:option.senderUserIds];
        }
        if (option.contentTypes.count > 0) {
            sql = [sql stringByAppendingString:jAndTypeIn];
            sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:option.contentTypes.count]];
            [args addObjectsFromArray:option.contentTypes];
        }
        if (option.conversations.count > 0) {
            sql = [sql stringByAppendingString:jAnd];
            sql = [sql stringByAppendingString:jLeftBracket];
            [option.conversations enumerateObjectsUsingBlock:^(JConversation * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                sql = [sql stringByAppendingString:jLeftBracket];
                sql = [sql stringByAppendingString:jConversationIs];
                sql = [sql stringByAppendingString:jRightBracket];
                [args addObject:@(obj.conversationType)];
                [args addObject:obj.conversationId];
                if (idx < option.conversations.count - 1) {
                    sql = [sql stringByAppendingString:jOr];
                }
            }];
            sql = [sql stringByAppendingString:jRightBracket];
        }
        if (option.states.count > 0) {
            sql = [sql stringByAppendingString:jAndStateIn];
            sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:option.states.count]];
            [args addObjectsFromArray:option.states];
        }
        if (option.conversationTypes.count > 0) {
            sql = [sql stringByAppendingString:jAndConversationTypeIn];
            sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:option.conversationTypes.count]];
            [args addObjectsFromArray:option.conversationTypes];
        }
    }
    
    sql = [sql stringByAppendingString:jGroupByConversationTypeAndId];
    sql = [sql stringByAppendingString:jOrderByTimestamp];
    sql = [sql stringByAppendingString:jDESC];
    
    NSMutableArray *resultList = [NSMutableArray array];
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:args
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        while ([resultSet next]) {
            JSearchConversationsResult *result = [[JSearchConversationsResult alloc] init];
            JConversation *c = [[JConversation alloc] init];
            c.conversationType = [resultSet intForColumn:jMessageConversationType];
            c.conversationId = [resultSet stringForColumn:jMessageConversationId];
            JConversationInfo *conversationInfo = [[JConversationInfo alloc] init];
            conversationInfo.conversation = c;
            result.conversationInfo = conversationInfo;
            result.matchedCount = [resultSet intForColumn:jMatchCount];
            [resultList addObject:result];
        }
    }];
    return [resultList copy];
}

- (NSArray <JMessage *> *)searchMessagesWithContent:(NSString *)searchContent
                                             count:(int)count
                                              time:(long long)time
                                     pullDirection:(JPullDirection)pullDirection
                                      contentTypes:(NSArray<NSString *> *)contentTypes
                                           senders:(NSArray<NSString *> *)senderUserIds
                                            states:(NSArray<NSNumber *> *)messageStates
                                     conversations:(NSArray<JConversation *> *)conversations
                                 conversationTypes:(NSArray<NSNumber *> *)conversationTypes
                                        currentTime:(long long)now {
    if (count < 1) {
        return [NSArray array];
    }
    if (time == 0) {
        time = INT64_MAX;
    }
    if (count > 200) {
        count = 200;
    }
    __block NSString *sql = jGetMessagesNotDeleted;
    NSMutableArray *args = [NSMutableArray array];
    [args addObject:@(now)];
    
    if (conversations.count > 0) {
        sql = [sql stringByAppendingString:jAnd];
        sql = [sql stringByAppendingString:jLeftBracket];
        [conversations enumerateObjectsUsingBlock:^(JConversation * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            sql = [sql stringByAppendingString:jLeftBracket];
            sql = [sql stringByAppendingString:jConversationIs];
            sql = [sql stringByAppendingString:jRightBracket];
            [args addObject:@(obj.conversationType)];
            NSString *conversationId = @"";
            if (obj.conversationId.length > 0) {
                conversationId = obj.conversationId;
            }
            [args addObject:conversationId];
            if (idx < conversations.count - 1) {
                sql = [sql stringByAppendingString:jOr];
            }
        }];
        sql = [sql stringByAppendingString:jRightBracket];
    }
    if (pullDirection == JPullDirectionNewer) {
        sql = [sql stringByAppendingString:jAndGreaterThan];
    } else {
        sql = [sql stringByAppendingString:jAndLessThan];
    }
    [args addObject:@(time)];
    
    if (searchContent.length > 0) {
        NSString *searchString = [NSString stringWithFormat:@"%%%@%%",searchContent];
        sql = [sql stringByAppendingString:jAndSearchContentIs];
        [args addObject:searchString];
    }
    if (contentTypes.count > 0) {
        sql = [sql stringByAppendingString:jAndTypeIn];
        sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:contentTypes.count]];
        [args addObjectsFromArray:contentTypes];
    }
    if (senderUserIds.count > 0) {
        sql = [sql stringByAppendingString:jAndSenderIn];
        sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:senderUserIds.count]];
        [args addObjectsFromArray:senderUserIds];
    }
    if (messageStates.count > 0) {
        sql = [sql stringByAppendingString:jAndStateIn];
        sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:messageStates.count]];
        [args addObjectsFromArray:messageStates];
    }
    if (conversationTypes.count > 0) {
        sql = [sql stringByAppendingString:jAndConversationTypeIn];
        sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:conversationTypes.count]];
        [args addObjectsFromArray:conversationTypes];
    }
    
    sql = [sql stringByAppendingString:jOrderByTimestamp];
    if (pullDirection == JPullDirectionNewer) {
        sql = [sql stringByAppendingString:jASC];
    } else {
        sql = [sql stringByAppendingString:jDESC];
    }
    sql = [sql stringByAppendingString:jLimit];
    [args addObject:@(count)];
    NSMutableArray *messages = [[NSMutableArray alloc] init];
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:args
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        while ([resultSet next]) {
            JConcreteMessage *m = [self messageWith:resultSet];
            [messages addObject:m];
        }
    }];
    for (JConcreteMessage * message in messages) {
        if(message.referMsgId.length > 0){
            message.referredMsg = [self getMessageWithMessageIdEvenDelete:message.referMsgId];
        }
    }
    NSArray *result;
    if (pullDirection == JPullDirectionOlder) {
        result = [[messages reverseObjectEnumerator] allObjects];
    } else {
        result = [messages copy];
    }
    return result;
}

- (NSString *)getLocalAttributeByMessageId:(NSString *)messageId{
    if (messageId.length == 0) {
        return @"";
    }
    NSString *sql = jGetMessageLocalAttribute;
    sql = [sql stringByAppendingString:jMessageIdIs];
    __block NSString * localAttribute;
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:@[messageId]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            localAttribute = [resultSet stringForColumn:jLocalAttribute];
        }
    }];
    return localAttribute;
    
}
- (void)setLocalAttribute:(NSString *)attribute forMessage:(NSString *)messageId{
    if (messageId.length == 0) {
        return;
    }
    if (!attribute) {
        attribute = @"";
    }
    NSString *sql = jUpdateMessageLocalAttribute;
    sql = [sql stringByAppendingString:jMessageIdIs];
    [self.dbHelper executeUpdate:sql
            withArgumentsInArray:@[attribute, messageId]];
    
}
- (NSString *)getLocalAttributeByClientMsgNo:(long long)clientMsgNo{
    NSString *sql = jGetMessageLocalAttribute;
    sql = [sql stringByAppendingString:jClientMsgNoIs];
    __block NSString * localAttribute;
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:@[@(clientMsgNo)]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            localAttribute = [resultSet stringForColumn:jLocalAttribute];
        }
    }];
    return localAttribute;
    
}
- (void)setLocalAttribute:(NSString *)attribute forClientMsgNo:(long long)clientMsgNo{
    if (!attribute) {
        attribute = @"";
    }
    NSString *sql = jUpdateMessageLocalAttribute;
    sql = [sql stringByAppendingString:jClientMsgNoIs];
    [self.dbHelper executeUpdate:sql
            withArgumentsInArray:@[attribute, @(clientMsgNo)]];
    
}

- (JConcreteMessage *)getLastMessage:(JConversation *)conversation
                         currentTime:(long long)now {
    if(conversation.conversationId.length == 0){
        return nil;
    }
    NSString * sql = jGetMessagesInConversation;
    sql = [sql stringByAppendingString:jOrderByTimestamp];
    sql = [sql stringByAppendingString:jDESC];
    sql = [sql stringByAppendingString:jLimit];
    NSMutableArray *messages = [[NSMutableArray alloc] init];
    NSMutableArray *args = [[NSMutableArray alloc] initWithArray:@[@(now), @(conversation.conversationType), conversation.conversationId, @(1)]];
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:args
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        while ([resultSet next]) {
            JConcreteMessage *m = [self messageWith:resultSet];
            [messages addObject:m];
        }
    }];
    JConcreteMessage * lastMessage;
    if(messages.count >= 1){
        lastMessage = messages.firstObject;
            if(lastMessage.referMsgId.length > 0){
                lastMessage.referredMsg = [self getMessageWithMessageIdEvenDelete:lastMessage.referMsgId];
            }
    }
    
    return lastMessage;
}

- (void)setMessageState:(JMessageState)state
        withClientMsgNo:(long long)clientMsgNo {
    [self.dbHelper executeUpdate:jUpdateMessageState
            withArgumentsInArray:@[@(state), @(clientMsgNo)]];
}

- (void)setMessagesRead:(NSArray<NSString *> *)messageIds {
    NSString *sql = [jSetMessagesRead stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:messageIds.count]];
    [self.dbHelper executeUpdate:sql
            withArgumentsInArray:messageIds];
}

- (void)setGroupMessageReadInfo:(NSDictionary<NSString *,JGroupMessageReadInfo *> *)msgs {
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        [msgs enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull key, JGroupMessageReadInfo * _Nonnull obj, BOOL * _Nonnull stop) {
            [db executeUpdate:jSetGroupReadInfo
         withArgumentsInArray:@[@(obj.readCount), @(obj.memberCount), key]];
        }];
    }];
}

- (void)clearChatroomMessageExclude:(NSArray<NSString *> *)chatroomIds {
    NSString *sql = [jClearChatroomMessagesExclude stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:chatroomIds.count]];
    [self.dbHelper executeUpdate:sql
            withArgumentsInArray:chatroomIds];
}

- (void)clearChatroomMessage:(NSString *)chatroomId {
    NSString *sql = jClearChatroomMessagesIn;
    [self.dbHelper executeUpdate:sql
            withArgumentsInArray:@[chatroomId]];
}

- (void)createTables {
    [self.dbHelper executeUpdate:kCreateMessageTable withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:kCreateMessageIndex withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:kCreateClientUidIndex withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:kCreateMessageConversationIndex withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:jCreateMessageConversationTSIndex withArgumentsInArray:nil];
    [[NSUserDefaults standardUserDefaults] setObject:@(jMessageTableVersion) forKey:jMessageTableVersionKey];
}

- (void)updateTables {
    NSNumber *existedVersionNumber = [[NSUserDefaults standardUserDefaults] objectForKey:jMessageTableVersionKey];
    int existedVersion = existedVersionNumber.intValue;
    if (jMessageTableVersion > existedVersion) {
        //update table
        if (existedVersion == 1 && jMessageTableVersion >= 2) {
            [self.dbHelper executeUpdate:kCreateClientUidIndex withArgumentsInArray:nil];
            existedVersion = 2;
        }
        if (existedVersion == 2 && jMessageTableVersion >= 3) {
            [self.dbHelper executeUpdate:kCreateMessageConversationIndex withArgumentsInArray:nil];
            existedVersion = 3;
        }
        
        [[NSUserDefaults standardUserDefaults] setObject:@(jMessageTableVersion) forKey:jMessageTableVersionKey];
    }
}

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper {
    if (self = [super init]){
        self.dbHelper = dbHelper;
    }
    return self;
}

#pragma mark - operation with db
- (void)insertMessage:(JMessage *)message inDb:(JFMDatabase *)db {
    long long seqNo = 0;
    long long msgIndex = 0;
    NSString *clientUid = @"";
    int flags = 0;
    long long lifeTime = 0;
    if ([message isKindOfClass:[JConcreteMessage class]]) {
        seqNo = ((JConcreteMessage *)message).seqNo;
        msgIndex = ((JConcreteMessage *)message).msgIndex;
        clientUid = ((JConcreteMessage *)message).clientUid;
        flags = ((JConcreteMessage *)message).flags;
        lifeTime = ((JConcreteMessage *)message).lifeTime;
    }
    NSData *data = [message.content encode];
    NSString *content = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    int memberCount = message.groupReadInfo.memberCount?:-1;
    
    NSString * mentionInfo;
    if(message.mentionInfo){
        mentionInfo = [message.mentionInfo encodeToJson];
    }else{
        mentionInfo = @"";
    }
    
    NSString * referMsgId;
    if (message.referredMsg) {
        referMsgId = message.referredMsg.messageId;
        if (referMsgId == nil) {
            referMsgId = @"";
        }
    }
    
    [db executeUpdate:jInsertMessage,
                      @(message.conversation.conversationType),
                      message.conversation.conversationId,
                      message.contentType,
                      message.messageId,
                      clientUid,
                      @(message.direction),
                      @(message.messageState),
                      @(message.hasRead),
                      @(message.timestamp),
                      message.senderUserId,
                      content,
                      @(seqNo),
                      @(msgIndex),
                      @(message.groupReadInfo.readCount),
                      @(memberCount),
                      message.content.searchContent,
                      mentionInfo,
                      referMsgId,
                      @(flags),
                      @(message.isDeleted),
                      @(lifeTime),
                      @(message.lifeTimeAfterRead),
                      @(message.destroyTime)
    ];
}

- (JConcreteMessage *)getMessageWithMessageId:(NSString *)messageId
                                  currentTime:(long long)now
                                         inDb:(JFMDatabase *)db {
    if (messageId.length == 0) {
        return nil;
    }
    JConcreteMessage *message = nil;
    JFMResultSet *resultSet = [db executeQuery:kGetMessageWithMessageId, messageId, @(now)];
    if ([resultSet next]) {
        message = [self messageWith:resultSet];
    }
    [resultSet close];
    return message;
}

#pragma mark - update table
+ (NSString *)alterTableAddFlags {
    return kAlterAddFlags;
}

+ (NSString *)addConversationTSIndex {
    return jCreateMessageConversationTSIndex;
}

+ (NSString *)addMessageClientUidIndex {
    return kCreateClientUidIndex;
}

+ (NSString *)alterTableAddLifeTime {
    return kAlterAddLifeTime;
}

+ (NSString *)alterTableAddLifeTimeAfterRead {
    return kAlterAddLifeTimeAfterRead;
}

+ (NSString *)alterTableAddDestroyTime {
    return kAlterAddDestroyTime;
}

#pragma mark - internal
- (JConcreteMessage *)getMessageWithClientUid:(NSString *)clientUid
                                         inDb:(JFMDatabase *)db {
    if (clientUid.length == 0) {
        return nil;
    }
    JConcreteMessage *message = nil;
    JFMResultSet *resultSet = [db executeQuery:kGetMessageWithClientUid, clientUid];
    if ([resultSet next]) {
        message = [self messageWith:resultSet];
    }
    [resultSet close];
    return message;
}

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
    if([message.content isKindOfClass:[JMergeMessage class]]){
        JMergeMessage * mergeMessage = (JMergeMessage *)message.content;
        if(mergeMessage.containerMsgId == nil || mergeMessage.containerMsgId.length == 0){
            mergeMessage.containerMsgId = message.messageId;
        }
    }
    message.seqNo = [rs longLongIntForColumn:jSeqNo];
    message.msgIndex = [rs longLongIntForColumn:jMessageIndex];
    JGroupMessageReadInfo *info = [[JGroupMessageReadInfo alloc] init];
    info.readCount = [rs intForColumn:jReadCount];
    info.memberCount = [rs intForColumn:jMemberCount];
    message.groupReadInfo = info;
    NSString *mentionInfoStr = [rs stringForColumn:jMessageMentionInfo];
    if (mentionInfoStr.length > 0) {
        message.mentionInfo = [JMessageMentionInfo decodeFromJson:mentionInfoStr];
    }
    message.referMsgId = [rs stringForColumn:jReferMsgId];
    message.localAttribute = [rs stringForColumn:jLocalAttribute];
    message.flags = [rs intForColumn:jFlags];
    message.isEdit = message.flags & JMessageFlagIsModified;
    message.isDeleted = [rs boolForColumn:jIsDeleted];
    message.lifeTime = [rs longLongIntForColumn:jLifeTime];
    message.lifeTimeAfterRead = [rs longLongIntForColumn:jLifeTimeAfterRead];
    message.destroyTime = [rs longLongIntForColumn:jDestroyTime];
    return message;
}

@end
