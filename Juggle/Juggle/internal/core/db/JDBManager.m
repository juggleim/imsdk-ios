//
//  JDBManager.m
//  Juggle
//
//  Created by Nathan on 2023/12/8.
//

#import "JDBManager.h"
#import "JDBHelper.h"

#define kJuggle @"juggle"
#define kJuggleDBName @"juggledb"

NSString *const kCreateMessageTable = @"CREATE TABLE IF NOT EXISTS message ("
                                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                        "conversation_type SMALLINT,"
                                        "conversation_id VARCHAR (64),"
                                        "type VARCHAR (64),"
                                        "message_uid VARCHAR (64),"
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

NSString *const kCreateConversationTable = @"CREATE TABLE IF NOT EXISTS conversation_info ("
                                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                        "conversation_type SMALLINT,"
                                        "conversation_id VARCHAR (64),"
                                        "draft TEXT,"
                                        "timestamp INTEGER,"
                                        "last_message_id INTEGER,"
                                        "last_read_message_index INTEGER,"
                                        "is_top BOOLEAN,"
                                        "top_time INTEGER,"
                                        "mute BOOLEAN,"
                                        "last_mention_message_id INTEGER"
                                        ")";

NSString *const kCreateSyncTable = @"CREATE TABLE IF NOT EXISTS sync ("
                                        "user_id VARCHAR(64) PRIMARY KEY,"
                                        "conversation_time INTEGER,"
                                        "send_time INTEGER,"
                                        "receive_time INTEGER"
                                        ")";

@implementation JDBManager
- (void)openIMDB:(NSString *)appKey
          userId:(NSString *)userId {
    NSString *path = [self dbPathWith:appKey userId:userId notExistsReturnEmpty:YES];
    if (path.length > 0) {
        [[JDBHelper sharedInstance] openDB:path];
    } else {
        [self buildDB:appKey
               userId:(NSString *)userId];
    }
}

#pragma mark - internal
- (void)buildDB:(NSString *)appKey
         userId:(NSString *)userId {
    NSString *path = [self dbDirectoryWith:appKey userId:userId];
    if (![[NSFileManager defaultManager] fileExistsAtPath:path]) {
        [[NSFileManager defaultManager] createDirectoryAtPath:path
                                  withIntermediateDirectories:YES
                                                   attributes:nil
                                                        error:nil];
    }
    path = [self dbPathWith:appKey userId:userId notExistsReturnEmpty:NO];
    [[JDBHelper sharedInstance] openDB:path];
    [self createTables];
}

- (void)createTables {
    [[JDBHelper sharedInstance] executeUpdate:kCreateMessageTable withArgumentsInArray:nil];
    [[JDBHelper sharedInstance] executeUpdate:kCreateConversationTable withArgumentsInArray:nil];
    [[JDBHelper sharedInstance] executeUpdate:kCreateSyncTable withArgumentsInArray:nil];
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

@end
