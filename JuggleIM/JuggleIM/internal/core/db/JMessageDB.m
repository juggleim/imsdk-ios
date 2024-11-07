//
//  JMessageDB.m
// JuggleIM
//
//  Created by Nathan on 2023/12/8.
//

#import "JMessageDB.h"
#import "JContentTypeCenter.h"

//message 最新版本
#define jMessageTableVersion 1
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
                                        "member_count INTEGER DEFAULT -1,"
                                        "is_deleted BOOLEAN DEFAULT 0,"
                                        "search_content TEXT,"
                                        "local_attribute TEXT,"
                                        "mention_info TEXT,"
                                        "refer_msg_id VARCHAR (64)"
                                        ")";
NSString *const kCreateMessageIndex = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_message ON message(message_uid)";
NSString *const kGetMessageWithMessageId = @"SELECT * FROM message WHERE message_uid = ? AND is_deleted = 0";
NSString *const jGetMessagesInConversation = @"SELECT * FROM message WHERE conversation_type = ? AND conversation_id = ? AND is_deleted = 0";
NSString *const jAndGreaterThan = @" AND timestamp > ?";
NSString *const jAndLessThan = @" AND timestamp < ?";
NSString *const jAndTypeIn = @" AND type IN ";
NSString *const jAndSenderIn = @" AND sender IN ";
NSString *const jAndStateIn = @" AND state IN ";
NSString *const jOrderByTimestamp = @" ORDER BY timestamp";
NSString *const jAnd = @" AND";
NSString *const jLeftBracket = @" (";
NSString *const jRightBracket = @")";
NSString *const jConversationIs = @" conversation_type = ? AND conversation_id = ?";
NSString *const jOr = @" OR";
NSString *const jASC = @" ASC";
NSString *const jDESC = @" DESC";
NSString *const jLimit = @" LIMIT ?";
NSString *const jInsertMessage = @"INSERT INTO message (conversation_type, conversation_id, type, message_uid, client_uid, direction, state, has_read, timestamp, sender, content, seq_no, message_index, read_count, member_count, search_content, mention_info ,refer_msg_id) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
NSString *const jUpdateMessageAfterSend = @"UPDATE message SET message_uid = ?, state = ?, timestamp = ?, seq_no = ? WHERE id = ?";
NSString *const jUpdateMessageContent = @"UPDATE message SET content = ?, type = ?,search_content = ? WHERE ";
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
NSString *const jGetMessagesBySearchContent = @"SELECT * FROM message WHERE search_content LIKE ? AND is_deleted = 0";
NSString *const jGetMessagesNotDeleted = @"SELECT * FROM message WHERE is_deleted = 0";
NSString *const jAndSearchContentIs = @" AND search_content LIKE ?";
NSString *const jAndInConversation = @" AND conversation_id = ?";
NSString *const jGetMessageLocalAttribute = @"SELECT local_attribute FROM message WHERE";
NSString *const jUpdateMessageLocalAttribute = @"UPDATE message SET local_attribute = ? WHERE";
NSString *const jClearChatroomMessagesExclude = @"DELETE FROM message WHERE conversation_type = 3 AND conversation_id NOT IN ";
NSString *const jClearChatroomMessagesIn = @"DELETE FROM message WHERE conversation_type = 3 AND conversation_id = ?";

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
    if(message.referMsgId.length > 0){
        message.referredMsg = [self getMessageWithMessageId:message.referMsgId];
    }
    return message;
}

- (void)insertMessages:(NSArray<JConcreteMessage *> *)messages {
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        [messages enumerateObjectsUsingBlock:^(JConcreteMessage * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            JConcreteMessage *m = nil;
            if (obj.messageId.length > 0) {
                m = [self getMessageWithMessageId:obj.messageId inDb:db];
            }
            if (m) {
                obj.clientMsgNo = m.clientMsgNo;
                obj.existed = YES;
            } else {
                [self insertMessage:obj inDb:db];
                obj.clientMsgNo = db.lastInsertRowId;
            }
            
            if(obj.referredMsg != nil) {
                JConcreteMessage * ref = [self getMessageWithMessageId:obj.referredMsg.messageId inDb:db];
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
                         seqNo:(long long)seqNo {
    if (messageId.length == 0) {
        return;
    }
    [self.dbHelper executeUpdate:jUpdateMessageAfterSend
            withArgumentsInArray:@[messageId, @(JMessageStateSent), @(timestamp), @(seqNo), @(clientMsgNo)]];
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

- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction
                            contentTypes:(NSArray<NSString *> *)contentTypes {
    if (conversation.conversationId.length == 0) {
        return [NSArray array];
    }
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
    for (JConcreteMessage * message in messages) {
        if(message.referMsgId.length > 0){
            message.referredMsg = [self getMessageWithMessageId:message.referMsgId];
        }
    }
    
    NSArray *result;
    if (direction == JPullDirectionOlder) {
        result = [[messages reverseObjectEnumerator] allObjects];
    } else {
        result = [messages copy];
    }
    return result;
}

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
            message.referredMsg = [self getMessageWithMessageId:message.referMsgId];
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
            message.referredMsg = [self getMessageWithMessageId:message.referMsgId];
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

- (NSArray<JMessage *> *)searchMessagesWithContent:(NSString *)searchContent
                                             count:(int)count
                                              time:(long long)time
                                     pullDirection:(JPullDirection)pullDirection
                                      contentTypes:(NSArray<NSString *> *)contentTypes
                                           senders:(NSArray<NSString *> *)senderUserIds
                                            states:(NSArray<NSNumber *> *)messageStates
                                     conversations:(NSArray<JConversation *> *)conversations {
    if (count < 1) {
        return [NSArray array];
    }
    if (time == 0) {
        time = INT64_MAX;
    }
    if (count > 100) {
        count = 100;
    }
    __block NSString *sql = jGetMessagesNotDeleted;
    NSMutableArray *args = [NSMutableArray array];
    if (searchContent.length > 0) {
        NSString *searchString = [NSString stringWithFormat:@"%%%@%%",searchContent];
        sql = [sql stringByAppendingString:jAndSearchContentIs];
        [args addObject:searchString];
    }
    if (pullDirection == JPullDirectionNewer) {
        sql = [sql stringByAppendingString:jAndGreaterThan];
    } else {
        sql = [sql stringByAppendingString:jAndLessThan];
    }
    [args addObject:@(time)];
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
    if (conversations.count > 0) {
        sql = [sql stringByAppendingString:jAnd];
        sql = [sql stringByAppendingString:jLeftBracket];
        [conversations enumerateObjectsUsingBlock:^(JConversation * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            sql = [sql stringByAppendingString:jLeftBracket];
            sql = [sql stringByAppendingString:jConversationIs];
            sql = [sql stringByAppendingString:jRightBracket];
            [args addObject:@(obj.conversationType)];
            [args addObject:obj.conversationId];
            if (idx < conversations.count - 1) {
                sql = [sql stringByAppendingString:jOr];
            }
        }];
        sql = [sql stringByAppendingString:jRightBracket];
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
            message.referredMsg = [self getMessageWithMessageId:message.referMsgId];
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

- (JConcreteMessage *)getLastMessage:(JConversation *)conversation{
    if(conversation.conversationId.length == 0){
        return nil;
    }
    NSString * sql = jGetMessagesInConversation;
    sql = [sql stringByAppendingString:jOrderByTimestamp];
    sql = [sql stringByAppendingString:jDESC];
    sql = [sql stringByAppendingString:jLimit];
    NSMutableArray *messages = [[NSMutableArray alloc] init];
    NSMutableArray *args = [[NSMutableArray alloc] initWithArray:@[@(conversation.conversationType), conversation.conversationId, @(1)]];
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
                lastMessage.referredMsg = [self getMessageWithMessageId:lastMessage.referMsgId];
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
    [[NSUserDefaults standardUserDefaults] setObject:@(jMessageTableVersion) forKey:jMessageTableVersionKey];
}

- (void)updateTables {
    NSNumber *existedVersion = [[NSUserDefaults standardUserDefaults] objectForKey:jMessageTableVersionKey];
    if (jMessageTableVersion > existedVersion.intValue) {
        //update table
        
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
    if ([message isKindOfClass:[JConcreteMessage class]]) {
        seqNo = ((JConcreteMessage *)message).seqNo;
        msgIndex = ((JConcreteMessage *)message).msgIndex;
        clientUid = ((JConcreteMessage *)message).clientUid;
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
    if(message.referredMsg){
        referMsgId = message.referredMsg.messageId;
        if(referMsgId == nil){
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
                      referMsgId];
}

- (JConcreteMessage *)getMessageWithMessageId:(NSString *)messageId
                                         inDb:(JFMDatabase *)db {
    if (messageId.length == 0) {
        return nil;
    }
    JConcreteMessage *message = nil;
    JFMResultSet *resultSet = [db executeQuery:kGetMessageWithMessageId, messageId];
    if ([resultSet next]) {
        message = [self messageWith:resultSet];
    }
    return message;
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
    return message;
}

@end
