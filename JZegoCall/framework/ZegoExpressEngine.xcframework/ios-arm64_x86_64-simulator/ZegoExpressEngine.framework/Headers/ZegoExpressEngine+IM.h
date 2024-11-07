//
//  ZegoExpressEngine+IM.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (IM)

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
/// @return The real-time sequential data manager instance, nil will be returned when the maximum number is exceeded.
- (nullable ZegoRealTimeSequentialDataManager *)createRealTimeSequentialDataManager:
    (NSString *)roomID;

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
- (void)destroyRealTimeSequentialDataManager:(ZegoRealTimeSequentialDataManager *)manager;

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
/// @param message The content of the message. Required: Yes. Value range: The length does not exceed 1024 bytes.
/// @param roomID Room ID, a string of up to 128 bytes in length.
///   Caution:
///   1. room ID is defined by yourself.
///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
///   3. If you need to communicate with the Web SDK, please do not use '%'.
/// @param callback Send a notification of the result of a broadcast message. Required: No. Caution: Passing [nil] means not receiving callback notifications.
- (void)sendBroadcastMessage:(NSString *)message
                      roomID:(NSString *)roomID
                    callback:(nullable ZegoIMSendBroadcastMessageCallback)callback;

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
/// @param message The content of the message. Required: Yes. Value range: The length does not exceed 1024 bytes.
/// @param roomID Room ID, a string of up to 128 bytes in length.
///   Caution:
///   1. room ID is defined by yourself.
///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
///   3. If you need to communicate with the Web SDK, please do not use '%'.
/// @param callback Send barrage message result callback.Required: No. Caution: Passing [nil] means not receiving callback notifications.
- (void)sendBarrageMessage:(NSString *)message
                    roomID:(NSString *)roomID
                  callback:(nullable ZegoIMSendBarrageMessageCallback)callback;

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
/// @param command Custom command content. Required: Yes. Value range: The maximum length is 1024 bytes. Caution: To protect privacy, please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
/// @param toUserList List of recipients of signaling. Required: Yes. Value range: user list or [nil]. Caution: When it is [nil], the SDK will send custom signaling back to all users in the room
/// @param roomID Room ID, a string of up to 128 bytes in length.
///   Caution:
///   1. room ID is defined by yourself.
///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
///   3. If you need to communicate with the Web SDK, please do not use '%'.
/// @param callback Send a notification of the signaling result. Required: No. Caution: Passing [nil] means not receiving callback notifications.
- (void)sendCustomCommand:(NSString *)command
               toUserList:(nullable NSArray<ZegoUser *> *)toUserList
                   roomID:(NSString *)roomID
                 callback:(nullable ZegoIMSendCustomCommandCallback)callback;

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
/// @param message ZegoRoomSendTransparentMessage
/// @param roomID Room ID, a string of up to 128 bytes in length.
///   Caution:
///   1. room ID is defined by yourself.
///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
///   3. If you need to communicate with the Web SDK, please do not use '%'.
/// @param callback Send a notification of the signaling result. Required: No. Caution: Passing [nil] means not receiving callback notifications.
- (void)sendTransparentMessage:(ZegoRoomSendTransparentMessage *)message
                        roomID:(NSString *)roomID
                      callback:(nullable ZegoRoomSendTransparentMessageCallback)callback;

@end

NS_ASSUME_NONNULL_END
