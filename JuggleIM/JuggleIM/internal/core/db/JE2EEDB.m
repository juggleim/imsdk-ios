//
//  JE2EEDB.m
//  JuggleIM
//
//  Created by Fei Li on 2026/6/18.
//

#import "JE2EEDB.h"

NSString *const jCreatePublicKeyTable = @"CREATE TABLE IF NOT EXISTS public_key ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "user_id VARCHAR (64),"
    "device_id VARCHAR (64),"
    "public_key BLOB"
    ")";
NSString *const jCreatePublicKeyIndex = @"CREATE UNIQUE INDEX IF NOT EXISTS idx_public_key ON public_key(user_id, device_id)";
NSString *const jGetE2EEInfo = @"SELECT * FROM public_key WHERE user_id = ?";
NSString *const jUpdateE2EEInfo = @"INSERT OR REPLACE INTO public_key (user_id, device_id, public_key) VALUES (?, ?, ?)";

@interface JE2EEDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JE2EEDB

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper {
    if (self = [super init]){
        self.dbHelper = dbHelper;
    }
    return self;
}

- (void)createTables {
    [self.dbHelper executeUpdate:jCreatePublicKeyTable withArgumentsInArray:nil];
    [self.dbHelper executeUpdate:jCreatePublicKeyIndex withArgumentsInArray:nil];
}

- (NSArray<JE2EEInfo *> *)getE2EEInfo:(NSString *)userId {
    if (userId.length == 0) {
        return @[];
    }
    NSMutableArray <JE2EEInfo *> *result = [NSMutableArray array];
    NSString *sql = jGetE2EEInfo;
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:@[userId]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        while ([resultSet next]) {
            JE2EEInfo *info = [self e2eeInfoWith:resultSet];
            [result addObject:info];
        }
    }];
    return [result copy];
}

- (void)updateE2EEInfo:(NSArray<JE2EEInfo *> *)infoList {
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        for (JE2EEInfo *info in infoList) {
            NSString *sql = jUpdateE2EEInfo;
            [db executeUpdate:sql,
             info.userId,
             info.deviceId,
             info.pubKey
            ];
        }
    }];
}

- (JE2EEInfo *)e2eeInfoWith:(JFMResultSet *)rs {
    JE2EEInfo *info = [JE2EEInfo new];
    info.userId = [rs stringForColumn:@"user_id"];
    info.deviceId = [rs stringForColumn:@"device_id"];
    info.pubKey = [rs dataForColumn:@"public_key"];
    return info;
}

+ (NSString *)createPublicKeyTable {
    return jCreatePublicKeyTable;
}

+ (NSString *)createPublicKeyIndex {
    return jCreatePublicKeyIndex;
}

@end
