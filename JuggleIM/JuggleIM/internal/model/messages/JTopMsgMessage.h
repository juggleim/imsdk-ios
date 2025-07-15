//
//  JTopMsgMessage.h
//  JuggleIM
//
//  Created by Fei Li on 2025/7/15.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JTopMsgMessage : JMessageContent
@property (nonatomic, assign) BOOL isTop;
@property (nonatomic, copy) NSString *messageId;
@end

NS_ASSUME_NONNULL_END
