//
//  JDBManager.m
//  Juggle
//
//  Created by Nathan on 2023/12/8.
//

#import "JDBManager.h"
#import "JDBHelper.h"
#import "JMessageDB.h"
#import "JConversationDB.h"
#import "JProfileDB.h"

#define kJuggle @"juggle"
#define kJuggleDBName @"juggledb"

@interface JDBManager ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@property (nonatomic, strong) JMessageDB *messageDb;
@property (nonatomic, strong) JConversationDB *conversationDb;
@property (nonatomic, strong) JProfileDB *profileDb;
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
- (void)insertConversations:(NSArray *)conversations {
    [self.conversationDb insertConversations:conversations];
}

- (JConcreteConversationInfo *)getConversationInfo:(JConversation *)conversation {
    return [self.conversationDb getConversationInfo:conversation];
}

- (NSArray<JConcreteConversationInfo *> *)getConversationInfoList {
    return [self.conversationDb getConversationInfoList];
}

#pragma mark - message table
- (void)insertMessages:(NSArray<JConcreteMessage *> *)messages {
    [self.messageDb insertMessages:messages];
}

- (void)updateMessageAfterSend:(long long)clientMsgNo
                     messageId:(NSString *)messageId
                     timestamp:(long long)timestamp
                  messageIndex:(long long)messageIndex {
    [self.messageDb updateMessageAfterSend:clientMsgNo
                                 messageId:messageId
                                 timestamp:timestamp
                              messageIndex:messageIndex];
}

- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction {
    return [self.messageDb getMessagesFrom:conversation
                                     count:count
                                      time:time
                                 direction:direction];
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
}

//DB 目录
- (NSString *)dbDirectoryWith:(NSString *)appKey
                       userId:(NSString *)userId {
    NSString *path =
            NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES)[0];
    path = [path stringByAppendingPathComponent:kJuggle];
    path = [path stringByAppendingPathComponent:appKey];
    path = [path stringByAppendingPathComponent:userId];
    return path;
}

//当 DB 文件存在时返回路径，否则返回 @""
- (NSString *)dbPathWith:(NSString *)appKey
                  userId:(NSString *)userId
    notExistsReturnEmpty:(BOOL)returnEmpty {
    NSString *path = [self dbDirectoryWith:appKey userId:userId];
    path = [path stringByAppendingPathComponent:kJuggleDBName];
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
    }
    return self;
}

@end
