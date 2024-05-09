//
//  JVoiceMessage.h
//  JetIM
//
//  Created by Nathan on 2023/12/26.
//

#import <JetIM/JetIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JVoiceMessage : JMediaMessageContent
/// 语音消息音频时长，单位: 秒
@property (nonatomic, assign) long duration;
/// 扩展字段
@property (nonatomic, copy) NSString *extra;
@end

NS_ASSUME_NONNULL_END
