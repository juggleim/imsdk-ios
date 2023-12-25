//
//  JDBManager.h
//  Juggle
//
//  Created by Nathan on 2023/12/8.
//

#import <Foundation/Foundation.h>
#import "JConcreteConversationInfo.h"

NS_ASSUME_NONNULL_BEGIN

@interface JDBManager : NSObject

- (BOOL)openIMDB:(NSString *)appKey
          userId:(NSString *)userId;

#pragma mark - sync table
- (long long)getConversationSyncTime;
- (long long)getMessageSendSyncTime;
- (long long)getMessageReceiveSyncTime;

- (void)setConversationSyncTime:(long long)time;
- (void)setMessageSendSyncTime:(long long)time;
- (void)setMessageReceiveSyncTime:(long long)time;

#pragma mark - conversation table
- (void)insertConversations:(NSArray<JConcreteConversationInfo *> *)conversations;


#pragma mark - message table

@end

NS_ASSUME_NONNULL_END
