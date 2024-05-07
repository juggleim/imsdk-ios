//
//  JRecallCmdMessage.h
//  JetIM
//
//  Created by Nathan on 2024/2/2.
//

#import <JetIM/JetIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JRecallCmdMessage : JMessageContent

@property (nonatomic, copy) NSString *originalMessageId;
@property (nonatomic, assign) long long originalMessageTime;

@end

NS_ASSUME_NONNULL_END
