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
 Message type
 */
@property (nonatomic, copy) NSString *messageType;

/*!
 Message content
 */
@property (nonatomic, copy) NSString *content;

/// Message attributes
@property (nonatomic, assign) JMessageFlag flags;

@end

NS_ASSUME_NONNULL_END
