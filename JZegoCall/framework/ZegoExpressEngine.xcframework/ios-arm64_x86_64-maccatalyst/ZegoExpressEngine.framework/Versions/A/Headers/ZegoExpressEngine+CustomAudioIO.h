//
//  ZegoExpressEngine+CustomAudioIO.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (CustomAudioIO)

/// Enable local collection and custom audio processing(before ear return).
///
/// Available since: 1.13.0
/// Description: Enable custom audio processing(before ear return), developers can receive locally collected audio frames through [onProcessCapturedAudioData], and can modify the audio data.
/// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing after the audio data is collected or before the remote audio data is drawn for rendering.
/// When to call: It needs to be called before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager] to be effective.
/// Restrictions: None.
///
/// @param enable Whether to enable local capture custom audio processing.
/// @param config Custom audio processing configuration.
- (void)enableCustomAudioCaptureProcessing:(BOOL)enable
                                    config:(ZegoCustomAudioProcessConfig *)config;

/// Turn on local collection and custom audio processing (after ear return).
///
/// Available since: 1.13.0
/// Description: Enable custom audio processing(after ear return), developers can receive locally collected audio frames through [onProcessCapturedAudioData], and can modify the audio data.
/// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing after the audio data is collected or before the remote audio data is drawn for rendering.
/// When to call: It needs to be called before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager] to be effective.
/// Restrictions: None.
///
/// @param enable Whether to enable local capture custom audio processing.
/// @param config Custom audio processing configuration.
- (void)enableCustomAudioCaptureProcessingAfterHeadphoneMonitor:(BOOL)enable
                                                         config:
                                                             (ZegoCustomAudioProcessConfig *)config;

/// Enable feature of throwing audio aux frames which aligned with accompany.
///
/// Available since: 2.22.0
/// Description: Enable feature of throwing audio aux frames which aligned with accompany, and developers can receive the aligned audio aux frame through [onAlignedAudioAuxData].
/// Use cases: In KTV scene, this function can be used if the user wants to record voice and accompaniment for free processing.
/// When to call: It needs to be called after [createEngine] to be effective.
/// Restrictions: None.
/// Caution: When throwing onAlignedAudioAuxData audio aux frames is enabled, the streaming data of [startPublishingStream] does not contain audio aux frames.
///
/// @param enable Whether to enable the feature of throwing alignmented audio aux frames.
/// @param param param of audio frame. Currently supports 8k, 16k, 32k, 44.1k, 48k sampling rate, mono or stereo.
- (void)enableAlignedAudioAuxData:(BOOL)enable param:(ZegoAudioFrameParam *)param;

/// Enable the feature of throwing audio data before SDK internal audio preprocessing.
///
/// Available since: 3.11.0
/// Description: Enables the feature of throwing audio data before SDK internal audio preprocessing, and developers can receive audio data before SDK internal audio preprocessing through [onBeforeAudioPrepAudioData].
/// When to call: It needs to be called after [createEngine] to be effective.
/// Restrictions: None.
///
/// @param enable Whether to enable feature of throwing audio data before SDK internal audio preprocessing.
/// @param param param of audio frame. Currently, it supports sampling rates of 0, 16k, 32k, 44.1k, and 48k. The 0 means using SDK internal value. It supports channels Unknown, Mono, and Stereo. The Unknown means using SDK internal value.
- (void)enableBeforeAudioPrepAudioData:(BOOL)enable param:(ZegoAudioFrameParam *)param;

/// Enable custom audio processing for remote playing stream.
///
/// Available since: 1.13.0
/// Description: Enable remote streaming custom audio processing, developers can receive remote streaming audio frames through [onProcessRemoteAudioData], and can modify the audio data.
/// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing before pulling the remote audio data for rendering.
/// When to call: It needs to be called before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager] to be effective.
/// Restrictions: None.
///
/// @param enable Whether to enable custom audio processing for remote playing stream.
/// @param config Custom audio processing configuration.
- (void)enableCustomAudioRemoteProcessing:(BOOL)enable
                                   config:(ZegoCustomAudioProcessConfig *)config;

/// Enable custom audio processing for SDK playback audio data.
///
/// Available since: 1.13.0
/// Description: Enable remote streaming custom audio processing, developers can receive remote streaming audio frames through [onProcessRemoteAudioData], and can modify the audio data.
/// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing after collecting audio data.
/// When to call: It needs to be called before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager] to be effective.
/// Restrictions: None.
///
/// @param enable Whether to enable custom audio processing for SDK playback audio data.
/// @param config Custom audio processing configuration.
- (void)enableCustomAudioPlaybackProcessing:(BOOL)enable
                                     config:(ZegoCustomAudioProcessConfig *)config;

/// Set up callback handler for custom audio processing.
///
/// Available since: 1.13.0
/// Description: When the custom audio processing is enabled, the custom audio processing callback is set through this function, and the developer can modify the processed audio frame data in the callback.
/// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing after the audio data is collected or before the remote audio data is drawn for rendering.
/// When to call: After creating the engine.
/// Restrictions: None.
///
/// @param handler Callback handler for custom audio processing.
- (void)setCustomAudioProcessHandler:(nullable id<ZegoCustomAudioProcessHandler>)handler;

/// Enable audio data observering.
///
/// Available since: 1.1.0
/// Description: When custom audio processing is turned on, use this function to turn on audio data callback monitoring.
/// Use cases: When develop need to monitor the original audio.
/// When to call: After creating the engine.
/// Restrictions: Audio monitoring is triggered only after this function has been called and the callback has been set by calling [setAudioDataHandler]. If you want to enable the [onPlayerAudioData] callback, you must also be in the pull stream, and the incoming sampling rate of the [startAudioDataObserver] function is not supported at 8000Hz, 22050Hz, and 24000Hz.
/// Caution: This api will start the media engine and occupy the microphone device.
///
/// @param observerBitMask The callback function bitmask marker for receive audio data, refer to enum [ZegoAudioDataCallbackBitMask], when this param converted to binary, 0b01 that means 1 << 0 for triggering [onCapturedAudioData], 0x10 that means 1 << 1 for triggering [onPlaybackAudioData], 0x100 that means 1 << 2 for triggering [onMixedAudioData], 0x1000 that means 1 << 3 for triggering [onPlayerAudioData]. The masks can be combined to allow different callbacks to be triggered simultaneously.
/// @param param param of audio frame.
- (void)startAudioDataObserver:(ZegoAudioDataCallbackBitMask)observerBitMask
                         param:(ZegoAudioFrameParam *)param;

/// Disable audio data observering.
///
/// Available since: 1.1.0
/// Description: Disable audio data observering.
/// Use cases: When develop need to monitor the original audio.
/// When to call: After calling [startAudioDataObserver] to start audio data monitoring.
- (void)stopAudioDataObserver;

/// Sets up the event callback handler for receiving audio data.
///
/// Available since: 1.1.0
/// Description: This function can be called to receive audio data thrown by SDK bypass.
/// Use cases: When the developer needs to obtain the audio data of the remote user or the data collected by the local microphone for other purposes (such as pure audio recording, pure audio third-party monitoring, pure audio real-time analysis).
/// When to call: After creating the engine.
/// Restrictions: The set callback needs to be effective after calling [startAudioDataObserver] and is in the push or pull state.
/// Related APIs: Turn on the audio data monitoring call [startAudioDataObserver], turn off the audio data monitoring call [stopAudioDataObserver].
/// Caution: The format of the thrown audio data is pcm. The SDK still controls the collection and playback of the sound device. This function is to copy a copy of the data collected or played inside the SDK for use by the developer.
///
/// @param handler Audio data handler for receive audio data.
- (void)setAudioDataHandler:(nullable id<ZegoAudioDataHandler>)handler;

/// Enables the custom audio I/O function.
///
/// Available since: 1.10.0
/// Description: Enable custom audio IO function, support PCM, AAC format data.
/// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing after the audio data is collected or before the remote audio data is drawn for rendering.
/// When to call: It needs to be called before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager] to be effective.
/// Restrictions: None.
///
/// @param enable Whether to enable custom audio IO, default is NO.
/// @param config Custom audio IO config.
- (void)enableCustomAudioIO:(BOOL)enable config:(nullable ZegoCustomAudioConfig *)config;

/// Enables the custom audio I/O function (for the specified channel), support PCM, AAC format data.
///
/// Available since: 1.10.0
/// Description: Enable custom audio IO function, support PCM, AAC format data.
/// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing after the audio data is collected or before the remote audio data is drawn for rendering.
/// When to call: It needs to be called before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager] to be effective.
/// Restrictions: None.
///
/// @param enable Whether to enable custom audio IO, default is NO.
/// @param config Custom audio IO config.
/// @param channel Specify the publish channel to enable custom audio IO.
- (void)enableCustomAudioIO:(BOOL)enable
                     config:(nullable ZegoCustomAudioConfig *)config
                    channel:(ZegoPublishChannel)channel;

/// Sends AAC audio data produced by custom audio capture to the SDK (for the specified channel).
///
/// Available since: 2.20.0
/// Description: Sends the captured audio AAC data to the SDK.
/// Use cases: The customer needs to obtain input after acquisition from the existing audio stream, audio file, or customized acquisition system, and hand it over to the SDK for transmission.
/// When to call: After [enableCustomAudioIO] and publishing stream successfully.
/// Restrictions: None.
/// Related APIs: Enable the custom audio IO function [enableCustomAudioIO], and start the push stream [startPublishingStream].
///
/// @param data AAC buffer data.
/// @param dataLength The total length of the buffer data.
/// @param configLength The length of AAC specific config (Note: The AAC encoded data length is 'encodedLength = dataLength - configLength').Value range: [0,64]
/// @param timestamp The UNIX timestamp of this AAC audio frame.
/// @param samples The number of samples for this AAC audio frame.Value range: [480,512,1024,1960,2048].
/// @param param The param of this AAC audio frame.
/// @param channel Publish channel for capturing audio frames.
- (void)sendCustomAudioCaptureAACData:(unsigned char *)data
                           dataLength:(unsigned int)dataLength
                         configLength:(unsigned int)configLength
                            timestamp:(CMTime)timestamp
                              samples:(unsigned int)samples
                                param:(ZegoAudioFrameParam *)param
                              channel:(ZegoPublishChannel)channel;

/// Sends PCM audio data produced by custom audio capture to the SDK.
///
/// Available since: 1.10.0
/// Description: Sends the captured audio PCM data to the SDK.
/// Use cases: 1.The customer needs to obtain input after acquisition from the existing audio stream, audio file, or customized acquisition system, and hand it over to the SDK for transmission. 2.Customers have their own requirements for special sound processing for PCM input sources. After the sound processing, the input will be sent to the SDK for transmission.
/// When to call: After [enableCustomAudioIO] and publishing stream successfully.
/// Restrictions: None.
/// Related APIs: Enable the custom audio IO function [enableCustomAudioIO], and start the push stream [startPublishingStream].
///
/// @param data PCM buffer data.
/// @param dataLength The total length of the buffer data.
/// @param param The param of this PCM audio frame.
- (void)sendCustomAudioCapturePCMData:(unsigned char *)data
                           dataLength:(unsigned int)dataLength
                                param:(ZegoAudioFrameParam *)param;

/// Sends PCM audio data produced by custom audio capture to the SDK (for the specified channel).
///
/// Available since: 1.10.0
/// Description: Sends the captured audio PCM data to the SDK.
/// Use cases: 1.The customer needs to obtain input after acquisition from the existing audio stream, audio file, or customized acquisition system, and hand it over to the SDK for transmission. 2.Customers have their own requirements for special sound processing for PCM input sources. After the sound processing, the input will be sent to the SDK for transmission.
/// When to call: After [enableCustomAudioIO] and publishing stream successfully.
/// Restrictions: None.
/// Related APIs: Enable the custom audio IO function [enableCustomAudioIO], and start the push stream [startPublishingStream].
///
/// @param data PCM buffer data.
/// @param dataLength The total length of the buffer data.
/// @param param The param of this PCM audio frame.
/// @param channel Publish channel for capturing audio frames.
- (void)sendCustomAudioCapturePCMData:(unsigned char *)data
                           dataLength:(unsigned int)dataLength
                                param:(ZegoAudioFrameParam *)param
                              channel:(ZegoPublishChannel)channel;

/// Fetches PCM audio data of the remote stream from the SDK for custom audio rendering.
///
/// Available since: 1.10.0
/// Description: Fetches PCM audio data of the remote stream from the SDK for custom audio rendering, it is recommended to use the system framework to periodically invoke this function to drive audio data rendering.
/// Use cases: When developers have their own rendering requirements, such as special applications or processing and rendering of the original PCM data that are pulled, it is recommended to use the custom audio rendering function of the SDK.
/// When to call: After [enableCustomAudioIO] and playing stream successfully.
/// Restrictions: None.
/// Related APIs: Enable the custom audio IO function [enableCustomAudioIO], and start the play stream [startPlayingStream].
///
/// @param data A block of memory for storing audio PCM data that requires user to manage the memory block's lifecycle, the SDK will copy the audio frame rendering data to this memory block.
/// @param dataLength The length of the audio data to be fetch this time (dataLength = duration * sampleRate * channels * 2(16 bit depth i.e. 2 Btye)).
/// @param param Specify the parameters of the fetched audio frame. sampleRate in ZegoAudioFrameParam must assignment
- (void)fetchCustomAudioRenderPCMData:(unsigned char *)data
                           dataLength:(unsigned int)dataLength
                                param:(ZegoAudioFrameParam *)param;

/// Send the PCM audio data customized by the developer to the SDK, which is used as a reference for custom rendering audio to eliminate echo.
///
/// Available since：2.13.0.
/// Description：Developers use the audio device clock as the driver to capture PCM audio data, and use it for custom audio rendering after processing. When submitting for rendering, call this function to send the processed audio data back to the SDK so that the SDK can use it as an echo cancellation reference.
/// Use cases：In entertainment scenarios, it may be necessary to customize the processing of PCM audio data from the remote end, such as synthesizing a background sound and KTV accompaniment before rendering and playing. At the same time, developers are required to send the audio data processed by themselves to the SDK for reference, so that the processed sound effects can be echo canceled after collection.
/// When to call：After calling [fetchCustomAudioRenderPCMData] to fetch and process the PCM audio data, this function is called while submitting to the system for rendering and playback.
/// Restrictions：You must call [setEngineConfig] to enable the external audio data as a reference for this function to take effect. If you need to get the use of the function or the details, please consult ZEGO technical support.
///
/// @param data PCM buffer data
/// @param dataLength The total length of the buffer data
/// @param param The param of this PCM audio frame
- (void)sendReferenceAudioPCMData:(unsigned char *)data
                       dataLength:(unsigned int)dataLength
                            param:(ZegoAudioFrameParam *)param;

@end

NS_ASSUME_NONNULL_END
