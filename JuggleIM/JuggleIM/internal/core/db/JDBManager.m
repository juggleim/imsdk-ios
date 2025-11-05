//
//  JDBManager.m
// JuggleIM
//
//  Created by Nathan on 2023/12/8.
//

#import "JDBManager.h"
#import "JDBHelper.h"
#import "JMessageDB.h"
#import "JConversationDB.h"
#import "JProfileDB.h"
#import "JUserInfoDB.h"
#import "JReactionDB.h"
#import "JUtility.h"
#import "JVersionDB.h"

#define kJetIMDBName @"jetimdb"

@interface JDBManager ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@property (nonatomic, strong) JMessageDB *messageDb;
@property (nonatomic, strong) JConversationDB *conversationDb;
@property (nonatomic, strong) JProfileDB *profileDb;
@property (nonatomic, strong) JUserInfoDB *userInfoDB;
@property (nonatomic, strong) JReactionDB *reactionDB;
@property (nonatomic, strong) JVersionDB *versionDB;
@end

@implementation JDBManager
- (BOOL)openIMDB:(NSString *)appKey
          userId:(NSString *)userId {
    NSString *path = [self dbPathWith:appKey userId:userId notExistsReturnEmpty:YES];
    if (path.length > 0) {
        return [self openIMDB:path];
    } else {
        return [self buildDB:appKey
                      userId:(NSString *)userId];
    }
}

- (void)closeIMDB {
    [self.dbHelper closeDB];
}

- (BOOL)isOpen {
    return [self.dbHelper isDBOpened];
}

#pragma mark - sync table
- (long long)getConversationSyncTime {
    return [self.profileDb getConversationSyncTime];
}

- (long long)getMessageSendSyncTime {
    return [self.profileDb getMessageSendSyncTime];
}

- (long long)getMessageReceiveSyncTime {
    return [self.profileDb getMessageReceiveSyncTime];
}

- (void)setConversationSyncTime:(long long)time {
    [self.profileDb setConversationSyncTime:time];
}

- (void)setMessageSendSyncTime:(long long)time {
    [self.profileDb setMessageSendSyncTime:time];
}

- (void)setMessageReceiveSyncTime:(long long)time {
    [self.profileDb setMessageReceiveSyncTime:time];
}

#pragma mark - conversation table
- (void)insertConversations:(NSArray *)conversations
                 completion:(nullable void (^)(NSArray<JConcreteConversationInfo *> * _Nonnull, NSArray<JConcreteConversationInfo *> * _Nonnull))completeBlock {
    [self.conversationDb insertConversations:conversations
                                  completion:completeBlock];
}

- (JConcreteConversationInfo *)getConversationInfo:(JConversation *)conversation {
    return [self.conversationDb getConversationInfo:conversation];
}

- (void)clearUnreadCountBy:(JConversation *)conversation
                  msgIndex:(long long)msgIndex {
    [self.conversationDb clearUnreadCountBy:conversation msgIndex:msgIndex];
}

- (void)deleteConversationInfoBy:(JConversation *)conversation {
    [self.conversationDb deleteConversationInfoBy:conversation];
}

- (NSArray<JConcreteConversationInfo *> *)getConversationInfoList {
    return [self.conversationDb getConversationInfoList];
}

- (NSArray<JConversationInfo *> *)getTopConversationInfoListWithTypes:(NSArray<NSNumber *> *)conversationTypes
                                                                count:(int)count
                                                            timestamp:(long long)ts
                                                            direction:(JPullDirection)direction {
    return [self.conversationDb getTopConversationInfoListWithTypes:conversationTypes
                                                              count:count
                                                          timestamp:ts
                                                          direction:direction];
}

- (NSArray<JConversationInfo *> *)getConversationInfoListWith:(JGetConversationOptions *)options {
    return [self.conversationDb getConversationInfoListWith:options];
}

- (void)setDraft:(NSString *)draft inConversation:(JConversation *)conversation {
    [self.conversationDb setDraft:draft inConversation:conversation];
}

- (void)clearDraftInConversation:(JConversation *)conversation {
    [self.conversationDb clearDraftInConversation:conversation];
}

- (void)updateLastMessage:(JConcreteMessage *)message {
    [self.conversationDb updateLastMessage:message];
}

- (void)setMute:(BOOL)isMute conversation:(JConversation *)conversation {
    [self.conversationDb setMute:isMute conversation:conversation];
}

- (void)setTop:(BOOL)isTop
          time:(long long)time
  conversation:(JConversation *)conversation {
    [self.conversationDb setTop:isTop time:time conversation:conversation];
}

- (void)setUnread:(BOOL)isUnread conversation:(JConversation *)conversation {
    [self.conversationDb setUnread:isUnread conversation:conversation];
}

- (void)clearUnreadTag {
    [self.conversationDb clearUnreadTag];
}

- (void)setMentionInfo:(JConversation *)conversation
      mentionInfoJson:(NSString *)mentionInfoJson{
    [self.conversationDb setMentionInfo:conversation mentionInfoJson:mentionInfoJson];
}

- (void)clearMentionInfo{
    [self.conversationDb clearMentionInfo];
}

- (int)getTotalUnreadCount {
    return [self.conversationDb getTotalUnreadCount];
}

- (int)getUnreadCountWithTypes:(NSArray<NSNumber *> *)conversationTypes {
    return [self.conversationDb getUnreadCountWithTypes:conversationTypes];
}

- (int)getUnreadCountWithTag:(NSString *)tagId {
    return [self.conversationDb getUnreadCountWithTag:tagId];
}

- (void)clearTotalUnreadCount{
    [self.conversationDb clearTotalUnreadCount];
}

- (void)updateTime:(long long)time forConversation:(JConversation *)conversation {
    [self.conversationDb updateTime:time forConversation:conversation];
}

- (void)clearLastMessage:(JConversation *)conversation{
    [self.conversationDb clearLastMessage:conversation];
}
- (void)updateLastMessageWithoutIndex:(JConcreteMessage *)message{
    [self.conversationDb updateLastMessageWithoutIndex:message];
}

- (void)setLastMessageHasRead:(JConversation *)conversation{
    [self.conversationDb setLastMessageHasRead:conversation];
}

- (void)updateLastMessageState:(JConversation *)conversation
                         state:(JMessageState)state
               withClientMsgNo:(long long)clientMsgNo{
    [self.conversationDb updateLastMessageState:conversation state:state withClientMsgNo:clientMsgNo];
}

- (void)setTopConversationsOrderType:(JTopConversationsOrderType)type {
    self.conversationDb.topConversationsOrderType = type;
}

#pragma mark - conversation tag table
- (void)updateConversationTag:(NSArray<JConcreteConversationInfo *> *)conversations {
    [self.conversationDb updateConversationTag:conversations];
}

- (void)addConversations:(NSArray <JConversation *> *)conversations
                   toTag:(NSString *)tagId {
    [self.conversationDb addConversations:conversations toTag:tagId];
}

- (void)removeConversations:(NSArray <JConversation *> *)conversations
                    fromTag:(NSString *)tagId {
    [self.conversationDb removeConversations:conversations fromTag:tagId];
}

#pragma mark - message table
- (void)insertMessages:(NSArray<JConcreteMessage *> *)messages {
    [self.messageDb insertMessages:messages];
}
- (nullable JConcreteMessage *)getMessageWithMessageId:(NSString *)messageId
                                           currentTime:(long long)now {
    return [self.messageDb getMessageWithMessageId:messageId
                                       currentTime:now];
}

- (void)updateMessageAfterSend:(long long)clientMsgNo
                     messageId:(NSString *)messageId
                     timestamp:(long long)timestamp
                         seqNo:(long long)seqNo
              groupMemberCount:(int)count {
    [self.messageDb updateMessageAfterSend:clientMsgNo
                                 messageId:messageId
                                 timestamp:timestamp
                                     seqNo:seqNo
                          groupMemberCount:count];
}

- (void)updateMessageAfterSendWithClientUid:(NSString *)clientUid
                                  messageId:(NSString *)messageId
                                  timestamp:(long long)timestamp
                                      seqNo:(long long)seqNo
                           groupMemberCount:(int)count {
    [self.messageDb updateMessageAfterSendWithClientUid:clientUid
                                              messageId:messageId
                                              timestamp:timestamp
                                                  seqNo:seqNo
                                       groupMemberCount:count];
}

- (void)updateMessageContent:(JMessageContent *)content
                 contentType:(nonnull NSString *)type
               withMessageId:(NSString *)messageId {
    [self.messageDb updateMessageContent:content
                             contentType:type
                           withMessageId:messageId];
}

- (void)setMessageFlags:(int)flags withMessageId:(NSString *)messageId {
    [self.messageDb setMessageFlags:flags withMessageId:messageId];
}

- (void)updateDestroyTime:(long long)destroyTime withMessageId:(NSString *)messageId {
    [self.messageDb updateDestroyTime:destroyTime withMessageId:messageId];
}

- (void)updateMessageContent:(JMessageContent *)content
                 contentType:(NSString *)type
             withClientMsgNo:(long long)clientMsgNo {
    [self.messageDb updateMessageContent:content
                             contentType:type
                         withClientMsgNo:clientMsgNo];
}

-(void)updateMessage:(JConcreteMessage *)message{
    [self.messageDb updateMessage:message];
}

- (void)setMessagesRead:(NSArray<NSString *> *)messageIds
               readTime:(long long)readTime {
    [self.messageDb setMessagesRead:messageIds
                           readTime:readTime];
}

- (void)setGroupMessageReadInfo:(NSDictionary<NSString *,JGroupMessageReadInfo *> *)msgs {
    [self.messageDb setGroupMessageReadInfo:msgs];
}

//- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
//                                   count:(int)count
//                                    time:(long long)time
//                               direction:(JPullDirection)direction
//                            contentTypes:(NSArray<NSString *> *)contentTypes
//                             currentTime:(long long)now {
//    return [self.messageDb getMessagesFrom:conversation
//                                     count:count
//                                      time:time
//                                 direction:direction
//                              contentTypes:(NSArray<NSString *> *)contentTypes
//                               currentTime:now];
//}

- (void)deleteMessageByClientIds:(NSArray <NSNumber *> *)clientMsgNos{
    [self.messageDb deleteMessageByClientIds:clientMsgNos];
}

- (void)deleteMessageByMessageIds:(NSArray <NSString *> *)messageIds{
    [self.messageDb deleteMessageByMessageIds:messageIds];
}

- (void)clearMessagesIn:(JConversation *)conversation startTime:(long long)startTime senderId:(NSString *)senderId{
    [self.messageDb clearMessagesIn:conversation startTime:startTime senderId:senderId];
}

- (void)clearChatroomMessage:(NSString *)chatroomId {
    [self.messageDb clearChatroomMessage:chatroomId];
}

- (NSArray<JMessage *> *)getMessagesByMessageIds:(NSArray<NSString *> *)messageIds {
    return [self.messageDb getMessagesByMessageIds:messageIds];
}

- (NSArray<JMessage *> *)getMessagesByClientMsgNos:(NSArray<NSNumber *> *)clientMsgNos {
    return [self.messageDb getMessagesByClientMsgNos:clientMsgNos];
}

- (void)setMessageState:(JMessageState)state
        withClientMsgNo:(long long)clientMsgNo {
    [self.messageDb setMessageState:state
                    withClientMsgNo:clientMsgNo];
}

- (NSArray<JMessage *> *)searchMessagesWithContent:(NSString *)searchContent
                                             count:(int)count
                                              time:(long long)time
                                     pullDirection:(JPullDirection)pullDirection
                                      contentTypes:(NSArray<NSString *> *)contentTypes
                                           senders:(NSArray<NSString *> *)senderUserIds
                                            states:(NSArray<NSNumber *> *)messageStates
                                     conversations:(NSArray<JConversation *> *)conversations
                                 conversationTypes:(NSArray<NSNumber *> *)conversationtypes
                                       currentTime:(long long)now {
    return [self.messageDb searchMessagesWithContent:searchContent
                                               count:count
                                                time:time
                                       pullDirection:pullDirection
                                        contentTypes:contentTypes
                                             senders:senderUserIds
                                              states:messageStates
                                       conversations:conversations
                                   conversationTypes:conversationtypes
                                         currentTime:now];
}

- (NSArray<JSearchConversationsResult *> *)searchMessageInConversations:(JQueryMessageOptions *)option
                                                            currentTime:(long long)now {
    NSArray <JSearchConversationsResult *> *resultList = [self.messageDb searchMessageInConversations:option currentTime:now];
    [resultList enumerateObjectsUsingBlock:^(JSearchConversationsResult * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        JConversationInfo *info = [self.conversationDb getConversationInfo:obj.conversationInfo.conversation];
        obj.conversationInfo = info;
    }];
    return resultList;
}

- (NSString *)getLocalAttributeByMessageId:(NSString *)messageId{
    return [self.messageDb getLocalAttributeByMessageId:messageId];
}
- (void)setLocalAttribute:(NSString *)attribute forMessage:(NSString *)messageId{
    [self.messageDb setLocalAttribute:attribute forMessage:messageId];
}
- (NSString *)getLocalAttributeByClientMsgNo:(long long)clientMsgNo{
    return [self.messageDb getLocalAttributeByClientMsgNo:clientMsgNo];
}
- (void)setLocalAttribute:(NSString *)attribute forClientMsgNo:(long long)clientMsgNo{
    [self.messageDb setLocalAttribute:attribute forClientMsgNo:clientMsgNo];
}
- (JConcreteMessage *)getLastMessage:(JConversation *)conversation
                         currentTime:(long long)now {
    return [self.messageDb getLastMessage:conversation currentTime:now];
}
- (void)clearChatroomMessageExclude:(NSArray<NSString *> *)chatroomIds {
    return [self.messageDb clearChatroomMessageExclude:chatroomIds];
}


#pragma mark - user table
- (JUserInfo *)getUserInfo:(NSString *)userId {
    return [self.userInfoDB getUserInfo:userId];
}

- (JGroupInfo *)getGroupInfo:(NSString *)groupId {
    return [self.userInfoDB getGroupInfo:groupId];
}

- (JGroupMember *)getGroupMember:(NSString *)groupId userId:(NSString *)userId {
    return [self.userInfoDB getGroupMemberIn:groupId userId:userId];
}

- (void)insertUserInfos:(NSArray<JUserInfo *> *)userInfos {
    [self.userInfoDB insertUserInfos:userInfos];
}

- (void)insertGroupInfos:(NSArray<JGroupInfo *> *)groupInfos {
    [self.userInfoDB insertGroupInfos:groupInfos];
}

- (void)insertGroupMembers:(NSArray<JGroupMember *> *)members {
    [self.userInfoDB insertGroupMembers:members];
}

#pragma mark - reaction table
- (NSArray<JMessageReaction *> *)getMessageReactions:(NSArray<NSString *> *)messageIds {
    return [self.reactionDB getMessageReactions:messageIds];
}

- (void)setMessageReactions:(NSArray<JMessageReaction *> *)reactions {
    [self.reactionDB setMessageReactions:reactions];
}

#pragma mark - internal
- (BOOL)buildDB:(NSString *)appKey
         userId:(NSString *)userId {
    BOOL result = NO;
    NSString *path = [self dbDirectoryWith:appKey userId:userId];
    if (![[NSFileManager defaultManager] fileExistsAtPath:path]) {
        [[NSFileManager defaultManager] createDirectoryAtPath:path
                                  withIntermediateDirectories:YES
                                                   attributes:nil
                                                        error:nil];
    }
    path = [self dbPathWith:appKey userId:userId notExistsReturnEmpty:NO];
    result = [self.dbHelper openDB:path];
    [self createTables];
    return result;
}

- (BOOL)openIMDB:(NSString *)path {
    BOOL result = [self.dbHelper openDB:path];
    [self updateTables];
    return result;
}

- (void)createTables {
    [self.messageDb createTables];
    [self.conversationDb createTables];
    [self.profileDb createTables];
    [self.userInfoDB createTables];
    [self.reactionDB createTables];
    [self.versionDB createTables];
}

- (void)updateTables {
    [self.messageDb updateTables];
    [self.conversationDb updateTables];
    [self.profileDb updateTables];
    [self.userInfoDB updateTables];
    [self.reactionDB updateTables];
    [self.versionDB updateTables];
}

//DB 目录
- (NSString *)dbDirectoryWith:(NSString *)appKey
                       userId:(NSString *)userId {
    NSString *path = [JUtility rootPath];
    path = [path stringByAppendingPathComponent:appKey];
    path = [path stringByAppendingPathComponent:userId];
    return path;
}

//当 DB 文件存在时返回路径，否则返回 @""
- (NSString *)dbPathWith:(NSString *)appKey
                  userId:(NSString *)userId
    notExistsReturnEmpty:(BOOL)returnEmpty {
    NSString *path = [self dbDirectoryWith:appKey userId:userId];
    path = [path stringByAppendingPathComponent:kJetIMDBName];
    if (returnEmpty && ![[NSFileManager defaultManager] fileExistsAtPath:path]) {
        return @"";
    }
    return path;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        self.dbHelper = [[JDBHelper alloc] init];
        self.messageDb = [[JMessageDB alloc] initWithDBHelper:self.dbHelper];
        self.profileDb = [[JProfileDB alloc] initWithDBHelper:self.dbHelper];
        self.conversationDb = [[JConversationDB alloc] initWithDBHelper:self.dbHelper];
        self.conversationDb.messageDB = self.messageDb;
        self.userInfoDB = [[JUserInfoDB alloc] initWithDBHelper:self.dbHelper];
        self.reactionDB = [[JReactionDB alloc] initWithDBHelper:self.dbHelper];
        self.versionDB = [[JVersionDB alloc] initWithDBHelper:self.dbHelper];
    }
    return self;
}

@end
