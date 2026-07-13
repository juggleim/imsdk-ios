//
//  JSyncDB.m
// JuggleIM
//
//  Created by Nathan on 2023/12/22.
//

#import "JProfileDB.h"

//Latest profile version.
#define jProfileTableVersion 1
//Key for saving the profile database version in NSUserDefaults.
#define jProfileTableVersionKey @"ProfileVersion"

NSString *const kCreateSyncTable = @"CREATE TABLE IF NOT EXISTS profile ("
                                        "key VARCHAR (64) PRIMARY KEY,"
                                        "value VARCHAR (64)"
                                        ")";
NSString *const kGetValue = @"SELECT value FROM profile WHERE key = ?";
NSString *const kSetValue = @"INSERT OR REPLACE INTO profile (key, value) values (?, ?)";

NSString *const kConversationTime = @"conversation_time";
NSString *const kSendTime = @"send_time";
NSString *const kReceiveTime = @"receive_time";
NSString *const kPublicKey = @"public_key";
NSString *const kPrivateKey = @"private_key";

NSString *const kValue = @"value";

@interface JProfileDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JProfileDB
- (void)createTables {
    [self.dbHelper executeUpdate:kCreateSyncTable withArgumentsInArray:nil];
    [[NSUserDefaults standardUserDefaults] setObject:@(jProfileTableVersion) forKey:jProfileTableVersionKey];
}

- (void)updateTables {
    NSNumber *existedVersion = [[NSUserDefaults standardUserDefaults] objectForKey:jProfileTableVersionKey];
    if (jProfileTableVersion > existedVersion.intValue) {
        //update table
        
        [[NSUserDefaults standardUserDefaults] setObject:@(jProfileTableVersion) forKey:jProfileTableVersionKey];
    }

}

- (long long)getConversationSyncTime {
    __block long long time = 0;
    [self.dbHelper executeQuery:kGetValue
           withArgumentsInArray:@[kConversationTime]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            time = [[resultSet stringForColumn:kValue] longLongValue];
        }
    }];
    return time;
}

- (long long)getMessageSendSyncTime {
    __block long long time = 0;
    [self.dbHelper executeQuery:kGetValue
           withArgumentsInArray:@[kSendTime]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            time = [[resultSet stringForColumn:kValue] longLongValue];
        }
    }];
    return time;
}

- (long long)getMessageReceiveSyncTime {
    __block long long time = 0;
    [self.dbHelper executeQuery:kGetValue
           withArgumentsInArray:@[kReceiveTime]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            time = [[resultSet stringForColumn:kValue] longLongValue];
        }
    }];
    return time;
}

- (NSData *)getE2EEPubKey {
    __block NSData *pubKey = nil;
    [self.dbHelper executeQuery:kGetValue
           withArgumentsInArray:@[kPublicKey]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            NSString *str = [resultSet stringForColumn:kValue];
            pubKey = [self dataFromHexString:str];
        }
    }];
    return pubKey;
}

- (NSData *)getE2EEPriKey {
    __block NSData *priKey = nil;
    [self.dbHelper executeQuery:kGetValue
           withArgumentsInArray:@[kPrivateKey]
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        if ([resultSet next]) {
            NSString *str = [resultSet stringForColumn:kValue];
            priKey = [self dataFromHexString:str];
        }
    }];
    return priKey;
}

- (void)setConversationSyncTime:(long long)time {
    [self.dbHelper executeUpdate:kSetValue
            withArgumentsInArray:@[kConversationTime, [NSString stringWithFormat:@"%lld", time]]];
}

- (void)setMessageSendSyncTime:(long long)time {
    [self.dbHelper executeUpdate:kSetValue
            withArgumentsInArray:@[kSendTime, [NSString stringWithFormat:@"%lld", time]]];
}

- (void)setMessageReceiveSyncTime:(long long)time {
    [self.dbHelper executeUpdate:kSetValue
            withArgumentsInArray:@[kReceiveTime, [NSString stringWithFormat:@"%lld", time]]];
}

- (void)setE2EEWithPubKey:(NSData *)pubKey priKey:(NSData *)priKey {
    [self.dbHelper executeUpdate:kSetValue
            withArgumentsInArray:@[kPublicKey, [self hexStringWith:pubKey]]];
    [self.dbHelper executeUpdate:kSetValue
            withArgumentsInArray:@[kPrivateKey,[self hexStringWith:priKey]]];
}

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper {
    if (self = [super init]) {
        self.dbHelper = dbHelper;
    }
    return self;
}

- (NSString *)hexStringWith:(NSData *)data {
    if (!data) {
        return @"";
    }
    const uint8_t *bytes = data.bytes;
    NSMutableString *hex = [NSMutableString stringWithCapacity:data.length * 2];
    for (NSUInteger i = 0; i < data.length; i++) {
        [hex appendFormat:@"%02x", bytes[i]];
    }
    return hex;
}

- (NSData *)dataFromHexString:(NSString *)hexStr {
    if (!hexStr || hexStr.length % 2 != 0) return nil;
    
    NSUInteger len = hexStr.length / 2;
    uint8_t *buf = malloc(len);
    if (!buf) return nil;
    
    unsigned int tempVal;
    for (NSUInteger i = 0; i < len; i++) {
        NSString *sub = [hexStr substringWithRange:NSMakeRange(i*2, 2)];
        NSScanner *scanner = [NSScanner scannerWithString:sub];
        [scanner scanHexInt:&tempVal];
        buf[i] = (uint8_t)tempVal;
    }
    
    NSData *data = [NSData dataWithBytes:buf length:len];
    free(buf);
    return data;
}

@end
