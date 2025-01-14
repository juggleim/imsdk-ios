//
//  JMessageReactionItem.h
//  JuggleIM
//
//  Created by Fei Li on 2025/1/13.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JUserInfo.h>

NS_ASSUME_NONNULL_BEGIN

@interface JMessageReactionItem : NSObject

@property (nonatomic, copy) NSString *reactionId;
@property (nonatomic, copy) NSArray <JUserInfo *> *userInfoList;

@end

NS_ASSUME_NONNULL_END
