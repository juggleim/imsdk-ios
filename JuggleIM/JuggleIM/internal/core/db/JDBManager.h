//
//  JDBManager.h
// JuggleIM
//
//  Created by Nathan on 2023/12/8.
//

#import <Foundation/Foundation.h>
#import "JConcreteConversationInfo.h"
#import "JConcreteMessage.h"
#import "JGroupMessageReadInfo.h"

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
- (NSArray<JConversationInfo *> *)getTopConversationInfoListWithTypes:(NSArray<NSNumber *> *)conversationTypes
                                                                count:(int)count
                                                            timestamp:(long long)ts
                                                            direction:(JPullDirection)direction;
- (void)setDraft:(NSString *)draft
  inConversation:(JConversation *)conversation;
- (void)clearDraftInConversation:(JConversation *)conversation;
- (void)clearUnreadCountBy:(JConversation *)conversation
                  msgIndex:(long long)msgIndex;
- (void)updateLastMessage:(JConcreteMessage *)message;
- (void)setMute:(BOOL)isMute
   conversation:(JConversation *)conversation;
- (void)setTop:(BOOL)isTop
          time:(long long)time
  conversation:(JConversation *)conversation;
- (void)setUnread:(BOOL)isUnread
     conversation:(JConversation *)conversation;
- (void)clearUnreadTag;
- (int)getTotalUnreadCount;
- (int)getUnreadCountWithTypes:(NSArray<NSNumber *> *)conversationTypes;
- (void)setMentionInfo:(JConversation *)conversation
      mentionInfoJson:(NSString *)mentionInfoJson;
- (void)clearMentionInfo;
- (void)clearTotalUnreadCount;
- (void)updateTime:(long long)time
   forConversation:(JConversation *)conversation;
- (void)clearLastMessage:(JConversation *)conversation;
- (void)updateLastMessageWithoutIndex:(JConcreteMessage *)message;
- (void)setLastMessageHasRead:(JConversation *)conversation;
- (void)updateLastMessageState:(JConversation *)conversation
                         state:(JMessageState)state
               withClientMsgNo:(long long)clientMsgNo;
- (void)setTopConversationOrderType:(JTopConversationsOrderType)type;
#pragma mark - message table
- (void)insertMessages:(NSArray<JConcreteMessage *> *)messages;
- (nullable JConcreteMessage *)getMessageWithMessageId:(NSString *)messageId;
- (void)updateMessageAfterSend:(long long)clientMsgNo
                     messageId:(NSString *)messageId
                     timestamp:(long long)timestamp
                  seqNo:(long long)messageIndex;
- (void)updateMessageContent:(JMessageContent *)content
                 contentType:(NSString *)type
               withMessageId:(NSString *)messageId;
- (void)updateMessageContent:(JMessageContent *)content
                 contentType:(NSString *)type
             withClientMsgNo:(long long)clientMsgNo;
- (void)updateMessage:(JConcreteMessage *)message;
- (void)setMessagesRead:(NSArray <NSString *> *)messageIds;
- (void)setGroupMessageReadInfo:(NSDictionary <NSString *, JGroupMessageReadInfo *> *)msgs;
- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction
                            contentTypes:(NSArray<NSString *> *)contentTypes;
- (void)deleteMessageByClientIds:(NSArray <NSNumber *> *)clientMsgNos;
- (void)deleteMessageByMessageIds:(NSArray <NSString *> *)messageIds;
- (void)clearMessagesIn:(JConversation *)conversation startTime:(long long)startTime senderId:(NSString *)senderId;
- (NSArray<JMessage *> *)getMessagesByMessageIds:(NSArray<NSString *> *)messageIds;
- (NSArray<JMessage *> *)getMessagesByClientMsgNos:(NSArray<NSNumber *> *)clientMsgNos;
- (void)setMessageState:(JMessageState)state
        withClientMsgNo:(long long)clientMsgNo;
- (NSArray<JMessage *> *)searchMessagesWithContent:(nullable NSString *)searchContent
                                             count:(int)count
                                              time:(long long)time
                                     pullDirection:(JPullDirection)pullDirection
                                      contentTypes:(nullable NSArray<NSString *> *)contentTypes
                                           senders:(nullable NSArray<NSString *> *)senderUserIds
                                            states:(nullable NSArray<NSNumber *> *)messageStates
                                     conversations:(nullable NSArray<JConversation *> *)conversations;
- (NSString *)getLocalAttributeByMessageId:(NSString *)messageId;
- (void)setLocalAttribute:(NSString *)attribute forMessage:(NSString *)messageId;
- (NSString *)getLocalAttributeByClientMsgNo:(long long)clientMsgNo;
- (void)setLocalAttribute:(NSString *)attribute forClientMsgNo:(long long)clientMsgNo;
- (JConcreteMessage *)getLastMessage:(JConversation *)conversation;
- (void)clearChatroomMessageExclude:(NSArray <NSString *> *)chatroomIds;
- (void)clearChatroomMessage:(NSString *)chatroomId;

#pragma mark - user table
- (JUserInfo *)getUserInfo:(NSString *)userId;

- (JGroupInfo *)getGroupInfo:(NSString *)groupId;

- (void)insertUserInfos:(NSArray <JUserInfo *> *)userInfos;

- (void)insertGroupInfos:(NSArray <JGroupInfo *> *)groupInfos;
@end

NS_ASSUME_NONNULL_END
