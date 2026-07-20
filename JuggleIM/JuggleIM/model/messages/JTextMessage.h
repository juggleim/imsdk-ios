//
//  JTextMessage.h
// JuggleIM
//
//  Created by Nathan on 2023/12/2.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JTextMessage : JMessageContent

/*!
 Text message content
 */
@property (nonatomic, copy) NSString *content;

/*!
 Extra field
 */
@property (nonatomic, copy) NSString *extra;

- (instancetype)initWithContent:(NSString *)content;

@end

NS_ASSUME_NONNULL_END
