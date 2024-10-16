//
//  JConversationDB.h
// JuggleIM
//
//  Created by Nathan on 2023/12/8.
//

#import <Foundation/Foundation.h>
#import "JDBHelper.h"
#import "JConcreteConversationInfo.h"
#import "JMessageDB.h"

NS_ASSUME_NONNULL_BEGIN

@interface JConversationDB : NSObject
@property (nonatomic, strong) JMessageDB *messageDB;
@property (nonatomic, assign) JTopConversationsOrderType topConversationsOrderType;

- (void)createTables;
- (void)updateTables;
- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper;
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
- (void)setMute:(BOOL)isMute conversation:(JConversation *)conversation;
- (void)setTop:(BOOL)isTop
          time:(long long)time
  conversation:(JConversation *)conversation;
- (void)setUnread:(BOOL)isUnread
     conversation:(JConversation *)conversation;
- (void)clearUnreadTag;
- (int)getTotalUnreadCount;
- (int)getUnreadCountWithTypes:(NSArray<NSNumber *> *)conversationTypes;
- (void)clearTotalUnreadCount;
- (void)updateTime:(long long)time
   forConversation:(JConversation *)conversation;
-(void)setMentionInfo:(JConversation *)conversation
      mentionInfoJson:(NSString *)mentionInfoJson;
-(void)clearMentionInfo;
- (void)clearLastMessage:(JConversation *)conversation;
- (void)updateLastMessageWithoutIndex:(JConcreteMessage *)message;
- (void)setLastMessageHasRead:(JConversation *)conversation;
- (void)updateLastMessageState:(JConversation *)conversation
                         state:(JMessageState)state
               withClientMsgNo:(long long)clientMsgNo;
@end

NS_ASSUME_NONNULL_END
