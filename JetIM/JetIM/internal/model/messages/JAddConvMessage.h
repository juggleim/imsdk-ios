//
//  JAddConvMessage.h
//  JetIM
//
//  Created by Nathan on 2024/6/12.
//

#import <JetIM/JetIM.h>
#import "JConcreteConversationInfo.h"

NS_ASSUME_NONNULL_BEGIN

@interface JAddConvMessage : JMessageContent
@property (nonatomic, strong) JConcreteConversationInfo *conversationInfo;
@end

NS_ASSUME_NONNULL_END
