//
//  AgoraRtcEngineKitEx.h
//  AgoraRtcEngineKit
//
//  Copyright (c) 2020 Agora. All rights reserved.
//  Created by LLF on 2020/3/9.
//

#import "AgoraRtcEngineKit.h"
#import "AgoraObjects.h"


NS_ASSUME_NONNULL_BEGIN
@interface AgoraRtcEngineKit(Ex)

/**
 * Joins a channel.
 *
 * You can call this method multiple times to join multiple channels.
 *
 * @param token The token for authentication.
 * - In situations not requiring high security: You can use the temporary token
 * generated at Console. For details, see [Get a temporary token](https://docs.agora.io/en/Agora%20Platform/token?platform=All%20Platforms#get-a-temporary-token).
 * - In situations requiring high security: Set it as the token generated at
 * you server. For details, see [Generate a token](https://docs.agora.io/en/Agora%20Platform/token?platform=All%20Platforms#get-a-token).
 * @param connection  \ref AgoraRtcConnection by channelId and uid combine
 * @param delegate AgoraRtcEngineDelegate protocol.(Need a new object when called)
 * @param mediaOptions AgoraRtcChannelMediaOptions Object.
 * @param joinSuccessBlock Same as \ref AgoraRtcEngineDelegate.rtcEngine:didJoinChannel:withUid:elapsed: didJoinChannel. We recommend you set this parameter as `nil` to use `didJoinChannel`.
 * - If `joinSuccessBlock` is nil, the SDK triggers the `didJoinChannel` callback.
 * - If you implement both `joinSuccessBlock` and `didJoinChannel`, `joinSuccessBlock` takes higher priority than `didJoinChannel`.
 *
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)joinChannelExByToken:(NSString* _Nullable)token
                 connection:(AgoraRtcConnection * _Nonnull)connection
                   delegate:(id<AgoraRtcEngineDelegate> _Nullable)delegate
               mediaOptions:(AgoraRtcChannelMediaOptions* _Nonnull)mediaOptions
                joinSuccess:(void(^ _Nullable)(NSString* _Nonnull channel, NSUInteger uid, NSInteger elapsed))joinSuccessBlock NS_SWIFT_NAME(joinChannelEx(byToken:connection:delegate:mediaOptions:joinSuccess:));

/** Joins the channel with a user account.
 *
 * After the user successfully joins the channel, the SDK triggers the following callbacks:
 *
 * - The local client: \ref agora::rtc::IRtcEngineEventHandler::onLocalUserRegistered "onLocalUserRegistered" and \ref agora::rtc::IRtcEngineEventHandler::onJoinChannelSuccess "onJoinChannelSuccess" .
 * The remote client: \ref agora::rtc::IRtcEngineEventHandler::onUserJoined "onUserJoined" and \ref agora::rtc::IRtcEngineEventHandler::onUserInfoUpdated "onUserInfoUpdated" , if the user joining the channel is in the `COMMUNICATION` profile, or is a host in the `LIVE_BROADCASTING` profile.
 *
 * @note To ensure smooth communication, use the same parameter type to identify the user. For example, if a user joins the channel with a user ID, then ensure all the other users use the user ID too. The same applies to the user account.
 * If a user joins the channel with the Agora Web SDK, ensure that the uid of the user is set to the same parameter type.
 *
 * @param token The token generated at your server:
 * - For low-security requirements: You can use the temporary token generated at Console. For details, see [Get a temporary toke](https://docs.agora.io/en/Voice/token?platform=All%20Platforms#get-a-temporary-token).
 * - For high-security requirements: Set it as the token generated at your server. For details, see [Get a token](https://docs.agora.io/en/Voice/token?platform=All%20Platforms#get-a-token).
 * @param channelId The channel name. The maximum length of this parameter is 64 bytes. Supported character scopes are:
 * - All lowercase English letters: a to z.
 * - All uppercase English letters: A to Z.
 * - All numeric characters: 0 to 9.
 * - The space character.
 * - Punctuation characters and other symbols, including: "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ",".
 * @param userAccount The user account. The maximum length of this parameter is 255 bytes. Ensure that you set this parameter and do not set it as null. Supported character scopes are:
 * - All lowercase English letters: a to z.
 * - All uppercase English letters: A to Z.
 * - All numeric characters: 0 to 9.
 * - The space character.
 * - Punctuation characters and other symbols, including: "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ",".
 * @param delegate AgoraRtcEngineDelegate protocol.
 * @param mediaOptions  The channel media options: \ref agora::rtc::ChannelMediaOptions::ChannelMediaOptions "ChannelMediaOptions"
 *
 * @return
 * - 0: Success.
 * - < 0: Failure.
 *  - #ERR_INVALID_ARGUMENT (-2)
 *  - #ERR_NOT_READY (-3)
 *  - #ERR_REFUSED (-5)
 */
- (int)joinChannelExByToken:(NSString* _Nullable)token
                  channelId:(NSString* _Nonnull)channelId
                userAccount:(NSString* _Nonnull)userAccount
                   delegate:(id<AgoraRtcEngineDelegate> _Nullable)delegate
               mediaOptions:(AgoraRtcChannelMediaOptions* _Nonnull)mediaOptions
                joinSuccess:(void(^ _Nullable)(NSString* _Nonnull channel, NSUInteger uid, NSInteger elapsed))joinSuccessBlock NS_SWIFT_NAME(joinChannelEx(byToken:channelId:userAccount:delegate:mediaOptions:joinSuccess:));

/**
 *  Updates the channel media options after joining the channel.
 *
 * @param mediaOptions The channel media options: ChannelMediaOptions.
 * @param connection  \ref AgoraRtcConnection by channelId and uid combine
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)updateChannelExWithMediaOptions:(AgoraRtcChannelMediaOptions* _Nonnull)mediaOptions
                            connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(updateChannelEx(with:connection:));

/**
 * Leaves the channel by connection.
 *
 * @param connection  \ref AgoraRtcConnection by channelId and uid combine
 * @param leaveChannelBlock This callback indicates that a user leaves a channel, and provides the statistics of the call in #AgoraChannelStats.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)leaveChannelEx:(AgoraRtcConnection * _Nonnull)connection
    leaveChannelBlock:(void(^ _Nullable)(AgoraChannelStats* _Nonnull stat))leaveChannelBlock NS_SWIFT_NAME(leaveChannelEx(_:leaveChannelBlock:));

/**
 * Leaves a channel with the channel ID and user account.
 *
 * @param channelId The channel name. The maximum length of this parameter is 64 bytes. Supported character scopes are:
 * - All lowercase English letters: a to z.
 * - All uppercase English letters: A to Z.
 * - All numeric characters: 0 to 9.
 * - The space character.
 * - Punctuation characters and other symbols, including: "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ",".
 * @param userAccount The user account. The maximum length of this parameter is 255 bytes. Ensure that you set this parameter and do not set it as null. Supported character scopes are:
 * - All lowercase English letters: a to z.
 * - All uppercase English letters: A to Z.
 * - All numeric characters: 0 to 9.
 * - The space character.
 * - Punctuation characters and other symbols, including: "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ",".
 * @param leaveChannelBlock This callback indicates that a user leaves a channel, and provides the statistics of the call in #AgoraChannelStats.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)leaveChannelEx:(NSString * _Nonnull)channelId
              userAccount:(NSString * _Nonnull)userAccount
    leaveChannelBlock:(void(^ _Nullable)(AgoraChannelStats* _Nonnull stat))leaveChannelBlock NS_SWIFT_NAME(leaveChannelEx(_:userAccount:leaveChannelBlock:));

/**
 * Resets the SDK delegate.
 *
 * The SDK uses the delegate to inform the app on engine runtime events. All methods defined in the
 * delegate are optional implementation methods.
 *
 * @param delegate The AgoraRtcEngineDelegate protocol.
 * @param connection The AgoraRtcConnection object.
 */
- (int)setDelegateEx:(id<AgoraRtcEngineDelegate> _Nullable)delegate connection:(AgoraRtcConnection * _Nonnull)connection;

/**
 *Stops or resumes sending the local audio stream with connection.
 *
 *@param mute Determines whether to send or stop sending the local audio stream:
 *- `YES`: Stop sending the local audio stream.
 *- `NO`: Send the local audio stream.
 *
 *@param connection \ref AgoraRtcConnection by channelId and uid combine
 *
 *@return
 *- 0: Success.
 *- < 0: Failure.
 */
- (int)muteLocalAudioStreamEx:(BOOL)mute connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(muteLocalAudioStreamEx(_:connection:));
/**
 *Stops or resumes sending the local video stream with connection.
 *
 *@param mute Determines whether to send or stop sending the local video stream:
 *- `YES`: Stop sending the local video stream.
 *- `NO`: Send the local video stream.
 *
 *@param connection \ref AgoraRtcConnection by channelId and uid combine
 *
 *@return
 *- 0: Success.
 *- < 0: Failure.
 */
- (int)muteLocalVideoStreamEx:(BOOL)mute connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(muteLocalVideoStreamEx(_:connection:));

/**
 * Leaves the channel by connection.
 *
 * @param connection  {@link AgoraRtcConnection} by channelId and uid combine
 * @param options The options for leaving the channel. See {@link LeaveChannelOptions}.
 * @param leaveChannelBlock This callback indicates that a user leaves a channel, and provides the statistics of the call in #AgoraChannelStats.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)leaveChannelEx:(AgoraRtcConnection * _Nonnull)connection
              options:(AgoraLeaveChannelOptions * _Nonnull)options
    leaveChannelBlock:(void(^ _Nullable)(AgoraChannelStats* _Nonnull stat))leaveChannelBlock;

/**
 * Leaves a channel with the channel ID and user account and sets the options for leaving.
 *
 * @param channelId The channel name. The maximum length of this parameter is 64 bytes. Supported character scopes are:
 * - All lowercase English letters: a to z.
 * - All uppercase English letters: A to Z.
 * - All numeric characters: 0 to 9.
 * - The space character.
 * - Punctuation characters and other symbols, including: "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ",".
 * @param userAccount The user account. The maximum length of this parameter is 255 bytes. Ensure that you set this parameter and do not set it as null. Supported character scopes are:
 * - All lowercase English letters: a to z.
 * - All uppercase English letters: A to Z.
 * - All numeric characters: 0 to 9.
 * - The space character.
 * - Punctuation characters and other symbols, including: "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ",".
 * @param options The options for leaving the channel. See {@link LeaveChannelOptions}.
 * @param leaveChannelBlock This callback indicates that a user leaves a channel, and provides the statistics of the call in #AgoraChannelStats.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)leaveChannelEx:(NSString * _Nonnull)channelId
              userAccount:(NSString * _Nonnull)userAccount
              options:(AgoraLeaveChannelOptions * _Nonnull)options
    leaveChannelBlock:(void(^ _Nullable)(AgoraChannelStats* _Nonnull stat))leaveChannelBlock NS_SWIFT_NAME(leaveChannelEx(_:userAccount:options:leaveChannelBlock:));

/** Mutes a specified remote user's audio stream.

 @note  When setting to YES, this method stops playing audio streams without affecting the audio stream receiving process.

 @param uid  User ID whose audio streams the user intends to mute.
 @param mute * YES: Stops playing a specified user’s audio streams.
 * NO: Resumes playing a specified user’s audio streams.
 @param connection  \ref AgoraRtcConnection by channelId and uid combine

 @return * 0: Success.
* <0: Failure.
 */
- (int)muteRemoteAudioStreamEx:(NSUInteger)uid
                          mute:(BOOL)mute
                    connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(muteRemoteAudioStreamEx(_:mute:connection:));

/**
 *Stops or resumes receiving all remote audio stream with connection.
 *
 *@param mute Whether to stop receiving remote audio streams:
 *- `YES`: Stop receiving any remote audio stream.
 *- `NO`: Resume receiving all remote audio streams.
 *
 *@param connection \ref AgoraRtcConnection by channelId and uid combine
 *
 *@return
 *- 0: Success.
 *- < 0: Failure.
 */
- (int)muteAllRemoteAudioStreamsEx:(BOOL)mute
                        connection:(AgoraRtcConnection * _Nonnull)connection  NS_SWIFT_NAME(muteAllRemoteAudioStreamsEx(_:connection:));

/**
 * Sets the video encoder configuration.
 *
 * Each configuration profile corresponds to a set of video parameters,
 * including the resolution, frame rate, and bitrate.
 *
 * The parameters specified in this method are the maximum values under ideal network conditions.
 * If the video engine cannot render the video using the specified parameters
 * due to poor network conditions, the parameters further down the list are
 * considered until a successful configuration is found.
 *
 * @param config The local video encoder configuration, see #AgoraVideoEncoderConfiguration.
 * @param connection  \ref AgoraRtcConnection by channelId and uid combine
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)setVideoEncoderConfigurationEx:(AgoraVideoEncoderConfiguration* _Nonnull)config
                           connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(setVideoEncoderConfigurationEx(_:connection:));

/** Binds the remote user to the video display window, that is, sets the view for the user of the specified uid.
*
* Usually, the application should specify the uid of the remote video in the method call before the user enters a channel. If the remote uid is unknown to the application, you can set the uid after receiving the \ref AgoraRtcEngineDelegate.rtcEngine:didJoinedOfUid:elapsed: didJoinedOfUid event.
*
* @param remote \ref AgoraRtcVideoCanvas
* @param connection  \ref AgoraRtcConnection by channelId and uid combine
* @return
* - 0: Success.
* - <0: Failure.
 */
- (int)setupRemoteVideoEx:(AgoraRtcVideoCanvas* _Nonnull)remote
               connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(setupRemoteVideoEx(_:connection:));

/** Configures the remote video display mode. The application can call this method multiple times to change the display mode.
*
* @param uid  User id of the user whose video streams are received.
* @param mode AgoraVideoRenderMode
* @param mirror AgoraVideoMirrorMode
* @param connection  \ref AgoraRtcConnection by channelId and uid combine
*
* @return
* - 0: Success.
* - <0: Failure.
*/
- (int)setRemoteRenderModeEx:(NSUInteger)uid
                        mode:(AgoraVideoRenderMode)mode
                      mirror:(AgoraVideoMirrorMode)mirror
                  connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(setRemoteRenderModeEx(_:mode:mirror:connection:));

/**
 * Stops or resumes receiving the video stream of a specified user.
 *
 * @note
 * Once you leave the channel, the settings in this method becomes invalid.
 *
 * @param uid ID of the specified remote user.
 * @param mute Determines whether to receive or stop receiving a specified video stream:
 * - `YES`: Stop receiving the specified video stream.
 * - `NO`: (Default) Receive the specified video stream.
 * @param connection  \ref AgoraRtcConnection by channelId and uid combine
 *
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)muteRemoteVideoStreamEx:(NSUInteger)uid
                          mute:(BOOL)mute
                    connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(muteRemoteVideoStreamEx(_:mute:connection:));

/**
 *Stops or resumes receiving all remote video stream with connection.
 *
 *@param mute Whether to stop receiving remote video streams:
 *- `YES`: Stop receiving any remote video stream.
 *- `NO`: Resume receiving all remote video streams.
 *
 *@param connection \ref AgoraRtcConnection by channelId and uid combine
 *
 *@return
 *- 0: Success.
 *- < 0: Failure.
 */
- (int)muteAllRemoteVideoStreamsEx:(BOOL)mute
                        connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(muteAllRemoteVideoStreamsEx(_:connection:));

 /**
   * Enables or disables the dual video stream mode.
   *
   * @deprecated v4.2.0. This method is deprecated. Use setDualStreamModeEx instead.
   *
   * If dual-stream mode is enabled, the subscriber can choose to receive the high-stream
   * (high-resolution high-bitrate video stream) or low-stream (low-resolution low-bitrate video
   * stream) video using \ref setRemoteVideoStreamType setRemoteVideoStreamType.
   *
   * @param enabled
   * - true: Enable the dual-stream mode.
   * - false: (default) Disable the dual-stream mode.
   * @param streamConfig The minor stream config
   * @param connection An output parameter which is used to control different connection instances.
   */
- (int)enableDualStreamModeEx:(BOOL)enabled
                 streamConfig:(AgoraSimulcastStreamConfig*)streamConfig
                   connection:(AgoraRtcConnection* _Nonnull)connection NS_SWIFT_NAME(enableDualStreamModeEx(_:streamConfig:connection:)) __deprecated_msg("use setDualStreamModeEx: instead.");

 /**
   * Enables or disables the dual video stream mode.
   *
   * If dual-stream mode is enabled, the subscriber can choose to receive the high-stream
   * (high-resolution high-bitrate video stream) or low-stream (low-resolution low-bitrate video
   * stream) video using \ref setRemoteVideoStreamType setRemoteVideoStreamType.
   *
   * @param mode The dual-stream mode.
   * @param streamConfig The minor stream config
   * @param connection An output parameter which is used to control different connection instances.
   */
- (int)setDualStreamModeEx:(AgoraSimulcastStreamMode)mode
              streamConfig:(AgoraSimulcastStreamConfig*)streamConfig
                connection:(AgoraRtcConnection* _Nonnull)connection NS_SWIFT_NAME(setDualStreamModeEx(_:streamConfig:connection:));

/**
 * Set the multi-layer video stream configuration.
 *
 * When users expect the same UID to send multiple streams of different resolutions, they can achieve this by calling setSimulcastConfig. 
 * 
 * If multi-layer is configed, the subscriber can choose to receive the corresponding layer
 * of video stream using {@link setRemoteVideoStreamType setRemoteVideoStreamType}.
 *
 * @details This method allows a broadcaster to simultaneously transmit multiple video streams 
 * with different resolutions. The configuration supports enabling up to four layers 
 * simultaneously: one major stream (highest resolution) and three additional simulcast 
 * streams. 
 * 
 * @param simulcastConfig
 * - The configuration for multi-layer video stream. It includes seven layers, ranging from
 *   STREAM_LAYER_1 to STREAM_LOW. A maximum of 3 layers can be enabled simultaneously.
 * @param connection The RtcConnection object.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 * @since v4.6.0
 * @scenarios This method applies to scenarios involving multiple channels.
 */
- (int)setSimulcastConfigEx:(AgoraSimulcastConfig*)simulcastConfig
                 connection:(AgoraRtcConnection* _Nonnull)connection NS_SWIFT_NAME(setDualStreamModeEx(_:connection:));

/**
 * Sets the remote video stream type.
 *
 * If the remote user has enabled the dual-stream mode, by default the SDK
 * receives the high-stream video. Call this method to switch to the low-stream
 * video.
 *
 * @note
 * This method applies to scenarios where the remote user has enabled the
 * dual-stream mode by \ref enableDualStreamMode: enableDualStreamMode
 * before joining the channel.
 *
 * @param uid ID of the remote user sending the video stream.
 * @param streamType The video stream type: #AgoraVideoStreamType.
 * @param connection  \ref AgoraRtcConnection by channelId and uid combine
 *
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)setRemoteVideoStreamEx:(NSUInteger)uid
                         type:(AgoraVideoStreamType)streamType
                   connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(setRemoteVideoStreamEx(_:type:connection:));
/**
   * Sets the remote video subscription options
   *
   *
   * @param uid ID of the remote user sending the video stream.
   * @param options Sets the video subscription options.
   * @param connection  \ref AgoraRtcConnection by channelId and uid combine
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
- (int)setRemoteVideoSubscriptionOptionsEx:(NSUInteger)uid
                                   options:(AgoraVideoSubscriptionOptions* _Nonnull)options
                                connection:(AgoraRtcConnection* _Nonnull)connection NS_SWIFT_NAME(setRemoteVideoSubscriptionOptionsEx(_:options:connection:));

/** Gets the user information by passing in the user account.

 * After a remote user joins the channel, the SDK gets the user ID and user account of the remote user, caches them in a mapping table object (`AgoraUserInfo`), and triggers the [didUpdatedUserInfo]([AgoraRtcEngineDelegate rtcEngine:didUpdatedUserInfo:withUid:]) callback on the local client.

 * After receiving the [didUpdatedUserInfo]([AgoraRtcEngineDelegate rtcEngine:didUpdatedUserInfo:withUid:]) callback, you can call this method to get the user ID of the remote user from the `userInfo` object by passing in the user account.

 * @param userAccount The user account of the user. Ensure that you set this parameter.
 * @param connection  \ref AgoraRtcConnection by channelId and uid combine
 * @return An [AgoraUserInfo](AgoraUserInfo) object that contains the user account and user ID of the user.
 */
- (AgoraUserInfo* _Nullable)getUserInfoByUserAccountEx:(NSString* _Nonnull)userAccount
                                            connection:(AgoraRtcConnection * _Nonnull)connection
                                             withError:(AgoraErrorCode* _Nullable)error NS_SWIFT_NAME(getUserInfo(byUserAccountEx:connection:withError:));

/** Gets the user information by passing in the user ID.
 *
 * After a remote user joins the channel, the SDK gets the user ID and user account of the remote user,
 * caches them in a mapping table object (`userInfo`), and triggers the \ref agora::rtc::IRtcEngineEventHandler::onUserInfoUpdated "onUserInfoUpdated" callback on the local client.
 *
 * After receiving the \ref agora::rtc::IRtcEngineEventHandler::onUserInfoUpdated "onUserInfoUpdated" callback, you can call this method to get the user account of the remote user
 * from the `userInfo` object by passing in the user ID.
 *
 * @param uid The user ID of the remote user. Ensure that you set this parameter.
 * @param connection  \ref AgoraRtcConnection by channelId and uid combine
 * @return An [AgoraUserInfo](AgoraUserInfo) object that contains the user account and user ID of the user.
 */
- (AgoraUserInfo* _Nullable)getUserInfoByUidEx:(NSUInteger)uid
                                    connection:(AgoraRtcConnection * _Nonnull)connection
                                     withError:(AgoraErrorCode* _Nullable)error NS_SWIFT_NAME(getUserInfo(byUidEx:connection:withError:));

/**
 * Gets the connection state of the SDK.
 *
 * @param connection  \ref AgoraRtcConnection by channelId and uid combine
 *
 * @return The connection state. See \ref AgoraConnectionState.
 */
- (AgoraConnectionState)getConnectionStateEx:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(getConnectionStateEx(_:));

#if (!(TARGET_OS_IPHONE) && (TARGET_OS_MAC))
/** Enables loopback sampling. (macOS only)
  * If you enable loopback sampling, the output of the sound card is mixed into the audio stream sent to the other end.
  * You can call this method either before or after joining a channel.
  
  * *Note:**
  * macOS does not support loopback sampling of the default sound card. If you need to use this method,
  * please use a virtual sound card and pass its name to the `deviceName` parameter. Agora has tested and recommends using soundflower.

  * @param enabled Sets whether to enable/disable loopback sampling.
  * YES: Enable loopback sampling.
  * NO: (Default) Disable loopback sampling.

  * @param deviceName Pointer to the device name of the sound card. The default value is nil (default sound card).
  * If you use a virtual sound card like "Soundflower", set this parameter as the name of the sound card, "Soundflower",
  * and the SDK will find the corresponding sound card and start capturing.
  * @param connection  \ref AgoraRtcConnection by channelId and uid combine
  * @return
  * 0: Success.
  * < 0: Failure.
  */
- (int)enableLoopbackRecordingEx:(BOOL)enabled
                      deviceName:(NSString* _Nullable)deviceName
                      connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(enableLoopbackRecordingEx(_:deviceName:connection:));
#endif

/**
 * Adjusts the recording volume.
 *
 * @param volume The recording volume, which ranges from 0 to 400:
 * - 0  : Mute the recording volume.
 * - 100: The original volume.
 * - 400: (Maximum) Four times the original volume with signal clipping protection.
 *
 * @param connection  \ref AgoraRtcConnection by channelId and uid combine
 *
 * @return
 * - 0  : Success.
 * - < 0: Failure.
 */
- (int)adjustRecordingSignalVolumeEx:(NSInteger)volume
                          connection:(AgoraRtcConnection* _Nonnull)connection;

/**
 * Mutes or resume recording signal volume.
 *
 * @param mute Determines whether to mute or resume the recording signal volume.
 * - YES: Mute the recording signal volume.
 * -  NO: (Default) Resume the recording signal volume.
 *
 * @param connection  \ref AgoraRtcConnection by channelId and uid combine
 *
 * @return
 * - 0  : Success.
 * - < 0: Failure.
 */
- (int)muteRecordingSignalEx:(BOOL)mute
                  connection:(AgoraRtcConnection* _Nonnull)connection;

/** Adjust the playback signal volume of a specified remote user.

 You can call this method as many times as necessary to adjust the playback volume of different remote users, or to repeatedly adjust the playback volume of the same remote user.

 **Note**

 - The playback volume here refers to the mixed volume of a specified remote user.
 - This method can only adjust the playback volume of one specified remote user at a time. To adjust the playback volume of different remote users, call the method as many times, once for each remote user.

 @param uid The ID of the remote user.
 @param volume The playback volume of the specified remote user. The value
 ranges between 0 and 400, including the following:

 - 0: Mute.
 - 100: (Default) Original volume.
 @param connection  \ref AgoraRtcConnection by channelId and uid combine

 @return
 - 0: Success.
 - < 0: Failure.
 */

- (int)adjustUserPlaybackSignalVolumeEx:(NSUInteger)uid
                              volume:(NSInteger)volume
                          connection:(AgoraRtcConnection* _Nonnull)connection;

- (int)sendCustomReportMessageEx:(NSString * _Nullable)messageId
                        category:(NSString * _Nullable)category
                           event:(NSString * _Nullable)event
                           label:(NSString * _Nullable)label
                           value:(NSInteger)value
                      connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(sendCustomReportMessageEx(_:category:event:label:value:connection:));

- (int)enableAudioVolumeIndicationEx:(NSInteger)interval
                              smooth:(NSInteger)smooth
                           reportVad:(BOOL)reportVad
                          connection:(AgoraRtcConnection* _Nonnull)connection NS_SWIFT_NAME(enableAudioVolumeIndicationEx(_:smooth:reportVad:connection:));

/** Sets the sound position and gain of a remote user.

 When the local user calls this method to set the sound position of a remote user, the sound difference between the left and right channels allows the local user to track the real-time position of the remote user, creating a real sense of space. This method applies to massively multiplayer online games, such as Battle Royale games.

**Note:**

- Ensure that you call this method after joining a channel. For this method to work, enable stereo panning for remote users by calling [enableSoundPositionIndication]([AgoraRtcEngineKit enableSoundPositionIndication:]) before joining a channel.
This method requires hardware support.
- For the best effect, we recommend using the following audio output devices:
  - (iOS) A stereo headset.
  - (macOS) A stereo loudspeaker.
 @param uid The ID of the remote user.
 @param pan The sound position of the remote user. The value ranges from -1.0 to 1.0:

 * 0.0: (default) the remote sound comes from the front.
 * -1.0: the remote sound comes from the left.
 * 1.0: the remote sound comes from the right.

 @param gain Gain of the remote user. The value ranges from 0.0 to 100.0. The default value is 100.0 (the original gain of the remote user). The smaller the value, the less the gain.
 @param connection  \ref AgoraRtcConnection by channelId and uid combine

 @return * 0: Success.
* < 0: Failure.
 */
- (int)setRemoteVoicePositionEx:(NSUInteger)uid
                          pan:(double)pan
                         gain:(double)gain
                   connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(setRemoteVoicePositionEx(_:pan:gain:connection:));

/** Sets spatial audio parameters of a remote user.

 When the local user calls this method to set the spatial audio parameters  of a remote user, the sound difference between the left and right channels allows the local user to track the real-time position of the remote user, creating a real sense of spatial.

**Note:**

- For the best effect, we recommend using the following audio output devices:
  - (iOS) A stereo headset.
  - (macOS) A stereo loudspeaker.
 @param uid The ID of the remote user.
 @param params The spatial audio parameters of the remote user. 
 @param connection  \ref AgoraRtcConnection by channelId and uid combine
 
 @return * 0: Success.
* < 0: Failure.
 */
- (int)setRemoteUserSpatialAudioParamsEx:(NSUInteger)uid
                                  params:(AgoraSpatialAudioParams* _Nonnull)params
                              connection:(AgoraRtcConnection* _Nonnull)connection NS_SWIFT_NAME(setRemoteUserSpatialAudioParamsEx(_:params:connection:));

/** Adds a watermark image to the local video.

 This method adds a PNG watermark image to the local video in the interactive live streaming. Once the watermark image is added, all the audience in the channel (CDN audience included), and the capturing device can see and capture it. Agora supports adding only one watermark image onto the local video, and the newly watermark image replaces the previous one.

 The watermark position depends on the settings in the [setVideoEncoderConfiguration]([AgoraRtcEngineKit setVideoEncoderConfiguration:]) method:

 - If the orientation mode of the encoding video is AgoraVideoOutputOrientationModeFixedLandscape, or the landscape mode in AgoraVideoOutputOrientationModeAdaptative, the watermark uses the landscape orientation.
 - If the orientation mode of the encoding video is AgoraVideoOutputOrientationModeFixedPortrait, or the portrait mode in AgoraVideoOutputOrientationModeAdaptative, the watermark uses the portrait orientation.
 - When setting the watermark position, the region must be less than the dimensions set in the [setVideoEncoderConfiguration]([AgoraRtcEngineKit setVideoEncoderConfiguration:]) method. Otherwise, the watermark image will be cropped.

 **Note**

 - Ensure that you have called the [enableVideo]([AgoraRtcEngineKit enableVideo]) method to enable the video module before calling this method.
 - If you only want to add a watermark image to the local video for the audience in the CDN live streaming channel to see and capture, you can call this method or the [setLiveTranscoding]([AgoraRtcEngineKit setLiveTranscoding:]) method.
 - This method supports adding a watermark image in the PNG file format only. Supported pixel formats of the PNG image are RGBA, RGB, Palette, Gray, and Alpha_gray.
 - If the dimensions of the PNG image differ from your settings in this method, the image will be cropped or zoomed to conform to your settings.
 - If you have enabled the local video preview by calling the [startPreview]([AgoraRtcEngineKit startPreview]) method, you can use the `visibleInPreview` member in the WatermarkOptions class to set whether or not the watermark is visible in preview.
 - If you have enabled the mirror mode for the local video, the watermark on the local video is also mirrored. To avoid mirroring the watermark, Agora recommends that you do not use the mirror and watermark functions for the local video at the same time. You can implement the watermark function in your application layer.

 @param url The local file path of the watermark image to be added. This method supports adding a watermark image from the local file path. If the watermark image to be added is in the project file, you need to change the image's Type from PNG image to Data in the Xcode property, otherwise, the Agora Native SDK cannot recognize the image.
 @param options The options of the watermark image to be added. See WatermarkOptions.
 @param connection  \ref AgoraRtcConnection by channelId and uid combine

 @return * 0: Success.
 * < 0: Failure.
 */
- (int)addVideoWatermarkEx:(NSURL* _Nonnull)url options:(WatermarkOptions* _Nonnull)options connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(addVideoWatermarkEx(_:options:connection:)) __deprecated_msg("use addVideoWatermark:config connection instead.");

/**
 * @brief Adds a watermark image to the local video with a watermark configuration.
 *
 * @since 4.6.0
 *
 * @param config The configuration of the watermark image to be added.
 * @param connection The connection information, including the channel ID and user ID.
 *
 * @return * 0: Success.
 *         * < 0: Failure.
 */
- (int)addVideoWatermarkEx:(WatermarkConfig* _Nonnull)configs connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(addVideoWatermarkEx(_:connection:));

/**
 * @brief Removes a specific watermark image from the video stream.
 *
 * @since 4.6.0
 *
 * @param id The identifier of the watermark to remove.
 * @param connection The connection information, including the channel ID and user ID.
 *
 * @return * 0: Success.
 *         * < 0: Failure.
 */
- (int)removeVideoWatermarkEx:(NSString* _Nonnull)id connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(removeVideoWatermarkEx(_:connection:));


/** Clears the watermark image on the video stream.
 @param connection  \ref AgoraRtcConnection by channelId and uid combine

 @return * 0: Success.
 * < 0: Failure.
 */
- (int)clearVideoWatermarkEx:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(clearVideoWatermarkEx(_:));

/**-----------------------------------------------------------------------------
 * @name Data Steam
 * -----------------------------------------------------------------------------
 */
/** Creates a data stream.
*
* Each user can create up to five data streams during the lifecycle of the `AgoraRtcEngineKit`.
*
* @note Set both the `reliable` and `ordered` parameters to `YES` or `NO`. Do not set one as `YES` and the other as `NO`.
*
* @param streamId ID of the created data stream.
* @param reliable Sets whether or not the recipients are guaranteed to receive the data stream from the sender within five seconds:
* - YES: The recipients receive the data stream from the sender within five seconds. If the recipient does not receive the data stream within five seconds, an error is reported to the app.
* - NO: There is no guarantee that the recipients receive the data stream within five seconds and no error message is reported for any delay or missing data stream.
*
* @param ordered  Sets whether or not the recipients receive the data stream in the sent order:
* - YES: The recipients receive the data stream in the sent order.
* - NO: The recipients do not receive the data stream in the sent order.
* @param connection  \ref AgoraRtcConnection by channelId and uid combine.
*
* @return
* - 0: Success.
* - < 0: Failure.
*/
- (int)createDataStreamEx:(NSInteger * _Nonnull)streamId
                 reliable:(BOOL)reliable
                  ordered:(BOOL)ordered
               connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(createDataStreamEx(_:reliable:ordered:connection:));
/** Creates a data stream.

 Each user can create up to five data streams during the lifecycle of the [AgoraRtcChannel](AgoraRtcChannel).

 @param streamId ID of the created data stream.
 @param config the config of data stream.
 @param connection  \ref AgoraRtcConnection by channelId and uid combine.
 @return * 0: Success.
* < 0: Failure.
 */
- (int)createDataStreamEx:(NSInteger * _Nonnull)streamId
                   config:(AgoraDataStreamConfig * _Nonnull)config
               connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(createDataStreamEx(_:config:connection:));

/** Sends data stream messages to all users in a channel.

The SDK has the following restrictions on this method:

- Up to 60 packets can be sent per second in a channel with each packet having a maximum size of 1 KB.
- Each client can send up to 30 KB of data per second.
- Each user can have up to five data streams simultaneously.

If the remote user receives the data stream within five seconds, the SDK triggers the \ref AgoraRtcEngineDelegate.rtcEngine:receiveStreamMessageFromUid:streamId:data: receiveStreamMessageFromUid callback on the remote client, from which the remote user gets the stream message.

If the remote user does not receive the data stream within five seconds, the SDK triggers the \ref AgoraRtcEngineDelegate.rtcEngine:didOccurStreamMessageErrorFromUid:streamId:error:missed:cached: didOccurStreamMessageErrorFromUid callback on the remote client.

 @note
 - This method applies only to the Communication profile or to the hosts in the live interactive streaming profile. If an audience in the live interactive streaming profile calls this method, the audience role may be changed to a host.
 - Ensure that you have created the data stream using \ref createDataStream:reliable:ordered: createDataStream before calling this method.

 @param streamId ID of the sent data stream returned in the `createDataStream` method.
 @param data   Sent data.
 @param connection  \ref AgoraRtcConnection by channelId and uid combine.

 @return
 - 0: Success.
 - < 0: Failure.
*/
- (int)sendStreamMessageEx:(NSInteger)streamId
                      data:(NSData * _Nonnull)data
                connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(sendStreamMessageEx(_:data:connection:));

/**-----------------------------------------------------------------------------
 * @name Common Data Transmission
 * -----------------------------------------------------------------------------
 */

/**
 * @brief Send Reliable message to remote uid in channel.
 *
 * @technical preview
 *
 * @param uid remote user id.
 * @param type Reliable Data Transmission tunnel message type. See AgoraRdtStreamType.
 * @param data The sent data.
 * @param connection AgoraRtcConnection by channelId and uid combine.

 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)sendRdtMessageEx:(NSInteger)uid
                   type:(AgoraRdtStreamType)type
                   data:(NSData * _Nonnull)data
             connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(sendRdtMessageEx(_:type:data:connection:));

/**
 * @brief Send media control message
 *
 * @technical preview
 *
 * @param uid Remote user id. In particular, if the uid is set to 0, it means broadcasting the message to the entire channel.
 * @param data The sent data.
 * @param connection AgoraRtcConnection by channelId and uid combine.

 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)sendMediaControlMessageEx:(NSInteger)uid
                   data:(NSData * _Nonnull)data
             connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(sendMediaControlMessageEx(_:data:connection:));

/**-----------------------------------------------------------------------------
 * @name Stream Fallback
 * -----------------------------------------------------------------------------
 */

 /**
 * Sets the high priority user list and related remote subscribe fallback option.
 *
 * @param uidList The uid list of high priority users.
 * @param option The remote subscribe fallback option of high priority users.
 * @param connection  \ref AgoraRtcConnection by channelId and uid combine.
 *
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */

- (int)setHighPriorityUserListEx:(NSArray <NSNumber *> *_Nullable)uidList
                          option:(AgoraStreamFallbackOptions)option
                      connection:(AgoraRtcConnection* _Nonnull)connection;

/**-----------------------------------------------------------------------------
 * @name Stream Publish
 * -----------------------------------------------------------------------------
 */

 /**
 * Sets the blocklist of subscribe remote stream audio.
 *
 * @param blocklist The uid list of users who do not subscribe to audio.
 * @param connection  \ref AgoraRtcConnection by channelId and uid combine.
 * @note
 * If uid is in blocklist, the remote user's audio will not be subscribed,
 * even if muteRemoteAudioStream(uid, false) and muteAllRemoteAudioStreams(false) are operated.
 *
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)setSubscribeAudioBlocklistEx:(NSArray <NSNumber *> *_Nonnull)blocklist connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(setSubscribeAudioBlocklistEx(_:connection:));

/**
 * Sets the allowlist of subscribe remote stream audio.
 *
 * @param allowlist The uid list of users who do subscribe to audio.
 * @param connection  \ref AgoraRtcConnection by channelId and uid combine.
 * @note
 * If uid is in allowlist, the remote user's audio will be subscribed,
 * even if muteRemoteAudioStream(uid, true) and muteAllRemoteAudioStreams(true) are operated.
 *
 * If a user is in the blocklist and allowlist at the same time, the user will not subscribe to audio.
 *
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)setSubscribeAudioAllowlistEx:(NSArray <NSNumber *> *_Nonnull)allowlist connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(setSubscribeAudioAllowlistEx(_:connection:));

/**
 * Sets the blocklist of subscribe remote stream video.
 *
 * @param blocklist The uid list of users who do not subscribe to video.
 * @param connection  \ref AgoraRtcConnection by channelId and uid combine.
 * @note
 * If uid is in blocklist, the remote user's video will not be subscribed,
 * even if muteRemoteVideoStream(uid, false) and muteAllRemoteVideoStreams(false) are operated.
 *
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)setSubscribeVideoBlocklistEx:(NSArray <NSNumber *> *_Nonnull)blocklist connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(setSubscribeVideoBlocklistEx(_:connection:));

/**
 * Sets the allowlist of subscribe remote stream video.
 *
 * @param allowlist The uid list of users who do subscribe to video.
 * @param connection  \ref AgoraRtcConnection by channelId and uid combine.
 * @note
 * If uid is in whitelist, the remote user's video will be subscribed,
 * even if muteRemoteVideoStream(uid, true) and muteAllRemoteVideoStreams(true) are operated.
 *
 * If a user is in the blocklist and allowlist at the same time, the user will not subscribe to video.
 *
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)setSubscribeVideoAllowlistEx:(NSArray <NSNumber *> *_Nonnull)allowlist connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(setSubscribeVideoAllowlistEx(_:connection:));

/**
  * Takes a snapshot of a video stream.
  *
  * This method takes a snapshot of a video stream from the specified user, generates a JPG
  * image, and saves it to the specified path.
  *
  * The method is asynchronous, and the SDK has not taken the snapshot when the method call
  * returns. After a successful method call, the SDK triggers the `snapshotTaken` callback
  * to report whether the snapshot is successfully taken, as well as the details for that
  * snapshot.
  *
  * @note
  * - Call this method after joining a channel.
  * - This method takes a snapshot of the published video stream specified in `ChannelMediaOptions`.
  * - If the user's video has been preprocessed, for example, watermarked or beautified, the resulting
  * snapshot includes the pre-processing effect.
  * @param connection The RtcConnection object.
  * @param uid The user ID. Set uid as 0 if you want to take a snapshot of the local user's video.
  * @param filePath The local path (including filename extensions) of the snapshot. For example:
  * - Windows: `C:\Users\<user_name>\AppData\Local\Agora\<process_name>\example.jpg`
  * - iOS: `/App Sandbox/Library/Caches/example.jpg`
  * - macOS: `～/Library/Logs/example.jpg`
  * - Android: `/storage/emulated/0/Android/data/<package name>/files/example.jpg`
  *
  * Ensure that the path you specify exists and is writable.
  * @return
  * - 0 : Success.
  * - < 0 : Failure.
  */
- (NSInteger)takeSnapshotEx:(AgoraRtcConnection * _Nonnull)connection uid:(NSInteger)uid filePath:(NSString* _Nonnull)filePath NS_SWIFT_NAME(takeSnapshotEx(_:uid:filePath:));

/**
  * Takes a snapshot of a video stream.
  *
  * This method takes a snapshot of a video stream from the specified user, generates a JPG
  * image, and saves it to the specified path.
  *
  * The method is asynchronous, and the SDK has not taken the snapshot when the method call
  * returns. After a successful method call, the SDK triggers the `snapshotTaken` callback
  * to report whether the snapshot is successfully taken, as well as the details for that
  * snapshot.
  *
  * @note
  * - Call this method after joining a channel.
  * - This method takes a snapshot of the published video stream specified in `ChannelMediaOptions`.
  *
  * @param connection The RtcConnection object.
  * @param uid The user ID. Set uid as 0 if you want to take a snapshot of the local user's video.
  * @param config The configuration for the take snapshot. See SnapshotConfig.
  *
  * Ensure that the path you specify exists and is writable.
  * @return
  * - 0 : Success.
  * - &lt; 0: Failure.
  *   - -4: Incorrect observation position. Modify the input observation position according to the reqiurements specified in SnapshotConfig.
  */
- (NSInteger)takeSnapshotExWithConfig:(AgoraRtcConnection * _Nonnull)connection uid:(NSInteger)uid config:(AgoraSnapshotConfig* _Nonnull)config NS_SWIFT_NAME(takeSnapshotExWithConfig(_:uid:config:));

/**
   * send audio metadata
   * @since v4.3.1
   * @param metadata The NSData of metadata
   * @param connection AgoraRtcConnection.
   * @return
   * - 0: success
   * - <0: failure
   * @technical preview
*/
- (int)sendAudioMetadataEx:(AgoraRtcConnection * _Nonnull)connection metadata:(NSData * _Nonnull)metadata NS_SWIFT_NAME(sendAudioMetadataEx(_:metadata:));

/** 
 *  Enables video screenshot and upload with the connection ID.
 * @param enabled Whether to video screenshot and upload:
 * - `true`: Yes.
 * - `false`: No.
 * @param config The configuration for video screenshot and upload.
 * @param connection The connection information. See AgoraRtcConnection.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)enableContentInspectEx:(BOOL)enabled config:(AgoraContentInspectConfig* _Nonnull)config connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(enableContentInspectEx(_:config:connection:));

/** Publishes the local stream without transcoding to a specified CDN live RTMP address.  (CDN live only.)
  *
  * @param url The CDN streaming URL in the RTMP format. The maximum length of this parameter is 1024 bytes.
  * @param connection AgoraRtcConnection.
  * @return
  * - 0: Success.
  * - < 0: Failure.
  */
- (int)startRtmpStreamWithoutTranscodingEx:(NSString* _Nonnull)url
                                connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(startRtmpStreamWithoutTranscodingEx(_:connection:));

/** Publishes the local stream with transcoding to a specified CDN live RTMP address.  (CDN live only.)
  *
  * @param url The CDN streaming URL in the RTMP format. The maximum length of this parameter is 1024 bytes.
  * @param transcoding Sets the CDN live audio/video transcoding settings.  See LiveTranscoding.
  * @param connection AgoraRtcConnection.
  *
  * @return
  * - 0: Success.
  * - < 0: Failure.
  */
- (int)startRtmpStreamWithTranscodingEx:(NSString* _Nonnull)url
                            transcoding:(AgoraLiveTranscoding* _Nullable)transcoding
                             connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(startRtmpStreamWithTranscodingEx(_:transcoding:connection:));

/** Update the video layout and audio settings for CDN live. (CDN live only.)
  * @note This method applies to Live Broadcast only.
  *
  * @param transcoding Sets the CDN live audio/video transcoding settings. See LiveTranscoding.
  * @param connection AgoraRtcConnection.
  *
  * @return
  * - 0: Success.
  * - < 0: Failure.
  */
- (int)updateRtmpTranscodingEx:(AgoraLiveTranscoding* _Nullable)transcoding
                    connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(updateRtmpTranscodingEx(_:connection:));

/** Stop an RTMP stream with transcoding or without transcoding from the CDN. (CDN live only.)
  * @param url The RTMP URL address to be removed. The maximum length of this parameter is 1024 bytes.
  * @param connection AgoraRtcConnection.
  * @return
  * - 0: Success.
  * - < 0: Failure.
  */
- (int)stopRtmpStreamEx:(NSString* _Nonnull)url
             connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(stopRtmpStreamEx(_:connection:));

/** Starts or update to relay media streams across channels.
 *
 * @since v4.2.0
 * @param config The configuration of the media stream relay:AgoraChannelMediaRelayConfiguration.
 * @param connection AgoraRtcConnection.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 *   - -1(ERR_FAILED): A general error occurs (no specified reason).
 *   - -2(ERR_INVALID_ARGUMENT): The argument is invalid.
 *   - -5(ERR_REFUSED): The request is rejected.
 *   - -8(ERR_INVALID_STATE): The current status is invalid, only allowed to be called when the role is the broadcaster.
 */
- (int)startOrUpdateChannelMediaRelayEx:(AgoraChannelMediaRelayConfiguration * _Nonnull)config connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(startOrUpdateChannelMediaRelayEx(_:connection:));

/** Stops the media stream relay.
 *
 * Once the relay stops, the host quits all the destination
 * channels.
 *
 * @param connection AgoraRtcConnection.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 *   - -1(ERR_FAILED): A general error occurs (no specified reason).
 *   - -2(ERR_INVALID_ARGUMENT): The argument is invalid.
 *   - -5(ERR_REFUSED): The request is rejected.
 *   - -7(ERR_NOT_INITIALIZED): cross channel media streams are not relayed.
 */
- (int)stopChannelMediaRelayEx:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(stopChannelMediaRelayEx(_:));

/** pause the channels for media stream relay.
 *
 * @param connection AgoraRtcConnection.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 *   - -1(ERR_FAILED): A general error occurs (no specified reason).
 *   - -2(ERR_INVALID_ARGUMENT): The argument is invalid.
 *   - -5(ERR_REFUSED): The request is rejected.
 *   - -7(ERR_NOT_INITIALIZED): cross channel media streams are not relayed.
 */
- (int)pauseAllChannelMediaRelayEx:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(pauseAllChannelMediaRelayEx(_:));

/** resume the channels for media stream relay.
 *
 * @param connection AgoraRtcConnection.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 *   - -1(ERR_FAILED): A general error occurs (no specified reason).
 *   - -2(ERR_INVALID_ARGUMENT): The argument is invalid.
 *   - -5(ERR_REFUSED): The request is rejected.
 *   - -7(ERR_NOT_INITIALIZED): cross channel media streams are not relayed.
 */
- (int)resumeAllChannelMediaRelayEx:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(resumeAllChannelMediaRelayEx(_:));

/**
 @brief Start tracing media rendering events.
 @since v4.1.1
 @discussion
 - SDK will trace media rendering events when this API is called.
 - The tracing result can be obtained through callback `[AgoraRtcEngineDelegate rtcEngine:videoRenderingTracingResultOfUid:currentEvent:tracingInfo:]`
 @param connection AgoraRtcConnection.
 @note
 - By default, SDK will trace media rendering events when join channel.
 - The start point of event tracing will be reset after leaving channel.
 @return
 - 0: Success.
 - < 0: Failure.
  - `-2(AgoraErrorCodeInvalidArgument)`: The parameter is invalid. Check the channel ID and local uid set by parameter `connection`.
  - `-7(AgoraErrorCodeNotInitialized)`: The SDK is not initialized.
 */
- (int)startMediaRenderingTracingEx:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(startMediaRenderingTracingEx(_:));

/** Provides the technical preview functionalities or special customizations by configuring the SDK with JSON options.

 @note  The JSON options are not public by default. Agora is working on making commonly used JSON options public in a standard way. Contact support@agora.io for more information.

 @param options SDK options in JSON format.
 @param connection AgoraRtcConnection.
 */
- (int)setParametersEx:(NSString * _Nonnull)options connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(setParametersEx(_:connection:));

/**
 * Gets the current call ID.
 *
 * When a user joins a channel, a call ID is generated to identify the call.
 *
 * After a call ends, you can call `rate` or `complain` to gather feedback from
 * your customer.
 *
 * @param connection The AgoraRtcConnection object.
 * @return The call ID if the method call is successful.
 */
- (NSString * _Nullable)getCallIdEx:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(getCallIdEx(_:));

/** Preloads a specified audio effect to a specified channel.
 * @since v4.6.0
 *
 * This method preloads only one specified audio effect into the memory each time
 * it is called. To preload multiple audio effects, call this method multiple times.
 *
 * After preloading, you can call \ref playEffect:filePath:loopCount:pitch:pan:gain:publish: playEffect
 * to play the preloaded audio effect or call
 * \ref playAllEffectsWithLoopCount:pitch:pan:gain:publish: playAllEffects to play all the preloaded
 * audio effects.
 *
 * @note
 * - This method applies to scenarios involving multiple channels.
 * - To ensure smooth communication, limit the size of the audio effect file.
 * - Agora recommends calling this method before joining the channel.
 *
 * @param connection The AgoraRtcConnection object.
 * @param soundId The ID of the audio effect.
 * @param filePath The absolute path of the local audio effect file or the URL
 * of the online audio effect file. Supported audio formats: mp3, mp4, m4a, aac,
 * 3gp, mkv and wav.

 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)preloadEffectEx:(AgoraRtcConnection* _Nonnull)connection
             soundId:(int)soundId
            filePath:(NSString* _Nonnull)filePath NS_SWIFT_NAME(preloadEffectEx(_:soundId:filePath:));

/** Preloads a specified audio effect to a specified channel.
 * @since v4.6.0
 *
 * This method preloads only one specified audio effect into the memory each time
 * it is called. To preload multiple audio effects, call this method multiple times.
 *
 * After preloading, you can call \ref playEffect:filePath:loopCount:pitch:pan:gain:publish: playEffect
 * to play the preloaded audio effect or call
 * \ref playAllEffectsWithLoopCount:pitch:pan:gain:publish: playAllEffects to play all the preloaded
 * audio effects.
 *
 * @note
 * - This method applies to scenarios involving multiple channels.
 * - To ensure smooth communication, limit the size of the audio effect file.
 * - Agora recommends calling this method before joining the channel.
 *
 * @param connection The AgoraRtcConnection object.
 * @param soundId The ID of the audio effect.
 * @param filePath The absolute path of the local audio effect file or the URL
 * of the online audio effect file. Supported audio formats: mp3, mp4, m4a, aac,
 * 3gp, mkv and wav.
 * @param startPos The playback position (ms) of the audio effect file.

 * @return
 * - 0: Success.
 * - < 0: Failure.
 */

- (int)preloadEffectEx:(AgoraRtcConnection* _Nonnull)connection
             soundId:(int)soundId
            filePath:(NSString* _Nonnull)filePath
            startPos:(int)startPos NS_SWIFT_NAME(preloadEffectEx(_:soundId:filePath:startPos:));

/** Plays a specified audio effect to a specified channel.
* @since v4.6.0
*
* @param connection The AgoraRtcConnection object.
* @param soundId ID of the audio effect. Each audio effect has a unique ID.
*
* @note This method applies to scenarios involving multiple channels. If you preloaded the audio effect into the memory through the [preloadEffect]([AgoraRtcEngineKit preloadEffect:filePath:]) method, ensure that the `soundID` value is set to the same value as in preloadEffect.
* @param filePath Absolute path of the audio effect file.
* @param loopCount Sets the number of times looping the audio effect:
* - 0: Plays the audio effect once.
* - 1: Plays the audio effect twice.
* - -1: Plays the audio effect in an indefinite loop until you call the [stopEffect]([AgoraRtcEngineKit stopEffect:]) or [stopAllEffects]([AgoraRtcEngineKit stopAllEffects]) method
*
* @param pitch Sets whether to change the pitch of the audio effect. The value ranges between 0.5 and 2.
* The default value is 1 (no change to the pitch). The lower the value, the lower the pitch.
* @param pan Sets the spatial position of the audio effect. The value ranges between -1.0 and 1.0.
* - 0.0: The audio effect displays ahead.
* - 1.0: The audio effect displays to the right.
* - -1.0: The audio effect displays to the left.
*
* @param gain Sets the volume of the sound effect. The value ranges between 0.0 and 100.0 (default). The lower the value, the lower the volume of the sound effect.
* @return
* - 0: Success.
* - < 0: Failure.
 */
- (int)playEffectEx:(AgoraRtcConnection* _Nonnull)connection
            soundId:(int)soundId
           filePath:(NSString* _Nonnull)filePath
          loopCount:(NSInteger)loopCount
              pitch:(double)pitch
                pan:(double)pan
               gain:(NSInteger)gain NS_SWIFT_NAME(playEffectEx(_:soundId:filePath:loopCount:pitch:pan:gain:));

/** Plays a specified audio effect to a specified channel.
* @since v4.6.0
*
* @see [playEffectEx](playEffectEx:soundId:filePath:loopCount:pitch:pan:gain:publish:)
* @param connection The AgoraRtcConnection object.
* @param soundId ID of the audio effect. Each audio effect has a unique ID.
*
* @note This method applies to scenarios involving multiple channels. If you preloaded the audio effect into the memory through the [preloadEffect]([AgoraRtcEngineKit preloadEffect:filePath:]) method, ensure that the `soundID` value is set to the same value as in preloadEffect.
* @param filePath Absolute path of the audio effect file.
* @param loopCount Sets the number of times looping the audio effect:
* - 0: Plays the audio effect once.
* - 1: Plays the audio effect twice.
* - -1: Plays the audio effect in an indefinite loop until you call the [stopEffect]([AgoraRtcEngineKit stopEffect:]) or [stopAllEffects]([AgoraRtcEngineKit stopAllEffects]) method
*
* @param pitch Sets whether to change the pitch of the audio effect. The value ranges between 0.5 and 2.
* The default value is 1 (no change to the pitch). The lower the value, the lower the pitch.
* @param pan Sets the spatial position of the audio effect. The value ranges between -1.0 and 1.0.
* - 0.0: The audio effect displays ahead.
* - 1.0: The audio effect displays to the right.
* - -1.0: The audio effect displays to the left.
*
* @param gain Sets the volume of the sound effect. The value ranges between 0.0 and 100.0 (default). The lower the value, the lower the volume of the sound effect.
* @param startPos The playback position (ms) of the audio effect file.
* @return
* - 0: Success.
* - < 0: Failure.
 */
- (int)playEffectEx:(AgoraRtcConnection* _Nonnull)connection
            soundId:(int)soundId
           filePath:(NSString* _Nonnull)filePath
          loopCount:(NSInteger)loopCount
              pitch:(double)pitch
                pan:(double)pan
               gain:(NSInteger)gain
           startPos:(int)startPos NS_SWIFT_NAME(playEffectEx(_:soundId:filePath:loopCount:pitch:pan:gain:startPos:));


/** Plays a specified audio effect to a specified channel.
* @since v4.6.0
*
* @param connection The AgoraRtcConnection object.
* @param soundId ID of the audio effect. Each audio effect has a unique ID.
*
* @note This method applies to scenarios involving multiple channels. If you preloaded the audio effect into the memory through the [preloadEffect]([AgoraRtcEngineKit preloadEffect:filePath:]) method, ensure that the `soundID` value is set to the same value as in preloadEffect.
* @param filePath Absolute path of the audio effect file.
* @param loopCount Sets the number of times looping the audio effect:
* - 0: Plays the audio effect once.
* - 1: Plays the audio effect twice.
* - -1: Plays the audio effect in an indefinite loop until you call the [stopEffect]([AgoraRtcEngineKit stopEffect:]) or [stopAllEffects]([AgoraRtcEngineKit stopAllEffects]) method
*
* @param pitch Sets whether to change the pitch of the audio effect. The value ranges between 0.5 and 2.
* The default value is 1 (no change to the pitch). The lower the value, the lower the pitch.
* @param pan Sets the spatial position of the audio effect. The value ranges between -1.0 and 1.0.
* - 0.0: The audio effect displays ahead.
* - 1.0: The audio effect displays to the right.
* - -1.0: The audio effect displays to the left.
*
* @param gain Sets the volume of the sound effect. The value ranges between 0.0 and 100.0 (default). The lower the value, the lower the volume of the sound effect.
* @param publish Sets whether or not to publish the specified audio effect to the remote stream:
* - YES: Publish the audio effect in the channel so that remote user can hear it.
* - NO: Do not publish the audio effect in the channel.
* @return
* - 0: Success.
* - < 0: Failure.
 */
- (int)playEffectEx:(AgoraRtcConnection* _Nonnull)connection
            soundId:(int)soundId
           filePath:(NSString* _Nonnull)filePath
          loopCount:(NSInteger)loopCount
              pitch:(double)pitch
                pan:(double)pan
               gain:(NSInteger)gain
           publish:(BOOL)publish NS_SWIFT_NAME(playEffectEx(_:soundId:filePath:loopCount:pitch:pan:gain:publish:));

/** Plays a specified audio effect to a specified channel.
* @since v4.6.0
*
* The difference between this method and playEffectEx is that this method allows you to set whether to publish an audio effect in a channel.
*
* To play multiple audio effect files simultaneously, call this method multiple times with different soundIds and filePaths. We recommend playing no more than three audio effect files at the same time.
*
* When the audio effect file playback is finished, the SDK triggers the \ref AgoraRtcEngineDelegate.rtcEngineDidAudioEffectFinish:soundId: rtcEngineDidAudioEffectFinish callback.
* @param connection The AgoraRtcConnection object.
* @param soundId ID of the specified audio effect. Each audio effect has a unique ID.
* If the audio effect is preloaded into the memory through the \ref preloadEffect:filePath: preloadEffect method, ensure that the `soundId` value is set to the same value as in \ref preloadEffect:filePath: preloadEffect.
* @param filePath Specifies the absolute path (including the suffixes of the filename) to the local audio effect file or the URL of the online audio effect file, for example, `/var/mobile/Containers/Data/audio.mp4`. Supported audio formats: mp3, mp4, m4a, aac, 3gp, mkv and wav.
* @param loopCount Sets the number of times the audio effect loops:
* - 0: Plays the audio effect once.
* - 1: Plays the audio effect twice.
* - -1: Plays the audio effect in an indefinite loop until you call the \ref stopEffect: stopEffect or \ref stopAllEffects stopAllEffects method.
*
* @param pitch Sets the pitch of the audio effect. The value ranges between 0.5 and 2. The default value is 1 (no change to the pitch). The lower the value, the lower the pitch.
* @param pan Sets the spatial position of the audio effect. The value ranges between -1.0 and 1.0.
* - 0.0: The audio effect displays ahead.
* - 1.0: The audio effect displays to the right.
* - -1.0: The audio effect displays to the left.
*
* @param gain Sets the volume of the audio effect. The value ranges between 0.0 and 100.0 (default). The lower the value, the lower the volume of the audio effect.
* @param publish Sets whether to publish the specified audio effect in a channel:
* - YES: Publish the audio effect in the channel so that remote user can hear it.
* - NO: Do not publish the audio effect in the channel.
* @param startPos The playback position (ms) of the audio effect file.
*
* @return
* - 0: Success.
* - < 0: Failure.
*/
- (int)playEffectEx:(AgoraRtcConnection* _Nonnull)connection
          soundId:(int)soundId
         filePath:(NSString* _Nonnull)filePath
        loopCount:(NSInteger)loopCount
            pitch:(double)pitch
              pan:(double)pan
             gain:(NSInteger)gain
          publish:(BOOL)publish
         startPos:(int)startPos NS_SWIFT_NAME(playEffectEx(_:soundId:filePath:loopCount:pitch:pan:gain:publish:startPos:));

/** 
 * Adds multiple SDK delegate.
 * 
 * @param delegate The AgoraRtcEngineDelegate object. 
 * @param connection The AgoraRtcConnection object.
 */
- (void)addDelegateEx:(id<AgoraRtcEngineDelegate> _Nonnull)delegate connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(addDelegateEx(_:connection:));

/** 
 * Removes multiple SDK delegate.
 * 
 * @param delegate The AgoraRtcEngineDelegate object. 
 * @param connection The AgoraRtcConnection object.
 */
- (void)removeDelegateEx:(id<AgoraRtcEngineDelegate> _Nonnull)delegate connection:(AgoraRtcConnection * _Nonnull)connection NS_SWIFT_NAME(removeDelegateEx(_:connection:));

/**-----------------------------------------------------------------------------
 * @name Built-in Encryption
 * -----------------------------------------------------------------------------
 */

/** Enables/Disables the built-in encryption.

 In scenarios requiring high security, Agora recommends calling enableEncryption to enable the built-in encryption before joining a channel.

 All users in the same channel must use the same encryption mode and encryption key. Once all users leave the channel, the encryption key of this channel is automatically cleared.

 **Note**

 - If you enable the built-in encryption, you cannot use the RTMP streaming function.

 @param enabled Whether to enable the built-in encryption:

 - YES: Enable the built-in encryption.
 - NO: Disable the built-in encryption.

 @param config Configurations of built-in encryption schemas. See AgoraEncryptionConfig.
 @param connection  \ref AgoraRtcConnection by channelId and uid combine
 
 @return - 0: Success.
 - < 0: Failure.

  - -2 (`AgoraErrorCodeInvalidArgument`): An invalid parameter is used. Set the parameter with a valid value.
  - -7 (`AgoraErrorCodeNotInitialized`): The SDK is not initialized. Initialize the `AgoraRtcEngineKit` instance before calling this method.
  - -4 (`AgoraErrorCodeNotSupported`): The encryption mode is incorrect or the SDK fails to load the external encryption library. Check the enumeration or reload the external encryption library.
 */
- (int)enableEncryptionEx:(BOOL)enabled encryptionConfig:(AgoraEncryptionConfig *_Nonnull)config connection:(AgoraRtcConnection* _Nonnull)connection NS_SWIFT_NAME(enableEncryptionEx(_:encryptionConfig:connection:));
@end

NS_ASSUME_NONNULL_END
