//
//  JConversation.h
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/*!
 Conversation type
 */
typedef NS_ENUM(NSUInteger, JConversationType) {
    
    JConversationTypeUnknown = 0,
    /*!
     One-to-one chat
     */
    JConversationTypePrivate = 1,

    /*!
     Group
     */
    JConversationTypeGroup = 2,

    /*!
     Chatroom
     */
    JConversationTypeChatroom = 3,

    /*!
     System conversation
     */
    JConversationTypeSystem = 4,
    
    /*!
     Official account
     */
    JConversationTypePublicService = 7,
    
    /*!
     Status change
     */
    JConversationTypeSubStatus = 8,
    
    /*!
     End-to-end encrypted one-to-one chat
     */
    JConversationTypePrivateE2EE = 11
};


@interface JConversation : NSObject <NSCopying>

// Conversation id
@property(nonatomic, copy) NSString *conversationId;
// Conversation type
@property(nonatomic, assign) JConversationType conversationType;

- (instancetype)initWithConversationType:(JConversationType)type
                          conversationId:(NSString *)conversationId;

// Conversation subchannel, empty by default.
@property(nonatomic, copy) NSString *subChannel;

@end

NS_ASSUME_NONNULL_END
