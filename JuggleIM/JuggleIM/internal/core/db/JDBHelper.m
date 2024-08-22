//
//  JDBHelper.m
// JuggleIM
//
//  Created by Nathan on 2023/12/8.
//

#import "JDBHelper.h"
#import "JLogger.h"

@interface JDBHelper ()
@property (nonatomic, strong) JFMDatabaseQueue *dbQueue;
@end

@implementation JDBHelper

- (BOOL)openDB:(NSString *)path {
    [self closeDB];
    if (path.length > 0) {
        self.dbQueue = [JFMDatabaseQueue databaseQueueWithPath:path];
    }
    BOOL result = (self.dbQueue != nil);
    JLogD(@"DB-Open", @"path is %@, result is %d", path, result);
    return (self.dbQueue != nil);
}

- (void)closeDB {
    JLogD(@"DB-Close", @"");
    if (self.dbQueue) {
        [self.dbQueue close];
    }
    self.dbQueue = nil;
}

- (BOOL)isDBOpened {
    return self.dbQueue != nil;
}

- (BOOL)executeUpdate:(NSString *)sql withArgumentsInArray:(NSArray *)arguments {
    __block BOOL result = NO;
    [self.dbQueue inDatabase:^(JFMDatabase *db) {
        result = [db executeUpdate:sql withArgumentsInArray:arguments];
    }];
    return result;
}

- (void)executeQuery:(NSString *)sql
withArgumentsInArray:(NSArray *)arguments
          syncResult:(void (^)(JFMResultSet *resultSet))syncResultBlock {
    [self.dbQueue inDatabase:^(JFMDatabase *db) {
        JFMResultSet *resultSet = [db executeQuery:sql withArgumentsInArray:arguments];
        syncResultBlock(resultSet);
        [resultSet close];
    }];
}

- (void)executeTransaction:(void (^)(JFMDatabase *db, BOOL *rollback))transactionBlock {
    [self.dbQueue inTransaction:^(JFMDatabase *db, BOOL *rollback) {
        if (transactionBlock) {
            transactionBlock(db, rollback);
        }
    }];
}

- (NSString *)getQuestionMarkPlaceholder:(NSUInteger)count {
    NSString *sql = @"(";
    for (int i = 0; i < count; i++) {
        if (i > 0) {
            sql = [sql stringByAppendingString:@", "];
        }
        sql = [sql stringByAppendingString:@"?"];
    }
    sql = [sql stringByAppendingString:@")"];
    return sql;
}

@end
