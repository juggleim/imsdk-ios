//
//  JMessageReaction.h
//  JuggleIM
//
//  Created by Fei Li on 2025/1/13.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JMessageReactionItem.h>

NS_ASSUME_NONNULL_BEGIN

@interface JMessageReaction : NSObject

@property (nonatomic, copy) NSString *messageId;
@property (nonatomic, copy) NSArray <JMessageReactionItem *> *itemList;

@end

NS_ASSUME_NONNULL_END
