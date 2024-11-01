
#ifndef __ZEGOEXPRESSSDK_H__
#define __ZEGOEXPRESSSDK_H__

#include "internal/ZegoInternalEngineImpl.hpp"

namespace ZEGO {
namespace EXPRESS {

class ZegoExpressSDK {
  private:
    ZegoExpressSDK() = default;
    ~ZegoExpressSDK() = default;

  public:
    /// Create ZegoExpressEngine singleton object and initialize SDK.
    ///
    /// Available since: 2.14.0
    /// Description: Create ZegoExpressEngine singleton object and initialize SDK.
    /// When to call: The engine needs to be created before calling other functions.
    /// Restrictions: None.
    /// Caution: The SDK only supports the creation of one instance of ZegoExpressEngine. If you need call [createEngine] multiple times, you need call [destroyEngine] before you call the next [createEngine]. Otherwise it will return the instance which created by [createEngine] you called last time.
    ///
    /// @param profile The basic configuration information is used to create the engine.
    /// @param eventHandler Event notification callback. [nullptr] means not receiving any callback notifications.It can also be managed later via [setEventHandler]. If [createEngine] is called repeatedly and the [destroyEngine] function is not called to destroy the engine before the second call, the eventHandler will not be updated.
    /// @return engine singleton instance.
    static IZegoExpressEngine *createEngine(ZegoEngineProfile profile,
                                            std::shared_ptr<IZegoEventHandler> eventHandler) {
        return ZegoExpressSDKInternal::createEngine(profile, eventHandler);
    }

    /// Destroy the ZegoExpressEngine singleton object and deinitialize the SDK.
    ///
    /// Available since: 1.1.0
    /// Description: Destroy the ZegoExpressEngine singleton object and deinitialize the SDK.
    /// When to call: When the SDK is no longer used, the resources used by the SDK can be released through this interface
    /// Restrictions: None.
    /// Caution: After using [createEngine] to create a singleton, if the singleton object has not been created or has been destroyed, you will not receive related callbacks when calling this function.
    ///
    /// @param engine engine instance that created by createEngine method.
    /// @param callback Notification callback for destroy engine completion. Developers can listen to this callback to ensure that device hardware resources are released. If the developer only uses SDK to implement audio and video functions, this parameter can be passed [nullptr].
    static void destroyEngine(IZegoExpressEngine *&engine,
                              ZegoDestroyCompletionCallback callback = nullptr) {
        ZegoExpressSDKInternal::destroyEngine(engine, callback);
    }

    /// Returns the singleton instance of ZegoExpressEngine.
    ///
    /// Available since: 1.1.0
    /// Description: If the engine has not been created or has been destroyed, returns [nullptr].
    /// When to call: After creating the engine, before destroying the engine.
    /// Restrictions: None.
    ///
    /// @return Engine singleton instance
    static IZegoExpressEngine *getEngine() { return ZegoExpressSDKInternal::getEngine(); }

    /// Set advanced engine configuration.
    ///
    /// Available since: 1.1.0
    /// Description: Used to enable advanced functions.
    /// When to call: Different configurations have different call timing requirements. For details, please consult ZEGO technical support.
    /// Restrictions: None.
    ///
    /// @param config Advanced engine configuration
    static void setEngineConfig(ZegoEngineConfig config) {
        ZegoExpressSDKInternal::setEngineConfig(config);
    }

    /// Set log configuration.
    ///
    /// Available since: 2.3.0
    /// Description: If you need to customize the log file size and path, please call this function to complete the configuration.
    /// When to call: It must be set before calling [createEngine] to take effect. If it is set after [createEngine], it will take effect at the next [createEngine] after [destroyEngine].
    /// Restrictions: None.
    /// Caution: Once this interface is called, the method of setting log size and path via [setEngineConfig] will be invalid.Therefore, it is not recommended to use [setEngineConfig] to set the log size and path.
    ///
    /// @param config log configuration.
    static void setLogConfig(ZegoLogConfig config) { ZegoExpressSDKInternal::setLogConfig(config); }

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
    static void setLocalProxyConfig(const std::vector<ZegoProxyInfo> &proxyList, bool enable) {
        ZegoExpressSDKInternal::setLocalProxyConfig(proxyList, enable);
    }

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
    static void setCloudProxyConfig(const std::vector<ZegoProxyInfo> &proxyList,
                                    const std::string &token, bool enable) {
        ZegoExpressSDKInternal::setCloudProxyConfig(proxyList, token, enable);
    }

    /// Set license auth.
    ///
    /// Available since: 3.5.0
    /// Description: This function needs to be called to complete the configuration when using License authentication charges.
    /// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
    /// Restrictions: Does not support midway changes.
    /// Caution: None.
    ///
    /// @param license license.
    static void setLicense(const std::string &license) {
        ZegoExpressSDKInternal::setLicense(license);
    }

    /// Set room mode.
    ///
    /// Available since: 2.9.0
    /// Description: If you need to use the multi-room feature, please call this function to complete the configuration.
    /// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
    /// Restrictions: If you need to use the multi-room feature, please contact the instant technical support to configure the server support.
    /// Caution: None.
    ///
    /// @param mode Room mode. Description: Used to set the room mode. Use cases: If you need to enter multiple rooms at the same time for publish-play stream, please turn on the multi-room mode through this interface. Required: True. Default value: ZEGO_ROOM_MODE_SINGLE_ROOM.
    static void setRoomMode(ZegoRoomMode mode) { ZegoExpressSDKInternal::setRoomMode(mode); }

    /// Set Geo Fence.
    ///
    /// Available since: 3.4.0
    /// Description: If you need to use the geo fence feature, please call this function to complete the configuration.
    /// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
    /// Restrictions: If you need to use the geo fence feature, please contact ZEGO Technical Support.
    /// Caution: None.
    ///
    /// @param type Geo fence type. Description: Used to set the geo fence type.
    /// @param areaList Geo fence area. Description: Used to describe the range of geo fence.
    static void setGeoFence(ZegoGeoFenceType type, std::vector<int> areaList) {
        ZegoExpressSDKInternal::setGeoFence(type, areaList);
    }

    /// Gets the SDK's version number.
    ///
    /// Available since: 1.1.0
    /// Description: If you encounter an abnormality during the running of the SDK, you can submit the problem, log and other information to the ZEGO technical staff to locate and troubleshoot. Developers can also collect current SDK version information through this API, which is convenient for App operation statistics and related issues.
    /// When to call: Any time.
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @return SDK version.
    static std::string getVersion() { return ZegoExpressSDKInternal::getVersion(); }

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
    static void setAndroidEnv(void *jvm, void *context) {
        ZegoExpressSDKInternal::setAndroidEnv(jvm, context);
    }

    /// Set method execution result callback.
    ///
    /// Available since: 2.3.0
    /// Description: Set the setting of the execution result of the calling method. After setting, you can get the detailed information of the result of each execution of the ZEGO SDK method.
    /// When to call: Any time.
    /// Restrictions: None.
    /// Caution: It is recommended that developers call this interface only when they need to obtain the call results of each interface. For example, when troubleshooting and tracing problems. Developers generally do not need to pay attention to this interface.
    ///
    /// @param callback Method execution result callback.
    static void setApiCalledCallback(std::shared_ptr<IZegoApiCalledEventHandler> callback) {
        ZegoExpressSDKInternal::setApiCalledCallback(callback);
    }

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
    /// @return Whether the specified feature is supported. true: supported; false: not supported.
    static bool isFeatureSupported(ZegoFeatureType featureType) {
        return ZegoExpressSDKInternal::isFeatureSupported(featureType);
    }

    /// Show loading SDK dynamic library.
    ///
    /// Available since: 3.14.0
    /// Description: Show loading SDK dynamic library.
    /// When to call: When a dynamic SDK library needs to be loaded explicitly, the SDK is the first interface called. Other SDK interfaces can be called only after the loading is successful.
    /// Restrictions: When the SDK dynamic library needs to be loaded explicitly, this interface must be called first.
    /// Caution: When developers need to display the SDK dynamic library, they must add the preprocessing macro ZEGOEXP_EXPLICIT to the project to enable display loading..
    ///
    /// @param sdk_library_full_path SDK dynamic library absolute path.
    /// @return Error code.
    static int loadLibrary(const std::string &sdk_library_full_path) {
        return ZegoExpressSDKInternal::loadLibrary(sdk_library_full_path);
    }

    /// Unload SDK dynamic library.
    ///
    /// Available since: 3.14.0
    /// Description: Unload and load SDK dynamic library.
    /// When to call: When the SDK function is no longer needed, uninstall the SDK dynamic library. This interface is the last SDK interface called.
    /// Restrictions: When there is no need to use the SDK function, this interface must be called last.
    /// Caution: When developers need to display the SDK dynamic library, they must add the preprocessing macro ZEGOEXP_EXPLICIT to the project to enable display loading. When the SDK function is not needed, call this interface to unload the dynamic library.
    static void unLoadLibrary() { ZegoExpressSDKInternal::unLoadLibrary(); }

    /// Uploads logs to the ZEGO server.
    ///
    /// Available since: 3.7.0
    /// Description: By default, SDK creates and prints log files in the App's default directory. Each log file defaults to a maximum of 5MB. Three log files are written over and over in a circular fashion. When calling this function, SDK will auto package and upload the log files to the ZEGO server.
    /// Use cases: Developers can provide a business “feedback” channel in the App. When users feedback problems, they can call this function to upload the local log information of SDK to help locate user problems.
    /// When to call: None.
    /// Restrictions: The frequency limit is once per minute.
    /// Caution: 1.After calling this interface to upload logs, if you call [destroyEngine] or exit the App too quickly, there may be a failure.It is recommended to wait a few seconds, and then call [destroyEngine] or exit the App after receiving the upload success callback. 2.It is supported to call before [createEngine]. If it had called [createEngine] before, the last appid will be used to upload the log, otherwise the log will not be uploaded until the next [createEngine].
    static void submitLog() { ZegoExpressSDKInternal::submitLog(); }

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
    /// @param appSign Application signature for each AppID, please apply from the ZEGO Admin Console. Application signature is a 64 character string. Each character has a range of '0' ~ '9', 'a' ~ 'z'. AppSign 2.17.0 and later allows nullptr or no transmission. If the token is passed empty or not passed, the token must be entered in the [ZegoRoomConfig] parameter for authentication when the [loginRoom] interface is called to login to the room.
    /// @param isTestEnv [Deprecated] For providing better and more standardized services, starting from 2021-11-16, ZEGO no longer classifies environments into production environments and testing environments. f you create your project in ZEGO Admin Console on/before 2021-11-16, refer to [Testing environment deprecation](https://docs.zegocloud.com/article/13315) to upgrade the SDK and adjust related codes.
    /// @param scenario The room scenario. the SDK will optimize the audio and video configuration for the specified scenario to achieve the best effect in this scenario. After specifying the scenario, you can call other APIs to adjusting the audio and video configuration. Differences between scenarios and how to choose a suitable scenario, please refer to https://docs.zegocloud.com/article/14940
    /// @param eventHandler Event notification callback. [nullptr] means not receiving any callback notifications.It can also be managed later via [setEventHandler]. If [createEngine] is called repeatedly and the [destroyEngine] function is not called to destroy the engine before the second call, the eventHandler will not be updated.
    /// @return Engine singleton instance.
    ZEGO_DEPRECATED
    static IZegoExpressEngine *createEngine(unsigned int appID, const std::string &appSign,
                                            bool isTestEnv, ZegoScenario scenario,
                                            std::shared_ptr<IZegoEventHandler> eventHandler) {
        return ZegoExpressSDKInternal::createEngine(appID, appSign, isTestEnv, scenario,
                                                    eventHandler);
    }
};

} //namespace EXPRESS
} //namespace ZEGO

#endif //__ZEGOEXPRESSSDK_H__
