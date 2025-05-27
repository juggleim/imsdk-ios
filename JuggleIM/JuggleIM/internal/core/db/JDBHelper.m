//
//  JDBHelper.m
// JuggleIM
//
//  Created by Nathan on 2023/12/8.
//

#import "JDBHelper.h"
#import "JLogger.h"

#define jDbDuration 500

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
        long long start = [[NSDate date] timeIntervalSince1970] * 1000;
        result = [db executeUpdate:sql withArgumentsInArray:arguments];
        long long end = [[NSDate date] timeIntervalSince1970] * 1000;
        if (end - start > jDbDuration) {
            JLogW(@"DB-Duration", @"executeUpdate lasts for %lld ms, sql is %@", end-start, sql);
        }
    }];
    return result;
}

- (void)executeQuery:(NSString *)sql
withArgumentsInArray:(NSArray *)arguments
          syncResult:(void (^)(JFMResultSet *resultSet))syncResultBlock {
    [self.dbQueue inDatabase:^(JFMDatabase *db) {
        long long start = [[NSDate date] timeIntervalSince1970] * 1000;
        JFMResultSet *resultSet = [db executeQuery:sql withArgumentsInArray:arguments];
        long long end1 = [[NSDate date] timeIntervalSince1970] * 1000;
        syncResultBlock(resultSet);
        long long end2 = [[NSDate date] timeIntervalSince1970] * 1000;
        if (end1 - start > jDbDuration) {
            JLogW(@"DB-Duration", @"executeQuery lasts for %lld ms, sql is %@", end1-start, sql);
        }
        if (end2 - end1 > jDbDuration) {
            JLogW(@"DB-Duration", @"executeQuery result block lasts for %lld ms, sql is %@", end1-start, sql);
        }
        [resultSet close];
    }];
}

- (void)executeTransaction:(void (^)(JFMDatabase *db, BOOL *rollback))transactionBlock {
    [self.dbQueue inTransaction:^(JFMDatabase *db, BOOL *rollback) {
        if (transactionBlock) {
            long long start = [[NSDate date] timeIntervalSince1970] * 1000;
            transactionBlock(db, rollback);
            long long end = [[NSDate date] timeIntervalSince1970] * 1000;
            if (end - start > jDbDuration) {
                JLogW(@"DB-Duration", @"executeTransaction lasts for %lld ms", end-start);
            }
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
