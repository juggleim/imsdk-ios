
#ifndef ZegoExpressErrorCode_h
#define ZegoExpressErrorCode_h

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, ZegoErrorCode) {

    /// Execution successful.
    ZegoErrorCodeCommonSuccess = 0,

    /// Description: The engine is not initialized and cannot call non-static functions. <br>Cause: Engine not created.<br>Solutions: Please call the [createEngine] function to create the engine first, and then call the current function.
    ZegoErrorCodeCommonEngineNotCreate = 1000001,

    /// Description: Not logged in to the room, unable to support function implementation. <br>Cause: Not logged in to the room.<br>Solutions: Please call [loginRoom] to log in to the room first, and use related functions during the online period after entering the room.
    ZegoErrorCodeCommonNotLoginRoom = 1000002,

    /// Description: The audio and video module of the engine is not started and cannot support function realization. <br>Cause: Audio and video modules with no engine started.<br>Solutions: Please call [startPreviewView] [startPublishingStream] [startPlayingStream] to start the audio and video module first.
    ZegoErrorCodeCommonEngineNotStarted = 1000003,

    /// Description: Call functions that are not supported on the current system/platform. <br>Cause: For example, calling the function of setting the Android context environment on a non-Android system.<br>Solutions: Check if the system environment matches.
    ZegoErrorCodeCommonUnsupportedPlatform = 1000006,

    /// Description: Invalid Android context. <br>Cause: Not set or set the wrong Android context environment.<br>Solutions: Set the correct Android context.
    ZegoErrorCodeCommonInvalidAndroidEnvironment = 1000007,

    /// Description: `setEventHandler` has been called to set the handler, please do not set it repeatedly. <br>Cause: Call `setEventHandler` repeatedly to set the handler.<br>Solutions: If you need to repeat the settings, please call `setEventHandler` first to empty the previous handler.
    ZegoErrorCodeCommonEventHandlerExists = 1000008,

    /// Description: The current SDK does not support this feature. <br>Cause: The SDK version used does not include this feature.<br>Solutions: Please contact ZEGO technical support.
    ZegoErrorCodeCommonSdkNoModule = 1000010,

    /// Description: The input streamID is too long. <br>Cause: The length of the streamID parameter passed in when calling [startPublishingStream] or [startPlayingStream] exceeds the limit. <br>Solutions: The streamID should be less than 256 bytes.
    ZegoErrorCodeCommonStreamIdTooLong = 1000014,

    /// Description: The input StreamID is null. <br>Cause: The streamID parameter passed in when calling [startPublishingStream] or [startPlayingStream] is null or empty string. <br>Solutions: Check whether the streamID parameter passed in when calling the function is normal.
    ZegoErrorCodeCommonStreamIdNull = 1000015,

    /// Description: The input streamID contains invalid characters. <br>Cause: The streamID parameter passed in when calling [startPublishingStream] or [startPlayingStream] contains invalid characters. <br>Solutions: Check whether the streamID parameter passed in when calling the function is normal, only support numbers, English characters and '-', '_'.
    ZegoErrorCodeCommonStreamIdInvalidCharacter = 1000016,

    /// Description: Illegal param.<br>Cause: The parameter is empty, or the parameter is an illegal value. <br>Solutions: Please check param when entered function to make sure it is correct.
    ZegoErrorCodeCommonIllegalParam = 1000017,

    /// Description: The Input CDN URL is too long. <br>Cause: The length of URL parameter passed in when calling [enablePublishDirectToCDN] or [startPlayingStream] exceeds the limit. <br>Solutions: URL length should be less than 1024 bytes.
    ZegoErrorCodeCommonCdnUrlTooLong = 1000018,

    /// Description: The Input CDN auth param is too long. <br>Cause: The length of auth parameter passed in when calling [enablePublishDirectToCDN] or [startPlayingStream] exceeds the limit. <br>Solutions: Auth param length should be less than 1024 bytes.
    ZegoErrorCodeCommonCdnAuthParamTooLong = 1000019,

    /// Description: When login room or login scene, userID or user name is different. <br>Cause: Login room, login scene use different user id or user name. <br> Solutions: Use same user id and user name when login room, login scene.
    ZegoErrorCodeCommonUserNotSame = 1000020,

    /// Description: This AppID has been removed from production. <br>Solutions: Please check the status of the AppID on the ZEGO official website console or contact ZEGO technical support.
    ZegoErrorCodeCommonAppOfflineError = 1000037,

    /// Description: The backend configuration of the server is incorrect. <br>Cause: 1. The domain name configuration is incorrect; 2. The media network is abnormal; 3. The media network link is empty. <br>Solutions: Please contact ZEGO technical support.
    ZegoErrorCodeCommonAppFlexiableConfigError = 1000038,

    /// Description: Server access denied. <br>Cause: 1.The APP has enabled the restriction of access from foreign IP addresses, and the current client is outside of the domain. <br>Solutions: Please contact ZEGO technical support.
    ZegoErrorCodeCommonAppAccessDeniedError = 1000039,

    /// Description: Incorrect CDN address. <br>Cause: The set CDN URL is not a standard RTMP or FLV protocol. <br>Solutions: Please check the supported protocol and format.
    ZegoErrorCodeCommonCdnUrlInvalid = 1000055,

    /// DNS resolution failed. Please check network configurations. This error code is deprecated.
    ZegoErrorCodeCommonDnsResolveError = 1000060,

    /// Server dispatching exception. Please contact ZEGO technical support to solve the problem. This error code is deprecated.
    ZegoErrorCodeCommonDispatchError = 1000065,

    /// Description: The engine audio and video module has been activated, and this setting is not supported. <br>Cause: Only supports setting before starting the audio and video module of the engine. <br>Solutions: Please set it before calling [startPreviewView] [startPublishingStream] [startPlayingStream] to start the audio and video module.
    ZegoErrorCodeCommonConfigAfterEngineStarted = 1000066,

    /// Description: The room is logged in, this setting is not supported. <br>Cause: Only supports setting before logging into the room. <br>Solutions: Please set it before calling [loginRoom] or after calling [logoutRoom]. Note that if you log in to multiple rooms, you need to log out of all rooms before setting.
    ZegoErrorCodeCommonConfigAfterRoomLoggedIn = 1000067,

    /// Description: Failed to load SDK dynamic library. <br>Cause: Incorrect SDK dynamic path passed in. <br>Solutions: Please pass in the correct SDK dynamic library path.
    ZegoErrorCodeCommonLoadLibraryFailed = 1000070,

    /// Description: Failed to obtain SDK export function when loading SDK dynamic library. <br>Cause: SDK dynamic library version does not match. <br>Solutions: Please load the matching version of the SDK dynamic library.
    ZegoErrorCodeCommonLoadLibraryFuncNotFound = 1000071,

    /// Description: This call is not supported. <br>Cause: ZEGOEXP_EXPLICIT macro is not enabled. <br>Solutions: Please add the ZEGOEXP_EXPLICIT macro to the preprocessor.
    ZegoErrorCodeCommonLoadLibraryNotSupport = 1000072,

    /// Description: SDK internal null pointer error. <br>Cause: The Android JVM environment is abnormal. <br>Solutions: Please check whether the Android JVM environment is normal or contact ZEGO technical support.
    ZegoErrorCodeCommonInnerNullptr = 1000090,

    /// AppID cannot be 0. Please check if the AppID is correct.
    ZegoErrorCodeEngineAppidZero = 1001000,

    /// The length of the input AppSign must be 64 bytes.
    ZegoErrorCodeEngineAppsignInvalidLength = 1001001,

    /// The input AppSign contains invalid characters. Only '0'-'9', 'a'-'f', 'A'-'F' are valid.
    ZegoErrorCodeEngineAppsignInvalidCharacter = 1001002,

    /// The input AppSign is empty.
    ZegoErrorCodeEngineAppsignNull = 1001003,

    /// Description: Authentication failed. <br>Cause: Incorrect AppID; using AppID in wrong environment. <br>Solutions: Please check AppID is correct or not on ZEGO manage console, or check whether the environment configured by AppID is consistent with the environment set by SDK.
    ZegoErrorCodeEngineAppidIncorrectOrNotOnline = 1001004,

    /// Description: Authentication failed. <br>Cause: Incorrect AppSign. <br>Solutions: Please check AppSign is correct or not on ZEGO manage console.
    ZegoErrorCodeEngineAppsignIncorrect = 1001005,

    /// The input License is empty.
    ZegoErrorCodeEngineLicenseNull = 1001006,

    /// Description: No write permission to the log file. <br>Cause: App has no write permission to log file folder. <br>Solutions: Please check has grant write permission to App or not; check log folder is protected or not.
    ZegoErrorCodeEngineLogNoWritePermission = 1001014,

    /// Description: The log file path is too long. <br>Cause: The length of log file path exceeds limit. <br>Solutions: Please check the length of log file path.
    ZegoErrorCodeEngineLogPathTooLong = 1001015,

    /// Description: Set room mode failed. <br>Cause: Set room mode after initialize the SDK. <br>Solutions: Please set room mode before initialize the SDK.
    ZegoErrorCodeEngineSetRoomModeErrorTime = 1001020,

    /// Description: Set geo fence failed. <br>Cause: Set geo fence after initialize the SDK. <br>Solutions: Please set geo fence before initialize the SDK.
    ZegoErrorCodeEngineSetGeoFenceErrorTime = 1001021,

    /// Description: The experimental API json parameter parsing failed. <br>Cause: Invalid json format; wrong function name or parameter. <br>Solutions: Please check json format is valid or not; check function name or parameter is correct or not, contact ZEGO technical support for specific function name and parameters.
    ZegoErrorCodeEngineExperimentalJsonStrInvalid = 1001091,

    /// Description: The number of rooms the user attempted to log into simultaneously exceeds the maximum number allowed. Currently, a user can only be logged in to one main room.<br>Cause: In single-room mode, log in to multiple main rooms at the same time (including repeated calls to log in to the same room A without exiting room A). <br>Solutions: Please check is login multiple rooms simultaneously or not under single room mode.
    ZegoErrorCodeRoomCountExceed = 1002001,

    /// Description: Haven't login with the input room ID.<br>Cause: Haven't login with the input room ID before call [logoutRoom] or [switchRoom] or [renewToken] or [setRoomExtraInfo]. <br>Solutions: Please check has login with the room ID or not.
    ZegoErrorCodeRoomRoomidIncorrect = 1002002,

    /// Description: The input user ID is empty.<br>Cause: The input user ID is empty. <br>Solutions: Please check the input user ID is empty or not.
    ZegoErrorCodeRoomUserIdNull = 1002005,

    /// Description: The input user ID contains invalid characters.<br>Cause: The input user ID contains invalid characters. <br>Solutions: User ID can only contains numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', ',', '.', '<', '>', '\'.
    ZegoErrorCodeRoomUserIdInvalidCharacter = 1002006,

    /// The input user ID is too long. <br>The length of the user ID input by the [loginRoom] function is greater than or equal to 64 bytes. <br>Please check the user ID entered when calling the [loginRoom] function to ensure that its length is less than 64 bytes.
    ZegoErrorCodeRoomUserIdTooLong = 1002007,

    /// The input user name is empty. <br>The user name entered by the [loginRoom] function is empty. <br>Please check the user name entered when calling the [loginRoom] function to make sure it is not empty.
    ZegoErrorCodeRoomUserNameNull = 1002008,

    /// The input user name contains invalid characters. <br>The user name entered by the [loginRoom] function contains illegal characters.<br>Please check the user name entered when calling the [loginRoom] function to ensure that it is only contain numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', ',', '.', '<', '>', '/', '\'.
    ZegoErrorCodeRoomUserNameInvalidCharacter = 1002009,

    /// The input user name is too long. <br>The length of the user name input by the [loginRoom] function is greater than or equal to 256 bytes. <br>Please check the user name entered when calling the [loginRoom] function to ensure that its length is less than 256 bytes.
    ZegoErrorCodeRoomUserNameTooLong = 1002010,

    /// The input room ID is empty. <br>The room ID entered by the [loginRoom] function is empty. <br>Please check the room ID entered when calling the [loginRoom] function to make sure it is not empty.
    ZegoErrorCodeRoomRoomidNull = 1002011,

    /// The input room ID contains invalid characters. <br>The room ID entered by the [loginRoom] function contains illegal characters.<br>Please check the room ID entered when calling the [loginRoom] function to ensure that it is only contain numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', ',', '.', '<', '>', '\'.
    ZegoErrorCodeRoomRoomidInvalidCharacter = 1002012,

    /// The input room ID is too long. <br>The length of the room ID input by the [loginRoom] function is greater than or equal to 128 bytes. <br>Please check the room ID entered when calling the [loginRoom] function to ensure that its length is less than 128 bytes.
    ZegoErrorCodeRoomRoomidTooLong = 1002013,

    /// The key for room extra info is empty. <br>The key for room extra info entered by the [setRoomExtraInfo] function is empty. <br>Please check the key for room extra info entered when calling the [setRoomExtraInfo] function to make sure it is not empty.
    ZegoErrorCodeRoomRoomExtraInfoKeyEmpty = 1002014,

    /// The key for room extra info is too long. <br>The length of the key for room extra info input by the [setRoomExtraInfo] function is greater than or equal to 128 bytes. <br>Please check the key for room extra info entered when calling the [setRoomExtraInfo] function to ensure that its length is less than 128 bytes.
    ZegoErrorCodeRoomRoomExtraInfoKeyTooLong = 1002015,

    /// The value for room extra info is too long. <br>The length of the value for room extra info input by the [setRoomExtraInfo] function is greater than or equal to 128 bytes. <br>Please check the value for room extra info entered when calling the [setRoomExtraInfo] function to ensure that its length is less than 128 bytes.
    ZegoErrorCodeRoomRoomExtraInfoValueTooLong = 1002016,

    /// Description: The number of keys set in the room additional message exceeds the maximum number of supported limits. <br>Cause: called setRoomExtraInfo Different keys have been passed in multiple times. <br> Solutions: please contact ZEGO technical support.
    ZegoErrorCodeRoomRoomExtraInfoExceedKeys = 1002017,

    /// Description: set multi room mode, userID or user name is different. <br>Cause: set multi room mode, login multi room use different user id or user name. <br> Solutions: Currently supports at most one key, if you need to support multiple, contact ZEGO technical support.
    ZegoErrorCodeRoomMultiRoomLoginUserNotSame = 1002018,

    /// Description: The [switchRoom] function cannot be used in multi-room mode. <br>Cause: multi room mode SDK not support. <br> Solutions: first call [logoutRoom] then call [loginRoom].
    ZegoErrorCodeRoomMultiRoomSwtichRoomInvalid = 1002019,

    /// Description: Login failed, possibly due to network problems. <br>Cause: The current network is abnormal. <br> Solutions: It is recommended to switch the network to try.
    ZegoErrorCodeRoomErrorConnectFailed = 1002030,

    /// Description: Login timed out, possibly due to network problems. <br>Cause: The current network delay is large. <br> Solutions: It is recommended to switch the network to try.
    ZegoErrorCodeRoomErrorLoginTimeout = 1002031,

    /// Description: Room login authentication failed. <br>Cause: login set token error or token expired. <br> Solutions: set new token.
    ZegoErrorCodeRoomErrorAuthenticationFailed = 1002033,

    /// Description: The number of users logging into the room exceeds the maximum number of concurrent users configured for the room. (In the test environment, the default maximum number of users in the room is 50). <br>Cause: too much user in room. <br> Solutions: contact ZEGO technical support.
    ZegoErrorCodeRoomErrorExceedMaximumMember = 1002034,

    /// Description: in test environment The total number of rooms logged in at the same time exceeds the limit. (In the test environment, the maximum number of concurrent rooms is 10). <br>Cause: Too many rooms logged in at the same time. <br> Solutions: logout some room, login room.
    ZegoErrorCodeRoomErrorExceedMaximumRoomCount = 1002035,

    /// Description: login failed, multi-room mode is not activate. <br>Cause: multi-room mode is not activate. <br> Solutions: please contact ZEGO technical support.
    ZegoErrorCodeRoomErrorLoginMultiRoomNotOpen = 1002036,

    /// The total number of rooms logged in at the same time exceeds the limit, Please contact ZEGO technical support.
    ZegoErrorCodeRoomErrorMultiRoomExceedMaximumRoomCount = 1002037,

    /// Description: The user was kicked out of the room. <br>Cause: Possibly because the same user ID is logged in on another device. <br>Solutions: Use a unique user ID.
    ZegoErrorCodeRoomKickedOut = 1002050,

    /// Description: Room connection is temporarily interrupted and is retrying. <br>Cause: Possibly due to network problems. <br>Solutions: Please wait or check whether the network is normal.
    ZegoErrorCodeRoomConnectTemporaryBroken = 1002051,

    /// Description: Room connection disconnected. <br>Cause: Possibly due to network problems. <br>Solutions: Please check whether the network is working or switch the network environment.
    ZegoErrorCodeRoomDisconnect = 1002052,

    /// Description: Room login retry has exceeded the maximum retry time. <br>Cause: Possibly due to network problems. <br>Solutions: Please check whether the network is working or switch the network environment.
    ZegoErrorCodeRoomRetryTimeout = 1002053,

    /// Description: The business server has sent a signal to kick the user out of the room. <br>Cause: The business server has sent a signal to kick the user out of the room. <br>Solutions: Contact the business server for processing.
    ZegoErrorCodeRoomManualKickedOut = 1002055,

    /// Description: The business server has sent a signal to kick the user out of the room. <br>Cause: The business server has sent a signal to kick the user out of the room because the token expired. <br>Solutions: Please update the token in time after receiving the callback notification that the token is about to expire.
    ZegoErrorCodeRoomTokenExpiredKickedOut = 1002056,

    /// Description: The business server has sent a signal to kick the user out of the room. <br>Cause: The business server has sent a signal to kick the user out of the room because the internal session was abnormal. <br>Solutions: Contact the business server for processing.
    ZegoErrorCodeRoomSessionExceptionKickedOut = 1002057,

    /// Description: Wrong order of login rooms. <br>Cause: Log in multi room without log in the main room. <br>Solutions: Please log in to the main room with `loginRoom` before logging in to multi room.
    ZegoErrorCodeRoomWrongLoginSequence = 1002061,

    /// Description: Wrong order of logout rooms. <br>Cause: Log out the main room without log out multi room. <br>Solutions: Please log out of the multi room before logging out of the main room.
    ZegoErrorCodeRoomWrongLogoutSequence = 1002062,

    /// Description: No multi-room permission. <br>Cause: No multi-room permission. <br>Solutions: Please contact ZEGO technical support to enable it.
    ZegoErrorCodeRoomNoMultiRoomPermission = 1002063,

    /// Description: The room ID has been used by other login room interface. Current user can not login room with the room ID before release the room ID. <br>Cause: The room ID has been used by other login room interface. <br>Solutions: Logout the room with the same room ID first.
    ZegoErrorCodeRoomRoomIdHasBeenUsed = 1002064,

    /// Description: This method has been deprecated after version 2.9.0. <br>Cause: This method has been deprecated after version 2.9.0. <br>Solutions: Please set [setRoomMode] to select multi-room mode before the engine started, and then call [loginRoom] to use multi-room.
    ZegoErrorCodeRoomMultiRoomDeprecated = 1002065,

    /// Description: If the user is in the server blacklist when logging in to the room, this error code will be returned, indicating that it is forbidden to log in to the room. <br>Cause: The user is currently in the server blacklist. <br>Solutions: Please contact ZEGO technical support.
    ZegoErrorCodeRoomUserInBlacklist = 1002066,

    /// Description: This error code is returned if the user does not pass [AppSign] when creating the engine or [Token] when logging in to the room. <br>Cause: The Token is not transmitted in the login room. <br>Solutions: Set the [Token] when you login to the room.
    ZegoErrorCodeRoomNoToken = 1002067,

    /// Description: The interface is called more frequently than the upper limit. <br>Cause: This application calls this interface too often. <br>Solutions: Please control the frequency of calls to this interface. Please refer to https://docs.zegocloud.com/article/7612 for details.
    ZegoErrorCodeRoomAppCallApiTooFrequent = 1002071,

    /// Description: The frequency of calls to the interface by users in the room exceeds the upper limit. <br>Cause: The interface is called too often by users in the room. <br>Solutions: Please control the frequency of calls to this interface by users in the room. Please refer to https://docs.zegocloud.com/article/7612 for details.
    ZegoErrorCodeRoomRoomCallApiTooFrequent = 1002072,

    /// Description: Room signalling type interfaces are called more frequently than the upper limit. <br>Cause: This application calls the room signalling type interface too often. (e.g., [sendCustomCommand] [sendBroadcastMessage]) <br>Solutions: Please control the frequency of application calls to the room signalling type interface. Please refer to https://docs.zegocloud.com/article/7612 for details.
    ZegoErrorCodeRoomAppCallTooFrequent = 1002073,

    /// Description: Token inner error. <br>Cause:  Unknown internal error. <br>Solutions: Contact ZEGO technical support to deal with it..
    ZegoErrorCodeRoomTokenInnerError = 1002074,

    /// Description: Token Format error. <br>Cause: The generated Token transmission parameter payload is in non-JSON format. <br>Solutions: The payload is correctly transmitted in json format when the Token is generated.
    ZegoErrorCodeRoomTokenFormatError = 1002075,

    /// Description: Token Appid error. <br>Cause: The generated Token Appid is inconsistent with the SDK used Appid, Or the appID type is incorrect. <br>Solutions: Use the Appid of the generated Token.
    ZegoErrorCodeRoomTokenAppidError = 1002076,

    /// Description: Token userID error. <br>Cause: The generated Token userID is inconsistent with the login used userID Or the userID type is incorrect. <br>Solutions: Login using the userID that generates the Token.
    ZegoErrorCodeRoomTokenUseridError = 1002077,

    /// Description: Token expire. <br>Cause: Token expire Or the generated Token validity period parameter type is incorrect. <br>Solutions: Regenerate the Token.
    ZegoErrorCodeRoomTokenExpireError = 1002078,

    /// Description: Token version error. <br>Cause: Description The Token Version was generated incorrectly. <br>Solutions: Generate the Token with the correct version.
    ZegoErrorCodeRoomTokenVersionError = 1002079,

    /// Description: The Token Nonce parameter type is incorrect. <br>Cause: The nonce parameter type of the generated Token is incorrect. <br>Solutions: Ensure that nonce is int64 data.
    ZegoErrorCodeRoomTokenNoncetypeError = 1002080,

    /// Description: Multi-room mode required. <br>Cause: Wrong room mode been used, e.g. single-room. <br>Solutions: Please use [setRoomMode] to select multi-room mode before the engine started.
    ZegoErrorCodeRoomMultiRoomRequired = 1002081,

    /// Description: Update room Token has no push permission. <br>Cause: Token Does not have the push permission. <br>Solutions: Pass in the Token with the push permission.
    ZegoErrorCodeRoomRenewTokenNoPublishAuthError = 1002082,

    /// Description: The length of the custom data attached during login exceeded the limit. Procedure. <br>Cause: The login custom callback data is too long. <br>Solutions: The length of user-defined login data cannot exceed 128 bytes.
    ZegoErrorCodeRoomRoomLoginCustomDataLengthError = 1002083,

    /// Description: login room used license auth, but can't find the license on zego server. <br>Cause: input wrong license. <br>Solutions: used the right license.
    ZegoErrorCodeRoomRoomLoginLicenseNotFound = 1002084,

    /// Description: login room used license auth, but the license is expired.  <br>Solutions:  Contact ZEGO technical support to deal with it.
    ZegoErrorCodeRoomRoomLoginLicenseExpired = 1002085,

    /// Description: The user has logged in elsewhere.  <br>Solutions:  Check whether the user has logged in elsewhere.
    ZegoErrorCodeRoomRoomLoginOther = 1002086,

    /// Description: License authentication is used, and when using the relevant function, the license is configured with a functional limitation that does not allow the use of the function.  <br>Solutions:  Proofread the license's functionality limitations and use the functionality appropriately.
    ZegoErrorCodeRoomRoomLicenseFeatureLimit = 1002087,

    /// Description: Room login failed due to internal system exceptions.<br>Cause: Unknown internal error.<br>Solutions: Contact ZEGO technical support to deal with it.
    ZegoErrorCodeRoomInnerError = 1002099,

    /// Description: Publishing failed due to no data in the stream.<br>Cause: No data in the stream.<br>Solutions: Check whether the video, audio capture module is working properly.
    ZegoErrorCodePublisherPublishStreamFailed = 1003001,

    /// Description: Publishing failed due to wrong bitrate setting.<br>Cause: The set video bitrate, audio bitrate, or minimum video bitrate threshold for traffic control exceeds the upper limit.<br>Solutions: Please check if the bitrate value is in the correct unit (kbps).Adjust the bitrate setting.
    ZegoErrorCodePublisherBitrateInvalid = 1003002,

    /// Description: The property param of the traffic control is set incorrectly.<br>Cause: The property param of the traffic control is less than 0 or exceeds all combinations.<br>Solutions: Check the settings of the property param of the traffic control.
    ZegoErrorCodePublisherTrafficModeInvalid = 1003005,

    /// Description: Streaming failed, H.265 encoding is not supported.<br>Cause: The hardware device does not support H.265 encoding, or the SDK does not include H.265 encoding module.<br>Solutions: Contact ZEGO technical support to confirm whether the SDK contains the H.265 encoding module, if the hardware device does not support it, it is recommended to upgrade the hardware.
    ZegoErrorCodePublisherErrorH265EncoderNotSupported = 1003010,

    /// Description: The performance of object segmentation device is not enough.<br>Caution: The performance of device is not enough.<br>Solutions: Please replace the device.
    ZegoErrorCodePublisherObjectSegmentationPerformanceWarning = 1003011,

    /// Description: This device does not support object segmentation.<br>Caution: The device is not supported, or the object segmentation of multiple channels is enabled.<br>Solutions: Please change the device or only open the object segmentation of one channel.
    ZegoErrorCodePublisherObjectSegmentationDeviceNotSupport = 1003012,

    /// Description: Object segmentation unauthorized.<br>Caution: The appid is not configured with object segmentation.<br>Solutions: Please contact ZEGO technical support to configure the object segmentation function.
    ZegoErrorCodePublisherObjectSegmentationNoAuth = 1003013,

    /// Description: Object segmentation parameter error.<br>Caution: Object segmentation is set to green screen type, background blur is not allowed.<br>Solutions: Please use the correct parameters to enable object segmentation.
    ZegoErrorCodePublisherObjectSegmentationParamError = 1003014,

    /// Description: Object segmentation background loading error.<br>Caution: The background video or image path is incorrect, or the content is incorrect..<br>Solutions: Please set the correct background image or video.
    ZegoErrorCodePublisherObjectSegmentationImageLoadError = 1003015,

    /// Description:Stream publishing is temporarily interrupted and is retrying. <br>Cause: The network fluctuates or the network signal is bad.<br>Solutions: Please wait or check whether the network is normal.
    ZegoErrorCodePublisherErrorNetworkInterrupt = 1003020,

    /// Description: Stream publish retry has exceeds the maximum retry time.<br>Cause: The the network signal is bad, and the maximum retry time is exceeded.<br>Solutions: Check the network status or switch to another network.
    ZegoErrorCodePublisherErrorRetryTimeout = 1003021,

    /// Description: Failed to publish the stream. The publish channel is already publishing streams.<br>Cause:  The publish channel is already publishing streams.<br>Solutions: Please check the business logic to avoid repeating the publish for publish channel which is publishing.
    ZegoErrorCodePublisherErrorAlreadyDoPublish = 1003023,

    /// Description: Failed to publish the stream. Publishing of this stream is prohibited by backend configuration.<br>Cause: Publishing of this stream is prohibited by backend configuration.<br>Solutions: Contact ZEGO technical support to deal with it.
    ZegoErrorCodePublisherErrorServerForbid = 1003025,

    /// Description: Failed to publish the stream. The same stream already exists in the room.<br>Cause: The same stream already exists in the room.<br>Solutions: Replace with a new stream ID. Adjust the stream ID generation strategy to ensure uniqueness.
    ZegoErrorCodePublisherErrorRepetitivePublishStream = 1003028,

    /// Description: Failed to publish the stream. The connection to the RTMP server is interrupted.<br>Cause: The publish address is wrong, or the network signal is bad.<br>Solutions: Please check whether there is any problem with the network connection or the stream publishing URL.
    ZegoErrorCodePublisherRtmpServerDisconnect = 1003029,

    /// Description: Failed to take a screenshot of the publis stream screen. <br>Cause: The preview is stopped and the push is abnormal. <br>Solutions: Turn on preview or re-publish.
    ZegoErrorCodePublisherTakePublishStreamSnapshotFailed = 1003030,

    /// Description: Failed to update the relay CDN status. <br>Cause: The URL of the relay address is incorrect. <br>Solutions: Check whether the input URL is valid.
    ZegoErrorCodePublisherUpdateCdnTargetError = 1003040,

    /// Description: Failed to send SEI. <br>Cause: data is empty. <br>Solutions: Incoming non-empty data.
    ZegoErrorCodePublisherSeiDataNull = 1003043,

    /// Description: Failed to send SEI. <br>Cause: The input data exceeds the length limit. <br>Solutions: The length of the sent SEI data should be less than 4096 bytes.
    ZegoErrorCodePublisherSeiDataTooLong = 1003044,

    /// Description: Failed to send audio side info. <br>Cause: data is empty. <br>Solutions: Incoming non-empty data.
    ZegoErrorCodePublisherAudioSideDataNull = 1003045,

    /// Description: Failed to send audio side info. <br>Cause: The input data exceeds the length limit. <br>Solutions: The length of the sent audio side data should be less than 1024 bytes.
    ZegoErrorCodePublisherAudioSideDataTooLong = 1003046,

    /// The extra info of the stream is null.
    ZegoErrorCodePublisherExtraInfoNull = 1003050,

    /// The extra info of the stream is too long. The maximum length should be less than 1024 bytes.
    ZegoErrorCodePublisherExtraInfoTooLong = 1003051,

    /// Failed to update the extra info of the stream. Please check the network connection.
    ZegoErrorCodePublisherUpdateExtraInfoFailed = 1003053,

    /// Description: Failed to set publish watermark. <br>Cause: The incoming watermark path is empty. <br>Solutions: Incoming non-empty path.
    ZegoErrorCodePublisherWatermarkUrlNull = 1003055,

    /// Description: Failed to set publish watermark. <br>Cause: The incoming watermark path exceeds the byte size limit. <br>Solutions: The incoming watermark path should be less than 512 bytes.
    ZegoErrorCodePublisherWatermarkUrlTooLong = 1003056,

    /// Description: Failed to set publish watermark. <br>Cause: The incoming watermark path was entered incorrectly or the image format is not supported. <br>Solutions: Incoming the correct watermark path, only `jpg` and `png` image formats are supported.
    ZegoErrorCodePublisherWatermarkUrlInvalid = 1003057,

    /// Description: Incorrect watermark layout.<br>Caution: The layout area exceed the encoding resolution.<br>Solutions: Make sure the layout area cannot exceed the encoding resolution and call current interface.
    ZegoErrorCodePublisherWatermarkLayoutInvalid = 1003058,

    /// Description: The publish stream encryption key is invalid.<br>Caution: The set encryption key length is not supported.<br>Solutions: The Publish-stream encryption key length to be 16/24/32 bytes.
    ZegoErrorCodePublisherEncryptionKeyInvalid = 1003060,

    /// Description: StartPlayingStream failed.<br>Caution: In multi-room mode, the publish-stream function is called incorrectly.<br>Solutions: In multi-room mode, A publish-stream function with the parameter 'ZegoPublisherConfig' needs to be called.
    ZegoErrorCodePublisherErrorPublishWhenUsingMultiRoom = 1003070,

    /// Description: StartPlayingStream failed.<br>Caution: In multi-room mode, the publish-stream function is called incorrectly.<br>Solutions: In multi-room mode, A publish-stream function parameter 'roomID' cannot be empty.
    ZegoErrorCodePublisherErrorPublishWithRoomIdIsNullWhenUsingMultiRoom = 1003071,

    /// Description: Push-pull flow authentication is incorrect. <br>Caution: An [appSign] error was passed when creating the engine, or a Token error or timeout was passed when logging into the room. <br>Solutions: Pass the correct [Token] upon login, or invoke [renewToken] when recive [onRoomTokenWillExpire] callback.
    ZegoErrorCodePublisherErrorDispatchAuthError = 1003072,

    /// Description: The specified path for displaying static images when the camera is turned off is incorrect. <br>Caution: The path may be misspelled or there may be no read permission.<br>Solutions: Please check if the designated image path is correct and if there is read permission.
    ZegoErrorCodePublisherDummyCaptureImagePathError = 1003073,

    /// Description: Unsupported video encoder.<br>Caution: There is no selected video encoder in the current SDK.<br>Solutions: Please contact ZEGO technical support.
    ZegoErrorCodePublisherVideoEncoderNoSupportted = 1003080,

    /// Description: Video encoder error.<br>Caution: Video encoder error.<br>Solutions: Please contact ZEGO technical support.
    ZegoErrorCodePublisherVideoEncoderFail = 1003081,

    /// Description: Stream publishing failed due to system internal exceptions.<br>Caution: Stream publishing failed due to system internal exceptions.<br>Solutions: Please contact ZEGO technical support to solve the problem.
    ZegoErrorCodePublisherInnerError = 1003099,

    /// Description: Stream playing failed.<br>Caution: Possibly due to no data in the stream.<br>Solutions: Check to see if the publish-stream is working or try to play stream again, and if the problem is still not resolved, please contact ZEGO technical support to solve the problem.
    ZegoErrorCodePlayerPlayStreamFailed = 1004001,

    /// Description: Stream playing failed.<br>Caution: The stream does not exist.<br>Solutions: Please check whether the remote end publish is indeed successful, or whether the publish and play environment are inconsistent.
    ZegoErrorCodePlayerPlayStreamNotExist = 1004002,

    /// Description: Play stream authentication is incorrect. <br>Caution: The [Token] error or timeout required to playing stream across APP. <br>Solutions: Playing the stream to pass the correct [Token], or update the [Token] in time.
    ZegoErrorCodePlayerErrorDispatchAuthError = 1004003,

    /// Description: This device does not support super resolution. <br>Caution: The device configured on the server does not support super resolution, or the configuration on the server failed to pull. <br>Solutions: replace the device and try again or contact ZEGO technical support.
    ZegoErrorCodePlayerSuperResolutionDeviceNotSupport = 1004004,

    /// Description: The number of super resolution streams exceeds the limit.<br>Caution: The number of super resolution streams exceeds the limit.<br>Solutions: Currently, super resolution support up to 1 playing steam at the same time.
    ZegoErrorCodePlayerSuperResolutionMaxCountExceed = 1004005,

    /// Description: The number of super resolution streams exceeds the limit.<br>Caution: The number of super resolution streams exceeds the limit.<br>Solutions: contact ZEGO technical support.
    ZegoErrorCodePlayerSuperResolutionVideoSizeNotSupport = 1004006,

    /// Description: The performance of super resolution device is not enough.<br>Caution: The performance of device is not enough.<br>Solutions: Please replace the device.
    ZegoErrorCodePlayerSuperResolutionDevicePerformanceError = 1004007,

    /// Description: Super resolution uninitialized.<br>Caution: Super resolution uninitialized.<br>Solutions: Please initialize super resolution.
    ZegoErrorCodePlayerSuperResolutionNotInitError = 1004008,

    /// Description: Stream playing error.<br>Caution: The number of streams the user attempted to play simultaneously exceeds the maximum number allowed.<br>Solutions: Currently, up to 12 steams can be played at the same time. Please contact ZEGO technical support to increase the capacity if necessary.
    ZegoErrorCodePlayerCountExceed = 1004010,

    /// Description: The stream was not played.<br>Caution: The stream was not played.<br>Solutions: Please confirm that the stream has been played through the [onPlayerStateUpdate] or [onUserStreamStateUpdate] callback.
    ZegoErrorCodePlayerStreamWasNotPlayed = 1004011,

    /// Description: Stream playing is temporarily interrupted.<br>Caution: Network exception.<br>Solutions: Please wait or check whether the network is normal.
    ZegoErrorCodePlayerErrorNetworkInterrupt = 1004020,

    /// Description: Failed to play the stream.<br>Caution: Publishing of this stream is prohibited by backend configuration.<br>Solutions: Please contact ZEGO technical support to solve the problem.
    ZegoErrorCodePlayerErrorServerForbid = 1004025,

    /// Description: Failed to capture the screenshot of the streaming screen, please check whether the stream to be captured is normal. <br>Cause: The stream is not pulled. <br>Solutions: Check whether it starts to play the stream, and whether there is an abnormality in the process of playing the stream.
    ZegoErrorCodePlayerTakePlayStreamSnapshotFailed = 1004030,

    /// Description: The play stream decryption key is invalid, the key length only supports 16/24/32 bytes. <br>Cause: The input key length is not 16/24/32 bytes. <br>Solutions: Adjust the input key length to 16/24/32 bytes.
    ZegoErrorCodePlayerDecryptionKeyInvalid = 1004060,

    /// Description: Pull stream decryption failed, please check whether the decryption key is correct. <br>Cause: Incorrect decryption key entered. <br>Solutions: Enter the correct decryption key.
    ZegoErrorCodePlayerDecryptionFailed = 1004061,

    /// Description: Calling the wrong function after enabling the multi-room function causes playing stream fail. <br>Cause: Called the playing stream function that only works for joining a single room mode. <br>Solutions: Please use the function of the same name with ZegoPlayerConfig and specify the room ID to play the stream.
    ZegoErrorCodePlayerErrorPlayStreamWhenUsingMultiRoom = 1004070,

    /// Description: In the multi-room mode, the roomID parameter of the play stream cannot be empty. <br>Cause: The roomID parameter of the pull stream is empty. <br>Solutions: Please enter the correct roomID.
    ZegoErrorCodePlayerErrorPlayStreamWithRoomIdIsNullWhenUsingMultiRoom = 1004071,

    /// Description: When using the SDK to play the latency of live streaming, this error code will be returned if you have not subscribed to the low latency live streaming service. <br>Cause: Low-latency live broadcast service is not activated. <br>Solutions: Please contact ZEGO technical support staff to open the low-latency live broadcast service.
    ZegoErrorCodePlayerNotConfigL3 = 1004072,

    /// Description: Unsupported video decoder.<br>Caution: There is no selected video decoder in the current SDK.<br>Solutions: Please contact ZEGO technical support.
    ZegoErrorCodePlayerVideoDecoderNoSupportted = 1004080,

    /// Description: Video decoder fail.<br>Caution: Video decoder fail.<br>Solutions: Please contact ZEGO technical support.
    ZegoErrorCodePlayerVideoDecoderFail = 1004081,

    /// Description: An internal system exception causes a failure to pull the stream. <br>Cause: SDK internal error. <br>Solutions: Please contact ZEGO technical support.
    ZegoErrorCodePlayerInnerError = 1004099,

    /// Description: Does not support the use of stream mixing service. <br>Cause: No stream mixing service configured. <br>Solutions: Please open the service on the console or contact ZEGO business staff.
    ZegoErrorCodeMixerNoServices = 1005000,

    /// Description: The mixing task ID is null. <br>Cause: The mixing task ID input when starting mixing task is empty. <br>Solutions: Please enter the correct mixing task ID.
    ZegoErrorCodeMixerTaskIdNull = 1005001,

    /// Description: The stream mixing task ID is too long. <br>Cause: The stream mixing task ID is greater than 256 bytes. <br>Solutions: Please enter a mixing task ID less than 256 bytes.
    ZegoErrorCodeMixerTaskIdTooLong = 1005002,

    /// Description: Invalid mixed flow task ID. <br>Cause: Illegal characters in the stream mixing task ID. <br>Solutions: Stream mixing task ID only supports numbers, English characters and'~','!','@','$','%','^','&','*','(',')', '_','+','=','-','`',';',''',',','.','<','>','/','\', please enter the stream mixing task ID in the correct format.
    ZegoErrorCodeMixerTaskIdInvalidCharacter = 1005003,

    /// Description: Illegal parameters exist in mixing task configuration. <br>Cause: 1. The mixing task ID is empty; 2. The mixing room ID is empty; 3. The mixing custom data length exceeds 1000 bytes; 4. The mixing output target stream is empty. <br>Solutions: Please check the configuration parameters of the mixing task.
    ZegoErrorCodeMixerNoOutputTarget = 1005005,

    /// Description: Illegal format of mixed stream output target parameter. <br>Cause: When the target of the mixed stream output target is streamID, an illegal character is passed in. <br>Solutions: Please check whether the target of the mixed stream output target is of streamID type, if so, target only support numbers, English characters and '-', '_'.
    ZegoErrorCodeMixerOutputTargetInvalid = 1005006,

    /// Description: Failed to start the stream mixing task. <br>Cause: Requests are too frequent, exceeding the qps limit of the service. Network exceptions. An illegal mixed stream output URL was passed in. <br>Solutions: Please ensure that the qps of the mixing request is less than 100. Check the network. Pass in a valid mixed stream output URL.
    ZegoErrorCodeMixerStartRequestError = 1005010,

    /// Description: Failed to stop the stream mixing task. <br>Cause: May be the cause of the network error. <br>Solutions: Please check the network ring.
    ZegoErrorCodeMixerStopRequestError = 1005011,

    /// The stream mixing task must be stopped by the user who started the task. This error code is deprecated.
    ZegoErrorCodeMixerNotOwnerStopMixer = 1005012,

    /// Description: Starts stream mixing tasks too frequently. <br>Cause: Requests are too frequent, exceeding the qps limit of the service. <br>Solutions: Please ensure that the qps of the mixing request is less than 100.
    ZegoErrorCodeMixerStartQpsOverload = 1005015,

    /// Description: Stop stream mixing tasks too frequently. <br>Cause: Requests are too frequent, exceeding the qps limit of the service. <br>Solutions: Please ensure that the qps of the stop mixing request is less than 100.
    ZegoErrorCodeMixerStopQpsOverload = 1005016,

    /// Description: The input stream list of the stream mixing task is empty. <br>Cause:  The input stream list of the stream mixing task is empty. <br>Solutions: Please check the input stream list of the mixing task.
    ZegoErrorCodeMixerInputListInvalid = 1005020,

    /// Description: The output stream list of the stream mixing task is empty. <br>Cause:  The output stream list of the stream mixing task is empty. <br>Solutions: Please check the output stream list of the mixing task.
    ZegoErrorCodeMixerOutputListInvalid = 1005021,

    /// Description: The video configuration of the stream mixing task is invalid. <br>Cause:  The video configuration of the stream mixing task is invalid. <br>Solutions: Please check the video configuration of the stream mixing task.
    ZegoErrorCodeMixerVideoConfigInvalid = 1005023,

    /// Description: The video configuration of the stream mixing task is invalid. <br>Cause: 1. An unsupported audio codec format is used. 2. The audio bit rate exceeds 192 kbps. <br>Solutions: Please check the audio configuration of the stream mixing task.
    ZegoErrorCodeMixerAudioConfigInvalid = 1005024,

    /// Description: The number of input streams exceeds the maximum number allowed. <br>Cause: Supports up to 9 input streams, and may pass more than 9 input streams. <br>Solutions: Please check the input stream configuration of the mixing task.
    ZegoErrorCodeMixerExceedMaxInputCount = 1005025,

    /// Description: Failed to start mixed stream. <br>Cause: The input stream does not exist. <br>Solutions: Please make sure that the stream corresponding to the entered streamID is being pushed.
    ZegoErrorCodeMixerInputStreamNotExists = 1005026,

    /// Description: Failed to start mixed stream. <br>Cause: The mixed stream input parameter is wrong, it may be that the layout of the input stream exceeds the canvas range. <br>Solutions: Please enter the correct mixed stream parameters. [ZegoMixerTask]
    ZegoErrorCodeMixerInputParametersError = 1005027,

    /// Description: Mixed input text watermark is too long. <br>Cause: The length of the text watermark of the mixed stream input parameter exceeds the limit. <br>Solutions: Please make sure the input text watermark length does not exceed 512 bytes.
    ZegoErrorCodeMixerInputLabelTextTooLong = 1005028,

    /// Description: Mixed stream output target is too long. <br>Cause: The length of the target parameter of the mixed stream output exceeds the limit. <br>Solutions: Please make sure that the output destination length does less than 1024 bytes.
    ZegoErrorCodeMixerOutputTargetTooLongError = 1005029,

    /// Description: Failed to start mixed stream. <br>Cause: Exceeding the maximum number of output streams. <br>Solutions: Support up to 3 output streams.
    ZegoErrorCodeMixerExceedMaxOutputCount = 1005030,

    /// Description: Failed to start mixed stream. <br>Cause: The maximum number of focus voice input streams is exceeded. <br>Solutions: Support up to 4 input streams to set the focus voice.
    ZegoErrorCodeMixerExceedMaxAudioFocusStreamCount = 1005031,

    /// Description: The mixed-flow advanced configuration is too long. <br>Cause: The length of the mixed stream advanced configuration parameter exceeds the limit. <br>Solutions: Please make sure that the advanced configuration length does not exceed 512 bytes.
    ZegoErrorCodeMixerAdvancedConfigTooLongError = 1005032,

    /// Description: The mixed stream watermark path is too long. <br>Cause: The length of the mixed stream watermark path parameter exceeds the limit. <br>Solution: Please make sure the watermark path length does not exceed 512 bytes.
    ZegoErrorCodeMixerWatermarkTooLong = 1005033,

    /// Description: The link to the muxed input image is too long. <br>Cause: The length of the image link of the mixed stream input parameter exceeds the limit. <br>Solution: Please make sure that the length of the input image link does not exceed 1024 bytes.
    ZegoErrorCodeMixerInputImageUrlTooLong = 1005034,

    /// Description: Failed to mix stream input image. <br>Cause: The image format of the mixed stream input parameter is incorrect. <br>Solution: Use JPG and PNG formats. There are 2 ways to use it: 1. URI: Provide the picture to ZEGO technical support for configuration. After the configuration is complete, the picture URI will be provided, for example: preset-id://xxx.jpg. 2. URL: Only HTTP protocol is supported.
    ZegoErrorCodeMixerInputImageUrlFormatError = 1005035,

    /// Description: Failed to mux input image. <br>Cause: The image size of the mixed stream input parameter exceeds the limit. <br>Solution: Image size is limited to 1M.
    ZegoErrorCodeMixerInputImageUrlSizeError = 1005036,

    /// Description: The mixed output stream target room is misconfigured. <br>Cause: The room doesn't exist when the mixing starts. <br>Solution: Please make sure that the room corresponding to the configured targetRoom exists.
    ZegoErrorCodeMixerOutputTargetRoomError = 1005037,

    /// Description: Failed to start mixed stream. <br>Cause: Mixed-stream authentication failed. <br>Solutions: Contact ZEGO technical support.
    ZegoErrorCodeMixerAuthenticationFailed = 1005050,

    /// Description: Failed to start mixed stream. <br>Cause: The input image watermark is empty. <br>Solutions: Please enter the correct watermark parameters [ZegoWatermark].
    ZegoErrorCodeMixerWatermarkNull = 1005061,

    /// Description: Failed to start mixed stream. <br>Cause: The input image watermark parameter is wrong, it may be that the layout exceeds the canvas range. <br>Solutions: Please enter the correct watermark parameters [ZegoWatermark].
    ZegoErrorCodeMixerWatermarkParametersError = 1005062,

    /// Description: Failed to start mixed stream. <br>Cause: The input watermark URL is illegal. <br>Solutions: The watermark URL must start with `preset-id://` and end with `.jpg` or `.png`.
    ZegoErrorCodeMixerWatermarkUrlInvalid = 1005063,

    /// Description: The mixed stream background image url is too long. <br>Cause: The length of the mixed stream background url parameter exceeds the limit. <br>Solution: Please make sure the background image url length does not exceed 1024 bytes.
    ZegoErrorCodeMixerBackgroundImageUrlTooLong = 1005066,

    /// Description: Failed to start mixed stream. <br>Cause: The URL of the background image entered is illegal. <br>Solutions: The URL of the background image must start with preset-id:// and end with `.jpg` or `.png`.
    ZegoErrorCodeMixerBackgroundImageUrlInvalid = 1005067,

    /// Description: Failed to start mixed stream. <br>Cause: The user-defined data is too long. <br>Solutions: The maximum length of the custom input should not exceed 1000 bytes.
    ZegoErrorCodeMixerUserDataTooLong = 1005068,

    /// Description: Failed to start mixed stream. <br>Cause: The auto-mixing server was not found. <br>Solutions: Please contact ZEGO technical support.
    ZegoErrorCodeMixerAutoMixStreamServerNotFound = 1005070,

    /// Description: Stream mixing internal error.<br>Cause: Unknown error occured in stream mixing internal.<br>Solutions: Please contact ZEGO technical support.
    ZegoErrorCodeMixerInnerError = 1005099,

    /// Description: Generic device error.<br>Cause: Device dose not work normally.<br>Solutions: Use the system's video or audio recording application to check whether the device can work normally. If the device is normal, please contact ZEGO technical support.
    ZegoErrorCodeDeviceErrorTypeGeneric = 1006001,

    /// Description: The device ID does not exist.<br>Cause: The device ID is spelled incorrectly, or the corresponding device is unplugged.<br>Solutions: Please use the SDK interface to obtain the device ID, and check whether the device is properly connected.
    ZegoErrorCodeDeviceErrorTypeInvalidId = 1006002,

    /// Description: No permission to access the device. <br>Cause: Did not apply for or obtain the permission to use the corresponding device.<br>Solutions: Please check whether the application has correctly applied for the camera or microphone permission, and whether the user has granted the corresponding permission.
    ZegoErrorCodeDeviceErrorTypeNoAuthorization = 1006003,

    /// Description: The frame rate of the capture device is 0.<br>Cause: Device error, or device does not have permission.<br>Solutions: Please use the system's video or audio recording application to check whether the device can work normally. Please check whether the application has correctly applied for the camera or microphone permission, and whether the user has granted the corresponding permission. If the device is normal and the application has obtained the corresponding device permissions, please contact ZEGO technical support.
    ZegoErrorCodeDeviceErrorTypeZeroFps = 1006004,

    /// Description: The device is occupied.<br>Cause: The device is occupied by other programs.<br>Solutions: Please use the system's video or audio recording application to check whether the device is working properly and make sure that the device is not occupied by other applications.
    ZegoErrorCodeDeviceErrorTypeInUseByOther = 1006005,

    /// Description: The device is unplugged.<br>Cause: The device is unplugged or not properly connected.<br>Solutions: Check the device wiring and reconnect the device.
    ZegoErrorCodeDeviceErrorTypeUnplugged = 1006006,

    /// Description: The device needs to be restarted.<br>Cause: Device driver update, or device error requires restart.<br>Solutions: Restart device.
    ZegoErrorCodeDeviceErrorTypeRebootRequired = 1006007,

    /// Description: The device media is lost.<br>Cause: Media service cannot be restored.<br>Solutions: Restart device.
    ZegoErrorCodeDeviceErrorMediaServicesLost = 1006008,

    /// Description: The device list cannot be empty when trying to release devices.<br>Cause: The device list has been released or has not been initialized.<br>Solutions: Ignore it.
    ZegoErrorCodeDeviceFreeDeviceListNull = 1006020,

    /// Description: The set sound level monitoring interval is out of range.<br>Cause: The set sound level monitoring interval is less than 100 milliseconds, or greater than 3000 milliseconds.<br>Solutions: Reset the effective sound level monitoring interval, the effective sound level monitoring interval is [100, 3000], in milliseconds.
    ZegoErrorCodeDeviceSouldLevelIntervalInvalid = 1006031,

    /// Description: The set audio spectrum monitoring interval is out of range.<br>Cause:  The set audio spectrum monitoring interval is less than 10 milliseconds.<br>Solutions: Reset audio spectrum monitoring interval which is not less than 10 milliseconds.
    ZegoErrorCodeDeviceAudioSpectrumIntervalInvalid = 1006032,

    /// Description: Failed to set the camera zoom. <br>Cause: The set camera zoom factor is out of range. <br>Solutions: The set camera zoom factor cannot exceed the maximum range obtained, the maximum range can be obtained through [getCameraMaxZoomFactor].
    ZegoErrorCodeDeviceZoomFactorInvalid = 1006040,

    /// Description: Failed to set the camera exposure compensation. <br>Cause: The set camera exposure compensation value is out of range. <br>Solutions: Set the camera exposure compensation range between [-1,1].
    ZegoErrorCodeDeviceExposureCompensationValueInvalid = 1006041,

    /// invalid audio VAD monitor type
    ZegoErrorCodeDeviceAudioVadStableStateMonitorTypeInvalid = 1006042,

    /// Description: Internal error of the device. <br>Solutions: Contact ZEGO technical support.
    ZegoErrorCodeDeviceInnerError = 1006099,

    /// Description: Unknown error of the pre-processing module. <br>Solutions: Please contact ZEGO technical support.
    ZegoErrorCodePreprocessPreprocessUnknownError = 1007001,

    /// Description: Failed to set the beauty configuration. <br>Cause: The incoming beauty parameters are incorrect. <br>Solutions: Please check the passed in [ZegoBeautifyOption] type parameter.
    ZegoErrorCodePreprocessBeautifyOptionInvalid = 1007005,

    /// The reverberation parameter is null. Please check the input parameter. This error code is deprecated.
    ZegoErrorCodePreprocessReverbParamNull = 1007006,

    /// The voice changer parameter is null. Please check the input parameter. This error code is deprecated.
    ZegoErrorCodePreprocessVoiceChangerParamNull = 1007007,

    /// Description: Failed to set the reverberation parameters. <br>Cause: the reverberation room size parameter is invalid. <br>Solutions: The normal range of the reverberation room size parameter is 0.0 ~ 1.0
    ZegoErrorCodePreprocessReverbParamRoomSizeInvalid = 1007011,

    /// Description: Failed to set the reverberation parameters. <br>Cause: The reverberance parameter is invalid. <br>Solutions: The normal range of the reverberance parameter is 0.0 ~ 0.5
    ZegoErrorCodePreprocessReverbParamReverberanceInvalid = 1007012,

    /// Description: Failed to set the reverberation parameters. <br>Cause: the reverberation damping parameter is invalid. <br>Solutions: The normal range of the reverberation damping parameter is 0.0 ~ 2.0
    ZegoErrorCodePreprocessReverbParamDampingInvalid = 1007013,

    /// Description: Failed to set the reverberation parameters. <br>Cause: The dry_wet_ratio parameter of the reverberation is invalid. <br>Solutions: The normal range of the dry_wet_ratio parameter of reverberation is greater than 0.0
    ZegoErrorCodePreprocessReverbParamDryWetRatioInvalid = 1007014,

    /// Description: Failed to start virtual stereo. <br>Cause: The virtual stereo angle parameter is invalid. <br>Solutions: The normal range of angle parameter is -1 ~ 360.
    ZegoErrorCodePreprocessVirtualStereoAngleInvalid = 1007015,

    /// Description: Failed to set the voice changing parameters. <br>Cause: The param setting of the voice changing parameter is invalid. <br>Solutions: The normal range of parameter param is -12.0 ~ 12.0
    ZegoErrorCodePreprocessVoiceChangerParamInvalid = 1007016,

    /// The reverberation echo parameters is null. Please check the input parameter.
    ZegoErrorCodePreprocessReverbEchoParamNull = 1007017,

    /// Description: Set reverberation echo parameters failed . <br>Cause: The reverberation echo parameters is invalid. <br> Solutions: Input the correct reverb echo parameters [setReverbEchoParam].
    ZegoErrorCodePreprocessReverbEchoParamInvalid = 1007018,

    /// Description: Failed to turn on or turn off the electronic sound effect. <br>Cause: the initial pitch parameter tonal of the electronic tone is invalid. <br>Solutions: The normal range of the starting pitch parameter of the electronic tone is 0 ~ 11.
    ZegoErrorCodePreprocessElectronicEffectsTonalInvalid = 1007019,

    /// Description: Failed to open or close the beauty environment. <br>Cause: The beauty environment was not turned on or off before the engine started. <br>Solutions: Please make sure to turn on or turn off the beauty environment before the engine starts, for example: before calling (startPreview), (startPublishingStream), (startPlayingStream), (createMediaPlayer) or (createAudioEffectPlayer).
    ZegoErrorCodePreprocessEnableEffectsEnvFailed = 1007020,

    /// Description: Failed to turn on or turn off the beauty effect. <br>Cause: The beauty environment is not activated. <br>Solutions: Please call [startEffectsEnv] to start the beauty environment first.
    ZegoErrorCodePreprocessEnableEffectsBeautyFailed = 1007021,

    /// Description: Failed to set beauty parameters. <br>Cause: The beauty environment is not activated. <br>Solutions: Please call [startEffectsEnv] to start the beauty environment first.
    ZegoErrorCodePreprocessSetEffectsParamFailed = 1007022,

    /// Description: Effects Beauty does not support the currently set video data type. <br>Cause: [enableCustomVideoProcessing] interface, Windows platform only supports raw_data, Apple device only supports cv_pixel_buffer, Android platform supports gl_texture_2d. <br>Solutions: select the correct video data type.
    ZegoErrorCodePreprocessNotSupportEffectsBufferType = 1007023,

    /// Description: Failed to enable speech enhancement. <br>Cause: speech enhancement level parameter is invalid. <br>Solutions: speech enhancement level ranges from 0 to 10. Use the correct parameter value.
    ZegoErrorCodePreprocessEnableSpeechEnhanceParamInvalid = 1007024,

    /// Description: The MediaPlayer failed to play the media. <br>Cause: The resource file is not loaded. <br> Solutions: Create a media player instance before using media players [createMediaPlayer].
    ZegoErrorCodeMediaPlayerNoInstance = 1008001,

    /// Description: The MediaPlayer failed to play the media. <br>Cause: The resource file is not loaded. <br> Solutions: The media player loads the media resource [loadResource] before starting.
    ZegoErrorCodeMediaPlayerNoFilePath = 1008003,

    /// Description: The specified media file path is too long. <br>Cause: The specified media file path is too long. The maximum length should be less than 1024 bytes. <br> Solutions: Please specify media file path less than 1024 bytes.
    ZegoErrorCodeMediaPlayerFilePathTooLong = 1008004,

    /// Description: The MediaPlayer failed to load the file. <br>Cause: File formats are not supported. <br> Solutions: Files in this format are not supported, please use files in the supporting format.
    ZegoErrorCodeMediaPlayerFileFormatError = 1008005,

    /// Description: The MediaPlayer failed to load the file. <br>Cause: The file path does not exist. <br> Solutions: Check the validity of the media file path.
    ZegoErrorCodeMediaPlayerFilePathNotExists = 1008006,

    /// Description: The MediaPlayer failed to load the file. <br>Cause: The file decoding failed. <br> Solutions: Check that the media file is corrupted or contact ZEGO technical support.
    ZegoErrorCodeMediaPlayerFileDecodeError = 1008007,

    /// Description: The MediaPlayer failed to load the file. <br>Cause: No supported audio/video stream exists. <br> Solutions: Check that the media file data is empty.
    ZegoErrorCodeMediaPlayerFileNoSupportedStream = 1008008,

    /// Description: The copyrighted music resource file has expired. <br>Cause: The resource file has expired. <br>Solutions: Please request song or accompaniment again.
    ZegoErrorCodeMediaPlayerFileExpired = 1008009,

    /// Description: The MediaPlayer failed to load the file. <br>Cause: There was an error during file resolution. <br> Solutions: Try again or contact ZEGO technical support.
    ZegoErrorCodeMediaPlayerDemuxError = 1008010,

    /// Description: The copyrighted music resource has expired. <br>Cause: The copyrighted music resource has expired. <br>Solutions: Please request the copyrighted music resource again.
    ZegoErrorCodeMediaPlayerResourceExpired = 1008011,

    /// Description: The copyrighted music resource ID is too long. <br>Cause: The copyrighted music resource ID is too long. The maximum length should be less than 512 bytes. <br> Solutions: Please copyrighted music resource ID less than 1024 bytes.
    ZegoErrorCodeMediaPlayerResourceIdTooLong = 1008012,

    /// Description: User cancel. <br>Cause: After calling [Load] to load the resource, call [Stop] to stop the player. <br> Solutions: Expected behavior when calling [Stop] while loading a resource.
    ZegoErrorCodeMediaPlayerUserCancel = 1008013,

    /// Description: The MediaPlayer is already start. <br>Cause: [Load] has been called. <br> Solutions: Please call [Stop] first, then call [Load].
    ZegoErrorCodeMediaPlayerAlreadyStart = 1008014,

    /// Description: The MediaPlayer failed to load the file. <br>Cause: Permission denied. <br> Solutions: Check the validity of the media file path.
    ZegoErrorCodeMediaPlayerPermissionDenied = 1008015,

    /// Description: The MediaPlayer failed to seek. <br>Cause: The file hasn't been loaded yet. <br> Solutions: The media player loads the media resource [loadResource] before seeking [seekTo].
    ZegoErrorCodeMediaPlayerSeekError = 1008016,

    /// Description: The MediaPlayer is configured with a video data format not supported by the platform. <br>Cause: The MediaPlayer is configured with a video data format not supported by the platform (e.g., CVPixelBuffer on iOS does not support NV21). <br> Solutions: Check the data format [setVideoHandler] supported by the current media player platform and set the correct data format.
    ZegoErrorCodeMediaPlayerPlatformFormatNotSupported = 1008020,

    /// Description: The number of MediaPlayer instances exceeds the maximum number allowed. <br>Cause: The number of MediaPlayer instances exceeds the maximum number allowed. Up to 4 instances can be created. <br> Solutions: Media players can create up to 4 instances, and make sure that the number of media player instances is not exceeded the maximum limit.
    ZegoErrorCodeMediaPlayerExceedMaxCount = 1008030,

    /// Description: The media player failed to specify the audio track index. <br>Cause: The audio track index not exist. <br>Solutions: Check file audio track index call [getAudioTrackCount] can get.
    ZegoErrorCodeMediaPlayerSetAudioTrackIndexError = 1008040,

    /// Description: Media player setting sound change parameter invalid. <br>Cause: Error setting parameters. <br>Solutions: Checking setting parameters control during -12.0 to 12.0
    ZegoErrorCodeMediaPlayerSetVoiceChangerParamInvalid = 1008041,

    /// Description: takeSnapshot screenshot failed <br>Cause: The video is not playing or 'setPlayerCanvas' is not called to display the video to the control. <br>Solutions: Check whether the video plays normally(check [onPlayStart] callback) and the screen is displayed normally.
    ZegoErrorCodeMediaPlayerTakeSnapshotTimingError = 1008042,

    /// Description: the passed parameter is not in the valid value range. <br>Cause: error setting parameters. <br>Solutions: Review the interface comment and pass in a value within the legal range.
    ZegoErrorCodeMediaPlayerParamValueRangeIllegal = 1008043,

    /// Description: Failed to cache network resource file. <br>Cause: During playback, [seekTo] operation will cause cache failure, and network reasons or incomplete playback caused by active stop will also cause cache failure. <br>Solutions: Check whether there is a [seekTo] operation, whether the playback failed due to network reasons, or whether the playback stopped actively.
    ZegoErrorCodeMediaPlayerLocalCacheFailed = 1008044,

    /// Description: MediaPlayer internal error. <br>Cause: internal error. <br>Solutions: Contact Technical support.
    ZegoErrorCodeMediaPlayerInnerError = 1008099,

    /// Description: the input message content is empty <br>Cause: imessage content is empty <br>Solutions: Input a non-empty message.
    ZegoErrorCodeIMContentNull = 1009001,

    /// Description: The input message content is too long <br>Cause: message more than 1024 bytes. <br>Solutions: The maximum length should be less than 1024 byte
    ZegoErrorCodeIMContentTooLong = 1009002,

    /// Description: The input real-time sequential data is too long. <br>Cause: The length of the input data is greater than 4096 bytes. <br>Solution: Check the length of the input data, consider splitting the large data packet into multiple small data and sending it multiple times.
    ZegoErrorCodeIMDataTooLong = 1009003,

    /// Description: The room where the message is sent is different from the room currently logged in. <br>Cause: The room where the message is sent is different from the room currently logged in. <br>Solutions: Send a message to the current logged-in room ID.
    ZegoErrorCodeIMInconsistentRoomId = 1009005,

    /// Description: Failed to send the message. <br>Cause: network problems. <br>Solutions: Check the network.
    ZegoErrorCodeIMSendFailed = 1009010,

    /// Description: Failed to send custom command. <br>Cause: The entered user ID is empty. <br>Solutions: Please enter the correct user ID.
    ZegoErrorCodeIMUserIdEmpty = 1009011,

    /// Description: Failed to send custom signaling. <br>Cause: The entered user ID is too long. <br>Solutions: Please enter the correct user ID, the maximum user ID cannot exceed 64 bytes.
    ZegoErrorCodeIMUserIdTooLong = 1009012,

    /// Description: Failed to send message. <br>Cause: The message input length exceeds the limit. <br>Solutions: Check the input content length or contact ZEGO technical support to expand the message content length.
    ZegoErrorCodeIMInputParamsLengthLimit = 1009013,

    /// Description: Failed to send broadcast message,. <br>Cause: QPS exceeds the limit. <br>Solutions: Control the maximum QPS is 2 .
    ZegoErrorCodeIMBroadcastMessageQpsOverload = 1009015,

    /// Description: The real-time sequential data manager instance creation failed. <br>Cause: A manager instance with this room ID has already been created. <br>Solution: A maximum of 1 instance can be created for each room ID. If you need to create multiple instances, please use other room IDs.
    ZegoErrorCodeIMManagerCreationFailed = 1009031,

    /// Description: The specified real-time sequential data manager instance could not be found. <br>Cause: This manager instance has not been created yet. <br>Solution: Please call the [createRealTimeSequentialDataManager] function to create a manager instance first.
    ZegoErrorCodeIMNoManagerInstance = 1009032,

    /// Description: No publish channel available for broadcasting. <br>Cause: The developer has used all publish channels. <br>Solution: Do not use all the publish channels, check if there are any streams that can stop publsihing, or contact ZEGO technical support to increase the available publish channels.
    ZegoErrorCodeIMNoAvailableBroadcastChannel = 1009033,

    /// Description: The stream ID to start broadcasting is not available. <br>Cause: The stream ID has been used in this device for RTC business (e.g. [startPublishingStream] / [startPlayingStream]). <br>Solution: Please use another stream ID for broadcasting.
    ZegoErrorCodeIMNoAvailableStreamId = 1009034,

    /// Description: Repeat broadcast. <br>Cause: The developer repeatedly calls the [startBroadcasting] function. <br>Solution: Please check the business logic to avoid repeating the broadcast for the stream which is broadcasting.
    ZegoErrorCodeIMRepeatBroadcast = 1009035,

    /// Description: The stream to stop broadcasting does not exist. <br>Cause: The stream ID set [stopBroadcasting] function is not in broadcasting. <br>Solution: Check if the stream ID is correct, or if the stream ID is not in broadcasting.
    ZegoErrorCodeIMNoBroadcatingStream = 1009036,

    /// Description: The stream to stop subscribing does not exist. <br>Cause: The stream ID set [stopSubscribing] function is not in subscribing. <br>Solution: Check if the stream ID is correct, or if the stream ID is not in subscribing.
    ZegoErrorCodeIMNoSubscribingStream = 1009037,

    /// Description: Repeat broadcast. <br>Cause: The developer repeatedly calls the [startBroadcasting] function. <br>Solution: Please check the business logic to avoid repeating the subscribe for the stream which is subscribing.
    ZegoErrorCodeIMRepeatSubscribe = 1009038,

    /// Description: Failed to send real-time sequential data. <br>Cause: The broadcast has not started yet, or the broadcast has encountered network problems. <br>Solution: Check whether [startBroadcasting] has been called to start broadcasting, or check whether the network is normal.
    ZegoErrorCodeIMRealTimeSequentialDataSendFailed = 1009039,

    /// Description: the file name suffix is not supported. <br>Cause: the file name suffix is not supported. <br>Solutions: only support .mp4/.aac/.flv.
    ZegoErrorCodeRecorderFileSuffixNameFormatNotSupport = 1010002,

    /// Description: Generic recording API error. <br>Cause: Invalid input parameter. <br> Solutions: Please check the record file path parameter or the record file format parameter is valid or not.
    ZegoErrorCodeRecorderCommonLiveroomApiError = 1010003,

    /// Description: The specified recorded file path is too long. <br>Cause: The specified recorded file path is too long. The maximum length should be less than 1024 bytes. <br> Solutions: Please specify recorded file path less than 1024 bytes.
    ZegoErrorCodeRecorderFilePathTooLong = 1010011,

    /// Description: SDK internal error. <br>Cause: Internal error. <br> Solutions: Please contact ZEGO technical support.
    ZegoErrorCodeRecorderInnerVeError = 1010012,

    /// Description: Open file failed. <br>Cause: Invalid file path or no permissions to read/write file. <br> Solutions: Please specify a valid file path and has proper permissions to read/write.
    ZegoErrorCodeRecorderOpenFileFailed = 1010013,

    /// Description: Write file failed. <br>Cause: No permissions to write file. <br> Solutions: Please specify a valid file path and has proper permissions to write.
    ZegoErrorCodeRecorderWriteFileError = 1010014,

    /// Description: Insufficient disk space. <br>Cause: Insufficient disk space. <br> Solutions: Please ensure sufficient disk space.
    ZegoErrorCodeRecorderNoEnoughSpareCapacity = 1010017,

    /// Description: File handle exception. <br>Cause: File handle exception. <br> Solutions: Please contact ZEGO technical support.
    ZegoErrorCodeRecorderFileHandleExceptions = 1010018,

    /// Description: I/O exception. <br>Cause: I/O exception. <br> Solutions: Please contact ZEGO technical support.
    ZegoErrorCodeRecorderIoExceptions = 1010019,

    /// Description: The custom video capturer is not created. <br>Cause: Create custom video capturer before onStart callback received. <br> Solutions: Please create custom video capturer after received onStart callback.
    ZegoErrorCodeCustomVideoIOCapturerNotCreated = 1011001,

    /// Description: The custom video capture module is not enabled. <br>Cause: Custom video capture module is not enabled in initialization configurations. <br> Solutions: Please contact ZEGO technical support, make sure custom video capture module is enabled in initialization configurations.
    ZegoErrorCodeCustomVideoIONoCustomVideoCapture = 1011002,

    /// Description: Failed to enable/disable custom video capture/rendering. <br>Cause: Not enable/disable custom video capture/rendering before engine is started. <br> Solutions: Please make sure to enable/disable custom video capture/rendering before engine is started, i.e., before calling (startPreview), (startPublishingStream), (startPlayingStream), (createMediaPlayer) or (createAudioEffectPlayer).
    ZegoErrorCodeCustomVideoIOEnableCustomIoFailed = 1011003,

    /// Description: The custom video capturer is not created. <br>Cause: Internal video-related modules are not created. <br> Solutions: Please call [startPreview] or [startPublishingStream] first.
    ZegoErrorCodeCustomVideoIOProcessModuleNotCreated = 1011004,

    /// Description: The custom video process module is not enabled. <br>Cause: The custom video process module is not enabled. <br> Solutions: Call [enableCustomVideoProcessing] to enable a custom video capturermodule.
    ZegoErrorCodeCustomVideoIONoCustomVideoProcessing = 1011005,

    /// Description: The currently configured custom video capture format does not support this API. <br>Cause: The currently configured custom video capture format does not support this API. <br> Solutions: Please contact ZEGO technical support.
    ZegoErrorCodeCustomVideoIONotSupportedFormat = 1011010,

    /// Description: Custom video rendering does not support the currently set video buffer type. <br>Cause: The buffer_type in the config parameter of [enableCustomVideoRender] only supports raw_data, cv_pixel_buffer, encoded_data. For [enableCustomVideoProcessing], only raw_data is supported on Windows platform, only cv_pixel_buffer is supported on Apple devices, and gl_texture_2d and surface_texture are supported on Android platform. <br> Solutions: Select the correct video buffer type.
    ZegoErrorCodeCustomVideoIONotSupportedBufferType = 1011011,

    /// Description: [customVideoCaptureSendD3DTextureData] specifies texture not supported by the SDK. <br>Cause: TThe device corresponding to the texture specified by [customVideoCaptureSendD3DTextureData] was not created by D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, NULL,...). <br> Solutions: [customVideoCaptureSendD3DTextureData] specifies the texture created by D3D11CreateDevice (0, D3D_DRIVER_TYPE_HARDWARE, NULL,...).
    ZegoErrorCodeCustomVideoIOInvalidActiveD3d11Device = 1011012,

    /// Description: Unsupported custom audio source type. <br>Cause: Only channel_aux supports zego_audio_source_type_media_player. <br> Solutions: Select the correct custom audio source type.
    ZegoErrorCodeCustomAudioIOUnsupportedAudioSourceType = 1012001,

    /// Description: The custom audio capture feature is not enabled. <br>Cause: The custom audio capture feature is not enabled. <br> Solutions: Please make sure that the custom audio IO module is enabled for the specified stream publishing channel.
    ZegoErrorCodeCustomAudioIOCapturerNotCreated = 1012002,

    /// Description: The custom audio rendering feature is not enabled. <br>Cause: The custom audio rendering feature is not enabled. <br> Solutions: Please make sure that the custom audio IO module is enabled.
    ZegoErrorCodeCustomAudioIORendererNotCreated = 1012003,

    /// Description: Failed to enable/disable custom audio IO. <br>Cause: Failed to enable/disable custom audio IO. <br> Solutions: Please make sure to enable/disable it before the engine is started (i.e., before calling `startPreview`, `startPublishingStream` or `startPlayingStream`).
    ZegoErrorCodeCustomAudioIOEnableCustomAudioIoFailed = 1012004,

    /// Description: The sample rate parameter is illegal. <br>Cause: Capture and render mix results recording does not support 8000, 22050, 24000 sample rates. <br> Solutions: Please confirm whether the sample rate parameter value allowed by the interface is legal.
    ZegoErrorCodeCustomAudioIOAudioDataCallbackSampleRateNoSupport = 1012010,

    /// Description: The MediaDataPublisher instance is not created. <br>Cause: The MediaDataPublisher instance is not created. <br> Solutions: Call [createMediaDataPublisher] to create a media pusher instance.
    ZegoErrorCodeMediaDataPublisherNoInstance = 1013000,

    /// Description: This error code is deprecated. <br>Cause: None. <br> Solutions: None.
    ZegoErrorCodeMediaDataPublisherFileParseError = 1013001,

    /// Description: This error code is deprecated. <br>Cause: None. <br> Solutions: None.
    ZegoErrorCodeMediaDataPublisherFilePathError = 1013002,

    /// Description: File decoding exception. <br>Cause: Invalid media file format. <br>Solutions: Please check the file is a valid media file or not; check the file format is in the MediaPlayer support file format list or not.
    ZegoErrorCodeMediaDataPublisherFileCodecError = 1013003,

    /// Description: Timestamp error. <br>Cause: the later frame timestamp is smaller than the previous frame timestamp. <br>Solutions: Please provide the media file, and contact ZEGO technical support.
    ZegoErrorCodeMediaDataPublisherTimestampGoBackError = 1013004,

    /// Description: The number of instances of the media data publisher created exceeds the maximum limit. <br>Cause: The number of instances of the media data publisher created exceeds the maximum limit. <br> Solutions: Please check if the number of instances of the media data publisher created exceeds the maximum limit, the maximum number of instances allowed to be created is 1.
    ZegoErrorCodeMediaDataPublisherExceedMaxCount = 1013005,

    /// Description: No audio effect player instance. <br>Cause: The audio effect player instance not created. <br> Solutions: Create an audio effect player instance before using it(createAudioEffectPlayer).
    ZegoErrorCodeAudioEffectPlayerNoInstance = 1014000,

    /// Description: Load audio effect resource failed. <br>Cause: Invalid audio effect resource file. <br> Solutions: Check the file format is in the AudioEffectPlayer support file format list or not.
    ZegoErrorCodeAudioEffectPlayerLoadFailed = 1014001,

    /// Description: Play audio effect failed. <br>Cause: Invalid audio effect resource file. <br> Solutions: Check the file format is in the AudioEffectPlayer support file format list or not.
    ZegoErrorCodeAudioEffectPlayerPlayFailed = 1014002,

    /// Description: Change audio effect progress failed. <br>Cause: The audio effect progress value exceed audio effect file duration. <br> Solutions: Please check the audio effect progress value exceed audio effect file duration or not.
    ZegoErrorCodeAudioEffectPlayerSeekFailed = 1014003,

    /// Description: The number of instances of the audio effect player created exceeds the maximum limit. <br>Cause: The number of instances of the audio effect player created exceeds the maximum limit. <br> Solutions: Please check if the number of instances of the audio effect player created exceeds the maximum limit, the maximum number of instances allowed to be created is 1.
    ZegoErrorCodeAudioEffectPlayerExceedMaxCount = 1014004,

    /// Description: Network connectivity test failed. <br>Cause: Not connected to the network. <br> Solutions: Please check if you can access the Internet properly.
    ZegoErrorCodeUtilitiesNetworkConnectivityTestFailed = 1015001,

    /// Description: Network speed test connection failure. <br>Cause: Not connected to the network. <br> Solutions: Please check if you can access the Internet properly.
    ZegoErrorCodeUtilitiesNetworkToolConnectServerFailed = 1015002,

    /// Description: RTP timeout. <br>Cause: Not connected to the network. <br> Solutions: Please check if you can access the Internet properly.
    ZegoErrorCodeUtilitiesNetworkToolRtpTimeoutError = 1015003,

    /// Description: The server side ends the network speed test. <br>Cause: Network speed test time is too long. <br> Solutions: Please stop network speed test in 30 seconds.
    ZegoErrorCodeUtilitiesNetworkToolEngineDenied = 1015004,

    /// Description: Network speed test stopped. <br>Cause: Network speed test not stopped before publishing stream. <br> Solutions: Please stop network speed test(stopNetworkSpeedTest) before publishing stream.
    ZegoErrorCodeUtilitiesNetworkToolStoppedByPublishingStream = 1015005,

    /// Description: Network speed test stopped. <br>Cause: Network speed test not stopped before playing stream. <br> Solutions: Please stop network speed test(stopNetworkSpeedTest) before playing stream.
    ZegoErrorCodeUtilitiesNetworkToolStoppedByPlayingStream = 1015006,

    /// Description: Network speed test internal error. <br>Cause: Internal error. <br> Solutions: Please contact ZEGO technical support.
    ZegoErrorCodeUtilitiesNetworkToolInnerError = 1015009,

    /// Description: Invalid system performance monitoring interval. <br>Cause: The set system performance monitoring interval is out of range. <br> Solutions: Please check if the system performance monitoring interval is out of range or not, valid range is [1000, 10000].
    ZegoErrorCodeUtilitiesPerformanceMonitorIntervalInvalid = 1015031,

    /// Description: Login to the room causes the network test to stop. <br>Cause: Already logged in to the room. <br>Solutions: Since the network test will take up bandwidth, please do it before logging in to the room.
    ZegoErrorCodeUtilitiesStopByLoginRoom = 1015032,

    /// Description: Fail to dump data. <br>Cause: Error occurred in AV engine. <br>Solutions: Please contact ZEGO technical support.
    ZegoErrorCodeUtilitiesStartDumpAudioDataFailed = 1015033,

    /// Description: Already start dumping data. <br>Cause:  Call [startDumpData] repeatedly. <br>Solutions: Stop or cancel the previous dump data task.
    ZegoErrorCodeUtilitiesAlreadyStartDumpDataError = 1015034,

    /// Description: No dump data. <br>Cause: Error occurred in AV engine. <br>Solutions: Please contact ZEGO technical support.
    ZegoErrorCodeUtilitiesNoDumpDataError = 1015035,

    /// Description: Failed to create dump data folder. <br>Cause: No read and write permission or the disk is full. <br>Solutions: Check to see if there is still space on the disk.
    ZegoErrorCodeUtilitiesCreateDumpDataFolderError = 1015036,

    /// Description: Internal error of upload module. <br>Cause: Error occurred in upload module. <br>Solutions: Please contact ZEGO technical support.
    ZegoErrorCodeUtilitiesUploadModuleInnerError = 1015037,

    /// Description: The dump data exceeding the maximum upload limit error. <br>Cause: The dump data is too large to exceed the limit of the upload module. <br>Solutions: Please contact ZEGO technical support and report to ZEGO manually.
    ZegoErrorCodeUtilitiesDumpDataExceedMaxSizeError = 1015038,

    /// Description: The function call failed. <br>Cause: No range auido instance has been created. <br>Solutions: Create a range audio instance.
    ZegoErrorCodeRangeAudioNoInstance = 1016000,

    /// Description: Failed to create range audio. <br>Cause: The instance exceeds the maximum limit. <br>Solutions: Use the used range audio example.
    ZegoErrorCodeRangeAudioExceedMaxCount = 1016001,

    /// Description: Failed to create range voice. <br>Cause: Range voice cannot be used in multi-room mode. <br>Solutions: Set the single-party mode.
    ZegoErrorCodeRangeAudioNotSupportMultiRoom = 1016002,

    /// Description: Failed to set the team ID. <br>Cause: The input team ID exceeds the maximum limit. <br>Solutions: The input string is less than 64 bytes.
    ZegoErrorCodeRangeAudioTeamIdTooLong = 1016003,

    /// Description: Failed to set the team ID.<br>Cause: The input user ID contains invalid characters. <br>Solutions: User ID can only contains numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', ',', '.', '<', '>', '/', '\'.
    ZegoErrorCodeRangeAudioTeamIdInvalidCharacter = 1016004,

    /// Description: The command invalid. <br>Cause: The command entered by the [sendExtendedRequest] function is empty. <br>Solutions: Please check the command entered when calling the [sendExtendedRequest] function to make sure it is not empty.
    ZegoErrorCodeCopyrightedMusicCommandInvalid = 1017000,

    /// Description: The params invalid. <br>Cause: The params entered by the [sendExtendedRequest] function is empty. <br>Solutions: Please check the params entered when calling the [sendExtendedRequest] function to make sure it is not empty.
    ZegoErrorCodeCopyrightedMusicParamsInvalid = 1017001,

    /// Description: The song_id invalid. <br>Cause: The song_id entered is empty. <br>Solutions: Please check the song_id entered when calling the function to make sure it is not empty.
    ZegoErrorCodeCopyrightedMusicSongIdInvalid = 1017002,

    /// Description: The share_token invalid. <br>Cause: The share_token entered is empty. <br>Solutions: Please check the share_token entered when calling the function to make sure it is not empty.share_token can be obtained by call [requestResource]
    ZegoErrorCodeCopyrightedMusicShareTokenInvalid = 1017003,

    /// Description: The resource_id invalid. <br>Cause: The resource_id entered is empty. <br>Solutions: Please check the resource_id entered when calling the function to make sure it is not empty.resource_id can be obtained by call [requestResource] [getSharedResource] [getMusicByToken]
    ZegoErrorCodeCopyrightedMusicResourceIdInvalid = 1017004,

    /// Description: The start_position invalid. <br>Cause: The start_position entered by the fuction [loadCopyrightedMusicResourceWithPosition] is invalid. <br>Solutions: Please check the start_position entered when calling the function [loadCopyrightedMusicResourceWithPosition] to make sure it is in 0 ~ song duration.
    ZegoErrorCodeCopyrightedMusicStartPositionInvalid = 1017005,

    /// Description: The position invalid. <br>Cause: The position entered by the fuction [seek] is invalid. <br>Solutions: Please check the position entered when calling the function [seek] to make sure it is in 0 ~ song duration.
    ZegoErrorCodeCopyrightedMusicPositionInvalid = 1017006,

    /// Description: The volume invalid.. <br>Cause: The Volume entered by the fuction [setPlayVolume] is invalid. <br>Solutions: Please check the Volume entered when calling the function [setPlayVolume] to make sure it is in 0 ~ 200.
    ZegoErrorCodeCopyrightedMusicVolumeInvalid = 1017007,

    /// Description: The krcToken invalid. <br>Cause: The krcToken entered is empty. <br>Solutions: Please check the krcToken entered when calling the function to make sure it is not empty.krcToken can be obtained by call [requestResource] [getSharedResource]
    ZegoErrorCodeCopyrightedMusicKrcTokenInvalid = 1017008,

    /// Description: Copyright music init authentication failed. <br>Cause: Appsign or token is not set. <br>Solutions: When using token authentication, call [loginRoom] before calling [initCopyrightedMusic] or use appsign authentication.
    ZegoErrorCodeCopyrightedMusicCopyrightedMusicAuthParamInvalid = 1017009,

    /// Description: Request copyrighted server fail. <br>Cause: The params entered make mistake or some network reasons. <br>Solutions: Please check the params entered and retry.
    ZegoErrorCodeCopyrightedMusicCopyrightedServerFail = 1017010,

    /// Description: Free space limit. <br>Cause: Free space limit. <br>Solutions: Please clean up local files and make sure there is enough free disk space.
    ZegoErrorCodeCopyrightedMusicFreeSpaceLimit = 1017011,

    /// Description: Downloading. <br>Cause: Download same resource. <br>Solutions: Please wait for the resource to download successfully.
    ZegoErrorCodeCopyrightedMusicDownloading = 1017012,

    /// Description: Resource file not exist. <br>Cause: Resource file has been deleted. <br>Solutions: Please reload resource.
    ZegoErrorCodeCopyrightedMusicResourceFileNotExist = 1017013,

    /// Description: Resource file expired. <br>Cause: The resource file has expired. <br>Solutions: Please request song or accompaniment again.
    ZegoErrorCodeCopyrightedMusicResourceFileExpired = 1017014,

    /// Description: Resource file invalid. <br>Cause: File is corrupted <br>Solutions: Please call [download] function to reload media resource.
    ZegoErrorCodeCopyrightedMusicResourceFileInvalid = 1017015,

    /// Description: Download canceled. <br>Cause: Call [cancelDownload] to actively cancel the download.
    ZegoErrorCodeCopyrightedMusicDownloadCanceled = 1017016,

    /// Description: The resource_id unauthorized. <br>Cause: The resource ID is not obtained through [requestResource] [getSharedResource] function. <br>Solutions: Please call the [requestResource] [getSharedResource] function to get valid resource ID.
    ZegoErrorCodeCopyrightedMusicResourceIdUnauthorized = 1017018,

    /// Description: The copyrighted resource has expired. <br>Cause: The copyrighted resource has expired. <br>Solutions: Please request the copyrighted resource again.
    ZegoErrorCodeCopyrightedMusicResourceExpired = 1017019,

    /// Description: The resource does not support this method. <br>Cause: Wrong resource ID selection. <br>Solutions: Please pass in the correct resource ID.
    ZegoErrorCodeCopyrightedMusicResourceNotSupportFunction = 1017020,

    /// Description: No copyright, unable to listen to and sing songs. <br>Cause: No copyright. <br>Solutions: Please select another music.
    ZegoErrorCodeCopyrightedMusicNoCopyright = 1017030,

    /// Description: No permissions of accompaniment, can only listen to songs, not sing. <br>Cause: No permissions of accompaniment. <br>Solutions: Please select another music.
    ZegoErrorCodeCopyrightedMusicNoPermissionsOfLyricsAndMusic = 1017031,

    /// Description: Non monthly membership. <br>Cause: Unopened monthly membership. <br>Solutions: Open monthly membership or ues COUNT mode request music.
    ZegoErrorCodeCopyrightedMusicNonMonthlyMembership = 1017032,

    /// Description: No accompany. <br>Cause: Music don't have accompany. <br>Solutions: Please choice music have accompany.
    ZegoErrorCodeCopyrightedMusicNoAccompany = 1017033,

    /// Description: Resource not exist. <br>Cause: Resource not exist. <br>Solutions: Please select another music.
    ZegoErrorCodeCopyrightedMusicResourceNotExist = 1017034,

    /// Description: Illegal param. <br>Cause: The entered param is incorrect. <br>Solutions: Please check param when entered function to make sure it is correct.
    ZegoErrorCodeCopyrightedMusicIllegalParam = 1017040,

    /// Description: AppID invalid. <br>Cause: The appID is not support copyrighted music. <br>Solutions: Please contact ZEGO technical support.
    ZegoErrorCodeCopyrightedMusicAppidInvalid = 1017041,

    /// Description: Billing mode invalid. <br>Cause: Billing mode invalid. <br>Solutions: Please select correct billing mode.
    ZegoErrorCodeCopyrightedMusicBillingModeInvalid = 1017042,

    /// Description: Unreasonable_access. <br>Cause: Monthly membership request music by COUNT. <br>Solutions: Please select correct billing mode.
    ZegoErrorCodeCopyrightedMusicUnreasonableAccess = 1017043,

    /// Description: Share token expired. <br>Cause: Share token expired. <br>Solutions: Please select an unexpired sharing token to get resources.
    ZegoErrorCodeCopyrightedMusicShareTokenExpired = 1017044,

    /// Description: Share token illegal. <br>Cause: Share token illegal. <br>Solutions: Please check songToken when entered by calling [getMusicByToken] to make sure it is correct.
    ZegoErrorCodeCopyrightedMusicShareTokenIllegal = 1017045,

    /// Description: krcToken illegal. <br>Cause: krcToken illegal. <br>Solutions: Please check krcToken when entered by calling [getKrcLyricByKrcToken] to make sure it is correct.
    ZegoErrorCodeCopyrightedMusicKrcTokenIllegal = 1017046,

    /// Description: krcToken expired. <br>Cause: krcToken expired. <br>Solutions: Please select an unexpired krcToken to get lyrics in KRC format.
    ZegoErrorCodeCopyrightedMusicKrcTokenExpired = 1017047,

    /// Description: Get lyric fail. <br>Cause: Lyrics not found. <br>Solutions: Please try again later.
    ZegoErrorCodeCopyrightedMusicGetLyricFail = 1017048,

    /// Description: Get pitch fail. <br>Cause: Pitch not found. <br>Solutions: Please select correct song.
    ZegoErrorCodeCopyrightedMusicGetPitchFail = 1017049,

    /// Description: This resource is not shared in the room. <br>Cause: No users in the room share this resource. <br>Solutions: Please any one user in the room to call the [requestresource] function to request resources and share them.
    ZegoErrorCodeCopyrightedMusicNotSharedResource = 1017050,

    /// Description: The number of times the resource is free in the room is exhausted. <br>Cause: 1. The shared resources cannot be obtained again; 2. Shared resources have been obtained. <br>Solutions: Please use the acquired resources, or use [requestResource] to share resources again.
    ZegoErrorCodeCopyrightedMusicGetSharedResourceTimesOver = 1017051,

    /// Description: The vendor is unavailable. <br>Cause: The corresponding copyright party has not been activated. <br>Solutions: Please pass in the correct vendor ID.
    ZegoErrorCodeCopyrightedMusicVendorIdNotAvaliable = 1017052,

    /// Description: This vendor does not support this function. <br>Cause: Wrong vendor selection. <br>Solutions: Please pass in the correct vendor ID.
    ZegoErrorCodeCopyrightedMusicVendorNotSupportFunction = 1017053,

    /// Description: The room ID is not logged in. <br>Cause: Room ID input error. <br>Solutions: If the copyright is not used in a multi room scenario, there is no need to pass in the roomID, otherwise please pass in the correct logged in room ID.
    ZegoErrorCodeCopyrightedMusicRoomNoLogin = 1017054,

    /// Description: Vendor ID invalid. <br>Cause: Wrong vendor selection. <br>Solutions: Please pass in the correct vendor ID.
    ZegoErrorCodeCopyrightedMusicVendorIdInvalid = 1017071,

    /// Description: This vendor does not support this song ID. <br>Cause: This song ID does not exist in the copyright music library. <br>Solutions: Please pass in the correct song ID.
    ZegoErrorCodeCopyrightedMusicSongIdNotSupport = 1017072,

    /// Description: master ID invalid. <br>Cause: When billing by owner is selected,  master ID is empty. <br>Solutions: Please pass in the correct master ID.
    ZegoErrorCodeCopyrightedMusicMasterIdInvalid = 1017073,

    /// Description: The page parameter is invalid for multiple copyright searches. <br>Cause: The page parameter must be 1 when multiple rights search is called for the first time. <br>Solutions: Please pass in the correct page.
    ZegoErrorCodeCopyrightedMusicPageInvalid = 1017074,

    /// Description: resource has no pitch. <br>Cause: resource has no pitch. <br>Solutions: Please pass in the correct song ID.
    ZegoErrorCodeCopyrightedMusicNoPitch = 1017075,

    /// Description: scene ID invalid. <br>Cause: scene ID invalid. <br>Solutions: Please pass in the scene ID corresponding to the opened scene.
    ZegoErrorCodeCopyrightedMusicSceneIdInvalid = 1017076,

    /// Description: top ID invalid. <br>Cause: An unsupported top ID was passed in when fetching chart songs. <br>Solutions: Please pass in the correct top ID.
    ZegoErrorCodeCopyrightedMusicTopIdInvalid = 1017077,

    /// Description: Loading copyright music plugin failed. <br>Cause: The program runtime path does not include the copyright music plugin dynamic library. <br>Solutions: Please configure the copyright music plugin dynamic library dependency correctly.
    ZegoErrorCodeCopyrightedMusicLoadPluginFail = 1017094,

    /// Description: The copyright music module does not support this method. <br>Cause: The copyright music module does not support this function under the current platform. <br>Solutions: Contact ZEGO technical support to deal with it.
    ZegoErrorCodeCopyrightedMusicNotSupportMethod = 1017095,

    /// Description: The copyright music module is not initialized. <br>Cause: The [initCopyrightedMusic] method is not called to initialize the copyright module. <br>Solutions: Please call the [initCopyrightedMusic] method first.
    ZegoErrorCodeCopyrightedMusicNoInit = 1017096,

    /// Description: System is busy. <br>Cause: System is busy. <br>Solutions: Please try again.
    ZegoErrorCodeCopyrightedMusicSystemBusy = 1017097,

    /// Description: Failed due to network exceptions.<br>Cause: Unknown internal error.<br>Solutions: Contact ZEGO technical support to deal with it.
    ZegoErrorCodeCopyrightedMusicNetworkError = 1017098,

    /// Description: Failed due to internal system exceptions.<br>Cause: Unknown internal error.<br>Solutions: Contact ZEGO technical support to deal with it.
    ZegoErrorCodeCopyrightedMusicInnerError = 1017099,

    /// Description: SDK internal return null pointer.
    ZegoErrorCodeAudioVADClientInnerNullptr = 1018000,

    /// Description: The function call failed. <br>Cause: No audio vad instance has been created. <br>Solutions: Create a audio vad instance.
    ZegoErrorCodeAudioVADClientNoInstance = 1018001,

    /// Description: Calling function failed. <br>Cause: Illegal parameter or SDK does not contain RangeScene module. <br>Solutions: Please confirm whether the SDK contains the RangeScene module and check the parameters.
    ZegoErrorCodeRangeSceneCallFunctionFailed = 1019000,

    /// Description: The RangeScene instance not created. <br>Cause: The RangeScene instance not created. <br> Solutions: Create a RangeScene instance before using RangeScene [createRangeScene].
    ZegoErrorCodeRangeSceneNoInstance = 1019001,

    /// Description: Not login scene. <br>Cause: Not login scene. <br> Solutions: Please call [loginScene] to login scene.
    ZegoErrorCodeRangeSceneNotLoginScene = 1019002,

    /// Description: Use not support feature. <br>Cause: Use the status synchronization interface when the status synchronization function is not enabled. <br> Solutions: To use the status synchronization interface, please enable the status synchronization function.
    ZegoErrorCodeRangeSceneNotSupport = 1019003,

    /// Description: Scene login retry has exceeded the maximum retry time. <br>Cause: Possibly due to network problems. <br>Solutions: Please check whether the network is working or switch the network environment.
    ZegoErrorCodeRangeSceneRetryTimeout = 1019004,

    /// Description: Scene connection is temporarily interrupted. <br>Cause: Possibly due to network problems. <br>Solutions: Please wait or check whether the network is normal.
    ZegoErrorCodeRangeSceneNetworkInterrupt = 1019005,

    /// Description: Token verification failed. <br>Cause: The parameters passed in during token generation are inconsistent with the SDK. <br>Solutions: Please use the correct token regenerated.
    ZegoErrorCodeRangeSceneTokenIllegal = 1019006,

    /// Description: Token expire. <br>Cause: Token expire or the generated Token validity period parameter type is incorrect. <br>Solutions: Regenerate the Token.
    ZegoErrorCodeRangeSceneTokenExpire = 1019007,

    /// Description: Coordinates out of range. <br>Cause: Coordinates out of scene range. <br>Solutions: Please pass in the correct coordinates.
    ZegoErrorCodeRangeSceneCoordinateOutOfRange = 1019020,

    /// Description: The item has been created. <br>Cause: The item has been created. <br>Solutions: The item has been created. You don't need to create it again.
    ZegoErrorCodeRangeSceneItemHasBeenCreated = 1019021,

    /// Description: The item binding user exceeds the maximum limit. <br>Cause: The item binding user exceeds the maximum limit. <br>Solutions: The item binding user exceeds the maximum limit. Please try again later.
    ZegoErrorCodeRangeSceneItemBindExceedLimit = 1019022,

    /// Description: The item does not exist. <br>Cause: The item does not exist. <br>Solutions: Please create an item first.
    ZegoErrorCodeRangeSceneItemNotExist = 1019023,

    /// Description: The item is not bound. <br>Cause: The item is not bound. <br>Solutions: Please bind the item first.
    ZegoErrorCodeRangeSceneItemNotBind = 1019024,

    /// Description: The item has been operated by others. <br>Cause: The item has been operated by others. <br>Solutions: Please try again later.
    ZegoErrorCodeRangeSceneItemCasFailed = 1019025,

    /// Description: The binding capacity of the item exceeds the maximum limit. <br>Cause: The binding capacity of the item exceeds the maximum limit. <br>Solutions: Please use a capacity that does not exceed the maximum binding capacity to create an item.
    ZegoErrorCodeRangeSceneItemCapacityExceedLimit = 1019026,

    /// Description: The user has bound the item to the maximum limit. <br>Cause: The user has bound the item to the maximum limit. <br>Solutions: Please unbind some items that do not need to be operated temporarily.
    ZegoErrorCodeRangeSceneUserBindItemExceedLimit = 1019027,

    /// Description: The item is beyond the user's view. <br>Cause: The item is beyond the user's view. <br>Solutions: Please operate the item within the user's view.
    ZegoErrorCodeRangeSceneItemOutOfUserView = 1019028,

    /// Description: The number of RangeScene instances exceeds the maximum number allowed. <br>Cause: The number of RangeScene instances exceeds the maximum number allowed. Up to 1 instances can be created. <br> Solutions: RangeScene can create up to 1 instances, and make sure that the number of RangeScene instances is not exceeded the maximum limit.
    ZegoErrorCodeRangeSceneExceedMaxCount = 1019030,

    /// Description: The number of joined RangeScene teams exceeds the maximum number allowed. <br>Cause: The number of joined RangeScene teams exceeds the maximum number allowed. Up to 5 are allowed by default. <br> Solutions: RangeScene teams can joined are up to 5, and make sure that the number of RangeScene teams joned is not exceeded the maximum limit.
    ZegoErrorCodeRangeSceneTeamExceedMaxCount = 1019031,

    /// Description: Team id already been used. <br>Cause: The team id already been used when join team. <br> Solutions: Use a new team id to join team.
    ZegoErrorCodeRangeSceneTeamIdOccupied = 1019032,

    /// Description: Team id not exist. <br>Cause: Use incorrect team id when leave team. <br> Solutions: Use correct team id when leave team.
    ZegoErrorCodeRangeSceneTeamIdIncorrect = 1019033,

    /// Description: Failed due to internal system exceptions.<br>Cause: Unknown internal error.<br>Solutions: Contact ZEGO technical support to deal with it.
    ZegoErrorCodeRangeSceneInnerError = 1019099,

    /// Description: Startup screen capture failed. <br>Cause: The user refused to grant the app screen capture permission. <br>Solutions: Allow the app to capture screen permissions.
    ZegoErrorCodeScreenCapturePermissionDenied = 1020000,

    /// Description: Startup screen capture failed. <br>Cause: The current system version does not support screen capture. <br>Solutions: Use system version above Android 5 (API level 21).
    ZegoErrorCodeScreenCaptureNotSupport = 1020001,

    /// Description: Startup screen capture failed. <br>Cause: Unable to share the screen module. <br>Solutions: Please introduce screen sharing module resources, or contact technical support.
    ZegoErrorCodeScreenCaptureSdkNoModule = 1020002,

    /// Description: The function call failed. <br>Cause: No screen capture source instance has been created. <br>Solutions: Create a screen capture source instance.
    ZegoErrorCodeScreenCaptureNoInstance = 1020003,

    /// Description: Failed to create screen capture source. <br>Cause: The instance exceeds the maximum limit. <br>Solutions: Use an existing screen capture instance or destroy the previous instance.
    ZegoErrorCodeScreenCaptureExceedMaxCount = 1020004,

    /// Description: Failed due to internal system exceptions.<br>Cause: Unknown internal error.<br>Solutions: Contact ZEGO technical support to deal with it.
    ZegoErrorCodeScreenCaptureInnerError = 1020005,

    /// Description: The function call failed. <br>Cause: No AI voice changer instance has been created. <br>Solutions: Create an AI voice changer instance.
    ZegoErrorCodeAIVoiceChangerNoInstance = 1021000,

    /// Description: Failed to create AI voice changer. <br>Cause: The instance exceeds the maximum limit. <br>Solutions: Use an existing AI voice changer instance or destroy the previous instance.
    ZegoErrorCodeAIVoiceChangerExceedMaxCount = 1021001,

    /// Description: Failed due to internal system exceptions.<br>Cause: Unknown internal error.<br>Solutions: Contact ZEGO technical support to deal with it.
    ZegoErrorCodeAIVoiceChangerInnerError = 1021002,

    /// Description: AI voice changer was not initialized.<br>Cause: Did not initialize AI voice changer before use it.<br>Solutions: Use AI voice changer after initialize it.
    ZegoErrorCodeAIVoiceChangerNotInitError = 1021003,

    /// Description: Initialize AI voie changer failed.<br>Cause: Did not set user ID before initialize AI voice changer, or AI voice changer service expired.<br>Solutions: Set user ID before initialize AI voice changer, or contact ZEGO technical support to deal with it.
    ZegoErrorCodeAIVoiceChangerInitFailedError = 1021004,

    /// Description: No AI voice changer resource files.<br>Cause: AI voice changer resource files were not downloaded.<br>Solutions: Please make sure AI voice changer resource files downloaded before use it.
    ZegoErrorCodeAIVoiceChangerNoModelError = 1021005,

    /// Description: Download AI voice changer resource files failed.<br>Cause: Poor network connection.<br>Solutions: Please check the network connection is ok.
    ZegoErrorCodeAIVoiceChangerModelDownloadFailedError = 1021006,

    /// Description: Can not run AI voice changer on this device.<br>Cause: AI voice changer feature can not run on this device because of poor perforamance.<br>Solutions: Do not use AI voice changer feature on this device.
    ZegoErrorCodeAIVoiceChangerDeviceNotSupportedError = 1021007,

};

#endif /* ZegoExpressErrorCode_h */
