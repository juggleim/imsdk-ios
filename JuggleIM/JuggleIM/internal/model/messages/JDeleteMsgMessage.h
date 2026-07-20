//
//  JDeleteMsgMessage.h
// JuggleIM
//
//  Created by Zheng Kai on 2024/5/21.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JDeleteMsgMessage : JMessageContent

@property (nonatomic, copy) NSArray <NSString *> * msgIdList;

@end

NS_ASSUME_NONNULL_END
