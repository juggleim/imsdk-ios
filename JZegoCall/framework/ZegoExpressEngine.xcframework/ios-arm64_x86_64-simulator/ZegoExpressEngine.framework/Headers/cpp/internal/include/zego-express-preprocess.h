
#ifndef __ZEGO_EXPRESS_PREPROCESS_H__
#define __ZEGO_EXPRESS_PREPROCESS_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Set low light enhancement.
///
/// Available since: 2.21.0
/// Description: According to the set low-light enhancement mode, the brightness of the image captured by the camera is enhanced, which is compatible with the beauty function. Users can watch the effect while previewing and toggle the low-light enhancement mode in real time.
/// Use cases: The environment on the streaming end is dark, or the frame rate set by the camera is high, which causes the picture to be dark, and the subject cannot be displayed or recognized normally.
/// Default value: Off.
/// When to call: After creating the engine [createEngine].
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param mode Low light enhancement mode.
/// @param channel Publish stream channel.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_low_light_enhancement(
    enum zego_low_light_enhancement_mode mode, enum zego_publish_channel channel);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_low_light_enhancement)(
    enum zego_low_light_enhancement_mode mode, enum zego_publish_channel channel);
#endif

/// Set low light enhancement params.
///
/// Available since: 3.19.0
/// Description: According to the set low-light enhancement mode, the brightness of the image captured by the camera is enhanced, which is compatible with the beauty function. Users can watch the effect while previewing and toggle the low-light enhancement mode in real time.
/// Use cases: The environment on the streaming end is dark, or the frame rate set by the camera is high, which causes the picture to be dark, and the subject cannot be displayed or recognized normally.
/// When to call: After creating the engine [createEngine].
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param params Low light enhancement params.
/// @param channel Publish stream channel.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_low_light_enhancement_params(
    struct zego_exp_low_light_enhancement_params params, enum zego_publish_channel channel);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_low_light_enhancement_params)(
    struct zego_exp_low_light_enhancement_params params, enum zego_publish_channel channel);
#endif

/// Set video denoise params.
///
/// Available since: 3.18.0
/// Description: Set video denoise parameters, including mode and strength.
/// Default value: Off.
/// When to call: After creating the engine [createEngine].
/// Platform differences: Only supports iOS and Android.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param params Video denoise params.
/// @param channel Publish stream channel.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_video_denoise_params(
    struct zego_video_denoise_params params, enum zego_publish_channel channel);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_video_denoise_params)(
    struct zego_video_denoise_params params, enum zego_publish_channel channel);
#endif

/// Whether to enable acoustic echo cancellation (AEC).
///
/// Available since: 1.1.0
/// Description: Turning on echo cancellation, the SDK filters the collected audio data to reduce the echo component in the audio.
/// Use case: When you need to reduce the echo to improve the call quality and user experience, you can turn on this feature.
/// When to call: It needs to be called after [createEngine].
/// Caution: The AEC function only supports the processing of sounds playbacked through the SDK, such as sounds played by the playing stream, media player, audio effect player, etc. Before this function is called, the SDK automatically determines whether to use AEC. Once this function is called, the SDK does not automatically determine whether to use AEC.
/// Restrictions: None.
/// Related APIs: Developers can use [enableHeadphoneAEC] to set whether to enable AEC when using headphones, and use [setAECMode] to set the echo cancellation mode.
///
/// @param enable Whether to enable echo cancellation, true: enable, false: disable
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_aec(bool enable);
#else
typedef zego_error(EXP_CALL *pfnzego_express_enable_aec)(bool enable);
#endif

/// Whether to turn on acoustic echo cancellation (AEC) when using the headphone.
///
/// Available since: 1.1.0
/// Description: When [enableAEC] is used to turn on echo cancellation, it is only turned on when the speaker is used for mobile terminal equipment. Call this function if you need to turn echo cancellation on or off when using the headset.
/// Use case: It is common when the mobile device is connected to a external sound card as the audio output source. In order to eliminate the echo in this case, you need to call this function to turn on the echo cancellation.
/// Default value: Android is off by default and iOS is on by default.
/// When to call: It needs to be called after [createEngine], before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager].
/// Caution: Turning on echo cancellation will increase the ear return delay. On the iOS platform, the SDK cannot distinguish between the headset and the external sound card. If you use this function to turn off the system echo cancellation when using the headset, the sound played by the external sound card will be collected when the user accesses the external sound card.
/// Restrictions: None.
/// Related APIs: When the headset is not used, you can set whether the SDK turns on echo cancellation through [enableAEC].
/// Platform differences: Only supports iOS and Android.
///
/// @param enable Whether to enable, true: enable, false: disable
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_headphone_aec(bool enable);
#else
typedef zego_error(EXP_CALL *pfnzego_express_enable_headphone_aec)(bool enable);
#endif

/// Sets the acoustic echo cancellation (AEC) mode.
///
/// Available since: 1.1.0
/// Description: When [enableAEC] is used to enable echo cancellation, this function can be used to switch between different echo cancellation modes to control the degree of echo cancellation.
/// Use case: When the default echo cancellation effect does not meet expectations, this function can be used to adjust the echo cancellation mode.
/// Default value: When this function is not called, the default echo cancellation mode is [Aggressive].
/// When to call: It needs to be called after [createEngine].
/// Restrictions: The value set by this function is valid only after the echo cancellation function is turned on.
///
/// @param mode Echo cancellation mode
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_aec_mode(enum zego_aec_mode mode);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_aec_mode)(enum zego_aec_mode mode);
#endif

/// Enables or disables automatic gain control (AGC).
///
/// Available since: 1.1.0
/// Description: After turning on this function, the SDK can automatically adjust the microphone volume to adapt to near and far sound pickups and keep the volume stable.
/// Use case: When you need to ensure volume stability to improve call quality and user experience, you can turn on this feature.
/// When to call: It needs to be called after [createEngine].
/// Caution: Before this function is called, the SDK automatically determines whether to use AGC. Once this function is called, the SDK does not automatically determine whether to use AGC.
/// Restrictions: None.
///
/// @param enable Whether to enable automatic gain control, true: enable, false: disable
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_agc(bool enable);
#else
typedef zego_error(EXP_CALL *pfnzego_express_enable_agc)(bool enable);
#endif

/// Enables or disables active noise suppression (ANS, aka ANC).
///
/// Available since: 1.1.0
/// Description: Enable the noise suppression can reduce the noise in the audio data and make the human voice clearer.
/// Use case: When you need to suppress noise to improve call quality and user experience, you can turn on this feature.
/// When to call: It needs to be called after [createEngine].
/// Related APIs: This function has a better suppression effect on continuous noise (such as the sound of rain, white noise). If you need to turn on transient noise suppression, please use [enableTransientANS]. And the noise suppression mode can be set by [setANSMode].
/// Caution: Before this function is called, the SDK automatically determines whether to use ANS. Once this function is called, the SDK does not automatically determine whether to use ANS.
/// Restrictions: None.
///
/// @param enable Whether to enable noise suppression, true: enable, false: disable
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_ans(bool enable);
#else
typedef zego_error(EXP_CALL *pfnzego_express_enable_ans)(bool enable);
#endif

/// Enables or disables transient noise suppression.
///
/// Available since: 1.17.0
/// Description: Enable the transient noise suppression can suppress the noises such as keyboard and desk knocks.
/// Use case: When you need to suppress transient noise to improve call quality and user experience, you can turn on this feature.
/// Default value: When this function is not called, this is disabled by default.
/// When to call: It needs to be called after [createEngine].
/// Related APIs: This function will not suppress normal noise after it is turned on. If you need to turn on normal noise suppression, please use [enableANS].
/// Restrictions: None.
///
/// @param enable Whether to enable transient noise suppression, true: enable, false: disable
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_transient_ans(bool enable);
#else
typedef zego_error(EXP_CALL *pfnzego_express_enable_transient_ans)(bool enable);
#endif

/// Sets the automatic noise suppression (ANS) mode.
///
/// Available since: 1.1.0
/// Description: When [enableANS] is used to enable noise suppression, this function can be used to switch between different noise suppression modes to control the degree of noise suppression.
/// Use case: When the default noise suppression effect does not meet expectations, this function can be used to adjust the noise suppression mode.
/// Default value: When this function is not called, the default automatic noise suppression (ANS) mode is [Medium].
/// When to call: It needs to be called after [createEngine].
/// Restrictions: The value set by this function is valid only after the noise suppression function is turned on.
///
/// @param mode Audio Noise Suppression mode
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_ans_mode(enum zego_ans_mode mode);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_ans_mode)(enum zego_ans_mode mode);
#endif

/// Enables or disables speech enhancement.
///
/// Available since: 3.3.0.
/// Description: Enable the speech enhancement function.
/// Use case: In the KTV external playback scene, due to 3A damage and distance, the voice is weak or empty or unclear, so it is necessary to enhance the external playback experience through the voice enhancement technology.
/// Default value: When this function is not called, this is disabled by default.
/// When to call: It needs to be called after [createEngine] and it Supports dynamic change.
/// Caution: 1. If this interface is called repeatedly, the settings of the last call are used;
///  2. After call [DestroyEngine], the Settings fail.
/// Restrictions: None.
///
/// @param enable Whether to enable speech enhancement, true: enable, false: disable
/// @param level Enhancement level; The value range is [0, 10]
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_speech_enhance(bool enable, int level);
#else
typedef zego_error(EXP_CALL *pfnzego_express_enable_speech_enhance)(bool enable, int level);
#endif

/// Enables or disables audio mixing function.
///
/// Available since: 1.9.0, will be deprecated soon. It is recommended to use the media player [createMediaPlayer] or the audio effect player [createAudioEffectPlayer] to implement the audio mixing function.
/// Description: After the audio mixing function is turned on, the SDK will mix the audio data prepared by the developer with the audio data collected by the SDK before publishing.
/// Use case: Developers can use this function when they need to mix their own songs, sound effects or other audio data into the publishing stream.
/// Default value: When this function is not called, this is disabled by default.
/// When to call: It needs to be called after [createEngine].
/// Related APIs: After enabling audio mixing, the developer also needs to call [setAudioMixingHandler] to set the audio mixing callback, so as to provide the SDK with the audio data that needs to be mixed in the [onAudioMixingCopyData] callback.
/// Caution: After you start audio mixing, the SDK will play the mixed audio on the local (publisher side) by default. If you do not want to play it locally but only on the remote (player side), please call [muteLocalAudioMixing] to set the local audio mixing mute. Recommend to use audio effect player or media player for audio mixing.
/// Restrictions: None.
///
/// @param enable Whether to enable audio mixting, true: enable, false: disable
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_audio_mixing(bool enable);
#else
typedef zego_error(EXP_CALL *pfnzego_express_enable_audio_mixing)(bool enable);
#endif

/// Mute or resume local playback of the mixing audio.
///
/// Available since: 1.9.0
/// Description: When calling this function to mute the local playback audio mixing, the local (publisher side) will not play the audio provided to the SDK through [onAudioMixingCopyData], but the remote (player side) can still playback it Mix.
/// Use case: When developers need to mix their own songs, sound effects or other audio data into the publishing stream, but the mixed audio only wants to be heard by the remote player, and does not want to be playback locally, you can use this function.
/// Default value: When this function is not called, the default is not mute, which is false.
/// When to call: It needs to be called after [createEngine].
/// Restrictions: None.
///
/// @param mute Whether to mute local audio mixting, true: mute, false: unmute
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_mute_local_audio_mixing(bool mute);
#else
typedef zego_error(EXP_CALL *pfnzego_express_mute_local_audio_mixing)(bool mute);
#endif

/// Sets the audio mixing output volume for both local playback and the stream to be published.
///
/// Available since: 1.9.0
/// Description: After enabling the audio mixing function through [enableAudioMixing], the developer can use this function to set the volume of the mixing audio to be mixed to SDK.
/// Use case: When developers need to mix their own songs, sound effects or other audio data into the publishing stream, and want to adjust the volume of the mixing audio, you can use this function.
/// When to call: It needs to be called after [createEngine].
/// Caution: This function will set both the mixing audio volume of the local playback and the remote playback at the same time.
/// Related APIs: If you need to set the mixing audio volume for local playback or remote playback separately, please use the function of the same name with the [type] parameter.
/// Restrictions: None.
///
/// @param volume The audio mixing volume, range from 0 to 200, 100 as default.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_audio_mixing_volume(int volume);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_audio_mixing_volume)(int volume);
#endif

/// Sets the audio mixing output volume for either local playback or the stream to published.
///
/// Available since: 1.9.0
/// Description: After enabling the audio mixing function through [enableAudioMixing], the developer can use this function to set the volume of the mixing audio to be mixed to SDK.
/// Use case: When developers need to mix their own songs, sound effects or other audio data into the publishing stream, and want to adjust the volume of the mixing audio, you can use this function.
/// When to call: It needs to be called after [createEngine].
/// Caution: This function can individually set the mixing audio volume of the local playback or the remote playback.
/// Restrictions: None.
///
/// @param volume The audio mixing volume, range from 0 to 200, 100 as default.
/// @param type Local playback volume / Remote playback volume
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_set_audio_mixing_volume_with_type(int volume, enum zego_volume_type type);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_audio_mixing_volume_with_type)(
    int volume, enum zego_volume_type type);
#endif

/// Enable the Effects beauty environment.
///
/// Available since: 2.16.0
/// Description: Enable the Effects beauty environment. The SDK uses the specified video frame data type for transmission. The Windows platform only supports video frame raw data, the Apple platform only supports CVPixelBuffer, and the Android platform only supports texture2d.
/// Use cases: It is often used in scenes such as video calls and live broadcasts.
/// Default value: When this function is not called, the beauty environment is not activated by default.
/// When to call: Must be called before calling [startPreview], [startPublishingStream]. If you need to modify the configuration, please call [logoutRoom] to log out of the room first, otherwise it will not take effect.
/// Related APIs: [enableEffectsBeauty] switch beauty, [setEffectsBeautyParam] set beauty parameters.
/// Caution: This beauty function is the basic function. If it does not meet the expectations of the developer, you can use the custom video pre-processing function [enableCustomVideoProcessing] or the custom video capture function [enableCustomVideoCapture] docking and constructing the AI ​​Effects SDK [ZegoEffects] https://doc-zh.zego.im/article/9556 for best results.
/// Restrictions: This function only supports Android system 5.0 and above, Android SDK version 21 and above.
/// Note: This function is only available in ZegoExpressVideo SDK!
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_start_effects_env();
#else
typedef zego_error(EXP_CALL *pfnzego_express_start_effects_env)();
#endif

/// Disable the Effects beauty environment.
///
/// Available since: 2.16.0
/// Description: Disable the Effects beauty environment.
/// Use cases: It is often used in scenes such as video calls and live broadcasts.
/// When to call: Must be called before calling [startPreview], [startPublishingStream]. If you need to modify the configuration, please call [logoutRoom] to log out of the room first, otherwise it will not take effect.
/// Related APIs: [enableEffectsBeauty] switch beauty, [setEffectsBeautyParam] set beauty parameters.
/// Caution: This beauty function is the basic function. If it does not meet the expectations of the developer, you can use the custom video pre-processing function [enableCustomVideoProcessing] or the custom video capture function [enableCustomVideoCapture] docking and constructing the AI ​​Effects SDK [ZegoEffects] for best results.
/// Restrictions: This function only supports Android system 5.0 and above, Android SDK version 21 and above.
/// Note: This function is only available in ZegoExpressVideo SDK!
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_effects_env();
#else
typedef zego_error(EXP_CALL *pfnzego_express_stop_effects_env)();
#endif

/// Enables or disables the beauty effect.
///
/// Available since: 2.16.0
/// Description: Support basic beauty functions, including whiten, rosy, smooth, and sharpen.
/// Use cases: It is often used in scenes such as video calls and live broadcasts.
/// When to call: You must call [startEffectsEnv] to enable the beauty environment before calling this function.
/// Default value: When this function is not called, the beauty effect is not enabled by default.
/// Related APIs: You can call the [setBeautifyOption] function to adjust the beauty parameters.
/// Caution: This beauty function is the basic function. If it does not meet the expectations of the developer, you can use the custom video pre-processing function [enableCustomVideoProcessing] or the custom video capture function [enableCustomVideoCapture] docking and constructing the AI ​​Effects SDK [ZegoEffects] for best results.
/// Restrictions: If this function is used on the Android platform, it only supports 5.0 and above, SDK version 21 and above.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param enable Whether to enable the beauty effect, true is enabled; false is disabled, and the default is false.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_effects_beauty(bool enable);
#else
typedef zego_error(EXP_CALL *pfnzego_express_enable_effects_beauty)(bool enable);
#endif

/// Set beautify param.
///
/// Available since: 2.16.0
/// Description: Set the beauty parameters, including whiten, rosy, smooth, and sharpen.
/// Use cases: It is often used in scenes such as video calls and live broadcasts.
/// When to call: You must call [startEffectsEnv] to enable the beauty environment before calling this function.
/// Related APIs: You can call [enableEffectsBeauty] to turn on or off the beauty function.
/// Restrictions: This function only supports Android system 5.0 and above, Android SDK version 21 and above.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param param Beauty option param.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_set_effects_beauty_param(struct zego_effects_beauty_param param);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_effects_beauty_param)(
    struct zego_effects_beauty_param param);
#endif

/// Set the sound equalizer (EQ).
///
/// Available since: 1.12.0
/// Description: Call this function to set the sound equalizer adjust the tone.
/// Use cases: Often used in voice chatroom, KTV.
/// When to call: It needs to be called after [createEngine].
/// Restrictions: None.
///
/// @param band_index Band frequency index, the value range is [0, 9], corresponding to 10 frequency bands, and the center frequencies are [31, 62, 125, 250, 500, 1K, 2K, 4K, 8K, 16K] Hz.
/// @param band_gain Band gain for the index, the value range is [-15, 15]. Default value is 0, if all gain values in all frequency bands are 0, EQ function will be disabled.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_audio_equalizer_gain(int band_index,
                                                                      float band_gain);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_audio_equalizer_gain)(int band_index,
                                                                       float band_gain);
#endif

/// Setting up the voice changer via preset enumeration.
///
/// Available since: 1.17.0
/// Description: Call this function to use preset voice changer effect.
/// Use cases: Often used in live broadcasting, voice chatroom and KTV.
/// When to call: It needs to be called after [createEngine].
/// Related APIs:
/// If you need advanced voice changer effect, please use [setVoiceChangerParam].
/// The effect of using this function together with [setReverbPreset] may be different from what is expected. If you need to use it at the same time, it is recommended to enable the voice changer first, and then enable the reverb.
/// Using ANDROID/ETHEREAL preset voice changer effect will modify reverberation or reverberation echo parameters. Calling [setVoiceChangerParam], [setReverbAdvancedParam], [setReverbEchoParam] may affect the voice changer effect after use these preset voice changer effect.
/// If you need advanced reverb/echo/electronic effects/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setElectronicEffects], [setVoiceChangerParam] together.
///
/// @param preset The voice changer preset enumeration.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_set_voice_changer_preset(enum zego_voice_changer_preset preset);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_voice_changer_preset)(
    enum zego_voice_changer_preset preset);
#endif

/// Setting up the specific voice changer parameters.
///
/// Available since: 1.10.0
/// Description: Call this function to set custom voice changer effect.
/// Use cases: Often used in live broadcasting, voice chatroom and KTV.
/// When to call: It needs to be called after [createEngine].
/// Related APIs:
/// [setVoiceChangerPreset] provide a set of preset voice changer effects.
/// If you need advanced reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
///
/// @param param Voice changer parameters.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_voice_changer_param(float param);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_voice_changer_param)(float param);
#endif

/// Setting up the reverberation via preset enumeration.
///
/// Available since: 1.17.0
/// Description: Call this function to set preset reverb effect.
/// Use cases: Often used in live broadcasting, voice chatroom and KTV.
/// When to call: It needs to be called after [createEngine]. Support call this function to change preset reverb effect during publishing stream.
/// Related APIs:
/// If you need advanced reverb effect, please use [setReverbAdvancedParam].
/// The effect of using this function together with [setVoiceChangerPreset] may be different from what is expected. If you need to use it at the same time, it is recommended to enable the voice changer first, and then enable the reverb.
/// If you need advanced reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
///
/// @param preset The reverberation preset enumeration.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_reverb_preset(enum zego_reverb_preset preset);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_reverb_preset)(enum zego_reverb_preset preset);
#endif

/// Setting up the specific reverberation parameters.
///
/// Available since: 1.10.0
/// Description: Call this function to set preset reverb effect.
/// Use cases: Often used in live broadcasting, voice chatroom and KTV.
/// When to call: It needs to be called after [createEngine].
/// Caution: Different values dynamically set during publishing stream will take effect. When all parameters are set to 0, the reverberation is turned off.
/// Related APIs:
/// [setReverbPreset] provide a set of preset reverb effects.
/// If you need advanced reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
///
/// @param param Reverb advanced parameter.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_set_reverb_advanced_param(struct zego_reverb_advanced_param param);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_reverb_advanced_param)(
    struct zego_reverb_advanced_param param);
#endif

/// Setting up the specific reverberation echo parameters.
///
/// Available since: 1.17.0
/// Description: Call this function to set reverb echo effect. This function can be used with voice changer and reverb to achieve a variety of custom sound effects.
/// Use cases: Often used in live broadcasting, voice chatroom and KTV.
/// When to call: It needs to be called after [createEngine].
/// Related APIs: If you need advanced reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
///
/// @param param The reverberation echo parameter.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_set_reverb_echo_param(struct zego_reverb_echo_param param);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_reverb_echo_param)(
    struct zego_reverb_echo_param param);
#endif

/// Enable or disable the virtual stereo effect when publishing stream.
///
/// Available since: 1.10.0; Note: Starting from 2.15.0, the angle parameter supports setting -1 to present a all round virtual stereo effect.
/// Description: Call this function to enable / disable the virtual stereo effect when publishing stream.
/// Use cases: Often used in live broadcasting, voice chatroom and KTV.
/// When to call: It needs to be called after [createEngine].
/// Caution: You need to set up a dual channel with [setAudioConfig] for the virtual stereo to take effect.
///
/// @param enable true to turn on the virtual stereo, false to turn off the virtual stereo.
/// @param angle The angle of the sound source in virtual stereo in the range of -1 ~ 360, with 90 being directly in front, 0 / 180 / 270 corresponding to the rightmost and leftmost respectively. In particular, when set to -1, it is all round virtual stereo effects.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_virtual_stereo(bool enable, int angle);
#else
typedef zego_error(EXP_CALL *pfnzego_express_enable_virtual_stereo)(bool enable, int angle);
#endif

/// Enable or disable the virtual stereo effect when playing stream.
///
/// Available since: 2.8.0
/// Description: Call this function to enable/disable the virtual stereo effect when playing stream.
/// Use cases: Often used in live broadcasting, voice chatroom and KTV.
/// When to call: It needs to be called after [createEngine].
/// Caution: It can dynamically switch and set angle parameters before and after playing stream. After stopping playing stream, it will automatically reset and disable.
///
/// @param enable true to turn on the virtual stereo, false to turn off the virtual stereo.
/// @param angle The angle of the sound source in virtual stereo in the range of 0 ~ 360, with 90 being directly in front, 0 / 180 / 270 corresponding to the rightmost and leftmost respectively.
/// @param stream_id Stream ID.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_enable_play_stream_virtual_stereo(bool enable, int angle, const char *stream_id);
#else
typedef zego_error(EXP_CALL *pfnzego_express_enable_play_stream_virtual_stereo)(
    bool enable, int angle, const char *stream_id);
#endif

/// Turn on or off the electronic sound effect.
///
/// Available since: 2.13.0
/// Description: Call this function to turn on or off the electronic sound effect.
/// Use cases: Often used in live broadcasting, voice chatroom and sung unaccompanied scenes.
/// Default value: When this function is not called, the electronic sound effect is not enabled by default.
/// When to call: It needs to be called after [createEngine].
/// Restrictions: None.
/// Caution: When the mode parameter is Harmonic Minor, the tonal parameter does not take effect.
/// Related APIs: Common electronic sound effect configurations can be set via [setVoiceChangerPreset].
///
/// @param enable true to turn on the electronic sound effect, false to turn off the electronic sound effect.
/// @param mode Mode of Electronic Effects reference.
/// @param tonal The starting pitch of an electric tone in a given mode, representing 12 semitones in one octave of the sound, in the range [0, 11].
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_set_electronic_effects(bool enable, enum zego_electronic_effects_mode mode, int tonal);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_electronic_effects)(
    bool enable, enum zego_electronic_effects_mode mode, int tonal);
#endif

/// Enable color enhancement.
///
/// Available since: 3.11.0
/// Description: Call this function to enable or disable color enhancement.
/// Use cases: Commonly used in video calling, live streaming, and similar scenarios.
/// Default value: When this function is not called, color enhancement is not enabled by default.
/// When to call: It needs to be called after [createEngine].
///
/// @param enable Whether to enable, true: enable, false: disable
/// @param params Color enhancement parameters.
/// @param channel Publish stream channel.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_color_enhancement(
    bool enable, struct zego_color_enhancement_params params, enum zego_publish_channel channel);
#else
typedef zego_error(EXP_CALL *pfnzego_express_enable_color_enhancement)(
    bool enable, struct zego_color_enhancement_params params, enum zego_publish_channel channel);
#endif

/// Audio mixing callback.
///
/// Available since: 1.9.0
/// Description: The callback for copying audio data to the SDK for audio mixing. This function should be used together with [enableAudioMixing].
/// Use cases: Developers can use this function when they need to mix their own songs, sound effects or other audio data into the publishing stream.
/// When to trigger: It will triggered after [createEngine], and call [enableAudioMixing] turn on audio mixing, and call [setAudioMixingHandler] set audio mixing callback handler.
/// Restrictions: Supports 16k 32k 44.1k 48k sample rate, mono or dual channel, 16-bit deep PCM audio data.
/// Caution: This callback is a high frequency callback. To ensure the quality of the mixing data, please do not handle time-consuming operations in this callback.
///
/// @param data Audio mixing data. Note that this parameter is an input parameter, and the developer needs to modify the value of each parameter in the struct.
/// @param user_context Context of user.
typedef void (*zego_on_copy_audio_mixing_data)(struct zego_audio_mixing_data *data,
                                               void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_copy_audio_mixing_data_callback(
    zego_on_copy_audio_mixing_data callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_copy_audio_mixing_data_callback)(
    zego_on_copy_audio_mixing_data callback_func, void *user_context);
#endif

/// [Deprecated] Enables or disables the beauty features for the specified publish channel. Deprecated since 2.16.0, please use the [enableEffectsBeauty] function instead.
///
/// Available since: 1.1.0
/// Description: When developers do not have much need for beauty features, they can use this function to set some very simple beauty effects.
/// When to call: It needs to be called after [createEngine].
/// Default value: When this function is not called, the beauty feature is not enabled by default.
/// Related APIs: After turning on the beauty features, you can call the [setBeautifyOption] function to adjust the beauty parameters.
/// Caution: This beauty feature is very simple and may not meet the developer’s expectations. It is recommended to use the custom video processing function [enableCustomVideoProcessing] or the custom video capture function [enableCustomVideoCapture] to connect the AI Effects SDK [ZegoEffects] https://docs.zegocloud.com/article/9896 for best results.
/// Restrictions: In the case of using the custom video capture function, since the developer has handle the video data capturing, the SDK is no longer responsible for the video data capturing, so this function is no longer valid. It is also invalid when using the custom video processing function.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @deprecated Deprecated since 2.16.0, please use the [enableEffectsBeauty] function instead.
/// @param feature_bit_mask Beauty features, bitmask format, you can choose to enable several features in [ZegoBeautifyFeature] at the same time
/// @param channel Publishing stream channel
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_beautify(int feature_bit_mask,
                                                             enum zego_publish_channel channel);
#else
typedef zego_error(EXP_CALL *pfnzego_express_enable_beautify)(int feature_bit_mask,
                                                              enum zego_publish_channel channel);
#endif

/// [Deprecated] Set beautify option. Deprecated since 2.16.0, please use the [setEffectsBeautyParam] function instead.
///
/// Available since: 1.1.0
/// Description: set beautify option for main publish channel.
/// Use cases: Often used in video call, live broadcasting.
/// When to call: It needs to be called after [createEngine].
/// Restrictions: None.
/// Caution: In the case of using a custom video capture function, because the developer has taken over the video data capturing, the SDK is no longer responsible for the video data capturing, call this function will not take effect. When using custom video processing, the video data collected by the SDK will be handed over to the business for further processing, call this function will not take effect either.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @deprecated Deprecated since 2.16.0, please use the [setEffectsBeautyParam] function instead.
/// @param option Beautify option.
/// @param channel stream publish channel.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_beautify_option(struct zego_beautify_option option,
                                                                 enum zego_publish_channel channel);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_beautify_option)(
    struct zego_beautify_option option, enum zego_publish_channel channel);
#endif

ZEGO_END_DECLS

#endif
