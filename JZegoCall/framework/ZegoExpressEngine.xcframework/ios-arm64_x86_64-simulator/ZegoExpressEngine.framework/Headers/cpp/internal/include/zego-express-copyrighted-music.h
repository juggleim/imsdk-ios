
#ifndef __ZEGO_EXPRESS_COPYRIGHTED_MUSIC_H__
#define __ZEGO_EXPRESS_COPYRIGHTED_MUSIC_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Creates a copyrighted music instance.
///
/// Available since: 2.13.0
/// Description: Creates a copyrighted music instance.
/// Use case: Often used in online KTV chorus scenarios, users can use related functions by creating copyrighted music instance objects.
/// When to call: It can be called after the engine by [createEngine] has been initialized.
/// Restrictions: The SDK only supports the creation of one instance of CopyrightedMusic. Multiple calls to this function return the same object.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_create_copyrighted_music();
#else
typedef zego_error(EXP_CALL *pfnzego_express_create_copyrighted_music)();
#endif

/// Destroys a copyrighted music instance.
///
/// Available since: 2.13.0
/// Description: Destroys a copyrighted music instance.
/// When to call: It can be called before the engine by [destroyEngine]
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_destroy_copyrighted_music();
#else
typedef zego_error(EXP_CALL *pfnzego_express_destroy_copyrighted_music)();
#endif

/// Initialize the copyrighted music module.
///
/// Available since: 2.13.0
/// Description: Initialize the copyrighted music so that you can use the function of the copyrighted music later.
/// When to call: After call [createCopyrightedMusic] and [loginRoom].
/// Caution: 1. The real user information must be passed in, otherwise the song resources cannot be obtained for playback. 2. The user ID set when initializing copyrighted music needs to be the same as the user ID set when logging in to the room.
///
/// @param config the copyrighted music configuration.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_init(
    struct zego_copyrighted_music_config config, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_init)(
    struct zego_copyrighted_music_config config, zego_seq *sequence);
#endif

/// Get cache size.
///
/// Available since: 2.13.0
/// Description: When using this module, some cache files may be generated, and the size of the cache file can be obtained through this interface.
/// Use case: Used to display the cache size of the App.
/// When to call: After initializing the copyrighted music [createCopyrightedMusic].
///
/// @param cache_size [in/out] cache file size, in byte.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_copyrighted_music_get_cache_size(unsigned long long *cache_size);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_get_cache_size)(
    unsigned long long *cache_size);
#endif

/// Clear cache.
///
/// Available since: 2.13.0
/// Description: When using this module, some cache files may be generated, which can be cleared through this interface.
/// Use case: Used to clear the cache of the App.
/// When to call: After initializing the copyrighted music [createCopyrightedMusic].
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_clear_cache();
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_clear_cache)();
#endif

/// Send extended feature request.
///
/// Available since: 2.13.0
/// Description: Initialize the copyrighted music so that you can use the function of the copyrighted music later.
/// Use case: Used to get a list of songs.
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
///
/// @param command request command, details about the commands supported.
/// @param params request parameters, each request command has corresponding request parameters.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_send_extended_request(
    const char *command, const char *params, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_send_extended_request)(
    const char *command, const char *params, zego_seq *sequence);
#endif

/// Get lyrics in lrc format.
///
/// Available since: 3.2.1
/// Description: Get lyrics in lrc format, support parsing lyrics line by line.
/// Use case: Used to display lyrics line by line.
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
///
/// @param song_id the ID of the song or accompaniment, the song and accompaniment of a song share the same ID.
/// @param vendor_id Copyright music resource song copyright provider.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_get_lrc_lyric_with_vendor(
    const char *song_id, enum zego_copyrighted_music_vendor_id vendor_id, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_get_lrc_lyric_with_vendor)(
    const char *song_id, enum zego_copyrighted_music_vendor_id vendor_id, zego_seq *sequence);
#endif

/// Get lyrics in lrc format.
///
/// Available since: 3.12.0
/// Description: Get lyrics in lrc format, support parsing lyrics line by line.
/// Use case: Used to display lyrics line by line.
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
///
/// @param config The configuration of get lyric.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_get_lrc_lyric_with_config(
    struct zego_copyrighted_music_get_lyric_config config, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_get_lrc_lyric_with_config)(
    struct zego_copyrighted_music_get_lyric_config config, zego_seq *sequence);
#endif

/// Get lyrics in krc format.
///
/// Available since: 2.13.0
/// Description: Get verbatim lyrics, support parsing lyrics word by word.
/// Use case: Used to display lyrics word by word.
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
///
/// @param krc_token The krcToken obtained when calling [requestResource] for accompaniment or climax clips, or when obtaining shared resources through the [getSharedResource] interface. For more details, please refer to https://doc-zh.zego.im/article/15079#2_2
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_copyrighted_music_get_krc_lyric_by_token(const char *krc_token, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_get_krc_lyric_by_token)(
    const char *krc_token, zego_seq *sequence);
#endif

/// Request music resource.
///
/// Available since: 3.0.2
/// Description: In addition to obtaining the basic information of the song (duration, song name, singer, etc.), and the most important resource id that can be used for local playback, there are also some related authentications information.
/// Use case: Get copyrighted songs for local playback and sharing.
/// Related APIs: After a user in the room successfully calls this interface to request a music resource, other users in the room can call the [getsharedresource] interface to get the music resource for free once.
/// When to call: After initializing the copyrighted music [initCopyrightedMusic].
/// Note:
///   1. Each resource has a unique resource ID.
///   2. Every time this API is called, it will be billed once, please consult ZEGO business personnel for details.
///   3. Each resource has a unique resource ID. The resources obtained by calling this API are time-sensitive, the valid duration is the minimum value between the SDK life cycle and 24-hour.
///
/// @param config The configuration of requesting music resource.
/// @param type The resource type of music.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_request_resource(
    struct zego_copyrighted_music_request_config config,
    enum zego_copyrighted_music_resource_type type, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_request_resource)(
    struct zego_copyrighted_music_request_config config,
    enum zego_copyrighted_music_resource_type type, zego_seq *sequence);
#endif

/// Request music resource.
///
/// Available since: 3.12.0
/// Description: In addition to obtaining the basic information of the song (duration, song name, singer, etc.), and the most important resource id that can be used for local playback, there are also some related authentications information.
/// Use case: Get copyrighted songs for local playback and sharing.
/// Related APIs: After a user in the room successfully calls this interface to request a music resource, other users in the room can call the [getsharedresource] interface to get the music resource for free once.
/// When to call: After initializing the copyrighted music [initCopyrightedMusic].
/// Note:
///   1. Each resource has a unique resource ID.
///   2. Every time this API is called, it will be billed once, please consult ZEGO business personnel for details.
///   3. Each resource has a unique resource ID. The resources obtained by calling this API are time-sensitive, the valid duration is the minimum value between the SDK life cycle and 24-hour.
///
/// @param config The configuration of requesting music resource.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_request_resource_v2(
    zego_copyrighted_music_request_config_v2 config, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_request_resource_v2)(
    zego_copyrighted_music_request_config_v2 config, zego_seq *sequence);
#endif

/// Get shared music resource.
///
/// Available since: 3.0.2
/// Description: In addition to obtaining the basic information of the song (duration, song name, singer, etc.), and the most important resource id that can be used for local playback, there are also some related authentications information.
/// Use case: Get copyrighted songs for local playback.
/// Related APIs: After a user in the room calls the [requestresource] interface to request a music resource successfully, other users in the room can call this interface to get the music resource for free once.
/// When to call: After initializing the copyrighted music [initCopyrightedMusic].
/// Note: Each resource has a unique resource ID. The resources obtained by calling this API are time-sensitive, the valid duration is the minimum value between the SDK life cycle and 24-hour.
///
/// @param config The configuration of getting shared music resource.
/// @param type The resource type of music.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_get_shared_resource(
    struct zego_copyrighted_music_get_shared_config config,
    enum zego_copyrighted_music_resource_type type, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_get_shared_resource)(
    struct zego_copyrighted_music_get_shared_config config,
    enum zego_copyrighted_music_resource_type type, zego_seq *sequence);
#endif

/// Get shared music resource.
///
/// Available since: 3.12.0
/// Description: In addition to obtaining the basic information of the song (duration, song name, singer, etc.), and the most important resource id that can be used for local playback, there are also some related authentications information.
/// Use case: Get copyrighted songs for local playback.
/// Related APIs: After a user in the room calls the [requestresource] interface to request a music resource successfully, other users in the room can call this interface to get the music resource for free once.
/// When to call: After initializing the copyrighted music [initCopyrightedMusic].
/// Note: Each resource has a unique resource ID. The resources obtained by calling this API are time-sensitive, the valid duration is the minimum value between the SDK life cycle and 24-hour.
///
/// @param config The configuration of getting shared music resource.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_get_shared_resource_v2(
    zego_copyrighted_music_get_shared_config_v2 config, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_get_shared_resource_v2)(
    zego_copyrighted_music_get_shared_config_v2 config, zego_seq *sequence);
#endif

/// Download music resource.
///
/// Available since: 2.13.0
/// Description: Download music resource. It can only be played after downloading successfully.
/// Use case: After obtaining the music resource authorization, use this interface to download the corresponding resources.
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
/// Caution: Downloading copyrighted music resources is affected by the Internet.
///
/// @param resource_id the resource ID corresponding to the song or accompaniment.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_download(const char *resource_id,
                                                                        zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_download)(const char *resource_id,
                                                                         zego_seq *sequence);
#endif

/// Cancel download of music resource.
///
/// Available since: 3.11.0
/// Description: Cancel download of music resource.
/// Use case: After starting to download music resources, use this interface to cancel the corresponding download task.
/// When to call: After starting the download [download].
/// Caution: When a valid resourceID is passed in, only unfinished download tasks will be cancelled. When empty is passed in, all unfinished download tasks will be canceled.
///
/// @param resource_id the resource ID corresponding to the song or accompaniment.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_copyrighted_music_cancel_download(const char *resource_id);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_cancel_download)(
    const char *resource_id);
#endif

/// Query the resource's cache is existed or not.
///
/// Available since: 3.9.0
/// Description: Query the resource is existed or not, query the Yinsuda resource cache by default
/// Use case: Can be used to check the resource's cache is existed or not
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
///
/// @param config The configuration of querying cache.
/// @param is_cache [in/out] the resource's cache is existed or not
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_query_cache_with_config(
    struct zego_copyrighted_music_query_cache_config config, bool *is_cache);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_query_cache_with_config)(
    struct zego_copyrighted_music_query_cache_config config, bool *is_cache);
#endif

/// Query the resource's cache is existed or not.
///
/// Available since: 3.12.0
/// Description: Query the resource is existed or not, query the Yinsuda resource cache by default
/// Use case: Can be used to check the resource's cache is existed or not
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
///
/// @param config The configuration of querying cache.
/// @param is_cache [in/out] the resource's cache is existed or not
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_query_cache_with_config_v2(
    zego_copyrighted_music_query_cache_config_v2 config, bool *is_cache);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_query_cache_with_config_v2)(
    zego_copyrighted_music_query_cache_config_v2 config, bool *is_cache);
#endif

/// Get the playing time of a song or accompaniment file.
///
/// Available since: 2.13.0
/// Description: Get the playing time of a song or accompaniment file.
/// Use case: Can be used to display the playing time information of the song or accompaniment on the view.
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
///
/// @param resource_id the resource ID corresponding to the song or accompaniment.
/// @param duration [in/out] The playing time of a song or accompaniment file.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_copyrighted_music_get_duration(const char *resource_id, unsigned long long *duration);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_get_duration)(
    const char *resource_id, unsigned long long *duration);
#endif

/// Set the difficulty level of scoring.
///
/// Available since: 2.22.0
/// Description: Users can set the scoring difficulty level through this function.
/// When to call: After calling [initCopyrightedMusic] to initialize copyrighted music successfully, call [startScore] to start scoring.
/// Default value: When this function is not called, the difficulty level of scoring is 4.
/// Restrictions: This function does not support dynamic settings. After calling this function successfully, the next call to [startScore] will take effect.
///
/// @param level The difficulty level of scoring. The level ranges from 0 to 4. The scoring difficulty decreases from 0 to 4.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_set_scoring_level(int level);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_set_scoring_level)(int level);
#endif

/// Start scoring.
///
/// Available since: 2.15.0
/// Description: Start the scoring function.After starting scoring, the scoring result OnCurrentPitchValueUpdate callback will be received according to the set callback time interval.
/// Use case: Can be used to display the singing score on the view.
/// When to call: After obtaining verbatim lyrics and playing the accompaniment resources of copyrighted music.
/// Restrictions: Only support use this api after [startPublishingStream].
///
/// @param resource_id the resource ID corresponding to the accompaniment or accompaniment clip.
/// @param pitch_value_interval the time interval of real-time pitch line callback, in milliseconds, the default is 50 milliseconds.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_copyrighted_music_start_score(const char *resource_id, int pitch_value_interval);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_start_score)(
    const char *resource_id, int pitch_value_interval);
#endif

/// Pause scoring.
///
/// Available since: 2.15.0
/// Description: Pause ongoing scoring,will stop the [OnCurrentPitchValueUpdate] callback.
/// Use case: You can call this interface to pause the scoring function while scoring.
/// When to call: It can be called while grading.
///
/// @param resource_id the resource ID corresponding to the accompaniment or accompaniment clip.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_pause_score(const char *resource_id);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_pause_score)(
    const char *resource_id);
#endif

/// Resume scoring.
///
/// Available since: 2.15.0
/// Description: Resume currently paused scoring.
/// Use case: When there is currently paused scoring, this interface can be called to resume the scoring function.
/// When to call: It can be called when there is currently a paused scoring.
///
/// @param resource_id the resource ID corresponding to the accompaniment or accompaniment clip.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_copyrighted_music_resume_score(const char *resource_id);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_resume_score)(
    const char *resource_id);
#endif

/// Stop scoring.
///
/// Available since: 2.15.0
/// Description: End the current rating.The [OnCurrentPitchValueUpdate] callback will be stopped, but the average or total score can still be obtained normally.
/// Use case: You can call this interface to end the scoring while scoring.
/// When to call: It can be called while grading.
///
/// @param resource_id the resource ID corresponding to the accompaniment or accompaniment clip.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_stop_score(const char *resource_id);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_stop_score)(const char *resource_id);
#endif

/// Reset scoring.
///
/// Available since: 2.15.0
/// Description: Reset the scores that have already been performed,The [OnCurrentPitchValueUpdate] callback will be stopped and the average or total score will be 0.
/// Use case: Often used in scenes where the same song is re-sung.
/// When to call: It can be called after scoring has been performed.
///
/// @param resource_id the resource ID corresponding to the accompaniment or accompaniment clip.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_reset_score(const char *resource_id);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_reset_score)(
    const char *resource_id);
#endif

/// Get the score of the previous sentence.
///
/// Available since: 2.15.0
/// Description: Get the score of the previous sentence.
/// Use case: Can be used to display the score of each sentence on the view.
/// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
///
/// @param resource_id the resource ID corresponding to the accompaniment or accompaniment clip.
/// @param total_score [in/out] The score of the previous sentence.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_copyrighted_music_get_previous_score(const char *resource_id, int *total_score);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_get_previous_score)(
    const char *resource_id, int *total_score);
#endif

/// Get average score.
///
/// Available since: 2.15.0
/// Description: Get the average score.
/// Use case: Can be used to display the average score on the view.
/// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
///
/// @param resource_id the resource ID corresponding to the accompaniment or accompaniment clip.
/// @param total_score [in/out] Average score.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_copyrighted_music_get_average_score(const char *resource_id, int *total_score);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_get_average_score)(
    const char *resource_id, int *total_score);
#endif

/// Get total score .
///
/// Available since: 2.15.0
/// Description: Get the total score.
/// Use case: Can be used to display the total score on the view.
/// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
///
/// @param resource_id the resource ID corresponding to the accompaniment or accompaniment clip.
/// @param total_score [in/out] total score.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_copyrighted_music_get_total_score(const char *resource_id, int *total_score);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_get_total_score)(
    const char *resource_id, int *total_score);
#endif

/// Get full score .
///
/// Available since: 3.0.2
/// Description: Get the full score.
/// Use case: Can be used to display the full score on the view.
/// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
///
/// @param resource_id the resource ID corresponding to the accompaniment or accompaniment clip.
/// @param full_score [in/out] full score.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_copyrighted_music_get_full_score(const char *resource_id, int *full_score);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_get_full_score)(
    const char *resource_id, int *full_score);
#endif

/// Get standard pitch data.
///
/// Available since: 2.15.0
/// Description: Get standard pitch data.
/// Use case: Can be used to display standard pitch lines on the view.
/// Caution: Only accompaniment or climactic clip assets have pitch lines.
///
/// @param resource_id the resource ID corresponding to the accompaniment or accompaniment clip.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_copyrighted_music_get_standard_pitch(const char *resource_id, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_get_standard_pitch)(
    const char *resource_id, zego_seq *sequence);
#endif

/// Get real-time pitch data.
///
/// Available since: 2.15.0
/// Description: Get real-time pitch data.
/// Use case: Can be used to display real-time pitch lines on the view.
/// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
///
/// @param resource_id the resource ID corresponding to the song or accompaniment.
/// @param pitch [in/out] real-time pitch data.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_copyrighted_music_get_current_pitch(const char *resource_id, int *pitch);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_get_current_pitch)(
    const char *resource_id, int *pitch);
#endif

/// Callback for download song or accompaniment progress rate.
///
/// @param seq Sequence.
/// @param resource_id The resource ID of the song or accompaniment that triggered this callback.
/// @param progress_rate download progress rate.
/// @param user_context Context of user.
typedef void (*zego_on_copyrighted_music_download_progress_update)(zego_seq seq,
                                                                   const char *resource_id,
                                                                   float progress_rate,
                                                                   void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_copyrighted_music_download_progress_update_callback(
    zego_on_copyrighted_music_download_progress_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_copyrighted_music_download_progress_update_callback)(
    zego_on_copyrighted_music_download_progress_update callback_func, void *user_context);
#endif

/// Real-time pitch line callback.
///
/// @param resource_id The resource ID of the song or accompaniment that triggered this callback.
/// @param current_duration Current playback progress.
/// @param pitch_value Real-time pitch accuracy or value.
/// @param user_context Context of user.
typedef void (*zego_on_copyrighted_music_current_pitch_value_update)(const char *resource_id,
                                                                     int current_duration,
                                                                     int pitch_value,
                                                                     void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_copyrighted_music_current_pitch_value_update_callback(
    zego_on_copyrighted_music_current_pitch_value_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_copyrighted_music_current_pitch_value_update_callback)(
    zego_on_copyrighted_music_current_pitch_value_update callback_func, void *user_context);
#endif

/// [Deprecated] Request a song. Deprecated since 3.0.2, please use the [requestResource] function instead.
///
/// Available since: 2.13.0
/// Description: By requesting a song, you can not only obtain basic information about a song (such as duration, song name, and artist), but also obtain the resource ID for local playback, share_token for sharing with others, and related authentication information. Support by the time, by the user monthly, by the room monthly subscription three ways.
/// Use case: Get copyrighted songs for local playback and sharing.
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
/// Caution: This interface will trigger billing. A song may have three sound qualities: normal, high-definition, and lossless. Each sound quality has a different resource file, and each resource file has a unique resource ID.
///
/// @deprecated Deprecated since 3.0.2, please use the [requestResource] function instead.
/// @param config request configuration.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_request_song(
    struct zego_copyrighted_music_request_config config, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_request_song)(
    struct zego_copyrighted_music_request_config config, zego_seq *sequence);
#endif

/// [Deprecated] Request accompaniment. Deprecated since 3.0.2, please use the [requestResource] function instead.
///
/// Available since: 2.13.0
/// Description: You can get the accompaniment resources of the song corresponding to the songID, including resource_id, krc_token, share_token, etc. Supports click-by-point accompaniment.
/// Use case: Get copyrighted accompaniment for local playback and sharing.
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
/// Caution: This interface will trigger billing.
///
/// @deprecated Deprecated since 3.0.2, please use the [requestResource] function instead.
/// @param config request configuration.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_request_accompaniment(
    struct zego_copyrighted_music_request_config config, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_request_accompaniment)(
    struct zego_copyrighted_music_request_config config, zego_seq *sequence);
#endif

/// [Deprecated] Request accompaniment clip. Deprecated since 3.0.2, please use the [requestResource] function instead.
///
/// Available since: 2.13.0
/// Description: You can get the accompaniment clip resources of the song corresponding to the songID, including resource_id, krc_token, share_token, etc. Supports accompaniment clips by pay-per-use.
/// Use case: Get copyrighted accompaniment clip for local playback and sharing.
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
/// Caution: This interface will trigger billing.
///
/// @deprecated Deprecated since 3.0.2, please use the [requestResource] function instead.
/// @param config request configuration.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_request_accompaniment_clip(
    struct zego_copyrighted_music_request_config config, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_request_accompaniment_clip)(
    struct zego_copyrighted_music_request_config config, zego_seq *sequence);
#endif

/// [Deprecated] Get a song or accompaniment. Deprecated since 3.0.2, please use the [getSharedResource] function instead.
///
/// Available since: 2.13.0
/// Description: After the user successfully obtains the song/accompaniment/accompaniment clip resource, he can get the corresponding shareToken, share the shareToken with other users, and other users call this interface to obtain the shared music resources.
/// Use case: In the online KTV scene, after receiving the song or accompaniment token shared by the lead singer, the chorus obtains the corresponding song or accompaniment through this interface, and then plays it on the local end.
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
///
/// @deprecated Deprecated since 3.0.2, please use the [getSharedResource] function instead.
/// @param share_token access the corresponding authorization token for a song or accompaniment.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_copyrighted_music_get_music_by_token(const char *share_token, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_get_music_by_token)(
    const char *share_token, zego_seq *sequence);
#endif

/// [Deprecated] Get lyrics in lrc format. Deprecated since 3.2.1, please use the method with the same name with [vendorID] parameter instead.
///
/// Available since: 2.13.0
/// Description: Get lyrics in lrc format, support parsing lyrics line by line.
/// Use case: Used to display lyrics line by line.
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
///
/// @deprecated Deprecated since 2.14.0, please use the method with the same name with [vendorID] parameter instead.
/// @param song_id the ID of the song or accompaniment, the song and accompaniment of a song share the same ID.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_get_lrc_lyric(const char *song_id,
                                                                             zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_get_lrc_lyric)(const char *song_id,
                                                                              zego_seq *sequence);
#endif

/// [Deprecated] Query the resource's cache is existed or not. Deprecated since 3.2.1, please use the method with the same name with [vendorID] parameter instead.
///
/// Available since: 2.13.0
/// Description: Query the resource is existed or not.
/// Use case: Can be used to check the resource's cache is existed or not
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
///
/// @deprecated Deprecated since 2.14.0, please use the method with the same name with [vendorID] parameter instead.
/// @param song_id the ID of the song or accompaniment, the song and accompaniment of a song share the same ID.
/// @param type the song resource type.
/// @param is_cache [in/out] the resource's cache is existed or not
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_query_cache(
    const char *song_id, enum zego_copyrighted_music_type type, bool *is_cache);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_query_cache)(
    const char *song_id, enum zego_copyrighted_music_type type, bool *is_cache);
#endif

/// [Deprecated] Query the resource's cache is existed or not. Deprecated since 3.9.0, please use the method with the same name with [config] parameter instead.
///
/// Available since: 3.2.1
/// Description: Query the resource is existed or not, query the Yinsuda resource cache by default
/// Use case: Can be used to check the resource's cache is existed or not
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
///
/// @deprecated Deprecated since 3.9.0, please use the method with the same name with [config] parameter instead.
/// @param song_id the ID of the song or accompaniment, the song and accompaniment of a song share the same ID.
/// @param type the song resource type.
/// @param vendor_id Copyright music resource song copyright provider.
/// @param is_cache [in/out] the resource's cache is existed or not
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_copyrighted_music_query_cache_with_vendor(
    const char *song_id, enum zego_copyrighted_music_type type,
    enum zego_copyrighted_music_vendor_id vendor_id, bool *is_cache);
#else
typedef zego_error(EXP_CALL *pfnzego_express_copyrighted_music_query_cache_with_vendor)(
    const char *song_id, enum zego_copyrighted_music_type type,
    enum zego_copyrighted_music_vendor_id vendor_id, bool *is_cache);
#endif

/// Callback for copyrighted music init.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param user_context Context of user.
typedef void (*zego_on_copyrighted_music_init)(zego_seq seq, zego_error error_code,
                                               void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_copyrighted_music_init_callback(
    zego_on_copyrighted_music_init callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_copyrighted_music_init_callback)(
    zego_on_copyrighted_music_init callback_func, void *user_context);
#endif

/// Callback of sending extended feature request.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param command request command.
/// @param result request result, each request command has corresponding request result.
/// @param user_context Context of user.
typedef void (*zego_on_copyrighted_music_send_extended_request)(zego_seq seq, zego_error error_code,
                                                                const char *command,
                                                                const char *result,
                                                                void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_copyrighted_music_send_extended_request_callback(
    zego_on_copyrighted_music_send_extended_request callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_copyrighted_music_send_extended_request_callback)(
    zego_on_copyrighted_music_send_extended_request callback_func, void *user_context);
#endif

/// Get lrc format lyrics complete callback.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param lyrics lrc format lyrics.
/// @param user_context Context of user.
typedef void (*zego_on_copyrighted_music_get_lrc_lyric)(zego_seq seq, zego_error error_code,
                                                        const char *lyrics, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_copyrighted_music_get_lrc_lyric_callback(
    zego_on_copyrighted_music_get_lrc_lyric callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_copyrighted_music_get_lrc_lyric_callback)(
    zego_on_copyrighted_music_get_lrc_lyric callback_func, void *user_context);
#endif

/// Get verbatim lyrics complete callback.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param lyrics verbatim lyrics.
/// @param user_context Context of user.
typedef void (*zego_on_copyrighted_music_get_krc_lyric_by_token)(zego_seq seq,
                                                                 zego_error error_code,
                                                                 const char *lyrics,
                                                                 void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_copyrighted_music_get_krc_lyric_by_token_callback(
    zego_on_copyrighted_music_get_krc_lyric_by_token callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_copyrighted_music_get_krc_lyric_by_token_callback)(
    zego_on_copyrighted_music_get_krc_lyric_by_token callback_func, void *user_context);
#endif

/// Callback for request song.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource The JSON string returned by the song ordering service, including song resource information.
/// @param user_context Context of user.
typedef void (*zego_on_copyrighted_music_request_song)(zego_seq seq, zego_error error_code,
                                                       const char *resource, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_copyrighted_music_request_song_callback(
    zego_on_copyrighted_music_request_song callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_copyrighted_music_request_song_callback)(
    zego_on_copyrighted_music_request_song callback_func, void *user_context);
#endif

/// Callback for request accompaniment.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource accompany resource information.
/// @param user_context Context of user.
typedef void (*zego_on_copyrighted_music_request_accompaniment)(zego_seq seq, zego_error error_code,
                                                                const char *resource,
                                                                void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_copyrighted_music_request_accompaniment_callback(
    zego_on_copyrighted_music_request_accompaniment callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_copyrighted_music_request_accompaniment_callback)(
    zego_on_copyrighted_music_request_accompaniment callback_func, void *user_context);
#endif

/// Callback for request accompaniment clip.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource accompany clip resource information.
/// @param user_context Context of user.
typedef void (*zego_on_copyrighted_music_request_accompaniment_clip)(zego_seq seq,
                                                                     zego_error error_code,
                                                                     const char *resource,
                                                                     void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_copyrighted_music_request_accompaniment_clip_callback(
    zego_on_copyrighted_music_request_accompaniment_clip callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_copyrighted_music_request_accompaniment_clip_callback)(
    zego_on_copyrighted_music_request_accompaniment_clip callback_func, void *user_context);
#endif

/// Callback for acquire songs or accompaniment through authorization token.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource song or accompany resource information.
/// @param user_context Context of user.
typedef void (*zego_on_copyrighted_music_get_music_by_token)(zego_seq seq, zego_error error_code,
                                                             const char *resource,
                                                             void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_copyrighted_music_get_music_by_token_callback(
    zego_on_copyrighted_music_get_music_by_token callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_copyrighted_music_get_music_by_token_callback)(
    zego_on_copyrighted_music_get_music_by_token callback_func, void *user_context);
#endif

/// Callback of requesting music resource.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource The JSON string returned by the song ordering service, including music resource information.
/// @param user_context Context of user.
typedef void (*zego_on_copyrighted_music_request_resource)(zego_seq seq, zego_error error_code,
                                                           const char *resource,
                                                           void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_copyrighted_music_request_resource_callback(
    zego_on_copyrighted_music_request_resource callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_copyrighted_music_request_resource_callback)(
    zego_on_copyrighted_music_request_resource callback_func, void *user_context);
#endif

/// Callback of getting shared music resource.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource The JSON string returned by the song ordering service, including music resource information.
/// @param user_context Context of user.
typedef void (*zego_on_copyrighted_music_get_shared_resource)(zego_seq seq, zego_error error_code,
                                                              const char *resource,
                                                              void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_copyrighted_music_get_shared_resource_callback(
    zego_on_copyrighted_music_get_shared_resource callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_copyrighted_music_get_shared_resource_callback)(
    zego_on_copyrighted_music_get_shared_resource callback_func, void *user_context);
#endif

/// Callback for download song or accompaniment.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param user_context Context of user.
typedef void (*zego_on_copyrighted_music_download)(zego_seq seq, zego_error error_code,
                                                   void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_copyrighted_music_download_callback(
    zego_on_copyrighted_music_download callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_copyrighted_music_download_callback)(
    zego_on_copyrighted_music_download callback_func, void *user_context);
#endif

/// Get standard pitch data complete callback.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param pitch Standard pitch data.
/// @param user_context Context of user.
typedef void (*zego_on_copyrighted_music_get_standard_pitch)(zego_seq seq, zego_error error_code,
                                                             const char *pitch, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_copyrighted_music_get_standard_pitch_callback(
    zego_on_copyrighted_music_get_standard_pitch callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_copyrighted_music_get_standard_pitch_callback)(
    zego_on_copyrighted_music_get_standard_pitch callback_func, void *user_context);
#endif

ZEGO_END_DECLS

#endif
