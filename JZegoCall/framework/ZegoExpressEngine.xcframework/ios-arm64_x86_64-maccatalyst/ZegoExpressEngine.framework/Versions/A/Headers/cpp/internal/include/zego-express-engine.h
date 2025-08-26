
#ifndef __ZEGO_EXPRESS_ENGINE_H__
#define __ZEGO_EXPRESS_ENGINE_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Create ZegoExpressEngine singleton object and initialize SDK.
///
/// Available since: 2.14.0
/// Description: Create ZegoExpressEngine singleton object and initialize SDK.
/// When to call: The engine needs to be created before calling other functions.
/// Restrictions: None.
/// Caution: The SDK only supports the creation of one instance of ZegoExpressEngine. If you need call [createEngine] multiple times, you need call [destroyEngine] before you call the next [createEngine]. Otherwise it will return the instance which created by [createEngine] you called last time.
///
/// @param profile The basic configuration information is used to create the engine.
/// @return Zego error code.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_engine_init_with_profile(struct zego_engine_profile profile);
#else
typedef zego_error(EXP_CALL *pfnzego_express_engine_init_with_profile)(
    struct zego_engine_profile profile);
#endif

/// Destroy the ZegoExpressEngine singleton object and deinitialize the SDK.
///
/// Available since: 1.1.0
/// Description: Destroy the ZegoExpressEngine singleton object and deinitialize the SDK.
/// When to call: When the SDK is no longer used, the resources used by the SDK can be released through this interface
/// Restrictions: None.
/// Caution: After using [createEngine] to create a singleton, if the singleton object has not been created or has been destroyed, you will not receive related callbacks when calling this function.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_engine_uninit_async(void);
#else
typedef zego_error(EXP_CALL *pfnzego_express_engine_uninit_async)(void);
#endif

/// Set advanced engine configuration.
///
/// Available since: 1.1.0
/// Description: Used to enable advanced functions.
/// When to call: Different configurations have different call timing requirements. For details, please consult ZEGO technical support.
/// Restrictions: None.
///
/// @param config Advanced engine configuration
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_engine_config(struct zego_engine_config config);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_engine_config)(struct zego_engine_config config);
#endif

/// Set log configuration.
///
/// Available since: 2.3.0
/// Description: If you need to customize the log file size and path, please call this function to complete the configuration.
/// When to call: It must be set before calling [createEngine] to take effect. If it is set after [createEngine], it will take effect at the next [createEngine] after [destroyEngine].
/// Restrictions: None.
/// Caution: Once this interface is called, the method of setting log size and path via [setEngineConfig] will be invalid.Therefore, it is not recommended to use [setEngineConfig] to set the log size and path.
///
/// @param config log configuration.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_log_config(struct zego_log_config config);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_log_config)(struct zego_log_config config);
#endif

/// Set local proxy config.
///
/// Available since: 3.1.0
/// Description: If you need to use the local proxy feature, please call this function to complete the configuration.
/// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
/// Restrictions: After [createEngine], can not change the proxy.
/// Caution: None.
///
/// @param proxy_list proxy info.
/// @param proxy_count proxy count.
/// @param enable enable proxy or not.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_local_proxy_config(
    const struct zego_proxy_info *proxy_list, int proxy_count, bool enable);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_local_proxy_config)(
    const struct zego_proxy_info *proxy_list, int proxy_count, bool enable);
#endif

/// Set cloud proxy config.
///
/// Available since: 3.1.0
/// Description: If you need to use the cloud proxy feature, please call this function to complete the configuration.
/// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
/// Restrictions: After [createEngine], can not change the proxy.
/// Caution: None.
///
/// @param proxy_list proxy info.
/// @param proxy_count proxy count.
/// @param token token. if use appsign auth, ignore.
/// @param enable enable proxy or not.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_cloud_proxy_config(
    const struct zego_proxy_info *proxy_list, int proxy_count, const char *token, bool enable);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_cloud_proxy_config)(
    const struct zego_proxy_info *proxy_list, int proxy_count, const char *token, bool enable);
#endif

/// Set license auth.
///
/// Available since: 3.5.0
/// Description: This function needs to be called to complete the configuration when using License authentication charges.
/// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
/// Restrictions: Does not support midway changes.
/// Caution: None.
///
/// @param license license.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_license(const char *license);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_license)(const char *license);
#endif

/// Set room mode.
///
/// Available since: 2.9.0
/// Description: If you need to use the multi-room feature, please call this function to complete the configuration.
/// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
/// Restrictions: If you need to use the multi-room feature, please contact the instant technical support to configure the server support.
/// Caution: None.
///
/// @param mode Room mode. Description: Used to set the room mode. Use cases: If you need to enter multiple rooms at the same time for publish-play stream, please turn on the multi-room mode through this interface. Required: True. Default value: ZEGO_ROOM_MODE_SINGLE_ROOM.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_room_mode(enum zego_room_mode mode);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_room_mode)(enum zego_room_mode mode);
#endif

/// Set Geo Fence.
///
/// Available since: 3.4.0
/// Description: If you need to use the geo fence feature, please call this function to complete the configuration.
/// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
/// Restrictions: If you need to use the geo fence feature, please contact ZEGO Technical Support.
/// Caution: None.
///
/// @param type Geo fence type. Description: Used to set the geo fence type.
/// @param area_list Geo fence area. Description: Used to describe the range of geo fence.
/// @param area_list_count geo fence area count.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_geo_fence(enum zego_geo_fence_type type,
                                                           const int *area_list,
                                                           int area_list_count);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_geo_fence)(enum zego_geo_fence_type type,
                                                            const int *area_list,
                                                            int area_list_count);
#endif

/// Gets the SDK's version number.
///
/// Available since: 1.1.0
/// Description: If you encounter an abnormality during the running of the SDK, you can submit the problem, log and other information to the ZEGO technical staff to locate and troubleshoot. Developers can also collect current SDK version information through this API, which is convenient for App operation statistics and related issues.
/// When to call: Any time.
/// Restrictions: None.
/// Caution: None.
///
/// @param version [in/out] SDK version.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_get_version(const char **version);
#else
typedef zego_error(EXP_CALL *pfnzego_express_get_version)(const char **version);
#endif

/// Sets the JVM and Context for the Android platform.
///
/// Available since: 1.1.0
/// Description: Sets the JVM and Context for the Android platform.
/// When to call: It must be called before calling [createEngine] function.
/// Restrictions: Only available on Android platform.
/// Caution: None.
///
/// @param jvm Java VM Object.
/// @param context Android Context, it must be valid in the SDK lifecycle.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_android_env(void *jvm, void *context);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_android_env)(void *jvm, void *context);
#endif

/// Config the Environment for the OpenHarmony platform.
///
/// Available since: 3.18.0
/// Description: Config the Environment for the OpenHarmony platform.
/// When to call: It must be called before calling [createEngine] function.
/// Restrictions: Only available on OpenHarmony platform.
/// Caution: None.
///
/// @param env napi_env Object.
/// @param exports napi_value exports Object.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_ohos_env(void *env, void *exports);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_ohos_env)(void *env, void *exports);
#endif

/// Get the previously set Android context.
///
/// Available since: 1.1.0
/// Description: Get the previously set Android context.
/// When to call: None.
/// Restrictions: Only available on Android platform.
/// Caution: None.
///
/// @param context [in/out] If [setAndroidEnv] has not been called before [createEngine], or if it is not called on the Android platform, it returns NULL.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_get_android_context(void **context);
#else
typedef zego_error(EXP_CALL *pfnzego_express_get_android_context)(void **context);
#endif

/// Query whether the current SDK supports the specified feature.
///
/// Available since: 2.22.0
/// Description:
///   Since the SDK supports feature trimming, some features may be trimmed;
///   you can use this function to quickly determine whether the current SDK supports the specified features,
///   such as querying whether the media player feature is supported.
/// When to call: Any time.
///
/// @param feature_type Type of feature to query.
/// @param is_supported [in/out] Whether the specified feature is supported.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_is_feature_supported(enum zego_feature_type feature_type, bool *is_supported);
#else
typedef zego_error(EXP_CALL *pfnzego_express_is_feature_supported)(
    enum zego_feature_type feature_type, bool *is_supported);
#endif

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
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_room_scenario(enum zego_scenario scenario);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_room_scenario)(enum zego_scenario scenario);
#endif

/// Uploads logs to the ZEGO server.
///
/// Available since: 1.1.0
/// Description: By default, SDK creates and prints log files in the App's default directory. Each log file defaults to a maximum of 5MB. Three log files are written over and over in a circular fashion. When calling this function, SDK will auto package and upload the log files to the ZEGO server.
/// Use cases: Developers can provide a business “feedback” channel in the App. When users feedback problems, they can call this function to upload the local log information of SDK to help locate user problems.
/// When to call:None.
/// Restrictions:  The frequency limit is once per minute.
/// Caution: 1.After calling this interface to upload logs, if you call [destroyEngine] or exit the App too quickly, there may be a failure.It is recommended to wait a few seconds, and then call [destroyEngine] or exit the App after receiving the upload success callback. 2.It is supported to call before [createEngine]. If it had called [createEngine] before, the last appid will be used to upload the log, otherwise the log will not be uploaded until the next [createEngine].
///
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_upload_log(zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_upload_log)(zego_seq *sequence);
#endif

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
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_debug_assistant(bool enable);
#else
typedef zego_error(EXP_CALL *pfnzego_express_enable_debug_assistant)(bool enable);
#endif

/// Call the experimental API.
///
/// Available since: 2.7.0
/// Description: ZEGO provides some technical previews or special customization functions in RTC business through this API. If you need to get the use of the function or the details, please consult ZEGO technical support.
/// When to call: After [createEngine].
///
/// @param params Parameters in the format of a JSON string, please consult ZEGO technical support for details.
/// @param result [in/out] Returns an argument in the format of a JSON string, please consult ZEGO technical support for details. After the call is successful, please call [freeCallExperimentalAPIResult] to reclaim the dynamic memory.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_call_experimental_api(const char *params,
                                                                   char **result);
#else
typedef zego_error(EXP_CALL *pfnzego_express_call_experimental_api)(const char *params,
                                                                    char **result);
#endif

/// Release the dynamic memory of the output parameter result of the [callExperimentalAPI] interface.
///
/// Available since: 3.6.0
/// Description: After calling [callExperimentalAPI], you need to call this interface to release the result parameter dynamic memory, otherwise it will cause a memory leak.
/// When to call: After [callExperimentalAPI].
///
/// @param p The output parameter result of the [callExperimentalAPI] interface is a dynamic memory.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_free_call_experimental_api_result(char *p);
#else
typedef zego_error(EXP_CALL *pfnzego_express_free_call_experimental_api_result)(char *p);
#endif

/// Get a seq for context association of callbacks.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_seq EXP_CALL zego_express_get_increase_seq();
#else
typedef zego_seq(EXP_CALL *pfnzego_express_get_increase_seq)();
#endif

/// The callback for obtaining debugging error information.
///
/// Available since: 1.1.0
/// Description: When the SDK functions are not used correctly, the callback prompts for detailed error information.
/// Trigger: Notify the developer when an exception occurs in the SDK.
/// Restrictions: None.
/// Caution: None.
///
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param func_name Function name.
/// @param info Detailed error information.
/// @param user_context context of user.
typedef void (*zego_on_debug_error)(int error_code, const char *func_name, const char *info,
                                    void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_debug_error_callback(zego_on_debug_error callback_func,
                                                             void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_debug_error_callback)(zego_on_debug_error callback_func,
                                                              void *user_context);
#endif

/// Method execution result callback
///
/// Available since: 2.3.0
/// Description: When the monitoring is turned on through [setApiCalledCallback], the results of the execution of all methods will be called back through this callback.
/// Trigger: When the developer calls the SDK method, the execution result of the method is called back.
/// Restrictions: None.
/// Caution: It is recommended to monitor and process this callback in the development and testing phases, and turn off the monitoring of this callback after going online.
///
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param func_name Function name.
/// @param info Detailed error information.
/// @param user_context context of user.
typedef void (*zego_on_api_called_result)(int error_code, const char *func_name, const char *info,
                                          void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_api_called_result_callback(
    zego_on_api_called_result callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_api_called_result_callback)(
    zego_on_api_called_result callback_func, void *user_context);
#endif

/// The callback triggered when the audio/video engine state changes.
///
/// Available since: 1.1.0
/// Description: Callback notification of audio/video engine status update. When audio/video functions are enabled, such as preview, push streaming, local media player, audio data observering, etc., the audio/video engine will enter the start state. When you exit the room or disable all audio/video functions , The audio/video engine will enter the stop state.
/// Trigger: The developer called the relevant function to change the state of the audio and video engine. For example: 1. Called ZegoExpressEngine's [startPreview], [stopPreview], [startPublishingStream], [stopPublishingStream], [startPlayingStream], [stopPlayingStream], [startAudioDataObserver], [stopAudioDataObserver] and other functions. 2. The related functions of MediaPlayer are called. 3. The [LogoutRoom] function was called. 4. The related functions of RealTimeSequentialDataManager are called.
/// Restrictions: None.
/// Caution:
///   1. When the developer calls [destroyEngine], this notification will not be triggered because the resources of the SDK are completely released.
///   2. If there is no special need, the developer does not need to pay attention to this callback.
///
/// @param state The audio/video engine state.
/// @param user_context context of user.
typedef void (*zego_on_engine_state_update)(enum zego_engine_state state, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_engine_state_update_callback(
    zego_on_engine_state_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_engine_state_update_callback)(
    zego_on_engine_state_update callback_func, void *user_context);
#endif

/// Audio and video engine destruction notification callback.
///
/// When you use the asynchronous destruction engine function, you can obtain whether the SDK has completely released resources by listening to this callback.
///
/// @param user_context context of user.
typedef void (*zego_on_engine_uninit)(void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_engine_uninit_callback(zego_on_engine_uninit callback_func,
                                                               void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_engine_uninit_callback)(zego_on_engine_uninit callback_func,
                                                                void *user_context);
#endif

/// Experimental API callback
///
/// Available since: 2.7.0
/// Description: Receive experimental API callbacks in JSON string format.
/// Caution: Please use this feature with the help of ZEGO Technical Support.
///
/// @param content Callback content in JSON string format.
/// @param user_context Context of user.
typedef void (*zego_on_recv_experimental_api)(const char *content, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_recv_experimental_api_callback(
    zego_on_recv_experimental_api callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_recv_experimental_api_callback)(
    zego_on_recv_experimental_api callback_func, void *user_context);
#endif

/// The callback that triggered a fatal error, causing the SDK to malfunction and unable to function properly.
///
/// Available since: 3.6.0
/// Description: The callback that triggered a fatal error.
/// Trigger: The APP has enabled the restriction of access from foreign IP addresses, and the current client is outside of the domain.
/// Restrictions: None.
/// Caution:
///   None.
///
/// @param error_code Error code.
/// @param user_context context of user.
typedef void (*zego_on_fatal_error)(int error_code, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_fatal_error_callback(zego_on_fatal_error callback_func,
                                                             void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_fatal_error_callback)(zego_on_fatal_error callback_func,
                                                              void *user_context);
#endif

/// Callback notification for video backend changes.
///
/// Available since: 3.18.0
/// Description: Callback notification for changes in the video backend used by the engine.
/// Caution:
///   1. When the developer calls [destroyEngine], this notification will not be triggered because the resources of the SDK are completely released.
///   2. If there is no special need, the developer does not need to pay attention to this callback.
///
/// @param type Video backend type.
/// @param user_context context of user.
typedef void (*zego_on_video_backend_type_changed)(enum zego_video_backend_type type,
                                                   void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_video_backend_type_changed_callback(
    zego_on_video_backend_type_changed callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_video_backend_type_changed_callback)(
    zego_on_video_backend_type_changed callback_func, void *user_context);
#endif

/// [Deprecated] Create ZegoExpressEngine singleton object and initialize SDK. Deprecated since 2.14.0, please use the method with the same name without [isTestEnv] parameter instead. Please refer to [Testing environment deprecation](https://docs.zegocloud.com/article/13315) for more details.
///
/// Available: 1.1.0 ~ 2.13.1, deprecated since 2.14.0, please use the method with the same name without [isTestEnv] parameter instead
/// Description: Create ZegoExpressEngine singleton object and initialize SDK.
/// When to call: The engine needs to be created before calling other functions.
/// Restrictions: None.
/// Caution: The SDK only supports the creation of one instance of ZegoExpressEngine. If you need call [createEngine] multiple times, you need call [destroyEngine] before you call the next [createEngine]. Otherwise it will return the instance which created by [createEngine] you called last time.
///
/// @deprecated Deprecated since 2.14.0, please use the method with the same name without [isTestEnv] parameter instead.
/// @param app_id Application ID issued by ZEGO for developers, please apply from the ZEGO Admin Console https://console.zegocloud.com The value ranges from 0 to 4294967295.
/// @param app_sign Application signature for each AppID, please apply from the ZEGO Admin Console. Application signature is a 64 character string. Each character has a range of '0' ~ '9', 'a' ~ 'z'. AppSign 2.17.0 and later allows NULL or no transmission. If the token is passed empty or not passed, the token must be entered in the [ZegoRoomConfig] parameter for authentication when the [loginRoom] interface is called to login to the room.
/// @param is_test_env [Deprecated] For providing better and more standardized services, starting from 2021-11-16, ZEGO no longer classifies environments into production environments and testing environments. f you create your project in ZEGO Admin Console on/before 2021-11-16, refer to [Testing environment deprecation](https://docs.zegocloud.com/article/13315) to upgrade the SDK and adjust related codes.
/// @param scenario The room scenario. the SDK will optimize the audio and video configuration for the specified scenario to achieve the best effect in this scenario. After specifying the scenario, you can call other APIs to adjusting the audio and video configuration. Differences between scenarios and how to choose a suitable scenario, please refer to https://docs.zegocloud.com/article/14940
/// @return Zego error code.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_engine_init(unsigned int app_id, const char *app_sign,
                                                         bool is_test_env,
                                                         enum zego_scenario scenario);
#else
typedef zego_error(EXP_CALL *pfnzego_express_engine_init)(unsigned int app_id, const char *app_sign,
                                                          bool is_test_env,
                                                          enum zego_scenario scenario);
#endif

/// Log upload result callback.
///
/// Description: After calling [uploadLog] to upload the log, get the upload result through this callback.
/// Use cases: When uploading logs, in order to determine whether the logs are uploaded successfully, you can get them through this callback.
/// Caution: In the case of poor network, the return time of this callback may be longer.
///
/// @param seq The serial number returned by calling [uploadLog] is used to match calls and callbacks.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param user_context Context of user.
typedef void (*zego_on_upload_log_result)(zego_seq seq, zego_error error_code, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_upload_log_result_callback(
    zego_on_upload_log_result callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_upload_log_result_callback)(
    zego_on_upload_log_result callback_func, void *user_context);
#endif

ZEGO_END_DECLS

#endif
