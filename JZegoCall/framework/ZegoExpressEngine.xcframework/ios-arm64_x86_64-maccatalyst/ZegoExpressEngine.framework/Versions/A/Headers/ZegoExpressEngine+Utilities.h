//
//  ZegoExpressEngine+Utilities.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (Utilities)

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
- (void)startPerformanceMonitor:(unsigned int)millisecond;

/// Stop system performance monitoring.
///
/// Available since: 1.19.0
/// Description: Stop system performance monitoring. After the monitoring is stopped, the [onPerformanceStatusUpdate] callback will not triggered.
/// Use cases: Monitor system performance can help user quickly locate and solve performance problems and improve user experience.
/// When to call: It needs to be called after [createEngine].
/// Restrictions: None.
/// Related APIs: Call [startPerformanceMonitor] to start system performance monitoring.
- (void)stopPerformanceMonitor;

/// Start network probe.
///
/// Available since: 2.3.0
/// Description: Some local network problems may cause audio and video calls to fail. Using this function to probe the network protocols, assist in locating and solving related network problems.
/// Use cases: Before pushing and pulling the stream, detect and locate some possible network problems.
/// When to call: It needs to be called after [createEngine].
/// Restrictions: The SDK will not perform multiple network probe at the same time, that is, if the network probe is in progress, the SDK will ignore subsequent calls of this function.
/// Caution: The SDK detects http, tcp, and udp in sequence internally. If the probe fails in the middle, the subsequent detection will not continue. Therefore, when reading the values ​​in the probe result, please pay attention to check if the value is nil. Network probe may take a long time. Developers can call [stopNetworkProbe] to stop network probe if needed. It is not recommended to start network probe during publishing/playing stream.
/// Related APIs: Call [stopNetworkProbe] to stop network probe.
///
/// @param config network probe config.
/// @param callback Network probe result callback.
- (void)startNetworkProbe:(ZegoNetworkProbeConfig *)config
                 callback:(nullable ZegoNetworkProbeResultCallback)callback;

/// Stop network probe.
///
/// Available since: 2.3.0
/// Description: Stop network probe.
/// Use cases: Before pushing and pulling the stream, detect and locate some possible network problems.
/// When to call: It needs to be called after [createEngine].
/// Restrictions: None.
/// Related APIs: Call [startNetworkProbe] to start network probe.
- (void)stopNetworkProbe;

/// Start network speed test.
///
/// Available since: 1.20.0
/// Description: This function supports uplink/downlink network speed test when the network can be connected.
/// Use cases: This function can be used to detect whether the network environment is suitable for pushing/pulling streams with specified bitrates.
/// When to call: It needs to be called after [loginRoom]. If you call [startPublishingStream]/[startPlayingStream] while speed testing, the speed test will automatically stop.
/// Restrictions: The default maximum allowable test time for a single network speed test is 30 seconds.
/// Caution: Developers can register [onNetworkSpeedTestQualityUpdate] callback to get the speed test result, which will be triggered every 3 seconds. If an error occurs during the speed test process, [onNetworkSpeedTestError] callback will be triggered. If this function is repeatedly called multiple times, the last functioh call's configuration will be used.
/// Related APIs: Call [stopNetworkSpeedTest] to stop network speed test.
///
/// @param config Network speed test configuration.
- (void)startNetworkSpeedTest:(ZegoNetworkSpeedTestConfig *)config;

/// Start network speed test. Support set speed test interval。
///
/// Available since: 1.20.0
/// Description: This function supports uplink/downlink network speed test when the network can be connected.
/// Use cases: This function can be used to detect whether the network environment is suitable for pushing/pulling streams with specified bitrates.
/// When to call: It needs to be called after [loginRoom]. If you call [startPublishingStream]/[startPlayingStream] while speed testing, the speed test will automatically stop.
/// Restrictions: The default maximum allowable test time for a single network speed test is 30 seconds.
/// Caution: Developers can register [onNetworkSpeedTestQualityUpdate] callback to get the speed test result, which will be triggered every 3 seconds. If an error occurs during the speed test process, [onNetworkSpeedTestError] callback will be triggered. If this function is repeatedly called multiple times, the last functioh call's configuration will be used.
/// Related APIs: Call [stopNetworkSpeedTest] to stop network speed test.
///
/// @param config Network speed test configuration.
/// @param interval Interval of network speed test. In milliseconds, default is 3000 ms.
- (void)startNetworkSpeedTest:(ZegoNetworkSpeedTestConfig *)config interval:(unsigned int)interval;

/// Stop network speed test.
///
/// Available since: 1.20.0
/// Description: Stop network speed test.
/// Use cases: This function can be used to detect whether the network environment is suitable for pushing/pulling streams with specified bitrates.
/// When to call: It needs to be called after [createEngine].
/// Restrictions: None.
/// Caution: After the network speed test stopped, [onNetworkSpeedTestQualityUpdate] callback will not be triggered.
/// Related APIs: Call [startNetworkSpeedTest] to start network speed test.
- (void)stopNetworkSpeedTest;

/// Obtain synchronization network time information.
///
/// Available since: 2.9.0
/// Description: Obtain synchronization network time(NTP), including timestamp and maximum deviation.
/// Use cases: When performing multi-terminal synchronization behaviors, network time synchronization is required.
/// When to call: It needs to be called after [createEngine].
/// Restrictions: None.
- (ZegoNetworkTimeInfo *)getNetworkTimeInfo;

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
- (void)startDumpData:(ZegoDumpDataConfig *)config;

/// Stop dumping data.
///
/// Available since: 3.10.0
/// Description: Stop dumping data.
/// Use cases: This is a debugging tool. When there is a problem with audio capturing, 3A processing, or other environment processing during publish, you can dump the audio data and upload it to the ZEGO server for further analysis.
/// When to call: It needs to be called after [startDumpData].
/// Restrictions: None.
/// Caution: It will trigger the [onUploadDumpData] callback.
- (void)stopDumpData;

/// Upload dumped data to the ZEGO server.
///
/// Available since: 3.10.0
/// Description: Upload dumped data to the ZEGO server.
/// Use cases: This is a debugging tool. When there is a problem with audio capturing, 3A processing, or other environment processing during publish, you can dump the audio data and upload it to the ZEGO server for further analysis.
/// When to call: It needs to be called after [stopDumpData].
/// Restrictions: None.
/// Caution: It will trigger the [onUploadDumpData] callback when dump data uploaded.
- (void)uploadDumpData;

/// Remove dumped data.
///
/// Available since: 3.10.0
/// Description: Remove dumped data.
/// Use cases: This is a debugging tool. When there is a problem with audio capturing, 3A processing, or other environment processing during publish, you can dump the audio data and upload it to the ZEGO server for further analysis.
/// When to call: It needs to be called after [stopDumpData]. If the dump data is to be uploaded to the ZEGO server, it should be deleted after the upload is successful.
/// Restrictions: None.
- (void)removeDumpData;

@end

NS_ASSUME_NONNULL_END
