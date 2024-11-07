
#ifndef __ZEGO_EXPRESS_ROOM_H__
#define __ZEGO_EXPRESS_ROOM_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
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
/// @param room_id Room ID, a string of up to 128 bytes in length.
///   Caution:
///   1. room ID is defined by yourself.
///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
///   3. If you need to communicate with the Web SDK, please do not use '%'.
/// @param user User object instance, configure userID, userName. Note that the userID needs to be globally unique with the same appID, otherwise the user who logs in later will kick out the user who logged in first.
/// @param config Advanced room configuration.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_login_room(const char *room_id, struct zego_user user,
                                                        struct zego_room_config *config);
#else
typedef zego_error(EXP_CALL *pfnzego_express_login_room)(const char *room_id, struct zego_user user,
                                                         struct zego_room_config *config);
#endif

/// Log in to the room by configuring advanced properties, and return the login result through the callback parameter. You must log in to the room before pushing or pulling the stream.
///
/// Available since: 2.18.0
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
/// @param room_id Room ID, a string of up to 128 bytes in length.
///   Caution:
///   1. room ID is defined by yourself.
///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
///   3. If you need to communicate with the Web SDK, please do not use '%'.
/// @param user User object instance, configure userID, userName. Note that the userID needs to be globally unique with the same appID, otherwise the user who logs in later will kick out the user who logged in first.
/// @param config Advanced room configuration.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_login_room_with_callback(const char *room_id, struct zego_user user,
                                      struct zego_room_config *config, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_login_room_with_callback)(
    const char *room_id, struct zego_user user, struct zego_room_config *config,
    zego_seq *sequence);
#endif

/// Exit the room.
///
/// Available since: 2.9.0
/// Description: This API will log out the current user has logged in the room, if user logs in more than one room, all the rooms will be logged out.
/// Use cases: In the same room, users can conduct live broadcast, audio and video calls, etc.
/// When to call /Trigger: After successfully logging in to the room, if the room is no longer used, the user can call the function [logoutRoom].
/// Restrictions: None.
/// Caution: 1. Exiting the room will stop all publishing and playing streams for user, and inner audio and video engine will stop, and then SDK will auto stop local preview UI. If you want to keep the preview ability when switching rooms, please use the [switchRoom] method. 2. If the user is not logged in to the room, calling this function will also return success.
/// Related callbacks: After calling this function, you will receive [onRoomStateChanged] (Not supported before 2.18.0, please use [onRoomStateUpdate]) callback notification successfully exits the room, while other users in the same room will receive the [onRoomUserUpdate] callback notification(On the premise of enabling isUserStatusNotify configuration).
/// Related APIs: Users can use [loginRoom], [switchRoom] functions to log in or switch rooms.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_logout_all_room();
#else
typedef zego_error(EXP_CALL *pfnzego_express_logout_all_room)();
#endif

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
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_logout_all_room_with_callback(zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_logout_all_room_with_callback)(zego_seq *sequence);
#endif

/// Exit the room of the specified room ID.
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
/// @param room_id Room ID, a string of up to 128 bytes in length.
///   Caution:
///   1. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
///   2. If you need to communicate with the Web SDK, please do not use '%'.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_logout_room(const char *room_id);
#else
typedef zego_error(EXP_CALL *pfnzego_express_logout_room)(const char *room_id);
#endif

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
/// @param room_id Room ID, a string of up to 128 bytes in length.
///   Caution:
///   1. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
///   2. If you need to communicate with the Web SDK, please do not use '%'.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_logout_room_with_callback(const char *room_id,
                                                                       zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_logout_room_with_callback)(const char *room_id,
                                                                        zego_seq *sequence);
#endif

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
/// @param from_room_id Current roomID.
/// @param to_room_id The next roomID.
/// @param config Advanced room configuration.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_switch_room(const char *from_room_id,
                                                         const char *to_room_id,
                                                         struct zego_room_config *config);
#else
typedef zego_error(EXP_CALL *pfnzego_express_switch_room)(const char *from_room_id,
                                                          const char *to_room_id,
                                                          struct zego_room_config *config);
#endif

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
/// @param room_id Room ID.
/// @param token The token that needs to be renew.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_renew_token(const char *room_id, const char *token);
#else
typedef zego_error(EXP_CALL *pfnzego_express_renew_token)(const char *room_id, const char *token);
#endif

/// Set room extra information.
///
/// Available since: 1.13.0
/// Description: The user can call this function to set the extra info of the room.
/// Use cases: You can set some room-related business attributes, such as whether someone is Co-hosting.
/// When to call /Trigger: After logging in the room successful.
/// Restrictions: For restrictions on the use of this function, please refer to https://docs.zegocloud.com/article/7611 or contact ZEGO technical support.
/// Caution: 'key' is non NULL. The length of key and value is limited, please refer to Restrictions. The newly set key and value will overwrite the old setting.
/// Related callbacks: Other users in the same room will be notified through the [onRoomExtraInfoUpdate] callback function.
/// Related APIs: None.
///
/// @param room_id Room ID.
/// @param key key of the extra info.
/// @param value value if the extra info.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_set_room_extra_info(const char *room_id,
                                                                 const char *key, const char *value,
                                                                 zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_set_room_extra_info)(const char *room_id,
                                                                  const char *key,
                                                                  const char *value,
                                                                  zego_seq *sequence);
#endif

/// Get room stream list.
///
/// Available since: 3.12.0
/// Description: Get room stream list.
/// Use cases: Get room stream list.
/// When to call /Trigger: After logging in the room successful.
/// Caution: This interface is to get a real-time internal stream list, which may be inaccurate when the room is disconnected from the service. Do not call this interface with high frequency.
/// Related APIs: None.
///
/// @param room_id Room ID.
/// @param stream_list_type Get type
/// @param stream_list [in/out] Room Stream list.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_get_room_stream_list(
    const char *room_id, enum zego_room_stream_list_type stream_list_type,
    struct zego_room_stream_list **stream_list);
#else
typedef zego_error(EXP_CALL *pfnzego_express_get_room_stream_list)(
    const char *room_id, enum zego_room_stream_list_type stream_list_type,
    struct zego_room_stream_list **stream_list);
#endif

/// Frees the dynamic memory of the outgoing streamlist of the [getRoomStreamList] interface.
///
/// Available since: 3.12.0
/// Description: After calling [getRoomStreamList], you need to call this interface to release streamlist dynamic memory, otherwise memory leakage will occur.
/// When to call: After [getRoomStreamList].
///
/// @param stream_list  [getRoomStreamList] Outgoing streamList dynamic memory of the interface.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_free_room_stream_list(struct zego_room_stream_list *stream_list);
#else
typedef zego_error(EXP_CALL *pfnzego_express_free_room_stream_list)(
    struct zego_room_stream_list *stream_list);
#endif

/// Notification of the room connection state changes.
///
/// Available since: 1.1.0
/// Description: This callback is triggered when the connection status of the room changes, and the reason for the change is notified.For versions 2.18.0 and above, it is recommended to use the onRoomStateChanged callback instead of the onRoomStateUpdate callback to monitor room state changes.
/// Use cases: Developers can use this callback to determine the status of the current user in the room.
/// When to trigger:
///  1. The developer will receive this notification when calling the [loginRoom], [logoutRoom], [switchRoom] functions.
///  2. This notification may also be received when the network condition of the user's device changes (SDK will automatically log in to the room when disconnected, please refer to [Does ZEGO SDK support a fast reconnection for temporary disconnection] for details](https://docs.zegocloud.com/faq/reconnect?product=ExpressVideo&platform=all).
/// Restrictions: None.
/// Caution: If the connection is being requested for a long time, the general probability is that the user's network is unstable.
/// Related APIs: [loginRoom]、[logoutRoom]、[switchRoom]
///
/// @param room_id Room ID, a string of up to 128 bytes in length.
/// @param state Changed room state.
/// @param error_code Error code, For details, please refer to [Common Error Codes](https://docs.zegocloud.com/article/5548).
/// @param extended_data Extended Information with state updates. When the room login is successful, the key "room_session_id" can be used to obtain the unique RoomSessionID of each audio and video communication, which identifies the continuous communication from the first user in the room to the end of the audio and video communication. It can be used in scenarios such as call quality scoring and call problem diagnosis.
/// @param user_context context of user
typedef void (*zego_on_room_state_update)(const char *room_id, enum zego_room_state state,
                                          zego_error error_code, const char *extended_data,
                                          void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_room_state_update_callback(
    zego_on_room_state_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_room_state_update_callback)(
    zego_on_room_state_update callback_func, void *user_context);
#endif

/// Notification of the room connection state changes, including specific reasons for state change.
///
/// Available since: 2.18.0
/// Description: This callback is triggered when the connection status of the room changes, and the reason for the change is notified.For versions 2.18.0 and above, it is recommended to use the onRoomStateChanged callback instead of the onRoomStateUpdate callback to monitor room state changes.
/// Use cases: Developers can use this callback to determine the status of the current user in the room.
/// When to trigger: Users will receive this notification when they call room functions (refer to [Related APIs]). 2. This notification may also be received when the user device's network conditions change (SDK will automatically log in to the room again when the connection is disconnected, refer to https://doc-zh.zego.im/faq/reconnect ).
/// Restrictions: None.
/// Caution: If the connection is being requested for a long time, the general probability is that the user's network is unstable.
/// Related APIs: [loginRoom], [logoutRoom], [switchRoom]
///
/// @param room_id Room ID, a string of up to 128 bytes in length.
/// @param reason Room state change reason.
/// @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
/// @param extended_data Extended Information with state updates. When the room login is successful, the key "room_session_id" can be used to obtain the unique RoomSessionID of each audio and video communication, which identifies the continuous communication from the first user in the room to the end of the audio and video communication. It can be used in scenarios such as call quality scoring and call problem diagnosis.
/// @param user_context context of user
typedef void (*zego_on_room_state_changed)(const char *room_id,
                                           enum zego_room_state_changed_reason reason,
                                           zego_error error_code, const char *extended_data,
                                           void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_room_state_changed_callback(
    zego_on_room_state_changed callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_room_state_changed_callback)(
    zego_on_room_state_changed callback_func, void *user_context);
#endif

/// The callback triggered when the number of other users in the room increases or decreases.
///
/// Available since: 1.1.0
/// Description: When other users in the room are online or offline, which causes the user list in the room to change, the developer will be notified through this callback.
/// Use cases: Developers can use this callback to update the user list display in the room in real time.
/// When to trigger:
///   1. When the user logs in to the room for the first time, if there are other users in the room, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeAdd], and `userList` is the other users in the room at this time.
///   2. The user is already in the room. If another user logs in to the room through the [loginRoom] or [switchRoom] functions, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeAdd].
///   3. If other users log out of this room through the [logoutRoom] or [switchRoom] functions, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeDelete].
///   4. The user is already in the room. If another user is kicked out of the room from the server, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeDelete].
/// Restrictions: If developers need to use ZEGO room users notifications, please ensure that the [ZegoRoomConfig] sent by each user when logging in to the room has the [isUserStatusNotify] property set to true, otherwise the callback notification will not be received.
/// Related APIs: [loginRoom]、[logoutRoom]、[switchRoom]
///
/// @param room_id Room ID where the user is logged in, a string of up to 128 bytes in length.
/// @param update_type Update type (add/delete).
/// @param user_list List of users changed in the current room.
/// @param user_count List count of users changed in the current room.
/// @param user_context context of user.
typedef void (*zego_on_room_user_update)(const char *room_id, enum zego_update_type update_type,
                                         const struct zego_user *user_list, unsigned int user_count,
                                         void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL
zego_register_room_user_update_callback(zego_on_room_user_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_room_user_update_callback)(
    zego_on_room_user_update callback_func, void *user_context);
#endif

/// The callback triggered every 30 seconds to report the current number of online users.
///
/// Available since: 1.7.0
/// Description: This method will notify the user of the current number of online users in the room.
/// Use cases: Developers can use this callback to show the number of user online in the current room.
/// When to call /Trigger: After successfully logging in to the room.
/// Restrictions: None.
/// Caution: 1. This function is called back every 30 seconds. 2. Because of this design, when the number of users in the room exceeds 500, there will be some errors in the statistics of the number of online people in the room.
///
/// @param room_id Room ID where the user is logged in, a string of up to 128 bytes in length.
/// @param count Count of online users.
/// @param user_context context of user
typedef void (*zego_on_room_online_user_count_update)(const char *room_id, int count,
                                                      void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_room_online_user_count_update_callback(
    zego_on_room_online_user_count_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_room_online_user_count_update_callback)(
    zego_on_room_online_user_count_update callback_func, void *user_context);
#endif

/// The callback triggered when the number of streams published by the other users in the same room increases or decreases.
///
/// Available since: 1.1.0
/// Description: When other users in the room start publishing stream or stop publishing stream, the streaming list in the room changes, and the developer will be notified through this callback.
/// Use cases: This callback is used to monitor stream addition or stream deletion notifications of other users in the room. Developers can use this callback to determine whether other users in the same room start or stop publishing stream, so as to achieve active playing stream [startPlayingStream] or take the initiative to stop the playing stream [stopPlayingStream], and use it to change the UI controls at the same time.
/// When to trigger:
///   1. When the user logs in to the room for the first time, if there are other users publishing streams in the room, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeAdd], and `streamList` is an existing stream list.
///   2. The user is already in the room. if another user adds a new push, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeAdd].
///   3. The user is already in the room. If other users stop streaming, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeDelete].
///   4. The user is already in the room. If other users leave the room, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeDelete].
/// Restrictions: None.
///
/// @param room_id Room ID where the user is logged in, a string of up to 128 bytes in length.
/// @param update_type Update type (add/delete).
/// @param stream_list Updated stream list.
/// @param stream_info_count List count of streams changed in the current room.
/// @param extended_data Extended information with stream updates.When receiving a stream deletion notification, the developer can convert the string into a json object to get the stream_delete_reason field, which is an array of stream deletion reasons, and the stream_delete_reason[].code field may have the following values: 1 (the user actively stops publishing stream) ; 2 (user heartbeat timeout); 3 (user repeated login); 4 (user kicked out); 5 (user disconnected); 6 (removed by the server).
/// @param user_context context of user.
typedef void (*zego_on_room_stream_update)(const char *room_id, enum zego_update_type update_type,
                                           const struct zego_stream *stream_list,
                                           unsigned int stream_info_count,
                                           const char *extended_data, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_room_stream_update_callback(
    zego_on_room_stream_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_room_stream_update_callback)(
    zego_on_room_stream_update callback_func, void *user_context);
#endif

/// The callback triggered when there is an update on the extra information of the streams published by other users in the same room.
///
/// Available since: 1.1.0
/// Description: All users in the room will be notified by this callback when the extra information of the stream in the room is updated.
/// Use cases: Users can realize some business functions through the characteristics of stream extra information consistent with stream life cycle.
/// When to call /Trigger: When a user publishing the stream update the extra information of the stream in the same room, other users in the same room will receive the callback.
/// Restrictions: None.
/// Caution: Unlike the stream ID, which cannot be modified during the publishing process, the stream extra information can be updated during the life cycle of the corresponding stream ID.
/// Related APIs: Users who publish stream can set extra stream information through [setStreamExtraInfo].
///
/// @param room_id Room ID where the user is logged in, a string of up to 128 bytes in length.
/// @param stream_list List of streams that the extra info was updated.
/// @param stream_info_count List count of streams changed in the current room
/// @param user_context context of user
typedef void (*zego_on_room_stream_extra_info_update)(const char *room_id,
                                                      const struct zego_stream *stream_list,
                                                      unsigned int stream_info_count,
                                                      void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_room_stream_extra_info_update_callback(
    zego_on_room_stream_extra_info_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_room_stream_extra_info_update_callback)(
    zego_on_room_stream_extra_info_update callback_func, void *user_context);
#endif

/// The callback triggered when there is an update on the extra information of the room.
///
/// Available since: 1.1.0
/// Description: After the room extra information is updated, all users in the room will be notified except update the room extra information user.
/// Use cases: Extra information for the room.
/// When to call /Trigger: When a user update the room extra information, other users in the same room will receive the callback.
/// Restrictions: None.
/// Related APIs: Users can update room extra information through [setRoomExtraInfo] function.
///
/// @param room_id Room ID where the user is logged in, a string of up to 128 bytes in length.
/// @param room_extra_info_list List of the extra info updated.
/// @param room_extra_info_count List count of extra info changed in the current room
/// @param user_context context of user
typedef void (*zego_on_room_extra_info_update)(
    const char *room_id, const struct zego_room_extra_info *room_extra_info_list,
    unsigned int room_extra_info_count, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_room_extra_info_update_callback(
    zego_on_room_extra_info_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_room_extra_info_update_callback)(
    zego_on_room_extra_info_update callback_func, void *user_context);
#endif

/// Callback notification that room Token authentication is about to expire.
///
/// Available since: 2.8.0
/// Description: The callback notification that the room Token authentication is about to expire, please use [renewToken] to update the room Token authentication.
/// Use cases: In order to prevent illegal entry into the room, it is necessary to perform authentication control on login room, push streaming, etc., to improve security.
/// When to call /Trigger: 30 seconds before the Token expires, the SDK will call [onRoomTokenWillExpire] to notify developer.
/// Restrictions: None.
/// Caution: The token contains important information such as the user's room permissions, publish stream permissions, and effective time, please refer to https://docs.zegocloud.com/article/11649.
/// Related APIs: When the developer receives this callback, he can use [renewToken] to update the token authentication information.
///
/// @param room_id Room ID where the user is logged in, a string of up to 128 bytes in length.
/// @param remain_time_in_second The remaining time before the token expires.
/// @param user_context context of user
typedef void (*zego_on_room_token_will_expire)(const char *room_id, int remain_time_in_second,
                                               void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_room_token_will_expire_callback(
    zego_on_room_token_will_expire callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_room_token_will_expire_callback)(
    zego_on_room_token_will_expire callback_func, void *user_context);
#endif

/// Callback for setting room extra information.
///
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param room_id Room ID where the user is logged in, a string of up to 128 bytes in length.
/// @param key the key of the extra info
/// @param seq Message sequence.
/// @param user_context Context of user.
typedef void (*zego_on_room_set_room_extra_info_result)(zego_error error_code, const char *room_id,
                                                        const char *key, zego_seq seq,
                                                        void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_room_set_room_extra_info_result_callback(
    zego_on_room_set_room_extra_info_result callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_room_set_room_extra_info_result_callback)(
    zego_on_room_set_room_extra_info_result callback_func, void *user_context);
#endif

/// Login room result callback.
///
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param extended_data Extended Information
/// @param seq Message sequence.
/// @param user_context Context of user.
typedef void (*zego_on_room_login_result)(zego_error error_code, const char *extended_data,
                                          const char *room_id, zego_seq seq, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_room_login_result_callback(
    zego_on_room_login_result callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_room_login_result_callback)(
    zego_on_room_login_result callback_func, void *user_context);
#endif

/// Logout room result callback.
///
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param extended_data Extended Information
/// @param seq Message sequence.
/// @param user_context Context of user.
typedef void (*zego_on_room_logout_result)(zego_error error_code, const char *extended_data,
                                           const char *room_id, zego_seq seq, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_room_logout_result_callback(
    zego_on_room_logout_result callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_room_logout_result_callback)(
    zego_on_room_logout_result callback_func, void *user_context);
#endif

ZEGO_END_DECLS

#endif
