//
//  JE2EEDB.h
//  JuggleIM
//
//  Created by Fei Li on 2026/6/18.
//

#import <Foundation/Foundation.h>
#import "JDBHelper.h"
#import "JE2EEInfo.h"

NS_ASSUME_NONNULL_BEGIN

@interface JE2EEDB : NSObject

- (void)createTables;
- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper;

- (NSArray<JE2EEInfo *> *)getE2EEInfo:(NSString *)userId;
- (void)updateE2EEInfo:(NSArray <JE2EEInfo *> *)infoList;

+ (NSString *)createPublicKeyTable;
+ (NSString *)createPublicKeyIndex;
@end

NS_ASSUME_NONNULL_END
