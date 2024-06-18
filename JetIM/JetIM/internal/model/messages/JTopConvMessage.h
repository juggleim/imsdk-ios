//
//  JTopConvMessage.h
//  JetIM
//
//  Created by 郑开 on 2024/5/21.
//

#import <JetIM/JetIM.h>
#import "JConcreteConversationInfo.h"

NS_ASSUME_NONNULL_BEGIN

@interface JTopConvMessage : JMessageContent

@property (nonatomic, copy) NSArray <JConcreteConversationInfo *> * conversations;



@end

NS_ASSUME_NONNULL_END
