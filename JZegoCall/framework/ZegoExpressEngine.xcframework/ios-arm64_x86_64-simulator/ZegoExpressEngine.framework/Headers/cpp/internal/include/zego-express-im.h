
#ifndef __ZEGO_EXPRESS_IM_H__
#define __ZEGO_EXPRESS_IM_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Create the real time sequential data manager instance
///
/// Available: since 2.14.0
/// Description: Create a manager object for sending/receiving real-time signaling.
/// Use cases: You can use this function when you need to use real-time signaling to implement services such as cloud games and remote desktops.
/// When to call: After logging in to a room.
/// Restrictions: Only one corresponding manager object can be created for each room ID of [loginRoom], that is, only one object can be created in single room mode, and multiple objects can be created in multi-room mode.
/// Caution: None.
///
/// @param room_id Fill in the room ID that has been logged in, and all related stuff will be do in this room.
/// @param instance_index [in/out] The real-time sequential data manager instance index, -1 will be returned when the maximum number is exceeded.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_create_real_time_sequential_data_manager(const char *room_id, int *instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_create_real_time_sequential_data_manager)(
    const char *room_id, int *instance_index);
#endif

/// Destroy the real time sequential data manager instance
///
/// Available: since 2.14.0
/// Description: Destroy the [ZegoRealTimeSequentialDataManager] instance object.
/// Use cases: When you no longer need to use the real-time signaling function, you can use this function to destroy the instance object created by the [createRealTimeSequentialDataManager] function
/// When to call: When you need to the real-time signaling manager object needs to be destroyed
/// Restrictions: After destroy the instance, you need to release the [ZegoRealTimeSequentialDataManager] instance object you hold by yourself, and don’t call the function of this instance object after the destruction.
/// Caution: None.
///
/// @param instance_index The real time sequential data manager instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_destroy_real_time_sequential_data_manager(int instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_destroy_real_time_sequential_data_manager)(
    int instance_index);
#endif

/// Start broadcasting real-time sequential data stream.
///
/// Available since: 2.14.0
/// Description: This function allows users to broadcast their local real-time sequential data stream to the ZEGO RTC server, and other users in the same room can subscribe to the real-time sequential data stream for intercommunication through "streamID".
/// Use cases: Before sending real-time sequential data, you need to call this function to start broadcasting.
/// When to call: After creating the [ZegoRealTimeSequentialDataManager] instance.
/// Restrictions: None.
/// Caution: After calling this function, you will receive the [onPublisherStateUpdate] callback to tell you the broadcast state (publish state) of this stream. After the broadcast is successful, other users in the same room will receive the [onRoomStreamUpdate] callback to tell them this stream has been added to the room.
///
/// @param stream_id Stream ID, a string of up to 256 characters.
///   Caution:
///   1. Need to be globally unique within the entire AppID (Note that it cannot be the same as the stream ID passed in [startPublishingStream]). If in the same AppID, different users publish each stream and the stream ID is the same, which will cause the user to publish the stream failure. You cannot include URL keywords, otherwise publishing stream and playing stream will fails.
///   2. Only support numbers, English characters and '-', '_'.
/// @param instance_index The real time sequential data manager instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_real_time_sequential_data_start_broadcasting(
    const char *stream_id, int instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_real_time_sequential_data_start_broadcasting)(
    const char *stream_id, int instance_index);
#endif

/// Stop broadcasting real-time sequential data stream.
///
/// Available since: 2.14.0
/// Description: This function allows users to stop broadcasting their local real-time sequential data stream.
/// Use cases: When you no longer need to send real-time sequential data, you need to call this function to stop broadcasting.
/// When to call: After creating the [ZegoRealTimeSequentialDataManager] instance.
/// Restrictions: None.
/// Caution: After calling this function, you will receive the [onPublisherStateUpdate] callback to tell you the broadcast state (publish state) of this stream. After stopping the broadcast, other users in the same room will receive the [onRoomStreamUpdate] callback to tell them this stream has been deleted from the room.
///
/// @param stream_id The ID of the stream that needs to stop broadcasting.
/// @param instance_index The real time sequential data manager instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_real_time_sequential_data_stop_broadcasting(const char *stream_id, int instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_real_time_sequential_data_stop_broadcasting)(
    const char *stream_id, int instance_index);
#endif

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
/// @param data_length Data content length.
/// @param stream_id The stream ID to which the real-time sequential data is sent.
/// @param instance_index The real time sequential data manager instance index.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_send_real_time_sequential_data(
    const unsigned char *data, unsigned int data_length, const char *stream_id, int instance_index,
    zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_send_real_time_sequential_data)(
    const unsigned char *data, unsigned int data_length, const char *stream_id, int instance_index,
    zego_seq *sequence);
#endif

/// Start subscribing real-time sequential data stream.
///
/// Available since: 2.14.0
/// Description: This function allows users to subscribe to the real-time sequential data stream of remote users from the ZEGO RTC server.
/// Use cases: When you need to receive real-time sequential data sent from other remote users, you need to call this function to start subscribing to the stream broadcasted by other remote users.
/// When to call: After creating the [ZegoRealTimeSequentialDataManager] instance.
/// Restrictions: None.
/// Caution: After calling this function, you will receive the [onPlayerStateUpdate] callback to tell you the subscribe state (play state) of this stream.
///
/// @param stream_id Stream ID, a string of up to 256 characters.
///   Caution:
///   Only support numbers, English characters and '-', '_'.
/// @param instance_index The real time sequential data manager instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_real_time_sequential_data_start_subscribing(const char *stream_id, int instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_real_time_sequential_data_start_subscribing)(
    const char *stream_id, int instance_index);
#endif

/// Stop subscribing real-time sequential data stream.
///
/// Available since: 2.14.0
/// Description: This function can be used to stop subscribing to the real-time sequential data stream.
/// Use cases: When you no longer need to receive real-time sequential data sent by other users, you need to call this function to stop subscribing to the other user's stream.
/// When to call: After creating the [ZegoRealTimeSequentialDataManager] instance.
/// Restrictions: None.
/// Caution: After calling this function, you will receive the [onPlayerStateUpdate] callback to tell you the subscribe state (play state) of this stream.
///
/// @param stream_id The ID of the stream that needs to stop subscribing.
/// @param instance_index The real time sequential data manager instance index.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL
zego_express_real_time_sequential_data_stop_subscribing(const char *stream_id, int instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_real_time_sequential_data_stop_subscribing)(
    const char *stream_id, int instance_index);
#endif

/// Callback for receiving real-time sequential data.
///
/// Available since: 2.14.0
/// Description: Through this callback, you can receive real-time sequential data from the current subscribing stream.
/// Use cases: You need to listen to this callback when you need to receive real-time sequential data.
/// When to trigger: After calling [startSubscribing] to successfully start the subscription, and when data is sent on the stream, this callback will be triggered.
/// Restrictions: None.
/// Caution: None.
///
/// @param manager The real time sequential data manager instance index.
/// @param data The received real-time sequential data.
/// @param data_length Data content length.
/// @param stream_id Subscribed stream ID
/// @param user_context Context of user.
typedef void (*zego_on_receive_real_time_sequential_data)(int manager, const unsigned char *data,
                                                          unsigned int data_length,
                                                          const char *stream_id,
                                                          void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_receive_real_time_sequential_data_callback(
    zego_on_receive_real_time_sequential_data callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_receive_real_time_sequential_data_callback)(
    zego_on_receive_real_time_sequential_data callback_func, void *user_context);
#endif

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
/// @param room_id Room ID, a string of up to 128 bytes in length.
///   Caution:
///   1. room ID is defined by yourself.
///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
///   3. If you need to communicate with the Web SDK, please do not use '%'.
/// @param message The content of the message. Required: Yes. Value range: The length does not exceed 1024 bytes.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_send_broadcast_message(const char *room_id,
                                                                    const char *message,
                                                                    zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_send_broadcast_message)(const char *room_id,
                                                                     const char *message,
                                                                     zego_seq *sequence);
#endif

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
/// @param room_id Room ID, a string of up to 128 bytes in length.
///   Caution:
///   1. room ID is defined by yourself.
///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
///   3. If you need to communicate with the Web SDK, please do not use '%'.
/// @param message The content of the message. Required: Yes. Value range: The length does not exceed 1024 bytes.
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_send_barrage_message(const char *room_id,
                                                                  const char *message,
                                                                  zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_send_barrage_message)(const char *room_id,
                                                                   const char *message,
                                                                   zego_seq *sequence);
#endif

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
/// @param room_id Room ID, a string of up to 128 bytes in length.
///   Caution:
///   1. room ID is defined by yourself.
///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
///   3. If you need to communicate with the Web SDK, please do not use '%'.
/// @param command Custom command content. Required: Yes. Value range: The maximum length is 1024 bytes. Caution: To protect privacy, please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
/// @param to_user_list List of recipients of signaling. Required: Yes. Value range: user list or [NULL]. Caution: When it is [NULL], the SDK will send custom signaling back to all users in the room
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_send_custom_command(const char *room_id,
                                                                 const char *command,
                                                                 struct zego_user *to_user_list,
                                                                 unsigned int to_user_count,
                                                                 zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_send_custom_command)(const char *room_id,
                                                                  const char *command,
                                                                  struct zego_user *to_user_list,
                                                                  unsigned int to_user_count,
                                                                  zego_seq *sequence);
#endif

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
/// @param room_id Room ID, a string of up to 128 bytes in length.
///   Caution:
///   1. room ID is defined by yourself.
///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
///   3. If you need to communicate with the Web SDK, please do not use '%'.
/// @param message ZegoRoomSendTransparentMessage
/// @param sequence [in/out] Context that identifies which invocation triggered this callback.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_send_transparent_message(
    const char *room_id, struct zego_room_send_transparent_message *message, zego_seq *sequence);
#else
typedef zego_error(EXP_CALL *pfnzego_express_send_transparent_message)(
    const char *room_id, struct zego_room_send_transparent_message *message, zego_seq *sequence);
#endif

/// The callback triggered when Barrage Messages are received.
///
/// Available since: 1.5.0
/// Description: This callback is used to receive room passthrough messages sent by other users in the same room.
/// When to trigger: After calling [loginRoom] to login to the room, this callback is triggered if there is a user in the room who sends a message received by the specified client through the [sendTransparentMessage] function.
/// Restrictions: None
/// Caution: Barrage messages sent by users themselves will not be notified through this callback. When there are a large number of barrage messages in the room, the notification may be delayed, and some barrage messages may be lost.
/// Related callbacks: A bullet-screen message sent by the user himself is not notified by this callback. [sendTransparentMessage] specifies that only a server callback is used. This callback is not triggered.
///
/// @param room_id Room ID. Value range: The maximum length is 128 bytes.
/// @param message recv message.
/// @param user_context User context. Value range: It is a custom pointer, which can be [NULL].
typedef void (*zego_on_recv_room_transparent_message)(
    const char *room_id, const struct zego_room_recv_transparent_message *message,
    void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_recv_room_transparent_message_callback(
    zego_on_recv_room_transparent_message callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_recv_room_transparent_message_callback)(
    zego_on_recv_room_transparent_message callback_func, void *user_context);
#endif

/// The callback triggered when Broadcast Messages are received.
///
/// Available since: 1.2.1
/// Description: This callback is used to receive broadcast messages sent by other users in the same room.
/// Use cases: Generally used when the number of people in the live room does not exceed 500
/// When to trigger: After calling [loginRoom] to log in to the room, if a user in the room sends a broadcast message via [sendBroadcastMessage] function, this callback will be triggered.
/// Restrictions: None
/// Caution: The broadcast message sent by the user will not be notified through this callback.
/// Related callbacks: You can receive room barrage messages through [onIMRecvBarrageMessage], and you can receive room custom signaling through [onIMRecvCustomCommand].
///
/// @param room_id Room ID. Value range: The maximum length is 128 bytes.
/// @param message_list List of received messages. Value range: Up to 50 messages can be received each time.
/// @param message_count The number of message lists. Value range: the maximum value is 50.
/// @param user_context User context. Value range: It is a custom pointer, which can be [NULL].
typedef void (*zego_on_im_recv_broadcast_message)(
    const char *room_id, const struct zego_broadcast_message_info *message_list,
    unsigned int message_count, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_im_recv_broadcast_message_callback(
    zego_on_im_recv_broadcast_message callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_im_recv_broadcast_message_callback)(
    zego_on_im_recv_broadcast_message callback_func, void *user_context);
#endif

/// The callback triggered when Barrage Messages are received.
///
/// Available since: 1.5.0
/// Description: This callback is used to receive barrage messages sent by other users in the same room.
/// Use cases: Generally used in scenarios where there is a large number of messages sent and received in the room and the reliability of the messages is not required, such as live barrage.
/// When to trigger: After calling [loginRoom] to log in to the room, if a user in the room sends a barrage message through the [sendBarrageMessage] function, this callback will be triggered.
/// Restrictions: None
/// Caution: Barrage messages sent by users themselves will not be notified through this callback. When there are a large number of barrage messages in the room, the notification may be delayed, and some barrage messages may be lost.
/// Related callbacks: Develop can receive room broadcast messages through [onIMRecvBroadcastMessage], and can receive room custom signaling through [onIMRecvCustomCommand].
///
/// @param room_id Room ID. Value range: The maximum length is 128 bytes.
/// @param message_list List of received messages. Value range: Up to 50 messages can be received each time.
/// @param message_count The number of message lists. Value range: the maximum value is 50.
/// @param user_context User context. Value range: It is a custom pointer, which can be [NULL].
typedef void (*zego_on_im_recv_barrage_message)(
    const char *room_id, const struct zego_barrage_message_info *message_list,
    unsigned int message_count, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_im_recv_barrage_message_callback(
    zego_on_im_recv_barrage_message callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_im_recv_barrage_message_callback)(
    zego_on_im_recv_barrage_message callback_func, void *user_context);
#endif

/// The callback triggered when a Custom Command is received.
///
/// Available since: 1.2.1
/// Description: This callback is used to receive custom command sent by other users in the same room.
/// Use cases: Generally used when the number of people in the live room does not exceed 500
/// When to trigger: After calling [loginRoom] to log in to the room, if other users in the room send custom signaling to the developer through the [sendCustomCommand] function, this callback will be triggered.
/// Restrictions: None
/// Caution: The custom command sent by the user himself will not be notified through this callback.
/// Related callbacks: You can receive room broadcast messages through [onIMRecvBroadcastMessage], and you can receive room barrage message through [onIMRecvBarrageMessage].
///
/// @param room_id Room ID. Value range: The maximum length is 128 bytes.
/// @param from_user Sender of the command.
/// @param command Command content received.Value range: The maximum length is 1024 bytes.
/// @param user_context User context. Value range: It is a custom pointer, which can be [NULL].
typedef void (*zego_on_im_recv_custom_command)(const char *room_id, struct zego_user from_user,
                                               const char *command, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_im_recv_custom_command_callback(
    zego_on_im_recv_custom_command callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_im_recv_custom_command_callback)(
    zego_on_im_recv_custom_command callback_func, void *user_context);
#endif

/// Callback for sending real-time sequential data.
///
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param instance_index The real time sequential data manager instance index.
/// @param seq Sequence of sending broadcast message.
/// @param user_context Context of user.
typedef void (*zego_on_real_time_sequential_data_sent)(zego_error error_code, int instance_index,
                                                       zego_seq seq, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_real_time_sequential_data_sent_callback(
    zego_on_real_time_sequential_data_sent callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_real_time_sequential_data_sent_callback)(
    zego_on_real_time_sequential_data_sent callback_func, void *user_context);
#endif

/// Callback for sending broadcast messages.
///
/// @param room_id Room ID.
/// @param message_id ID of this message
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param seq Sequence of sending broadcast message.
/// @param user_context Context of user.
typedef void (*zego_on_im_send_broadcast_message_result)(const char *room_id,
                                                         unsigned long long message_id,
                                                         zego_error error_code, zego_seq seq,
                                                         void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_im_send_broadcast_message_result_callback(
    zego_on_im_send_broadcast_message_result callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_im_send_broadcast_message_result_callback)(
    zego_on_im_send_broadcast_message_result callback_func, void *user_context);
#endif

/// Callback for sending barrage message.
///
/// @param room_id Room ID.
/// @param message_id ID of this message
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param seq Sequence of sending broadcast message.
/// @param user_context Context of user.
typedef void (*zego_on_im_send_barrage_message_result)(const char *room_id, const char *message_id,
                                                       zego_error error_code, zego_seq seq,
                                                       void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_im_send_barrage_message_result_callback(
    zego_on_im_send_barrage_message_result callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_im_send_barrage_message_result_callback)(
    zego_on_im_send_barrage_message_result callback_func, void *user_context);
#endif

/// Callback for sending custom command.
///
/// @param room_id Room ID.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param seq Sequence of sending broadcast message.
/// @param user_context Context of user.
typedef void (*zego_on_im_send_custom_command_result)(const char *room_id, zego_error error_code,
                                                      zego_seq seq, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_im_send_custom_command_result_callback(
    zego_on_im_send_custom_command_result callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_im_send_custom_command_result_callback)(
    zego_on_im_send_custom_command_result callback_func, void *user_context);
#endif

/// Callback for sending custom command.
///
/// @param room_id Room ID.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param seq Sequence of sending broadcast message.
/// @param user_context Context of user.
typedef void (*zego_on_send_transparent_message_result)(const char *room_id, zego_error error_code,
                                                        zego_seq seq, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_send_transparent_message_result_callback(
    zego_on_send_transparent_message_result callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_send_transparent_message_result_callback)(
    zego_on_send_transparent_message_result callback_func, void *user_context);
#endif

ZEGO_END_DECLS

#endif
