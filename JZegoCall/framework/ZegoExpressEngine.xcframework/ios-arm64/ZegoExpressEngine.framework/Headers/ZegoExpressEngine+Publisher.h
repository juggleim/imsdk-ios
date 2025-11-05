//
//  ZegoExpressEngine+Publisher.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (Publisher)

/// Start publishing a stream.
///
/// Available since: 1.1.0
/// Description: Users push their local audio and video streams to the ZEGO RTC server or CDN, and other users in the same room can pull the audio and video streams to watch through the `streamID` or CDN pull stream address.
/// Use cases: It can be used to publish streams in real-time connecting wheat, live broadcast and other scenarios.
/// When to call: After [loginRoom].
/// Restrictions: None.
/// Caution:
///   1. Before start to publish the stream, the user can choose to call [setVideoConfig] to set the relevant video parameters, and call [startPreview] to preview the video.
///   2. Other users in the same room can get the streamID by monitoring the [onRoomStreamUpdate] event callback after the local user publishing stream successfully.
///   3. In the case of poor network quality, user publish may be interrupted, and the SDK will attempt to reconnect. You can learn about the current state and error information of the stream published by monitoring the [onPublisherStateUpdate] event.
///
/// @param streamID Stream ID, a string of up to 256 characters.
///   Caution:
///   1. Stream ID is defined by you.
///   2. needs to be globally unique within the entire AppID. If in the same AppID, different users publish each stream and the stream ID is the same, which will cause the user to publish the stream failure. You cannot include URL keywords, otherwise publishing stream and playing stream will fails.
///   3. Only support numbers, English characters and '-', '_'.
- (void)startPublishingStream:(NSString *)streamID;

/// Starts publishing a stream (for the specified channel). You can call this function to publish a second stream.
///
/// Available since: 1.1.0
/// Description: Users push their local audio and video streams to the ZEGO RTC server or CDN, and other users in the same room can pull the audio and video streams to watch through the `streamID` or CDN pull stream address.
/// Use cases: It can be used to publish streams in real-time connecting wheat, live broadcast and other scenarios.
/// When to call: After [loginRoom].
/// Restrictions: None.
/// Caution:
///   1. Before start to publish the stream, the user can choose to call [setVideoConfig] to set the relevant video parameters, and call [startPreview] to preview the video.
///   2. Other users in the same room can get the streamID by monitoring the [onRoomStreamUpdate] event callback after the local user publishing stream successfully.
///   3. In the case of poor network quality, user publish may be interrupted, and the SDK will attempt to reconnect. You can learn about the current state and error information of the stream published by monitoring the [onPublisherStateUpdate] event.
///
/// @param streamID Stream ID, a string of up to 256 characters.
///   Caution:
///   1. Stream ID is defined by you.
///   2. needs to be globally unique within the entire AppID. If in the same AppID, different users publish each stream and the stream ID is the same, which will cause the user to publish the stream failure. You cannot include URL keywords, otherwise publishing stream and playing stream will fails.
///   3. Only support numbers, English characters and '-', '_'.
/// @param channel Publish stream channel.
- (void)startPublishingStream:(NSString *)streamID channel:(ZegoPublishChannel)channel;

/// Starts publishing a stream. Support multi-room mode.
///
/// Available since: 1.1.0
/// Description: Users push their local audio and video streams to the ZEGO RTC server or CDN, and other users in the same room can pull the audio and video streams to watch through the `streamID` or CDN pull stream address.
/// Use cases: It can be used to publish streams in real-time connecting wheat, live broadcast and other scenarios.
/// When to call: After [loginRoom].
/// Restrictions: None.
/// Caution:
///   1. Before start to publish the stream, the user can choose to call [setVideoConfig] to set the relevant video parameters, and call [startPreview] to preview the video.
///   2. Other users in the same room can get the streamID by monitoring the [onRoomStreamUpdate] event callback after the local user publishing stream successfully.
///   3. In the case of poor network quality, user publish may be interrupted, and the SDK will attempt to reconnect. You can learn about the current state and error information of the stream published by monitoring the [onPublisherStateUpdate] event.
///   4. To call [SetRoomMode] function to select multiple rooms, the room ID must be specified explicitly.
///
/// @param streamID Stream ID, a string of up to 256 characters.
///   Caution:
///   1. Stream ID is defined by you.
///   2. needs to be globally unique within the entire AppID. If in the same AppID, different users publish each stream and the stream ID is the same, which will cause the user to publish the stream failure. You cannot include URL keywords, otherwise publishing stream and playing stream will fails.
///   3. Only support numbers, English characters and '-', '_'.
/// @param config Advanced publish configuration.
/// @param channel Publish stream channel.
- (void)startPublishingStream:(NSString *)streamID
                       config:(ZegoPublisherConfig *)config
                      channel:(ZegoPublishChannel)channel;

/// Starts publishing a stream in RangeScene scene.
///
/// Available since: 3.0.0
/// Description: Users push their local audio and video streams to the ZEGO RTC server.
/// Use cases: Starts publishing a stream in RangeScene scene.
/// When to call: After [loginScene].
/// Restrictions: None.
/// Caution: None.
///
/// @param streamID Stream ID, a string of up to 256 characters.
///   Caution:
///   1. Stream ID is defined by you.
///   2. needs to be globally unique within the entire AppID. If in the same AppID, different users publish each stream and the stream ID is the same, which will cause the user to publish the stream failure. You cannot include URL keywords, otherwise publishing stream and playing stream will fails.
///   3. Only support numbers, English characters and '-', '_'.
/// @param channel Publish stream channel.
/// @param config Advanced scene publish configuration.
- (void)startPublishingStreamInScene:(NSString *)streamID
                             channel:(ZegoPublishChannel)channel
                              config:(ZegoScenePublisherConfig *)config;

/// Stops publishing a stream.
///
/// Available since: 1.1.0
/// Description: The user stops sending local audio and video streams, and other users in the room will receive a stream deletion notification.
/// Use cases: It can be used to stop publish streams in real-time connecting wheat, live broadcast and other scenarios.
/// When to call: After [startPublishingStream].
/// Restrictions: None.
/// Caution:
///   1. After stopping the streaming, other users in the same room can receive the delete notification of the stream by listening to the [onRoomStreamUpdate] callback.
///   2. If the user has initiated publish flow, this function must be called to stop the publish of the current stream before publishing the new stream (new streamID), otherwise the new stream publish will return a failure.
///   3. After stopping streaming, the developer should stop the local preview based on whether the business situation requires it.
- (void)stopPublishingStream;

/// Stops publishing a stream (for the specified channel).
///
/// Available since: 1.1.0
/// Description: The user stops sending local audio and video streams, and other users in the room will receive a stream deletion notification.
/// Use cases: It can be used to stop publish streams in real-time connecting wheat, live broadcast and other scenarios.
/// When to call: After [startPublishingStream].
/// Restrictions: None.
/// Caution:
///   1. After stopping the streaming, other users in the same room can receive the delete notification of the stream by listening to the [onRoomStreamUpdate] callback.
///   2. If the user has initiated publish flow, this function must be called to stop the publish of the current stream before publishing the new stream (new streamID), otherwise the new stream publish will return a failure.
///   3. After stopping streaming, the developer should stop the local preview based on whether the business situation requires it.
///
/// @param channel Publish stream channel.
- (void)stopPublishingStream:(ZegoPublishChannel)channel;

/// Sets the extra information of the stream being published.
///
/// Available since: 1.1.0
/// Description: Use this function to set the extra info of the stream. The stream extra information is an extra information identifier of the stream ID. Unlike the stream ID, which cannot be modified during the publishing process, the stream extra information can be modified midway through the stream corresponding to the stream ID. Developers can synchronize variable content related to stream IDs based on stream additional information.
/// When to call: After the engine is created [createEngine], Called before and after [startPublishingStream] can both take effect.
/// Restrictions: None.
/// Caution: Act on the main publish channel ZegoPublishChannel.Main.
/// Related callbacks: Other users in the same room will be notified through the [onRoomStreamExtraInfoUpdate] callback function.
///
/// @param extraInfo Stream extra information, a string of up to 1024 characters.
/// @param callback Set stream extra information execution result notification.
- (void)setStreamExtraInfo:(NSString *)extraInfo
                  callback:(nullable ZegoPublisherSetStreamExtraInfoCallback)callback;

/// Sets the extra information of the stream being published for the specified publish channel.
///
/// Available since: 1.1.0
/// Description: Use this function to set the extra info of the stream. The stream extra information is an extra information identifier of the stream ID. Unlike the stream ID, which cannot be modified during the publishing process, the stream extra information can be modified midway through the stream corresponding to the stream ID. Developers can synchronize variable content related to stream IDs based on stream additional information.
/// When to call: After the engine is created [createEngine], Called before and after [startPublishingStream] can both take effect.
/// Restrictions: None.
/// Related callbacks: Users can obtain the execution result of the function through [ZegoPublisherSetStreamExtraInfoCallback] callback.
///
/// @param extraInfo Stream extra information, a string of up to 1024 characters.
/// @param channel Publish stream channel.
/// @param callback Set stream extra information execution result notification.
- (void)setStreamExtraInfo:(NSString *)extraInfo
                   channel:(ZegoPublishChannel)channel
                  callback:(nullable ZegoPublisherSetStreamExtraInfoCallback)callback;

/// Starts/Updates the local video preview.
///
/// Available since: 1.1.0
/// Description: The user can see his own local image by calling this function.
/// Use cases: It can be used for local preview in real-time connecting wheat, live broadcast and other scenarios.
/// When to call: After [createEngine].
/// Restrictions: None.
/// Caution: 1. The preview function does not require you to log in to the room or publish the stream first. But after exiting the room, SDK internally actively stops previewing by default. 2. Local view and preview modes can be updated by calling this function again. The user can only preview on one view. If you call [startPreview] again to pass in a new view, the preview screen will only be displayed in the new view. 3. You can set the mirror mode of the preview by calling the [setVideoMirrorMode] function. The default preview setting is image mirrored. 4. When this function is called, the audio and video engine module inside SDK will start really, and it will start to try to collect audio and video.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param canvas The view used to display the preview image. If the view is set to nil, no preview will be made.
- (void)startPreview:(nullable ZegoCanvas *)canvas;

/// Starts/Updates the local video preview (for the specified channel).
///
/// Available since: 1.1.0
/// Description: The user can see his own local image by calling this function.
/// Use cases: It can be used for local preview in real-time connecting wheat, live broadcast and other scenarios.
/// When to call: After [createEngine].
/// Restrictions: None.
/// Caution: 1. The preview function does not require you to log in to the room or publish the stream first. But after exiting the room, SDK internally actively stops previewing by default. 2. Local view and preview modes can be updated by calling this function again. The user can only preview on one view. If you call [startPreview] again to pass in a new view, the preview screen will only be displayed in the new view. 3. You can set the mirror mode of the preview by calling the [setVideoMirrorMode] function. The default preview setting is image mirrored. 4. When this function is called, the audio and video engine module inside SDK will start really, and it will start to try to collect audio and video.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param canvas The view used to display the preview image. If the view is set to nil, no preview will be made.
/// @param channel Publish stream channel
- (void)startPreview:(nullable ZegoCanvas *)canvas channel:(ZegoPublishChannel)channel;

/// Starts the local audio preview.
///
/// Available since: 1.1.0
/// Description: Call this function after creating the engine and before publishing/playing a stream, and then the engine will start to capture audio.
/// Use cases: Can check whether the audio equipment works ok by calling this function and by the soundlevel function of Engine.
/// When to call: After [createEngine].
/// Restrictions: None.
/// Caution: 1. The preview function does not require you to log in to the room or publish the stream first. But after exiting the room, SDK internally actively stops previewing by default. 2. When this function is called, the audio engine module inside SDK will start, and it will start to try to collect audio.
- (void)startPreview;

/// Stops the local preview.
///
/// Available since: 1.1.0
/// Description: This function can be called to stop the preview when the preview is not needed locally.
/// Caution: Stopping the preview will not affect the publish stream and playing stream functions.
- (void)stopPreview;

/// Stops the local preview (for the specified channel).
///
/// Available since: 1.1.0
/// Description: This function can be called to stop the preview when the preview is not needed locally.
/// Caution: Stopping the preview will not affect the publish stream and playing stream functions.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param channel Publish stream channel
- (void)stopPreview:(ZegoPublishChannel)channel;

/// Sets up the video configurations.
///
/// Available since: 1.1.0
/// Description: Set the video frame rate, bit rate, video capture resolution, and video encoding output resolution.
/// Use cases: Recommended configuration in different business scenarios https://docs.zegocloud.com/article/12122.
/// Default value: The default video capture resolution is 360p, the video encoding output resolution is 360p, the bit rate is 600 kbps, and the frame rate is 15 fps.
/// When to call: After [createEngine].
/// Restrictions: It is necessary to set the relevant video configuration before [startPreview], and only support the modification of the encoding resolution, the bit rate and the frame rate after [startPreview].
/// Caution: Developers should note that the wide and high resolution of the mobile end is opposite to the wide and high resolution of the PC. For example, in the case of 360p, the resolution of the mobile end is 360x640, and the resolution of the PC end is 640x360.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param config Video configuration, the SDK provides a common setting combination of resolution, frame rate and bit rate, they also can be customized.
- (void)setVideoConfig:(ZegoVideoConfig *)config;

/// Sets up the video configurations (for the specified channel).
///
/// Available since: 1.1.0
/// Description: Set the video frame rate, bit rate, video capture resolution, and video encoding output resolution.
/// Default value: The default video capture resolution is 360p, the video encoding output resolution is 360p, the bit rate is 600 kbps, and the frame rate is 15 fps.
/// When to call: After [createEngine].
/// Restrictions: It is necessary to set the relevant video configuration before [startPreview], and only support the modification of the encoding resolution, the bit rate and the frame rate after [startPreview].
/// Caution: Developers should note that the wide and high resolution of the mobile end is opposite to the wide and high resolution of the PC. For example, in the case of 360p, the resolution of the mobile end is 360x640, and the resolution of the PC end is 640x360.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param config Video configuration, the SDK provides a common setting combination of resolution, frame rate and bit rate, they also can be customized.
/// @param channel Publish stream channel.
- (void)setVideoConfig:(ZegoVideoConfig *)config channel:(ZegoPublishChannel)channel;

/// Gets the current video configurations.
///
/// This function can be used to get the main publish channel's current video frame rate, bit rate, video capture resolution, and video encoding output resolution.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @return Video configuration object
- (ZegoVideoConfig *)getVideoConfig;

/// Gets the current video configurations (for the specified channel).
///
/// This function can be used to get the specified publish channel's current video frame rate, bit rate, video capture resolution, and video encoding output resolution.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param channel Publish stream channel
/// @return Video configuration object
- (ZegoVideoConfig *)getVideoConfig:(ZegoPublishChannel)channel;

/// Set dual stream config.
///
/// Available since: 3.7.0
/// Description: Set the stream config.
/// When to call: This must take effect when the codecID specified in the call to [ZegoExpressEngine > setVideoConfig] is ZegoVideoCodecIDH264DualStream after [createEngine] is called.
/// Restrictions: To take effect, the parameters of flow and small flow must be specified at the same time. The resolution ratio of flow and small flow must be the same. For example, both are 4:3 .
/// Caution: Width, height, resolution and bitrate are all greater than zero to take effect.
///
/// @param configList config info.
/// @param channel ZegoPublishChannel.
- (void)setPublishDualStreamConfig:(NSArray<ZegoPublishDualStreamConfig *> *)configList
                           channel:(ZegoPublishChannel)channel;

/// Sets the video mirroring mode.
///
/// Available since: 1.1.0
/// Description: Set whether the local preview video and the published video have mirror mode enabled. For specific mirroring mode, please refer to https://docs.zegocloud.com/article/12122.
/// When to call: After [createEngine].
/// Restrictions: This setting only works if the SDK is responsible for rendering.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param mirrorMode Mirror mode for previewing or publishing the stream.
- (void)setVideoMirrorMode:(ZegoVideoMirrorMode)mirrorMode;

/// Sets the video mirroring mode (for the specified channel).
///
/// Available since: 1.1.0
/// Description: Set whether the local preview video and the published video have mirror mode enabled. For specific mirroring mode.
/// When to call: After [createEngine].
/// Restrictions: This setting only works if the SDK is responsible for rendering.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param mirrorMode Mirror mode for previewing or publishing the stream.
/// @param channel Publish stream channel.
- (void)setVideoMirrorMode:(ZegoVideoMirrorMode)mirrorMode channel:(ZegoPublishChannel)channel;

#if TARGET_OS_IPHONE
/// Set the video orientation.
///
/// Available since: 1.1.0
/// Description: Set the video orientation.
/// Use cases: When users use mobile devices to conduct live broadcasts or video calls, they can set different video directions according to the scene.
/// When to call: After [createEngine].
/// Restrictions: None.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param orientation Video orientation.
- (void)setAppOrientation:(UIInterfaceOrientation)orientation;
#endif

#if TARGET_OS_IPHONE
/// Sets the video orientation (for the specified channel).
///
/// Available since: 1.1.0
/// Description: Set the video orientation.
/// Use cases: When users use mobile devices to conduct live broadcasts or video calls, they can set different video directions according to the scene.
/// When to call: After [createEngine].
/// Restrictions: Currently only supports iOS and Android platforms.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param orientation Video orientation.
/// @param channel Publish stream channel.
- (void)setAppOrientation:(UIInterfaceOrientation)orientation channel:(ZegoPublishChannel)channel;
#endif

/// Sets up the audio configurations.
///
/// Available since: 1.3.4
/// Description: You can set the combined value of the audio codec, bit rate, and audio channel through this function. If the preset value cannot meet the developer's scenario, the developer can set the parameters according to the business requirements.
/// Default value: The default audio config refers to the default value of [ZegoAudioConfig].
/// When to call: After the engine is created [createEngine], and before publishing [startPublishingStream].
/// Restrictions: None.
/// Caution: Act on the main publish channel ZegoPublishChannel.Main.
/// Related APIs: [getAudioConfig].
///
/// @param config Audio config.
- (void)setAudioConfig:(ZegoAudioConfig *)config;

/// Sets up the audio configurations for the specified publish channel.
///
/// Available since: 1.3.4
/// Description: You can set the combined value of the audio codec, bit rate, and audio channel through this function. If the preset value cannot meet the developer's scenario, the developer can set the parameters according to the business requirements.
/// Default value: The default audio config refers to the default value of [ZegoAudioConfig].
/// When to call: After the engine is created [createEngine], and before publishing [startPublishingStream].
/// Restrictions: None.
/// Related APIs: [getAudioConfig].
///
/// @param config Audio config.
/// @param channel Publish stream channel.
- (void)setAudioConfig:(ZegoAudioConfig *)config channel:(ZegoPublishChannel)channel;

/// Gets the current audio configurations.
///
/// Available since: 1.8.0
/// Description: You can get the current audio codec, bit rate, and audio channel through this function.
/// When to call: After the engine is created [createEngine].
/// Restrictions: None.
/// Caution: Act on the main publish channel ZegoPublishChannel.Main.
/// Related APIs: [setAudioConfig].
///
/// @return Audio config.
- (ZegoAudioConfig *)getAudioConfig;

/// Gets the current audio configurations from the specified publish channel.
///
/// Available since: 1.8.0
/// Description: You can get the current audio codec, bit rate, and audio channel through this function.
/// When to call: After the engine is created [createEngine].
/// Restrictions: None.
/// Related APIs: [setAudioConfig].
///
/// @param channel Publish stream channel.
/// @return Audio config.
- (ZegoAudioConfig *)getAudioConfig:(ZegoPublishChannel)channel;

/// Set encryption key for the publishing stream.
///
/// Available since: 1.19.0
/// Description: Support calling this function to update the encryption key while publishing stream.
/// When to call: After the engine is created [createEngine], Called before and after [startPublishingStream] can both take effect.
/// Restrictions: This function is only valid when publishing stream to the Zego RTC server.
/// Caution: Note that developers need to update the player's decryption key before updating the publisher's encryption key. Act on the main publish channel ZegoPublishChannel.Main.
/// Related APIs: Calling [stopPublishingStream] or [logoutRoom] will clear the encryption key.
///
/// @param key The encryption key, note that the key length only supports 16/24/32 bytes.
- (void)setPublishStreamEncryptionKey:(NSString *)key;

/// Set encryption key for the publishing stream for the specified publish channel.
///
/// Available since: 1.19.0
/// Description: Support calling this function to update the encryption key while publishing stream.
/// When to call: After the engine is created [createEngine], Called before and after [startPublishingStream] can both take effect.
/// Restrictions: This function is only valid when publishing stream to the Zego RTC server.
/// Caution: Note that developers need to update the player's decryption key before updating the publisher's encryption key.
/// Related APIs: Calling [stopPublishingStream] or [logoutRoom] will clear the encryption key.
///
/// @param key The encryption key, note that the key length only supports 16/24/32 bytes.
/// @param channel Publish stream channel.
- (void)setPublishStreamEncryptionKey:(NSString *)key channel:(ZegoPublishChannel)channel;

/// Take a snapshot of the publishing stream.
///
/// Available since: 1.17.0
/// Description: Take a snapshot of the publishing stream.
/// When to call: Called this function after calling [startPublishingStream] or [startPreview].
/// Restrictions: None.
/// Caution: The resolution of the snapshot is the encoding resolution set in [setVideoConfig]. If you need to change it to capture resolution, please call [setCapturePipelineScaleMode] to change the capture pipeline scale mode to [Post]. Act on the main publish channel ZegoPublishChannel.Main.
/// Related callbacks: The screenshot result will be called back through [ZegoPublisherTakeSnapshotCallback] (Windows HBITMAP; macOS/iOS CGImageRef; Linux QImage; Android Bitmap).
/// Related APIs: [takePlayStreamSnapshot].
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param callback Results of take publish stream snapshot.
- (void)takePublishStreamSnapshot:(ZegoPublisherTakeSnapshotCallback)callback;

/// Take a snapshot of the publishing stream for the specified publish channel.
///
/// Available since: 1.17.0
/// Description: Take a snapshot of the publishing stream.
/// When to call: Called this function after calling [startPublishingStream] or [startPreview].
/// Restrictions: None.
/// Caution: The resolution of the snapshot is the encoding resolution set in [setVideoConfig]. If you need to change it to capture resolution, please call [setCapturePipelineScaleMode] to change the capture pipeline scale mode to [Post].
/// Related callbacks: The screenshot result will be called back through [ZegoPublisherTakeSnapshotCallback].
/// Related APIs: [takePlayStreamSnapshot].
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param callback Results of take publish stream snapshot.
/// @param channel Publish stream channel.
- (void)takePublishStreamSnapshot:(ZegoPublisherTakeSnapshotCallback)callback
                          channel:(ZegoPublishChannel)channel;

/// Stops or resumes sending the audio part of a stream.
///
/// Available since: 1.1.0
/// Description: This function can be called when publishing the stream to realize not publishing the audio data stream. The SDK still collects and processes the audio, but send muted audio frame packets to the network.
/// Use case: Users can call this interface when they do not want to publish any audio data. This interface does not affect [onBeforeAudioPrepAudioData].
/// When to call: Called after the engine is created [createEngine] can take effect.
/// Restrictions: None.
/// Caution: Act on the main publish channel ZegoPublishChannel.Main.
/// Related callbacks: If you stop sending audio streams, the remote user that play stream of local user publishing stream can receive `Mute` status change notification by monitoring [onRemoteMicStateUpdate] callbacks.
/// Related APIs: [mutePublishStreamVideo].
///
/// @param mute Whether to stop sending audio streams, YES means not to send audio stream, and NO means sending audio stream. The default is NO.
- (void)mutePublishStreamAudio:(BOOL)mute;

/// Stops or resumes sending the audio part of a stream for the specified channel.
///
/// Available since: 1.1.0
/// Description: This function can be called when publishing the stream to realize not publishing the audio data stream. The SDK still collects and processes the audio, but send muted audio frame packets to the network.
/// Use case: Users can call this interface when they do not want to publish any audio data. This interface does not affect [onBeforeAudioPrepAudioData].
/// When to call: Called after the engine is created [createEngine] can take effect.
/// Restrictions: None.
/// Related callbacks: If you stop sending audio streams, the remote user that play stream of local user publishing stream can receive `Mute` status change notification by monitoring [onRemoteMicStateUpdate] callbacks.
/// Related APIs: [mutePublishStreamVideo].
///
/// @param mute Whether to stop sending audio streams, YES means not to send audio stream, and NO means sending audio stream. The default is NO.
/// @param channel Publish stream channel.
- (void)mutePublishStreamAudio:(BOOL)mute channel:(ZegoPublishChannel)channel;

/// Stops or resumes sending the video part of a stream.
///
/// Available since: 1.1.0
/// Description: This function can be called when publishing the stream to realize not publishing the video stream. The local camera can still work normally, can capture, preview and process video images normally, but does not send the video data to the network.
/// When to call: Called after the engine is created [createEngine] can take effect.
/// Restrictions: None.
/// Caution: Act on the main publish channel ZegoPublishChannel.Main.
/// Related callbacks: If you stop sending video streams locally, the remote user that play stream of local user publishing stream can receive `Mute` status change notification by monitoring [onRemoteCameraStateUpdate] callbacks.
/// Related APIs: [mutePublishStreamAudio].
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param mute Whether to stop sending video streams, YES means not to send video stream, and NO means sending video stream. The default is NO.
- (void)mutePublishStreamVideo:(BOOL)mute;

/// Stops or resumes sending the video part of a stream for the specified channel.
///
/// Available since: 1.1.0
/// Description: This function can be called when publishing the stream to realize not publishing the video stream. The local camera can still work normally, can capture, preview and process video images normally, but does not send the video data to the network.
/// When to call: Called after the engine is created [createEngine] can take effect.
/// Restrictions: None.
/// Related callbacks: If you stop sending video streams locally, the remote user that play stream of local user publishing stream can receive `Mute` status change notification by monitoring [onRemoteCameraStateUpdate] callbacks.
/// Related APIs: [mutePublishStreamAudio].
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param mute Whether to stop sending video streams, YES means not to send video stream, and NO means sending video stream. The default is NO.
/// @param channel Publish stream channel.
- (void)mutePublishStreamVideo:(BOOL)mute channel:(ZegoPublishChannel)channel;

/// Enable or disable the stream precision alignment function.
///
/// Available since: 2.11.0.
/// Description: Use this interface to enable stream alignment, the SDK will attach network time information to the stream when publishing it for accurate stream alignment.
/// Use case: Generally used in scenarios such as KTV where stream mixing alignment is required.
/// When to call: After the engine is created [createEngine].
/// Caution: If you need to align each stream through network time when playing multiple streams or mixed streams, you need to call [startPublishingStream] to publish the stream and [forceSynchronousNetworkTime] in [ZegoPublisherConfig] is 1 to enable network time synchronization.
/// Related APIs: [startMixerTask], [startAutoMixerTask]
///
/// @param alignment Whether to enable the stream mixing precision alignment function.
/// @param channel Publish stream channel
- (void)setStreamAlignmentProperty:(int)alignment channel:(ZegoPublishChannel)channel;

/// Enables or disables traffic control.
///
/// Available since: 1.5.0
/// Description: Enabling traffic control allows the SDK to adjust the audio and video streaming bitrate according to the current upstream network environment conditions, or according to the counterpart's downstream network environment conditions in a one-to-one interactive scenario, to ensure smooth results. At the same time, you can further specify the attributes of traffic control to adjust the corresponding control strategy.
/// Default value: Enable.
/// When to call: After the engine is created [createEngine], Called before [startPublishingStream] can take effect.
/// Restrictions: Only support RTC publish.
/// Caution: Act on the main publish channel ZegoPublishChannel.Main.
///
/// @param enable Whether to enable traffic control. The default is ture.
/// @param property Adjustable property of traffic control, bitmask OR format. Should be one or the combinations of [ZegoTrafficControlProperty] enumeration. [AdaptiveFPS] as default.
- (void)enableTrafficControl:(BOOL)enable property:(ZegoTrafficControlProperty)property;

/// Enables or disables the traffic control for the specified publish channel.
///
/// Available since: 1.5.0
/// Description: Enabling traffic control allows the SDK to adjust the audio and video streaming bitrate according to the current upstream network environment conditions, or according to the counterpart's downstream network environment conditions in a one-to-one interactive scenario, to ensure smooth results. At the same time, you can further specify the attributes of traffic control to adjust the corresponding control strategy.
/// Default value: Enable.
/// When to call: After the engine is created [createEngine], Called before [startPublishingStream] can take effect.
/// Restrictions: Only support RTC publish.
///
/// @param enable Whether to enable traffic control. The default is ture.
/// @param property Adjustable property of traffic control, bitmask format. Should be one or the combinations of [ZegoTrafficControlProperty] enumeration. [AdaptiveFPS] as default.
/// @param channel Publish stream channel.
- (void)enableTrafficControl:(BOOL)enable
                    property:(ZegoTrafficControlProperty)property
                     channel:(ZegoPublishChannel)channel;

/// Set the minimum video bitrate threshold for traffic control.
///
/// Available since: 1.1.0
/// Description: Set the control strategy when the video bitrate reaches the lowest threshold during flow control. When the bitrate is lower than the minimum threshold, you can choose not to send video data or send it at a very low frame bitrate.
/// Default value: There is no control effect of the lowest threshold of video bitrate.
/// When to call: After the engine is created [createEngine], Called before [startPublishingStream] can take effect.
/// Restrictions: The traffic control must be turned on [enableTrafficControl].
/// Caution: Act on the main publish channel ZegoPublishChannel.Main.
/// Related APIs: [enableTrafficControl].
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param bitrate Minimum video bitrate threshold for traffic control(kbps).
/// @param mode Video sending mode below the minimum bitrate.
- (void)setMinVideoBitrateForTrafficControl:(int)bitrate
                                       mode:(ZegoTrafficControlMinVideoBitrateMode)mode;

/// Sets the minimum video bitrate for traffic control for the specified publish channel.
///
/// Available since: 1.1.0
/// Description: Set the control strategy when the video bitrate reaches the lowest threshold during flow control. When the bitrate is lower than the minimum threshold, you can choose not to send video data or send it at a very low frame bitrate.
/// Default value: There is no control effect of the lowest threshold of video bitrate.
/// When to call: After the engine is created [createEngine], Called before [startPublishingStream] can take effect.
/// Restrictions: The traffic control must be turned on [enableTrafficControl].
/// Related APIs: [enableTrafficControl].
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param bitrate Minimum video bitrate (kbps).
/// @param mode Video sending mode below the minimum bitrate.
/// @param channel Publish stream channel.
- (void)setMinVideoBitrateForTrafficControl:(int)bitrate
                                       mode:(ZegoTrafficControlMinVideoBitrateMode)mode
                                    channel:(ZegoPublishChannel)channel;

/// Sets the minimum video frame rate threshold for traffic control.
///
/// Available since: 2.17.0
/// Description: When enabling traffic control [enableTrafficControl], and its parameter [property] includes the attribute AdaptiveFPS, the minimum frame rate of the video will not be lower than the value set by the [setMinVideoFpsForTrafficControl] interface. A value of 0 indicates no limit.
/// Default value: There is no control effect of the lowest threshold of video frame rate.
/// When to call: The call takes effect after the engine [createEngine] is created.
/// Restrictions: The traffic control must be turned on [enableTrafficControl]. And its parameter [property] must contain AdaptiveFPS, Please refer to [ZegoTrafficControlProperty] for details.
/// Related APIs: [enableTrafficControl].
/// Caution: If you need to cancel the limit of the setting value, you can set it to 0.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param fps The minimum video frame rate threshold for traffic control(fps).
/// @param channel Publish stream channel.
- (void)setMinVideoFpsForTrafficControl:(int)fps channel:(ZegoPublishChannel)channel;

/// Sets the minimum video resolution threshold for traffic control.
///
/// Available since: 2.17.0
/// Description: When enabling traffic control [enableTrafficControl], and its parameter [property] includes the attribute AdaptiveResolution, the minimum resolution of the video will not be lower than the value set by the [setMinVideoResolutionForTrafficControl] interface. A value of 0 indicates no limit.
/// Default value: There is no control effect of the lowest threshold of video resolution.
/// When to call: The call takes effect after the engine [createEngine] is created.
/// Restrictions: The traffic control must be turned on [enableTrafficControl]. And its parameter [property] must contain AdaptiveResolution, Please refer to [ZegoTrafficControlProperty] for details.
/// Related APIs: [enableTrafficControl].
/// Caution: If you need to cancel the limit of the setting value, you can set width and height to 0.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param width The flow controls the width of the lowest video resolution.
/// @param height The flow controls the height of the lowest video resolution.
/// @param channel Publish stream channel.
- (void)setMinVideoResolutionForTrafficControl:(int)width
                                        height:(int)height
                                       channel:(ZegoPublishChannel)channel;

/// Set the factors of concern that trigger traffic control.
///
/// Available since: 2.4.0
/// Description: Use this interface to control whether to start traffic control due to poor remote network conditions.
/// Default value: Default is ZegoTrafficControlFounsOnRemote.
/// When to call: After the engine is created [createEngine], Called before [startPublishingStream] can take effect.
/// Restrictions: The traffic control must be turned on [enableTrafficControl].
/// Related APIs: [enableTrafficControl.
///
/// @param mode When LOCAL_ONLY is selected, only the local network status is concerned. When choosing REMOTE, also take into account the remote network.
- (void)setTrafficControlFocusOn:(ZegoTrafficControlFocusOnMode)mode;

/// Set the factors of concern that trigger traffic control for the specified publish channel.
///
/// Available since: 2.4.0
/// Description: Use this interface to control whether to start traffic control due to poor remote network conditions.
/// Default value: Default is disable.
/// When to call: After the engine is created [createEngine], Called before [startPublishingStream] can take effect.
/// Restrictions: The traffic control must be turned on [enableTrafficControl].
/// Related APIs: [enableTrafficControl.
///
/// @param mode When LOCAL_ONLY is selected, only the local network status is concerned. When choosing REMOTE, also take into account the remote network.
/// @param channel Publish stream channel.
- (void)setTrafficControlFocusOn:(ZegoTrafficControlFocusOnMode)mode
                         channel:(ZegoPublishChannel)channel;

/// Sets the audio recording volume for stream publishing.
///
/// Available since: 1.13.0
/// Description: This function is used to perform gain processing based on the device's collected volume. The local user can control the sound level of the audio stream sent to the remote end.
/// Default value: Default is 100.
/// When to call: After creating the engine [createEngine].
/// Restrictions: The capture volume can be dynamically set during publishing.
/// Related APIs: Set the playing stream volume [setPlayVolume].
///
/// @param volume The volume gain percentage, the range is 0 ~ 200, and the default value is 100, which means 100% of the original collection volume of the device.
- (void)setCaptureVolume:(int)volume;

/// Set audio capture stereo mode.
///
/// Available since: 1.15.0 (iOS/Android/Windows/OHOS); support macOS since 2.16.0
/// Description: This function is used to set the audio capture channel mode. When the developer turns on the stereo capture, using a special stereo capture device, the stereo audio data can be captured and streamed.
/// Use cases: In some professional scenes, users are particularly sensitive to sound effects, such as voice radio and musical instrument performance. At this time, support for stereo and high-quality sound is required.
/// Default value: The default is None, which means mono capture.
/// When to call: It needs to be called after [createEngine]， before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager].
/// Restrictions: If you need to enable stereo capture, you also need to meet the following conditions: For iOS/Android, you need to connect an external audio device that supports stereo capture and be at the media volume. For macOS, it needs to be at the media volume. For Windows, an external audio device that supports stereo capture is required.
/// Related APIs: When streaming, you need to enable the stereo audio encoding function through the [setAudioConfig] interface at the same time.
///
/// @param mode Audio stereo capture mode.
- (void)setAudioCaptureStereoMode:(ZegoAudioCaptureStereoMode)mode;

/// Adds a target CDN URL to which the stream will be relayed from ZEGO RTC server.
///
/// Available since: 1.1.0
/// Description: Forward audio and video streams from ZEGO RTC servers to custom CDN content distribution networks with high latency but support for high concurrent pull streams.
/// Use cases: 1. It is often used in large-scale live broadcast scenes that do not have particularly high requirements for delay. 2. Since ZEGO RTC server itself can be configured to support CDN(content distribution networks), this function is mainly used by developers who have CDN content distribution services themselves. 3. This function supports dynamic relay to the CDN content distribution network, so developers can use this function as a disaster recovery solution for CDN content distribution services.
/// When to call: After calling the [createEngine] function to create the engine.
/// Restrictions: When the [enablePublishDirectToCDN] function is set to YES to publish the stream straight to the CDN, then calling this function will have no effect.
/// Caution: Removing URLs retweeted to CDN requires calling [removePublishCdnUrl], calling [stopPublishingStream] will not remove URLs publish to CDN.
/// Related APIs: Remove URLs that are re-pushed to the CDN [removePublishCdnUrl].
///
/// @param targetURL CDN relay address, supported address format is rtmp, rtmps.
/// @param streamID Stream ID.
/// @param callback The execution result of update the relay CDN operation.
- (void)addPublishCdnUrl:(NSString *)targetURL
                streamID:(NSString *)streamID
                callback:(nullable ZegoPublisherUpdateCdnUrlCallback)callback;

/// Adds a target CDN URL to which the stream will be relayed from ZEGO RTC server.
///
/// Available since: 1.1.0
/// Description: Forward audio and video streams from ZEGO RTC servers to custom CDN content distribution networks with high latency but support for high concurrent pull streams.
/// Use cases: 1. It is often used in large-scale live broadcast scenes that do not have particularly high requirements for delay. 2. Since ZEGO RTC server itself can be configured to support CDN(content distribution networks), this function is mainly used by developers who have CDN content distribution services themselves. 3. This function supports dynamic relay to the CDN content distribution network, so developers can use this function as a disaster recovery solution for CDN content distribution services.
/// When to call: After calling the [createEngine] function to create the engine.
/// Restrictions: When the [enablePublishDirectToCDN] function is set to YES to publish the stream straight to the CDN, then calling this function will have no effect.
/// Caution: Removing URLs retweeted to CDN requires calling [removePublishCdnUrl], calling [stopPublishingStream] will not remove URLs publish to CDN.
/// Related APIs: Remove URLs that are re-pushed to the CDN [removePublishCdnUrl].
///
/// @param targetURL CDN relay address, supported address format is rtmp, rtmps.
/// @param streamID Stream ID.
/// @param timeout Timeout. Callback if it does not start in the time. Default is 0, which means no timeout. Valid range is [5, 600], in seconds. Less than 0 will be reset to 0, 1 to 4 will be reset to 5, and a greater than 600 will be reset to 600.
/// @param callback The execution result of update the relay CDN operation.
- (void)addPublishCdnUrl:(NSString *)targetURL
                streamID:(NSString *)streamID
                 timeout:(int)timeout
                callback:(nullable ZegoPublisherUpdateCdnUrlCallback)callback;

/// Deletes the specified CDN URL, which is used for relaying streams from ZEGO RTC server to CDN.
///
/// Available since: 1.1.0
/// Description: When a CDN forwarding address has been added via [addPublishCdnUrl], this function is called when the stream needs to be stopped.
/// When to call: After calling the [createEngine] function to create the engine, When you don't need to continue publish to the CDN.
/// Restrictions: When the [enablePublishDirectToCDN] function is set to YES to publish the stream straight to the CDN, then calling this function will have no effect.
/// Caution: This function does not stop publishing audio and video stream to the ZEGO ZEGO RTC server.
/// Related APIs: Add URLs that are re-pushed to the CDN [addPublishCdnUrl].
///
/// @param targetURL CDN relay address, supported address format rtmp.
/// @param streamID Stream ID.
/// @param callback The execution result of update the relay CDN operation.
- (void)removePublishCdnUrl:(NSString *)targetURL
                   streamID:(NSString *)streamID
                   callback:(nullable ZegoPublisherUpdateCdnUrlCallback)callback;

/// Whether to directly push to CDN (without going through the ZEGO RTC server).
///
/// Available since: 1.5.0
/// Description: Whether to publish streams directly from the client to CDN without passing through Zego RTC server.
/// Use cases: It is often used in large-scale live broadcast scenes that do not have particularly high requirements for delay.
/// Default value: The default is NO, and direct push is not enabled.
/// When to call: After creating the engine [createEngine], before starting to push the stream [startPublishingStream].
/// Caution: The Direct Push CDN feature does not pass through the ZEGO Real-Time Audio and Video Cloud during network transmission, so you cannot use ZEGO's ultra-low latency audio and video services.
/// Related APIs: Dynamic re-push to CDN function [addPublishCdnUrl], [removePublishCdnUrl].
///
/// @param enable Whether to enable direct publish CDN, YES: enable direct publish CDN, NO: disable direct publish CDN.
/// @param config CDN configuration, if nil, use Zego's background default configuration.
- (void)enablePublishDirectToCDN:(BOOL)enable config:(nullable ZegoCDNConfig *)config;

/// Whether to directly push to CDN (without going through the ZEGO RTC server), for the specified channel.
///
/// Available since: 1.5.0
/// Description: Whether to publish streams directly from the client to CDN without passing through Zego RTC server.
/// Use cases: It is often used in large-scale live broadcast scenes that do not have particularly high requirements for delay.
/// Default value: The default is NO, and direct push is not enabled.
/// When to call: After creating the engine [createEngine], before starting to push the stream [startPublishingStream].
/// Caution: The Direct Push CDN feature does not pass through the ZEGO Real-Time Audio and Video Cloud during network transmission, so you cannot use ZEGO's ultra-low latency audio and video services.
/// Related APIs: Dynamic re-push to CDN function [addPublishCdnUrl], [removePublishCdnUrl].
///
/// @param enable Whether to enable direct publish CDN, YES: enable direct publish CDN, NO: disable direct publish CDN.
/// @param config CDN configuration, if nil, use Zego's background default configuration.
/// @param channel Publish stream channel.
- (void)enablePublishDirectToCDN:(BOOL)enable
                          config:(nullable ZegoCDNConfig *)config
                         channel:(ZegoPublishChannel)channel;

/// Sets up the stream watermark before stream publishing.
///
/// Available since: 1.1.0
/// Description: Set watermark for publish stream screen.
/// Use cases: It is often used to identify the source of the publish.
/// When to call: After creating the engine through [createEngine] function.
/// Caution: The layout of the watermark cannot exceed the video encoding resolution of the stream. It can be set at any time before or during the publishing stream.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param watermark The upper left corner of the watermark layout is the origin of the coordinate system, and the area cannot exceed the size set by the encoding resolution. If it is nil, the watermark is cancelled.
/// @param isPreviewVisible Whether the watermark can be seen in the local preview.
- (void)setPublishWatermark:(nullable ZegoWatermark *)watermark
           isPreviewVisible:(BOOL)isPreviewVisible;

/// Sets up the stream watermark before stream publishing (for the specified channel).
///
/// Available since: 1.1.0
/// Description: Set watermark for publish stream screen.
/// Use cases: It is often used to identify the source of the publish.
/// When to call: After creating the engine through [createEngine] function.
/// Caution: The layout of the watermark cannot exceed the video encoding resolution of the stream. It can be set at any time before or during the publishing stream.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param watermark The upper left corner of the watermark layout is the origin of the coordinate system, and the area cannot exceed the size set by the encoding resolution. If it is nil, the watermark is cancelled.
/// @param isPreviewVisible the watermark is visible on local preview
/// @param channel Publish stream channel.
- (void)setPublishWatermark:(nullable ZegoWatermark *)watermark
           isPreviewVisible:(BOOL)isPreviewVisible
                    channel:(ZegoPublishChannel)channel;

/// Set the Supplemental Enhancement Information type.
///
/// Available since: 1.18.0
/// Description: By default, the SDK wraps the data with ZEGO's self-defined SEI type, which is not specified by the SEI standard. When the developer needs to use a third-party decoder to decode the SEI, the correct SEI will not be decoded and the [setSEIConfig] interface needs to be called to change the type of the SEI sent by the SDK to UserUnregister type.
/// Use cases: This function needs to be executed when the developer uses a third-party decoder to decode the SEI.
/// When to call: After creating the engine [createEngine], before starting to push the stream [startPublishingStream].
/// Restrictions: None.
///
/// @param config SEI configuration. The SEI defined by ZEGO is used by default.
- (void)setSEIConfig:(ZegoSEIConfig *)config;

/// Sends Supplemental Enhancement Information.
///
/// Available since: 1.1.0
/// Description: While pushing the stream to transmit the audio and video stream data, the stream media enhancement supplementary information is sent to synchronize some other additional information.
/// Use cases: Generally used in scenes such as synchronizing music lyrics or precise video layout, you can choose to send SEI.
/// When to call: After starting to push the stream [startPublishingStream].
/// Restrictions: Do not exceed 30 times per second, and the SEI data length is limited to 4096 bytes.
/// Caution: 1. Due to network issues, frame loss may occur, which means SEI information may also be lost. To address this situation, it is advisable to send it multiple times within a limited frequency. 2. Even if the [enableCamera] interface is called to turn off the camera or [mutePublishStreamVideo] is used to stop sending video data, SEI can still be successfully sent; as long as the playback side does not call the [mutePlayStreamVideo] interface to stop pulling audio data, SEI can still be received normally. 3. If the SDK does not support the video module but does support the SEI functional module, SEI information can still be sent normally.
/// Related APIs: After the pusher sends the SEI, the puller can obtain the SEI content by monitoring the callback of [onPlayerRecvSEI].
///
/// @param data SEI data.
- (void)sendSEI:(NSData *)data;

/// Sends Supplemental Enhancement Information to the specified publish channel.
///
/// Available since: 1.1.0
/// Description: While pushing the stream to transmit the audio and video stream data, the stream media enhancement supplementary information is sent to synchronize some other additional information.
/// Use cases: Generally used in scenes such as synchronizing music lyrics or precise video layout, you can choose to send SEI.
/// When to call: After starting to push the stream [startPublishingStream].
/// Restrictions: Do not exceed 30 times per second, and the SEI data length is limited to 4096 bytes.
/// Caution: 1. Due to network issues, frame loss may occur, which means SEI information may also be lost. To address this situation, it is advisable to send it multiple times within a limited frequency. 2. Even if the [enableCamera] interface is called to turn off the camera or [mutePublishStreamVideo] is used to stop sending video data, SEI can still be successfully sent; as long as the playback side does not call the [mutePlayStreamVideo] interface to stop pulling audio data, SEI can still be received normally. 3. If the SDK does not support the video module but does support the SEI functional module, SEI information can still be sent normally.
/// Related APIs: After the pusher sends the SEI, the puller can obtain the SEI content by monitoring the callback of [onPlayerRecvSEI].
///
/// @param data SEI data.
/// @param channel Publish stream channel.
- (void)sendSEI:(NSData *)data channel:(ZegoPublishChannel)channel;

/// Send the media supplementary enhancement information synchronized with the current video frame during custom video capture.
///
/// Available since: 2.15.0
/// Description: When using custom video capture, push the stream to transmit the video stream data, and send the stream media enhancement supplementary information to synchronize some other additional information, which is synchronized with the current video frame.
/// Use cases: Generally used in scenes that need to be played back in strong synchronization with the video frame, such as video karaoke, video and lyrics are strongly synchronized; or scenes that need to be accurate to the frame level for strategic processing.
/// When to call: To ensure that the SEI is synchronized with the current video frame, it must be called before sending the video frame data.
/// Restrictions: Only used for custom video capture; only supports video-driven SEI sending; you should try to avoid calling this interface to send SEI continuously; to ensure that SEI is synchronized with the current video frame, it must be kept in the same thread as the interface for sending custom video frame data; SEI data length is limited to 4096 bytes.
/// Caution: The sent SEI information follows the video frame. Due to network problems, there may be frame loss. At this time, the SEI will follow the next frame of video data. Therefore, in order to keep the SEI synchronized with the video frame, you should avoid sending SEI continuously; only when using SurfaceTexture on the Android platform , Need to pass the time stamp parameter timeStampNs, otherwise the parameter is invalid.
/// Related APIs: After the pusher sends the SEI, the puller can obtain the SEI content by monitoring the callback of [onPlayerRecvSEI].
///
/// @param data SEI data.
/// @param timeStampNs video frame reference time, UNIX timestamp, in nanosecond.
/// @param channel Publish stream channel.
- (void)sendSEISyncWithCustomVideo:(NSData *)data
                       timeStampNs:(unsigned long long)timeStampNs
                           channel:(ZegoPublishChannel)channel;

/// Send audio side information.
///
/// Available since: 2.19.0
/// Description: While streaming audio data, send audio side information to synchronize some additional information.
/// Use cases: In the carousel scene, the audio data channel is required to carry accompanying information, such as timestamps to help align the accompaniment, and the need to know who the user is currently singing, whether to amplify the volume, and so on.
/// When to call: After starting to push the stream [startPublishingStream].
/// Restrictions: 1. This function is only valid when publishing stream to the Zego RTC server and it also doesn't work when retweeting the stream from the RTC server to the CDN. 2. The audio side information data length is limited to 1024 bytes.
/// Caution: 1. Audio side information is driven by audio data, so audio data must be pushed (audio side information may be lost when the DTX function is enabled via the interface [setEngineConfig]). 2. Due to network issues, audio side information may be lost, and the SDK is responsible for transmission but does not guarantee reliability.
/// Related APIs: After the pusher sends the side information, the puller can obtain the side information content by monitoring the callback of [onPlayerRecvAudioSideInfo].
///
/// @param data Audio side info data.
/// @param timeStampMs timeStampMs, derived from custom audio processing, in milliseconds. If you fill in 0, it is sent along with the frame that is currently ready to be sent.
/// @param channel Publish stream channel.
- (void)sendAudioSideInfo:(NSData *)data
              timeStampMs:(double)timeStampMs
                  channel:(ZegoPublishChannel)channel;

/// Enables or disables hardware encoding.
///
/// Available since: 1.1.0
/// Description: Whether to use the hardware encoding function when publishing the stream, the GPU is used to encode the stream and to reduce the CPU usage.
/// When to call: The setting can take effect before the stream published. If it is set after the stream published, the stream should be stopped first before it takes effect.
/// Caution: Because hard-coded support is not particularly good for a few models, SDK uses software encoding by default. If the developer finds that the device is hot when publishing a high-resolution audio and video stream during testing of some models, you can consider calling this function to enable hard coding.
///
/// @param enable Whether to enable hardware encoding, YES: enable hardware encoding, NO: disable hardware encoding.
- (void)enableHardwareEncoder:(BOOL)enable;

/// Sets the timing of video scaling in the video capture workflow. You can choose to do video scaling right after video capture (the default value) or before encoding.
///
/// Available since: 1.1.0
/// When to call: This function needs to be set before call [startPreview] or [startPublishingStream].
/// Caution: The main effect is Whether the local preview is affected when the acquisition resolution is different from the encoding resolution.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param mode The capture scale timing mode.
- (void)setCapturePipelineScaleMode:(ZegoCapturePipelineScaleMode)mode;

/// Set the path of the static picture would be published when the camera is closed.
///
/// Available: since 2.9.0
/// Description: Set the path of the static picture would be published when enableCamera(NO) is called, it would start to publish static pictures, and when enableCamera(YES) is called, it would end publishing static pictures.
/// Use case: The developer wants to display a static picture when the camera is closed. For example, when the anchor exits the background, the camera would be actively closed. At this time, the audience side needs to display the image of the anchor temporarily leaving.
/// When to call: After the engine is initialized, call this API to configure the parameters before closing the camera.
/// Restrictions:
///   1. Supported picture types are JPEG/JPG, PNG, BMP, HEIF.
///   2. The function is only for SDK video capture and does not take effect for custom video capture.
///   3. Not supported that the filePath is a network link.
/// Caution:
///   1. The static picture cannot be seen in the local preview.
///   2. External filters, mirroring, watermarks, and snapshots are all invalid.
///   3. If the picture aspect ratio is inconsistent with the set code aspect ratio, it will be cropped according to the code aspect ratio.
/// Platform differences:
///   1. Windows: Fill in the location of the picture directly, such as "D://dir//image.jpg".
///   2. iOS: If it is a full path, add the prefix "file:", such as @"file:/var/image.png"; If it is a assets picture path, add the prefix "asset:", such as @"asset:watermark".
///   3. Android: If it is a full path, add the prefix "file:", such as "file:/sdcard/image.png"; If it is a assets directory path, add the prefix "asset:", such as "asset:watermark.png".
///   4. Flutter: If it is a absolute path, add the prefix "file:", such as "file:/sdcard/image.png"; If it is a assets resources directory path, add the prefix "flutter-asset://", such as "flutter-asset://assets/watermark.png".
///   5. UniApp: Only absolute paths are supported. You need to add a "file:" prefix, such as: "file:/sdcard/image.png".
///
/// @param filePath Picture file path
/// @param channel Publish channel.
- (void)setDummyCaptureImagePath:(NSString *)filePath channel:(ZegoPublishChannel)channel;

/// Set the params of the static picture would be published when the camera is closed.
///
/// Available: since 3.19.0
/// Description: Set the params of the static picture would be published when enableCamera(NO) is called, it would start to publish static pictures, and when enableCamera(YES) is called, it would end publishing static pictures.
/// Use case: The developer wants to display a static picture when the camera is closed. For example, when the anchor exits the background, the camera would be actively closed. At this time, the audience side needs to display the image of the anchor temporarily leaving.
/// When to call: After the engine is initialized, call this API to configure the parameters before closing the camera.
/// Restrictions:
///   1. Supported picture types are JPEG/JPG, PNG, BMP, HEIF.
///   2. The function is only for SDK video capture and does not take effect for custom video capture.
///   3. Not supported that the filePath is a network link.
/// Caution:
///   1. The static picture cannot be seen in the local preview.
///   2. External filters, mirroring, watermarks, and snapshots are all invalid.
///   3. If the picture aspect ratio is inconsistent with the set code aspect ratio, it will be cropped according to the code aspect ratio.
/// Platform differences:
///   1. Windows: Fill in the location of the picture directly, such as "D://dir//image.jpg".
///   2. iOS: If it is a full path, add the prefix "file:", such as @"file:/var/image.png"; If it is a assets picture path, add the prefix "asset:", such as @"asset:watermark".
///   3. Android: If it is a full path, add the prefix "file:", such as "file:/sdcard/image.png"; If it is a assets directory path, add the prefix "asset:", such as "asset:watermark.png".
///   4. Flutter: If it is a absolute path, add the prefix "file:", such as "file:/sdcard/image.png"; If it is a assets resources directory path, add the prefix "flutter-asset://", such as "flutter-asset://assets/watermark.png".
///   5. UniApp: Only absolute paths are supported. You need to add a "file:" prefix, such as: "file:/sdcard/image.png".
///
/// @param params Dummy capture image params.
/// @param channel Publish channel.
- (void)setDummyCaptureImageParams:(ZegoDummyCaptureImageParams *)params
                           channel:(ZegoPublishChannel)channel;

/// Whether to enable H.265 encoding to automatically downgrade to H.264 encoding.
///
/// Available since: 2.12.0
/// Description: When using H.265 encoding to push the stream, whether to enable the strategy of automatically degrading H.265 encoding to H.264 encoding under abnormal circumstances.After enabling automatic downgrade, when H.265 encoding is not supported or H.265 encoding fails, the SDK will try to downgrade and use H.264 encoding to push the stream.After turning off automatic downgrade, when H.265 encoding is not supported or H.265 encoding fails, the direct streaming fails.
/// Use cases: In the Co-hosting and Showroom Live Streaming scenarios, use H265 encoding to push the stream to save CDN traffic without degrading the picture quality.
/// Default Value: When this interface is not called, the default is yes, which means that H.265 encoding is turned on and automatically downgraded to H.264 encoding.
/// When to call: After creating the engine, call the [startPublishingStream] function before pushing the stream.
/// Related callbacks: When the H.265 encoding is automatically downgraded to the H.264 encoding strategy, the [onPublisherVideoEncoderChanged] callback will be triggered when the encoding method changes.
/// Caution: When downgrading from H.265 to H.264 encoding occurs during the streaming process, if you are recording local video or cloud recording, multiple recording files will be generated, which needs to be dealt with.
///
/// @param enable Whether to enable H.265 coding automatically fallback to H.264 coding, YES: enable, NO: disable, and the default value is YES
- (void)enableH265EncodeFallback:(BOOL)enable;

/// Whether the specified video encoding type is supported.
///
/// Available since: 2.12.0 and above
/// Description: Whether the specified video encoding is supported depends on the following aspects, whether the hardware model supports hard encoding, whether the performance of the hardware model supports soft encoding, and whether the SDK has the encoding module.
/// When to call: After creating the engine.
/// Caution: It is recommended that users call this interface to obtain H.265 encoding support capability before publish stream with H.265 encoding, if not supported, you can use other encodings for publish, such as H.264.On the mobile platform, the SDK only supports H.265 hardware encoding, and it is affected by the model and hardware capabilities.
///
/// @param codecID Video codec id. Required: Yes.
/// @return Whether the specified video encoding is supported.Value range: YES means support, you can use this encoding format for publish; NO means the is not supported, and the encoding format cannot be used for publish.
- (BOOL)isVideoEncoderSupported:(ZegoVideoCodecID)codecID;

/// Whether the specified video encoding type and implementation are supported.
///
/// Available since: 3.0.0 and above
/// Description: Whether the specified video encoding is supported depends on the following aspects, whether the hardware model supports hard encoding, whether the performance of the hardware model supports soft encoding, and whether the SDK has the encoding module.
/// When to call: After creating the engine.
/// Caution: It is recommended that users call this interface to obtain H.265 encoding support capability before publish stream with H.265 encoding, if not supported, you can use other encodings for publish, such as H.264.On the mobile platform, the SDK only supports H.265 hardware encoding, and it is affected by the model and hardware capabilities.
///
/// @param codecID Video codec id. Required: Yes.
/// @param codecBackend Backend implementation of encoder. Required: Yes.
/// @return Whether the specified video encoding format is supported; 0 means not supported, and the encoding format cannot be used for publish stream; 1 means support, you can use this encoding format for publish stream; 2 means not confirmed, it is recommended to call this interface later.
- (int)isVideoEncoderSupported:(ZegoVideoCodecID)codecID
                  codecBackend:(ZegoVideoCodecBackend)codecBackend;

#if TARGET_OS_IPHONE
/// Set the orientation mode of the video.
///
/// Available since: 2.23.0
/// Description: In order to simplify the complexity of processing video screen rotation for mobile developers, the SDK supports setting multiple video orientation modes, and developers can choose different modes according to the needs of the scene.
/// Use cases: Scenarios for live streaming or video calls using mobile devices.
/// Default value: Custom mode.
/// When to call: This function needs to be valid after calling [createEngine] and before calling preview [startPreview] or push stream [startPublishingStream].
/// Caution:
///   1. It is valid for all channels.
///   2. The adaptive mode takes effect in preview, streaming, and mixed streaming scenarios. It does not support external video capture, media player, cloud recording, local recording, or publishing/playing stream through CDN.
/// Related APIs: You can call the [setAppOrientation] function to set the orientation of the App in custom mode.
///
/// @param mode Orientation mode of the video.
- (void)setAppOrientationMode:(ZegoOrientationMode)mode;
#endif

/// Set video capture source.
///
/// Available since: 3.1.0
/// Description: Set video capture source for switching between different video capture sources.
/// Use cases: Typically used in educational scenarios that require switching between different video capture sources.
/// When to call: After the engine is created [createEngine].
/// Restrictions: None.
/// Caution: 1. Act on the main publish channel ZegoPublishChannel.Main.
///  2. Main push channel ZegoPublishChannel.Main does not support using ZegoVideoSourceType.Player and ZegoVideoSourceType.MainPublishChannel video source type.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param source Video capture source.
- (int)setVideoSource:(ZegoVideoSourceType)source;

/// Set a video capture instance as the video capture source.
///
/// Available since: 3.1.0
/// Description: Set video capture source for switching between different video capture sources.
/// Use cases: Typically used in educational scenarios that require switching between different video capture sources.
/// When to call: After the engine is created [createEngine].
/// Restrictions: None.
/// Caution: 1. Act on the main publish channel ZegoPublishChannel.Main.
///  2. Main push channel ZegoPublishChannel.Main does not support using ZegoVideoSourceType.Player and ZegoVideoSourceType.MainPublishChannel video source type.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param source Video capture source.
/// @param instanceID Video capture source instance id.
- (int)setVideoSource:(ZegoVideoSourceType)source instanceID:(unsigned int)instanceID;

/// Set video capture source for the specified channel.
///
/// Available since: 3.1.0
/// Description: Set video capture source for switching between different video capture sources.
/// Use cases: Typically used in educational scenarios that require switching between different video capture sources.
/// When to call: After the engine is created [createEngine].
/// Restrictions: None.
/// Caution: 1. Main push channel ZegoPublishChannel.Main does not support using ZegoVideoSourceType.Player and ZegoVideoSourceType.MainPublishChannel video source type.
///  2. When using ZegoVideoSourceType.Player and ZegoVideoSourceType.MainPublishChannel video source type in aux publish channel ZegoPublishChannel.Aux, must ensure that physical device works on main publish channel ZegoPublishChannel.Main.
///  3. Preemptive video sources are not allowed to be used on multiple channels at the same time, such as ZegoVideoSourceType.Camera and ZegoVideoSourceType.ScreenCapture.
///  4. The other publish channel can copy the main publish channel only when the main publish channel uses internal video capture. A maximum of one copy is supported.
///  5. When using ZegoVideoSourceType.Player video source type, please ensure that the ZegoMediaPlayer instance is created successfully.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param source Video capture source.
/// @param channel Publish stream channel.
- (int)setVideoSource:(ZegoVideoSourceType)source channel:(ZegoPublishChannel)channel;

/// Set a video capture instance as video capture source for the specified channel.
///
/// Available since: 3.1.0
/// Description: Set video capture source for switching between different video capture sources.
/// Use cases: Typically used in educational scenarios that require switching between different video capture sources.
/// When to call: After the engine is created [createEngine].
/// Restrictions: None.
/// Caution: 1. Main push channel ZegoPublishChannel.Main does not support using ZegoVideoSourceType.Player and ZegoVideoSourceType.MainPublishChannel video source type.
///  2. When using ZegoVideoSourceType.Player and ZegoVideoSourceType.MainPublishChannel video source type in aux publish channel ZegoPublishChannel.Aux, must ensure that physical device works on main publish channel ZegoPublishChannel.Main.
///  3. Preemptive video sources are not allowed to be used on multiple channels at the same time, such as ZegoVideoSourceType.Camera and ZegoVideoSourceType.ScreenCapture.
///  4. The other publish channel can copy the main publish channel only when the main publish channel uses internal video capture. A maximum of one copy is supported.
///  5. When using ZegoVideoSourceType.Player video source type, please ensure that the ZegoMediaPlayer instance is created successfully.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param source Video capture source.
/// @param instanceID Video capture source instance id.
/// @param channel Publish stream channel.
- (int)setVideoSource:(ZegoVideoSourceType)source
           instanceID:(unsigned int)instanceID
              channel:(ZegoPublishChannel)channel;

/// Set audio capture source.
///
/// Available since: 3.1.0
/// Description: Set audio capture source for switching between different audio capture sources.
/// Use cases: Typically used in educational scenarios that require switching between different audio capture sources.
/// When to call: After the engine is created [createEngine].
/// Restrictions: None.
/// Caution: 1. Act on the main publish channel ZegoPublishChannel.Main.
///  2. Main push channel ZegoPublishChannel.Main does not support using ZegoAudioSourceType.MediaPlayer and ZegoAudioSourceType.MainPublishChannel audio source type.
///
/// @param source Audio capture source.
- (int)setAudioSource:(ZegoAudioSourceType)source;

/// Set audio capture source for the specified channel.
///
/// Available since: 3.1.0
/// Description: Set audio capture source for switching between different audio capture sources.
/// Use cases: Typically used in educational scenarios that require switching between different audio capture sources.
/// When to call: After the engine is created [createEngine].
/// Caution: 1. Main push channel ZegoPublishChannel.Main does not support using ZegoAudioSourceType.MediaPlayer and ZegoAudioSourceType.MainPublishChannel audio source type.
///  2. When using ZegoAudioSourceType.MediaPlayer and ZegoAudioSourceType.MainPublishChannel audio source type in aux publish channel ZegoPublishChannel.Aux, must ensure that physical device works on main publish channel ZegoPublishChannel.Main.
///  3. Preemptive audio sources are not allowed to be used on multiple channels at the same time, such as ZegoAudioSourceType.Microphone.
///  4. When using ZegoAudioSourceType.MediaPlayer audio source type, please ensure that the ZegoMediaPlayer instance is created successfully.
///
/// @param source Audio capture source.
/// @param channel Publish stream channel.
- (int)setAudioSource:(ZegoAudioSourceType)source channel:(ZegoPublishChannel)channel;

/// Set audio capture source with audio mix config.
///
/// Available since: 3.1.0
/// Description: Set audio capture source for switching between different audio capture sources.
/// Use cases: Typically used in educational scenarios that require switching between different audio capture sources.
/// When to call: After the engine is created [createEngine].
/// Restrictions: None.
/// Caution: 1. Act on the main publish channel ZegoPublishChannel.Main.
///  2. Main push channel ZegoPublishChannel.Main does not support using ZegoAudioSourceType.MediaPlayer and ZegoAudioSourceType.MainPublishChannel audio source type.
///
/// @param source Audio capture source.
/// @param config Audio capture source mix config.
- (int)setAudioSource:(ZegoAudioSourceType)source config:(ZegoAudioSourceMixConfig *)config;

/// Enable video object segmentation.
///
/// Available since: 3.4.0
/// Description: Video object segmentation and transmission is a technology that separates the video object (in most cases, a person) in a rectangular video through an AI algorithm at the push end, transmits it in the RTC network, and renders it at the stream playing end.
/// Use cases: Scenes where the object in the video needs to be separated from the background, such as mixed reality, multi-person interaction scenes, and so on.
/// When to call:
///  1. After the engine is created [createEngine].
///  2. If you want to listen to the [onVideoObjectSegmentationStateChanged] callback, you need to call [startPublishingStream] or [startPreview].
/// Restrictions: Enabling object segmentation is a time-consuming operation, and it is not necessary to turn it on and off frequently.
/// Caution: This feature requires special braiding, please contact ZEGO Technical Support
/// Related callbacks: Listen for changes of the object segmentation state through [onVideoObjectSegmentationStateChanged].
/// Related APIs: Use [enableAlphaChannelVideoEncoder] to support the transparent background encoding of the divided object, and then publish the stream, you can render the object with a transparent background effect on the stream playing side.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param enable Whether to enable video object segmentation, off by default.
/// @param type The type of object segmentation.
/// @param channel Publish stream channel.
- (void)enableVideoObjectSegmentation:(BOOL)enable
                                 type:(ZegoObjectSegmentationType)type
                              channel:(ZegoPublishChannel)channel;

/// Enable video object segmentation.
///
/// Available since: 3.6.0
/// Description: Video object segmentation and transmission is a technology that separates the video object (in most cases, a person) in a rectangular video through an AI algorithm at the push end, transmits it in the RTC network, and renders it at the stream playing end.
/// Use cases: Scenes where the object in the video needs to be separated from the background, such as mixed reality, multi-person interaction scenes, and so on.
/// When to call: After the engine is created [createEngine].
/// Restrictions: Enabling object segmentation is a time-consuming operation, and it is not necessary to turn it on and off frequently.
/// Caution: This feature requires special braiding, please contact ZEGO Technical Support
/// Related callbacks: When the object segmentation is turned on or off, the notification of the object segmentation state can be received through [onVideoObjectSegmentationStateChanged].
/// Related APIs: Use [enableAlphaChannelVideoEncoder] to support the transparent background encoding of the divided object, and then publish the stream, you can render the object with a transparent background effect on the stream playing side.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param enable Whether to enable video object segmentation, off by default.
/// @param config The type of object segmentation.
/// @param channel Publish stream channel.
- (void)enableVideoObjectSegmentation:(BOOL)enable
                               config:(ZegoObjectSegmentationConfig *)config
                              channel:(ZegoPublishChannel)channel;

/// Enable video encoder alpha channel support.
///
/// Available since: 3.4.0
/// Description: Enable the alpha channel support of the video encoder on the stream publishing end, and encode the split video body for streaming.
/// Use cases: Scenes where the object in the video needs to be separated from the background, such as mixed reality, multi-person interaction scenes, and so on.
/// When to call: After creating the engine.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param enable Enable video encoder alpha channel support, off by default.
/// @param alphaLayout Specify the layout position of the alpha channel data.
/// @param channel Publish stream channel.
- (void)enableAlphaChannelVideoEncoder:(BOOL)enable
                           alphaLayout:(ZegoAlphaLayoutType)alphaLayout
                               channel:(ZegoPublishChannel)channel;

/// Set the camera stabilization mode.
///
/// Available since: 3.13.0
/// Description: Set the camera stabilization mode.
/// Use case: Users can use this function to set the camera stabilization mode to reduce the impact of camera shake on video collection.
/// When to call: Called after the engine is created [createEngine].
/// Restrictions: Only supports internal video capture.
/// Platform differences: Only supports iPhone and Android.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param mode The camera stabilization mode. \niOS:See Apple AVCaptureVideoStabilizationMode definition for details. The default value is 0. \nAndroid:AUTO:-1, OFF:0. The default value is 0.
/// @param channel Publish stream channel.
- (void)setCameraStabilizationMode:(int)mode channel:(ZegoPublishChannel)channel;

/// Turn on or off the adaptive mode to adjust the volume of the human voice according to the volume of the BGM.
///
/// Available since: 3.18.0
/// Description: Turn on or off the adaptive mode to adjust the volume of the human voice according to the volume of the BGM to balance the volume of the human voice and the BGM. Default is off.
/// When to call: Called after the engine is created [createEngine].
/// Restrictions: This interface will take effect only when the [EnableAux] interface of the media player is called to enable aux.
///
/// @param enable Turn on or off the adaptive mode to adjust the volume of the human voice according to the volume of the BGM.
- (void)enableAuxBgmBalance:(BOOL)enable;

/// Turn on or off the face detection.
///
/// Available since: 3.20.0
/// Description: Turn on or off the face detection. Default is on.
/// When to call: Called after the engine is created [createEngine].
/// Related callbacks: Detect results will be called back through [onPublisherFaceDetectInfo].
///
/// @param enable Turn on or off the face detection.
/// @param channel Publish stream channel.
- (void)enableFaceDetection:(BOOL)enable channel:(ZegoPublishChannel)channel;

@end

NS_ASSUME_NONNULL_END
