//
//  ZegoExpressEngine+AudioVADClient.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (AudioVADClient)

/// Create a audio vad client instance.
///
/// Available since: 2.21.0
/// Description: Create a audio vad client instance.
/// Use case: Can be used to detect if an audio packet contains speech.
/// When to call: It can be called after [createEngine].
/// Restrictions: Each stream should correspond to a detected audio packet class object, or reset the existing object and apply it to a different stream.
///
/// @return audio vad client instance.
- (nullable ZegoAudioVADClient *)createAudioVADClient;

/// Destroys a audio vad client instance.
///
/// Available since: 2.21.0
/// Description: Destroys a audio vad client instance.
/// When to call: It can be called after [createEngine].
///
/// @param client The audio vad client instance object to be destroyed.
- (void)destroyAudioVADClient:(ZegoAudioVADClient *)client;

@end

NS_ASSUME_NONNULL_END
