
#ifndef __ZEGO_EXPRESS_AI_VOICE_CHANGER_H__
#define __ZEGO_EXPRESS_AI_VOICE_CHANGER_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Creates an AI voice changer instance.
///
/// Available since: 3.10.0
/// Description: Creates an AI voice changer instance.
/// Use case: Often used in live broadcasting, voice chatroom and KTV.
/// When to call: It can be called after the SDK by [createEngine] has been initialized.
/// Restrictions: Currently, a maximum of 1 instance can be created, after which it will return NULL.
/// Related APIs: User can call [destroyAIVoiceChanger] function to destroy an AI voice changer instance.
///
/// @param instance_index [in/out] AI voice changer instance, -1 will be returned when the maximum number is exceeded.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_create_ai_voice_changer(int *instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_create_ai_voice_changer)(int *instance_index);
#endif

/// Destroys an AI voice changer instance.
///
/// Available since: 3.10.0
/// Description: Destroys the AI voice changer instance.
/// Related APIs: User can call [createAIVoiceChanger] function to create an AI voice changer instance.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_destroy_ai_voice_changer(int instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_destroy_ai_voice_changer)(int instance_index);
#endif

/// Check the device can run AI voice changer or not.
///
/// Available since: 3.14.0
/// Description: Check the device can run AI voice changer or not.
/// Use case: Often used in live broadcasting, voice chatroom and KTV.
/// When to call: It can be called after the SDK by [createEngine] has been initialized.
///
/// @param is_supported [in/out] Return 1 if the device can run AI voice changer, otherwise return 0.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_is_ai_voice_changer_supported(int *is_supported);
#else
typedef zego_error(EXP_CALL *pfnzego_express_is_ai_voice_changer_supported)(int *is_supported);
#endif

/// Initialize AI voice changer engine.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_ai_voice_changer_init(int instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_ai_voice_changer_init)(int instance_index);
#endif

/// Update AI voice changer engine models.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_ai_voice_changer_update(int instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_ai_voice_changer_update)(int instance_index);
#endif

/// Get AI voice changer speaker list.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_ai_voice_changer_get_speaker_list(int instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_ai_voice_changer_get_speaker_list)(int instance_index);
#endif

/// Set AI voice changer speaker.
///
/// @param speaker_id Speaker ID.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_ai_voice_changer_set_speaker(int instance_index,
                                                                          int speaker_id);
#else
typedef zego_error(EXP_CALL *pfnzego_express_ai_voice_changer_set_speaker)(int instance_index,
                                                                           int speaker_id);
#endif

/// Initialize AI voice changer engine status callback.
///
/// Available since: 3.10.0.
/// Description: Initialize AI voice changer engine status callback.
/// Trigger: The callback triggered when call [init] function.
/// Restrictions: None.
///
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param instance_index AI voice changer instance index.
/// @param user_context Context of user.
typedef void (*zego_on_ai_voice_changer_init)(zego_error error_code, int instance_index,
                                              void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_ai_voice_changer_init_callback(
    zego_on_ai_voice_changer_init callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_ai_voice_changer_init_callback)(
    zego_on_ai_voice_changer_init callback_func, void *user_context);
#endif

/// Update AI voice changer engine models progress callback.
///
/// Available since: 3.12.0.
/// Description: Update AI voice changer engine models progress callback.
/// Trigger: The callback triggered when call [update] function.
/// Restrictions: None.
///
/// @param percent Current file update progress.
/// @param file_index Current update file index.
/// @param file_count Total update file count.
/// @param instance_index AI voice changer instance index.
/// @param user_context Context of user.
typedef void (*zego_on_ai_voice_changer_update_progress)(double percent, int file_index,
                                                         int file_count, int instance_index,
                                                         void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_ai_voice_changer_update_progress_callback(
    zego_on_ai_voice_changer_update_progress callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_ai_voice_changer_update_progress_callback)(
    zego_on_ai_voice_changer_update_progress callback_func, void *user_context);
#endif

/// Update AI voice changer engine models status callback.
///
/// Available since: 3.10.0.
/// Description: Update AI voice changer engine models status callback.
/// Trigger: The callback triggered when call [update] function.
/// Restrictions: None.
///
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param instance_index AI voice changer instance index.
/// @param user_context Context of user.
typedef void (*zego_on_ai_voice_changer_update)(zego_error error_code, int instance_index,
                                                void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_ai_voice_changer_update_callback(
    zego_on_ai_voice_changer_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_ai_voice_changer_update_callback)(
    zego_on_ai_voice_changer_update callback_func, void *user_context);
#endif

/// Get AI voice changer engine available speaker list callback.
///
/// Available since: 3.10.0.
/// Description: Get AI voice changer engine available speaker list callback.
/// Trigger: The callback triggered when call [getSpeakerList] function.
/// Restrictions: None.
///
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param speaker_list Available speaker list.
/// @param speaker_count Speaker count.
/// @param instance_index AI voice changer instance index.
/// @param user_context Context of user.
typedef void (*zego_on_ai_voice_changer_get_speaker_list)(
    zego_error error_code, const struct zego_ai_voice_changer_speaker_info *speaker_list,
    unsigned int speaker_count, int instance_index, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_ai_voice_changer_get_speaker_list_callback(
    zego_on_ai_voice_changer_get_speaker_list callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_ai_voice_changer_get_speaker_list_callback)(
    zego_on_ai_voice_changer_get_speaker_list callback_func, void *user_context);
#endif

/// AI voice changer event callback.
///
/// Available since: 3.12.0.
/// Description: AI voice changer event callback.
/// Trigger: The callback triggered when AI voice changer transcode failure occurred.
/// Restrictions: None.
///
/// @param event AI voice changer event.
/// @param instance_index AI voice changer instance index.
/// @param user_context Context of user.
typedef void (*zego_on_ai_voice_changer_event)(enum zego_ai_voice_changer_event event,
                                               int instance_index, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_ai_voice_changer_event_callback(
    zego_on_ai_voice_changer_event callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_ai_voice_changer_event_callback)(
    zego_on_ai_voice_changer_event callback_func, void *user_context);
#endif

/// Set AI voice changer engine speaker status callback.
///
/// Available since: 3.12.0.
/// Description: Set AI voice changer engine speaker status callback.
/// Trigger: The callback triggered when call [setSpeaker] function.
/// Restrictions: None.
///
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param instance_index AI voice changer instance index.
/// @param user_context Context of user.
typedef void (*zego_on_ai_voice_changer_set_speaker)(zego_error error_code, int instance_index,
                                                     void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_ai_voice_changer_set_speaker_callback(
    zego_on_ai_voice_changer_set_speaker callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_ai_voice_changer_set_speaker_callback)(
    zego_on_ai_voice_changer_set_speaker callback_func, void *user_context);
#endif

ZEGO_END_DECLS

#endif
