//
//  JMessageProtocol.h
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import <JuggleIM/JMessage.h>
#import <JuggleIM/JConversation.h>
#import <JuggleIM/JuggleIMConst.h>
#import <JuggleIM/JMessageUploadProvider.h>
#import <JuggleIM/JMediaMessageContent.h>
#import <JuggleIM/JTimePeriod.h>
#import <JuggleIM/JGetMessageOptions.h>
#import <JuggleIM/JQueryMessageOptions.h>
#import <JuggleIM/JSearchConversationsResult.h>
#import <JuggleIM/JMessageReaction.h>
#import <UIKit/UIImage.h>
#import <JuggleIM/JGetFavoriteMessageOption.h>
#import <JuggleIM/JFavoriteMessage.h>
#import <JuggleIM/JGroupMessageReadInfoDetail.h>

@class JMergeMessage;

@protocol JMessageDelegate <NSObject>
/// Callback when a message is received.
- (void)messageDidReceive:(JMessage *)message;

@optional
/// Callback when a message is recalled.
- (void)messageDidRecall:(JMessage *)message;
/// Callback when messages are deleted.
- (void)messageDidDelete:(JConversation *)conversation
            clientMsgNos:(NSArray <NSNumber *> *)clientMsgNos;
/// Callback when messages are cleared before a specific time in a specific conversation.
/// - Parameters:
///   - conversation: Conversation identifier for the cleared messages.
///   - timestamp: Timestamp in milliseconds. Messages before this timestamp are cleared.
///   - senderId: If not empty, only messages from this sender ID are cleared.
- (void)messageDidClear:(JConversation *)conversation
              timestamp:(long long)timestamp
               senderId:(NSString *)senderId;
/// Callback when a message is updated.
/// - Parameter message: Updated message.
- (void)messageDidUpdate:(JMessage *)message;

/// Callback when a message reaction is added.
/// - Parameter reaction: Added message reaction.
/// - Parameter conversation: Conversation it belongs to.
- (void)messageReactionDidAdd:(JMessageReaction *)reaction
               inConversation:(JConversation *)conversation;

/// Callback when a message reaction is removed.
/// - Parameter reaction: Removed message reaction.
/// - Parameter conversation: Conversation it belongs to.
- (void)messageReactionDidRemove:(JMessageReaction *)reaction
                  inConversation:(JConversation *)conversation;

/// Callback when a message is pinned or unpinned.
/// - Parameters:
///   - isTop: YES to pin, NO to unpin.
///   - message: Corresponding message.
///   - userInfo: User who performed the pin operation.
- (void)messageDidSetTop:(BOOL)isTop
                 message:(JMessage *)message
                    user:(JUserInfo *)userInfo;

@end

@protocol JMessageSyncDelegate <NSObject>
/// Callback when message synchronization completes.
- (void)messageSyncDidComplete;
@end

@protocol JMessageReadReceiptDelegate <NSObject>
/// Callback for one-to-one message reads.
/// - Parameters:
///   - messageIds: Message ID array.
///   - conversation: Conversation where the messages are located.
- (void)messagesDidRead:(NSArray <NSString *> *)messageIds
         inConversation:(JConversation *)conversation;

/// Callback for group message reads.
/// - Parameters:
///   - msgs: Dictionary whose key is messageId.
///   - conversation: Conversation where the messages are located.
- (void)groupMessagesDidRead:(NSDictionary <NSString *, JGroupMessageReadInfo *> *)msgs
              inConversation:(JConversation *)conversation;
@end

@protocol JMessageDestroyDelegate <NSObject>
/// Callback when the message destruction time is updated. This usually occurs in burn-after-reading scenarios.
/// - Parameters:
///   - messageId: Message ID.
///   - conversation: Conversation where the message is located.
///   - destroyTime: Updated destruction time.
- (void)messageDestroyTimeDidUpdate:(NSString *)messageId
                     inConversation:(JConversation *)conversation
                        destroyTime:(long long)destroyTime;
@end

@protocol JMessagePreprocessor <NSObject>
/// Callback for message encryption.
/// Callback timing: after the message is stored and before it is sent.
/// - Parameter content: Message content to send, serialized as NSData.
/// - Parameter conversation: Conversation where the message is located.
/// - Parameter contentType: Message type.
/// - Return: Processed message content.
- (NSData *)encryptMessageContent:(NSData *)content
                   inConversation:(JConversation *)conversation
                      contentType:(NSString *)contentType;

/// Callback for message decryption.
/// Callback timing: after receiving the message and before it is stored.
/// - Parameter content: Received message content in NSData format, not yet deserialized.
/// - Parameter conversation: Conversation where the message is located.
/// - Parameter contentType: Message type.
/// - Return: Processed message content.
- (NSData *)decryptMessageContent:(NSData *)content
                   inConversation:(JConversation *)conversation
                      contentType:(NSString *)contentType;

@end

@protocol JStreamMessageDelegate <NSObject>
/// Callback when a streaming message fragment is appended.
/// - Parameters:
///   - messageId: Streaming message ID.
///   - content: Appended fragment content. Developers can append this content to the end of JStreamTextMessage's content in the UI.
- (void)streamTextMessageDidAppend:(NSString *)messageId
                           content:(NSString *)content;
/// Callback when a streaming message completes.
/// - Parameter message: Completed streaming message. Developers can use messageId to find the corresponding streaming message in the UI and refresh it.
- (void)streamTextMessageDidComplete:(JMessage *)message;
@end

@protocol JMessageProtocol <NSObject>

/// Sends a message.
/// - Parameters:
///   - content: Message entity.
///   - conversation: Conversation.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (JMessage *)sendMessage:(JMessageContent *)content
           inConversation:(JConversation *)conversation
                  success:(void (^)(JMessage *message))successBlock
                    error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock;

/// Sends a message.
/// - Parameters:
///   - content: Message entity.
///   - messageOption: Message extension options.
///   - conversation: Conversation.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (JMessage *)sendMessage:(JMessageContent *)content
            messageOption:(JMessageOptions *)messageOption
           inConversation:(JConversation *)conversation
                  success:(void (^)(JMessage *message))successBlock
                    error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock;

/// Sends a media message by uploading the media first, then sending the message.
/// - Parameters:
///   - content: Media message entity.
///   - conversation: Conversation.
///   - progressBlock: Upload progress callback.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
///   - cancelBlock: User upload cancellation callback.
- (JMessage *)sendMediaMessage:(JMediaMessageContent *)content
                inConversation:(JConversation *)conversation
                      progress:(void (^)(int progress, JMessage *message))progressBlock
                       success:(void (^)(JMessage *message))successBlock
                         error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock
                        cancel:(void (^)(JMessage *message))cancelBlock;

/// Sends a media message by uploading the media first, then sending the message.
/// - Parameters:
///   - content: Media message entity.
///   - messageOption: Message extension options.
///   - conversation: Conversation.
///   - progressBlock: Upload progress callback.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
///   - cancelBlock: User upload cancellation callback.
- (JMessage *)sendMediaMessage:(JMediaMessageContent *)content
                 messageOption:(JMessageOptions *)messageOption
                inConversation:(JConversation *)conversation
                      progress:(void (^)(int progress, JMessage *message))progressBlock
                       success:(void (^)(JMessage *message))successBlock
                         error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock
                        cancel:(void (^)(JMessage *message))cancelBlock;

/// Resends a message after a send failure. If the message has already been sent successfully, the success callback is returned directly.
/// - Parameters:
///   - messsage: Message object.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (JMessage *)resend:(JMessage *)messsage
             success:(void (^)(JMessage *message))successBlock
               error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock;

/// Resends a media message after a send failure. If the message has already been sent successfully, the success callback is returned directly.
/// - Parameters:
///   - message: Message object.
///   - progressBlock: Upload progress callback.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
///   - cancelBlock: Cancellation callback.
- (JMessage *)resendMediaMessage:(JMessage *)message
                        progress:(void (^)(int progress, JMessage *message))progressBlock
                         success:(void (^)(JMessage *message))successBlock
                           error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock
                          cancel:(void (^)(JMessage *message))cancelBlock;
/// Saves a message.
/// - Parameters:
///   - content: Message entity.
///   - conversation: Conversation.
///   - direction: Message direction.
- (JMessage *)saveMessage:(JMessageContent *)content
           inConversation:(JConversation *)conversation
                direction:(JMessageDirection)direction;

/// Saves a message.
/// - Parameters:
///   - content: Message entity.
///   - messageOption: Message extension options.
///   - conversation: Conversation.
///   - direction: Message direction.
- (JMessage *)saveMessage:(JMessageContent *)content
            messageOption:(JMessageOptions *)messageOption
           inConversation:(JConversation *)conversation
                direction:(JMessageDirection)direction;

/// Gets messages from local storage. Results are sorted by message time in ascending order, oldest first.
/// Gets the message list.
/// - Parameters:
///   - conversation: Conversation object.
///   - count: Number of messages to fetch. Values over 100 are treated as 100.
///   - time: Message timestamp. Pass 0 for the current time.
///   - direction: Fetch direction.
- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation 
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction;

/// Gets messages from local storage. Results are sorted by message time in ascending order, oldest first.
/// - Parameters:
///   - conversation: Conversation object.
///   - count: Number of messages to fetch. Values over 100 are treated as 100.
///   - time: Message timestamp. Pass 0 for the current time.
///   - direction: Fetch direction.
///   - contentTypes: Message type list to fetch. Example for getting a message type: " [JTextMessage contentType] "
- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction
                            contentTypes:(NSArray <NSString *> *)contentTypes;

/// Searches messages from local storage.
/// - Parameters:
///   - count: Fetch count. Values over 100 are treated as 100.
///   - time: Message timestamp. Pass 0 for the current time.
///   - direction: Fetch direction.
///   - option: Search options.
- (NSArray<JMessage *> *)getMessages:(int)count
                                time:(long long)time
                           direction:(JPullDirection)direction
                         queryOption:(JQueryMessageOptions *)option;

/// Deletes messages in batches by local message unique number within the same conversation.
/// - Parameters:
///   - clientMsgNos: Local message unique number list.
///   - conversation: Conversation identifier.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)deleteMessagesByClientMsgNoList:(NSArray<NSNumber *> *)clientMsgNos
                           conversation:(JConversation *)conversation
                                success:(void (^)(void))successBlock
                                  error:(void (^)(JErrorCode errorCode))errorBlock;

/// Deletes messages in batches by message ID within the same conversation.
/// - Parameters:
///   - messageIds: Message ID list.
///   - conversation: Conversation identifier.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)deleteMessagesByMessageIds:(NSArray<NSString *> *)messageIds
                      conversation:(JConversation *)conversation
                           success:(void (^)(void))successBlock
                             error:(void (^)(JErrorCode errorCode))errorBlock;

/// Deletes messages in batches by local message unique number within the same conversation.
/// - Parameters:
///   - clientMsgNos: Local message unique number list.
///   - conversation: Conversation identifier.
///   - forAllUsers: Whether to delete the messages for all users in the conversation.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)deleteMessagesByClientMsgNoList:(NSArray<NSNumber *> *)clientMsgNos
                           conversation:(JConversation *)conversation
                            forAllUsers:(BOOL)forAllUsers
                                success:(void (^)(void))successBlock
                                  error:(void (^)(JErrorCode))errorBlock;

/// Deletes messages in batches by message ID within the same conversation.
/// - Parameters:
///   - messageIds: Message ID list.
///   - conversation: Conversation identifier.
///   - forAllUsers: Whether to delete the messages for all users in the conversation.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)deleteMessagesByMessageIds:(NSArray<NSString *> *)messageIds
                      conversation:(JConversation *)conversation
                       forAllUsers:(BOOL)forAllUsers
                           success:(void (^)(void))successBlock
                             error:(void (^)(JErrorCode errorCode))errorBlock;

/// Recalls a message. After recall, no one in the conversation can see the original message.
/// - Parameters:
///   - messageId: ID of the message to recall.
///   - extras: Extension information. Keys and values must both be NSString.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)recallMessage:(NSString *)messageId
               extras:(NSDictionary <NSString *, NSString *> *)extras
              success:(void (^)(JMessage *message))successBlock
                error:(void (^)(JErrorCode errorCode))errorBlock;

/// Clears all messages in a conversation before the specified time. Pass 0 for startTime to use the current time.
/// - Parameters:
///   - conversation: Conversation identifier.
///   - startTime: Start time. Pass 0 for the current time.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)clearMessagesIn:(JConversation *)conversation
              startTime:(long long)startTime
                success:(void (^)(void))successBlock
                  error:(void (^)(JErrorCode errorCode))errorBlock;

/// Clears all messages in a conversation before the specified time. Pass 0 for startTime to use the current time.
/// - Parameters:
///   - conversation: Conversation identifier.
///   - startTime: Start time. Pass 0 for the current time.
///   - forAllUsers: Whether to clear messages for all users in the conversation.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)clearMessagesIn:(JConversation *)conversation
              startTime:(long long)startTime
            forAllUsers:(BOOL)forAllUsers
                success:(void (^)(void))successBlock
                  error:(void (^)(JErrorCode errorCode))errorBlock;

/// Physically deletes all messages before the specified time. Only local messages are deleted, which can free local storage space.
/// - Parameters:
///   - timestamp: Timestamp. Pass 0 for the current time.
///   - conversationTypes: Conversation type list to delete. Pass nil to delete messages of all conversation types.
- (void)purgeMessagesBefore:(long long)timestamp
          conversationTypes:(NSArray<NSNumber *> *)conversationTypes;

/// Gets the corresponding local messages by messageId array.
/// - Parameter messageIds: messageId array.
- (NSArray<JMessage *> *)getMessagesByMessageIds:(NSArray<NSString *> *)messageIds;

/// Gets the corresponding messages by messageId array. Local messages are preferred when available; otherwise, messages are fetched from the server.
/// - Parameters:
///   - messageIds: messageId array.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)getMessagesByMessageIds:(NSArray<NSString *> *)messageIds
                 inConversation:(JConversation *)conversation
                        success:(void (^)(NSArray <JMessage *> *messages))successBlock
                          error:(void (^)(JErrorCode errorCode))errorBlock;

/// Gets the corresponding local messages by clientMsgNo array.
/// - Parameter clientMsgNos: clientMsgNo array.
- (NSArray<JMessage *> *)getMessagesByClientMsgNos:(NSArray<NSNumber *> *)clientMsgNos;

/// Registers a custom message. The SDK cannot parse unregistered custom messages correctly.
/// - Parameter messageClass: Custom message class. It must inherit from JMessageContent.
- (void)registerContentType:(Class)messageClass;

- (void)addDelegate:(id<JMessageDelegate>)delegate;

- (void)addSyncDelegate:(id<JMessageSyncDelegate>)delegate;

- (void)addReadReceiptDelegate:(id<JMessageReadReceiptDelegate>)delegate;

- (void)addDestroyDelegate:(id<JMessageDestroyDelegate>)delegate;

- (void)addStreamMessageDelegate:(id<JStreamMessageDelegate>)delegate;

- (void)setPreprocessor:(id<JMessagePreprocessor>)preprocessor;

- (void)setMessageUploadProvider:(id<JMessageUploadProvider>)uploadProvider;

/// Fetches historical messages from the remote server. Results are sorted by message time in ascending order, oldest first.
/// - Parameters:
///   - conversation: Conversation object.
///   - startTime: Message timestamp. Pass 0 for the current time.
///   - count: Fetch count. Values over 100 are treated as 100.
///   - direction: Fetch direction.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)getRemoteMessagesFrom:(JConversation *)conversation
                    startTime:(long long)startTime
                        count:(int)count
                    direction:(JPullDirection)direction
                      success:(void (^)(NSArray *messages, BOOL isFinished))successBlock
                        error:(void (^)(JErrorCode code))errorBlock;

/// Gets messages. Results are sorted by message time in ascending order, oldest first. When messages are missing and the network has issues, locally cached messages are returned.
/// - Parameters:
///   - conversation: Conversation object.
///   - direction: Fetch direction.
///   - option: Message fetch options.
///   - completeBlock: messages: Message list; timestamp: Message timestamp that can be used to fetch the next batch; hasMore: Whether more messages are available;
///                    code: Error code. When code is not 0, locally cached messages are returned in messages if local cache exists.
- (void)getMessages:(JConversation *)conversation
          direction:(JPullDirection)direction
             option:(JGetMessageOptions *)option
           complete:(void (^)(NSArray <JMessage *> *messages, long long timestamp, BOOL hasMore, JErrorCode code))completeBlock;

/// Sends read receipts.
/// - Parameters:
///   - messageIds: Message ID list that needs read receipts.
///   - conversation: Conversation where the messages are located.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)sendReadReceipt:(NSArray <NSString *> *)messageIds
         inConversation:(JConversation *)conversation
                success:(void (^)(void))successBlock
                  error:(void (^)(JErrorCode code))errorBlock;

/// Gets group message read status.
/// - Parameters:
///   - messageId: Group message ID to query.
///   - conversation: Conversation where the message is located.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)getGroupMessageReadInfoDetail:(NSString *)messageId
                       inConversation:(JConversation *)conversation
                              success:(void (^)(JGroupMessageReadInfoDetail * detail))successBlock
                                error:(void (^)(JErrorCode code))errorBlock;

/// Gets the read time for a one-to-one message. Use getGroupMessageReadInfoDetail:inConversation:success:error: for group message read status.
/// - Parameter clientMsgNo: Local message unique number.
- (long long)getMessageReadTime:(long long)clientMsgNo;

/// Gets the merged message list.
/// - Parameters:
///   - messageId: Merged message ID.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)getMergedMessageList:(NSString *)messageId
                     success:(void (^)(NSArray<JMessage *> *mergedMessages))successBlock
                       error:(void (^)(JErrorCode code))errorBlock;

/// Searches local messages.
/// - Parameters:
///   - searchContent: Search content.
///   - conversation: Conversation to query.
///   - count: Fetch count. Values over 100 are treated as 100.
///   - time: Message timestamp. Pass 0 for the current time.
///   - direction: Query direction.
///   - contentTypes: Content types. Pass empty to return all types.
- (NSArray <JMessage *> *)searchMessagesWithContent:(NSString *)searchContent
                                     inConversation:(JConversation *)conversation
                                             count:(int)count
                                              time:(long long)time
                                         direction:(JPullDirection)direction
                                      contentTypes:(NSArray<NSString *> *)contentTypes;

/// Searches local messages.
/// - Parameters:
///   - searchContent: Search content.
///   - conversation: Conversation to query.
///   - count: Fetch count. Values over 100 are treated as 100.
///   - time: Message timestamp. Pass 0 for the current time.
///   - direction: Query direction.
///   - contentTypes: Content types. Pass empty to return all types.
///   - senderUserIds: Message sender ID list.
- (NSArray <JMessage *> *)searchMessagesWithContent:(NSString *)searchContent
                                     inConversation:(JConversation *)conversation
                                             count:(int)count
                                              time:(long long)time
                                         direction:(JPullDirection)direction
                                      contentTypes:(NSArray<NSString *> *)contentTypes
                                      senderUserIds:(NSArray <NSString *> *)senderUserIds;

/// Searches conversations by message keywords.
/// - Parameters:
///   - option: Search options.
///   - completeBlock: Result callback.
- (void)searchConversationsWithMessageContent:(JQueryMessageOptions *)option
                                     complete:(void (^)(NSArray<JSearchConversationsResult*> *result))completeBlock;

/// Gets mention messages in the specified conversation.
/// - Parameters:
///   - conversation: Conversation identifier.
///   - count: Fetch count. Values over 100 are treated as 100.
///   - time: Message timestamp. Pass 0 for the current time.
///   - direction: Query direction.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)getMentionMessages:(JConversation *)conversation
                     count:(int)count
                      time:(long long)time
                 direction:(JPullDirection)direction
                   success:(void (^)(NSArray<JMessage *> *messages, BOOL isFinished))successBlock
                     error:(void (^)(JErrorCode code))errorBlock;

/// Gets unread mention messages in the specified conversation.
/// - Parameters:
///   - conversation: Conversation identifier.
///   - count: Fetch count. Values over 100 are treated as 100.
///   - time: Message timestamp. Pass 0 for the current time.
///   - direction: Query direction.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)getUnreadMentionMessages:(JConversation *)conversation
                           count:(int)count
                            time:(long long)time
                       direction:(JPullDirection)direction
                         success:(void (^)(NSArray<JMessage *> *messages, BOOL isFinished))successBlock
                           error:(void (^)(JErrorCode code))errorBlock;

/// Sets top.
/// - Parameters:
///   - isTop: YES to pin, NO to unpin.
///   - messageId: Message ID.
///   - conversation: Conversation identifier.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)setTop:(BOOL)isTop
     messageId:(NSString *)messageId
  conversation:(JConversation *)conversation
       success:(void (^)(void))successBlock
         error:(void (^)(JErrorCode code))errorBlock;

/// Gets the pinned message.
/// - Parameters:
///   - conversation: Conversation identifier.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)getTopMessage:(JConversation *)conversation
              success:(void (^)(JMessage *message, JUserInfo *userInfo, long long timestamp))successBlock
                error:(void (^)(JErrorCode code))errorBlock;

/// Adds message favorites.
/// - Parameters:
///   - messageIdList: Message ID list to favorite.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)addFavorite:(NSArray <NSString *> *)messageIdList
            success:(void (^)(void))successBlock
              error:(void (^)(JErrorCode code))errorBlock;

/// Removes message favorites.
/// - Parameters:
///   - messageIdList: Message ID list to remove.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)removeFavorite:(NSArray <NSString *> *)messageIdList
               success:(void (^)(void))successBlock
                 error:(void (^)(JErrorCode code))errorBlock;

/// Gets favorite messages.
/// - Parameters:
///   - option: Query parameters.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)getFavorite:(JGetFavoriteMessageOption *)option
            success:(void (^)(NSArray <JFavoriteMessage *> *messageList, NSString *offset))successBlock
              error:(void (^)(JErrorCode code))errorBlock;

/// Gets the first unread message in the conversation.
/// - Parameters:
///   - conversation: Conversation identifier.
///   - successBlock: Success callback. Returns nil if there are no unread messages.
///   - errorBlock: Failure callback.
- (void)getFirstUnreadMessage:(JConversation *)conversation
                      success:(void (^)(JMessage *message))successBlock
                        error:(void (^)(JErrorCode code))errorBlock;

/// Gets a message local attribute.
/// - Parameter messageId: Message ID.
- (NSString *)getLocalAttributeByMessageId:(NSString *)messageId;


/// Sets a message local attribute. It only takes effect locally and is not synchronized to the remote server.
/// - Parameters:
///   - attribute: Local attribute. JSON can be used for complex business scenarios.
///   - messageId: Message ID.
- (void)setLocalAttribute:(NSString *)attribute forMessage:(NSString *)messageId;

/// Gets a message local attribute.
/// - Parameter clientMsgNo: Local message unique number.
- (NSString *)getLocalAttributeByClientMsgNo:(long long)clientMsgNo;

/// Sets a message local attribute. It only takes effect locally and is not synchronized to the remote server.
/// - Parameters:
///   - attribute: Local attribute. JSON can be used for complex business scenarios.
///   - clientMsgNo: Local message unique number.
- (void)setLocalAttribute:(NSString *)attribute forClientMsgNo:(long long)clientMsgNo;

/// Adds a message reaction.
/// - Parameters:
///   - messageId: Message ID.
///   - conversation: Conversation the message belongs to.
///   - reactionId: Reaction ID.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)addMessageReaction:(NSString *)messageId
              conversation:(JConversation *)conversation
                reactionId:(NSString *)reactionId
                   success:(void (^)(void))successBlock
                     error:(void (^)(JErrorCode code))errorBlock;

/// Deletes a message reaction.
/// - Parameters:
///   - messageId: Message ID.
///   - conversation: Conversation the message belongs to.
///   - reactionId: Reaction ID.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)removeMessageReaction:(NSString *)messageId
                 conversation:(JConversation *)conversation
                   reactionId:(NSString *)reactionId
                      success:(void (^)(void))successBlock
                        error:(void (^)(JErrorCode code))errorBlock;

/// Gets message reactions in batches. Messages must belong to the same conversation.
/// - Parameters:
///   - messageIdList: Message ID list.
///   - conversation: Conversation the messages belong to.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)getMessagesReaction:(NSArray <NSString *> *)messageIdList
               conversation:(JConversation *)conversation
                    success:(void (^)(NSArray <JMessageReaction *> *reactionList))successBlock
                      error:(void (^)(JErrorCode code))errorBlock;

/// Gets cached message reactions. Cached data may not be the latest version and can be used for immediate UI rendering to optimize user experience.
/// - Parameter messageIdList: Message ID list.
- (NSArray <JMessageReaction *> *)getCachedMessagesReaction:(NSArray <NSString *> *)messageIdList;

/// Broadcasts a message. Sends the message to multiple conversations at the same time, without affecting conversation ordering on the sender side.
/// - Parameters:
///   - content: Message entity.
///   - conversations: Target conversation list.
///   - progressBlock: Progress callback.
///   - completeBlock: Completion callback.
- (void)broadcastMessage:(JMessageContent *)content
         inConversations:(NSArray <JConversation *> *)conversations
                progress:(void (^)(JMessage *sentMessage, JErrorCode code, int processCount, int totalCount))progressBlock
                complete:(void (^)(void))completeBlock;

/// Downloads the media file in a message.
/// - Parameters:
///   - messageId: Message ID.
///   - progressBlock: Progress update callback [progress: Current download progress, 0 <= progress <= 100].
///   - successBlock: Success callback [message: The downloaded local path has been written to message].
///   - errorBlock: Failure callback [errorCode: Error code].
- (void)downloadMediaMessage:(NSString *)messageId
                    progress:(void (^)(JMessage *message, int progress))progressBlock
                     success:(void (^)(JMessage *message))successBlock
                       error:(void (^)(JErrorCode errorCode))errorBlock;

/// Cancels message download.
/// - Parameter messageId: Message ID.
- (void)cancelDownloadMediaMessage:(NSString *)messageId;

/// Updates a message.
/// - Parameters:
///   - content: Updated message entity.
///   - messageId: Message ID.
///   - conversation: Conversation.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)updateMessage:(JMessageContent *)content
            messageId:(NSString *)messageId
       inConversation:(JConversation *)conversation
              success:(void (^)(JMessage *message))successBlock
                error:(void (^)(JErrorCode errorCode))errorBlock;

/// Sets global message mute.
/// - Parameters:
///   - isMute: Whether to mute.
///   - periods: Mute time periods. Empty means muted all day.
///   - completeBlock: Result callback.
- (void)setMute:(BOOL)isMute
        periods:(NSArray <JTimePeriod *> *)periods
       complete:(void (^)(JErrorCode errorCode))completeBlock;

/// Gets the global message mute configuration.
/// - Parameter completeBlock: Result callback [errorCode: Error code, 0 means success; isMute: Whether muted; timezone: Time zone; periods: Mute time periods, empty means muted all day].
- (void)getMuteStatus:(void (^)(JErrorCode errorCode, BOOL isMute, NSString *timezone, NSArray <JTimePeriod *> *periods))completeBlock;

/// Uploads an image.
/// - Parameters:
///   - image: Image.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)uploadImage:(UIImage *)image
            success:(void (^)(NSString * url))successBlock
              error:(void (^)(JErrorCode code))errorBlock;

@end
