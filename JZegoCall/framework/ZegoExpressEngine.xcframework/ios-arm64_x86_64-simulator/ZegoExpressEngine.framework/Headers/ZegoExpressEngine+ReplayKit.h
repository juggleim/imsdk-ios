//
//  ZegoExpressEngine+ReplayKit.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressEngine.h"
#if TARGET_OS_IPHONE
#import <ReplayKit/ReplayKit.h>
#endif

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (ReplayKit)

#if defined(TARGET_OS_IOS) && TARGET_OS_IOS
/// Initialize the Express ReplayKit module.
///
/// Only use in the ReplayKit sub-process, don't use it in the main App process
/// Note: This function is only available in ZegoExpressVideo SDK!
- (void)prepareForReplayKit;
#endif

#if defined(TARGET_OS_IOS) && TARGET_OS_IOS
/// Set ReplayKit Microphone volume
///
/// Available since: 2.13.0
/// Description: Set Microphone audio volume for ReplayKit.
/// Use cases: Set Microphone audio volume in the game live scenarios.
/// When to call /Trigger: This function needs to be called after the success of [startPreview] or [startPublishingStream] to be effective.
/// Restrictions: None.
/// Caution: This function affects the audio volume of type RPSampleBufferTypeAudioMic.
/// Related APIs: [setReplayKitApplicationVolume] function can set the Application audio volume for ReplayKit.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param volume The range is 0 ~ 200. The default is 100.
- (void)setReplayKitMicrophoneVolume:(int)volume;
#endif

#if defined(TARGET_OS_IOS) && TARGET_OS_IOS
/// Set ReplayKit Application volume
///
/// Available since: 2.13.0
/// Description: Set Application audio volume for ReplayKit.
/// Use cases: Set Application audio volume in the game live scenarios.
/// When to call /Trigger: This function needs to be called after the success of [startPreview] or [startPublishingStream] to be effective.
/// Restrictions: None.
/// Caution: This function affects the audio volume of type RPSampleBufferTypeAudioApp.
/// Related APIs: [setReplayKitMicrophoneVolume] function can set the Microphone audio volume for ReplayKit.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param volume The range is 0 ~ 200. The default is 100.
- (void)setReplayKitApplicationVolume:(int)volume;
#endif

#if defined(TARGET_OS_IOS) && TARGET_OS_IOS
/// Handles ReplayKit's SampleBuffer, supports receiving video and audio buffer.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param sampleBuffer Video or audio buffer returned by ReplayKit
/// @param bufferType Buffer type returned by ReplayKit
- (void)handleReplayKitSampleBuffer:(CMSampleBufferRef)sampleBuffer
                         bufferType:(RPSampleBufferType)bufferType API_AVAILABLE(ios(10.0));
#endif

@end

NS_ASSUME_NONNULL_END
