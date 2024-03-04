//
//  JDBManager.h
//  JetIM
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
- (void)closeIMDB;
- (BOOL)isOpen;

#pragma mark - sync table
- (long long)getConversationSyncTime;
- (long long)getMessageSendSyncTime;
- (long long)getMessageReceiveSyncTime;

- (void)setConversationSyncTime:(long long)time;
- (void)setMessageSendSyncTime:(long long)time;
- (void)setMessageReceiveSyncTime:(long long)time;

#pragma mark - conversation table
- (void)insertConversations:(NSArray<JConcreteConversationInfo *> *)conversations
                 completion:(nullable void (^)(NSArray<JConcreteConversationInfo *> *insertConversations, NSArray<JConcreteConversationInfo *> *updateConversations))completeBlock;
- (nullable JConcreteConversationInfo *)getConversationInfo:(JConversation *)conversation;
- (void)deleteConversationInfoBy:(JConversation *)conversation;
- (NSArray<JConcreteConversationInfo *> *)getConversationInfoList;
- (NSArray<JConversationInfo *> *)getConversationInfoListWithTypes:(NSArray<NSNumber *> *)conversationTypes
                                                             count:(int)count
                                                         timestamp:(long long)ts
                                                         direction:(JPullDirection)direction;
- (void)setDraft:(NSString *)draft
  inConversation:(JConversation *)conversation;
- (void)clearDraftInConversation:(JConversation *)conversation;
- (void)clearUnreadCountBy:(JConversation *)conversation
                  msgIndex:(long long)msgIndex;
- (void)updateLastMessage:(JConcreteMessage *)message;

#pragma mark - message table
- (void)insertMessages:(NSArray<JConcreteMessage *> *)messages;
- (void)updateMessageAfterSend:(long long)clientMsgNo
                     messageId:(NSString *)messageId
                     timestamp:(long long)timestamp
                  messageIndex:(long long)messageIndex;
- (void)updateMessageContent:(JMessageContent *)content
                 contentType:(NSString *)type
               withMessageId:(NSString *)messageId;
- (void)messageSendFail:(long long)clientMsgNo;
- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction
                            contentTypes:(NSArray<NSString *> *)contentTypes;
- (void)deleteMessageByClientId:(long long)clientMsgNo;
- (void)deleteMessageByMessageId:(NSString *)messageId;
- (void)clearMessagesIn:(JConversation *)conversation;
- (NSArray<JMessage *> *)getMessagesByMessageIds:(NSArray<NSString *> *)messageIds;
- (NSArray<JMessage *> *)getMessagesByClientMsgNos:(NSArray<NSNumber *> *)clientMsgNos;
- (void)setMessageState:(JMessageState)state
        withClientMsgNo:(long long)clientMsgNo;
@end

NS_ASSUME_NONNULL_END
