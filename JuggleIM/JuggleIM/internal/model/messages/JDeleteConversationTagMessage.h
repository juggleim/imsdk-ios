//
//  JDeleteConversationTagMessage.h
//  JuggleIM
//
//  Created by Fei Li on 2026/4/23.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JDeleteConversationTagMessage : JMessageContent

@property (nonatomic, copy) NSArray <NSString *> *tagIdList;

@end

NS_ASSUME_NONNULL_END
