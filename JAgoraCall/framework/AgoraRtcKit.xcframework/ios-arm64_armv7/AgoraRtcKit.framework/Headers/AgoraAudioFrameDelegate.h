//
//  AgoraRtcEngineKit.h
//  AgoraRtcEngineKit
//
//  Copyright (c) 2018 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AgoraEnumerates.h"

@class AgoraAudioFrame;
@class AgoraAudioParams;

/**
 * The AgoraAudioFrameDelegate protocol enables audio frame callback event notifications to your application.
 */
@protocol AgoraAudioFrameDelegate <NSObject>
@optional

/**
 * Occurs when the recorded audio frame is received.
 * @param frame A pointer to the audio frame: AgoraAudioFrame.
 * @param channelId Unique channel name for the AgoraRTC session in the string
 * format. The string length must be less than 64 bytes. Supported character
 * scopes are:
 * - All lowercase English letters: a to z.
 * - All uppercase English letters: A to Z.
 * - All numeric characters: 0 to 9.
 * - The space character.
 * - Punctuation characters and other symbols, including: "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ",".
 * @return
 * - true: The recorded audio frame is valid and is encoded and sent.
 * - false: The recorded audio frame is invalid and is not encoded or sent.
 */
- (BOOL)onRecordAudioFrame:(AgoraAudioFrame* _Nonnull)frame channelId:(NSString * _Nonnull)channelId  NS_SWIFT_NAME(onRecordAudioFrame(_:channelId:));

/**
 * Occurs when the playback audio frame is received.
 * @param channelId Unique channel name for the AgoraRTC session in the string
 * format. The string length must be less than 64 bytes. Supported character
 * scopes are:
 * - All lowercase English letters: a to z.
 * - All uppercase English letters: A to Z.
 * - All numeric characters: 0 to 9.
 * - The space character.
 * - Punctuation characters and other symbols, including: "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ",".
 * @param frame A pointer to the audio frame: AgoraAudioFrame.
 * @return
 * - true: The playback audio frame is valid and is encoded and sent.
 * - false: The playback audio frame is invalid and is not encoded or sent.
 */
- (BOOL)onPlaybackAudioFrame:(AgoraAudioFrame* _Nonnull)frame channelId:(NSString * _Nonnull)channelId  NS_SWIFT_NAME(onPlaybackAudioFrame(_:channelId:));

/**
 * Occurs when the mixed audio data is received.
 * @param frame The A pointer to the audio frame: AgoraAudioFrame.
 * @param channelId Unique channel name for the AgoraRTC session in the string
 * format. The string length must be less than 64 bytes. Supported character
 * scopes are:
 * - All lowercase English letters: a to z.
 * - All uppercase English letters: A to Z.
 * - All numeric characters: 0 to 9.
 * - The space character.
 * - Punctuation characters and other symbols, including: "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ",".
 * @return
 * - true: The mixed audio data is valid and is encoded and sent.
 * - false: The mixed audio data is invalid and is not encoded or sent.
 */
- (BOOL)onMixedAudioFrame:(AgoraAudioFrame* _Nonnull)frame channelId:(NSString * _Nonnull)channelId  NS_SWIFT_NAME(onMixedAudioFrame(_:channelId:));

/**
 * Occurs when the ear monitoring audio frame is received.
 * @param frame A pointer to the audio frame: AgoraAudioFrame.
 * @return
 * - true: The ear monitoring audio frame is valid and is encoded and sent.
 * - false: The ear monitoring audio frame is invalid and is not encoded or sent.
 */
- (BOOL)onEarMonitoringAudioFrame:(AgoraAudioFrame* _Nonnull)frame NS_SWIFT_NAME(onEarMonitoringAudioFrame(_:));

/**
 Sets the frame position for the audio observer.
 * @return A bit mask that controls the frame position of the audio observer.
 * @note - Use '|' (the OR operator) to observe multiple frame positions.
 * <p>
 * After you successfully register the audio observer, the SDK triggers this callback each time it receives a audio frame. You can determine which position to observe by setting the return value.
 * The SDK provides 4 positions for observer. Each position corresponds to a callback function:
 * - `AgoraAudioFramePositionPlayback (1 << 0)`: The position for playback audio frame is received, which corresponds to the \ref onPlaybackFrame "onPlaybackFrame" callback.
 * - `AgoraAudioFramePositionRecord (1 << 1)`: The position for record audio frame is received, which corresponds to the \ref onRecordFrame "onRecordFrame" callback.
 * - `AgoraAudioFramePositionMixed (1 << 2)`: The position for mixed audio frame is received, which corresponds to the \ref onMixedFrame "onMixedFrame" callback.
 * - `AgoraAudioFramePositionBeforeMixing (1 << 3)`: The position for playback audio frame before mixing is received, which corresponds to the \ref onPlaybackFrameBeforeMixing "onPlaybackFrameBeforeMixing" callback.
 *  @return The bit mask that controls the audio observation positions.
 See AgoraAudioFramePosition.
 */

- (AgoraAudioFramePosition)getObservedAudioFramePosition NS_SWIFT_NAME(getObservedAudioFramePosition());

/** Sets the audio mixing format for the
 [onMixedAudioFrame]([AgoraAudioFrameDelegate onMixedAudioFrame:]) callback.

 Register the `getMixedAudioParams` callback when calling the
 [setAudioFrameDelegate]([AgoraRtcEngineKit setAudioFrameDelegate:]) method. After you
 successfully register the audio delegate, the SDK triggers this callback each
 time it receives an audio frame. You can set the audio mixing format in
 the return value of this callback.

 **Note**:

 - The SDK calculates the sample interval according to the `AgoraAudioParams`
 you set in the return value of this callback and triggers the
 `onMixedAudioFrame` callback at the calculated sample interval.
 Sample interval (seconds) = `samplesPerCall`/(`sampleRate` × `channel`).
 Ensure that the value of sample interval is equal to or greater than 0.01.

 @return Sets the audio format. See AgoraAudioParams.
 */
- (AgoraAudioParams* _Nonnull)getMixedAudioParams NS_SWIFT_NAME(getMixedAudioParams());

/** Sets the audio recording format for the
 [onRecordAudioFrame]([AgoraAudioFrameDelegate onRecordAudioFrame:])
 callback.

 Register the `getRecordAudioParams` callback when calling the
 [setAudioFrameDelegate]([AgoraRtcEngineKit setAudioFrameDelegate:]) method. After you
 successfully register the audio delegate, the SDK triggers this callback each
 time it receives an audio frame. You can set the audio recording format in
 the return value of this callback.

 **Note**:

 - This callback applies to iOS only.
 - The SDK calculates the sample interval according to the `AgoraAudioParams`
 you set in the return value of this callback and triggers the
 `onRecordAudioFrame` callback at the calculated sample interval.
 Sample interval (seconds) = `samplesPerCall`/(`sampleRate` × `channel`).
 Ensure that the value of sample interval is equal to or greater than 0.01.

 @return Sets the audio format. See AgoraAudioParams.
 */
- (AgoraAudioParams* _Nonnull)getRecordAudioParams NS_SWIFT_NAME(getRecordAudioParams());

/** Sets the audio playback format for the
 [onPlaybackAudioFrame]([AgoraAudioFrameDelegate onPlaybackAudioFrame:])
 callback.

 Register the `getPlaybackAudioParams` callback when calling the
 [setAudioFrameDelegate]([AgoraRtcEngineKit setAudioFrameDelegate:]) method. After you
 successfully register the audio delegate, the SDK triggers this callback each
 time it receives an audio frame. You can set the audio playback format in
 the return value of this callback.

 **Note**:

 - The SDK calculates the sample interval according to the `AgoraAudioParams`
 you set in the return value of this callback and triggers the
 `onPlaybackAudioFrame` callback at the calculated sample interval.
 Sample interval (seconds) = `samplesPerCall`/(`sampleRate` × `channel`).
 Ensure that the value of sample interval is equal to or greater than 0.01.

 @return Sets the audio format. See AgoraAudioParams.
 */
- (AgoraAudioParams* _Nonnull)getPlaybackAudioParams NS_SWIFT_NAME(getPlaybackAudioParams());

/** Sets the audio recording format for the
 [onEarMonitoringAudioFrame]([AgoraAudioFrameDelegate onEarMonitoringAudioFrame:])
 callback.

 Register the `getEarMonitoringAudioParams` callback when calling the
 [setAudioFrameDelegate]([AgoraRtcEngineKit setAudioFrameDelegate:]) method. After you
 successfully register the audio delegate, the SDK triggers this callback each
 time it receives an audio frame. You can set the audio recording format in
 the return value of this callback.

 **Note**:

 - This callback applies to iOS only.
 - The SDK calculates the sample interval according to the `AgoraAudioParams`
 you set in the return value of this callback and triggers the
 `onEarMonitoringAudioFrame` callback at the calculated sample interval.
 Sample interval (seconds) = `samplesPerCall`/(`sampleRate` × `channel`).
 Ensure that the value of sample interval is equal to or greater than 0.01.

 @return Sets the audio format. See AgoraAudioParams.
 */
- (AgoraAudioParams* _Nonnull)getEarMonitoringAudioParams NS_SWIFT_NAME(getEarMonitoringAudioParams());

/**
 * Occurs when the before-mixing playback audio frame is received.
 * @param channelId Unique channel name for the AgoraRTC session in the string
 * format. The string length must be less than 64 bytes. Supported character
 * scopes are:
 * - All lowercase English letters: a to z.
 * - All uppercase English letters: A to Z.
 * - All numeric characters: 0 to 9.
 * - The space character.
 * - Punctuation characters and other symbols, including: "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ",".
 * @param uid ID of the remote user.
 * @param frame A pointer to the audio frame: AgoraAudioFrame.
 * @return
 * - true: The before-mixing playback audio frame is valid and is encoded and sent.
 * - false: The before-mixing playback audio frame is invalid and is not encoded or sent.
 */
- (BOOL)onPlaybackAudioFrameBeforeMixing:(AgoraAudioFrame* _Nonnull)frame channelId:(NSString * _Nonnull)channelId uid:(NSUInteger)uid  NS_SWIFT_NAME(onPlaybackAudioFrame(beforeMixing:channelId:uid:));

@end
