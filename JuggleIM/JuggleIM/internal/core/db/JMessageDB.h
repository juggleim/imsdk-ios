//
//  JMessageDB.h
// JuggleIM
//
//  Created by Nathan on 2023/12/8.
//

#import <Foundation/Foundation.h>
#import "JDBHelper.h"
#import "JConcreteMessage.h"

NS_ASSUME_NONNULL_BEGIN

@interface JMessageDB : NSObject

- (void)createTables;

- (void)updateTables;

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper;

- (nullable JConcreteMessage *)getMessageWithMessageId:(NSString *)messageId
                                           currentTime:(long long)now;

- (void)insertMessages:(NSArray<JConcreteMessage *> *)messages;

- (void)updateMessageAfterSend:(long long)clientMsgNo
                     messageId:(NSString *)messageId
                     timestamp:(long long)timestamp
                         seqNo:(long long)seqNo
              groupMemberCount:(int)count;

- (void)updateMessageAfterSendWithClientUid:(NSString *)clientUid
                                  messageId:(NSString *)messageId
                                  timestamp:(long long)timestamp
                                      seqNo:(long long)seqNo
                           groupMemberCount:(int)count;

- (void)updateMessageContent:(JMessageContent *)content
                 contentType:(nonnull NSString *)type
               withMessageId:(NSString *)messageId;

- (void)updateMessageContent:(JMessageContent *)content
                 contentType:(NSString *)type
             withClientMsgNo:(long long)clientMsgNo;

- (void)setMessageFlags:(int)flags withMessageId:(NSString *)messageId;

- (void)updateDestroyTime:(long long)destroyTime
            withMessageId:(NSString *)messageId;

-(void)updateMessage:(JConcreteMessage *)message;

//- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
//                                   count:(int)count
//                                    time:(long long)time
//                               direction:(JPullDirection)direction
//                            contentTypes:(NSArray<NSString *> *)contentTypes
//                             currentTime:(long long)now;

- (void)deleteMessageByClientIds:(NSArray <NSNumber *> *)clientMsgNos;
- (void)deleteMessageByMessageIds:(NSArray <NSString *> *)messageIds;
- (void)clearMessagesIn:(JConversation *)conversation startTime:(long long)startTime senderId:(NSString *)senderId;
- (NSArray<JMessage *> *)getMessagesByMessageIds:(NSArray<NSString *> *)messageIds;
- (NSArray<JMessage *> *)getMessagesByClientMsgNos:(NSArray<NSNumber *> *)clientMsgNos;
- (JConcreteMessage *)getMessageWithClientUid:(NSString *)clientUid;
- (void)setMessageState:(JMessageState)state
        withClientMsgNo:(long long)clientMsgNo;
- (void)setMessagesRead:(NSArray<NSString *> *)messageIds
               readTime:(long long)readTime;
- (void)setGroupMessageReadInfo:(NSDictionary<NSString *,JGroupMessageReadInfo *> *)msgs;
- (NSArray<JMessage *> *)searchMessagesWithContent:(NSString *)searchContent
                                             count:(int)count
                                              time:(long long)time
                                     pullDirection:(JPullDirection)pullDirection
                                      contentTypes:(NSArray<NSString *> *)contentTypes
                                           senders:(NSArray<NSString *> *)senderUserIds
                                            states:(NSArray<NSNumber *> *)messageStates
                                     conversations:(NSArray<JConversation *> *)conversations
                                 conversationTypes:(NSArray<NSNumber *> *)conversationtypes
                                       currentTime:(long long)now;
- (NSArray <JSearchConversationsResult *> *)searchMessageInConversations:(JQueryMessageOptions *)option
                                                             currentTime:(long long)now;
- (NSString *)getLocalAttributeByMessageId:(NSString *)messageId;
- (void)setLocalAttribute:(NSString *)attribute forMessage:(NSString *)messageId;
- (NSString *)getLocalAttributeByClientMsgNo:(long long)clientMsgNo;
- (void)setLocalAttribute:(NSString *)attribute forClientMsgNo:(long long)clientMsgNo;
- (JConcreteMessage *)getLastMessage:(JConversation *)conversation
                         currentTime:(long long)now;
- (void)clearChatroomMessageExclude:(NSArray<NSString *> *)chatroomIds;
- (void)clearChatroomMessage:(NSString *)chatroomId;
- (void)batchSetStateFail;
#pragma mark - operation with db
- (void)insertMessage:(JMessage *)message
                 inDb:(JFMDatabase *)db;
#pragma mark - update table
+ (NSString *)alterTableAddFlags;
+ (NSString *)addConversationTSIndex;
+ (NSString *)addDTConversationTSIndex;
+ (NSString *)addMessageClientUidIndex;
+ (NSString *)alterTableAddLifeTime;
+ (NSString *)alterTableAddLifeTimeAfterRead;
+ (NSString *)alterTableAddDestroyTime;
+ (NSString *)alterTableAddSubChannel;
+ (NSString *)alterTableAddReadTime;
+ (NSString *)addDTConversationTSIndex2;
+ (NSString *)addDestroyTimeIndex;
+ (NSString *)addTimestampIndex;
+ (NSString *)addConversationSubchannelIndex;
+ (NSString *)addStateIndex;
+ (NSString *)removeConversationIndex;
+ (NSString *)removeConversationTSIndex;
+ (NSString *)removeDSConversationTSIndex;
@end

NS_ASSUME_NONNULL_END
