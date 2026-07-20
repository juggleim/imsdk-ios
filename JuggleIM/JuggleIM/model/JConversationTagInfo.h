//
//  JConversationTagInfo.h
//  JuggleIM
//
//  Created by Fei Li on 2026/4/23.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/*!
 Conversation tag type
 */
typedef NS_ENUM(NSUInteger, JConversationTagType) {
    /*!
     User type
     */
    JConversationTagTypeUser = 0,
    /*!
     System type
     */
    JConversationTagTypeSystem = 1,
    /*!
     Global type
     */
    JConversationTagTypeGlobal = 2
};

@interface JConversationTagInfo : NSObject

@property (nonatomic, copy) NSString *tagId;
@property (nonatomic, copy) NSString *name;
@property (nonatomic, assign) JConversationTagType type;

@end

NS_ASSUME_NONNULL_END
