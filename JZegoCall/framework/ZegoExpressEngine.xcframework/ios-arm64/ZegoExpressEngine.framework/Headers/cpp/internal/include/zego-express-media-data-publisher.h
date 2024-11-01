
#ifndef __ZEGO_EXPRESS_MEDIA_DATA_PUBLISHER_H__
#define __ZEGO_EXPRESS_MEDIA_DATA_PUBLISHER_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Creates a media data publisher instance.
///
/// Available since: 3.4.0
/// Description: Developers can use this interface to create ZegoMediaDataPublisher objects to push local media resource files to implement server-side streaming scenarios, such as AI classrooms.
/// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
/// When to call: After calling the [createEngine] function to create an engine.
/// Caution: It returns the instance object of the specified publish channel. It will return the instance object if it already exists.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param config Config the media data publisher.
/// @param instance_index [in/out] return instance index, -1 will be returned when the maximum number is exceeded.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_create_media_data_publisher(
    struct zego_media_data_publisher_config config, zego_instance *instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_create_media_data_publisher)(
    struct zego_media_data_publisher_config config, zego_instance *instance_index);
#endif

/// Destroys a media data publisher instance.
///
/// Available since: 2.17.0
/// Description: Destroys a media data publisher instance.
/// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
/// When to call: After calling the [createEngine] function to create an engine.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param instance_index The media data publisher instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_destroy_media_data_publisher(zego_instance instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_destroy_media_data_publisher)(
    zego_instance instance_index);
#endif

/// Add media file to the publish queue. Currently, only mp4 / m4a / aac files are supported, and special conversion is required.
///
/// Available since: 2.17.0
/// Description: Add media file to the publish queue. Currently, only mp4 / m4a / aac file are supported, and special conversion is required.
/// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
/// When to call: After calling the [createMediaDataPublisher] function to create a media data publisher.
/// Caution: The mp4 file format must meet the following points：The video must be encoded as H.264 and cannot contain B frames, only I and P frames. The I frame interval is 2s, that is, a single GOP value is 2s; The frame rate, bit rate, and resolution of the video are consistent with the frame rate, bit rate, and resolution set by [setVideoConfig] before publishing stream; Audio encoding must be MPEG-4 AAC.
///
/// @param path Local absolute path to the media file.
/// @param is_clear Whether to clear the publish queue.
/// @param instance_index The media data publisher instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_data_publisher_add_media_file_path(
    const char *path, bool is_clear, zego_instance instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_data_publisher_add_media_file_path)(
    const char *path, bool is_clear, zego_instance instance_index);
#endif

/// Clear all the status in this media data publisher, so that it can be restarted next time.
///
/// Available since: 2.17.0
/// Description: When you need to re-publish stream and do not need to continue publishing from the previous publish queue, you can call this function to reset this media data publisher's state.
/// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
/// Caution: When the developer calls [logoutRoom], the state is automatically reset.
///
/// @param instance_index The media data publisher instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_media_data_publisher_reset(zego_instance instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_data_publisher_reset)(
    zego_instance instance_index);
#endif

/// Set the delay time of video playback.
///
/// Available since: 2.17.0
/// Description: When this value is set, when publishing video file stream, the SDK will permanently delay the video to the set time value before sending.
/// Use cases: Mainly used to correct the fixed audio and picture asynchronous phenomenon that occurs during streaming.
/// When to call: After calling the [createMediaDataPublisher] function to create a media data publisher.
///
/// @param delay_time Video playback time.Required: Yes.Value range: [0, 100] ms.
/// @param instance_index The media data publisher instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_data_publisher_set_video_send_delay_time(
    int delay_time, zego_instance instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_data_publisher_set_video_send_delay_time)(
    int delay_time, zego_instance instance_index);
#endif

/// Specify the starting point of the current video file publishing.
///
/// Available since: 2.17.0
/// Description: Specify the starting point of the current video file publishing.
/// When to call: Called at any point between [OnMediaDataPublisherFileOpen] and [OnMediaDataPublisherFileClose]. For example: this function can be called directly in the [OnMediaDataPublisherFileOpen] callback.
///
/// @param millisecond The timestamp of the start of streaming (relative to the timestamp of the file currently being published, the starting value is 0). the unit is milliseconds
/// @param instance_index The media data publisher instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_data_publisher_seek_to(
    unsigned long long millisecond, zego_instance instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_data_publisher_seek_to)(
    unsigned long long millisecond, zego_instance instance_index);
#endif

/// Get the total duration of the current file.
///
/// Available since: 2.17.0
/// Description: Get the total duration of the current file, in milliseconds.
/// When to call: After [onMediaDataPublisherFileDataBegin] callback.
///
/// @param instance_index The media data publisher instance index.
/// @param total_duration [in/out] The total duration of the current file.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_data_publisher_get_total_duration(
    zego_instance instance_index, unsigned long long *total_duration);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_data_publisher_get_total_duration)(
    zego_instance instance_index, unsigned long long *total_duration);
#endif

/// Get the playing progress of the current file.
///
/// Available since: 2.17.0
/// Description: Get the playing progress of the current file, in milliseconds.
/// When to call: After received the [onMediaDataPublisherFileDataBegin] callback.
///
/// @param instance_index The media data publisher instance index.
/// @param current_duration [in/out] Get the progress of the current file.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_media_data_publisher_get_current_duration(
    zego_instance instance_index, unsigned long long *current_duration);
#else
typedef zego_error(EXP_CALL *pfnzego_express_media_data_publisher_get_current_duration)(
    zego_instance instance_index, unsigned long long *current_duration);
#endif

/// The event callback of the media data publisher opening a media file.
///
/// Available since: 2.17.0
/// Description: The event callback of the media data publisher opening a media file.
/// Trigger: The callback triggered when the media data publisher start loading a media file.
/// Restrictions: None.
///
/// @param path Path of currently open file
/// @param instance_index Media data publisher instance index
/// @param user_context Context of user.
typedef void (*zego_on_media_data_publisher_file_open)(const char *path,
                                                       zego_instance instance_index,
                                                       void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_data_publisher_file_open_callback(
    zego_on_media_data_publisher_file_open callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_data_publisher_file_open_callback)(
    zego_on_media_data_publisher_file_open callback_func, void *user_context);
#endif

/// The event callback of the media data publisher closing a media file.
///
/// Available since: 2.17.0
/// Description: The event callback of the media data publisher closing a media file.
/// Trigger: The callback triggered when the media data publisher start unloading a media file.
/// Restrictions: None.
///
/// @param error_code error code. 0 means closing the file normally. -1 is a file error. -2 is a path exception. -3 is a decoding exception. -4 is an incorrect timestamp. -5 Unable to fix audio/video synchronization. -6 Unsupported audio sample rate. Contact technical support for details.
/// @param path File path currently closed
/// @param instance_index Media data publisher instance index
/// @param user_context Context of user.
typedef void (*zego_on_media_data_publisher_file_close)(zego_error error_code, const char *path,
                                                        zego_instance instance_index,
                                                        void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_data_publisher_file_close_callback(
    zego_on_media_data_publisher_file_close callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_data_publisher_file_close_callback)(
    zego_on_media_data_publisher_file_close callback_func, void *user_context);
#endif

/// The event callback that the media data publisher has read data from the media file.
///
/// Available since: 2.17.0
/// Description: The event callback that the media data publisher has read data from the media file.
/// Trigger: The callback triggered when the media data publisher begin to read media data from a media file.
/// Restrictions: None.
///
/// @param path The file path where the streaming is currently started
/// @param instance_index Media data publisher instance index
/// @param user_context Context of user.
typedef void (*zego_on_media_data_publisher_file_data_begin)(const char *path,
                                                             zego_instance instance_index,
                                                             void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_data_publisher_file_data_begin_callback(
    zego_on_media_data_publisher_file_data_begin callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_data_publisher_file_data_begin_callback)(
    zego_on_media_data_publisher_file_data_begin callback_func, void *user_context);
#endif

/// The event callback that the media data publisher has completed a file stream publishing.
///
/// Available since: 3.14.0
/// Description: The media data publisher has finished a file stream publishing.
/// Trigger: The callback triggered when the media data publisher has completed a file stream publishing.
/// Restrictions: None.
///
/// @param path The path of the file that has completed stream publishing.
/// @param instance_index Media data publisher instance index
/// @param user_context Context of user.
typedef void (*zego_on_media_data_publisher_file_data_end)(const char *path,
                                                           zego_instance instance_index,
                                                           void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_media_data_publisher_file_data_end_callback(
    zego_on_media_data_publisher_file_data_end callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_media_data_publisher_file_data_end_callback)(
    zego_on_media_data_publisher_file_data_end callback_func, void *user_context);
#endif

ZEGO_END_DECLS

#endif
