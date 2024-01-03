//
//  JSyncDB.h
//  JetIM
//
//  Created by Nathan on 2023/12/22.
//

#import <Foundation/Foundation.h>
#import "JDBHelper.h"

NS_ASSUME_NONNULL_BEGIN

@interface JProfileDB : NSObject

- (void)createTables;

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper;

- (long long)getConversationSyncTime;
- (long long)getMessageSendSyncTime;
- (long long)getMessageReceiveSyncTime;

- (void)setConversationSyncTime:(long long)time;
- (void)setMessageSendSyncTime:(long long)time;
- (void)setMessageReceiveSyncTime:(long long)time;
@end

NS_ASSUME_NONNULL_END
