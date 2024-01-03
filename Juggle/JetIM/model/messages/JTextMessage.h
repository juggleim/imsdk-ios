//
//  JTextMessage.h
//  JetIM
//
//  Created by Nathan on 2023/12/2.
//

#import <JetIM/JetIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JTextMessage : JMessageContent

/*!
 文本消息的内容
 */
@property (nonatomic, copy) NSString *content;

- (instancetype)initWithContent:(NSString *)content;

@end

NS_ASSUME_NONNULL_END
