//
//  JDBHelper.m
//  Juggle
//
//  Created by Nathan on 2023/12/8.
//

#import "JDBHelper.h"

@interface JDBHelper ()
@property (nonatomic, strong) JFMDatabaseQueue *dbQueue;
@end

@implementation JDBHelper

+ (instancetype)sharedInstance {
    static id instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[[self class] alloc] init];
    });
    return instance;
}

- (BOOL)openDB:(NSString *)path {
    [self closeDB];
    if (path.length > 0) {
        self.dbQueue = [JFMDatabaseQueue databaseQueueWithPath:path];
    }
    return (self.dbQueue != nil);
}

- (void)closeDB {
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



@end