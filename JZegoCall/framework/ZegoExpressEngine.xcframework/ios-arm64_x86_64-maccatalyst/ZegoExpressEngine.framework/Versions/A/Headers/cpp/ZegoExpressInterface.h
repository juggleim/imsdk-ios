
#ifndef __ZEGOEXPRESSINTERFACE_H__
#define __ZEGOEXPRESSINTERFACE_H__

#include "ZegoExpressDefines.h"
#include "ZegoExpressErrorCode.h"
#include "ZegoExpressEventHandler.h"

namespace ZEGO {
namespace EXPRESS {

class IZegoMediaPlayer;
class IZegoAudioEffectPlayer;
class IZegoRangeAudio;
class IZegoRangeScene;
class IZegoRealTimeSequentialDataManager;
class IZegoScreenCaptureSource;
class IZegoMediaDataPublisher;

class IZegoExpressEngine {
  protected:
    virtual ~IZegoExpressEngine() {}

  public:
    /// Sets up the event notification callbacks that need to be handled. If the eventHandler is set to [nullptr], all the callbacks set previously will be cleared.
    ///
    /// Available since: 1.1.0
    /// Description: Set up event notification callbacks, used to monitor callbacks such as engine status changes, room status changes, etc.
    /// When to call: After [createEngine].
    /// Restrictions: None.
    /// Caution: After calling this function or [createEngine] setting a callback, unless this function is called to clear the callback, setting a callback again will not take effect. After calling [destroyEngine], the event handler that has been set will be invalid and need to be reset after next calling of [createEngine].
    ///
    /// @param eventHandler Event notification callback. If the eventHandler is set to [nullptr], all the callbacks set previously will be cleared. Developers should monitor the corresponding callbacks according to their own business scenarios. The main callback functions of the SDK are here.
    virtual void setEventHandler(std::shared_ptr<IZegoEventHandler> eventHandler) = 0;

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
    virtual void setRoomScenario(ZegoScenario scenario) = 0;

    /// Uploads logs to the ZEGO server.
    ///
    /// Available since: 1.1.0
    /// Description: By default, SDK creates and prints log files in the App's default directory. Each log file defaults to a maximum of 5MB. Three log files are written over and over in a circular fashion. When calling this function, SDK will auto package and upload the log files to the ZEGO server.
    /// Use cases: Developers can provide a business “feedback” channel in the App. When users feedback problems, they can call this function to upload the local log information of SDK to help locate user problems.
    /// When to call: After [createEngine].
    /// Restrictions:  The frequency limit is once per minute.
    /// Caution: 1.After calling this interface to upload logs, if you call [destroyEngine] or exit the App too quickly, there may be a failure.It is recommended to wait a few seconds, and then call [destroyEngine] or exit the App after receiving the upload success callback. 2.If you want to call before [createEngine], use the [submitLog] interface.
    virtual void uploadLog() = 0;

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
    virtual void uploadLog(ZegoUploadLogResultCallback callback) = 0;

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
    virtual void enableDebugAssistant(bool enable) = 0;

    /// Call the experimental API.
    ///
    /// Available since: 2.7.0
    /// Description: ZEGO provides some technical previews or special customization functions in RTC business through this API. If you need to get the use of the function or the details, please consult ZEGO technical support.
    /// When to call: After [createEngine].
    ///
    /// @param params Parameters in the format of a JSON string, please consult ZEGO technical support for details.
    /// @return Returns an argument in the format of a JSON string, please consult ZEGO technical support for details.
    virtual std::string callExperimentalAPI(const std::string &params) = 0;

    /// Logs in to a room. You must log in to a room before publishing or playing streams.
    ///
    /// Available since: 1.1.0
    /// Description: If the room does not exist, [loginRoom] creates and logs in the room. SDK uses the 'room' to organize users. After users log in to a room, they can use interface such as push stream [startPublishingStream], pull stream [startPlayingStream], send and receive broadcast messages [sendBroadcastMessage], etc.
    /// Use cases: In the same room, users can conduct live broadcast, audio and video calls, etc.
    /// When to call /Trigger: This interface is called after [createEngine] initializes the SDK.
    /// Restrictions: For restrictions on the use of this function, please refer to https://docs.zegocloud.com/article/7611 or contact ZEGO technical support.
    /// Caution:
    ///   1. Apps that use different appIDs cannot intercommunication with each other.
    ///   2. SDK supports startPlayingStream audio and video streams from different rooms under the same appID, that is, startPlayingStream audio and video streams across rooms. Since ZegoExpressEngine's room related callback notifications are based on the same room, when developers want to startPlayingStream streams across rooms, developers need to maintain related messages and signaling notifications by themselves.
    ///   3. It is strongly recommended that userID corresponds to the user ID of the business APP, that is, a userID and a real user are fixed and unique, and should not be passed to the SDK in a random userID. Because the unique and fixed userID allows ZEGO technicians to quickly locate online problems.
    ///   4. After the first login failure due to network reasons or the room is disconnected, the default time of SDK reconnection is 20min.
    /// Privacy reminder: Please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
    /// Related callbacks:
    ///   1. When the user starts to log in to the room, the room is successfully logged in, or the room fails to log in, the [onRoomStateChanged] (Not supported before 2.18.0, please use [onRoomStateUpdate]) callback will be triggered to notify the developer of the status of the current user connected to the room.
    ///   2. Different users who log in to the same room can get room related notifications in the same room (eg [onRoomUserUpdate], [onRoomStreamUpdate], etc.), and users in one room cannot receive room signaling notifications in another room.
    ///   3. If the network is temporarily interrupted due to network quality reasons, the SDK will automatically reconnect internally. You can get the current connection status of the local room by listening to the [onRoomStateChanged] (Not supported before 2.18.0, please use [onRoomStateUpdate]) callback method, and other users in the same room will receive [onRoomUserUpdate] callback notification.
    ///   4. Messages sent in one room (e.g. [setStreamExtraInfo], [sendBroadcastMessage], [sendBarrageMessage], [sendCustomCommand], etc.) cannot be received callback ((eg [onRoomStreamExtraInfoUpdate], [onIMRecvBroadcastMessage], [onIMRecvBarrageMessage], [onIMRecvCustomCommand], etc) in other rooms. Currently, SDK does not provide the ability to send messages across rooms. Developers can integrate the SDK of third-party IM to achieve.
    /// Related APIs:
    ///   1. Users can call [logoutRoom] to log out. In the case that a user has successfully logged in and has not logged out, if the login interface is called again, the console will report an error and print the error code 1002001.
    ///   2. SDK supports multi-room login, please call [setRoomMode] function to select multi-room mode before engine initialization, and then call [loginRoom] to log in to multi-room.
    ///   3. Calling [destroyEngine] will also automatically log out.
    ///
    /// @param roomID Room ID, a string of less 128 bytes in length.
    ///   Caution:
    ///   1. room ID is defined by yourself.
    ///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
    ///   3. If you need to communicate with the Web SDK, please do not use '%'.
    /// @param user User object instance, configure userID, userName. Note that the userID needs to be globally unique with the same appID, otherwise the user who logs in later will kick out the user who logged in first.
    virtual void loginRoom(const std::string &roomID, ZegoUser user) = 0;

    /// Logs in to a room with advanced room configurations. You must log in to a room before publishing or playing streams.
    ///
    /// Available since: 1.1.0
    /// Description: If the room does not exist, [loginRoom] creates and logs in the room. SDK uses the 'room' to organize users. After users log in to a room, they can use interface such as push stream [startPublishingStream], pull stream [startPlayingStream], send and receive broadcast messages [sendBroadcastMessage], etc. To prevent the app from being impersonated by a malicious user, you can add authentication before logging in to the room, that is, the [token] parameter in the ZegoRoomConfig object passed in by the [config] parameter.
    /// Use cases: In the same room, users can conduct live broadcast, audio and video calls, etc.
    /// When to call /Trigger: This interface is called after [createEngine] initializes the SDK.
    /// Restrictions: For restrictions on the use of this function, please refer to https://docs.zegocloud.com/article/7611 or contact ZEGO technical support.
    /// Caution:
    ///   1. Apps that use different appIDs cannot intercommunication with each other.
    ///   2. SDK supports startPlayingStream audio and video streams from different rooms under the same appID, that is, startPlayingStream audio and video streams across rooms. Since ZegoExpressEngine's room related callback notifications are based on the same room, when developers want to startPlayingStream streams across rooms, developers need to maintain related messages and signaling notifications by themselves.
    ///   3. It is strongly recommended that userID corresponds to the user ID of the business APP, that is, a userID and a real user are fixed and unique, and should not be passed to the SDK in a random userID. Because the unique and fixed userID allows ZEGO technicians to quickly locate online problems.
    ///   4. After the first login failure due to network reasons or the room is disconnected, the default time of SDK reconnection is 20min.
    /// Privacy reminder: Please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
    /// Related callbacks:
    ///   1. When the user starts to log in to the room, the room is successfully logged in, or the room fails to log in, the [onRoomStateChanged] (Not supported before 2.18.0, please use [onRoomStateUpdate]) callback will be triggered to notify the developer of the status of the current user connected to the room.
    ///   2. Different users who log in to the same room can get room related notifications in the same room (eg [onRoomUserUpdate], [onRoomStreamUpdate], etc.), and users in one room cannot receive room signaling notifications in another room.
    ///   3. If the network is temporarily interrupted due to network quality reasons, the SDK will automatically reconnect internally. You can get the current connection status of the local room by listening to the [onRoomStateChanged] (Not supported before 2.18.0, please use [onRoomStateUpdate]) callback method, and other users in the same room will receive [onRoomUserUpdate] callback notification.
    ///   4. Messages sent in one room (e.g. [setStreamExtraInfo], [sendBroadcastMessage], [sendBarrageMessage], [sendCustomCommand], etc.) cannot be received callback ((eg [onRoomStreamExtraInfoUpdate], [onIMRecvBroadcastMessage], [onIMRecvBarrageMessage], [onIMRecvCustomCommand], etc) in other rooms. Currently, SDK does not provide the ability to send messages across rooms. Developers can integrate the SDK of third-party IM to achieve.
    /// Related APIs:
    ///   1. Users can call [logoutRoom] to log out. In the case that a user has successfully logged in and has not logged out, if the login interface is called again, the console will report an error and print the error code 1002001.
    ///   2. SDK supports multi-room login, please call [setRoomMode] function to select multi-room mode before engine initialization, and then call [loginRoom] to log in to multi-room.
    ///   3. Calling [destroyEngine] will also automatically log out.
    ///
    /// @param roomID Room ID, a string of less 128 bytes in length.
    ///   Caution:
    ///   1. room ID is defined by yourself.
    ///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
    ///   3. If you need to communicate with the Web SDK, please do not use '%'.
    /// @param user User object instance, configure userID, userName. Note that the userID needs to be globally unique with the same appID, otherwise the user who logs in later will kick out the user who logged in first.
    /// @param config Advanced room configuration.
    /// @param callback The callback of this login result(2.18.0 and above version support), if you need detailed room status, please pay attention to the [onRoomStateChanged] callback. Required: No. Default value: nullptr.Caution: If the connection is retried multiple times due to network problems, the retry status will not be thrown by this callback.
    virtual void loginRoom(const std::string &roomID, ZegoUser user, ZegoRoomConfig config,
                           ZegoRoomLoginCallback callback = nullptr) = 0;

    /// Exit the room with callback.
    ///
    /// Available since: 2.9.0
    /// Description: This API will log out the current user has logged in the room, if user logs in more than one room, all the rooms will be logged out.
    /// Use cases: In the same room, users can conduct live broadcast, audio and video calls, etc.
    /// When to call /Trigger: After successfully logging in to the room, if the room is no longer used, the user can call the function [logoutRoom].
    /// Restrictions: None.
    /// Caution: 1. Exiting the room will stop all publishing and playing streams for user, and inner audio and video engine will stop, and then SDK will auto stop local preview UI. If you want to keep the preview ability when switching rooms, please use the [switchRoom] method. 2. If the user is not logged in to the room, calling this function will also return success.
    /// Related callbacks: After calling this function, you will receive [onRoomStateChanged] (Not supported before 2.18.0, please use [onRoomStateUpdate]) callback notification successfully exits the room, while other users in the same room will receive the [onRoomUserUpdate] callback notification(On the premise of enabling isUserStatusNotify configuration).
    /// Related APIs: Users can use [loginRoom], [switchRoom] functions to log in or switch rooms.
    ///
    /// @param callback The callback of this logout room result, if you need detailed room status, please pay attention to the [onRoomStateChanged] callback. Required: No. Default value: nullptr.
    virtual void logoutRoom(ZegoRoomLogoutCallback callback = nullptr) = 0;

    /// Exit the room of the specified room ID with callback.
    ///
    /// Available since: 1.1.0
    /// Description: This API will log out the room named roomID.
    /// Use cases: In the same room, users can conduct live broadcast, audio and video calls, etc.
    /// When to call /Trigger: After successfully logging in to the room, if the room is no longer used, the user can call the function [logoutRoom].
    /// Restrictions: None.
    /// Caution: 1. Exiting the room will stop all publishing and playing streams for user, and inner audio and video engine will stop, and then SDK will auto stop local preview UI. If you want to keep the preview ability when switching rooms, please use the [switchRoom] method. 2. If the user logs out to the room, but the incoming 'roomID' is different from the logged-in room name, SDK will return failure.
    /// Related callbacks: After calling this function, you will receive [onRoomStateChanged] (Not supported before 2.18.0, please use [onRoomStateUpdate]) callback notification successfully exits the room, while other users in the same room will receive the [onRoomUserUpdate] callback notification(On the premise of enabling isUserStatusNotify configuration).
    /// Related APIs: Users can use [loginRoom], [switchRoom] functions to log in or switch rooms.
    ///
    /// @param roomID Room ID, a string of up to 128 bytes in length.
    ///   Caution:
    ///   1. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
    ///   2. If you need to communicate with the Web SDK, please do not use '%'.
    /// @param callback The callback of this logout room result, if you need detailed room status, please pay attention to the [onRoomStateChanged] callback. Required: No. Default value: nullptr.
    virtual void logoutRoom(const std::string &roomID,
                            ZegoRoomLogoutCallback callback = nullptr) = 0;

    /// Switch the room with advanced room configurations.
    ///
    /// Available since: 1.15.0
    /// Description: Using this interface allows users to quickly switch from one room to another room.
    /// Use cases: if you need to quickly switch to the next room, you can call this function.
    /// When to call /Trigger: After successfully login room.
    /// Restrictions: None.
    /// Caution:
    ///   1. When this function is called, all streams currently publishing or playing will stop (but the local preview will not stop).
    ///   2. To prevent the app from being impersonated by a malicious user, you can add authentication before logging in to the room, that is, the [token] parameter in the ZegoRoomConfig object passed in by the [config] parameter. This parameter configuration affects the room to be switched over.
    ///   3. Multi-room mode is supported in version 3.5.0 (use the function [setRoomMode] to set ZegoRoomMode to ZEGO_ROOM_MODE_MULTI_ROOM).
    ///   4. If a Token is passed in for login when logging into the room [loginRoom], the [switchroom] interface must be used with the config parameter and the corresponding Token value passed in when switching rooms.
    /// Privacy reminder: Please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
    /// Related callbacks: When the user call the [switchRoom] function, the [onRoomStateChanged] (Not supported before 2.18.0, please use [onRoomStateUpdate]) callback will be triggered to notify the developer of the status of the current user connected to the room.
    /// Related APIs: Users can use the [logoutRoom] function to log out of the room.
    ///
    /// @param fromRoomID Current roomID.
    /// @param toRoomID The next roomID.
    /// @param config Advanced room configuration.
    virtual void switchRoom(const std::string &fromRoomID, const std::string &toRoomID,
                            ZegoRoomConfig *config = nullptr) = 0;

    /// Renew token.
    ///
    /// Available since: 2.8.0
    /// Description: After the developer receives [onRoomTokenWillExpire], they can use this API to update the token to ensure that the subsequent RTC functions are normal.
    /// Use cases: Used when the token is about to expire.
    /// When to call /Trigger: After the developer receives [onRoomTokenWillExpire].
    /// Restrictions: None.
    /// Caution: The token contains important information such as the user's room permissions, publish stream permissions, and effective time, please refer to https://docs.zegocloud.com/article/11649.
    /// Related callbacks: None.
    /// Related APIs: None.
    ///
    /// @param roomID Room ID.
    /// @param token The token that needs to be renew.
    virtual void renewToken(const std::string &roomID, const std::string &token) = 0;

    /// Set room extra information.
    ///
    /// Available since: 1.13.0
    /// Description: The user can call this function to set the extra info of the room.
    /// Use cases: You can set some room-related business attributes, such as whether someone is Co-hosting.
    /// When to call /Trigger: After logging in the room successful.
    /// Restrictions: For restrictions on the use of this function, please refer to https://docs.zegocloud.com/article/7611 or contact ZEGO technical support.
    /// Caution: 'key' is non nullptr. The length of key and value is limited, please refer to Restrictions. The newly set key and value will overwrite the old setting.
    /// Related callbacks: Other users in the same room will be notified through the [onRoomExtraInfoUpdate] callback function.
    /// Related APIs: None.
    ///
    /// @param roomID Room ID.
    /// @param key key of the extra info.
    /// @param value value if the extra info.
    /// @param callback Callback for setting room extra information.
    virtual void setRoomExtraInfo(const std::string &roomID, const std::string &key,
                                  const std::string &value,
                                  ZegoRoomSetRoomExtraInfoCallback callback) = 0;

    /// Get room stream list.
    ///
    /// Available since: 3.12.0
    /// Description: Get room stream list.
    /// Use cases: Get room stream list.
    /// When to call /Trigger: After logging in the room successful.
    /// Caution: This interface is to get a real-time internal stream list, which may be inaccurate when the room is disconnected from the service. Do not call this interface with high frequency.
    /// Related APIs: None.
    ///
    /// @param roomID Room ID.
    /// @param streamListType Get type
    /// @return return stream list
    virtual ZegoRoomStreamList getRoomStreamList(const std::string &roomID,
                                                 ZegoRoomStreamListType streamListType) = 0;

    /// Starts publishing a stream (for the specified channel). You can call this function to publish a second stream.
    ///
    /// Available since: 1.1.0
    /// Description: Users push their local audio and video streams to the ZEGO RTC server or CDN, and other users in the same room can pull the audio and video streams to watch through the `streamID` or CDN pull stream address.
    /// Use cases: It can be used to publish streams in real-time connecting wheat, live broadcast and other scenarios.
    /// When to call: After [loginRoom].
    /// Restrictions: None.
    /// Caution:
    ///   1. Before start to publish the stream, the user can choose to call [setVideoConfig] to set the relevant video parameters, and call [startPreview] to preview the video.
    ///   2. Other users in the same room can get the streamID by monitoring the [onRoomStreamUpdate] event callback after the local user publishing stream successfully.
    ///   3. In the case of poor network quality, user publish may be interrupted, and the SDK will attempt to reconnect. You can learn about the current state and error information of the stream published by monitoring the [onPublisherStateUpdate] event.
    ///
    /// @param streamID Stream ID, a string of up to 256 characters.
    ///   Caution:
    ///   1. Stream ID is defined by you.
    ///   2. needs to be globally unique within the entire AppID. If in the same AppID, different users publish each stream and the stream ID is the same, which will cause the user to publish the stream failure. You cannot include URL keywords, otherwise publishing stream and playing stream will fails.
    ///   3. Only support numbers, English characters and '-', '_'.
    /// @param channel Publish stream channel.
    virtual void startPublishingStream(const std::string &streamID,
                                       ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Starts publishing a stream. Support multi-room mode.
    ///
    /// Available since: 1.1.0
    /// Description: Users push their local audio and video streams to the ZEGO RTC server or CDN, and other users in the same room can pull the audio and video streams to watch through the `streamID` or CDN pull stream address.
    /// Use cases: It can be used to publish streams in real-time connecting wheat, live broadcast and other scenarios.
    /// When to call: After [loginRoom].
    /// Restrictions: None.
    /// Caution:
    ///   1. Before start to publish the stream, the user can choose to call [setVideoConfig] to set the relevant video parameters, and call [startPreview] to preview the video.
    ///   2. Other users in the same room can get the streamID by monitoring the [onRoomStreamUpdate] event callback after the local user publishing stream successfully.
    ///   3. In the case of poor network quality, user publish may be interrupted, and the SDK will attempt to reconnect. You can learn about the current state and error information of the stream published by monitoring the [onPublisherStateUpdate] event.
    ///   4. To call [SetRoomMode] function to select multiple rooms, the room ID must be specified explicitly.
    ///
    /// @param streamID Stream ID, a string of up to 256 characters.
    ///   Caution:
    ///   1. Stream ID is defined by you.
    ///   2. needs to be globally unique within the entire AppID. If in the same AppID, different users publish each stream and the stream ID is the same, which will cause the user to publish the stream failure. You cannot include URL keywords, otherwise publishing stream and playing stream will fails.
    ///   3. Only support numbers, English characters and '-', '_'.
    /// @param config Advanced publish configuration.
    /// @param channel Publish stream channel.
    virtual void startPublishingStream(const std::string &streamID, ZegoPublisherConfig config,
                                       ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Starts publishing a stream in RangeScene scene.
    ///
    /// Available since: 3.0.0
    /// Description: Users push their local audio and video streams to the ZEGO RTC server.
    /// Use cases: Starts publishing a stream in RangeScene scene.
    /// When to call: After [loginScene].
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param streamID Stream ID, a string of up to 256 characters.
    ///   Caution:
    ///   1. Stream ID is defined by you.
    ///   2. needs to be globally unique within the entire AppID. If in the same AppID, different users publish each stream and the stream ID is the same, which will cause the user to publish the stream failure. You cannot include URL keywords, otherwise publishing stream and playing stream will fails.
    ///   3. Only support numbers, English characters and '-', '_'.
    /// @param channel Publish stream channel.
    /// @param config Advanced scene publish configuration.
    virtual void startPublishingStreamInScene(const std::string &streamID,
                                              ZegoPublishChannel channel,
                                              ZegoScenePublisherConfig config) = 0;

    /// Stops publishing a stream (for the specified channel).
    ///
    /// Available since: 1.1.0
    /// Description: The user stops sending local audio and video streams, and other users in the room will receive a stream deletion notification.
    /// Use cases: It can be used to stop publish streams in real-time connecting wheat, live broadcast and other scenarios.
    /// When to call: After [startPublishingStream].
    /// Restrictions: None.
    /// Caution:
    ///   1. After stopping the streaming, other users in the same room can receive the delete notification of the stream by listening to the [onRoomStreamUpdate] callback.
    ///   2. If the user has initiated publish flow, this function must be called to stop the publish of the current stream before publishing the new stream (new streamID), otherwise the new stream publish will return a failure.
    ///   3. After stopping streaming, the developer should stop the local preview based on whether the business situation requires it.
    ///
    /// @param channel Publish stream channel.
    virtual void stopPublishingStream(ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Sets the extra information of the stream being published for the specified publish channel.
    ///
    /// Available since: 1.1.0
    /// Description: Use this function to set the extra info of the stream. The stream extra information is an extra information identifier of the stream ID. Unlike the stream ID, which cannot be modified during the publishing process, the stream extra information can be modified midway through the stream corresponding to the stream ID. Developers can synchronize variable content related to stream IDs based on stream additional information.
    /// When to call: After the engine is created [createEngine], Called before and after [startPublishingStream] can both take effect.
    /// Restrictions: None.
    /// Related callbacks: Users can obtain the execution result of the function through [ZegoPublisherSetStreamExtraInfoCallback] callback.
    ///
    /// @param extraInfo Stream extra information, a string of up to 1024 characters.
    /// @param callback Set stream extra information execution result notification.
    /// @param channel Publish stream channel.
    virtual void setStreamExtraInfo(const std::string &extraInfo,
                                    ZegoPublisherSetStreamExtraInfoCallback callback,
                                    ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Starts/Updates the local video preview (for the specified channel).
    ///
    /// Available since: 1.1.0
    /// Description: The user can see his own local image by calling this function.
    /// Use cases: It can be used for local preview in real-time connecting wheat, live broadcast and other scenarios.
    /// When to call: After [createEngine].
    /// Restrictions: None.
    /// Caution: 1. The preview function does not require you to log in to the room or publish the stream first. But after exiting the room, SDK internally actively stops previewing by default. 2. Local view and preview modes can be updated by calling this function again. The user can only preview on one view. If you call [startPreview] again to pass in a new view, the preview screen will only be displayed in the new view. 3. You can set the mirror mode of the preview by calling the [setVideoMirrorMode] function. The default preview setting is image mirrored. 4. When this function is called, the audio and video engine module inside SDK will start really, and it will start to try to collect audio and video.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param canvas The view used to display the preview image. If the view is set to nullptr, no preview will be made.
    /// @param channel Publish stream channel
    virtual void startPreview(ZegoCanvas *canvas,
                              ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Starts the local audio preview.
    ///
    /// Available since: 1.1.0
    /// Description: Call this function after creating the engine and before publishing/playing a stream, and then the engine will start to capture audio.
    /// Use cases: Can check whether the audio equipment works ok by calling this function and by the soundlevel function of Engine.
    /// When to call: After [createEngine].
    /// Restrictions: None.
    /// Caution: 1. The preview function does not require you to log in to the room or publish the stream first. But after exiting the room, SDK internally actively stops previewing by default. 2. When this function is called, the audio engine module inside SDK will start, and it will start to try to collect audio.
    virtual void startPreview() = 0;

    /// Stops the local preview.
    ///
    /// Available since: 1.1.0
    /// Description: This function can be called to stop the preview when the preview is not needed locally.
    /// Caution: Stopping the preview will not affect the publish stream and playing stream functions.
    virtual void stopPreview() = 0;

    /// Stops the local preview (for the specified channel).
    ///
    /// Available since: 1.1.0
    /// Description: This function can be called to stop the preview when the preview is not needed locally.
    /// Caution: Stopping the preview will not affect the publish stream and playing stream functions.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param channel Publish stream channel
    virtual void stopPreview(ZegoPublishChannel channel) = 0;

    /// Sets up the video configurations (for the specified channel).
    ///
    /// Available since: 1.1.0
    /// Description: Set the video frame rate, bit rate, video capture resolution, and video encoding output resolution.
    /// Default value: The default video capture resolution is 360p, the video encoding output resolution is 360p, the bit rate is 600 kbps, and the frame rate is 15 fps.
    /// When to call: After [createEngine].
    /// Restrictions: It is necessary to set the relevant video configuration before [startPreview], and only support the modification of the encoding resolution, the bit rate and the frame rate after [startPreview].
    /// Caution: Developers should note that the wide and high resolution of the mobile end is opposite to the wide and high resolution of the PC. For example, in the case of 360p, the resolution of the mobile end is 360x640, and the resolution of the PC end is 640x360.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param config Video configuration, the SDK provides a common setting combination of resolution, frame rate and bit rate, they also can be customized.
    /// @param channel Publish stream channel.
    virtual void setVideoConfig(ZegoVideoConfig config,
                                ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Gets the current video configurations (for the specified channel).
    ///
    /// This function can be used to get the specified publish channel's current video frame rate, bit rate, video capture resolution, and video encoding output resolution.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param channel Publish stream channel
    /// @return Video configuration object
    virtual ZegoVideoConfig
    getVideoConfig(ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Set dual stream config.
    ///
    /// Available since: 3.7.0
    /// Description: Set the stream config.
    /// When to call: This must take effect when the codecID specified in the call to [ZegoExpressEngine > setVideoConfig] is ZegoVideoCodecIDH264DualStream after [createEngine] is called.
    /// Restrictions: To take effect, the parameters of flow and small flow must be specified at the same time. The resolution ratio of flow and small flow must be the same. For example, both are 4:3 .
    /// Caution: Width, height, resolution and bitrate are all greater than zero to take effect.
    ///
    /// @param configList config info.
    /// @param channel ZegoPublishChannel.
    virtual void
    setPublishDualStreamConfig(const std::vector<ZegoPublishDualStreamConfig> &configList,
                               ZegoPublishChannel channel) = 0;

    /// Sets the video mirroring mode (for the specified channel).
    ///
    /// Available since: 1.1.0
    /// Description: Set whether the local preview video and the published video have mirror mode enabled. For specific mirroring mode.
    /// When to call: After [createEngine].
    /// Restrictions: This setting only works if the SDK is responsible for rendering.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param mirrorMode Mirror mode for previewing or publishing the stream.
    /// @param channel Publish stream channel.
    virtual void setVideoMirrorMode(ZegoVideoMirrorMode mirrorMode,
                                    ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    /// Sets the video orientation (for the specified channel).
    ///
    /// Available since: 1.1.0
    /// Description: Set the video orientation.
    /// Use cases: When users use mobile devices to conduct live broadcasts or video calls, they can set different video directions according to the scene.
    /// When to call: After [createEngine].
    /// Restrictions: Currently only supports iOS and Android platforms.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param orientation Video orientation.
    /// @param channel Publish stream channel.
    virtual void setAppOrientation(ZegoOrientation orientation,
                                   ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;
#endif

    /// Sets up the audio configurations for the specified publish channel.
    ///
    /// Available since: 1.3.4
    /// Description: You can set the combined value of the audio codec, bit rate, and audio channel through this function. If the preset value cannot meet the developer's scenario, the developer can set the parameters according to the business requirements.
    /// Default value: The default audio config refers to the default value of [ZegoAudioConfig].
    /// When to call: After the engine is created [createEngine], and before publishing [startPublishingStream].
    /// Restrictions: None.
    /// Related APIs: [getAudioConfig].
    ///
    /// @param config Audio config.
    /// @param channel Publish stream channel.
    virtual void setAudioConfig(ZegoAudioConfig config,
                                ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Gets the current audio configurations from the specified publish channel.
    ///
    /// Available since: 1.8.0
    /// Description: You can get the current audio codec, bit rate, and audio channel through this function.
    /// When to call: After the engine is created [createEngine].
    /// Restrictions: None.
    /// Related APIs: [setAudioConfig].
    ///
    /// @param channel Publish stream channel.
    /// @return Audio config.
    virtual ZegoAudioConfig
    getAudioConfig(ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Set encryption key for the publishing stream for the specified publish channel.
    ///
    /// Available since: 1.19.0
    /// Description: Support calling this function to update the encryption key while publishing stream.
    /// When to call: After the engine is created [createEngine], Called before and after [startPublishingStream] can both take effect.
    /// Restrictions: This function is only valid when publishing stream to the Zego RTC server.
    /// Caution: Note that developers need to update the player's decryption key before updating the publisher's encryption key.
    /// Related APIs: Calling [stopPublishingStream] or [logoutRoom] will clear the encryption key.
    ///
    /// @param key The encryption key, note that the key length only supports 16/24/32 bytes.
    /// @param channel Publish stream channel.
    virtual void
    setPublishStreamEncryptionKey(const std::string &key,
                                  ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Take a snapshot of the publishing stream for the specified publish channel.
    ///
    /// Available since: 1.17.0
    /// Description: Take a snapshot of the publishing stream.
    /// When to call: Called this function after calling [startPublishingStream] or [startPreview].
    /// Restrictions: None.
    /// Caution: The resolution of the snapshot is the encoding resolution set in [setVideoConfig]. If you need to change it to capture resolution, please call [setCapturePipelineScaleMode] to change the capture pipeline scale mode to [Post].
    /// Related callbacks: The screenshot result will be called back through [ZegoPublisherTakeSnapshotCallback].
    /// Related APIs: [takePlayStreamSnapshot].
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param callback Results of take publish stream snapshot.
    /// @param channel Publish stream channel.
    virtual void
    takePublishStreamSnapshot(ZegoPublisherTakeSnapshotCallback callback,
                              ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Stops or resumes sending the audio part of a stream for the specified channel.
    ///
    /// Available since: 1.1.0
    /// Description: This function can be called when publishing the stream to realize not publishing the audio data stream. The SDK still collects and processes the audio, but send muted audio frame packets to the network.
    /// Use case: Users can call this interface when they do not want to publish any audio data. This interface does not affect [onBeforeAudioPrepAudioData].
    /// When to call: Called after the engine is created [createEngine] can take effect.
    /// Restrictions: None.
    /// Related callbacks: If you stop sending audio streams, the remote user that play stream of local user publishing stream can receive `Mute` status change notification by monitoring [onRemoteMicStateUpdate] callbacks.
    /// Related APIs: [mutePublishStreamVideo].
    ///
    /// @param mute Whether to stop sending audio streams, true means not to send audio stream, and false means sending audio stream. The default is false.
    /// @param channel Publish stream channel.
    virtual void mutePublishStreamAudio(bool mute,
                                        ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Stops or resumes sending the video part of a stream for the specified channel.
    ///
    /// Available since: 1.1.0
    /// Description: This function can be called when publishing the stream to realize not publishing the video stream. The local camera can still work normally, can capture, preview and process video images normally, but does not send the video data to the network.
    /// When to call: Called after the engine is created [createEngine] can take effect.
    /// Restrictions: None.
    /// Related callbacks: If you stop sending video streams locally, the remote user that play stream of local user publishing stream can receive `Mute` status change notification by monitoring [onRemoteCameraStateUpdate] callbacks.
    /// Related APIs: [mutePublishStreamAudio].
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param mute Whether to stop sending video streams, true means not to send video stream, and false means sending video stream. The default is false.
    /// @param channel Publish stream channel.
    virtual void mutePublishStreamVideo(bool mute,
                                        ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Enable or disable the stream precision alignment function.
    ///
    /// Available since: 2.11.0.
    /// Description: Use this interface to enable stream alignment, the SDK will attach network time information to the stream when publishing it for accurate stream alignment.
    /// Use case: Generally used in scenarios such as KTV where stream mixing alignment is required.
    /// When to call: After the engine is created [createEngine].
    /// Caution: If you need to align each stream through network time when playing multiple streams or mixed streams, you need to call [startPublishingStream] to publish the stream and [forceSynchronousNetworkTime] in [ZegoPublisherConfig] is 1 to enable network time synchronization.
    /// Related APIs: [startMixerTask], [startAutoMixerTask]
    ///
    /// @param alignment Whether to enable the stream mixing precision alignment function.
    /// @param channel Publish stream channel
    virtual void
    setStreamAlignmentProperty(int alignment,
                               ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Enables or disables the traffic control for the specified publish channel.
    ///
    /// Available since: 1.5.0
    /// Description: Enabling traffic control allows the SDK to adjust the audio and video streaming bitrate according to the current upstream network environment conditions, or according to the counterpart's downstream network environment conditions in a one-to-one interactive scenario, to ensure smooth results. At the same time, you can further specify the attributes of traffic control to adjust the corresponding control strategy.
    /// Default value: Enable.
    /// When to call: After the engine is created [createEngine], Called before [startPublishingStream] can take effect.
    /// Restrictions: Only support RTC publish.
    ///
    /// @param enable Whether to enable traffic control. The default is ture.
    /// @param property Adjustable property of traffic control, bitmask format. Should be one or the combinations of [ZegoTrafficControlProperty] enumeration. [AdaptiveFPS] as default.
    /// @param channel Publish stream channel.
    virtual void enableTrafficControl(bool enable, int property,
                                      ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Sets the minimum video bitrate for traffic control for the specified publish channel.
    ///
    /// Available since: 1.1.0
    /// Description: Set the control strategy when the video bitrate reaches the lowest threshold during flow control. When the bitrate is lower than the minimum threshold, you can choose not to send video data or send it at a very low frame bitrate.
    /// Default value: There is no control effect of the lowest threshold of video bitrate.
    /// When to call: After the engine is created [createEngine], Called before [startPublishingStream] can take effect.
    /// Restrictions: The traffic control must be turned on [enableTrafficControl].
    /// Related APIs: [enableTrafficControl].
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param bitrate Minimum video bitrate (kbps).
    /// @param mode Video sending mode below the minimum bitrate.
    /// @param channel Publish stream channel.
    virtual void
    setMinVideoBitrateForTrafficControl(int bitrate, ZegoTrafficControlMinVideoBitrateMode mode,
                                        ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Sets the minimum video frame rate threshold for traffic control.
    ///
    /// Available since: 2.17.0
    /// Description: When enabling traffic control [enableTrafficControl], and its parameter [property] includes the attribute AdaptiveFPS, the minimum frame rate of the video will not be lower than the value set by the [setMinVideoFpsForTrafficControl] interface. A value of 0 indicates no limit.
    /// Default value: There is no control effect of the lowest threshold of video frame rate.
    /// When to call: The call takes effect after the engine [createEngine] is created.
    /// Restrictions: The traffic control must be turned on [enableTrafficControl]. And its parameter [property] must contain AdaptiveFPS, Please refer to [ZegoTrafficControlProperty] for details.
    /// Related APIs: [enableTrafficControl].
    /// Caution: If you need to cancel the limit of the setting value, you can set it to 0.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param fps The minimum video frame rate threshold for traffic control(fps).
    /// @param channel Publish stream channel.
    virtual void
    setMinVideoFpsForTrafficControl(int fps,
                                    ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Sets the minimum video resolution threshold for traffic control.
    ///
    /// Available since: 2.17.0
    /// Description: When enabling traffic control [enableTrafficControl], and its parameter [property] includes the attribute AdaptiveResolution, the minimum resolution of the video will not be lower than the value set by the [setMinVideoResolutionForTrafficControl] interface. A value of 0 indicates no limit.
    /// Default value: There is no control effect of the lowest threshold of video resolution.
    /// When to call: The call takes effect after the engine [createEngine] is created.
    /// Restrictions: The traffic control must be turned on [enableTrafficControl]. And its parameter [property] must contain AdaptiveResolution, Please refer to [ZegoTrafficControlProperty] for details.
    /// Related APIs: [enableTrafficControl].
    /// Caution: If you need to cancel the limit of the setting value, you can set width and height to 0.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param width The flow controls the width of the lowest video resolution.
    /// @param height The flow controls the height of the lowest video resolution.
    /// @param channel Publish stream channel.
    virtual void setMinVideoResolutionForTrafficControl(
        int width, int height, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Set the factors of concern that trigger traffic control for the specified publish channel.
    ///
    /// Available since: 2.4.0
    /// Description: Use this interface to control whether to start traffic control due to poor remote network conditions.
    /// Default value: Default is disable.
    /// When to call: After the engine is created [createEngine], Called before [startPublishingStream] can take effect.
    /// Restrictions: The traffic control must be turned on [enableTrafficControl].
    /// Related APIs: [enableTrafficControl.
    ///
    /// @param mode When LOCAL_ONLY is selected, only the local network status is concerned. When choosing REMOTE, also take into account the remote network.
    /// @param channel Publish stream channel.
    virtual void
    setTrafficControlFocusOn(ZegoTrafficControlFocusOnMode mode,
                             ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Sets the audio recording volume for stream publishing.
    ///
    /// Available since: 1.13.0
    /// Description: This function is used to perform gain processing based on the device's collected volume. The local user can control the sound level of the audio stream sent to the remote end.
    /// Default value: Default is 100.
    /// When to call: After creating the engine [createEngine].
    /// Restrictions: The capture volume can be dynamically set during publishing.
    /// Related APIs: Set the playing stream volume [setPlayVolume].
    ///
    /// @param volume The volume gain percentage, the range is 0 ~ 200, and the default value is 100, which means 100% of the original collection volume of the device.
    virtual void setCaptureVolume(int volume) = 0;

    /// Gets the audio recording volume for stream publishing.
    ///
    /// Available since: 1.13.0
    /// Description: This function is used to get the device's collected volume.
    /// When to call: After creating the engine [createEngine].
    /// Related APIs: Set the capture stream volume [setCaptureVolume].
    ///
    /// @return The volume gain percentage.
    virtual int getCaptureVolume() = 0;

    /// Set audio capture stereo mode.
    ///
    /// Available since: 1.15.0 (iOS/Android/Windows/OHOS); support macOS since 2.16.0
    /// Description: This function is used to set the audio capture channel mode. When the developer turns on the stereo capture, using a special stereo capture device, the stereo audio data can be captured and streamed.
    /// Use cases: In some professional scenes, users are particularly sensitive to sound effects, such as voice radio and musical instrument performance. At this time, support for stereo and high-quality sound is required.
    /// Default value: The default is None, which means mono capture.
    /// When to call: It needs to be called after [createEngine]， before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager].
    /// Restrictions: If you need to enable stereo capture, you also need to meet the following conditions: For iOS/Android, you need to connect an external audio device that supports stereo capture and be at the media volume. For macOS, it needs to be at the media volume. For Windows, an external audio device that supports stereo capture is required.
    /// Related APIs: When streaming, you need to enable the stereo audio encoding function through the [setAudioConfig] interface at the same time.
    ///
    /// @param mode Audio stereo capture mode.
    virtual void setAudioCaptureStereoMode(ZegoAudioCaptureStereoMode mode) = 0;

    /// Adds a target CDN URL to which the stream will be relayed from ZEGO RTC server.
    ///
    /// Available since: 1.1.0
    /// Description: Forward audio and video streams from ZEGO RTC servers to custom CDN content distribution networks with high latency but support for high concurrent pull streams.
    /// Use cases: 1. It is often used in large-scale live broadcast scenes that do not have particularly high requirements for delay. 2. Since ZEGO RTC server itself can be configured to support CDN(content distribution networks), this function is mainly used by developers who have CDN content distribution services themselves. 3. This function supports dynamic relay to the CDN content distribution network, so developers can use this function as a disaster recovery solution for CDN content distribution services.
    /// When to call: After calling the [createEngine] function to create the engine.
    /// Restrictions: When the [enablePublishDirectToCDN] function is set to true to publish the stream straight to the CDN, then calling this function will have no effect.
    /// Caution: Removing URLs retweeted to CDN requires calling [removePublishCdnUrl], calling [stopPublishingStream] will not remove URLs publish to CDN.
    /// Related APIs: Remove URLs that are re-pushed to the CDN [removePublishCdnUrl].
    ///
    /// @param streamID Stream ID.
    /// @param targetURL CDN relay address, supported address format is rtmp, rtmps.
    /// @param callback The execution result of update the relay CDN operation.
    virtual void addPublishCdnUrl(const std::string &streamID, const std::string &targetURL,
                                  ZegoPublisherUpdateCdnUrlCallback callback) = 0;

    /// Adds a target CDN URL to which the stream will be relayed from ZEGO RTC server.
    ///
    /// Available since: 1.1.0
    /// Description: Forward audio and video streams from ZEGO RTC servers to custom CDN content distribution networks with high latency but support for high concurrent pull streams.
    /// Use cases: 1. It is often used in large-scale live broadcast scenes that do not have particularly high requirements for delay. 2. Since ZEGO RTC server itself can be configured to support CDN(content distribution networks), this function is mainly used by developers who have CDN content distribution services themselves. 3. This function supports dynamic relay to the CDN content distribution network, so developers can use this function as a disaster recovery solution for CDN content distribution services.
    /// When to call: After calling the [createEngine] function to create the engine.
    /// Restrictions: When the [enablePublishDirectToCDN] function is set to true to publish the stream straight to the CDN, then calling this function will have no effect.
    /// Caution: Removing URLs retweeted to CDN requires calling [removePublishCdnUrl], calling [stopPublishingStream] will not remove URLs publish to CDN.
    /// Related APIs: Remove URLs that are re-pushed to the CDN [removePublishCdnUrl].
    ///
    /// @param streamID Stream ID.
    /// @param targetURL CDN relay address, supported address format is rtmp, rtmps.
    /// @param timeout Timeout. Callback if it does not start in the time. Default is 0, which means no timeout. Valid range is [5, 600], in seconds. Less than 0 will be reset to 0, 1 to 4 will be reset to 5, and a greater than 600 will be reset to 600.
    /// @param callback The execution result of update the relay CDN operation.
    virtual void addPublishCdnUrl(const std::string &streamID, const std::string &targetURL,
                                  int timeout, ZegoPublisherUpdateCdnUrlCallback callback) = 0;

    /// Deletes the specified CDN URL, which is used for relaying streams from ZEGO RTC server to CDN.
    ///
    /// Available since: 1.1.0
    /// Description: When a CDN forwarding address has been added via [addPublishCdnUrl], this function is called when the stream needs to be stopped.
    /// When to call: After calling the [createEngine] function to create the engine, When you don't need to continue publish to the CDN.
    /// Restrictions: When the [enablePublishDirectToCDN] function is set to true to publish the stream straight to the CDN, then calling this function will have no effect.
    /// Caution: This function does not stop publishing audio and video stream to the ZEGO ZEGO RTC server.
    /// Related APIs: Add URLs that are re-pushed to the CDN [addPublishCdnUrl].
    ///
    /// @param streamID Stream ID.
    /// @param targetURL CDN relay address, supported address format rtmp.
    /// @param callback The execution result of update the relay CDN operation.
    virtual void removePublishCdnUrl(const std::string &streamID, const std::string &targetURL,
                                     ZegoPublisherUpdateCdnUrlCallback callback) = 0;

    /// Whether to directly push to CDN (without going through the ZEGO RTC server), for the specified channel.
    ///
    /// Available since: 1.5.0
    /// Description: Whether to publish streams directly from the client to CDN without passing through Zego RTC server.
    /// Use cases: It is often used in large-scale live broadcast scenes that do not have particularly high requirements for delay.
    /// Default value: The default is false, and direct push is not enabled.
    /// When to call: After creating the engine [createEngine], before starting to push the stream [startPublishingStream].
    /// Caution: The Direct Push CDN feature does not pass through the ZEGO Real-Time Audio and Video Cloud during network transmission, so you cannot use ZEGO's ultra-low latency audio and video services.
    /// Related APIs: Dynamic re-push to CDN function [addPublishCdnUrl], [removePublishCdnUrl].
    ///
    /// @param enable Whether to enable direct publish CDN, true: enable direct publish CDN, false: disable direct publish CDN.
    /// @param config CDN configuration, if nullptr, use Zego's background default configuration.
    /// @param channel Publish stream channel.
    virtual void
    enablePublishDirectToCDN(bool enable, ZegoCDNConfig *config,
                             ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Sets up the stream watermark before stream publishing (for the specified channel).
    ///
    /// Available since: 1.1.0
    /// Description: Set watermark for publish stream screen.
    /// Use cases: It is often used to identify the source of the publish.
    /// When to call: After creating the engine through [createEngine] function.
    /// Caution: The layout of the watermark cannot exceed the video encoding resolution of the stream. It can be set at any time before or during the publishing stream.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param watermark The upper left corner of the watermark layout is the origin of the coordinate system, and the area cannot exceed the size set by the encoding resolution. If it is nullptr, the watermark is cancelled.
    /// @param isPreviewVisible the watermark is visible on local preview
    /// @param channel Publish stream channel.
    virtual void setPublishWatermark(ZegoWatermark *watermark, bool isPreviewVisible,
                                     ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Set the Supplemental Enhancement Information type.
    ///
    /// Available since: 1.18.0
    /// Description: By default, the SDK wraps the data with ZEGO's self-defined SEI type, which is not specified by the SEI standard. When the developer needs to use a third-party decoder to decode the SEI, the correct SEI will not be decoded and the [setSEIConfig] interface needs to be called to change the type of the SEI sent by the SDK to UserUnregister type.
    /// Use cases: This function needs to be executed when the developer uses a third-party decoder to decode the SEI.
    /// When to call: After creating the engine [createEngine], before starting to push the stream [startPublishingStream].
    /// Restrictions: None.
    ///
    /// @param config SEI configuration. The SEI defined by ZEGO is used by default.
    virtual void setSEIConfig(ZegoSEIConfig config) = 0;

    /// Sends Supplemental Enhancement Information to the specified publish channel.
    ///
    /// Available since: 1.1.0
    /// Description: While pushing the stream to transmit the audio and video stream data, the stream media enhancement supplementary information is sent to synchronize some other additional information.
    /// Use cases: Generally used in scenes such as synchronizing music lyrics or precise video layout, you can choose to send SEI.
    /// When to call: After starting to push the stream [startPublishingStream].
    /// Restrictions: Do not exceed 30 times per second, and the SEI data length is limited to 4096 bytes.
    /// Caution: 1. Due to network issues, frame loss may occur, which means SEI information may also be lost. To address this situation, it is advisable to send it multiple times within a limited frequency. 2. Even if the [enableCamera] interface is called to turn off the camera or [mutePublishStreamVideo] is used to stop sending video data, SEI can still be successfully sent; as long as the playback side does not call the [mutePlayStreamVideo] interface to stop pulling audio data, SEI can still be received normally. 3. If the SDK does not support the video module but does support the SEI functional module, SEI information can still be sent normally.
    /// Related APIs: After the pusher sends the SEI, the puller can obtain the SEI content by monitoring the callback of [onPlayerRecvSEI].
    ///
    /// @param data SEI data.
    /// @param dataLength SEI data length.
    /// @param channel Publish stream channel.
    virtual void sendSEI(const unsigned char *data, unsigned int dataLength,
                         ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Send the media supplementary enhancement information synchronized with the current video frame during custom video capture.
    ///
    /// Available since: 2.15.0
    /// Description: When using custom video capture, push the stream to transmit the video stream data, and send the stream media enhancement supplementary information to synchronize some other additional information, which is synchronized with the current video frame.
    /// Use cases: Generally used in scenes that need to be played back in strong synchronization with the video frame, such as video karaoke, video and lyrics are strongly synchronized; or scenes that need to be accurate to the frame level for strategic processing.
    /// When to call: To ensure that the SEI is synchronized with the current video frame, it must be called before sending the video frame data.
    /// Restrictions: Only used for custom video capture; only supports video-driven SEI sending; you should try to avoid calling this interface to send SEI continuously; to ensure that SEI is synchronized with the current video frame, it must be kept in the same thread as the interface for sending custom video frame data; SEI data length is limited to 4096 bytes.
    /// Caution: The sent SEI information follows the video frame. Due to network problems, there may be frame loss. At this time, the SEI will follow the next frame of video data. Therefore, in order to keep the SEI synchronized with the video frame, you should avoid sending SEI continuously; only when using SurfaceTexture on the Android platform , Need to pass the time stamp parameter timeStampNs, otherwise the parameter is invalid.
    /// Related APIs: After the pusher sends the SEI, the puller can obtain the SEI content by monitoring the callback of [onPlayerRecvSEI].
    ///
    /// @param data SEI data.
    /// @param dataLength SEI data length.
    /// @param timeStampNs video frame reference time, UNIX timestamp, in nanosecond.
    /// @param channel Publish stream channel.
    virtual void
    sendSEISyncWithCustomVideo(const unsigned char *data, unsigned int dataLength,
                               unsigned long long timeStampNs,
                               ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Send audio side information.
    ///
    /// Available since: 2.19.0
    /// Description: While streaming audio data, send audio side information to synchronize some additional information.
    /// Use cases: In the carousel scene, the audio data channel is required to carry accompanying information, such as timestamps to help align the accompaniment, and the need to know who the user is currently singing, whether to amplify the volume, and so on.
    /// When to call: After starting to push the stream [startPublishingStream].
    /// Restrictions: 1. This function is only valid when publishing stream to the Zego RTC server and it also doesn't work when retweeting the stream from the RTC server to the CDN. 2. The audio side information data length is limited to 1024 bytes.
    /// Caution: 1. Audio side information is driven by audio data, so audio data must be pushed (audio side information may be lost when the DTX function is enabled via the interface [setEngineConfig]). 2. Due to network issues, audio side information may be lost, and the SDK is responsible for transmission but does not guarantee reliability.
    /// Related APIs: After the pusher sends the side information, the puller can obtain the side information content by monitoring the callback of [onPlayerRecvAudioSideInfo].
    ///
    /// @param data Audio side info data.
    /// @param dataLength Audio side info data length.
    /// @param timeStampMs timeStampMs, derived from custom audio processing, in milliseconds. If you fill in 0, it is sent along with the frame that is currently ready to be sent.
    /// @param channel Publish stream channel.
    virtual void sendAudioSideInfo(const unsigned char *data, unsigned int dataLength,
                                   double timeStampMs,
                                   ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Enables or disables hardware encoding.
    ///
    /// Available since: 1.1.0
    /// Description: Whether to use the hardware encoding function when publishing the stream, the GPU is used to encode the stream and to reduce the CPU usage.
    /// When to call: The setting can take effect before the stream published. If it is set after the stream published, the stream should be stopped first before it takes effect.
    /// Caution: Because hard-coded support is not particularly good for a few models, SDK uses software encoding by default. If the developer finds that the device is hot when publishing a high-resolution audio and video stream during testing of some models, you can consider calling this function to enable hard coding.
    ///
    /// @param enable Whether to enable hardware encoding, true: enable hardware encoding, false: disable hardware encoding.
    virtual void enableHardwareEncoder(bool enable) = 0;

    /// Sets the timing of video scaling in the video capture workflow. You can choose to do video scaling right after video capture (the default value) or before encoding.
    ///
    /// Available since: 1.1.0
    /// When to call: This function needs to be set before call [startPreview] or [startPublishingStream].
    /// Caution: The main effect is Whether the local preview is affected when the acquisition resolution is different from the encoding resolution.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param mode The capture scale timing mode.
    virtual void setCapturePipelineScaleMode(ZegoCapturePipelineScaleMode mode) = 0;

    /// Set the path of the static picture would be published when the camera is closed.
    ///
    /// Available: since 2.9.0
    /// Description: Set the path of the static picture would be published when enableCamera(false) is called, it would start to publish static pictures, and when enableCamera(true) is called, it would end publishing static pictures.
    /// Use case: The developer wants to display a static picture when the camera is closed. For example, when the anchor exits the background, the camera would be actively closed. At this time, the audience side needs to display the image of the anchor temporarily leaving.
    /// When to call: After the engine is initialized, call this API to configure the parameters before closing the camera.
    /// Restrictions:
    ///   1. Supported picture types are JPEG/JPG, PNG, BMP, HEIF.
    ///   2. The function is only for SDK video capture and does not take effect for custom video capture.
    ///   3. Not supported that the filePath is a network link.
    /// Caution:
    ///   1. The static picture cannot be seen in the local preview.
    ///   2. External filters, mirroring, watermarks, and snapshots are all invalid.
    ///   3. If the picture aspect ratio is inconsistent with the set code aspect ratio, it will be cropped according to the code aspect ratio.
    /// Platform differences:
    ///   1. Windows: Fill in the location of the picture directly, such as "D://dir//image.jpg".
    ///   2. iOS: If it is a full path, add the prefix "file:", such as @"file:/var/image.png"; If it is a assets picture path, add the prefix "asset:", such as @"asset:watermark".
    ///   3. Android: If it is a full path, add the prefix "file:", such as "file:/sdcard/image.png"; If it is a assets directory path, add the prefix "asset:", such as "asset:watermark.png".
    ///   4. Flutter: If it is a absolute path, add the prefix "file:", such as "file:/sdcard/image.png"; If it is a assets resources directory path, add the prefix "flutter-asset://", such as "flutter-asset://assets/watermark.png".
    ///   5. UniApp: Only absolute paths are supported. You need to add a "file:" prefix, such as: "file:/sdcard/image.png".
    ///
    /// @param filePath Picture file path
    /// @param channel Publish channel.
    virtual void
    setDummyCaptureImagePath(const std::string &filePath,
                             ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Set the params of the static picture would be published when the camera is closed.
    ///
    /// Available: since 3.19.0
    /// Description: Set the params of the static picture would be published when enableCamera(false) is called, it would start to publish static pictures, and when enableCamera(true) is called, it would end publishing static pictures.
    /// Use case: The developer wants to display a static picture when the camera is closed. For example, when the anchor exits the background, the camera would be actively closed. At this time, the audience side needs to display the image of the anchor temporarily leaving.
    /// When to call: After the engine is initialized, call this API to configure the parameters before closing the camera.
    /// Restrictions:
    ///   1. Supported picture types are JPEG/JPG, PNG, BMP, HEIF.
    ///   2. The function is only for SDK video capture and does not take effect for custom video capture.
    ///   3. Not supported that the filePath is a network link.
    /// Caution:
    ///   1. The static picture cannot be seen in the local preview.
    ///   2. External filters, mirroring, watermarks, and snapshots are all invalid.
    ///   3. If the picture aspect ratio is inconsistent with the set code aspect ratio, it will be cropped according to the code aspect ratio.
    /// Platform differences:
    ///   1. Windows: Fill in the location of the picture directly, such as "D://dir//image.jpg".
    ///   2. iOS: If it is a full path, add the prefix "file:", such as @"file:/var/image.png"; If it is a assets picture path, add the prefix "asset:", such as @"asset:watermark".
    ///   3. Android: If it is a full path, add the prefix "file:", such as "file:/sdcard/image.png"; If it is a assets directory path, add the prefix "asset:", such as "asset:watermark.png".
    ///   4. Flutter: If it is a absolute path, add the prefix "file:", such as "file:/sdcard/image.png"; If it is a assets resources directory path, add the prefix "flutter-asset://", such as "flutter-asset://assets/watermark.png".
    ///   5. UniApp: Only absolute paths are supported. You need to add a "file:" prefix, such as: "file:/sdcard/image.png".
    ///
    /// @param params Dummy capture image params.
    /// @param channel Publish channel.
    virtual void
    setDummyCaptureImageParams(ZegoDummyCaptureImageParams params,
                               ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Whether to enable H.265 encoding to automatically downgrade to H.264 encoding.
    ///
    /// Available since: 2.12.0
    /// Description: When using H.265 encoding to push the stream, whether to enable the strategy of automatically degrading H.265 encoding to H.264 encoding under abnormal circumstances.After enabling automatic downgrade, when H.265 encoding is not supported or H.265 encoding fails, the SDK will try to downgrade and use H.264 encoding to push the stream.After turning off automatic downgrade, when H.265 encoding is not supported or H.265 encoding fails, the direct streaming fails.
    /// Use cases: In the Co-hosting and Showroom Live Streaming scenarios, use H265 encoding to push the stream to save CDN traffic without degrading the picture quality.
    /// Default Value: When this interface is not called, the default is yes, which means that H.265 encoding is turned on and automatically downgraded to H.264 encoding.
    /// When to call: After creating the engine, call the [startPublishingStream] function before pushing the stream.
    /// Related callbacks: When the H.265 encoding is automatically downgraded to the H.264 encoding strategy, the [onPublisherVideoEncoderChanged] callback will be triggered when the encoding method changes.
    /// Caution: When downgrading from H.265 to H.264 encoding occurs during the streaming process, if you are recording local video or cloud recording, multiple recording files will be generated, which needs to be dealt with.
    ///
    /// @param enable Whether to enable H.265 coding automatically fallback to H.264 coding, true: enable, false: disable, and the default value is true
    virtual void enableH265EncodeFallback(bool enable) = 0;

    /// Whether the specified video encoding type is supported.
    ///
    /// Available since: 2.12.0 and above
    /// Description: Whether the specified video encoding is supported depends on the following aspects, whether the hardware model supports hard encoding, whether the performance of the hardware model supports soft encoding, and whether the SDK has the encoding module.
    /// When to call: After creating the engine.
    /// Caution: It is recommended that users call this interface to obtain H.265 encoding support capability before publish stream with H.265 encoding, if not supported, you can use other encodings for publish, such as H.264.On the mobile platform, the SDK only supports H.265 hardware encoding, and it is affected by the model and hardware capabilities.
    ///
    /// @param codecID Video codec id. Required: Yes.
    /// @return Whether the specified video encoding is supported.Value range: true means support, you can use this encoding format for publish; false means the is not supported, and the encoding format cannot be used for publish.
    virtual bool isVideoEncoderSupported(ZegoVideoCodecID codecID) = 0;

    /// Whether the specified video encoding type and implementation are supported.
    ///
    /// Available since: 3.0.0 and above
    /// Description: Whether the specified video encoding is supported depends on the following aspects, whether the hardware model supports hard encoding, whether the performance of the hardware model supports soft encoding, and whether the SDK has the encoding module.
    /// When to call: After creating the engine.
    /// Caution: It is recommended that users call this interface to obtain H.265 encoding support capability before publish stream with H.265 encoding, if not supported, you can use other encodings for publish, such as H.264.On the mobile platform, the SDK only supports H.265 hardware encoding, and it is affected by the model and hardware capabilities.
    ///
    /// @param codecID Video codec id. Required: Yes.
    /// @param codecBackend Backend implementation of encoder. Required: Yes.
    /// @return Whether the specified video encoding format is supported; 0 means not supported, and the encoding format cannot be used for publish stream; 1 means support, you can use this encoding format for publish stream; 2 means not confirmed, it is recommended to call this interface later.
    virtual int isVideoEncoderSupported(ZegoVideoCodecID codecID,
                                        ZegoVideoCodecBackend codecBackend) = 0;

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    /// Set the orientation mode of the video.
    ///
    /// Available since: 2.23.0
    /// Description: In order to simplify the complexity of processing video screen rotation for mobile developers, the SDK supports setting multiple video orientation modes, and developers can choose different modes according to the needs of the scene.
    /// Use cases: Scenarios for live streaming or video calls using mobile devices.
    /// Default value: Custom mode.
    /// When to call: This function needs to be valid after calling [createEngine] and before calling preview [startPreview] or push stream [startPublishingStream].
    /// Caution:
    ///   1. It is valid for all channels.
    ///   2. The adaptive mode takes effect in preview, streaming, and mixed streaming scenarios. It does not support external video capture, media player, cloud recording, local recording, or publishing/playing stream through CDN.
    /// Related APIs: You can call the [setAppOrientation] function to set the orientation of the App in custom mode.
    ///
    /// @param mode Orientation mode of the video.
    virtual void setAppOrientationMode(ZegoOrientationMode mode) = 0;
#endif

    /// Set low light enhancement.
    ///
    /// Available since: 2.21.0
    /// Description: According to the set low-light enhancement mode, the brightness of the image captured by the camera is enhanced, which is compatible with the beauty function. Users can watch the effect while previewing and toggle the low-light enhancement mode in real time.
    /// Use cases: The environment on the streaming end is dark, or the frame rate set by the camera is high, which causes the picture to be dark, and the subject cannot be displayed or recognized normally.
    /// Default value: Off.
    /// When to call: After creating the engine [createEngine].
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param mode Low light enhancement mode.
    /// @param channel Publish stream channel.
    virtual void setLowlightEnhancement(ZegoLowlightEnhancementMode mode,
                                        ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Set low light enhancement params.
    ///
    /// Available since: 3.19.0
    /// Description: According to the set low-light enhancement mode, the brightness of the image captured by the camera is enhanced, which is compatible with the beauty function. Users can watch the effect while previewing and toggle the low-light enhancement mode in real time.
    /// Use cases: The environment on the streaming end is dark, or the frame rate set by the camera is high, which causes the picture to be dark, and the subject cannot be displayed or recognized normally.
    /// When to call: After creating the engine [createEngine].
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param params Low light enhancement params.
    /// @param channel Publish stream channel.
    virtual void
    setLowlightEnhancementParams(ZegoExpLowlightEnhancementParams params,
                                 ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Set video capture source.
    ///
    /// Available since: 3.1.0
    /// Description: Set video capture source for switching between different video capture sources.
    /// Use cases: Typically used in educational scenarios that require switching between different video capture sources.
    /// When to call: After the engine is created [createEngine].
    /// Restrictions: None.
    /// Caution: 1. Act on the main publish channel ZegoPublishChannel.Main.
    ///  2. Main push channel ZegoPublishChannel.Main does not support using ZegoVideoSourceType.Player and ZegoVideoSourceType.MainPublishChannel video source type.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param source Video capture source.
    virtual int setVideoSource(ZegoVideoSourceType source) = 0;

    /// Set a video capture instance as the video capture source.
    ///
    /// Available since: 3.1.0
    /// Description: Set video capture source for switching between different video capture sources.
    /// Use cases: Typically used in educational scenarios that require switching between different video capture sources.
    /// When to call: After the engine is created [createEngine].
    /// Restrictions: None.
    /// Caution: 1. Act on the main publish channel ZegoPublishChannel.Main.
    ///  2. Main push channel ZegoPublishChannel.Main does not support using ZegoVideoSourceType.Player and ZegoVideoSourceType.MainPublishChannel video source type.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param source Video capture source.
    /// @param instanceID Video capture source instance id.
    virtual int setVideoSource(ZegoVideoSourceType source, unsigned int instanceID) = 0;

    /// Set video capture source for the specified channel.
    ///
    /// Available since: 3.1.0
    /// Description: Set video capture source for switching between different video capture sources.
    /// Use cases: Typically used in educational scenarios that require switching between different video capture sources.
    /// When to call: After the engine is created [createEngine].
    /// Restrictions: None.
    /// Caution: 1. Main push channel ZegoPublishChannel.Main does not support using ZegoVideoSourceType.Player and ZegoVideoSourceType.MainPublishChannel video source type.
    ///  2. When using ZegoVideoSourceType.Player and ZegoVideoSourceType.MainPublishChannel video source type in aux publish channel ZegoPublishChannel.Aux, must ensure that physical device works on main publish channel ZegoPublishChannel.Main.
    ///  3. Preemptive video sources are not allowed to be used on multiple channels at the same time, such as ZegoVideoSourceType.Camera and ZegoVideoSourceType.ScreenCapture.
    ///  4. The other publish channel can copy the main publish channel only when the main publish channel uses internal video capture. A maximum of one copy is supported.
    ///  5. When using ZegoVideoSourceType.Player video source type, please ensure that the ZegoMediaPlayer instance is created successfully.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param source Video capture source.
    /// @param channel Publish stream channel.
    virtual int setVideoSource(ZegoVideoSourceType source, ZegoPublishChannel channel) = 0;

    /// Set a video capture instance as video capture source for the specified channel.
    ///
    /// Available since: 3.1.0
    /// Description: Set video capture source for switching between different video capture sources.
    /// Use cases: Typically used in educational scenarios that require switching between different video capture sources.
    /// When to call: After the engine is created [createEngine].
    /// Restrictions: None.
    /// Caution: 1. Main push channel ZegoPublishChannel.Main does not support using ZegoVideoSourceType.Player and ZegoVideoSourceType.MainPublishChannel video source type.
    ///  2. When using ZegoVideoSourceType.Player and ZegoVideoSourceType.MainPublishChannel video source type in aux publish channel ZegoPublishChannel.Aux, must ensure that physical device works on main publish channel ZegoPublishChannel.Main.
    ///  3. Preemptive video sources are not allowed to be used on multiple channels at the same time, such as ZegoVideoSourceType.Camera and ZegoVideoSourceType.ScreenCapture.
    ///  4. The other publish channel can copy the main publish channel only when the main publish channel uses internal video capture. A maximum of one copy is supported.
    ///  5. When using ZegoVideoSourceType.Player video source type, please ensure that the ZegoMediaPlayer instance is created successfully.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param source Video capture source.
    /// @param instanceID Video capture source instance id.
    /// @param channel Publish stream channel.
    virtual int setVideoSource(ZegoVideoSourceType source, unsigned int instanceID,
                               ZegoPublishChannel channel) = 0;

    /// Set audio capture source.
    ///
    /// Available since: 3.1.0
    /// Description: Set audio capture source for switching between different audio capture sources.
    /// Use cases: Typically used in educational scenarios that require switching between different audio capture sources.
    /// When to call: After the engine is created [createEngine].
    /// Restrictions: None.
    /// Caution: 1. Act on the main publish channel ZegoPublishChannel.Main.
    ///  2. Main push channel ZegoPublishChannel.Main does not support using ZegoAudioSourceType.MediaPlayer and ZegoAudioSourceType.MainPublishChannel audio source type.
    ///
    /// @param source Audio capture source.
    virtual int setAudioSource(ZegoAudioSourceType source) = 0;

    /// Set audio capture source for the specified channel.
    ///
    /// Available since: 3.1.0
    /// Description: Set audio capture source for switching between different audio capture sources.
    /// Use cases: Typically used in educational scenarios that require switching between different audio capture sources.
    /// When to call: After the engine is created [createEngine].
    /// Caution: 1. Main push channel ZegoPublishChannel.Main does not support using ZegoAudioSourceType.MediaPlayer and ZegoAudioSourceType.MainPublishChannel audio source type.
    ///  2. When using ZegoAudioSourceType.MediaPlayer and ZegoAudioSourceType.MainPublishChannel audio source type in aux publish channel ZegoPublishChannel.Aux, must ensure that physical device works on main publish channel ZegoPublishChannel.Main.
    ///  3. Preemptive audio sources are not allowed to be used on multiple channels at the same time, such as ZegoAudioSourceType.Microphone.
    ///  4. When using ZegoAudioSourceType.MediaPlayer audio source type, please ensure that the ZegoMediaPlayer instance is created successfully.
    ///
    /// @param source Audio capture source.
    /// @param channel Publish stream channel.
    virtual int setAudioSource(ZegoAudioSourceType source, ZegoPublishChannel channel) = 0;

    /// Set audio capture source with audio mix config.
    ///
    /// Available since: 3.1.0
    /// Description: Set audio capture source for switching between different audio capture sources.
    /// Use cases: Typically used in educational scenarios that require switching between different audio capture sources.
    /// When to call: After the engine is created [createEngine].
    /// Restrictions: None.
    /// Caution: 1. Act on the main publish channel ZegoPublishChannel.Main.
    ///  2. Main push channel ZegoPublishChannel.Main does not support using ZegoAudioSourceType.MediaPlayer and ZegoAudioSourceType.MainPublishChannel audio source type.
    ///
    /// @param source Audio capture source.
    /// @param config Audio capture source mix config.
    virtual int setAudioSource(ZegoAudioSourceType source, ZegoAudioSourceMixConfig config) = 0;

    /// Enable video object segmentation.
    ///
    /// Available since: 3.4.0
    /// Description: Video object segmentation and transmission is a technology that separates the video object (in most cases, a person) in a rectangular video through an AI algorithm at the push end, transmits it in the RTC network, and renders it at the stream playing end.
    /// Use cases: Scenes where the object in the video needs to be separated from the background, such as mixed reality, multi-person interaction scenes, and so on.
    /// When to call:
    ///  1. After the engine is created [createEngine].
    ///  2. If you want to listen to the [onVideoObjectSegmentationStateChanged] callback, you need to call [startPublishingStream] or [startPreview].
    /// Restrictions: Enabling object segmentation is a time-consuming operation, and it is not necessary to turn it on and off frequently.
    /// Caution: This feature requires special braiding, please contact ZEGO Technical Support
    /// Related callbacks: Listen for changes of the object segmentation state through [onVideoObjectSegmentationStateChanged].
    /// Related APIs: Use [enableAlphaChannelVideoEncoder] to support the transparent background encoding of the divided object, and then publish the stream, you can render the object with a transparent background effect on the stream playing side.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param enable Whether to enable video object segmentation, off by default.
    /// @param type The type of object segmentation.
    /// @param channel Publish stream channel.
    virtual void
    enableVideoObjectSegmentation(bool enable, ZegoObjectSegmentationType type,
                                  ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Enable video object segmentation.
    ///
    /// Available since: 3.6.0
    /// Description: Video object segmentation and transmission is a technology that separates the video object (in most cases, a person) in a rectangular video through an AI algorithm at the push end, transmits it in the RTC network, and renders it at the stream playing end.
    /// Use cases: Scenes where the object in the video needs to be separated from the background, such as mixed reality, multi-person interaction scenes, and so on.
    /// When to call: After the engine is created [createEngine].
    /// Restrictions: Enabling object segmentation is a time-consuming operation, and it is not necessary to turn it on and off frequently.
    /// Caution: This feature requires special braiding, please contact ZEGO Technical Support
    /// Related callbacks: When the object segmentation is turned on or off, the notification of the object segmentation state can be received through [onVideoObjectSegmentationStateChanged].
    /// Related APIs: Use [enableAlphaChannelVideoEncoder] to support the transparent background encoding of the divided object, and then publish the stream, you can render the object with a transparent background effect on the stream playing side.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param enable Whether to enable video object segmentation, off by default.
    /// @param config The type of object segmentation.
    /// @param channel Publish stream channel.
    virtual void enableVideoObjectSegmentation(bool enable, ZegoObjectSegmentationConfig config,
                                               ZegoPublishChannel channel) = 0;

    /// Enable video encoder alpha channel support.
    ///
    /// Available since: 3.4.0
    /// Description: Enable the alpha channel support of the video encoder on the stream publishing end, and encode the split video body for streaming.
    /// Use cases: Scenes where the object in the video needs to be separated from the background, such as mixed reality, multi-person interaction scenes, and so on.
    /// When to call: After creating the engine.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param enable Enable video encoder alpha channel support, off by default.
    /// @param alphaLayout Specify the layout position of the alpha channel data.
    /// @param channel Publish stream channel.
    virtual void
    enableAlphaChannelVideoEncoder(bool enable, ZegoAlphaLayoutType alphaLayout,
                                   ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Turn on or off the adaptive mode to adjust the volume of the human voice according to the volume of the BGM.
    ///
    /// Available since: 3.18.0
    /// Description: Turn on or off the adaptive mode to adjust the volume of the human voice according to the volume of the BGM to balance the volume of the human voice and the BGM. Default is off.
    /// When to call: Called after the engine is created [createEngine].
    /// Restrictions: This interface will take effect only when the [EnableAux] interface of the media player is called to enable aux.
    ///
    /// @param enable Turn on or off the adaptive mode to adjust the volume of the human voice according to the volume of the BGM.
    virtual void enableAuxBgmBalance(bool enable) = 0;

    /// Starts playing a stream from ZEGO RTC server.
    ///
    /// Available since: 1.1.0
    /// Description: Play audio and video streams from the ZEGO RTC server.
    /// Use cases: In the real-time scenario, developers can listen to the [onRoomStreamUpdate] event callback to obtain the new stream information in the room where they are located, and call this interface to pass in streamID for play streams.
    /// When to call: After [loginRoom].
    /// Restrictions: None.
    /// Caution: 1. The developer can update the player canvas by calling this function again (the streamID must be the same). 2. After the first play stream failure due to network reasons or the play stream is interrupted, the default time for SDK reconnection is 20min. 3. In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event. please refer to https://docs.zegocloud.com/faq/reconnect. 4. Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio and video stream can be actually played.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param streamID Stream ID, a string of up to 256 characters.
    ///   Caution:
    ///   Only support numbers, English characters and '-', '_'.
    /// @param canvas The view used to display the play audio and video stream's image. When the view is set to [nullptr], no video is displayed, only audio is played.
    virtual void startPlayingStream(const std::string &streamID, ZegoCanvas *canvas) = 0;

    /// Starts playing a stream from ZEGO RTC server or from third-party CDN. Support multi-room mode.
    ///
    /// Available since: 1.1.0
    /// Description: Play audio and video streams from the ZEGO RTC server or CDN.
    /// Use cases: In real-time or live broadcast scenarios, developers can listen to the [onRoomStreamUpdate] event callback to obtain the new stream information in the room where they are located, and call this interface to pass in streamID for play streams.
    /// When to call: After [loginRoom].
    /// Restrictions: None.
    /// Caution: 1. The developer can update the player canvas by calling this function again (the streamID must be the same). 2. After the first play stream failure due to network reasons or the play stream is interrupted, the default time for SDK reconnection is 20min. 3. In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event. please refer to https://docs.zegocloud.com/faq/reconnect. 4. Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio and video stream can be actually played.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param streamID Stream ID, a string of up to 256 characters.
    ///   Caution:
    ///   Only support numbers, English characters and '-', '_'.
    /// @param canvas The view used to display the play audio and video stream's image. When the view is set to [nullptr], no video is displayed, only audio is played.
    /// @param config Advanced player configuration Room [roomID] in [ZegoPlayerConfig] is the login roomID.
    virtual void startPlayingStream(const std::string &streamID, ZegoCanvas *canvas,
                                    ZegoPlayerConfig config) = 0;

    /// Starts playing a stream from ZEGO RTC server, without Canvas, it is more suitable for pure audio stream.
    ///
    /// Available since: 1.1.0
    /// Description: Play audio streams from the ZEGO RTC server or CDN.
    /// Use cases: In the real-time scenario, developers can listen to the [onRoomStreamUpdate] event callback to obtain the new stream information in the room where they are located, and call this interface to pass in streamID for play streams.
    /// When to call: After [loginRoom].
    /// Restrictions: None.
    /// Caution: 1. After the first play stream failure due to network reasons or the play stream is interrupted, the default time for SDK reconnection is 20min. 2. In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event. please refer to https://docs.zegocloud.com/faq/reconnect. 3. Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio stream can be actually played.
    ///
    /// @param streamID Stream ID, a string of up to 256 characters.
    ///   Caution:
    ///   Only support numbers, English characters and '-', '_'.
    virtual void startPlayingStream(const std::string &streamID) = 0;

    /// Starts playing a stream from ZEGO RTC server or from third-party CDN, without Canvas, it is more suitable for pure audio stream.
    ///
    /// Available since: 1.1.0
    /// Description: Play audio streams from the ZEGO RTC server or CDN.
    /// Use cases: In real-time or live broadcast scenarios, developers can listen to the [onRoomStreamUpdate] event callback to obtain the new stream information in the room where they are located, and call this interface to pass in streamID for play streams.
    /// When to call: After [loginRoom].
    /// Restrictions: None.
    /// Caution: 1. After the first play stream failure due to network reasons or the play stream is interrupted, the default time for SDK reconnection is 20min. 2. In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event. please refer to https://docs.zegocloud.com/faq/reconnect. 3. Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio stream can be actually played.
    ///
    /// @param streamID Stream ID, a string of up to 256 characters.
    ///   Caution:
    ///   Only support numbers, English characters and '-', '_'.
    /// @param config Advanced player configuration.
    virtual void startPlayingStream(const std::string &streamID, ZegoPlayerConfig config) = 0;

    /// Starts playing a stream from ZEGO RTC server or from third-party CDN when using RangeScene.
    ///
    /// Available since: 3.4.0
    /// Description: Play audio and video streams from the ZEGO RTC server or CDN.
    /// Use cases: When using RangeScene, users can use this function to customize play the streaming.
    /// When to call: After [loginScene].
    /// Restrictions: None.
    /// Caution: 1. The developer can update the player canvas by calling this function again (the streamID must be the same). 2. After the first play stream failure due to network reasons or the play stream is interrupted, the default time for SDK reconnection is 20min. 3. In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event. please refer to https://docs.zegocloud.com/faq/reconnect. 4. Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio and video stream can be actually played.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param streamID Stream ID, a string of up to 256 characters.
    ///   Caution:
    ///   Only support numbers, English characters and '-', '_'.
    /// @param canvas The view used to display the play audio and video stream's image. When the view is set to [nullptr], no video is displayed, only audio is played.
    /// @param config Advanced scene player configuration.
    virtual void startPlayingStreamInScene(const std::string &streamID, ZegoCanvas *canvas,
                                           ZegoScenePlayerConfig config) = 0;

    /// Starts playing a stream from ZEGO RTC server or from third-party CDN when using RangeScene, without Canvas, it is more suitable for pure audio stream.
    ///
    /// Available since: 3.3.0
    /// Description: Play audio and video streams from the ZEGO RTC server or CDN.
    /// Use cases: When using RangeScene, users can use this function to customize play the streaming.
    /// When to call: After [loginScene].
    /// Restrictions: None.
    /// Caution: 1. The developer can update the player canvas by calling this function again (the streamID must be the same). 2. After the first play stream failure due to network reasons or the play stream is interrupted, the default time for SDK reconnection is 20min. 3. In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event. please refer to https://docs.zegocloud.com/faq/reconnect. 4. Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio and video stream can be actually played.
    ///
    /// @param streamID Stream ID, a string of up to 256 characters.
    ///   Caution:
    ///   Only support numbers, English characters and '-', '_'.
    /// @param config Advanced scene player configuration.
    virtual void startPlayingStreamInScene(const std::string &streamID,
                                           ZegoScenePlayerConfig config) = 0;

    /// Switch from playing a certain stream to playing another stream.
    ///
    /// Available since: 3.16.0
    /// Description: Smoothly switch from one flv stream to another flv stream.
    /// Use cases: For the same stream, there may be multiple streams with different resolutions. When the network quality deteriorates, in order to ensure the streaming quality, the streaming end can choose to switch from a high-resolution stream to a low-resolution stream..
    /// When to call: After [startPlayingStream].
    /// Restrictions: Only supports flv protocol stream.
    /// Related callbacks:
    ///  1. The result of the switching request can be obtained through the [onPlayerSwitched] event.
    ///  2. When the stream is switched successfully, the current streaming status can be known through the [onPlayerStateUpdate] event.
    ///  3. When switching streams fails, there may not necessarily be an [onPlayerStateUpdate] event notification.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param fromStreamID Stream ID, a string of up to 256 characters.
    ///   Caution:
    ///   Only support numbers, English characters and '-', '_'.
    /// @param toStreamID Stream ID, a string of up to 256 characters.
    ///   Caution:
    ///   Only support numbers, English characters and '-', '_'.
    /// @param config Advanced player configuration.
    virtual void switchPlayingStream(const std::string &fromStreamID, const std::string &toStreamID,
                                     ZegoPlayerConfig config) = 0;

    /// Stops playing a stream.
    ///
    /// Available since: 1.1.0
    /// Description: Play audio and video streams from the ZEGO RTC server.
    /// Use cases: In the real-time scenario, developers can listen to the [onRoomStreamUpdate] event callback to obtain the delete stream information in the room where they are located, and call this interface to pass in streamID for stop play streams.
    /// When to call: After [loginRoom].
    /// Restrictions: None.
    /// Caution: 1. When stopped, the attributes set for this stream previously, such as [setPlayVolume], [mutePlayStreamAudio], [mutePlayStreamVideo], etc., will be invalid and need to be reset when playing the the stream next time.
    ///  2. After stopping pulling, the iOS platform view will clear the last frame by default and keep the background color of the view. The Android platform view remains at the last frame by default. If you need to clear the last frame, please contact ZEGO technical support.
    ///
    /// @param streamID Stream ID.
    virtual void stopPlayingStream(const std::string &streamID) = 0;

    /// Set decryption key for the playing stream.
    ///
    /// Available since: 1.19.0
    /// Description: When streaming, the audio and video data will be decrypted according to the set key.
    /// Use cases: Usually used in scenarios that require high security for audio and video calls.
    /// When to call: after [createEngine], after the play stream can be changed at any time.
    /// Restrictions: This function is only valid when calling from Zego RTC or L3 server.
    /// Related APIs: [setPublishStreamEncryptionKey]Set the publish stream encryption key.
    /// Caution: This interface can only be called if encryption is set on the publish. Calling [stopPlayingStream] or [logoutRoom] will clear the decryption key.
    ///
    /// @param streamID Stream ID.
    /// @param key The decryption key, note that the key length only supports 16/24/32 bytes.
    virtual void setPlayStreamDecryptionKey(const std::string &streamID,
                                            const std::string &key) = 0;

    /// Set up cross App playing stream information.
    ///
    /// Available since: 2.19.0
    /// Description: This information is used for authentication before playing a stream or when retrying playing a stream.
    /// Use cases: Used in scenarios that playing streams across apps.
    /// When to call: after [createEngine], after the play stream can be changed at any time.
    /// Restrictions: This function is only valid when playing stream from Zego RTC server.
    /// Caution: Calling [stopPlayingStream] or [logoutRoom] will clear this information.
    ///
    /// @param streamID Stream ID.
    /// @param info Information for cross App playing stream.
    virtual void setPlayStreamCrossAppInfo(const std::string &streamID, ZegoCrossAppInfo info) = 0;

    /// Take a snapshot of the playing stream.
    ///
    /// Available since: 1.17.0
    /// Description: Take a screenshot of the specified stream ID.
    /// When to call: after called [startPlayingStream].
    /// Restrictions: None.
    /// Related callbacks: [onPlayerTakeSnapshotResult] Screenshot data callback.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param streamID Stream ID to be snapshot.
    /// @param callback Results of take play stream snapshot.
    virtual void takePlayStreamSnapshot(const std::string &streamID,
                                        ZegoPlayerTakeSnapshotCallback callback) = 0;

    /// Sets the stream playback volume.
    ///
    /// Available since: 1.16.0
    /// Description: Set the sound size of the stream, the local user can control the playback volume of the audio stream.
    /// When to call: after called [startPlayingStream].
    /// Restrictions: None.
    /// Related APIs: [setAllPlayStreamVolume] Set all stream volume.
    /// Caution: You need to reset after [stopPlayingStream] and [startPlayingStream]. This function and the [setAllPlayStreamVolume] function overwrite each other, and the last call takes effect.
    ///
    /// @param streamID Stream ID.
    /// @param volume Volume percentage. The value ranges from 0 to 200, and the default value is 100.
    virtual void setPlayVolume(const std::string &streamID, int volume) = 0;

    /// Sets the all stream playback volume.
    ///
    /// Available since: 2.3.0
    /// Description: Set the sound size of the stream, the local user can control the playback volume of the audio stream.
    /// When to call: after called [startPlayingStream].
    /// Restrictions: None.
    /// Related APIs: [setPlayVolume] Set the specified streaming volume.
    /// Caution: Set the specified streaming volume and [setAllPlayStreamVolume] interface to override each other, and the last call takes effect.
    ///
    /// @param volume Volume percentage. The value ranges from 0 to 200, and the default value is 100.
    virtual void setAllPlayStreamVolume(int volume) = 0;

    /// Set play video stream type.
    ///
    /// Available since: 2.3.0
    /// Description: When the publish stream sets the codecID to SVC through [setVideoConfig], the puller can dynamically set and select different stream types (small resolution is one-half of the standard layer).
    /// Use cases: In general, when the network is weak or the rendered UI window is small, you can choose to pull videos with small resolutions to save bandwidth.
    /// When to call: before or after called [startPlayingStream].
    /// Restrictions: None.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param streamID Stream ID.
    /// @param streamType Video stream type.
    virtual void setPlayStreamVideoType(const std::string &streamID,
                                        ZegoVideoStreamType streamType) = 0;

    /// Set the adaptive adjustment interval range of the buffer for playing stream.
    ///
    /// Available since: 2.1.0
    /// Description: Set the range of adaptive adjustment of the internal buffer of the sdk when streaming is 0-4000ms.
    /// Use cases: Generally, in the case of a poor network environment, adjusting and increasing the playback buffer of the pull stream will significantly reduce the audio and video freezes, but will increase the delay.
    /// When to call: after called [createEngine], if it has been set, you need to reset it every time you play the stream again.
    /// Restrictions: None.
    /// Caution: When the upper limit of the cache interval set by the developer exceeds 4000ms, the value will be 4000ms. When the upper limit of the cache interval set by the developer is less than the lower limit of the cache interval, the upper limit will be automatically set as the lower limit.
    ///
    /// @param streamID Stream ID.
    /// @param minBufferInterval The lower limit of the buffer adaptation interval, in milliseconds. The default value is 0ms.
    /// @param maxBufferInterval The upper limit of the buffer adaptation interval, in milliseconds. The default value is 4000ms.
    virtual void setPlayStreamBufferIntervalRange(const std::string &streamID,
                                                  unsigned int minBufferInterval,
                                                  unsigned int maxBufferInterval) = 0;

    /// Set the playing stream ID to highlight when multiple streams are mixed. Streams in the stream list will be highlighted when multiple streams sound at the same time.
    ///
    /// Available since: 3.15.0
    /// Set the playing stream ID to highlight when multiple streams are mixed. Streams in the stream list will be highlighted when multiple streams sound at the same time.
    /// When to call: after called [createEngine].
    /// Restrictions: None.
    ///
    /// @param mode audio mix mode.
    /// @param streamList stream list.
    virtual void setAudioMixMode(ZegoAudioMixMode mode,
                                 const std::vector<std::string> &streamList) = 0;

    /// Set the weight of the pull stream priority.
    ///
    /// Available since: 1.1.0
    /// Description: Set the weight of the streaming priority.
    /// Use cases: This interface can be used when developers need to prioritize the quality of a audio and video stream in business（do not use in pure audio case）. For example: in class scene, if students pull multiple streams, you can set high priority for teacher stream.
    /// When to call: after called [startPlayingStream].
    /// Restrictions: None.
    /// Caution: By default, all streams have the same weight. Only one stream can be set with high priority, whichever is set last. After the flow is stopped, the initial state is automatically restored, and all flows have the same weight.When the local network is not good, while ensuring the focus flow, other stalls may be caused more.
    ///
    /// @param streamID Stream ID.
    virtual void setPlayStreamFocusOn(const std::string &streamID) = 0;

    /// Whether the pull stream can receive the specified audio data.
    ///
    /// Available since: 1.1.0
    /// Description: In the process of real-time audio and video interaction, local users can use this function to control whether to receive audio data from designated remote users when pulling streams as needed. When the developer does not receive the audio receipt, the hardware and network overhead can be reduced.
    /// Use cases: Call this function when developers need to quickly close and restore remote audio. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
    /// When to call: This function can be called after calling [createEngine].
    /// Caution:
    ///  1. When used together with [muteAllPlayAudioStreams], they can override each other's configurations.
    ///  2. When used together with [muteAllPlayStreamAudio], this function only works when the [muteAllPlayStreamAudio] function is set to `false`.
    ///  3. After stopping streaming, any properties previously set for this stream such as [setPlayVolume], [mutePlayStreamAudio], [mutePlayStreamVideo] and other streaming-related configurations will become invalid and need to be reset before the next stream is pulled.
    /// Related APIs: You can call the [muteAllPlayStreamAudio] function to control whether to receive all audio data. When the two functions [muteAllPlayStreamAudio] and [mutePlayStreamAudio] are set to `false` at the same time, the local user can receive the audio data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamAudio(true)] function is called, it is globally effective, that is, local users will be prohibited from receiving all remote users' audio data. At this time, the [mutePlayStreamAudio] function will not take effect whether it is called before or after [muteAllPlayStreamAudio].2. When the [muteAllPlayStreamAudio(false)] function is called, the local user can receive the audio data of all remote users. At this time, the [mutePlayStreamAudio] function can be used to control whether to receive a single audio data. Calling the [mutePlayStreamAudio(true, streamID)] function allows the local user to receive audio data other than the `streamID`; calling the [mutePlayStreamAudio(false, streamID)] function allows the local user to receive all audio data.
    ///
    /// @param streamID Stream ID.
    /// @param mute Whether it can receive the audio data of the specified remote user when streaming, "true" means prohibition, "false" means receiving, the default value is "false".
    virtual void mutePlayStreamAudio(const std::string &streamID, bool mute) = 0;

    /// Whether the pull stream can receive the specified video data.
    ///
    /// Available since: 1.1.0
    /// Description: In the process of real-time video and video interaction, local users can use this function to control whether to receive video data from designated remote users when pulling streams as needed. When the developer does not receive the video data, the hardware and network overhead can be reduced.
    /// Use cases: This function can be called when developers need to quickly close and resume watching remote video. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
    /// When to call: This function can be called after calling [createEngine].
    /// Caution:
    ///  1. When used together with [muteAllPlayVideoStreams], they can override each other's configurations.
    ///  2. When used together with [muteAllPlayStreamAudio], this function only works when the [muteAllPlayStreamVideo] function is set to `false`.
    ///  3. When you mute the video stream, the view remains at the last frame by default, if you need to clear the last frame, please contact ZEGO technical support.
    ///  4. After stopping streaming, any properties previously set for this stream such as [setPlayVolume], [mutePlayStreamAudio], [mutePlayStreamVideo] and other streaming-related configurations will become invalid and need to be reset before the next stream is pulled.
    /// Related APIs: You can call the [muteAllPlayStreamVideo] function to control whether to receive all video data. When the two functions [muteAllPlayStreamVideo] and [mutePlayStreamVideo] are set to `false` at the same time, the local user can receive the video data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamVideo(true)] function is called, it will take effect globally, that is, local users will be prohibited from receiving all remote users' video data. At this time, the [mutePlayStreamVideo] function will not take effect whether it is called before or after [muteAllPlayStreamVideo]. 2. When the [muteAllPlayStreamVideo(false)] function is called, the local user can receive the video data of all remote users. At this time, the [mutePlayStreamVideo] function can be used to control whether to receive a single video data. Call the [mutePlayStreamVideo(true, streamID)] function, the local user can receive other video data other than the `streamID`; call the [mutePlayStreamVideo(false, streamID)] function, the local user can receive all the video data.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param streamID Stream ID.
    /// @param mute Whether it is possible to receive the video data of the specified remote user when streaming, "true" means prohibition, "false" means receiving, the default value is "false". The default value for automatically played streams within the SDK is false.
    virtual void mutePlayStreamVideo(const std::string &streamID, bool mute) = 0;

    /// Can the pull stream receive all audio data. (When set to true, calling [mutePlayStreamAudio] will not take effect)
    ///
    /// Available since: 2.4.0
    /// Description: In the process of real-time audio and video interaction, local users can use this function to control whether to receive audio data from all remote users when pulling streams (including the audio streams pushed by users who have newly joined the room after calling this function). By default, users can receive audio data pushed by all remote users after joining the room. When the developer does not receive the audio receipt, the hardware and network overhead can be reduced.
    /// Use cases: Call this function when developers need to quickly close and restore remote audio. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
    /// When to call: This function can be called after calling [createEngine].
    /// Caution: This function cannot be used together with [muteAllPlayAudioStreams] throughout the SDK lifecycle.
    /// Related APIs: You can call the [mutePlayStreamAudio] function to control whether to receive a single piece of audio data. When the two functions [muteAllPlayStreamAudio] and [mutePlayStreamAudio] are set to `false` at the same time, the local user can receive the audio data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamAudio(true)] function is called, it takes effect globally, that is, local users will be prohibited from receiving audio data from all remote users. At this time, the [mutePlayStreamAudio] function will not take effect no matter if the [mutePlayStreamAudio] function is called before or after [muteAllPlayStreamAudio]. 2. When the [muteAllPlayStreamAudio(false)] function is called, the local user can receive the audio data of all remote users. At this time, the [mutePlayStreamAudio] function can be used to control whether to receive a single audio data. Calling the [mutePlayStreamAudio(true, streamID)] function allows the local user to receive audio data other than the `streamID`; calling the [mutePlayStreamAudio(false, streamID)] function allows the local user to receive all audio data.
    ///
    /// @param mute Whether it is possible to receive audio data from all remote users when streaming, "true" means prohibition, "false" means receiving, and the default value is "false".
    virtual void muteAllPlayStreamAudio(bool mute) = 0;

    /// Can the pull stream receive all audio data.
    ///
    /// Available since: 3.10.0
    /// Description: In the process of real-time audio and video interaction, local users can use this function to control whether to receive audio data from all remote users when pulling streams (including the audio streams pushed by users who have newly joined the room after calling this function). By default, users can receive audio data pushed by all remote users after joining the room. When the developer does not receive the audio receipt, the hardware and network overhead can be reduced.
    /// Use cases: Call this function when developers need to quickly close and restore remote audio. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
    /// When to call: This function can be called after calling [createEngine].
    /// Caution: This function cannot be used together with [muteAllPlayStreamAudio] throughout the SDK lifecycle.
    /// Related APIs: You can call the [mutePlayStreamAudio] function to control whether to receive a single piece of audio data. When the two functions [muteAllPlayStreamAudio] and [mutePlayStreamAudio] are set to `false` at the same time, the local user can receive the audio data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamAudio(true)] function is called, it takes effect globally, that is, local users will be prohibited from receiving audio data from all remote users. At this time, the [mutePlayStreamAudio] function will not take effect no matter if the [mutePlayStreamAudio] function is called before or after [muteAllPlayStreamAudio]. 2. When the [muteAllPlayStreamAudio(false)] function is called, the local user can receive the audio data of all remote users. At this time, the [mutePlayStreamAudio] function can be used to control whether to receive a single audio data. Calling the [mutePlayStreamAudio(true, streamID)] function allows the local user to receive audio data other than the `streamID`; calling the [mutePlayStreamAudio(false, streamID)] function allows the local user to receive all audio data.
    ///
    /// @param mute Whether it is possible to receive audio data from all remote users when streaming, "true" means prohibition, "false" means receiving, and the default value is "false".
    virtual void muteAllPlayAudioStreams(bool mute) = 0;

    /// Can the pull stream receive all video data. (When set to true, calling [mutePlayStreamVideo] will not take effect)
    ///
    /// Available since: 2.4.0
    /// Description: In the process of real-time video and video interaction, local users can use this function to control whether to receive all remote users' video data when pulling the stream (including the video stream pushed by the new user who joins the room after calling this function). By default, users can receive video data pushed by all remote users after joining the room. When the developer does not receive the video data, the hardware and network overhead can be reduced.
    /// Use cases: This function can be called when developers need to quickly close and resume watching remote video. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
    /// When to call: This function can be called after calling [createEngine].
    /// Caution:
    ///  1. This function cannot be used together with [muteAllPlayVideoStreams] throughout the SDK lifecycle.
    ///  2. When you mute the video stream, the view remains at the last frame by default, if you need to clear the last frame, please contact ZEGO technical support.
    /// Related APIs: You can call the [mutePlayStreamVideo] function to control whether to receive a single piece of video data. When the two functions [muteAllPlayStreamVideo] and [mutePlayStreamVideo] are set to `false` at the same time, the local user can receive the video data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamVideo(true)] function is called, it will take effect globally, that is, the local user will be prohibited from receiving all remote users' video data. At this time, the [mutePlayStreamVideo] function will not take effect whether it is called before or after [muteAllPlayStreamVideo]. 2. When the [muteAllPlayStreamVideo(false)] function is called, the local user can receive the video data of all remote users. At this time, the [mutePlayStreamVideo] function can be used to control whether to receive a single video data. Call the [mutePlayStreamVideo(true, streamID)] function, the local user can receive other video data other than the `streamID`; call the [mutePlayStreamVideo(false, streamID)] function, the local user can receive all the video data.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param mute Whether it is possible to receive all remote users' video data when streaming, "true" means prohibition, "false" means receiving, and the default value is "false".
    virtual void muteAllPlayStreamVideo(bool mute) = 0;

    /// Can the pull stream receive all video data.
    ///
    /// Available since: 3.10.0
    /// Description: In the process of real-time video and video interaction, local users can use this function to control whether to receive all remote users' video data when pulling the stream (including the video stream pushed by the new user who joins the room after calling this function). By default, users can receive video data pushed by all remote users after joining the room. When the developer does not receive the video data, the hardware and network overhead can be reduced.
    /// Use cases: This function can be called when developers need to quickly close and resume watching remote video. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
    /// When to call: This function can be called after calling [createEngine].
    /// Caution:
    ///  1. This function cannot be used together with [muteAllPlayStreamVideo] throughout the SDK lifecycle.
    ///  2. When you mute the video stream, the view remains at the last frame by default, if you need to clear the last frame, please contact ZEGO technical support.
    /// Related APIs: You can call the [mutePlayStreamVideo] function to control whether to receive a single piece of video data.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param mute Whether it is possible to receive all remote users' video data when streaming, "true" means prohibition, "false" means receiving, and the default value is "false".
    virtual void muteAllPlayVideoStreams(bool mute) = 0;

    /// Enables or disables hardware decoding.
    ///
    /// Available since: 1.1.0
    /// Description: Control whether hardware decoding is used when playing streams, with hardware decoding enabled the SDK will use the GPU for decoding, reducing CPU usage.
    /// Use cases: If developers find that the device heats up badly when playing large resolution audio and video streams during testing on some models, consider calling this function to enable hardware decoding.
    /// Default value: Hardware decoding is disabled by default when this interface is not called.
    /// When to call: This function needs to be called after [createEngine] creates an instance.
    /// Restrictions: None.
    /// Caution: Need to be called before calling [startPlayingStream], if called after playing the stream, it will only take effect after stopping the stream and re-playing it. Once this configuration has taken effect, it will remain in force until the next call takes effect.
    ///
    /// @param enable Whether to turn on hardware decoding switch, true: enable hardware decoding, false: disable hardware decoding.
    virtual void enableHardwareDecoder(bool enable) = 0;

    /// Enables or disables frame order detection.
    ///
    /// Available since: 1.1.0
    /// Description: Control whether to turn on frame order detection.
    /// Use cases: Turning on frame order detection when pulling cdn's stream will prevent splash screens.
    /// Default value: Turn on frame order detection by default when this interface is not called.
    /// When to call: This function needs to be called after [createEngine] creates an instance.
    /// Restrictions: None.
    /// Caution: Turn off frame order detection during playing stream may result in a brief splash screen.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param enable Whether to turn on frame order detection, true: enable check poc, false: disable check poc.
    virtual void enableCheckPoc(bool enable) = 0;

    /// Whether the specified video decoding format is supported.
    ///
    /// Available since: 2.12.0
    /// Description: Whether the specified video decoding is supported depends on the following aspects: whether the hardware model supports hard decoding, whether the performance of the hardware model supports soft decoding, and whether the SDK includes the decoding module.
    /// When to call: After creating the engine.
    /// Caution: It is recommended that users call this interface to obtain the H.265 decoding support capability before pulling the H.265 stream. If it is not supported, the user can pull the stream of other encoding formats, such as H.264.
    ///
    /// @param codecID Video codec id.Required: Yes.
    /// @return Whether the specified video decoding format is supported; true means supported, you can use this decoding format for playing stream; false means not supported, the SDK does not recommend users to play streams of this encoding format. If users force the play, low frame rates may occur.
    virtual bool isVideoDecoderSupported(ZegoVideoCodecID codecID) = 0;

    /// Whether the specified video decoding format is supported.
    ///
    /// Available since: 3.0.0
    /// Description: Whether the specified video decoding is supported depends on the following aspects: whether the hardware model supports hard decoding, whether the performance of the hardware model supports soft decoding, and whether the SDK includes the decoding module.
    /// When to call: After creating the engine.
    /// Caution: It is recommended that users call this interface to obtain the H.265 decoding support capability before pulling the H.265 stream. If it is not supported, the user can pull the stream of other encoding formats, such as H.264.
    ///
    /// @param codecID Video codec id. Required: Yes.
    /// @param codecBackend Backend implementation of decoder. Required: Yes.
    /// @return Whether the specified video decoding format is supported; 0 means not supported, and the decoding format cannot be used for play stream; 1 means support, you can use this decoding format for playing stream; 2 means not confirmed, it is recommended to call this interface later.
    virtual int isVideoDecoderSupported(ZegoVideoCodecID codecID,
                                        ZegoVideoCodecBackend codecBackend) = 0;

    /// Set the play stream alignment properties.
    ///
    /// Available since: 2.14.0
    /// Description: When playing at the streaming end, control whether the playing RTC stream needs to be accurately aligned. If necessary, all the streams that contain precise alignment parameters will be aligned; if not, all streams are not aligned.
    /// Use case: It is often used in scenes that require to play multiple streams alignment such as KTV to ensure that users can switch between singing anchors, ordinary Maishangyu chat anchors, and Maixia audiences at any time during use.
    /// Default value: If this interface is not called, the default is not aligned.
    /// When to call: Called after [createEngine]. Call the interface repeatedly, and the latest setting is valid.
    /// Related APIs: Set the precise alignment parameter of the stream channel [setStreamAlignmentProperty].
    ///
    /// @param mode Setting the stream alignment mode.
    virtual void setPlayStreamsAlignmentProperty(ZegoStreamAlignmentMode mode) = 0;

#if TARGET_OS_IPHONE || defined(ANDROID)
    /// Enable video super resolution.
    ///
    /// Available since: 3.0.0
    /// Description: Whether to enable video super resolution when playing stream, the resolution of the played video can be doubled at the stream playing end through video super resolution. For example, the original resolution is 640x360, and the super-resolution is 1280x720.
    /// Use cases: Live streaming scenario.
    /// When to call: Video super resolution is only valid for playing stream video. Needs to be called after [initVideoSuperResolution].
    /// Caution:
    ///  1. This function requires a special package, please contact ZEGO technical support;
    ///  2. This function will consume extra system resources. In order to ensure user experience, ZEGO can only enable video super resolution for one stream, and the original video resolution is not recommended to exceed 640 × 360.
    /// Related callbacks: Developer can use the [onPlayerVideoSuperResolutionUpdate] callback to monitor the video super resolution status change.
    ///
    /// @param streamID The ID of the stream that currently needs to turn on or off overscore.
    /// @param enable Whether to enable super resolution, it is not enabled by default.
    virtual void enableVideoSuperResolution(const std::string &streamID, bool enable) = 0;
#endif

#if TARGET_OS_IPHONE || defined(ANDROID)
    /// Init video super resolution.
    ///
    /// Available since: 3.3.0
    /// Description: After the super resolution is initialized, the super resolution function can be used normally
    /// Use cases: Live streaming scenario.
    /// When to call: Video super resolution is only valid for playing stream video. Needs to be called after [createEngine].
    /// Caution:
    ///  1. Initializing the video screen is divided into time-consuming operations, and frequent initialization and de-initialization are not recommended;
    ///  2. This function requires a special package, please contact ZEGO technical support.
    virtual void initVideoSuperResolution() = 0;
#endif

#if TARGET_OS_IPHONE || defined(ANDROID)
    /// Uninit video super resolution.
    ///
    /// Available since: 3.3.0
    /// Description: After deinitializing the super resolution, the SDK will release the resources occupied by the super resolution and make the super resolution function unavailable.
    /// Use cases: Live streaming scenario.
    /// When to call: Needs to be called after [initVideoSuperResolution].
    /// Caution: Initializing the video screen is divided into time-consuming operations, and frequent initialization and de-initialization are not recommended.
    virtual void uninitVideoSuperResolution() = 0;
#endif

    /// Update playing canvas.
    ///
    /// Available: since 3.4.0
    /// Description: This interface will update playing view.
    /// Use case: The user can call this function to update canvas display video.
    /// When to call: After calling the [startPlayingStream] interface.
    /// Restrictions: None.
    /// Caution: None.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param streamID Stream ID, a string of up to 256 characters.
    ///   Caution:
    ///   Only support numbers, English characters and '-', '_'.
    /// @param canvas The view used to display the play audio and video stream's image. When the view is set to [nullptr], no video is displayed, only audio is played.
    /// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
    virtual int updatePlayingCanvas(const std::string &streamID, ZegoCanvas *canvas) = 0;

    /// Starts a stream mixing task.
    ///
    /// Available since: 1.2.1
    /// Description: Initiate a mixing stream request to the ZEGO RTC server, the server will look for the stream currently being pushed, and mix the layers according to the parameters of the mixing stream task requested by the SDK. When you need to update a mixing task, that is, when the input stream increases or decreases, you need to update the input stream list. At this time, you can update the field of the [ZegoMixerTask] object inputList and call this function again to pass in the same [ZegoMixerTask] object to update the mixing task.
    /// Use cases: It is often used when multiple video images are required to synthesize a video using mixed streaming, such as education, live broadcast of teacher and student images.
    /// When to call: After calling [loginRoom] to log in to the room.
    /// Restrictions: None.
    /// Caution: Due to the performance considerations of the client device, the SDK muxing is to start the mixing task on the ZEGO RTC server for mixing. If an exception occurs when the mixing task is requested to start, for example, the most common mixing input stream does not exist, the error code will be given from the callback callback. If a certain input stream does not exist in the middle, the muxing task will automatically retry to pull this input stream for 90 seconds, and will not retry after 90 seconds. If all input streams no longer exist, the server will automatically stop the mixing task after 90 seconds.
    /// Related callbacks: [OnMixerRelayCDNStateUpdate] can be used to obtain the CDN status update notification of the mixed stream repost, and the sound update notification of each single stream in the mixed stream can be obtained through [onMixerSoundLevelUpdate].
    /// Related APIs: the mixing task can be stopped by the [stopMixerTask] function.
    ///
    /// @param task Mixing task object. Required: Yes.
    /// @param callback Start notification of mixing task results.Required: No. Caution: Passing [nullptr] means not receiving callback notifications.
    virtual void startMixerTask(ZegoMixerTask task, ZegoMixerStartCallback callback) = 0;

    /// Stops a stream mixing task.
    ///
    /// Available since: 1.2.1
    /// Description: Initiate a request to end the mixing task to the ZEGO RTC server.
    /// Use cases: It is often used when multiple video images are required to synthesize a video using mixed streaming, such as education, live broadcast of teacher and student images.
    /// When to call: After calling [startMixerTask] to start mixing.
    /// Restrictions: None.
    /// Caution: If the developer starts the next mixing task without stopping the previous mixing task, the previous mixing task will not automatically stop until the input stream of the previous mixing task does not exist for 90 seconds. Before starting the next mixing task, you should stop the previous mixing task, so that when an anchor has already started the next mixing task to mix with other anchors, the audience is still pulling the output stream of the previous mixing task.
    /// Related APIs: You can start mixing by using the [startMixerTask] function.
    ///
    /// @param task Mixing task object. Required: Yes.
    /// @param callback Stop stream mixing task result callback notification.Required: No. Caution: Passing [nullptr] means not receiving callback notifications.
    virtual void stopMixerTask(ZegoMixerTask task, ZegoMixerStopCallback callback) = 0;

    /// Starts an auto stream mixing task.
    ///
    /// Available since: 2.10.0
    /// Description: Local users can use this function to start an auto stream mixing task, all streams in a room wil be mixed. Currently, only audio streams can be mixed. When auto stream mixing is enabled, the audio of all streams in the room is automatically mixed, and any further pushed streams in the room are automatically mixed into the final output stream.
    /// Use cases: Often used in voice chat room scenarios.
    /// When to call: After the engine is created, user can call this function to enable auto stream mixing in the target room if the target room has already been created
    /// Restrictions: Please refer to https://docs.zegocloud.com/article/7611 or contact ZEGO technical support.
    /// Caution: Before starting the next auto stream mixing task in the same room, call [stopAutoMixerTask] function to end the last auto stream mixing task. Otherwise, when one anchor has started the next auto stream mixing task and other anchors have started the next auto stream mixing task, the audience still pulls the output stream of the first auto stream mixing task. If the user does not end the current auto stream mixing task, the task will automatically end after the room does not exist or the input streams do not exist for 90 seconds.
    /// Related callbacks: Users can obtain the execution result of the function through [ZegoMixerStartCallback] callback.Users can get automatic mixed flow information through [onAutoMixerSoundLevelUpdate] callback.
    /// Related APIs: Users can call [stopAutoMixerTask] function to stop an auto stream mixing task.
    ///
    /// @param task Auto mix stream task object
    /// @param callback Start auto mix stream task result callback notification
    virtual void startAutoMixerTask(ZegoAutoMixerTask task, ZegoMixerStartCallback callback) = 0;

    /// Stops an auto stream mixing task.
    ///
    /// Available since: 2.10.0
    /// Description: Local users can use this function to stop an auto stream mixing task.
    /// Use cases: Often used in voice chat room scenarios.
    /// When to call: Call this function after [startAutoMixerTask] function is called to start an auto stream mixing task.
    /// Caution: Before calling [startAutoMixerTask] function to start the next auto stream mixing task in the same room, call this function to end the last auto stream mixing task. Otherwise, when one anchor has started the next auto stream mixing task and other anchors have started the next auto stream mixing task, the audience still pulls the output stream of the first auto stream mixing task. If the user does not end the current auto stream mixing task, the task will automatically end after the room does not exist or the input streams do not exist for 90 seconds.
    /// Related callbacks: Users can obtain the execution result of the function through [ZegoMixerStopCallback] callback.
    /// Related APIs: Users can call [startAutoMixerTask] function to start an auto stream mixing task.
    ///
    /// @param task Auto mix stream task object
    /// @param callback Stop auto mix stream task result callback notification
    virtual void stopAutoMixerTask(ZegoAutoMixerTask task, ZegoMixerStopCallback callback) = 0;

    /// Mutes or unmutes the microphone.
    ///
    /// Available since: 1.1.0
    /// Description: This function is used to control whether to use the collected audio data. Mute (turn off the microphone) will use the muted data to replace the audio data collected by the device for streaming. At this time, the microphone device will still be occupied.
    /// Use case: Users can call this interface by only turning off the human voice collected by the microphone and not turning off the music sound of the media player. This interface affects [onBeforeAudioPrepAudioData].
    /// Default value: The default is `false`, which means no muting.
    /// When to call: After creating the engine [createEngine].
    /// Restrictions: None.
    /// Related APIs: Developers who want to control whether to use microphone on the UI should use this function to avoid unnecessary performance overhead by using the [enableAudioCaptureDevice]. You can use [isMicrophoneMuted] to check if the microphone is muted.
    ///
    /// @param mute Whether to mute (disable) the microphone, `true`: mute (disable) microphone, `false`: enable microphone.
    virtual void muteMicrophone(bool mute) = 0;

    /// Checks whether the microphone is muted.
    ///
    /// Available since: 1.1.0
    /// Description: Used to determine whether the microphone is set to mute.
    /// When to call: After creating the engine [createEngine].
    /// Restrictions: None.
    /// Related APIs: [muteMicrophone].
    ///
    /// @return Whether the microphone is muted; true: the microphone is muted; `false`: the microphone is enable (not muted).
    virtual bool isMicrophoneMuted() = 0;

    /// Mutes or unmutes the audio output speaker.
    ///
    /// Available since: 1.1.0
    /// Description: After mute speaker, all the SDK sounds will not play, including playing stream, mediaplayer, etc.
    /// Default value: The default is `false`, which means no muting.
    /// When to call: After creating the engine [createEngine].
    /// Restrictions: None.
    ///
    /// @param mute Whether to mute (disable) speaker audio output, `true`: mute (disable) speaker audio output, `false`: enable speaker audio output.
    virtual void muteSpeaker(bool mute) = 0;

    /// Checks whether the audio output speaker is muted.
    ///
    /// Available since: 1.1.0
    /// Description: Used to determine whether the audio output is muted.
    /// When to call: After creating the engine [createEngine].
    /// Restrictions: None.
    /// Related APIs: [muteSpeaker].
    ///
    /// @return Whether the speaker is muted; `true`: the speaker is muted; `false`: the speaker is enable (not muted).
    virtual bool isSpeakerMuted() = 0;

    /// Gets a list of audio devices.
    ///
    /// Only for Windows / macOS / Linux
    ///
    /// @param deviceType Audio device type
    /// @return Audo device List
    virtual std::vector<ZegoDeviceInfo> getAudioDeviceList(ZegoAudioDeviceType deviceType) = 0;

    /// Get the device ID of the default audio device.
    ///
    /// Only for Windows / macOS / Linux
    ///
    /// @param deviceType Audio device type
    /// @return Default Audio device ID
    virtual std::string getDefaultAudioDeviceID(ZegoAudioDeviceType deviceType) = 0;

    /// Chooses to use the specified audio device.
    ///
    /// Available since: 1.1.0
    /// Description: Chooses to use the specified audio device.
    /// When to call: After creating the engine [createEngine].
    /// Restrictions: Only supports Windows / macOS / Linux
    ///
    /// @param deviceType Audio device type
    /// @param deviceID ID of a device obtained by [getAudioDeviceList]
    virtual void useAudioDevice(ZegoAudioDeviceType deviceType, const std::string &deviceID) = 0;

    /// Get volume for the specified audio device.
    ///
    /// Get volume for the specified audio device. Only for Windows / macOS / Linux
    ///
    /// @param deviceType Audio device type
    /// @param deviceID ID of a device obtained by [getAudioDeviceList]
    /// @return Device volume
    virtual int getAudioDeviceVolume(ZegoAudioDeviceType deviceType,
                                     const std::string &deviceID) = 0;

    /// Set volume for the specified audio device.
    ///
    /// The direct operating system device may fail due to system restrictions. Please use [setCaptureVolume] and [setPlayVolume] first to adjust the volume of publish and play streams.
    /// Only for Windows / macOS / Linux
    ///
    /// @param deviceType Audio device type
    /// @param deviceID ID of a device obtained by [getAudioDeviceList]
    /// @param volume Device volume
    virtual void setAudioDeviceVolume(ZegoAudioDeviceType deviceType, const std::string &deviceID,
                                      int volume) = 0;

    /// Set the volume of the speaker in the App.
    ///
    /// Available since: 3.3.0
    /// Description: Set the volume of the speaker in the App.
    /// Use cases: You need to adjust only the playback volume of the app, and do not adjust the playback volume of the system.
    /// When to call: After creating the engine [createEngine].
    /// Caution: This interface triggers startup switchover of the device. You are advised not to invoke this interface frequently to avoid unnecessary overhead and hardware problems. This interface may cause the volume mode to switch between call and media. If the media volume is inconsistent with the call volume, the volume may change.
    /// Restrictions: Only for Windows.
    ///
    /// @param deviceID ID of a device obtained by [getAudioDeviceList].
    /// @param volume Device volume.
    virtual void setSpeakerVolumeInAPP(const std::string &deviceID, int volume) = 0;

    /// Get the volume of the speaker in the App, only support Windows.
    ///
    /// Available since: 3.3.0.
    /// Description: Get the volume of the speaker in the App, only support Windows.
    /// When to call: After creating the engine [createEngine].
    /// Restrictions: Only for Windows.
    /// Related APIs: Set the volume of the speaker in the App [setSpeakerVolumeInAPP].
    ///
    /// @param deviceID ID of a device obtained by [getAudioDeviceList].
    /// @return Device volume.
    virtual int getSpeakerVolumeInAPP(const std::string &deviceID) = 0;

    /// Turn on audio device volume monitoring.
    ///
    /// Available since: 1.1.0
    /// Description: Starts the audio device volume monitor. When the volume of the device changes, the changed volume will be called back via [onAudioDeviceVolumeChanged].
    /// Caution: Currently, only one audio output device and one audio input device are supported to be monitored at the same time. When this API is called multiple times and the same device type is set, the device ID set to this API during the last call will be overwritten.
    /// When to call: After creating the engine via [createEngine].
    /// Platform differences: Only supports Windows and macOS.
    /// Related APIs: When you no longer need to monitor the device volume, please call [stopAudioDeviceVolumeMonitor] to stop monitoring.
    ///
    /// @param deviceType Audio device type
    /// @param deviceID ID of a device obtained by [getAudioDeviceList]
    virtual void startAudioDeviceVolumeMonitor(ZegoAudioDeviceType deviceType,
                                               const std::string &deviceID) = 0;

    /// Turn off audio device volume monitoring. Only for Windows/macOS.
    ///
    /// Available since: 1.1.0
    /// Description: Stops the audio device volume monitor.
    /// When to call: After creating the engine via [createEngine], and when you no longer need to monitor the device volume.
    /// Platform differences: Only supports Windows and macOS.
    ///
    /// @param deviceType Audio device type
    /// @param deviceID ID of a device obtained by [getAudioDeviceList]
    virtual void stopAudioDeviceVolumeMonitor(ZegoAudioDeviceType deviceType,
                                              const std::string &deviceID) = 0;

    /// Mutes or unmutes the audio device.
    ///
    /// Only for Windows / macOS / Linux
    ///
    /// @param deviceType Audio device type
    /// @param deviceID ID of a device obtained by [getAudioDeviceList]
    /// @param mute Whether to mute the audio device; `true` means to mute the audio device; `false` means to unmute the audio device.
    virtual void muteAudioDevice(ZegoAudioDeviceType deviceType, const std::string &deviceID,
                                 bool mute) = 0;

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    /// Set the audio device mode.
    ///
    /// Available since: 2.22.0
    /// Description: Select audio equipment mode according to the need of the scene (only supported by Android and iOS).
    /// Use cases: In the case of KTV, the General mode must be used, but in the language room, the Communication2 or Communication3 mode is required in order to avoid the sound of third-party music being collected. For details on how to set the audio device mode, see https://doc-zh.zego.im/faq/AudioDeviceMod?product=ExpressVideo&platform=macos
    /// When to call: After creating the engine [createEngine].
    /// Caution: This interface triggers startup switchover of the device. You are advised not to invoke this interface frequently to avoid unnecessary overhead and hardware problems. This interface may cause the volume mode to switch between call and media. If the media volume is inconsistent with the call volume, the volume may change.
    ///
    /// @param deviceMode Audio device mode
    virtual void setAudioDeviceMode(ZegoAudioDeviceMode deviceMode) = 0;
#endif

    /// Check if the audio device is muted.
    ///
    /// Only for Windows / macOS / Linux
    ///
    /// @param deviceType Audio device type
    /// @param deviceID ID of a device obtained by [getAudioDeviceList]
    /// @return Whether the audio device is muted; `true` means the audio device is muted; `false` means the audio device is not muted.
    virtual bool isAudioDeviceMuted(ZegoAudioDeviceType deviceType,
                                    const std::string &deviceID) = 0;

    /// Enables or disables the audio capture device.
    ///
    /// Available since: 1.1.0
    /// Description: This function is used to control whether to use the audio collection device. When the audio collection device is turned off, the SDK will no longer occupy the audio device. Of course, if the stream is being published at this time, by default, mute data will be used as audio data for streaming. not support Linux.
    /// Use cases: When the user never needs to use the audio, you can call this function to close the audio collection.
    /// Default value: The default is `true`.
    /// When to call: After creating the engine [createEngine].
    /// Restrictions: None.
    /// Related APIs: Turning off or turning on the microphone on the hardware is a time-consuming operation, and there is a certain performance overhead when the user performs frequent operations. [muteMicrophone] is generally recommended.
    ///
    /// @param enable Whether to enable the audio capture device, `true`: enable audio capture device, `false`: disable audio capture device.
    virtual void enableAudioCaptureDevice(bool enable) = 0;

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    /// get current audio route type.
    ///
    /// Available since: 1.1.0
    /// Description: Audio routing refers to the audio output device that an app uses to play audio, and common audio routes are: speakers, handsets, headphones, Bluetooth devices, and so on.
    /// When to call: After creating the engine [createEngine].
    /// Restrictions: Not supported under win or mac platforms.
    /// Related APIs: Set audio route to speaker [setAudioRouteToSpeaker].
    virtual ZegoAudioRoute getAudioRouteType() = 0;
#endif

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    /// Whether to use the built-in speaker to play audio.
    ///
    /// Available since: 1.1.0
    /// Description: Whether to use the speaker to play audio, when you choose not to use the built-in speaker to play the sound, the SDK will select the audio output device with the highest current priority to play the sound according to the system schedule, and common audio routes are: handsets, headphones, Bluetooth devices, and so on.
    /// When to call: After creating the engine [createEngine].
    /// Restrictions: Only switching between the earpiece and the speaker is supported. If it is a Bluetooth headset or a wired headset, it does not support routing to the speaker through this interface.
    /// Related APIs: Get the current audio route [getAudioRouteType].
    ///
    /// @param defaultToSpeaker Whether to use the built-in speaker to play sound, `true`: use the built-in speaker to play sound, `false`: use the highest priority audio output device scheduled by the current system to play sound
    virtual void setAudioRouteToSpeaker(bool defaultToSpeaker) = 0;
#endif

    /// Turns on/off the camera (for the specified channel).
    ///
    /// Available since: 1.1.0
    /// Description: This function is used to control whether to start the capture of the camera. After the camera is turned off, the video capture will not be performed. At this time, there will be no video data for local preview and push streaming.
    /// Default value: The default is `true` which means the camera is turned on.
    /// When to call: After creating the engine [createEngine].
    /// Restrictions: None.
    /// Caution: In the case of using the custom video capture function [enableCustomVideoCapture], since the developer has taken over the video data capture, the SDK is no longer responsible for the video data capture, but this function still affects whether to encode or not. Therefore, when developers use custom video capture, please ensure that the value of this function is `true`.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param enable Whether to turn on the camera, `true`: turn on camera, `false`: turn off camera
    /// @param channel Publishing stream channel
    virtual void enableCamera(bool enable,
                              ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    /// Switches to the front or the rear camera (for the specified channel).
    ///
    /// Available since: 1.1.0
    /// Description: This function controls whether [ZegoVideoSourceTypeCamera] uses the front camera or the rear camera (only supported by Android and iOS).
    /// Default value: The default is `true` which means the front camera is used.
    /// When to call: After creating the engine [createEngine].
    /// Restrictions: None.
    /// Caution: When the custom video capture function [enableCustomVideoCapture] is turned on, since the developer has taken over the video data capture, the SDK is no longer responsible for the video data capture, and this function is no longer valid.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param enable [ZegoVideoSourceTypeCamera] if or not use front camera, `true`: use the front camera, `false`: use the the rear camera.
    /// @param channel Publishing stream channel.
    virtual void useFrontCamera(bool enable,
                                ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;
#endif

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    /// Whether the camera supports focusing.
    ///
    /// Available since: 2.14.0
    /// Description: Whether the camera supports focusing.
    /// Trigger: Called after turn on preview [startPreview].
    /// Caution: Need to start the camera successfully.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param channel Publishing stream channel
    /// @return Whether to support focus, support is true, not support is false.
    virtual bool isCameraFocusSupported(ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;
#endif

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    /// Set the camera focus mode.
    ///
    /// Available since: 2.14.0
    /// Description: Set the camera focus mode.
    /// Trigger: Called after turn on preview [startPreview].
    /// Restrictions: Currently only supports iOS and Android platforms.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param mode focus mode.
    /// @param channel Publishing stream channel
    virtual void setCameraFocusMode(ZegoCameraFocusMode mode,
                                    ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;
#endif

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    /// Set the focus point in the preview view.
    ///
    /// Available since: 2.14.0
    /// Description: Set the focus point in the preview view. (x, y) are the normalized coordinates in the preview view, that is, the ratio of the position of the focus point relative to the preview view and the width and height of the preview view. The upper left corner is (0, 0).
    /// Trigger: Called after turn on preview [startPreview].
    /// Restrictions: Currently only supports iOS and Android platforms.
    /// Caution: Every time the camera restarts the acquisition, the settings will become invalid and need to be reset.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param x Normalized X axis coordinate value, effective value [0,1].
    /// @param y Normalized Y axis coordinate value, effective value [0,1].
    /// @param channel Publishing stream channel
    virtual void
    setCameraFocusPointInPreview(float x, float y,
                                 ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;
#endif

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    /// Set the camera exposure mode.
    ///
    /// Available since: 2.14.0
    /// Description: Set the camera exposure mode.
    /// Trigger: Called after turn on preview [startPreview].
    /// Restrictions: Currently only supports iOS and Android platforms.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param mode Exposure mode.
    /// @param channel Publishing stream channel
    virtual void setCameraExposureMode(ZegoCameraExposureMode mode,
                                       ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;
#endif

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    /// Set the exposure point in the preview view.
    ///
    /// Available since: 2.14.0
    /// Description: Set the exposure point in the preview view. (x, y) are the normalized coordinates in the preview view, that is, the ratio of the position of the exposure point relative to the preview view and the width and height of the preview view. The upper left corner is (0, 0).
    /// Trigger: Called after turn on preview [startPreview].
    /// Restrictions: Currently only supports iOS and Android platforms.
    /// Caution: Every time the camera restarts the acquisition, the settings will become invalid and need to be reset.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param x Normalized X axis coordinate value, effective value [0,1].
    /// @param y Normalized Y axis coordinate value, effective value [0,1].
    /// @param channel Publishing stream channel
    virtual void
    setCameraExposurePointInPreview(float x, float y,
                                    ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;
#endif

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    /// Set the camera exposure compensation value and support spedifying the publish channel.
    ///
    /// Available since: 2.10.0
    /// Description: Set the camera exposure compensation value.
    /// Use cases: User can call this function to set the camera exposure compensation value.
    /// When to call /Trigger: Called this function after calling [startPublishingStream] or [startPreview].
    /// Restrictions: None.
    /// Caution: The setting will be invalid when the camera is restarted.
    /// Platform differences: Only supports iOS and Android.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param value Camera exposure, the value range is [-1,1], the default 0, -1 tends to darken, 1 tends to brighten.
    /// @param channel Publishing stream channel
    virtual void
    setCameraExposureCompensation(float value,
                                  ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;
#endif

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    /// Set the zoom factor of the camera and support specifying the publish channel. Every time the camera is restarted, the camera zoom factor will return to the initial value (1.0).
    ///
    /// Available since: 1.20.0
    /// Description: Set the camera zoom factor. Every time the camera is restarted, the camera zoom factor will be restored to its initial value.
    /// When to call: After creating the engine [createEngine].
    /// Restrictions: The settings will not take effect until the camera is started.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param factor The zoom factor of the camera, the minimum value is 1.0, and the maximum value is the return value of [getCameraMaxZoomFactor].
    /// @param channel Publishing stream channel
    virtual void setCameraZoomFactor(float factor,
                                     ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;
#endif

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    /// Get the maximum zoom factor of the camera and support specifying the publish channel.
    ///
    /// Available since: 1.20.0
    /// Description: Set the camera zoom factor. Every time the camera is restarted, the camera zoom factor will be restored to its initial value.
    /// When to call: This is only available after the camera has been successfully started, and can generally be called when the captured first frame is received [onPublisherCapturedVideoFirstFrame] callback.
    /// Restrictions: None.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param channel Publishing stream channel
    /// @return The maximum zoom factor of the camera.
    virtual float
    getCameraMaxZoomFactor(ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;
#endif

    /// Enable camera adaptive frame rate.
    ///
    /// Available since: 2.20.0
    /// Description: After enabling, the SDK matches the capture frame rate range supported by the camera according to the set frame rate range, and dynamically adjusts the capture frame rate of the camera according to the ambient brightness within this range to improve the screen brightness when the set frame rate is too high.
    /// Use cases: The frame rate set by the user on the streaming end is too high, and the ambient lighting is low, so the subject cannot be displayed or recognized normally. For example, live broadcast scenes with high exposure requirements.
    /// When to call: After creating the engine [createEngine], before the camera starts.
    /// Caution: Takes When calling [setVideoConfig] to set the frame rate lower than the expected minimum frame rate, the frame rate value set by [setVideoConfig] will be used. Due to the different hardware and algorithm strategies of different mobile phone manufacturers, the effect of this interface is different on different models or on the front and rear cameras of the same model.
    /// Related APIs: Through [setVideoConfig], you can set the camera capture frame rate and the encoder encoding frame rate.
    ///
    /// @param enable Whether to enable camera adaptive frame rate. true means on, false means off.Off by default.
    /// @param minFPS Desired minimum frame rate, 15 recommended. Unit: fps.
    /// @param maxFPS Desired minimum frame rate, 25 recommended. Unit: fps.
    /// @param channel Publishing stream channel.
    virtual void
    enableCameraAdaptiveFPS(bool enable, int minFPS, int maxFPS,
                            ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Chooses to use the specified video device (for the specified channel).
    ///
    /// Only for Windows / macOS / Linux
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param deviceID ID of a device obtained by [getVideoDeviceList]
    /// @param channel Publishing stream channel
    virtual void useVideoDevice(const std::string &deviceID,
                                ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Gets a list of video devices.
    ///
    /// Only for Windows / macOS / Linux
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @return Video device List
    virtual std::vector<ZegoDeviceInfo> getVideoDeviceList() = 0;

    /// Get the device ID of the default video device.
    ///
    /// Only for Windows / macOS / Linux
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @return Default video device ID
    virtual std::string getDefaultVideoDeviceID() = 0;

    /// Starts sound level monitoring. Support setting the listening interval.
    ///
    /// Available since: 1.15.0
    /// Description: After starting monitoring, you can receive local audio sound level via [onCapturedSoundLevelUpdate] callback, and receive remote audio sound level via [onRemoteSoundLevelUpdate] callback. Before entering the room, you can call [startPreview] with this function and combine it with [onCapturedSoundLevelUpdate] callback to determine whether the audio device is working properly.
    /// Use cases: During the publishing and playing process, determine who is talking on the wheat and do a UI presentation.
    /// When to call: After the engine is created [createEngine].
    /// Caution:
    ///   1. [onCapturedSoundLevelUpdate] and [onRemoteSoundLevelUpdate] callback notification period is the value set by the parameter. If you want to use advanced feature of sound level, please use the function of the same name (the parameter type is ZegoSoundLevelConfig) instead.
    ///   2. After the sound monitoring is started, even if the local audio capture is not started, [onCapturedSoundLevelUpdate] will have a callback, and the sound level is 0.
    ///
    /// @param millisecond Monitoring time period of the sound level, in milliseconds, has a value range of [100, 3000]. Default is 100 ms.
    virtual void startSoundLevelMonitor(unsigned int millisecond = 100) = 0;

    /// Starts sound level monitoring. Support enable some advanced feature.
    ///
    /// Available since: 2.10.0
    /// Description: After starting monitoring, you can receive local audio sound level via [onCapturedSoundLevelUpdate] callback, and receive remote audio sound level via [onRemoteSoundLevelUpdate] callback. Before entering the room, you can call [startPreview] with this function and combine it with [onCapturedSoundLevelUpdate] callback to determine whether the audio device is working properly.
    /// Use cases: During the publishing and playing process, determine who is talking on the wheat and do a UI presentation.
    /// When to call: After the engine is created [createEngine].
    /// Caution:
    ///   1. [onCapturedSoundLevelUpdate] and [onRemoteSoundLevelUpdate] callback notification period is the value set by the parameter.
    ///   2. After the sound monitoring is started, even if the local audio capture is not started, [onCapturedSoundLevelUpdate] will have a callback, and the sound level is 0.
    ///
    /// @param config Configuration for starts the sound level monitor.
    virtual void startSoundLevelMonitor(ZegoSoundLevelConfig config) = 0;

    /// Stops sound level monitoring.
    ///
    /// Available since: 1.1.0
    /// Description: After the monitoring is stopped, the callback of the local/remote audio sound level will be stopped.
    /// When to call: After the engine is created [createEngine].
    /// Related APIs: Soundwave monitoring can be initiated via [startSoundLevelMonitor].
    virtual void stopSoundLevelMonitor() = 0;

    /// Starts audio spectrum monitoring. Support setting the listening interval.
    ///
    /// Available since: 1.15.0
    /// Description: After starting monitoring, you can receive local audio spectrum via [onCapturedAudioSpectrumUpdate] callback, and receive remote audio spectrum via [onRemoteAudioSpectrumUpdate] callback.
    /// Use cases: In the host K song scene, has been published or played under the premise that the host or audience to see the tone and volume change animation.
    /// When to call: After the engine is created [createEngine].
    /// Caution: [onCapturedAudioSpectrumUpdate] and [onRemoteAudioSpectrumUpdate] callback notification period is the value set by the parameter.
    ///
    /// @param millisecond Monitoring time period of the audio spectrum, in milliseconds, the minimum value is 10. Default is 500 ms.
    virtual void startAudioSpectrumMonitor(unsigned int millisecond = 100) = 0;

    /// Stops audio spectrum monitoring.
    ///
    /// Available since: 1.1.0
    /// Description: After the monitoring is stopped, the callback of the local/remote audio spectrum will be stopped.
    /// When to call: After the engine is created [createEngine].
    /// Related APIs: Audio spectrum monitoring can be initiated via [startAudioSpectrumMonitor].
    virtual void stopAudioSpectrumMonitor() = 0;

    /// Enables or disables headphone monitoring.
    ///
    /// Available since: 1.9.0
    /// Description: Enable/Disable headphone monitor, and users hear their own voices as they use the microphone to capture sounds.
    /// When to call: After the engine is created [createEngine].
    /// Default value: Disable.
    /// Caution:
    ///   1. This setting does not actually take effect until both the headset and microphone are connected.
    ///   2. The default is to return after acquisition and before pre-processing. If you need to return after pre-processing, please contact ZEGO technical support.
    ///
    /// @param enable Whether to use headphone monitor, true: enable, false: disable
    virtual void enableHeadphoneMonitor(bool enable) = 0;

    /// Sets the headphone monitor volume.
    ///
    /// Available since: 1.9.0
    /// Description: set headphone monitor volume.
    /// When to call: After the engine is created [createEngine].
    /// Caution: This setting does not actually take effect until both the headset and microphone are connected.
    /// Related APIs: Enables or disables headphone monitoring via [enableHeadphoneMonitor].
    ///
    /// @param volume headphone monitor volume, range from 0 to 200, 60 as default.
    virtual void setHeadphoneMonitorVolume(int volume) = 0;

    /// Enable or disable system audio capture.
    ///
    /// Available since: 1.9.0
    /// Description: Enable sound card capture to mix sounds played by the system into the publishing stream, such as sounds played by the browser, sounds played by the third-party player, etc.
    /// Default value: Default is disable.
    /// When to call: Called this function after calling [startPublishingStream] or [startPreview].
    /// Restrictions: None.
    /// Caution: The system sound card sound does not include streaming sound, media player sound and sound effect player sound.
    /// Related APIs: [setMixSystemPlayoutVolume] function can set system audio capture volume.
    /// Platform differences: Only supports Windows and macOS.
    ///
    /// @param enable Whether to mix system playout.
    virtual void enableMixSystemPlayout(bool enable) = 0;

    /// set system audio capture volume.
    ///
    /// Available since: 2.4.0
    /// Description:  set system audio capture volume.
    /// Use cases: User needs to adjust the volume which system playout mix to stream publishing.
    /// When to call /Trigger: Called this function after calling [startPublishingStream] or [startPreview].
    /// Restrictions: None.
    /// Related APIs: [enableMixSystemPlayout] enable or disable mix system playout.
    /// Platform differences: Only supports Windows and macOS.
    ///
    /// @param volume the volume. Valid range [0, 200], default is 100.
    virtual void setMixSystemPlayoutVolume(int volume) = 0;

    /// Enable or disable mix SDK playout to stream publishing.
    ///
    /// Available since: 1.1.0
    /// Description: Enable mix SDK playout sounds into the stream publishing.
    /// Use cases: Users need to mix the sound of SDK playout into stream publishing. For example, when the class scene, the teacher and student Co-hosting, and the teacher can mix the play streaming sound into the publish streaming.
    /// Default value: Default is disable.
    /// When to call /Trigger: Called this function after calling [startPublishingStream] or [startPreview].
    /// Restrictions: None.
    ///
    /// @param enable Whether to mix engine playout
    virtual void enableMixEnginePlayout(bool enable) = 0;

    /// Start audio VAD stable state monitoring.
    ///
    /// Available: since 2.14.0
    /// Description: After monitoring is started, you can use the [onAudioVADStateUpdate] callback to receive the specified type of audio VAD callback.
    /// Use cases: For example, when you specify the type of collection and use the microphone to collect, you can check whether the host has continuous and stable voice input through this interface.
    /// When to call: Before publish stream, you can call [startPreview] with this function and combine it with [onAudioVADStateUpdate] callback to determine whether the audio device is working properly.
    /// Restrictions: [onAudioVADStateUpdate] callback notification period is 3 seconds.
    /// Related APIs: [stopAudioVADStableStateMonitor].
    ///
    /// @param type audio VAD monitor type.
    virtual void startAudioVADStableStateMonitor(ZegoAudioVADStableStateMonitorType type) = 0;

    /// Start audio VAD stable state monitoring, and the monitoring period can be set.
    ///
    /// Available: since 2.17.0
    /// Description: After monitoring is started, you can use the [onAudioVADStateUpdate] callback to receive the specified type of audio VAD callback.
    /// Use cases: For example, when you specify the type of collection and use the microphone to collect, you can check whether the host has continuous and stable voice input through this interface.
    /// When to call: Before publish stream, you can call [startPreview] with this function and combine it with [onAudioVADStateUpdate] callback to determine whether the audio device is working properly.
    /// Restrictions: None.
    /// Related APIs: [stopAudioVADStableStateMonitor].
    ///
    /// @param type audio VAD monitor type.
    /// @param millisecond monitoring period default 3000. value of [200, 10000]
    virtual void startAudioVADStableStateMonitor(ZegoAudioVADStableStateMonitorType type,
                                                 int millisecond) = 0;

    /// Stop audio VAD stable state monitoring.
    ///
    /// Available since: 2.14.0
    /// Description: After calling this interface, the specified type of [onAudioVADStateUpdate] callback can no longer be received.
    /// When to call: None.
    /// Restrictions: None.
    /// Related APIs: [startAudioVADStableStateMonitor].
    ///
    /// @param type audio VAD monitor type.
    virtual void stopAudioVADStableStateMonitor(ZegoAudioVADStableStateMonitorType type) = 0;

    /// Get the audio device information currently in use.
    ///
    /// Available since: 2.12.0
    /// Description: Get the audio device information currently in use.
    /// Use cases: Used for scenes that need to manually switch between multiple audio devices.
    /// When to call: Called this function after calling [startPublishingStream] or [startPreview].
    /// Restrictions: Only supports Windows and macOS.
    /// Related APIs: The default audio device ID can be obtained through [getDefaultAudioDeviceID].
    ///
    /// @param deviceType Audio device type.Required:Yes.
    /// @return Audio device information.
    virtual ZegoDeviceInfo getCurrentAudioDevice(ZegoAudioDeviceType deviceType) = 0;

    /// Whether to enable acoustic echo cancellation (AEC).
    ///
    /// Available since: 1.1.0
    /// Description: Turning on echo cancellation, the SDK filters the collected audio data to reduce the echo component in the audio.
    /// Use case: When you need to reduce the echo to improve the call quality and user experience, you can turn on this feature.
    /// When to call: It needs to be called after [createEngine].
    /// Caution: The AEC function only supports the processing of sounds playbacked through the SDK, such as sounds played by the playing stream, media player, audio effect player, etc. Before this function is called, the SDK automatically determines whether to use AEC. Once this function is called, the SDK does not automatically determine whether to use AEC.
    /// Restrictions: None.
    /// Related APIs: Developers can use [enableHeadphoneAEC] to set whether to enable AEC when using headphones, and use [setAECMode] to set the echo cancellation mode.
    ///
    /// @param enable Whether to enable echo cancellation, true: enable, false: disable
    virtual void enableAEC(bool enable) = 0;

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    /// Whether to turn on acoustic echo cancellation (AEC) when using the headphone.
    ///
    /// Available since: 1.1.0
    /// Description: When [enableAEC] is used to turn on echo cancellation, it is only turned on when the speaker is used for mobile terminal equipment. Call this function if you need to turn echo cancellation on or off when using the headset.
    /// Use case: It is common when the mobile device is connected to a external sound card as the audio output source. In order to eliminate the echo in this case, you need to call this function to turn on the echo cancellation.
    /// Default value: Android is off by default and iOS is on by default.
    /// When to call: It needs to be called after [createEngine], before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager].
    /// Caution: Turning on echo cancellation will increase the ear return delay. On the iOS platform, the SDK cannot distinguish between the headset and the external sound card. If you use this function to turn off the system echo cancellation when using the headset, the sound played by the external sound card will be collected when the user accesses the external sound card.
    /// Restrictions: None.
    /// Related APIs: When the headset is not used, you can set whether the SDK turns on echo cancellation through [enableAEC].
    /// Platform differences: Only supports iOS and Android.
    ///
    /// @param enable Whether to enable, true: enable, false: disable
    virtual void enableHeadphoneAEC(bool enable) = 0;
#endif

    /// Sets the acoustic echo cancellation (AEC) mode.
    ///
    /// Available since: 1.1.0
    /// Description: When [enableAEC] is used to enable echo cancellation, this function can be used to switch between different echo cancellation modes to control the degree of echo cancellation.
    /// Use case: When the default echo cancellation effect does not meet expectations, this function can be used to adjust the echo cancellation mode.
    /// Default value: When this function is not called, the default echo cancellation mode is [Aggressive].
    /// When to call: It needs to be called after [createEngine].
    /// Restrictions: The value set by this function is valid only after the echo cancellation function is turned on.
    ///
    /// @param mode Echo cancellation mode
    virtual void setAECMode(ZegoAECMode mode) = 0;

    /// Enables or disables automatic gain control (AGC).
    ///
    /// Available since: 1.1.0
    /// Description: After turning on this function, the SDK can automatically adjust the microphone volume to adapt to near and far sound pickups and keep the volume stable.
    /// Use case: When you need to ensure volume stability to improve call quality and user experience, you can turn on this feature.
    /// When to call: It needs to be called after [createEngine].
    /// Caution: Before this function is called, the SDK automatically determines whether to use AGC. Once this function is called, the SDK does not automatically determine whether to use AGC.
    /// Restrictions: None.
    ///
    /// @param enable Whether to enable automatic gain control, true: enable, false: disable
    virtual void enableAGC(bool enable) = 0;

    /// Enables or disables active noise suppression (ANS, aka ANC).
    ///
    /// Available since: 1.1.0
    /// Description: Enable the noise suppression can reduce the noise in the audio data and make the human voice clearer.
    /// Use case: When you need to suppress noise to improve call quality and user experience, you can turn on this feature.
    /// When to call: It needs to be called after [createEngine].
    /// Related APIs: This function has a better suppression effect on continuous noise (such as the sound of rain, white noise). If you need to turn on transient noise suppression, please use [enableTransientANS]. And the noise suppression mode can be set by [setANSMode].
    /// Caution: Before this function is called, the SDK automatically determines whether to use ANS. Once this function is called, the SDK does not automatically determine whether to use ANS.
    /// Restrictions: None.
    ///
    /// @param enable Whether to enable noise suppression, true: enable, false: disable
    virtual void enableANS(bool enable) = 0;

    /// Enables or disables transient noise suppression.
    ///
    /// Available since: 1.17.0
    /// Description: Enable the transient noise suppression can suppress the noises such as keyboard and desk knocks.
    /// Use case: When you need to suppress transient noise to improve call quality and user experience, you can turn on this feature.
    /// Default value: When this function is not called, this is disabled by default.
    /// When to call: It needs to be called after [createEngine].
    /// Related APIs: This function will not suppress normal noise after it is turned on. If you need to turn on normal noise suppression, please use [enableANS].
    /// Restrictions: None.
    ///
    /// @param enable Whether to enable transient noise suppression, true: enable, false: disable
    virtual void enableTransientANS(bool enable) = 0;

    /// Sets the automatic noise suppression (ANS) mode.
    ///
    /// Available since: 1.1.0
    /// Description: When [enableANS] is used to enable noise suppression, this function can be used to switch between different noise suppression modes to control the degree of noise suppression.
    /// Use case: When the default noise suppression effect does not meet expectations, this function can be used to adjust the noise suppression mode.
    /// Default value: When this function is not called, the default automatic noise suppression (ANS) mode is [Medium].
    /// When to call: It needs to be called after [createEngine].
    /// Restrictions: The value set by this function is valid only after the noise suppression function is turned on.
    ///
    /// @param mode Audio Noise Suppression mode
    virtual void setANSMode(ZegoANSMode mode) = 0;

    /// Enables or disables speech enhancement.
    ///
    /// Available since: 3.3.0.
    /// Description: Enable the speech enhancement function.
    /// Use case: In the KTV external playback scene, due to 3A damage and distance, the voice is weak or empty or unclear, so it is necessary to enhance the external playback experience through the voice enhancement technology.
    /// Default value: When this function is not called, this is disabled by default.
    /// When to call: It needs to be called after [createEngine] and it Supports dynamic change.
    /// Caution: 1. If this interface is called repeatedly, the settings of the last call are used;
    ///  2. After call [DestroyEngine], the Settings fail.
    /// Restrictions: None.
    ///
    /// @param enable Whether to enable speech enhancement, true: enable, false: disable
    /// @param level Enhancement level; The value range is [0, 10]
    virtual void enableSpeechEnhance(bool enable, int level) = 0;

    /// Enables or disables audio mixing function.
    ///
    /// Available since: 1.9.0, will be deprecated soon. It is recommended to use the media player [createMediaPlayer] or the audio effect player [createAudioEffectPlayer] to implement the audio mixing function.
    /// Description: After the audio mixing function is turned on, the SDK will mix the audio data prepared by the developer with the audio data collected by the SDK before publishing.
    /// Use case: Developers can use this function when they need to mix their own songs, sound effects or other audio data into the publishing stream.
    /// Default value: When this function is not called, this is disabled by default.
    /// When to call: It needs to be called after [createEngine].
    /// Related APIs: After enabling audio mixing, the developer also needs to call [setAudioMixingHandler] to set the audio mixing callback, so as to provide the SDK with the audio data that needs to be mixed in the [onAudioMixingCopyData] callback.
    /// Caution: After you start audio mixing, the SDK will play the mixed audio on the local (publisher side) by default. If you do not want to play it locally but only on the remote (player side), please call [muteLocalAudioMixing] to set the local audio mixing mute. Recommend to use audio effect player or media player for audio mixing.
    /// Restrictions: None.
    ///
    /// @param enable Whether to enable audio mixting, true: enable, false: disable
    virtual void enableAudioMixing(bool enable) = 0;

    /// Sets up the audio mixing event handler.
    ///
    /// Available since: 1.9.0
    /// Description: After the audio mixing function is turned on, the SDK will mix the audio data prepared by the developer with the audio data collected by the SDK before publishing.
    /// Use case: Developers can use this function when they need to mix their own songs, sound effects or other audio data into the publishing stream.
    /// When to call: It needs to be called after [createEngine].
    /// Caution: When this function is called again to set the callback, the previously set mixing callback will be overwritten.
    /// Restrictions: None.
    ///
    /// @param handler Audio mixing callback handler
    virtual void setAudioMixingHandler(std::shared_ptr<IZegoAudioMixingHandler> handler) = 0;

    /// Mute or resume local playback of the mixing audio.
    ///
    /// Available since: 1.9.0
    /// Description: When calling this function to mute the local playback audio mixing, the local (publisher side) will not play the audio provided to the SDK through [onAudioMixingCopyData], but the remote (player side) can still playback it Mix.
    /// Use case: When developers need to mix their own songs, sound effects or other audio data into the publishing stream, but the mixed audio only wants to be heard by the remote player, and does not want to be playback locally, you can use this function.
    /// Default value: When this function is not called, the default is not mute, which is false.
    /// When to call: It needs to be called after [createEngine].
    /// Restrictions: None.
    ///
    /// @param mute Whether to mute local audio mixting, true: mute, false: unmute
    virtual void muteLocalAudioMixing(bool mute) = 0;

    /// Sets the audio mixing output volume for either local playback or the stream to published.
    ///
    /// Available since: 1.9.0
    /// Description: After enabling the audio mixing function through [enableAudioMixing], the developer can use this function to set the volume of the mixing audio to be mixed to SDK.
    /// Use case: When developers need to mix their own songs, sound effects or other audio data into the publishing stream, and want to adjust the volume of the mixing audio, you can use this function.
    /// When to call: It needs to be called after [createEngine].
    /// Caution: This function can individually set the mixing audio volume of the local playback or the remote playback.
    /// Restrictions: None.
    ///
    /// @param volume The audio mixing volume, range from 0 to 200, 100 as default.
    /// @param type Local playback volume / Remote playback volume
    virtual void setAudioMixingVolume(int volume, ZegoVolumeType type) = 0;

    /// Enable the Effects beauty environment.
    ///
    /// Available since: 2.16.0
    /// Description: Enable the Effects beauty environment. The SDK uses the specified video frame data type for transmission. The Windows platform only supports video frame raw data, the Apple platform only supports CVPixelBuffer, and the Android platform only supports texture2d.
    /// Use cases: It is often used in scenes such as video calls and live broadcasts.
    /// Default value: When this function is not called, the beauty environment is not activated by default.
    /// When to call: Must be called before calling [startPreview], [startPublishingStream]. If you need to modify the configuration, please call [logoutRoom] to log out of the room first, otherwise it will not take effect.
    /// Related APIs: [enableEffectsBeauty] switch beauty, [setEffectsBeautyParam] set beauty parameters.
    /// Caution: This beauty function is the basic function. If it does not meet the expectations of the developer, you can use the custom video pre-processing function [enableCustomVideoProcessing] or the custom video capture function [enableCustomVideoCapture] docking and constructing the AI ​​Effects SDK [ZegoEffects] https://doc-zh.zego.im/article/9556 for best results.
    /// Restrictions: This function only supports Android system 5.0 and above, Android SDK version 21 and above.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    virtual void startEffectsEnv() = 0;

    /// Disable the Effects beauty environment.
    ///
    /// Available since: 2.16.0
    /// Description: Disable the Effects beauty environment.
    /// Use cases: It is often used in scenes such as video calls and live broadcasts.
    /// When to call: Must be called before calling [startPreview], [startPublishingStream]. If you need to modify the configuration, please call [logoutRoom] to log out of the room first, otherwise it will not take effect.
    /// Related APIs: [enableEffectsBeauty] switch beauty, [setEffectsBeautyParam] set beauty parameters.
    /// Caution: This beauty function is the basic function. If it does not meet the expectations of the developer, you can use the custom video pre-processing function [enableCustomVideoProcessing] or the custom video capture function [enableCustomVideoCapture] docking and constructing the AI ​​Effects SDK [ZegoEffects] for best results.
    /// Restrictions: This function only supports Android system 5.0 and above, Android SDK version 21 and above.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    virtual void stopEffectsEnv() = 0;

    /// Enables or disables the beauty effect.
    ///
    /// Available since: 2.16.0
    /// Description: Support basic beauty functions, including whiten, rosy, smooth, and sharpen.
    /// Use cases: It is often used in scenes such as video calls and live broadcasts.
    /// When to call: You must call [startEffectsEnv] to enable the beauty environment before calling this function.
    /// Default value: When this function is not called, the beauty effect is not enabled by default.
    /// Related APIs: You can call the [setBeautifyOption] function to adjust the beauty parameters.
    /// Caution: This beauty function is the basic function. If it does not meet the expectations of the developer, you can use the custom video pre-processing function [enableCustomVideoProcessing] or the custom video capture function [enableCustomVideoCapture] docking and constructing the AI ​​Effects SDK [ZegoEffects] for best results.
    /// Restrictions: If this function is used on the Android platform, it only supports 5.0 and above, SDK version 21 and above.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param enable Whether to enable the beauty effect, true is enabled; false is disabled, and the default is false.
    virtual void enableEffectsBeauty(bool enable) = 0;

    /// Set beautify param.
    ///
    /// Available since: 2.16.0
    /// Description: Set the beauty parameters, including whiten, rosy, smooth, and sharpen.
    /// Use cases: It is often used in scenes such as video calls and live broadcasts.
    /// When to call: You must call [startEffectsEnv] to enable the beauty environment before calling this function.
    /// Related APIs: You can call [enableEffectsBeauty] to turn on or off the beauty function.
    /// Restrictions: This function only supports Android system 5.0 and above, Android SDK version 21 and above.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param param Beauty option param.
    virtual void setEffectsBeautyParam(ZegoEffectsBeautyParam param) = 0;

    /// Set the sound equalizer (EQ).
    ///
    /// Available since: 1.12.0
    /// Description: Call this function to set the sound equalizer adjust the tone.
    /// Use cases: Often used in voice chatroom, KTV.
    /// When to call: It needs to be called after [createEngine].
    /// Restrictions: None.
    ///
    /// @param bandIndex Band frequency index, the value range is [0, 9], corresponding to 10 frequency bands, and the center frequencies are [31, 62, 125, 250, 500, 1K, 2K, 4K, 8K, 16K] Hz.
    /// @param bandGain Band gain for the index, the value range is [-15, 15]. Default value is 0, if all gain values in all frequency bands are 0, EQ function will be disabled.
    virtual void setAudioEqualizerGain(int bandIndex, float bandGain) = 0;

    /// Setting up the voice changer via preset enumeration.
    ///
    /// Available since: 1.17.0
    /// Description: Call this function to use preset voice changer effect.
    /// Use cases: Often used in live broadcasting, voice chatroom and KTV.
    /// When to call: It needs to be called after [createEngine].
    /// Related APIs:
    /// If you need advanced voice changer effect, please use [setVoiceChangerParam].
    /// The effect of using this function together with [setReverbPreset] may be different from what is expected. If you need to use it at the same time, it is recommended to enable the voice changer first, and then enable the reverb.
    /// Using ANDROID/ETHEREAL preset voice changer effect will modify reverberation or reverberation echo parameters. Calling [setVoiceChangerParam], [setReverbAdvancedParam], [setReverbEchoParam] may affect the voice changer effect after use these preset voice changer effect.
    /// If you need advanced reverb/echo/electronic effects/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setElectronicEffects], [setVoiceChangerParam] together.
    ///
    /// @param preset The voice changer preset enumeration.
    virtual void setVoiceChangerPreset(ZegoVoiceChangerPreset preset) = 0;

    /// Setting up the specific voice changer parameters.
    ///
    /// Available since: 1.10.0
    /// Description: Call this function to set custom voice changer effect.
    /// Use cases: Often used in live broadcasting, voice chatroom and KTV.
    /// When to call: It needs to be called after [createEngine].
    /// Related APIs:
    /// [setVoiceChangerPreset] provide a set of preset voice changer effects.
    /// If you need advanced reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
    ///
    /// @param param Voice changer parameters.
    virtual void setVoiceChangerParam(ZegoVoiceChangerParam param) = 0;

    /// Setting up the reverberation via preset enumeration.
    ///
    /// Available since: 1.17.0
    /// Description: Call this function to set preset reverb effect.
    /// Use cases: Often used in live broadcasting, voice chatroom and KTV.
    /// When to call: It needs to be called after [createEngine]. Support call this function to change preset reverb effect during publishing stream.
    /// Related APIs:
    /// If you need advanced reverb effect, please use [setReverbAdvancedParam].
    /// The effect of using this function together with [setVoiceChangerPreset] may be different from what is expected. If you need to use it at the same time, it is recommended to enable the voice changer first, and then enable the reverb.
    /// If you need advanced reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
    ///
    /// @param preset The reverberation preset enumeration.
    virtual void setReverbPreset(ZegoReverbPreset preset) = 0;

    /// Setting up the specific reverberation parameters.
    ///
    /// Available since: 1.10.0
    /// Description: Call this function to set preset reverb effect.
    /// Use cases: Often used in live broadcasting, voice chatroom and KTV.
    /// When to call: It needs to be called after [createEngine].
    /// Caution: Different values dynamically set during publishing stream will take effect. When all parameters are set to 0, the reverberation is turned off.
    /// Related APIs:
    /// [setReverbPreset] provide a set of preset reverb effects.
    /// If you need advanced reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
    ///
    /// @param param Reverb advanced parameter.
    virtual void setReverbAdvancedParam(ZegoReverbAdvancedParam param) = 0;

    /// Setting up the specific reverberation echo parameters.
    ///
    /// Available since: 1.17.0
    /// Description: Call this function to set reverb echo effect. This function can be used with voice changer and reverb to achieve a variety of custom sound effects.
    /// Use cases: Often used in live broadcasting, voice chatroom and KTV.
    /// When to call: It needs to be called after [createEngine].
    /// Related APIs: If you need advanced reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
    ///
    /// @param param The reverberation echo parameter.
    virtual void setReverbEchoParam(ZegoReverbEchoParam param) = 0;

    /// Enable or disable the virtual stereo effect when publishing stream.
    ///
    /// Available since: 1.10.0; Note: Starting from 2.15.0, the angle parameter supports setting -1 to present a all round virtual stereo effect.
    /// Description: Call this function to enable / disable the virtual stereo effect when publishing stream.
    /// Use cases: Often used in live broadcasting, voice chatroom and KTV.
    /// When to call: It needs to be called after [createEngine].
    /// Caution: You need to set up a dual channel with [setAudioConfig] for the virtual stereo to take effect.
    ///
    /// @param enable true to turn on the virtual stereo, false to turn off the virtual stereo.
    /// @param angle The angle of the sound source in virtual stereo in the range of -1 ~ 360, with 90 being directly in front, 0 / 180 / 270 corresponding to the rightmost and leftmost respectively. In particular, when set to -1, it is all round virtual stereo effects.
    virtual void enableVirtualStereo(bool enable, int angle) = 0;

    /// Enable or disable the virtual stereo effect when playing stream.
    ///
    /// Available since: 2.8.0
    /// Description: Call this function to enable/disable the virtual stereo effect when playing stream.
    /// Use cases: Often used in live broadcasting, voice chatroom and KTV.
    /// When to call: It needs to be called after [createEngine].
    /// Caution: It can dynamically switch and set angle parameters before and after playing stream. After stopping playing stream, it will automatically reset and disable.
    ///
    /// @param enable true to turn on the virtual stereo, false to turn off the virtual stereo.
    /// @param angle The angle of the sound source in virtual stereo in the range of 0 ~ 360, with 90 being directly in front, 0 / 180 / 270 corresponding to the rightmost and leftmost respectively.
    /// @param streamID Stream ID.
    virtual void enablePlayStreamVirtualStereo(bool enable, int angle,
                                               const std::string &streamID) = 0;

    /// Turn on or off the electronic sound effect.
    ///
    /// Available since: 2.13.0
    /// Description: Call this function to turn on or off the electronic sound effect.
    /// Use cases: Often used in live broadcasting, voice chatroom and sung unaccompanied scenes.
    /// Default value: When this function is not called, the electronic sound effect is not enabled by default.
    /// When to call: It needs to be called after [createEngine].
    /// Restrictions: None.
    /// Caution: When the mode parameter is Harmonic Minor, the tonal parameter does not take effect.
    /// Related APIs: Common electronic sound effect configurations can be set via [setVoiceChangerPreset].
    ///
    /// @param enable true to turn on the electronic sound effect, false to turn off the electronic sound effect.
    /// @param mode Mode of Electronic Effects reference.
    /// @param tonal The starting pitch of an electric tone in a given mode, representing 12 semitones in one octave of the sound, in the range [0, 11].
    virtual void setElectronicEffects(bool enable, ZegoElectronicEffectsMode mode, int tonal) = 0;

    /// Enable color enhancement.
    ///
    /// Available since: 3.11.0
    /// Description: Call this function to enable or disable color enhancement.
    /// Use cases: Commonly used in video calling, live streaming, and similar scenarios.
    /// Default value: When this function is not called, color enhancement is not enabled by default.
    /// When to call: It needs to be called after [createEngine].
    ///
    /// @param enable Whether to enable, true: enable, false: disable
    /// @param params Color enhancement parameters.
    /// @param channel Publish stream channel.
    virtual void enableColorEnhancement(bool enable, ZegoColorEnhancementParams params,
                                        ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Create the real time sequential data manager instance
    ///
    /// Available: since 2.14.0
    /// Description: Create a manager object for sending/receiving real-time signaling.
    /// Use cases: You can use this function when you need to use real-time signaling to implement services such as cloud games and remote desktops.
    /// When to call: After logging in to a room.
    /// Restrictions: Only one corresponding manager object can be created for each room ID of [loginRoom], that is, only one object can be created in single room mode, and multiple objects can be created in multi-room mode.
    /// Caution: None.
    ///
    /// @param roomID Fill in the room ID that has been logged in, and all related stuff will be do in this room.
    /// @return The real-time sequential data manager instance, nullptr will be returned when the maximum number is exceeded.
    virtual IZegoRealTimeSequentialDataManager *
    createRealTimeSequentialDataManager(const std::string &roomID) = 0;

    /// Destroy the real time sequential data manager instance
    ///
    /// Available: since 2.14.0
    /// Description: Destroy the [ZegoRealTimeSequentialDataManager] instance object.
    /// Use cases: When you no longer need to use the real-time signaling function, you can use this function to destroy the instance object created by the [createRealTimeSequentialDataManager] function
    /// When to call: When you need to the real-time signaling manager object needs to be destroyed
    /// Restrictions: After destroy the instance, you need to release the [ZegoRealTimeSequentialDataManager] instance object you hold by yourself, and don’t call the function of this instance object after the destruction.
    /// Caution: None.
    ///
    /// @param manager The real time sequential data manager instance to be destroyed.
    virtual void
    destroyRealTimeSequentialDataManager(IZegoRealTimeSequentialDataManager *&manager) = 0;

    /// Sends a Broadcast Message.
    ///
    /// Available since: 1.2.1
    /// Description: Send a broadcast message to the room, users who have entered the same room can receive the message, and the message is reliable.
    /// Use cases: Generally used when the number of people in the live room does not exceed 500.
    /// When to call: After calling [loginRoom] to log in to the room.
    /// Restrictions: It is not supported when the number of people online in the room exceeds 500. If you need to increase the limit, please contact ZEGO technical support to apply for evaluation. The frequency of sending broadcast messages in the same room cannot be higher than 10 messages/s. The maximum QPS for a single user calling this interface from the client side is 2. For restrictions on the use of this function, please contact ZEGO technical support.
    /// Related callbacks: The room broadcast message can be received through [onIMRecvBroadcastMessage].
    /// Related APIs: Barrage messages can be sent through the [sendBarrageMessage] function, and custom command can be sent through the [sendCustomCommand] function.
    ///
    /// @param roomID Room ID, a string of less 128 bytes in length.
    ///   Caution:
    ///   1. room ID is defined by yourself.
    ///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
    ///   3. If you need to communicate with the Web SDK, please do not use '%'.
    /// @param message The content of the message. Required: Yes. Value range: The length does not exceed 1024 bytes.
    /// @param callback Send a notification of the result of a broadcast message. Required: No. Caution: Passing [nullptr] means not receiving callback notifications.
    virtual void sendBroadcastMessage(const std::string &roomID, const std::string &message,
                                      ZegoIMSendBroadcastMessageCallback callback) = 0;

    /// Sends a Barrage Message (bullet screen) to all users in the same room, without guaranteeing the delivery.
    ///
    /// Available since: 1.5.0
    /// Description: Send a barrage message to the room, users who have logged in to the same room can receive the message, the message is unreliable.
    /// Use cases: Generally used in scenarios where there is a large number of messages sent and received in the room and the reliability of the messages is not required, such as live barrage.
    /// When to call: After calling [loginRoom] to log in to the room.
    /// Restrictions: The frequency of sending barrage messages in the same room cannot be higher than 20 messages/s. For restrictions on the use of this function, please contact ZEGO technical support.
    /// Related callbacks: The room barrage message can be received through [onIMRecvBarrageMessage].
    /// Related APIs: Broadcast messages can be sent through the [sendBroadcastMessage] function, and custom command can be sent through the [sendCustomCommand] function.
    ///
    /// @param roomID Room ID, a string of less 128 bytes in length.
    ///   Caution:
    ///   1. room ID is defined by yourself.
    ///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
    ///   3. If you need to communicate with the Web SDK, please do not use '%'.
    /// @param message The content of the message. Required: Yes. Value range: The length does not exceed 1024 bytes.
    /// @param callback Send barrage message result callback.Required: No. Caution: Passing [nullptr] means not receiving callback notifications.
    virtual void sendBarrageMessage(const std::string &roomID, const std::string &message,
                                    ZegoIMSendBarrageMessageCallback callback) = 0;

    /// Sends a Custom Command to the specified users in the same room.
    ///
    /// Available since: 1.2.1
    /// Description: After calling this function, users in the same room who have entered the room can receive the message, the message is unreliable.
    /// Use cases: Generally used in scenarios where there is a large number of messages sent and received in the room and the reliability of the messages is not required, such as live barrage.
    /// When to call: After calling [loginRoom] to log in to the room.
    /// Restrictions: Generally used when the number of people in the live room does not exceed 500.The frequency of sending barrage messages in the same room cannot be higher than 20 messages/s. For restrictions on the use of this function, please contact ZEGO technical support.
    /// Related callbacks: The room custom command can be received through [onIMRecvCustomCommand].
    /// Related APIs: Broadcast messages can be sent through the [sendBroadcastMessage] function, and barrage messages can be sent through the [sendBarrageMessage] function.
    /// Privacy reminder: Please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
    ///
    /// @param roomID Room ID, a string of less 128 bytes in length.
    ///   Caution:
    ///   1. room ID is defined by yourself.
    ///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
    ///   3. If you need to communicate with the Web SDK, please do not use '%'.
    /// @param command Custom command content. Required: Yes. Value range: The maximum length is 1024 bytes. Caution: To protect privacy, please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
    /// @param toUserList List of recipients of signaling. Required: Yes. Value range: user list or [nullptr]. Caution: When it is [nullptr], the SDK will send custom signaling back to all users in the room
    /// @param callback Send a notification of the signaling result. Required: No. Caution: Passing [nullptr] means not receiving callback notifications.
    virtual void sendCustomCommand(const std::string &roomID, const std::string &command,
                                   std::vector<ZegoUser> toUserList,
                                   ZegoIMSendCustomCommandCallback callback) = 0;

    /// Sends a transparent message in room.
    ///
    /// Available since: 1.2.1
    /// Description: After calling this function, users in the same room who have entered the room can receive the message, the message is unreliable.
    /// Use cases: It is generally used when the number of people in the live room is not more than 500. The frequency of custom messages sent to a single user in the same room cannot be higher than 200 /s .
    /// When to call: After calling [loginRoom] to log in to the room.
    /// Restrictions: Generally used when the number of people in the live room does not exceed 500.The frequency of sending barrage messages in the same room cannot be higher than 20 messages/s. For restrictions on the use of this function, please contact ZEGO technical support.
    /// Related callbacks: When sending a message, Mode is specified for ZegoRoomTransparentMessageModeOnlyClient or ZegoRoomTransparentMessageModeClientAndServer can pass [onRecvRoomTransparentMessage] received sends the message content.
    /// Privacy reminder: Please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
    ///
    /// @param roomID Room ID, a string of less 128 bytes in length.
    ///   Caution:
    ///   1. room ID is defined by yourself.
    ///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
    ///   3. If you need to communicate with the Web SDK, please do not use '%'.
    /// @param message ZegoRoomSendTransparentMessage
    /// @param callback Send a notification of the signaling result. Required: No. Caution: Passing [nullptr] means not receiving callback notifications.
    virtual void sendTransparentMessage(const std::string &roomID,
                                        const ZegoRoomSendTransparentMessage &message,
                                        ZegoRoomSendTransparentMessageCallback callback) = 0;

    /// Creates a media player instance.
    ///
    /// Available since: 2.1.0
    /// Description: Creates a media player instance.
    /// Use case: It is often used to play media resource scenes, For example, play video files, push the video of media resources in combination with custom video acquisition, and the remote end can pull the stream for viewing.
    /// When to call: It can be called after the SDK by [createEngine] has been initialized.
    /// Restrictions: Currently, a maximum of 4 instances can be created, after which it will return nullptr.
    /// Caution: The more instances of a media player, the greater the performance overhead on the device.
    /// Related APIs: User can call [destroyMediaPlayer] function to destroy a media player instance.
    ///
    /// @return Media player instance, nullptr will be returned when the maximum number is exceeded.
    virtual IZegoMediaPlayer *createMediaPlayer() = 0;

    /// Destroys a media player instance.
    ///
    /// Available since: 2.1.0
    /// Description: Destroys a media player instance.
    /// Related APIs: User can call [createMediaPlayer] function to create a media player instance.
    ///
    /// @param mediaPlayer The media player instance object to be destroyed.
    virtual void destroyMediaPlayer(IZegoMediaPlayer *&mediaPlayer) = 0;

    /// Creates a audio effect player instance.
    ///
    /// Available since: 1.16.0
    /// Description: Creates a audio effect player instance.
    /// Use cases: When you need to play short sound effects, such as applause, cheers, etc., you can use audioEffectPlayer to achieve.
    /// When to call: It can be called after [createEngine].
    /// Restrictions: Currently, a maximum of 1 instances can be created, after which it will return nullptr.
    /// Related APIs: [destroyAudioEffectPlayer].
    ///
    /// @return audio effect player instance, nullptr will be returned when the maximum number is exceeded.
    virtual IZegoAudioEffectPlayer *createAudioEffectPlayer() = 0;

    /// Destroys a audio effect player instance.
    ///
    /// Available since: 1.16.0
    /// Description: Destroys the specified audio effect player instance.
    /// When to call: It can be called after [createAudioEffectPlayer].
    /// Restrictions: None.
    /// Related APIs: [createAudioEffectPlayer].
    ///
    /// @param audioEffectPlayer The audio effect player instance object to be destroyed.
    virtual void destroyAudioEffectPlayer(IZegoAudioEffectPlayer *&audioEffectPlayer) = 0;

    /// Creates a media data publisher instance.
    ///
    /// Available since: 2.17.0
    /// Description: Developers can use this interface to create ZegoMediaDataPublisher objects to push local media resource files to implement server-side streaming scenarios, such as AI classrooms.
    /// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
    /// When to call: After calling the [createEngine] function to create an engine.
    /// Caution: It returns the instance object of the main publish channel. It will return the instance object if it already exists.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @return Media data publisher instance.
    virtual IZegoMediaDataPublisher *createMediaDataPublisher() = 0;

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
    /// @return Media data publisher instance.
    virtual IZegoMediaDataPublisher *
    createMediaDataPublisher(ZegoMediaDataPublisherConfig config) = 0;

    /// Destroys a media data publisher instance.
    ///
    /// Available since: 2.17.0
    /// Description: Destroys a media data publisher instance.
    /// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
    /// When to call: After calling the [createEngine] function to create an engine.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param mediaDataPublisher The media data publisher instance object to be destroyed
    virtual void destroyMediaDataPublisher(IZegoMediaDataPublisher *&mediaDataPublisher) = 0;

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
    virtual void
    startRecordingCapturedData(ZegoDataRecordConfig config,
                               ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Stops recording locally captured audio or video.
    ///
    /// Available since: 1.10.0
    /// Description: Stops recording locally captured audio or video.
    /// When to call: After [startRecordingCapturedData].
    /// Restrictions: None.
    ///
    /// @param channel Publishing stream channel.
    virtual void
    stopRecordingCapturedData(ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Sets up the event callback handler for data recording.
    ///
    /// Available since: 1.10.0
    /// Description: Implemente the functions of ZegoDataRecordEventHandler to get notified when recode state and process changed.
    /// When to call: After [createEngine].
    /// Restrictions: None.
    ///
    /// @param eventHandler Data record event handler.
    virtual void
    setDataRecordEventHandler(std::shared_ptr<IZegoDataRecordEventHandler> eventHandler) = 0;

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
    virtual void startPerformanceMonitor(unsigned int millisecond = 2000) = 0;

    /// Stop system performance monitoring.
    ///
    /// Available since: 1.19.0
    /// Description: Stop system performance monitoring. After the monitoring is stopped, the [onPerformanceStatusUpdate] callback will not triggered.
    /// Use cases: Monitor system performance can help user quickly locate and solve performance problems and improve user experience.
    /// When to call: It needs to be called after [createEngine].
    /// Restrictions: None.
    /// Related APIs: Call [startPerformanceMonitor] to start system performance monitoring.
    virtual void stopPerformanceMonitor() = 0;

    /// Start network probe.
    ///
    /// Available since: 2.3.0
    /// Description: Some local network problems may cause audio and video calls to fail. Using this function to probe the network protocols, assist in locating and solving related network problems.
    /// Use cases: Before pushing and pulling the stream, detect and locate some possible network problems.
    /// When to call: It needs to be called after [createEngine].
    /// Restrictions: The SDK will not perform multiple network probe at the same time, that is, if the network probe is in progress, the SDK will ignore subsequent calls of this function.
    /// Caution: The SDK detects http, tcp, and udp in sequence internally. If the probe fails in the middle, the subsequent detection will not continue. Therefore, when reading the values ​​in the probe result, please pay attention to check if the value is nullptr. Network probe may take a long time. Developers can call [stopNetworkProbe] to stop network probe if needed. It is not recommended to start network probe during publishing/playing stream.
    /// Related APIs: Call [stopNetworkProbe] to stop network probe.
    ///
    /// @param config network probe config.
    /// @param callback Network probe result callback.
    virtual void startNetworkProbe(ZegoNetworkProbeConfig config,
                                   ZegoNetworkProbeResultCallback callback) = 0;

    /// Stop network probe.
    ///
    /// Available since: 2.3.0
    /// Description: Stop network probe.
    /// Use cases: Before pushing and pulling the stream, detect and locate some possible network problems.
    /// When to call: It needs to be called after [createEngine].
    /// Restrictions: None.
    /// Related APIs: Call [startNetworkProbe] to start network probe.
    virtual void stopNetworkProbe() = 0;

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
    virtual void startNetworkSpeedTest(ZegoNetworkSpeedTestConfig config,
                                       unsigned int interval = 3000) = 0;

    /// Stop network speed test.
    ///
    /// Available since: 1.20.0
    /// Description: Stop network speed test.
    /// Use cases: This function can be used to detect whether the network environment is suitable for pushing/pulling streams with specified bitrates.
    /// When to call: It needs to be called after [createEngine].
    /// Restrictions: None.
    /// Caution: After the network speed test stopped, [onNetworkSpeedTestQualityUpdate] callback will not be triggered.
    /// Related APIs: Call [startNetworkSpeedTest] to start network speed test.
    virtual void stopNetworkSpeedTest() = 0;

    /// Obtain synchronization network time information.
    ///
    /// Available since: 2.9.0
    /// Description: Obtain synchronization network time(NTP), including timestamp and maximum deviation.
    /// Use cases: When performing multi-terminal synchronization behaviors, network time synchronization is required.
    /// When to call: It needs to be called after [createEngine].
    /// Restrictions: None.
    virtual ZegoNetworkTimeInfo getNetworkTimeInfo() = 0;

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
    virtual void startDumpData(ZegoDumpDataConfig config) = 0;

    /// Stop dumping data.
    ///
    /// Available since: 3.10.0
    /// Description: Stop dumping data.
    /// Use cases: This is a debugging tool. When there is a problem with audio capturing, 3A processing, or other environment processing during publish, you can dump the audio data and upload it to the ZEGO server for further analysis.
    /// When to call: It needs to be called after [startDumpData].
    /// Restrictions: None.
    /// Caution: It will trigger the [onUploadDumpData] callback.
    virtual void stopDumpData() = 0;

    /// Upload dumped data to the ZEGO server.
    ///
    /// Available since: 3.10.0
    /// Description: Upload dumped data to the ZEGO server.
    /// Use cases: This is a debugging tool. When there is a problem with audio capturing, 3A processing, or other environment processing during publish, you can dump the audio data and upload it to the ZEGO server for further analysis.
    /// When to call: It needs to be called after [stopDumpData].
    /// Restrictions: None.
    /// Caution: It will trigger the [onUploadDumpData] callback when dump data uploaded.
    virtual void uploadDumpData() = 0;

    /// Remove dumped data.
    ///
    /// Available since: 3.10.0
    /// Description: Remove dumped data.
    /// Use cases: This is a debugging tool. When there is a problem with audio capturing, 3A processing, or other environment processing during publish, you can dump the audio data and upload it to the ZEGO server for further analysis.
    /// When to call: It needs to be called after [stopDumpData]. If the dump data is to be uploaded to the ZEGO server, it should be deleted after the upload is successful.
    /// Restrictions: None.
    virtual void removeDumpData() = 0;

    /// Enables or disables custom video rendering.
    ///
    /// Available since: 1.9.0
    /// Description: When enable is `true`,video custom rendering is enabled; if the value of `false`, video custom rendering is disabled.
    /// Use case: Use beauty features or apps that use a cross-platform UI framework (for example, Qt requires a complex hierarchical UI to achieve high-experience interaction) or game engine (e.g. Unity, Unreal Engine, Cocos)
    /// Default value: Custom video rendering is turned off by default when this function is not called.
    /// When to call: After [createEngine], and before calling [startPreview], [startPublishingStream], [startPlayingStream], [createRealTimeSequentialDataManager]. The configuration can only be modified after the engine is stopped, that is, after [logoutRoom] is called.
    /// Caution: Custom video rendering can be used in conjunction with custom video capture, but when both are enabled, the local capture frame callback for custom video rendering will no longer be called back, you should directly use the captured video frame from the custom video capture source.
    /// Related callbacks: Call [setCustomVideoRenderHandler] to set the callback to get video frame data. [onCapturedVideoFrameRawData] local preview video frame data callback, [onRemoteVideoFrameRawData] remote playing stream video frame data callback.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param enable enable or disable
    /// @param config custom video render config
    virtual void enableCustomVideoRender(bool enable, ZegoCustomVideoRenderConfig *config) = 0;

    /// Sets up the event callback handler for custom video rendering.
    ///
    /// Available since: 1.9.0
    /// Description: Custom video rendering, where the developer is responsible for rendering the video data onto the UI components. With the change callback set, a callback that throws video data to the developer will be triggered when the developer calls startPreview, startPublishingStream, and startPlayingStream.
    /// Use case: Use beauty features or apps that use a cross-platform UI framework (for example, Qt requires a complex hierarchical UI to achieve high-experience interaction) or game engine (e.g. Unity, Unreal Engine, Cocos)
    /// When to call: After [createEngine].
    /// Related callbacks: [onCapturedVideoFrameRawData] local preview video frame data callback, [onRemoteVideoFrameRawData] remote playing stream video frame data callback.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param handler Custom video render handler
    virtual void
    setCustomVideoRenderHandler(std::shared_ptr<IZegoCustomVideoRenderHandler> handler) = 0;

    /// Enables or disables captured video to custom video rendering.
    ///
    /// Available since: 3.13.0
    /// Description: enable or disable custom video rendering of captured video during publish.
    /// Use case: Use beauty features or apps that use a cross-platform UI framework (for example, Qt requires a complex hierarchical UI to achieve high-experience interaction) or game engine (e.g. Unity, Unreal Engine, Cocos)
    /// Default value: After [enableCustomVideoRender] enables custom video rendering, all captured videos will be used as custom video data.
    /// When to call: After [enableCustomVideoRender] enables custom video rendering, call [enableCapturedVideoCustomVideoRender] to config whether the specified channel used custom video render or not.
    /// Caution: You can call this API only after [enableCustomVideoRender] enables custom video rendering.Custom video rendering can be used in conjunction with custom video capture, but when both are enabled, the local capture frame callback for custom video rendering will no longer be called back, you should directly use the captured video frame from the custom video capture source.
    /// Related callbacks: Call [setCustomVideoRenderHandler] to set the callback to get video frame data. [onCapturedVideoFrameRawData] local preview video frame data callback, [onRemoteVideoFrameRawData] remote playing stream video frame data callback.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param enable enable or disable
    /// @param channel publish channel
    virtual void enableCapturedVideoCustomVideoRender(bool enable, ZegoPublishChannel channel) = 0;

    /// Enables or disables remote playback video to custom video rendering.
    ///
    /// Available since: 3.13.0
    /// Description: enable or disable custom video rendering during playback.
    /// Use case: Use beauty features or apps that use a cross-platform UI framework (for example, Qt requires a complex hierarchical UI to achieve high-experience interaction) or game engine (e.g. Unity, Unreal Engine, Cocos)
    /// Default value: After [enableCustomVideoRender] enables custom video rendering, all remote playback videos will be used as custom video data.
    /// When to call: After [enableCustomVideoRender] enables custom video rendering, call [enableRemoteVideoCustomVideoRender] to config whether the specified stream used custom video render or not.
    /// Caution: You can call this API only after [enableCustomVideoRender] enables custom video rendering.
    /// Related callbacks: Call [setCustomVideoRenderHandler] to set the callback to get video frame data. [onCapturedVideoFrameRawData] local preview video frame data callback, [onRemoteVideoFrameRawData] remote playing stream video frame data callback.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param enable enable or disable
    /// @param streamID Stream ID.
    virtual void enableRemoteVideoCustomVideoRender(bool enable, const std::string &streamID) = 0;

    /// Enables or disables custom video capture (for the specified channel).
    ///
    /// Available since: 1.9.0
    /// Description: If the value of enable is true, the video collection function is enabled. If the value of enable is false, the video collection function is disabled.
    /// Use case: The App developed by the developer uses the beauty SDK of a third-party beauty manufacturer to broadcast non-camera collected data.
    /// Default value: When this function is not called, custom video collection is disabled by default.
    /// When to call: After [createEngine], call [startPreview], [startPublishingStream], [createRealTimeSequentialDataManager], and call [logoutRoom] to modify the configuration.
    /// Caution: Custom video rendering can be used in conjunction with custom video capture, but when both are enabled, the local capture frame callback for custom video rendering will no longer be triggered, and the developer should directly capture the captured video frame from the custom video capture source.
    /// Related callbacks: When developers to open a custom collection, by calling [setCustomVideoCaptureHandler] can be set up to receive a custom collection start-stop event notification.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param enable enable or disable
    /// @param config custom video capture config
    /// @param channel publish channel
    virtual void
    enableCustomVideoCapture(bool enable, ZegoCustomVideoCaptureConfig *config,
                             ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Sets the event callback handler for custom video capture.
    ///
    /// Available since: 1.9.0
    /// Description: Example Set the callback for custom video capture. That is, the developer is responsible for collecting video data, and sends the collected video data to the SDK for coding and pushing the video data to the ZEGO RTC server.
    /// Use case: This feature is typically used by developers using third-party beauty features or livestreaming of games.
    /// When to call: After [createEngine] .
    /// Caution: In the case of customized video collection, the SDK will not start the camera to collect video data, and developers need to collect video data from the video collection source. A callback notifying the developer that it can start sending video data is triggered when the developer calls [startPreview] to start pushing the stream to [startPublishingStream]. A callback notifying the developer that it can stop sending video data is triggered when [stopPreview] and [stopPublishingStream].
    /// Related callbacks: [onStart] Starts collection, and [onStop] stops collection.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param handler Custom video capture handler
    virtual void
    setCustomVideoCaptureHandler(std::shared_ptr<IZegoCustomVideoCaptureHandler> handler) = 0;

    /// Sends the video frames (Raw Data) produced by custom video capture to the SDK (for the specified channel).
    ///
    /// Available since: 1.9.0
    /// Description: Sends customized raw video frame data to the SDK.
    /// When to call: After receiving the [onStart] notification, the developer starts the call after the collection logic starts and ends the call after the [onStop] notification.
    /// Caution: This interface must be called with [enableCustomVideoCapture] passing the parameter type RAW_DATA.
    /// Related APIs: [enableCustomVideoCapture], [setCustomVideoCaptureHandler].
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param data video frame data
    /// @param dataLength video frame data length
    /// @param params video frame param
    /// @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
    /// @param channel Publishing stream channel
    virtual void
    sendCustomVideoCaptureRawData(const unsigned char *data, unsigned int dataLength,
                                  ZegoVideoFrameParam params,
                                  unsigned long long referenceTimeMillisecond,
                                  ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Send custom captured video frame D3D Texture data to the SDK (for the specified channel).
    ///
    /// Available since: 3.3.0
    /// Description: Send custom captured video frame type ZEGO_VIDEO_BUFFER_TYPE_D3D_TEXTURE_2D data to the SDK.
    /// When to call: After receiving the [onStart] notification, the developer starts the call after the collection logic starts and ends the call after the [onStop] notification.
    /// Caution: Calling this interface must be [enableCustomVideoCapture] with an argument of type ZEGO_VIDEO_BUFFER_TYPE_D3D_TEXTURE_2D. only D3D11 DXGI_FORMAT_B8G8R8A8_UNORM texture is supported
    /// Related APIs: [enableCustomVideoCapture], [setCustomVideoCaptureHandler].
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param texture D3D texture。只支持 D3D11 的 DXGI_FORMAT_B8G8R8A8_UNORM 格式纹理
    /// @param rotation The rotation direction of the video frame, the SDK rotates clockwise
    /// @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
    /// @param channel Publishing stream channel
    /// @return Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
    virtual int sendCustomVideoCaptureD3DTextureData(
        void *texture, int rotation, unsigned long long referenceTimeMillisecond,
        ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Sends the video frames (Encoded Data) produced by custom video capture to the SDK (for the specified channel).
    ///
    /// Available since: 1.9.0
    /// Description: Send custom captured video frame type CVPixelBuffer data to the SDK.
    /// When to call: After receiving the [onStart] notification, the developer starts the call after the collection logic starts and ends the call after the [onStop] notification.
    /// Caution: It is recommended to make a GOP every 2s; each I frame must carry SPS and PPS which should be placed at the front of the data; the SPS, PPS and I frame need to be combined into one frame for transmission. Before calling this API, you need to ensure that the parameter type specified when calling [enableCustomVideoCapture] is [EncodedData]
    /// Related APIs: [enableCustomVideoCapture], [setCustomVideoCaptureHandler].
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param data video frame encoded data
    /// @param dataLength video frame data length
    /// @param params video frame param
    /// @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
    /// @param channel Publishing stream channel
    virtual void
    sendCustomVideoCaptureEncodedData(const unsigned char *data, unsigned int dataLength,
                                      ZegoVideoEncodedFrameParam params,
                                      unsigned long long referenceTimeMillisecond,
                                      ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Sets the video fill mode of custom video capture (for the specified channel).
    ///
    /// Available since: 1.10.0
    /// Use cases: If the customized collection resolution and SDK coding resolution are inconsistent, THE SDK will process the video frame according to the fill mode and adjust it to the coding resolution.
    /// Description: When the custom video collection function is enabled, you can use this interface to set the video zooming and filling mode.
    /// When to call: Before [sendCustomVideoCaptureRawData], [sendCustomVideoCaptureTextureData], [sendCustomVideoCapturePixelBuffer], [sendCustomVideoCaptureEncodedData].
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param mode View mode
    /// @param channel Publishing stream channel
    virtual void
    setCustomVideoCaptureFillMode(ZegoViewMode mode,
                                  ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Sets device state of custom video capture (for the specified channel).
    ///
    /// Available since: 2.15.0.
    /// Description: User can set the video device state of custom video capture (for the specified channel) through this function.
    /// When to call: After the callback [onStart] is received.
    /// Restrictions: None.
    /// Related Callbacks: The remote user that play stream of local user publishing stream can receive state update notification by monitoring [onRemoteCameraStateUpdate] callbacks.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param isEnable Whether the device is enable.
    /// @param state The device state.
    /// @param channel Publishing stream channel
    virtual void
    setCustomVideoCaptureDeviceState(bool isEnable, ZegoRemoteDeviceState state,
                                     ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Set the region of interest (ROI) for encoder of custom video capture (for the specified channel).
    ///
    /// Available since: 2.16.0.
    /// Description: You can set the video encoder's region of interest rectangle areas of custom video capture (for the specified channel) through this function.
    /// When to call: After the callback [onStart] is received.
    /// Restrictions: Currently, only certain video encoders support this function, please contact ZEGO technical support before using it.
    /// Caution: This function is currently an experimental feature, please contact ZEGO technical support before using it.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param rectList ROI rectangle area list, currently supports up to 6 areas.
    /// @param channel Publish channel
    virtual void setCustomVideoCaptureRegionOfInterest(
        std::vector<ZegoRoiRect> rectList,
        ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Enables or disables custom video processing, and support specifying the publish channel.
    ///
    /// Available since: 2.2.0 (Android/iOS/macOS native), 2.4.0 (Windows/macOS C++).
    /// Description: When the developer opens custom pre-processing, by calling [setCustomVideoProcessHandler] you can set the custom video pre-processing callback.
    /// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
    /// Default value: Off by default
    /// When to call: Must be set before calling [startPreview], [startPublishingStream], [createRealTimeSequentialDataManager]. If you need to modify the configuration, please call [logoutRoom] to log out of the room first, otherwise it will not take effect.
    /// Restrictions: None.
    /// Related APIs: Call the [setCustomVideoProcessHandler] function to set the callback before custom video processing.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param enable enable or disable. Required: Yes.
    /// @param config custom video processing configuration. Required: Yes.Caution: If NULL is passed, the platform default value is used.
    /// @param channel Publishing stream channel.Required: No.Default value: Main publish channel.
    virtual void
    enableCustomVideoProcessing(bool enable, ZegoCustomVideoProcessConfig *config,
                                ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Sets up the event callback handler for custom video processing.
    ///
    /// Available since: 2.2.0 (Android/iOS/macOS native), 2.4.0 (Windows/macOS C++).
    /// Description: When the developer opens the custom pre-processing, by calling [setCustomVideoProcessHandler], you can set the custom video pre-processing callback to obtain the original video data.
    /// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
    /// When to call: Called before [startPreview], [startPublishingStream] and [createRealTimeSequentialDataManager], otherwise it may cause the timing of obtaining video data to be too slow.
    /// Restrictions: None.
    /// Related APIs: Call [enableCustomVideoProcessing] function to enable custom video pre-processing callback.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param handler Custom video process handler.Required: Yes.
    virtual void
    setCustomVideoProcessHandler(std::shared_ptr<IZegoCustomVideoProcessHandler> handler) = 0;

    /// Send the original video data after the pre-processing of the custom video to the SDK, and support other channels to push the stream.
    ///
    /// Available since: 2.4.0
    /// Description: When the developer opens the custom pre-processing, by calling [setCustomVideoProcessHandler], you can set the custom video pre-processing callback to obtain the original video data.
    /// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
    /// When to call: Must be called in the [onCapturedUnprocessedCVPixelbuffer] callback.
    /// Restrictions: None.
    /// Platform differences: Only valid on Windows platform.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param data Raw video data. RGB format data storage location is data[0], YUV format data storage location is Y component：data[0], U component：data[1], V component：data[2].
    /// @param dataLength Raw video data length. RGB format data length storage location is dataLength[0], YUV format data storage location respectively Y component length：dataLength[0], U component length：dataLength[1], V component length：dataLength[2].
    /// @param params video frame param.
    /// @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
    /// @param channel Publishing stream channel.Required: No.Default value: Main publish channel.
    virtual void
    sendCustomVideoProcessedRawData(const unsigned char **data, unsigned int *dataLength,
                                    ZegoVideoFrameParam params,
                                    unsigned long long referenceTimeMillisecond,
                                    ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Send the original video data after the pre-processing of the custom video to the SDK, support other channels to push the stream, and support specified data usage.
    ///
    /// Available since: 3.14.0
    /// Description: When the developer opens the custom pre-processing, by calling [setCustomVideoProcessHandler], you can set the custom video pre-processing callback to obtain the original video data.
    /// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
    /// When to call: Must be called in the [onCapturedUnprocessedCVPixelbuffer] callback.
    /// Restrictions: None.
    /// Platform differences: Only valid on Windows platform.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param data Raw video data. RGB format data storage location is data[0], YUV format data storage location is Y component：data[0], U component：data[1], V component：data[2].
    /// @param dataLength Raw video data length. RGB format data length storage location is dataLength[0], YUV format data storage location respectively Y component length：dataLength[0], U component length：dataLength[1], V component length：dataLength[2].
    /// @param params video frame param.
    /// @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
    /// @param usage Data usage
    /// @param channel Publishing stream channel.Required: No.Default value: Main publish channel.
    virtual void sendCustomVideoProcessedRawData(
        const unsigned char **data, unsigned int *dataLength, ZegoVideoFrameParam params,
        unsigned long long referenceTimeMillisecond, ZegoProcessedDataUsageType usage,
        ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Send the [CVPixelBuffer] type video data after the custom video processing to the SDK (for the specified channel).
    ///
    /// Available since: 2.2.0 (iOS native), 2.4.0 (macOS C++).
    /// Description: When the custom video pre-processing is turned on, the [CVPixelBuffer] format video data after the custom video pre-processing is sent to the SDK, and other channels are supported.
    /// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
    /// When to call: Must be called in the [onCapturedUnprocessedCVPixelbuffer] callback.
    /// Restrictions: This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video pre-processing and the bufferType of config is passed in [ZegoVideoBufferTypeCVPixelBuffer] or [ZegoVideoBufferTypeNV12CVPixelBuffer].
    /// Platform differences: Only valid on Windows platform.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param buffer CVPixelBuffer type video frame data to be sent to the SDK.
    /// @param timestamp Timestamp of this video frame.
    /// @param channel Publishing stream channel.
    virtual void sendCustomVideoProcessedCVPixelBuffer(
        void *buffer, unsigned long long timestamp,
        ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Send the [CVPixelBuffer] type video data after the custom video processing to the SDK (for the specified channel), and support specified data usage.
    ///
    /// Available since: 3.14.0 (iOS native, macOS C++).
    /// Description: When the custom video pre-processing is turned on, the [CVPixelBuffer] format video data after the custom video pre-processing is sent to the SDK, and other channels are supported.
    /// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
    /// When to call: Must be called in the [onCapturedUnprocessedCVPixelbuffer] callback.
    /// Restrictions: This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video pre-processing and the bufferType of config is passed in [ZegoVideoBufferTypeCVPixelBuffer] or [ZegoVideoBufferTypeNV12CVPixelBuffer].
    /// Platform differences: Only valid on Windows platform.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param buffer CVPixelBuffer type video frame data to be sent to the SDK.
    /// @param timestamp Timestamp of this video frame.
    /// @param usage Data usage
    /// @param channel Publishing stream channel.
    virtual void sendCustomVideoProcessedCVPixelBuffer(
        void *buffer, unsigned long long timestamp, ZegoProcessedDataUsageType usage,
        ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Enable local collection and custom audio processing(before ear return).
    ///
    /// Available since: 1.13.0
    /// Description: Enable custom audio processing(before ear return), developers can receive locally collected audio frames through [onProcessCapturedAudioData], and can modify the audio data.
    /// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing after the audio data is collected or before the remote audio data is drawn for rendering.
    /// When to call: It needs to be called before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager] to be effective.
    /// Restrictions: None.
    ///
    /// @param enable Whether to enable local capture custom audio processing.
    /// @param config Custom audio processing configuration.
    virtual void enableCustomAudioCaptureProcessing(bool enable,
                                                    ZegoCustomAudioProcessConfig *config) = 0;

    /// Turn on local collection and custom audio processing (after ear return).
    ///
    /// Available since: 1.13.0
    /// Description: Enable custom audio processing(after ear return), developers can receive locally collected audio frames through [onProcessCapturedAudioData], and can modify the audio data.
    /// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing after the audio data is collected or before the remote audio data is drawn for rendering.
    /// When to call: It needs to be called before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager] to be effective.
    /// Restrictions: None.
    ///
    /// @param enable Whether to enable local capture custom audio processing.
    /// @param config Custom audio processing configuration.
    virtual void enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(
        bool enable, ZegoCustomAudioProcessConfig *config) = 0;

    /// Enable the feature of throwing audio data before SDK internal audio preprocessing.
    ///
    /// Available since: 3.11.0
    /// Description: Enables the feature of throwing audio data before SDK internal audio preprocessing, and developers can receive audio data before SDK internal audio preprocessing through [onBeforeAudioPrepAudioData].
    /// When to call: It needs to be called after [createEngine] to be effective.
    /// Restrictions: None.
    ///
    /// @param enable Whether to enable feature of throwing audio data before SDK internal audio preprocessing.
    /// @param param param of audio frame. Currently, it supports sampling rates of 0, 16k, 32k, 44.1k, and 48k. The 0 means using SDK internal value. It supports channels Unknown, Mono, and Stereo. The Unknown means using SDK internal value.
    virtual void enableBeforeAudioPrepAudioData(bool enable, ZegoAudioFrameParam param) = 0;

    /// Enable custom audio processing for remote playing stream.
    ///
    /// Available since: 1.13.0
    /// Description: Enable remote streaming custom audio processing, developers can receive remote streaming audio frames through [onProcessRemoteAudioData], and can modify the audio data.
    /// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing before pulling the remote audio data for rendering.
    /// When to call: It needs to be called before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager] to be effective.
    /// Restrictions: None.
    ///
    /// @param enable Whether to enable custom audio processing for remote playing stream.
    /// @param config Custom audio processing configuration.
    virtual void enableCustomAudioRemoteProcessing(bool enable,
                                                   ZegoCustomAudioProcessConfig *config) = 0;

    /// Enable custom audio processing for SDK playback audio data.
    ///
    /// Available since: 1.13.0
    /// Description: Enable remote streaming custom audio processing, developers can receive remote streaming audio frames through [onProcessPlaybackAudioData], and can modify the audio data.
    /// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing after collecting audio data.
    /// When to call: It needs to be called before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager] to be effective.
    /// Restrictions: None.
    ///
    /// @param enable Whether to enable custom audio processing for SDK playback audio data.
    /// @param config Custom audio processing configuration.
    virtual void enableCustomAudioPlaybackProcessing(bool enable,
                                                     ZegoCustomAudioProcessConfig *config) = 0;

    /// Set up callback handler for custom audio processing.
    ///
    /// Available since: 1.13.0
    /// Description: When the custom audio processing is enabled, the custom audio processing callback is set through this function, and the developer can modify the processed audio frame data in the callback.
    /// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing after the audio data is collected or before the remote audio data is drawn for rendering.
    /// When to call: After creating the engine.
    /// Restrictions: None.
    ///
    /// @param handler Callback handler for custom audio processing.
    virtual void
    setCustomAudioProcessHandler(std::shared_ptr<IZegoCustomAudioProcessHandler> handler) = 0;

    /// Enable audio data observering.
    ///
    /// Available since: 1.1.0
    /// Description: When custom audio processing is turned on, use this function to turn on audio data callback monitoring.
    /// Use cases: When develop need to monitor the original audio.
    /// When to call: After creating the engine.
    /// Restrictions: Audio monitoring is triggered only after this function has been called and the callback has been set by calling [setAudioDataHandler]. If you want to enable the [onPlayerAudioData] callback, you must also be in the pull stream, and the incoming sampling rate of the [startAudioDataObserver] function is not supported at 8000Hz, 22050Hz, and 24000Hz.
    /// Caution: This api will start the media engine and occupy the microphone device.
    ///
    /// @param observerBitMask The callback function bitmask marker for receive audio data, refer to enum [ZegoAudioDataCallbackBitMask], when this param converted to binary, 0b01 that means 1 << 0 for triggering [onCapturedAudioData], 0x10 that means 1 << 1 for triggering [onPlaybackAudioData], 0x100 that means 1 << 2 for triggering [onMixedAudioData], 0x1000 that means 1 << 3 for triggering [onPlayerAudioData]. The masks can be combined to allow different callbacks to be triggered simultaneously.
    /// @param param param of audio frame.
    virtual void startAudioDataObserver(unsigned int observerBitMask,
                                        ZegoAudioFrameParam param) = 0;

    /// Disable audio data observering.
    ///
    /// Available since: 1.1.0
    /// Description: Disable audio data observering.
    /// Use cases: When develop need to monitor the original audio.
    /// When to call: After calling [startAudioDataObserver] to start audio data monitoring.
    virtual void stopAudioDataObserver() = 0;

    /// Sets up the event callback handler for receiving audio data.
    ///
    /// Available since: 1.1.0
    /// Description: This function can be called to receive audio data thrown by SDK bypass.
    /// Use cases: When the developer needs to obtain the audio data of the remote user or the data collected by the local microphone for other purposes (such as pure audio recording, pure audio third-party monitoring, pure audio real-time analysis).
    /// When to call: After creating the engine.
    /// Restrictions: The set callback needs to be effective after calling [startAudioDataObserver] and is in the push or pull state.
    /// Related APIs: Turn on the audio data monitoring call [startAudioDataObserver], turn off the audio data monitoring call [stopAudioDataObserver].
    /// Caution: The format of the thrown audio data is pcm. The SDK still controls the collection and playback of the sound device. This function is to copy a copy of the data collected or played inside the SDK for use by the developer.
    ///
    /// @param handler Audio data handler for receive audio data.
    virtual void setAudioDataHandler(std::shared_ptr<IZegoAudioDataHandler> handler) = 0;

    /// Enables the custom audio I/O function (for the specified channel), support PCM, AAC format data.
    ///
    /// Available since: 1.10.0
    /// Description: Enable custom audio IO function, support PCM, AAC format data.
    /// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing after the audio data is collected or before the remote audio data is drawn for rendering.
    /// When to call: It needs to be called before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager] to be effective.
    /// Restrictions: None.
    ///
    /// @param enable Whether to enable custom audio IO, default is false.
    /// @param config Custom audio IO config.
    /// @param channel Specify the publish channel to enable custom audio IO.
    virtual void enableCustomAudioIO(bool enable, ZegoCustomAudioConfig *config,
                                     ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Sends AAC audio data produced by custom audio capture to the SDK (for the specified channel).
    ///
    /// Available since: 2.20.0
    /// Description: Sends the captured audio AAC data to the SDK.
    /// Use cases: The customer needs to obtain input after acquisition from the existing audio stream, audio file, or customized acquisition system, and hand it over to the SDK for transmission.
    /// When to call: After [enableCustomAudioIO] and publishing stream successfully.
    /// Restrictions: None.
    /// Related APIs: Enable the custom audio IO function [enableCustomAudioIO], and start the push stream [startPublishingStream].
    ///
    /// @param data AAC buffer data.
    /// @param dataLength The total length of the buffer data.
    /// @param configLength The length of AAC specific config (Note: The AAC encoded data length is 'encodedLength = dataLength - configLength').Value range: [0,64]
    /// @param referenceTimeMillisecond The UNIX timestamp of this AAC audio frame in millisecond.
    /// @param samples The number of samples for this AAC audio frame.Value range: [480,512,1024,1960,2048].
    /// @param param The param of this AAC audio frame.
    /// @param channel Publish channel for capturing audio frames.
    virtual void sendCustomAudioCaptureAACData(
        unsigned char *data, unsigned int dataLength, unsigned int configLength,
        unsigned long long referenceTimeMillisecond, unsigned int samples,
        ZegoAudioFrameParam param, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Sends PCM audio data produced by custom audio capture to the SDK (for the specified channel).
    ///
    /// Available since: 1.10.0
    /// Description: Sends the captured audio PCM data to the SDK.
    /// Use cases: 1.The customer needs to obtain input after acquisition from the existing audio stream, audio file, or customized acquisition system, and hand it over to the SDK for transmission. 2.Customers have their own requirements for special sound processing for PCM input sources. After the sound processing, the input will be sent to the SDK for transmission.
    /// When to call: After [enableCustomAudioIO] and publishing stream successfully.
    /// Restrictions: None.
    /// Related APIs: Enable the custom audio IO function [enableCustomAudioIO], and start the push stream [startPublishingStream].
    ///
    /// @param data PCM buffer data.
    /// @param dataLength The total length of the buffer data.
    /// @param param The param of this PCM audio frame.
    /// @param channel Publish channel for capturing audio frames.
    virtual void
    sendCustomAudioCapturePCMData(unsigned char *data, unsigned int dataLength,
                                  ZegoAudioFrameParam param,
                                  ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

    /// Fetches PCM audio data of the remote stream from the SDK for custom audio rendering.
    ///
    /// Available since: 1.10.0
    /// Description: Fetches PCM audio data of the remote stream from the SDK for custom audio rendering, it is recommended to use the system framework to periodically invoke this function to drive audio data rendering.
    /// Use cases: When developers have their own rendering requirements, such as special applications or processing and rendering of the original PCM data that are pulled, it is recommended to use the custom audio rendering function of the SDK.
    /// When to call: After [enableCustomAudioIO] and playing stream successfully.
    /// Restrictions: None.
    /// Related APIs: Enable the custom audio IO function [enableCustomAudioIO], and start the play stream [startPlayingStream].
    ///
    /// @param data A block of memory for storing audio PCM data that requires user to manage the memory block's lifecycle, the SDK will copy the audio frame rendering data to this memory block.
    /// @param dataLength The length of the audio data to be fetch this time (dataLength = duration * sampleRate * channels * 2(16 bit depth i.e. 2 Btye)).
    /// @param param Specify the parameters of the fetched audio frame. sampleRate in ZegoAudioFrameParam must assignment
    virtual void fetchCustomAudioRenderPCMData(unsigned char *data, unsigned int dataLength,
                                               ZegoAudioFrameParam param) = 0;

    /// Send the PCM audio data customized by the developer to the SDK, which is used as a reference for custom rendering audio to eliminate echo.
    ///
    /// Available since：2.13.0.
    /// Description：Developers use the audio device clock as the driver to capture PCM audio data, and use it for custom audio rendering after processing. When submitting for rendering, call this function to send the processed audio data back to the SDK so that the SDK can use it as an echo cancellation reference.
    /// Use cases：In entertainment scenarios, it may be necessary to customize the processing of PCM audio data from the remote end, such as synthesizing a background sound and KTV accompaniment before rendering and playing. At the same time, developers are required to send the audio data processed by themselves to the SDK for reference, so that the processed sound effects can be echo canceled after collection.
    /// When to call：After calling [fetchCustomAudioRenderPCMData] to fetch and process the PCM audio data, this function is called while submitting to the system for rendering and playback.
    /// Restrictions：You must call [setEngineConfig] to enable the external audio data as a reference for this function to take effect. If you need to get the use of the function or the details, please consult ZEGO technical support.
    ///
    /// @param data PCM buffer data
    /// @param dataLength The total length of the buffer data
    /// @param param The param of this PCM audio frame
    virtual void sendReferenceAudioPCMData(unsigned char *data, unsigned int dataLength,
                                           ZegoAudioFrameParam param) = 0;

    /// Creates a range audio instance.
    ///
    /// Available since: 2.11.0
    /// Description: Creates a range audio instance.
    /// Use case: Often used in game voice scenarios, users can use related functions by creating range audio instance objects.
    /// When to call: It can be called after the engine by [createEngine] has been initialized.
    /// Restrictions: Currently, a maximum of 1 instances can be created, after which it will return nullptr.
    /// Impacts on other APIs: If you use the range audio module, you cannot use the basic push-pull stream [startPublishingStream], [startPlayingStream] interfaces and related callbacks.
    ///
    /// @return range audio instance, nullptr will be returned when the maximum number is exceeded.
    virtual IZegoRangeAudio *createRangeAudio() = 0;

    /// Destroys a range audio instance.
    ///
    /// Available since: 2.11.0
    /// Description: Destroys a range audio instance.
    ///
    /// @param rangeAudio The range audio instance object to be destroyed.
    virtual void destroyRangeAudio(IZegoRangeAudio *&rangeAudio) = 0;

    /// Creates a copyrighted music instance.
    ///
    /// Available since: 2.13.0
    /// Description: Creates a copyrighted music instance.
    /// Use case: Often used in online KTV chorus scenarios, users can use related functions by creating copyrighted music instance objects.
    /// When to call: It can be called after the engine by [createEngine] has been initialized.
    /// Restrictions: The SDK only supports the creation of one instance of CopyrightedMusic. Multiple calls to this function return the same object.
    ///
    /// @return copyrighted music instance, multiple calls to this function return the same object.
    virtual IZegoCopyrightedMusic *createCopyrightedMusic() = 0;

    /// Destroys a copyrighted music instance.
    ///
    /// Available since: 2.13.0
    /// Description: Destroys a copyrighted music instance.
    /// When to call: It can be called before the engine by [destroyEngine]
    ///
    /// @param copyrightedMusic The copyrighted music instance object to be destroyed.
    virtual void destroyCopyrightedMusic(IZegoCopyrightedMusic *&copyrightedMusic) = 0;

    /// Create a range scene instance object.
    ///
    /// Available since: 3.0.0
    /// Description: Create a range scene instance object.
    /// Use case: Often used in meta world scenarios, users can use related functions by range scene instance objects.
    /// When to call: After initializing the engine by [createEngine].
    /// Restrictions: None.
    ///
    /// @return range scene instance object.
    virtual IZegoRangeScene *createRangeScene() = 0;

    /// Destroy a range scene instance object.
    ///
    /// Available since: 3.0.0
    /// Description: Destroy a range scene instance.
    /// When to call: After create the range scene by [createRangeScene].
    /// Restrictions: None.
    ///
    /// @param rangeScene The range scene instance object to be destroyed.
    virtual void destroyRangeScene(IZegoRangeScene *&rangeScene) = 0;

    /// Get a list of screens or windows in a screen.
    ///
    /// Available since: 3.1.0
    /// Description: Get a list of screens or windows in a screen.
    /// Restrictions: Only support in Windows/macOS.
    ///
    /// @param thumbnailWidth Get the thumbnail width corresponding to the window, the thumbnail can be used to draw on the window selection interface. (unit is pixel)
    /// @param thumbnailHeight Get the thumbnail height corresponding to the window, the thumbnail can be used to draw on the window selection interface. (unit is pixel)
    /// @param iconWidth Get the width of the icon corresponding to the program. (unit is pixel)
    /// @param iconHeight Get the height of the icon corresponding to the program. (unit is pixel)
    /// @return List of capture source info objects.
    virtual std::vector<ZegoScreenCaptureSourceInfo> getScreenCaptureSources(int thumbnailWidth,
                                                                             int thumbnailHeight,
                                                                             int iconWidth,
                                                                             int iconHeight) = 0;

    /// Release screen capture source list
    ///
    /// Description: Release screen capture source list
    /// Platform differences: Only for Windows / macOS / Linux
    /// When to call: After calling [getScreenCaptureSources] to obtain the collection source, this interface needs to be called to release it
    ///
    /// @param sourceList Screen capture source list
    virtual void freeScreenCaptureSources(std::vector<ZegoScreenCaptureSourceInfo> &sourceList) = 0;

    /// Create the screen capture source
    ///
    /// Available: since 3.1.0
    /// Description: Creates a screen capture source object based on the provided source ID and source type.
    /// Use cases: It is used when you need to record and share the screen or window.
    /// When to call: It needs to be called after [createEngine].
    /// Platform differences: Only supports Windows and macOS.
    ///
    /// @param sourceId The specified screen ID or window ID.
    /// @param sourceType The specified screen source type.
    /// @return The screen capture instance, nullptr will be returned when the maximum number is exceeded.
    virtual IZegoScreenCaptureSource *
    createScreenCaptureSource(void *sourceId, ZegoScreenCaptureSourceType sourceType) = 0;

    /// Destroy the screen capture source instance
    ///
    /// Available: since 3.1.0
    /// Description: Destroy the [ZegoScreenCaptureSource] instance object.
    /// Use cases: When you no longer need to use the screen capture function, you can use this function to destroy the instance object created by the [createScreenCaptureSource] function
    /// When to call: When you need to the screen capture source object needs to be destroyed
    /// Restrictions: After destroy the instance, you need to release the [ZegoScreenCaptureSource] instance object you hold by yourself, and don’t call the function of this instance object after the destruction.
    /// Platform differences: Only supports Windows and macOS.
    ///
    /// @param source The screen capture source instance to be destroyed.
    virtual void destroyScreenCaptureSource(IZegoScreenCaptureSource *&source) = 0;

    /// Creates an AI voice changer instance.
    ///
    /// Available since: 3.10.0
    /// Description: Creates an AI voice changer instance.
    /// Use case: Often used in live broadcasting, voice chatroom and KTV.
    /// When to call: It can be called after the SDK by [createEngine] has been initialized.
    /// Restrictions: Currently, a maximum of 1 instance can be created, after which it will return nullptr.
    /// Related APIs: User can call [destroyAIVoiceChanger] function to destroy an AI voice changer instance.
    ///
    /// @return AI voice changer instance.
    virtual IZegoAIVoiceChanger *createAIVoiceChanger() = 0;

    /// Destroys an AI voice changer instance.
    ///
    /// Available since: 3.10.0
    /// Description: Destroys the AI voice changer instance.
    /// Related APIs: User can call [createAIVoiceChanger] function to create an AI voice changer instance.
    ///
    /// @param aiVoiceChanger The AI voice changer instance object to be destroyed.
    virtual void destroyAIVoiceChanger(IZegoAIVoiceChanger *&aiVoiceChanger) = 0;

    /// Check the device can run AI voice changer or not.
    ///
    /// Available since: 3.14.0
    /// Description: Check the device can run AI voice changer or not.
    /// Use case: Often used in live broadcasting, voice chatroom and KTV.
    /// When to call: It can be called after the SDK by [createEngine] has been initialized.
    ///
    /// @return Return true if the device can run AI voice changer, otherwise return false.
    virtual bool isAIVoiceChangerSupported() = 0;
};

class IZegoRealTimeSequentialDataManager {
  protected:
    virtual ~IZegoRealTimeSequentialDataManager() {}

  public:
    /// Sets up the real-time sequential data event handler.
    ///
    /// Available since: 2.14.0
    /// Description: Set up real-time sequential data callback to monitor callbacks such as sending data results， receiving data, etc.
    /// When to call:After create the [ZegoRealTimeSequentialDataManager] instance.
    /// Restrictions: None.
    /// Caution: Calling this function will overwrite the callback set by the last call to this function.
    ///
    /// @param handler Event handler for real-time sequential data
    virtual void
    setEventHandler(std::shared_ptr<IZegoRealTimeSequentialDataEventHandler> handler) = 0;

    /// Start broadcasting real-time sequential data stream.
    ///
    /// Available since: 2.14.0
    /// Description: This function allows users to broadcast their local real-time sequential data stream to the ZEGO RTC server, and other users in the same room can subscribe to the real-time sequential data stream for intercommunication through "streamID".
    /// Use cases: Before sending real-time sequential data, you need to call this function to start broadcasting.
    /// When to call: After creating the [ZegoRealTimeSequentialDataManager] instance.
    /// Restrictions: None.
    /// Caution: After calling this function, you will receive the [onPublisherStateUpdate] callback to tell you the broadcast state (publish state) of this stream. After the broadcast is successful, other users in the same room will receive the [onRoomStreamUpdate] callback to tell them this stream has been added to the room.
    ///
    /// @param streamID Stream ID, a string of up to 256 characters.
    ///   Caution:
    ///   1. Need to be globally unique within the entire AppID (Note that it cannot be the same as the stream ID passed in [startPublishingStream]). If in the same AppID, different users publish each stream and the stream ID is the same, which will cause the user to publish the stream failure. You cannot include URL keywords, otherwise publishing stream and playing stream will fails.
    ///   2. Only support numbers, English characters and '-', '_'.
    virtual void startBroadcasting(const std::string &streamID) = 0;

    /// Stop broadcasting real-time sequential data stream.
    ///
    /// Available since: 2.14.0
    /// Description: This function allows users to stop broadcasting their local real-time sequential data stream.
    /// Use cases: When you no longer need to send real-time sequential data, you need to call this function to stop broadcasting.
    /// When to call: After creating the [ZegoRealTimeSequentialDataManager] instance.
    /// Restrictions: None.
    /// Caution: After calling this function, you will receive the [onPublisherStateUpdate] callback to tell you the broadcast state (publish state) of this stream. After stopping the broadcast, other users in the same room will receive the [onRoomStreamUpdate] callback to tell them this stream has been deleted from the room.
    ///
    /// @param streamID The ID of the stream that needs to stop broadcasting.
    virtual void stopBroadcasting(const std::string &streamID) = 0;

    /// Send real-time sequential data to the broadcasting stream ID.
    ///
    /// Available since: 2.14.0
    /// Description: This function can be used to send real-time sequential data on the stream currently being broadcast.
    /// Use cases: You need to call this function when you need to send real-time sequential data.
    /// When to call: After calling [startBroadcasting].
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param data The real-time sequential data to be sent.
    /// @param dataLength Data content length.
    /// @param streamID The stream ID to which the real-time sequential data is sent.
    /// @param callback Send real-time sequential data result callback.
    virtual void sendRealTimeSequentialData(const unsigned char *data, unsigned int dataLength,
                                            const std::string &streamID,
                                            ZegoRealTimeSequentialDataSentCallback callback) = 0;

    /// Start subscribing real-time sequential data stream.
    ///
    /// Available since: 2.14.0
    /// Description: This function allows users to subscribe to the real-time sequential data stream of remote users from the ZEGO RTC server.
    /// Use cases: When you need to receive real-time sequential data sent from other remote users, you need to call this function to start subscribing to the stream broadcasted by other remote users.
    /// When to call: After creating the [ZegoRealTimeSequentialDataManager] instance.
    /// Restrictions: None.
    /// Caution: After calling this function, you will receive the [onPlayerStateUpdate] callback to tell you the subscribe state (play state) of this stream.
    ///
    /// @param streamID Stream ID, a string of up to 256 characters.
    ///   Caution:
    ///   Only support numbers, English characters and '-', '_'.
    virtual void startSubscribing(const std::string &streamID) = 0;

    /// Stop subscribing real-time sequential data stream.
    ///
    /// Available since: 2.14.0
    /// Description: This function can be used to stop subscribing to the real-time sequential data stream.
    /// Use cases: When you no longer need to receive real-time sequential data sent by other users, you need to call this function to stop subscribing to the other user's stream.
    /// When to call: After creating the [ZegoRealTimeSequentialDataManager] instance.
    /// Restrictions: None.
    /// Caution: After calling this function, you will receive the [onPlayerStateUpdate] callback to tell you the subscribe state (play state) of this stream.
    ///
    /// @param streamID The ID of the stream that needs to stop subscribing.
    virtual void stopSubscribing(const std::string &streamID) = 0;

    /// Get real-time sequential data manager index.
    ///
    /// @return Index of the real-time sequential data manager.
    virtual int getIndex() = 0;
};

class IZegoMediaPlayer {
  protected:
    virtual ~IZegoMediaPlayer() {}

  public:
    /// Set event notification callback handler of the media player.
    ///
    /// Available since: 2.1.0
    /// Description: Listen to the event notification callback of the media player.
    /// Use Cases: You can change the media player UI widget according to the related event callback.
    /// When to call: After the [ZegoMediaPlayer] instance created.
    /// Restrictions: None.
    /// Caution: Calling this function will overwrite the callback set by the last call to this function.
    ///
    /// @param handler Event callback handler for media player
    virtual void setEventHandler(std::shared_ptr<IZegoMediaPlayerEventHandler> handler) = 0;

    /// Set video data callback handler of the media player.
    ///
    /// Available since: 2.1.0
    /// Description: By setting this callback, the video data of the media resource file played by the media player can be called back.
    /// When to call: After the [ZegoMediaPlayer] instance created.
    /// Restrictions: None.
    /// Caution: When you no longer need to get the video frame data, please call this function again to clear the handler to stop the video frame data callback.
    ///
    /// @param handler Video data callback handler for media player
    /// @param format Video frame format for video data
    virtual void setVideoHandler(std::shared_ptr<IZegoMediaPlayerVideoHandler> handler,
                                 ZegoVideoFrameFormat format) = 0;

    /// Set audio data callback handler of the media player.
    ///
    /// Available since: 2.1.0
    /// Description: By setting this callback, the audio data of the media resource file played by the media player can be called back.
    /// When to call: After the [ZegoMediaPlayer] instance created.
    /// Restrictions: None.
    /// Caution: When you no longer need to get the audio data, please call this function again to clear the handler to stop the audio data callback.
    ///
    /// @param handler Audio data callback handler for media player
    virtual void setAudioHandler(std::shared_ptr<IZegoMediaPlayerAudioHandler> handler) = 0;

    /// Set the media resource block data callback handler of the media player.
    ///
    /// Available since: 3.4.0
    /// Description: This callback can be set to listen to and decrypt the media resource block data for playing the user's own ciphertext media resources.
    /// When to call: After the [ZegoMediaPlayer] instance created, before playing media resources.
    /// Restrictions: None.
    /// Caution: When it is no longer necessary to listen to the callback for data decryption, please call this function again to clear the handler.
    ///
    /// @param handler The media resource block data callback handler of the media player.
    /// @param blockSize The size of the encrypted data block. The bufferSize in the OnBlockData callback is an integer multiple of blockSize.
    virtual void setBlockDataHandler(std::shared_ptr<IZegoMediaPlayerBlockDataHandler> handler,
                                     unsigned int blockSize) = 0;

    /// Load local or network media resource.
    ///
    /// Available: since 1.3.4
    /// Description: Load media resources.
    /// Use case: Developers can load the absolute path to the local resource or the URL of the network resource incoming.
    /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
    /// Related APIs: Resources can be loaded through the [loadResourceWithPosition] or [loadResourceFromMediaData] function.
    /// Caution: If the mediaplayer has already loaded resources or is in the process of playing, please first call the [stop] interface to halt the playback, and then proceed to call the interface to load the media resources; failure to do so will result in an unsuccessful load.
    ///
    /// @param path The absolute resource path or the URL of the network resource and cannot be nullptr or "". Android can set this path string with Uri.
    /// @param callback Notification of resource loading results
    virtual void loadResource(const std::string &path,
                              ZegoMediaPlayerLoadResourceCallback callback) = 0;

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
    /// @param path The absolute resource path or the URL of the network resource and cannot be nullptr or "". Android can set this path string with Uri.
    /// @param startPosition The progress at which the playback started.
    /// @param callback Notification of resource loading results
    virtual void loadResourceWithPosition(const std::string &path, unsigned long long startPosition,
                                          ZegoMediaPlayerLoadResourceCallback callback) = 0;

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
    /// @param mediaData Binary audio data.
    /// @param mediaDataLength The length of the binary audio data.
    /// @param startPosition Position of starting playback, in milliseconds.
    /// @param callback Notification of resource loading results.
    virtual void loadResourceFromMediaData(unsigned char *mediaData, int mediaDataLength,
                                           unsigned long long startPosition,
                                           ZegoMediaPlayerLoadResourceCallback callback) = 0;

    /// Load copyrighted music resource.
    ///
    /// Available: since 2.14.0
    /// Description: Load media resources, and specify the progress, in milliseconds, at which playback begins.
    /// Use case: Developers can load the resource ID of copyrighted music.
    /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
    /// Caution: 1.When [startPosition] exceeds the total playing time, it will start playing from the beginning.
    ///  2.If the mediaplayer has already loaded resources or is in the process of playing, please first call the [stop] interface to halt the playback, and then proceed to call the interface to load the media resources; failure to do so will result in an unsuccessful load.
    ///
    /// @param resourceID The resource ID obtained from the copyrighted music module.
    /// @param startPosition The progress at which the playback started.
    /// @param callback Notification of resource loading results
    virtual void
    loadCopyrightedMusicResourceWithPosition(const std::string &resourceID,
                                             unsigned long long startPosition,
                                             ZegoMediaPlayerLoadResourceCallback callback) = 0;

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
    /// @param callback Notification of resource loading results
    virtual void loadResourceWithConfig(ZegoMediaPlayerResource resource,
                                        ZegoMediaPlayerLoadResourceCallback callback) = 0;

    /// Start playing.
    ///
    /// You need to load resources before playing
    virtual void start() = 0;

    /// Stop playing.
    virtual void stop() = 0;

    /// Pause playing.
    virtual void pause() = 0;

    /// Resume playing.
    virtual void resume() = 0;

    /// Set the specified playback progress.
    ///
    /// Unit is millisecond
    ///
    /// @param millisecond Point in time of specified playback progress
    /// @param callback The result notification of set the specified playback progress
    virtual void seekTo(unsigned long long millisecond, ZegoMediaPlayerSeekToCallback callback) = 0;

    /// Whether to repeat playback.
    ///
    /// @param enable repeat playback flag. The default is false.
    virtual void enableRepeat(bool enable) = 0;

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
    virtual void setPlayLoopCount(int count) = 0;

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
    virtual void setPlaySpeed(float speed) = 0;

    /// Whether to mix the player's sound into the stream being published.
    ///
    /// This interface will only mix the media player sound into the main channel
    ///
    /// @param enable Aux audio flag. The default is false.
    virtual void enableAux(bool enable) = 0;

    /// Whether to play locally silently.
    ///
    /// If [enableAux] switch is turned on, there is still sound in the publishing stream. The default is false.
    ///
    /// @param mute Mute local audio flag, The default is false.
    virtual void muteLocal(bool mute) = 0;

    /// Set the view of the player playing video.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param canvas Video rendered canvas object
    virtual void setPlayerCanvas(ZegoCanvas *canvas) = 0;

    /// Set mediaplayer volume. Both the local play volume and the publish volume are set.
    ///
    /// @param volume The range is 0 ~ 200. The default is 60.
    virtual void setVolume(int volume) = 0;

    /// Set mediaplayer local playback volume.
    ///
    /// @param volume The range is 0 ~ 200. The default is 60.
    virtual void setPlayVolume(int volume) = 0;

    /// Set mediaplayer publish volume.
    ///
    /// @param volume The range is 0 ~ 200. The default is 60.
    virtual void setPublishVolume(int volume) = 0;

    /// Set playback progress callback interval.
    ///
    /// This function can control the callback frequency of [onMediaPlayerPlayingProgress]. When the callback interval is set to 0, the callback is stopped. The default callback interval is 1s
    /// This callback are not returned exactly at the set callback interval, but rather at the frequency at which the audio or video frames are processed to determine whether the callback is needed to call
    ///
    /// @param millisecond Interval of playback progress callback in milliseconds
    virtual void setProgressInterval(unsigned long long millisecond) = 0;

    /// Gets the current local playback volume of the mediaplayer, the range is 0 ~ 200, with the default value of 60.
    ///
    /// @return current volume
    virtual int getPlayVolume() = 0;

    /// Gets the current publish volume of the mediaplayer, the range is 0 ~ 200, with the default value of 60.
    ///
    /// @return current volume
    virtual int getPublishVolume() = 0;

    /// Get the total progress of your media resources.
    ///
    /// You should load resource before invoking this function, otherwise the return value is 0
    ///
    /// @return Unit is millisecond
    virtual unsigned long long getTotalDuration() = 0;

    /// Get current playing progress.
    ///
    /// You should load resource before invoking this function, otherwise the return value is 0
    ///
    /// @return current progress
    virtual unsigned long long getCurrentProgress() = 0;

    /// Get current rendering progress.
    ///
    /// You should load resource before invoking this function, otherwise the return value is 0
    ///
    /// @return current rendering progress
    virtual unsigned long long getCurrentRenderingProgress() = 0;

    /// Get the number of audio tracks of the playback file.
    ///
    /// @return Number of audio tracks
    virtual unsigned int getAudioTrackCount() = 0;

    /// Set the audio track of the playback file.
    ///
    /// @param index Audio track index, the number of audio tracks can be obtained through the [getAudioTrackCount] function.
    virtual void setAudioTrackIndex(unsigned int index) = 0;

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
    virtual void setAudioTrackMode(ZegoMediaPlayerAudioTrackMode mode) = 0;

    /// Set the audio track for the media file to be publish.
    ///
    /// Available since: 3.1.0
    /// Description: Set the audio track for the media file to be publish.
    /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
    /// Related APIs: The number of audio tracks can be obtained through the [getAudioTrackCount] function.
    /// Caution: This call takes effect only after multitrack mode is enabled by calling the interface [setAudioTrackMode].
    ///
    /// @param index Audio track index, the number of audio tracks can be obtained through the [getAudioTrackCount] function.
    virtual void setAudioTrackPublishIndex(unsigned int index) = 0;

    /// Enable voice changer, set up the specific voice changer parameters.
    ///
    /// Available since: 3.15.0
    /// Description: Enable voice changer, set up the specific voice changer parameters.
    /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
    ///
    /// @param audioChannel The audio channel to be voice changed
    /// @param enable Whether enable voice changer or not. True - enabled, false - disabled, default value is false.
    /// @param param Voice changer parameters
    virtual void enableVoiceChanger(ZegoMediaPlayerAudioChannel audioChannel, bool enable,
                                    ZegoVoiceChangerParam param) = 0;

    /// Get the current playback status.
    ///
    /// @return current state
    virtual ZegoMediaPlayerState getCurrentState() = 0;

    /// Get media player index.
    ///
    /// Description: Get media player index.
    /// When to call: It can be called after [createMediaPlayer].
    /// Restrictions: None.
    ///
    /// @return Media player index.
    virtual int getIndex() = 0;

    /// Take a screenshot of the current playing screen of the media player.
    ///
    /// Only in the case of calling [setPlayerCanvas] to set the display controls and the playback state, can the screenshot be taken normally
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param callback The callback of the screenshot of the media player playing screen
    virtual void takeSnapshot(ZegoMediaPlayerTakeSnapshotCallback callback) = 0;

    /// Enable accurate seek and set relevant attributes.
    ///
    /// Available since: 2.4.0
    /// Description: The timestamp specified by normal seek may not an I frame, and then returns the I frame near the specified timestamp, which is not so accurate. But the accurate seek, when the specified timestamp is not an I frame, it will use the I frame near the specified timestamp to decode the frame of the specified timestamp.
    /// Use cases: When user needs to seek to the specified timestamp accurately.
    /// When to call: The setting must be called before [loadResource], and it will take effect during the entire life cycle of the media player.
    ///
    /// @param enable Whether to enable accurate seek
    /// @param config The property setting of accurate seek, only valid when enable is true.
    virtual void enableAccurateSeek(bool enable, ZegoAccurateSeekConfig *config) = 0;

    /// Set the maximum cache duration and cache data size of web materials.
    ///
    /// The setting must be called before loading the resource, and it will take effect during the entire life cycle of the media player.
    /// Time and size are not allowed to be 0 at the same time. The SDK internal default time is 5000, and the size is 15*1024*1024 byte.When one of time and size reaches the set value first, the cache will stop.
    ///
    /// @param time The maximum length of the cache time, in ms, the SDK internal default is 5000; the effective value is greater than or equal to 2000; if you fill in 0, it means no limit.
    /// @param size The maximum size of the cache, the unit is byte, the internal default size of the SDK is 15*1024*1024 byte; the effective value is greater than or equal to 5000000, if you fill in 0, it means no limit.
    virtual void setNetWorkResourceMaxCache(unsigned int time, unsigned int size) = 0;

    /// Get the playable duration and size of the cached data of the current network material cache queue
    ///
    /// @return Returns the current cached information, including the length of time the data can be played and the size of the cached data.
    virtual ZegoNetWorkResourceCache *getNetWorkResourceCache() = 0;

    /// Use this interface to set the cache threshold that the media player needs to resume playback. The SDK default value is 5000ms，The valid value is greater than or equal to 1000ms
    ///
    /// The setting must be called before loading the resource, and it will take effect during the entire life cycle of the media player.
    /// When the network status is poor and the media player has finished playing the cached network resources, it will stop playing, and notify the user through the `ZegoMediaPlayerNetworkEvent.BUFFER_BEGIN` state of the callback interface `onMediaPlayerNetworkEvent` that the network resources are being recached.
    /// Only when the cached network resources are greater than the set threshold, the media player will automatically resume playback at the original paused position, and notify the user through the `ZegoMediaPlayerNetworkEvent.BUFFER_ENDED` of the callback interface `onMediaPlayerNetworkEvent` that the user has cached the network resources The threshold was reached and playback resumed.
    ///
    /// @param threshold Threshold that needs to be reached to resume playback, unit ms.
    virtual void setNetWorkBufferThreshold(unsigned int threshold) = 0;

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
    virtual void enableSoundLevelMonitor(bool enable, unsigned int millisecond) = 0;

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
    virtual void enableFrequencySpectrumMonitor(bool enable, unsigned int millisecond) = 0;

    /// Set the playback channel.
    ///
    /// Available since: 2.20.0
    /// Description: Set the playback channel.
    /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
    /// Restrictions: None.
    ///
    /// @param audioChannel Playback channel, the default is ZegoMediaPlayerAudioChannelAll.
    virtual void setActiveAudioChannel(ZegoMediaPlayerAudioChannel audioChannel) = 0;

    /// Clears the last frame of the playback control that remains on the control after playback ends.
    ///
    /// Available since: 2.20.0
    /// Description: Clears the last frame of the playback control that remains on the control after playback ends.
    /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
    /// Restrictions: The interface call takes effect only when the media player ends playing.
    virtual void clearView() = 0;

    /// Get meida information such as video resolution from media file.
    ///
    /// Available since: 3.6.0
    /// Description: Get meida information such as video resolution from media file.
    /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
    /// Restrictions: None.
    virtual ZegoMediaPlayerMediaInfo getMediaInfo() = 0;

    /// Update the position of the media player (audio source).
    ///
    /// Available since: 3.6.0
    /// Description: Update the position of the media player (audio source).
    /// Use cases: The media player also needs to have 3D spatial sound.
    /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
    /// Restrictions: This interface needs to be used in conjunction with the RangeAudio/RangeScene module. This interface can only be called successfully after the RangeAudio/RangeScene module enables 3D sound effects.
    ///
    /// @param position The unit vector of the front axis of its own coordinate system. The parameter is a float array with a length of 3.
    virtual void updatePosition(const float position[3]) = 0;

    /// Set http headers.
    ///
    /// Available since: 3.8.0
    /// Description: Call this function to set the http headers of the http network resource.
    /// Use cases: When the network resource needs to set special header information.
    /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
    /// Restrictions: Called before the corresponding network resource is loaded.
    ///
    /// @param headers Headers info.
    virtual void setHttpHeader(std::unordered_map<std::string, std::string> headers) = 0;

    /// Set play media stream type.
    ///
    /// Available since: 3.10.0
    /// Description: Configure the media stream type to be played. You can only play video streams or audio streams. This will take effect during the life cycle of the media player.
    /// Use cases: When only the video stream or audio stream needs to be played.
    /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
    /// Caution: Changing the media stream type during playing will take effect in the next playing.
    ///
    /// @param streamType Stream type.
    virtual void setPlayMediaStreamType(ZegoMediaStreamType streamType) = 0;

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
    virtual void enableLiveAudioEffect(bool enable, ZegoLiveAudioEffectMode mode) = 0;

    /// Enable local caching of http/https network resources.
    ///
    /// Available since: 3.12.0
    /// Description: When playing http/https network resources, when the local cache is enabled, the network resources will be saved locally and the cache information will be called back through [onMediaPlayerLocalCache].
    /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer]. Called after [loadResource] or during playback, it will take effect next playback.
    /// Caution: Only http/https single file type network resources are supported. If there is a [seek] operation during playback, the cache will fail.
    ///
    /// @param enable Whether to enable local caching.
    /// @param cacheDir Cache dir. If left blank, the directory specified internally by SDK will be used.
    virtual void enableLocalCache(bool enable, const std::string &cacheDir) = 0;

    /// Enable the view mirror.
    ///
    /// Available since: 3.14.0
    /// Description: Enable view mirror.
    /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
    ///
    /// @param enable Whether to enable view mirror.
    virtual void enableViewMirror(bool enable) = 0;

    /// Get playback statistics.
    ///
    /// Available since: 3.12.0
    /// Description: Get current playback statistics to monitor whether decoding and rendering anomalies occur in the player.
    /// Use cases: Typically used in cloud-based media player scenarios.
    /// When to call: Invoke after the [loadResource] callback succeeds.
    virtual ZegoMediaPlayerStatisticsInfo getPlaybackStatistics() = 0;

    /// [Deprecated] Setting up the specific voice changer parameters.
    ///
    /// @deprecated Deprecated since 3.15.0, please use [enableVoiceChanger] instead.
    /// @param audioChannel The audio channel to be voice changed
    /// @param param Voice changer parameters
    ZEGO_DEPRECATED
    virtual void setVoiceChangerParam(ZegoMediaPlayerAudioChannel audioChannel,
                                      ZegoVoiceChangerParam param) = 0;
};

class IZegoAudioEffectPlayer {
  protected:
    virtual ~IZegoAudioEffectPlayer() {}

  public:
    /// Set audio effect player event handler.
    ///
    /// Available since: 1.16.0
    /// Description: Set audio effect player event handler.
    /// When to call: It can be called after [createAudioEffectPlayer].
    /// Restrictions: None.
    /// Related APIs: [createAudioEffectPlayer].
    ///
    /// @param handler event handler for audio effect player.
    virtual void setEventHandler(std::shared_ptr<IZegoAudioEffectPlayerEventHandler> handler) = 0;

    /// Start playing audio effect.
    ///
    /// Available since: 1.16.0
    /// Description: Start playing audio effect. The default is only played once and is not mixed into the publishing stream, if you want to change this please modify [config] param.
    /// Use cases: When you need to play short sound effects, such as applause, cheers, etc., you can use this interface to achieve, and further configure the number of plays through the [config] parameter, and mix the sound effects into the push stream.
    /// When to call: It can be called after [createAudioEffectPlayer].
    /// Restrictions: None.
    ///
    /// @param audioEffectID Description: ID for the audio effect. The SDK uses audioEffectID to control the playback of sound effects. The SDK does not force the user to pass in this parameter as a fixed value. It is best to ensure that each sound effect can have a unique ID. The recommended methods are static self-incrementing ID or the hash of the incoming sound effect file path.
    /// @param path The absolute path of the local resource. <br>Value range: "assets://"、"ipod-library://" and network url are not supported. Set path as nullptr or "" if resource is loaded already using [loadResource].
    /// @param config Audio effect playback configuration. <br>Default value: Set nullptr will only be played once, and will not be mixed into the publishing stream.
    virtual void start(unsigned int audioEffectID, const std::string &path,
                       ZegoAudioEffectPlayConfig *config = nullptr) = 0;

    /// Stop playing audio effect.
    ///
    /// Available since: 1.16.0
    /// Description: Stop playing the specified audio effect [audioEffectID].
    /// When to call: The specified [audioEffectID] is [start].
    /// Restrictions: None.
    ///
    /// @param audioEffectID ID for the audio effect.
    virtual void stop(unsigned int audioEffectID) = 0;

    /// Pause playing audio effect.
    ///
    /// Available since: 1.16.0
    /// Description: Pause playing the specified audio effect [audioEffectID].
    /// When to call: The specified [audioEffectID] is [start].
    /// Restrictions: None.
    ///
    /// @param audioEffectID ID for the audio effect.
    virtual void pause(unsigned int audioEffectID) = 0;

    /// Resume playing audio effect.
    ///
    /// Available since: 1.16.0
    /// Description: Resume playing the specified audio effect [audioEffectID].
    /// When to call: The specified [audioEffectID] is [pause].
    /// Restrictions: None.
    ///
    /// @param audioEffectID ID for the audio effect.
    virtual void resume(unsigned int audioEffectID) = 0;

    /// Stop playing all audio effect.
    ///
    /// Available since: 1.16.0
    /// Description: Stop playing all audio effect.
    /// When to call: Some audio effects are Playing.
    /// Restrictions: None.
    virtual void stopAll() = 0;

    /// Pause playing all audio effect.
    ///
    /// Available since: 1.16.0
    /// Description: Pause playing all audio effect.
    /// When to call: It can be called after [createAudioEffectPlayer].
    /// Restrictions: None.
    virtual void pauseAll() = 0;

    /// Resume playing all audio effect.
    ///
    /// Available since: 1.16.0
    /// Description: Resume playing all audio effect.
    /// When to call: It can be called after [pauseAll].
    /// Restrictions: None.
    virtual void resumeAll() = 0;

    /// Set the specified playback progress.
    ///
    /// Available since: 1.16.0
    /// Description: Set the specified audio effect playback progress. Unit is millisecond.
    /// When to call: The specified [audioEffectID] is[start], and not finished.
    /// Restrictions: None.
    ///
    /// @param audioEffectID ID for the audio effect.
    /// @param millisecond Point in time of specified playback progress.
    /// @param callback The result of seek.
    virtual void seekTo(unsigned int audioEffectID, unsigned long long millisecond,
                        ZegoAudioEffectPlayerSeekToCallback callback) = 0;

    /// Set volume for a single audio effect. Both the local play volume and the publish volume are set.
    ///
    /// Available since: 1.16.0
    /// Description: Set volume for a single audio effect. Both the local play volume and the publish volume are set.
    /// When to call: The specified [audioEffectID] is [start].
    /// Restrictions: None.
    ///
    /// @param audioEffectID ID for the audio effect.
    /// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
    virtual void setVolume(unsigned int audioEffectID, int volume) = 0;

    /// Set local play volume for a single audio effect.
    ///
    /// Available since: 3.11.0
    /// Description: Set local play volume for a single audio effect.
    /// When to call: The specified [audioEffectID] is [start].
    /// Restrictions: None.
    ///
    /// @param audioEffectID ID for the audio effect.
    /// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
    virtual void setPlayVolume(unsigned int audioEffectID, int volume) = 0;

    /// Set publish volume for a single audio effect.
    ///
    /// Available since: 3.11.0
    /// Description: Set publish volume for a single audio effect.
    /// When to call: The specified [audioEffectID] is [start].
    /// Restrictions: None.
    ///
    /// @param audioEffectID ID for the audio effect.
    /// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
    virtual void setPublishVolume(unsigned int audioEffectID, int volume) = 0;

    /// Set volume for all audio effect. Both the local play volume and the publish volume are set.
    ///
    /// Available since: 1.16.0
    /// Description: Set volume for all audio effect. Both the local play volume and the publish volume are set.
    /// When to call: It can be called after [createAudioEffectPlayer].
    /// Restrictions: None.
    ///
    /// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
    virtual void setVolumeAll(int volume) = 0;

    /// Set local play volume for all audio effect.
    ///
    /// Available since: 3.11.0
    /// Description: Set local play volume for all audio effect.
    /// When to call: It can be called after [createAudioEffectPlayer].
    /// Restrictions: None.
    ///
    /// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
    virtual void setPlayVolumeAll(int volume) = 0;

    /// Set publish volume for all audio effect.
    ///
    /// Available since: 3.11.0
    /// Description: Set publish volume for all audio effect.
    /// When to call: It can be called after [createAudioEffectPlayer].
    /// Restrictions: None.
    ///
    /// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
    virtual void setPublishVolumeAll(int volume) = 0;

    /// Set the playback speed for a given audio effect. Both the local play speed and the publish speed are set. (separate settings are not supported).
    ///
    /// Available since: 2.18.0
    /// Description: Set the playback speed for a given audio effect. Both the local play speed and the publish speed are set. (separate settings are not supported).
    /// When to call: The specified [audioEffectID] is [start].
    /// Restrictions: None.
    ///
    /// @param audioEffectID ID for the audio effect.
    /// @param speed The speed of play. <br>Value range: The range is 0.5 ~ 2.0. <br>Default value: The default is 1.0.
    virtual void setPlaySpeed(unsigned int audioEffectID, float speed) = 0;

    /// Get the total duration of the specified audio effect resource.
    ///
    /// Available since: 1.16.0
    /// Description: Get the total duration of the specified audio effect resource. Unit is millisecond.
    /// When to call: You should invoke this function after the audio effect resource already loaded, otherwise the return value is 0.
    /// Restrictions: It can be called after [createAudioEffectPlayer].
    /// Related APIs: [start], [loadResource].
    ///
    /// @param audioEffectID ID for the audio effect.
    /// @return Unit is millisecond.
    virtual unsigned long long getTotalDuration(unsigned int audioEffectID) = 0;

    /// Get current playback progress.
    ///
    /// Available since: 1.16.0
    /// Description: Get current playback progress of the specified audio effect. Unit is millisecond.
    /// When to call: You should invoke this function after the audio effect resource already loaded, otherwise the return value is 0.
    /// Restrictions: None.
    /// Related APIs: [start], [loadResource].
    ///
    /// @param audioEffectID ID for the audio effect.
    virtual unsigned long long getCurrentProgress(unsigned int audioEffectID) = 0;

    /// Load audio effect resource.
    ///
    /// Available since: 1.16.0
    /// Description: Load audio effect resource.
    /// Use cases: In a scene where the same sound effect is played frequently, the SDK provides the function of preloading the sound effect file into the memory in order to optimize the performance of repeatedly reading and decoding the file.
    /// When to call: It can be called after [createAudioEffectPlayer].
    /// Restrictions: Preloading supports loading up to 15 sound effect files at the same time, and the duration of the sound effect files cannot exceed 30s, otherwise an error will be reported when loading.
    ///
    /// @param audioEffectID ID for the audio effect.
    /// @param path the absolute path of the audio effect resource and cannot be nullptr or "". <br>Value range: "assets://"、"ipod-library://" and network url are not supported.
    /// @param callback load audio effect resource result.
    virtual void loadResource(unsigned int audioEffectID, const std::string &path,
                              ZegoAudioEffectPlayerLoadResourceCallback callback) = 0;

    /// Unload audio effect resource.
    ///
    /// Available since: 1.16.0
    /// Description: Unload the specified audio effect resource.
    /// When to call: After the sound effects are used up, related resources can be released through this function; otherwise, the SDK will release the loaded resources when the AudioEffectPlayer instance is destroyed.
    /// Restrictions: None.
    /// Related APIs: [loadResource].
    ///
    /// @param audioEffectID ID for the audio effect loaded.
    virtual void unloadResource(unsigned int audioEffectID) = 0;

    /// Update the position of the audio effect player (audio source).
    ///
    /// Available since: 3.6.0
    /// Description: Update the position of the audio effect player (audio source).
    /// Use cases: The audio effect player also needs to have 3D spatial sound.
    /// When to call: Listen to the [onAudioEffectPlayStateUpdate] callback, please call this interface after the player state is ZegoAudioEffectPlayState.Playing and before ZegoAudioEffectPlayState.NoPlay/PlayEnded.
    /// Restrictions: This interface needs to be used in conjunction with the RangeAudio/RangeScene module. This interface can only be called successfully after the RangeAudio/RangeScene module enables 3D sound effects.
    ///
    /// @param audioEffectID ID for the audio effect.
    /// @param position The unit vector of the front axis of its own coordinate system. The parameter is a float array with a length of 3.
    virtual void updatePosition(unsigned int audioEffectID, const float position[3]) = 0;

    /// Get audio effect player index.
    ///
    /// Available since: 1.16.0
    /// Description: Get audio effect player index.
    /// When to call: It can be called after [createAudioEffectPlayer].
    /// Restrictions: None.
    ///
    /// @return Audio effect player index.
    virtual int getIndex() = 0;
};

class IZegoMediaDataPublisher {
  protected:
    virtual ~IZegoMediaDataPublisher() {}

  public:
    /// Set event callback handler for the media data publisher.
    ///
    /// Available since: 2.17.0
    /// Description: Set event callback handler for media data publisher.
    /// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
    /// When to call: After calling the [createMediaDataPublisher] function to create a media data publisher.
    /// Caution: This API used to be named [setMediaDataPublisherEventHandler] before version 3.8.0. Please refer to [Upgrade guide v3.8.0+](https://docs.zegocloud.com/en/16413.html).
    ///
    /// @param handler The event handler
    virtual void setEventHandler(std::shared_ptr<IZegoMediaDataPublisherEventHandler> handler) = 0;

    /// Add media file to the publish queue. Currently, only mp4 / m4a / aac files are supported, and special conversion is required.
    ///
    /// Available since: 2.17.0
    /// Description: Add media file to the publish queue. Currently, only mp4 / m4a / aac file are supported, and special conversion is required.
    /// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
    /// When to call: After calling the [createMediaDataPublisher] function to create a media data publisher.
    /// Caution: The mp4 file format must meet the following points：The video must be encoded as H.264 and cannot contain B frames, only I and P frames. The I frame interval is 2s, that is, a single GOP value is 2s; The frame rate, bit rate, and resolution of the video are consistent with the frame rate, bit rate, and resolution set by [setVideoConfig] before publishing stream; Audio encoding must be MPEG-4 AAC.
    ///
    /// @param path Local absolute path to the media file.
    /// @param isClear Whether to clear the publish queue.
    virtual void addMediaFilePath(const std::string &path, bool isClear) = 0;

    /// Clear all the status in this media data publisher, so that it can be restarted next time.
    ///
    /// Available since: 2.17.0
    /// Description: When you need to re-publish stream and do not need to continue publishing from the previous publish queue, you can call this function to reset this media data publisher's state.
    /// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
    /// Caution: When the developer calls [logoutRoom], the state is automatically reset.
    virtual void reset() = 0;

    /// Set the delay time of video playback.
    ///
    /// Available since: 2.17.0
    /// Description: When this value is set, when publishing video file stream, the SDK will permanently delay the video to the set time value before sending.
    /// Use cases: Mainly used to correct the fixed audio and picture asynchronous phenomenon that occurs during streaming.
    /// When to call: After calling the [createMediaDataPublisher] function to create a media data publisher.
    ///
    /// @param delayTime Video playback time.Required: Yes.Value range: [0, 100] ms.
    virtual void setVideoSendDelayTime(int delayTime) = 0;

    /// Specify the starting point of the current video file publishing.
    ///
    /// Available since: 2.17.0
    /// Description: Specify the starting point of the current video file publishing.
    /// When to call: Called at any point between [OnMediaDataPublisherFileOpen] and [OnMediaDataPublisherFileClose]. For example: this function can be called directly in the [OnMediaDataPublisherFileOpen] callback.
    ///
    /// @param millisecond The timestamp of the start of streaming (relative to the timestamp of the file currently being published, the starting value is 0). the unit is milliseconds
    virtual void seekTo(unsigned long long millisecond) = 0;

    /// Get the total duration of the current file.
    ///
    /// Available since: 2.17.0
    /// Description: Get the total duration of the current file, in milliseconds.
    /// When to call: After [onMediaDataPublisherFileDataBegin] callback.
    ///
    /// @return The total duration of the current file.
    virtual unsigned long long getTotalDuration() = 0;

    /// Get the playing progress of the current file.
    ///
    /// Available since: 2.17.0
    /// Description: Get the playing progress of the current file, in milliseconds.
    /// When to call: After received the [onMediaDataPublisherFileDataBegin] callback.
    ///
    /// @return The playing progress of the current file.
    virtual unsigned long long getCurrentDuration() = 0;

    /// Get the channel index of the media data publisher.
    ///
    /// Available since: 3.4.0
    /// Description: Get the channel index of the media data publisher.
    virtual int getIndex() = 0;
};

class IZegoRangeAudio {
  protected:
    virtual ~IZegoRangeAudio() {}

  public:
    /// set range audio event handler.
    ///
    /// Available since: 2.11.0
    /// Description: Set the callback function of the range audio module, which can receive the callback notification of the microphone on state [onRangeAudioMicrophoneStateUpdate].
    /// Use case: Used to monitor the connection status of the current microphone.
    /// When to call: After initializing the range audio [createRangeAudio].
    ///
    /// @param handler The object used to receive range audio callbacks.
    virtual void setEventHandler(std::shared_ptr<IZegoRangeAudioEventHandler> handler) = 0;

    /// Set the maximum range of received audio.
    ///
    /// Available since: 2.11.0
    /// Description: Set the audio receiving range, the audio source sound beyond this range will not be received.
    /// Use case: Set the receiver's receiving range in the `World` mode.
    /// Default value: When this function is not called, only the voices of the members in the team can be received, and all voices outside the team cannot be received.
    /// When to call: After initializing the range audio [createRangeAudio].
    /// Restrictions: This range only takes effect for people outside the team.
    ///
    /// @param range the audio range, the value must be greater than or equal to 0.
    virtual void setAudioReceiveRange(float range) = 0;

    /// Set the configuration of the audio receiving range.
    ///
    /// Available since: 3.7.0
    /// Description: Set the audio receiving range, the audio source sound beyond this range will not be received.
    /// Use case: Set the receiver's receiving range in the `World` mode.
    /// Default value: When this function is not called, only the voices of the members in the team can be received, and all voices outside the team cannot be received.
    /// When to call: After initializing the range audio [createRangeAudio].
    /// Restrictions: This range only takes effect for people outside the team.
    ///
    /// @param param Configuration of audio receiving range.
    /// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
    virtual int setAudioReceiveRange(ZegoReceiveRangeParam param) = 0;

    /// Set the frequency of real-time update locations within the SDK.
    ///
    /// Available since: 2.21.0
    /// Description: Set the frequency of real-time update locations within the SDK min 15 ms.
    /// Use case: After setting the update position, the sensitivity of audio gradient is very high.
    /// Default value: 100 ms.
    /// When to call: After initializing the range audio [createRangeAudio].
    ///
    /// @param frequency the frequency, the value must be greater than 15 ms.
    virtual void setPositionUpdateFrequency(int frequency) = 0;

    /// Set range voice volume.
    ///
    /// Available since: 2.23.0
    /// Description: Set range voice volume.
    /// Use case: This interface allows you to increase or decrease the volume of a range voice stream when the user calls [startPlayingStream] and pulls another stream.
    /// Default value: 100.
    /// When to call: After initializing the range audio [createRangeAudio].
    ///
    /// @param volume volume, [0,200].
    virtual void setRangeAudioVolume(int volume) = 0;

    /// Set the sound range for the stream.
    ///
    /// Available since: 2.23.0
    /// Description: Set range voice volume.
    /// Use case: When a user calls [startPlayingStream] and pulls another stream, the stream has a range speech effect by setting the range of sounds for that stream and calling [updateStreamPosition]. After the call will be the sound source of the sound range of the distance attenuation effect.
    /// When to call: After initializing the range audio [createRangeAudio] and after [startPlayingStream].
    /// Caution:  When calling [enableMicrophone] to enable range speech, the resource of the stream will be switched to RTC, regardless of whether the resource specified when [startPlayingStream] was originally called to pull the stream is RTC. If you really need to specify the resource of the stream as CDN, please configure it to pull a custom CDN stream and specify the CDN address information.
    ///
    /// @param streamID play stream id
    /// @param vocalRange Flow sound range.
    virtual void setStreamVocalRange(const std::string &streamID, float vocalRange) = 0;

    /// Set the sound range for the stream.
    ///
    /// Available since: 3.7.0
    /// Description: Set range voice volume.
    /// Use case: When a user calls [startPlayingStream] and pulls another stream, the stream has a range speech effect by setting the range of sounds for that stream and calling [updateStreamPosition]. After the call will be the sound source of the sound range of the distance attenuation effect.
    /// When to call: After initializing the range audio [createRangeAudio] and after [startPlayingStream].
    /// Caution:  When calling [enableMicrophone] to enable range speech, the resource of the stream will be switched to RTC, regardless of whether the resource specified when [startPlayingStream] was originally called to pull the stream is RTC. If you really need to specify the resource of the stream as CDN, please configure it to pull a custom CDN stream and specify the CDN address information.
    ///
    /// @param streamID play stream id
    /// @param param Flow sound range.
    /// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
    virtual int setStreamVocalRange(const std::string &streamID, ZegoVocalRangeParam param) = 0;

    /// Update the location of the flow.
    ///
    /// Available since: 2.23.0
    /// Description: Set range voice volume.
    /// Use case: When the user calls [startPlayingStream] to pull another stream, call [setStreamVocalRange] to set the stream's voice position, then call this interface to set the stream's position, so that the stream also has the range voice effect.
    /// When to call: After initializing the range audio [createRangeAudio] and after [startPlayingStream].
    ///
    /// @param streamID play stream id.
    /// @param position The unit vector of the front axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
    virtual void updateStreamPosition(const std::string &streamID, float position[3]) = 0;

    /// Update self position and orentation.
    ///
    /// Available since: 2.11.0
    /// Description: Update the user's position and orientation so that the SDK can calculate the distance between the user and the audio source and the stereo effect of the left and right ears.
    /// Use case: When the role operated by the user in the game moves on the world map, the position information and head orientation of the role are updated.
    /// When to call: Called after logging in to the room [loginRoom].
    /// Caution: Before calling [enableSpeaker] to turn on the speaker, if you do not call this interface to set the location information, you will not be able to receive voices from other people except the team.
    ///
    /// @param position The coordinates of the oneself in the world coordinate system. The parameter is a float array of length 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
    /// @param axisForward The unit vector of the front axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
    /// @param axisRight The unit vector of the right axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
    /// @param axisUp The unit vector of the up axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
    virtual void updateSelfPosition(float position[3], float axisForward[3], float axisRight[3],
                                    float axisUp[3]) = 0;

    /// Add or update audio source position information.
    ///
    /// Available since: 2.11.0
    /// Description: Set the position of the audio source corresponding to the userID on the game map in the room, so that the SDK can calculate the distance and orientation of the listener to the audio source.
    /// Use case: Update the position of the voice user in the game map coordinates.
    /// When to call: Call [loginRoom] to call after logging in to the room, and the recorded audio source information will be cleared after logging out of the room.
    ///
    /// @param userID The userID of the sound source.
    /// @param position The coordinates of the speaker in the world coordinate system. The parameter is a float array of length 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
    virtual void updateAudioSource(const std::string &userID, float position[3]) = 0;

    /// Turn the 3D spatial sound on or off.
    ///
    /// Available since: 2.11.0
    /// Description: After the 3D sound effect is turned on, the sound effect in the actual space will be simulated according to the position of the speaker equivalent to the listener. The intuitive feeling is that the sound size and the left and right sound difference will also change when the distance and orientation of the sound source change.
    /// Use case: It is a feature of audio recognition in FPS games or social scene games.
    /// Default value: When this function is not called, 3D sound effects are turned off by default.
    /// When to call: After initializing the range audio [createRangeAudio].
    /// Caution: The 3D audio effect will only take effect when [setRangeAudioMode] is called and set to `World` mode.
    /// Related APIs: After enabling the 3D sound effect, you can use [updateAudioSource] or [updateSelfPosition] to change the position and orientation to experience the 3D effect.
    ///
    /// @param enable Whether to enable 3D sound effects.
    virtual void enableSpatializer(bool enable) = 0;

    /// Turn the microphone on or off.
    ///
    /// Available since: 2.11.0
    /// Description: When enable is `true`, turn on the microphone and push audio stream; when it is `false`, turn off the microphone and stop pushing audio stream.
    /// Use case: The user turns on or off the microphone to communicate in the room.
    /// Default value: When this function is not called, the microphone is turned off by default.
    /// When to call: After initializing the range audio [createRangeAudio] and login room [loginRoom].
    /// Caution: Turning on the microphone will automatically use the main channel to push the audio stream.
    /// Related callbacks: Get the microphone switch state change through the callback [onRangeAudioMicrophoneStateUpdate].
    ///
    /// @param enable Whether to turn on the microphone.
    virtual void enableMicrophone(bool enable) = 0;

    /// Turn the speaker on or off.
    ///
    /// Available since: 2.11.0
    /// Description: When enable is `true`, turn on the speaker and play audio stream; when it is `false`, turn off the speaker and stop playing audio stream.
    /// Use case: The user turns on or off the speaker to communicate in the room.
    /// Default value: When this function is not called, the speaker is turned off by default.
    /// When to call: After initializing the range audio [createRangeAudio] and login room [loginRoom].
    /// Caution: Turning on the speaker will automatically pull the audio stream in the room.
    ///
    /// @param enable Whether to turn on the speaker.
    virtual void enableSpeaker(bool enable) = 0;

    /// Set range audio mode.
    ///
    /// Available since: 2.11.0
    /// Description: The audio mode can be set to `World` mode or `Team` mode.
    /// Use case: The user can choose to chat with everyone in the `World` mode (with distance limitation), or to communicate within the team in the `Team` mode (without distance limitation).
    /// Default value: If this function is not called, the `World` mode is used by default.
    /// When to call: After initializing the range audio [createRangeAudio].
    /// Related APIs: In the `World` mode, you can set the sound receiving range [setAudioReceiveRange], in the `Team` mode, you need to set [setTeamID] to join the corresponding team to hear the voice in the team.
    ///
    /// @param mode The range audio mode.
    virtual void setRangeAudioMode(ZegoRangeAudioMode mode) = 0;

    /// Set range audio custom mode.
    ///
    /// Available since: 3.3.0
    /// Description: Can set the speak mode and listening mode respectively to control the speak and listening behavior in the world and team.
    /// Use case: The user can decide who can listen to his voice by selecting the speak mode, and can also decide whose voice to listen to by selecting the listening mode.
    /// Default value: If this interface is not called, the ZegoRangeAudioSpeakModeAll mode and ZegoRangeAudioListenModeAll mode is used by default.
    /// When to call: After initializing the range audio [createRangeAudio].
    /// Related APIs: When you want to listen to sounds from the world, you need to set the sound reception range [setAudioReceiveRange]. When you want to sound and listen in the squad, you need to set [setTeamID] to join the corresponding squad.
    /// Restrictions: 1. Cannot be called with [setRangeAudioMode];
    ///  2. Not compatible with versions prior to 3.3.0.
    ///
    /// @param speakMode The range audio speak mode.
    /// @param listenMode The range audio listening mode.
    virtual void setRangeAudioCustomMode(ZegoRangeAudioSpeakMode speakMode,
                                         ZegoRangeAudioListenMode listenMode) = 0;

    /// Set team ID.
    ///
    /// Available: since 2.11.0
    /// Description: After setting the team ID, you will be able to communicate with other users of the same team, and the sound will not change with the distance. It is also possible to exit the team by setting an empty string.
    /// Use case: Users join the team or exit the team.
    /// Default value: When this function is not called, no team will be added by default.
    /// When to call: After initializing the range audio [createRangeAudio].
    /// Caution: There will be no distance limit for the sounds in the team, and there will be no 3D sound effects.
    ///
    /// @param teamID Team ID, empty to exit the team, a string of up to 64 bytes in length. Support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
    virtual void setTeamID(const std::string &teamID) = 0;

    /// Whether can receive the audio data of the specified user.
    ///
    /// Available since: 2.16.0
    /// Description: In the process of real-time audio and video interaction, local users can use this function to control whether to receive audio data from designated remote users when pulling streams as needed. When the developer does not receive the audio receipt, the hardware and network overhead can be reduced.
    /// Use cases: When developers need to quickly close and restore remote audio, they can call this function to enhance the interactive experience.
    /// Default value: The default is `false`, which means to receive audio data from all users.
    /// When to call: After initializing the range audio [createRangeAudio].
    /// Caution: This function is valid only when the [muteAllPlayStreamAudio] function is set to `false`.
    /// Related APIs: You can call the [muteAllPlayStreamAudio] function to control whether to receive all audio data. 1. When the [muteAllPlayStreamAudio(true)] function is called, it takes effect globally, that is, local users will be prohibited from receiving audio data from all remote users. At this time, the [muteUser] function will not take effect regardless of whether the [muteUser] function is called before or after [muteAllPlayStreamAudio]. 2. When the [muteAllPlayStreamAudio(false)] function is called, the local user can receive the audio data of all remote users. At this time, the [muteUser] function can be used to control whether to receive the audio data of the specified user. Calling the [muteUser(userID, true)] function allows the local user to receive audio data other than the `userID`; calling the [muteUser(userID, false)] function allows the local user to receive the audio data of the `userID`.
    ///
    /// @param userID User ID.
    /// @param mute Whether it can receive the audio data of the specified remote user, "true" means prohibition, "false" means receiving, the default value is "false".
    virtual void muteUser(const std::string &userID, bool mute) = 0;
};

class IZegoCopyrightedMusic {
  protected:
    virtual ~IZegoCopyrightedMusic() {}

  public:
    /// set copyrighted music event handler.
    ///
    /// Available since: 2.13.0
    /// Description: Set the callback function of the copyrighted music module, which can receive callback notifications related to song playback status .
    /// When to call: After create the copyrighted music [createCopyrightedMusic].
    ///
    /// @param handler The object used to receive copyrighted music callbacks.
    virtual void setEventHandler(std::shared_ptr<IZegoCopyrightedMusicEventHandler> handler) = 0;

    /// Initialize the copyrighted music module.
    ///
    /// Available since: 2.13.0
    /// Description: Initialize the copyrighted music so that you can use the function of the copyrighted music later.
    /// When to call: After call [createCopyrightedMusic] and [loginRoom].
    /// Caution: 1. The real user information must be passed in, otherwise the song resources cannot be obtained for playback. 2. The user ID set when initializing copyrighted music needs to be the same as the user ID set when logging in to the room.
    ///
    /// @param config the copyrighted music configuration.
    /// @param callback init result
    virtual void initCopyrightedMusic(ZegoCopyrightedMusicConfig config,
                                      ZegoCopyrightedMusicInitCallback callback) = 0;

    /// Get cache size.
    ///
    /// Available since: 2.13.0
    /// Description: When using this module, some cache files may be generated, and the size of the cache file can be obtained through this interface.
    /// Use case: Used to display the cache size of the App.
    /// When to call: After initializing the copyrighted music [createCopyrightedMusic].
    ///
    /// @return cache file size, in byte.
    virtual unsigned long long getCacheSize() = 0;

    /// Clear cache.
    ///
    /// Available since: 2.13.0
    /// Description: When using this module, some cache files may be generated, which can be cleared through this interface.
    /// Use case: Used to clear the cache of the App.
    /// When to call: After initializing the copyrighted music [createCopyrightedMusic].
    virtual void clearCache() = 0;

    /// Send extended feature request.
    ///
    /// Available since: 2.13.0
    /// Description: Initialize the copyrighted music so that you can use the function of the copyrighted music later.
    /// Use case: Used to get a list of songs.
    /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
    ///
    /// @param command request command, details about the commands supported.
    /// @param params request parameters, each request command has corresponding request parameters.
    /// @param callback send extended feature request result
    virtual void sendExtendedRequest(const std::string &command, const std::string &params,
                                     ZegoCopyrightedMusicSendExtendedRequestCallback callback) = 0;

    /// Get lyrics in lrc format.
    ///
    /// Available since: 3.2.1
    /// Description: Get lyrics in lrc format, support parsing lyrics line by line.
    /// Use case: Used to display lyrics line by line.
    /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
    ///
    /// @param songID the ID of the song or accompaniment, the song and accompaniment of a song share the same ID.
    /// @param vendorID Copyright music resource song copyright provider.
    /// @param callback get lyrics result
    virtual void getLrcLyric(const std::string &songID, ZegoCopyrightedMusicVendorID vendorID,
                             ZegoCopyrightedMusicGetLrcLyricCallback callback) = 0;

    /// Get lyrics in lrc format.
    ///
    /// Available since: 3.12.0
    /// Description: Get lyrics in lrc format, support parsing lyrics line by line.
    /// Use case: Used to display lyrics line by line.
    /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
    ///
    /// @param config The configuration of get lyric.
    /// @param callback get lyrics result
    virtual void getLrcLyric(ZegoCopyrightedMusicGetLyricConfig config,
                             ZegoCopyrightedMusicGetLrcLyricCallback callback) = 0;

    /// Get lyrics in krc format.
    ///
    /// Available since: 2.13.0
    /// Description: Get verbatim lyrics, support parsing lyrics word by word.
    /// Use case: Used to display lyrics word by word.
    /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
    ///
    /// @param krcToken The krcToken obtained when calling [requestResource] for accompaniment or climax clips, or when obtaining shared resources through the [getSharedResource] interface. For more details, please refer to https://doc-zh.zego.im/article/15079#2_2
    /// @param callback get lyrics result.
    virtual void getKrcLyricByToken(const std::string &krcToken,
                                    ZegoCopyrightedMusicGetKrcLyricByTokenCallback callback) = 0;

    /// Request music resource.
    ///
    /// Available since: 3.0.2
    /// Description: In addition to obtaining the basic information of the song (duration, song name, singer, etc.), and the most important resource id that can be used for local playback, there are also some related authentications information.
    /// Use case: Get copyrighted songs for local playback and sharing.
    /// Related APIs: After a user in the room successfully calls this interface to request a music resource, other users in the room can call the [getsharedresource] interface to get the music resource for free once.
    /// When to call: After initializing the copyrighted music [initCopyrightedMusic].
    /// Note:
    ///   1. Each resource has a unique resource ID.
    ///   2. Every time this API is called, it will be billed once, please consult ZEGO business personnel for details.
    ///   3. Each resource has a unique resource ID. The resources obtained by calling this API are time-sensitive, the valid duration is the minimum value between the SDK life cycle and 24-hour.
    ///
    /// @param config The configuration of requesting music resource.
    /// @param type The resource type of music.
    /// @param callback Callback of requesting music resource.
    virtual void requestResource(ZegoCopyrightedMusicRequestConfig config,
                                 ZegoCopyrightedMusicResourceType type,
                                 ZegoCopyrightedMusicRequestResourceCallback callback) = 0;

    /// Request music resource.
    ///
    /// Available since: 3.12.0
    /// Description: In addition to obtaining the basic information of the song (duration, song name, singer, etc.), and the most important resource id that can be used for local playback, there are also some related authentications information.
    /// Use case: Get copyrighted songs for local playback and sharing.
    /// Related APIs: After a user in the room successfully calls this interface to request a music resource, other users in the room can call the [getsharedresource] interface to get the music resource for free once.
    /// When to call: After initializing the copyrighted music [initCopyrightedMusic].
    /// Note:
    ///   1. Each resource has a unique resource ID.
    ///   2. Every time this API is called, it will be billed once, please consult ZEGO business personnel for details.
    ///   3. Each resource has a unique resource ID. The resources obtained by calling this API are time-sensitive, the valid duration is the minimum value between the SDK life cycle and 24-hour.
    ///
    /// @param config The configuration of requesting music resource.
    /// @param callback Callback of requesting music resource.
    virtual void requestResource(ZegoCopyrightedMusicRequestConfigV2 config,
                                 ZegoCopyrightedMusicRequestResourceCallback callback) = 0;

    /// Get shared music resource.
    ///
    /// Available since: 3.0.2
    /// Description: In addition to obtaining the basic information of the song (duration, song name, singer, etc.), and the most important resource id that can be used for local playback, there are also some related authentications information.
    /// Use case: Get copyrighted songs for local playback.
    /// Related APIs: After a user in the room calls the [requestresource] interface to request a music resource successfully, other users in the room can call this interface to get the music resource for free once.
    /// When to call: After initializing the copyrighted music [initCopyrightedMusic].
    /// Note: Each resource has a unique resource ID. The resources obtained by calling this API are time-sensitive, the valid duration is the minimum value between the SDK life cycle and 24-hour.
    ///
    /// @param config The configuration of getting shared music resource.
    /// @param type The resource type of music.
    /// @param callback Callback of getting shared music resource.
    virtual void getSharedResource(ZegoCopyrightedMusicGetSharedConfig config,
                                   ZegoCopyrightedMusicResourceType type,
                                   ZegoCopyrightedMusicGetSharedResourceCallback callback) = 0;

    /// Get shared music resource.
    ///
    /// Available since: 3.12.0
    /// Description: In addition to obtaining the basic information of the song (duration, song name, singer, etc.), and the most important resource id that can be used for local playback, there are also some related authentications information.
    /// Use case: Get copyrighted songs for local playback.
    /// Related APIs: After a user in the room calls the [requestresource] interface to request a music resource successfully, other users in the room can call this interface to get the music resource for free once.
    /// When to call: After initializing the copyrighted music [initCopyrightedMusic].
    /// Note: Each resource has a unique resource ID. The resources obtained by calling this API are time-sensitive, the valid duration is the minimum value between the SDK life cycle and 24-hour.
    ///
    /// @param config The configuration of getting shared music resource.
    /// @param callback Callback of getting shared music resource.
    virtual void getSharedResource(ZegoCopyrightedMusicGetSharedConfigV2 config,
                                   ZegoCopyrightedMusicGetSharedResourceCallback callback) = 0;

    /// Download music resource.
    ///
    /// Available since: 2.13.0
    /// Description: Download music resource. It can only be played after downloading successfully.
    /// Use case: After obtaining the music resource authorization, use this interface to download the corresponding resources.
    /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
    /// Caution: Downloading copyrighted music resources is affected by the Internet.
    ///
    /// @param resourceID the resource ID corresponding to the song or accompaniment.
    /// @param callback download song or accompaniment result.
    virtual void download(const std::string &resourceID,
                          ZegoCopyrightedMusicDownloadCallback callback) = 0;

    /// Cancel download of music resource.
    ///
    /// Available since: 3.11.0
    /// Description: Cancel download of music resource.
    /// Use case: After starting to download music resources, use this interface to cancel the corresponding download task.
    /// When to call: After starting the download [download].
    /// Caution: When a valid resourceID is passed in, only unfinished download tasks will be cancelled. When empty is passed in, all unfinished download tasks will be canceled.
    ///
    /// @param resourceID the resource ID corresponding to the song or accompaniment.
    virtual void cancelDownload(const std::string &resourceID) = 0;

    /// Query the resource's cache is existed or not.
    ///
    /// Available since: 3.9.0
    /// Description: Query the resource is existed or not, query the Yinsuda resource cache by default
    /// Use case: Can be used to check the resource's cache is existed or not
    /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
    ///
    /// @param config The configuration of querying cache.
    virtual bool queryCache(ZegoCopyrightedMusicQueryCacheConfig config) = 0;

    /// Query the resource's cache is existed or not.
    ///
    /// Available since: 3.12.0
    /// Description: Query the resource is existed or not, query the Yinsuda resource cache by default
    /// Use case: Can be used to check the resource's cache is existed or not
    /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
    ///
    /// @param config The configuration of querying cache.
    virtual bool queryCache(ZegoCopyrightedMusicQueryCacheConfigV2 config) = 0;

    /// Get the playing time of a song or accompaniment file.
    ///
    /// Available since: 2.13.0
    /// Description: Get the playing time of a song or accompaniment file.
    /// Use case: Can be used to display the playing time information of the song or accompaniment on the view.
    /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
    ///
    /// @param resourceID the resource ID corresponding to the song or accompaniment.
    virtual unsigned long long getDuration(const std::string &resourceID) = 0;

    /// Set the difficulty level of scoring.
    ///
    /// Available since: 2.22.0
    /// Description: Users can set the scoring difficulty level through this function.
    /// When to call: After calling [initCopyrightedMusic] to initialize copyrighted music successfully, call [startScore] to start scoring.
    /// Default value: When this function is not called, the difficulty level of scoring is 4.
    /// Restrictions: This function does not support dynamic settings. After calling this function successfully, the next call to [startScore] will take effect.
    ///
    /// @param level The difficulty level of scoring. The level ranges from 0 to 4. The scoring difficulty decreases from 0 to 4.
    virtual void setScoringLevel(int level) = 0;

    /// Start scoring.
    ///
    /// Available since: 2.15.0
    /// Description: Start the scoring function.After starting scoring, the scoring result OnCurrentPitchValueUpdate callback will be received according to the set callback time interval.
    /// Use case: Can be used to display the singing score on the view.
    /// When to call: After obtaining verbatim lyrics and playing the accompaniment resources of copyrighted music.
    /// Restrictions: Only support use this api after [startPublishingStream].
    ///
    /// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
    /// @param pitchValueInterval the time interval of real-time pitch line callback, in milliseconds, the default is 50 milliseconds.
    virtual int startScore(const std::string &resourceID, int pitchValueInterval) = 0;

    /// Pause scoring.
    ///
    /// Available since: 2.15.0
    /// Description: Pause ongoing scoring,will stop the [OnCurrentPitchValueUpdate] callback.
    /// Use case: You can call this interface to pause the scoring function while scoring.
    /// When to call: It can be called while grading.
    ///
    /// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
    virtual int pauseScore(const std::string &resourceID) = 0;

    /// Resume scoring.
    ///
    /// Available since: 2.15.0
    /// Description: Resume currently paused scoring.
    /// Use case: When there is currently paused scoring, this interface can be called to resume the scoring function.
    /// When to call: It can be called when there is currently a paused scoring.
    ///
    /// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
    virtual int resumeScore(const std::string &resourceID) = 0;

    /// Stop scoring.
    ///
    /// Available since: 2.15.0
    /// Description: End the current rating.The [OnCurrentPitchValueUpdate] callback will be stopped, but the average or total score can still be obtained normally.
    /// Use case: You can call this interface to end the scoring while scoring.
    /// When to call: It can be called while grading.
    ///
    /// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
    virtual int stopScore(const std::string &resourceID) = 0;

    /// Reset scoring.
    ///
    /// Available since: 2.15.0
    /// Description: Reset the scores that have already been performed,The [OnCurrentPitchValueUpdate] callback will be stopped and the average or total score will be 0.
    /// Use case: Often used in scenes where the same song is re-sung.
    /// When to call: It can be called after scoring has been performed.
    ///
    /// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
    virtual int resetScore(const std::string &resourceID) = 0;

    /// Get the score of the previous sentence.
    ///
    /// Available since: 2.15.0
    /// Description: Get the score of the previous sentence.
    /// Use case: Can be used to display the score of each sentence on the view.
    /// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
    ///
    /// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
    virtual int getPreviousScore(const std::string &resourceID) = 0;

    /// Get average score.
    ///
    /// Available since: 2.15.0
    /// Description: Get the average score.
    /// Use case: Can be used to display the average score on the view.
    /// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
    ///
    /// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
    virtual int getAverageScore(const std::string &resourceID) = 0;

    /// Get total score .
    ///
    /// Available since: 2.15.0
    /// Description: Get the total score.
    /// Use case: Can be used to display the total score on the view.
    /// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
    ///
    /// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
    virtual int getTotalScore(const std::string &resourceID) = 0;

    /// Get full score .
    ///
    /// Available since: 3.0.2
    /// Description: Get the full score.
    /// Use case: Can be used to display the full score on the view.
    /// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
    ///
    /// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
    virtual int getFullScore(const std::string &resourceID) = 0;

    /// Get standard pitch data.
    ///
    /// Available since: 2.15.0
    /// Description: Get standard pitch data.
    /// Use case: Can be used to display standard pitch lines on the view.
    /// Caution: Only accompaniment or climactic clip assets have pitch lines.
    ///
    /// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
    /// @param callback get standard pitch data result.
    virtual void getStandardPitch(const std::string &resourceID,
                                  ZegoCopyrightedMusicGetStandardPitchCallback callback) = 0;

    /// Get real-time pitch data.
    ///
    /// Available since: 2.15.0
    /// Description: Get real-time pitch data.
    /// Use case: Can be used to display real-time pitch lines on the view.
    /// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
    ///
    /// @param resourceID the resource ID corresponding to the song or accompaniment.
    virtual int getCurrentPitch(const std::string &resourceID) = 0;

    /// [Deprecated] Request a song. Deprecated since 3.0.2, please use the [requestResource] function instead.
    ///
    /// Available since: 2.13.0
    /// Description: By requesting a song, you can not only obtain basic information about a song (such as duration, song name, and artist), but also obtain the resource ID for local playback, share_token for sharing with others, and related authentication information. Support by the time, by the user monthly, by the room monthly subscription three ways.
    /// Use case: Get copyrighted songs for local playback and sharing.
    /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
    /// Caution: This interface will trigger billing. A song may have three sound qualities: normal, high-definition, and lossless. Each sound quality has a different resource file, and each resource file has a unique resource ID.
    ///
    /// @deprecated Deprecated since 3.0.2, please use the [requestResource] function instead.
    /// @param config request configuration.
    /// @param callback request a song result
    ZEGO_DEPRECATED
    virtual void requestSong(ZegoCopyrightedMusicRequestConfig config,
                             ZegoCopyrightedMusicRequestSongCallback callback) = 0;

    /// [Deprecated] Request accompaniment. Deprecated since 3.0.2, please use the [requestResource] function instead.
    ///
    /// Available since: 2.13.0
    /// Description: You can get the accompaniment resources of the song corresponding to the songID, including resource_id, krc_token, share_token, etc. Supports click-by-point accompaniment.
    /// Use case: Get copyrighted accompaniment for local playback and sharing.
    /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
    /// Caution: This interface will trigger billing.
    ///
    /// @deprecated Deprecated since 3.0.2, please use the [requestResource] function instead.
    /// @param config request configuration.
    /// @param callback request accompaniment result.
    ZEGO_DEPRECATED
    virtual void
    requestAccompaniment(ZegoCopyrightedMusicRequestConfig config,
                         ZegoCopyrightedMusicRequestAccompanimentCallback callback) = 0;

    /// [Deprecated] Request accompaniment clip. Deprecated since 3.0.2, please use the [requestResource] function instead.
    ///
    /// Available since: 2.13.0
    /// Description: You can get the accompaniment clip resources of the song corresponding to the songID, including resource_id, krc_token, share_token, etc. Supports accompaniment clips by pay-per-use.
    /// Use case: Get copyrighted accompaniment clip for local playback and sharing.
    /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
    /// Caution: This interface will trigger billing.
    ///
    /// @deprecated Deprecated since 3.0.2, please use the [requestResource] function instead.
    /// @param config request configuration.
    /// @param callback request accompaniment clip result.
    ZEGO_DEPRECATED
    virtual void
    requestAccompanimentClip(ZegoCopyrightedMusicRequestConfig config,
                             ZegoCopyrightedMusicRequestAccompanimentClipCallback callback) = 0;

    /// [Deprecated] Get a song or accompaniment. Deprecated since 3.0.2, please use the [getSharedResource] function instead.
    ///
    /// Available since: 2.13.0
    /// Description: After the user successfully obtains the song/accompaniment/accompaniment clip resource, he can get the corresponding shareToken, share the shareToken with other users, and other users call this interface to obtain the shared music resources.
    /// Use case: In the online KTV scene, after receiving the song or accompaniment token shared by the lead singer, the chorus obtains the corresponding song or accompaniment through this interface, and then plays it on the local end.
    /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
    ///
    /// @deprecated Deprecated since 3.0.2, please use the [getSharedResource] function instead.
    /// @param shareToken access the corresponding authorization token for a song or accompaniment.
    /// @param callback get a song or accompaniment result.
    ZEGO_DEPRECATED
    virtual void getMusicByToken(const std::string &shareToken,
                                 ZegoCopyrightedMusicGetMusicByTokenCallback callback) = 0;

    /// [Deprecated] Get lyrics in lrc format. Deprecated since 3.2.1, please use the method with the same name with [vendorID] parameter instead.
    ///
    /// Available since: 2.13.0
    /// Description: Get lyrics in lrc format, support parsing lyrics line by line.
    /// Use case: Used to display lyrics line by line.
    /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
    ///
    /// @deprecated Deprecated since 2.14.0, please use the method with the same name with [vendorID] parameter instead.
    /// @param songID the ID of the song or accompaniment, the song and accompaniment of a song share the same ID.
    /// @param callback get lyrics result
    ZEGO_DEPRECATED
    virtual void getLrcLyric(const std::string &songID,
                             ZegoCopyrightedMusicGetLrcLyricCallback callback) = 0;

    /// [Deprecated] Query the resource's cache is existed or not. Deprecated since 3.2.1, please use the method with the same name with [vendorID] parameter instead.
    ///
    /// Available since: 2.13.0
    /// Description: Query the resource is existed or not.
    /// Use case: Can be used to check the resource's cache is existed or not
    /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
    ///
    /// @deprecated Deprecated since 2.14.0, please use the method with the same name with [vendorID] parameter instead.
    /// @param songID the ID of the song or accompaniment, the song and accompaniment of a song share the same ID.
    /// @param type the song resource type.
    ZEGO_DEPRECATED
    virtual bool queryCache(const std::string &songID, ZegoCopyrightedMusicType type) = 0;

    /// [Deprecated] Query the resource's cache is existed or not. Deprecated since 3.9.0, please use the method with the same name with [config] parameter instead.
    ///
    /// Available since: 3.2.1
    /// Description: Query the resource is existed or not, query the Yinsuda resource cache by default
    /// Use case: Can be used to check the resource's cache is existed or not
    /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
    ///
    /// @deprecated Deprecated since 3.9.0, please use the method with the same name with [config] parameter instead.
    /// @param songID the ID of the song or accompaniment, the song and accompaniment of a song share the same ID.
    /// @param type the song resource type.
    /// @param vendorID Copyright music resource song copyright provider.
    ZEGO_DEPRECATED
    virtual bool queryCache(const std::string &songID, ZegoCopyrightedMusicType type,
                            ZegoCopyrightedMusicVendorID vendorID) = 0;
};

class IZegoRangeSceneStream {
  protected:
    virtual ~IZegoRangeSceneStream() {}

  public:
    /// set range scene stream event handler.
    ///
    /// Available since: 3.0.0
    /// Description: Set the callback function of the range scene stream module.
    /// When to call: After getting range scene stream instance by [getRangeSceneStream].
    ///
    /// @param handler The object used to receive range scene stream callbacks.
    /// @return The result of set range scene stream event handler, true: success, false: fail.
    virtual bool setEventHandler(std::shared_ptr<IZegoRangeSceneStreamEventHandler> handler) = 0;

    /// Set the receiving range of audio and video streams.
    ///
    /// Available: since 3.0.0
    /// Description: This interface will set the receiving range of audio and video streams, and ZEGO SDK will actively playing the streams of users in this range.
    /// When to call: After [getRangeSceneStream].
    /// Default value: The default value of receive range is 0.0 .
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param range The range distance from the center point of the person.
    /// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
    virtual int setReceiveRange(float range) = 0;

    /// Set the receiving range of audio and video streams.
    ///
    /// Available: since 3.7.0
    /// Description: This interface will set the receiving range of audio and video streams, and ZEGO SDK will actively playing the streams of users in this range.
    /// When to call: After [getRangeSceneStream].
    /// Default value: The default value of receive range is 0.0 .
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param param The configuration of audio receiving range.
    /// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
    virtual int setReceiveRange(ZegoReceiveRangeParam param) = 0;

    /// Turn 3D spatial sound on or off.
    ///
    /// Available: since 3.0.0
    /// Description: After it is turned on, the audio of non-team members in the world will change spatially with the distance and direction from the person.
    /// When to call: After [getRangeSceneStream].
    /// Default value: Disable.
    /// Restrictions: Media volume is required to use 3D spatial sound.
    /// Caution: None.
    ///
    /// @param enable Whether to turn 3D spatial sound on, true: enable, false: disable.
    /// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
    virtual int enableRangeSpatializer(bool enable) = 0;

    /// Set whether to receive the audio data of the specified user.
    ///
    /// Available: since 3.0.0
    /// Description: Set whether to receive the audio data of the specified user.
    /// When to call: After [LoginScene], before [LogoutScene].
    /// Default value: Receive.
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param userID User ID.
    /// @param mute true: do not receive the user's audio stream, false: receive the user's audio stream.
    /// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
    virtual int mutePlayAudio(const std::string &userID, bool mute) = 0;

    /// Set whether to receive the video data of the specified user.
    ///
    /// Available: since 3.0.0
    /// Description: After it is turned on, the audio of non-team members in the world will change spatially with the distance and direction from the person.
    /// When to call: After [LoginScene], before [LogoutScene].
    /// Default value: Receive.
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param userID User ID.
    /// @param mute true: do not receive the user's video stream, false: receive the user's video stream.
    /// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
    virtual int mutePlayVideo(const std::string &userID, bool mute) = 0;
};

class IZegoRangeSceneTeam {
  protected:
    virtual ~IZegoRangeSceneTeam() {}

  public:
    /// set range scene team event handler.
    ///
    /// Available since: 3.1.0
    /// Description: Set the callback function of the range scene team module.
    /// When to call: After getting range scene team instance by [getRangeSceneTeam].
    ///
    /// @param handler The object used to receive range scene team callbacks.
    /// @return The result of set range scene team event handler, true: success, false: fail.
    virtual bool setEventHandler(std::shared_ptr<IZegoRangeSceneTeamEventHandler> handler) = 0;

    /// Join team.
    ///
    /// Available since: 3.1.0
    /// Description: Join team.
    /// Use case: Often used in meta world scenarios, after join the same team, team members can see and hear each other.
    /// When to call: After login scene by [loginScene].
    /// Restrictions: None.
    ///
    /// @param config Team Param.
    /// @param callback The callback of join team.
    virtual void joinTeam(ZegoTeamParam config, ZegoRangeSceneJoinTeamCallback callback) = 0;

    /// Leave team.
    ///
    /// Available since: 3.1.0
    /// Description: Join team.
    /// Use case: Often used in meta world scenarios, after join the same team, team members can see and hear each other.
    /// When to call: After join team by [joinTeam].
    /// Restrictions: None.
    ///
    /// @param teamID Team ID.
    /// @param callback The callback of leave team.
    virtual void leaveTeam(unsigned int teamID, ZegoRangeSceneLeaveTeamCallback callback) = 0;
};

class IZegoRangeSceneItem {
  protected:
    virtual ~IZegoRangeSceneItem() {}

  public:
    /// set range scene item event handler.
    ///
    /// Available since: 3.1.0
    /// Description: Set the callback function of the range scene item module.
    /// When to call: After getting range scene item instance by [getRangeSceneItem].
    ///
    /// @param handler The object used to receive range scene item callbacks.
    /// @return The result of set range scene item event handler, true: success, false: fail.
    virtual bool setEventHandler(std::shared_ptr<IZegoRangeSceneItemEventHandler> handler) = 0;

    /// Create item.
    ///
    /// Available: since 3.1.0
    /// Description: Create item.
    /// Use cases: Item competition in the meta world.
    /// When to call: After receive login Scene success, before [logoutScene].
    /// Caution:
    ///   1. Items belong to range scene, not a certain user. When a user successfully binds an item, it only means that the user has the temporary right to use the item.
    ///   2. An item is allowed to have one or more bound users, and the principle of first-come, first-served is followed when applying for bound items.
    ///  3.  When multiple users bind an item at the same time, their changes to the item follow the cas principle.
    ///   4. When creating an item, it is allowed to specify whether to bind the item after the creation is successful.
    ///   5. When the item is created successfully, users within the range of the item will receive the [onItemEnterView] callback notification.
    /// Related APIs: Users can call [destroyItem] to destroy item.
    ///
    /// @param param Item param.
    /// @param callback The callback of create item.
    virtual void createItem(ZegoItemParam param, ZegoRangeSceneCreateItemCallback callback) = 0;

    /// Destroy item.
    ///
    /// Available: since 3.1.0
    /// Description: Destroy item.
    /// Use cases: Item competition in the meta world.
    /// When to call: After receive after login Scene success , before [logoutScene].
    /// Caution: When an item is destroyed, users within the range of the item will be notified by the [onItemLeaveView] callback.
    /// Related APIs: Users can call [createItem] to create item.
    ///
    /// @param itemID Item ID.
    /// @param callback The callback of destroy item.
    virtual void destroyItem(long long itemID, ZegoRangeSceneDestroyItemCallback callback) = 0;

    /// Bind item.
    ///
    /// Available: since 3.1.0
    /// Description: Bind item.
    /// Use cases: Item competition in the meta world.
    /// When to call: After receive after login Scene success, before [logoutScene].
    /// Caution: When an item is successfully bound, users within the range of the item will be notified by the [onItemBindUpdate] callback.
    /// Related APIs: Users can call [unbindItem] to unbind item.
    ///
    /// @param itemID Item ID.
    /// @param callback The callback of bind item.
    virtual void bindItem(long long itemID, ZegoRangeSceneBindItemCallback callback) = 0;

    /// Unbind item.
    ///
    /// Available: since 3.1.0
    /// Description: Unbind item.
    /// Use cases: Item competition in the meta world.
    /// When to call: After receive [bindItem], before [logoutScene].
    /// Caution: When an item is successfully unbound, users within the scope of the item will be notified by the [onItemUnbindUpdate] callback.
    /// Related APIs: Users can call [bindItem] to bind item.
    ///
    /// @param itemID Item ID.
    /// @param callback The callback of unbind item.
    virtual void unbindItem(long long itemID, ZegoRangeSceneUnbindItemCallback callback) = 0;

    /// Update item status.
    ///
    /// Available: since 3.1.0
    /// Description: Developers can call this function to update the item status.
    /// When to call: After [onBindItem], before [unbindItem].
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param itemID Item ID.
    /// @param position Item's current location.
    /// @param channel The channel to which the status belongs, starting from 0, cannot exceed the maximum channel number.
    /// @param status Current status data.
    /// @param statusLength Current status data length.
    /// @param callback The callback of update item status.
    virtual void updateItemStatus(long long itemID, ZegoPosition position, unsigned int channel,
                                  const unsigned char *status, unsigned int statusLength,
                                  ZegoRangeSceneUpdateItemStatusCallback callback) = 0;

    /// Update item status.
    ///
    /// Available: since 3.1.0
    /// Description: Developers can call this function to update the item command.
    /// When to call: After [onBindItem], before [unbindItem].
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param itemID Item ID.
    /// @param position Item's current location.
    /// @param channel The channel to which the status belongs, starting from 0, cannot exceed the maximum channel number.
    /// @param command Next command data.
    /// @param commandLength Next command data length.
    /// @param callback The callback of update item next command.
    virtual void updateItemCommand(long long itemID, ZegoPosition position, unsigned int channel,
                                   const unsigned char *command, unsigned int commandLength,
                                   ZegoRangeSceneUpdateItemCommandCallback callback) = 0;
};

class IZegoRangeScene {
  protected:
    virtual ~IZegoRangeScene() {}

  public:
    /// Get a range scene stream instance.
    ///
    /// Available since: 3.0.0
    /// Description: Get a range scene stream instance.
    /// Use case: Often used in meta world scenarios, users can use related functions by getting range scene stream instance objects.
    /// When to call: After creating the range scene via [createRangeScene], before destroying the range scene via [destroyRangeScene].
    /// Restrictions: None.
    ///
    /// @return range scene stream instance.
    virtual IZegoRangeSceneStream *getRangeSceneStream() = 0;

    /// Get a range scene team instance.
    ///
    /// Available since: 3.1.0
    /// Description: Get a range scene team instance.
    /// Use case: Often used in meta world scenarios, users can use related functions by getting range scene team instance objects.
    /// When to call: After creating the range scene via [createRangeScene], before destroying the range scene via [destroyRangeScene].
    /// Restrictions: None.
    ///
    /// @return range scene team instance.
    virtual IZegoRangeSceneTeam *getRangeSceneTeam() = 0;

    /// Get a range scene item instance.
    ///
    /// Available since: 3.1.0
    /// Description: Get a range scene item instance.
    /// Use case: Often used in meta world scenarios, users can use related functions by getting range scene item instance objects.
    /// When to call: After creating the range scene via [createRangeScene], before destroying the range scene via [destroyRangeScene].
    /// Restrictions: None.
    ///
    /// @return range scene item instance.
    virtual IZegoRangeSceneItem *getRangeSceneItem() = 0;

    /// Get range scene instance handle.
    ///
    /// Available since: 3.0.0
    /// Description: Get range scene instance handle.
    /// Use case: Used to publish stream to the scene.
    /// When to call: After creating the range scene via [createRangeScene], before destroying the range scene via [destroyRangeScene].
    /// Restrictions: None.
    ///
    /// @return Range scene instance handle.
    virtual int getRangeSceneHandle() = 0;

    /// set range scene event handler.
    ///
    /// Available since: 3.0.0
    /// Description: Set the callback function of the range scene module.
    /// When to call: After creating the range scene via [createRangeScene], before destroying the range scene via [destroyRangeScene].
    ///
    /// @param handler The object used to receive range scene callbacks.
    /// @return The result of set range scene event handler, true: success, false: fail.
    virtual bool setEventHandler(std::shared_ptr<IZegoRangeSceneEventHandler> handler) = 0;

    /// Logs in to a scene.
    ///
    /// Available: since 3.0.0
    /// Description: SDK RangeScene uses the 'scene' to organize users.
    /// Use cases: Users interact in the RangeScene within the same scene.
    /// When to call: After creating the range scene via [createRangeScene], before destroying the range scene via [destroyRangeScene].
    /// Caution:
    ///   1. Apps that use different appIDs cannot intercommunication with each other.
    ///   2. It is strongly recommended that userID corresponds to the user ID of the business APP, that is, a userID and a real user are fixed and unique, and should not be passed to the SDK in a random userID. Because the unique and fixed userID allows ZEGO technicians to quickly locate online problems.
    /// Privacy reminder: Please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
    /// Related callbacks:
    ///   1. When the user starts to log in to the scene, the scene is successfully logged in, or the scene fails to log in, the [onSceneStateUpdate] callback will be triggered to notify the developer of the status of the current user connected to the scene.
    ///   2. If the network is temporarily interrupted due to network quality reasons, the SDK will automatically reconnect internally. You can get the current connection status of the local scene by listening to the [onSceneStateUpdate] callback method.
    /// Related APIs:
    ///   1. Users can call [logoutScene] to log out.
    ///
    /// @param param Scene param.
    /// @param callback The callback of login scene.
    virtual void loginScene(ZegoSceneParam param, ZegoRangeSceneLoginSceneCallback callback) = 0;

    /// Logs out of a scene.
    ///
    /// Available: since 3.0.0
    /// Description: This API will log out the current user has logged in the scene.
    /// When to call: After successfully logging in to the scene, if the scene is no longer used, the user can call the function [logoutScene].
    /// Restrictions: None.
    /// Caution: Exiting a scene stops all publishing and playing stream associated with the scene for that user.
    /// Related callbacks: After calling this function, you will receive [onSceneStateUpdate] callback notification successfully exits the scene.
    /// Related APIs: Users can use [loginScene] functions to log in scene.
    ///
    /// @param callback The callback of logout scene.
    virtual void logoutScene(ZegoRangeSceneLogoutSceneCallback callback) = 0;

    /// Update user status.
    ///
    /// Available: since 3.0.0
    /// Description: Developers can call this function to update the user status.
    /// When to call: After [LoginScene], before [LogoutScene].
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param position User's current location.
    /// @param channel The channel to which the status belongs, starting from 0, cannot exceed the maximum channel number.
    /// @param status Current status data.
    /// @param statusLength Current status data length.
    /// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
    virtual int updateUserStatus(ZegoPosition position, unsigned int channel,
                                 const unsigned char *status, unsigned int statusLength) = 0;

    /// Update user status.
    ///
    /// Available: since 3.0.0
    /// Description: Developers can call this function to update the user command.
    /// When to call: After [LoginScene], before [LogoutScene].
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param position User's current location.
    /// @param channel The channel to which the command belongs, starting from 0, cannot exceed the maximum channel number.
    /// @param command Next command data.
    /// @param commandLength Next command data length.
    /// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
    virtual int updateUserCommand(ZegoPosition position, unsigned int channel,
                                  const unsigned char *command, unsigned int commandLength) = 0;

    /// Update user position.
    ///
    /// Available: since 3.0.0
    /// Description: Developers can call this function to update the user position.
    /// When to call: After [LoginScene], before [LogoutScene].
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param position User's current location.
    /// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
    virtual int updateUserPosition(ZegoPosition position) = 0;

    /// Get user count in scene.
    ///
    /// Available: since 3.0.0
    /// Description: Developers can call this function to get user count in scene.
    /// When to call: After [LoginScene], before [LogoutScene].
    /// Default value: NULL.
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param callback The callback of get user count in scene.
    virtual void getUserCount(ZegoRangeSceneGetUserCountCallback callback) = 0;

    /// Get user list in view.
    ///
    /// Available: since 3.0.0
    /// Description: Developers can call this function to get user list in view.
    /// When to call: After [LoginScene], before [LogoutScene].
    /// Default value: NULL.
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param callback The callback of get user list in view.
    virtual void getUserListInView(ZegoRangeSceneGetUserListInViewCallback callback) = 0;

    /// Send custom command.
    ///
    /// Available: since 3.0.0
    /// Description: Developers can call this function to send custom command.
    /// When to call: After [LoginScene], before [LogoutScene].
    /// Default value: NULL.
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param command Custom command.
    /// @param commandLength Custom command data length.
    /// @param callback The callback of send custom command.
    virtual void sendCustomCommand(const unsigned char *command, unsigned int commandLength,
                                   ZegoRangeSceneSendCustomCommandCallback callback) = 0;

    /// Renew token.
    ///
    /// Available since: 3.1.0
    /// Description: After the developer receives [onSceneTokenWillExpire], they can use this API to update the token to ensure that the subsequent RTC functions are normal.
    /// Use cases: Used when the token is about to expire.
    /// When to call /Trigger: After the developer receives [onSceneTokenWillExpire].
    /// Restrictions: None.
    ///
    /// @param token The token that needs to be renew.
    /// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
    virtual int renewToken(const std::string &token) = 0;

    /// Set range scene stream config.
    ///
    /// Available: since 3.2.0
    /// Description: Developers can call this function to set range scene stream config.
    /// When to call: After [createRangeScene].
    /// Default value: NULL.
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param config Range scene stream config.
    /// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
    virtual int setStreamConfig(ZegoSceneStreamConfig config) = 0;
};

class IZegoScreenCaptureSource {
  protected:
    virtual ~IZegoScreenCaptureSource() {}

  public:
    /// Sets up the screen capture source event handler.
    ///
    /// Available since: 3.1.0
    /// Description: Set up screen capture source callback to monitor callbacks such as capture data, etc.
    /// When to call:After create the [ZegoScreenCaptureSource] instance.
    /// Restrictions: None.
    /// Caution: Calling this function will overwrite the callback set by the last call to this function.
    ///
    /// @param handler Event handler for real-time sequential data.
    virtual void setEventHandler(std::shared_ptr<IZegoScreenCaptureSourceEventHandler> handler) = 0;

    /// Create the screen capture source
    ///
    /// Available: since 3.1.0
    /// Description: Update a screen capture source object based on the provided source ID and source type.
    /// Use cases: It is used when you need to record and share the screen or window.
    /// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
    /// Restrictions: Only available on Windows/macOS.
    ///
    /// @param sourceId The specified screen ID or window ID.
    /// @param sourceType The specified screen source type.
    virtual void updateCaptureSource(void *sourceId, ZegoScreenCaptureSourceType sourceType) = 0;

    /// Start screen capture.
    ///
    /// Available since: 3.1.0
    /// Description: Start screen capture.
    /// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
    virtual void startCapture() = 0;

    /// Stop screen capture.
    ///
    /// Available since: 3.1.0
    /// Description: Stop screen capture.
    virtual void stopCapture() = 0;

    /// Gets the rectangle of the screen capture source.
    ///
    /// Available since: 3.6.0
    /// Description: Gets the rectangle of the screen capture source.
    /// Restrictions: Called after starting [startScreenCapture] acquisition, only support in Windows/macOS.
    ///
    /// @return Rect information about the capture resource.
    virtual ZegoRect getCaptureSourceRect() = 0;

    /// Update the area captured by the screen.
    ///
    /// Available since: 3.1.0
    /// Description: Update the area captured by the screen.
    /// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
    /// Restrictions: Only support in Windows/macOS.
    ///
    /// @param rect The position of the area to be captured relative to the entire screen or window.
    virtual void updateCaptureRegion(ZegoRect rect) = 0;

    /// Update the streaming area of ​​the screen capture.
    ///
    /// Available since: 3.8.0
    /// Description: Update the streaming area of ​​the screen capture.
    /// When to call: After updating the screen capture source [updateScreenCaptureSource].
    /// Restrictions: Every time the screen capture source [updateScreenCaptureSource] is updated, the area will be cleared and needs to be reset, only support in Windows/macOS.
    /// Caution: The set area cannot exceed the size returned by [onAvailableFrame], otherwise the setting will fail and the original frame will be pushed. The failure of the streaming area will be notified through [onExceptionOccurred].
    ///
    /// @param rect The position of the area to be pushed relative to the upper left corner of the capture screen, the actual capture screen size can be obtained through [onAvailableFrame].
    virtual void updatePublishRegion(ZegoRect rect) = 0;

    /// Sets the filtered list of windows.
    ///
    /// Available since: 3.1.0
    /// Description: Specify a list of windows, and filter these windows when capturing the screen, and not display them on the screen.
    /// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
    /// Restrictions: Only available on Windows/macOS.
    ///
    /// @param list List of IDs to filter windows.
    /// @param count the number of lists.
    virtual void setExcludeWindowList(void **list, int count) = 0;

    /// Whether to activate the promotion of the window to the foreground.
    ///
    /// Available since: 3.1.0
    /// Description: When the capture target is a window, set whether to activate the window to be displayed in the foreground during the first capture.
    /// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
    /// Restrictions: Only available on Windows/macOS.
    ///
    /// @param active Whether to activate the window. true to activate the window, false to not activate the window, the default is true.
    virtual void enableWindowActivate(bool active) = 0;

    /// Set whether to show the cursor
    ///
    /// Available since: 3.1.0
    /// Description: Set whether to show the cursor.
    /// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
    /// Restrictions: Only available on Windows/macOS.
    ///
    /// @param visible Whether to show the cursor. true to show the cursor, false to not show the cursor, the default is false.
    virtual void enableCursorVisible(bool visible) = 0;

    /// Set whether to highlight the capture area
    ///
    /// Available since: 3.21.0
    /// Description: Set whether to highlight the capture area.
    /// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
    /// Restrictions: Only available on Windows/macOS.
    ///
    /// @param enable Whether to highlight the capture area. true to highlight, false to not highlight, the default is false.
    /// @param config Highlight capture area border configuration.
    virtual void enableHightLight(bool enable,
                                  ZegoLayerBorderConfig config = ZegoLayerBorderConfig()) = 0;

    /// Whether to collect the sound of the window process during window collection
    ///
    /// Available since: 3.13.0
    /// Description: Whether to collect the sound of the window process during window collection.
    /// When to call: Before starting the collection [startScreencapture].
    /// Restrictions: Only applicable to Windows 10 and above versions.
    ///
    /// @param enable Whether to collect sound. true for collection, false for no collection, default false.
    /// @param audioParam Audio collection parameters.
    virtual void enableAudioCapture(bool enable, ZegoAudioFrameParam audioParam) = 0;

    /// Get screen capture source index.
    ///
    /// @return Index of the screen capture source.
    virtual int getIndex() = 0;
};

class IZegoAIVoiceChanger {
  protected:
    virtual ~IZegoAIVoiceChanger() {}

  public:
    /// Get AI voice changer instance index.
    ///
    /// @return AI voice changer instance index.
    virtual int getIndex() = 0;

    /// Set event notification callback handler of the AI voice changer.
    ///
    /// Available since: 3.10.0
    /// Description: Listen to the event notification callback of the AI voice changer.
    /// Use Cases: Developers can handle this according to the related event callback.
    /// When to call: After the [ZegoAIVoiceChanger] instance created.
    /// Restrictions: None.
    /// Caution: Calling this function will overwrite the callback set by the last call to this function.
    ///
    /// @param handler Event callback handler for AI voice changer
    virtual void setEventHandler(std::shared_ptr<IZegoAIVoiceChangerEventHandler> handler) = 0;

    /// Initialize AI voice changer engine.
    virtual void initEngine() = 0;

    /// Update AI voice changer engine models.
    virtual void update() = 0;

    /// Get AI voice changer speaker list.
    virtual void getSpeakerList() = 0;

    /// Set AI voice changer speaker.
    ///
    /// @param speakerID Speaker ID.
    virtual void setSpeaker(int speakerID) = 0;
};

} // namespace EXPRESS
} // namespace ZEGO

#endif // __ZEGOEXPRESSINTERFACE_H__
