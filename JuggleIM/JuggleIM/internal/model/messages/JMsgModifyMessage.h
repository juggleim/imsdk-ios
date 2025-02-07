//
//  JMsgModifyMessage.h
//  JuggleIM
//
//  Created by Fei Li on 2025/1/8.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JMsgModifyMessage : JMessageContent
@property (nonatomic, copy) NSString *originalMessageId;
@property (nonatomic, assign) long long originalMessageTime;
@property (nonatomic, copy) NSString *messageType;
@property (nonatomic, strong) JMessageContent *messageContent;
@end

NS_ASSUME_NONNULL_END
