//
//  JUserInfoDB.h
// JuggleIM
//
//  Created by Nathan on 2024/3/12.
//

#import <Foundation/Foundation.h>
#import "JDBHelper.h"
#import "JUserInfo.h"
#import "JGroupInfo.h"

NS_ASSUME_NONNULL_BEGIN

@interface JUserInfoDB : NSObject

- (void)createTables;

- (void)updateTables;

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper;

- (JUserInfo *)getUserInfo:(NSString *)userId;

- (JGroupInfo *)getGroupInfo:(NSString *)groupId;

- (void)insertUserInfos:(NSArray <JUserInfo *> *)userInfos;

- (void)insertGroupInfos:(NSArray <JGroupInfo *> *)groupInfos;
@end

NS_ASSUME_NONNULL_END
