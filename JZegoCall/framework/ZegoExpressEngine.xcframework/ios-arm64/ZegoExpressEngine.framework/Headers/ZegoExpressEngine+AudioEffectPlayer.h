//
//  ZegoExpressEngine+AudioEffectPlayer.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (AudioEffectPlayer)

/// Creates a audio effect player instance.
///
/// Available since: 1.16.0
/// Description: Creates a audio effect player instance.
/// Use cases: When you need to play short sound effects, such as applause, cheers, etc., you can use audioEffectPlayer to achieve.
/// When to call: It can be called after [createEngine].
/// Restrictions: Currently, a maximum of 1 instances can be created, after which it will return nil.
/// Related APIs: [destroyAudioEffectPlayer].
///
/// @return audio effect player instance, nil will be returned when the maximum number is exceeded.
- (nullable ZegoAudioEffectPlayer *)createAudioEffectPlayer;

/// Destroys a audio effect player instance.
///
/// Available since: 1.16.0
/// Description: Destroys the specified audio effect player instance.
/// When to call: It can be called after [createAudioEffectPlayer].
/// Restrictions: None.
/// Related APIs: [createAudioEffectPlayer].
///
/// @param audioEffectPlayer The audio effect player instance object to be destroyed.
- (void)destroyAudioEffectPlayer:(ZegoAudioEffectPlayer *)audioEffectPlayer;

@end

NS_ASSUME_NONNULL_END
