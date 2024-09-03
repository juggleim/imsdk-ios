//
//  JMarkUnreadMessage.h
//  JuggleIM
//
//  Created by Fei Li on 2024/8/23.
//

#import <JuggleIM/JuggleIM.h>
#import "JConversation.h"

NS_ASSUME_NONNULL_BEGIN

@interface JMarkUnreadMessage : JMessageContent

@property (nonatomic, copy) NSArray <JConversation *> *conversations;

@end

NS_ASSUME_NONNULL_END
