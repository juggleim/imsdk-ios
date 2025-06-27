//
//  JUnknownMessage.h
//  JuggleIM
//
//  Created by Fei Li on 2025/6/10.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JUnknownMessage : JMessageContent

/*!
 消息的类型
 */
@property (nonatomic, copy) NSString *messageType;

/*!
 消息的内容
 */
@property (nonatomic, copy) NSString *content;

@end

NS_ASSUME_NONNULL_END
