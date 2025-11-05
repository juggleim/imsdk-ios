//
//  JConversation.h
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/*!
 会话类型
 */
typedef NS_ENUM(NSUInteger, JConversationType) {
    
    JConversationTypeUnknown = 0,
    /*!
     单聊
     */
    JConversationTypePrivate = 1,

    /*!
     群组
     */
    JConversationTypeGroup = 2,

    /*!
     聊天室
     */
    JConversationTypeChatroom = 3,

    /*!
     系统会话
     */
    JConversationTypeSystem = 4,
    
    /*!
     公众号
     */
    JConversationTypePublicService = 7
};


@interface JConversation : NSObject

// 会话 id
@property(nonatomic, copy) NSString *conversationId;
// 会话类型
@property(nonatomic, assign) JConversationType conversationType;

- (instancetype)initWithConversationType:(JConversationType)type
                          conversationId:(NSString *)conversationId;

@end

NS_ASSUME_NONNULL_END
