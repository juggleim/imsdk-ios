//
//  AgoraRtcEngineKit.h
//  AgoraRtcEngineKit
//
//  Copyright (c) 2018 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol AgoraMediaRecorderDelegate;
@class AgoraMediaRecorderConfiguration;

__attribute__((visibility("default"))) @interface AgoraMediaRecorder : NSObject

/**
 * Registers the AgoraMediaRecorderDelegate delegate.
 *
 * @since v4.0.0
 *
 * @note Call this method before the startRecording method.
 *
 * @param delegate The callbacks for recording audio and video streams. See AgoraMediaRecorderDelegate 
 *
 * @return
 * - 0(ERR_OK): Success.
 * - < 0: Failure:
 */
- (int)setMediaRecorderDelegate:(id<AgoraMediaRecorderDelegate> _Nullable)delegate;

/** Enables/Disables dispatching delegate methods to the main queue.

 * If disabled, the app should dispatch UI operations to the main queue.
 * @param enabled Sets whether or not to dispatch delegate methods to the main queue:
 * YES: Dispatch delegate methods to the main queue.
 * NO: Do not dispatch delegate methods to the main queue
 * @return * 0: Success.
 * < 0: Failure.
 */
- (int)enableMainQueueDispatch:(BOOL)enabled NS_SWIFT_NAME(enableMainQueueDispatch(_:));

/**
 * Starts recording the local audio and video.
 *
 * @since v4.0.0
 *
 * After successfully calling \ref AgoraRtcEngineKit.createMediaRecorder: createMediaRecorder to get the media recorder object
 * , you can call this method to enable the recording of the local audio and video.
 *
 * This method can record the following content:
 * - The audio captured by the local microphone and encoded in AAC format.
 * - The video captured by the local camera and encoded by the SDK.
 * 
 * This method can record the following content:
 * - The audio received from remote users and encoded in AAC format.
 * - The video received from remote users.
 *
 *
 * The SDK can generate a recording file only when it detects the recordable audio and video streams; when there are
 * no audio and video streams to be recorded or the audio and video streams are interrupted for more than five
 * seconds, the SDK stops recording and triggers the
 * \ref AgoraMediaRecorderDelegate.stateDidChanged "stateDidChanged" (AgoraMediaRecorderState, AgoraMediaRecorderReasonCode)
 * callback.
 *
 * @note Call this method after joining the channel.
 *
 * @param config The recording configurations. See AgoraMediaRecorderConfiguration object.
 *
 * @return
 * - 0(ERR_OK): Success.
 * - < 0: Failure:
 *    - `-1(ERR_FAILED)`: IRtcEngine does not support the request due to one of the following reasons:
 *      - During remote recording, There is no subscription to the target channel or user。
 *    - `-2(ERR_INVALID_ARGUMENT)`: The parameter is invalid. Ensure the following:
 *      - The specified path of the recording file exists and is writable.
 *      - The specified format of the recording file is supported.
 *      - The maximum recording duration is correctly set.
 *      - During remote recording, ensure the user whose media streams you want record did join the channel.
 *    - `-4(ERR_NOT_SUPPORTED)`: IRtcEngine does not support the request due to one of the following reasons:
 *      - The recording is ongoing.
 *      - The recording stops because an error occurs.
 *      - No \ref AgoraMediaRecorderDelegate object is registered.
 */
- (int)startRecording:(AgoraMediaRecorderConfiguration* _Nonnull)config NS_SWIFT_NAME(startRecording(_:));
/**
 * Stops recording the audio and video.
 *
 * @since v4.0.0
 *
 * @note After calling \ref AgoraMediaRecorder.startRecording: startRecording, if you want to stop the recording,
 * you must call `stopRecording`; otherwise, the generated recording files might not be playable.
 *
 *
 * @return
 * - 0(ERR_OK): Success.
 * - < 0: Failure:
 */
/**
 Stop recording.
 @return 0: Success.
 * < 0: Failure.
 */
- (int)stopRecording NS_SWIFT_NAME(stopRecording());

@end