//
//  JReadNtfMessage.h
// JuggleIM
//
//  Created by Nathan on 2024/3/4.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JReadNtfMessage : JMessageContent
@property (nonatomic, copy) NSArray <NSString *> *messageIds;
@end

NS_ASSUME_NONNULL_END
