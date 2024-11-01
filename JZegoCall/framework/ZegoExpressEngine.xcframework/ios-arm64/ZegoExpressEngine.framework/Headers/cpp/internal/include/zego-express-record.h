
#ifndef __ZEGO_EXPRESS_RECORD_H__
#define __ZEGO_EXPRESS_RECORD_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Starts to record and directly save the data to a file.
///
/// Available since: 1.10.0
/// Description: Starts to record locally captured audio or video and directly save the data to a file, The recorded data will be the same as the data publishing through the specified channel.
/// Restrictions: None.
/// Caution: Developers should not [stopPreview] or [stopPublishingStream] during recording, otherwise the SDK will end the current recording task. The data of the media player needs to be mixed into the publishing stream to be recorded.
/// Related callbacks: Developers will receive the [onCapturedDataRecordStateUpdate] and the [onCapturedDataRecordProgressUpdate] callback after start recording.
///
/// @param config Record config.
/// @param channel Publishing stream channel.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_start_recording_captured_data(
    struct zego_data_record_config config, enum zego_publish_channel channel);
#else
typedef zego_error(EXP_CALL *pfnzego_express_start_recording_captured_data)(
    struct zego_data_record_config config, enum zego_publish_channel channel);
#endif

/// Stops recording locally captured audio or video.
///
/// Available since: 1.10.0
/// Description: Stops recording locally captured audio or video.
/// When to call: After [startRecordingCapturedData].
/// Restrictions: None.
///
/// @param channel Publishing stream channel.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_stop_recording_captured_data(enum zego_publish_channel channel);
#else
typedef zego_error(EXP_CALL *pfnzego_express_stop_recording_captured_data)(
    enum zego_publish_channel channel);
#endif

/// The callback triggered when the state of data recording (to a file) changes.
///
/// Available since: 1.10.0
/// Description: The callback triggered when the state of data recording (to a file) changes.
/// Use cases: The developer should use this callback to determine the status of the file recording or for UI prompting.
/// When to trigger: After [startRecordingCapturedData] is called, if the state of the recording process changes, this callback will be triggered.
/// Restrictions: None.
///
/// @param state File recording status.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param config Record config.
/// @param channel Publishing stream channel.
/// @param user_context Context of user.
typedef void (*zego_on_captured_data_record_state_update)(enum zego_data_record_state state,
                                                          zego_error error_code,
                                                          struct zego_data_record_config config,
                                                          enum zego_publish_channel channel,
                                                          void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_captured_data_record_state_update_callback(
    zego_on_captured_data_record_state_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_captured_data_record_state_update_callback)(
    zego_on_captured_data_record_state_update callback_func, void *user_context);
#endif

/// The callback to report the current recording progress.
///
/// Available since: 1.10.0
/// Description: Recording progress update callback, triggered at regular intervals during recording.
/// Use cases: Developers can do UI hints for the user interface.
/// When to trigger: After [startRecordingCapturedData] is called, If configured to require a callback, timed trigger during recording.
/// Restrictions: None.
///
/// @param progress File recording progress, which allows developers to hint at the UI, etc.
/// @param config Record config.
/// @param channel Publishing stream channel.
/// @param user_context Context of user.
typedef void (*zego_on_captured_data_record_progress_update)(
    struct zego_data_record_progress progress, struct zego_data_record_config config,
    enum zego_publish_channel channel, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_captured_data_record_progress_update_callback(
    zego_on_captured_data_record_progress_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_captured_data_record_progress_update_callback)(
    zego_on_captured_data_record_progress_update callback_func, void *user_context);
#endif

ZEGO_END_DECLS

#endif
