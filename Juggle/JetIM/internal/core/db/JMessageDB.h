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
                  messageIndex:(long long)messageIndex;

- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction;

- (void)deleteMessageByClientId:(long long)clientMsgNo;
- (void)deleteMessageByMessageId:(NSString *)messageId;

#pragma mark - operation with db
- (void)insertMessage:(JMessage *)message
                 inDb:(JFMDatabase *)db;
@end

NS_ASSUME_NONNULL_END
