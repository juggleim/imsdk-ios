//
//  JVoiceMessage.h
// JuggleIM
//
//  Created by Nathan on 2023/12/26.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JVoiceMessage : JMediaMessageContent
/// Voice message audio duration, in seconds
@property (nonatomic, assign) long duration;
/// Extra field
@property (nonatomic, copy) NSString *extra;
@end

NS_ASSUME_NONNULL_END
