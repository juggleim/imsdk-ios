//
//  JConversationDB.m
// JuggleIM
//
//  Created by Nathan on 2023/12/8.
//

#import "JConversationDB.h"
#import "JContentTypeCenter.h"
#import "JIM.h"

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
                                        "unread_tag BOOLEAN,"
                                        "subchannel VARCHAR (64) DEFAULT ''"
                                        ")";
NSString *const jCreateConversationTagTable = @"CREATE TABLE IF NOT EXISTS conversation_tag ("
                                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                        "tag_id VARCHAR (64),"
                                        "conversation_type SMALLINT,"
                                        "conversation_id VARCHAR (64),"
                                        "subchannel VARCHAR (64) DEFAULT ''"
                                        ")";
NSString *const jCreateConversationTagIndex = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_conversation_tag ON conversation_tag(tag_id, conversation_type, conversation_id)";
NSString *const jCreateConversationTagIndex2 = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_conversation_tag2 ON conversation_tag(tag_id, conversation_type, conversation_id, subchannel)";
NSString *const jCreateConversationIndex = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_conversation ON conversation_info(conversation_type, conversation_id)";
NSString *const jCreateConversationIndex2 = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_conversation2 ON conversation_info(conversation_type, conversation_id, subchannel)";
NSString *const kInsertConversation = @"INSERT OR REPLACE INTO conversation_info"
                                       "(conversation_type, conversation_id, subchannel, timestamp, last_message_id,"
                                       "last_read_message_index, last_message_index, is_top, top_time, mute, mention_info,"
                                       "last_message_type, last_message_client_uid, last_message_client_msg_no, last_message_direction, last_message_state,"
                                       "last_message_has_read, last_message_timestamp, last_message_sender, last_message_content, last_message_mention_info,"
                                       "last_message_seq_no, unread_tag)"
                                       "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
NSString *const jUpdateConversation = @"UPDATE conversation_info SET timestamp=?, last_message_id=?, last_read_message_index=?, "
                                       "last_message_index=?, is_top=?, top_time=?, mute=?, mention_info=?, last_message_type=?,  "
                                       "last_message_client_uid=?, last_message_client_msg_no=?, last_message_direction=?, last_message_state=?, "
                                       "last_message_has_read=?, last_message_timestamp=?, last_message_sender=?, "
                                       "last_message_content=?, last_message_mention_info=?, last_message_seq_no=?, unread_tag =?  WHERE conversation_type = ? "
                                       "AND conversation_id = ? AND subchannel = ?";
NSString *const kGetConversation = @"SELECT * FROM conversation_info WHERE conversation_type = ? AND conversation_id = ? AND subchannel = ?";
NSString *const jGetConversations = @"SELECT * FROM conversation_info";
NSString *const jGetConversationsBy = @"SELECT * FROM conversation_info WHERE";
NSString *const jIsTopEqualsTrue = @" is_top = 1";
NSString *const jTimestampGreaterThan = @" timestamp > ?";
NSString *const jTimestampLessThan = @" timestamp < ?";
NSString *const jConversationAnd = @" AND ";
NSString *const jConversationTypeIn = @" conversation_type in ";
NSString *const jConversationInfoTypeIn = @" conversation_info.conversation_type in ";
NSString *const jConversationOrderByTopTopTimeTimestamp = @" ORDER BY is_top DESC, top_time DESC, timestamp DESC";
NSString *const jConversationOrderByTopTimestamp = @" ORDER BY is_top DESC, timestamp DESC";
NSString *const jConversationOrderByTopTime = @" ORDER BY top_time DESC";
NSString *const jConversationOrderByTimestamp = @" ORDER BY timestamp DESC";
NSString *const jConversationLimit = @" LIMIT ?";
NSString *const jDeleteConversation = @"DELETE FROM conversation_info WHERE conversation_type = ? AND conversation_id = ? AND subchannel = ?";
NSString *const jSetDraft = @"UPDATE conversation_info SET draft = ? WHERE conversation_type = ? AND conversation_id = ? AND subchannel = ?";
NSString *const jClearUnreadCount = @"UPDATE conversation_info SET last_read_message_index = ? WHERE conversation_type = ? AND conversation_id = ? AND subchannel = ?";
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
NSString *const jTimestampEqualsQuestion = @", timestamp=?";
NSString *const jLastMessageIndexEqualsQuestion = @", last_message_index=?";
NSString *const jSetMute = @"UPDATE conversation_info SET mute = ? WHERE conversation_type = ? AND conversation_id = ? AND subchannel = ?";
NSString *const jSetTop = @"UPDATE conversation_info SET is_top = ?, top_time = ?";
NSString *const jSetUnread = @"UPDATE conversation_info SET unread_tag = ?";
NSString *const jGetTotalUnreadCount = @"SELECT SUM(CASE WHEN last_message_index = last_read_message_index AND unread_tag = 1 THEN 1 WHEN last_message_index - last_read_message_index > 0 THEN last_message_index - last_read_message_index ELSE 0 END) AS total_count FROM conversation_info WHERE mute = 0";
NSString *const jGetUnreadCountWithTag = @"SELECT SUM(CASE WHEN last_message_index = last_read_message_index AND unread_tag = 1 THEN 1 WHEN last_message_index - last_read_message_index > 0 THEN last_message_index - last_read_message_index ELSE 0 END) AS total_count FROM conversation_info INNER JOIN conversation_tag ON conversation_info.conversation_type = conversation_tag.conversation_type AND conversation_info.conversation_id = conversation_tag.conversation_id AND conversation_info.subchannel = conversation_tag.subchannel WHERE tag_id = ?";
NSString *const jWhereConversationIs = @" WHERE conversation_type = ? AND conversation_id = ? AND subchannel = ?";
NSString *const jClearTotalUnreadCount = @"UPDATE conversation_info SET last_read_message_index = last_message_index";
NSString *const jUpdateConversationTime = @"UPDATE conversation_info SET timestamp = ?";
NSString *const jUpdateConversationMentionInfo = @"UPDATE conversation_info SET mention_info = ? WHERE conversation_type = ? AND conversation_id = ? AND subchannel = ?";
NSString *const jClearConversationMentionInfo = @"UPDATE conversation_info SET mention_info = NULL";
NSString *const jUpdateConversationLastMessageHasRead = @"UPDATE conversation_info SET last_message_has_read = 1 WHERE conversation_type = ? AND conversation_id = ? AND subchannel = ?";
NSString *const jUpdateConversationLastMessageState = @"UPDATE conversation_info SET last_message_state = ? WHERE conversation_type = ? AND conversation_id = ? AND subchannel = ? AND last_message_client_msg_no = ?";
NSString *const jClearTagByConversation = @"DELETE FROM conversation_tag WHERE conversation_type = ? AND conversation_id = ? AND subchannel = ?";
NSString *const jInsertConversationTag = @"INSERT INTO conversation_tag (tag_id, conversation_type, conversation_id, subchannel) VALUES ";
NSString *const jRemoveConversationFromTag = @"DELETE FROM conversation_tag WHERE conversation_type = ? AND conversation_id = ? AND subchannel = ? AND tag_id = ?";
NSString *const jGetConversationsByTag = @"SELECT * FROM conversation_info INNER JOIN conversation_tag"
                                            " ON conversation_info.conversation_type = conversation_tag.conversation_type"
                                            " AND conversation_info.conversation_id = conversation_tag.conversation_id"
                                            " AND conversation_info.subchannel = conversation_tag.subchannel"
                                            " WHERE tag_id = ? ";

NSString *const jConversationType = @"conversation_type";
NSString *const jConversationId = @"conversation_id";
NSString *const jSubChannel = @"subchannel";
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
NSString *const jAlterConversationInfoAddSubChannel = @"ALTER TABLE conversation_info ADD COLUMN subchannel VARCHAR (64) DEFAULT ''";
NSString *const jAlterConversationTagAddSubChannel = @"ALTER TABLE conversation_tag ADD COLUMN subchannel VARCHAR (64) DEFAULT ''";
NSString *const jDropConversationIndex1 = @"DROP INDEX IF EXISTS idx_conversation";
NSString *const jDropConversationTagIndex1 = @"DROP INDEX IF EXISTS idx_conversation_tag";

@interface JConversationDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JConversationDB

- (void)createTables {
    [self.dbHelper executeUpdate:kCreateConversationTable withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:jCreateConversationIndex2 withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:jCreateConversationTagTable withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:jCreateConversationTagIndex2 withArgumentsInArray:nil];
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
            JFMResultSet *resultSet = [db executeQuery:kGetConversation, @(obj.conversation.conversationType), obj.conversation.conversationId, obj.conversation.subChannel];
            if ([resultSet next]) {
                info = [self conversationInfoWith:resultSet];
            }
            [resultSet close];
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
                [db executeUpdate:jUpdateConversation, @(obj.sortTime), lastMessage.messageId, @(obj.lastReadMessageIndex), @(obj.lastMessageIndex), @(obj.isTop), @(obj.topTime), @(obj.mute), mentionInfo, lastMessage.contentType, lastMessage.clientUid, @(lastMessage.clientMsgNo),@(lastMessage.direction), @(lastMessage.messageState), @(lastMessage.hasRead), @(lastMessage.timestamp), lastMessage.senderUserId, content, lastMsgMentionInfo, @(lastMessage.seqNo), @(obj.hasUnread), @(obj.conversation.conversationType), obj.conversation.conversationId, obj.conversation.subChannel];
            } else {
                [insertConversations addObject:obj];
                [db executeUpdate:kInsertConversation, @(obj.conversation.conversationType), obj.conversation.conversationId, obj.conversation.subChannel, @(obj.sortTime), lastMessage.messageId, @(obj.lastReadMessageIndex), @(obj.lastMessageIndex), @(obj.isTop), @(obj.topTime), @(obj.mute), mentionInfo, lastMessage.contentType, lastMessage.clientUid, @(lastMessage.clientMsgNo),@(lastMessage.direction), @(lastMessage.messageState), @(lastMessage.hasRead), @(lastMessage.timestamp), lastMessage.senderUserId, content, lastMsgMentionInfo, @(lastMessage.seqNo), @(obj.hasUnread)];
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
           withArgumentsInArray:@[@(conversation.conversationType), conversation.conversationId, conversation.subChannel]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            info = [self conversationInfoWith:resultSet];
        }
    }];
    info = [self checkLastMessage:info];
    return info;
}

- (void)deleteConversationInfoBy:(JConversation *)conversation {
    if (conversation.conversationId.length == 0) {
        return;
    }
    [self.dbHelper executeUpdate:jDeleteConversation
            withArgumentsInArray:@[@(conversation.conversationType), conversation.conversationId, conversation.subChannel]];
}

- (NSArray<JConcreteConversationInfo *> *)getConversationInfoList {
    NSMutableArray<JConcreteConversationInfo *> *array = [[NSMutableArray alloc] init];
    NSString *sql = jGetConversations;
    sql = [self appendOrderSql:sql];
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:nil
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        while ([resultSet next]) {
            JConcreteConversationInfo *info = [self conversationInfoWith:resultSet];
            [array addObject:info];
        }
    }];
    [array enumerateObjectsUsingBlock:^(JConcreteConversationInfo * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        obj = [self checkLastMessage:obj];
    }];
    return array;
}

- (NSArray<JConversationInfo *> *)getConversationInfoListWith:(JGetConversationOptions *)options {
    NSString *sql;
    NSMutableArray *args = [[NSMutableArray alloc] init];
    if (options.tagId.length > 0) {
        sql = jGetConversationsByTag;
        sql = [sql stringByAppendingString:jConversationAnd];
        [args addObject:options.tagId];
    } else {
        sql = jGetConversationsBy;
    }
    
    long long ts = options.timestamp;
    if (ts == 0) {
        ts = INT64_MAX;
    }
    if (options.direction == JPullDirectionOlder) {
        sql = [sql stringByAppendingString:jTimestampLessThan];
    } else {
        sql = [sql stringByAppendingString:jTimestampGreaterThan];
    }
    [args addObject:@(ts)];
    if (options.conversationTypes.count > 0) {
        sql = [sql stringByAppendingString:jConversationAnd];
        sql = [sql stringByAppendingString:jConversationInfoTypeIn];
        sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:options.conversationTypes.count]];
        [args addObjectsFromArray:options.conversationTypes];
    }
    sql = [self appendOrderSql:sql];
    sql = [sql stringByAppendingString:jConversationLimit];
    [args addObject:@(options.count)];
    
    NSMutableArray<JConcreteConversationInfo *> *array = [[NSMutableArray alloc] init];
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:args
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        while ([resultSet next]) {
            JConcreteConversationInfo *info = [self conversationInfoWith:resultSet];
            [array addObject:info];
        }
    }];
    [array enumerateObjectsUsingBlock:^(JConcreteConversationInfo * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        obj = [self checkLastMessage:obj];
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
    
    if (self.topConversationsOrderType == JTopConversationsOrderByTopTime) {
        sql = [sql stringByAppendingString:jConversationOrderByTopTime];
    } else {
        sql = [sql stringByAppendingString:jConversationOrderByTimestamp];
    }
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
    [array enumerateObjectsUsingBlock:^(JConcreteConversationInfo * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        obj = [self checkLastMessage:obj];
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
    [self.dbHelper executeUpdate:jSetDraft withArgumentsInArray:@[draft, @(conversation.conversationType), conversation.conversationId, conversation.subChannel]];
}

- (void)clearDraftInConversation:(JConversation *)conversation {
    [self setDraft:@"" inConversation:conversation];
}

- (void)clearUnreadCountBy:(JConversation *)conversation
                  msgIndex:(long long)msgIndex {
    if (conversation.conversationId.length == 0) {
        return;
    }
    [self.dbHelper executeUpdate:jClearUnreadCount withArgumentsInArray:@[@(msgIndex), @(conversation.conversationType), conversation.conversationId, conversation.subChannel]];
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
        sql = [sql stringByAppendingString:jTimestampEqualsQuestion];
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
    [args addObject:message.conversation.subChannel];
    
    [self.dbHelper executeUpdate:sql withArgumentsInArray:args];
} 

- (void)setMute:(BOOL)isMute conversation:(JConversation *)conversation {
    if (conversation.conversationId.length == 0) {
        return;
    }
    [self.dbHelper executeUpdate:jSetMute withArgumentsInArray:@[@(isMute), @(conversation.conversationType), conversation.conversationId, conversation.subChannel]];
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
    [self.dbHelper executeUpdate:sql withArgumentsInArray:@[@(isTop), @(time), @(conversation.conversationType), conversation.conversationId, conversation.subChannel]];
}

- (void)setUnread:(BOOL)isUnread
     conversation:(JConversation *)conversation {
    if (conversation.conversationId.length == 0) {
        return;
    }
    NSString *sql;
    sql = [jSetUnread stringByAppendingString:jWhereConversationIs];
    [self.dbHelper executeUpdate:sql withArgumentsInArray:@[@(isUnread), @(conversation.conversationType), conversation.conversationId, conversation.subChannel]];
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

- (int)getUnreadCountWithTypes:(NSArray<NSNumber *> *)conversationTypes {
    NSString *sql = jGetTotalUnreadCount;
    NSMutableArray *args = [[NSMutableArray alloc] init];
    sql = [sql stringByAppendingString:jConversationAnd];
    sql = [sql stringByAppendingString:jConversationTypeIn];
    sql = [sql stringByAppendingString:[self.dbHelper getQuestionMarkPlaceholder:conversationTypes.count]];
    [args addObjectsFromArray:conversationTypes];
    __block int count = 0;
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:args
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            count = [resultSet intForColumn:jTotalCount];
        }
    }];
    return count;
}

- (int)getUnreadCountWithTag:(NSString *)tagId {
    NSString *sql = jGetUnreadCountWithTag;
    __block int count = 0;
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:@[tagId]
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
    [self.dbHelper executeUpdate:sql withArgumentsInArray:@[@(time), @(conversation.conversationType), conversation.conversationId, conversation.subChannel]];
}


- (void)setMentionInfo:(JConversation *)conversation
      mentionInfoJson:(NSString *)mentionInfoJson{
    if (conversation.conversationId.length == 0) {
        return;
    }
    [self.dbHelper executeUpdate:jUpdateConversationMentionInfo withArgumentsInArray:@[mentionInfoJson, @(conversation.conversationType), conversation.conversationId, conversation.subChannel]];
}

- (void)clearMentionInfo{
    [self.dbHelper executeUpdate:jClearConversationMentionInfo withArgumentsInArray:nil];
}

- (void)clearLastMessage:(JConversation *)conversation{
    if (conversation.conversationId.length == 0) {
        return;
    }
    NSString *sql = jClearLastMessage;
    sql = [sql stringByAppendingString:jWhereConversationIs];
    [self.dbHelper executeUpdate:sql withArgumentsInArray:@[@(conversation.conversationType), conversation.conversationId, conversation.subChannel]];
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
                                                                   message.conversation.conversationId?:@"",
                                                                   message.conversation.subChannel]];
    [self.dbHelper executeUpdate:sql withArgumentsInArray:args];
}
- (void)setLastMessageHasRead:(JConversation *)conversation{
    [self.dbHelper executeUpdate:jUpdateConversationLastMessageHasRead withArgumentsInArray:@[@(conversation.conversationType), conversation.conversationId, conversation.subChannel]];
}
- (void)updateLastMessageState:(JConversation *)conversation
                         state:(JMessageState)state
               withClientMsgNo:(long long)clientMsgNo{
    NSString *sql = jUpdateConversationLastMessageState;
    NSMutableArray *args = [[NSMutableArray alloc] initWithArray:@[@(state),
                                                                   @(conversation.conversationType),
                                                                   conversation.conversationId,
                                                                   conversation.subChannel,
                                                                   @(clientMsgNo)]];
    [self.dbHelper executeUpdate:sql withArgumentsInArray:args];

}

- (void)updateConversationTag:(NSArray<JConcreteConversationInfo *> *)conversations {
    __block NSString *sql = jInsertConversationTag;
    NSMutableArray *arguments = [NSMutableArray array];
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        for (JConcreteConversationInfo *info in conversations) {
            if (info.tagIdList.count > 0) {
                [db executeUpdate:jClearTagByConversation, @(info.conversation.conversationType), info.conversation.conversationId, info.conversation.subChannel];
                for (NSString *tagId in info.tagIdList) {
                    sql = [sql stringByAppendingFormat:@"%@", [self.dbHelper getQuestionMarkPlaceholder:4]];
                    sql = [sql stringByAppendingFormat:@", "];
                    [arguments addObject:tagId];
                    [arguments addObject:@(info.conversation.conversationType)];
                    [arguments addObject:info.conversation.conversationId];
                    [arguments addObject:info.conversation.subChannel];
                }
            }
        }
        if ([sql hasSuffix:@", "]) {
            sql = [sql substringToIndex:sql.length-2];
            [db executeUpdate:sql withArgumentsInArray:arguments];
        }
    }];
}

- (void)addConversations:(NSArray <JConversation *> *)conversations
                   toTag:(NSString *)tagId {
    if (conversations.count == 0 || tagId.length == 0) {
        return;
    }
    NSString *sql = jInsertConversationTag;
    NSMutableArray *arguments = [NSMutableArray array];
    for (int i = 0; i < conversations.count; i++) {
        JConversation *conversation = conversations[i];
        sql = [sql stringByAppendingFormat:@"%@", [self.dbHelper getQuestionMarkPlaceholder:4]];
        if (i != conversations.count - 1) {
            sql = [sql stringByAppendingFormat:@", "];
        }
        [arguments addObject:tagId];
        [arguments addObject:@(conversation.conversationType)];
        [arguments addObject:conversation.conversationId];
        [arguments addObject:conversation.subChannel];
    }
    [self.dbHelper executeUpdate:sql withArgumentsInArray:arguments];
}

- (void)removeConversations:(NSArray <JConversation *> *)conversations
                    fromTag:(NSString *)tagId {
    if (conversations.count == 0 || tagId.length == 0) {
        return;
    }
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        for (JConversation *conversation in conversations) {
            [db executeUpdate:jRemoveConversationFromTag withArgumentsInArray:@[@(conversation.conversationType), conversation.conversationId, conversation.subChannel, tagId]];
        }
    }];
}

+ (NSString *)createConversationTagTable {
    return jCreateConversationTagTable;
}

+ (nonnull NSString *)createConversationTagIndex {
    return jCreateConversationTagIndex;
}

+ (NSString *)alterConversationInfoAddSubChannel {
    return jAlterConversationInfoAddSubChannel;
}

+ (NSString *)alterConversationTagAddSubChannel {
    return jAlterConversationTagAddSubChannel;
}

+ (NSString *)dropConversationIndex1 {
    return jDropConversationIndex1;
}

+ (NSString *)dropConversationTagIndex1 {
    return jDropConversationTagIndex1;
}

+ (NSString *)addConversationIndex2 {
    return jCreateConversationIndex2;
}

+ (NSString *)addConversationTagIndex2 {
    return jCreateConversationTagIndex2;
}

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper {
    if (self = [super init]) {
        self.dbHelper = dbHelper;
        self.topConversationsOrderType = JTopConversationsOrderByTopTime;
    }
    return self;
}

#pragma mark - internal
- (JConcreteConversationInfo *)conversationInfoWith:(JFMResultSet *)rs {
    JConcreteConversationInfo *info = [[JConcreteConversationInfo alloc] init];
    JConversation *c = [[JConversation alloc] init];
    c.conversationType = [rs intForColumn:jConversationType];
    c.conversationId = [rs stringForColumn:jConversationId];
    c.subChannel = [rs stringForColumn:jSubChannel];
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

- (JConcreteConversationInfo *)checkLastMessage:(JConcreteConversationInfo *)info {
    BOOL needUpdate = NO;
    long long timeDifference = [JIM.shared getTimeDifference];
    long long now = [[NSDate date] timeIntervalSince1970] * 1000 + timeDifference;
    // 当 lastMessage 存在的时候，检查它是否被删除或者过期了。不存在的时候不做处理
    if ([info.lastMessage isKindOfClass:[JConcreteMessage class]]) {
        JConcreteMessage *conversationLastMessage = (JConcreteMessage *)info.lastMessage;
        JConcreteMessage *lastMessage = [self.messageDB getMessageWithClientUid:conversationLastMessage.clientUid];
        if (lastMessage.isDeleted || (lastMessage.destroyTime > 0 && lastMessage.destroyTime <= now)) {
            needUpdate = YES;
        } else {
            info.lastMessage.destroyTime = lastMessage.destroyTime;
            info.lastMessage.lifeTimeAfterRead = lastMessage.lifeTimeAfterRead;
        }
    }
    if (needUpdate) {
        JMessage *newLast = [self.messageDB getLastMessage:info.conversation currentTime:now];
        info.lastMessage = newLast;
    }
    
    return info;
}

- (NSString *)appendOrderSql:(NSString *)originSql {
    if (self.topConversationsOrderType == JTopConversationsOrderByTopTime) {
        originSql = [originSql stringByAppendingString:jConversationOrderByTopTopTimeTimestamp];
    } else {
        originSql = [originSql stringByAppendingString:jConversationOrderByTopTimestamp];
    }
    return originSql;
}

@end
