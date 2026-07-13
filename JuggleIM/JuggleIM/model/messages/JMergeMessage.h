//
//  JMergeMessage.h
// JuggleIM
//
//  Created by Nathan on 2024/3/19.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JMergeMessagePreviewUnit : NSObject
@property (nonatomic, copy) NSString *previewContent;
@property (nonatomic, strong) JUserInfo *sender;
@end

/// Merged message
@interface JMergeMessage : JMessageContent
/// Conversation identifier of the merged messages
@property (nonatomic, strong) JConversation *conversation;
/// Title
@property (nonatomic, copy, readonly) NSString *title;
/// List of all merged message ids. Cannot exceed 100 entries (all messages must come from the same conversation)
@property (nonatomic, copy, readonly) NSArray <NSString *> *messageIdList;
/// List of merged messages used for preview in the message bubble. Cannot exceed 10 entries
@property (nonatomic, copy, readonly) NSArray <JMergeMessagePreviewUnit *> *previewList;
/// Extra field
@property (nonatomic, copy) NSString *extra;
/// Merged message id
@property (nonatomic, copy) NSString *containerMsgId;


/// Constructor
/// - Parameters:
///   - title: Title
///   - conversation: Conversation identifier of the merged messages
///   - messageIdList: Merged message id list (the full merged message set; all messages must come from the same conversation. messageIdList.count must be less than or equal to 100, and extra entries will be truncated)
///   - previewList: Message preview list (used for preview in the message bubble. previewList.count must be less than or equal to 10, and extra entries will be truncated)
- (instancetype)initWithTitle:(NSString *)title
                 conversation:(JConversation *)conversation
                MessageIdList:(NSArray <NSString *> *)messageIdList
                  previewList:(NSArray <JMergeMessagePreviewUnit *> *)previewList;
@end

NS_ASSUME_NONNULL_END
