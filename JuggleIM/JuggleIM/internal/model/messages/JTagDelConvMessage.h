//
//  JTagDelConvMessage.h
//  JuggleIM
//
//  Created by Fei Li on 2025/2/20.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JTagDelConvMessage : JMessageContent
@property (nonatomic, copy) NSString *tagId;
@property (nonatomic, copy) NSString *tagName;
@property (nonatomic, copy) NSArray <JConversation *> *conversations;
@end

NS_ASSUME_NONNULL_END
