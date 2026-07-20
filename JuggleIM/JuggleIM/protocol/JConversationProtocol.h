//
//  JConversationProtocol.h
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import <JuggleIM/JConversation.h>
#import <JuggleIM/JConversationInfo.h>
#import <JuggleIM/JuggleIMConst.h>
#import <JuggleIM/JGetConversationOptions.h>
#import <JuggleIM/JConversationTagInfo.h>

@protocol JConversationDelegate <NSObject>

- (void)conversationInfoDidAdd:(NSArray<JConversationInfo *> *)conversationInfoList;

- (void)conversationInfoDidUpdate:(NSArray<JConversationInfo *> *)conversationInfoList;

- (void)conversationInfoDidDelete:(NSArray<JConversationInfo *> *)conversationInfoList;

- (void)totalUnreadMessageCountDidUpdate:(int)count;

@end

@protocol JConversationSyncDelegate <NSObject>

- (void)conversationSyncDidComplete;

@end

@protocol JConversationTagDelegate <NSObject>
/// Tag created.
/// - Parameter tagInfo: Conversation tag.
- (void)tagDidCreate:(JConversationTagInfo *)tagInfo;

/// Tag destroyed.
/// - Parameter tagId: Tag ID.
- (void)tagDidDestroy:(NSString *)tagId;

/// Tag name changed.
/// - Parameters:
///   - tagId: Tag ID.
///   - tagName: Tag name.
- (void)tagNameDidUpdate:(NSString *)tagId
                    name:(NSString *)tagName;

- (void)conversationsDidAddToTag:(NSString *)tagId
                   conversations:(NSArray <JConversation *> *)conversationList;

- (void)conversationsDidRemoveFromTag:(NSString *)tagId
                        conversations:(NSArray <JConversation *> *)conversationList;

@end

@protocol JConversationProtocol <NSObject>

/// Gets all conversation information, ordered from newest to oldest.
- (NSArray<JConversationInfo *> *)getConversationInfoList;

/// Gets conversation information by page. Results are sorted by conversation time in descending order(newest first).
/// - Parameters:
///   - conversationTypes: Conversation type list.
///   - count: Fetch count.
///   - ts: Fetch timestamp. Pass 0 for the current time.
///   - direction: Fetch direction.
- (NSArray<JConversationInfo *> *)getConversationInfoListWithTypes:(NSArray<NSNumber *> *)conversationTypes
                                                             count:(int)count
                                                         timestamp:(long long)ts
                                                         direction:(JPullDirection)direction;

/// Gets conversation information by page. Results are sorted by conversation time in descending order, newest first.
/// - Parameters:
///   - count: Fetch count.
///   - ts: Fetch timestamp. Pass 0 for the current time.
///   - direction: Fetch direction.
- (NSArray<JConversationInfo *> *)getConversationInfoListByCount:(int)count
                                                       timestamp:(long long)ts
                                                       direction:(JPullDirection)direction;

/// Gets conversation information by query options.
/// - Parameter options: Query options.
- (NSArray<JConversationInfo *> *)getConversationInfoListWith:(JGetConversationOptions *)options;

/// Gets information for a specific conversation.
/// - Parameter conversation: Conversation identifier.
- (JConversationInfo *)getConversationInfo:(JConversation *)conversation;


/// Deletes a specific conversation.
/// - Parameter conversation: Conversation identifier.
- (void)deleteConversationInfoBy:(JConversation *)conversation
                         success:(void (^)(void))successBlock
                           error:(void (^)(JErrorCode code))errorBlock;

/// Sets draft content.
/// - Parameters:
///   - draft: Draft content.
///   - conversation: Conversation identifier.
- (void)setDraft:(NSString *)draft
  inConversation:(JConversation *)conversation;

/// Clears the draft.
/// - Parameter conversation: Conversation identifier.
- (void)clearDraftInConversation:(JConversation *)conversation;

/// Creates conversation information.
/// - Parameters:
///   - conversation: Conversation identifier.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)createConversationInfo:(JConversation *)conversation
                       success:(void (^)(JConversationInfo *))successBlock
                         error:(void (^)(JErrorCode code))errorBlock;

/// Gets the total unread message count.
- (int)getTotalUnreadCount;

/// Gets the unread message count by conversation type.
/// - Parameter conversationTypes: Conversation type array. Convert JConversationType to NSNumber and build an NSArray.
- (int)getUnreadCountWithTypes:(NSArray<NSNumber *> *)conversationTypes;

/// Adds a conversation tag.
/// - Parameters:
///   - tagId: Tag ID.
///   - name: Tag name.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)createConversationTag:(NSString *)tagId
                         name:(NSString *)name
                      success:(void (^)(void))successBlock
                        error:(void (^)(JErrorCode code))errorBlock;

/// Deletes a conversation tag.
/// - Parameters:
///   - tagId: Tag ID.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)destroyConversationTag:(NSString *)tagId
                       success:(void (^)(void))successBlock
                         error:(void (^)(JErrorCode code))errorBlock;

/// Updates the conversation tag name.
/// - Parameters:
///   - name: Tag name.
///   - tagId: Tag ID.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)updateConversationTagName:(NSString *)name
                            forId:(NSString *)tagId
                          success:(void (^)(void))successBlock
                            error:(void (^)(JErrorCode code))errorBlock;

/// Gets the cached conversation tag list. Cached data may not be the latest version and can be used for immediate UI rendering to optimize user experience.
- (NSArray <JConversationTagInfo *> *)getCachedConversationTagList;

/// Gets the conversation tag list.
/// - Parameters:
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)getConversationTagList:(void (^)(NSArray <JConversationTagInfo *> *tagInfoList))successBlock
                         error:(void (^)(JErrorCode code))errorBlock;

/// Gets all tags for a specific conversation.
/// - Parameter conversation: Conversation identifier.
- (NSArray <JConversationTagInfo *> *)getTagsForConversation:(JConversation *)conversation;

/// Gets the unread message count by tag ID.
/// - Parameter tagId: Tag ID.
- (int)getUnreadCountWithTag:(NSString *)tagId;

/// Clears the unread count for a conversation.
/// - Parameter conversation: Conversation object.
- (void)clearUnreadCountByConversation:(JConversation *)conversation
                               success:(void (^)(void))successBlock
                                 error:(void (^)(JErrorCode code))errorBlock;

/// Adds conversations to a tag.
/// - Parameters:
///   - conversationList: Conversation list.
///   - tagId: Tag ID.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)addConversationList:(NSArray <JConversation *> *)conversationList
                      toTag:(NSString *)tagId
                    success:(void (^)(void))successBlock
                      error:(void (^)(JErrorCode code))errorBlock;

/// Removes conversations from a tag.
/// - Parameters:
///   - conversationList: Conversation list to remove.
///   - tagId: Tag ID.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)removeConversationList:(NSArray <JConversation *> *)conversationList
                       fromTag:(NSString *)tagId
                       success:(void (^)(void))successBlock
                         error:(void (^)(JErrorCode code))errorBlock;

/// Sets mute.
/// - Parameters:
///   - isMute: YES to mute, NO to unmute.
///   - conversation: Conversation identifier.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)setMute:(BOOL)isMute
   conversation:(JConversation *)conversation
        success:(void (^)(void))successBlock
          error:(void (^)(JErrorCode code))errorBlock;

/// Sets top.
/// - Parameters:
///   - isTop: YES to pin, NO to unpin.
///   - conversation: Conversation identifier.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)setTop:(BOOL)isTop
  conversation:(JConversation *)conversation
       success:(void (^)(void))successBlock
         error:(void (^)(JErrorCode code))errorBlock;

/// Gets pinned conversation information by page.
- (NSArray<JConversationInfo *> *)getTopConversationInfoListByCount:(int)count
                                                          timestamp:(long long)ts
                                                          direction:(JPullDirection)direction;

/// Clears the total unread count.
- (void)clearTotalUnreadCount:(void (^)(void))successBlock
                        error:(void (^)(JErrorCode code))errorBlock;

/// Marks as unread.
/// - Parameters:
///   - conversation: Conversation identifier.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)setUnread:(JConversation *)conversation
          success:(void (^)(void))successBlock
            error:(void (^)(JErrorCode code))errorBlock;

/// Sets the pinned conversation sort rule.
/// - Parameter type: Sort rule.
- (void)setTopConversationsOrderType:(JTopConversationsOrderType)type;

- (void)addDelegate:(id<JConversationDelegate>)delegate;

- (void)addSyncDelegate:(id<JConversationSyncDelegate>)delegate;

- (void)addTagDelegate:(id<JConversationTagDelegate>)delegate;

@end
