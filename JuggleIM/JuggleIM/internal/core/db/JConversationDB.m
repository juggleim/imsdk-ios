//
//  JConversationDB.m
// JuggleIM
//
//  Created by Nathan on 2023/12/8.
//

#import "JConversationDB.h"
#import "JContentTypeCenter.h"

//conversation_info 最新版本
#define jConversationTableVersion 2
//NSUserDefault 中保存 conversation_info 数据库版本的 key
#define jConversationTableVersionKey @"ConversationVersion"

NSString *const kCreateConversationTable = @"CREATE TABLE IF NOT EXISTS conversation_info ("
                                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                        "conversation_type SMALLINT,"
                                        "conversation_id VARCHAR (64),"
                                        "draft TEXT,"
                                        "timestamp INTEGER,"
                                        "last_message_id VARCHAR (64),"
                                        "last_read_message_index INTEGER,"//最后一条读过的消息的未读数 index
                                        "last_message_index INTEGER,"//最后一条消息的未读数 index
                                        "is_top BOOLEAN,"
                                        "top_time INTEGER,"
                                        "mute BOOLEAN,"
                                        "mention_info VARCHAR (64),"
                                        "last_message_type VARCHAR (64),"
                                        "last_message_client_uid VARCHAR (64),"
                                        "last_message_client_msg_no INTEGER,"
                                        "last_message_direction BOOLEAN,"
                                        "last_message_state SMALLINT,"
                                        "last_message_has_read BOOLEAN,"
                                        "last_message_timestamp INTEGER,"
                                        "last_message_sender VARCHAR (64),"
                                        "last_message_content TEXT,"
                                        "last_message_mention_info TEXT,"
                                        "last_message_seq_no INTEGER,"//最后一条消息的排序号
                                        "unread_tag BOOLEAN"
//                                        "extra TEXT"
                                        ")";
NSString *const kCreateConversationIndex = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_conversation ON conversation_info(conversation_type, conversation_id)";
NSString *const kInsertConversation = @"INSERT OR REPLACE INTO conversation_info"
                                       "(conversation_type, conversation_id, timestamp, last_message_id,"
                                       "last_read_message_index, last_message_index, is_top, top_time, mute, mention_info,"
                                       "last_message_type, last_message_client_uid, last_message_client_msg_no, last_message_direction, last_message_state,"
                                       "last_message_has_read, last_message_timestamp, last_message_sender, last_message_content, last_message_mention_info,"
                                       "last_message_seq_no, unread_tag)"
                                       "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
NSString *const jUpdateConversation = @"UPDATE conversation_info SET timestamp=?, last_message_id=?, last_read_message_index=?, "
                                       "last_message_index=?, is_top=?, top_time=?, mute=?, mention_info=?, last_message_type=?,  "
                                       "last_message_client_uid=?, last_message_client_msg_no=?, last_message_direction=?, last_message_state=?, "
                                       "last_message_has_read=?, last_message_timestamp=?, last_message_sender=?, "
                                       "last_message_content=?, last_message_mention_info=?, last_message_seq_no=?, unread_tag =?  WHERE conversation_type = ? "
                                       "AND conversation_id = ?";
NSString *const kGetConversation = @"SELECT * FROM conversation_info WHERE conversation_type = ? AND conversation_id = ?";
NSString *const jGetConversations = @"SELECT * FROM conversation_info ORDER BY is_top DESC, top_time DESC, timestamp DESC";
NSString *const jGetConversationsBy = @"SELECT * FROM conversation_info WHERE";
NSString *const jIsTopEqualsTrue = @" is_top = 1";
NSString *const jTimestampGreaterThan = @" timestamp > ?";
NSString *const jTimestampLessThan = @" timestamp < ?";
NSString *const jConversationAnd = @" AND ";
NSString *const jConversationTypeIn = @" conversation_type in ";
NSString *const jConversationOrderByTopAndTimestamp = @" ORDER BY is_top DESC, top_time DESC, timestamp DESC";
NSString *const jConversationOrderByTopTime = @" ORDER BY top_time DESC";
NSString *const jConversationLimit = @" LIMIT ?";
NSString *const jDeleteConversation = @"DELETE FROM conversation_info WHERE conversation_type = ? AND conversation_id = ?";
NSString *const jSetDraft = @"UPDATE conversation_info SET draft = ? WHERE conversation_type = ? AND conversation_id = ?";
NSString *const jClearUnreadCount = @"UPDATE conversation_info SET last_read_message_index = ? WHERE conversation_type = ? AND conversation_id = ?";
NSString *const jUpdateLastMessage = @"UPDATE conversation_info SET last_message_id=?, last_message_type=?,"
                                      "last_message_client_uid=?, last_message_client_msg_no=?, "
                                      "last_message_direction=?, last_message_state=?, last_message_has_read=?, last_message_timestamp=?, "
                                      "last_message_sender=?, last_message_content=?, last_message_mention_info=?, last_message_seq_no=?";
NSString *const jClearLastMessage = @"UPDATE conversation_info SET "
                                     "last_message_id=NULL, last_message_type=NULL,"
                                     "last_message_client_uid=NULL, last_message_client_msg_no=0, last_message_seq_no=0,"
                                     "last_message_direction=0, last_message_state=0, last_message_has_read=0, last_message_timestamp=0, "
                                     "last_message_sender=NULL, last_message_content=NULL, last_message_mention_info=NULL, "
                                     "mention_info=NULL";
NSString *const jTimestampEqualsQustion = @", timestamp=?";
NSString *const jLastMessageIndexEqualsQuestion = @", last_message_index=?";
NSString *const jSetMute = @"UPDATE conversation_info SET mute = ? WHERE conversation_type = ? AND conversation_id = ?";
NSString *const jSetTop = @"UPDATE conversation_info SET is_top = ?, top_time = ?";
NSString *const jSetUnread = @"UPDATE conversation_info SET unread_tag = ?";
NSString *const jGetTotalUnreadCount = @"SELECT SUM(CASE WHEN last_message_index - last_read_message_index >= 0 THEN last_message_index - last_read_message_index ELSE 0 END) AS total_count FROM conversation_info WHERE mute = 0";
NSString *const jWhereConversationIs = @" WHERE conversation_type = ? AND conversation_id = ?";
NSString *const jClearTotalUnreadCount = @"UPDATE conversation_info SET last_read_message_index = last_message_index";
NSString *const jUpdateConversationTime = @"UPDATE conversation_info SET timestamp = ?";
NSString *const jUpdateConversationMentionInfo = @"UPDATE conversation_info SET mention_info = ? WHERE conversation_type = ? AND conversation_id = ?";
NSString *const jClearConversationMentionInfo = @"UPDATE conversation_info SET mention_info = NULL";
NSString *const jUpdateConversationLastMessageHasRead = @"UPDATE conversation_info SET last_message_has_read = 1 WHERE conversation_type = ? AND conversation_id = ?";
NSString *const jUpdateConversationLastMessageState = @"UPDATE conversation_info SET last_message_state = ? WHERE conversation_type = ? AND conversation_id = ? AND last_message_client_msg_no = ?";

NSString *const jConversationType = @"conversation_type";
NSString *const jConversationId = @"conversation_id";
NSString *const jDraft = @"draft";
NSString *const jConversationTimestamp = @"timestamp";
NSString *const jLastMessageId = @"last_message_id";
NSString *const jLastReadMessageIndex = @"last_read_message_index";
NSString *const jLastMessageIndex = @"last_message_index";
NSString *const jIsTop = @"is_top";
NSString *const jTopTime = @"top_time";
NSString *const jMute = @"mute";
NSString *const jConversationMentionInfo = @"mention_info";
NSString *const jLastMessageType = @"last_message_type";
NSString *const jLastMessageClientUid = @"last_message_client_uid";
NSString *const jLastMessageDirection = @"last_message_direction";
NSString *const jLastMessageState = @"last_message_state";
NSString *const jLastMessageHasRead = @"last_message_has_read";
NSString *const jLastMessageTimestamp = @"last_message_timestamp";
NSString *const jLastMessageSender = @"last_message_sender";
NSString *const jLastMessageContent = @"last_message_content";
NSString *const jLastMessageMentionInfo = @"last_message_mention_info";
NSString *const jLastMessageClientMsgNo = @"last_message_client_msg_no";
NSString *const jLastMessageSeqNo = @"last_message_seq_no";
NSString *const jTotalCount = @"total_count";
NSString *const jHasUnread = @"unread_tag";

@interface JConversationDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JConversationDB

- (void)createTables {
    [self.dbHelper executeUpdate:kCreateConversationTable withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:kCreateConversationIndex withArgumentsInArray:nil];
    [[NSUserDefaults standardUserDefaults] setObject:@(jConversationTableVersion) forKey:jConversationTableVersionKey];
}

- (void)updateTables {
    NSNumber *existedVersionNumber = [[NSUserDefaults standardUserDefaults] objectForKey:jConversationTableVersionKey];
    int existedVersion = existedVersionNumber.intValue;
    if (jConversationTableVersion > existedVersion) {
        //update table
        if (existedVersion == 1 && jConversationTableVersion >= 2) {
            NSString *s = @"ALTER TABLE conversation_info ADD COLUMN unread_tag BOOLEAN";
            [self.dbHelper executeUpdate:s withArgumentsInArray:nil];
            existedVersion = 2;
        }
        
        [[NSUserDefaults standardUserDefaults] setObject:@(jConversationTableVersion) forKey:jConversationTableVersionKey];
    }
}

- (void)insertConversations:(NSArray<JConcreteConversationInfo *> *)conversations
                 completion:(nullable void (^)(NSArray<JConcreteConversationInfo *> * _Nonnull, NSArray<JConcreteConversationInfo *> * _Nonnull))completeBlock {
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
            NSString * mentionInfo;
            if(obj.mentionInfo){
                mentionInfo = [obj.mentionInfo encodeToJson];
            }else{
                mentionInfo = @"";
            }
            NSString * lastMsgMentionInfo;
            if(lastMessage.mentionInfo != nil){
                lastMsgMentionInfo = [lastMessage.mentionInfo encodeToJson];
            }else{
                lastMsgMentionInfo = @"";
            }
            if (info) {
                [updateConversations addObject:obj];
                [db executeUpdate:jUpdateConversation, @(obj.sortTime), lastMessage.messageId, @(obj.lastReadMessageIndex), @(obj.lastMessageIndex), @(obj.isTop), @(obj.topTime), @(obj.mute), mentionInfo, lastMessage.contentType, lastMessage.clientUid, @(lastMessage.clientMsgNo),@(lastMessage.direction), @(lastMessage.messageState), @(lastMessage.hasRead), @(lastMessage.timestamp), lastMessage.senderUserId, content, lastMsgMentionInfo, @(lastMessage.seqNo), @(obj.hasUnread), @(obj.conversation.conversationType), obj.conversation.conversationId];
            } else {
                [insertConversations addObject:obj];
                [db executeUpdate:kInsertConversation, @(obj.conversation.conversationType), obj.conversation.conversationId, @(obj.sortTime), lastMessage.messageId, @(obj.lastReadMessageIndex), @(obj.lastMessageIndex), @(obj.isTop), @(obj.topTime), @(obj.mute), mentionInfo, lastMessage.contentType, lastMessage.clientUid, @(lastMessage.clientMsgNo),@(lastMessage.direction), @(lastMessage.messageState), @(lastMessage.hasRead), @(lastMessage.timestamp), lastMessage.senderUserId, content, lastMsgMentionInfo, @(lastMessage.seqNo), @(obj.hasUnread)];
            }
        }];
    }];
    if (completeBlock) {
        completeBlock(insertConversations, updateConversations);
    }
}

- (JConcreteConversationInfo *)getConversationInfo:(JConversation *)conversation {
    if (conversation.conversationId.length == 0) {
        return nil;
    }
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
    if (conversation.conversationId.length == 0) {
        return;
    }
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
    sql = [sql stringByAppendingString:jConversationOrderByTopAndTimestamp];
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

- (NSArray<JConversationInfo *> *)getTopConversationInfoListWithTypes:(NSArray<NSNumber *> *)conversationTypes
                                                                count:(int)count
                                                            timestamp:(long long)ts
                                                            direction:(JPullDirection)direction {
    if (ts == 0) {
        ts = INT64_MAX;
    }
    NSMutableArray *args = [[NSMutableArray alloc] init];
    NSString *sql = jGetConversationsBy;
    sql = [sql stringByAppendingString:jIsTopEqualsTrue];
    sql = [sql stringByAppendingString:jConversationAnd];
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
    sql = [sql stringByAppendingString:jConversationOrderByTopTime];
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
    if (conversation.conversationId.length == 0) {
        return;
    }
    if (!draft) {
        draft = @"";
    }
    [self.dbHelper executeUpdate:jSetDraft withArgumentsInArray:@[draft, @(conversation.conversationType), conversation.conversationId]];
}

- (void)clearDraftInConversation:(JConversation *)conversation {
    [self setDraft:@"" inConversation:conversation];
}

- (void)clearUnreadCountBy:(JConversation *)conversation
                  msgIndex:(long long)msgIndex {
    if (conversation.conversationId.length == 0) {
        return;
    }
    [self.dbHelper executeUpdate:jClearUnreadCount withArgumentsInArray:@[@(msgIndex), @(conversation.conversationType), conversation.conversationId]];
}

- (void)updateLastMessage:(JConcreteMessage *)message {
    NSData *data = [message.content encode];
    NSString *content = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    NSString *sql = jUpdateLastMessage;
    
    BOOL isUpdateSortTime = YES;
    if (message.direction == JMessageDirectionSend && (message.flags & JMessageFlagIsBroadcast)) {
        isUpdateSortTime = NO;
    }
    if (isUpdateSortTime) {
        sql = [sql stringByAppendingString:jTimestampEqualsQustion];
    }
    if (message.direction == JMessageDirectionReceive) {
        sql = [sql stringByAppendingString:jLastMessageIndexEqualsQuestion];
    }
    sql = [sql stringByAppendingString:jWhereConversationIs];
    
    NSString * mentionInfo;
    if(message.mentionInfo){
        mentionInfo = [message.mentionInfo encodeToJson];
    }else{
        mentionInfo = @"";
    }
    
    NSMutableArray *args = [[NSMutableArray alloc] initWithArray:@[message.messageId?:@"",
                                                                   message.contentType?:@"",
                                                                   message.clientUid?:@"",
                                                                   @(message.clientMsgNo),
                                                                   @(message.direction),
                                                                   @(message.messageState),
                                                                   @(message.hasRead),
                                                                   @(message.timestamp),
                                                                   message.senderUserId?:@"",
                                                                   content?:@"",
                                                                   mentionInfo?:@"",
                                                                   @(message.seqNo)]];
    if (isUpdateSortTime) {
        [args addObject:@(message.timestamp)];
    }
    if (message.direction == JMessageDirectionReceive) {
        [args addObject:@(message.msgIndex)];
    }
    [args addObject:@(message.conversation.conversationType)];
    [args addObject:message.conversation.conversationId];
    
    [self.dbHelper executeUpdate:sql withArgumentsInArray:args];
} 

- (void)setMute:(BOOL)isMute conversation:(JConversation *)conversation {
    if (conversation.conversationId.length == 0) {
        return;
    }
    [self.dbHelper executeUpdate:jSetMute withArgumentsInArray:@[@(isMute), @(conversation.conversationType), conversation.conversationId]];
}

- (void)setTop:(BOOL)isTop time:(long long)time conversation:(JConversation *)conversation {
    if (conversation.conversationId.length == 0) {
        return;
    }
    NSString *sql;
    sql = jSetTop;
    if (!isTop) {
        time = 0;
    }
    sql = [sql stringByAppendingString:jWhereConversationIs];
    [self.dbHelper executeUpdate:sql withArgumentsInArray:@[@(isTop), @(time), @(conversation.conversationType), conversation.conversationId]];
}

- (void)setUnread:(BOOL)isUnread
     conversation:(JConversation *)conversation {
    if (conversation.conversationId.length == 0) {
        return;
    }
    NSString *sql;
    sql = [jSetUnread stringByAppendingString:jWhereConversationIs];
    [self.dbHelper executeUpdate:sql withArgumentsInArray:@[@(isUnread), @(conversation.conversationType), conversation.conversationId]];
}

- (void)clearUnreadTag {
    NSString *sql = jSetUnread;
    [self.dbHelper executeUpdate:sql withArgumentsInArray:@[@(NO)]];
}

- (int)getTotalUnreadCount {
    __block int count = 0;
    [self.dbHelper executeQuery:jGetTotalUnreadCount
           withArgumentsInArray:nil
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            count = [resultSet intForColumn:jTotalCount];
        }
    }];
    return count;
}

- (void)clearTotalUnreadCount{
    [self.dbHelper executeUpdate:jClearTotalUnreadCount withArgumentsInArray:nil];
}

- (void)updateTime:(long long)time
   forConversation:(JConversation *)conversation {
    if (conversation.conversationId.length == 0) {
        return;
    }
    NSString *sql = [jUpdateConversationTime stringByAppendingString:jWhereConversationIs];
    [self.dbHelper executeUpdate:sql withArgumentsInArray:@[@(time), @(conversation.conversationType), conversation.conversationId]];
}


-(void)setMentionInfo:(JConversation *)conversation
      mentionInfoJson:(NSString *)mentionInfoJson{
    if (conversation.conversationId.length == 0) {
        return;
    }
    [self.dbHelper executeUpdate:jUpdateConversationMentionInfo withArgumentsInArray:@[mentionInfoJson, @(conversation.conversationType), conversation.conversationId]];
}

-(void)clearMentionInfo{
    [self.dbHelper executeUpdate:jClearConversationMentionInfo withArgumentsInArray:nil];
}

- (void)clearLastMessage:(JConversation *)conversation{
    if (conversation.conversationId.length == 0) {
        return;
    }
    NSString *sql = jClearLastMessage;
    sql = [sql stringByAppendingString:jWhereConversationIs];
    [self.dbHelper executeUpdate:sql withArgumentsInArray:@[@(conversation.conversationType), conversation.conversationId]];


}

- (void)updateLastMessageWithoutIndex:(JConcreteMessage *)message{
    NSString *sql = jUpdateLastMessage;
    sql = [sql stringByAppendingString:jWhereConversationIs];
    
    NSData *data = [message.content encode];
    NSString *content = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    NSString * mentionInfo;
    if(message.mentionInfo){
        mentionInfo = [message.mentionInfo encodeToJson];
    }else{
        mentionInfo = @"";
    }
    
    NSMutableArray *args = [[NSMutableArray alloc] initWithArray:@[message.messageId?:@"",
                                                                   message.contentType?:@"",
                                                                   message.clientUid?:@"",
                                                                   @(message.clientMsgNo),
                                                                   @(message.direction),
                                                                   @(message.messageState),
                                                                   @(message.hasRead),
                                                                   @(message.timestamp),
                                                                   message.senderUserId?:@"",
                                                                   content?:@"",
                                                                   mentionInfo?:@"",
                                                                   @(message.seqNo),
                                                                   @(message.conversation.conversationType),
                                                                   message.conversation.conversationId?:@""]];
    [self.dbHelper executeUpdate:sql withArgumentsInArray:args];
}
- (void)setLastMessageHasRead:(JConversation *)conversation{
    [self.dbHelper executeUpdate:jUpdateConversationLastMessageHasRead withArgumentsInArray:@[@(conversation.conversationType), conversation.conversationId]];
}
- (void)updateLastMessageState:(JConversation *)conversation
                         state:(JMessageState)state
               withClientMsgNo:(long long)clientMsgNo{
    NSString *sql = jUpdateConversationLastMessageState;
    NSMutableArray *args = [[NSMutableArray alloc] initWithArray:@[@(state),
                                                                   @(conversation.conversationType),
                                                                   conversation.conversationId,
                                                                   @(clientMsgNo)]];
    [self.dbHelper executeUpdate:sql withArgumentsInArray:args];

}

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper {
    if (self = [super init]) {
        self.dbHelper = dbHelper;
    }
    return self;
}

#pragma mark - internal
- (JConcreteConversationInfo *)conversationInfoWith:(JFMResultSet *)rs {
    JConcreteConversationInfo *info = [[JConcreteConversationInfo alloc] init];
    JConversation *c = [[JConversation alloc] init];
    c.conversationType = [rs intForColumn:jConversationType];
    c.conversationId = [rs stringForColumn:jConversationId];
    info.conversation = c;
    info.draft = [rs stringForColumn:jDraft];
    info.sortTime = [rs longLongIntForColumn:jConversationTimestamp];
    info.lastReadMessageIndex = [rs longLongIntForColumn:jLastReadMessageIndex];
    info.lastMessageIndex = [rs longLongIntForColumn:jLastMessageIndex];
    info.isTop = [rs boolForColumn:jIsTop];
    info.topTime = [rs longLongIntForColumn:jTopTime];
    info.mute = [rs boolForColumn:jMute];
    info.mentionInfo = [JConversationMentionInfo decodeFromJson:[rs stringForColumn:jConversationMentionInfo]];
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
    lastMessage.clientMsgNo = [rs intForColumn:jLastMessageClientMsgNo];
    NSString *content = [rs stringForColumn:jLastMessageContent];
    NSData *data = [content dataUsingEncoding:NSUTF8StringEncoding];
    lastMessage.content = [[JContentTypeCenter shared] contentWithData:data
                                                           contentType:lastMessage.contentType];
    
    NSString * lastMsgMentionInfo = [rs stringForColumn:jLastMessageMentionInfo];
    if(lastMsgMentionInfo != nil){
        lastMessage.mentionInfo = [JMessageMentionInfo decodeFromJson:[rs stringForColumn:jLastMessageMentionInfo]];
    }
    lastMessage.seqNo = [rs longLongIntForColumn:jLastMessageSeqNo];
    lastMessage.msgIndex = [rs longLongIntForColumn:jLastMessageIndex];
    info.lastMessage = lastMessage;
    info.hasUnread = [rs boolForColumn:jHasUnread];
    return info;
}
@end
