//
//  JRecallCmdMessage.h
// JuggleIM
//
//  Created by Nathan on 2024/2/2.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JRecallCmdMessage : JMessageContent

@property (nonatomic, copy) NSString *originalMessageId;
@property (nonatomic, assign) long long originalMessageTime;
@property (nonatomic, copy) NSDictionary * extra;

@end

NS_ASSUME_NONNULL_END
