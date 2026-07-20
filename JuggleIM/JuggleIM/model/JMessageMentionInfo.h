//
//  JMentionInfo.h
// JuggleIM
//
//  Created by Nathan on 2024/4/19.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JUserInfo.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, JMentionType) {
    
    JMentionTypeDefault = 0,
    /*!
     mention everyone
     */
    JMentionTypeAll = 1,

    /*!
     mention specified users
     */
    JMentionTypeSomeOne = 2,

    /*!
     mention everyone and specified users
     */
    JMentionTypeAllAndSomeOne = 3,
};

@interface JMessageMentionInfo : NSObject

@property (nonatomic, assign) JMentionType type;
@property (nonatomic, copy) NSArray <JUserInfo *> *targetUsers;

- (NSString *)encodeToJson;

+ (instancetype)decodeFromJson:(NSString *)json;

@end

NS_ASSUME_NONNULL_END
