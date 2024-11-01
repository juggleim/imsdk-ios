
#ifndef __ZEGO_EXPRESS_AUDIO_EFFECT_PLAYER_H__
#define __ZEGO_EXPRESS_AUDIO_EFFECT_PLAYER_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Creates a audio effect player instance.
///
/// Available since: 1.16.0
/// Description: Creates a audio effect player instance.
/// Use cases: When you need to play short sound effects, such as applause, cheers, etc., you can use audioEffectPlayer to achieve.
/// When to call: It can be called after [createEngine].
/// Restrictions: Currently, a maximum of 1 instances can be created, after which it will return NULL.
/// Related APIs: [destroyAudioEffectPlayer].
///
/// @param instance_index [in/out] audio effect player instance, -1 will be returned when the maximum number is exceeded.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_create_audio_effect_player(
    enum zego_audio_effect_player_instance_index *instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_create_audio_effect_player)(
    enum zego_audio_effect_player_instance_index *instance_index);
#endif

/// Destroys a audio effect player instance.
///
/// Available since: 1.16.0
/// Description: Destroys the specified audio effect player instance.
/// When to call: It can be called after [createAudioEffectPlayer].
/// Restrictions: None.
/// Related APIs: [createAudioEffectPlayer].
///
/// @param instance_index audio effect instance index
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_destroy_audio_effect_player(
    enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_destroy_audio_effect_player)(
    enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Start playing audio effect.
///
/// Available since: 1.16.0
/// Description: Start playing audio effect. The default is only played once and is not mixed into the publishing stream, if you want to change this please modify [config] param.
/// Use cases: When you need to play short sound effects, such as applause, cheers, etc., you can use this interface to achieve, and further configure the number of plays through the [config] parameter, and mix the sound effects into the push stream.
/// When to call: It can be called after [createAudioEffectPlayer].
/// Restrictions: None.
///
/// @param audio_effect_id Description: ID for the audio effect. The SDK uses audioEffectID to control the playback of sound effects. The SDK does not force the user to pass in this parameter as a fixed value. It is best to ensure that each sound effect can have a unique ID. The recommended methods are static self-incrementing ID or the hash of the incoming sound effect file path.
/// @param path The absolute path of the local resource. <br>Value range: "assets://"、"ipod-library://" and network url are not supported. Set path as NULL or "" if resource is loaded already using [loadResource].
/// @param config Audio effect playback configuration. <br>Default value: Set NULL will only be played once, and will not be mixed into the publishing stream.
/// @param instance_index audio effect instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_start(
    unsigned int audio_effect_id, const char *path, struct zego_audio_effect_play_config *config,
    enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_start)(
    unsigned int audio_effect_id, const char *path, struct zego_audio_effect_play_config *config,
    enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Stop playing audio effect.
///
/// Available since: 1.16.0
/// Description: Stop playing the specified audio effect [audioEffectID].
/// When to call: The specified [audioEffectID] is [start].
/// Restrictions: None.
///
/// @param audio_effect_id ID for the audio effect.
/// @param instance_index audio effect instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_stop(
    unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_stop)(
    unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Pause playing audio effect.
///
/// Available since: 1.16.0
/// Description: Pause playing the specified audio effect [audioEffectID].
/// When to call: The specified [audioEffectID] is [start].
/// Restrictions: None.
///
/// @param audio_effect_id ID for the audio effect.
/// @param instance_index audio effect instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_pause(
    unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_pause)(
    unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Resume playing audio effect.
///
/// Available since: 1.16.0
/// Description: Resume playing the specified audio effect [audioEffectID].
/// When to call: The specified [audioEffectID] is [pause].
/// Restrictions: None.
///
/// @param audio_effect_id ID for the audio effect.
/// @param instance_index audio effect instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_resume(
    unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_resume)(
    unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Stop playing all audio effect.
///
/// Available since: 1.16.0
/// Description: Stop playing all audio effect.
/// When to call: Some audio effects are Playing.
/// Restrictions: None.
///
/// @param instance_index audio effect instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_stop_all(
    enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_stop_all)(
    enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Pause playing all audio effect.
///
/// Available since: 1.16.0
/// Description: Pause playing all audio effect.
/// When to call: It can be called after [createAudioEffectPlayer].
/// Restrictions: None.
///
/// @param instance_index audio effect instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_pause_all(
    enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_pause_all)(
    enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Resume playing all audio effect.
///
/// Available since: 1.16.0
/// Description: Resume playing all audio effect.
/// When to call: It can be called after [pauseAll].
/// Restrictions: None.
///
/// @param instance_index audio effect instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_resume_all(
    enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_resume_all)(
    enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Set the specified playback progress.
///
/// Available since: 1.16.0
/// Description: Set the specified audio effect playback progress. Unit is millisecond.
/// When to call: The specified [audioEffectID] is[start], and not finished.
/// Restrictions: None.
///
/// @param audio_effect_id ID for the audio effect.
/// @param millisecond Point in time of specified playback progress.
/// @param instance_index audio effect instance index.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_seek_to(
    unsigned int audio_effect_id, unsigned long long millisecond,
    enum zego_audio_effect_player_instance_index instance_index, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_seek_to)(
    unsigned int audio_effect_id, unsigned long long millisecond,
    enum zego_audio_effect_player_instance_index instance_index, zego_seq *sequence);
#endif

/// Set volume for a single audio effect. Both the local play volume and the publish volume are set.
///
/// Available since: 1.16.0
/// Description: Set volume for a single audio effect. Both the local play volume and the publish volume are set.
/// When to call: The specified [audioEffectID] is [start].
/// Restrictions: None.
///
/// @param audio_effect_id ID for the audio effect.
/// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
/// @param instance_index audio effect instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_set_volume(
    unsigned int audio_effect_id, int volume,
    enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_set_volume)(
    unsigned int audio_effect_id, int volume,
    enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Set local play volume for a single audio effect.
///
/// Available since: 3.11.0
/// Description: Set local play volume for a single audio effect.
/// When to call: The specified [audioEffectID] is [start].
/// Restrictions: None.
///
/// @param audio_effect_id ID for the audio effect.
/// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
/// @param instance_index audio effect instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_set_play_volume(
    unsigned int audio_effect_id, int volume,
    enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_set_play_volume)(
    unsigned int audio_effect_id, int volume,
    enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Set publish volume for a single audio effect.
///
/// Available since: 3.11.0
/// Description: Set publish volume for a single audio effect.
/// When to call: The specified [audioEffectID] is [start].
/// Restrictions: None.
///
/// @param audio_effect_id ID for the audio effect.
/// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
/// @param instance_index audio effect instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_set_publish_volume(
    unsigned int audio_effect_id, int volume,
    enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_set_publish_volume)(
    unsigned int audio_effect_id, int volume,
    enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Set volume for all audio effect. Both the local play volume and the publish volume are set.
///
/// Available since: 1.16.0
/// Description: Set volume for all audio effect. Both the local play volume and the publish volume are set.
/// When to call: It can be called after [createAudioEffectPlayer].
/// Restrictions: None.
///
/// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
/// @param instance_index audio effect instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_set_volume_all(
    int volume, enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_set_volume_all)(
    int volume, enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Set local play volume for all audio effect.
///
/// Available since: 3.11.0
/// Description: Set local play volume for all audio effect.
/// When to call: It can be called after [createAudioEffectPlayer].
/// Restrictions: None.
///
/// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
/// @param instance_index audio effect instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_set_play_volume_all(
    int volume, enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_set_play_volume_all)(
    int volume, enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Set publish volume for all audio effect.
///
/// Available since: 3.11.0
/// Description: Set publish volume for all audio effect.
/// When to call: It can be called after [createAudioEffectPlayer].
/// Restrictions: None.
///
/// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
/// @param instance_index audio effect instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_set_publish_volume_all(
    int volume, enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_set_publish_volume_all)(
    int volume, enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Set the playback speed for a given audio effect. Both the local play speed and the publish speed are set. (separate settings are not supported).
///
/// Available since: 2.18.0
/// Description: Set the playback speed for a given audio effect. Both the local play speed and the publish speed are set. (separate settings are not supported).
/// When to call: The specified [audioEffectID] is [start].
/// Restrictions: None.
///
/// @param audio_effect_id ID for the audio effect.
/// @param speed The speed of play. <br>Value range: The range is 0.5 ~ 2.0. <br>Default value: The default is 1.0.
/// @param instance_index audio effect instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_set_play_speed(
    unsigned int audio_effect_id, float speed,
    enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_set_play_speed)(
    unsigned int audio_effect_id, float speed,
    enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Get the total duration of the specified audio effect resource.
///
/// Available since: 1.16.0
/// Description: Get the total duration of the specified audio effect resource. Unit is millisecond.
/// When to call: You should invoke this function after the audio effect resource already loaded, otherwise the return value is 0.
/// Restrictions: It can be called after [createAudioEffectPlayer].
/// Related APIs: [start], [loadResource].
///
/// @param audio_effect_id ID for the audio effect.
/// @param instance_index audio effect instance index.
/// @param total_duration [in/out] The total duration of the specified audio effect resource.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_get_total_duration(
    unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index,
    unsigned long long *total_duration);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_get_total_duration)(
    unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index,
    unsigned long long *total_duration);
#endif

/// Get current playback progress.
///
/// Available since: 1.16.0
/// Description: Get current playback progress of the specified audio effect. Unit is millisecond.
/// When to call: You should invoke this function after the audio effect resource already loaded, otherwise the return value is 0.
/// Restrictions: None.
/// Related APIs: [start], [loadResource].
///
/// @param audio_effect_id ID for the audio effect.
/// @param instance_index audio effect instance index.
/// @param current_progress [in/out] Current playback progress.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_get_current_progress(
    unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index,
    unsigned long long *current_progress);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_get_current_progress)(
    unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index,
    unsigned long long *current_progress);
#endif

/// Load audio effect resource.
///
/// Available since: 1.16.0
/// Description: Load audio effect resource.
/// Use cases: In a scene where the same sound effect is played frequently, the SDK provides the function of preloading the sound effect file into the memory in order to optimize the performance of repeatedly reading and decoding the file.
/// When to call: It can be called after [createAudioEffectPlayer].
/// Restrictions: Preloading supports loading up to 15 sound effect files at the same time, and the duration of the sound effect files cannot exceed 30s, otherwise an error will be reported when loading.
///
/// @param audio_effect_id ID for the audio effect.
/// @param path the absolute path of the audio effect resource and cannot be NULL or "". <br>Value range: "assets://"、"ipod-library://" and network url are not supported.
/// @param instance_index audio effect instance index.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_load_resource(
    unsigned int audio_effect_id, const char *path,
    enum zego_audio_effect_player_instance_index instance_index, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_load_resource)(
    unsigned int audio_effect_id, const char *path,
    enum zego_audio_effect_player_instance_index instance_index, zego_seq *sequence);
#endif

/// Unload audio effect resource.
///
/// Available since: 1.16.0
/// Description: Unload the specified audio effect resource.
/// When to call: After the sound effects are used up, related resources can be released through this function; otherwise, the SDK will release the loaded resources when the AudioEffectPlayer instance is destroyed.
/// Restrictions: None.
/// Related APIs: [loadResource].
///
/// @param audio_effect_id ID for the audio effect loaded.
/// @param instance_index audio effect instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_unload_resource(
    unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_unload_resource)(
    unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Update the position of the audio effect player (audio source).
///
/// Available since: 3.6.0
/// Description: Update the position of the audio effect player (audio source).
/// Use cases: The audio effect player also needs to have 3D spatial sound.
/// When to call: Listen to the [onAudioEffectPlayStateUpdate] callback, please call this interface after the player state is ZegoAudioEffectPlayState.Playing and before ZegoAudioEffectPlayState.NoPlay/PlayEnded.
/// Restrictions: This interface needs to be used in conjunction with the RangeAudio/RangeScene module. This interface can only be called successfully after the RangeAudio/RangeScene module enables 3D sound effects.
///
/// @param audio_effect_id ID for the audio effect.
/// @param position The unit vector of the front axis of its own coordinate system. The parameter is a float array with a length of 3.
/// @param instance_index audio effect instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_update_position(
    unsigned int audio_effect_id, const float position[3],
    enum zego_audio_effect_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_audio_effect_player_update_position)(
    unsigned int audio_effect_id, const float position[3],
    enum zego_audio_effect_player_instance_index instance_index);
#endif

/// Audio effect playback state callback.
///
/// Available since: 1.16.0
/// Description: This callback is triggered when the playback state of a audio effect of the audio effect player changes.
/// Trigger: This callback is triggered when the playback status of the audio effect changes.
/// Restrictions: None.
///
/// @param audio_effect_id The ID of the audio effect resource that triggered this callback.
/// @param state The playback state of the audio effect.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param instance_index audio effect instance index.
/// @param user_context Context of user.
typedef void (*zego_on_audio_effect_play_state_update)(
    unsigned int audio_effect_id, enum zego_audio_effect_play_state state, zego_error error_code,
    enum zego_audio_effect_player_instance_index instance_index, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_audio_effect_play_state_update_callback(
    zego_on_audio_effect_play_state_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_audio_effect_play_state_update_callback)(
    zego_on_audio_effect_play_state_update callback_func, void *user_context);
#endif

/// Callback for audio effect player loads resources.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param instance_index Audio effect player instance index
/// @param user_context Context of user.
typedef void (*zego_on_audio_effect_player_load_resource)(
    zego_seq seq, zego_error error_code,
    enum zego_audio_effect_player_instance_index instance_index, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_audio_effect_player_load_resource_callback(
    zego_on_audio_effect_player_load_resource callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_audio_effect_player_load_resource_callback)(
    zego_on_audio_effect_player_load_resource callback_func, void *user_context);
#endif

/// Callback for audio effect player seek to playback progress.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param instance_index Audio effect player instance index
/// @param user_context Context of user.
typedef void (*zego_on_audio_effect_player_seek_to)(
    zego_seq seq, zego_error error_code,
    enum zego_audio_effect_player_instance_index instance_index, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_audio_effect_player_seek_to_callback(
    zego_on_audio_effect_player_seek_to callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_audio_effect_player_seek_to_callback)(
    zego_on_audio_effect_player_seek_to callback_func, void *user_context);
#endif

ZEGO_END_DECLS

#endif
