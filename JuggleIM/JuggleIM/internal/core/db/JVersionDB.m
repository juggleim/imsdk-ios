//
//  JVersionDB.m
//  JuggleIM
//
//  Created by Fei Li on 2025/2/21.
//

#import "JVersionDB.h"
#import "JMessageDB.h"
#import "JUserInfoDB.h"
#import "JReactionDB.h"

#define jDBVersion 1

NSString *const jCreateVersionTable = @"CREATE TABLE IF NOT EXISTS version (version INTEGER)";
NSString *const jGetVersion = @"SELECT version FROM version";
NSString *const jSetVersion = @"UPDATE version SET version = ?";

@interface JVersionDB()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JVersionDB

- (void)createTables {
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        [db executeUpdate:jCreateVersionTable];
        [db executeUpdate:jSetVersion withArgumentsInArray:@[@(jDBVersion)]];
    }];
}

- (void)updateTables {
    int version = [self getVersion];
    
    if (version < jDBVersion) {
        [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
            if (version < 1) {
                [db executeUpdate:jCreateVersionTable];
                [db executeUpdate:[JMessageDB alterTableAddFlags]];
                [db executeUpdate:[JUserInfoDB alterUserTableAddType]];
                [db executeUpdate:[JReactionDB createReactionTable]];
            }
            [db executeUpdate:jSetVersion withArgumentsInArray:@[@(jDBVersion)]];
        }];
    }
}

- (int)getVersion {
    __block int version = 0;
    [self.dbHelper executeQuery:jGetVersion
           withArgumentsInArray:nil
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            version = [resultSet intForColumn:@"version"];
        }
    }];
    return version;
}

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper {
    if (self = [super init]) {
        self.dbHelper = dbHelper;
    }
    return self;
}

@end
