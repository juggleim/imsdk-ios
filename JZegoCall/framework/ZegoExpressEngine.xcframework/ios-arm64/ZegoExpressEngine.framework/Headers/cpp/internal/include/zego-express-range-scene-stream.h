
#ifndef __ZEGO_EXPRESS_RANGE_SCENE_STREAM_H__
#define __ZEGO_EXPRESS_RANGE_SCENE_STREAM_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Set the receiving range of audio and video streams.
///
/// Available: since 3.0.0
/// Description: This interface will set the receiving range of audio and video streams, and ZEGO SDK will actively playing the streams of users in this range.
/// When to call: After [getRangeSceneStream].
/// Default value: The default value of receive range is 0.0 .
/// Restrictions: None.
/// Caution: None.
///
/// @param range_scene_handle Range scene instance handle.
/// @param range The range distance from the center point of the person.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_range_scene_stream_set_receive_range(int range_scene_handle, float range);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_scene_stream_set_receive_range)(
    int range_scene_handle, float range);
#endif

/// Set the receiving range of audio and video streams.
///
/// Available: since 3.7.0
/// Description: This interface will set the receiving range of audio and video streams, and ZEGO SDK will actively playing the streams of users in this range.
/// When to call: After [getRangeSceneStream].
/// Default value: The default value of receive range is 0.0 .
/// Restrictions: None.
/// Caution: None.
///
/// @param range_scene_handle Range scene instance handle.
/// @param param The configuration of audio receiving range.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_scene_stream_set_receive_range_with_param(
    int range_scene_handle, struct zego_receive_range_param param);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_scene_stream_set_receive_range_with_param)(
    int range_scene_handle, struct zego_receive_range_param param);
#endif

/// Turn 3D spatial sound on or off.
///
/// Available: since 3.0.0
/// Description: After it is turned on, the audio of non-team members in the world will change spatially with the distance and direction from the person.
/// When to call: After [getRangeSceneStream].
/// Default value: Disable.
/// Restrictions: Media volume is required to use 3D spatial sound.
/// Caution: None.
///
/// @param range_scene_handle Range scene instance handle.
/// @param enable Whether to turn 3D spatial sound on, true: enable, false: disable.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_range_scene_stream_enable_range_spatializer(int range_scene_handle, bool enable);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_scene_stream_enable_range_spatializer)(
    int range_scene_handle, bool enable);
#endif

/// Set whether to receive the audio data of the specified user.
///
/// Available: since 3.0.0
/// Description: Set whether to receive the audio data of the specified user.
/// When to call: After [LoginScene], before [LogoutScene].
/// Default value: Receive.
/// Restrictions: None.
/// Caution: None.
///
/// @param range_scene_handle Range scene instance handle.
/// @param userid User ID.
/// @param mute true: do not receive the user's audio stream, false: receive the user's audio stream.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_scene_stream_mute_play_audio(
    int range_scene_handle, const char *userid, bool mute);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_scene_stream_mute_play_audio)(
    int range_scene_handle, const char *userid, bool mute);
#endif

/// Set whether to receive the video data of the specified user.
///
/// Available: since 3.0.0
/// Description: After it is turned on, the audio of non-team members in the world will change spatially with the distance and direction from the person.
/// When to call: After [LoginScene], before [LogoutScene].
/// Default value: Receive.
/// Restrictions: None.
/// Caution: None.
///
/// @param range_scene_handle Range scene instance handle.
/// @param userid User ID.
/// @param mute true: do not receive the user's video stream, false: receive the user's video stream.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_scene_stream_mute_play_video(
    int range_scene_handle, const char *userid, bool mute);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_scene_stream_mute_play_video)(
    int range_scene_handle, const char *userid, bool mute);
#endif

/// The callback notification of local user play other users' stream state change.
///
/// Available since: 3.0.0
/// Description: The callback notification of local user plays other users' stream state change.
/// When to trigger: The local user plays other users' stream state changes.
/// Restrictions: Do not call the SDK interface in the callback thread.
///
/// @param range_scene_handle Range scene instance handle.
/// @param userid User ID.
/// @param stream_id User's stream ID.
/// @param state The state of local user play stream.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_stream_user_stream_state_update)(int range_scene_handle,
                                                                    const char *userid,
                                                                    const char *stream_id,
                                                                    enum zego_stream_state state,
                                                                    void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_stream_user_stream_state_update_callback(
    zego_on_range_scene_stream_user_stream_state_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_stream_user_stream_state_update_callback)(
    zego_on_range_scene_stream_user_stream_state_update callback_func, void *user_context);
#endif

/// User microphone state change callback.
///
/// Available: Since 3.0.0
/// Description: User microphone state change callback.
/// When to trigger: User microphone state change
/// Restrictions: Do not call the SDK interface in the callback thread.
///
/// @param range_scene_handle Range scene instance handle.
/// @param userid User ID.
/// @param state Device state.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_stream_user_mic_update)(int range_scene_handle,
                                                           const char *userid,
                                                           enum zego_device_state state,
                                                           void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_stream_user_mic_update_callback(
    zego_on_range_scene_stream_user_mic_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_stream_user_mic_update_callback)(
    zego_on_range_scene_stream_user_mic_update callback_func, void *user_context);
#endif

/// User camera state change callback.
///
/// Available: Since 3.0.0
/// Description: User camera state change callback.
/// When to trigger: User camera state change.
/// Restrictions: Do not call the SDK interface in the callback thread.
///
/// @param range_scene_handle Range scene instance handle.
/// @param userid User ID.
/// @param state Device state.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_stream_user_camera_update)(int range_scene_handle,
                                                              const char *userid,
                                                              enum zego_device_state state,
                                                              void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_stream_user_camera_update_callback(
    zego_on_range_scene_stream_user_camera_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_stream_user_camera_update_callback)(
    zego_on_range_scene_stream_user_camera_update callback_func, void *user_context);
#endif

/// User speaker state change callback.
///
/// Available: Since 3.0.0
/// Description: User speaker state change callback.
/// When to trigger: User speaker state change.
/// Restrictions: Do not call the SDK interface in the callback thread.
///
/// @param range_scene_handle Range scene instance handle.
/// @param userid User ID.
/// @param state Device state.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_stream_user_speaker_update)(int range_scene_handle,
                                                               const char *userid,
                                                               enum zego_device_state state,
                                                               void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_stream_user_speaker_update_callback(
    zego_on_range_scene_stream_user_speaker_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_stream_user_speaker_update_callback)(
    zego_on_range_scene_stream_user_speaker_update callback_func, void *user_context);
#endif

ZEGO_END_DECLS

#endif
