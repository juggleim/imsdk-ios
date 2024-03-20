//
//  JDeleteConvMessage.h
//  JetIM
//
//  Created by Nathan on 2024/2/23.
//

#import <JetIM/JetIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JDeleteConvMessage : JMessageContent
@property (nonatomic, copy) NSArray <JConversation *> *conversations;
@end

NS_ASSUME_NONNULL_END
