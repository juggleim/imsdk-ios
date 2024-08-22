//
//  JLogCommandMessage.h
// JuggleIM
//
//  Created by Nathan on 2024/5/21.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JLogCommandMessage : JMessageContent
@property (nonatomic, assign) long long startTime;
@property (nonatomic, assign) long long endTime;
@end

NS_ASSUME_NONNULL_END
