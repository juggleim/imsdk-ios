//
//  JCreateConversationTagMessage.h
//  JuggleIM
//
//  Created by Fei Li on 2026/4/28.
//

#import <JuggleIM/JuggleIM.h>
#import "JConversationTagInfoContainer.h"

NS_ASSUME_NONNULL_BEGIN

@interface JCreateConversationTagMessage : JMessageContent

@property (nonatomic, copy) NSArray <JConversationTagInfoContainer *> *tagList;

@end

NS_ASSUME_NONNULL_END
