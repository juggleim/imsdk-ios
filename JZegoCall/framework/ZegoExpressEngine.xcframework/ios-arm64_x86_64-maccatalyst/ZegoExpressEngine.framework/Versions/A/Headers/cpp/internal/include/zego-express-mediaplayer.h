
#ifndef __ZEGO_EXPRESS_MEDIAPLAYER_H__
#define __ZEGO_EXPRESS_MEDIAPLAYER_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Creates a media player instance.
///
/// Available since: 2.1.0
/// Description: Creates a media player instance.
/// Use case: It is often used to play media resource scenes, For example, play video files, push the video of media resources in combination with custom video acquisition, and the remote end can pull the stream for viewing.
/// When to call: It can be called after the SDK by [createEngine] has been initialized.
/// Restrictions: Currently, a maximum of 4 instances can be created, after which it will return NULL.
/// Caution: The more instances of a media player, the greater the performance overhead on the device.
/// Related APIs: User can call [destroyMediaPlayer] function to destroy a media player instance.
///
/// @param instance_index [in/out] Media player instance, -1 will be returned when the maximum number is exceeded.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_create_media_player(enum zego_media_player_instance_index *instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_create_media_player)(
    enum zego_media_player_instance_index *instance_index);
#endif

/// Destroys a media player instance.
///
/// Available since: 2.1.0
/// Description: Destroys a media player instance.
/// Related APIs: User can call [createMediaPlayer] function to create a media player instance.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_destroy_media_player(enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_destroy_media_player)(
    enum zego_media_player_instance_index instance_index);
#endif

/// Load local or network media resource.
///
/// Available: since 1.3.4
/// Description: Load media resources.
/// Use case: Developers can load the absolute path to the local resource or the URL of the network resource incoming.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Related APIs: Resources can be loaded through the [loadResourceWithPosition] or [loadResourceFromMediaData] function.
/// Caution: If the mediaplayer has already loaded resources or is in the process of playing, please first call the [stop] interface to halt the playback, and then proceed to call the interface to load the media resources; failure to do so will result in an unsuccessful load.
///
/// @param path The absolute resource path or the URL of the network resource and cannot be NULL or "". Android can set this path string with Uri.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_load_resource(
    const char *path, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_load_resource)(
    const char *path, enum zego_media_player_instance_index instance_index);
#endif

/// Load local or network media resource and specify the start position.
///
/// Available: since 2.14.0
/// Description: Load media resources, and specify the progress, in milliseconds, at which playback begins.
/// Use case: Developers can load the absolute path to the local resource or the URL of the network resource incoming.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Related APIs: Resources can be loaded through the [loadResource] or [loadResourceFromMediaData] function.
/// Caution: 1.When [startPosition] exceeds the total playing time, it will start playing from the beginning.
///  2.If the mediaplayer has already loaded resources or is in the process of playing, please first call the [stop] interface to halt the playback, and then proceed to call the interface to load the media resources; failure to do so will result in an unsuccessful load.
///
/// @param path The absolute resource path or the URL of the network resource and cannot be NULL or "". Android can set this path string with Uri.
/// @param start_position The progress at which the playback started.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_load_resource_with_position(
    const char *path, long start_position, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_load_resource_with_position)(
    const char *path, long start_position, enum zego_media_player_instance_index instance_index);
#endif

/// Load binary audio resource.
///
/// Available: since 2.10.0
/// Description: Load binary audio data.
/// Use case: Developers do not want to cache the audio data locally, and directly transfer the audio binary data to the media player, directly load and play the audio.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Related APIs: Resources can be loaded through the [loadResource] or [loadResourceWithPosition] function.
/// Caution: 1.When [startPosition] exceeds the total playing time, it will start playing from the beginning.
///  2.If the mediaplayer has already loaded resources or is in the process of playing, please first call the [stop] interface to halt the playback, and then proceed to call the interface to load the media resources; failure to do so will result in an unsuccessful load.
///
/// @param media_data Binary audio data.
/// @param media_data_length The length of the binary audio data.
/// @param start_position Position of starting playback, in milliseconds.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_load_resource_from_media_data(
    unsigned char *media_data, int media_data_length, long start_position,
    enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_load_resource_from_media_data)(
    unsigned char *media_data, int media_data_length, long start_position,
    enum zego_media_player_instance_index instance_index);
#endif

/// Load copyrighted music resource.
///
/// Available: since 2.14.0
/// Description: Load media resources, and specify the progress, in milliseconds, at which playback begins.
/// Use case: Developers can load the resource ID of copyrighted music.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Caution: 1.When [startPosition] exceeds the total playing time, it will start playing from the beginning.
///  2.If the mediaplayer has already loaded resources or is in the process of playing, please first call the [stop] interface to halt the playback, and then proceed to call the interface to load the media resources; failure to do so will result in an unsuccessful load.
///
/// @param resource_id The resource ID obtained from the copyrighted music module.
/// @param start_position The progress at which the playback started.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_media_player_load_copyrighted_music_resource_with_position(
    const char *resource_id, long start_position,
    enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(
    EXP_CALL *pfnzego_express_media_player_load_copyrighted_music_resource_with_position)(
    const char *resource_id, long start_position,
    enum zego_media_player_instance_index instance_index);
#endif

/// Load local or network media resource with config.
///
/// Available: since 3.3.0
/// Description: Load media resources.
/// Use case: Developers can load the absolute path to the local resource or the URL of the network resource incoming.
/// When to call: Called after the engine [createEngine] has been initialized and the media player [createMediaPlayer] has been created.
/// Related APIs: Support for loading resources through the [loadResourceWithPosition] or [loadResourceFromMediaData] interface.
/// Caution: If the mediaplayer has already loaded resources or is in the process of playing, please first call the [stop] interface to halt the playback, and then proceed to call the interface to load the media resources; failure to do so will result in an unsuccessful load.
///
/// @param resource Multimedia resources that need to be loaded.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_load_resource_with_config(
    struct zego_media_player_resource *resource,
    enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_load_resource_with_config)(
    struct zego_media_player_resource *resource,
    enum zego_media_player_instance_index instance_index);
#endif

/// Start playing.
///
/// You need to load resources before playing
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_media_player_start(enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_start)(
    enum zego_media_player_instance_index instance_index);
#endif

/// Stop playing.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_media_player_stop(enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_stop)(
    enum zego_media_player_instance_index instance_index);
#endif

/// Pause playing.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_media_player_pause(enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_pause)(
    enum zego_media_player_instance_index instance_index);
#endif

/// Resume playing.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_media_player_resume(enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_resume)(
    enum zego_media_player_instance_index instance_index);
#endif

/// Set the specified playback progress.
///
/// Unit is millisecond
///
/// @param millisecond Point in time of specified playback progress
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_seek_to(
    unsigned long long millisecond, enum zego_media_player_instance_index instance_index,
    zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_seek_to)(
    unsigned long long millisecond, enum zego_media_player_instance_index instance_index,
    zego_seq *sequence);
#endif

/// Whether to repeat playback.
///
/// @param enable repeat playback flag. The default is false.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_enable_repeat(
    bool enable, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_enable_repeat)(
    bool enable, enum zego_media_player_instance_index instance_index);
#endif

/// Set the count of play loops.
///
/// Available: since 2.10.0
/// Description: Set the count of play loops.
/// Use cases: Users can call this function when they need to use the media player to loop playback resources.
/// When to call /Trigger: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Restrictions: None.
/// Caution: Please note that after using this interface, the [enableRepeat] interface will become invalid.
///
/// @param count the count of play loops.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_set_play_loop_count(
    int count, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_set_play_loop_count)(
    int count, enum zego_media_player_instance_index instance_index);
#endif

/// Set the speed of play.
///
/// Available since: 2.12.0
/// Description: Set the playback speed of the player.
/// When to call: You should load resource before invoking this function.
/// Restrictions: None.
/// Related APIs: Resources can be loaded through the [loadResource] function.
///
/// @param speed The speed of play. The default is 1.0.
///   Versions 2.12.0 to 3.15.1: The range is 0.5 ~ 4.0.
///   Versions 3.16.0 and above: The range is 0.3 ~ 4.0.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_set_play_speed(
    float speed, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_set_play_speed)(
    float speed, enum zego_media_player_instance_index instance_index);
#endif

/// Whether to mix the player's sound into the stream being published.
///
/// This interface will only mix the media player sound into the main channel
///
/// @param enable Aux audio flag. The default is false.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_enable_aux(
    bool enable, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_enable_aux)(
    bool enable, enum zego_media_player_instance_index instance_index);
#endif

/// Whether to play locally silently.
///
/// If [enableAux] switch is turned on, there is still sound in the publishing stream. The default is false.
///
/// @param mute Mute local audio flag, The default is false.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_mute_local_audio(
    bool mute, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_mute_local_audio)(
    bool mute, enum zego_media_player_instance_index instance_index);
#endif

/// Set the view of the player playing video.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param canvas Video rendered canvas object
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_set_player_canvas(
    struct zego_canvas *canvas, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_set_player_canvas)(
    struct zego_canvas *canvas, enum zego_media_player_instance_index instance_index);
#endif

/// Set mediaplayer volume. Both the local play volume and the publish volume are set.
///
/// @param volume The range is 0 ~ 200. The default is 60.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_set_volume(
    int volume, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_set_volume)(
    int volume, enum zego_media_player_instance_index instance_index);
#endif

/// Set mediaplayer local playback volume.
///
/// @param volume The range is 0 ~ 200. The default is 60.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_set_play_volume(
    int volume, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_set_play_volume)(
    int volume, enum zego_media_player_instance_index instance_index);
#endif

/// Set mediaplayer publish volume.
///
/// @param volume The range is 0 ~ 200. The default is 60.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_set_publish_volume(
    int volume, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_set_publish_volume)(
    int volume, enum zego_media_player_instance_index instance_index);
#endif

/// Set playback progress callback interval.
///
/// This function can control the callback frequency of [onMediaPlayerPlayingProgress]. When the callback interval is set to 0, the callback is stopped. The default callback interval is 1s
/// This callback are not returned exactly at the set callback interval, but rather at the frequency at which the audio or video frames are processed to determine whether the callback is needed to call
///
/// @param millisecond Interval of playback progress callback in milliseconds
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_set_progress_interval(
    unsigned long long millisecond, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_set_progress_interval)(
    unsigned long long millisecond, enum zego_media_player_instance_index instance_index);
#endif

/// Gets the current local playback volume of the mediaplayer, the range is 0 ~ 200, with the default value of 60.
///
/// @param volume [in/out] The current local playback volume.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_get_play_volume(
    enum zego_media_player_instance_index instance_index, int *volume);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_get_play_volume)(
    enum zego_media_player_instance_index instance_index, int *volume);
#endif

/// Gets the current publish volume of the mediaplayer, the range is 0 ~ 200, with the default value of 60.
///
/// @param volume [in/out] The current publish volume.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_get_publish_volume(
    enum zego_media_player_instance_index instance_index, int *volume);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_get_publish_volume)(
    enum zego_media_player_instance_index instance_index, int *volume);
#endif

/// Get the total progress of your media resources.
///
/// You should load resource before invoking this function, otherwise the return value is 0
///
/// @param duration [in/out] The total progress of media resources.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_get_total_duration(
    enum zego_media_player_instance_index instance_index, unsigned long long *duration);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_get_total_duration)(
    enum zego_media_player_instance_index instance_index, unsigned long long *duration);
#endif

/// Get current playing progress.
///
/// You should load resource before invoking this function, otherwise the return value is 0
///
/// @param progress [in/out] The current playing progress.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_get_current_progress(
    enum zego_media_player_instance_index instance_index, unsigned long long *progress);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_get_current_progress)(
    enum zego_media_player_instance_index instance_index, unsigned long long *progress);
#endif

/// Get current rendering progress.
///
/// You should load resource before invoking this function, otherwise the return value is 0
///
/// @param progress [in/out] The current rendering progress.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_get_current_rendering_progress(
    enum zego_media_player_instance_index instance_index, unsigned long long *progress);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_get_current_rendering_progress)(
    enum zego_media_player_instance_index instance_index, unsigned long long *progress);
#endif

/// Get the number of audio tracks of the playback file.
///
/// @param track_count [in/out] The number of audio tracks of the playback file.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_get_audio_track_count(
    enum zego_media_player_instance_index instance_index, unsigned int *track_count);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_get_audio_track_count)(
    enum zego_media_player_instance_index instance_index, unsigned int *track_count);
#endif

/// Set the audio track of the playback file.
///
/// @param index Audio track index, the number of audio tracks can be obtained through the [getAudioTrackCount] function.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_set_audio_track_index(
    unsigned int index, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_set_audio_track_index)(
    unsigned int index, enum zego_media_player_instance_index instance_index);
#endif

/// Set the audio track mode of the player.
///
/// Available since: 3.1.0
/// Description: Set the audio track mode of the player.
/// Use case: Under the real-time chorus (KTV), call the interface enable multi-track mode, call the interface [setAudioTrackIndex] to specify the original track to play, call interface [setAudioTrackPublishIndex] specified need publish of accompaniment tracks.
/// When to call: The call takes effect before [start] starts playing
/// Related APIs: Call [setAudioTrackIndex] to specified the play track of media file and call [setAudioTrackPublishIndex] to specified the publish track of media file.
/// Caution: When multi-track mode is enabled, the resource consumption of the hardware device is increased.
///
/// @param mode Audio track mode.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_set_audio_track_mode(
    enum zego_media_player_audio_track_mode mode,
    enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_set_audio_track_mode)(
    enum zego_media_player_audio_track_mode mode,
    enum zego_media_player_instance_index instance_index);
#endif

/// Set the audio track for the media file to be publish.
///
/// Available since: 3.1.0
/// Description: Set the audio track for the media file to be publish.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Related APIs: The number of audio tracks can be obtained through the [getAudioTrackCount] function.
/// Caution: This call takes effect only after multitrack mode is enabled by calling the interface [setAudioTrackMode].
///
/// @param index Audio track index, the number of audio tracks can be obtained through the [getAudioTrackCount] function.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_set_audio_track_publish_index(
    unsigned int index, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_set_audio_track_publish_index)(
    unsigned int index, enum zego_media_player_instance_index instance_index);
#endif

/// Enable voice changer, set up the specific voice changer parameters.
///
/// Available since: 3.15.0
/// Description: Enable voice changer, set up the specific voice changer parameters.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
///
/// @param audio_channel The audio channel to be voice changed
/// @param enable Whether enable voice changer or not. True - enabled, false - disabled, default value is false.
/// @param param Voice changer parameters
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_enable_voice_changer(
    enum zego_media_player_audio_channel audio_channel, bool enable, float param,
    enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_enable_voice_changer)(
    enum zego_media_player_audio_channel audio_channel, bool enable, float param,
    enum zego_media_player_instance_index instance_index);
#endif

/// Get the current playback status.
///
/// @param state [in/out] The current playback status.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_get_current_state(
    enum zego_media_player_instance_index instance_index, enum zego_media_player_state *state);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_get_current_state)(
    enum zego_media_player_instance_index instance_index, enum zego_media_player_state *state);
#endif

/// Whether to audio data playback.
///
/// @param enable Audio data playback flag. The default is false.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_enable_audio_data(
    bool enable, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_enable_audio_data)(
    bool enable, enum zego_media_player_instance_index instance_index);
#endif

/// Whether to video data playback.
///
/// Available since: 2.1.0
/// Description: Whether to throw out the video data of the media resource file played by the media player.
/// When to call: After the [ZegoMediaPlayer] instance created.
/// Restrictions: None.
/// Caution: When you no longer need to get the video frame data, please call this function again to clear the handler to stop the video frame data callback.
///
/// @param enable Video data playback flag. The default is false.
/// @param format Video frame format for video data
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_media_player_enable_video_data(bool enable, enum zego_video_frame_format format,
                                            enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_enable_video_data)(
    bool enable, enum zego_video_frame_format format,
    enum zego_media_player_instance_index instance_index);
#endif

/// Whether to throw block data of the media resource.
///
/// Available since: 3.4.0
/// Description: Whether to throw block data of the media resource.
/// When to call: After the [ZegoMediaPlayer] instance created, before playing media resources.
/// Restrictions: None.
/// Caution: When it is no longer necessary to listen to the callback for data decryption, please call this function again to clear the handler.
///
/// @param enable Throw out the media resource block data tag, which is false by default.
/// @param block_size The size of the encrypted data block. The bufferSize in the OnBlockData callback is an integer multiple of blockSize.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_enable_block_data(
    bool enable, unsigned int block_size, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_enable_block_data)(
    bool enable, unsigned int block_size, enum zego_media_player_instance_index instance_index);
#endif

/// Take a screenshot of the current playing screen of the media player.
///
/// Only in the case of calling [setPlayerCanvas] to set the display controls and the playback state, can the screenshot be taken normally
/// Note: This function is only available in ZegoExpressVideo SDK!
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_media_player_take_snapshot(enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_take_snapshot)(
    enum zego_media_player_instance_index instance_index);
#endif

/// Enable accurate seek and set relevant attributes.
///
/// Available since: 2.4.0
/// Description: The timestamp specified by normal seek may not an I frame, and then returns the I frame near the specified timestamp, which is not so accurate. But the accurate seek, when the specified timestamp is not an I frame, it will use the I frame near the specified timestamp to decode the frame of the specified timestamp.
/// Use cases: When user needs to seek to the specified timestamp accurately.
/// When to call: The setting must be called before [loadResource], and it will take effect during the entire life cycle of the media player.
///
/// @param enable Whether to enable accurate seek
/// @param config The property setting of accurate seek, only valid when enable is true.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_enable_accurate_seek(
    bool enable, struct zego_accurate_seek_config *config,
    enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_enable_accurate_seek)(
    bool enable, struct zego_accurate_seek_config *config,
    enum zego_media_player_instance_index instance_index);
#endif

/// Set the maximum cache duration and cache data size of web materials.
///
/// The setting must be called before loading the resource, and it will take effect during the entire life cycle of the media player.
/// Time and size are not allowed to be 0 at the same time. The SDK internal default time is 5000, and the size is 15*1024*1024 byte.When one of time and size reaches the set value first, the cache will stop.
///
/// @param time The maximum length of the cache time, in ms, the SDK internal default is 5000; the effective value is greater than or equal to 2000; if you fill in 0, it means no limit.
/// @param size The maximum size of the cache, the unit is byte, the internal default size of the SDK is 15*1024*1024 byte; the effective value is greater than or equal to 5000000, if you fill in 0, it means no limit.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_set_network_resource_max_cache(
    unsigned int time, unsigned int size, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_set_network_resource_max_cache)(
    unsigned int time, unsigned int size, enum zego_media_player_instance_index instance_index);
#endif

/// Get the playable duration and size of the cached data of the current network material cache queue
///
/// @param cache [in/out] The cached data.
/// @return Returns the current cached information, including the length of time the data can be played and the size of the cached data.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_get_network_resource_cache(
    struct zego_network_resource_cache *cache,
    enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_get_network_resource_cache)(
    struct zego_network_resource_cache *cache,
    enum zego_media_player_instance_index instance_index);
#endif

/// Use this interface to set the cache threshold that the media player needs to resume playback. The SDK default value is 5000ms，The valid value is greater than or equal to 1000ms
///
/// The setting must be called before loading the resource, and it will take effect during the entire life cycle of the media player.
/// When the network status is poor and the media player has finished playing the cached network resources, it will stop playing, and notify the user through the `ZegoMediaPlayerNetworkEvent.BUFFER_BEGIN` state of the callback interface `onMediaPlayerNetworkEvent` that the network resources are being recached.
/// Only when the cached network resources are greater than the set threshold, the media player will automatically resume playback at the original paused position, and notify the user through the `ZegoMediaPlayerNetworkEvent.BUFFER_ENDED` of the callback interface `onMediaPlayerNetworkEvent` that the user has cached the network resources The threshold was reached and playback resumed.
///
/// @param threshold Threshold that needs to be reached to resume playback, unit ms.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_set_network_buffer_threshold(
    unsigned int threshold, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_set_network_buffer_threshold)(
    unsigned int threshold, enum zego_media_player_instance_index instance_index);
#endif

/// Whether to enable sound level monitoring.
///
/// Available since: 2.15.0
/// Description: Whether to enable sound level monitoring.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Restrictions: None.
/// Related callbacks: After it is turned on, user can use the [onMediaPlayerSoundLevelUpdate] callback to monitor sound level updates.
///
/// @param enable Whether to enable monitoring, true is enabled, false is disabled.
/// @param millisecond Monitoring time period of the sound level, in milliseconds, has a value range of [100, 3000].
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_enable_sound_level_monitor(
    bool enable, unsigned int millisecond, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_enable_sound_level_monitor)(
    bool enable, unsigned int millisecond, enum zego_media_player_instance_index instance_index);
#endif

/// Whether to enable frequency spectrum monitoring.
///
/// Available since: 2.15.0
/// Description: Whether to enable frequency spectrum monitoring.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Restrictions: None.
/// Related APIs: After it is turned on, user can use the [onMediaPlayerFrequencySpectrumUpdate] callback to monitor frequency spectrum updates.
///
/// @param enable Whether to enable monitoring, true is enabled, false is disabled.
/// @param millisecond Monitoring time period of the frequency spectrum, in milliseconds, has a value range of [10, 3000]. Note that on v3.19.0 and older version, the value range is [100, 3000].
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_enable_frequency_spectrum_monitor(
    bool enable, unsigned int millisecond, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_enable_frequency_spectrum_monitor)(
    bool enable, unsigned int millisecond, enum zego_media_player_instance_index instance_index);
#endif

/// Set the playback channel.
///
/// Available since: 2.20.0
/// Description: Set the playback channel.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Restrictions: None.
///
/// @param audio_channel Playback channel, the default is ZegoMediaPlayerAudioChannelAll.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_set_active_audio_channel(
    enum zego_media_player_audio_channel audio_channel,
    enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_set_active_audio_channel)(
    enum zego_media_player_audio_channel audio_channel,
    enum zego_media_player_instance_index instance_index);
#endif

/// Clears the last frame of the playback control that remains on the control after playback ends.
///
/// Available since: 2.20.0
/// Description: Clears the last frame of the playback control that remains on the control after playback ends.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Restrictions: The interface call takes effect only when the media player ends playing.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_media_player_clear_view(enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_clear_view)(
    enum zego_media_player_instance_index instance_index);
#endif

/// Get meida information such as video resolution from media file.
///
/// Available since: 3.6.0
/// Description: Get meida information such as video resolution from media file.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Restrictions: None.
///
/// @param media_info [in/out] Media information such as video resolution.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_media_player_get_media_info(struct zego_media_player_media_info *media_info,
                                         enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_get_media_info)(
    struct zego_media_player_media_info *media_info,
    enum zego_media_player_instance_index instance_index);
#endif

/// Update the position of the media player (audio source).
///
/// Available since: 3.6.0
/// Description: Update the position of the media player (audio source).
/// Use cases: The media player also needs to have 3D spatial sound.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Restrictions: This interface needs to be used in conjunction with the RangeAudio/RangeScene module. This interface can only be called successfully after the RangeAudio/RangeScene module enables 3D sound effects.
///
/// @param position The unit vector of the front axis of its own coordinate system. The parameter is a float array with a length of 3.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_update_position(
    const float position[3], enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_update_position)(
    const float position[3], enum zego_media_player_instance_index instance_index);
#endif

/// Set http headers.
///
/// Available since: 3.8.0
/// Description: Call this function to set the http headers of the http network resource.
/// Use cases: When the network resource needs to set special header information.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Restrictions: Called before the corresponding network resource is loaded.
///
/// @param headers Headers info.
/// @param headers_size Headers size.
/// @param instance_index Media player instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_media_player_set_http_header(struct zego_key_value_pair *headers, int headers_size,
                                          enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_set_http_header)(
    struct zego_key_value_pair *headers, int headers_size,
    enum zego_media_player_instance_index instance_index);
#endif

/// Set play media stream type.
///
/// Available since: 3.10.0
/// Description: Configure the media stream type to be played. You can only play video streams or audio streams. This will take effect during the life cycle of the media player.
/// Use cases: When only the video stream or audio stream needs to be played.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Caution: Changing the media stream type during playing will take effect in the next playing.
///
/// @param stream_type Stream type.
/// @param instance_index Media player instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_set_play_media_stream_type(
    enum zego_media_stream_type stream_type, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_set_play_media_stream_type)(
    enum zego_media_stream_type stream_type, enum zego_media_player_instance_index instance_index);
#endif

/// Enable live audio effect.
///
/// Available since: 3.10.0
/// Description: When the live audio effect is turned on, the spatial sense is enhanced and the instrument sounds become more prominent, without any increase in delay.
/// Use cases: It is commonly used in voice chat rooms and karaoke scenarios to enhance the live audio effects of the accompaniment.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Caution: To enhance the live audio effect experience, it is recommended to configure dual-channel stereo encoding. Developers can achieve this configuration by using the [setAudioCaptureStereoMode] method. Failure to configure dual-channel stereo encoding may significantly diminish the effects of certain songs, as the left and right channel effects may cancel each other out when synthesizing mono audio, resulting in less noticeable effects.
///
/// @param enable Whether to enable live audio effect.
/// @param mode Live audio effect mode.
/// @param instance_index Media player instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_enable_live_audio_effect(
    bool enable, enum zego_live_audio_effect_mode mode,
    enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_enable_live_audio_effect)(
    bool enable, enum zego_live_audio_effect_mode mode,
    enum zego_media_player_instance_index instance_index);
#endif

/// Enable local caching of http/https network resources.
///
/// Available since: 3.12.0
/// Description: When playing http/https network resources, when the local cache is enabled, the network resources will be saved locally and the cache information will be called back through [onMediaPlayerLocalCache].
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer]. Called after [loadResource] or during playback, it will take effect next playback.
/// Caution: Only http/https single file type network resources are supported. If there is a [seek] operation during playback, the cache will fail.
///
/// @param enable Whether to enable local caching.
/// @param cache_dir Cache dir. If left blank, the directory specified internally by SDK will be used.
/// @param instance_index Media player instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_enable_local_cache(
    bool enable, const char *cache_dir, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_enable_local_cache)(
    bool enable, const char *cache_dir, enum zego_media_player_instance_index instance_index);
#endif

/// Enable the view mirror.
///
/// Available since: 3.14.0
/// Description: Enable view mirror.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
///
/// @param enable Whether to enable view mirror.
/// @param instance_index Media player instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_enable_view_mirror(
    bool enable, enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_enable_view_mirror)(
    bool enable, enum zego_media_player_instance_index instance_index);
#endif

/// Get playback statistics.
///
/// Available since: 3.12.0
/// Description: Get current playback statistics to monitor whether decoding and rendering anomalies occur in the player.
/// Use cases: Typically used in cloud-based media player scenarios.
/// When to call: Invoke after the [loadResource] callback succeeds.
///
/// @param stat [in/out] Playback statistics.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_get_playback_statistics(
    struct zego_media_player_statistics_info *stat,
    enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_get_playback_statistics)(
    struct zego_media_player_statistics_info *stat,
    enum zego_media_player_instance_index instance_index);
#endif

/// MediaPlayer playback status callback.
///
/// Available since: 1.3.4
/// Description: MediaPlayer playback status callback.
/// Trigger: The callback triggered when the state of the media player changes.
/// Restrictions: None.
///
/// @param state Media player status.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param instance_index Media player instance index.
/// @param user_context Context of user.
typedef void (*zego_on_media_player_state_update)(
    enum zego_media_player_state state, zego_error error_code,
    enum zego_media_player_instance_index instance_index, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_state_update_callback(
    zego_on_media_player_state_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_player_state_update_callback)(
    zego_on_media_player_state_update callback_func, void *user_context);
#endif

/// The callback triggered when the network status of the media player changes.
///
/// Available since: 1.3.4
/// Description: The callback triggered when the network status of the media player changes.
/// Trigger: When the media player is playing network resources, this callback will be triggered when the status change of the cached data.
/// Restrictions: The callback will only be triggered when the network resource is played.
/// Related APIs: [setNetWorkBufferThreshold].
///
/// @param network_event Network status event.
/// @param instance_index Media player instance index.
/// @param user_context Context of user.
typedef void (*zego_on_media_player_network_event)(
    enum zego_media_player_network_event network_event,
    enum zego_media_player_instance_index instance_index, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_network_event_callback(
    zego_on_media_player_network_event callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_player_network_event_callback)(
    zego_on_media_player_network_event callback_func, void *user_context);
#endif

/// The callback to report the current playback progress of the media player.
///
/// Available since: 1.3.4
/// Description: The callback triggered when the network status of the media player changes. Set the callback interval by calling [setProgressInterval]. When the callback interval is set to 0, the callback is stopped. The default callback interval is 1 second.
/// Trigger: When the media player is playing network resources, this callback will be triggered when the status change of the cached data.
/// Restrictions: None.
/// Related APIs: [setProgressInterval].
///
/// @param millisecond Progress in milliseconds.
/// @param instance_index Media player instance index.
/// @param user_context Context of user.
typedef void (*zego_on_media_player_playing_progress)(
    unsigned long long millisecond, enum zego_media_player_instance_index instance_index,
    void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_playing_progress_callback(
    zego_on_media_player_playing_progress callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_player_playing_progress_callback)(
    zego_on_media_player_playing_progress callback_func, void *user_context);
#endif

/// The callback to report the current rendering progress of the media player.
///
/// Available since: 3.8.0
/// Description: The callback to report the current rendering progress of the media player. Set the callback interval by calling [setProgressInterval]. When the callback interval is set to 0, the callback is stopped. The default callback interval is 1 second.
/// Trigger: This callback will be triggered when the media player starts playing resources.
/// Restrictions: None.
/// Related APIs: [setProgressInterval].
///
/// @param millisecond Progress in milliseconds.
/// @param instance_index Media player instance index.
/// @param user_context Context of user.
typedef void (*zego_on_media_player_rendering_progress)(
    unsigned long long millisecond, enum zego_media_player_instance_index instance_index,
    void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_rendering_progress_callback(
    zego_on_media_player_rendering_progress callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_player_rendering_progress_callback)(
    zego_on_media_player_rendering_progress callback_func, void *user_context);
#endif

/// The callback triggered when when the resolution of the playback video changes.
///
/// Available since: 3.11.0
/// Description: The callback triggered when when the resolution of the playback video changes.
/// Trigger: When the media player is playing a video resource, This callback will be triggered when playback starts and the resolution of the video changes.
///
/// @param width width.
/// @param height height.
/// @param instance_index Media player instance index.
/// @param user_context Context of user.
typedef void (*zego_on_media_player_video_size_changed)(
    int width, int height, enum zego_media_player_instance_index instance_index,
    void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_video_size_changed_callback(
    zego_on_media_player_video_size_changed callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_player_video_size_changed_callback)(
    zego_on_media_player_video_size_changed callback_func, void *user_context);
#endif

/// The callback triggered when the media player got media side info.
///
/// Available since: 2.2.0
/// Description: The callback triggered when the media player got media side info.
/// Trigger: When the media player starts playing media files, the callback is triggered if the SEI is resolved to the media file.
/// Caution: The callback does not actually take effect until call [setEventHandler] to set.
///
/// @param data SEI content.
/// @param data_length SEI content length.
/// @param instance_index Media player instance index.
/// @param user_context Context of user.
typedef void (*zego_on_media_player_recv_sei)(const unsigned char *data, unsigned int data_length,
                                              enum zego_media_player_instance_index instance_index,
                                              void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_recv_sei_callback(
    zego_on_media_player_recv_sei callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_player_recv_sei_callback)(
    zego_on_media_player_recv_sei callback_func, void *user_context);
#endif

/// The callback of sound level update.
///
/// Available since: 2.15.0
/// Description: The callback of sound level update.
/// Trigger: The callback frequency is specified by [EnableSoundLevelMonitor].
/// Caution: The callback does not actually take effect until call [setEventHandler] to set.
/// Related APIs: To monitor this callback, you need to enable it through [EnableSoundLevelMonitor].
///
/// @param sound_level Sound level value, value range: [0.0, 100.0] (This value only represents the range of the sound level value returned by the callback, not the precision.).
/// @param instance_index Media player instance index.
/// @param user_context Context of user.
typedef void (*zego_on_media_player_sound_level_update)(
    float sound_level, enum zego_media_player_instance_index instance_index, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_sound_level_update_callback(
    zego_on_media_player_sound_level_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_player_sound_level_update_callback)(
    zego_on_media_player_sound_level_update callback_func, void *user_context);
#endif

/// The callback of frequency spectrum update.
///
/// Available since: 2.15.0
/// Description: The callback of frequency spectrum update.
/// Trigger: The callback frequency is specified by [EnableFrequencySpectrumMonitor].
/// Caution: The callback does not actually take effect until call [setEventHandler] to set.
/// Related APIs: To monitor this callback, you need to enable it through [EnableFrequencySpectrumMonitor].
///
/// @param spectrum_list Spectrum values list.
/// @param spectrum_count Spectrum values list of count.
/// @param instance_index Media player instance index.
/// @param user_context Context of user.
typedef void (*zego_on_media_player_frequency_spectrum_update)(
    float *spectrum_list, unsigned int spectrum_count,
    enum zego_media_player_instance_index instance_index, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_frequency_spectrum_update_callback(
    zego_on_media_player_frequency_spectrum_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_player_frequency_spectrum_update_callback)(
    zego_on_media_player_frequency_spectrum_update callback_func, void *user_context);
#endif

/// The callback triggered when the media player plays the first frame.
///
/// Available since: 3.5.0
/// Description: The callback triggered when the media player plays the first frame.
/// Trigger: This callback is generated when the media player starts playing.
/// Caution: The callback does not actually take effect until call [setEventHandler] to set.
/// Related APIs: You need to call the [setPlayerCanvas] interface to set the view for the media player in order to receive the video first frame event callback.
///
/// @param event The first frame callback event type.
/// @param instance_index Media player instance index.
/// @param user_context Context of user.
typedef void (*zego_on_media_player_first_frame_event)(
    enum zego_media_player_first_frame_event event,
    enum zego_media_player_instance_index instance_index, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_first_frame_event_callback(
    zego_on_media_player_first_frame_event callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_player_first_frame_event_callback)(
    zego_on_media_player_first_frame_event callback_func, void *user_context);
#endif

/// The callback triggered when the media player caches http/https network resource locally.
///
/// Available since: 3.12.0
/// Description: The callback triggered when the media player caches http/https network resource locally.
/// Trigger: This callback occurs after the media player caches http/https network resources.
/// Caution: This callback will not take effect until the [enableLocalCache] API is called and the http/https network resources are played.
/// Related APIs: Need to call the [enableLocalCache] interface.
///
/// @param error_code Error code.
/// @param resource Played resource
/// @param cached_file Cached file
/// @param instance_index Media player instance index.
/// @param user_context Context of user.
typedef void (*zego_on_media_player_local_cache)(
    int error_code, const char *resource, const char *cached_file,
    enum zego_media_player_instance_index instance_index, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_local_cache_callback(
    zego_on_media_player_local_cache callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_player_local_cache_callback)(
    zego_on_media_player_local_cache callback_func, void *user_context);
#endif

/// The callback triggered when the media player throws out video frame data, with additional information for the video frame.
///
/// Available since: 2.16.0
/// Description: The callback triggered when the media player throws out video frame data.
/// Trigger: The callback is generated when the media player starts playing.
/// Caution: The callback does not actually take effect until call [setVideoHandler] to set.
/// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
///
/// @param data Raw data of video frames (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2]).
/// @param data_length Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2]).
/// @param param Video frame flip mode.
/// @param extra_info Video frame extra info. json format data.
/// @param instance_index Media player instance index.
/// @param user_context Context of user.
typedef void (*zego_on_media_player_video_frame)(
    const unsigned char **data, unsigned int *data_length,
    const struct zego_video_frame_param param, const char *extra_info,
    enum zego_media_player_instance_index instance_index, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_video_frame_callback(
    zego_on_media_player_video_frame callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_player_video_frame_callback)(
    zego_on_media_player_video_frame callback_func, void *user_context);
#endif

/// The callback triggered when the media player throws out audio frame data.
///
/// Available since: 1.3.4
/// Description: The callback triggered when the media player throws out audio frame data.
/// Trigger: The callback is generated when the media player starts playing.
/// Caution: The callback does not actually take effect until call [setAudioHandler] to set.
/// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
///
/// @param data Raw data of audio frames.
/// @param data_length Data length.
/// @param param audio frame flip mode.
/// @param instance_index Media player instance index.
/// @param user_context Context of user.
typedef void (*zego_on_media_player_audio_frame)(
    unsigned char *data, unsigned int data_length, const struct zego_audio_frame_param param,
    enum zego_media_player_instance_index instance_index, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_audio_frame_callback(
    zego_on_media_player_audio_frame callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_player_audio_frame_callback)(
    zego_on_media_player_audio_frame callback_func, void *user_context);
#endif

/// The callback triggered when the media player is about to throw the block data of the media resource.
///
/// Available since: 3.4.0
/// Description: The callback triggered when the media player is about to throw the block data of the media resource.
/// Trigger: The callback is generated when the media player starts playing.
/// Caution: The callback does not actually take effect until call [setBlockDataHandler] to set.
/// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
///
/// @param path The path of the media resource.
/// @param instance_index Media player instance index.
/// @param user_context Context of user.
typedef void (*zego_on_media_player_block_begin)(
    const char *path, enum zego_media_player_instance_index instance_index, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_block_begin_callback(
    zego_on_media_player_block_begin callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_player_block_begin_callback)(
    zego_on_media_player_block_begin callback_func, void *user_context);
#endif

/// The callback triggered when the media player throws the block data of the media resource.
///
/// Available since: 3.4.0
/// Description: The callback triggered when the media player throws the block data of the media resource.
/// Trigger: This callback will be generated after receiving the [onBlockBegin] callback.
/// Caution: The callback does not actually take effect until call [setBlockDataHandler] to set. The buffer size before and after decryption should be consistent.
/// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
///
/// @param buffer The block data of the media resource.
/// @param buffer_size Length of media resource block data.
/// @param instance_index Media player instance index.
/// @param user_context Context of user.
/// @return The size of the buffer, -1 is returned for failure.
typedef unsigned int (*zego_on_media_player_block_data)(
    unsigned char *const buffer, unsigned int buffer_size,
    enum zego_media_player_instance_index instance_index, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_block_data_callback(
    zego_on_media_player_block_data callback_func, void *user_context);
#else
typedef unsigned int(EXP_CALL *pfnzego_register_media_player_block_data_callback)(
    zego_on_media_player_block_data callback_func, void *user_context);
#endif

/// [Deprecated] Setting up the specific voice changer parameters.
///
/// @deprecated Deprecated since 3.15.0, please use [enableVoiceChanger] instead.
/// @param audio_channel The audio channel to be voice changed
/// @param param Voice changer parameters
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_player_set_voice_changer_param(
    enum zego_media_player_audio_channel audio_channel, float param,
    enum zego_media_player_instance_index instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_player_set_voice_changer_param)(
    enum zego_media_player_audio_channel audio_channel, float param,
    enum zego_media_player_instance_index instance_index);
#endif

/// Callback for media player loads resources.
///
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param instance_index Media player instance index
/// @param user_context Context of user.
typedef void (*zego_on_media_player_load_resource)(
    zego_error error_code, enum zego_media_player_instance_index instance_index,
    void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_load_resource_callback(
    zego_on_media_player_load_resource callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_player_load_resource_callback)(
    zego_on_media_player_load_resource callback_func, void *user_context);
#endif

/// Callback for media player seek to playback progress.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param instance_index Media player instance index
/// @param user_context Context of user.
typedef void (*zego_on_media_player_seek_to)(zego_seq seq, zego_error error_code,
                                             enum zego_media_player_instance_index instance_index,
                                             void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_seek_to_callback(
    zego_on_media_player_seek_to callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_player_seek_to_callback)(
    zego_on_media_player_seek_to callback_func, void *user_context);
#endif

/// The callback of the screenshot of the media player playing screen
///
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param image Snapshot image (Windows: HBITMAP; macOS/iOS: CGImageRef; Linux: QImage; Android: Bitmap)
/// @param instance_index Media player instance index
/// @param user_context Context of user.
typedef void (*zego_on_media_player_take_snapshot_result)(
    zego_error error_code, void *image, enum zego_media_player_instance_index instance_index,
    void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_player_take_snapshot_result_callback(
    zego_on_media_player_take_snapshot_result callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_player_take_snapshot_result_callback)(
    zego_on_media_player_take_snapshot_result callback_func, void *user_context);
#endif

ZEGO_END_DECLS

#endif
