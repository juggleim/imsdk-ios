//
//  JDBHelper.h
//  Juggle
//
//  Created by Nathan on 2023/12/8.
//

#import <Juggle/Juggle.h>
#import "JFMDB.h"

NS_ASSUME_NONNULL_BEGIN

@interface JDBHelper : NSObject

@property(nonatomic, readonly, assign) BOOL isDBOpened;

+ (instancetype)sharedInstance;

- (BOOL)openDB:(NSString *)path;

- (void)closeDB;

- (BOOL)executeUpdate:(NSString *)sql withArgumentsInArray:(NSArray *)arguments;

- (void)executeQuery:(NSString *)sql
withArgumentsInArray:(NSArray *)arguments
          syncResult:(void (^)(JFMResultSet *resultSet))syncResultBlock;

- (void)executeTransaction:(void (^)(JFMDatabase *db, BOOL *rollback))transactionBlock;
@end

NS_ASSUME_NONNULL_END
