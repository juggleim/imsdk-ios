
#ifndef __ZEGO_EXPRESS_MIXER_H__
#define __ZEGO_EXPRESS_MIXER_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Starts a stream mixing task.
///
/// Available since: 1.2.1
/// Description: Initiate a mixing stream request to the ZEGO RTC server, the server will look for the stream currently being pushed, and mix the layers according to the parameters of the mixing stream task requested by the SDK. When you need to update a mixing task, that is, when the input stream increases or decreases, you need to update the input stream list. At this time, you can update the field of the [ZegoMixerTask] object inputList and call this function again to pass in the same [ZegoMixerTask] object to update the mixing task.
/// Use cases: It is often used when multiple video images are required to synthesize a video using mixed streaming, such as education, live broadcast of teacher and student images.
/// When to call: After calling [loginRoom] to log in to the room.
/// Restrictions: None.
/// Caution: Due to the performance considerations of the client device, the SDK muxing is to start the mixing task on the ZEGO RTC server for mixing. If an exception occurs when the mixing task is requested to start, for example, the most common mixing input stream does not exist, the error code will be given from the callback callback. If a certain input stream does not exist in the middle, the muxing task will automatically retry to pull this input stream for 90 seconds, and will not retry after 90 seconds. If all input streams no longer exist, the server will automatically stop the mixing task after 90 seconds.
/// Related callbacks: [OnMixerRelayCDNStateUpdate] can be used to obtain the CDN status update notification of the mixed stream repost, and the sound update notification of each single stream in the mixed stream can be obtained through [onMixerSoundLevelUpdate].
/// Related APIs: the mixing task can be stopped by the [stopMixerTask] function.
///
/// @param task Mixing task object. Required: Yes.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_start_mixer_task(struct zego_mixer_task task,
                                                              zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_start_mixer_task)(struct zego_mixer_task task,
                                                               zego_seq *sequence);
#endif

/// Stops a stream mixing task.
///
/// Available since: 1.2.1
/// Description: Initiate a request to end the mixing task to the ZEGO RTC server.
/// Use cases: It is often used when multiple video images are required to synthesize a video using mixed streaming, such as education, live broadcast of teacher and student images.
/// When to call: After calling [startMixerTask] to start mixing.
/// Restrictions: None.
/// Caution: If the developer starts the next mixing task without stopping the previous mixing task, the previous mixing task will not automatically stop until the input stream of the previous mixing task does not exist for 90 seconds. Before starting the next mixing task, you should stop the previous mixing task, so that when an anchor has already started the next mixing task to mix with other anchors, the audience is still pulling the output stream of the previous mixing task.
/// Related APIs: You can start mixing by using the [startMixerTask] function.
///
/// @param task Mixing task object. Required: Yes.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_mixer_task(struct zego_mixer_task task,
                                                             zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_stop_mixer_task)(struct zego_mixer_task task,
                                                              zego_seq *sequence);
#endif

/// Starts an auto stream mixing task.
///
/// Available since: 2.10.0
/// Description: Local users can use this function to start an auto stream mixing task, all streams in a room wil be mixed. Currently, only audio streams can be mixed. When auto stream mixing is enabled, the audio of all streams in the room is automatically mixed, and any further pushed streams in the room are automatically mixed into the final output stream.
/// Use cases: Often used in voice chat room scenarios.
/// When to call: After the engine is created, user can call this function to enable auto stream mixing in the target room if the target room has already been created
/// Restrictions: Please refer to https://docs.zegocloud.com/article/7611 or contact ZEGO technical support.
/// Caution: Before starting the next auto stream mixing task in the same room, call [stopAutoMixerTask] function to end the last auto stream mixing task. Otherwise, when one anchor has started the next auto stream mixing task and other anchors have started the next auto stream mixing task, the audience still pulls the output stream of the first auto stream mixing task. If the user does not end the current auto stream mixing task, the task will automatically end after the room does not exist or the input streams do not exist for 90 seconds.
/// Related callbacks: Users can obtain the execution result of the function through [ZegoMixerStartCallback] callback.Users can get automatic mixed flow information through [onAutoMixerSoundLevelUpdate] callback.
/// Related APIs: Users can call [stopAutoMixerTask] function to stop an auto stream mixing task.
///
/// @param task Auto mix stream task object
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_start_auto_mixer_task(struct zego_auto_mixer_task task,
                                                                   zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_start_auto_mixer_task)(
    struct zego_auto_mixer_task task, zego_seq *sequence);
#endif

/// Stops an auto stream mixing task.
///
/// Available since: 2.10.0
/// Description: Local users can use this function to stop an auto stream mixing task.
/// Use cases: Often used in voice chat room scenarios.
/// When to call: Call this function after [startAutoMixerTask] function is called to start an auto stream mixing task.
/// Caution: Before calling [startAutoMixerTask] function to start the next auto stream mixing task in the same room, call this function to end the last auto stream mixing task. Otherwise, when one anchor has started the next auto stream mixing task and other anchors have started the next auto stream mixing task, the audience still pulls the output stream of the first auto stream mixing task. If the user does not end the current auto stream mixing task, the task will automatically end after the room does not exist or the input streams do not exist for 90 seconds.
/// Related callbacks: Users can obtain the execution result of the function through [ZegoMixerStopCallback] callback.
/// Related APIs: Users can call [startAutoMixerTask] function to start an auto stream mixing task.
///
/// @param task Auto mix stream task object
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_auto_mixer_task(struct zego_auto_mixer_task task,
                                                                  zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_stop_auto_mixer_task)(struct zego_auto_mixer_task task,
                                                                   zego_seq *sequence);
#endif

/// The callback triggered when the state of relayed streaming of the mixed stream to CDN changes.
///
/// Available since: 1.2.1
/// Description: The general situation of the mixing task on the ZEGO RTC server will push the output stream to the CDN using the RTMP protocol, and the state change during the push process will be notified from the callback function.
/// Use cases: It is often used when multiple video images are required to synthesize a video using mixed streaming, such as education, live teacher and student images.
/// When to trigger: After the developer calls the [startMixerTask] function to start mixing, when the ZEGO RTC server pushes the output stream to the CDN, there is a state change.
/// Restrictions: None.
/// Related callbacks: Develop can get the sound update notification of each single stream in the mixed stream through [OnMixerSoundLevelUpdate].
/// Related APIs: Develop can start a mixed flow task through [startMixerTask].
///
/// @param task_id The mixing task ID. Value range: the length does not exceed 256. Caution: This parameter is in string format and cannot contain URL keywords, such as 'http' and '?' etc., otherwise the push and pull flow will fail. Only supports numbers, English characters and'~','!','@','$','%','^','&','*','(',')','_' ,'+','=','-','`',';',''',',','.','<','>','\'.
/// @param info_list List of information that the current CDN is being mixed.
/// @param info_count List count of information that the current CDN is being mixed.
/// @param user_context User context. Value range: It is a custom pointer, which can be [NULL].
typedef void (*zego_on_mixer_relay_cdn_state_update)(
    const char *task_id, const struct zego_stream_relay_cdn_info *info_list,
    unsigned int info_count, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_mixer_relay_cdn_state_update_callback(
    zego_on_mixer_relay_cdn_state_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_mixer_relay_cdn_state_update_callback)(
    zego_on_mixer_relay_cdn_state_update callback_func, void *user_context);
#endif

/// The callback triggered when the sound level of any input stream changes in the stream mixing process.
///
/// Available since: 1.2.1
/// Description: Developers can use this callback to display the effect of which stream’s anchor is talking on the UI interface of the mixed stream of the audience.
/// Use cases: It is often used when multiple video images are required to synthesize a video using mixed streaming, such as education, live teacher and student images.
/// When to trigger: After the developer calls the [startPlayingStream] function to start playing the mixed stream. Callback notification period is 100 ms.
/// Restrictions: The callback is triggered every 100 ms, and the trigger frequency cannot be set.Due to the high frequency of this callback, please do not perform time-consuming tasks or UI operations in this callback to avoid stalling.
/// Related callbacks: [OnMixerRelayCDNStateUpdate] can be used to get update notification of mixing stream repost CDN status.
/// Related APIs: Develop can start a mixed flow task through [startMixerTask].
///
/// @param sound_levels The sound key-value pair of each single stream in the mixed stream, the key is the soundLevelID of each single stream, and the value is the sound value of the corresponding single stream. Value range: The value range of value is 0.0 ~ 100.0 (This value only represents the range of the sound level value returned by the callback, not the precision.).
/// @param info_count Count of mixing stream sound level.
/// @param user_context User context. Value range: It is a custom pointer, which can be [NULL].
typedef void (*zego_on_mixer_sound_level_update)(
    const struct zego_mixer_sound_level_info *sound_levels, unsigned int info_count,
    void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_mixer_sound_level_update_callback(
    zego_on_mixer_sound_level_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_mixer_sound_level_update_callback)(
    zego_on_mixer_sound_level_update callback_func, void *user_context);
#endif

/// The callback triggered when the sound level of any input stream changes in the auto stream mixing process.
///
/// Available since: 2.10.0
/// Description: According to this callback, user can obtain the sound level information of each stream pulled during auto stream mixing.
/// Use cases: Often used in voice chat room scenarios.Users can use this callback to show which streamer is speaking when an audience pulls a mixed stream.
/// Trigger: Call [startPlayingStream] function to pull the stream.
/// Related APIs: Users can call [startAutoMixerTask] function to start an auto stream mixing task.Users can call [stopAutoMixerTask] function to stop an auto stream mixing task.
///
/// @param sound_levels Sound level hash map, key is the streamID of every single stream in this mixer stream, value is the sound level value of that single stream, value ranging from 0.0 to 100.0 (This value only represents the range of the sound level value returned by the callback, not the precision.).
/// @param info_count Count of mixing stream sound level
/// @param user_context Context of user.
typedef void (*zego_on_auto_mixer_sound_level_update)(
    const struct zego_sound_level_info *sound_levels, unsigned int info_count, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_auto_mixer_sound_level_update_callback(
    zego_on_auto_mixer_sound_level_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_auto_mixer_sound_level_update_callback)(
    zego_on_auto_mixer_sound_level_update callback_func, void *user_context);
#endif

/// Results of starting a mixer task.
///
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param seq Sequence.
/// @param extended_data Extended Information
/// @param user_context Context of user.
typedef void (*zego_on_mixer_start_result)(zego_error error_code, zego_seq seq,
                                           const char *extended_data, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_mixer_start_result_callback(
    zego_on_mixer_start_result callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_mixer_start_result_callback)(
    zego_on_mixer_start_result callback_func, void *user_context);
#endif

/// Results of stoping a mixer task.
///
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param seq Sequence.
/// @param user_context Context of user.
typedef void (*zego_on_mixer_stop_result)(zego_error error_code, zego_seq seq, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_mixer_stop_result_callback(
    zego_on_mixer_stop_result callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_mixer_stop_result_callback)(
    zego_on_mixer_stop_result callback_func, void *user_context);
#endif

/// Results of starting a auto mixer task.
///
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param seq Sequence.
/// @param extended_data Extended Information
/// @param user_context Context of user.
typedef void (*zego_on_auto_mixer_start_result)(zego_error error_code, zego_seq seq,
                                                const char *extended_data, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_auto_mixer_start_result_callback(
    zego_on_auto_mixer_start_result callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_auto_mixer_start_result_callback)(
    zego_on_auto_mixer_start_result callback_func, void *user_context);
#endif

/// Results of stoping a auto mixer task.
///
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param seq Sequence.
/// @param user_context Context of user.
typedef void (*zego_on_auto_mixer_stop_result)(zego_error error_code, zego_seq seq,
                                               void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_auto_mixer_stop_result_callback(
    zego_on_auto_mixer_stop_result callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_auto_mixer_stop_result_callback)(
    zego_on_auto_mixer_stop_result callback_func, void *user_context);
#endif

ZEGO_END_DECLS

#endif
