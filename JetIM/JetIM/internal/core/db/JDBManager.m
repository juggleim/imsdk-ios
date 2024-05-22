//
//  JDBManager.m
//  JetIM
//
//  Created by Nathan on 2023/12/8.
//

#import "JDBManager.h"
#import "JDBHelper.h"
#import "JMessageDB.h"
#import "JConversationDB.h"
#import "JProfileDB.h"
#import "JUserInfoDB.h"
#import "JUtility.h"

#define kJetIMDBName @"jetimdb"

@interface JDBManager ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@property (nonatomic, strong) JMessageDB *messageDb;
@property (nonatomic, strong) JConversationDB *conversationDb;
@property (nonatomic, strong) JProfileDB *profileDb;
@property (nonatomic, strong) JUserInfoDB *userInfoDB;
@end

@implementation JDBManager
- (BOOL)openIMDB:(NSString *)appKey
          userId:(NSString *)userId {
    NSString *path = [self dbPathWith:appKey userId:userId notExistsReturnEmpty:YES];
    if (path.length > 0) {
        return [self.dbHelper openDB:path];
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

- (NSArray<JConversationInfo *> *)getConversationInfoListWithTypes:(NSArray<NSNumber *> *)conversationTypes
                                                             count:(int)count
                                                         timestamp:(long long)ts
                                                         direction:(JPullDirection)direction {
    return [self.conversationDb getConversationInfoListWithTypes:conversationTypes
                                                           count:count
                                                       timestamp:ts
                                                       direction:direction];
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

- (void)setTop:(BOOL)isTop conversation:(JConversation *)conversation {
    [self.conversationDb setTop:isTop conversation:conversation];
}

- (void)setTopTime:(long long)time conversation:(JConversation *)conversation {
    [self.conversationDb setTopTime:time conversation:conversation];
}

- (void)setMention:(BOOL)isMention conversation:(JConversation *)conversation {
    [self.conversationDb setMention:isMention conversation:conversation];
}

- (void)clearMentionstatus {
    [self.conversationDb clearMentionstatus];
}

- (int)getTotalUnreadCount {
    return [self.conversationDb getTotalUnreadCount];
}

- (void)clearTotalUnreadCount{
    [self.conversationDb clearTotalUnreadCount];
}


#pragma mark - message table
- (void)insertMessages:(NSArray<JConcreteMessage *> *)messages {
    [self.messageDb insertMessages:messages];
}

- (void)updateMessageAfterSend:(long long)clientMsgNo
                     messageId:(NSString *)messageId
                     timestamp:(long long)timestamp
                         seqNo:(long long)seqNo {
    [self.messageDb updateMessageAfterSend:clientMsgNo
                                 messageId:messageId
                                 timestamp:timestamp
                                     seqNo:seqNo];
}

- (void)updateMessageContent:(JMessageContent *)content
                 contentType:(nonnull NSString *)type
               withMessageId:(NSString *)messageId {
    [self.messageDb updateMessageContent:content
                             contentType:type
                           withMessageId:messageId];
}

- (void)messageSendFail:(long long)clientMsgNo {
    [self.messageDb messageSendFail:clientMsgNo];
}

- (void)setMessagesRead:(NSArray<NSString *> *)messageIds {
    [self.messageDb setMessagesRead:messageIds];
}

- (void)setGroupMessageReadInfo:(NSDictionary<NSString *,JGroupMessageReadInfo *> *)msgs {
    [self.messageDb setGroupMessageReadInfo:msgs];
}

- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction
                            contentTypes:(NSArray<NSString *> *)contentTypes {
    return [self.messageDb getMessagesFrom:conversation
                                     count:count
                                      time:time
                                 direction:direction
                              contentTypes:(NSArray<NSString *> *)contentTypes];
}

- (void)deleteMessageByClientId:(long long)clientMsgNo {
    [self.messageDb deleteMessageByClientId:clientMsgNo];
}

- (void)deleteMessageByMessageId:(NSString *)messageId {
    [self.messageDb deleteMessageByMessageId:messageId];
}

- (void)clearMessagesIn:(JConversation *)conversation {
    [self.messageDb clearMessagesIn:conversation];
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
                                    inConversation:(JConversation *)conversation
                                             count:(int)count
                                              time:(long long)time
                                         direction:(JPullDirection)direction
                                      contentTypes:(NSArray<NSString *> *)contentTypes{
    return [self.messageDb searchMessagesWithContent:searchContent inConversation:conversation count:count time:time direction:direction contentTypes:contentTypes];
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


//- (NSArray <JMessage *> *)getMentionMessages:(JConversation *)conversation
//                                       count:(int)count
//                                        time:(long long)time
//                                   direction:(JPullDirection)direction {
//    return [self.messageDb getMentionMessages:conversation
//                                        count:count
//                                         time:time
//                                    direction:direction];
//}

#pragma mark - user table
- (JUserInfo *)getUserInfo:(NSString *)userId {
    return [self.userInfoDB getUserInfo:userId];
}

- (JGroupInfo *)getGroupInfo:(NSString *)groupId {
    return [self.userInfoDB getGroupInfo:groupId];
}

- (void)insertUserInfos:(NSArray<JUserInfo *> *)userInfos {
    [self.userInfoDB insertUserInfos:userInfos];
}

- (void)insertGroupInfos:(NSArray<JGroupInfo *> *)groupInfos {
    [self.userInfoDB insertGroupInfos:groupInfos];
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

- (void)createTables {
    [self.messageDb createTables];
    [self.conversationDb createTables];
    [self.profileDb createTables];
    [self.userInfoDB createTables];
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
    }
    return self;
}

@end
