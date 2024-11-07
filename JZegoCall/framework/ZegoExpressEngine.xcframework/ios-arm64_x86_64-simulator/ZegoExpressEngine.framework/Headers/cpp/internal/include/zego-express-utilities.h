
#ifndef __ZEGO_EXPRESS_UTILITIES_H__
#define __ZEGO_EXPRESS_UTILITIES_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Start system performance monitoring.
///
/// Available since: 1.19.0
/// Description: Start system performance monitoring, monitor system or App's CPU usage and memory usage. Support set the monitoring interval.
/// Use cases: Monitor system performance can help user quickly locate and solve performance problems and improve user experience.
/// When to call: It needs to be called after [createEngine].
/// Restrictions: None.
/// Related callbacks: After starting monitoring, you can receive system performance status via [onPerformanceStatusUpdate] callback. [onPerformanceStatusUpdate] callback notification period is the value set by millisecond parameter.
/// Related APIs: Call [stopPerformanceMonitor] to stop system performance monitoring.
///
/// @param millisecond Monitoring time period(in milliseconds), the value range is [1000, 10000]. Default value is 2000 ms.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_start_performance_monitor(unsigned int millisecond);
#else
typedef zego_error(EXP_CALL *pfnzego_express_start_performance_monitor)(unsigned int millisecond);
#endif

/// Stop system performance monitoring.
///
/// Available since: 1.19.0
/// Description: Stop system performance monitoring. After the monitoring is stopped, the [onPerformanceStatusUpdate] callback will not triggered.
/// Use cases: Monitor system performance can help user quickly locate and solve performance problems and improve user experience.
/// When to call: It needs to be called after [createEngine].
/// Restrictions: None.
/// Related APIs: Call [startPerformanceMonitor] to start system performance monitoring.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_performance_monitor();
#else
typedef zego_error(EXP_CALL *pfnzego_express_stop_performance_monitor)();
#endif

/// Start network probe.
///
/// Available since: 2.3.0
/// Description: Some local network problems may cause audio and video calls to fail. Using this function to probe the network protocols, assist in locating and solving related network problems.
/// Use cases: Before pushing and pulling the stream, detect and locate some possible network problems.
/// When to call: It needs to be called after [createEngine].
/// Restrictions: The SDK will not perform multiple network probe at the same time, that is, if the network probe is in progress, the SDK will ignore subsequent calls of this function.
/// Caution: The SDK detects http, tcp, and udp in sequence internally. If the probe fails in the middle, the subsequent detection will not continue. Therefore, when reading the values ​​in the probe result, please pay attention to check if the value is NULL. Network probe may take a long time. Developers can call [stopNetworkProbe] to stop network probe if needed. It is not recommended to start network probe during publishing/playing stream.
/// Related APIs: Call [stopNetworkProbe] to stop network probe.
///
/// @param config network probe config.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_start_network_probe(struct zego_network_probe_config config, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_start_network_probe)(
    struct zego_network_probe_config config, zego_seq *sequence);
#endif

/// Stop network probe.
///
/// Available since: 2.3.0
/// Description: Stop network probe.
/// Use cases: Before pushing and pulling the stream, detect and locate some possible network problems.
/// When to call: It needs to be called after [createEngine].
/// Restrictions: None.
/// Related APIs: Call [startNetworkProbe] to start network probe.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_network_probe();
#else
typedef zego_error(EXP_CALL *pfnzego_express_stop_network_probe)();
#endif

/// Test network connectivity.
///
/// Test network connectivity.
///
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_test_network_connectivity(zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_test_network_connectivity)(zego_seq *sequence);
#endif

/// Start network speed test. Support set speed test interval。
///
/// Available since: 1.20.0
/// Description: This function supports uplink/downlink network speed test when the network can be connected.
/// Use cases: This function can be used to detect whether the network environment is suitable for pushing/pulling streams with specified bitrates.
/// When to call: It needs to be called after [createEngine], and before [startPublishingStream]. If you call [startPublishingStream] while speed testing, the speed test will automatically stop.
/// Restrictions: The default maximum allowable test time for a single network speed test is 30 seconds.
/// Caution: Developers can register [onNetworkSpeedTestQualityUpdate] callback to get the speed test result, which will be triggered every 3 seconds. If an error occurs during the speed test process, [onNetworkSpeedTestError] callback will be triggered. If this function is repeatedly called multiple times, the last functioh call's configuration will be used.
/// Related APIs: Call [stopNetworkSpeedTest] to stop network speed test.
///
/// @param config Network speed test configuration.
/// @param interval Interval of network speed test. In milliseconds, default is 3000 ms.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_start_network_speed_test(
    struct zego_network_speed_test_config config, unsigned int interval);
#else
typedef zego_error(EXP_CALL *pfnzego_express_start_network_speed_test)(
    struct zego_network_speed_test_config config, unsigned int interval);
#endif

/// Stop network speed test.
///
/// Available since: 1.20.0
/// Description: Stop network speed test.
/// Use cases: This function can be used to detect whether the network environment is suitable for pushing/pulling streams with specified bitrates.
/// When to call: It needs to be called after [createEngine].
/// Restrictions: None.
/// Caution: After the network speed test stopped, [onNetworkSpeedTestQualityUpdate] callback will not be triggered.
/// Related APIs: Call [startNetworkSpeedTest] to start network speed test.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_network_speed_test();
#else
typedef zego_error(EXP_CALL *pfnzego_express_stop_network_speed_test)();
#endif

/// Obtain synchronization network time information.
///
/// Available since: 2.9.0
/// Description: Obtain synchronization network time(NTP), including timestamp and maximum deviation.
/// Use cases: When performing multi-terminal synchronization behaviors, network time synchronization is required.
/// When to call: It needs to be called after [createEngine].
/// Restrictions: None.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_get_network_time_info(zego_network_time_info *time_info);
#else
typedef zego_error(EXP_CALL *pfnzego_express_get_network_time_info)(
    zego_network_time_info *time_info);
#endif

/// Dump audio and video data.
///
/// Available since: 3.10.0
/// Description: Dump audio and video data. Currently, only audio data is supported.
/// Use cases: This is a debugging tool. When there is a problem with audio capturing, 3A processing, or other environment processing during publish, you can dump the audio data and upload it to the ZEGO server for further analysis.
/// When to call: It needs to be called after [createEngine].
/// Restrictions: None.
/// Caution: It will trigger the [onStartDumpData] callback when data dumping starts.
/// Related APIs: Call [stopDumpData] to stop dumping data.
///
/// @param config Dump data config.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_start_dump_data(struct zego_dump_data_config config);
#else
typedef zego_error(EXP_CALL *pfnzego_express_start_dump_data)(struct zego_dump_data_config config);
#endif

/// Stop dumping data.
///
/// Available since: 3.10.0
/// Description: Stop dumping data.
/// Use cases: This is a debugging tool. When there is a problem with audio capturing, 3A processing, or other environment processing during publish, you can dump the audio data and upload it to the ZEGO server for further analysis.
/// When to call: It needs to be called after [startDumpData].
/// Restrictions: None.
/// Caution: It will trigger the [onUploadDumpData] callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_dump_data();
#else
typedef zego_error(EXP_CALL *pfnzego_express_stop_dump_data)();
#endif

/// Upload dumped data to the ZEGO server.
///
/// Available since: 3.10.0
/// Description: Upload dumped data to the ZEGO server.
/// Use cases: This is a debugging tool. When there is a problem with audio capturing, 3A processing, or other environment processing during publish, you can dump the audio data and upload it to the ZEGO server for further analysis.
/// When to call: It needs to be called after [stopDumpData].
/// Restrictions: None.
/// Caution: It will trigger the [onUploadDumpData] callback when dump data uploaded.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_upload_dump_data();
#else
typedef zego_error(EXP_CALL *pfnzego_express_upload_dump_data)();
#endif

/// Remove dumped data.
///
/// Available since: 3.10.0
/// Description: Remove dumped data.
/// Use cases: This is a debugging tool. When there is a problem with audio capturing, 3A processing, or other environment processing during publish, you can dump the audio data and upload it to the ZEGO server for further analysis.
/// When to call: It needs to be called after [stopDumpData]. If the dump data is to be uploaded to the ZEGO server, it should be deleted after the upload is successful.
/// Restrictions: None.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_remove_dump_data();
#else
typedef zego_error(EXP_CALL *pfnzego_express_remove_dump_data)();
#endif

/// System performance monitoring callback.
///
/// Available since: 1.19.0
/// Description: System performance monitoring callback. The callback notification period is the value of millisecond parameter set by call [startPerformanceMonitor].
/// Use cases: Monitor system performance can help user quickly locate and solve performance problems and improve user experience.
/// When to trigger: It will triggered after [createEngine], and call [startPerformanceMonitor] to start system performance monitoring.
/// Restrictions: None.
///
/// @param status System performance monitoring status.
/// @param user_context Context of user.
typedef void (*zego_on_performance_status_update)(struct zego_performance_status status,
                                                  void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_performance_status_update_callback(
    zego_on_performance_status_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_performance_status_update_callback)(
    zego_on_performance_status_update callback_func, void *user_context);
#endif

/// Network mode changed callback.
///
/// Available since: 1.20.0
/// Description: Network mode changed callback.
/// When to trigger: This callback will be triggered when the device's network mode changed, such as switched from WiFi to 5G, or when network is disconnected.
/// Restrictions: None.
///
/// @param mode Current network mode.
/// @param user_context Context of user.
typedef void (*zego_on_network_mode_changed)(enum zego_network_mode mode, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_network_mode_changed_callback(
    zego_on_network_mode_changed callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_network_mode_changed_callback)(
    zego_on_network_mode_changed callback_func, void *user_context);
#endif

/// Network speed test error callback.
///
/// Available since: 1.20.0
/// Description: Network speed test error callback.
/// Use cases: This function can be used to detect whether the network environment is suitable for pushing/pulling streams with specified bitrates.
/// When to Trigger: If an error occurs during the speed test, such as: can not connect to speed test server, this callback will be triggered.
/// Restrictions: None.
///
/// @param error_code Network speed test error code. Please refer to error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param type Uplink or downlink.
/// @param user_context Context of user.
typedef void (*zego_on_network_speed_test_error)(int error_code,
                                                 enum zego_network_speed_test_type type,
                                                 void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_network_speed_test_error_callback(
    zego_on_network_speed_test_error callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_network_speed_test_error_callback)(
    zego_on_network_speed_test_error callback_func, void *user_context);
#endif

/// Network speed test quality callback.
///
/// Available since: 1.20.0
/// Description: Network speed test quality callback when the network can be connected.
/// Use cases: This function can be used to detect whether the network environment is suitable for pushing/pulling streams with specified bitrates.
/// When to Trigger: After call [startNetworkSpeedTest] start network speed test, this callback will be triggered. The trigger period is determined by the parameter value specified by call [startNetworkSpeedTest], default value is 3 seconds
/// Restrictions: None.
/// Caution: When error occurred during network speed test or [stopNetworkSpeedTest] called, this callback will not be triggered.
///
/// @param quality Network speed test quality.
/// @param type Uplink or downlink.
/// @param user_context Context of user.
typedef void (*zego_on_network_speed_test_quality_update)(
    const struct zego_network_speed_test_quality quality, enum zego_network_speed_test_type type,
    void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_network_speed_test_quality_update_callback(
    zego_on_network_speed_test_quality_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_network_speed_test_quality_update_callback)(
    zego_on_network_speed_test_quality_update callback_func, void *user_context);
#endif

/// The network quality callback of users who are publishing in the room.
///
/// Available since: 2.10.0
/// Description: The uplink and downlink network callbacks of the local and remote users, that would be called by default every two seconds for the local and each playing remote user's network quality.
///   Versions 2.10.0 to 2.13.1:
///   1. Developer must both publish and play streams before you receive your own network quality callback.
///   2. When playing a stream, the publish end has a play stream and the publish end is in the room where it is located, then the user's network quality will be received.
///   Versions 2.14.0 to 2.21.1:
///   1. As long as you publish or play a stream, you will receive your own network quality callback.
///   2. When you play a stream, the publish end is in the room where you are, and you will receive the user's network quality.
///   Version 2.22.0 and above:
///   1. Estimate the network conditions of the remote stream publishing user. If the remote stream publishing user loses one heartbeat, the network quality will be called back as unknown; if the remote stream publishing user's heartbeat loss reaches 3 Second, call back its network quality to die.
/// Use case: When the developer wants to analyze the network condition on the link, or wants to know the network condition of local and remote users.
/// When to Trigger: After publishing a stream by called [startPublishingStream] or playing a stream by called [startPlayingStream].
///
/// @param userid User ID, empty means local user
/// @param upstream_quality Upstream network quality
/// @param downstream_quality Downstream network quality
/// @param user_context Context of user.
typedef void (*zego_on_network_quality)(const char *userid,
                                        enum zego_stream_quality_level upstream_quality,
                                        enum zego_stream_quality_level downstream_quality,
                                        void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL
zego_register_network_quality_callback(zego_on_network_quality callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_network_quality_callback)(
    zego_on_network_quality callback_func, void *user_context);
#endif

/// Successful callback of network time synchronization.
///
/// Available since: 2.12.0
/// This callback is triggered when internal network time synchronization completes after a developer calls [createEngine].
///
/// @param user_context context of user
typedef void (*zego_on_network_time_synchronized)(void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_network_time_synchronized_callback(
    zego_on_network_time_synchronized callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_network_time_synchronized_callback)(
    zego_on_network_time_synchronized callback_func, void *user_context);
#endif

/// Request to dump data.
///
/// Available since: 3.10.0
/// When to Trigger: When the customer reports back the problem, ZEGO expects the user to dump the data to analyze the audio / video processing problem, which will trigger this callback.
///
/// @param user_context context of user
typedef void (*zego_on_request_dump_data)(void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_request_dump_data_callback(
    zego_on_request_dump_data callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_request_dump_data_callback)(
    zego_on_request_dump_data callback_func, void *user_context);
#endif

/// Request to dump data.
///
/// Available since: 3.11.0
/// When to Trigger: When the customer reports back the problem, ZEGO expects the user to dump the data to analyze the audio / video processing problem, which will trigger this callback.
///
/// @param dump_dir Dump data dir.
/// @param take_photo Need to take photo when uploading dump data
/// @param user_context context of user
typedef void (*zego_on_request_upload_dump_data)(const char *dump_dir, bool take_photo,
                                                 void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_request_upload_dump_data_callback(
    zego_on_request_upload_dump_data callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_request_upload_dump_data_callback)(
    zego_on_request_upload_dump_data callback_func, void *user_context);
#endif

/// Callback when starting to dump data.
///
/// Available since: 3.10.0
/// When to Trigger: This callback is triggered when [startDumpData] is called.
///
/// @param error_code Error code.
/// @param user_context context of user
typedef void (*zego_on_start_dump_data)(int error_code, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL
zego_register_start_dump_data_callback(zego_on_start_dump_data callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_start_dump_data_callback)(
    zego_on_start_dump_data callback_func, void *user_context);
#endif

/// Callback when stopping to dump data.
///
/// Available since: 3.10.0
/// When to Trigger: This callback is triggered when [stopDumpData] is called.
///
/// @param error_code Error code.
/// @param dump_dir Dump data dir.
/// @param user_context context of user
typedef void (*zego_on_stop_dump_data)(int error_code, const char *dump_dir, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL
zego_register_stop_dump_data_callback(zego_on_stop_dump_data callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_stop_dump_data_callback)(
    zego_on_stop_dump_data callback_func, void *user_context);
#endif

/// Callback after uploading the dump data.
///
/// Available since: 3.10.0
/// When to Trigger: When [uploadDumpData] is called, this callback will be triggered after SDK executes the upload task.
///
/// @param error_code Error code.
/// @param user_context context of user
typedef void (*zego_on_upload_dump_data)(int error_code, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL
zego_register_upload_dump_data_callback(zego_on_upload_dump_data callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_upload_dump_data_callback)(
    zego_on_upload_dump_data callback_func, void *user_context);
#endif

/// Callback for test network connectivity.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param result Network connectivity test results
/// @param user_context Context of user.
typedef void (*zego_on_test_network_connectivity)(
    zego_seq seq, zego_error error_code, const struct zego_test_network_connectivity_result result,
    void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_test_network_connectivity_callback(
    zego_on_test_network_connectivity callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_test_network_connectivity_callback)(
    zego_on_test_network_connectivity callback_func, void *user_context);
#endif

/// Callback for network probe.
///
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param result Network probe result
/// @param user_context Context of user.
typedef void (*zego_on_network_probe_result)(zego_seq seq, zego_error error_code,
                                             const struct zego_network_probe_result result,
                                             void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_network_probe_result_callback(
    zego_on_network_probe_result callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_network_probe_result_callback)(
    zego_on_network_probe_result callback_func, void *user_context);
#endif

ZEGO_END_DECLS

#endif
