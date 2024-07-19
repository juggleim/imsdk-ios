//
//  JMentionInfo.h
// JuggleIM
//
//  Created by Nathan on 2024/4/19.
//

#import <Foundation/Foundation.h>
#import "JUserInfo.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, JMentionType) {
    
    JMentionTypeDefault = 0,
    /*!
     @ 所有人
     */
    JMentionTypeAll = 1,

    /*!
     @ 指定用户
     */
    JMentionTypeSomeOne = 2,

    /*!
     @ 所有人和指定用户
     */
    JMentionTypeAllAndSomeOne = 3,
};

@interface JMessageMentionInfo : NSObject

@property (nonatomic, assign) JMentionType type;
@property (nonatomic, strong) NSArray <JUserInfo *> *targetUsers;

- (NSString *)encodeToJson;

+ (instancetype)decodeFromJson:(NSString *)json;

@end

NS_ASSUME_NONNULL_END
