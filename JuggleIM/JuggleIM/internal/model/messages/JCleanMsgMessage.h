//
//  JCleanMsgMessage.h
// JuggleIM
//
//  Created by Zheng Kai on 2024/5/21.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JCleanMsgMessage : JMessageContent

@property (nonatomic, assign) long long cleanTime;

@property (nonatomic, copy) NSString * senderId;
@end

NS_ASSUME_NONNULL_END
