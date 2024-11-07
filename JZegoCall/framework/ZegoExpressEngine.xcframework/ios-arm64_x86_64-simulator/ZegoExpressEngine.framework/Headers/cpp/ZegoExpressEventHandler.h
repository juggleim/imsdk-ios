
#ifndef __ZEGOEXPRESSHANDLERS_H__
#define __ZEGOEXPRESSHANDLERS_H__

#include "ZegoExpressDefines.h"

namespace ZEGO {
namespace EXPRESS {

class IZegoMediaPlayer;
class IZegoAudioEffectPlayer;
class IZegoRangeAudio;
class IZegoRangeScene;
class IZegoRealTimeSequentialDataManager;
class IZegoCopyrightedMusic;
class IZegoScreenCaptureSource;
class IZegoMediaDataPublisher;
class IZegoAIVoiceChanger;

class IZegoEventHandler {
  protected:
    virtual ~IZegoEventHandler() {}

  public:
    /// The callback for obtaining debugging error information.
    ///
    /// Available since: 1.1.0
    /// Description: When the SDK functions are not used correctly, the callback prompts for detailed error information.
    /// Trigger: Notify the developer when an exception occurs in the SDK.
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
    /// @param funcName Function name.
    /// @param info Detailed error information.
    virtual void onDebugError(int /*errorCode*/, const std::string & /*funcName*/,
                              const std::string & /*info*/) {}

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
    virtual void onEngineStateUpdate(ZegoEngineState /*state*/) {}

    /// Experimental API callback
    ///
    /// Available since: 2.7.0
    /// Description: Receive experimental API callbacks in JSON string format.
    /// Caution: Please use this feature with the help of ZEGO Technical Support.
    ///
    /// @param content Callback content in JSON string format.
    virtual void onRecvExperimentalAPI(const std::string & /*content*/) {}

    /// The callback that triggered a fatal error, causing the SDK to malfunction and unable to function properly.
    ///
    /// Available since: 3.6.0
    /// Description: The callback that triggered a fatal error.
    /// Trigger: The APP has enabled the restriction of access from foreign IP addresses, and the current client is outside of the domain.
    /// Restrictions: None.
    /// Caution:
    ///   None.
    ///
    /// @param errorCode Error code.
    virtual void onFatalError(int /*errorCode*/) {}

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
    /// @param roomID Room ID, a string of up to 128 bytes in length.
    /// @param state Changed room state.
    /// @param errorCode Error code, For details, please refer to [Common Error Codes](https://docs.zegocloud.com/article/5548).
    /// @param extendedData Extended Information with state updates. When the room login is successful, the key "room_session_id" can be used to obtain the unique RoomSessionID of each audio and video communication, which identifies the continuous communication from the first user in the room to the end of the audio and video communication. It can be used in scenarios such as call quality scoring and call problem diagnosis.
    virtual void onRoomStateUpdate(const std::string & /*roomID*/, ZegoRoomState /*state*/,
                                   int /*errorCode*/, const std::string & /*extendedData*/) {}

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
    /// @param roomID Room ID, a string of up to 128 bytes in length.
    /// @param reason Room state change reason.
    /// @param errorCode Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
    /// @param extendedData Extended Information with state updates. When the room login is successful, the key "room_session_id" can be used to obtain the unique RoomSessionID of each audio and video communication, which identifies the continuous communication from the first user in the room to the end of the audio and video communication. It can be used in scenarios such as call quality scoring and call problem diagnosis.
    virtual void onRoomStateChanged(const std::string & /*roomID*/,
                                    ZegoRoomStateChangedReason /*reason*/, int /*errorCode*/,
                                    const std::string & /*extendedData*/) {}

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
    /// @param roomID Room ID where the user is logged in, a string of up to 128 bytes in length.
    /// @param updateType Update type (add/delete).
    /// @param userList List of users changed in the current room.
    virtual void onRoomUserUpdate(const std::string & /*roomID*/, ZegoUpdateType /*updateType*/,
                                  const std::vector<ZegoUser> & /*userList*/) {}

    /// The callback triggered every 30 seconds to report the current number of online users.
    ///
    /// Available since: 1.7.0
    /// Description: This method will notify the user of the current number of online users in the room.
    /// Use cases: Developers can use this callback to show the number of user online in the current room.
    /// When to call /Trigger: After successfully logging in to the room.
    /// Restrictions: None.
    /// Caution: 1. This function is called back every 30 seconds. 2. Because of this design, when the number of users in the room exceeds 500, there will be some errors in the statistics of the number of online people in the room.
    ///
    /// @param roomID Room ID where the user is logged in, a string of up to 128 bytes in length.
    /// @param count Count of online users.
    virtual void onRoomOnlineUserCountUpdate(const std::string & /*roomID*/, int /*count*/) {}

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
    /// @param roomID Room ID where the user is logged in, a string of up to 128 bytes in length.
    /// @param updateType Update type (add/delete).
    /// @param streamList Updated stream list.
    /// @param extendedData Extended information with stream updates.When receiving a stream deletion notification, the developer can convert the string into a json object to get the stream_delete_reason field, which is an array of stream deletion reasons, and the stream_delete_reason[].code field may have the following values: 1 (the user actively stops publishing stream) ; 2 (user heartbeat timeout); 3 (user repeated login); 4 (user kicked out); 5 (user disconnected); 6 (removed by the server).
    virtual void onRoomStreamUpdate(const std::string & /*roomID*/, ZegoUpdateType /*updateType*/,
                                    const std::vector<ZegoStream> & /*streamList*/,
                                    const std::string & /*extendedData*/) {}

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
    /// @param roomID Room ID where the user is logged in, a string of up to 128 bytes in length.
    /// @param streamList List of streams that the extra info was updated.
    virtual void onRoomStreamExtraInfoUpdate(const std::string & /*roomID*/,
                                             const std::vector<ZegoStream> & /*streamList*/) {}

    /// The callback triggered when there is an update on the extra information of the room.
    ///
    /// Available since: 1.1.0
    /// Description: After the room extra information is updated, all users in the room will be notified except update the room extra information user.
    /// Use cases: Extra information for the room.
    /// When to call /Trigger: When a user update the room extra information, other users in the same room will receive the callback.
    /// Restrictions: None.
    /// Related APIs: Users can update room extra information through [setRoomExtraInfo] function.
    ///
    /// @param roomID Room ID where the user is logged in, a string of up to 128 bytes in length.
    /// @param roomExtraInfoList List of the extra info updated.
    virtual void
    onRoomExtraInfoUpdate(const std::string & /*roomID*/,
                          const std::vector<ZegoRoomExtraInfo> & /*roomExtraInfoList*/) {}

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
    /// @param roomID Room ID where the user is logged in, a string of up to 128 bytes in length.
    /// @param remainTimeInSecond The remaining time before the token expires.
    virtual void onRoomTokenWillExpire(const std::string & /*roomID*/, int /*remainTimeInSecond*/) {

    }

    /// The callback triggered when the state of stream publishing changes.
    ///
    /// Available since: 1.1.0
    /// Description: After calling the [startPublishingStream] successfully, the notification of the publish stream state change can be obtained through the callback function. You can roughly judge the user's uplink network status based on whether the state parameter is in [PUBLISH_REQUESTING].
    /// Caution: The parameter [extendedData] is extended information with state updates. If you use ZEGO's CDN content distribution network, after the stream is successfully published, the keys of the content of this parameter are [flv_url_list], [rtmp_url_list], [hls_url_list], these correspond to the publishing stream URLs of the flv, rtmp, and hls protocols.
    /// Related callbacks: After calling the [startPlayingStream] successfully, the notification of the play stream state change can be obtained through the callback function [onPlayerStateUpdate]. You can roughly judge the user's downlink network status based on whether the state parameter is in [PLAY_REQUESTING].
    ///
    /// @param streamID Stream ID.
    /// @param state State of publishing stream.
    /// @param errorCode The error code corresponding to the status change of the publish stream, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
    /// @param extendedData Extended information with state updates, include playing stream CDN address.
    virtual void onPublisherStateUpdate(const std::string & /*streamID*/,
                                        ZegoPublisherState /*state*/, int /*errorCode*/,
                                        const std::string & /*extendedData*/) {}

    /// Callback for current stream publishing quality.
    ///
    /// Available since: 1.1.0
    /// Description: After calling the [startPublishingStream] successfully, the callback will be received every 3 seconds default(If you need to change the time, please contact the instant technical support to configure). Through the callback, the collection frame rate, bit rate, RTT, packet loss rate and other quality data of the published audio and video stream can be obtained, and the health of the publish stream can be monitored in real time.You can monitor the health of the published audio and video streams in real time according to the quality parameters of the callback function, in order to show the uplink network status in real time on the device UI.
    /// Caution: If you does not know how to use the parameters of this callback function, you can only pay attention to the [level] field of the [quality] parameter, which is a comprehensive value describing the uplink network calculated by SDK based on the quality parameters.
    /// Related callbacks: After calling the [startPlayingStream] successfully, the callback [onPlayerQualityUpdate] will be received every 3 seconds. You can monitor the health of play streams in real time based on quality data such as frame rate, code rate, RTT, packet loss rate, etc.
    ///
    /// @param streamID Stream ID.
    /// @param quality Publishing stream quality, including audio and video framerate, bitrate, RTT, etc.
    virtual void onPublisherQualityUpdate(const std::string & /*streamID*/,
                                          const ZegoPublishStreamQuality & /*quality*/) {}

    /// The callback triggered when the first audio frame is captured.
    ///
    /// Available since: 1.1.0
    /// Description: This callback will be received when the SDK starts the microphone to capture the first frame of audio data. If this callback is not received, the audio capture device is occupied or abnormal.
    /// Trigger: When the engine of the audio/video module inside the SDK starts, the SDK will go and collect the audio data from the local device and will receive the callback at that time.
    /// Related callbacks: Determine if the SDK actually collected video data by the callback function [onPublisherCapturedVideoFirstFrame], determine if the SDK has rendered the first frame of video data collected by calling back [onPublisherRenderVideoFirstFrame].
    virtual void onPublisherCapturedAudioFirstFrame() {}

    /// The callback triggered when the first video frame is captured.
    ///
    /// Available since: 1.1.0
    /// Description: The SDK will receive this callback when the first frame of video data is captured. If this callback is not received, the video capture device is occupied or abnormal.
    /// Trigger: When the SDK's internal audio/video module's engine starts, the SDK will collect video data from the local device and will receive this callback.
    /// Related callbacks: Determine if the SDK actually collected audio data by the callback function [onPublisherCapturedAudioFirstFrame], determine if the SDK has rendered the first frame of video data collected by calling back [onPublisherRenderVideoFirstFrame].
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param channel Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
    virtual void onPublisherCapturedVideoFirstFrame(ZegoPublishChannel /*channel*/) {}

    /// The callback triggered when the first audio frame is sent.
    ///
    /// Available since: 3.5.0
    /// Description: After the [startPublishingStream] function is called successfully, this callback will be called when SDK received the first frame of audio data. Developers can use this callback to determine whether SDK has actually collected audio data. If the callback is not received, the audio capture device is occupied or abnormal.
    /// Trigger: In the case of no startPublishingStream audio stream, the first startPublishingStream audio stream, it will receive this callback.
    /// Related callbacks: After the [startPublishingStream] function is called successfully, determine if the SDK actually collected video data by the callback function [onPublisherCapturedVideoFirstFrame], determine if the SDK has rendered the first frame of video data collected by calling back [onPublisherRenderVideoFirstFrame].
    ///
    /// @param channel Publishing stream channel.If you only publish one audio stream, you can ignore this parameter.
    virtual void onPublisherSendAudioFirstFrame(ZegoPublishChannel /*channel*/) {}

    /// The callback triggered when the first video frame is sent.
    ///
    /// Available since: 3.5.0
    /// Description: After the [startPublishingStream] function is called successfully, this callback will be called when SDK received the first frame of video data. Developers can use this callback to determine whether SDK has actually collected video data. If the callback is not received, the video capture device is occupied or abnormal.
    /// Trigger: In the case of no startPublishingStream video stream, the first startPublishingStream video stream, it will receive this callback.
    /// Related callbacks: After the [startPublishingStream] function is called successfully, determine if the SDK actually collected audio data by the callback function [onPublisherCapturedAudioFirstFrame], determine if the SDK has rendered the first frame of video data collected by calling back [onPublisherRenderVideoFirstFrame].
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param channel Publishing stream channel.If you only publish one video stream, you can ignore this parameter.
    virtual void onPublisherSendVideoFirstFrame(ZegoPublishChannel /*channel*/) {}

    /// The callback triggered when the first video frame is rendered.
    ///
    /// Available since: 2.4.0
    /// Description: this callback will be called after SDK rendered the first frame of video data captured. This interface is for preview rendering. The first frame callback is only available for external collection and internal preview. If it is not for SDK rendering, there is no such callback.
    /// Related callbacks: After the [startPublishingStream] function is called successfully, determine if the SDK actually collected audio data by the callback function [onPublisherCapturedAudioFirstFrame], determine if the SDK actually collected video data by the callback function [onPublisherCapturedVideoFirstFrame].
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param channel Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
    virtual void onPublisherRenderVideoFirstFrame(ZegoPublishChannel /*channel*/) {}

    /// The callback triggered when the video capture resolution changes.
    ///
    /// Available since: 1.1.0
    /// Description: When the audio and video stream is not published [startPublishingStream] or previewed [startPreview] for the first time, the publishing stream or preview first time, that is, the engine of the audio and video module inside the SDK is started, the video data of the local device will be collected, and the collection resolution will change at this time.
    /// Trigger: After the successful publish [startPublishingStream], the callback will be received if there is a change in the video capture resolution in the process of publishing the stream.
    /// Use cases: You can use this callback to remove the cover of the local preview UI and similar operations.You can also dynamically adjust the scale of the preview view based on the resolution of the callback.
    /// Caution: What is notified during external collection is the change in encoding resolution, which will be affected by flow control.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param width Video capture resolution width.
    /// @param height Video capture resolution height.
    /// @param channel Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
    virtual void onPublisherVideoSizeChanged(int /*width*/, int /*height*/,
                                             ZegoPublishChannel /*channel*/) {}

    /// The callback triggered when the state of relayed streaming to CDN changes.
    ///
    /// Available since: 1.1.0
    /// Description: Developers can use this callback to determine whether the audio and video streams of the relay CDN are normal. If they are abnormal, further locate the cause of the abnormal audio and video streams of the relay CDN and make corresponding disaster recovery strategies.
    /// Trigger: After the ZEGO RTC server relays the audio and video streams to the CDN, this callback will be received if the CDN relay status changes, such as a stop or a retry.
    /// Caution: If you do not understand the cause of the abnormality, you can contact ZEGO technicians to analyze the specific cause of the abnormality.
    ///
    /// @param streamID Stream ID.
    /// @param infoList List of information that the current CDN is relaying.
    virtual void
    onPublisherRelayCDNStateUpdate(const std::string & /*streamID*/,
                                   const std::vector<ZegoStreamRelayCDNInfo> & /*infoList*/) {}

    /// The callback triggered when the video encoder changes in publishing stream.
    ///
    /// Available since: 2.12.0
    /// Description: After the H.265 automatic downgrade policy is enabled, if H.265 encoding is not supported or the encoding fails during the streaming process with H.265 encoding, the SDK will actively downgrade to the specified encoding (H.264), and this callback will be triggered at this time.
    /// When to trigger: In the process of streaming with H.265 encoding, if H.265 encoding is not supported or encoding fails, the SDK will actively downgrade to the specified encoding (H.264), and this callback will be triggered at this time.
    /// Caution: When this callback is triggered, if local video recording or cloud recording is in progress, multiple recording files will be generated. Developers need to collect all the recording files for processing after the recording ends. When this callback is triggered, because the streaming code has changed, the developer can evaluate whether to notify the streaming end, so that the streaming end can deal with it accordingly.
    ///
    /// @param fromCodecID Video codec ID before the change.
    /// @param toCodecID Video codec ID after the change.
    /// @param channel Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
    virtual void onPublisherVideoEncoderChanged(ZegoVideoCodecID /*fromCodecID*/,
                                                ZegoVideoCodecID /*toCodecID*/,
                                                ZegoPublishChannel /*channel*/) {}

    /// The callback triggered when publishing stream.
    ///
    /// Available since: 2.18.0
    /// Description: After start publishing stream, this callback will return the current stream address, resource type and protocol-related information.
    /// When to trigger: Publish and retry publish events.
    /// Caution: None.
    ///
    /// @param eventID Publish stream event ID
    /// @param streamID Stream ID.
    /// @param extraInfo extra info. it is in JSON format. Included information includes "url" for address, "streamProtocol" for stream protocol, including rtmp, flv, avertp, hls, webrtc, etc. "netProtocol" for network protocol, including tcp, udp, quic, "resourceType" for resource type , including cdn, rtc, l3.
    virtual void onPublisherStreamEvent(ZegoStreamEvent /*eventID*/,
                                        const std::string & /*streamID*/,
                                        const std::string & /*extraInfo*/) {}

    /// The video object segmentation state changed.
    ///
    /// Available since: 3.4.0
    /// Description: The object segmentation state of the stream publishing end changes.
    /// When to trigger: When [enableVideoObjectSegmentation] enables or disables object segmentation, notify the developer whether to enable object segmentation according to the actual state.
    /// Caution: This callback depends on enabling preview or stream publishing.
    ///
    /// @param state Object segmentation state.
    /// @param channel Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
    /// @param errorCode The error code corresponding to the status change of the object segmentation, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
    virtual void onVideoObjectSegmentationStateChanged(ZegoObjectSegmentationState /*state*/,
                                                       ZegoPublishChannel /*channel*/,
                                                       int /*errorCode*/) {}

    /// Video encoding low frame rate warning.
    ///
    /// Available since: 3.8.0
    /// Description: Video encoding low frame rate warning.
    /// When to trigger: This callback triggered by low frame rate in video encoding.
    /// Caution: This callback is disabled by default, if necessary, please contact ZEGO technical support.
    ///
    /// @param codecID Video codec ID.
    /// @param channel Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
    virtual void onPublisherLowFpsWarning(ZegoVideoCodecID /*codecID*/,
                                          ZegoPublishChannel /*channel*/) {}

    /// The notification for setting the path of the static image displayed when the camera is turned off is incorrect.
    ///
    /// Available since: 3.9.0
    /// Description: The notification for setting the path of the static image displayed when the camera is turned off is incorrect.
    /// When to trigger: If the path for the image is set using [setDummyCaptureImagePath], but the image cannot be obtained during streaming, this callback will be triggered.
    /// Caution: Please make sure that the image path is correct and has read permission before setting it.
    ///
    /// @param errorCode error code.
    /// @param path Image path.
    /// @param channel Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
    virtual void onPublisherDummyCaptureImagePathError(int /*errorCode*/,
                                                       const std::string & /*path*/,
                                                       ZegoPublishChannel /*channel*/) {}

    /// The callback triggered when the state of stream playing changes.
    ///
    /// Available since: 1.1.0
    /// Description: After calling the [startPlayingStream] successfully, the notification of the playing stream state change can be obtained through the callback function. You can roughly judge the user's downlink network status based on whether the state parameter is in [PLAY_REQUESTING].
    /// When to trigger:  After calling the [startPublishingStream], this callback is triggered when a playing stream's state changed.
    /// Related callbacks: After calling the [startPublishingStream] successfully, the notification of the publish stream state change can be obtained through the callback function [onPublisherStateUpdate]. You can roughly judge the user's uplink network status based on whether the state parameter is in [PUBLISH_REQUESTING].
    ///
    /// @param streamID stream ID.
    /// @param state State of playing stream.
    /// @param errorCode The error code corresponding to the status change of the playing stream, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
    /// @param extendedData Extended Information with state updates. As the standby, only an empty json table is currently returned.
    virtual void onPlayerStateUpdate(const std::string & /*streamID*/, ZegoPlayerState /*state*/,
                                     int /*errorCode*/, const std::string & /*extendedData*/) {}

    /// Switch playing stream callback.
    ///
    /// Available since: 3.16.0
    /// Description: After calling the [switchPlayingStream] successfully, the result of switching streams can be obtained through this callback function.
    /// When to trigger:  After calling the [switchPlayingStream], this callback is triggered when a switch stream request ultimately succeeds or fails.
    /// Related callbacks: After the stream switching is successful or failed, you can obtain the current streaming status through the callback function [onPlayerStateUpdate].
    ///
    /// @param streamID The stream ID currently playing.
    /// @param errorCode The error code corresponding to the result of the switch stream, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
    virtual void onPlayerSwitched(const std::string & /*streamID*/, int /*errorCode*/) {}

    /// Callback for current stream playing quality.
    ///
    /// Available since: 1.1.0
    /// Description: After calling the [startPlayingStream] successfully, the callback will be received every 3 seconds default(If you need to change the time, please contact the instant technical support to configure). Through the callback, the collection frame rate, bit rate, RTT, packet loss rate and other quality data can be obtained, and the health of the played audio and video streams can be monitored in real time.
    /// Use cases: You can monitor the health of the played audio and video streams in real time according to the quality parameters of the callback function, in order to show the downlink network status on the device UI in real time.
    /// Caution: If you does not know how to use the various parameters of the callback function, you can only focus on the level field of the quality parameter, which is a comprehensive value describing the downlink network calculated by SDK based on the quality parameters.
    /// Related callbacks: After calling the [startPublishingStream] successfully, a callback [onPublisherQualityUpdate] will be received every 3 seconds. You can monitor the health of publish streams in real time based on quality data such as frame rate, code rate, RTT, packet loss rate, etc.
    ///
    /// @param streamID Stream ID.
    /// @param quality Playing stream quality, including audio and video framerate, bitrate, RTT, etc.
    virtual void onPlayerQualityUpdate(const std::string & /*streamID*/,
                                       const ZegoPlayStreamQuality & /*quality*/) {}

    /// The callback triggered when a media event occurs during streaming playing.
    ///
    /// Available since: 1.1.0
    /// Description: This callback is used to receive pull streaming events.
    /// Use cases: You can use this callback to make statistics on stutters or to make friendly displays in the UI of the app.
    /// When to trigger:  After calling the [startPublishingStream], this callback is triggered when an event such as audio and video jamming and recovery occurs in the playing stream.
    ///
    /// @param streamID Stream ID.
    /// @param event Specific events received when playing the stream.
    virtual void onPlayerMediaEvent(const std::string & /*streamID*/,
                                    ZegoPlayerMediaEvent /*event*/) {}

    /// The callback triggered when the first audio frame is received.
    ///
    /// Available since: 1.1.0
    /// Description: After the [startPlayingStream] function is called successfully, this callback will be called when SDK received the first frame of audio data.
    /// Use cases: Developer can use this callback to count time consuming that take the first frame time or update the UI for playing stream.
    /// Trigger: This callback is triggered when SDK receives the first frame of audio data from the network.
    /// Related callbacks: After a successful call to [startPlayingStream], the callback function [onPlayerRecvVideoFirstFrame] determines whether the SDK has received the video data, and the callback [onPlayerRenderVideoFirstFrame] determines whether the SDK has rendered the first frame of the received video data.
    ///
    /// @param streamID Stream ID.
    virtual void onPlayerRecvAudioFirstFrame(const std::string & /*streamID*/) {}

    /// The callback triggered when the first video frame is received.
    ///
    /// Available since: 1.1.0
    /// Description: After the [startPlayingStream] function is called successfully, this callback will be called when SDK received the first frame of video data.
    /// Use cases: Developer can use this callback to count time consuming that take the first frame time or update the UI for playing stream.
    /// Trigger: This callback is triggered when SDK receives the first frame of video data from the network.
    /// Related callbacks: After a successful call to [startPlayingStream], the callback function [onPlayerRecvAudioFirstFrame] determines whether the SDK has received the audio data, and the callback [onPlayerRenderVideoFirstFrame] determines whether the SDK has rendered the first frame of the received video data.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param streamID Stream ID.
    virtual void onPlayerRecvVideoFirstFrame(const std::string & /*streamID*/) {}

    /// The callback triggered when the first video frame is received. Please do not call the SDK interface in the callback thread.
    ///
    /// Available since: 3.8.0
    /// Description: After the [startPlayingStream] function is called successfully, this callback will be called when SDK received the first frame of video data.
    /// Use cases: Developer can use this callback to count time consuming that take the first frame time.
    /// Trigger: This callback is triggered when SDK receives the first frame of video data from the network.
    /// Related callbacks: After a successful call to [startPlayingStream], the callback function [onPlayerRecvAudioFirstFrame] determines whether the SDK has received the audio data, and the callback [onPlayerRenderVideoFirstFrame] determines whether the SDK has rendered the first frame of the received video data.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param streamID Stream ID.
    virtual void onPlayerSyncRecvVideoFirstFrame(const std::string & /*streamID*/) {}

    /// The callback triggered when the first video frame is rendered.
    ///
    /// Available since: 1.1.0
    /// Description: After the [startPlayingStream] function is called successfully, this callback will be called when SDK rendered the first frame of video data.
    /// Use cases: Developer can use this callback to count time consuming that take the first frame time or update the UI for playing stream.
    /// Trigger: This callback is triggered when SDK rendered the first frame of video data from the network.
    /// Related callbacks: After a successful call to [startPlayingStream], the callback function [onPlayerRecvAudioFirstFrame] determines whether the SDK has received the audio data, and the callback [onPlayerRecvVideoFirstFrame] determines whether the SDK has received the video data.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param streamID Stream ID.
    virtual void onPlayerRenderVideoFirstFrame(const std::string & /*streamID*/) {}

    /// Calls back when the stream playing end renders the first frame of the video from the remote camera.
    ///
    /// Available since: 3.0.0
    /// Description: After calling the [startPlayingStream] function to pull the stream successfully, the SDK will receive this callback after pulling the stream and rendering the first frame of remote camera video data.
    /// Use cases: Developer can use this callback to count time consuming that take the first frame time or update the UI for playing stream.
    /// Trigger: After the remote [enableCamera] enables the camera, or after [mutePublishStreamVideo] is true and starts to send video data, the SDK will receive this callback after playing the stream and rendering the first frame of the remote camera video data.
    /// Caution: It is only applicable when the remote end uses the camera to push the stream. Only applicable to RTC publishing and playing streaming scenarios.
    /// Related callbacks: After a successful call to [startPlayingStream], the callback [onPlayerRecvVideoFirstFrame] determines whether the SDK has received the video data.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param streamID Stream ID.
    virtual void onPlayerRenderCameraVideoFirstFrame(const std::string & /*streamID*/) {}

    /// The callback triggered when the stream playback resolution changes.
    ///
    /// Available since: 1.1.0
    /// Description: After the [startPlayingStream] function is called successfully, the play resolution will change when the first frame of video data is received, or when the publisher changes the encoding resolution by calling [setVideoConfig], or when the network traffic control strategies work.
    /// Use cases: Developers can update or switch the UI components that actually play the stream based on the final resolution of the stream.
    /// Trigger: After the [startPlayingStream] function is called successfully, this callback is triggered when the video resolution changes while playing the stream.
    /// Caution: If the stream is only audio data, the callback will not be triggered.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param streamID Stream ID.
    /// @param width Video decoding resolution width.
    /// @param height Video decoding resolution height.
    virtual void onPlayerVideoSizeChanged(const std::string & /*streamID*/, int /*width*/,
                                          int /*height*/) {}

    /// The callback triggered when Supplemental Enhancement Information is received.
    ///
    /// Available since: 1.1.0
    /// Description: After the [startPlayingStream] function is called successfully, when the remote stream sends SEI (such as directly calling [sendSEI], audio mixing with SEI data, and sending custom video capture encoded data with SEI, etc.), the local end will receive this callback.
    /// Trigger: After the [startPlayingStream] function is called successfully, when the remote stream sends SEI, the local end will receive this callback.
    /// Caution:
    ///  1.The customer can directly operate the UI control in this callback function.
    ///  2. Since the video encoder itself generates an SEI with a payload type of 5, or when a video file is used for publishing, such SEI may also exist in the video file. Therefore, if the developer needs to filter out this type of SEI, it can be before [createEngine] Call [ZegoEngineConfig.advancedConfig("unregister_sei_filter", "XXXXX")]. Among them, unregister_sei_filter is the key, and XXXXX is the uuid filter string to be set.
    ///  3. When [mutePlayStreamVideo] or [muteAllPlayStreamVideo] is called to set only the audio stream to be pulled, the SEI will not be received.
    ///
    /// @deprecated It will be deprecated in version 3.4.0 and above. Please use the [onPlayerSyncRecvSEI] function instead.
    /// @param streamID Stream ID.
    /// @param data SEI content.
    /// @param dataLength SEI content length.
    ZEGO_DEPRECATED
    virtual void onPlayerRecvSEI(const std::string & /*streamID*/, const unsigned char * /*data*/,
                                 unsigned int /*dataLength*/) {}

    /// The callback triggered when Supplemental Enhancement Information is received synchronously.
    ///
    /// Available since: 3.4.0
    /// Description: After the [startPlayingStream] function is called successfully, when the remote stream sends SEI (such as directly calling [sendSEI], audio mixing with SEI data, and sending custom video capture encoded data with SEI, etc.), the local end will receive this callback.
    /// Trigger: After the [startPlayingStream] function is called successfully, when the remote stream sends SEI, the local end will receive this callback.
    /// Caution:
    ///  1. Since the video encoder itself generates an SEI with a payload type of 5, or when a video file is used for publishing, such SEI may also exist in the video file. Therefore, if the developer needs to filter out this type of SEI, it can be before [createEngine] Call [ZegoEngineConfig.advancedConfig("unregister_sei_filter", "XXXXX")]. Among them, unregister_sei_filter is the key, and XXXXX is the uuid filter string to be set.
    ///  2. When [mutePlayStreamVideo] or [muteAllPlayStreamVideo] is called to set only the audio stream to be pulled, the SEI will not be received.
    ///
    /// @param streamID Stream ID.
    /// @param data SEI content.
    /// @param dataLength SEI content length.
    virtual void onPlayerSyncRecvSEI(const std::string & /*streamID*/,
                                     const unsigned char * /*data*/, unsigned int /*dataLength*/) {}

    /// The callback triggered when Supplemental Enhancement Information is received synchronously.
    ///
    /// Available since: 3.9.0
    /// Description: After the [startPlayingStream] function is called successfully, when the remote stream sends SEI (such as directly calling [sendSEI], audio mixing with SEI data, and sending custom video capture encoded data with SEI, etc.), the local end will receive this callback.
    /// Trigger: After the [startPlayingStream] function is called successfully, when the remote stream sends SEI, the local end will receive this callback.
    /// Caution: 1. Since the video encoder itself generates an SEI with a payload type of 5, or when a video file is used for publishing, such SEI may also exist in the video file. Therefore, if the developer needs to filter out this type of SEI, it can be before [createEngine] Call [ZegoEngineConfig.advancedConfig("unregister_sei_filter", "XXXXX")]. Among them, unregister_sei_filter is the key, and XXXXX is the uuid filter string to be set. 2. When [mutePlayStreamVideo] or [muteAllPlayStreamVideo] is called to set only the audio stream to be pulled, the SEI will not be received.
    ///
    /// @param info SEI Callback info.
    virtual void onPlayerRecvMediaSideInfo(const ZegoMediaSideInfo & /*info*/) {}

    /// Receive the audio side information content of the remote stream.
    ///
    /// Available since: 2.19.0
    /// Description: After the [startPlayingStream] function is called successfully, when the remote stream sends audio side information, the local end will receive this callback.
    /// Trigger: After the [startPlayingStream] function is called successfully, when the remote stream sends audio side information, the local end will receive this callback.
    /// Caution: 1. When [mutePlayStreamAudio] or [muteAllPlayStreamAudio] is called to set only the video stream to be pulled, the audio side information not be received. 2. Due to factors such as the network, the received data may be missing, but the order is guaranteed.
    /// Related APIs: Send audio side information by the [sendAudioSideInfo] function.
    ///
    /// @param streamID Stream ID.
    /// @param data Audio side information content.
    /// @param dataLength Audio side information content length.
    virtual void onPlayerRecvAudioSideInfo(const std::string & /*streamID*/,
                                           const unsigned char * /*data*/,
                                           unsigned int /*dataLength*/) {}

    /// Playing stream low frame rate warning.
    ///
    /// Available since: 2.14.0
    /// Description: This callback triggered by low frame rate when playing stream.
    /// When to trigger: This callback triggered by low frame rate when playing stream.
    /// Caution: If the callback is triggered when the user playing the h.265 stream, you can stop playing the h.265 stream and switch to play the H.264 stream.
    ///
    /// @param codecID Video codec ID.
    /// @param streamID Stream ID.
    virtual void onPlayerLowFpsWarning(ZegoVideoCodecID /*codecID*/,
                                       const std::string & /*streamID*/) {}

    /// The callback triggered when playing stream.
    ///
    /// Available since: 2.18.0
    /// Description: After start playing stream, this callback will return the current stream address, resource type and protocol-related information.
    /// When to trigger: Play and retry play events.
    /// Caution: None.
    ///
    /// @param eventID Play stream event ID
    /// @param streamID Stream ID.
    /// @param extraInfo extra info. it is in JSON format. Included information includes "url" for address, "streamProtocol" for stream protocol, including rtmp, flv, avertp, hls, webrtc, etc. "netProtocol" for network protocol, including tcp, udp, quic, "resourceType" for resource type , including cdn, rtc, l3.
    virtual void onPlayerStreamEvent(ZegoStreamEvent /*eventID*/, const std::string & /*streamID*/,
                                     const std::string & /*extraInfo*/) {}

#if TARGET_OS_IPHONE || defined(ANDROID)
    /// Playing stream video super resolution enabled state changes.
    ///
    /// Available since: 3.0.0
    /// Description: Playing stream video super resolution enabled state changes.
    /// When to trigger: When [enableVideoSuperResolution] enables or disables video super resolution, the developer will be notified whether to enable video super resolution according to the actual situation when playing stream video rendering.
    /// Caution: None.
    ///
    /// @param streamID Stream ID.
    /// @param state Video super resolution state.
    /// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
    virtual void onPlayerVideoSuperResolutionUpdate(const std::string & /*streamID*/,
                                                    ZegoSuperResolutionState /*state*/,
                                                    int /*errorCode*/) {}
#endif

    /// The callback triggered when the state of relayed streaming of the mixed stream to CDN changes.
    ///
    /// Available since: 1.2.1
    /// Description: The general situation of the mixing task on the ZEGO RTC server will push the output stream to the CDN using the RTMP protocol, and the state change during the push process will be notified from the callback function.
    /// Use cases: It is often used when multiple video images are required to synthesize a video using mixed streaming, such as education, live teacher and student images.
    /// When to trigger: After the developer calls the [startMixerTask] function to start mixing, when the ZEGO RTC server pushes the output stream to the CDN, there is a state change.
    /// Restrictions: None.
    /// Related callbacks: Develop can get the sound update notification of each single stream in the mixed stream through [OnMixerSoundLevelUpdate].
    /// Related APIs: Develop can start a mixed flow task through [startMixerTask].
    ///
    /// @param taskID The mixing task ID. Value range: the length does not exceed 256. Caution: This parameter is in string format and cannot contain URL keywords, such as 'http' and '?' etc., otherwise the push and pull flow will fail. Only supports numbers, English characters and'~','!','@','$','%','^','&','*','(',')','_' ,'+','=','-','`',';',''',',','.','<','>','\'.
    /// @param infoList List of information that the current CDN is being mixed.
    virtual void
    onMixerRelayCDNStateUpdate(const std::string & /*taskID*/,
                               const std::vector<ZegoStreamRelayCDNInfo> & /*infoList*/) {}

    /// The callback triggered when the sound level of any input stream changes in the stream mixing process.
    ///
    /// Available since: 1.2.1
    /// Description: Developers can use this callback to display the effect of which stream’s anchor is talking on the UI interface of the mixed stream of the audience.
    /// Use cases: It is often used when multiple video images are required to synthesize a video using mixed streaming, such as education, live teacher and student images.
    /// When to trigger: After the developer calls the [startPlayingStream] function to start playing the mixed stream. Callback notification period is 100 ms.
    /// Restrictions: The callback is triggered every 100 ms, and the trigger frequency cannot be set.Due to the high frequency of this callback, please do not perform time-consuming tasks or UI operations in this callback to avoid stalling.
    /// Related callbacks: [OnMixerRelayCDNStateUpdate] can be used to get update notification of mixing stream repost CDN status.
    /// Related APIs: Develop can start a mixed flow task through [startMixerTask].
    ///
    /// @param soundLevels The sound key-value pair of each single stream in the mixed stream, the key is the soundLevelID of each single stream, and the value is the sound value of the corresponding single stream. Value range: The value range of value is 0.0 ~ 100.0 (This value only represents the range of the sound level value returned by the callback, not the precision.).
    virtual void
    onMixerSoundLevelUpdate(const std::unordered_map<unsigned int, float> & /*soundLevels*/) {}

    /// The callback triggered when the sound level of any input stream changes in the auto stream mixing process.
    ///
    /// Available since: 2.10.0
    /// Description: According to this callback, user can obtain the sound level information of each stream pulled during auto stream mixing.
    /// Use cases: Often used in voice chat room scenarios.Users can use this callback to show which streamer is speaking when an audience pulls a mixed stream.
    /// Trigger: Call [startPlayingStream] function to pull the stream.
    /// Related APIs: Users can call [startAutoMixerTask] function to start an auto stream mixing task.Users can call [stopAutoMixerTask] function to stop an auto stream mixing task.
    ///
    /// @param soundLevels Sound level hash map, key is the streamID of every single stream in this mixer stream, value is the sound level value of that single stream, value ranging from 0.0 to 100.0 (This value only represents the range of the sound level value returned by the callback, not the precision.).
    virtual void
    onAutoMixerSoundLevelUpdate(const std::unordered_map<std::string, float> & /*soundLevels*/) {}

    /// The callback triggered when there is a change to audio devices (i.e. new device added or existing device deleted).
    ///
    /// Available since: 1.1.0
    /// Description: By listening to this callback, users can update the sound collection or output using a specific device when necessary.
    /// When to trigger: This callback is triggered when an audio device is added or removed from the system.
    /// Restrictions: None.
    /// Platform differences: Only supports Windows and macOS.
    ///
    /// @param updateType Update type (add/delete)
    /// @param deviceType Audio device type
    /// @param deviceInfo Audio device information
    virtual void onAudioDeviceStateChanged(ZegoUpdateType /*updateType*/,
                                           ZegoAudioDeviceType /*deviceType*/,
                                           const ZegoDeviceInfo & /*deviceInfo*/) {}

    /// The callback triggered when there is a change of the volume for the audio devices.
    ///
    /// Available since: 1.1.0
    /// Description: Audio device volume change event callback.
    /// When to trigger: After calling the [startAudioDeviceVolumeMonitor] function to start the device volume monitor, and the volume of the monitored audio device changes.
    /// Platform differences: Only supports Windows and macOS.
    ///
    /// @param deviceType Audio device type
    /// @param deviceID Audio device ID
    /// @param volume audio device volume
    virtual void onAudioDeviceVolumeChanged(ZegoAudioDeviceType /*deviceType*/,
                                            const std::string & /*deviceID*/, int /*volume*/) {}

    /// The callback triggered when there is a change to video devices (i.e. new device added or existing device deleted).
    ///
    /// Available since: 1.1.0
    /// Description: By listening to this callback, users can update the video capture using a specific device when necessary.
    /// When to trigger: This callback is triggered when a video device is added or removed from the system.
    /// Restrictions: None
    /// Platform differences: Only supports Windows and macOS.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param updateType Update type (add/delete)
    /// @param deviceInfo Audio device information
    virtual void onVideoDeviceStateChanged(ZegoUpdateType /*updateType*/,
                                           const ZegoDeviceInfo & /*deviceInfo*/) {}

    /// The local captured audio sound level callback.
    ///
    /// Available since: 1.1.0
    /// Description: The local captured audio sound level callback.
    /// Trigger: After you start the sound level monitor by calling [startSoundLevelMonitor].
    /// Caution:
    ///   1. The callback notification period is the parameter value set when the [startSoundLevelMonitor] is called. The callback value is the default value of 0 When you have not called the interface [startPublishingStream] and [startPreview].
    ///   2. This callback is a high-frequency callback, and it is recommended not to do complex logic processing inside the callback.
    /// Related APIs: Start sound level monitoring via [startSoundLevelMonitor]. Monitoring remote played audio sound level by callback [onRemoteSoundLevelUpdate]
    ///
    /// @param soundLevel Locally captured sound level value, ranging from 0.0 to 100.0 (This value only represents the range of the sound level value returned by the callback, not the precision.) .
    virtual void onCapturedSoundLevelUpdate(float /*soundLevel*/) {}

    /// The local captured audio sound level callback, supported vad.
    ///
    /// Available since: 2.10.0
    /// Description: The local captured audio sound level callback.
    /// Trigger: After you start the sound level monitor by calling [startSoundLevelMonitor].
    /// Caution:
    ///   1. The callback notification period is the parameter value set when the [startSoundLevelMonitor] is called.
    ///   2. This callback is a high-frequency callback, and it is recommended not to do complex logic processing inside the callback.
    /// Related APIs: Start sound level monitoring via [startSoundLevelMonitor]. Monitoring remote played audio sound level by callback [onRemoteSoundLevelUpdate] or [onRemoteSoundLevelInfoUpdate].
    ///
    /// @param soundLevelInfo Locally captured sound level value, ranging from 0.0 to 100.0 (This value only represents the range of the sound level value returned by the callback, not the precision.).
    virtual void onCapturedSoundLevelInfoUpdate(const ZegoSoundLevelInfo & /*soundLevelInfo*/) {}

    /// The remote playing streams audio sound level callback.
    ///
    /// Available since: 1.1.0
    /// Description: The remote playing streams audio sound level callback.
    /// Trigger: After you start the sound level monitor by calling [startSoundLevelMonitor], you are in the state of playing the stream [startPlayingStream].
    /// Caution: The callback notification period is the parameter value set when the [startSoundLevelMonitor] is called.
    /// Related APIs: Start sound level monitoring via [startSoundLevelMonitor]. Monitoring local captured audio sound by callback [onCapturedSoundLevelUpdate] or [onCapturedSoundLevelInfoUpdate].
    ///
    /// @param soundLevels Remote sound level hash map, key is the streamID, value is the sound level value of the corresponding streamID, value ranging from 0.0 to 100.0 (This value only represents the range of the sound level value returned by the callback, not the precision.).
    virtual void
    onRemoteSoundLevelUpdate(const std::unordered_map<std::string, float> & /*soundLevels*/) {}

    /// The remote playing streams audio sound level callback, supported vad.
    ///
    /// Available since: 2.10.0
    /// Description: The remote playing streams audio sound level callback.
    /// Trigger: After you start the sound level monitor by calling [startSoundLevelMonitor], you are in the state of playing the stream [startPlayingStream].
    /// Caution: The callback notification period is the parameter value set when the [startSoundLevelMonitor] is called.
    /// Related APIs: Start sound level monitoring via [startSoundLevelMonitor]. Monitoring local captured audio sound by callback [onCapturedSoundLevelUpdate] or [onCapturedSoundLevelInfoUpdate].
    ///
    /// @param soundLevelInfos Remote sound level hash map, key is the streamID, value is the sound level value of the corresponding streamID, value ranging from 0.0 to 100.0 (This value only represents the range of the sound level value returned by the callback, not the precision.).
    virtual void onRemoteSoundLevelInfoUpdate(
        const std::unordered_map<std::string, ZegoSoundLevelInfo> & /*soundLevelInfos*/) {}

    /// The local captured audio spectrum callback.
    ///
    /// Available since: 1.1.0
    /// Description: The local captured audio spectrum callback.
    /// Trigger: After you start the audio spectrum monitor by calling [startAudioSpectrumMonitor].
    /// Caution: The callback notification period is the parameter value set when the [startAudioSpectrumMonitor] is called. The callback value is the default value of 0 When you have not called the interface [startPublishingStream] and [startPreview].
    /// Related APIs: Start audio spectrum monitoring via [startAudioSpectrumMonitor]. Monitoring remote played audio spectrum by callback [onRemoteAudioSpectrumUpdate]
    ///
    /// @param audioSpectrum Locally captured audio spectrum value list. Spectrum value range is [0-2^30].
    virtual void onCapturedAudioSpectrumUpdate(const ZegoAudioSpectrum & /*audioSpectrum*/) {}

    /// The remote playing streams audio spectrum callback.
    ///
    /// Available since: 1.1.0
    /// Description: The remote playing streams audio spectrum callback.
    /// Trigger: After you start the audio spectrum monitor by calling [startAudioSpectrumMonitor], you are in the state of playing the stream [startPlayingStream].
    /// Caution: The callback notification period is the parameter value set when the [startAudioSpectrumMonitor] is called.
    /// Related APIs: Start audio spectrum monitoring via [startAudioSpectrumMonitor]. Monitoring local played audio spectrum by callback [onCapturedAudioSpectrumUpdate].
    ///
    /// @param audioSpectrums Remote audio spectrum hash map, key is the streamID, value is the audio spectrum list of the corresponding streamID. Spectrum value range is [0-2^30]
    virtual void onRemoteAudioSpectrumUpdate(
        const std::unordered_map<std::string, ZegoAudioSpectrum> & /*audioSpectrums*/) {}

    /// The callback triggered when a local device exception occurred.
    ///
    /// Available since: 2.15.0
    /// Description: The callback triggered when a local device exception occurs.
    /// Trigger: This callback is triggered when the function of the local audio or video device is abnormal.
    ///
    /// @param exceptionType The type of the device exception.
    /// @param deviceType The type of device where the exception occurred.
    /// @param deviceID Device ID. Currently, only desktop devices are supported to distinguish different devices; for mobile devices, this parameter will return an empty string.
    virtual void onLocalDeviceExceptionOccurred(ZegoDeviceExceptionType /*exceptionType*/,
                                                ZegoDeviceType /*deviceType*/,
                                                const std::string & /*deviceID*/) {}

    /// The callback triggered when the state of the remote camera changes.
    ///
    /// Available since: 1.1.0
    /// Description: The callback triggered when the state of the remote camera changes.
    /// Use cases: Developers of 1v1 education scenarios or education small class scenarios and similar scenarios can use this callback notification to determine whether the camera device of the remote publishing stream device is working normally, and preliminary understand the cause of the device problem according to the corresponding state.
    /// Trigger: When the state of the remote camera device changes, such as switching the camera, by monitoring this callback, it is possible to obtain an event related to the far-end camera, which can be used to prompt the user that the video may be abnormal.
    /// Caution: This callback will not be called back when the remote stream is play from the CDN, or when custom video acquisition is used at the peer.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param streamID Stream ID.
    /// @param state Remote camera status.
    virtual void onRemoteCameraStateUpdate(const std::string & /*streamID*/,
                                           ZegoRemoteDeviceState /*state*/) {}

    /// The callback triggered when the state of the remote microphone changes.
    ///
    /// Available since: 1.1.0
    /// Description: The callback triggered when the state of the remote microphone changes.
    /// Use cases: Developers of 1v1 education scenarios or education small class scenarios and similar scenarios can use this callback notification to determine whether the microphone device of the remote publishing stream device is working normally, and preliminary understand the cause of the device problem according to the corresponding state.
    /// Trigger: When the state of the remote microphone device is changed, such as switching a microphone, etc., by listening to the callback, it is possible to obtain an event related to the remote microphone, which can be used to prompt the user that the audio may be abnormal.
    /// Caution: This callback will not be called back when the remote stream is play from the CDN, or when custom audio acquisition is used at the peer (But the stream is not published to the ZEGO RTC server.).
    ///
    /// @param streamID Stream ID.
    /// @param state Remote microphone status.
    virtual void onRemoteMicStateUpdate(const std::string & /*streamID*/,
                                        ZegoRemoteDeviceState /*state*/) {}

    /// The callback triggered when the state of the remote speaker changes.
    ///
    /// Available since: 1.1.0
    /// Description: The callback triggered when the state of the remote microphone changes.
    /// Use cases: Developers of 1v1 education scenarios or education small class scenarios and similar scenarios can use this callback notification to determine whether the speaker device of the remote publishing stream device is working normally, and preliminary understand the cause of the device problem according to the corresponding state.
    /// Trigger: When the state of the remote speaker device changes, such as switching the speaker, by monitoring this callback, you can get events related to the remote speaker.
    /// Caution: This callback will not be called back when the remote stream is play from the CDN.
    ///
    /// @param streamID Stream ID.
    /// @param state Remote speaker status.
    virtual void onRemoteSpeakerStateUpdate(const std::string & /*streamID*/,
                                            ZegoRemoteDeviceState /*state*/) {}

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    /// Callback for device's audio route changed.
    ///
    /// Available since: 1.20.0
    /// Description: Callback for device's audio route changed.
    /// Trigger: This callback will be called when there are changes in audio routing such as earphone plugging, speaker and receiver switching, etc.
    /// Platform differences: Only supports iOS and Android.
    ///
    /// @param audioRoute Current audio route.
    virtual void onAudioRouteChange(ZegoAudioRoute /*audioRoute*/) {}
#endif

    /// Callback for audio VAD  stable state update.
    ///
    /// Available since: 2.14.0
    /// Description: Callback for audio VAD  stable state update.
    /// When to trigger: the [startAudioVADStableStateMonitor] function must be called to start the audio VAD monitor and you must be in a state where it is publishing the audio and video stream or be in [startPreview] state.
    /// Restrictions: The callback notification period is 3 seconds.
    /// Related APIs: [startAudioVADStableStateMonitor], [stopAudioVADStableStateMonitor].
    ///
    /// @param type audio VAD monitor type
    /// @param state VAD result
    virtual void onAudioVADStateUpdate(ZegoAudioVADStableStateMonitorType /*type*/,
                                       ZegoAudioVADType /*state*/) {}

    /// The callback triggered when Barrage Messages are received.
    ///
    /// Available since: 1.5.0
    /// Description: This callback is used to receive room passthrough messages sent by other users in the same room.
    /// When to trigger: After calling [loginRoom] to login to the room, this callback is triggered if there is a user in the room who sends a message received by the specified client through the [sendTransparentMessage] function.
    /// Restrictions: None
    /// Caution: Barrage messages sent by users themselves will not be notified through this callback. When there are a large number of barrage messages in the room, the notification may be delayed, and some barrage messages may be lost.
    /// Related callbacks: A bullet-screen message sent by the user himself is not notified by this callback. [sendTransparentMessage] specifies that only a server callback is used. This callback is not triggered.
    ///
    /// @param roomID Room ID. Value range: The maximum length is 128 bytes.
    /// @param message recv message.
    virtual void onRecvRoomTransparentMessage(const std::string & /*roomID*/,
                                              const ZegoRoomRecvTransparentMessage & /*message*/) {}

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
    /// @param roomID Room ID. Value range: The maximum length is 128 bytes.
    /// @param messageList List of received messages. Value range: Up to 50 messages can be received each time.
    virtual void onIMRecvBroadcastMessage(const std::string & /*roomID*/,
                                          std::vector<ZegoBroadcastMessageInfo> /*messageList*/) {}

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
    /// @param roomID Room ID. Value range: The maximum length is 128 bytes.
    /// @param messageList List of received messages. Value range: Up to 50 messages can be received each time.
    virtual void onIMRecvBarrageMessage(const std::string & /*roomID*/,
                                        std::vector<ZegoBarrageMessageInfo> /*messageList*/) {}

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
    /// @param roomID Room ID. Value range: The maximum length is 128 bytes.
    /// @param fromUser Sender of the command.
    /// @param command Command content received.Value range: The maximum length is 1024 bytes.
    virtual void onIMRecvCustomCommand(const std::string & /*roomID*/, ZegoUser /*fromUser*/,
                                       const std::string & /*command*/) {}

    /// System performance monitoring callback.
    ///
    /// Available since: 1.19.0
    /// Description: System performance monitoring callback. The callback notification period is the value of millisecond parameter set by call [startPerformanceMonitor].
    /// Use cases: Monitor system performance can help user quickly locate and solve performance problems and improve user experience.
    /// When to trigger: It will triggered after [createEngine], and call [startPerformanceMonitor] to start system performance monitoring.
    /// Restrictions: None.
    ///
    /// @param status System performance monitoring status.
    virtual void onPerformanceStatusUpdate(const ZegoPerformanceStatus & /*status*/) {}

    /// Network mode changed callback.
    ///
    /// Available since: 1.20.0
    /// Description: Network mode changed callback.
    /// When to trigger: This callback will be triggered when the device's network mode changed, such as switched from WiFi to 5G, or when network is disconnected.
    /// Restrictions: None.
    ///
    /// @param mode Current network mode.
    virtual void onNetworkModeChanged(ZegoNetworkMode /*mode*/) {}

    /// Network speed test error callback.
    ///
    /// Available since: 1.20.0
    /// Description: Network speed test error callback.
    /// Use cases: This function can be used to detect whether the network environment is suitable for pushing/pulling streams with specified bitrates.
    /// When to Trigger: If an error occurs during the speed test, such as: can not connect to speed test server, this callback will be triggered.
    /// Restrictions: None.
    ///
    /// @param errorCode Network speed test error code. Please refer to error codes document https://docs.zegocloud.com/en/5548.html for details.
    /// @param type Uplink or downlink.
    virtual void onNetworkSpeedTestError(int /*errorCode*/, ZegoNetworkSpeedTestType /*type*/) {}

    /// Network speed test quality callback.
    ///
    /// Available since: 1.20.0
    /// Description: Network speed test quality callback when the network can be connected.
    /// Use cases: This function can be used to detect whether the network environment is suitable for pushing/pulling streams with specified bitrates.
    /// When to Trigger: After call [startNetworkSpeedTest] start network speed test, this callback will be triggered. The trigger period is determined by the parameter value specified by call [startNetworkSpeedTest], default value is 3 seconds
    /// Restrictions: None.
    /// Caution: When error occurred during network speed test or [stopNetworkSpeedTest] called, this callback will not be triggered.
    ///
    /// @param quality Network speed test quality.
    /// @param type Uplink or downlink.
    virtual void onNetworkSpeedTestQualityUpdate(const ZegoNetworkSpeedTestQuality & /*quality*/,
                                                 ZegoNetworkSpeedTestType /*type*/) {}

    /// The network quality callback of users who are publishing in the room.
    ///
    /// Available since: 2.10.0
    /// Description: The uplink and downlink network callbacks of the local and remote users, that would be called by default every two seconds for the local and each playing remote user's network quality.
    ///   Versions 2.10.0 to 2.13.1:
    ///   1. Developer must both publish and play streams before you receive your own network quality callback.
    ///   2. When playing a stream, the publish end has a play stream and the publish end is in the room where it is located, then the user's network quality will be received.
    ///   Versions 2.14.0 to 2.21.1:
    ///   1. As long as you publish or play a stream, you will receive your own network quality callback.
    ///   2. When you play a stream, the publish end is in the room where you are, and you will receive the user's network quality.
    ///   Version 2.22.0 and above:
    ///   1. Estimate the network conditions of the remote stream publishing user. If the remote stream publishing user loses one heartbeat, the network quality will be called back as unknown; if the remote stream publishing user's heartbeat loss reaches 3 Second, call back its network quality to die.
    /// Use case: When the developer wants to analyze the network condition on the link, or wants to know the network condition of local and remote users.
    /// When to Trigger: After publishing a stream by called [startPublishingStream] or playing a stream by called [startPlayingStream].
    ///
    /// @param userID User ID, empty means local user
    /// @param upstreamQuality Upstream network quality
    /// @param downstreamQuality Downstream network quality
    virtual void onNetworkQuality(const std::string & /*userID*/,
                                  ZegoStreamQualityLevel /*upstreamQuality*/,
                                  ZegoStreamQualityLevel /*downstreamQuality*/) {}

    /// Successful callback of network time synchronization.
    ///
    /// Available since: 2.12.0
    /// This callback is triggered when internal network time synchronization completes after a developer calls [createEngine].
    virtual void onNetworkTimeSynchronized() {}

    /// Request to dump data.
    ///
    /// Available since: 3.10.0
    /// When to Trigger: When the customer reports back the problem, ZEGO expects the user to dump the data to analyze the audio / video processing problem, which will trigger this callback.
    virtual void onRequestDumpData() {}

    /// Request to dump data.
    ///
    /// Available since: 3.11.0
    /// When to Trigger: When the customer reports back the problem, ZEGO expects the user to dump the data to analyze the audio / video processing problem, which will trigger this callback.
    ///
    /// @param dumpDir Dump data dir.
    /// @param takePhoto Need to take photo when uploading dump data
    virtual void onRequestUploadDumpData(const std::string & /*dumpDir*/, bool /*takePhoto*/) {}

    /// Callback when starting to dump data.
    ///
    /// Available since: 3.10.0
    /// When to Trigger: This callback is triggered when [startDumpData] is called.
    ///
    /// @param errorCode Error code.
    virtual void onStartDumpData(int /*errorCode*/) {}

    /// Callback when stopping to dump data.
    ///
    /// Available since: 3.10.0
    /// When to Trigger: This callback is triggered when [stopDumpData] is called.
    ///
    /// @param errorCode Error code.
    /// @param dumpDir Dump data dir.
    virtual void onStopDumpData(int /*errorCode*/, const std::string & /*dumpDir*/) {}

    /// Callback after uploading the dump data.
    ///
    /// Available since: 3.10.0
    /// When to Trigger: When [uploadDumpData] is called, this callback will be triggered after SDK executes the upload task.
    ///
    /// @param errorCode Error code.
    virtual void onUploadDumpData(int /*errorCode*/) {}
};

class IZegoApiCalledEventHandler {
  protected:
    virtual ~IZegoApiCalledEventHandler() {}

  public:
    /// Method execution result callback
    ///
    /// Available since: 2.3.0
    /// Description: When the monitoring is turned on through [setApiCalledCallback], the results of the execution of all methods will be called back through this callback.
    /// Trigger: When the developer calls the SDK method, the execution result of the method is called back.
    /// Restrictions: None.
    /// Caution: It is recommended to monitor and process this callback in the development and testing phases, and turn off the monitoring of this callback after going online.
    ///
    /// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
    /// @param funcName Function name.
    /// @param info Detailed error information.
    virtual void onApiCalledResult(int /*errorCode*/, const std::string & /*funcName*/,
                                   const std::string & /*info*/) {}
};

class IZegoAudioMixingHandler {
  protected:
    virtual ~IZegoAudioMixingHandler() {}

  public:
    /// Audio mixing callback.
    ///
    /// Available since: 1.9.0
    /// Description: The callback for copying audio data to the SDK for audio mixing. This function should be used together with [enableAudioMixing].
    /// Use cases: Developers can use this function when they need to mix their own songs, sound effects or other audio data into the publishing stream.
    /// When to trigger: It will triggered after [createEngine], and call [enableAudioMixing] turn on audio mixing, and call [setAudioMixingHandler] set audio mixing callback handler.
    /// Restrictions: Supports 16k 32k 44.1k 48k sample rate, mono or dual channel, 16-bit deep PCM audio data.
    /// Caution: This callback is a high frequency callback. To ensure the quality of the mixing data, please do not handle time-consuming operations in this callback.
    ///
    /// @param data Audio mixing data. Note that this parameter is an input parameter, and the developer needs to modify the value of each parameter in the struct.
    virtual void onAudioMixingCopyData(ZegoAudioMixingData * /*data*/) {}
};

class IZegoRealTimeSequentialDataEventHandler {
  protected:
    virtual ~IZegoRealTimeSequentialDataEventHandler() {}

  public:
    /// Callback for receiving real-time sequential data.
    ///
    /// Available since: 2.14.0
    /// Description: Through this callback, you can receive real-time sequential data from the current subscribing stream.
    /// Use cases: You need to listen to this callback when you need to receive real-time sequential data.
    /// When to trigger: After calling [startSubscribing] to successfully start the subscription, and when data is sent on the stream, this callback will be triggered.
    /// Restrictions: None.
    /// Caution: None.
    ///
    /// @param manager The real-time sequential data manager instance that triggers this callback.
    /// @param data The received real-time sequential data.
    /// @param dataLength Data content length.
    /// @param streamID Subscribed stream ID
    virtual void onReceiveRealTimeSequentialData(IZegoRealTimeSequentialDataManager * /*manager*/,
                                                 const unsigned char * /*data*/,
                                                 unsigned int /*dataLength*/,
                                                 const std::string & /*streamID*/) {}
};

class IZegoMediaPlayerEventHandler {
  protected:
    virtual ~IZegoMediaPlayerEventHandler() {}

  public:
    /// MediaPlayer playback status callback.
    ///
    /// Available since: 1.3.4
    /// Description: MediaPlayer playback status callback.
    /// Trigger: The callback triggered when the state of the media player changes.
    /// Restrictions: None.
    ///
    /// @param mediaPlayer Callback player object.
    /// @param state Media player status.
    /// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
    virtual void onMediaPlayerStateUpdate(IZegoMediaPlayer * /*mediaPlayer*/,
                                          ZegoMediaPlayerState /*state*/, int /*errorCode*/) {}

    /// The callback triggered when the network status of the media player changes.
    ///
    /// Available since: 1.3.4
    /// Description: The callback triggered when the network status of the media player changes.
    /// Trigger: When the media player is playing network resources, this callback will be triggered when the status change of the cached data.
    /// Restrictions: The callback will only be triggered when the network resource is played.
    /// Related APIs: [setNetWorkBufferThreshold].
    ///
    /// @param mediaPlayer Callback player object.
    /// @param networkEvent Network status event.
    virtual void onMediaPlayerNetworkEvent(IZegoMediaPlayer * /*mediaPlayer*/,
                                           ZegoMediaPlayerNetworkEvent /*networkEvent*/) {}

    /// The callback to report the current playback progress of the media player.
    ///
    /// Available since: 1.3.4
    /// Description: The callback triggered when the network status of the media player changes. Set the callback interval by calling [setProgressInterval]. When the callback interval is set to 0, the callback is stopped. The default callback interval is 1 second.
    /// Trigger: When the media player is playing network resources, this callback will be triggered when the status change of the cached data.
    /// Restrictions: None.
    /// Related APIs: [setProgressInterval].
    ///
    /// @param mediaPlayer Callback player object.
    /// @param millisecond Progress in milliseconds.
    virtual void onMediaPlayerPlayingProgress(IZegoMediaPlayer * /*mediaPlayer*/,
                                              unsigned long long /*millisecond*/) {}

    /// The callback to report the current rendering progress of the media player.
    ///
    /// Available since: 3.8.0
    /// Description: The callback to report the current rendering progress of the media player. Set the callback interval by calling [setProgressInterval]. When the callback interval is set to 0, the callback is stopped. The default callback interval is 1 second.
    /// Trigger: This callback will be triggered when the media player starts playing resources.
    /// Restrictions: None.
    /// Related APIs: [setProgressInterval].
    ///
    /// @param mediaPlayer Callback player object.
    /// @param millisecond Progress in milliseconds.
    virtual void onMediaPlayerRenderingProgress(IZegoMediaPlayer * /*mediaPlayer*/,
                                                unsigned long long /*millisecond*/) {}

    /// The callback triggered when when the resolution of the playback video changes.
    ///
    /// Available since: 3.11.0
    /// Description: The callback triggered when when the resolution of the playback video changes.
    /// Trigger: When the media player is playing a video resource, This callback will be triggered when playback starts and the resolution of the video changes.
    ///
    /// @param mediaPlayer Callback player object.
    /// @param width width.
    /// @param height height.
    virtual void onMediaPlayerVideoSizeChanged(IZegoMediaPlayer * /*mediaPlayer*/, int /*width*/,
                                               int /*height*/) {}

    /// The callback triggered when the media player got media side info.
    ///
    /// Available since: 2.2.0
    /// Description: The callback triggered when the media player got media side info.
    /// Trigger: When the media player starts playing media files, the callback is triggered if the SEI is resolved to the media file.
    /// Caution: The callback does not actually take effect until call [setEventHandler] to set.
    ///
    /// @param mediaPlayer Callback player object.
    /// @param data SEI content.
    /// @param dataLength SEI content length.
    virtual void onMediaPlayerRecvSEI(IZegoMediaPlayer * /*mediaPlayer*/,
                                      const unsigned char * /*data*/, unsigned int /*dataLength*/) {

    }

    /// The callback of sound level update.
    ///
    /// Available since: 2.15.0
    /// Description: The callback of sound level update.
    /// Trigger: The callback frequency is specified by [EnableSoundLevelMonitor].
    /// Caution: The callback does not actually take effect until call [setEventHandler] to set.
    /// Related APIs: To monitor this callback, you need to enable it through [EnableSoundLevelMonitor].
    ///
    /// @param mediaPlayer Callback player object.
    /// @param soundLevel Sound level value, value range: [0.0, 100.0] (This value only represents the range of the sound level value returned by the callback, not the precision.).
    virtual void onMediaPlayerSoundLevelUpdate(IZegoMediaPlayer * /*mediaPlayer*/,
                                               float /*soundLevel*/) {}

    /// The callback of frequency spectrum update.
    ///
    /// Available since: 2.15.0
    /// Description: The callback of frequency spectrum update.
    /// Trigger: The callback frequency is specified by [EnableFrequencySpectrumMonitor].
    /// Caution: The callback does not actually take effect until call [setEventHandler] to set.
    /// Related APIs: To monitor this callback, you need to enable it through [EnableFrequencySpectrumMonitor].
    ///
    /// @param mediaPlayer Callback player object.
    /// @param spectrumList Locally captured frequency spectrum value list. Spectrum value range is [0-2^30].
    virtual void onMediaPlayerFrequencySpectrumUpdate(IZegoMediaPlayer * /*mediaPlayer*/,
                                                      const ZegoAudioSpectrum & /*spectrumList*/) {}

    /// The callback triggered when the media player plays the first frame.
    ///
    /// Available since: 3.5.0
    /// Description: The callback triggered when the media player plays the first frame.
    /// Trigger: This callback is generated when the media player starts playing.
    /// Caution: The callback does not actually take effect until call [setEventHandler] to set.
    /// Related APIs: You need to call the [setPlayerCanvas] interface to set the view for the media player in order to receive the video first frame event callback.
    ///
    /// @param mediaPlayer Callback player object.
    /// @param event The first frame callback event type.
    virtual void onMediaPlayerFirstFrameEvent(IZegoMediaPlayer * /*mediaPlayer*/,
                                              ZegoMediaPlayerFirstFrameEvent /*event*/) {}

    /// The callback triggered when the media player caches http/https network resource locally.
    ///
    /// Available since: 3.12.0
    /// Description: The callback triggered when the media player caches http/https network resource locally.
    /// Trigger: This callback occurs after the media player caches http/https network resources.
    /// Caution: This callback will not take effect until the [enableLocalCache] API is called and the http/https network resources are played.
    /// Related APIs: Need to call the [enableLocalCache] interface.
    ///
    /// @param mediaPlayer Callback player object.
    /// @param errorCode Error code.
    /// @param resource Played resource
    /// @param cachedFile Cached file
    virtual void onMediaPlayerLocalCache(IZegoMediaPlayer * /*mediaPlayer*/, int /*errorCode*/,
                                         const std::string & /*resource*/,
                                         const std::string & /*cachedFile*/) {}
};

class IZegoMediaPlayerVideoHandler {
  protected:
    virtual ~IZegoMediaPlayerVideoHandler() {}

  public:
    /// The callback triggered when the media player throws out video frame data.
    ///
    /// Available since: 1.3.4
    /// Description: The callback triggered when the media player throws out video frame data.
    /// Trigger: The callback is generated when the media player starts playing.
    /// Caution: The callback does not actually take effect until call [setVideoHandler] to set.
    /// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
    ///
    /// @param mediaPlayer Callback player object.
    /// @param data Raw data of video frames (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2]).
    /// @param dataLength Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2]).
    /// @param param video data frame param.
    virtual void onVideoFrame(IZegoMediaPlayer * /*mediaPlayer*/, const unsigned char ** /*data*/,
                              unsigned int * /*dataLength*/, ZegoVideoFrameParam /*param*/) {}

    /// The callback triggered when the media player throws out video frame data, with additional information for the video frame.
    ///
    /// Available since: 2.16.0
    /// Description: The callback triggered when the media player throws out video frame data.
    /// Trigger: The callback is generated when the media player starts playing.
    /// Caution: The callback does not actually take effect until call [setVideoHandler] to set.
    /// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
    ///
    /// @param mediaPlayer Callback player object.
    /// @param data Raw data of video frames (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2]).
    /// @param dataLength Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2]).
    /// @param param video data frame param.
    /// @param extraInfo Video frame extra info. json format data.
    virtual void onVideoFrame(IZegoMediaPlayer * /*mediaPlayer*/, const unsigned char ** /*data*/,
                              unsigned int * /*dataLength*/, ZegoVideoFrameParam /*param*/,
                              const char * /*extraInfo*/) {}
};

class IZegoMediaPlayerAudioHandler {
  protected:
    virtual ~IZegoMediaPlayerAudioHandler() {}

  public:
    /// The callback triggered when the media player throws out audio frame data.
    ///
    /// Available since: 1.3.4
    /// Description: The callback triggered when the media player throws out audio frame data.
    /// Trigger: The callback is generated when the media player starts playing.
    /// Caution: The callback does not actually take effect until call [setAudioHandler] to set.
    /// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
    ///
    /// @param mediaPlayer Callback player object.
    /// @param data Raw data of audio frames.
    /// @param dataLength Data length.
    /// @param param audio frame flip mode.
    virtual void onAudioFrame(IZegoMediaPlayer * /*mediaPlayer*/, const unsigned char * /*data*/,
                              unsigned int /*dataLength*/, ZegoAudioFrameParam /*param*/) {}
};

class IZegoMediaPlayerBlockDataHandler {
  protected:
    virtual ~IZegoMediaPlayerBlockDataHandler() {}

  public:
    /// The callback triggered when the media player is about to throw the block data of the media resource.
    ///
    /// Available since: 3.4.0
    /// Description: The callback triggered when the media player is about to throw the block data of the media resource.
    /// Trigger: The callback is generated when the media player starts playing.
    /// Caution: The callback does not actually take effect until call [setBlockDataHandler] to set.
    /// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
    ///
    /// @param mediaPlayer Callback player object.
    /// @param path The path of the media resource.
    virtual void onBlockBegin(IZegoMediaPlayer *mediaPlayer, const std::string &path) = 0;

    /// The callback triggered when the media player throws the block data of the media resource.
    ///
    /// Available since: 3.4.0
    /// Description: The callback triggered when the media player throws the block data of the media resource.
    /// Trigger: This callback will be generated after receiving the [onBlockBegin] callback.
    /// Caution: The callback does not actually take effect until call [setBlockDataHandler] to set. The buffer size before and after decryption should be consistent.
    /// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
    ///
    /// @param mediaPlayer Callback player object.
    /// @param buffer The block data of the media resource.
    /// @param bufferSize Length of media resource block data.
    /// @return The size of the buffer, -1 is returned for failure.
    virtual unsigned int onBlockData(IZegoMediaPlayer *mediaPlayer, unsigned char *const buffer,
                                     unsigned int bufferSize) = 0;
};

class IZegoAudioEffectPlayerEventHandler {
  protected:
    virtual ~IZegoAudioEffectPlayerEventHandler() {}

  public:
    /// Audio effect playback state callback.
    ///
    /// Available since: 1.16.0
    /// Description: This callback is triggered when the playback state of a audio effect of the audio effect player changes.
    /// Trigger: This callback is triggered when the playback status of the audio effect changes.
    /// Restrictions: None.
    ///
    /// @param audioEffectPlayer Audio effect player instance that triggers this callback.
    /// @param audioEffectID The ID of the audio effect resource that triggered this callback.
    /// @param state The playback state of the audio effect.
    /// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
    virtual void onAudioEffectPlayStateUpdate(IZegoAudioEffectPlayer * /*audioEffectPlayer*/,
                                              unsigned int /*audioEffectID*/,
                                              ZegoAudioEffectPlayState /*state*/,
                                              int /*errorCode*/) {}
};

class IZegoMediaDataPublisherEventHandler {
  protected:
    virtual ~IZegoMediaDataPublisherEventHandler() {}

  public:
    /// The event callback of the media data publisher opening a media file.
    ///
    /// Available since: 2.17.0
    /// Description: The event callback of the media data publisher opening a media file.
    /// Trigger: The callback triggered when the media data publisher start loading a media file.
    /// Restrictions: None.
    ///
    /// @param mediaDataPublisher Callback publisher object
    /// @param path Path of currently open file
    virtual void onMediaDataPublisherFileOpen(IZegoMediaDataPublisher * /*mediaDataPublisher*/,
                                              const std::string & /*path*/) {}

    /// The event callback of the media data publisher closing a media file.
    ///
    /// Available since: 2.17.0
    /// Description: The event callback of the media data publisher closing a media file.
    /// Trigger: The callback triggered when the media data publisher start unloading a media file.
    /// Restrictions: None.
    ///
    /// @param mediaDataPublisher Callback publisher object
    /// @param errorCode error code. 0 means closing the file normally. -1 is a file error. -2 is a path exception. -3 is a decoding exception. -4 is an incorrect timestamp. -5 Unable to fix audio/video synchronization. -6 Unsupported audio sample rate. Contact technical support for details.
    /// @param path Path of currently open file
    virtual void onMediaDataPublisherFileClose(IZegoMediaDataPublisher * /*mediaDataPublisher*/,
                                               int /*errorCode*/, const std::string & /*path*/) {}

    /// The event callback that the media data publisher has read data from the media file.
    ///
    /// Available since: 2.17.0
    /// Description: The event callback that the media data publisher has read data from the media file.
    /// Trigger: The callback triggered when the media data publisher begin to read media data from a media file.
    /// Restrictions: None.
    ///
    /// @param mediaDataPublisher Callback publisher object
    /// @param path Path of currently open file
    virtual void onMediaDataPublisherFileDataBegin(IZegoMediaDataPublisher * /*mediaDataPublisher*/,
                                                   const std::string & /*path*/) {}

    /// The event callback that the media data publisher has completed a file stream publishing.
    ///
    /// Available since: 3.14.0
    /// Description: The media data publisher has finished a file stream publishing.
    /// Trigger: The callback triggered when the media data publisher has completed a file stream publishing.
    /// Restrictions: None.
    ///
    /// @param mediaDataPublisher Callback publisher object.
    /// @param path The path of the file that has completed stream publishing.
    virtual void onMediaDataPublisherFileDataEnd(IZegoMediaDataPublisher * /*mediaDataPublisher*/,
                                                 const std::string & /*path*/) {}
};

class IZegoDataRecordEventHandler {
  protected:
    virtual ~IZegoDataRecordEventHandler() {}

  public:
    /// The callback triggered when the state of data recording (to a file) changes.
    ///
    /// Available since: 1.10.0
    /// Description: The callback triggered when the state of data recording (to a file) changes.
    /// Use cases: The developer should use this callback to determine the status of the file recording or for UI prompting.
    /// When to trigger: After [startRecordingCapturedData] is called, if the state of the recording process changes, this callback will be triggered.
    /// Restrictions: None.
    ///
    /// @param state File recording status.
    /// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
    /// @param config Record config.
    /// @param channel Publishing stream channel.
    virtual void onCapturedDataRecordStateUpdate(ZegoDataRecordState /*state*/, int /*errorCode*/,
                                                 ZegoDataRecordConfig /*config*/,
                                                 ZegoPublishChannel /*channel*/) {}

    /// The callback to report the current recording progress.
    ///
    /// Available since: 1.10.0
    /// Description: Recording progress update callback, triggered at regular intervals during recording.
    /// Use cases: Developers can do UI hints for the user interface.
    /// When to trigger: After [startRecordingCapturedData] is called, If configured to require a callback, timed trigger during recording.
    /// Restrictions: None.
    ///
    /// @param progress File recording progress, which allows developers to hint at the UI, etc.
    /// @param config Record config.
    /// @param channel Publishing stream channel.
    virtual void onCapturedDataRecordProgressUpdate(ZegoDataRecordProgress /*progress*/,
                                                    ZegoDataRecordConfig /*config*/,
                                                    ZegoPublishChannel /*channel*/) {}
};

class IZegoCustomVideoCaptureHandler {
  protected:
    virtual ~IZegoCustomVideoCaptureHandler() {}

  public:
    /// Customize the notification of the start of video capture.
    ///
    /// Available since: 1.1.0
    /// Description: The SDK informs that the video frame is about to be collected, and the video frame data sent to the SDK is valid after receiving the callback.
    /// Use cases: Live data collected by non-cameras. For example, local video file playback, screen sharing, game live broadcast, etc.
    /// When to Trigger: After calling [startPreview] or [startPublishingStream] successfully.
    /// Caution: The video frame data sent to the SDK after receiving the callback is valid.
    /// Related callbacks: Customize the end of capture notification [onCaptureStop].
    /// Related APIs: Call [setCustomVideoCaptureHandler] to set custom video capture callback.
    ///
    /// @param channel Publishing stream channel.
    virtual void onStart(ZegoPublishChannel channel) = 0;

    /// Customize the notification of the end of the collection.
    ///
    /// Available since: 1.1.0
    /// Description: The SDK informs that it is about to end the video frame capture.
    /// Use cases: Live data collected by non-cameras. For example, local video file playback, screen sharing, game live broadcast, etc.
    /// When to Trigger: After calling [stopPreview] or [stopPublishingStream] successfully.
    /// Caution: If you call [startPreview] and [startPublishingStream] to start preview and push stream at the same time after you start custom capture, you should call [stopPreview] and [stopPublishingStream] to stop the preview and push stream before triggering the callback.
    /// Related callbacks: Custom video capture start notification [onCaptureStart].
    /// Related APIs: Call [setCustomVideoCaptureHandler] to set custom video capture callback.
    ///
    /// @param channel Publishing stream channel.
    virtual void onStop(ZegoPublishChannel channel) = 0;

    /// When network changes are detected during custom video capture, the developer is notified that traffic control is required, and the encoding configuration is adjusted according to the recommended parameters of the SDK.
    ///
    /// Available since: 1.14.0
    /// Description: When using custom video capture, the SDK detects a network change, informs the developer that it needs to do flow control, and adjust the encoding configuration according to the recommended parameters of the SDK. In the case of custom collection and transmission of encoded data, the SDK cannot know the external encoding configuration, so the flow control operation needs to be completed by the developer. The SDK will notify the developer of the recommended value of the video configuration according to the current network situation, and the developer needs to modify the encoder configuration by himself to ensure the smoothness of video transmission.
    /// Use cases: Live data collected by non-cameras. For example, local video file playback, screen sharing, game live broadcast, etc.
    /// When to Trigger: When network status changes during the process of custom video capture and flow control is required.
    /// Caution: Please do not perform time-consuming operations in this callback, such as reading and writing large files. If you need to perform time-consuming operations, please switch threads.
    /// Related APIs: Call [setCustomVideoCaptureHandler] to set custom video capture callback.
    ///
    /// @param trafficControlInfo traffic control info.
    /// @param channel Publishing stream channel.
    virtual void onEncodedDataTrafficControl(ZegoTrafficControlInfo /*trafficControlInfo*/,
                                             ZegoPublishChannel /*channel*/) {}
};

class IZegoCustomVideoRenderHandler {
  protected:
    virtual ~IZegoCustomVideoRenderHandler() {}

  public:
    /// When custom video rendering is enabled, the original video frame data collected by the local preview is called back.
    ///
    /// Available since: 1.1.0
    /// Description: When using custom video rendering, the SDK callbacks the original video frame data collected by the local preview, which is rendered by the developer.
    /// Use cases: Use a cross-platform interface framework or game engine; need to obtain the video frame data collected or streamed by the SDK for special processing.
    /// When to Trigger: When the local preview is turned on, when the SDK collects the local preview video frame data.
    /// Related APIs: Call [setCustomVideoRenderHandler] to set custom video rendering callback.
    ///
    /// @param data Raw video frame data (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2]).
    /// @param dataLength Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2]).
    /// @param param Video frame parameters.
    /// @param flipMode video flip mode.
    /// @param channel Publishing stream channel.
    virtual void onCapturedVideoFrameRawData(unsigned char ** /*data*/,
                                             unsigned int * /*dataLength*/,
                                             ZegoVideoFrameParam /*param*/,
                                             ZegoVideoFlipMode /*flipMode*/,
                                             ZegoPublishChannel /*channel*/) {}

    /// When custom video rendering is enabled, the remote end pulls the original video frame data to call back, and distinguishes different streams by streamID.
    ///
    /// Available since: 1.1.0
    /// Description: When custom video rendering is enabled, the SDK calls back the remote end to pull the original video frame data, distinguishes different streams by streamID, and renders them by the developer.
    /// Use cases: Use a cross-platform interface framework or game engine; need to obtain the video frame data collected or streamed by the SDK for special processing.
    /// When to Trigger: After starting to stream, when the SDK receives the video frame data of the remote stream.
    /// Related APIs: Call [setCustomVideoRenderHandler] to set custom video rendering callback.
    ///
    /// @param data Raw video frame data (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2]).
    /// @param dataLength Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2]).
    /// @param param Video frame parameters.
    /// @param streamID Stream ID.
    virtual void onRemoteVideoFrameRawData(unsigned char ** /*data*/, unsigned int * /*dataLength*/,
                                           ZegoVideoFrameParam /*param*/,
                                           const std::string & /*streamID*/) {}

    /// Custom video rendering remote playing stream video frame undecoded data callback.
    ///
    /// Available since: 1.10.0
    /// Description: When [enableCustomVideoRender] is called to enable custom video rendering and the specified data type is [EncodedData], this function will be called back with the remote playing stream undecoded video frame data.
    /// When to Trigger: After calling [startPlayingStream], the SDK will call back when the remote streaming video frame data is received.
    /// Related APIs: Call [setCustomVideoRenderHandler] to set custom video rendering callback.
    ///
    /// @param data Encoded data of video frames.
    /// @param dataLength Data length.
    /// @param param Video frame parameters.
    /// @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
    /// @param streamID Stream ID.
    virtual void onRemoteVideoFrameEncodedData(const unsigned char * /*data*/,
                                               unsigned int /*dataLength*/,
                                               ZegoVideoEncodedFrameParam /*param*/,
                                               unsigned long long /*referenceTimeMillisecond*/,
                                               const std::string & /*streamID*/) {}
};

class IZegoCustomVideoProcessHandler {
  protected:
    virtual ~IZegoCustomVideoProcessHandler() {}

  public:
    /// Call back when the original video data is obtained.
    ///
    /// Available since: 2.2.0
    /// Description: When the custom video pre-processing is turned on, after calling [setCustomVideoProcessHandler] to set the callback, the SDK receives the original video data and calls back to the developer. After the developer has processed the original image, he must call [sendCustomVideoProcessedRawData] to send the processed data back to the SDK, otherwise it will cause frame loss.
    /// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
    /// When to Trigger: When the custom video pre-processing is enabled, the SDK collects the original video data.
    /// Restrictions: This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video pre-processing and the bufferType of config is passed in [ZegoVideoBufferTypeRawData].
    /// Platform differences: Available for Window, Android platforms.
    ///
    /// @param data Raw video data. RGB format data storage location is data[0], YUV format data storage location is Y component：data[0], U component：data[1], V component：data[2].
    /// @param dataLength Raw video data length. RGB format data length storage location is dataLength[0], YUV format data storage location respectively Y component length：dataLength[0], U component length：dataLength[1], V component length：dataLength[2].
    /// @param param Video frame parameters.
    /// @param referenceTimeMillisecond video frame reference time, UNIX timestamp, or system startup timestamp, in milliseconds.
    /// @param channel Publishing stream channel.
    virtual void onCapturedUnprocessedRawData(const unsigned char ** /*data*/,
                                              unsigned int * /*dataLength*/,
                                              ZegoVideoFrameParam /*param*/,
                                              unsigned long long /*referenceTimeMillisecond*/,
                                              ZegoPublishChannel /*channel*/) {}

    /// Call back when the original video data of type [CVPixelBuffer] is obtained.
    ///
    /// Available since: 2.2.0
    /// Description: When the custom video pre-processing is turned on, after calling [setCustomVideoProcessHandler] to set the callback, the SDK receives the original video data and calls back to the developer. After the developer has processed the original image, he must call [sendCustomVideoProcessedCVPixelbuffer] to send the processed data back to the SDK, otherwise it will cause frame loss.
    /// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
    /// When to Trigger: When the custom video pre-processing is enabled, the SDK collects the original video data.
    /// Restrictions: This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video pre-processing and the bufferType of config is passed in [ZegoVideoBufferTypeCVPixelBuffer] or [ZegoVideoBufferTypeNV12CVPixelBuffer].
    /// Platform differences: It only takes effect on the iOS/macOS platform.
    ///
    /// @param buffer CVPixelBufferRef type data.
    /// @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
    /// @param channel Publishing stream channel.
    virtual void onCapturedUnprocessedCVPixelBuffer(void * /*buffer*/,
                                                    unsigned long long /*referenceTimeMillisecond*/,
                                                    ZegoPublishChannel /*channel*/) {}
};

class IZegoCustomAudioProcessHandler {
  protected:
    virtual ~IZegoCustomAudioProcessHandler() {}

  public:
    /// Custom audio processing local captured PCM audio frame callback.
    ///
    /// Available: Since 2.13.0
    /// Description: In this callback, you can receive the PCM audio frames captured locally after used headphone monitor. Developers can modify the audio frame data, as well as the audio channels and sample rate. The timestamp can be used for data synchronization, such as lyrics, etc. If you need the data after used headphone monitor, please use the [onProcessCapturedAudioDataAfterUsedHeadphoneMonitor] callback.
    /// When to trigger: You need to call [enableCustomAudioCaptureProcessing] to enable the function first, and call [startPreivew] or [startPublishingStream] to trigger this callback function.
    /// Restrictions: None.
    /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
    ///
    /// @param data Audio data in PCM format.
    /// @param dataLength Length of the data.
    /// @param param Parameters of the audio frame.
    /// @param timestamp The audio frame timestamp, starting from 0 when capture is started, the unit is milliseconds.
    virtual void onProcessCapturedAudioData(unsigned char * /*data*/, unsigned int /*dataLength*/,
                                            ZegoAudioFrameParam * /*param*/, double /*timestamp*/) {

    }

    /// Custom audio processing local captured PCM audio frame callback after used headphone monitor.
    ///
    /// Available: Since 2.13.0
    /// Description: In this callback, you can receive the PCM audio frames captured locally after used headphone monitor. Developers can modify the audio frame data, as well as the audio channels and sample rate. The timestamp can be used for data synchronization, such as lyrics, etc.
    /// When to trigger: You need to call [enableCustomAudioCaptureProcessingAfterHeadphoneMonitor] to enable the function first, and call [startPreivew] or [startPublishingStream] to trigger this callback function.
    /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
    ///
    /// @param data Audio data in PCM format
    /// @param dataLength Length of the data
    /// @param param Parameters of the audio frame
    /// @param timestamp The audio frame timestamp, starting from 0 when capture is started, the unit is milliseconds.
    virtual void onProcessCapturedAudioDataAfterUsedHeadphoneMonitor(
        unsigned char * /*data*/, unsigned int /*dataLength*/, ZegoAudioFrameParam * /*param*/,
        double /*timestamp*/) {}

    /// Audio data callback before SDK internal audio preprocessing.
    ///
    /// Available: Since 3.11.0
    /// Description: In this callback, you can receive the audio data before SDK internal audio preprocessing.
    /// When to trigger: This callback function will not be triggered until [enableBeforeAudioPrepAudioData] is called to turn on the function and [startpublishingstream] is called.
    /// Restrictions: None.
    /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback, and the data in this callback cannot be modified.
    ///
    /// @param data Audio data in PCM format.
    /// @param dataLength Length of the data.
    /// @param param Parameters of the audio frame.
    virtual void onBeforeAudioPrepAudioData(const unsigned char * /*data*/,
                                            unsigned int /*dataLength*/,
                                            ZegoAudioFrameParam /*param*/) {}

    /// Custom audio processing remote playing stream PCM audio frame callback.
    ///
    /// Available: Since 2.13.0
    /// Description: In this callback, you can receive the PCM audio frames of remote playing stream. Developers can modify the audio frame data, as well as the audio channels and sample rate. The timestamp can be used for data synchronization, such as lyrics, etc.
    /// When to trigger: You need to call [enableCustomAudioRemoteProcessing] to enable the function first, and call [startPlayingStream] to trigger this callback function.
    /// Restrictions: None.
    /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
    ///
    /// @param data Audio data in PCM format.
    /// @param dataLength Length of the data.
    /// @param param Parameters of the audio frame.
    /// @param streamID Corresponding stream ID.
    /// @param timestamp The audio frame timestamp, starting from 0 when capture is started, the unit is milliseconds.
    virtual void onProcessRemoteAudioData(unsigned char * /*data*/, unsigned int /*dataLength*/,
                                          ZegoAudioFrameParam * /*param*/,
                                          const std::string & /*streamID*/, double /*timestamp*/) {}

    /// Custom audio processing SDK playback PCM audio frame callback.
    ///
    /// Available: Since 2.13.0
    /// Description: In this callback, you can receive the SDK playback PCM audio frame. Developers can modify the audio frame data, as well as the audio channels and sample rate. The timestamp can be used for data synchronization, such as lyrics, etc.
    /// When to trigger: You need to call [enableCustomAudioPlaybackProcessing] to enable the function first, and call [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] or [createAudioEffectPlayer] to trigger this callback function.
    /// Restrictions: None.
    /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
    ///
    /// @param data Audio data in PCM format.
    /// @param dataLength Length of the data.
    /// @param param Parameters of the audio frame.
    /// @param timestamp The audio frame timestamp, starting from 0 when capture is started, the unit is milliseconds (It is effective when there is one and only one stream).
    virtual void onProcessPlaybackAudioData(unsigned char * /*data*/, unsigned int /*dataLength*/,
                                            ZegoAudioFrameParam * /*param*/, double /*timestamp*/) {

    }
};

class IZegoAudioDataHandler {
  protected:
    virtual ~IZegoAudioDataHandler() {}

  public:
    /// The callback for obtaining the audio data captured by the local microphone.
    ///
    /// Available: Since 1.1.0
    /// Description: In non-custom audio capture mode, the SDK capture the microphone's sound, but the developer may also need to get a copy of the audio data captured by the SDK is available through this callback.
    /// When to trigger: On the premise of calling [setAudioDataHandler] to set the listener callback, after calling [startAudioDataObserver] to set the mask 0b01 that means 1 << 0, this callback will be triggered only when it is in the publishing stream state.
    /// Restrictions: None.
    /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
    ///
    /// @param data Audio data in PCM format.
    /// @param dataLength Length of the data.
    /// @param param Parameters of the audio frame.
    virtual void onCapturedAudioData(const unsigned char * /*data*/, unsigned int /*dataLength*/,
                                     ZegoAudioFrameParam /*param*/) {}

    /// The callback for obtaining the audio data of all the streams playback by SDK.
    ///
    /// Available: Since 1.1.0
    /// Description: This function will callback all the mixed audio data to be playback. This callback can be used for that you needs to fetch all the mixed audio data to be playback to proccess.
    /// When to trigger: On the premise of calling [setAudioDataHandler] to set the listener callback, after calling [startAudioDataObserver] to set the mask 0b10 that means 1 << 1, this callback will be triggered only when it is in the SDK inner audio and video engine started(called the [startPreivew] or [startPlayingStream] or [startPublishingStream]).
    /// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
    /// Caution: This callback is a high-frequency callback. Please do not perform time-consuming operations in this callback. When the engine is not in the stream publishing state and the media player is not used to play media files, the audio data in the callback is muted audio data.
    ///
    /// @param data Audio data in PCM format.
    /// @param dataLength Length of the data.
    /// @param param Parameters of the audio frame.
    virtual void onPlaybackAudioData(const unsigned char * /*data*/, unsigned int /*dataLength*/,
                                     ZegoAudioFrameParam /*param*/) {}

    /// Callback to get the audio data played by the SDK and the audio data captured by the local microphone. The audio data is the data mixed by the SDK.
    ///
    /// Available: Since 1.1.0
    /// Description: The audio data played by the SDK is mixed with the data captured by the local microphone before being sent to the speaker, and is called back through this function.
    /// When to trigger: On the premise of calling [setAudioDataHandler] to set the listener callback, after calling [startAudioDataObserver] to set the mask 0x04, this callback will be triggered only when it is in the publishing stream state or playing stream state.
    /// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
    /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
    ///
    /// @param data Audio data in PCM format.
    /// @param dataLength Length of the data.
    /// @param param Parameters of the audio frame.
    virtual void onMixedAudioData(const unsigned char * /*data*/, unsigned int /*dataLength*/,
                                  ZegoAudioFrameParam /*param*/) {}

    /// The callback for obtaining the audio data of each stream.
    ///
    /// Available: Since 1.1.0
    /// Description: This function will call back the data corresponding to each playing stream. Different from [onPlaybackAudioData], the latter is the mixed data of all playing streams. If developers need to process a piece of data separately, they can use this callback.
    /// When to trigger: On the premise of calling [setAudioDataHandler] to set up listening for this callback, calling [startAudioDataObserver] to set the mask 0x08 that is 1 << 3, and this callback will be triggered when the SDK audio and video engine starts to play the stream.
    /// Restrictions: None.
    /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
    ///
    /// @param data Audio data in PCM format.
    /// @param dataLength Length of the data.
    /// @param param Parameters of the audio frame.
    /// @param streamID Corresponding stream ID.
    virtual void onPlayerAudioData(const unsigned char * /*data*/, unsigned int /*dataLength*/,
                                   ZegoAudioFrameParam /*param*/,
                                   const std::string & /*streamID*/) {}
};

class IZegoRangeAudioEventHandler {
  protected:
    virtual ~IZegoRangeAudioEventHandler() {}

  public:
    /// Range audio microphone state callback.
    ///
    /// Available since: 2.11.0
    /// Description: The status change notification of the microphone, starting to send audio is an asynchronous process, and the state switching in the middle is called back through this function.
    /// When to Trigger: After the [enableMicrophone] function.
    /// Caution: 1. It must be monitored before the [enableMicrophone] function is called. 2. The callback is not triggered when the [enableMicrophone] is stopped normally, and the Off state is triggered only when the [enableMicrophone] is stopped abnormally.
    ///
    /// @param rangeAudio Range audio instance that triggers this callback.
    /// @param state The use state of the range audio.
    /// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
    virtual void onRangeAudioMicrophoneStateUpdate(IZegoRangeAudio * /*rangeAudio*/,
                                                   ZegoRangeAudioMicrophoneState /*state*/,
                                                   int /*errorCode*/) {}
};

class IZegoCopyrightedMusicEventHandler {
  protected:
    virtual ~IZegoCopyrightedMusicEventHandler() {}

  public:
    /// Callback for download song or accompaniment progress rate.
    ///
    /// @param copyrightedMusic Copyrighted music instance that triggers this callback.
    /// @param resourceID The resource ID of the song or accompaniment that triggered this callback.
    /// @param progressRate download progress rate.
    virtual void onDownloadProgressUpdate(IZegoCopyrightedMusic * /*copyrightedMusic*/,
                                          const std::string & /*resourceID*/,
                                          float /*progressRate*/) {}

    /// Real-time pitch line callback.
    ///
    /// @param copyrightedMusic Copyrighted music instance that triggers this callback.
    /// @param resourceID The resource ID of the song or accompaniment that triggered this callback.
    /// @param currentDuration Current playback progress.
    /// @param pitchValue Real-time pitch accuracy or value.
    virtual void onCurrentPitchValueUpdate(IZegoCopyrightedMusic * /*copyrightedMusic*/,
                                           const std::string & /*resourceID*/,
                                           int /*currentDuration*/, int /*pitchValue*/) {}
};

class IZegoRangeSceneStreamEventHandler {
  protected:
    virtual ~IZegoRangeSceneStreamEventHandler() {}

  public:
    /// The callback notification of local user play other users' stream state change.
    ///
    /// Available since: 3.0.0
    /// Description: The callback notification of local user plays other users' stream state change.
    /// When to trigger: The local user plays other users' stream state changes.
    /// Restrictions: Do not call the SDK interface in the callback thread.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param userID User ID.
    /// @param streamID User's stream ID.
    /// @param state The state of local user play stream.
    virtual void onUserStreamStateUpdate(IZegoRangeScene * /*rangeScene*/,
                                         const std::string & /*userID*/,
                                         const std::string & /*streamID*/,
                                         ZegoStreamState /*state*/) {}

    /// User microphone state change callback.
    ///
    /// Available: Since 3.0.0
    /// Description: User microphone state change callback.
    /// When to trigger: User microphone state change
    /// Restrictions: Do not call the SDK interface in the callback thread.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param userID User ID.
    /// @param state Device state.
    virtual void onUserMicUpdate(IZegoRangeScene * /*rangeScene*/, const std::string & /*userID*/,
                                 ZegoDeviceState /*state*/) {}

    /// User camera state change callback.
    ///
    /// Available: Since 3.0.0
    /// Description: User camera state change callback.
    /// When to trigger: User camera state change.
    /// Restrictions: Do not call the SDK interface in the callback thread.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param userID User ID.
    /// @param state Device state.
    virtual void onUserCameraUpdate(IZegoRangeScene * /*rangeScene*/,
                                    const std::string & /*userID*/, ZegoDeviceState /*state*/) {}

    /// User speaker state change callback.
    ///
    /// Available: Since 3.0.0
    /// Description: User speaker state change callback.
    /// When to trigger: User speaker state change.
    /// Restrictions: Do not call the SDK interface in the callback thread.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param userID User ID.
    /// @param state Device state.
    virtual void onUserSpeakerUpdate(IZegoRangeScene * /*rangeScene*/,
                                     const std::string & /*userID*/, ZegoDeviceState /*state*/) {}
};

class IZegoRangeSceneTeamEventHandler {
  protected:
    virtual ~IZegoRangeSceneTeamEventHandler() {}

  public:
    /// The callback triggered when the team connection state changes.
    ///
    /// Available since: 3.1.0
    /// Description: This callback is triggered when the connection status of the team changes, and the reason for the change is notified.
    /// Use cases: Developers can use this callback to determine the status of the current user in the team.
    /// When to trigger:
    ///  1. The developer will receive this notification when calling the [joinTeam], [leaveTeam] functions.
    ///  2. This notification may also be received when the network condition of the user's device changes (SDK will automatically join the team when reconnected, please refer to [Does ZEGO SDK support a fast reconnection for temporary disconnection] for details](https://docs.zegocloud.com/faq/reconnect?product=ExpressVideo&platform=all).
    /// Restrictions: None.
    /// Caution: If the connection is being requested for a long time, the general probability is that the user's network is unstable.
    /// Related APIs: [joinTeam]、[leaveTeam]
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param teamID Team ID.
    /// @param state Changed team state.
    /// @param errorCode Error code, For details, please refer to [Common Error Codes](https://docs.zegocloud.com/article/5548).
    virtual void onTeamStateUpdate(IZegoRangeScene * /*rangeScene*/, unsigned int /*teamID*/,
                                   ZegoTeamState /*state*/, int /*errorCode*/) {}

    /// The callback triggered when the number of other users in the team increases or decreases.
    ///
    /// Available since: 3.1.0
    /// Description: When other users in the room are online or offline, which causes the user list in the room to change, the developer will be notified through this callback.
    /// Use cases: Developers can use this callback to update the user list display in the room in real time.
    /// When to trigger:
    ///   1. When the user join the room for the first time, if there are other users in the team, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeAdd], and `userList` is the other users in the team at this time.
    ///   2. The user is already in the team. If another user join the team through the [joinTeam] function, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeAdd].
    ///   3. If other users leave the team through the [leaveTeam] function, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeDelete].
    /// Related APIs: [joinTeam]、[leaveTeam]
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param teamID Team ID.
    /// @param updateType Update type (add/delete).
    /// @param userList List of users changed in the current team.
    virtual void onTeamMemberUpdate(IZegoRangeScene * /*rangeScene*/, unsigned int /*teamID*/,
                                    ZegoUpdateType /*updateType*/,
                                    const std::vector<ZegoUser> & /*userList*/) {}
};

class IZegoRangeSceneItemEventHandler {
  protected:
    virtual ~IZegoRangeSceneItemEventHandler() {}

  public:
    /// Callback notification when item enters the current user's field of view.
    ///
    /// Available: Since 3.1.0
    /// Description: Callback notification when item enters the current user's field of view.
    /// When to trigger: Item enters the current user's field of view.
    /// Restrictions: Do not call the SDK interface in the callback thread.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param itemID Item ID.
    /// @param capacity The number of users allowed to bind the item.
    /// @param position Item position.
    /// @param userList List of users the item is currently bound to.
    virtual void onItemEnterView(IZegoRangeScene * /*rangeScene*/, long long /*itemID*/,
                                 unsigned int /*capacity*/, const ZegoPosition & /*position*/,
                                 const std::vector<std::string> & /*userList*/) {}

    /// Callback notification when item leave the current user's field of view.
    ///
    /// Available: Since 3.1.0
    /// Description: Callback notification when item leave the current user's field of view.
    /// When to trigger: Item leave the current user's field of view.
    /// Restrictions: Do not call the SDK interface in the callback thread.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param itemID Item ID.
    virtual void onItemLeaveView(IZegoRangeScene * /*rangeScene*/, long long /*itemID*/) {}

    /// Item binding user change callback notification.
    ///
    /// Available: Since 3.1.0
    /// Description: Item binding user change callback notification.
    /// When to trigger: Item binding user change.
    /// Restrictions: Do not call the SDK interface in the callback thread.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param itemID Item ID.
    /// @param userID The user ID of the item binding.
    virtual void onItemBindUpdate(IZegoRangeScene * /*rangeScene*/, long long /*itemID*/,
                                  const std::string & /*userID*/) {}

    /// Item unbind user change callback notification.
    ///
    /// Available: Since 3.1.0
    /// Description: Item unbind user change callback notification.
    /// When to trigger: Item unbind user change.
    /// Restrictions: Do not call the SDK interface in the callback thread.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param itemID Item ID.
    /// @param userList List of users the item unbind.
    virtual void onItemUnbindUpdate(IZegoRangeScene * /*rangeScene*/, long long /*itemID*/,
                                    const std::vector<std::string> & /*userList*/) {}

    /// Item state update callback.
    ///
    /// Available: Since 3.1.0
    /// Description: Item state update callback.
    /// When to trigger: Item state update.
    /// Restrictions: Do not call the SDK interface in the callback thread.
    /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param itemID Item ID.
    /// @param position Item's current position.
    /// @param channel The channel to which the status belongs.
    /// @param status Current status data.
    /// @param statusLength Current status data length.
    virtual void onItemStatusUpdate(IZegoRangeScene * /*rangeScene*/, long long /*itemID*/,
                                    const ZegoPosition & /*position*/, unsigned int /*channel*/,
                                    const unsigned char * /*status*/,
                                    unsigned int /*statusLength*/) {}

    /// Item command update callback.
    ///
    /// Available: Since 3.1.0
    /// Description: Item command update callback.
    /// When to trigger: Item command update.
    /// Restrictions: Do not call the SDK interface in the callback thread.
    /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param itemID Item ID.
    /// @param position Item's current position.
    /// @param channel The channel to which the command belongs.
    /// @param command Next command data.
    /// @param commandLength Next command data length.
    virtual void onItemCommandUpdate(IZegoRangeScene * /*rangeScene*/, long long /*itemID*/,
                                     const ZegoPosition & /*position*/, unsigned int /*channel*/,
                                     const unsigned char * /*command*/,
                                     unsigned int /*commandLength*/) {}
};

class IZegoRangeSceneEventHandler {
  protected:
    virtual ~IZegoRangeSceneEventHandler() {}

  public:
    /// The callback triggered when the scene connection state changes.
    ///
    /// Available: Since 3.0.0
    /// Description: The callback triggered when the scene connection state changes.
    /// When to trigger: Scene connection state changes.
    /// Restrictions: Do not call the SDK interface in the callback thread.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param state Current scene state.
    /// @param errorCode Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
    virtual void onSceneStateUpdate(IZegoRangeScene * /*rangeScene*/, ZegoSceneState /*state*/,
                                    int /*errorCode*/) {}

    /// Callback notification when another user enters the current user's field of view.
    ///
    /// Available: Since 3.0.0
    /// Description: Callback notification when another user enters the current user's field of view.
    /// When to trigger: Another user enters the current user's field of view.
    /// Restrictions: Do not call the SDK interface in the callback thread.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param user User object.
    /// @param position User position.
    virtual void onEnterView(IZegoRangeScene * /*rangeScene*/, const ZegoUser & /*user*/,
                             const ZegoPosition & /*position*/) {}

    /// Callback notification when other users leave the current user's field of view.
    ///
    /// Available: Since 3.0.0
    /// Description: Callback notification when other users leave the current user's field of view.
    /// When to trigger: Other users leave the current user's field of view.
    /// Restrictions: Do not call the SDK interface in the callback thread.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param userID User ID.
    virtual void onLeaveView(IZegoRangeScene * /*rangeScene*/, const std::string & /*userID*/) {}

    /// User state update callback.
    ///
    /// Available: Since 3.0.0
    /// Description: User state update callback.
    /// When to trigger: User state update.
    /// Restrictions: Do not call the SDK interface in the callback thread.
    /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param userID User ID.
    /// @param position User's current position.
    /// @param channel The channel to which the status belongs.
    /// @param status Current status data.
    /// @param statusLength Current status data length.
    virtual void onUserStatusUpdate(IZegoRangeScene * /*rangeScene*/,
                                    const std::string & /*userID*/,
                                    const ZegoPosition & /*position*/, unsigned int /*channel*/,
                                    const unsigned char * /*status*/,
                                    unsigned int /*statusLength*/) {}

    /// User command update callback.
    ///
    /// Available: Since 3.0.0
    /// Description: User command update callback.
    /// When to trigger: User command update.
    /// Restrictions: Do not call the SDK interface in the callback thread.
    /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param userID User ID.
    /// @param position User's current position.
    /// @param channel The channel to which the command belongs.
    /// @param command Next command data.
    /// @param commandLength Next command data length.
    virtual void onUserCommandUpdate(IZegoRangeScene * /*rangeScene*/,
                                     const std::string & /*userID*/,
                                     const ZegoPosition & /*position*/, unsigned int /*channel*/,
                                     const unsigned char * /*command*/,
                                     unsigned int /*commandLength*/) {}

    /// Custom command update callback.
    ///
    /// Available: Since 3.0.0
    /// Description: Custom command update callback.
    /// When to trigger: Custom command update.
    /// Restrictions: Do not call the SDK interface in the callback thread.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param command Custom command.
    /// @param commandLength Custom command data length.
    virtual void onCustomCommandUpdate(IZegoRangeScene * /*rangeScene*/,
                                       const unsigned char * /*command*/,
                                       unsigned int /*commandLength*/) {}

    /// Callback notification that scene Token authentication is about to expire.
    ///
    /// Available since: 3.1.0
    /// Description: The callback notification that the scene Token authentication is about to expire, please use [renewToken] to update the scene Token authentication.
    /// When to call /Trigger: 30 seconds before the Token expires, the SDK will call [onSceneTokenWillExpire] to notify developer.
    /// Restrictions: Do not call the SDK interface in the callback thread.
    /// Related APIs: When the developer receives this callback, he can use [renewToken] to update the token authentication information.
    ///
    /// @param rangeScene Range scene instance that triggers this callback.
    /// @param remainTimeInSecond The remaining time before the token expires.
    virtual void onSceneTokenWillExpire(IZegoRangeScene * /*rangeScene*/,
                                        int /*remainTimeInSecond*/) {}
};

class IZegoScreenCaptureSourceEventHandler {
  protected:
    virtual ~IZegoScreenCaptureSourceEventHandler() {}

  public:
    /// Callback for screen capture data
    ///
    /// Available since: 3.1.0
    /// Description: Callback for screen capture data.
    /// Trigger: This callback is fired after the screen starts capturing [startCapture].
    /// Caution: The callback does not actually take effect until call [setEventHandler] to set.
    ///
    /// @param source Callback screen capture source object.
    /// @param data The screen captures the data of the image frame (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2]).
    /// @param dataLength Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2]).
    /// @param param Screen capture image frame parameters.
    virtual void onAvailableFrame(IZegoScreenCaptureSource * /*source*/, const void * /*data*/,
                                  unsigned int /*dataLength*/, ZegoVideoFrameParam /*param*/) {}

    /// The callback triggered when a screen capture source exception occurred
    ///
    /// Available since: 3.1.0
    /// Description: The callback triggered when a screen capture source exception occurred.
    /// Trigger: This callback is triggered when an exception occurs after the screen start capture.
    /// Caution: The callback does not actually take effect until call [setEventHandler] to set.
    /// Restrictions: Only available on Windows/macOS.
    ///
    /// @param source Callback screen capture source object.
    /// @param exceptionType Capture source exception type.
    virtual void onExceptionOccurred(IZegoScreenCaptureSource * /*source*/,
                                     ZegoScreenCaptureSourceExceptionType /*exceptionType*/) {}

    /// The callback will be triggered when the state of the capture target window change.
    ///
    /// Available since: 3.4.0
    /// Caution: The callback does not actually take effect until call [setEventHandler] to set.
    /// Restrictions: Only available on Windows/macOS.
    ///
    /// @param source Callback screen capture source object.
    /// @param windowState Capture window state.
    /// @param windowRect Capture window rect.
    virtual void onWindowStateChanged(IZegoScreenCaptureSource * /*source*/,
                                      ZegoScreenCaptureWindowState /*windowState*/,
                                      ZegoRect /*windowRect*/) {}

    /// The callback will be triggered when the state of the capture target window change.
    ///
    /// Available since: 3.7.0
    /// Caution: The callback does not actually take effect until call [setEventHandler] to set.
    /// Restrictions: Only available on Windows/macOS.
    ///
    /// @param source Callback screen capture source object.
    /// @param captureRect Capture source rect.
    virtual void onRectChanged(IZegoScreenCaptureSource * /*source*/, ZegoRect /*captureRect*/) {}
};

class IZegoAIVoiceChangerEventHandler {
  protected:
    virtual ~IZegoAIVoiceChangerEventHandler() {}

  public:
    /// Initialize AI voice changer engine status callback.
    ///
    /// Available since: 3.10.0.
    /// Description: Initialize AI voice changer engine status callback.
    /// Trigger: The callback triggered when call [init] function.
    /// Restrictions: None.
    ///
    /// @param aiVoiceChanger Callback AI voice changer instance.
    /// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
    virtual void onInit(IZegoAIVoiceChanger * /*aiVoiceChanger*/, int /*errorCode*/) {}

    /// Update AI voice changer engine models progress callback.
    ///
    /// Available since: 3.12.0.
    /// Description: Update AI voice changer engine models progress callback.
    /// Trigger: The callback triggered when call [update] function.
    /// Restrictions: None.
    ///
    /// @param aiVoiceChanger Callback AI voice changer instance.
    /// @param percent Current file update progress.
    /// @param fileIndex Current update file index.
    /// @param fileCount Total update file count.
    virtual void onUpdateProgress(IZegoAIVoiceChanger * /*aiVoiceChanger*/, double /*percent*/,
                                  int /*fileIndex*/, int /*fileCount*/) {}

    /// Update AI voice changer engine models status callback.
    ///
    /// Available since: 3.10.0.
    /// Description: Update AI voice changer engine models status callback.
    /// Trigger: The callback triggered when call [update] function.
    /// Restrictions: None.
    ///
    /// @param aiVoiceChanger Callback AI voice changer instance.
    /// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
    virtual void onUpdate(IZegoAIVoiceChanger * /*aiVoiceChanger*/, int /*errorCode*/) {}

    /// Get AI voice changer engine available speaker list callback.
    ///
    /// Available since: 3.10.0.
    /// Description: Get AI voice changer engine available speaker list callback.
    /// Trigger: The callback triggered when call [getSpeakerList] function.
    /// Restrictions: None.
    ///
    /// @param aiVoiceChanger Callback AI voice changer instance.
    /// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
    /// @param speakerList Available speaker list.
    virtual void
    onGetSpeakerList(IZegoAIVoiceChanger * /*aiVoiceChanger*/, int /*errorCode*/,
                     const std::vector<ZegoAIVoiceChangerSpeakerInfo> & /*speakerList*/) {}

    /// AI voice changer event callback.
    ///
    /// Available since: 3.12.0.
    /// Description: AI voice changer event callback.
    /// Trigger: The callback triggered when AI voice changer transcode failure occurred.
    /// Restrictions: None.
    ///
    /// @param aiVoiceChanger Callback AI voice changer instance.
    /// @param event AI voice changer event.
    virtual void onEvent(IZegoAIVoiceChanger * /*aiVoiceChanger*/,
                         ZegoAIVoiceChangerEvent /*event*/) {}

    /// Set AI voice changer engine speaker status callback.
    ///
    /// Available since: 3.12.0.
    /// Description: Set AI voice changer engine speaker status callback.
    /// Trigger: The callback triggered when call [setSpeaker] function.
    /// Restrictions: None.
    ///
    /// @param aiVoiceChanger Callback AI voice changer instance.
    /// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
    virtual void onSetSpeaker(IZegoAIVoiceChanger * /*aiVoiceChanger*/, int /*errorCode*/) {}
};

} //namespace EXPRESS
} //namespace ZEGO

#endif // __ZEGOEXPRESSHANDLERS_H__
