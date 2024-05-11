//
//  JConversationDB.h
//  JetIM
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

- (void)createTables;
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
- (void)setTop:(BOOL)isTop conversation:(JConversation *)conversation;
- (void)setTopTime:(long long)time conversation:(JConversation *)conversation;
- (void)setMention:(BOOL)isMention conversation:(JConversation *)conversation;
- (void)clearMentionstatus;
- (int)getTotalUnreadCount;
- (void)clearTotalUnreadCount;
@end

NS_ASSUME_NONNULL_END
