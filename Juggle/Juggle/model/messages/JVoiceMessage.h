//
//  JVoiceMessage.h
//  Juggle
//
//  Created by Nathan on 2023/12/26.
//

#import <Juggle/Juggle.h>

NS_ASSUME_NONNULL_BEGIN

@interface JVoiceMessage : JMessageContent
@property (nonatomic, copy) NSString *url;
@property (nonatomic, assign) long duration;
@end

NS_ASSUME_NONNULL_END
