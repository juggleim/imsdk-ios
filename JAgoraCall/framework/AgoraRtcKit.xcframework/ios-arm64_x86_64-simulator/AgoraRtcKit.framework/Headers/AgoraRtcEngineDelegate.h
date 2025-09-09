//
//  AgoraRtcEngineKit.h
//  AgoraRtcEngineKit
//
//  Copyright (c) 2018 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AgoraConstants.h"
#import "AgoraObjects.h"
#import "AgoraRtcMediaPlayerProtocol.h"
#import "AgoraH265TranscoderProtocol.h"

/** Agora provides ensured quality of experience (QoE) for worldwide Internet-based voice and video communications through a virtual global network that is especially optimized for real-time web and mobile-to-mobile applications.

 The AgoraRtcEngineKit class is the entry point of the Agora SDK that provides simple APIs for applications to easily start voice and video communication.
 */
@class AgoraRtcEngineKit;
@class AgoraMediaRecorder;

/**
 * The AgoraRtcEngineDelegate protocol enables callback event notifications to your application.

 The SDK uses delegate callbacks in the AgoraRtcEngineDelegate protocol to report runtime events to the application.
 From v1.1, some block callbacks in the SDK are replaced with delegate callbacks. The old block callbacks are therefore deprecated, but can still be used in the current version. However, Agora recommends replacing block callbacks with delegate callbacks. The SDK calls the block callback if a callback is defined in both the block and delegate callbacks.
 */
@protocol AgoraRtcEngineDelegate <NSObject>
@optional

#pragma mark Delegate Methods

/**-----------------------------------------------------------------------------
 * @name Delegate Methods

 The SDK uses delegate callbacks in the AgoraRtcEngineDelegate protocol to report runtime events to the application.
 From v1.1, some block callbacks in the SDK are replaced with delegate callbacks. The old block callbacks are therefore deprecated, but can still be used in the current version. However, Agora recommends replacing block callbacks with delegate callbacks. The SDK calls the block callback if a callback is defined in both the block and delegate callbacks.
 * -----------------------------------------------------------------------------
 */

#pragma mark Core Delegate Methods

/**-----------------------------------------------------------------------------
 * @name Core Delegate Methods
 * -----------------------------------------------------------------------------
 */
/** An error occurred during SDK runtime.

 In most cases, reporting an error means that the SDK cannot fix the issue and resume running, and therefore requires actions from the application or simply informs the user on the issue.
 For instance, the SDK reports an AgoraErrorCodeStartCall = 1002 error when failing to initialize a call. In this case, the application informs the user that the call initialization failed and calls the \ref AgoraRtcEngineKit.leaveChannel: leaveChannel method to exit the channel.

 @param engine    AgoraRtcEngineKit object
 @param errorCode AgoraErrorCode
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didOccurError:(AgoraErrorCode)errorCode NS_SWIFT_NAME(rtcEngine(_:didOccurError:));

/**
 * Occurs when the token has expired.
 *
 * If a token is specified when calling `joinChannelByToken`,
 * the token expires after a certain period of time and you need a new token to
 * reconnect to the server.
 *
 * Upon receiving this callback, generate a new token at your app server and
 * call \ref AgoraRtcEngineKit.renewToken: renewToken to pass the new token
 * to the SDK.
 * @param engine The AgoraRtcEngineKit object.
 * @sa [How to generate a token](https://docs.agora.io/en/Interactive%20Broadcast/token_server_cpp?platform=CPP).
 */
- (void)rtcEngineRequestToken:(AgoraRtcEngineKit * _Nonnull)engine NS_SWIFT_NAME(rtcEngineRequestToken(_:));

/**
 * Occurs when the token will expire in 30 seconds.
 *
 * If the token you specified when calling
 * `joinChannelByToken`
 * expires, the user drops offline. This callback is triggered 30 seconds
 * before the token expires, to remind you to renew the token.
 * Upon receiving this callback, generate a new token at your app server and
 * call \ref AgoraRtcEngineKit.renewToken: renewToken to pass the new token
 * to the SDK.
 * @param engine The AgoraRtcEngineKit object.
 * @param token  The token that will expire in 30 seconds.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine tokenPrivilegeWillExpire:(NSString *_Nonnull)token NS_SWIFT_NAME(rtcEngine(_:tokenPrivilegeWillExpire:));

 /**
   * @brief Reports the result of calling renewToken.
   * @since v4.6.0
   * 
   * Occurs when a user renews the token.
   * 
   * This callback notifies the app of the result after the user calls \ref AgoraRtcEngineKit.renewToken: renewToken to renew the token.
   * The app can obtain the result of the \ref AgoraRtcEngineKit.renewToken: renewToken call from this callback.
   *
   * @param engine The AgoraRtcEngineKit object.
   * @param token The token.
   * @param code The error code.
   */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine renewTokenResult:(NSString *_Nonnull)token code:(AgoraRenewTokenErrorCode)code NS_SWIFT_NAME(rtcEngine(_:renewTokenResult:code:));


/**
   * Occurs when connection license verification fails
   *
   * You can know the reason accordding to error code
   */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine licenseValidationFailure:(AgoraLicenseVerifyCode) error;

/** The SDK disconnected from the server.

 Once the connection is lost, the SDK tries to reconnect it repeatedly until the application calls [leave](@ref AgoraRtcEngineKit.leaveChannel:) .

 @param engine AgoraRtcEngineKit object
 */
- (void)rtcEngineConnectionDidInterrupted:(AgoraRtcEngineKit * _Nonnull)engine NS_SWIFT_NAME(rtcEngineConnectionDidInterrupted(_:));


/**
 * Occurs when the SDK cannot reconnect to Agora's edge server 10 seconds after
 * its connection to the server is interrupted.
 *
 * The SDK triggers this callback when it cannot connect to the server 10 seconds after calling
 * `joinChannelByToken`, regardless of whether it is in the channel or not.
 * @param engine The AgoraRtcEngineKit object.
 */
- (void)rtcEngineConnectionDidLost:(AgoraRtcEngineKit * _Nonnull)engine NS_SWIFT_NAME(rtcEngineConnectionDidLost(_:));


/** A connection is banned by the server.

 The SDK will not try to reconnect the server.

 @param engine AgoraRtcEngineKit object
 */
- (void)rtcEngineConnectionDidBanned:(AgoraRtcEngineKit * _Nonnull)engine NS_SWIFT_NAME(rtcEngineConnectionDidBanned(_:));

/**
 * Occurs when the connection state of the SDK to the server is changed.
 *
 * @param engine    The AgoraRtcEngineKit object.
 * @param type See \ref AgoraNetworkType.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine networkTypeChanged:(AgoraNetworkType)type NS_SWIFT_NAME(rtcEngine(_:networkTypeChanged:));

/**
 * Occurs when permission error
 *
 * @param engine    The AgoraRtcEngineKit object.
 * @param type See \ref AgoraPermissionType.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine permissionError:(AgoraPermissionType)type NS_SWIFT_NAME(rtcEngine(_:permissionError:));

/**
 * Occurs when the connection state of the SDK to the server is changed.
 *
 * @param engine    The AgoraRtcEngineKit object.
 * @param state See \ref AgoraConnectionState.
 * @param reason See \ref AgoraConnectionChangedReason.
 */
- (void)rtcEngine:(AgoraRtcEngineKit* _Nonnull)engine connectionChangedToState:(AgoraConnectionState)state reason:(AgoraConnectionChangedReason)reason NS_SWIFT_NAME(rtcEngine(_:connectionChangedTo:reason:));

/**
 * Reports the statistics of the current call.
 *
 * This callback is triggered once every two seconds after the user joins the channel.
 *
 * @param engine    The AgoraRtcEngineKit object.
 * @param stats The statistics on the current call: #AgoraChannelStats.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine reportRtcStats:(AgoraChannelStats * _Nonnull)stats NS_SWIFT_NAME(rtcEngine(_:reportRtcStats:));


/** The last mile network quality of the local user.

 This callback is triggered once after you have called [startLastmileProbeTest]([AgoraRtcEngineKit startLastmileProbeTest]) to report the network quality of the local user.

 @param engine  AgoraRtcEngineKit object
 @param quality AgoraNetworkQuality
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine lastmileQuality:(AgoraNetworkQuality)quality NS_SWIFT_NAME(rtcEngine(_:lastmileQuality:));


/**
 * Reports the last-mile network probe result.
 *
 * The SDK triggers this callback within 30 seconds after the app calls the \ref AgoraRtcEngineKit.startLastmileProbeTest: startLastmileProbeTest method.
 * @param engine The AgoraRtcEngineKit object.
 * @param result The uplink and downlink last-mile network probe test result, see \ref AgoraLastmileProbeResult.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine lastmileProbeTestResult:(AgoraLastmileProbeResult * _Nonnull)result NS_SWIFT_NAME(rtcEngine(_:lastmileProbeTest:));


#if (!(TARGET_OS_IPHONE) && (TARGET_OS_MAC))

/** The specified device state.

 @note  This method applies to macOS only.

 @param engine     AgoraRtcEngineKit object
 @param deviceId   Device ID
 @param deviceType AgoraMediaDeviceType
 @param state      State of the device:

 * 0: Added.
 * 8: Removed.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine device:(NSString * _Nonnull)deviceId type:(AgoraMediaDeviceType)deviceType stateChanged:(NSInteger)state NS_SWIFT_NAME(rtcEngine(_:device:type:stateChanged:));

#endif

/** An error of encryption occurred during SDK runtime.

 @param errorType AgoraEncryptionErrorType
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didOccurEncryptionError:(AgoraEncryptionErrorType)errorType NS_SWIFT_NAME(rtcEngine(_:didOccur:));
/**
   * Reports the user log upload result
   * @param requestId RequestId of the upload
   * @param success Is upload success
   * @param reason Reason of the upload, 0: OK, 1 Network Error, 2 Server Error.
   */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine uploadLogResultRequestId:(NSString * _Nonnull)requestId success:(BOOL)success reason:(AgoraUploadErrorReason)reason NS_SWIFT_NAME(rtcEngine(_:uploadLogResultRequestId:success:reason:));

#pragma mark Local User Core Delegate Methods

/**-----------------------------------------------------------------------------
 * @name Local User Core Delegate Methods
 * -----------------------------------------------------------------------------
 */

/**
 * Occurs when the local user successfully joins a specified channel.
 *
 * @param engine  AgoraRtcEngineKit object
 * @param channel The channel name.
 * @param uid The user ID.
 * @param elapsed The time elapsed (ms) from the local user calling `joinChannelByToken` until this event occurs.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didJoinChannel:(NSString * _Nonnull)channel withUid:(NSUInteger)uid elapsed:(NSInteger)elapsed NS_SWIFT_NAME(rtcEngine(_:didJoinChannel:withUid:elapsed:));

/**
 * Occurs when the local user rejoins a channel.
 *
 * If the client loses connection with the server because of network problems,
 * the SDK automatically attempts to reconnect and then triggers this callback
 * upon reconnection, indicating that the user rejoins the channel with the
 * assigned channel ID and user ID.
 *
 * @param engine  The AgoraRtcEngineKit object.
 * @param channel The channel name.
 * @param uid     The user ID.
 * @param elapsed Time elapsed (ms) from the local user calling `joinChannelByToken` until this event occurs.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didRejoinChannel:(NSString * _Nonnull)channel withUid:(NSUInteger)uid elapsed:(NSInteger)elapsed NS_SWIFT_NAME(rtcEngine(_:didRejoinChannel:withUid:elapsed:));

/**
 * Occurs when the local user role switches in a live broadcast.
 *
 * @param engine  The AgoraRtcEngineKit object.
 * @param oldRole The role that the user switches from: #AgoraClientRole.
 * @param newRole The role that the user switches to: #AgoraClientRole.
 * @param newRoleOptions The client role option of the new role: #AgoraClientRoleOptions.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didClientRoleChanged:(AgoraClientRole)oldRole newRole:(AgoraClientRole)newRole newRoleOptions:(AgoraClientRoleOptions * _Nullable)newRoleOptions NS_SWIFT_NAME(rtcEngine(_:didClientRoleChanged:newRole:newRoleOptions:));

/**
 * Occurs when the local user role switches in a live broadcast.
 *
 * @param engine  The AgoraRtcEngineKit object.
 * @param reason  The reason of the failure of the local user role switches: #AgoraClientRoleChangeFailedReason.
 * @param currentRole The current role of the user: #AgoraClientRole.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didClientRoleChangeFailed:(AgoraClientRoleChangeFailedReason)reason currentRole:(AgoraClientRole)currentRole NS_SWIFT_NAME(rtcEngine(_:didClientRoleChangeFailed:currentRole:));

/**
 * Occurs when the local user leaves a channel.
 *
 * When the user successfully leaves the channel after calling
 * \ref AgoraRtcEngineKit.leaveChannel: leaveChannel method, this callback
 * notifies the app that a user leaves a channel.
 *
 * This callback also reports information such as the call duration and the
 * statistics of data received or transmitted by the SDK.
 * @param engine The AgoraRtcEngineKit object.
 * @param stats  The statistics of the call. See #AgoraChannelStats.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didLeaveChannelWithStats:(AgoraChannelStats * _Nonnull)stats NS_SWIFT_NAME(rtcEngine(_:didLeaveChannelWith:));


/** The network quality of a specified user in a channel.

 This callback is triggered every two seconds to update the application on the network quality of the specified user in a communication or live broadcast channel.

 @param engine    AgoraRtcEngineKit object
 @param uid       User ID
 @param txQuality Network transmission quality defined in AgoraNetworkQuality.
 @param rxQuality Network receiving quality defined in AgoraNetworkQuality.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine networkQuality:(NSUInteger)uid txQuality:(AgoraNetworkQuality)txQuality rxQuality:(AgoraNetworkQuality)rxQuality NS_SWIFT_NAME(rtcEngine(_:networkQuality:txQuality:rxQuality:));


#pragma mark Local User Audio Delegate Methods

/**-----------------------------------------------------------------------------
 * @name Local User Audio Delegate Methods
 * -----------------------------------------------------------------------------
 */
/**
 * Occurs when the first local audio frame is published.
 *
 * @param engine  The AgoraRtcEngineKit object.
 * @param elapsed The time elapsed (ms) from calling `joinChannelByToken` until the SDK triggers this callback.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine firstLocalAudioFramePublished:(NSInteger)elapsed NS_SWIFT_NAME(rtcEngine(_:firstLocalAudioFramePublished:));

/**
 * Reports the statistics of the local audio stream.
 *
 * The SDK triggers this callback once every two seconds.
 * @param engine The AgoraRtcEngineKit object.
 * @param stats  The statistics of the local audio stream. See \ref AgoraRtcLocalAudioStats.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine localAudioStats:(AgoraRtcLocalAudioStats * _Nonnull)stats NS_SWIFT_NAME(rtcEngine(_:localAudioStats:));

/**
 * Occurs when the local audio stream state changes.
 *
 * This callback indicates the state change of the local audio stream, including
 * the state of the audio recording and encoding, and allows you to troubleshoot
 * issues when exceptions occur.
 *
 * @note
 * When the state is `AgoraAudioLocalStateFailed`(3), see the `error` parameter for details.
 * @param engine AgoraRtcEngineKit object
 * @param state  The state of the local audio. See \ref AgoraAudioLocalState.
 * @param reason The reason information of the local audio. See \ref AgoraAudioLocalReason.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine localAudioStateChanged:(AgoraAudioLocalState)state reason:(AgoraAudioLocalReason)reason NS_SWIFT_NAME(rtcEngine(_:localAudioStateChanged:reason:));

/**
 * Occurs when the local audio route changes.
 *
 * The SDK triggers this callback when the local audio route changes.
 * @param engine  The AgoraRtcEngineKit object.
 * @param routing The audio route. See \ref AgoraAudioOutputRouting.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didAudioRouteChanged:(AgoraAudioOutputRouting)routing NS_SWIFT_NAME(rtcEngine(_:didAudioRouteChanged:));

/** The audio mixing file playback stopped after calling [startAudioMixing]([AgoraRtcEngineKit startAudioMixing:loopback:replace:cycle:]).
  @deprecated Use [rtcEngine:audioMixingStateChanged:reasonCode:] instead.

 If you failed to call [startAudioMixing]([AgoraRtcEngineKit startAudioMixing:loopback:replace:cycle:]), it returns the error code in the [didOccurError]([AgoraRtcEngineDelegate rtcEngine:didOccurError:])  callback.

 @param engine AgoraRtcEngineKit object
 */
- (void)rtcEngineLocalAudioMixingDidFinish:(AgoraRtcEngineKit * _Nonnull)engine NS_SWIFT_NAME(rtcEngineLocalAudioMixingDidFinish(_:)) __deprecated_msg("Use rtcEngine:audioMixingStateChanged:reasonCode: instead");

/** Occurs when the local audio effect playback finishes.

 You can start a local audio effect playback by calling the \ref AgoraRtcEngineKit.playEffect:filePath:loopCount:pitch:pan:gain:publish: playEffect method. The SDK triggers this callback when the local audio effect file playback finishes.

 @param engine  AgoraRtcEngineKit object.
 @param soundId ID of the local audio effect. Each local audio effect has a unique ID.
 */
- (void)rtcEngineDidAudioEffectFinish:(AgoraRtcEngineKit * _Nonnull)engine soundId:(int)soundId NS_SWIFT_NAME(rtcEngineDidAudioEffectFinish(_:soundId:));


#pragma mark Local User Video Delegate Methods

/**-----------------------------------------------------------------------------
 * @name Local User Video Delegate Methods
 * -----------------------------------------------------------------------------
 */


/** A camera turned on and is ready to capture video.
 @deprecated Use [rtcEngine:localVideoStateChangedOfState:reason:sourceType:] instead.

 @param engine AgoraRtcEngineKit object
 */
- (void)rtcEngineCameraDidReady:(AgoraRtcEngineKit * _Nonnull)engine NS_SWIFT_NAME(rtcEngineCameraDidReady(_:)) __deprecated_msg("Use rtcEngine:localVideoStateChangedOfState:reason:sourceType: instead");

#if TARGET_OS_IPHONE

/** A camera focus position changed.

 @param engine AgoraRtcEngineKit object
 @param rect   Focus rectangle in the local preview
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine cameraFocusDidChangedToRect:(CGRect)rect NS_SWIFT_NAME(rtcEngine(_:cameraFocusDidChangedTo:));

/** A camera exposure position changed.

 @param engine AgoraRtcEngineKit object
 @param rect   Exposure rectangle in the local preview
 */
- (void)rtcEngine:(AgoraRtcEngineKit* _Nonnull)engine cameraExposureDidChangedToRect:(CGRect)rect NS_SWIFT_NAME(rtcEngine(_:cameraExposureDidChangedTo:));

#endif

/** The video playback stopped.
 @deprecated Use [rtcEngine:localVideoStateChangedOfState:reason:sourceType:] instead.

 The application can use this callback to change the configuration of the view (for example, displaying other pictures in the view) after the video stops.

 @param engine AgoraRtcEngineKit object
 */
- (void)rtcEngineVideoDidStop:(AgoraRtcEngineKit * _Nonnull)engine NS_SWIFT_NAME(rtcEngineVideoDidStop(_:)) __deprecated_msg("Use rtcEngine:localVideoStateChangedOfState:reason:sourceType: instead");

/** Event of the first local video frame is published.
 *  @param engine  The engine kit
 *  @param elapsed The elapsed time(ms) from the beginning of the session.
 *  @param sourceType source type of the orignated video source. See \ref AgoraVideoSourceType.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine firstLocalVideoFramePublishedWithElapsed:(NSInteger)elapsed sourceType:(AgoraVideoSourceType)sourceType NS_SWIFT_NAME(rtcEngine(_:firstLocalVideoFramePublishedWithElapsed:sourceType:));

/** The first local frame displayed on the video window.

 Same as [firstLocalVideoFrameBlock]([AgoraRtcEngineKit firstLocalVideoFrameBlock:]).

 @param engine  The AgoraRtcEngineKit object.
 @param size    Size of the video (width and height).
 @param elapsed Time elapsed (ms) from calling `joinChannelByToken` until this callback is triggered.
 @param sourceType source type of the orignated video source. See \ref AgoraVideoSourceType.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine firstLocalVideoFrameWithSize:(CGSize)size elapsed:(NSInteger)elapsed sourceType:(AgoraVideoSourceType)sourceType NS_SWIFT_NAME(rtcEngine(_:firstLocalVideoFrameWith:elapsed:sourceType:));

/** Reports the statistics of the local video stream.

 * The SDK triggers this callback once every two seconds for each
 * user/host. If there are multiple users/hosts in the channel, the SDK
 * triggers this callback as many times.
 *
 * @note If you have called the
 * \ref AgoraRtcEngineKit.enableDualStreamMode: enableDualStreamMode
 * method, this callback reports the statistics of the high-video
 * stream (high bitrate, and high-resolution video stream).
 * @param engine The \ref AgoraRtcEngineKitobject.
 * @param stats  Statistics of the local video stream. See \ref AgoraRtcLocalVideoStats.
 * @param sourceType source type of the orignated video source. See \ref AgoraVideoSourceType.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine localVideoStats:(AgoraRtcLocalVideoStats * _Nonnull)stats sourceType:(AgoraVideoSourceType)sourceType NS_SWIFT_NAME(rtcEngine(_:localVideoStats:sourceType:));

/**
 * Reports the tracing result of video rendering event of the user.
 *
 * @param uid The user ID.
 * @param currentEvent The current event of the tracing result: #AgoraMediaTraceEvent.
 * @param tracingInfo The tracing result: #AgoraVideoRenderingTracingInfo.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine videoRenderingTracingResultOfUid:(NSUInteger)uid currentEvent:(AgoraMediaTraceEvent)currentEvent tracingInfo:(AgoraVideoRenderingTracingInfo * _Nonnull)tracingInfo NS_SWIFT_NAME(rtcEngine(_:videoRenderingTracingResultOfUid:currentEvent:tracingInfo:));



#pragma mark Remote User Core Delegate Methods

/**-----------------------------------------------------------------------------
 * @name Remote User Core Delegate Methods
 * -----------------------------------------------------------------------------
 */

/**
 * Occurs when a remote user or user joins the channel.
 *
 * If other users or hosts are already in the channel, the SDK also reports to
 * the app on the existing users/hosts.
 *
 * The SDK triggers this callback under one of the following circumstances:
 * - A remote user/host joins the channel by calling `joinChannelByToken`.
 * - A remote user switches the user role to the host by calling
 * \ref AgoraRtcEngineKit.setClientRole: setClientRole method after joining
 * the channel.
 * - A remote user/host rejoins the channel after a network interruption.
 * @note
 * When a web user joins the channel, this callback is triggered as long as the
 * user publishes a stream.
 @param engine  The AgoraRtcEngineKit object.
 @param uid     The user ID.
 @param elapsed Time elapsed (ms) from calling `joinChannelByToken` until this callback is triggered.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didJoinedOfUid:(NSUInteger)uid elapsed:(NSInteger)elapsed NS_SWIFT_NAME(rtcEngine(_:didJoinedOfUid:elapsed:));

/**
 * Occurs when a remote user or host goes offline.
 *
 * There are two reasons for a user to go offline:
 * - Leave the channel: When the user leaves the channel, the user sends a
 * goodbye message. When this message is received, the SDK determines that the
 * user leaves the channel.
 * - Drop offline: When no data packet of the user is received for a certain
 * period of time, the SDK assumes that the user drops offline. A poor network
 * connection may lead to false detection, so we recommend using
 * the RTM SDK for reliable offline detection.
 *
 * @param engine The AgoraRtcEngineKit object.
 * @param uid The ID of the user who goes offline.
 * @param reason The reason why the user goes offline: #AgoraUserOfflineReason.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didOfflineOfUid:(NSUInteger)uid reason:(AgoraUserOfflineReason)reason NS_SWIFT_NAME(rtcEngine(_:didOfflineOfUid:reason:));


/** Occurs when the user receives the data stream.

The SDK triggers this callback when the local user receives the stream message that the remote user sends by calling the \ref AgoraRtcEngineKit.sendStreamMessage:data: sendStreamMessage method within five seconds.

 @param engine   AgoraRtcEngineKit object.
 @param uid      User ID of the remote user sending the message.
 @param streamId Stream ID.
 @param data     Data received by the local user.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine receiveStreamMessageFromUid:(NSUInteger)uid streamId:(NSInteger)streamId data:(NSData * _Nonnull)data NS_SWIFT_NAME(rtcEngine(_:receiveStreamMessageFromUid:streamId:data:));

/** Occurs when the user does not receive the data stream.

 The SDK triggers this callback when the local user fails to receive the stream message that the remote user sends by calling the \ref AgoraRtcEngineKit.sendStreamMessage:data: sendStreamMessage
  method within five seconds.

 @param engine   AgoraRtcEngineKit object.
 @param uid      User ID of the remote user sending the message.
 @param streamId Stream ID.
 @param error    Error code. See \ref AgoraErrorCode.
 @param missed Number of lost messages.
 @param cached Number of incoming cached messages when the data stream is interrupted.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didOccurStreamMessageErrorFromUid:(NSUInteger)uid streamId:(NSInteger)streamId error:(NSInteger)error missed:(NSInteger)missed cached:(NSInteger)cached NS_SWIFT_NAME(rtcEngine(_:didOccurStreamMessageErrorFromUid:streamId:error:missed:cached:));


/**
 * @brief Occurs when the local user receives data via Reliable Data Transmission (RDT) from a remote user.
 *
 * @technical preview
 *
 *  @param engine The engine kit
 *  @param uid The remote user id
 *  @param type The stream type. See AgoraRdtStreamType.
 *  @param data The data received.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine receiveRdtMessageFromUid:(NSUInteger)uid type:(AgoraRdtStreamType)type data:(NSData * _Nonnull)data NS_SWIFT_NAME(rtcEngine(_:receiveRdtMessageFromUid:type:data:));

/**
 * @brief Occurs when the RDT tunnel state changed
 *
 * @technical preview
 *
 *  @param engine The engine kit
 *  @param uid The remote user id
 *  @param state  The RDT state. See AgoraRdtState.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didOccurRdtMessageStateFromUid:(NSUInteger)uid state:(AgoraRdtState)state NS_SWIFT_NAME(rtcEngine(_:didOccurRdtMessageStateFromUid:state:));

/**
 * @brief Occurs when the local user receives media control message sent by a remote user.
 *
 * @technical preview
 *
 *  @param engine The engine kit
 *  @param uid The remote user id
 *  @param data The data received.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine receiveMediaControlMessageFromUid:(NSUInteger)uid data:(NSData * _Nonnull)data NS_SWIFT_NAME(rtcEngine(_:receiveMediaControlMessageFromUid:data:));


#pragma mark Remote User Audio Delegate Methods

/**-----------------------------------------------------------------------------
 * @name Remote User Audio Delegate Methods
 * -----------------------------------------------------------------------------
 */


/** The first audio frame received and decoded from the remote user.

 @param engine  The AgoraRtcEngineKit object.
 @param uid     Remote user ID.
 @param elapsed Time elapsed (ms) from calling `joinChannelByToken` until this callback is triggered.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine firstRemoteAudioFrameOfUid:(NSUInteger)uid elapsed:(NSInteger)elapsed NS_SWIFT_NAME(rtcEngine(_:firstRemoteAudioFrameOfUid:elapsed:));


/** Occurs when the SDK decodes the first remote audio frame for playback.

 **Deprecated**  from v3.0.0. Use `AgoraAudioRemoteStateDecoding(2)` in the [remoteAudioStateChangedOfUid]([AgoraRtcEngineDelegate rtcEngine:remoteAudioStateChangedOfUid:state:reason:elapsed:]) callback instead.

 This callback is triggered in either of the following scenarios:

 - The remote user joins the channel and sends the audio stream.
 - The remote user stops sending the audio stream and re-sends it after 15 seconds. Reasons for such an interruption include:

     - The remote user leaves channel.
     - The remote user drops offline.
     - The remote user calls the [muteLocalAudioStream]([AgoraRtcEngineKit muteLocalAudioStream:]) method to stop sending the local audio stream.
     - The remote user calls the [disableAudio]([AgoraRtcEngineKit disableAudio]) method to disable audio.

 @param engine AgoraRtcEngineKit object.
 @param uid User ID of the remote user sending the audio stream.
 @param elapsed The time elapsed (ms) from the local user calling the joinChannel method until the SDK triggers this callback.
 */
- (void)rtcEngine:(AgoraRtcEngineKit* _Nonnull)engine firstRemoteAudioFrameDecodedOfUid:(NSUInteger)uid elapsed:(NSInteger)elapsed NS_SWIFT_NAME(rtcEngine(_:firstRemoteAudioFrameDecodedOfUid:elapsed:)) __deprecated_msg("use rtcEngine:remoteAudioStateChangedOfUid:state:reason:elapsed: instead.");

/**
 * Reports the statistics of the remote audio stream.
 *
 * The SDK triggers this callback once every two seconds for each remote user or broadcaster. If a
 * channel has multiple remote users, the SDK triggers this callback as many times.
 * @param engine The AgoraRtcEngineKit object.
 * @param stats  The statistics of the received audio. See \ref AgoraRtcRemoteAudioStats.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine remoteAudioStats:(AgoraRtcRemoteAudioStats * _Nonnull)stats NS_SWIFT_NAME(rtcEngine(_:remoteAudioStats:));

/** A remote user's audio was muted or unmuted.

 @param engine AgoraRtcEngineKit object
 @param muted  Mute or unmute
 @param uid    Remote user ID
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didAudioMuted:(BOOL)muted byUid:(NSUInteger)uid NS_SWIFT_NAME(rtcEngine(_:didAudioMuted:byUid:));


/**
 * This callback reports the IDs and volumes of the loudest speakers at the
 * moment in the channel, and whether the local user is speaking.
 *
 * Once enabled, this callback is triggered at the set interval, regardless of
 * whether a user speaks or not.
 *
 * The SDK triggers two independent `reportAudioVolumeIndicationOfSpeakers`
 * callbacks at one time, which separately report the volume information of the
 * local user and all the remote speakers.
 *
 * @param engine      The AgoraRtcEngineKit object.
 * @param speakers    An array containing the user ID and volume information
 * for each speaker: #AgoraRtcAudioVolumeInfo.
 * - In the local user's callback, this array contains the following members:
 *   - `uid` = 0,
 *   - `volume` = `totalVolume`, which reports the sum of the voice volume
 * and audio-mixing volume of the local user.
 * - In the remote users' callback, this array contains the following members:
 *   - `uid` of each remote speaker.
 *   - `volume`, which reports the sum of the voice volume and audio-mixing
 * volume of each remote speaker.
 * An empty `speakers` array in the callback indicates that no remote user is
 * speaking at the moment.
 * @param totalVolume The total volume after audio mixing. The value ranges
 * between 0 (the lowest volume) and 255 (the highest volume).
 * - In the local user's callback, `totalVolume` is the sum of the voice volume
 * and audio-mixing volume of the local user.
 * - In the remote users' callback, `totalVolume` is the sum of the voice
 * volume and audio-mixing volume of all the remote speakers.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine reportAudioVolumeIndicationOfSpeakers:(NSArray<AgoraRtcAudioVolumeInfo *> * _Nonnull)speakers totalVolume:(NSInteger)totalVolume NS_SWIFT_NAME(rtcEngine(_:reportAudioVolumeIndicationOfSpeakers:totalVolume:));


/**
 * Occurs when an active speaker is detected.
 *
 * If you have called \ref AgoraRtcEngineKit.enableAudioVolumeIndication:smooth: enableAudioVolumeIndication,
 * this callback is triggered when the volume detecting unit has detected an
 * active speaker in the channel. This callback also returns the `uid` of the
 * active speaker.
 *
 * @note
 * - You need to call `enableAudioVolumeIndication` to receive this callback.
 * - The active speaker means the user ID of the speaker who speaks at the
 * highest volume during a certain period of time.
 * @param engine The AgoraRtcEngineKit object.
 * @param speakerUid The ID of the active speaker. A `speakerUid` of 0 means
 * the local user.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine activeSpeaker:(NSUInteger)speakerUid NS_SWIFT_NAME(rtcEngine(_:activeSpeaker:));

/** The audio quality of the specified user every two seconds. Same as [audioQualityBlock]([AgoraRtcEngineKit audioQualityBlock:]).
 @deprecated Use [rtcEngine:remoteAudioStats:] instead.

 @param engine  The AgoraRtcEngineKit object.
 @param uid     User ID of the speaker.
 @param quality AgoraNetworkQuality
 @param delay   Time delay (ms).
 @param lost    Packet loss rate (%).
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine audioQualityOfUid:(NSUInteger)uid quality:(AgoraNetworkQuality)quality delay:(NSUInteger)delay lost:(NSUInteger)lost NS_SWIFT_NAME(rtcEngine(_:audioQualityOfUid:quality:delay:lost:)) __deprecated_msg("use rtcEngine:remoteAudioStats: instead");

/** The remote audio transport statistics.

 This callback is triggered every two seconds after the user receives the audio data packet sent from a remote user.
 @deprecated Use [rtcEngine:remoteAudioStats:] instead.

 @param engine     The AgoraRtcEngineKit object.
 @param uid        User ID of the remote user whose audio data packet has been received.
 @param delay      Time delay (ms) from the remote user to the local client.
 @param lost       Packet loss rate (%).
 @param rxKBitRate Received audio bitrate (kbit/s) of the packet from the remote user.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine audioTransportStatsOfUid:(NSUInteger)uid delay:(NSUInteger)delay lost:(NSUInteger)lost rxKBitRate:(NSUInteger)rxKBitRate NS_SWIFT_NAME(rtcEngine(_:audioTransportStatsOfUid:delay:lost:rxKBitRate:)) __deprecated_msg("use rtcEngine:remoteAudioStats: instead");


/** Intra request received.
 * @param engine  The AgoraRtcEngineKit object.
*/
- (void)rtcEngineIntraRequestReceived:(AgoraRtcEngineKit *_Nonnull)engine NS_SWIFT_NAME(rtcEngineIntraRequestReceived(_:));

/** Target bitrate updated.
 * @param engine   The AgoraRtcEngineKit object.
 * @param networkInfo  The uplink network info, including target bitrate bps.
*/
- (void)rtcEngine:(AgoraRtcEngineKit *_Nonnull)engine uplinkNetworkInfoUpdate:(AgoraUplinkNetworkInfo *_Nonnull)networkInfo NS_SWIFT_NAME(rtcEngine(_:uplinkNetworkInfoUpdate:));

/**
 * Occurs when the audio subscribe state changed.
 *
 * @param engine       The AgoraRtcEngineKit object.
 * @param channelId The channel name of user joined.
 * @param uid The remote user ID that is subscribed to.
 * @param oldState The old state of the audio stream subscribe : #AgoraStreamSubscribeState.
 * @param newState The new state of the audio stream subscribe : #AgoraStreamSubscribeState.
 * @param elapseSinceLastState The time elapsed (ms) from the old state to the new state.
*/
- (void)rtcEngine:(AgoraRtcEngineKit *_Nonnull)engine didAudioSubscribeStateChange:(NSString * _Nonnull)channelId
                uid:(unsigned int)uid
           oldState:(AgoraStreamSubscribeState)oldState
           newState:(AgoraStreamSubscribeState)newState
elapseSinceLastState:(int)elapseSinceLastState NS_SWIFT_NAME(rtcEngine(_:didAudioSubscribeStateChange:uid:oldState:newState:elapseSinceLastState:));

/**
 * Occurs when the audio publish state changed.
 *
 * @param channelId The channel name of user joined.
 * @param oldState The old state of the audio stream publish : #AgoraStreamPublishState.
 * @param newState The new state of the audio stream publish : #AgoraStreamPublishState.
 * @param elapseSinceLastState The time elapsed (ms) from the old state to the new state.
 */
- (void)rtcEngine:(AgoraRtcEngineKit *_Nonnull)engine didAudioPublishStateChange:(NSString * _Nonnull)channelId
           oldState:(AgoraStreamPublishState)oldState
           newState:(AgoraStreamPublishState)newState
elapseSinceLastState:(int)elapseSinceLastState NS_SWIFT_NAME(rtcEngine(_:didAudioPublishStateChange:oldState:newState:elapseSinceLastState:));

#pragma mark String UID

/** Occurs when the local user successfully registers a user account by calling the
 * This callback reports the user ID and user account of the local user.
 * @param engine  The AgoraRtcEngineKit object.
 * @param uid The ID of the local user.
 * @param userAccount The user account of the local user.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didLocalUserRegisteredWithUserId:(NSUInteger)uid userAccount:(NSString * _Nonnull)userAccount NS_SWIFT_NAME(rtcEngine(_:didLocalUserRegisteredWithUserId:userAccount:));

/** Occurs when the SDK gets the user ID and user account of the remote user.

 * After a remote user joins the channel, the SDK gets the UID and user account of the remote user,
 * caches them in a mapping table object (`userInfo`), and triggers this callback on the local client.

 * @param engine  The AgoraRtcEngineKit object.
 * @param uid The ID of the remote user.
 * @param userInfo The `AgoraUserInfo` object that contains the user ID and user account of the remote user.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didUserInfoUpdatedWithUserId:(NSUInteger)uid userInfo:(AgoraUserInfo* _Nonnull)userInfo NS_SWIFT_NAME(rtcEngine(_:didUserInfoUpdatedWithUserId:userInfo:));

#pragma mark Rhythm Player Delegates Methods

- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didRhythmPlayerStateChanged:(AgoraRhythmPlayerState)state
        reason:(AgoraRhythmPlayerReason)reason NS_SWIFT_NAME(rtcEngine(_:didRhythmPlayerStateChanged:reason:));

#pragma mark  Local Video Transcoder Delegates Methods
/**
 * Occurs when local video transcoder stream has error.
 *
 * @param stream Stream type of AgoraTranscodingVideoStream.
 * @param errorCode Error code of AgoraVideoTranscoderError.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didLocalVideoTranscoderErrorWithStream:(AgoraTranscodingVideoStream * _Nonnull )stream
        errorCode:(AgoraVideoTranscoderError)errorCode NS_SWIFT_NAME(rtcEngine(_:didLocalVideoTranscoderErrorWithStream:errorCode:));

#pragma mark Remote User Video Delegates Methods

/**-----------------------------------------------------------------------------
 * @name Remote User Video Delegates Methods
 * -----------------------------------------------------------------------------
 */


/** Occurs when the first frame of the remote user was decoded successfully.

 @deprecated This callback is deprecated. Use [remoteVideoStateChangedOfUid](remoteVideoStateChangedOfUid:state:reason:elapsed:)
   instead.

  This callback is triggered upon the SDK receiving and successfully decoding
  the first video frame from a remote video. The app can configure the
  user view settings in this delegate.

 @param engine  The AgoraRtcEngineKit object.
 @param uid     ID of the user whose video streams are received.
 @param size    Size of the video (width and height) in pixels.
 @param elapsed Time elapsed (ms) from calling `joinChannelByToken` until this callback is triggered.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine firstRemoteVideoDecodedOfUid:(NSUInteger)uid size:(CGSize)size elapsed:(NSInteger)elapsed NS_SWIFT_NAME(rtcEngine(_:firstRemoteVideoDecodedOfUid:size:elapsed:)) __deprecated_msg("use rtcEngine:remoteVideoStateChangedOfUid:state:reason: instead.");

/** The first frame of the remote user was displayed successfully. Same as [firstRemoteVideoFrameBlock]([AgoraRtcEngineKit firstRemoteVideoFrameBlock:]).

 @param engine  The AgoraRtcEngineKit object.
 @param uid     Remote user ID.
 @param size    Size of the video (width and height) in pixels.
 @param elapsed Time elapsed (ms) from calling `joinChannelByToken` until this callback is triggered.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine firstRemoteVideoFrameOfUid:(NSUInteger)uid size:(CGSize)size elapsed:(NSInteger)elapsed NS_SWIFT_NAME(rtcEngine(_:firstRemoteVideoFrameOfUid:size:elapsed:));

  /** Occurs when the local or remote video size or rotation has changed.
  *
  * @param engine AgoraRtcEngineKit object.
  * @param sourceType The video source type.
  * @param uid The user ID. 0 indicates the local user.
  * @param size Size of the video (width and height) in pixels.
  * @param rotation The rotation information of the video.
  */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine videoSizeChangedOfSourceType:(AgoraVideoSourceType)sourceType uid:(NSUInteger)uid size:(CGSize)size rotation:(NSInteger)rotation NS_SWIFT_NAME(rtcEngine(_:videoSizeChangedOf:uid:size:rotation:));
- (void)rtcEngine:(AgoraRtcEngineKit* _Nonnull)engine contentInspectResult:(AgoraContentInspectResult)result NS_SWIFT_NAME(rtcEngine(_:contentInspectResult:));
- (void)rtcEngine:(AgoraRtcEngineKit* _Nonnull)engine snapshotTaken:(NSUInteger)uid filePath:(NSString* _Nonnull)filePath width:(NSInteger)width height:(NSInteger)height errCode:(NSInteger)errCode NS_SWIFT_NAME(rtcEngine(_:snapshotTaken:filePath:width:height:errCode:));
- (void)rtcEngine:(AgoraRtcEngineKit* _Nonnull)engine audioMetadataReceived:(NSUInteger)uid metadata:(NSData * _Nonnull)metadata NS_SWIFT_NAME(rtcEngine(_:audioMetadataReceived:metadata:));
/** Occurs when the local video stream state changes.
 *
 * This callback indicates the state of the local video stream, including camera capturing and video encoding,
 * and allows you to troubleshoot issues when exceptions occur.
 *
 * @note For some device models, the SDK will not trigger this callback when the state of the local video changes
 * while the local video capturing device is in use, so you have to make your own timeout judgment.
 * @param engine AgoraRtcEngineKit object
 * @param state State type #AgoraVideoLocalState. When the state is AgoraVideoLocalStateFailed (3), see the `error` parameter for details.
 * @param reason The detailed error information: #AgoraLocalVideoStreamReason.
 * @param sourceType source type of the orignated video source. See \ref AgoraVideoSourceType.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine localVideoStateChangedOfState:(AgoraVideoLocalState)state reason:(AgoraLocalVideoStreamReason)reason sourceType:(AgoraVideoSourceType)sourceType NS_SWIFT_NAME(rtcEngine(_:localVideoStateChangedOf:reason:sourceType:));

/** Occurs when the remote video state has changed.
 *
 * @note This callback does not work properly when the number of users (in the `AgoraChannelProfileCommunication` profile) or hosts
 * (in the `AgoraChannelProfileLiveBroadcasting` profile) in the channel exceeds 17.
 *
 * @param engine AgoraRtcEngineKit object.
 * @param uid    ID of the user whose video state has changed.
 * @param state  The remote video state: #AgoraVideoRemoteState.
 * @param reason The reason of the remote video state change: #AgoraVideoRemoteReason.
 * @param elapsed The time elapsed (ms) from the local user calling `joinChannel` until this callback is triggered.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine remoteVideoStateChangedOfUid:(NSUInteger)uid state:(AgoraVideoRemoteState)state reason:(AgoraVideoRemoteReason)reason elapsed:(NSInteger)elapsed NS_SWIFT_NAME(rtcEngine(_:remoteVideoStateChangedOfUid:state:reason:elapsed:));

/**
 * Occurs when the state of a remote audio stream changes.
 *
 * @param engine The AgoraRtcEngineKit object.
 * @param uid    The ID of the user whose audio state has changed.
 * @param state  The state of the remote audio. See \ref AgoraAudioRemoteState.
 * @param reason  The reason of the remote audio state change. See \ref AgoraAudioRemoteReason.
 * @param elapsed The time elapsed (ms) from calling `joinChannelByToken` until the SDK triggers this callback.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine remoteAudioStateChangedOfUid:(NSUInteger)uid state:(AgoraAudioRemoteState)state reason:(AgoraAudioRemoteReason)reason elapsed:(NSInteger)elapsed NS_SWIFT_NAME(rtcEngine(_:remoteAudioStateChangedOfUid:state:reason:elapsed:));

/** Occurs when the state of the media stream relay changes.

 The SDK reports the state of the current media relay and possible error messages in this callback.

 @param engine AgoraRtcEngineKit object.
 @param state The state code in [AgoraChannelMediaRelayState](AgoraChannelMediaRelayState).
 @param error The error code in [AgoraChannelMediaRelayError](AgoraChannelMediaRelayError).
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine
channelMediaRelayStateDidChange:(AgoraChannelMediaRelayState)state
            error:(AgoraChannelMediaRelayError)error NS_SWIFT_NAME(rtcEngine(_:channelMediaRelayStateDidChange:error:));

/** A remote user's video paused or resumed. Same as [userMuteVideoBlock]([AgoraRtcEngineKit userMuteVideoBlock:]).

 @note   Invalid when the number of users in a channel exceeds 20.

 @param engine The AgoraRtcEngineKit object.
 @param muted  Paused or resumed:

 * Yes: Remote user's video paused.
 * No: Remote user's video resumed.

 @param uid    Remote user ID.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didVideoMuted:(BOOL)muted byUid:(NSUInteger)uid NS_SWIFT_NAME(rtcEngine(_:didVideoMuted:byUid:));

/** A remote user's video was enabled or disabled.

 Once the video function is disabled, users can only perform an audio call and cannot see any video.

 @note  Invalid when the number of users in a channel exceeds 20.

 @param engine  The AgoraRtcEngineKit object.
 @param enabled Enabled or disabled:

 * Yes: User has enabled the video function.
 * No: User has disabled the video function.

 @param uid     Remote user ID.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didVideoEnabled:(BOOL)enabled byUid:(NSUInteger)uid NS_SWIFT_NAME(rtcEngine(_:didVideoEnabled:byUid:));

/** A remote user's local video was enabled or disabled.
 @deprecated
 @param engine  The AgoraRtcEngineKit object.
 @param enabled Enabled or disabled:

 * Yes: User has enabled the local video function.
 * No: User has disabled the local video function.

 @param uid     Remote user ID.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didLocalVideoEnabled:(BOOL)enabled byUid:(NSUInteger)uid NS_SWIFT_NAME(rtcEngine(_:didLocalVideoEnabled:byUid:)) __deprecated_msg("use rtcEngine:remoteVideoStateChangedOfUid:state:reason: instead.");

/** Occurs when join success after calling [setLocalAccessPoint]([AgoraRtcEngineKit setLocalAccessPoint:channelId:info:uid:joinSuccess:]) or [setCloudProxy]([AgoraRtcEngineKit setCloudProxy:proxyType])
 @param engine  AgoraRtcEngineKit object.
 @param channel Channel name.
 @param uid     User ID. If the `uid` is specified in the [joinChannelByToken]([AgoraRtcEngineKit joinChannelByToken:channelId:info:uid:joinSuccess:]) method, the  specified user ID is returned. If the user ID is not specified when the joinChannel method is called, the server automatically assigns a `uid`.
 @param proxyType type of proxy agora sdk connected, proxyType will be AgoraNoneProxyType if not connected to proxy(fallback).
 @param localProxyIp local proxy ip list, if not join local proxy, it will be ""
 @param elapsed Time elapsed (ms) from the user calling the [joinChannelByToken]([AgoraRtcEngineKit joinChannelByToken:channelId:info:uid:joinSuccess:]) method until the SDK triggers this callback.
  */
- (void)rtcEngine:(AgoraRtcEngineKit* _Nonnull)engine didProxyConnected:(NSString* _Nonnull)channel withUid:(NSUInteger)uid proxyType:(AgoraProxyType)proxyType localProxyIp:(NSString* _Nonnull)localProxyIp elapsed:(NSInteger)elapsed NS_SWIFT_NAME(rtcEngine(_:didProxyConnected:withUid:proxyType:localProxyIp:elapsed:));

/**
 *  Occurs when the remote user state is updated.
 *
 *  @param engine  The AgoraRtcEngineKit object.
 *  @param uid     Remote user ID.
 *  @param state   The remote user state.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine remoteUserStateChangedOfUid:(NSUInteger)uid state:(NSUInteger)state NS_SWIFT_NAME(rtcEngine(_:remoteUserStateChangedOfUid:state:));

/**
 * Reports the statistics of the video stream from each remote user/host.
 *
 * The SDK triggers this callback once every two seconds for each remote user
 * or host. If a channel includes multiple remote users, the SDK triggers this
 * callback as many times.
 *
 * @param engine The AgoraRtcEngineKit object.
 * @param stats  The statistics of the received remote video streams. See
 * #AgoraRtcRemoteVideoStats.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine remoteVideoStats:(AgoraRtcRemoteVideoStats * _Nonnull)stats NS_SWIFT_NAME(rtcEngine(_:remoteVideoStats:));


 /** The remote published media stream fell back to an audio-only stream in poor network conditions or switched back to the video when the network conditions improved.

 Once you enabled [setRemoteSubscribeFallbackOption]([AgoraRtcEngineKit setRemoteSubscribeFallbackOption:]),
 this event will be triggered to receive audio only due to poor network conditions or resubscribes the video when the network condition improves.

 @note  Once the remote subscribe stream is switched to the low stream due to poor network conditions, you can monitor the stream switch between a high and low stream in the [remoteVideoStats]([AgoraRtcEngineDelegate rtcEngine:remoteVideoStats:]) callback.

 @param engine              The AgoraRtcEngineKit object.
 @param isFallbackOrRecover * YES: The remote subscribe stream fell back to audio-only due to poor network conditions.
 * NO: The remote subscribe stream switched back to the video stream when the network conditions improved.
 @param uid                 Remote user ID
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didRemoteSubscribeFallbackToAudioOnly:(BOOL)isFallbackOrRecover byUid:(NSUInteger)uid NS_SWIFT_NAME(rtcEngine(_:didRemoteSubscribeFallbackToAudioOnly:byUid:));

/** The remote video transport statistics.
 This callback is triggered every two seconds after the user receives the video data packet sent from a remote user.
 @deprecated Use [rtcEngine:remoteVideoStats:] instead.

 @param engine     The AgoraRtcEngineKit object.
 @param uid        User ID of the remote user whose video packet has been received.
 @param delay      Time delay (ms) from the remote user to the local client.
 @param lost       Packet loss rate (%).
 @param rxKBitRate Received video bitrate (kbit/s) of the packet from the remote user.
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine videoTransportStatsOfUid:(NSUInteger)uid delay:(NSUInteger)delay lost:(NSUInteger)lost rxKBitRate:(NSUInteger)rxKBitRate NS_SWIFT_NAME(rtcEngine(_:videoTransportStatsOfUid:delay:lost:rxKBitRate:)) __deprecated_msg("use rtcEngine:remoteVideoStats: instead");

/**
 * Occurs when the video subscribe state changed.
 *
 * @param engine     The AgoraRtcEngineKit object.
 * @param channelId The channel name of user joined.
 * @param uid The remote user ID that is subscribed to.
 * @param oldState The old state of the video stream subscribe : #AgoraStreamSubscribeState.
 * @param newState The new state of the video stream subscribe : #AgoraStreamSubscribeState.
 * @param elapseSinceLastState The time elapsed (ms) from the old state to the new state.
 */
- (void)rtcEngine:(AgoraRtcEngineKit *_Nonnull)engine didVideoSubscribeStateChange:(NSString * _Nonnull)channelId
                uid:(unsigned int)uid
           oldState:(AgoraStreamSubscribeState)oldState
           newState:(AgoraStreamSubscribeState)newState
elapseSinceLastState:(int)elapseSinceLastState NS_SWIFT_NAME(rtcEngine(_:didVideoSubscribeStateChange:uid:oldState:newState:elapseSinceLastState:));

/**
 * Occurs when the video publish state changed.
 *
 * @param engine     The AgoraRtcEngineKit object.
 * @param channelId The channel name of user joined.
 * @param sourceType source type of the orignated video source. See \ref AgoraVideoSourceType.
 * @param oldState The old state of the video stream publish : #AgoraStreamPublishState.
 * @param newState The new state of the video stream publish : #AgoraStreamPublishState.
 * @param elapseSinceLastState The time elapsed (ms) from the old state to the new state.
*/
- (void)rtcEngine:(AgoraRtcEngineKit *_Nonnull)engine didVideoPublishStateChange:(NSString * _Nonnull)channelId
         sourceType:(AgoraVideoSourceType)sourceType
           oldState:(AgoraStreamPublishState)oldState
           newState:(AgoraStreamPublishState)newState
elapseSinceLastState:(int)elapseSinceLastState NS_SWIFT_NAME(rtcEngine(_:didVideoPublishStateChange:sourceType:oldState:newState:elapseSinceLastState:));

#pragma mark Stream Publish Delegate Methods

/**-----------------------------------------------------------------------------
 * @name Stream Publish Delegate Methods
 * -----------------------------------------------------------------------------
 */

/**-----------------------------------------------------------------------------
 * @name CDN Live Streaming Delegate Methods
 * -----------------------------------------------------------------------------
 */

/** Occurs when the state of the RTMP or RTMPS streaming changes.

The SDK triggers this callback to report the result of the local user calling the [addPublishStreamUrl](addPublishStreamUrl:transcodingEnabled:) or [removePublishStreamUrl](removePublishStreamUrl:) method.

This callback returns the URL and its current streaming state.

This callback indicates the state of the RTMP or RTMPS streaming. When exceptions occur, you can troubleshoot issues by referring to the detailed error descriptions in the `errorCode` parameter.

@param engine AgoraRtcEngineKit object.
@param url The CDN streaming URL.
@param state The RTMP or RTMPS streaming state: AgoraRtmpStreamingState.
@param reason The detailed error information for streaming: AgoraRtmpStreamingReason.
 */
- (void)rtcEngine:(AgoraRtcEngineKit* _Nonnull)engine rtmpStreamingChangedToState:(NSString* _Nonnull)url state:(AgoraRtmpStreamingState)state reason:(AgoraRtmpStreamingReason)reason NS_SWIFT_NAME(rtcEngine(_:rtmpStreamingChangedToState:state:reason:));

/** Reports events during the RTMP or RTMPS streaming.

 @since v3.1.0

 @param engine AgoraRtcEngineKit object.
 @param url The RTMP or RTMPS streaming URL.
 @param eventCode The event code. See AgoraRtmpStreamingEvent.
 */
- (void)rtcEngine:(AgoraRtcEngineKit* _Nonnull)engine rtmpStreamingEventWithUrl:(NSString* _Nonnull)url eventCode:(AgoraRtmpStreamingEvent)eventCode NS_SWIFT_NAME(rtcEngine(_:rtmpStreamingEventWithUrl:eventCode:));

/** The publisher transcoding was updated.

 @param engine The AgoraRtcEngineKit object.
 */
- (void)rtcEngineTranscodingUpdated:(AgoraRtcEngineKit * _Nonnull)engine NS_SWIFT_NAME(rtcEngineTranscodingUpdated(_:));

/** Audio mixing state changed.
 * @param engine  The AgoraRtcEngineKit object.
 * @param state  AgoraAudioMixingStateType
 * @param reasonCode  AgoraAudioMixingReasonCode
*/
- (void)rtcEngine:(AgoraRtcEngineKit *_Nonnull)engine audioMixingStateChanged:(AgoraAudioMixingStateType)state
                                                                    reasonCode:(AgoraAudioMixingReasonCode)reasonCode NS_SWIFT_NAME(rtcEngine(_:audioMixingStateChanged:reasonCode:));

/**
 * @brief Reports current AudioMixing progress.
 *
 * The callback occurs once every one second during the playback and reports the current playback progress.
 * @param position Current AudioMixing progress (millisecond).
 */
- (void)rtcEngine:(AgoraRtcEngineKit *_Nonnull)engine audioMixingPositionChanged:(NSInteger)position NS_SWIFT_NAME(rtcEngine(_:audioMixingPositionChanged:));

#if TARGET_OS_IPHONE
#pragma mark Face Detection Delegate Methods
/**-----------------------------------------------------------------------------
 * @name Face Detection Delegate Methods
 * -----------------------------------------------------------------------------
 */

/** Reports the face detection result of the local user. (iOS only)

 **Since:** v3.0.1.

 Once you enable face detection by calling [enableFaceDetection]([AgoraRtcEngineKit enableFaceDetection:]), you can get the following information on the local user in real-time:

 - The width and height of the local video.
 - The position of the human face in the local video.
 - The distance between the human face and the device screen. This value is based on the fitting calculation of the local video size and the position of the human face.

 **Note**

 - If the SDK does not detect a face, it reduces the frequency of this callback to reduce power consumption on the local device.
 - The SDK stops triggering this callback when a human face is in close proximity to the screen.

 @param engine AgoraRtcEngineKit object.
 @param width The width (px) of the local video.
 @param height The height (px) of the local video.
 @param faces An AgoraFacePositionInfo array, which contains the information of the detected human face.

 The number of the AgoraFacePositionInfo array depends on the number of human faces detected. If the array length is 0, it means that no human face is detected.
 */
- (void)rtcEngine:(AgoraRtcEngineKit* _Nonnull)engine facePositionDidChangeWidth:(int)width previewHeight:(int)height faces:(NSArray<AgoraFacePositionInfo*>* _Nullable)faces  NS_SWIFT_NAME(rtcEngine(_:facePositionDidChangeWidth:previewHeight:faces:));
#endif

#pragma mark  Remote Video Layout Info Delegates Methods
/**
 * Occurs when local video transcoder stream has error.
 *
 * @param uid The ID of the remote user.
 * @param videoLayoutInfo The `AgoraVideoLayoutInfo` object that contains video layout info of the remote user..
 */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didTranscodedStreamLayoutInfoUpdatedWithUserId:(NSUInteger)uid videoLayoutInfo:(AgoraVideoLayoutInfo* _Nonnull)videoLayoutInfo NS_SWIFT_NAME(rtcEngine(_:didTranscodedStreamLayoutInfoUpdatedWithUserId:videoLayoutInfo:));

/**
  * @brief Report the multipath transmission statistics
  *
  * @post This callback is triggered after you set `enableMultipath` to `true` to enable multipath transmission.
  *
  * @since 4.6.0
  *
  * @param stats The multipath statistics. See the MultipathStats structure for details.
  */
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine multiPathStats:(AgoraMultipathStats * _Nonnull)stats NS_SWIFT_NAME(rtcEngine(_:multiPathStats:));

#pragma mark - Unavailable Delegate Methods
typedef NSUInteger AgoraLocalVideoStreamError;
typedef NSUInteger AgoraAudioLocalError;
typedef NSUInteger AgoraRhythmPlayerError;
typedef NSUInteger AgoraRtmpStreamingErrorCode;

#if TARGET_OS_IPHONE
- (void)rtcEngine:(AgoraRtcEngineKit* _Nonnull)engine rtmpStreamingChangedToState:(NSString* _Nonnull)url state:(AgoraRtmpStreamingState)state errCode:(AgoraRtmpStreamingErrorCode)errCode NS_SWIFT_NAME(rtcEngine(_:rtmpStreamingChangedToState:state:errCode:)) __attribute__((availability(ios,deprecated=7_0,message="Use rtcEngine:rtmpStreamingChangedToState:state:reason: instead.")));
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didRhythmPlayerStateChanged:(AgoraRhythmPlayerState)state errorCode:(AgoraRhythmPlayerError)errorCode NS_SWIFT_NAME(rtcEngine(_:didRhythmPlayerStateChanged:errorCode:)) __attribute__((availability(ios,deprecated=7_0,message="Use rtcEngine:didRhythmPlayerStateChanged:reason: instead.")));
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine localAudioStateChanged:(AgoraAudioLocalState)state error:(AgoraAudioLocalError)error NS_SWIFT_NAME(rtcEngine(_:localAudioStateChanged:error:)) __attribute__((availability(ios,deprecated=7_0,message="Use rtcEngine:localAudioStateChanged:reason: instead.")));
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine localVideoStateChangedOfState:(AgoraVideoLocalState)state error:(AgoraLocalVideoStreamError)error NS_SWIFT_NAME(rtcEngine(_:localVideoStateChangedOf:error:)) __attribute__((availability(ios,deprecated=7_0,message="Use rtcEngine:localVideoStateChangedOfState:reason:sourceType: instead.")));
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine localVideoStateChangedOfState:(AgoraVideoLocalState)state error:(AgoraLocalVideoStreamError)error sourceType:(AgoraVideoSourceType)sourceType NS_SWIFT_NAME(rtcEngine(_:localVideoStateChangedOf:error:sourceType:)) __attribute__((availability(ios,deprecated=7_0,message="Use rtcEngine:localVideoStateChangedOfState:reason:sourceType: instead.")));
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine localVideoStats:(AgoraRtcLocalVideoStats * _Nonnull)stats NS_SWIFT_NAME(rtcEngine(_:localVideoStats:)) __attribute__((availability(ios,deprecated=7_0,message="Use rtcEngine:localVideoStats:sourceType instead.")));
- (void)rtcEngine:(AgoraRtcEngineKit *_Nonnull)engine didVideoPublishStateChange:(NSString * _Nonnull)channelId
         oldState:(AgoraStreamPublishState)oldState
         newState:(AgoraStreamPublishState)newState
elapseSinceLastState:(int)elapseSinceLastState NS_SWIFT_NAME(rtcEngine(_:didVideoPublishStateChange:oldState:newState:elapseSinceLastState:)) __attribute__((availability(ios,deprecated=7_0,message="Use rtcEngine:didVideoPublishStateChange:sourceType:oldState:newState:elapseSinceLastState: instead.")));
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine firstLocalVideoFramePublishedWithElapsed:(NSInteger)elapsed NS_SWIFT_NAME(rtcEngine(_:firstLocalVideoFramePublishedWithElapsed:)) __attribute__((availability(ios,deprecated=7_0,message="Use rtcEngine:firstLocalVideoFramePublishedWithElapsed:sourceType: instead.")));
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine videoSizeChangedOfUid:(NSUInteger)uid size:(CGSize)size rotation:(NSInteger)rotation NS_SWIFT_NAME(rtcEngine(_:videoSizeChangedOfUid:size:rotation:)) __attribute__((availability(ios,deprecated=7_0,message="Use rtcEngine:videoSizeChangedOfSourceType:uid:size:rotation instead.")));
#endif

#if (!(TARGET_OS_IPHONE) && (TARGET_OS_MAC))
- (void)rtcEngine:(AgoraRtcEngineKit* _Nonnull)engine rtmpStreamingChangedToState:(NSString* _Nonnull)url state:(AgoraRtmpStreamingState)state errCode:(AgoraRtmpStreamingErrorCode)errCode NS_SWIFT_NAME(rtcEngine(_:rtmpStreamingChangedToState:state:errCode:)) __attribute__((availability(macos,deprecated=10_9,message="Use rtcEngine:rtmpStreamingChangedToState:state:reason: instead.")));
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine didRhythmPlayerStateChanged:(AgoraRhythmPlayerState)state errorCode:(AgoraRhythmPlayerError)errorCode NS_SWIFT_NAME(rtcEngine(_:didRhythmPlayerStateChanged:errorCode:)) __attribute__((availability(macos,deprecated=10_9,message="Use rtcEngine:didRhythmPlayerStateChanged:reason: instead.")));
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine localAudioStateChanged:(AgoraAudioLocalState)state error:(AgoraAudioLocalError)error NS_SWIFT_NAME(rtcEngine(_:localAudioStateChanged:error:)) __attribute__((availability(macos,deprecated=10_9,message="Use rtcEngine:localAudioStateChanged:reason: instead.")));
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine localVideoStateChangedOfState:(AgoraVideoLocalState)state error:(AgoraLocalVideoStreamError)error NS_SWIFT_NAME(rtcEngine(_:localVideoStateChangedOf:error:)) __attribute__((availability(macos,deprecated=10_9,message="Use rtcEngine:localVideoStateChangedOfState:reason:sourceType: instead.")));
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine localVideoStateChangedOfState:(AgoraVideoLocalState)state error:(AgoraLocalVideoStreamError)error sourceType:(AgoraVideoSourceType)sourceType NS_SWIFT_NAME(rtcEngine(_:localVideoStateChangedOf:error:sourceType:)) __attribute__((availability(macos,deprecated=10_9,message="Use rtcEngine:localVideoStateChangedOfState:reason:sourceType: instead.")));
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine localVideoStats:(AgoraRtcLocalVideoStats * _Nonnull)stats NS_SWIFT_NAME(rtcEngine(_:localVideoStats:)) __attribute__((availability(macos,deprecated=10_9,message="Use rtcEngine:localVideoStats:sourceType instead.")));
- (void)rtcEngine:(AgoraRtcEngineKit *_Nonnull)engine didVideoPublishStateChange:(NSString * _Nonnull)channelId
         oldState:(AgoraStreamPublishState)oldState
         newState:(AgoraStreamPublishState)newState
elapseSinceLastState:(int)elapseSinceLastState NS_SWIFT_NAME(rtcEngine(_:didVideoPublishStateChange:oldState:newState:elapseSinceLastState:)) __attribute__((availability(macos,deprecated=10_9,message="Use rtcEngine:didVideoPublishStateChange:sourceType:oldState:newState:elapseSinceLastState: instead.")));
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine firstLocalVideoFramePublishedWithElapsed:(NSInteger)elapsed NS_SWIFT_NAME(rtcEngine(_:firstLocalVideoFramePublishedWithElapsed:)) __attribute__((availability(macos,deprecated=10_9,message="Use rtcEngine:firstLocalVideoFramePublishedWithElapsed:sourceType: instead.")));
- (void)rtcEngine:(AgoraRtcEngineKit * _Nonnull)engine videoSizeChangedOfUid:(NSUInteger)uid size:(CGSize)size rotation:(NSInteger)rotation NS_SWIFT_NAME(rtcEngine(_:videoSizeChangedOfUid:size:rotation:)) __attribute__((availability(macos,deprecated=10_9,message="Use rtcEngine:videoSizeChangedOfSourceType:uid:size:rotation instead.")));
#endif
@end