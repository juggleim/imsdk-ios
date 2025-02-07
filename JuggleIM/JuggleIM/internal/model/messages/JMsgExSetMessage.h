//
//  JMsgExSetMessage.h
//  JuggleIM
//
//  Created by Fei Li on 2025/1/13.
//

#import <JuggleIM/JuggleIM.h>
#import <JuggleIM/JMessageReactionItem.h>

NS_ASSUME_NONNULL_BEGIN

@interface JMsgExSetMessage : JMessageContent

@property (nonatomic, copy) NSString *originalMessageId;
@property (nonatomic, copy) NSArray <JMessageReactionItem *> *addItemList;
@property (nonatomic, copy) NSArray <JMessageReactionItem *> *removeItemList;

@end

NS_ASSUME_NONNULL_END
