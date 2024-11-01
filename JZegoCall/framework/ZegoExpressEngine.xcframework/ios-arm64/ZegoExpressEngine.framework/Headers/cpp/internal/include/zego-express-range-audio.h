
#ifndef __ZEGO_EXPRESS_RANGE_AUDIO_H__
#define __ZEGO_EXPRESS_RANGE_AUDIO_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Creates a range audio instance.
///
/// Available since: 2.11.0
/// Description: Creates a range audio instance.
/// Use case: Often used in game voice scenarios, users can use related functions by creating range audio instance objects.
/// When to call: It can be called after the engine by [createEngine] has been initialized.
/// Restrictions: Currently, a maximum of 1 instances can be created, after which it will return NULL.
/// Impacts on other APIs: If you use the range audio module, you cannot use the basic push-pull stream [startPublishingStream], [startPlayingStream] interfaces and related callbacks.
///
/// @param instance_index [in/out] range audio instance, -1 will be returned when the maximum number is exceeded.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_create_range_audio(enum zego_range_audio_instance_index *instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_create_range_audio)(
    enum zego_range_audio_instance_index *instance_index);
#endif

/// Destroys a range audio instance.
///
/// Available since: 2.11.0
/// Description: Destroys a range audio instance.
///
/// @param instance_index range audio instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_destroy_range_audio(enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_destroy_range_audio)(
    enum zego_range_audio_instance_index instance_index);
#endif

/// Set the maximum range of received audio.
///
/// Available since: 2.11.0
/// Description: Set the audio receiving range, the audio source sound beyond this range will not be received.
/// Use case: Set the receiver's receiving range in the `World` mode.
/// Default value: When this function is not called, only the voices of the members in the team can be received, and all voices outside the team cannot be received.
/// When to call: After initializing the range audio [createRangeAudio].
/// Restrictions: This range only takes effect for people outside the team.
///
/// @param range the audio range, the value must be greater than or equal to 0.
/// @param instance_index range audio instance index
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_audio_set_audio_receive_range(
    float range, enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_audio_set_audio_receive_range)(
    float range, enum zego_range_audio_instance_index instance_index);
#endif

/// Set the configuration of the audio receiving range.
///
/// Available since: 3.7.0
/// Description: Set the audio receiving range, the audio source sound beyond this range will not be received.
/// Use case: Set the receiver's receiving range in the `World` mode.
/// Default value: When this function is not called, only the voices of the members in the team can be received, and all voices outside the team cannot be received.
/// When to call: After initializing the range audio [createRangeAudio].
/// Restrictions: This range only takes effect for people outside the team.
///
/// @param param Configuration of audio receiving range.
/// @param instance_index range audio instance index
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_audio_set_audio_receive_range_with_param(
    struct zego_receive_range_param param, enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_audio_set_audio_receive_range_with_param)(
    struct zego_receive_range_param param, enum zego_range_audio_instance_index instance_index);
#endif

/// Set the frequency of real-time update locations within the SDK.
///
/// Available since: 2.21.0
/// Description: Set the frequency of real-time update locations within the SDK min 15 ms.
/// Use case: After setting the update position, the sensitivity of audio gradient is very high.
/// Default value: 100 ms.
/// When to call: After initializing the range audio [createRangeAudio].
///
/// @param frequency the frequency, the value must be greater than 15 ms.
/// @param instance_index range audio instance index
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_audio_set_position_update_frequency(
    int frequency, enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_audio_set_position_update_frequency)(
    int frequency, enum zego_range_audio_instance_index instance_index);
#endif

/// Set range voice volume.
///
/// Available since: 2.23.0
/// Description: Set range voice volume.
/// Use case: This interface allows you to increase or decrease the volume of a range voice stream when the user calls [startPlayingStream] and pulls another stream.
/// Default value: 100.
/// When to call: After initializing the range audio [createRangeAudio].
///
/// @param volume volume, [0,200].
/// @param instance_index range audio instance index
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_audio_set_audio_volume(
    int volume, enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_audio_set_audio_volume)(
    int volume, enum zego_range_audio_instance_index instance_index);
#endif

/// Set the sound range for the stream.
///
/// Available since: 2.23.0
/// Description: Set range voice volume.
/// Use case: When a user calls [startPlayingStream] and pulls another stream, the stream has a range speech effect by setting the range of sounds for that stream and calling [updateStreamPosition]. After the call will be the sound source of the sound range of the distance attenuation effect.
/// When to call: After initializing the range audio [createRangeAudio] and after [startPlayingStream].
/// Caution:  When calling [enableMicrophone] to enable range speech, the resource of the stream will be switched to RTC, regardless of whether the resource specified when [startPlayingStream] was originally called to pull the stream is RTC. If you really need to specify the resource of the stream as CDN, please configure it to pull a custom CDN stream and specify the CDN address information.
///
/// @param stream_id play stream id
/// @param vocal_range Flow sound range.
/// @param instance_index range audio instance index
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_audio_set_stream_vocal_range(
    const char *stream_id, float vocal_range, enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_audio_set_stream_vocal_range)(
    const char *stream_id, float vocal_range, enum zego_range_audio_instance_index instance_index);
#endif

/// Set the sound range for the stream.
///
/// Available since: 3.7.0
/// Description: Set range voice volume.
/// Use case: When a user calls [startPlayingStream] and pulls another stream, the stream has a range speech effect by setting the range of sounds for that stream and calling [updateStreamPosition]. After the call will be the sound source of the sound range of the distance attenuation effect.
/// When to call: After initializing the range audio [createRangeAudio] and after [startPlayingStream].
/// Caution:  When calling [enableMicrophone] to enable range speech, the resource of the stream will be switched to RTC, regardless of whether the resource specified when [startPlayingStream] was originally called to pull the stream is RTC. If you really need to specify the resource of the stream as CDN, please configure it to pull a custom CDN stream and specify the CDN address information.
///
/// @param stream_id play stream id
/// @param param Flow sound range.
/// @param instance_index range audio instance index
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_audio_set_stream_vocal_range_with_param(
    const char *stream_id, struct zego_vocal_range_param param,
    enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_audio_set_stream_vocal_range_with_param)(
    const char *stream_id, struct zego_vocal_range_param param,
    enum zego_range_audio_instance_index instance_index);
#endif

/// Update the location of the flow.
///
/// Available since: 2.23.0
/// Description: Set range voice volume.
/// Use case: When the user calls [startPlayingStream] to pull another stream, call [setStreamVocalRange] to set the stream's voice position, then call this interface to set the stream's position, so that the stream also has the range voice effect.
/// When to call: After initializing the range audio [createRangeAudio] and after [startPlayingStream].
///
/// @param stream_id play stream id.
/// @param position The unit vector of the front axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
/// @param instance_index range audio instance index
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_audio_update_stream_position(
    const char *stream_id, float position[3], enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_audio_update_stream_position)(
    const char *stream_id, float position[3], enum zego_range_audio_instance_index instance_index);
#endif

/// Update self position and orentation.
///
/// Available since: 2.11.0
/// Description: Update the user's position and orientation so that the SDK can calculate the distance between the user and the audio source and the stereo effect of the left and right ears.
/// Use case: When the role operated by the user in the game moves on the world map, the position information and head orientation of the role are updated.
/// When to call: Called after logging in to the room [loginRoom].
/// Caution: Before calling [enableSpeaker] to turn on the speaker, if you do not call this interface to set the location information, you will not be able to receive voices from other people except the team.
///
/// @param position The coordinates of the oneself in the world coordinate system. The parameter is a float array of length 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
/// @param axis_forward The unit vector of the front axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
/// @param axis_right The unit vector of the right axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
/// @param axis_up The unit vector of the up axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
/// @param instance_index range audio instance index
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_audio_update_self_position(
    float position[3], float axis_forward[3], float axis_right[3], float axis_up[3],
    enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_audio_update_self_position)(
    float position[3], float axis_forward[3], float axis_right[3], float axis_up[3],
    enum zego_range_audio_instance_index instance_index);
#endif

/// Add or update audio source position information.
///
/// Available since: 2.11.0
/// Description: Set the position of the audio source corresponding to the userID on the game map in the room, so that the SDK can calculate the distance and orientation of the listener to the audio source.
/// Use case: Update the position of the voice user in the game map coordinates.
/// When to call: Call [loginRoom] to call after logging in to the room, and the recorded audio source information will be cleared after logging out of the room.
///
/// @param userid The userID of the sound source.
/// @param position The coordinates of the speaker in the world coordinate system. The parameter is a float array of length 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
/// @param instance_index range audio instance index
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_audio_update_audio_source(
    const char *userid, float position[3], enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_audio_update_audio_source)(
    const char *userid, float position[3], enum zego_range_audio_instance_index instance_index);
#endif

/// Turn the 3D spatial sound on or off.
///
/// Available since: 2.11.0
/// Description: After the 3D sound effect is turned on, the sound effect in the actual space will be simulated according to the position of the speaker equivalent to the listener. The intuitive feeling is that the sound size and the left and right sound difference will also change when the distance and orientation of the sound source change.
/// Use case: It is a feature of audio recognition in FPS games or social scene games.
/// Default value: When this function is not called, 3D sound effects are turned off by default.
/// When to call: After initializing the range audio [createRangeAudio].
/// Caution: The 3D audio effect will only take effect when [setRangeAudioMode] is called and set to `World` mode.
/// Related APIs: After enabling the 3D sound effect, you can use [updateAudioSource] or [updateSelfPosition] to change the position and orientation to experience the 3D effect.
///
/// @param enable Whether to enable 3D sound effects.
/// @param instance_index range audio instance index
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_audio_enable_spatializer(
    bool enable, enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_audio_enable_spatializer)(
    bool enable, enum zego_range_audio_instance_index instance_index);
#endif

/// Turn the microphone on or off.
///
/// Available since: 2.11.0
/// Description: When enable is `true`, turn on the microphone and push audio stream; when it is `false`, turn off the microphone and stop pushing audio stream.
/// Use case: The user turns on or off the microphone to communicate in the room.
/// Default value: When this function is not called, the microphone is turned off by default.
/// When to call: After initializing the range audio [createRangeAudio] and login room [loginRoom].
/// Caution: Turning on the microphone will automatically use the main channel to push the audio stream.
/// Related callbacks: Get the microphone switch state change through the callback [onRangeAudioMicrophoneStateUpdate].
///
/// @param enable Whether to turn on the microphone.
/// @param instance_index range audio instance index
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_audio_enable_microphone(
    bool enable, enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_audio_enable_microphone)(
    bool enable, enum zego_range_audio_instance_index instance_index);
#endif

/// Turn the speaker on or off.
///
/// Available since: 2.11.0
/// Description: When enable is `true`, turn on the speaker and play audio stream; when it is `false`, turn off the speaker and stop playing audio stream.
/// Use case: The user turns on or off the speaker to communicate in the room.
/// Default value: When this function is not called, the speaker is turned off by default.
/// When to call: After initializing the range audio [createRangeAudio] and login room [loginRoom].
/// Caution: Turning on the speaker will automatically pull the audio stream in the room.
///
/// @param enable Whether to turn on the speaker.
/// @param instance_index range audio instance index
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_audio_enable_speaker(
    bool enable, enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_audio_enable_speaker)(
    bool enable, enum zego_range_audio_instance_index instance_index);
#endif

/// Set range audio mode.
///
/// Available since: 2.11.0
/// Description: The audio mode can be set to `World` mode or `Team` mode.
/// Use case: The user can choose to chat with everyone in the `World` mode (with distance limitation), or to communicate within the team in the `Team` mode (without distance limitation).
/// Default value: If this function is not called, the `World` mode is used by default.
/// When to call: After initializing the range audio [createRangeAudio].
/// Related APIs: In the `World` mode, you can set the sound receiving range [setAudioReceiveRange], in the `Team` mode, you need to set [setTeamID] to join the corresponding team to hear the voice in the team.
///
/// @param mode The range audio mode.
/// @param instance_index range audio instance index
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_range_audio_mode(
    enum zego_range_audio_mode mode, enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_range_audio_mode)(
    enum zego_range_audio_mode mode, enum zego_range_audio_instance_index instance_index);
#endif

/// Set range audio custom mode.
///
/// Available since: 3.3.0
/// Description: Can set the speak mode and listening mode respectively to control the speak and listening behavior in the world and team.
/// Use case: The user can decide who can listen to his voice by selecting the speak mode, and can also decide whose voice to listen to by selecting the listening mode.
/// Default value: If this interface is not called, the ZegoRangeAudioSpeakModeAll mode and ZegoRangeAudioListenModeAll mode is used by default.
/// When to call: After initializing the range audio [createRangeAudio].
/// Related APIs: When you want to listen to sounds from the world, you need to set the sound reception range [setAudioReceiveRange]. When you want to sound and listen in the squad, you need to set [setTeamID] to join the corresponding squad.
/// Restrictions: 1. Cannot be called with [setRangeAudioMode];
///  2. Not compatible with versions prior to 3.3.0.
///
/// @param speak_mode The range audio speak mode.
/// @param listen_mode The range audio listening mode.
/// @param instance_index range audio instance index
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_range_audio_custom_mode(
    enum zego_range_audio_speak_mode speak_mode, enum zego_range_audio_listen_mode listen_mode,
    enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_range_audio_custom_mode)(
    enum zego_range_audio_speak_mode speak_mode, enum zego_range_audio_listen_mode listen_mode,
    enum zego_range_audio_instance_index instance_index);
#endif

/// Set team ID.
///
/// Available: since 2.11.0
/// Description: After setting the team ID, you will be able to communicate with other users of the same team, and the sound will not change with the distance. It is also possible to exit the team by setting an empty string.
/// Use case: Users join the team or exit the team.
/// Default value: When this function is not called, no team will be added by default.
/// When to call: After initializing the range audio [createRangeAudio].
/// Caution: There will be no distance limit for the sounds in the team, and there will be no 3D sound effects.
///
/// @param team_id Team ID, empty to exit the team, a string of up to 64 bytes in length. Support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
/// @param instance_index range audio instance index
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_audio_set_team_id(
    const char *team_id, enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_audio_set_team_id)(
    const char *team_id, enum zego_range_audio_instance_index instance_index);
#endif

/// Whether can receive the audio data of the specified user.
///
/// Available since: 2.16.0
/// Description: In the process of real-time audio and video interaction, local users can use this function to control whether to receive audio data from designated remote users when pulling streams as needed. When the developer does not receive the audio receipt, the hardware and network overhead can be reduced.
/// Use cases: When developers need to quickly close and restore remote audio, they can call this function to enhance the interactive experience.
/// Default value: The default is `false`, which means to receive audio data from all users.
/// When to call: After initializing the range audio [createRangeAudio].
/// Caution: This function is valid only when the [muteAllPlayStreamAudio] function is set to `false`.
/// Related APIs: You can call the [muteAllPlayStreamAudio] function to control whether to receive all audio data. 1. When the [muteAllPlayStreamAudio(true)] function is called, it takes effect globally, that is, local users will be prohibited from receiving audio data from all remote users. At this time, the [muteUser] function will not take effect regardless of whether the [muteUser] function is called before or after [muteAllPlayStreamAudio]. 2. When the [muteAllPlayStreamAudio(false)] function is called, the local user can receive the audio data of all remote users. At this time, the [muteUser] function can be used to control whether to receive the audio data of the specified user. Calling the [muteUser(userID, true)] function allows the local user to receive audio data other than the `userID`; calling the [muteUser(userID, false)] function allows the local user to receive the audio data of the `userID`.
///
/// @param userid User ID.
/// @param mute Whether it can receive the audio data of the specified remote user, "true" means prohibition, "false" means receiving, the default value is "false".
/// @param instance_index range audio instance index
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_audio_mute_user(
    const char *userid, bool mute, enum zego_range_audio_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_audio_mute_user)(
    const char *userid, bool mute, enum zego_range_audio_instance_index instance_index);
#endif

/// Range audio microphone state callback.
///
/// Available since: 2.11.0
/// Description: The status change notification of the microphone, starting to send audio is an asynchronous process, and the state switching in the middle is called back through this function.
/// When to Trigger: After the [enableMicrophone] function.
/// Caution: 1. It must be monitored before the [enableMicrophone] function is called. 2. The callback is not triggered when the [enableMicrophone] is stopped normally, and the Off state is triggered only when the [enableMicrophone] is stopped abnormally.
///
/// @param state The use state of the range audio.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param instance_index Range audio instance index
/// @param user_context Context of user.
typedef void (*zego_on_range_audio_microphone_state_update)(
    enum zego_range_audio_microphone_state state, zego_error error_code,
    enum zego_range_audio_instance_index instance_index, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_audio_microphone_state_update_callback(
    zego_on_range_audio_microphone_state_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_audio_microphone_state_update_callback)(
    zego_on_range_audio_microphone_state_update callback_func, void *user_context);
#endif

ZEGO_END_DECLS

#endif
