//
//  ZegoExpressEngine.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressEventHandler.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine : NSObject

/// Create ZegoExpressEngine singleton object and initialize SDK.
///
/// Available since: 2.14.0
/// Description: Create ZegoExpressEngine singleton object and initialize SDK.
/// When to call: The engine needs to be created before calling other functions.
/// Restrictions: None.
/// Caution: The SDK only supports the creation of one instance of ZegoExpressEngine. If you need call [createEngine] multiple times, you need call [destroyEngine] before you call the next [createEngine]. Otherwise it will return the instance which created by [createEngine] you called last time.
///
/// @param profile The basic configuration information is used to create the engine.
/// @param eventHandler Event notification callback. [nil] means not receiving any callback notifications.It can also be managed later via [setEventHandler]. If [createEngine] is called repeatedly and the [destroyEngine] function is not called to destroy the engine before the second call, the eventHandler will not be updated.
/// @return engine singleton instance.
+ (ZegoExpressEngine *)createEngineWithProfile:(ZegoEngineProfile *)profile
                                  eventHandler:(nullable id<ZegoEventHandler>)eventHandler;

/// Destroy the ZegoExpressEngine singleton object and deinitialize the SDK.
///
/// Available since: 1.1.0
/// Description: Destroy the ZegoExpressEngine singleton object and deinitialize the SDK.
/// When to call: When the SDK is no longer used, the resources used by the SDK can be released through this interface
/// Restrictions: None.
/// Caution: After using [createEngine] to create a singleton, if the singleton object has not been created or has been destroyed, you will not receive related callbacks when calling this function.
///
/// @param callback Notification callback for destroy engine completion. Developers can listen to this callback to ensure that device hardware resources are released. If the developer only uses SDK to implement audio and video functions, this parameter can be passed [nil].
+ (void)destroyEngine:(nullable ZegoDestroyCompletionCallback)callback;

/// Returns the singleton instance of ZegoExpressEngine.
///
/// Available since: 1.1.0
/// Description: If the engine has not been created or has been destroyed, an unusable engine object will be returned.
/// When to call: After creating the engine, before destroying the engine.
/// Restrictions: None.
///
/// @return Engine singleton instance
+ (ZegoExpressEngine *)sharedEngine;

/// Set advanced engine configuration.
///
/// Available since: 1.1.0
/// Description: Used to enable advanced functions.
/// When to call: Different configurations have different call timing requirements. For details, please consult ZEGO technical support.
/// Restrictions: None.
///
/// @param config Advanced engine configuration
+ (void)setEngineConfig:(ZegoEngineConfig *)config;

/// Set log configuration.
///
/// Available since: 2.3.0
/// Description: If you need to customize the log file size and path, please call this function to complete the configuration.
/// When to call: It must be set before calling [createEngine] to take effect. If it is set after [createEngine], it will take effect at the next [createEngine] after [destroyEngine].
/// Restrictions: None.
/// Caution: Once this interface is called, the method of setting log size and path via [setEngineConfig] will be invalid.Therefore, it is not recommended to use [setEngineConfig] to set the log size and path.
///
/// @param config log configuration.
+ (void)setLogConfig:(ZegoLogConfig *)config;

/// Set local proxy config.
///
/// Available since: 3.1.0
/// Description: If you need to use the local proxy feature, please call this function to complete the configuration.
/// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
/// Restrictions: After [createEngine], can not change the proxy.
/// Caution: None.
///
/// @param proxyList proxy info.
/// @param enable enable proxy or not.
+ (void)setLocalProxyConfig:(NSArray<ZegoProxyInfo *> *)proxyList enable:(BOOL)enable;

/// Set cloud proxy config.
///
/// Available since: 3.1.0
/// Description: If you need to use the cloud proxy feature, please call this function to complete the configuration.
/// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
/// Restrictions: After [createEngine], can not change the proxy.
/// Caution: None.
///
/// @param proxyList proxy info.
/// @param token token. if use appsign auth, ignore.
/// @param enable enable proxy or not.
+ (void)setCloudProxyConfig:(NSArray<ZegoProxyInfo *> *)proxyList
                      token:(NSString *)token
                     enable:(BOOL)enable;

/// Set license auth.
///
/// Available since: 3.5.0
/// Description: This function needs to be called to complete the configuration when using License authentication charges.
/// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
/// Restrictions: Does not support midway changes.
/// Caution: None.
///
/// @param license license.
+ (void)setLicense:(NSString *)license;

/// Set room mode.
///
/// Available since: 2.9.0
/// Description: If you need to use the multi-room feature, please call this function to complete the configuration.
/// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
/// Restrictions: If you need to use the multi-room feature, please contact the instant technical support to configure the server support.
/// Caution: None.
///
/// @param mode Room mode. Description: Used to set the room mode. Use cases: If you need to enter multiple rooms at the same time for publish-play stream, please turn on the multi-room mode through this interface. Required: True. Default value: ZEGO_ROOM_MODE_SINGLE_ROOM.
+ (void)setRoomMode:(ZegoRoomMode)mode;

/// Set Geo Fence.
///
/// Available since: 3.4.0
/// Description: If you need to use the geo fence feature, please call this function to complete the configuration.
/// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
/// Restrictions: If you need to use the geo fence feature, please contact ZEGO Technical Support.
/// Caution: None.
///
/// @param type Geo fence type. Description: Used to set the geo fence type.
/// @param area Geo fence area. Description: Used to describe the range of geo fence.
+ (void)setGeoFence:(ZegoGeoFenceType)type area:(NSArray<NSNumber *> *)area;

/// Gets the SDK's version number.
///
/// Available since: 1.1.0
/// Description: If you encounter an abnormality during the running of the SDK, you can submit the problem, log and other information to the ZEGO technical staff to locate and troubleshoot. Developers can also collect current SDK version information through this API, which is convenient for App operation statistics and related issues.
/// When to call: Any time.
/// Restrictions: None.
/// Caution: None.
///
/// @return SDK version.
+ (NSString *)getVersion;

/// Set method execution result callback.
///
/// Available since: 2.3.0
/// Description: Set the setting of the execution result of the calling method. After setting, you can get the detailed information of the result of each execution of the ZEGO SDK method.
/// When to call: Any time.
/// Restrictions: None.
/// Caution: It is recommended that developers call this interface only when they need to obtain the call results of each interface. For example, when troubleshooting and tracing problems. Developers generally do not need to pay attention to this interface.
///
/// @param callback Method execution result callback.
+ (void)setApiCalledCallback:(nullable id<ZegoApiCalledEventHandler>)callback;

/// Query whether the current SDK supports the specified feature.
///
/// Available since: 2.22.0
/// Description:
///   Since the SDK supports feature trimming, some features may be trimmed;
///   you can use this function to quickly determine whether the current SDK supports the specified features,
///   such as querying whether the media player feature is supported.
/// When to call: Any time.
///
/// @param featureType Type of feature to query.
/// @return Whether the specified feature is supported. YES: supported; NO: not supported.
+ (BOOL)isFeatureSupported:(ZegoFeatureType)featureType;

/// Sets up the event notification callbacks that need to be handled. If the eventHandler is set to [nil], all the callbacks set previously will be cleared.
///
/// Available since: 1.1.0
/// Description: Set up event notification callbacks, used to monitor callbacks such as engine status changes, room status changes, etc.
/// When to call: After [createEngine].
/// Restrictions: None.
/// Caution: After calling this function or [createEngine] setting a callback, unless this function is called to clear the callback, setting a callback again will not take effect. After calling [destroyEngine], the event handler that has been set will be invalid and need to be reset after next calling of [createEngine].
///
/// @param eventHandler Event notification callback. If the eventHandler is set to [nil], all the callbacks set previously will be cleared. Developers should monitor the corresponding callbacks according to their own business scenarios. The main callback functions of the SDK are here.
- (void)setEventHandler:(nullable id<ZegoEventHandler>)eventHandler;

/// Set room scenario.
///
/// Available since: 3.0.0
/// Description: You can set the scenario of the room, and the SDK will adopt different optimization strategies for different scenarios in order to obtain better effects; this function does exactly the same thing as the [scenario] parameter in the [profile] configuration of [createEngine].
/// Use cases: This function is suitable for apps in various audio and video business scenarios, such as 1v1 video call (or voice call) scenario and live show scenario; this function can be used to switch scenarios without destroying the engine through [destroyEngine].
/// When to call: Must be set before calling [loginRoom] AND after calling [createEngine].
/// Restrictions: Once you log in to the room, you are no longer allowed to modify the room scenario. If you need to modify the scenario, you need to log out of the room first. If you log in to multiple rooms, you need to log out of all rooms before you can modify it.
/// Caution:
///   1. Users in the same room are recommended to use the same room scenario for best results.
///   2. Setting the scenario will affect the audio and video bit rate, frame rate, resolution, codec id, audio device mode, audio route type, traffic control, 3A, ear return and other audio and video configurations. If you have special needs, you can call various other APIs to set the above configuration after calling this API.
///   3. Calling this function will override the scenario specified on [createEngine] or the scenario set by the last call to this function.
///   4. Calling this function will overwrite the audio and video related configuration you set through APIs such as [setVideoConfig], [setAudioConfig], so it is recommended to set the scenario first and then adjust the audio and video configuration through other APIs.
///
/// @param scenario Room scenario.
- (void)setRoomScenario:(ZegoScenario)scenario;

/// Uploads logs to the ZEGO server.
///
/// Available since: 1.1.0
/// Description: By default, SDK creates and prints log files in the App's default directory. Each log file defaults to a maximum of 5MB. Three log files are written over and over in a circular fashion. When calling this function, SDK will auto package and upload the log files to the ZEGO server.
/// Use cases: Developers can provide a business “feedback” channel in the App. When users feedback problems, they can call this function to upload the local log information of SDK to help locate user problems.
/// When to call: After [createEngine].
/// Restrictions:  The frequency limit is once per minute.
/// Caution: 1.After calling this interface to upload logs, if you call [destroyEngine] or exit the App too quickly, there may be a failure.It is recommended to wait a few seconds, and then call [destroyEngine] or exit the App after receiving the upload success callback. 2.If you want to call before [createEngine], use the [submitLog] interface.
- (void)uploadLog;

/// Uploads logs to the ZEGO server, with callback.
///
/// Available since: 2.4.0
/// Description: By default, SDK creates and prints log files in the App's default directory. Each log file defaults to a maximum of 5MB. Three log files are written over and over in a circular fashion. When calling this function, SDK will auto package and upload the log files to the ZEGO server.
/// Use cases: Developers can provide a business “feedback” channel in the App. When users feedback problems, they can call this function to upload the local log information of SDK to help locate user problems.
/// When to call: After [createEngine].
/// Restrictions:  The frequency limit is once per minute.
/// Caution: 1.After calling this interface to upload logs, if you call [destroyEngine] or exit the App too quickly, there may be a failure.It is recommended to wait a few seconds, and then call [destroyEngine] or exit the App after receiving the upload success callback. 2.If you want to call before [createEngine], use the [submitLog] interface.
///
/// @param callback Log upload result callback.
- (void)uploadLog:(nullable ZegoUploadLogResultCallback)callback;

/// Uploads logs to the ZEGO server.
///
/// Available since: 3.7.0
/// Description: By default, SDK creates and prints log files in the App's default directory. Each log file defaults to a maximum of 5MB. Three log files are written over and over in a circular fashion. When calling this function, SDK will auto package and upload the log files to the ZEGO server.
/// Use cases: Developers can provide a business “feedback” channel in the App. When users feedback problems, they can call this function to upload the local log information of SDK to help locate user problems.
/// When to call: None.
/// Restrictions: The frequency limit is once per minute.
/// Caution: 1.After calling this interface to upload logs, if you call [destroyEngine] or exit the App too quickly, there may be a failure.It is recommended to wait a few seconds, and then call [destroyEngine] or exit the App after receiving the upload success callback. 2.It is supported to call before [createEngine]. If it had called [createEngine] before, the last appid will be used to upload the log, otherwise the log will not be uploaded until the next [createEngine].
+ (void)submitLog;

/// Enable the debug assistant. Note, do not enable this feature in the online version! Use only during development phase!
///
/// Available since: 2.17.0
/// Description: After enabled, the SDK will print logs to the console, and will pop-up an alert (toast) UI message when there is a problem with calling other SDK functions.
/// Default value: This function is disabled by default.
/// When to call: This function can be called right after [createEngine].
/// Platform differences: The pop-up alert function only supports Android / iOS / macOS / Windows, and the console log function supports all platforms.
/// Caution: Be sure to confirm that this feature is turned off before the app is released to avoid pop-up UI alert when an error occurs in your release version's app. It is recommended to associate the [enable] parameter of this function with the DEBUG variable of the app, that is, only enable the debug assistant in the DEBUG environment.
/// Restrictions: None.
///
/// @param enable Whether to enable the debug assistant.
- (void)enableDebugAssistant:(BOOL)enable;

/// Call the experimental API.
///
/// Available since: 2.7.0
/// Description: ZEGO provides some technical previews or special customization functions in RTC business through this API. If you need to get the use of the function or the details, please consult ZEGO technical support.
/// When to call: After [createEngine].
///
/// @param params Parameters in the format of a JSON string, please consult ZEGO technical support for details.
/// @return Returns an argument in the format of a JSON string, please consult ZEGO technical support for details.
- (NSString *)callExperimentalAPI:(NSString *)params;

/// This function is unavailable.
///
/// Please use [+createEngineWithAppID:appSign:isTestEnv:scenario:eventHandler:] instead
+ (instancetype)new NS_UNAVAILABLE;

/// This function is unavailable.
///
/// Please use [+createEngineWithAppID:appSign:isTestEnv:scenario:eventHandler:] instead
- (instancetype)init NS_UNAVAILABLE;

/// [Deprecated] Create ZegoExpressEngine singleton object and initialize SDK. Deprecated since 2.14.0, please use the method with the same name without [isTestEnv] parameter instead. Please refer to [Testing environment deprecation](https://docs.zegocloud.com/article/13315) for more details.
///
/// Available: 1.1.0 ~ 2.13.1, deprecated since 2.14.0, please use the method with the same name without [isTestEnv] parameter instead
/// Description: Create ZegoExpressEngine singleton object and initialize SDK.
/// When to call: The engine needs to be created before calling other functions.
/// Restrictions: None.
/// Caution: The SDK only supports the creation of one instance of ZegoExpressEngine. If you need call [createEngine] multiple times, you need call [destroyEngine] before you call the next [createEngine]. Otherwise it will return the instance which created by [createEngine] you called last time.
///
/// @deprecated Deprecated since 2.14.0, please use the method with the same name without [isTestEnv] parameter instead.
/// @param appID Application ID issued by ZEGO for developers, please apply from the ZEGO Admin Console https://console.zegocloud.com The value ranges from 0 to 4294967295.
/// @param appSign Application signature for each AppID, please apply from the ZEGO Admin Console. Application signature is a 64 character string. Each character has a range of '0' ~ '9', 'a' ~ 'z'. AppSign 2.17.0 and later allows nil or no transmission. If the token is passed empty or not passed, the token must be entered in the [ZegoRoomConfig] parameter for authentication when the [loginRoom] interface is called to login to the room.
/// @param isTestEnv [Deprecated] For providing better and more standardized services, starting from 2021-11-16, ZEGO no longer classifies environments into production environments and testing environments. f you create your project in ZEGO Admin Console on/before 2021-11-16, refer to [Testing environment deprecation](https://docs.zegocloud.com/article/13315) to upgrade the SDK and adjust related codes.
/// @param scenario The room scenario. the SDK will optimize the audio and video configuration for the specified scenario to achieve the best effect in this scenario. After specifying the scenario, you can call other APIs to adjusting the audio and video configuration. Differences between scenarios and how to choose a suitable scenario, please refer to https://docs.zegocloud.com/article/14940
/// @param eventHandler Event notification callback. [nil] means not receiving any callback notifications.It can also be managed later via [setEventHandler]. If [createEngine] is called repeatedly and the [destroyEngine] function is not called to destroy the engine before the second call, the eventHandler will not be updated.
/// @return Engine singleton instance.
+ (ZegoExpressEngine *)createEngineWithAppID:(unsigned int)appID
                                     appSign:(NSString *)appSign
                                   isTestEnv:(BOOL)isTestEnv
                                    scenario:(ZegoScenario)scenario
                                eventHandler:(nullable id<ZegoEventHandler>)eventHandler
    DEPRECATED_ATTRIBUTE;

@end

NS_ASSUME_NONNULL_END

#import "ZegoExpressEngine+AIVoiceChanger.h"
#import "ZegoExpressEngine+AudioEffectPlayer.h"
#import "ZegoExpressEngine+AudioVADClient.h"
#import "ZegoExpressEngine+CopyrightedMusic.h"
#import "ZegoExpressEngine+CustomAudioIO.h"
#import "ZegoExpressEngine+CustomVideoIO.h"
#import "ZegoExpressEngine+Device.h"
#import "ZegoExpressEngine+IM.h"
#import "ZegoExpressEngine+MediaDataPublisher.h"
#import "ZegoExpressEngine+MediaPlayer.h"
#import "ZegoExpressEngine+Mixer.h"
#import "ZegoExpressEngine+Player.h"
#import "ZegoExpressEngine+Preprocess.h"
#import "ZegoExpressEngine+Publisher.h"
#import "ZegoExpressEngine+RangeAudio.h"
#import "ZegoExpressEngine+RangeScene.h"
#import "ZegoExpressEngine+Record.h"
#import "ZegoExpressEngine+ReplayKit.h"
#import "ZegoExpressEngine+Room.h"
#import "ZegoExpressEngine+ScreenCapture.h"
#import "ZegoExpressEngine+Utilities.h"
