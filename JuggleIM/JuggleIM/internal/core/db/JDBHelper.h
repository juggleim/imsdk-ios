//
//  JDBHelper.h
// JuggleIM
//
//  Created by Nathan on 2023/12/8.
//

#import <JuggleIM/JuggleIM.h>
#import "JFMDB.h"

NS_ASSUME_NONNULL_BEGIN

@interface JDBHelper : NSObject

@property(nonatomic, readonly, assign) BOOL isDBOpened;

- (BOOL)openDB:(NSString *)path;

- (void)closeDB;

- (BOOL)executeUpdate:(NSString *)sql withArgumentsInArray:(nullable NSArray *)arguments;

- (void)executeQuery:(NSString *)sql
withArgumentsInArray:(nullable NSArray *)arguments
          syncResult:(void (^)(JFMResultSet *resultSet))syncResultBlock;

- (void)executeTransaction:(void (^)(JFMDatabase *db, BOOL *rollback))transactionBlock;

- (NSString *)getQuestionMarkPlaceholder:(NSUInteger)count;
@end

NS_ASSUME_NONNULL_END
