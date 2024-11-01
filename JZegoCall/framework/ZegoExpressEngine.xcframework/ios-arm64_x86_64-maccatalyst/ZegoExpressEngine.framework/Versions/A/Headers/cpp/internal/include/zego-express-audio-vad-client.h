
#ifndef __ZEGO_EXPRESS_AUDIO_VAD_CLIENT_H__
#define __ZEGO_EXPRESS_AUDIO_VAD_CLIENT_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Create a audio vad client instance.
///
/// Available since: 2.21.0
/// Description: Create a audio vad client instance.
/// Use case: Can be used to detect if an audio packet contains speech.
/// When to call: It can be called after [createEngine].
/// Restrictions: Each stream should correspond to a detected audio packet class object, or reset the existing object and apply it to a different stream.
///
/// @param result_instance [in/out] audio vad client instance point.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_create_audio_vad_client(unsigned long long *result_instance);
#else
typedef zego_error(EXP_CALL *pfnzego_express_create_audio_vad_client)(
    unsigned long long *result_instance);
#endif

/// Destroys a audio vad client instance.
///
/// Available since: 2.21.0
/// Description: Destroys a audio vad client instance.
/// When to call: It can be called after [createEngine].
///
/// @param instance audio vad client instance point.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_destroy_audio_vad_client(unsigned long long instance);
#else
typedef zego_error(EXP_CALL *pfnzego_express_destroy_audio_vad_client)(unsigned long long instance);
#endif

/// Check if an audio packet contains speech.
///
/// Available since: 2.21.0
/// Description: Check if an audio packet contains speech.
/// When to call: It can be called after [createAudioVADClient].
/// Caution: ZegoAudioVADTypeSpeech will also be returned when the voice data is abnormal to ensure that the voice will not be missed.
///
/// @param data audio data.
/// @param data_length audio data length.
/// @param sample_rate sampling rate.
/// @param channels number of channels.
/// @param instance audio vad client instance point.
/// @param result_type [in/out] voice detection results.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_vad_client_update(
    unsigned char *data, int data_length, int sample_rate, int channels,
    unsigned long long instance, zego_audio_vad_type *result_type);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_vad_client_update)(
    unsigned char *data, int data_length, int sample_rate, int channels,
    unsigned long long instance, zego_audio_vad_type *result_type);
#endif

/// Reset audio vad client instance.
///
/// Available since: 2.21.0
/// Description: Reset audio vad client instance.
/// When to call: It can be called after [createAudioVADClient].
///
/// @param instance audio vad client instance point.
/// @param result [in/out] reset results.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_vad_client_reset(unsigned long long instance,
                                                                    bool *result);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_vad_client_reset)(unsigned long long instance,
                                                                     bool *result);
#endif

ZEGO_END_DECLS

#endif
