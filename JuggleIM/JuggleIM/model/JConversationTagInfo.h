//
//  JConversationTagInfo.h
//  JuggleIM
//
//  Created by Fei Li on 2026/4/23.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/*!
 会话标签类型
 */
typedef NS_ENUM(NSUInteger, JConversationTagType) {
    /*!
     用户类型
     */
    JConversationTagTypeUser = 0,
    /*!
     系统类型
     */
    JConversationTagTypeSystem = 1,
    /*!
     全局类型
     */
    JConversationTagTypeGlobal = 2
};

@interface JConversationTagInfo : NSObject

@property (nonatomic, copy) NSString *tagId;
@property (nonatomic, copy) NSString *name;
@property (nonatomic, assign) JConversationTagType type;

@end

NS_ASSUME_NONNULL_END
