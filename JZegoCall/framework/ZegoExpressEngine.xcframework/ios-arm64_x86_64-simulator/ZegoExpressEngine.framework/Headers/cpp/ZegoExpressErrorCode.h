

#ifndef __ZEGOEXPRESSERRORCODE_H__
#define __ZEGOEXPRESSERRORCODE_H__

namespace ZEGO {
namespace EXPRESS {

enum ZegoErrorCode {
    /// Execution successful.
    ZEGO_ERROR_CODE_COMMON_SUCCESS = 0,

    /// Description: The engine is not initialized and cannot call non-static functions. <br>Cause: Engine not created.<br>Solutions: Please call the [createEngine] function to create the engine first, and then call the current function.
    ZEGO_ERROR_CODE_COMMON_ENGINE_NOT_CREATE = 1000001,

    /// Description: Not logged in to the room, unable to support function implementation. <br>Cause: Not logged in to the room.<br>Solutions: Please call [loginRoom] to log in to the room first, and use related functions during the online period after entering the room.
    ZEGO_ERROR_CODE_COMMON_NOT_LOGIN_ROOM = 1000002,

    /// Description: The audio and video module of the engine is not started and cannot support function realization. <br>Cause: Audio and video modules with no engine started.<br>Solutions: Please call [startPreviewView] [startPublishingStream] [startPlayingStream] to start the audio and video module first.
    ZEGO_ERROR_CODE_COMMON_ENGINE_NOT_STARTED = 1000003,

    /// Description: Call functions that are not supported on the current system/platform. <br>Cause: For example, calling the function of setting the Android context environment on a non-Android system.<br>Solutions: Check if the system environment matches.
    ZEGO_ERROR_CODE_COMMON_UNSUPPORTED_PLATFORM = 1000006,

    /// Description: Invalid Android context. <br>Cause: Not set or set the wrong Android context environment.<br>Solutions: Set the correct Android context.
    ZEGO_ERROR_CODE_COMMON_INVALID_ANDROID_ENVIRONMENT = 1000007,

    /// Description: `setEventHandler` has been called to set the handler, please do not set it repeatedly. <br>Cause: Call `setEventHandler` repeatedly to set the handler.<br>Solutions: If you need to repeat the settings, please call `setEventHandler` first to empty the previous handler.
    ZEGO_ERROR_CODE_COMMON_EVENT_HANDLER_EXISTS = 1000008,

    /// Description: The current SDK does not support this feature. <br>Cause: The SDK version used does not include this feature.<br>Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_COMMON_SDK_NO_MODULE = 1000010,

    /// Description: The input streamID is too long. <br>Cause: The length of the streamID parameter passed in when calling [startPublishingStream] or [startPlayingStream] exceeds the limit. <br>Solutions: The streamID should be less than 256 bytes.
    ZEGO_ERROR_CODE_COMMON_STREAM_ID_TOO_LONG = 1000014,

    /// Description: The input StreamID is null. <br>Cause: The streamID parameter passed in when calling [startPublishingStream] or [startPlayingStream] is null or empty string. <br>Solutions: Check whether the streamID parameter passed in when calling the function is normal.
    ZEGO_ERROR_CODE_COMMON_STREAM_ID_NULL = 1000015,

    /// Description: The input streamID contains invalid characters. <br>Cause: The streamID parameter passed in when calling [startPublishingStream] or [startPlayingStream] contains invalid characters. <br>Solutions: Check whether the streamID parameter passed in when calling the function is normal, only support numbers, English characters and '-', '_'.
    ZEGO_ERROR_CODE_COMMON_STREAM_ID_INVALID_CHARACTER = 1000016,

    /// Description: Illegal param.<br>Cause: The parameter is empty, or the parameter is an illegal value. <br>Solutions: Please check param when entered function to make sure it is correct.
    ZEGO_ERROR_CODE_COMMON_ILLEGAL_PARAM = 1000017,

    /// Description: The Input CDN URL is too long. <br>Cause: The length of URL parameter passed in when calling [enablePublishDirectToCDN] or [startPlayingStream] exceeds the limit. <br>Solutions: URL length should be less than 1024 bytes.
    ZEGO_ERROR_CODE_COMMON_CDN_URL_TOO_LONG = 1000018,

    /// Description: The Input CDN auth param is too long. <br>Cause: The length of auth parameter passed in when calling [enablePublishDirectToCDN] or [startPlayingStream] exceeds the limit. <br>Solutions: Auth param length should be less than 1024 bytes.
    ZEGO_ERROR_CODE_COMMON_CDN_AUTH_PARAM_TOO_LONG = 1000019,

    /// Description: When login room or login scene, userID or user name is different. <br>Cause: Login room, login scene use different user id or user name. <br> Solutions: Use same user id and user name when login room, login scene.
    ZEGO_ERROR_CODE_COMMON_USER_NOT_SAME = 1000020,

    /// Description: This AppID has been removed from production. <br>Solutions: Please check the status of the AppID on the ZEGO official website console or contact ZEGO technical support.
    ZEGO_ERROR_CODE_COMMON_APP_OFFLINE_ERROR = 1000037,

    /// Description: The backend configuration of the server is incorrect. <br>Cause: 1. The domain name configuration is incorrect; 2. The media network is abnormal; 3. The media network link is empty. <br>Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_COMMON_APP_FLEXIABLE_CONFIG_ERROR = 1000038,

    /// Description: Server access denied. <br>Cause: 1.The APP has enabled the restriction of access from foreign IP addresses, and the current client is outside of the domain. <br>Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_COMMON_APP_ACCESS_DENIED_ERROR = 1000039,

    /// Description: Incorrect CDN address. <br>Cause: The set CDN URL is not a standard RTMP or FLV protocol. <br>Solutions: Please check the supported protocol and format.
    ZEGO_ERROR_CODE_COMMON_CDN_URL_INVALID = 1000055,

    /// DNS resolution failed. Please check network configurations. This error code is deprecated.
    ZEGO_ERROR_CODE_COMMON_DNS_RESOLVE_ERROR = 1000060,

    /// Server dispatching exception. Please contact ZEGO technical support to solve the problem. This error code is deprecated.
    ZEGO_ERROR_CODE_COMMON_DISPATCH_ERROR = 1000065,

    /// Description: The engine audio and video module has been activated, and this setting is not supported. <br>Cause: Only supports setting before starting the audio and video module of the engine. <br>Solutions: Please set it before calling [startPreviewView] [startPublishingStream] [startPlayingStream] to start the audio and video module.
    ZEGO_ERROR_CODE_COMMON_CONFIG_AFTER_ENGINE_STARTED = 1000066,

    /// Description: The room is logged in, this setting is not supported. <br>Cause: Only supports setting before logging into the room. <br>Solutions: Please set it before calling [loginRoom] or after calling [logoutRoom]. Note that if you log in to multiple rooms, you need to log out of all rooms before setting.
    ZEGO_ERROR_CODE_COMMON_CONFIG_AFTER_ROOM_LOGGED_IN = 1000067,

    /// Description: Failed to load SDK dynamic library. <br>Cause: Incorrect SDK dynamic path passed in. <br>Solutions: Please pass in the correct SDK dynamic library path.
    ZEGO_ERROR_CODE_COMMON_LOAD_LIBRARY_FAILED = 1000070,

    /// Description: Failed to obtain SDK export function when loading SDK dynamic library. <br>Cause: SDK dynamic library version does not match. <br>Solutions: Please load the matching version of the SDK dynamic library.
    ZEGO_ERROR_CODE_COMMON_LOAD_LIBRARY_FUNC_NOT_FOUND = 1000071,

    /// Description: This call is not supported. <br>Cause: ZEGOEXP_EXPLICIT macro is not enabled. <br>Solutions: Please add the ZEGOEXP_EXPLICIT macro to the preprocessor.
    ZEGO_ERROR_CODE_COMMON_LOAD_LIBRARY_NOT_SUPPORT = 1000072,

    /// Description: SDK internal null pointer error. <br>Cause: The Android JVM environment is abnormal. <br>Solutions: Please check whether the Android JVM environment is normal or contact ZEGO technical support.
    ZEGO_ERROR_CODE_COMMON_INNER_NULLPTR = 1000090,

    /// AppID cannot be 0. Please check if the AppID is correct.
    ZEGO_ERROR_CODE_ENGINE_APPID_ZERO = 1001000,

    /// The length of the input AppSign must be 64 bytes.
    ZEGO_ERROR_CODE_ENGINE_APPSIGN_INVALID_LENGTH = 1001001,

    /// The input AppSign contains invalid characters. Only '0'-'9', 'a'-'f', 'A'-'F' are valid.
    ZEGO_ERROR_CODE_ENGINE_APPSIGN_INVALID_CHARACTER = 1001002,

    /// The input AppSign is empty.
    ZEGO_ERROR_CODE_ENGINE_APPSIGN_NULL = 1001003,

    /// Description: Authentication failed. <br>Cause: Incorrect AppID; using AppID in wrong environment. <br>Solutions: Please check AppID is correct or not on ZEGO manage console, or check whether the environment configured by AppID is consistent with the environment set by SDK.
    ZEGO_ERROR_CODE_ENGINE_APPID_INCORRECT_OR_NOT_ONLINE = 1001004,

    /// Description: Authentication failed. <br>Cause: Incorrect AppSign. <br>Solutions: Please check AppSign is correct or not on ZEGO manage console.
    ZEGO_ERROR_CODE_ENGINE_APPSIGN_INCORRECT = 1001005,

    /// The input License is empty.
    ZEGO_ERROR_CODE_ENGINE_LICENSE_NULL = 1001006,

    /// Description: No write permission to the log file. <br>Cause: App has no write permission to log file folder. <br>Solutions: Please check has grant write permission to App or not; check log folder is protected or not.
    ZEGO_ERROR_CODE_ENGINE_LOG_NO_WRITE_PERMISSION = 1001014,

    /// Description: The log file path is too long. <br>Cause: The length of log file path exceeds limit. <br>Solutions: Please check the length of log file path.
    ZEGO_ERROR_CODE_ENGINE_LOG_PATH_TOO_LONG = 1001015,

    /// Description: Set room mode failed. <br>Cause: Set room mode after initialize the SDK. <br>Solutions: Please set room mode before initialize the SDK.
    ZEGO_ERROR_CODE_ENGINE_SET_ROOM_MODE_ERROR_TIME = 1001020,

    /// Description: Set geo fence failed. <br>Cause: Set geo fence after initialize the SDK. <br>Solutions: Please set geo fence before initialize the SDK.
    ZEGO_ERROR_CODE_ENGINE_SET_GEO_FENCE_ERROR_TIME = 1001021,

    /// Description: The experimental API json parameter parsing failed. <br>Cause: Invalid json format; wrong function name or parameter. <br>Solutions: Please check json format is valid or not; check function name or parameter is correct or not, contact ZEGO technical support for specific function name and parameters.
    ZEGO_ERROR_CODE_ENGINE_EXPERIMENTAL_JSON_STR_INVALID = 1001091,

    /// Description: The number of rooms the user attempted to log into simultaneously exceeds the maximum number allowed. Currently, a user can only be logged in to one main room.<br>Cause: In single-room mode, log in to multiple main rooms at the same time (including repeated calls to log in to the same room A without exiting room A). <br>Solutions: Please check is login multiple rooms simultaneously or not under single room mode.
    ZEGO_ERROR_CODE_ROOM_COUNT_EXCEED = 1002001,

    /// Description: Haven't login with the input room ID.<br>Cause: Haven't login with the input room ID before call [logoutRoom] or [switchRoom] or [renewToken] or [setRoomExtraInfo]. <br>Solutions: Please check has login with the room ID or not.
    ZEGO_ERROR_CODE_ROOM_ROOMID_INCORRECT = 1002002,

    /// Description: The input user ID is empty.<br>Cause: The input user ID is empty. <br>Solutions: Please check the input user ID is empty or not.
    ZEGO_ERROR_CODE_ROOM_USER_ID_NULL = 1002005,

    /// Description: The input user ID contains invalid characters.<br>Cause: The input user ID contains invalid characters. <br>Solutions: User ID can only contains numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', ',', '.', '<', '>', '\'.
    ZEGO_ERROR_CODE_ROOM_USER_ID_INVALID_CHARACTER = 1002006,

    /// The input user ID is too long. <br>The length of the user ID input by the [loginRoom] function is greater than or equal to 64 bytes. <br>Please check the user ID entered when calling the [loginRoom] function to ensure that its length is less than 64 bytes.
    ZEGO_ERROR_CODE_ROOM_USER_ID_TOO_LONG = 1002007,

    /// The input user name is empty. <br>The user name entered by the [loginRoom] function is empty. <br>Please check the user name entered when calling the [loginRoom] function to make sure it is not empty.
    ZEGO_ERROR_CODE_ROOM_USER_NAME_NULL = 1002008,

    /// The input user name contains invalid characters. <br>The user name entered by the [loginRoom] function contains illegal characters.<br>Please check the user name entered when calling the [loginRoom] function to ensure that it is only contain numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', ',', '.', '<', '>', '/', '\'.
    ZEGO_ERROR_CODE_ROOM_USER_NAME_INVALID_CHARACTER = 1002009,

    /// The input user name is too long. <br>The length of the user name input by the [loginRoom] function is greater than or equal to 256 bytes. <br>Please check the user name entered when calling the [loginRoom] function to ensure that its length is less than 256 bytes.
    ZEGO_ERROR_CODE_ROOM_USER_NAME_TOO_LONG = 1002010,

    /// The input room ID is empty. <br>The room ID entered by the [loginRoom] function is empty. <br>Please check the room ID entered when calling the [loginRoom] function to make sure it is not empty.
    ZEGO_ERROR_CODE_ROOM_ROOMID_NULL = 1002011,

    /// The input room ID contains invalid characters. <br>The room ID entered by the [loginRoom] function contains illegal characters.<br>Please check the room ID entered when calling the [loginRoom] function to ensure that it is only contain numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', ',', '.', '<', '>', '\'.
    ZEGO_ERROR_CODE_ROOM_ROOMID_INVALID_CHARACTER = 1002012,

    /// The input room ID is too long. <br>The length of the room ID input by the [loginRoom] function is greater than or equal to 128 bytes. <br>Please check the room ID entered when calling the [loginRoom] function to ensure that its length is less than 128 bytes.
    ZEGO_ERROR_CODE_ROOM_ROOMID_TOO_LONG = 1002013,

    /// The key for room extra info is empty. <br>The key for room extra info entered by the [setRoomExtraInfo] function is empty. <br>Please check the key for room extra info entered when calling the [setRoomExtraInfo] function to make sure it is not empty.
    ZEGO_ERROR_CODE_ROOM_ROOM_EXTRA_INFO_KEY_EMPTY = 1002014,

    /// The key for room extra info is too long. <br>The length of the key for room extra info input by the [setRoomExtraInfo] function is greater than or equal to 128 bytes. <br>Please check the key for room extra info entered when calling the [setRoomExtraInfo] function to ensure that its length is less than 128 bytes.
    ZEGO_ERROR_CODE_ROOM_ROOM_EXTRA_INFO_KEY_TOO_LONG = 1002015,

    /// The value for room extra info is too long. <br>The length of the value for room extra info input by the [setRoomExtraInfo] function is greater than or equal to 128 bytes. <br>Please check the value for room extra info entered when calling the [setRoomExtraInfo] function to ensure that its length is less than 128 bytes.
    ZEGO_ERROR_CODE_ROOM_ROOM_EXTRA_INFO_VALUE_TOO_LONG = 1002016,

    /// Description: The number of keys set in the room additional message exceeds the maximum number of supported limits. <br>Cause: called setRoomExtraInfo Different keys have been passed in multiple times. <br> Solutions: please contact ZEGO technical support.
    ZEGO_ERROR_CODE_ROOM_ROOM_EXTRA_INFO_EXCEED_KEYS = 1002017,

    /// Description: set multi room mode, userID or user name is different. <br>Cause: set multi room mode, login multi room use different user id or user name. <br> Solutions: Currently supports at most one key, if you need to support multiple, contact ZEGO technical support.
    ZEGO_ERROR_CODE_ROOM_MULTI_ROOM_LOGIN_USER_NOT_SAME = 1002018,

    /// Description: The [switchRoom] function cannot be used in multi-room mode. <br>Cause: multi room mode SDK not support. <br> Solutions: first call [logoutRoom] then call [loginRoom].
    ZEGO_ERROR_CODE_ROOM_MULTI_ROOM_SWTICH_ROOM_INVALID = 1002019,

    /// Description: Login failed, possibly due to network problems. <br>Cause: The current network is abnormal. <br> Solutions: It is recommended to switch the network to try.
    ZEGO_ERROR_CODE_ROOM_ERROR_CONNECT_FAILED = 1002030,

    /// Description: Login timed out, possibly due to network problems. <br>Cause: The current network delay is large. <br> Solutions: It is recommended to switch the network to try.
    ZEGO_ERROR_CODE_ROOM_ERROR_LOGIN_TIMEOUT = 1002031,

    /// Description: Room login authentication failed. <br>Cause: login set token error or token expired. <br> Solutions: set new token.
    ZEGO_ERROR_CODE_ROOM_ERROR_AUTHENTICATION_FAILED = 1002033,

    /// Description: The number of users logging into the room exceeds the maximum number of concurrent users configured for the room. (In the test environment, the default maximum number of users in the room is 50). <br>Cause: too much user in room. <br> Solutions: contact ZEGO technical support.
    ZEGO_ERROR_CODE_ROOM_ERROR_EXCEED_MAXIMUM_MEMBER = 1002034,

    /// Description: in test environment The total number of rooms logged in at the same time exceeds the limit. (In the test environment, the maximum number of concurrent rooms is 10). <br>Cause: Too many rooms logged in at the same time. <br> Solutions: logout some room, login room.
    ZEGO_ERROR_CODE_ROOM_ERROR_EXCEED_MAXIMUM_ROOM_COUNT = 1002035,

    /// Description: login failed, multi-room mode is not activate. <br>Cause: multi-room mode is not activate. <br> Solutions: please contact ZEGO technical support.
    ZEGO_ERROR_CODE_ROOM_ERROR_LOGIN_MULTI_ROOM_NOT_OPEN = 1002036,

    /// The total number of rooms logged in at the same time exceeds the limit, Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_ROOM_ERROR_MULTI_ROOM_EXCEED_MAXIMUM_ROOM_COUNT = 1002037,

    /// Description: The user was kicked out of the room. <br>Cause: Possibly because the same user ID is logged in on another device. <br>Solutions: Use a unique user ID.
    ZEGO_ERROR_CODE_ROOM_KICKED_OUT = 1002050,

    /// Description: Room connection is temporarily interrupted and is retrying. <br>Cause: Possibly due to network problems. <br>Solutions: Please wait or check whether the network is normal.
    ZEGO_ERROR_CODE_ROOM_CONNECT_TEMPORARY_BROKEN = 1002051,

    /// Description: Room connection disconnected. <br>Cause: Possibly due to network problems. <br>Solutions: Please check whether the network is working or switch the network environment.
    ZEGO_ERROR_CODE_ROOM_DISCONNECT = 1002052,

    /// Description: Room login retry has exceeded the maximum retry time. <br>Cause: Possibly due to network problems. <br>Solutions: Please check whether the network is working or switch the network environment.
    ZEGO_ERROR_CODE_ROOM_RETRY_TIMEOUT = 1002053,

    /// Description: The business server has sent a signal to kick the user out of the room. <br>Cause: The business server has sent a signal to kick the user out of the room. <br>Solutions: Contact the business server for processing.
    ZEGO_ERROR_CODE_ROOM_MANUAL_KICKED_OUT = 1002055,

    /// Description: The business server has sent a signal to kick the user out of the room. <br>Cause: The business server has sent a signal to kick the user out of the room because the token expired. <br>Solutions: Please update the token in time after receiving the callback notification that the token is about to expire.
    ZEGO_ERROR_CODE_ROOM_TOKEN_EXPIRED_KICKED_OUT = 1002056,

    /// Description: The business server has sent a signal to kick the user out of the room. <br>Cause: The business server has sent a signal to kick the user out of the room because the internal session was abnormal. <br>Solutions: Contact the business server for processing.
    ZEGO_ERROR_CODE_ROOM_SESSION_EXCEPTION_KICKED_OUT = 1002057,

    /// Description: Wrong order of login rooms. <br>Cause: Log in multi room without log in the main room. <br>Solutions: Please log in to the main room with `loginRoom` before logging in to multi room.
    ZEGO_ERROR_CODE_ROOM_WRONG_LOGIN_SEQUENCE = 1002061,

    /// Description: Wrong order of logout rooms. <br>Cause: Log out the main room without log out multi room. <br>Solutions: Please log out of the multi room before logging out of the main room.
    ZEGO_ERROR_CODE_ROOM_WRONG_LOGOUT_SEQUENCE = 1002062,

    /// Description: No multi-room permission. <br>Cause: No multi-room permission. <br>Solutions: Please contact ZEGO technical support to enable it.
    ZEGO_ERROR_CODE_ROOM_NO_MULTI_ROOM_PERMISSION = 1002063,

    /// Description: The room ID has been used by other login room interface. Current user can not login room with the room ID before release the room ID. <br>Cause: The room ID has been used by other login room interface. <br>Solutions: Logout the room with the same room ID first.
    ZEGO_ERROR_CODE_ROOM_ROOM_ID_HAS_BEEN_USED = 1002064,

    /// Description: This method has been deprecated after version 2.9.0. <br>Cause: This method has been deprecated after version 2.9.0. <br>Solutions: Please set [setRoomMode] to select multi-room mode before the engine started, and then call [loginRoom] to use multi-room.
    ZEGO_ERROR_CODE_ROOM_MULTI_ROOM_DEPRECATED = 1002065,

    /// Description: If the user is in the server blacklist when logging in to the room, this error code will be returned, indicating that it is forbidden to log in to the room. <br>Cause: The user is currently in the server blacklist. <br>Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_ROOM_USER_IN_BLACKLIST = 1002066,

    /// Description: This error code is returned if the user does not pass [AppSign] when creating the engine or [Token] when logging in to the room. <br>Cause: The Token is not transmitted in the login room. <br>Solutions: Set the [Token] when you login to the room.
    ZEGO_ERROR_CODE_ROOM_NO_TOKEN = 1002067,

    /// Description: The interface is called more frequently than the upper limit. <br>Cause: This application calls this interface too often. <br>Solutions: Please control the frequency of calls to this interface. Please refer to https://docs.zegocloud.com/article/7612 for details.
    ZEGO_ERROR_CODE_ROOM_APP_CALL_API_TOO_FREQUENT = 1002071,

    /// Description: The frequency of calls to the interface by users in the room exceeds the upper limit. <br>Cause: The interface is called too often by users in the room. <br>Solutions: Please control the frequency of calls to this interface by users in the room. Please refer to https://docs.zegocloud.com/article/7612 for details.
    ZEGO_ERROR_CODE_ROOM_ROOM_CALL_API_TOO_FREQUENT = 1002072,

    /// Description: Room signalling type interfaces are called more frequently than the upper limit. <br>Cause: This application calls the room signalling type interface too often. (e.g., [sendCustomCommand] [sendBroadcastMessage]) <br>Solutions: Please control the frequency of application calls to the room signalling type interface. Please refer to https://docs.zegocloud.com/article/7612 for details.
    ZEGO_ERROR_CODE_ROOM_APP_CALL_TOO_FREQUENT = 1002073,

    /// Description: Token inner error. <br>Cause:  Unknown internal error. <br>Solutions: Contact ZEGO technical support to deal with it..
    ZEGO_ERROR_CODE_ROOM_TOKEN_INNER_ERROR = 1002074,

    /// Description: Token Format error. <br>Cause: The generated Token transmission parameter payload is in non-JSON format. <br>Solutions: The payload is correctly transmitted in json format when the Token is generated.
    ZEGO_ERROR_CODE_ROOM_TOKEN_FORMAT_ERROR = 1002075,

    /// Description: Token Appid error. <br>Cause: The generated Token Appid is inconsistent with the SDK used Appid, Or the appID type is incorrect. <br>Solutions: Use the Appid of the generated Token.
    ZEGO_ERROR_CODE_ROOM_TOKEN_APPID_ERROR = 1002076,

    /// Description: Token userID error. <br>Cause: The generated Token userID is inconsistent with the login used userID Or the userID type is incorrect. <br>Solutions: Login using the userID that generates the Token.
    ZEGO_ERROR_CODE_ROOM_TOKEN_USERID_ERROR = 1002077,

    /// Description: Token expire. <br>Cause: Token expire Or the generated Token validity period parameter type is incorrect. <br>Solutions: Regenerate the Token.
    ZEGO_ERROR_CODE_ROOM_TOKEN_EXPIRE_ERROR = 1002078,

    /// Description: Token version error. <br>Cause: Description The Token Version was generated incorrectly. <br>Solutions: Generate the Token with the correct version.
    ZEGO_ERROR_CODE_ROOM_TOKEN_VERSION_ERROR = 1002079,

    /// Description: The Token Nonce parameter type is incorrect. <br>Cause: The nonce parameter type of the generated Token is incorrect. <br>Solutions: Ensure that nonce is int64 data.
    ZEGO_ERROR_CODE_ROOM_TOKEN_NONCETYPE_ERROR = 1002080,

    /// Description: Multi-room mode required. <br>Cause: Wrong room mode been used, e.g. single-room. <br>Solutions: Please use [setRoomMode] to select multi-room mode before the engine started.
    ZEGO_ERROR_CODE_ROOM_MULTI_ROOM_REQUIRED = 1002081,

    /// Description: Update room Token has no push permission. <br>Cause: Token Does not have the push permission. <br>Solutions: Pass in the Token with the push permission.
    ZEGO_ERROR_CODE_ROOM_RENEW_TOKEN_NO_PUBLISH_AUTH_ERROR = 1002082,

    /// Description: The length of the custom data attached during login exceeded the limit. Procedure. <br>Cause: The login custom callback data is too long. <br>Solutions: The length of user-defined login data cannot exceed 128 bytes.
    ZEGO_ERROR_CODE_ROOM_ROOM_LOGIN_CUSTOM_DATA_LENGTH_ERROR = 1002083,

    /// Description: login room used license auth, but can't find the license on zego server. <br>Cause: input wrong license. <br>Solutions: used the right license.
    ZEGO_ERROR_CODE_ROOM_ROOM_LOGIN_LICENSE_NOT_FOUND = 1002084,

    /// Description: login room used license auth, but the license is expired.  <br>Solutions:  Contact ZEGO technical support to deal with it.
    ZEGO_ERROR_CODE_ROOM_ROOM_LOGIN_LICENSE_EXPIRED = 1002085,

    /// Description: The user has logged in elsewhere.  <br>Solutions:  Check whether the user has logged in elsewhere.
    ZEGO_ERROR_CODE_ROOM_ROOM_LOGIN_OTHER = 1002086,

    /// Description: License authentication is used, and when using the relevant function, the license is configured with a functional limitation that does not allow the use of the function.  <br>Solutions:  Proofread the license's functionality limitations and use the functionality appropriately.
    ZEGO_ERROR_CODE_ROOM_ROOM_LICENSE_FEATURE_LIMIT = 1002087,

    /// Description: Room login failed due to internal system exceptions.<br>Cause: Unknown internal error.<br>Solutions: Contact ZEGO technical support to deal with it.
    ZEGO_ERROR_CODE_ROOM_INNER_ERROR = 1002099,

    /// Description: Publishing failed due to no data in the stream.<br>Cause: No data in the stream.<br>Solutions: Check whether the video, audio capture module is working properly.
    ZEGO_ERROR_CODE_PUBLISHER_PUBLISH_STREAM_FAILED = 1003001,

    /// Description: Publishing failed due to wrong bitrate setting.<br>Cause: The set video bitrate, audio bitrate, or minimum video bitrate threshold for traffic control exceeds the upper limit.<br>Solutions: Please check if the bitrate value is in the correct unit (kbps).Adjust the bitrate setting.
    ZEGO_ERROR_CODE_PUBLISHER_BITRATE_INVALID = 1003002,

    /// Description: The property param of the traffic control is set incorrectly.<br>Cause: The property param of the traffic control is less than 0 or exceeds all combinations.<br>Solutions: Check the settings of the property param of the traffic control.
    ZEGO_ERROR_CODE_PUBLISHER_TRAFFIC_MODE_INVALID = 1003005,

    /// Description: Streaming failed, H.265 encoding is not supported.<br>Cause: The hardware device does not support H.265 encoding, or the SDK does not include H.265 encoding module.<br>Solutions: Contact ZEGO technical support to confirm whether the SDK contains the H.265 encoding module, if the hardware device does not support it, it is recommended to upgrade the hardware.
    ZEGO_ERROR_CODE_PUBLISHER_ERROR_H265_ENCODER_NOT_SUPPORTED = 1003010,

    /// Description: The performance of object segmentation device is not enough.<br>Caution: The performance of device is not enough.<br>Solutions: Please replace the device.
    ZEGO_ERROR_CODE_PUBLISHER_OBJECT_SEGMENTATION_PERFORMANCE_WARNING = 1003011,

    /// Description: This device does not support object segmentation.<br>Caution: The device is not supported, or the object segmentation of multiple channels is enabled.<br>Solutions: Please change the device or only open the object segmentation of one channel.
    ZEGO_ERROR_CODE_PUBLISHER_OBJECT_SEGMENTATION_DEVICE_NOT_SUPPORT = 1003012,

    /// Description: Object segmentation unauthorized.<br>Caution: The appid is not configured with object segmentation.<br>Solutions: Please contact ZEGO technical support to configure the object segmentation function.
    ZEGO_ERROR_CODE_PUBLISHER_OBJECT_SEGMENTATION_NO_AUTH = 1003013,

    /// Description: Object segmentation parameter error.<br>Caution: Object segmentation is set to green screen type, background blur is not allowed.<br>Solutions: Please use the correct parameters to enable object segmentation.
    ZEGO_ERROR_CODE_PUBLISHER_OBJECT_SEGMENTATION_PARAM_ERROR = 1003014,

    /// Description: Object segmentation background loading error.<br>Caution: The background video or image path is incorrect, or the content is incorrect..<br>Solutions: Please set the correct background image or video.
    ZEGO_ERROR_CODE_PUBLISHER_OBJECT_SEGMENTATION_IMAGE_LOAD_ERROR = 1003015,

    /// Description:Stream publishing is temporarily interrupted and is retrying. <br>Cause: The network fluctuates or the network signal is bad.<br>Solutions: Please wait or check whether the network is normal.
    ZEGO_ERROR_CODE_PUBLISHER_ERROR_NETWORK_INTERRUPT = 1003020,

    /// Description: Stream publish retry has exceeds the maximum retry time.<br>Cause: The the network signal is bad, and the maximum retry time is exceeded.<br>Solutions: Check the network status or switch to another network.
    ZEGO_ERROR_CODE_PUBLISHER_ERROR_RETRY_TIMEOUT = 1003021,

    /// Description: Failed to publish the stream. The publish channel is already publishing streams.<br>Cause:  The publish channel is already publishing streams.<br>Solutions: Please check the business logic to avoid repeating the publish for publish channel which is publishing.
    ZEGO_ERROR_CODE_PUBLISHER_ERROR_ALREADY_DO_PUBLISH = 1003023,

    /// Description: Failed to publish the stream. Publishing of this stream is prohibited by backend configuration.<br>Cause: Publishing of this stream is prohibited by backend configuration.<br>Solutions: Contact ZEGO technical support to deal with it.
    ZEGO_ERROR_CODE_PUBLISHER_ERROR_SERVER_FORBID = 1003025,

    /// Description: Failed to publish the stream. The same stream already exists in the room.<br>Cause: The same stream already exists in the room.<br>Solutions: Replace with a new stream ID. Adjust the stream ID generation strategy to ensure uniqueness.
    ZEGO_ERROR_CODE_PUBLISHER_ERROR_REPETITIVE_PUBLISH_STREAM = 1003028,

    /// Description: Failed to publish the stream. The connection to the RTMP server is interrupted.<br>Cause: The publish address is wrong, or the network signal is bad.<br>Solutions: Please check whether there is any problem with the network connection or the stream publishing URL.
    ZEGO_ERROR_CODE_PUBLISHER_RTMP_SERVER_DISCONNECT = 1003029,

    /// Description: Failed to take a screenshot of the publis stream screen. <br>Cause: The preview is stopped and the push is abnormal. <br>Solutions: Turn on preview or re-publish.
    ZEGO_ERROR_CODE_PUBLISHER_TAKE_PUBLISH_STREAM_SNAPSHOT_FAILED = 1003030,

    /// Description: Failed to update the relay CDN status. <br>Cause: The URL of the relay address is incorrect. <br>Solutions: Check whether the input URL is valid.
    ZEGO_ERROR_CODE_PUBLISHER_UPDATE_CDN_TARGET_ERROR = 1003040,

    /// Description: Failed to send SEI. <br>Cause: data is empty. <br>Solutions: Incoming non-empty data.
    ZEGO_ERROR_CODE_PUBLISHER_SEI_DATA_NULL = 1003043,

    /// Description: Failed to send SEI. <br>Cause: The input data exceeds the length limit. <br>Solutions: The length of the sent SEI data should be less than 4096 bytes.
    ZEGO_ERROR_CODE_PUBLISHER_SEI_DATA_TOO_LONG = 1003044,

    /// Description: Failed to send audio side info. <br>Cause: data is empty. <br>Solutions: Incoming non-empty data.
    ZEGO_ERROR_CODE_PUBLISHER_AUDIO_SIDE_DATA_NULL = 1003045,

    /// Description: Failed to send audio side info. <br>Cause: The input data exceeds the length limit. <br>Solutions: The length of the sent audio side data should be less than 1024 bytes.
    ZEGO_ERROR_CODE_PUBLISHER_AUDIO_SIDE_DATA_TOO_LONG = 1003046,

    /// The extra info of the stream is null.
    ZEGO_ERROR_CODE_PUBLISHER_EXTRA_INFO_NULL = 1003050,

    /// The extra info of the stream is too long. The maximum length should be less than 1024 bytes.
    ZEGO_ERROR_CODE_PUBLISHER_EXTRA_INFO_TOO_LONG = 1003051,

    /// Failed to update the extra info of the stream. Please check the network connection.
    ZEGO_ERROR_CODE_PUBLISHER_UPDATE_EXTRA_INFO_FAILED = 1003053,

    /// Description: Failed to set publish watermark. <br>Cause: The incoming watermark path is empty. <br>Solutions: Incoming non-empty path.
    ZEGO_ERROR_CODE_PUBLISHER_WATERMARK_URL_NULL = 1003055,

    /// Description: Failed to set publish watermark. <br>Cause: The incoming watermark path exceeds the byte size limit. <br>Solutions: The incoming watermark path should be less than 512 bytes.
    ZEGO_ERROR_CODE_PUBLISHER_WATERMARK_URL_TOO_LONG = 1003056,

    /// Description: Failed to set publish watermark. <br>Cause: The incoming watermark path was entered incorrectly or the image format is not supported. <br>Solutions: Incoming the correct watermark path, only `jpg` and `png` image formats are supported.
    ZEGO_ERROR_CODE_PUBLISHER_WATERMARK_URL_INVALID = 1003057,

    /// Description: Incorrect watermark layout.<br>Caution: The layout area exceed the encoding resolution.<br>Solutions: Make sure the layout area cannot exceed the encoding resolution and call current interface.
    ZEGO_ERROR_CODE_PUBLISHER_WATERMARK_LAYOUT_INVALID = 1003058,

    /// Description: The publish stream encryption key is invalid.<br>Caution: The set encryption key length is not supported.<br>Solutions: The Publish-stream encryption key length to be 16/24/32 bytes.
    ZEGO_ERROR_CODE_PUBLISHER_ENCRYPTION_KEY_INVALID = 1003060,

    /// Description: StartPlayingStream failed.<br>Caution: In multi-room mode, the publish-stream function is called incorrectly.<br>Solutions: In multi-room mode, A publish-stream function with the parameter 'ZegoPublisherConfig' needs to be called.
    ZEGO_ERROR_CODE_PUBLISHER_ERROR_PUBLISH_WHEN_USING_MULTI_ROOM = 1003070,

    /// Description: StartPlayingStream failed.<br>Caution: In multi-room mode, the publish-stream function is called incorrectly.<br>Solutions: In multi-room mode, A publish-stream function parameter 'roomID' cannot be empty.
    ZEGO_ERROR_CODE_PUBLISHER_ERROR_PUBLISH_WITH_ROOM_ID_IS_NULL_WHEN_USING_MULTI_ROOM = 1003071,

    /// Description: Push-pull flow authentication is incorrect. <br>Caution: An [appSign] error was passed when creating the engine, or a Token error or timeout was passed when logging into the room. <br>Solutions: Pass the correct [Token] upon login, or invoke [renewToken] when recive [onRoomTokenWillExpire] callback.
    ZEGO_ERROR_CODE_PUBLISHER_ERROR_DISPATCH_AUTH_ERROR = 1003072,

    /// Description: The specified path for displaying static images when the camera is turned off is incorrect. <br>Caution: The path may be misspelled or there may be no read permission.<br>Solutions: Please check if the designated image path is correct and if there is read permission.
    ZEGO_ERROR_CODE_PUBLISHER_DUMMY_CAPTURE_IMAGE_PATH_ERROR = 1003073,

    /// Description: Unsupported video encoder.<br>Caution: There is no selected video encoder in the current SDK.<br>Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_PUBLISHER_VIDEO_ENCODER_NO_SUPPORTTED = 1003080,

    /// Description: Video encoder error.<br>Caution: Video encoder error.<br>Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_PUBLISHER_VIDEO_ENCODER_FAIL = 1003081,

    /// Description: Stream publishing failed due to system internal exceptions.<br>Caution: Stream publishing failed due to system internal exceptions.<br>Solutions: Please contact ZEGO technical support to solve the problem.
    ZEGO_ERROR_CODE_PUBLISHER_INNER_ERROR = 1003099,

    /// Description: Stream playing failed.<br>Caution: Possibly due to no data in the stream.<br>Solutions: Check to see if the publish-stream is working or try to play stream again, and if the problem is still not resolved, please contact ZEGO technical support to solve the problem.
    ZEGO_ERROR_CODE_PLAYER_PLAY_STREAM_FAILED = 1004001,

    /// Description: Stream playing failed.<br>Caution: The stream does not exist.<br>Solutions: Please check whether the remote end publish is indeed successful, or whether the publish and play environment are inconsistent.
    ZEGO_ERROR_CODE_PLAYER_PLAY_STREAM_NOT_EXIST = 1004002,

    /// Description: Play stream authentication is incorrect. <br>Caution: The [Token] error or timeout required to playing stream across APP. <br>Solutions: Playing the stream to pass the correct [Token], or update the [Token] in time.
    ZEGO_ERROR_CODE_PLAYER_ERROR_DISPATCH_AUTH_ERROR = 1004003,

    /// Description: This device does not support super resolution. <br>Caution: The device configured on the server does not support super resolution, or the configuration on the server failed to pull. <br>Solutions: replace the device and try again or contact ZEGO technical support.
    ZEGO_ERROR_CODE_PLAYER_SUPER_RESOLUTION_DEVICE_NOT_SUPPORT = 1004004,

    /// Description: The number of super resolution streams exceeds the limit.<br>Caution: The number of super resolution streams exceeds the limit.<br>Solutions: Currently, super resolution support up to 1 playing steam at the same time.
    ZEGO_ERROR_CODE_PLAYER_SUPER_RESOLUTION_MAX_COUNT_EXCEED = 1004005,

    /// Description: The number of super resolution streams exceeds the limit.<br>Caution: The number of super resolution streams exceeds the limit.<br>Solutions: contact ZEGO technical support.
    ZEGO_ERROR_CODE_PLAYER_SUPER_RESOLUTION_VIDEO_SIZE_NOT_SUPPORT = 1004006,

    /// Description: The performance of super resolution device is not enough.<br>Caution: The performance of device is not enough.<br>Solutions: Please replace the device.
    ZEGO_ERROR_CODE_PLAYER_SUPER_RESOLUTION_DEVICE_PERFORMANCE_ERROR = 1004007,

    /// Description: Super resolution uninitialized.<br>Caution: Super resolution uninitialized.<br>Solutions: Please initialize super resolution.
    ZEGO_ERROR_CODE_PLAYER_SUPER_RESOLUTION_NOT_INIT_ERROR = 1004008,

    /// Description: Stream playing error.<br>Caution: The number of streams the user attempted to play simultaneously exceeds the maximum number allowed.<br>Solutions: Currently, up to 12 steams can be played at the same time. Please contact ZEGO technical support to increase the capacity if necessary.
    ZEGO_ERROR_CODE_PLAYER_COUNT_EXCEED = 1004010,

    /// Description: The stream was not played.<br>Caution: The stream was not played.<br>Solutions: Please confirm that the stream has been played through the [onPlayerStateUpdate] or [onUserStreamStateUpdate] callback.
    ZEGO_ERROR_CODE_PLAYER_STREAM_WAS_NOT_PLAYED = 1004011,

    /// Description: Stream playing is temporarily interrupted.<br>Caution: Network exception.<br>Solutions: Please wait or check whether the network is normal.
    ZEGO_ERROR_CODE_PLAYER_ERROR_NETWORK_INTERRUPT = 1004020,

    /// Description: Failed to play the stream.<br>Caution: Publishing of this stream is prohibited by backend configuration.<br>Solutions: Please contact ZEGO technical support to solve the problem.
    ZEGO_ERROR_CODE_PLAYER_ERROR_SERVER_FORBID = 1004025,

    /// Description: Failed to capture the screenshot of the streaming screen, please check whether the stream to be captured is normal. <br>Cause: The stream is not pulled. <br>Solutions: Check whether it starts to play the stream, and whether there is an abnormality in the process of playing the stream.
    ZEGO_ERROR_CODE_PLAYER_TAKE_PLAY_STREAM_SNAPSHOT_FAILED = 1004030,

    /// Description: The play stream decryption key is invalid, the key length only supports 16/24/32 bytes. <br>Cause: The input key length is not 16/24/32 bytes. <br>Solutions: Adjust the input key length to 16/24/32 bytes.
    ZEGO_ERROR_CODE_PLAYER_DECRYPTION_KEY_INVALID = 1004060,

    /// Description: Pull stream decryption failed, please check whether the decryption key is correct. <br>Cause: Incorrect decryption key entered. <br>Solutions: Enter the correct decryption key.
    ZEGO_ERROR_CODE_PLAYER_DECRYPTION_FAILED = 1004061,

    /// Description: Calling the wrong function after enabling the multi-room function causes playing stream fail. <br>Cause: Called the playing stream function that only works for joining a single room mode. <br>Solutions: Please use the function of the same name with ZegoPlayerConfig and specify the room ID to play the stream.
    ZEGO_ERROR_CODE_PLAYER_ERROR_PLAY_STREAM_WHEN_USING_MULTI_ROOM = 1004070,

    /// Description: In the multi-room mode, the roomID parameter of the play stream cannot be empty. <br>Cause: The roomID parameter of the pull stream is empty. <br>Solutions: Please enter the correct roomID.
    ZEGO_ERROR_CODE_PLAYER_ERROR_PLAY_STREAM_WITH_ROOM_ID_IS_NULL_WHEN_USING_MULTI_ROOM = 1004071,

    /// Description: When using the SDK to play the latency of live streaming, this error code will be returned if you have not subscribed to the low latency live streaming service. <br>Cause: Low-latency live broadcast service is not activated. <br>Solutions: Please contact ZEGO technical support staff to open the low-latency live broadcast service.
    ZEGO_ERROR_CODE_PLAYER_NOT_CONFIG_L3 = 1004072,

    /// Description: Unsupported video decoder.<br>Caution: There is no selected video decoder in the current SDK.<br>Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_PLAYER_VIDEO_DECODER_NO_SUPPORTTED = 1004080,

    /// Description: Video decoder fail.<br>Caution: Video decoder fail.<br>Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_PLAYER_VIDEO_DECODER_FAIL = 1004081,

    /// Description: An internal system exception causes a failure to pull the stream. <br>Cause: SDK internal error. <br>Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_PLAYER_INNER_ERROR = 1004099,

    /// Description: Does not support the use of stream mixing service. <br>Cause: No stream mixing service configured. <br>Solutions: Please open the service on the console or contact ZEGO business staff.
    ZEGO_ERROR_CODE_MIXER_NO_SERVICES = 1005000,

    /// Description: The mixing task ID is null. <br>Cause: The mixing task ID input when starting mixing task is empty. <br>Solutions: Please enter the correct mixing task ID.
    ZEGO_ERROR_CODE_MIXER_TASK_ID_NULL = 1005001,

    /// Description: The stream mixing task ID is too long. <br>Cause: The stream mixing task ID is greater than 256 bytes. <br>Solutions: Please enter a mixing task ID less than 256 bytes.
    ZEGO_ERROR_CODE_MIXER_TASK_ID_TOO_LONG = 1005002,

    /// Description: Invalid mixed flow task ID. <br>Cause: Illegal characters in the stream mixing task ID. <br>Solutions: Stream mixing task ID only supports numbers, English characters and'~','!','@','$','%','^','&','*','(',')', '_','+','=','-','`',';',''',',','.','<','>','/','\', please enter the stream mixing task ID in the correct format.
    ZEGO_ERROR_CODE_MIXER_TASK_ID_INVALID_CHARACTER = 1005003,

    /// Description: Illegal parameters exist in mixing task configuration. <br>Cause: 1. The mixing task ID is empty; 2. The mixing room ID is empty; 3. The mixing custom data length exceeds 1000 bytes; 4. The mixing output target stream is empty. <br>Solutions: Please check the configuration parameters of the mixing task.
    ZEGO_ERROR_CODE_MIXER_NO_OUTPUT_TARGET = 1005005,

    /// Description: Illegal format of mixed stream output target parameter. <br>Cause: When the target of the mixed stream output target is streamID, an illegal character is passed in. <br>Solutions: Please check whether the target of the mixed stream output target is of streamID type, if so, target only support numbers, English characters and '-', '_'.
    ZEGO_ERROR_CODE_MIXER_OUTPUT_TARGET_INVALID = 1005006,

    /// Description: Failed to start the stream mixing task. <br>Cause: Requests are too frequent, exceeding the qps limit of the service. Network exceptions. An illegal mixed stream output URL was passed in. <br>Solutions: Please ensure that the qps of the mixing request is less than 100. Check the network. Pass in a valid mixed stream output URL.
    ZEGO_ERROR_CODE_MIXER_START_REQUEST_ERROR = 1005010,

    /// Description: Failed to stop the stream mixing task. <br>Cause: May be the cause of the network error. <br>Solutions: Please check the network ring.
    ZEGO_ERROR_CODE_MIXER_STOP_REQUEST_ERROR = 1005011,

    /// The stream mixing task must be stopped by the user who started the task. This error code is deprecated.
    ZEGO_ERROR_CODE_MIXER_NOT_OWNER_STOP_MIXER = 1005012,

    /// Description: Starts stream mixing tasks too frequently. <br>Cause: Requests are too frequent, exceeding the qps limit of the service. <br>Solutions: Please ensure that the qps of the mixing request is less than 100.
    ZEGO_ERROR_CODE_MIXER_START_QPS_OVERLOAD = 1005015,

    /// Description: Stop stream mixing tasks too frequently. <br>Cause: Requests are too frequent, exceeding the qps limit of the service. <br>Solutions: Please ensure that the qps of the stop mixing request is less than 100.
    ZEGO_ERROR_CODE_MIXER_STOP_QPS_OVERLOAD = 1005016,

    /// Description: The input stream list of the stream mixing task is empty. <br>Cause:  The input stream list of the stream mixing task is empty. <br>Solutions: Please check the input stream list of the mixing task.
    ZEGO_ERROR_CODE_MIXER_INPUT_LIST_INVALID = 1005020,

    /// Description: The output stream list of the stream mixing task is empty. <br>Cause:  The output stream list of the stream mixing task is empty. <br>Solutions: Please check the output stream list of the mixing task.
    ZEGO_ERROR_CODE_MIXER_OUTPUT_LIST_INVALID = 1005021,

    /// Description: The video configuration of the stream mixing task is invalid. <br>Cause:  The video configuration of the stream mixing task is invalid. <br>Solutions: Please check the video configuration of the stream mixing task.
    ZEGO_ERROR_CODE_MIXER_VIDEO_CONFIG_INVALID = 1005023,

    /// Description: The video configuration of the stream mixing task is invalid. <br>Cause: 1. An unsupported audio codec format is used. 2. The audio bit rate exceeds 192 kbps. <br>Solutions: Please check the audio configuration of the stream mixing task.
    ZEGO_ERROR_CODE_MIXER_AUDIO_CONFIG_INVALID = 1005024,

    /// Description: The number of input streams exceeds the maximum number allowed. <br>Cause: Supports up to 9 input streams, and may pass more than 9 input streams. <br>Solutions: Please check the input stream configuration of the mixing task.
    ZEGO_ERROR_CODE_MIXER_EXCEED_MAX_INPUT_COUNT = 1005025,

    /// Description: Failed to start mixed stream. <br>Cause: The input stream does not exist. <br>Solutions: Please make sure that the stream corresponding to the entered streamID is being pushed.
    ZEGO_ERROR_CODE_MIXER_INPUT_STREAM_NOT_EXISTS = 1005026,

    /// Description: Failed to start mixed stream. <br>Cause: The mixed stream input parameter is wrong, it may be that the layout of the input stream exceeds the canvas range. <br>Solutions: Please enter the correct mixed stream parameters. [ZegoMixerTask]
    ZEGO_ERROR_CODE_MIXER_INPUT_PARAMETERS_ERROR = 1005027,

    /// Description: Mixed input text watermark is too long. <br>Cause: The length of the text watermark of the mixed stream input parameter exceeds the limit. <br>Solutions: Please make sure the input text watermark length does not exceed 512 bytes.
    ZEGO_ERROR_CODE_MIXER_INPUT_LABEL_TEXT_TOO_LONG = 1005028,

    /// Description: Mixed stream output target is too long. <br>Cause: The length of the target parameter of the mixed stream output exceeds the limit. <br>Solutions: Please make sure that the output destination length does less than 1024 bytes.
    ZEGO_ERROR_CODE_MIXER_OUTPUT_TARGET_TOO_LONG_ERROR = 1005029,

    /// Description: Failed to start mixed stream. <br>Cause: Exceeding the maximum number of output streams. <br>Solutions: Support up to 3 output streams.
    ZEGO_ERROR_CODE_MIXER_EXCEED_MAX_OUTPUT_COUNT = 1005030,

    /// Description: Failed to start mixed stream. <br>Cause: The maximum number of focus voice input streams is exceeded. <br>Solutions: Support up to 4 input streams to set the focus voice.
    ZEGO_ERROR_CODE_MIXER_EXCEED_MAX_AUDIO_FOCUS_STREAM_COUNT = 1005031,

    /// Description: The mixed-flow advanced configuration is too long. <br>Cause: The length of the mixed stream advanced configuration parameter exceeds the limit. <br>Solutions: Please make sure that the advanced configuration length does not exceed 512 bytes.
    ZEGO_ERROR_CODE_MIXER_ADVANCED_CONFIG_TOO_LONG_ERROR = 1005032,

    /// Description: The mixed stream watermark path is too long. <br>Cause: The length of the mixed stream watermark path parameter exceeds the limit. <br>Solution: Please make sure the watermark path length does not exceed 512 bytes.
    ZEGO_ERROR_CODE_MIXER_WATERMARK_TOO_LONG = 1005033,

    /// Description: The link to the muxed input image is too long. <br>Cause: The length of the image link of the mixed stream input parameter exceeds the limit. <br>Solution: Please make sure that the length of the input image link does not exceed 1024 bytes.
    ZEGO_ERROR_CODE_MIXER_INPUT_IMAGE_URL_TOO_LONG = 1005034,

    /// Description: Failed to mix stream input image. <br>Cause: The image format of the mixed stream input parameter is incorrect. <br>Solution: Use JPG and PNG formats. There are 2 ways to use it: 1. URI: Provide the picture to ZEGO technical support for configuration. After the configuration is complete, the picture URI will be provided, for example: preset-id://xxx.jpg. 2. URL: Only HTTP protocol is supported.
    ZEGO_ERROR_CODE_MIXER_INPUT_IMAGE_URL_FORMAT_ERROR = 1005035,

    /// Description: Failed to mux input image. <br>Cause: The image size of the mixed stream input parameter exceeds the limit. <br>Solution: Image size is limited to 1M.
    ZEGO_ERROR_CODE_MIXER_INPUT_IMAGE_URL_SIZE_ERROR = 1005036,

    /// Description: The mixed output stream target room is misconfigured. <br>Cause: The room doesn't exist when the mixing starts. <br>Solution: Please make sure that the room corresponding to the configured targetRoom exists.
    ZEGO_ERROR_CODE_MIXER_OUTPUT_TARGET_ROOM_ERROR = 1005037,

    /// Description: Failed to start mixed stream. <br>Cause: Mixed-stream authentication failed. <br>Solutions: Contact ZEGO technical support.
    ZEGO_ERROR_CODE_MIXER_AUTHENTICATION_FAILED = 1005050,

    /// Description: Failed to start mixed stream. <br>Cause: The input image watermark is empty. <br>Solutions: Please enter the correct watermark parameters [ZegoWatermark].
    ZEGO_ERROR_CODE_MIXER_WATERMARK_NULL = 1005061,

    /// Description: Failed to start mixed stream. <br>Cause: The input image watermark parameter is wrong, it may be that the layout exceeds the canvas range. <br>Solutions: Please enter the correct watermark parameters [ZegoWatermark].
    ZEGO_ERROR_CODE_MIXER_WATERMARK_PARAMETERS_ERROR = 1005062,

    /// Description: Failed to start mixed stream. <br>Cause: The input watermark URL is illegal. <br>Solutions: The watermark URL must start with `preset-id://` and end with `.jpg` or `.png`.
    ZEGO_ERROR_CODE_MIXER_WATERMARK_URL_INVALID = 1005063,

    /// Description: The mixed stream background image url is too long. <br>Cause: The length of the mixed stream background url parameter exceeds the limit. <br>Solution: Please make sure the background image url length does not exceed 1024 bytes.
    ZEGO_ERROR_CODE_MIXER_BACKGROUND_IMAGE_URL_TOO_LONG = 1005066,

    /// Description: Failed to start mixed stream. <br>Cause: The URL of the background image entered is illegal. <br>Solutions: The URL of the background image must start with preset-id:// and end with `.jpg` or `.png`.
    ZEGO_ERROR_CODE_MIXER_BACKGROUND_IMAGE_URL_INVALID = 1005067,

    /// Description: Failed to start mixed stream. <br>Cause: The user-defined data is too long. <br>Solutions: The maximum length of the custom input should not exceed 1000 bytes.
    ZEGO_ERROR_CODE_MIXER_USER_DATA_TOO_LONG = 1005068,

    /// Description: Failed to start mixed stream. <br>Cause: The auto-mixing server was not found. <br>Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_MIXER_AUTO_MIX_STREAM_SERVER_NOT_FOUND = 1005070,

    /// Description: Stream mixing internal error.<br>Cause: Unknown error occured in stream mixing internal.<br>Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_MIXER_INNER_ERROR = 1005099,

    /// Description: Generic device error.<br>Cause: Device dose not work normally.<br>Solutions: Use the system's video or audio recording application to check whether the device can work normally. If the device is normal, please contact ZEGO technical support.
    ZEGO_ERROR_CODE_DEVICE_ERROR_TYPE_GENERIC = 1006001,

    /// Description: The device ID does not exist.<br>Cause: The device ID is spelled incorrectly, or the corresponding device is unplugged.<br>Solutions: Please use the SDK interface to obtain the device ID, and check whether the device is properly connected.
    ZEGO_ERROR_CODE_DEVICE_ERROR_TYPE_INVALID_ID = 1006002,

    /// Description: No permission to access the device. <br>Cause: Did not apply for or obtain the permission to use the corresponding device.<br>Solutions: Please check whether the application has correctly applied for the camera or microphone permission, and whether the user has granted the corresponding permission.
    ZEGO_ERROR_CODE_DEVICE_ERROR_TYPE_NO_AUTHORIZATION = 1006003,

    /// Description: The frame rate of the capture device is 0.<br>Cause: Device error, or device does not have permission.<br>Solutions: Please use the system's video or audio recording application to check whether the device can work normally. Please check whether the application has correctly applied for the camera or microphone permission, and whether the user has granted the corresponding permission. If the device is normal and the application has obtained the corresponding device permissions, please contact ZEGO technical support.
    ZEGO_ERROR_CODE_DEVICE_ERROR_TYPE_ZERO_FPS = 1006004,

    /// Description: The device is occupied.<br>Cause: The device is occupied by other programs.<br>Solutions: Please use the system's video or audio recording application to check whether the device is working properly and make sure that the device is not occupied by other applications.
    ZEGO_ERROR_CODE_DEVICE_ERROR_TYPE_IN_USE_BY_OTHER = 1006005,

    /// Description: The device is unplugged.<br>Cause: The device is unplugged or not properly connected.<br>Solutions: Check the device wiring and reconnect the device.
    ZEGO_ERROR_CODE_DEVICE_ERROR_TYPE_UNPLUGGED = 1006006,

    /// Description: The device needs to be restarted.<br>Cause: Device driver update, or device error requires restart.<br>Solutions: Restart device.
    ZEGO_ERROR_CODE_DEVICE_ERROR_TYPE_REBOOT_REQUIRED = 1006007,

    /// Description: The device media is lost.<br>Cause: Media service cannot be restored.<br>Solutions: Restart device.
    ZEGO_ERROR_CODE_DEVICE_ERROR_MEDIA_SERVICES_LOST = 1006008,

    /// Description: The device list cannot be empty when trying to release devices.<br>Cause: The device list has been released or has not been initialized.<br>Solutions: Ignore it.
    ZEGO_ERROR_CODE_DEVICE_FREE_DEVICE_LIST_NULL = 1006020,

    /// Description: The set sound level monitoring interval is out of range.<br>Cause: The set sound level monitoring interval is less than 100 milliseconds, or greater than 3000 milliseconds.<br>Solutions: Reset the effective sound level monitoring interval, the effective sound level monitoring interval is [100, 3000], in milliseconds.
    ZEGO_ERROR_CODE_DEVICE_SOULD_LEVEL_INTERVAL_INVALID = 1006031,

    /// Description: The set audio spectrum monitoring interval is out of range.<br>Cause:  The set audio spectrum monitoring interval is less than 10 milliseconds.<br>Solutions: Reset audio spectrum monitoring interval which is not less than 10 milliseconds.
    ZEGO_ERROR_CODE_DEVICE_AUDIO_SPECTRUM_INTERVAL_INVALID = 1006032,

    /// Description: Failed to set the camera zoom. <br>Cause: The set camera zoom factor is out of range. <br>Solutions: The set camera zoom factor cannot exceed the maximum range obtained, the maximum range can be obtained through [getCameraMaxZoomFactor].
    ZEGO_ERROR_CODE_DEVICE_ZOOM_FACTOR_INVALID = 1006040,

    /// Description: Failed to set the camera exposure compensation. <br>Cause: The set camera exposure compensation value is out of range. <br>Solutions: Set the camera exposure compensation range between [-1,1].
    ZEGO_ERROR_CODE_DEVICE_EXPOSURE_COMPENSATION_VALUE_INVALID = 1006041,

    /// invalid audio VAD monitor type
    ZEGO_ERROR_CODE_DEVICE_AUDIO_VAD_STABLE_STATE_MONITOR_TYPE_INVALID = 1006042,

    /// Description: Internal error of the device. <br>Solutions: Contact ZEGO technical support.
    ZEGO_ERROR_CODE_DEVICE_INNER_ERROR = 1006099,

    /// Description: Unknown error of the pre-processing module. <br>Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_PREPROCESS_PREPROCESS_UNKNOWN_ERROR = 1007001,

    /// Description: Failed to set the beauty configuration. <br>Cause: The incoming beauty parameters are incorrect. <br>Solutions: Please check the passed in [ZegoBeautifyOption] type parameter.
    ZEGO_ERROR_CODE_PREPROCESS_BEAUTIFY_OPTION_INVALID = 1007005,

    /// The reverberation parameter is null. Please check the input parameter. This error code is deprecated.
    ZEGO_ERROR_CODE_PREPROCESS_REVERB_PARAM_NULL = 1007006,

    /// The voice changer parameter is null. Please check the input parameter. This error code is deprecated.
    ZEGO_ERROR_CODE_PREPROCESS_VOICE_CHANGER_PARAM_NULL = 1007007,

    /// Description: Failed to set the reverberation parameters. <br>Cause: the reverberation room size parameter is invalid. <br>Solutions: The normal range of the reverberation room size parameter is 0.0 ~ 1.0
    ZEGO_ERROR_CODE_PREPROCESS_REVERB_PARAM_ROOM_SIZE_INVALID = 1007011,

    /// Description: Failed to set the reverberation parameters. <br>Cause: The reverberance parameter is invalid. <br>Solutions: The normal range of the reverberance parameter is 0.0 ~ 0.5
    ZEGO_ERROR_CODE_PREPROCESS_REVERB_PARAM_REVERBERANCE_INVALID = 1007012,

    /// Description: Failed to set the reverberation parameters. <br>Cause: the reverberation damping parameter is invalid. <br>Solutions: The normal range of the reverberation damping parameter is 0.0 ~ 2.0
    ZEGO_ERROR_CODE_PREPROCESS_REVERB_PARAM_DAMPING_INVALID = 1007013,

    /// Description: Failed to set the reverberation parameters. <br>Cause: The dry_wet_ratio parameter of the reverberation is invalid. <br>Solutions: The normal range of the dry_wet_ratio parameter of reverberation is greater than 0.0
    ZEGO_ERROR_CODE_PREPROCESS_REVERB_PARAM_DRY_WET_RATIO_INVALID = 1007014,

    /// Description: Failed to start virtual stereo. <br>Cause: The virtual stereo angle parameter is invalid. <br>Solutions: The normal range of angle parameter is -1 ~ 360.
    ZEGO_ERROR_CODE_PREPROCESS_VIRTUAL_STEREO_ANGLE_INVALID = 1007015,

    /// Description: Failed to set the voice changing parameters. <br>Cause: The param setting of the voice changing parameter is invalid. <br>Solutions: The normal range of parameter param is -12.0 ~ 12.0
    ZEGO_ERROR_CODE_PREPROCESS_VOICE_CHANGER_PARAM_INVALID = 1007016,

    /// The reverberation echo parameters is null. Please check the input parameter.
    ZEGO_ERROR_CODE_PREPROCESS_REVERB_ECHO_PARAM_NULL = 1007017,

    /// Description: Set reverberation echo parameters failed . <br>Cause: The reverberation echo parameters is invalid. <br> Solutions: Input the correct reverb echo parameters [setReverbEchoParam].
    ZEGO_ERROR_CODE_PREPROCESS_REVERB_ECHO_PARAM_INVALID = 1007018,

    /// Description: Failed to turn on or turn off the electronic sound effect. <br>Cause: the initial pitch parameter tonal of the electronic tone is invalid. <br>Solutions: The normal range of the starting pitch parameter of the electronic tone is 0 ~ 11.
    ZEGO_ERROR_CODE_PREPROCESS_ELECTRONIC_EFFECTS_TONAL_INVALID = 1007019,

    /// Description: Failed to open or close the beauty environment. <br>Cause: The beauty environment was not turned on or off before the engine started. <br>Solutions: Please make sure to turn on or turn off the beauty environment before the engine starts, for example: before calling (startPreview), (startPublishingStream), (startPlayingStream), (createMediaPlayer) or (createAudioEffectPlayer).
    ZEGO_ERROR_CODE_PREPROCESS_ENABLE_EFFECTS_ENV_FAILED = 1007020,

    /// Description: Failed to turn on or turn off the beauty effect. <br>Cause: The beauty environment is not activated. <br>Solutions: Please call [startEffectsEnv] to start the beauty environment first.
    ZEGO_ERROR_CODE_PREPROCESS_ENABLE_EFFECTS_BEAUTY_FAILED = 1007021,

    /// Description: Failed to set beauty parameters. <br>Cause: The beauty environment is not activated. <br>Solutions: Please call [startEffectsEnv] to start the beauty environment first.
    ZEGO_ERROR_CODE_PREPROCESS_SET_EFFECTS_PARAM_FAILED = 1007022,

    /// Description: Effects Beauty does not support the currently set video data type. <br>Cause: [enableCustomVideoProcessing] interface, Windows platform only supports raw_data, Apple device only supports cv_pixel_buffer, Android platform supports gl_texture_2d. <br>Solutions: select the correct video data type.
    ZEGO_ERROR_CODE_PREPROCESS_NOT_SUPPORT_EFFECTS_BUFFER_TYPE = 1007023,

    /// Description: Failed to enable speech enhancement. <br>Cause: speech enhancement level parameter is invalid. <br>Solutions: speech enhancement level ranges from 0 to 10. Use the correct parameter value.
    ZEGO_ERROR_CODE_PREPROCESS_ENABLE_SPEECH_ENHANCE_PARAM_INVALID = 1007024,

    /// Description: The MediaPlayer failed to play the media. <br>Cause: The resource file is not loaded. <br> Solutions: Create a media player instance before using media players [createMediaPlayer].
    ZEGO_ERROR_CODE_MEDIA_PLAYER_NO_INSTANCE = 1008001,

    /// Description: The MediaPlayer failed to play the media. <br>Cause: The resource file is not loaded. <br> Solutions: The media player loads the media resource [loadResource] before starting.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_NO_FILE_PATH = 1008003,

    /// Description: The specified media file path is too long. <br>Cause: The specified media file path is too long. The maximum length should be less than 1024 bytes. <br> Solutions: Please specify media file path less than 1024 bytes.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_FILE_PATH_TOO_LONG = 1008004,

    /// Description: The MediaPlayer failed to load the file. <br>Cause: File formats are not supported. <br> Solutions: Files in this format are not supported, please use files in the supporting format.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_FILE_FORMAT_ERROR = 1008005,

    /// Description: The MediaPlayer failed to load the file. <br>Cause: The file path does not exist. <br> Solutions: Check the validity of the media file path.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_FILE_PATH_NOT_EXISTS = 1008006,

    /// Description: The MediaPlayer failed to load the file. <br>Cause: The file decoding failed. <br> Solutions: Check that the media file is corrupted or contact ZEGO technical support.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_FILE_DECODE_ERROR = 1008007,

    /// Description: The MediaPlayer failed to load the file. <br>Cause: No supported audio/video stream exists. <br> Solutions: Check that the media file data is empty.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_FILE_NO_SUPPORTED_STREAM = 1008008,

    /// Description: The copyrighted music resource file has expired. <br>Cause: The resource file has expired. <br>Solutions: Please request song or accompaniment again.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_FILE_EXPIRED = 1008009,

    /// Description: The MediaPlayer failed to load the file. <br>Cause: There was an error during file resolution. <br> Solutions: Try again or contact ZEGO technical support.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_DEMUX_ERROR = 1008010,

    /// Description: The copyrighted music resource has expired. <br>Cause: The copyrighted music resource has expired. <br>Solutions: Please request the copyrighted music resource again.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_RESOURCE_EXPIRED = 1008011,

    /// Description: The copyrighted music resource ID is too long. <br>Cause: The copyrighted music resource ID is too long. The maximum length should be less than 512 bytes. <br> Solutions: Please copyrighted music resource ID less than 1024 bytes.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_RESOURCE_ID_TOO_LONG = 1008012,

    /// Description: User cancel. <br>Cause: After calling [Load] to load the resource, call [Stop] to stop the player. <br> Solutions: Expected behavior when calling [Stop] while loading a resource.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_USER_CANCEL = 1008013,

    /// Description: The MediaPlayer is already start. <br>Cause: [Load] has been called. <br> Solutions: Please call [Stop] first, then call [Load].
    ZEGO_ERROR_CODE_MEDIA_PLAYER_ALREADY_START = 1008014,

    /// Description: The MediaPlayer failed to load the file. <br>Cause: Permission denied. <br> Solutions: Check the validity of the media file path.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_PERMISSION_DENIED = 1008015,

    /// Description: The MediaPlayer failed to seek. <br>Cause: The file hasn't been loaded yet. <br> Solutions: The media player loads the media resource [loadResource] before seeking [seekTo].
    ZEGO_ERROR_CODE_MEDIA_PLAYER_SEEK_ERROR = 1008016,

    /// Description: The MediaPlayer is configured with a video data format not supported by the platform. <br>Cause: The MediaPlayer is configured with a video data format not supported by the platform (e.g., CVPixelBuffer on iOS does not support NV21). <br> Solutions: Check the data format [setVideoHandler] supported by the current media player platform and set the correct data format.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_PLATFORM_FORMAT_NOT_SUPPORTED = 1008020,

    /// Description: The number of MediaPlayer instances exceeds the maximum number allowed. <br>Cause: The number of MediaPlayer instances exceeds the maximum number allowed. Up to 4 instances can be created. <br> Solutions: Media players can create up to 4 instances, and make sure that the number of media player instances is not exceeded the maximum limit.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_EXCEED_MAX_COUNT = 1008030,

    /// Description: The media player failed to specify the audio track index. <br>Cause: The audio track index not exist. <br>Solutions: Check file audio track index call [getAudioTrackCount] can get.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_SET_AUDIO_TRACK_INDEX_ERROR = 1008040,

    /// Description: Media player setting sound change parameter invalid. <br>Cause: Error setting parameters. <br>Solutions: Checking setting parameters control during -12.0 to 12.0
    ZEGO_ERROR_CODE_MEDIA_PLAYER_SET_VOICE_CHANGER_PARAM_INVALID = 1008041,

    /// Description: takeSnapshot screenshot failed <br>Cause: The video is not playing or 'setPlayerCanvas' is not called to display the video to the control. <br>Solutions: Check whether the video plays normally(check [onPlayStart] callback) and the screen is displayed normally.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_TAKE_SNAPSHOT_TIMING_ERROR = 1008042,

    /// Description: the passed parameter is not in the valid value range. <br>Cause: error setting parameters. <br>Solutions: Review the interface comment and pass in a value within the legal range.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_PARAM_VALUE_RANGE_ILLEGAL = 1008043,

    /// Description: Failed to cache network resource file. <br>Cause: During playback, [seekTo] operation will cause cache failure, and network reasons or incomplete playback caused by active stop will also cause cache failure. <br>Solutions: Check whether there is a [seekTo] operation, whether the playback failed due to network reasons, or whether the playback stopped actively.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_LOCAL_CACHE_FAILED = 1008044,

    /// Description: MediaPlayer internal error. <br>Cause: internal error. <br>Solutions: Contact Technical support.
    ZEGO_ERROR_CODE_MEDIA_PLAYER_INNER_ERROR = 1008099,

    /// Description: the input message content is empty <br>Cause: imessage content is empty <br>Solutions: Input a non-empty message.
    ZEGO_ERROR_CODE_IM_CONTENT_NULL = 1009001,

    /// Description: The input message content is too long <br>Cause: message more than 1024 bytes. <br>Solutions: The maximum length should be less than 1024 byte
    ZEGO_ERROR_CODE_IM_CONTENT_TOO_LONG = 1009002,

    /// Description: The input real-time sequential data is too long. <br>Cause: The length of the input data is greater than 4096 bytes. <br>Solution: Check the length of the input data, consider splitting the large data packet into multiple small data and sending it multiple times.
    ZEGO_ERROR_CODE_IM_DATA_TOO_LONG = 1009003,

    /// Description: The room where the message is sent is different from the room currently logged in. <br>Cause: The room where the message is sent is different from the room currently logged in. <br>Solutions: Send a message to the current logged-in room ID.
    ZEGO_ERROR_CODE_IM_INCONSISTENT_ROOM_ID = 1009005,

    /// Description: Failed to send the message. <br>Cause: network problems. <br>Solutions: Check the network.
    ZEGO_ERROR_CODE_IM_SEND_FAILED = 1009010,

    /// Description: Failed to send custom command. <br>Cause: The entered user ID is empty. <br>Solutions: Please enter the correct user ID.
    ZEGO_ERROR_CODE_IM_USER_ID_EMPTY = 1009011,

    /// Description: Failed to send custom signaling. <br>Cause: The entered user ID is too long. <br>Solutions: Please enter the correct user ID, the maximum user ID cannot exceed 64 bytes.
    ZEGO_ERROR_CODE_IM_USER_ID_TOO_LONG = 1009012,

    /// Description: Failed to send message. <br>Cause: The message input length exceeds the limit. <br>Solutions: Check the input content length or contact ZEGO technical support to expand the message content length.
    ZEGO_ERROR_CODE_IM_INPUT_PARAMS_LENGTH_LIMIT = 1009013,

    /// Description: Failed to send broadcast message,. <br>Cause: QPS exceeds the limit. <br>Solutions: Control the maximum QPS is 2 .
    ZEGO_ERROR_CODE_IM_BROADCAST_MESSAGE_QPS_OVERLOAD = 1009015,

    /// Description: The real-time sequential data manager instance creation failed. <br>Cause: A manager instance with this room ID has already been created. <br>Solution: A maximum of 1 instance can be created for each room ID. If you need to create multiple instances, please use other room IDs.
    ZEGO_ERROR_CODE_IM_MANAGER_CREATION_FAILED = 1009031,

    /// Description: The specified real-time sequential data manager instance could not be found. <br>Cause: This manager instance has not been created yet. <br>Solution: Please call the [createRealTimeSequentialDataManager] function to create a manager instance first.
    ZEGO_ERROR_CODE_IM_NO_MANAGER_INSTANCE = 1009032,

    /// Description: No publish channel available for broadcasting. <br>Cause: The developer has used all publish channels. <br>Solution: Do not use all the publish channels, check if there are any streams that can stop publsihing, or contact ZEGO technical support to increase the available publish channels.
    ZEGO_ERROR_CODE_IM_NO_AVAILABLE_BROADCAST_CHANNEL = 1009033,

    /// Description: The stream ID to start broadcasting is not available. <br>Cause: The stream ID has been used in this device for RTC business (e.g. [startPublishingStream] / [startPlayingStream]). <br>Solution: Please use another stream ID for broadcasting.
    ZEGO_ERROR_CODE_IM_NO_AVAILABLE_STREAM_ID = 1009034,

    /// Description: Repeat broadcast. <br>Cause: The developer repeatedly calls the [startBroadcasting] function. <br>Solution: Please check the business logic to avoid repeating the broadcast for the stream which is broadcasting.
    ZEGO_ERROR_CODE_IM_REPEAT_BROADCAST = 1009035,

    /// Description: The stream to stop broadcasting does not exist. <br>Cause: The stream ID set [stopBroadcasting] function is not in broadcasting. <br>Solution: Check if the stream ID is correct, or if the stream ID is not in broadcasting.
    ZEGO_ERROR_CODE_IM_NO_BROADCATING_STREAM = 1009036,

    /// Description: The stream to stop subscribing does not exist. <br>Cause: The stream ID set [stopSubscribing] function is not in subscribing. <br>Solution: Check if the stream ID is correct, or if the stream ID is not in subscribing.
    ZEGO_ERROR_CODE_IM_NO_SUBSCRIBING_STREAM = 1009037,

    /// Description: Repeat broadcast. <br>Cause: The developer repeatedly calls the [startBroadcasting] function. <br>Solution: Please check the business logic to avoid repeating the subscribe for the stream which is subscribing.
    ZEGO_ERROR_CODE_IM_REPEAT_SUBSCRIBE = 1009038,

    /// Description: Failed to send real-time sequential data. <br>Cause: The broadcast has not started yet, or the broadcast has encountered network problems. <br>Solution: Check whether [startBroadcasting] has been called to start broadcasting, or check whether the network is normal.
    ZEGO_ERROR_CODE_IM_REAL_TIME_SEQUENTIAL_DATA_SEND_FAILED = 1009039,

    /// Description: the file name suffix is not supported. <br>Cause: the file name suffix is not supported. <br>Solutions: only support .mp4/.aac/.flv.
    ZEGO_ERROR_CODE_RECORDER_FILE_SUFFIX_NAME_FORMAT_NOT_SUPPORT = 1010002,

    /// Description: Generic recording API error. <br>Cause: Invalid input parameter. <br> Solutions: Please check the record file path parameter or the record file format parameter is valid or not.
    ZEGO_ERROR_CODE_RECORDER_COMMON_LIVEROOM_API_ERROR = 1010003,

    /// Description: The specified recorded file path is too long. <br>Cause: The specified recorded file path is too long. The maximum length should be less than 1024 bytes. <br> Solutions: Please specify recorded file path less than 1024 bytes.
    ZEGO_ERROR_CODE_RECORDER_FILE_PATH_TOO_LONG = 1010011,

    /// Description: SDK internal error. <br>Cause: Internal error. <br> Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_RECORDER_INNER_VE_ERROR = 1010012,

    /// Description: Open file failed. <br>Cause: Invalid file path or no permissions to read/write file. <br> Solutions: Please specify a valid file path and has proper permissions to read/write.
    ZEGO_ERROR_CODE_RECORDER_OPEN_FILE_FAILED = 1010013,

    /// Description: Write file failed. <br>Cause: No permissions to write file. <br> Solutions: Please specify a valid file path and has proper permissions to write.
    ZEGO_ERROR_CODE_RECORDER_WRITE_FILE_ERROR = 1010014,

    /// Description: Insufficient disk space. <br>Cause: Insufficient disk space. <br> Solutions: Please ensure sufficient disk space.
    ZEGO_ERROR_CODE_RECORDER_NO_ENOUGH_SPARE_CAPACITY = 1010017,

    /// Description: File handle exception. <br>Cause: File handle exception. <br> Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_RECORDER_FILE_HANDLE_EXCEPTIONS = 1010018,

    /// Description: I/O exception. <br>Cause: I/O exception. <br> Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_RECORDER_IO_EXCEPTIONS = 1010019,

    /// Description: The custom video capturer is not created. <br>Cause: Create custom video capturer before onStart callback received. <br> Solutions: Please create custom video capturer after received onStart callback.
    ZEGO_ERROR_CODE_CUSTOM_VIDEO_IO_CAPTURER_NOT_CREATED = 1011001,

    /// Description: The custom video capture module is not enabled. <br>Cause: Custom video capture module is not enabled in initialization configurations. <br> Solutions: Please contact ZEGO technical support, make sure custom video capture module is enabled in initialization configurations.
    ZEGO_ERROR_CODE_CUSTOM_VIDEO_IO_NO_CUSTOM_VIDEO_CAPTURE = 1011002,

    /// Description: Failed to enable/disable custom video capture/rendering. <br>Cause: Not enable/disable custom video capture/rendering before engine is started. <br> Solutions: Please make sure to enable/disable custom video capture/rendering before engine is started, i.e., before calling (startPreview), (startPublishingStream), (startPlayingStream), (createMediaPlayer) or (createAudioEffectPlayer).
    ZEGO_ERROR_CODE_CUSTOM_VIDEO_IO_ENABLE_CUSTOM_IO_FAILED = 1011003,

    /// Description: The custom video capturer is not created. <br>Cause: Internal video-related modules are not created. <br> Solutions: Please call [startPreview] or [startPublishingStream] first.
    ZEGO_ERROR_CODE_CUSTOM_VIDEO_IO_PROCESS_MODULE_NOT_CREATED = 1011004,

    /// Description: The custom video process module is not enabled. <br>Cause: The custom video process module is not enabled. <br> Solutions: Call [enableCustomVideoProcessing] to enable a custom video capturermodule.
    ZEGO_ERROR_CODE_CUSTOM_VIDEO_IO_NO_CUSTOM_VIDEO_PROCESSING = 1011005,

    /// Description: The currently configured custom video capture format does not support this API. <br>Cause: The currently configured custom video capture format does not support this API. <br> Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_CUSTOM_VIDEO_IO_NOT_SUPPORTED_FORMAT = 1011010,

    /// Description: Custom video rendering does not support the currently set video buffer type. <br>Cause: The buffer_type in the config parameter of [enableCustomVideoRender] only supports raw_data, cv_pixel_buffer, encoded_data. For [enableCustomVideoProcessing], only raw_data is supported on Windows platform, only cv_pixel_buffer is supported on Apple devices, and gl_texture_2d and surface_texture are supported on Android platform. <br> Solutions: Select the correct video buffer type.
    ZEGO_ERROR_CODE_CUSTOM_VIDEO_IO_NOT_SUPPORTED_BUFFER_TYPE = 1011011,

    /// Description: [customVideoCaptureSendD3DTextureData] specifies texture not supported by the SDK. <br>Cause: TThe device corresponding to the texture specified by [customVideoCaptureSendD3DTextureData] was not created by D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, NULL,...). <br> Solutions: [customVideoCaptureSendD3DTextureData] specifies the texture created by D3D11CreateDevice (0, D3D_DRIVER_TYPE_HARDWARE, NULL,...).
    ZEGO_ERROR_CODE_CUSTOM_VIDEO_IO_INVALID_ACTIVE_D3D11_DEVICE = 1011012,

    /// Description: Unsupported custom audio source type. <br>Cause: Only channel_aux supports zego_audio_source_type_media_player. <br> Solutions: Select the correct custom audio source type.
    ZEGO_ERROR_CODE_CUSTOM_AUDIO_IO_UNSUPPORTED_AUDIO_SOURCE_TYPE = 1012001,

    /// Description: The custom audio capture feature is not enabled. <br>Cause: The custom audio capture feature is not enabled. <br> Solutions: Please make sure that the custom audio IO module is enabled for the specified stream publishing channel.
    ZEGO_ERROR_CODE_CUSTOM_AUDIO_IO_CAPTURER_NOT_CREATED = 1012002,

    /// Description: The custom audio rendering feature is not enabled. <br>Cause: The custom audio rendering feature is not enabled. <br> Solutions: Please make sure that the custom audio IO module is enabled.
    ZEGO_ERROR_CODE_CUSTOM_AUDIO_IO_RENDERER_NOT_CREATED = 1012003,

    /// Description: Failed to enable/disable custom audio IO. <br>Cause: Failed to enable/disable custom audio IO. <br> Solutions: Please make sure to enable/disable it before the engine is started (i.e., before calling `startPreview`, `startPublishingStream` or `startPlayingStream`).
    ZEGO_ERROR_CODE_CUSTOM_AUDIO_IO_ENABLE_CUSTOM_AUDIO_IO_FAILED = 1012004,

    /// Description: The sample rate parameter is illegal. <br>Cause: Capture and render mix results recording does not support 8000, 22050, 24000 sample rates. <br> Solutions: Please confirm whether the sample rate parameter value allowed by the interface is legal.
    ZEGO_ERROR_CODE_CUSTOM_AUDIO_IO_AUDIO_DATA_CALLBACK_SAMPLE_RATE_NO_SUPPORT = 1012010,

    /// Description: The MediaDataPublisher instance is not created. <br>Cause: The MediaDataPublisher instance is not created. <br> Solutions: Call [createMediaDataPublisher] to create a media pusher instance.
    ZEGO_ERROR_CODE_MEDIA_DATA_PUBLISHER_NO_INSTANCE = 1013000,

    /// Description: This error code is deprecated. <br>Cause: None. <br> Solutions: None.
    ZEGO_ERROR_CODE_MEDIA_DATA_PUBLISHER_FILE_PARSE_ERROR = 1013001,

    /// Description: This error code is deprecated. <br>Cause: None. <br> Solutions: None.
    ZEGO_ERROR_CODE_MEDIA_DATA_PUBLISHER_FILE_PATH_ERROR = 1013002,

    /// Description: File decoding exception. <br>Cause: Invalid media file format. <br>Solutions: Please check the file is a valid media file or not; check the file format is in the MediaPlayer support file format list or not.
    ZEGO_ERROR_CODE_MEDIA_DATA_PUBLISHER_FILE_CODEC_ERROR = 1013003,

    /// Description: Timestamp error. <br>Cause: the later frame timestamp is smaller than the previous frame timestamp. <br>Solutions: Please provide the media file, and contact ZEGO technical support.
    ZEGO_ERROR_CODE_MEDIA_DATA_PUBLISHER_TIMESTAMP_GO_BACK_ERROR = 1013004,

    /// Description: The number of instances of the media data publisher created exceeds the maximum limit. <br>Cause: The number of instances of the media data publisher created exceeds the maximum limit. <br> Solutions: Please check if the number of instances of the media data publisher created exceeds the maximum limit, the maximum number of instances allowed to be created is 1.
    ZEGO_ERROR_CODE_MEDIA_DATA_PUBLISHER_EXCEED_MAX_COUNT = 1013005,

    /// Description: No audio effect player instance. <br>Cause: The audio effect player instance not created. <br> Solutions: Create an audio effect player instance before using it(createAudioEffectPlayer).
    ZEGO_ERROR_CODE_AUDIO_EFFECT_PLAYER_NO_INSTANCE = 1014000,

    /// Description: Load audio effect resource failed. <br>Cause: Invalid audio effect resource file. <br> Solutions: Check the file format is in the AudioEffectPlayer support file format list or not.
    ZEGO_ERROR_CODE_AUDIO_EFFECT_PLAYER_LOAD_FAILED = 1014001,

    /// Description: Play audio effect failed. <br>Cause: Invalid audio effect resource file. <br> Solutions: Check the file format is in the AudioEffectPlayer support file format list or not.
    ZEGO_ERROR_CODE_AUDIO_EFFECT_PLAYER_PLAY_FAILED = 1014002,

    /// Description: Change audio effect progress failed. <br>Cause: The audio effect progress value exceed audio effect file duration. <br> Solutions: Please check the audio effect progress value exceed audio effect file duration or not.
    ZEGO_ERROR_CODE_AUDIO_EFFECT_PLAYER_SEEK_FAILED = 1014003,

    /// Description: The number of instances of the audio effect player created exceeds the maximum limit. <br>Cause: The number of instances of the audio effect player created exceeds the maximum limit. <br> Solutions: Please check if the number of instances of the audio effect player created exceeds the maximum limit, the maximum number of instances allowed to be created is 1.
    ZEGO_ERROR_CODE_AUDIO_EFFECT_PLAYER_EXCEED_MAX_COUNT = 1014004,

    /// Description: Network connectivity test failed. <br>Cause: Not connected to the network. <br> Solutions: Please check if you can access the Internet properly.
    ZEGO_ERROR_CODE_UTILITIES_NETWORK_CONNECTIVITY_TEST_FAILED = 1015001,

    /// Description: Network speed test connection failure. <br>Cause: Not connected to the network. <br> Solutions: Please check if you can access the Internet properly.
    ZEGO_ERROR_CODE_UTILITIES_NETWORK_TOOL_CONNECT_SERVER_FAILED = 1015002,

    /// Description: RTP timeout. <br>Cause: Not connected to the network. <br> Solutions: Please check if you can access the Internet properly.
    ZEGO_ERROR_CODE_UTILITIES_NETWORK_TOOL_RTP_TIMEOUT_ERROR = 1015003,

    /// Description: The server side ends the network speed test. <br>Cause: Network speed test time is too long. <br> Solutions: Please stop network speed test in 30 seconds.
    ZEGO_ERROR_CODE_UTILITIES_NETWORK_TOOL_ENGINE_DENIED = 1015004,

    /// Description: Network speed test stopped. <br>Cause: Network speed test not stopped before publishing stream. <br> Solutions: Please stop network speed test(stopNetworkSpeedTest) before publishing stream.
    ZEGO_ERROR_CODE_UTILITIES_NETWORK_TOOL_STOPPED_BY_PUBLISHING_STREAM = 1015005,

    /// Description: Network speed test stopped. <br>Cause: Network speed test not stopped before playing stream. <br> Solutions: Please stop network speed test(stopNetworkSpeedTest) before playing stream.
    ZEGO_ERROR_CODE_UTILITIES_NETWORK_TOOL_STOPPED_BY_PLAYING_STREAM = 1015006,

    /// Description: Network speed test internal error. <br>Cause: Internal error. <br> Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_UTILITIES_NETWORK_TOOL_INNER_ERROR = 1015009,

    /// Description: Invalid system performance monitoring interval. <br>Cause: The set system performance monitoring interval is out of range. <br> Solutions: Please check if the system performance monitoring interval is out of range or not, valid range is [1000, 10000].
    ZEGO_ERROR_CODE_UTILITIES_PERFORMANCE_MONITOR_INTERVAL_INVALID = 1015031,

    /// Description: Login to the room causes the network test to stop. <br>Cause: Already logged in to the room. <br>Solutions: Since the network test will take up bandwidth, please do it before logging in to the room.
    ZEGO_ERROR_CODE_UTILITIES_STOP_BY_LOGIN_ROOM = 1015032,

    /// Description: Fail to dump data. <br>Cause: Error occurred in AV engine. <br>Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_UTILITIES_START_DUMP_AUDIO_DATA_FAILED = 1015033,

    /// Description: Already start dumping data. <br>Cause:  Call [startDumpData] repeatedly. <br>Solutions: Stop or cancel the previous dump data task.
    ZEGO_ERROR_CODE_UTILITIES_ALREADY_START_DUMP_DATA_ERROR = 1015034,

    /// Description: No dump data. <br>Cause: Error occurred in AV engine. <br>Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_UTILITIES_NO_DUMP_DATA_ERROR = 1015035,

    /// Description: Failed to create dump data folder. <br>Cause: No read and write permission or the disk is full. <br>Solutions: Check to see if there is still space on the disk.
    ZEGO_ERROR_CODE_UTILITIES_CREATE_DUMP_DATA_FOLDER_ERROR = 1015036,

    /// Description: Internal error of upload module. <br>Cause: Error occurred in upload module. <br>Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_UTILITIES_UPLOAD_MODULE_INNER_ERROR = 1015037,

    /// Description: The dump data exceeding the maximum upload limit error. <br>Cause: The dump data is too large to exceed the limit of the upload module. <br>Solutions: Please contact ZEGO technical support and report to ZEGO manually.
    ZEGO_ERROR_CODE_UTILITIES_DUMP_DATA_EXCEED_MAX_SIZE_ERROR = 1015038,

    /// Description: The function call failed. <br>Cause: No range auido instance has been created. <br>Solutions: Create a range audio instance.
    ZEGO_ERROR_CODE_RANGE_AUDIO_NO_INSTANCE = 1016000,

    /// Description: Failed to create range audio. <br>Cause: The instance exceeds the maximum limit. <br>Solutions: Use the used range audio example.
    ZEGO_ERROR_CODE_RANGE_AUDIO_EXCEED_MAX_COUNT = 1016001,

    /// Description: Failed to create range voice. <br>Cause: Range voice cannot be used in multi-room mode. <br>Solutions: Set the single-party mode.
    ZEGO_ERROR_CODE_RANGE_AUDIO_NOT_SUPPORT_MULTI_ROOM = 1016002,

    /// Description: Failed to set the team ID. <br>Cause: The input team ID exceeds the maximum limit. <br>Solutions: The input string is less than 64 bytes.
    ZEGO_ERROR_CODE_RANGE_AUDIO_TEAM_ID_TOO_LONG = 1016003,

    /// Description: Failed to set the team ID.<br>Cause: The input user ID contains invalid characters. <br>Solutions: User ID can only contains numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', ',', '.', '<', '>', '/', '\'.
    ZEGO_ERROR_CODE_RANGE_AUDIO_TEAM_ID_INVALID_CHARACTER = 1016004,

    /// Description: The command invalid. <br>Cause: The command entered by the [sendExtendedRequest] function is empty. <br>Solutions: Please check the command entered when calling the [sendExtendedRequest] function to make sure it is not empty.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_COMMAND_INVALID = 1017000,

    /// Description: The params invalid. <br>Cause: The params entered by the [sendExtendedRequest] function is empty. <br>Solutions: Please check the params entered when calling the [sendExtendedRequest] function to make sure it is not empty.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_PARAMS_INVALID = 1017001,

    /// Description: The song_id invalid. <br>Cause: The song_id entered is empty. <br>Solutions: Please check the song_id entered when calling the function to make sure it is not empty.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_SONG_ID_INVALID = 1017002,

    /// Description: The share_token invalid. <br>Cause: The share_token entered is empty. <br>Solutions: Please check the share_token entered when calling the function to make sure it is not empty.share_token can be obtained by call [requestResource]
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_SHARE_TOKEN_INVALID = 1017003,

    /// Description: The resource_id invalid. <br>Cause: The resource_id entered is empty. <br>Solutions: Please check the resource_id entered when calling the function to make sure it is not empty.resource_id can be obtained by call [requestResource] [getSharedResource] [getMusicByToken]
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_RESOURCE_ID_INVALID = 1017004,

    /// Description: The start_position invalid. <br>Cause: The start_position entered by the fuction [loadCopyrightedMusicResourceWithPosition] is invalid. <br>Solutions: Please check the start_position entered when calling the function [loadCopyrightedMusicResourceWithPosition] to make sure it is in 0 ~ song duration.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_START_POSITION_INVALID = 1017005,

    /// Description: The position invalid. <br>Cause: The position entered by the fuction [seek] is invalid. <br>Solutions: Please check the position entered when calling the function [seek] to make sure it is in 0 ~ song duration.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_POSITION_INVALID = 1017006,

    /// Description: The volume invalid.. <br>Cause: The Volume entered by the fuction [setPlayVolume] is invalid. <br>Solutions: Please check the Volume entered when calling the function [setPlayVolume] to make sure it is in 0 ~ 200.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_VOLUME_INVALID = 1017007,

    /// Description: The krcToken invalid. <br>Cause: The krcToken entered is empty. <br>Solutions: Please check the krcToken entered when calling the function to make sure it is not empty.krcToken can be obtained by call [requestResource] [getSharedResource]
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_KRC_TOKEN_INVALID = 1017008,

    /// Description: Copyright music init authentication failed. <br>Cause: Appsign or token is not set. <br>Solutions: When using token authentication, call [loginRoom] before calling [initCopyrightedMusic] or use appsign authentication.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_COPYRIGHTED_MUSIC_AUTH_PARAM_INVALID = 1017009,

    /// Description: Request copyrighted server fail. <br>Cause: The params entered make mistake or some network reasons. <br>Solutions: Please check the params entered and retry.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_COPYRIGHTED_SERVER_FAIL = 1017010,

    /// Description: Free space limit. <br>Cause: Free space limit. <br>Solutions: Please clean up local files and make sure there is enough free disk space.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_FREE_SPACE_LIMIT = 1017011,

    /// Description: Downloading. <br>Cause: Download same resource. <br>Solutions: Please wait for the resource to download successfully.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_DOWNLOADING = 1017012,

    /// Description: Resource file not exist. <br>Cause: Resource file has been deleted. <br>Solutions: Please reload resource.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_RESOURCE_FILE_NOT_EXIST = 1017013,

    /// Description: Resource file expired. <br>Cause: The resource file has expired. <br>Solutions: Please request song or accompaniment again.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_RESOURCE_FILE_EXPIRED = 1017014,

    /// Description: Resource file invalid. <br>Cause: File is corrupted <br>Solutions: Please call [download] function to reload media resource.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_RESOURCE_FILE_INVALID = 1017015,

    /// Description: Download canceled. <br>Cause: Call [cancelDownload] to actively cancel the download.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_DOWNLOAD_CANCELED = 1017016,

    /// Description: The resource_id unauthorized. <br>Cause: The resource ID is not obtained through [requestResource] [getSharedResource] function. <br>Solutions: Please call the [requestResource] [getSharedResource] function to get valid resource ID.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_RESOURCE_ID_UNAUTHORIZED = 1017018,

    /// Description: The copyrighted resource has expired. <br>Cause: The copyrighted resource has expired. <br>Solutions: Please request the copyrighted resource again.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_RESOURCE_EXPIRED = 1017019,

    /// Description: The resource does not support this method. <br>Cause: Wrong resource ID selection. <br>Solutions: Please pass in the correct resource ID.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_RESOURCE_NOT_SUPPORT_FUNCTION = 1017020,

    /// Description: No copyright, unable to listen to and sing songs. <br>Cause: No copyright. <br>Solutions: Please select another music.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_NO_COPYRIGHT = 1017030,

    /// Description: No permissions of accompaniment, can only listen to songs, not sing. <br>Cause: No permissions of accompaniment. <br>Solutions: Please select another music.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_NO_PERMISSIONS_OF_LYRICS_AND_MUSIC = 1017031,

    /// Description: Non monthly membership. <br>Cause: Unopened monthly membership. <br>Solutions: Open monthly membership or ues COUNT mode request music.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_NON_MONTHLY_MEMBERSHIP = 1017032,

    /// Description: No accompany. <br>Cause: Music don't have accompany. <br>Solutions: Please choice music have accompany.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_NO_ACCOMPANY = 1017033,

    /// Description: Resource not exist. <br>Cause: Resource not exist. <br>Solutions: Please select another music.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_RESOURCE_NOT_EXIST = 1017034,

    /// Description: Illegal param. <br>Cause: The entered param is incorrect. <br>Solutions: Please check param when entered function to make sure it is correct.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_ILLEGAL_PARAM = 1017040,

    /// Description: AppID invalid. <br>Cause: The appID is not support copyrighted music. <br>Solutions: Please contact ZEGO technical support.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_APPID_INVALID = 1017041,

    /// Description: Billing mode invalid. <br>Cause: Billing mode invalid. <br>Solutions: Please select correct billing mode.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_BILLING_MODE_INVALID = 1017042,

    /// Description: Unreasonable_access. <br>Cause: Monthly membership request music by COUNT. <br>Solutions: Please select correct billing mode.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_UNREASONABLE_ACCESS = 1017043,

    /// Description: Share token expired. <br>Cause: Share token expired. <br>Solutions: Please select an unexpired sharing token to get resources.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_SHARE_TOKEN_EXPIRED = 1017044,

    /// Description: Share token illegal. <br>Cause: Share token illegal. <br>Solutions: Please check songToken when entered by calling [getMusicByToken] to make sure it is correct.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_SHARE_TOKEN_ILLEGAL = 1017045,

    /// Description: krcToken illegal. <br>Cause: krcToken illegal. <br>Solutions: Please check krcToken when entered by calling [getKrcLyricByKrcToken] to make sure it is correct.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_KRC_TOKEN_ILLEGAL = 1017046,

    /// Description: krcToken expired. <br>Cause: krcToken expired. <br>Solutions: Please select an unexpired krcToken to get lyrics in KRC format.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_KRC_TOKEN_EXPIRED = 1017047,

    /// Description: Get lyric fail. <br>Cause: Lyrics not found. <br>Solutions: Please try again later.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_GET_LYRIC_FAIL = 1017048,

    /// Description: Get pitch fail. <br>Cause: Pitch not found. <br>Solutions: Please select correct song.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_GET_PITCH_FAIL = 1017049,

    /// Description: This resource is not shared in the room. <br>Cause: No users in the room share this resource. <br>Solutions: Please any one user in the room to call the [requestresource] function to request resources and share them.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_NOT_SHARED_RESOURCE = 1017050,

    /// Description: The number of times the resource is free in the room is exhausted. <br>Cause: 1. The shared resources cannot be obtained again; 2. Shared resources have been obtained. <br>Solutions: Please use the acquired resources, or use [requestResource] to share resources again.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_GET_SHARED_RESOURCE_TIMES_OVER = 1017051,

    /// Description: The vendor is unavailable. <br>Cause: The corresponding copyright party has not been activated. <br>Solutions: Please pass in the correct vendor ID.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_VENDOR_ID_NOT_AVALIABLE = 1017052,

    /// Description: This vendor does not support this function. <br>Cause: Wrong vendor selection. <br>Solutions: Please pass in the correct vendor ID.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_VENDOR_NOT_SUPPORT_FUNCTION = 1017053,

    /// Description: The room ID is not logged in. <br>Cause: Room ID input error. <br>Solutions: If the copyright is not used in a multi room scenario, there is no need to pass in the roomID, otherwise please pass in the correct logged in room ID.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_ROOM_NO_LOGIN = 1017054,

    /// Description: Vendor ID invalid. <br>Cause: Wrong vendor selection. <br>Solutions: Please pass in the correct vendor ID.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_VENDOR_ID_INVALID = 1017071,

    /// Description: This vendor does not support this song ID. <br>Cause: This song ID does not exist in the copyright music library. <br>Solutions: Please pass in the correct song ID.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_SONG_ID_NOT_SUPPORT = 1017072,

    /// Description: master ID invalid. <br>Cause: When billing by owner is selected,  master ID is empty. <br>Solutions: Please pass in the correct master ID.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_MASTER_ID_INVALID = 1017073,

    /// Description: The page parameter is invalid for multiple copyright searches. <br>Cause: The page parameter must be 1 when multiple rights search is called for the first time. <br>Solutions: Please pass in the correct page.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_PAGE_INVALID = 1017074,

    /// Description: resource has no pitch. <br>Cause: resource has no pitch. <br>Solutions: Please pass in the correct song ID.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_NO_PITCH = 1017075,

    /// Description: scene ID invalid. <br>Cause: scene ID invalid. <br>Solutions: Please pass in the scene ID corresponding to the opened scene.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_SCENE_ID_INVALID = 1017076,

    /// Description: top ID invalid. <br>Cause: An unsupported top ID was passed in when fetching chart songs. <br>Solutions: Please pass in the correct top ID.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_TOP_ID_INVALID = 1017077,

    /// Description: Loading copyright music plugin failed. <br>Cause: The program runtime path does not include the copyright music plugin dynamic library. <br>Solutions: Please configure the copyright music plugin dynamic library dependency correctly.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_LOAD_PLUGIN_FAIL = 1017094,

    /// Description: The copyright music module does not support this method. <br>Cause: The copyright music module does not support this function under the current platform. <br>Solutions: Contact ZEGO technical support to deal with it.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_NOT_SUPPORT_METHOD = 1017095,

    /// Description: The copyright music module is not initialized. <br>Cause: The [initCopyrightedMusic] method is not called to initialize the copyright module. <br>Solutions: Please call the [initCopyrightedMusic] method first.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_NO_INIT = 1017096,

    /// Description: System is busy. <br>Cause: System is busy. <br>Solutions: Please try again.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_SYSTEM_BUSY = 1017097,

    /// Description: Failed due to network exceptions.<br>Cause: Unknown internal error.<br>Solutions: Contact ZEGO technical support to deal with it.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_NETWORK_ERROR = 1017098,

    /// Description: Failed due to internal system exceptions.<br>Cause: Unknown internal error.<br>Solutions: Contact ZEGO technical support to deal with it.
    ZEGO_ERROR_CODE_COPYRIGHTED_MUSIC_INNER_ERROR = 1017099,

    /// Description: SDK internal return null pointer.
    ZEGO_ERROR_CODE_AUDIO_VAD_CLIENT_INNER_NULLPTR = 1018000,

    /// Description: The function call failed. <br>Cause: No audio vad instance has been created. <br>Solutions: Create a audio vad instance.
    ZEGO_ERROR_CODE_AUDIO_VAD_CLIENT_NO_INSTANCE = 1018001,

    /// Description: Calling function failed. <br>Cause: Illegal parameter or SDK does not contain RangeScene module. <br>Solutions: Please confirm whether the SDK contains the RangeScene module and check the parameters.
    ZEGO_ERROR_CODE_RANGE_SCENE_CALL_FUNCTION_FAILED = 1019000,

    /// Description: The RangeScene instance not created. <br>Cause: The RangeScene instance not created. <br> Solutions: Create a RangeScene instance before using RangeScene [createRangeScene].
    ZEGO_ERROR_CODE_RANGE_SCENE_NO_INSTANCE = 1019001,

    /// Description: Not login scene. <br>Cause: Not login scene. <br> Solutions: Please call [loginScene] to login scene.
    ZEGO_ERROR_CODE_RANGE_SCENE_NOT_LOGIN_SCENE = 1019002,

    /// Description: Use not support feature. <br>Cause: Use the status synchronization interface when the status synchronization function is not enabled. <br> Solutions: To use the status synchronization interface, please enable the status synchronization function.
    ZEGO_ERROR_CODE_RANGE_SCENE_NOT_SUPPORT = 1019003,

    /// Description: Scene login retry has exceeded the maximum retry time. <br>Cause: Possibly due to network problems. <br>Solutions: Please check whether the network is working or switch the network environment.
    ZEGO_ERROR_CODE_RANGE_SCENE_RETRY_TIMEOUT = 1019004,

    /// Description: Scene connection is temporarily interrupted. <br>Cause: Possibly due to network problems. <br>Solutions: Please wait or check whether the network is normal.
    ZEGO_ERROR_CODE_RANGE_SCENE_NETWORK_INTERRUPT = 1019005,

    /// Description: Token verification failed. <br>Cause: The parameters passed in during token generation are inconsistent with the SDK. <br>Solutions: Please use the correct token regenerated.
    ZEGO_ERROR_CODE_RANGE_SCENE_TOKEN_ILLEGAL = 1019006,

    /// Description: Token expire. <br>Cause: Token expire or the generated Token validity period parameter type is incorrect. <br>Solutions: Regenerate the Token.
    ZEGO_ERROR_CODE_RANGE_SCENE_TOKEN_EXPIRE = 1019007,

    /// Description: Coordinates out of range. <br>Cause: Coordinates out of scene range. <br>Solutions: Please pass in the correct coordinates.
    ZEGO_ERROR_CODE_RANGE_SCENE_COORDINATE_OUT_OF_RANGE = 1019020,

    /// Description: The item has been created. <br>Cause: The item has been created. <br>Solutions: The item has been created. You don't need to create it again.
    ZEGO_ERROR_CODE_RANGE_SCENE_ITEM_HAS_BEEN_CREATED = 1019021,

    /// Description: The item binding user exceeds the maximum limit. <br>Cause: The item binding user exceeds the maximum limit. <br>Solutions: The item binding user exceeds the maximum limit. Please try again later.
    ZEGO_ERROR_CODE_RANGE_SCENE_ITEM_BIND_EXCEED_LIMIT = 1019022,

    /// Description: The item does not exist. <br>Cause: The item does not exist. <br>Solutions: Please create an item first.
    ZEGO_ERROR_CODE_RANGE_SCENE_ITEM_NOT_EXIST = 1019023,

    /// Description: The item is not bound. <br>Cause: The item is not bound. <br>Solutions: Please bind the item first.
    ZEGO_ERROR_CODE_RANGE_SCENE_ITEM_NOT_BIND = 1019024,

    /// Description: The item has been operated by others. <br>Cause: The item has been operated by others. <br>Solutions: Please try again later.
    ZEGO_ERROR_CODE_RANGE_SCENE_ITEM_CAS_FAILED = 1019025,

    /// Description: The binding capacity of the item exceeds the maximum limit. <br>Cause: The binding capacity of the item exceeds the maximum limit. <br>Solutions: Please use a capacity that does not exceed the maximum binding capacity to create an item.
    ZEGO_ERROR_CODE_RANGE_SCENE_ITEM_CAPACITY_EXCEED_LIMIT = 1019026,

    /// Description: The user has bound the item to the maximum limit. <br>Cause: The user has bound the item to the maximum limit. <br>Solutions: Please unbind some items that do not need to be operated temporarily.
    ZEGO_ERROR_CODE_RANGE_SCENE_USER_BIND_ITEM_EXCEED_LIMIT = 1019027,

    /// Description: The item is beyond the user's view. <br>Cause: The item is beyond the user's view. <br>Solutions: Please operate the item within the user's view.
    ZEGO_ERROR_CODE_RANGE_SCENE_ITEM_OUT_OF_USER_VIEW = 1019028,

    /// Description: The number of RangeScene instances exceeds the maximum number allowed. <br>Cause: The number of RangeScene instances exceeds the maximum number allowed. Up to 1 instances can be created. <br> Solutions: RangeScene can create up to 1 instances, and make sure that the number of RangeScene instances is not exceeded the maximum limit.
    ZEGO_ERROR_CODE_RANGE_SCENE_EXCEED_MAX_COUNT = 1019030,

    /// Description: The number of joined RangeScene teams exceeds the maximum number allowed. <br>Cause: The number of joined RangeScene teams exceeds the maximum number allowed. Up to 5 are allowed by default. <br> Solutions: RangeScene teams can joined are up to 5, and make sure that the number of RangeScene teams joned is not exceeded the maximum limit.
    ZEGO_ERROR_CODE_RANGE_SCENE_TEAM_EXCEED_MAX_COUNT = 1019031,

    /// Description: Team id already been used. <br>Cause: The team id already been used when join team. <br> Solutions: Use a new team id to join team.
    ZEGO_ERROR_CODE_RANGE_SCENE_TEAM_ID_OCCUPIED = 1019032,

    /// Description: Team id not exist. <br>Cause: Use incorrect team id when leave team. <br> Solutions: Use correct team id when leave team.
    ZEGO_ERROR_CODE_RANGE_SCENE_TEAM_ID_INCORRECT = 1019033,

    /// Description: Failed due to internal system exceptions.<br>Cause: Unknown internal error.<br>Solutions: Contact ZEGO technical support to deal with it.
    ZEGO_ERROR_CODE_RANGE_SCENE_INNER_ERROR = 1019099,

    /// Description: Startup screen capture failed. <br>Cause: The user refused to grant the app screen capture permission. <br>Solutions: Allow the app to capture screen permissions.
    ZEGO_ERROR_CODE_SCREEN_CAPTURE_PERMISSION_DENIED = 1020000,

    /// Description: Startup screen capture failed. <br>Cause: The current system version does not support screen capture. <br>Solutions: Use system version above Android 5 (API level 21).
    ZEGO_ERROR_CODE_SCREEN_CAPTURE_NOT_SUPPORT = 1020001,

    /// Description: Startup screen capture failed. <br>Cause: Unable to share the screen module. <br>Solutions: Please introduce screen sharing module resources, or contact technical support.
    ZEGO_ERROR_CODE_SCREEN_CAPTURE_SDK_NO_MODULE = 1020002,

    /// Description: The function call failed. <br>Cause: No screen capture source instance has been created. <br>Solutions: Create a screen capture source instance.
    ZEGO_ERROR_CODE_SCREEN_CAPTURE_NO_INSTANCE = 1020003,

    /// Description: Failed to create screen capture source. <br>Cause: The instance exceeds the maximum limit. <br>Solutions: Use an existing screen capture instance or destroy the previous instance.
    ZEGO_ERROR_CODE_SCREEN_CAPTURE_EXCEED_MAX_COUNT = 1020004,

    /// Description: Failed due to internal system exceptions.<br>Cause: Unknown internal error.<br>Solutions: Contact ZEGO technical support to deal with it.
    ZEGO_ERROR_CODE_SCREEN_CAPTURE_INNER_ERROR = 1020005,

    /// Description: The function call failed. <br>Cause: No AI voice changer instance has been created. <br>Solutions: Create an AI voice changer instance.
    ZEGO_ERROR_CODE_AI_VOICE_CHANGER_NO_INSTANCE = 1021000,

    /// Description: Failed to create AI voice changer. <br>Cause: The instance exceeds the maximum limit. <br>Solutions: Use an existing AI voice changer instance or destroy the previous instance.
    ZEGO_ERROR_CODE_AI_VOICE_CHANGER_EXCEED_MAX_COUNT = 1021001,

    /// Description: Failed due to internal system exceptions.<br>Cause: Unknown internal error.<br>Solutions: Contact ZEGO technical support to deal with it.
    ZEGO_ERROR_CODE_AI_VOICE_CHANGER_INNER_ERROR = 1021002,

    /// Description: AI voice changer was not initialized.<br>Cause: Did not initialize AI voice changer before use it.<br>Solutions: Use AI voice changer after initialize it.
    ZEGO_ERROR_CODE_AI_VOICE_CHANGER_NOT_INIT_ERROR = 1021003,

    /// Description: Initialize AI voie changer failed.<br>Cause: Did not set user ID before initialize AI voice changer, or AI voice changer service expired.<br>Solutions: Set user ID before initialize AI voice changer, or contact ZEGO technical support to deal with it.
    ZEGO_ERROR_CODE_AI_VOICE_CHANGER_INIT_FAILED_ERROR = 1021004,

    /// Description: No AI voice changer resource files.<br>Cause: AI voice changer resource files were not downloaded.<br>Solutions: Please make sure AI voice changer resource files downloaded before use it.
    ZEGO_ERROR_CODE_AI_VOICE_CHANGER_NO_MODEL_ERROR = 1021005,

    /// Description: Download AI voice changer resource files failed.<br>Cause: Poor network connection.<br>Solutions: Please check the network connection is ok.
    ZEGO_ERROR_CODE_AI_VOICE_CHANGER_MODEL_DOWNLOAD_FAILED_ERROR = 1021006,

    /// Description: Can not run AI voice changer on this device.<br>Cause: AI voice changer feature can not run on this device because of poor perforamance.<br>Solutions: Do not use AI voice changer feature on this device.
    ZEGO_ERROR_CODE_AI_VOICE_CHANGER_DEVICE_NOT_SUPPORTED_ERROR = 1021007,

};

} //namespace EXPRESS
} //namespace ZEGO

#endif // __ZEGOEXPRESSERRORCODE_H__
