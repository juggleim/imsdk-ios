//
//  ZegoExpressEngine+Player.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (Player)

/// Starts playing a stream from ZEGO RTC server.
///
/// Available since: 1.1.0
/// Description: Play audio and video streams from the ZEGO RTC server.
/// Use cases: In the real-time scenario, developers can listen to the [onRoomStreamUpdate] event callback to obtain the new stream information in the room where they are located, and call this interface to pass in streamID for play streams.
/// When to call: After [loginRoom].
/// Restrictions: None.
/// Caution: 1. The developer can update the player canvas by calling this function again (the streamID must be the same). 2. After the first play stream failure due to network reasons or the play stream is interrupted, the default time for SDK reconnection is 20min. 3. In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event. please refer to https://docs.zegocloud.com/faq/reconnect. 4. Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio and video stream can be actually played.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param streamID Stream ID, a string of up to 256 characters.
///   Caution:
///   Only support numbers, English characters and '-', '_'.
/// @param canvas The view used to display the play audio and video stream's image. When the view is set to [nil], no video is displayed, only audio is played.
- (void)startPlayingStream:(NSString *)streamID canvas:(nullable ZegoCanvas *)canvas;

/// Starts playing a stream from ZEGO RTC server or from third-party CDN. Support multi-room mode.
///
/// Available since: 1.1.0
/// Description: Play audio and video streams from the ZEGO RTC server or CDN.
/// Use cases: In real-time or live broadcast scenarios, developers can listen to the [onRoomStreamUpdate] event callback to obtain the new stream information in the room where they are located, and call this interface to pass in streamID for play streams.
/// When to call: After [loginRoom].
/// Restrictions: None.
/// Caution: 1. The developer can update the player canvas by calling this function again (the streamID must be the same). 2. After the first play stream failure due to network reasons or the play stream is interrupted, the default time for SDK reconnection is 20min. 3. In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event. please refer to https://docs.zegocloud.com/faq/reconnect. 4. Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio and video stream can be actually played.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param streamID Stream ID, a string of up to 256 characters.
///   Caution:
///   Only support numbers, English characters and '-', '_'.
/// @param canvas The view used to display the play audio and video stream's image. When the view is set to [nil], no video is displayed, only audio is played.
/// @param config Advanced player configuration Room [roomID] in [ZegoPlayerConfig] is the login roomID.
- (void)startPlayingStream:(NSString *)streamID
                    canvas:(nullable ZegoCanvas *)canvas
                    config:(ZegoPlayerConfig *)config;

/// Starts playing a stream from ZEGO RTC server, without Canvas, it is more suitable for pure audio stream.
///
/// Available since: 1.1.0
/// Description: Play audio streams from the ZEGO RTC server or CDN.
/// Use cases: In the real-time scenario, developers can listen to the [onRoomStreamUpdate] event callback to obtain the new stream information in the room where they are located, and call this interface to pass in streamID for play streams.
/// When to call: After [loginRoom].
/// Restrictions: None.
/// Caution: 1. After the first play stream failure due to network reasons or the play stream is interrupted, the default time for SDK reconnection is 20min. 2. In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event. please refer to https://docs.zegocloud.com/faq/reconnect. 3. Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio stream can be actually played.
///
/// @param streamID Stream ID, a string of up to 256 characters.
///   Caution:
///   Only support numbers, English characters and '-', '_'.
- (void)startPlayingStream:(NSString *)streamID;

/// Starts playing a stream from ZEGO RTC server or from third-party CDN, without Canvas, it is more suitable for pure audio stream.
///
/// Available since: 1.1.0
/// Description: Play audio streams from the ZEGO RTC server or CDN.
/// Use cases: In real-time or live broadcast scenarios, developers can listen to the [onRoomStreamUpdate] event callback to obtain the new stream information in the room where they are located, and call this interface to pass in streamID for play streams.
/// When to call: After [loginRoom].
/// Restrictions: None.
/// Caution: 1. After the first play stream failure due to network reasons or the play stream is interrupted, the default time for SDK reconnection is 20min. 2. In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event. please refer to https://docs.zegocloud.com/faq/reconnect. 3. Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio stream can be actually played.
///
/// @param streamID Stream ID, a string of up to 256 characters.
///   Caution:
///   Only support numbers, English characters and '-', '_'.
/// @param config Advanced player configuration.
- (void)startPlayingStream:(NSString *)streamID config:(ZegoPlayerConfig *)config;

/// Starts playing a stream from ZEGO RTC server or from third-party CDN when using RangeScene.
///
/// Available since: 3.4.0
/// Description: Play audio and video streams from the ZEGO RTC server or CDN.
/// Use cases: When using RangeScene, users can use this function to customize play the streaming.
/// When to call: After [loginScene].
/// Restrictions: None.
/// Caution: 1. The developer can update the player canvas by calling this function again (the streamID must be the same). 2. After the first play stream failure due to network reasons or the play stream is interrupted, the default time for SDK reconnection is 20min. 3. In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event. please refer to https://docs.zegocloud.com/faq/reconnect. 4. Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio and video stream can be actually played.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param streamID Stream ID, a string of up to 256 characters.
///   Caution:
///   Only support numbers, English characters and '-', '_'.
/// @param canvas The view used to display the play audio and video stream's image. When the view is set to [nil], no video is displayed, only audio is played.
/// @param config Advanced scene player configuration.
- (void)startPlayingStreamInScene:(NSString *)streamID
                           canvas:(nullable ZegoCanvas *)canvas
                           config:(ZegoScenePlayerConfig *)config;

/// Starts playing a stream from ZEGO RTC server or from third-party CDN when using RangeScene, without Canvas, it is more suitable for pure audio stream.
///
/// Available since: 3.3.0
/// Description: Play audio and video streams from the ZEGO RTC server or CDN.
/// Use cases: When using RangeScene, users can use this function to customize play the streaming.
/// When to call: After [loginScene].
/// Restrictions: None.
/// Caution: 1. The developer can update the player canvas by calling this function again (the streamID must be the same). 2. After the first play stream failure due to network reasons or the play stream is interrupted, the default time for SDK reconnection is 20min. 3. In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event. please refer to https://docs.zegocloud.com/faq/reconnect. 4. Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio and video stream can be actually played.
///
/// @param streamID Stream ID, a string of up to 256 characters.
///   Caution:
///   Only support numbers, English characters and '-', '_'.
/// @param config Advanced scene player configuration.
- (void)startPlayingStreamInScene:(NSString *)streamID config:(ZegoScenePlayerConfig *)config;

/// Switch from playing a certain stream to playing another stream.
///
/// Available since: 3.16.0
/// Description: Smoothly switch from one flv stream to another flv stream.
/// Use cases: For the same stream, there may be multiple streams with different resolutions. When the network quality deteriorates, in order to ensure the streaming quality, the streaming end can choose to switch from a high-resolution stream to a low-resolution stream..
/// When to call: After [startPlayingStream].
/// Restrictions: Only supports flv protocol stream.
/// Related callbacks:
///  1. The result of the switching request can be obtained through the [onPlayerSwitched] event.
///  2. When the stream is switched successfully, the current streaming status can be known through the [onPlayerStateUpdate] event.
///  3. When switching streams fails, there may not necessarily be an [onPlayerStateUpdate] event notification.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param fromStreamID Stream ID, a string of up to 256 characters.
///   Caution:
///   Only support numbers, English characters and '-', '_'.
/// @param toStreamID Stream ID, a string of up to 256 characters.
///   Caution:
///   Only support numbers, English characters and '-', '_'.
/// @param config Advanced player configuration.
- (void)switchPlayingStream:(NSString *)fromStreamID
                 toStreamID:(NSString *)toStreamID
                     config:(ZegoPlayerConfig *)config;

/// Stops playing a stream.
///
/// Available since: 1.1.0
/// Description: Play audio and video streams from the ZEGO RTC server.
/// Use cases: In the real-time scenario, developers can listen to the [onRoomStreamUpdate] event callback to obtain the delete stream information in the room where they are located, and call this interface to pass in streamID for stop play streams.
/// When to call: After [loginRoom].
/// Restrictions: None.
/// Caution: 1. When stopped, the attributes set for this stream previously, such as [setPlayVolume], [mutePlayStreamAudio], [mutePlayStreamVideo], etc., will be invalid and need to be reset when playing the the stream next time.
///  2. After stopping pulling, the iOS platform view will clear the last frame by default and keep the background color of the view. The Android platform view remains at the last frame by default. If you need to clear the last frame, please contact ZEGO technical support.
///
/// @param streamID Stream ID.
- (void)stopPlayingStream:(NSString *)streamID;

/// Set decryption key for the playing stream.
///
/// Available since: 1.19.0
/// Description: When streaming, the audio and video data will be decrypted according to the set key.
/// Use cases: Usually used in scenarios that require high security for audio and video calls.
/// When to call: after [createEngine], after the play stream can be changed at any time.
/// Restrictions: This function is only valid when calling from Zego RTC or L3 server.
/// Related APIs: [setPublishStreamEncryptionKey]Set the publish stream encryption key.
/// Caution: This interface can only be called if encryption is set on the publish. Calling [stopPlayingStream] or [logoutRoom] will clear the decryption key.
///
/// @param key The decryption key, note that the key length only supports 16/24/32 bytes.
/// @param streamID Stream ID.
- (void)setPlayStreamDecryptionKey:(NSString *)key streamID:(NSString *)streamID;

/// Set up cross App playing stream information.
///
/// Available since: 2.19.0
/// Description: This information is used for authentication before playing a stream or when retrying playing a stream.
/// Use cases: Used in scenarios that playing streams across apps.
/// When to call: after [createEngine], after the play stream can be changed at any time.
/// Restrictions: This function is only valid when playing stream from Zego RTC server.
/// Caution: Calling [stopPlayingStream] or [logoutRoom] will clear this information.
///
/// @param info Information for cross App playing stream.
/// @param streamID Stream ID.
- (void)setPlayStreamCrossAppInfo:(ZegoCrossAppInfo *)info streamID:(NSString *)streamID;

/// Take a snapshot of the playing stream.
///
/// Available since: 1.17.0
/// Description: Take a screenshot of the specified stream ID.
/// When to call: after called [startPlayingStream].
/// Restrictions: None.
/// Related callbacks: [onPlayerTakeSnapshotResult] Screenshot data callback.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param streamID Stream ID to be snapshot.
/// @param callback Results of take play stream snapshot.
- (void)takePlayStreamSnapshot:(NSString *)streamID
                      callback:(ZegoPlayerTakeSnapshotCallback)callback;

/// Sets the stream playback volume.
///
/// Available since: 1.16.0
/// Description: Set the sound size of the stream, the local user can control the playback volume of the audio stream.
/// When to call: after called [startPlayingStream].
/// Restrictions: None.
/// Related APIs: [setAllPlayStreamVolume] Set all stream volume.
/// Caution: You need to reset after [stopPlayingStream] and [startPlayingStream]. This function and the [setAllPlayStreamVolume] function overwrite each other, and the last call takes effect.
///
/// @param volume Volume percentage. The value ranges from 0 to 200, and the default value is 100.
/// @param streamID Stream ID.
- (void)setPlayVolume:(int)volume streamID:(NSString *)streamID;

/// Sets the all stream playback volume.
///
/// Available since: 2.3.0
/// Description: Set the sound size of the stream, the local user can control the playback volume of the audio stream.
/// When to call: after called [startPlayingStream].
/// Restrictions: None.
/// Related APIs: [setPlayVolume] Set the specified streaming volume.
/// Caution: Set the specified streaming volume and [setAllPlayStreamVolume] interface to override each other, and the last call takes effect.
///
/// @param volume Volume percentage. The value ranges from 0 to 200, and the default value is 100.
- (void)setAllPlayStreamVolume:(int)volume;

/// Set play video stream type.
///
/// Available since: 2.3.0
/// Description: When the publish stream sets the codecID to SVC through [setVideoConfig], the puller can dynamically set and select different stream types (small resolution is one-half of the standard layer).
/// Use cases: In general, when the network is weak or the rendered UI window is small, you can choose to pull videos with small resolutions to save bandwidth.
/// When to call: before or after called [startPlayingStream].
/// Restrictions: None.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param streamType Video stream type.
/// @param streamID Stream ID.
- (void)setPlayStreamVideoType:(ZegoVideoStreamType)streamType streamID:(NSString *)streamID;

/// Set the adaptive adjustment interval range of the buffer for playing stream.
///
/// Available since: 2.1.0
/// Description: Set the range of adaptive adjustment of the internal buffer of the sdk when streaming is 0-4000ms.
/// Use cases: Generally, in the case of a poor network environment, adjusting and increasing the playback buffer of the pull stream will significantly reduce the audio and video freezes, but will increase the delay.
/// When to call: after called [createEngine], if it has been set, you need to reset it every time you play the stream again.
/// Restrictions: None.
/// Caution: When the upper limit of the cache interval set by the developer exceeds 4000ms, the value will be 4000ms. When the upper limit of the cache interval set by the developer is less than the lower limit of the cache interval, the upper limit will be automatically set as the lower limit.
///
/// @param streamID Stream ID.
/// @param min The lower limit of the buffer adaptation interval, in milliseconds. The default value is 0ms.
/// @param max The upper limit of the buffer adaptation interval, in milliseconds. The default value is 4000ms.
- (void)setPlayStreamBufferIntervalRange:(NSString *)streamID
                                     min:(unsigned int)min
                                     max:(unsigned int)max;

/// Set the weight of the pull stream priority.
///
/// Available since: 1.1.0
/// Description: Set the weight of the streaming priority.
/// Use cases: This interface can be used when developers need to prioritize the quality of a audio and video stream in business（do not use in pure audio case）. For example: in class scene, if students pull multiple streams, you can set high priority for teacher stream.
/// When to call: after called [startPlayingStream].
/// Restrictions: None.
/// Caution: By default, all streams have the same weight. Only one stream can be set with high priority, whichever is set last. After the flow is stopped, the initial state is automatically restored, and all flows have the same weight.When the local network is not good, while ensuring the focus flow, other stalls may be caused more.
///
/// @param streamID Stream ID.
- (void)setPlayStreamFocusOn:(NSString *)streamID;

/// Whether the pull stream can receive the specified audio data.
///
/// Available since: 1.1.0
/// Description: In the process of real-time audio and video interaction, local users can use this function to control whether to receive audio data from designated remote users when pulling streams as needed. When the developer does not receive the audio receipt, the hardware and network overhead can be reduced.
/// Use cases: Call this function when developers need to quickly close and restore remote audio. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
/// When to call: This function can be called after calling [createEngine].
/// Caution:
///  1. When used together with [muteAllPlayAudioStreams], they can override each other's configurations.
///  2. When used together with [muteAllPlayStreamAudio], this function only works when the [muteAllPlayStreamAudio] function is set to `NO`.
///  3. After stopping streaming, any properties previously set for this stream such as [setPlayVolume], [mutePlayStreamAudio], [mutePlayStreamVideo] and other streaming-related configurations will become invalid and need to be reset before the next stream is pulled.
/// Related APIs: You can call the [muteAllPlayStreamAudio] function to control whether to receive all audio data. When the two functions [muteAllPlayStreamAudio] and [mutePlayStreamAudio] are set to `NO` at the same time, the local user can receive the audio data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamAudio(YES)] function is called, it is globally effective, that is, local users will be prohibited from receiving all remote users' audio data. At this time, the [mutePlayStreamAudio] function will not take effect whether it is called before or after [muteAllPlayStreamAudio].2. When the [muteAllPlayStreamAudio(NO)] function is called, the local user can receive the audio data of all remote users. At this time, the [mutePlayStreamAudio] function can be used to control whether to receive a single audio data. Calling the [mutePlayStreamAudio(YES, streamID)] function allows the local user to receive audio data other than the `streamID`; calling the [mutePlayStreamAudio(NO, streamID)] function allows the local user to receive all audio data.
///
/// @param mute Whether it can receive the audio data of the specified remote user when streaming, "YES" means prohibition, "NO" means receiving, the default value is "NO".
/// @param streamID Stream ID.
- (void)mutePlayStreamAudio:(BOOL)mute streamID:(NSString *)streamID;

/// Whether the pull stream can receive the specified video data.
///
/// Available since: 1.1.0
/// Description: In the process of real-time video and video interaction, local users can use this function to control whether to receive video data from designated remote users when pulling streams as needed. When the developer does not receive the video data, the hardware and network overhead can be reduced.
/// Use cases: This function can be called when developers need to quickly close and resume watching remote video. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
/// When to call: This function can be called after calling [createEngine].
/// Caution:
///  1. When used together with [muteAllPlayVideoStreams], they can override each other's configurations.
///  2. When used together with [muteAllPlayStreamAudio], this function only works when the [muteAllPlayStreamVideo] function is set to `NO`.
///  3. When you mute the video stream, the view remains at the last frame by default, if you need to clear the last frame, please contact ZEGO technical support.
///  4. After stopping streaming, any properties previously set for this stream such as [setPlayVolume], [mutePlayStreamAudio], [mutePlayStreamVideo] and other streaming-related configurations will become invalid and need to be reset before the next stream is pulled.
/// Related APIs: You can call the [muteAllPlayStreamVideo] function to control whether to receive all video data. When the two functions [muteAllPlayStreamVideo] and [mutePlayStreamVideo] are set to `NO` at the same time, the local user can receive the video data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamVideo(YES)] function is called, it will take effect globally, that is, local users will be prohibited from receiving all remote users' video data. At this time, the [mutePlayStreamVideo] function will not take effect whether it is called before or after [muteAllPlayStreamVideo]. 2. When the [muteAllPlayStreamVideo(NO)] function is called, the local user can receive the video data of all remote users. At this time, the [mutePlayStreamVideo] function can be used to control whether to receive a single video data. Call the [mutePlayStreamVideo(YES, streamID)] function, the local user can receive other video data other than the `streamID`; call the [mutePlayStreamVideo(NO, streamID)] function, the local user can receive all the video data.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param mute Whether it is possible to receive the video data of the specified remote user when streaming, "YES" means prohibition, "NO" means receiving, the default value is "NO". The default value for automatically played streams within the SDK is NO.
/// @param streamID Stream ID.
- (void)mutePlayStreamVideo:(BOOL)mute streamID:(NSString *)streamID;

/// Can the pull stream receive all audio data. (When set to YES, calling [mutePlayStreamAudio] will not take effect)
///
/// Available since: 2.4.0
/// Description: In the process of real-time audio and video interaction, local users can use this function to control whether to receive audio data from all remote users when pulling streams (including the audio streams pushed by users who have newly joined the room after calling this function). By default, users can receive audio data pushed by all remote users after joining the room. When the developer does not receive the audio receipt, the hardware and network overhead can be reduced.
/// Use cases: Call this function when developers need to quickly close and restore remote audio. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
/// When to call: This function can be called after calling [createEngine].
/// Caution: This function cannot be used together with [muteAllPlayAudioStreams] throughout the SDK lifecycle.
/// Related APIs: You can call the [mutePlayStreamAudio] function to control whether to receive a single piece of audio data. When the two functions [muteAllPlayStreamAudio] and [mutePlayStreamAudio] are set to `NO` at the same time, the local user can receive the audio data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamAudio(YES)] function is called, it takes effect globally, that is, local users will be prohibited from receiving audio data from all remote users. At this time, the [mutePlayStreamAudio] function will not take effect no matter if the [mutePlayStreamAudio] function is called before or after [muteAllPlayStreamAudio]. 2. When the [muteAllPlayStreamAudio(NO)] function is called, the local user can receive the audio data of all remote users. At this time, the [mutePlayStreamAudio] function can be used to control whether to receive a single audio data. Calling the [mutePlayStreamAudio(YES, streamID)] function allows the local user to receive audio data other than the `streamID`; calling the [mutePlayStreamAudio(NO, streamID)] function allows the local user to receive all audio data.
///
/// @param mute Whether it is possible to receive audio data from all remote users when streaming, "YES" means prohibition, "NO" means receiving, and the default value is "NO".
- (void)muteAllPlayStreamAudio:(BOOL)mute;

/// Can the pull stream receive all audio data.
///
/// Available since: 3.10.0
/// Description: In the process of real-time audio and video interaction, local users can use this function to control whether to receive audio data from all remote users when pulling streams (including the audio streams pushed by users who have newly joined the room after calling this function). By default, users can receive audio data pushed by all remote users after joining the room. When the developer does not receive the audio receipt, the hardware and network overhead can be reduced.
/// Use cases: Call this function when developers need to quickly close and restore remote audio. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
/// When to call: This function can be called after calling [createEngine].
/// Caution: This function cannot be used together with [muteAllPlayStreamAudio] throughout the SDK lifecycle.
/// Related APIs: You can call the [mutePlayStreamAudio] function to control whether to receive a single piece of audio data. When the two functions [muteAllPlayStreamAudio] and [mutePlayStreamAudio] are set to `NO` at the same time, the local user can receive the audio data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamAudio(YES)] function is called, it takes effect globally, that is, local users will be prohibited from receiving audio data from all remote users. At this time, the [mutePlayStreamAudio] function will not take effect no matter if the [mutePlayStreamAudio] function is called before or after [muteAllPlayStreamAudio]. 2. When the [muteAllPlayStreamAudio(NO)] function is called, the local user can receive the audio data of all remote users. At this time, the [mutePlayStreamAudio] function can be used to control whether to receive a single audio data. Calling the [mutePlayStreamAudio(YES, streamID)] function allows the local user to receive audio data other than the `streamID`; calling the [mutePlayStreamAudio(NO, streamID)] function allows the local user to receive all audio data.
///
/// @param mute Whether it is possible to receive audio data from all remote users when streaming, "YES" means prohibition, "NO" means receiving, and the default value is "NO".
- (void)muteAllPlayAudioStreams:(BOOL)mute;

/// Can the pull stream receive all video data. (When set to YES, calling [mutePlayStreamVideo] will not take effect)
///
/// Available since: 2.4.0
/// Description: In the process of real-time video and video interaction, local users can use this function to control whether to receive all remote users' video data when pulling the stream (including the video stream pushed by the new user who joins the room after calling this function). By default, users can receive video data pushed by all remote users after joining the room. When the developer does not receive the video data, the hardware and network overhead can be reduced.
/// Use cases: This function can be called when developers need to quickly close and resume watching remote video. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
/// When to call: This function can be called after calling [createEngine].
/// Caution:
///  1. This function cannot be used together with [muteAllPlayVideoStreams] throughout the SDK lifecycle.
///  2. When you mute the video stream, the view remains at the last frame by default, if you need to clear the last frame, please contact ZEGO technical support.
/// Related APIs: You can call the [mutePlayStreamVideo] function to control whether to receive a single piece of video data. When the two functions [muteAllPlayStreamVideo] and [mutePlayStreamVideo] are set to `NO` at the same time, the local user can receive the video data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamVideo(YES)] function is called, it will take effect globally, that is, the local user will be prohibited from receiving all remote users' video data. At this time, the [mutePlayStreamVideo] function will not take effect whether it is called before or after [muteAllPlayStreamVideo]. 2. When the [muteAllPlayStreamVideo(NO)] function is called, the local user can receive the video data of all remote users. At this time, the [mutePlayStreamVideo] function can be used to control whether to receive a single video data. Call the [mutePlayStreamVideo(YES, streamID)] function, the local user can receive other video data other than the `streamID`; call the [mutePlayStreamVideo(NO, streamID)] function, the local user can receive all the video data.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param mute Whether it is possible to receive all remote users' video data when streaming, "YES" means prohibition, "NO" means receiving, and the default value is "NO".
- (void)muteAllPlayStreamVideo:(BOOL)mute;

/// Can the pull stream receive all video data.
///
/// Available since: 3.10.0
/// Description: In the process of real-time video and video interaction, local users can use this function to control whether to receive all remote users' video data when pulling the stream (including the video stream pushed by the new user who joins the room after calling this function). By default, users can receive video data pushed by all remote users after joining the room. When the developer does not receive the video data, the hardware and network overhead can be reduced.
/// Use cases: This function can be called when developers need to quickly close and resume watching remote video. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
/// When to call: This function can be called after calling [createEngine].
/// Caution:
///  1. This function cannot be used together with [muteAllPlayStreamVideo] throughout the SDK lifecycle.
///  2. When you mute the video stream, the view remains at the last frame by default, if you need to clear the last frame, please contact ZEGO technical support.
/// Related APIs: You can call the [mutePlayStreamVideo] function to control whether to receive a single piece of video data.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param mute Whether it is possible to receive all remote users' video data when streaming, "YES" means prohibition, "NO" means receiving, and the default value is "NO".
- (void)muteAllPlayVideoStreams:(BOOL)mute;

/// Enables or disables hardware decoding.
///
/// Available since: 1.1.0
/// Description: Control whether hardware decoding is used when playing streams, with hardware decoding enabled the SDK will use the GPU for decoding, reducing CPU usage.
/// Use cases: If developers find that the device heats up badly when playing large resolution audio and video streams during testing on some models, consider calling this function to enable hardware decoding.
/// Default value: Hardware decoding is disabled by default when this interface is not called.
/// When to call: This function needs to be called after [createEngine] creates an instance.
/// Restrictions: None.
/// Caution: Need to be called before calling [startPlayingStream], if called after playing the stream, it will only take effect after stopping the stream and re-playing it. Once this configuration has taken effect, it will remain in force until the next call takes effect.
///
/// @param enable Whether to turn on hardware decoding switch, YES: enable hardware decoding, NO: disable hardware decoding.
- (void)enableHardwareDecoder:(BOOL)enable;

/// Enables or disables frame order detection.
///
/// Available since: 1.1.0
/// Description: Control whether to turn on frame order detection.
/// Use cases: Turning on frame order detection when pulling cdn's stream will prevent splash screens.
/// Default value: Turn on frame order detection by default when this interface is not called.
/// When to call: This function needs to be called after [createEngine] creates an instance.
/// Restrictions: None.
/// Caution: Turn off frame order detection during playing stream may result in a brief splash screen.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param enable Whether to turn on frame order detection, YES: enable check poc, NO: disable check poc.
- (void)enableCheckPoc:(BOOL)enable;

/// Whether the specified video decoding format is supported.
///
/// Available since: 2.12.0
/// Description: Whether the specified video decoding is supported depends on the following aspects: whether the hardware model supports hard decoding, whether the performance of the hardware model supports soft decoding, and whether the SDK includes the decoding module.
/// When to call: After creating the engine.
/// Caution: It is recommended that users call this interface to obtain the H.265 decoding support capability before pulling the H.265 stream. If it is not supported, the user can pull the stream of other encoding formats, such as H.264.
///
/// @param codecID Video codec id.Required: Yes.
/// @return Whether the specified video decoding format is supported; YES means supported, you can use this decoding format for playing stream; NO means not supported, the SDK does not recommend users to play streams of this encoding format. If users force the play, low frame rates may occur.
- (BOOL)isVideoDecoderSupported:(ZegoVideoCodecID)codecID;

/// Whether the specified video decoding format is supported.
///
/// Available since: 3.0.0
/// Description: Whether the specified video decoding is supported depends on the following aspects: whether the hardware model supports hard decoding, whether the performance of the hardware model supports soft decoding, and whether the SDK includes the decoding module.
/// When to call: After creating the engine.
/// Caution: It is recommended that users call this interface to obtain the H.265 decoding support capability before pulling the H.265 stream. If it is not supported, the user can pull the stream of other encoding formats, such as H.264.
///
/// @param codecID Video codec id. Required: Yes.
/// @param codecBackend Backend implementation of decoder. Required: Yes.
/// @return Whether the specified video decoding format is supported; 0 means not supported, and the decoding format cannot be used for play stream; 1 means support, you can use this decoding format for playing stream; 2 means not confirmed, it is recommended to call this interface later.
- (int)isVideoDecoderSupported:(ZegoVideoCodecID)codecID
                  codecBackend:(ZegoVideoCodecBackend)codecBackend;

/// Set the play stream alignment properties.
///
/// Available since: 2.14.0
/// Description: When playing at the streaming end, control whether the playing RTC stream needs to be accurately aligned. If necessary, all the streams that contain precise alignment parameters will be aligned; if not, all streams are not aligned.
/// Use case: It is often used in scenes that require to play multiple streams alignment such as KTV to ensure that users can switch between singing anchors, ordinary Maishangyu chat anchors, and Maixia audiences at any time during use.
/// Default value: If this interface is not called, the default is not aligned.
/// When to call: Called after [createEngine]. Call the interface repeatedly, and the latest setting is valid.
/// Related APIs: Set the precise alignment parameter of the stream channel [setStreamAlignmentProperty].
///
/// @param mode Setting the stream alignment mode.
- (void)setPlayStreamsAlignmentProperty:(ZegoStreamAlignmentMode)mode;

#if TARGET_OS_IPHONE
/// Enable video super resolution.
///
/// Available since: 3.0.0
/// Description: Whether to enable video super resolution when playing stream, the resolution of the played video can be doubled at the stream playing end through video super resolution. For example, the original resolution is 640x360, and the super-resolution is 1280x720.
/// Use cases: Live streaming scenario.
/// When to call: Video super resolution is only valid for playing stream video. Needs to be called after [initVideoSuperResolution].
/// Caution:
///  1. This function requires a special package, please contact ZEGO technical support;
///  2. This function will consume extra system resources. In order to ensure user experience, ZEGO can only enable video super resolution for one stream, and the original video resolution is not recommended to exceed 640 × 360.
/// Related callbacks: Developer can use the [onPlayerVideoSuperResolutionUpdate] callback to monitor the video super resolution status change.
///
/// @param streamID The ID of the stream that currently needs to turn on or off overscore.
/// @param enable Whether to enable super resolution, it is not enabled by default.
- (void)enableVideoSuperResolution:(NSString *)streamID enable:(BOOL)enable;
#endif

#if TARGET_OS_IPHONE
/// Init video super resolution.
///
/// Available since: 3.3.0
/// Description: After the super resolution is initialized, the super resolution function can be used normally
/// Use cases: Live streaming scenario.
/// When to call: Video super resolution is only valid for playing stream video. Needs to be called after [createEngine].
/// Caution:
///  1. Initializing the video screen is divided into time-consuming operations, and frequent initialization and de-initialization are not recommended;
///  2. This function requires a special package, please contact ZEGO technical support.
- (void)initVideoSuperResolution;
#endif

#if TARGET_OS_IPHONE
/// Uninit video super resolution.
///
/// Available since: 3.3.0
/// Description: After deinitializing the super resolution, the SDK will release the resources occupied by the super resolution and make the super resolution function unavailable.
/// Use cases: Live streaming scenario.
/// When to call: Needs to be called after [initVideoSuperResolution].
/// Caution: Initializing the video screen is divided into time-consuming operations, and frequent initialization and de-initialization are not recommended.
- (void)uninitVideoSuperResolution;
#endif

/// Update playing canvas.
///
/// Available: since 3.4.0
/// Description: This interface will update playing view.
/// Use case: The user can call this function to update canvas display video.
/// When to call: After calling the [startPlayingStream] interface.
/// Restrictions: None.
/// Caution: None.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param streamID Stream ID, a string of up to 256 characters.
///   Caution:
///   Only support numbers, English characters and '-', '_'.
/// @param canvas The view used to display the play audio and video stream's image. When the view is set to [nil], no video is displayed, only audio is played.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
- (int)updatePlayingCanvas:(NSString *)streamID canvas:(nullable ZegoCanvas *)canvas;

/// [Deprecated] Set the adaptive adjustment interval range of the buffer for playing stream.
///
/// @deprecated Deprecated since 3.4.0, please use [setPlayStreamBufferIntervalRange:min:max:] instead.
/// @param range The buffer adaptive interval range.
/// @param streamID Stream ID.
- (void)setPlayStreamBufferIntervalRange:(NSRange)range
                                streamID:(NSString *)streamID DEPRECATED_ATTRIBUTE;

@end

NS_ASSUME_NONNULL_END
