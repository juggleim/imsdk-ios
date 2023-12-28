//
//  JDBManager.h
//  Juggle
//
//  Created by Nathan on 2023/12/8.
//

#import <Foundation/Foundation.h>
#import "JConcreteConversationInfo.h"
#import "JConcreteMessage.h"

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
- (nullable JConcreteConversationInfo *)getConversationInfo:(JConversation *)conversation;
- (NSArray<JConcreteConversationInfo *> *)getConversationInfoList;

#pragma mark - message table
- (void)insertMessages:(NSArray<JConcreteMessage *> *)messages;
- (void)updateMessageAfterSend:(long long)clientMsgNo
                     messageId:(NSString *)messageId
                     timestamp:(long long)timestamp
                  messageIndex:(long long)messageIndex;

@end

NS_ASSUME_NONNULL_END
