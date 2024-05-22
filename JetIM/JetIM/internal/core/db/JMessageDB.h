//
//  JMessageDB.h
//  JetIM
//
//  Created by Nathan on 2023/12/8.
//

#import <Foundation/Foundation.h>
#import "JDBHelper.h"
#import "JConcreteMessage.h"

NS_ASSUME_NONNULL_BEGIN

@interface JMessageDB : NSObject

- (void)createTables;

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper;

- (nullable JConcreteMessage *)getMessageWithMessageId:(NSString *)messageId;

- (void)insertMessages:(NSArray<JConcreteMessage *> *)messages;

- (void)updateMessageAfterSend:(long long)clientMsgNo
                     messageId:(NSString *)messageId
                     timestamp:(long long)timestamp
                         seqNo:(long long)seqNo;

- (void)updateMessageContent:(JMessageContent *)content
                 contentType:(nonnull NSString *)type
               withMessageId:(NSString *)messageId;

- (void)messageSendFail:(long long)clientMsgNo;
- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction
                            contentTypes:(NSArray<NSString *> *)contentTypes;

- (void)deleteMessageByClientId:(long long)clientMsgNo;
- (void)deleteMessageByMessageId:(NSString *)messageId;
- (void)clearMessagesIn:(JConversation *)conversation startTime:(long long)startTime senderId:(NSString *)senderId;
- (NSArray<JMessage *> *)getMessagesByMessageIds:(NSArray<NSString *> *)messageIds;
- (NSArray<JMessage *> *)getMessagesByClientMsgNos:(NSArray<NSNumber *> *)clientMsgNos;
- (void)setMessageState:(JMessageState)state
        withClientMsgNo:(long long)clientMsgNo;
- (void)setMessagesRead:(NSArray<NSString *> *)messageIds;
- (void)setGroupMessageReadInfo:(NSDictionary<NSString *,JGroupMessageReadInfo *> *)msgs;
- (NSArray<JMessage *> *)searchMessagesWithContent:(NSString *)searchContent
                                    inConversation:(JConversation *)conversation
                                             count:(int)count
                                              time:(long long)time
                                         direction:(JPullDirection)direction
                                      contentTypes:(NSArray<NSString *> *)contentTypes;
- (NSString *)getLocalAttributeByMessageId:(NSString *)messageId;
- (void)setLocalAttribute:(NSString *)attribute forMessage:(NSString *)messageId;
- (NSString *)getLocalAttributeByClientMsgNo:(long long)clientMsgNo;
- (void)setLocalAttribute:(NSString *)attribute forClientMsgNo:(long long)clientMsgNo;

//- (NSArray <JMessage *> *)getMentionMessages:(JConversation *)conversation
//                                       count:(int)count
//                                        time:(long long)time
//                                   direction:(JPullDirection)direction;

#pragma mark - operation with db
- (void)insertMessage:(JMessage *)message
                 inDb:(JFMDatabase *)db;
@end

NS_ASSUME_NONNULL_END
