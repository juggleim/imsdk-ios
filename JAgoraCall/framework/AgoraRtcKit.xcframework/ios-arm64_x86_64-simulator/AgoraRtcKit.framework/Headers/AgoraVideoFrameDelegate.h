//
//  AgoraRtcEngineKit.h
//  AgoraRtcEngineKit
//
//  Copyright (c) 2018 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AgoraEnumerates.h"

@class AgoraOutputVideoFrame;
@protocol AgoraVideoFrameDelegate <NSObject>
@optional

/**
 * Occurs each time the SDK receives a video frame captured by the local camera.
 * Notes: If getVideoFrameProcessMode is read-only, use MUST implement this method
 *
 * After you successfully register the video frame observer, the SDK triggers this callback each time
 * a video frame is received. In this callback, you can get the video data captured by the local
 * camera. You can then pre-process the data according to your scenarios.
 *
 * After pre-processing, you can send the processed video data back to the SDK by setting the
 * `videoFrame` parameter in this callback.
 *
 * @param videoFrame A pointer to the video frame: AgoraOutputVideoFrame
 * @return Determines whether to ignore the current video frame if the pre-processing fails:
 * - true: Do not ignore.
 * - false: Ignore, in which case this method does not sent the current video frame to the SDK.
 */
- (BOOL)onCaptureVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame sourceType:(AgoraVideoSourceType)sourceType;

/**
 * Occurs each time the SDK receives a video frame before encoding.
 *
 * After you successfully register the video frame observer, the SDK triggers this callback each time
 * when it receives a video frame. In this callback, you can get the video data before encoding. You can then
 * process the data according to your particular scenarios.
 *
 * After processing, you can send the processed video data back to the SDK by setting the
 * `videoFrame` parameter in this callback.
 *
 * The video data that this callback gets has been pre-processed, with its content cropped, rotated, and the image enhanced.
 *
 * @param videoFrame A pointer to the video frame: VideoFrame
 * @return Determines whether to ignore the current video frame if the pre-processing fails:
 * - true: Do not ignore.
 * - false: Ignore, in which case this method does not sent the current video frame to the SDK.
 */
- (BOOL)onPreEncodeVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame sourceType:(AgoraVideoSourceType)sourceType;


- (BOOL)onTranscodedVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame NS_SWIFT_NAME(onTranscodedVideoFrame(_:));

/**
 * Occurs each time the SDK receives a video frame sent by the remote user.
 *
 * After you successfully register the video frame observer, the SDK triggers this callback each time a
 * video frame is received. In this callback, you can get the video data sent by the remote user. You
 * can then post-process the data according to your scenarios.
 *
 * After post-processing, you can send the processed data back to the SDK by setting the `videoFrame`
 * parameter in this callback.
 *
 * @param uid ID of the remote user who sends the current video frame.
 * @param channelId Channel name.
 * @param videoFrame A pointer to the video frame: AgoraOutputVideoFrame
 * @return Determines whether to ignore the current video frame if the post-processing fails:
 * - true: Do not ignore.
 * - false: Ignore, in which case this method does not sent the current video frame to the SDK.
 */
- (BOOL)onRenderVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame uid:(NSUInteger)uid channelId:(NSString * _Nonnull)channelId NS_SWIFT_NAME(onRenderVideoFrame(_:uid:channelId:));

/**
 * Occurs each time the SDK receives a video frame decoded by the MediaPlayer.
 *
 * After you successfully register the video frame observer, the SDK triggers this callback each
 * time a video frame is decoded. In this callback, you can get the video data decoded by the
 * MediaPlayer. You can then pre-process the data according to your scenarios.
 *
 * After pre-processing, you can send the processed video data back to the SDK by setting the
 * `videoFrame` parameter in this callback.
 * 
 * @note
 * - This callback will not be affected by the return values of \ref getVideoFrameProcessMode "getVideoFrameProcessMode", \ref getRotationApplied "getRotationApplied", \ref getMirrorApplied "getMirrorApplied", \ref getObservedFramePosition "getObservedFramePosition".
 *
 * @param videoFrame A pointer to the video frame: AgoraOutputVideoFrame
 * @param mediaPlayerId of the mediaPlayer.
 * @return Determines whether to ignore the current video frame if the pre-processing fails:
 * - true: Do not ignore.
 * - false: Ignore, in which case this method does not sent the current video frame to the SDK.
 */
- (BOOL)onMediaPlayerVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame mediaPlayerId:(NSInteger)mediaPlayerId NS_SWIFT_NAME(onMediaPlayerVideoFrame(_:mediaPlayerId:));

/**
 * Occurs each time needs to get rotation angle.
 * @return rotation angle.
 */
- (BOOL)getRotationApplied NS_SWIFT_NAME(getRotationApplied());

/**
 * Occurs each time needs to get whether mirror is applied or not.
 * @return Determines whether to mirror.
 * - true: need to mirror.
 * - false: no mirror.
 */
- (BOOL)getMirrorApplied NS_SWIFT_NAME(getMirrorApplied());

/**
 * Indicate the video frame mode of the observer.
 * @return AgoraVideoFrameProcessMode
 */
- (AgoraVideoFrameProcessMode)getVideoFrameProcessMode NS_SWIFT_NAME(getVideoFrameProcessMode());

/**
 * Occurs each time needs to get preference video frame type.
 * @return AgoraVideoFormat.
 */
- (AgoraVideoFormat)getVideoFormatPreference NS_SWIFT_NAME(getVideoFormatPreference());

/**
 * Sets the frame position for the video observer.
 *
 * After you successfully register the video observer, the SDK triggers this callback each time it receives
 * a video frame. You can determine which position to observe by setting the return value. The SDK provides
 * 3 positions for observer. Each position corresponds to a callback function:
 *
 * AgoraVideoFramePositionPostCapture(1 << 0): The position after capturing the video data, which corresponds to the onCaptureVideoFrame callback.
 * AgoraVideoFramePositionPreRenderer(1 << 1): The position before receiving the remote video data, which corresponds to the onRenderVideoFrame callback.
 * AgoraVideoFramePositionPreEncoder(1 << 2): The position before encoding the video data, which corresponds to the onPreEncodeVideoFrame callback.
 *
 * To observe multiple frame positions, use '|' (the OR operator).
 * This callback observes AgoraVideoFramePositionPostCapture(1 << 0) and AgoraVideoFramePositionPreRenderer(1 << 1) by default.
 * To conserve the system consumption, you can reduce the number of frame positions that you want to observe.
 *
 * @return A bit mask that controls the frame position of the video observer: AgoraVideoFramePosition.
 */
- (AgoraVideoFramePosition)getObservedFramePosition NS_SWIFT_NAME(getObservedFramePosition());

/* Unavailable Delegate Methods */
#if TARGET_OS_IPHONE
- (BOOL)onCaptureVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame NS_SWIFT_NAME(onCapture(_:)) __attribute__((availability(ios,deprecated=7_0,message="Use onCaptureVideoFrame:sourceType: instead.")));
- (BOOL)onSecondaryCameraCaptureVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame NS_SWIFT_NAME(onSecondaryCameraCapture(_:)) __attribute__((availability(ios,deprecated=7_0,message="Use onCaptureVideoFrame:sourceType: instead.")));
- (BOOL)onScreenCaptureVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame NS_SWIFT_NAME(onScreenCapture(_:)) __attribute__((availability(ios,deprecated=7_0,message="Use onCaptureVideoFrame:sourceType: instead.")));
- (BOOL)onSecondaryScreenCaptureVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame NS_SWIFT_NAME(onSecondaryScreenCapture(_:)) __attribute__((availability(ios,deprecated=7_0,message="Use onCaptureVideoFrame:sourceType: instead.")));
- (BOOL)onPreEncodeVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame NS_SWIFT_NAME(onPreEncode(_:)) __attribute__((availability(ios,deprecated=7_0,message="Use onPreEncodeVideoFrame:sourceType: instead.")));
- (BOOL)onPreEncodeScreenVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame NS_SWIFT_NAME(onPreEncodeScreenVideoFrame(_:)) __attribute__((availability(ios,deprecated=7_0,message="Use onPreEncodeVideoFrame:sourceType: instead.")));
#endif

#if (!(TARGET_OS_IPHONE) && (TARGET_OS_MAC))
- (BOOL)onCaptureVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame NS_SWIFT_NAME(onCapture(_:)) __attribute__((availability(macos,deprecated=10_9,message="Use onCaptureVideoFrame:sourceType: instead.")));
- (BOOL)onSecondaryCameraCaptureVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame NS_SWIFT_NAME(onSecondaryCameraCapture(_:)) __attribute__((availability(macos,deprecated=10_9,message="Use onCaptureVideoFrame:sourceType: instead.")));
- (BOOL)onScreenCaptureVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame NS_SWIFT_NAME(onScreenCapture(_:)) __attribute__((availability(macos,deprecated=10_9,message="Use onCaptureVideoFrame:sourceType: instead.")));
- (BOOL)onSecondaryScreenCaptureVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame NS_SWIFT_NAME(onSecondaryScreenCapture(_:)) __attribute__((availability(macos,deprecated=10_9,message="Use onCaptureVideoFrame:sourceType: instead.")));
- (BOOL)onPreEncodeVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame NS_SWIFT_NAME(onPreEncode(_:)) __attribute__((availability(macos,deprecated=10_9,message="Use onPreEncodeVideoFrame:sourceType: instead.")));
- (BOOL)onPreEncodeScreenVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame NS_SWIFT_NAME(onPreEncodeScreenVideoFrame(_:)) __attribute__((availability(macos,deprecated=10_9,message="Use onPreEncodeVideoFrame:sourceType: instead.")));
#endif

@end
