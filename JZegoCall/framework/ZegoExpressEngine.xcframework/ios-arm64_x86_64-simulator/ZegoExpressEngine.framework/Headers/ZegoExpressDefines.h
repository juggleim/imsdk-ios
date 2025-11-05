//
//  ZegoExpressDefines.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressErrorCode.h"
#import <AVFoundation/AVFoundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>
#import <ReplayKit/ReplayKit.h>

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
typedef UIView ZGView;
typedef UIImage ZGImage;
#elif TARGET_OS_OSX
#import <AppKit/AppKit.h>
typedef NSView ZGView;
typedef NSImage ZGImage;
#endif

NS_ASSUME_NONNULL_BEGIN

@class ZegoAccurateSeekConfig;
@class ZegoNetWorkResourceCache;
@class ZegoNetworkProbeResult;
@class ZegoSceneConfig;
@class ZegoSceneStreamConfig;
@class ZegoTeamParam;
@class ZegoTestNetworkConnectivityResult;

@protocol ZegoAIVoiceChangerDataHandler;
@protocol ZegoAIVoiceChangerEventHandler;
@protocol ZegoAudioEffectPlayerEventHandler;
@protocol ZegoCopyrightedMusicEventHandler;
@protocol ZegoMediaDataPublisherEventHandler;
@protocol ZegoMediaPlayerAudioHandler;
@protocol ZegoMediaPlayerBlockDataHandler;
@protocol ZegoMediaPlayerEventHandler;
@protocol ZegoMediaPlayerVideoHandler;
@protocol ZegoRangeAudioEventHandler;
@protocol ZegoRangeSceneEventHandler;
@protocol ZegoRangeSceneItemEventHandler;
@protocol ZegoRangeSceneStreamEventHandler;
@protocol ZegoRangeSceneTeamEventHandler;
@protocol ZegoRealTimeSequentialDataEventHandler;
@protocol ZegoReplayKitExtHandler;
@protocol ZegoScreenCaptureSourceEventHandler;

/// Callback for asynchronous destruction completion.
///
/// In general, developers do not need to listen to this callback.
typedef void (^ZegoDestroyCompletionCallback)(void);

/// Callback for setting room extra information.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
typedef void (^ZegoRoomSetRoomExtraInfoCallback)(int errorCode);

/// Login room result callback.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param extendedData Extended Information
typedef void (^ZegoRoomLoginCallback)(int errorCode, NSDictionary *extendedData);

/// Logout room result callback.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param extendedData Extended Information
typedef void (^ZegoRoomLogoutCallback)(int errorCode, NSDictionary *extendedData);

/// Log upload result callback.
///
/// Description: After calling [uploadLog] to upload the log, get the upload result through this callback.
/// Use cases: When uploading logs, in order to determine whether the logs are uploaded successfully, you can get them through this callback.
/// Caution: In the case of poor network, the return time of this callback may be longer.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
typedef void (^ZegoUploadLogResultCallback)(int errorCode);

/// Callback for setting stream extra information.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
typedef void (^ZegoPublisherSetStreamExtraInfoCallback)(int errorCode);

/// Callback for add/remove CDN URL.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
typedef void (^ZegoPublisherUpdateCdnUrlCallback)(int errorCode);

/// Results of take publish stream snapshot.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param image Snapshot image
typedef void (^ZegoPublisherTakeSnapshotCallback)(int errorCode, ZGImage *_Nullable image);

/// Results of take play stream snapshot.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param image Snapshot image
typedef void (^ZegoPlayerTakeSnapshotCallback)(int errorCode, ZGImage *_Nullable image);

/// Results of starting a mixer task.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param extendedData Extended Information
typedef void (^ZegoMixerStartCallback)(int errorCode, NSDictionary *_Nullable extendedData);

/// Results of stoping a mixer task.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
typedef void (^ZegoMixerStopCallback)(int errorCode);

/// Callback for sending real-time sequential data.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
typedef void (^ZegoRealTimeSequentialDataSentCallback)(int errorCode);

/// Callback for sending broadcast messages.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param messageID ID of this message
typedef void (^ZegoIMSendBroadcastMessageCallback)(int errorCode, unsigned long long messageID);

/// Callback for sending barrage message.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param messageID ID of this message
typedef void (^ZegoIMSendBarrageMessageCallback)(int errorCode, NSString *messageID);

/// Callback for sending custom command.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
typedef void (^ZegoIMSendCustomCommandCallback)(int errorCode);

/// Callback for sending custom command.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
typedef void (^ZegoRoomSendTransparentMessageCallback)(int errorCode);

/// Callback for test network connectivity.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param result Network connectivity test results
typedef void (^ZegoTestNetworkConnectivityCallback)(int errorCode,
                                                    ZegoTestNetworkConnectivityResult *result);

/// Callback for network probe.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param result Network probe result
typedef void (^ZegoNetworkProbeResultCallback)(int errorCode, ZegoNetworkProbeResult *result);

/// Callback for media player loads resources.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
typedef void (^ZegoMediaPlayerLoadResourceCallback)(int errorCode);

/// Callback for media player seek to playback progress.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
typedef void (^ZegoMediaPlayerSeekToCallback)(int errorCode);

/// The callback of the screenshot of the media player playing screen
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param image Snapshot image
typedef void (^ZegoMediaPlayerTakeSnapshotCallback)(int errorCode, ZGImage *_Nullable image);

/// Callback for audio effect player loads resources.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
typedef void (^ZegoAudioEffectPlayerLoadResourceCallback)(int errorCode);

/// Callback for audio effect player seek to playback progress.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
typedef void (^ZegoAudioEffectPlayerSeekToCallback)(int errorCode);

/// Callback for copyrighted music init.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
typedef void (^ZegoCopyrightedMusicInitCallback)(int errorCode);

/// Callback of sending extended feature request.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param command request command.
/// @param result request result, each request command has corresponding request result.
typedef void (^ZegoCopyrightedMusicSendExtendedRequestCallback)(int errorCode, NSString *command,
                                                                NSString *result);

/// Get lrc format lyrics complete callback.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param lyrics lrc format lyrics.
typedef void (^ZegoCopyrightedMusicGetLrcLyricCallback)(int errorCode, NSString *lyrics);

/// Get verbatim lyrics complete callback.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param lyrics verbatim lyrics.
typedef void (^ZegoCopyrightedMusicGetKrcLyricByTokenCallback)(int errorCode, NSString *lyrics);

/// Callback for request song.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource The JSON string returned by the song ordering service, including song resource information.
typedef void (^ZegoCopyrightedMusicRequestSongCallback)(int errorCode, NSString *resource);

/// Callback for request accompaniment.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource accompany resource information.
typedef void (^ZegoCopyrightedMusicRequestAccompanimentCallback)(int errorCode, NSString *resource);

/// Callback for request accompaniment clip.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource accompany clip resource information.
typedef void (^ZegoCopyrightedMusicRequestAccompanimentClipCallback)(int errorCode,
                                                                     NSString *resource);

/// Callback for acquire songs or accompaniment through authorization token.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource song or accompany resource information.
typedef void (^ZegoCopyrightedMusicGetMusicByTokenCallback)(int errorCode, NSString *resource);

/// Callback of requesting music resource.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource The JSON string returned by the song ordering service, including music resource information.
typedef void (^ZegoCopyrightedMusicRequestResourceCallback)(int errorCode, NSString *resource);

/// Callback of getting shared music resource.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource The JSON string returned by the song ordering service, including music resource information.
typedef void (^ZegoCopyrightedMusicGetSharedResourceCallback)(int errorCode, NSString *resource);

/// Callback for download song or accompaniment.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
typedef void (^ZegoCopyrightedMusicDownloadCallback)(int errorCode);

/// Get standard pitch data complete callback.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param pitch Standard pitch data.
typedef void (^ZegoCopyrightedMusicGetStandardPitchCallback)(int errorCode, NSString *pitch);

/// Login scene result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param config State channel count.
typedef void (^ZegoRangeSceneLoginSceneCallback)(int errorCode, ZegoSceneConfig *config);

/// Logout scene result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
typedef void (^ZegoRangeSceneLogoutSceneCallback)(int errorCode);

/// The callback of getting user count in scene. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param count User count in scene.
typedef void (^ZegoRangeSceneGetUserCountCallback)(int errorCode, unsigned int count);

/// The callback of getting user list in view. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param userList User list in view.
typedef void (^ZegoRangeSceneGetUserListInViewCallback)(int errorCode,
                                                        NSArray<NSString *> *userList);

/// The callback of send custom command. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
typedef void (^ZegoRangeSceneSendCustomCommandCallback)(int errorCode);

/// Join team result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param teamID Team ID.
typedef void (^ZegoRangeSceneJoinTeamCallback)(int errorCode, unsigned int teamID);

/// Leave team result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param teamID Team ID.
typedef void (^ZegoRangeSceneLeaveTeamCallback)(int errorCode, unsigned int teamID);

/// Create item result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param itemID Item ID.
typedef void (^ZegoRangeSceneCreateItemCallback)(int errorCode, long long itemID);

/// Destroy item result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param itemID Item ID.
typedef void (^ZegoRangeSceneDestroyItemCallback)(int errorCode, long long itemID);

/// Bind item result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param itemID Item ID.
typedef void (^ZegoRangeSceneBindItemCallback)(int errorCode, long long itemID);

/// Unbind item result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param itemID Item ID.
typedef void (^ZegoRangeSceneUnbindItemCallback)(int errorCode, long long itemID);

/// Update item status result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param itemID Item ID.
typedef void (^ZegoRangeSceneUpdateItemStatusCallback)(int errorCode, long long itemID);

/// Update item command result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param itemID Item ID.
typedef void (^ZegoRangeSceneUpdateItemCommandCallback)(int errorCode, long long itemID);

/// Room scenario.
typedef NS_ENUM(NSUInteger, ZegoScenario) {
    /// @deprecated Legacy general scenario
    ZegoScenarioGeneral DEPRECATED_ATTRIBUTE = 0,
    /// @deprecated Legacy communication scenario
    ZegoScenarioCommunication DEPRECATED_ATTRIBUTE = 1,
    /// @deprecated Legacy live broadcast scenario
    ZegoScenarioLive DEPRECATED_ATTRIBUTE = 2,
    /// Available since: 3.0.0. Description: The default (generic) scenario. If none of the following scenarios conform to your actual application scenario, this default scenario can be used.
    ZegoScenarioDefault = 3,
    /// Available since: 3.0.0. Description: Standard video call scenario, it is suitable for one-to-one video call scenarios.
    ZegoScenarioStandardVideoCall = 4,
    /// Available since: 3.0.0. Description: High quality video call scenario, it is similar to the standard video call scenario, but this scenario uses a higher video frame rate, bit rate, and resolution (540p) by default, which is suitable for video call scenario with high image quality requirements.
    ZegoScenarioHighQualityVideoCall = 5,
    /// Available since: 3.0.0. Description: Standard chatroom scenario, suitable for multi-person pure voice calls (low data usage). Note: On the ExpressVideo SDK, the camera is not enabled by default in this scenario.
    ZegoScenarioStandardChatroom = 6,
    /// Available since: 3.0.0. Description: High quality chatroom scenario, it is similar to the standard chatroom scenario, but this scenario uses a higher audio bit rate than the standard chatroom scenario by default. It is suitable for multi-person pure voice call scenarios with high requirements on sound quality. Note: On the ExpressVideo SDK, the camera is not enabled by default in this scenario.
    ZegoScenarioHighQualityChatroom = 7,
    /// Available since: 3.0.0. Description: Live broadcast scenario, it is suitable for one-to-many live broadcast scenarios such as shows, games, e-commerce, and large educational classes. The audio and video quality, fluency, and compatibility have been optimized. Note: Even in live broadcast scenarios, the SDK has no business "roles" (such as anchors and viewers), and all users in the room can publish and play streams.
    ZegoScenarioBroadcast = 8,
    /// Available since: 3.0.0. Description: Karaoke (KTV) scenario, it is suitable for real-time chorus and online karaoke scenarios, and has optimized delay, sound quality, ear return, echo cancellation, etc., and also ensures accurate alignment and ultra-low delay when multiple people chorus.
    ZegoScenarioKaraoke = 9,
    /// Available since: 3.3.0. Description: Standard voice call scenario, it is suitable for one-to-one video or voice call scenarios. Note: On the ExpressVideo SDK, the camera is not enabled by default in this scenario.
    ZegoScenarioStandardVoiceCall = 10
};

/// SDK feature type.
typedef NS_ENUM(NSUInteger, ZegoFeatureType) {
    /// Basic audio feature.
    ZegoFeatureTypeAudio = 0,
    /// Basic video feature.
    ZegoFeatureTypeVideo = 1,
    /// Media player feature.
    ZegoFeatureTypeMediaPlayer = 2,
    /// Local media data recorder feature.
    ZegoFeatureTypeMediaDataRecorder = 3,
    /// Media data publisher feature.
    ZegoFeatureTypeMediaDataPublisher = 4,
    /// Supplemental Enhancement Information (media side info) feature.
    ZegoFeatureTypeSEI = 5,
    /// SDK video capture feature.
    ZegoFeatureTypeSdkVideoCapture = 6,
    /// Custom video capture feature.
    ZegoFeatureTypeCustomVideoCapture = 7,
    /// SDK video rendering feature.
    ZegoFeatureTypeSdkVideoRender = 8,
    /// Custom video rendering feature.
    ZegoFeatureTypeCustomVideoRender = 9,
    /// SDK video processing feature (including low-light enhancement feature).
    ZegoFeatureTypeSdkVideoProcessing = 10,
    /// Custom video processing feature.
    ZegoFeatureTypeCustomVideoProcessing = 11,
    /// Streaming encryption / decryption feature.
    ZegoFeatureTypeStreamEncryption = 12,
    /// RTMP streaming feature.
    ZegoFeatureTypeRtmp = 13,
    /// RTMPS streaming feature.
    ZegoFeatureTypeRtmps = 14,
    /// RTMP over QUIC streaming feature.
    ZegoFeatureTypeRtmpOverQuic = 15,
    /// RTMP streaming feature.
    ZegoFeatureTypeHttpFlv = 16,
    /// HTTPS-FLV streaming feature.
    ZegoFeatureTypeHttpsFlv = 17,
    /// HTTP-FLV over QUIC streaming feature.
    ZegoFeatureTypeHttpFlvOverQuic = 18,
    /// Super resolution imaging feature.
    ZegoFeatureTypeSuperResolution = 19,
    /// Effects beauty feature.
    ZegoFeatureTypeEffectsBeauty = 20,
    /// Whiteboard beauty feature.
    ZegoFeatureTypeWhiteboard = 21,
    /// Range audio feature.
    ZegoFeatureTypeRangeAudio = 22,
    /// Copy righted music feature.
    ZegoFeatureTypeCopyRightedMusic = 23,
    /// Video object segmentation feature.
    ZegoFeatureTypeVideoObjectSegmentation = 24,
    /// Range scene feature. (3.0.0 and above support)
    ZegoFeatureTypeRangeScene = 25,
    /// Screen capture feature. (3.1.0 and above support)
    ZegoFeatureTypeScreenCapture = 26,
    /// AI voice changer feature. (3.8.0 and above support)
    ZegoFeatureTypeAIVoiceChanger = 27
};

/// Language.
typedef NS_ENUM(NSUInteger, ZegoLanguage) {
    /// English
    ZegoLanguageEnglish = 0,
    /// Chinese
    ZegoLanguageChinese = 1
};

/// Room mode.
typedef NS_ENUM(NSUInteger, ZegoRoomMode) {
    /// Single room mode.
    ZegoRoomModeSingleRoom = 0,
    /// Multiple room mode.
    ZegoRoomModeMultiRoom = 1
};

/// Geo fence type.
typedef NS_ENUM(NSUInteger, ZegoGeoFenceType) {
    /// Not use geo fence.
    ZegoGeoFenceTypeNone = 0,
    /// Include the specified geo fence areas.
    ZegoGeoFenceTypeInclude = 1,
    /// Exclude the specified geo fence areas.
    ZegoGeoFenceTypeExclude = 2
};

/// Geo fence area code.
typedef NS_ENUM(NSUInteger, ZegoGeoFenceAreaCode) {
    /// Chinese mainland (excluding Hong Kong, Macao and Taiwan).
    ZegoGeoFenceAreaCodeCN = 2,
    /// North America.
    ZegoGeoFenceAreaCodeNA = 3,
    /// Europe, including the UK.
    ZegoGeoFenceAreaCodeEU = 4,
    /// Asia, excluding Chinese mainland and India.
    ZegoGeoFenceAreaCodeAS = 5,
    /// India.
    ZegoGeoFenceAreaCodeIN = 6
};

/// engine state.
typedef NS_ENUM(NSUInteger, ZegoEngineState) {
    /// The engine has started
    ZegoEngineStateStart = 0,
    /// The engine has stoped
    ZegoEngineStateStop = 1
};

/// Room state.
typedef NS_ENUM(NSUInteger, ZegoRoomState) {
    /// Unconnected state, enter this state before logging in and after exiting the room. If there is a steady state abnormality in the process of logging in to the room, such as AppID or Token are incorrect, or if the same user name is logged in elsewhere and the local end is KickOut, it will enter this state.
    ZegoRoomStateDisconnected = 0,
    /// The state that the connection is being requested. It will enter this state after successful execution login room function. The display of the UI is usually performed using this state. If the connection is interrupted due to poor network quality, the SDK will perform an internal retry and will return to the requesting connection status.
    ZegoRoomStateConnecting = 1,
    /// The status that is successfully connected. Entering this status indicates that the login to the room has been successful. The user can receive the callback notification of the user and the stream information in the room.
    ZegoRoomStateConnected = 2
};

/// Room state change reason.
typedef NS_ENUM(NSUInteger, ZegoRoomStateChangedReason) {
    /// Logging in to the room. When calling [loginRoom] to log in to the room or [switchRoom] to switch to the target room, it will enter this state, indicating that it is requesting to connect to the server. The application interface is usually displayed through this state.
    ZegoRoomStateChangedReasonLogining = 0,
    /// Log in to the room successfully. When the room is successfully logged in or switched, it will enter this state, indicating that the login to the room has been successful, and users can normally receive callback notifications of other users in the room and all stream information additions and deletions.
    ZegoRoomStateChangedReasonLogined = 1,
    /// Failed to log in to the room. When the login or switch room fails, it will enter this state, indicating that the login or switch room has failed, for example, AppID or Token is incorrect, etc.
    ZegoRoomStateChangedReasonLoginFailed = 2,
    /// The room connection is temporarily interrupted. If the interruption occurs due to poor network quality, the SDK will retry internally.
    ZegoRoomStateChangedReasonReconnecting = 3,
    /// The room is successfully reconnected. If there is an interruption due to poor network quality, the SDK will retry internally, and enter this state after successful reconnection.
    ZegoRoomStateChangedReasonReconnected = 4,
    /// The room fails to reconnect. If there is an interruption due to poor network quality, the SDK will retry internally, and enter this state after the reconnection fails.
    ZegoRoomStateChangedReasonReconnectFailed = 5,
    /// Kicked out of the room by the server. For example, if you log in to the room with the same user name in other places, and the local end is kicked out of the room, it will enter this state.
    ZegoRoomStateChangedReasonKickOut = 6,
    /// Logout of the room is successful. It is in this state by default before logging into the room. When calling [logoutRoom] to log out of the room successfully or [switchRoom] to log out of the current room successfully, it will enter this state.
    ZegoRoomStateChangedReasonLogout = 7,
    /// Failed to log out of the room. Enter this state when calling [logoutRoom] fails to log out of the room or [switchRoom] fails to log out of the current room internally.
    ZegoRoomStateChangedReasonLogoutFailed = 8
};

/// Room mode.
typedef NS_ENUM(NSUInteger, ZegoRoomTransparentMessageMode) {
    /// Single room mode.
    ZegoRoomTransparentMessageModeOnlyClient = 0,
    /// Multiple room mode.
    ZegoRoomTransparentMessageModeOnlyServer = 1,
    /// Multiple room mode.
    ZegoRoomTransparentMessageModeClientAndServer = 2
};

/// Room mode.
typedef NS_ENUM(NSUInteger, ZegoRoomTransparentMessageType) {
    /// Single room mode.
    ZegoRoomTransparentMessageNormal = 0,
    /// Multiple room mode.
    ZegoRoomTransparentMessageSequence = 1
};

/// Publish channel.
typedef NS_ENUM(NSUInteger, ZegoPublishChannel) {
    /// The main (default/first) publish channel.
    ZegoPublishChannelMain = 0,
    /// The auxiliary (second) publish channel
    ZegoPublishChannelAux = 1,
    /// The third publish channel
    ZegoPublishChannelThird = 2,
    /// The fourth publish channel
    ZegoPublishChannelFourth = 3
};

/// Publish CensorshipMode.
typedef NS_ENUM(NSUInteger, ZegoStreamCensorshipMode) {
    /// no censorship.
    ZegoStreamCensorshipModeNone = 0,
    /// only censorship stream audio.
    ZegoStreamCensorshipModeAudio = 1,
    /// only censorship stream video.
    ZegoStreamCensorshipModeVideo = 2,
    /// censorship stream audio and video.
    ZegoStreamCensorshipModeAudioAndVideo = 3
};

/// Type of capability negotiation for publish stream references.
typedef NS_ENUM(NSUInteger, ZegoCapabilityNegotiationType) {
    /// no reference to the outcome of the capability negotiation.
    ZegoCapabilityNegotiationTypeNone = 0,
    /// refer to the outcome of the capability negotiation of all user in the room.
    ZegoCapabilityNegotiationTypeAll = 1,
    /// refer to the outcome of the capability negotiation of publisher in the room.
    ZegoCapabilityNegotiationTypePublisher = 2
};

/// Video rendering fill mode.
typedef NS_ENUM(NSUInteger, ZegoViewMode) {
    /// The proportional scaling up, there may be black borders
    ZegoViewModeAspectFit = 0,
    /// The proportional zoom fills the entire View and may be partially cut
    ZegoViewModeAspectFill = 1,
    /// Fill the entire view, the image may be stretched
    ZegoViewModeScaleToFill = 2
};

/// Mirror mode for previewing or playing the of the stream.
typedef NS_ENUM(NSUInteger, ZegoVideoMirrorMode) {
    /// The mirror image only for previewing locally. This mode is used by default. When the mobile terminal uses a rear camera, this mode is still used by default, but it does not work. Local preview does not set mirroring.
    ZegoVideoMirrorModeOnlyPreviewMirror = 0,
    /// Both the video previewed locally and the far end playing the stream will see mirror image.
    ZegoVideoMirrorModeBothMirror = 1,
    /// Both the video previewed locally and the far end playing the stream will not see mirror image.
    ZegoVideoMirrorModeNoMirror = 2,
    /// The mirror image only for far end playing the stream.
    ZegoVideoMirrorModeOnlyPublishMirror = 3
};

/// SEI type
typedef NS_ENUM(NSUInteger, ZegoSEIType) {
    /// Using H.264 SEI (nalu type = 6, payload type = 243) type packaging, this type is not specified by the SEI standard, there is no conflict with the video encoder or the SEI in the video file, users do not need to follow the SEI content Do filtering, SDK uses this type by default.
    ZegoSEITypeZegoDefined = 0,
    /// SEI (nalu type = 6, payload type = 5) of H.264 is used for packaging. The H.264 standard has a prescribed format for this type: startcode + nalu type (6) + payload type (5) + len + payload (uuid + content) + trailing bits. Because the video encoder itself generates an SEI with a payload type of 5, or when a video file is used for streaming, such SEI may also exist in the video file, so when using this type, the user needs to use uuid + context as a buffer sending SEI. At this time, in order to distinguish the SEI generated by the video encoder itself, when the App sends this type of SEI, it can fill in the service-specific uuid (uuid length is 16 bytes). When the receiver uses the SDK to parse the SEI of the payload type 5, it will set filter string filters out the SEI matching the uuid and throws it to the business. If the filter string is not set, the SDK will throw all received SEI to the developer. uuid filter string setting function, [ZegoEngineConfig.advancedConfig("unregister_sei_filter","XXXXXX")], where unregister_sei_filter is the key, and XXXXX is the uuid filter string to be set.
    ZegoSEITypeUserUnregister = 1
};

/// Publish stream status.
typedef NS_ENUM(NSUInteger, ZegoPublisherState) {
    /// The state is not published, and it is in this state before publishing the stream. If a steady-state exception occurs in the publish process, such as AppID or Token are incorrect, or if other users are already publishing the stream, there will be a failure and enter this state.
    ZegoPublisherStateNoPublish = 0,
    /// The state that it is requesting to publish the stream after the [startPublishingStream] function is successfully called. The UI is usually displayed through this state. If the connection is interrupted due to poor network quality, the SDK will perform an internal retry and will return to the requesting state.
    ZegoPublisherStatePublishRequesting = 1,
    /// The state that the stream is being published, entering the state indicates that the stream has been successfully published, and the user can communicate normally.
    ZegoPublisherStatePublishing = 2
};

/// Voice changer preset value.
typedef NS_ENUM(NSUInteger, ZegoVoiceChangerPreset) {
    /// No Voice changer
    ZegoVoiceChangerPresetNone = 0,
    /// Male to child voice (loli voice effect)
    ZegoVoiceChangerPresetMenToChild = 1,
    /// Male to female voice (kindergarten voice effect)
    ZegoVoiceChangerPresetMenToWomen = 2,
    /// Female to child voice
    ZegoVoiceChangerPresetWomenToChild = 3,
    /// Female to male voice
    ZegoVoiceChangerPresetWomenToMen = 4,
    /// Foreigner voice effect
    ZegoVoiceChangerPresetForeigner = 5,
    /// Autobot Optimus Prime voice effect
    ZegoVoiceChangerPresetOptimusPrime = 6,
    /// Android robot voice effect
    ZegoVoiceChangerPresetAndroid = 7,
    /// Ethereal voice effect
    ZegoVoiceChangerPresetEthereal = 8,
    /// Magnetic(Male) voice effect
    ZegoVoiceChangerPresetMaleMagnetic = 9,
    /// Fresh(Female) voice effect
    ZegoVoiceChangerPresetFemaleFresh = 10,
    /// Electronic effects in C major voice effect
    ZegoVoiceChangerPresetMajorC = 11,
    /// Electronic effects in A minor voice effect
    ZegoVoiceChangerPresetMinorA = 12,
    /// Electronic effects in harmonic minor voice effect
    ZegoVoiceChangerPresetHarmonicMinor = 13,
    /// Female Vitality Sound effect
    ZegoVoiceChangerPresetFemaleEnergetic = 14,
    /// Richness effect
    ZegoVoiceChangerPresetRichNess = 15,
    /// Muffled effect
    ZegoVoiceChangerPresetMuffled = 16,
    /// Roundness effect
    ZegoVoiceChangerPresetRoundness = 17,
    /// Falsetto effect
    ZegoVoiceChangerPresetFalsetto = 18,
    /// Fullness effect
    ZegoVoiceChangerPresetFullness = 19,
    /// Clear effect
    ZegoVoiceChangerPresetClear = 20,
    /// Hight effect
    ZegoVoiceChangerPresetHighlyResonant = 21,
    /// Loud clear effect
    ZegoVoiceChangerPresetLoudClear = 22,
    /// Minions effect
    ZegoVoiceChangerPresetMinions = 23,
    /// Sunshine effect
    ZegoVoiceChangerPresetSunshine = 24,
    /// Gentle effect
    ZegoVoiceChangerPresetGentle = 25,
    /// Sweet effect
    ZegoVoiceChangerPresetSweet = 26,
    /// Sweet male effect
    ZegoVoiceChangerPresetSweetMale = 27,
    /// Sweet female effec
    ZegoVoiceChangerPresetSweetFemale = 28,
    /// Bright effect
    ZegoVoiceChangerPresetBright = 29,
    /// Autobot effect
    ZegoVoiceChangerPresetAutobot = 30,
    /// Out of power effect
    ZegoVoiceChangerPresetOutOfPower = 31
};

/// Reverberation preset value.
typedef NS_ENUM(NSUInteger, ZegoReverbPreset) {
    /// No Reverberation
    ZegoReverbPresetNone = 0,
    /// Soft room reverb effect
    ZegoReverbPresetSoftRoom = 1,
    /// Large room reverb effect
    ZegoReverbPresetLargeRoom = 2,
    /// Concert hall reverb effect
    ZegoReverbPresetConcertHall = 3,
    /// Valley reverb effect
    ZegoReverbPresetValley = 4,
    /// Recording studio reverb effect
    ZegoReverbPresetRecordingStudio = 5,
    /// Basement reverb effect
    ZegoReverbPresetBasement = 6,
    /// KTV reverb effect
    ZegoReverbPresetKTV = 7,
    /// Popular reverb effect
    ZegoReverbPresetPopular = 8,
    /// Rock reverb effect
    ZegoReverbPresetRock = 9,
    /// Vocal concert reverb effect
    ZegoReverbPresetVocalConcert = 10,
    /// Gramophone reverb effect
    ZegoReverbPresetGramoPhone = 11,
    /// Enhanced KTV reverb effect. Provide KTV effect with more concentrated voice and better brightness. Compared with the original KTV reverb effect, the reverberation time is shortened and the dry-wet ratio is increased.
    ZegoReverbPresetEnhancedKTV = 12,
    /// Enhanced Rock reverb effect
    ZegoReverbPresetEnhancedRock = 13,
    /// Enhanced misty reverb effect
    ZegoReverbPresetEnhancedMisty = 14,
    /// Hip Hop reverb effect
    ZegoReverbPresetHipHop = 15,
    /// Misty reverb effect
    ZegoReverbPresetMisty = 16,
    /// 3D voice reverb effect
    ZegoReverbPresetThreeDimensionalVoice = 17
};

/// Mode of Electronic Effects.
typedef NS_ENUM(NSUInteger, ZegoElectronicEffectsMode) {
    /// Major
    ZegoElectronicEffectsModeMajor = 0,
    /// Minor
    ZegoElectronicEffectsModeMinor = 1,
    /// Harmonic Minor
    ZegoElectronicEffectsModeHarmonicMinor = 2
};

/// Video configuration resolution and bitrate preset enumeration. The preset resolutions are adapted for mobile and desktop. On mobile, height is longer than width, and desktop is the opposite. For example, 1080p is actually 1080(w) x 1920(h) on mobile and 1920(w) x 1080(h) on desktop.
typedef NS_ENUM(NSUInteger, ZegoVideoConfigPreset) {
    /// Set the resolution to 320x180, the default is 15 fps, the code rate is 300 kbps
    ZegoVideoConfigPreset180P = 0,
    /// Set the resolution to 480x270, the default is 15 fps, the code rate is 400 kbps
    ZegoVideoConfigPreset270P = 1,
    /// Set the resolution to 640x360, the default is 15 fps, the code rate is 600 kbps
    ZegoVideoConfigPreset360P = 2,
    /// Set the resolution to 960x540, the default is 15 fps, the code rate is 1200 kbps
    ZegoVideoConfigPreset540P = 3,
    /// Set the resolution to 1280x720, the default is 15 fps, the code rate is 1500 kbps
    ZegoVideoConfigPreset720P = 4,
    /// Set the resolution to 1920x1080, the default is 15 fps, the code rate is 3000 kbps
    ZegoVideoConfigPreset1080P = 5
};

/// Stream quality level.
typedef NS_ENUM(NSUInteger, ZegoStreamQualityLevel) {
    /// Excellent
    ZegoStreamQualityLevelExcellent = 0,
    /// Good
    ZegoStreamQualityLevelGood = 1,
    /// Normal
    ZegoStreamQualityLevelMedium = 2,
    /// Bad
    ZegoStreamQualityLevelBad = 3,
    /// Failed
    ZegoStreamQualityLevelDie = 4,
    /// Unknown
    ZegoStreamQualityLevelUnknown = 5
};

/// Audio channel type.
typedef NS_ENUM(NSUInteger, ZegoAudioChannel) {
    /// Unknown
    ZegoAudioChannelUnknown = 0,
    /// Mono
    ZegoAudioChannelMono = 1,
    /// Stereo
    ZegoAudioChannelStereo = 2
};

/// Audio capture stereo mode.
typedef NS_ENUM(NSUInteger, ZegoAudioCaptureStereoMode) {
    /// Disable stereo capture, that is, mono.
    ZegoAudioCaptureStereoModeNone = 0,
    /// Always enable stereo capture.
    ZegoAudioCaptureStereoModeAlways = 1,
    /// @deprecated Same as [Always], that is, always enable stereo capture
    ZegoAudioCaptureStereoModeAdaptive DEPRECATED_ATTRIBUTE = 2
};

/// Audio mix mode.
typedef NS_ENUM(NSUInteger, ZegoAudioMixMode) {
    /// Default mode, no special behavior
    ZegoAudioMixModeRaw = 0,
    /// Audio focus mode, which can highlight the sound of a certain stream in multiple audio streams
    ZegoAudioMixModeFocused = 1
};

/// Audio codec ID.
typedef NS_ENUM(NSUInteger, ZegoAudioCodecID) {
    /// Default, determined by the [scenario] when calling [createEngine].
    ZegoAudioCodecIDDefault = 0,
    /// Can be used for RTC and CDN streaming; bitrate range from 10kbps to 128kbps; supports stereo; latency is around 500ms. Server cloud transcoding is required when communicating with the Web SDK, and it is not required when relaying to CDN.
    ZegoAudioCodecIDNormal = 1,
    /// Can be used for RTC and CDN streaming; good compatibility; bitrate range from 16kbps to 192kbps; supports stereo; latency is around 350ms; the sound quality is worse than [Normal] in the same (low) bitrate. Server cloud transcoding is required when communicating with the Web SDK, and it is not required when relaying to CDN.
    ZegoAudioCodecIDNormal2 = 2,
    /// Not recommended; if you need to use it, please contact ZEGO technical support. Can only be used for RTC streaming.
    ZegoAudioCodecIDNormal3 = 3,
    /// Not recommended; if you need to use it, please contact ZEGO technical support. Can only be used for RTC streaming.
    ZegoAudioCodecIDLow = 4,
    /// Not recommended; if you need to use it, please contact ZEGO technical support. Can only be used for RTC streaming; maximum bitrate is 16kbps.
    ZegoAudioCodecIDLow2 = 5,
    /// Can only be used for RTC streaming; bitrate range from 6kbps to 192kbps; supports stereo; latency is around 200ms; Under the same bitrate (low bitrate), the sound quality is significantly better than [Normal] and [Normal2]; low CPU overhead. Server cloud transcoding is not required when communicating with the Web SDK, and it is required when relaying to CDN.
    ZegoAudioCodecIDLow3 = 6
};

/// Video codec ID.
typedef NS_ENUM(NSUInteger, ZegoVideoCodecID) {
    /// Default (H.264)
    ZegoVideoCodecIDDefault = 0,
    /// Scalable Video Coding (H.264 SVC)
    ZegoVideoCodecIDSVC = 1,
    /// VP8
    ZegoVideoCodecIDVP8 = 2,
    /// H.265
    ZegoVideoCodecIDH265 = 3,
    /// Dualstream Scalable Video Coding
    ZegoVideoCodecIDH264DualStream = 4,
    /// Unknown Video Coding
    ZegoVideoCodecIDUnknown = 100
};

/// Backend implementation of video codec.
typedef NS_ENUM(NSUInteger, ZegoVideoCodecBackend) {
    /// Software or Hardware
    ZegoVideoCodecBackendAny = 0,
    /// Software
    ZegoVideoCodecBackendSoftware = 1,
    /// Hardware
    ZegoVideoCodecBackendHardware = 2
};

/// Video stream type
typedef NS_ENUM(NSUInteger, ZegoVideoStreamType) {
    /// The type to be played depends on the network status
    ZegoVideoStreamTypeDefault = 0,
    /// small resolution type
    ZegoVideoStreamTypeSmall = 1,
    /// big resolution type
    ZegoVideoStreamTypeBig = 2
};

/// Audio echo cancellation mode.
typedef NS_ENUM(NSUInteger, ZegoAECMode) {
    /// Aggressive echo cancellation may affect the sound quality slightly, but the echo will be very clean.
    ZegoAECModeAggressive = 0,
    /// Moderate echo cancellation, which may slightly affect a little bit of sound, but the residual echo will be less.
    ZegoAECModeMedium = 1,
    /// Comfortable echo cancellation, that is, echo cancellation does not affect the sound quality of the sound, and sometimes there may be a little echo, but it will not affect the normal listening.
    ZegoAECModeSoft = 2,
    /// AI echo cancellation. Supports intelligent recognition and elimination of echo, with a significant improvement in vocal fidelity compared to traditional AEC algorithms, without additional delay or power consumption increase.
    ZegoAECModeAI = 3
};

/// Active Noise Suppression mode.
typedef NS_ENUM(NSUInteger, ZegoANSMode) {
    /// Soft ANS. In most instances, the sound quality will not be damaged, but some noise will remain.
    ZegoANSModeSoft = 0,
    /// Medium ANS. It may damage some sound quality, but it has a good noise reduction effect.
    ZegoANSModeMedium = 1,
    /// Aggressive ANS. It may significantly impair the sound quality, but it has a good noise reduction effect.
    ZegoANSModeAggressive = 2,
    /// AI mode ANS. It will cause great damage to music, so it can not be used for noise suppression of sound sources that need to collect background sound. Please contact ZEGO technical support before use.
    ZegoANSModeAI = 3,
    /// Balanced AI mode ANS. It will cause great damage to music, so it can not be used for noise suppression of sound sources that need to collect background sound. Please contact ZEGO technical support before use.
    ZegoANSModeAIBalanced = 4,
    /// Low latency AI mode ANS. It will cause great damage to music, so it can not be used for noise suppression of sound sources that need to collect background sound. Please contact ZEGO technical support before use.
    ZegoANSModeAILowLatency = 5,
    /// Aggressive AI mode ANS. It will cause great damage to music, so it can not be used for noise suppression of sound sources that need to collect background sound. Please contact ZEGO technical support before use.
    ZegoANSModeAIAggressive = 6
};

/// video encode profile.
typedef NS_ENUM(NSUInteger, ZegoEncodeProfile) {
    /// The default video encode specifications, The default value is the video encoding specification at the Main level.
    ZegoEncodeProfileDefault = 0,
    /// Baseline-level video encode specifications have low decoding consumption and poor picture effects. They are generally used for low-level applications or applications that require additional fault tolerance.
    ZegoEncodeProfileBaseline = 1,
    /// Main-level video encode specifications, decoding consumption is slightly higher than Baseline, the picture effect is also better, generally used in mainstream consumer electronic products.
    ZegoEncodeProfileMain = 2,
    /// High-level video encode specifications, decoding consumption is higher than Main, the picture effect is better, generally used for broadcasting and video disc storage, high-definition TV.
    ZegoEncodeProfileHigh = 3
};

/// Video rate control mode, the default mode is constant video rate.
typedef NS_ENUM(NSUInteger, ZegoVideoRateControlMode) {
    /// Constant rate.
    ZegoVideoRateControlModeConstantRate = 0,
    /// Constant quality, if this mode is used, the video rate fluctuates according to the network speed. For example, in the live broadcast of games, the constant quality mode will be used to improve the video quality in order to let the audience see smooth operation pictures.
    ZegoVideoRateControlModeConstantQuality = 1
};

/// Stream alignment mode.
typedef NS_ENUM(NSUInteger, ZegoStreamAlignmentMode) {
    /// Disable stream alignment.
    ZegoStreamAlignmentModeNone = 0,
    /// Streams should be aligned as much as possible, call the [setStreamAlignmentProperty] function to enable the stream alignment of the push stream network time alignment of the specified channel.
    ZegoStreamAlignmentModeTry = 1
};

/// Mixed stream sets the image parameter check mode.
typedef NS_ENUM(NSUInteger, ZegoMixImageCheckMode) {
    /// Strictly perform image verification, set the background image, watermark will verify the image path, the image occupy set in the mixed flow input parameter will also verify whether the set image resource request is successful, in order to normally initiate mixed flow, otherwise fail to initiate mixed flow.
    ZegoMixImageCheckModeNormal = 0,
    /// Only verify image path URI/URL As long as the path is correct, the mixed flow is successfully initiated.
    ZegoMixImageCheckModePath = 1,
    /// The mixed flow can be initiated successfully without checking the related parameters of the picture.
    ZegoMixImageCheckModeNothing = 2
};

/// Traffic control property (bitmask enumeration).
typedef NS_OPTIONS(NSUInteger, ZegoTrafficControlProperty) {
    /// Basic (Adaptive (reduce) video bitrate)
    ZegoTrafficControlPropertyBasic = 0,
    /// Adaptive (reduce) video FPS
    ZegoTrafficControlPropertyAdaptiveFPS = 1,
    /// Adaptive (reduce) video resolution
    ZegoTrafficControlPropertyAdaptiveResolution = 2,
    /// Adaptive (reduce) audio bitrate
    ZegoTrafficControlPropertyAdaptiveAudioBitrate = 4
};

/// Video transmission mode when current bitrate is lower than the set minimum bitrate.
typedef NS_ENUM(NSUInteger, ZegoTrafficControlMinVideoBitrateMode) {
    /// Stop video transmission when current bitrate is lower than the set minimum bitrate
    ZegoTrafficControlMinVideoBitrateModeNoVideo = 0,
    /// Video is sent at a very low frequency (no more than 2fps) which is lower than the set minimum bitrate
    ZegoTrafficControlMinVideoBitrateModeUltraLowFPS = 1
};

/// Factors that trigger traffic control
typedef NS_ENUM(NSUInteger, ZegoTrafficControlFocusOnMode) {
    /// Focus only on the local network
    ZegoTrafficControlFounsOnLocalOnly = 0,
    /// Pay attention to the local network, but also take into account the remote network, currently only effective in the 1v1 scenario
    ZegoTrafficControlFounsOnRemote = 1
};

/// Playing stream status.
typedef NS_ENUM(NSUInteger, ZegoPlayerState) {
    /// The state of the flow is not played, and it is in this state before the stream is played. If the steady flow anomaly occurs during the playing process, such as AppID or Token are incorrect, it will enter this state.
    ZegoPlayerStateNoPlay = 0,
    /// The state that the stream is being requested for playing. After the [startPlayingStream] function is successfully called, it will enter the state. The UI is usually displayed through this state. If the connection is interrupted due to poor network quality, the SDK will perform an internal retry and will return to the requesting state.
    ZegoPlayerStatePlayRequesting = 1,
    /// The state that the stream is being playing, entering the state indicates that the stream has been successfully played, and the user can communicate normally.
    ZegoPlayerStatePlaying = 2
};

/// Media event when playing.
typedef NS_ENUM(NSUInteger, ZegoPlayerMediaEvent) {
    /// Audio stuck event when playing
    ZegoPlayerMediaEventAudioBreakOccur = 0,
    /// Audio stuck event recovery when playing
    ZegoPlayerMediaEventAudioBreakResume = 1,
    /// Video stuck event when playing
    ZegoPlayerMediaEventVideoBreakOccur = 2,
    /// Video stuck event recovery when playing
    ZegoPlayerMediaEventVideoBreakResume = 3
};

/// Resource Type.
typedef NS_ENUM(NSUInteger, ZegoResourceType) {
    /// CDN
    ZegoResourceTypeCDN = 0,
    /// RTC
    ZegoResourceTypeRTC = 1,
    /// L3
    ZegoResourceTypeL3 = 2
};

/// Stream Resource Mode
typedef NS_ENUM(NSUInteger, ZegoStreamResourceMode) {
    /// Default mode. The SDK will automatically select the streaming resource according to the cdnConfig parameters set by the player config and the ready-made background configuration.
    ZegoStreamResourceModeDefault = 0,
    /// Playing stream only from CDN.
    ZegoStreamResourceModeOnlyCDN = 1,
    /// Playing stream only from L3.
    ZegoStreamResourceModeOnlyL3 = 2,
    /// Playing stream only from RTC.
    ZegoStreamResourceModeOnlyRTC = 3,
    /// @deprecated Legacy CDN Plus
    ZegoStreamResourceModeCDNPlus DEPRECATED_ATTRIBUTE = 4,
    /// Custom mode. The SDK selects the streaming resource based on the customResourceConfig parameter of the streaming settings.
    ZegoStreamResourceModeCustom = 5
};

/// Stream Switch Resource Mode
typedef NS_ENUM(NSUInteger, ZegoStreamResourceSwitchMode) {
    /// Default mode. The SDK will automatically select the streaming resource according to the parameters set by the player config and the ready-made background configuration.
    ZegoStreamResourceSwitchModeDefault = 0,
    /// Auto switch to RTC resource when publishing.
    ZegoStreamResourceSwitchModeSwitchToRTC = 1,
    /// Keep using original resource when publishing, not switch to RTC resource.
    ZegoStreamResourceSwitchModeKeepOriginal = 2
};

/// Stream Resource Type
typedef NS_ENUM(NSUInteger, ZegoStreamResourceType) {
    /// Default mode. The SDK will automatically select the streaming resource according to the parameters set by the player config and the ready-made background configuration.
    ZegoStreamResourceTypeDefault = 0,
    /// CDN resource.
    ZegoStreamResourceTypeCDN = 1,
    /// L3 resource.
    ZegoStreamResourceTypeL3 = 2
};

/// Update type.
typedef NS_ENUM(NSUInteger, ZegoUpdateType) {
    /// Add
    ZegoUpdateTypeAdd = 0,
    /// Delete
    ZegoUpdateTypeDelete = 1
};

/// Get room stream list type.
typedef NS_ENUM(NSUInteger, ZegoRoomStreamListType) {
    /// List of all online streams in the current room, excluding your own streams
    ZegoRoomStreamListTypePlay = 0,
    /// List of all online streams in the current room, including your own streams
    ZegoRoomStreamListTypeAll = 1
};

/// Capability negotiation enable bitmask enumeration.
typedef NS_OPTIONS(NSUInteger, ZegoRoomCapabilityNegotiationTypesBitMask) {
    /// The mask bit of this field corresponds to enable the capability negotiation of all user in the room.
    ZegoRoomCapabilityNegotiationTypesBitMaskAll = 1 << 0,
    /// The mask bit of this field corresponds to enable the capability negotiation of publisher in the room.
    ZegoRoomCapabilityNegotiationTypesPublisher = 1 << 1
};

/// State of CDN relay.
typedef NS_ENUM(NSUInteger, ZegoStreamRelayCDNState) {
    /// The state indicates that there is no CDN relay
    ZegoStreamRelayCDNStateNoRelay = 0,
    /// The CDN relay is being requested
    ZegoStreamRelayCDNStateRelayRequesting = 1,
    /// Entering this status indicates that the CDN relay has been successful
    ZegoStreamRelayCDNStateRelaying = 2
};

/// Reason for state of CDN relay changed.
typedef NS_ENUM(NSUInteger, ZegoStreamRelayCDNUpdateReason) {
    /// No error
    ZegoStreamRelayCDNUpdateReasonNone = 0,
    /// Server error
    ZegoStreamRelayCDNUpdateReasonServerError = 1,
    /// Handshake error
    ZegoStreamRelayCDNUpdateReasonHandshakeFailed = 2,
    /// Access point error
    ZegoStreamRelayCDNUpdateReasonAccessPointError = 3,
    /// Stream create failure
    ZegoStreamRelayCDNUpdateReasonCreateStreamFailed = 4,
    /// Bad stream ID
    ZegoStreamRelayCDNUpdateReasonBadName = 5,
    /// CDN server actively disconnected
    ZegoStreamRelayCDNUpdateReasonCDNServerDisconnected = 6,
    /// Active disconnect
    ZegoStreamRelayCDNUpdateReasonDisconnected = 7,
    /// All mixer input streams sessions closed
    ZegoStreamRelayCDNUpdateReasonMixStreamAllInputStreamClosed = 8,
    /// All mixer input streams have no data
    ZegoStreamRelayCDNUpdateReasonMixStreamAllInputStreamNoData = 9,
    /// Internal error of stream mixer server
    ZegoStreamRelayCDNUpdateReasonMixStreamServerInternalError = 10
};

/// Beauty feature (bitmask enumeration).
typedef NS_OPTIONS(NSUInteger, ZegoBeautifyFeature) {
    /// No beautifying
    ZegoBeautifyFeatureNone = 0,
    /// Polish
    ZegoBeautifyFeaturePolish = 1 << 0,
    /// Sharpen
    ZegoBeautifyFeatureWhiten = 1 << 1,
    /// Skin whiten
    ZegoBeautifyFeatureSkinWhiten = 1 << 2,
    /// Whiten
    ZegoBeautifyFeatureSharpen = 1 << 3
};

/// Device type.
typedef NS_ENUM(NSUInteger, ZegoDeviceType) {
    /// Unknown device type.
    ZegoDeviceTypeUnknown = 0,
    /// Camera device.
    ZegoDeviceTypeCamera = 1,
    /// Microphone device.
    ZegoDeviceTypeMicrophone = 2,
    /// Speaker device.
    ZegoDeviceTypeSpeaker = 3,
    /// Audio device. (Other audio device that cannot be accurately classified into microphones or speakers.)
    ZegoDeviceTypeAudioDevice = 4,
    /// Audio Session.
    ZegoDeviceTypeAudioSession = 5
};

/// The exception type for the device.
typedef NS_ENUM(NSUInteger, ZegoDeviceExceptionType) {
    /// Unknown device exception.
    ZegoDeviceExceptionTypeUnknown = 0,
    /// Generic device exception.
    ZegoDeviceExceptionTypeGeneric = 1,
    /// Invalid device ID exception.
    ZegoDeviceExceptionTypeInvalidId = 2,
    /// Device permission is not granted.
    ZegoDeviceExceptionTypePermissionNotGranted = 3,
    /// The capture frame rate of the device is 0.
    ZegoDeviceExceptionTypeZeroCaptureFps = 4,
    /// The device is being occupied.
    ZegoDeviceExceptionTypeDeviceOccupied = 5,
    /// The device is unplugged (not plugged in).
    ZegoDeviceExceptionTypeDeviceUnplugged = 6,
    /// The device requires the system to restart before it can work (Windows platform only).
    ZegoDeviceExceptionTypeRebootRequired = 7,
    /// The system media service is unavailable, e.g. when the iOS system detects that the current pressure is huge (such as playing a lot of animation), it is possible to disable all media related services (Apple platform only).
    ZegoDeviceExceptionTypeMediaServicesWereLost = 8,
    /// The device is being occupied by Siri (Apple platform only).
    ZegoDeviceExceptionTypeSiriIsRecording = 9,
    /// The device captured sound level is too low (Windows platform only).
    ZegoDeviceExceptionTypeSoundLevelTooLow = 10,
    /// The device is being occupied, and maybe cause by iPad magnetic case (Apple platform only).
    ZegoDeviceExceptionTypeMagneticCase = 11,
    /// Audio session deactive (Apple platform only).
    ZegoDeviceExceptionTypeAudioSessionDeactive = 12,
    /// Audio session category change (Apple platform only).
    ZegoDeviceExceptionTypeAudioSessionCategoryChange = 13
};

/// Mode for notifying the local device state.
typedef NS_ENUM(NSUInteger, ZegoExpNotifyDeviceStateMode) {
    /// Notify the local device state.
    ZegoExpNotifyDeviceStateModeOpen = 0,
    /// Do not notify the local device state. Only valid for disable device.
    ZegoExpNotifyDeviceStateModeClose = 1
};

/// Remote device status.
typedef NS_ENUM(NSUInteger, ZegoRemoteDeviceState) {
    /// Device on
    ZegoRemoteDeviceStateOpen = 0,
    /// General device error
    ZegoRemoteDeviceStateGenericError = 1,
    /// Invalid device ID
    ZegoRemoteDeviceStateInvalidID = 2,
    /// No permission
    ZegoRemoteDeviceStateNoAuthorization = 3,
    /// Captured frame rate is 0
    ZegoRemoteDeviceStateZeroFPS = 4,
    /// The device is occupied
    ZegoRemoteDeviceStateInUseByOther = 5,
    /// The device is not plugged in or unplugged
    ZegoRemoteDeviceStateUnplugged = 6,
    /// The system needs to be restarted
    ZegoRemoteDeviceStateRebootRequired = 7,
    /// System media services stop, such as under the iOS platform, when the system detects that the current pressure is huge (such as playing a lot of animation), it is possible to disable all media related services.
    ZegoRemoteDeviceStateSystemMediaServicesLost = 8,
    /// The remote user calls [enableCamera] or [enableAudioCaptureDevice] to disable the camera or microphone.
    ZegoRemoteDeviceStateDisable = 9,
    /// The remote user actively calls [muteMicrophone] or [mutePublishStreamAudio] or [mutePublishStreamVideo] to stop publish the audio or video stream.
    ZegoRemoteDeviceStateMute = 10,
    /// The device is interrupted, such as a phone call interruption, etc.
    ZegoRemoteDeviceStateInterruption = 11,
    /// There are multiple apps at the same time in the foreground, such as the iPad app split screen, the system will prohibit all apps from using the camera.
    ZegoRemoteDeviceStateInBackground = 12,
    /// CDN server actively disconnected
    ZegoRemoteDeviceStateMultiForegroundApp = 13,
    /// The system is under high load pressure and may cause abnormal equipment.
    ZegoRemoteDeviceStateBySystemPressure = 14,
    /// The remote device is not supported to publish the device state.
    ZegoRemoteDeviceStateNotSupport = 15
};

/// Audio device type.
typedef NS_ENUM(NSUInteger, ZegoAudioDeviceType) {
    /// Audio input type
    ZegoAudioDeviceTypeInput = 0,
    /// Audio output type
    ZegoAudioDeviceTypeOutput = 1
};

/// Audio device mode.
typedef NS_ENUM(NSUInteger, ZegoAudioDeviceMode) {
    /// Enable system echo cancellation.
    ZegoAudioDeviceModeCommunication = 1,
    /// The system echo cancellation function is disabled.
    ZegoAudioDeviceModeGeneral = 2,
    /// Automatically select whether to enable system echo cancellation based on the scenario.
    ZegoAudioDeviceModeAuto = 3,
    /// Enable system echo cancellation. Compared with Communication, this mode always occupies the microphone device.
    ZegoAudioDeviceModeCommunication2 = 4,
    /// Enable system echo cancellation. Compared with Communication, in this mode, the microphone is released and the media volume is reduced.
    ZegoAudioDeviceModeCommunication3 = 5,
    /// Disable system echo cancellation. Compared with General, this mode is not released when a microphone device is used.
    ZegoAudioDeviceModeGeneral2 = 6,
    /// Disable system echo cancellation. Compared with General, this mode will always occupy the microphone device.
    ZegoAudioDeviceModeGeneral3 = 7,
    /// Enable system echo cancellation. Compared with Communication, this mode of wheat after releasing the microphone, maintain the call volume.
    ZegoAudioDeviceModeCommunication4 = 8
};

/// Audio route
typedef NS_ENUM(NSUInteger, ZegoAudioRoute) {
    /// Speaker
    ZegoAudioRouteSpeaker = 0,
    /// Headphone
    ZegoAudioRouteHeadphone = 1,
    /// Bluetooth device
    ZegoAudioRouteBluetooth = 2,
    /// Receiver
    ZegoAudioRouteReceiver = 3,
    /// External USB audio device
    ZegoAudioRouteExternalUSB = 4,
    /// Apple AirPlay
    ZegoAudioRouteAirPlay = 5
};

/// Mix stream content type.
typedef NS_ENUM(NSUInteger, ZegoMixerInputContentType) {
    /// Mix stream for audio only
    ZegoMixerInputContentTypeAudio = 0,
    /// Mix stream for both audio and video
    ZegoMixerInputContentTypeVideo = 1,
    /// Mix stream for video only. On web platforms, this property does not take effect
    ZegoMixerInputContentTypeVideoOnly = 2
};

/// Capture pipeline scale mode.
typedef NS_ENUM(NSUInteger, ZegoCapturePipelineScaleMode) {
    /// Zoom immediately after acquisition, default
    ZegoCapturePipelineScaleModePre = 0,
    /// Scaling while encoding
    ZegoCapturePipelineScaleModePost = 1
};

/// Video frame format.
typedef NS_ENUM(NSUInteger, ZegoVideoFrameFormat) {
    /// Unknown format, will take platform default
    ZegoVideoFrameFormatUnknown = 0,
    /// I420 (YUV420Planar) format
    ZegoVideoFrameFormatI420 = 1,
    /// NV12 (YUV420SemiPlanar) format
    ZegoVideoFrameFormatNV12 = 2,
    /// NV21 (YUV420SemiPlanar) format
    ZegoVideoFrameFormatNV21 = 3,
    /// BGRA32 format
    ZegoVideoFrameFormatBGRA32 = 4,
    /// RGBA32 format
    ZegoVideoFrameFormatRGBA32 = 5,
    /// ARGB32 format
    ZegoVideoFrameFormatARGB32 = 6,
    /// ABGR32 format
    ZegoVideoFrameFormatABGR32 = 7,
    /// I422 (YUV422Planar) format
    ZegoVideoFrameFormatI422 = 8,
    /// BGR24 format
    ZegoVideoFrameFormatBGR24 = 9,
    /// RGB24 format
    ZegoVideoFrameFormatRGB24 = 10
};

/// Video encoded frame format.
typedef NS_ENUM(NSUInteger, ZegoVideoEncodedFrameFormat) {
    /// AVC AVCC format
    ZegoVideoEncodedFrameFormatAVCC = 0,
    /// AVC Annex-B format
    ZegoVideoEncodedFrameFormatAnnexB = 1,
    /// VP8 format
    ZegoVideoEncodedFrameFormatVP8 = 2,
    /// HEVC AVCC format
    ZegoVideoEncodedFrameFormatHEVCAVCC = 3,
    /// HEVC Annex-B format
    ZegoVideoEncodedFrameFormatHEVCAnnexB = 4
};

/// Video frame buffer type.
typedef NS_ENUM(NSUInteger, ZegoVideoBufferType) {
    /// Raw data type video frame
    ZegoVideoBufferTypeUnknown = 0,
    /// Raw data type video frame
    ZegoVideoBufferTypeRawData = 1,
    /// Encoded data type video frame
    ZegoVideoBufferTypeEncodedData = 2,
    /// Texture 2D type video frame
    ZegoVideoBufferTypeGLTexture2D = 3,
    /// CVPixelBuffer type video frame
    ZegoVideoBufferTypeCVPixelBuffer = 4,
    /// CVPixelBuffer type nv12 format video frame. Only for custom video processing
    ZegoVideoBufferTypeNV12CVPixelBuffer = 9
};

/// Video frame format series.
typedef NS_ENUM(NSUInteger, ZegoVideoFrameFormatSeries) {
    /// RGB series
    ZegoVideoFrameFormatSeriesRGB = 0,
    /// YUV series
    ZegoVideoFrameFormatSeriesYUV = 1
};

/// Video frame flip mode.
typedef NS_ENUM(NSUInteger, ZegoVideoFlipMode) {
    /// No flip
    ZegoVideoFlipModeNone = 0,
    /// X-axis flip
    ZegoVideoFlipModeX = 1,
    /// Y-axis flip
    ZegoVideoFlipModeY = 2,
    /// X-Y-axis flip
    ZegoVideoFlipModeXY = 3
};

/// Customize the audio processing configuration type.
typedef NS_ENUM(NSUInteger, ZegoCustomAudioProcessType) {
    /// Remote audio processing
    ZegoCustomAudioProcessTypeRemote = 0,
    /// Capture audio processing
    ZegoCustomAudioProcessTypeCapture = 1,
    /// Remote audio and capture audio processing
    ZegoCustomAudioProcessTypeCaptureAndRemote = 2
};

/// Audio Config Preset.
typedef NS_ENUM(NSUInteger, ZegoAudioConfigPreset) {
    /// Basic sound quality (16 kbps, Mono, ZegoAudioCodecIDDefault)
    ZegoAudioConfigPresetBasicQuality = 0,
    /// Standard sound quality (48 kbps, Mono, ZegoAudioCodecIDDefault)
    ZegoAudioConfigPresetStandardQuality = 1,
    /// Standard sound quality (56 kbps, Stereo, ZegoAudioCodecIDDefault)
    ZegoAudioConfigPresetStandardQualityStereo = 2,
    /// High sound quality (128 kbps, Mono, ZegoAudioCodecIDDefault)
    ZegoAudioConfigPresetHighQuality = 3,
    /// High sound quality (192 kbps, Stereo, ZegoAudioCodecIDDefault)
    ZegoAudioConfigPresetHighQualityStereo = 4
};

/// Range audio mode
typedef NS_ENUM(NSUInteger, ZegoRangeAudioMode) {
    /// World mode, you can communicate with everyone in the room.
    ZegoRangeAudioModeWorld = 0,
    /// Team mode, only communicate with members of the team.
    ZegoRangeAudioModeTeam = 1,
    /// Secret team mode, communicate with members of the team (with team members), can hear the voices of members who within the audio receive range (except the team members).
    ZegoRangeAudioModeSecretTeam = 2
};

/// Range audio speak mode
typedef NS_ENUM(NSUInteger, ZegoRangeAudioSpeakMode) {
    /// All mode, his voice can be heard by members of the team and by those within range.
    ZegoRangeAudioSpeakModeAll = 0,
    /// Only world mode, only those within range can hear his voice.
    ZegoRangeAudioSpeakModeWorld = 1,
    /// Only team mode, only members of the team can hear his voice (not restricted by range).
    ZegoRangeAudioSpeakModeTeam = 2
};

/// Range audio listening mode
typedef NS_ENUM(NSUInteger, ZegoRangeAudioListenMode) {
    /// All mode, can hear everyone in the room.
    ZegoRangeAudioListenModeAll = 0,
    /// Only world mode, only listen to people within range.
    ZegoRangeAudioListenModeWorld = 1,
    /// Only team mode, only listen to the voices of members of the team you belong to (not restricted by range).
    ZegoRangeAudioListenModeTeam = 2
};

/// Range audio microphone state.
typedef NS_ENUM(NSUInteger, ZegoRangeAudioMicrophoneState) {
    /// The range audio microphone is off.
    ZegoRangeAudioMicrophoneStateOff = 0,
    /// The range audio microphone is turning on.
    ZegoRangeAudioMicrophoneStateTurningOn = 1,
    /// The range audio microphone is on.
    ZegoRangeAudioMicrophoneStateOn = 2
};

/// Player state.
typedef NS_ENUM(NSUInteger, ZegoMediaPlayerState) {
    /// Not playing
    ZegoMediaPlayerStateNoPlay = 0,
    /// Playing
    ZegoMediaPlayerStatePlaying = 1,
    /// Pausing
    ZegoMediaPlayerStatePausing = 2,
    /// End of play
    ZegoMediaPlayerStatePlayEnded = 3
};

/// Player audio track mode.
typedef NS_ENUM(NSUInteger, ZegoMediaPlayerAudioTrackMode) {
    /// Normal audio track mode
    ZegoMediaPlayerAudioTrackModeNormal = 0,
    /// Multiple audio track mode
    ZegoMediaPlayerAudioTrackModeMultiple = 1
};

/// Player network event.
typedef NS_ENUM(NSUInteger, ZegoMediaPlayerNetworkEvent) {
    /// Network resources are not playing well, and start trying to cache data
    ZegoMediaPlayerNetworkEventBufferBegin = 0,
    /// Network resources can be played smoothly
    ZegoMediaPlayerNetworkEventBufferEnded = 1
};

/// Audio channel.
typedef NS_ENUM(NSUInteger, ZegoMediaPlayerAudioChannel) {
    /// Audio channel left
    ZegoMediaPlayerAudioChannelLeft = 0,
    /// Audio channel right
    ZegoMediaPlayerAudioChannelRight = 1,
    /// Audio channel all
    ZegoMediaPlayerAudioChannelAll = 2
};

/// Media player first frame event type.
typedef NS_ENUM(NSUInteger, ZegoMediaPlayerFirstFrameEvent) {
    /// The first video frame is rendered event.
    ZegoMediaPlayerFirstFrameEventVideoRendered = 0,
    /// The first audio frame is rendered event.
    ZegoMediaPlayerFirstFrameEventAudioRendered = 1
};

/// AudioEffectPlayer state.
typedef NS_ENUM(NSUInteger, ZegoAudioEffectPlayState) {
    /// Not playing
    ZegoAudioEffectPlayStateNoPlay = 0,
    /// Playing
    ZegoAudioEffectPlayStatePlaying = 1,
    /// Pausing
    ZegoAudioEffectPlayStatePausing = 2,
    /// End of play
    ZegoAudioEffectPlayStatePlayEnded = 3
};

/// volume type.
typedef NS_ENUM(NSUInteger, ZegoVolumeType) {
    /// volume local
    ZegoVolumeTypeLocal = 0,
    /// volume remote
    ZegoVolumeTypeRemote = 1
};

/// audio sample rate.
typedef NS_ENUM(NSUInteger, ZegoAudioSampleRate) {
    /// Unknown
    ZegoAudioSampleRateUnknown = 0,
    /// 8K
    ZegoAudioSampleRate8K = 8000,
    /// 16K
    ZegoAudioSampleRate16K = 16000,
    /// 22.05K
    ZegoAudioSampleRate22K = 22050,
    /// 24K
    ZegoAudioSampleRate24K = 24000,
    /// 32K
    ZegoAudioSampleRate32K = 32000,
    /// 44.1K
    ZegoAudioSampleRate44K = 44100,
    /// 48K
    ZegoAudioSampleRate48K = 48000
};

/// Audio capture source type.
typedef NS_ENUM(NSUInteger, ZegoAudioSourceType) {
    /// Default audio capture source (the main channel uses custom audio capture by default; the aux channel uses the same sound as main channel by default).
    ZegoAudioSourceTypeDefault = 0,
    /// Use custom audio capture, refer to [enableCustomAudioIO] or [setAudioSource].
    ZegoAudioSourceTypeCustom = 1,
    /// Use media player as audio source, only support aux channel.
    ZegoAudioSourceTypeMediaPlayer = 2,
    /// No audio source. This audio source type can only be used in [setAudioSource] interface, has no effect when used in [enableCustomAudioIO] interface.
    ZegoAudioSourceTypeNone = 3,
    /// Using microphone as audio source. This audio source type can only be used in [setAudioSource] interface, has no effect when used in [enableCustomAudioIO] interface.
    ZegoAudioSourceTypeMicrophone = 4,
    /// Using main channel as audio source. Ineffective when used in main channel. This audio source type can only be used in [setAudioSource] interface, has no effect when used in [enableCustomAudioIO] interface.
    ZegoAudioSourceTypeMainPublishChannel = 5,
    /// Using screen capture as audio source. Typically used in mobile screen sharing scenarios. This audio source type can only be used in [setAudioSource] interface, has no effect when used in [enableCustomAudioIO] interface.
    ZegoAudioSourceTypeScreenCapture = 6
};

/// Record type.
typedef NS_ENUM(NSUInteger, ZegoDataRecordType) {
    /// This field indicates that the Express-Audio SDK records audio by default, and the Express-Video SDK records audio and video by default. When recording files in .aac format, audio is also recorded by default.
    ZegoDataRecordTypeDefault = 0,
    /// only record audio
    ZegoDataRecordTypeOnlyAudio = 1,
    /// only record video, Audio SDK and recording .aac format files are invalid.
    ZegoDataRecordTypeOnlyVideo = 2,
    /// record audio and video. Express-Audio SDK and .aac format files are recorded only audio.
    ZegoDataRecordTypeAudioAndVideo = 3
};

/// Record state.
typedef NS_ENUM(NSUInteger, ZegoDataRecordState) {
    /// Unrecorded state, which is the state when a recording error occurs or before recording starts.
    ZegoDataRecordStateNoRecord = 0,
    /// Recording in progress, in this state after successfully call [startRecordingCapturedData] function
    ZegoDataRecordStateRecording = 1,
    /// Record successs
    ZegoDataRecordStateSuccess = 2
};

/// Audio data callback function enable bitmask enumeration.
typedef NS_OPTIONS(NSUInteger, ZegoAudioDataCallbackBitMask) {
    /// The mask bit of this field corresponds to the enable [onCapturedAudioData] callback function
    ZegoAudioDataCallbackBitMaskCaptured = 1 << 0,
    /// The mask bit of this field corresponds to the enable [onPlaybackAudioData] callback function
    ZegoAudioDataCallbackBitMaskPlayback = 1 << 1,
    /// The mask bit of this field corresponds to the enable [onMixedAudioData] callback function
    ZegoAudioDataCallbackBitMaskMixed = 1 << 2,
    /// The mask bit of this field corresponds to the enable [onPlayerAudioData] callback function
    ZegoAudioDataCallbackBitMaskPlayer = 1 << 3
};

/// Network mode
typedef NS_ENUM(NSUInteger, ZegoNetworkMode) {
    /// Offline (No network)
    ZegoNetworkModeOffline = 0,
    /// Unknown network mode
    ZegoNetworkModeUnknown = 1,
    /// Wired Ethernet (LAN)
    ZegoNetworkModeEthernet = 2,
    /// Wi-Fi (WLAN)
    ZegoNetworkModeWiFi = 3,
    /// 2G Network (GPRS/EDGE/CDMA1x/etc.)
    ZegoNetworkMode2G = 4,
    /// 3G Network (WCDMA/HSDPA/EVDO/etc.)
    ZegoNetworkMode3G = 5,
    /// 4G Network (LTE)
    ZegoNetworkMode4G = 6,
    /// 5G Network (NR (NSA/SA))
    ZegoNetworkMode5G = 7
};

/// network speed test type
typedef NS_ENUM(NSUInteger, ZegoNetworkSpeedTestType) {
    /// uplink
    ZegoNetworkSpeedTestTypeUplink = 0,
    /// downlink
    ZegoNetworkSpeedTestTypeDownlink = 1
};

/// VOD billing mode.
typedef NS_ENUM(NSUInteger, ZegoCopyrightedMusicBillingMode) {
    /// Pay-per-use.Each time a user obtains a song resource, a charge is required, that is, the user will be charged for each time based on the actual call to obtain the song resource interface (such as [requestResource] etc.).
    ZegoCopyrightedMusicBillingModeCount = 0,
    /// Monthly billing by user.Billing for a single user is based on the monthly dimension, that is, the statistics call to obtain song resources (such as [requestResource], etc.) and the parameters are the user ID of the monthly subscription, and the charging is based on the monthly dimension.
    ZegoCopyrightedMusicBillingModeUser = 1,
    /// Monthly billing by room.The room users are billed on a monthly basis, that is, statistical calls to obtain song resources (such as [requestResource], etc.) are passed as Roomid for a monthly subscription of the room, and fees are charged on a monthly basis.
    ZegoCopyrightedMusicBillingModeRoom = 2,
    /// Monthly billing by master. Every time a user obtains a resource, it is counted as the owner’s acquisition of resources, that is, according to the actual call to obtain the song resource interface (such as [requestResource], etc.), the parameters are passed as the Roomid of the room and the Masterid of the owner, and the fee is charged according to the owner.
    ZegoCopyrightedMusicBillingModeMaster = 3
};

/// The music resource type. For [querycache] interface.
typedef NS_ENUM(NSUInteger, ZegoCopyrightedMusicType) {
    /// @deprecated Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.
    ZegoCopyrightedMusicSong DEPRECATED_ATTRIBUTE = 0,
    /// @deprecated Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.
    ZegoCopyrightedMusicSongHQ DEPRECATED_ATTRIBUTE = 1,
    /// @deprecated Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.
    ZegoCopyrightedMusicSongSQ DEPRECATED_ATTRIBUTE = 2,
    /// @deprecated Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.
    ZegoCopyrightedMusicAccompaniment DEPRECATED_ATTRIBUTE = 3,
    /// @deprecated Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.
    ZegoCopyrightedMusicAccompanimentClip DEPRECATED_ATTRIBUTE = 4,
    /// @deprecated Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.
    ZegoCopyrightedMusicAccompanimentSegment DEPRECATED_ATTRIBUTE = 5
};

/// The music resource type. For [ZegoCopyrightedMusicRequestConfig], [ZegoCopyrightedMusicGetSharedConfig] and [ZegoCopyrightedMusicQueryCacheConfig].
typedef NS_ENUM(NSUInteger, ZegoCopyrightedMusicResourceType) {
    /// Song.
    ZegoCopyrightedMusicResourceSong = 0,
    /// Song accompaniment.
    ZegoCopyrightedMusicResourceAccompaniment = 1,
    /// Song accompaniment clip.
    ZegoCopyrightedMusicResourceAccompanimentClip = 2,
    /// Song accompaniment segment.
    ZegoCopyrightedMusicResourceAccompanimentSegment = 3
};

/// Copyright music resource song copyright provider. For more information about the copyright owner, please contact ZEGO business personnel.
typedef NS_ENUM(NSUInteger, ZegoCopyrightedMusicVendorID) {
    /// Default copyright provider.
    ZegoCopyrightedMusicVendorDefault = 0,
    /// First copyright provider.
    ZegoCopyrightedMusicVendor1 = 1,
    /// Second copyright provider.
    ZegoCopyrightedMusicVendor2 = 2,
    /// Third copyright provider.
    ZegoCopyrightedMusicVendor3 = 4
};

/// The music resource quality type. For [ZegoCopyrightedMusicQueryCacheConfig].
typedef NS_ENUM(NSUInteger, ZegoCopyrightedMusicResourceQualityType) {
    /// Standard Definition Audio.
    ZegoCopyrightedMusicResourceQualityNormal = 0,
    /// High Definition Audio.
    ZegoCopyrightedMusicResourceQualityHQ = 1,
    /// Lossless Audio Quality.
    ZegoCopyrightedMusicResourceQualitySQ = 2
};

/// Font type.
typedef NS_ENUM(NSUInteger, ZegoFontType) {
    /// Source han sans.
    ZegoFontTypeSourceHanSans = 0,
    /// Alibaba sans.
    ZegoFontTypeAlibabaSans = 1,
    /// Pang men zheng dao title.
    ZegoFontTypePangMenZhengDaoTitle = 2,
    /// HappyZcool.
    ZegoFontTypeHappyZcool = 3
};

/// Mixing stream video view render mode.
typedef NS_ENUM(NSUInteger, ZegoMixRenderMode) {
    /// The proportional zoom fills the entire area and may be partially cut.
    ZegoMixRenderModeFill = 0,
    /// Scale the filled area proportionally. If the scale does not match the set size after scaling, the extra part will be displayed as transparent.
    ZegoMixRenderModeFit = 1
};

/// Camera focus mode.
typedef NS_ENUM(NSUInteger, ZegoCameraFocusMode) {
    /// Auto focus.
    ZegoCameraFocusModeAutoFocus = 0,
    /// Continuous auto focus.
    ZegoCameraFocusModeContinuousAutoFocus = 1
};

/// Camera exposure mode.
typedef NS_ENUM(NSUInteger, ZegoCameraExposureMode) {
    /// Auto exposure.
    ZegoCameraExposureModeAutoExposure = 0,
    /// Continuous auto exposure.
    ZegoCameraExposureModeContinuousAutoExposure = 1
};

/// voice activity detection type
typedef NS_ENUM(NSUInteger, ZegoAudioVADType) {
    /// noise
    ZegoAudioVADTypeNoise = 0,
    /// speech
    ZegoAudioVADTypeSpeech = 1
};

/// stable voice activity detection type
typedef NS_ENUM(NSUInteger, ZegoAudioVADStableStateMonitorType) {
    /// captured
    ZegoAudioVADStableStateMonitorTypeCaptured = 0,
    /// custom processed
    ZegoAudioVADStableStateMonitorTypeCustomProcessed = 1
};

/// Orientation mode of the video.
typedef NS_ENUM(NSUInteger, ZegoOrientationMode) {
    /// Custom mode. The default is the custom mode. In this mode, the user needs to set the orientation through [SetAppOrientation], and set the video resolution through [SetVideoConfig] to control the video ratio. The SDK rotates the video at the stream publishing end.
    ZegoOrientationModeCustom = 0,
    /// Description: Player self adaption mode. The video orientation of the stream playing end is automatically vertically upward, and the user of the stream publishing end no longer needs to set the orientation through [SetAppOrientation], and no longer need to set the video resolution to control the video ratio through [SetVideoConfig]. Caution: 1. Both the stream publishing end and the stream playing end need to be set to [ZegoOrientationModeAdaption] mode. 2. Media players, cloud recording, local recording, and publish or play streaming scenarios via CDN are not supported.  3. In this mode, the SDK will automatically swap the width and height of the encoding resolution according to the actual orientation of the device.
    ZegoOrientationModeAdaption = 1,
    /// Description: Player adapt to pulisher mode. Taking the Status Bar as a reference, the video direction of the stream playing end is the same as the preview video direction of the stream publishing end. The SDK will use the Status Bar as a reference to rotate the image on the stream playing end, and the rotation angle is the same as the rotation angle of the preview on the stream publishing end. Stream publishing end users no longer need to set the orientation through [SetAppOrientation], and no longer need to set the video resolution to control the video ratio through [SetVideoConfig]. Caution: 1. Media players, cloud recording, local recording, and publish or play streaming scenarios via CDN are not supported.2. In this mode, the SDK will automatically swap the width and height of the encoding resolution according to the actual position of the Status Bar.
    ZegoOrientationModeAlignment = 2,
    /// Description: Fixed resolution ratio mode. Taking the Status Bar as a reference, the video orientation of the stream playing end is the same as the previewed video direction of the stream publishing end, and the video resolution is the same as the encoding resolution. Users of the streaming end no longer need to set the orientation through [SetAppOrientation].
    ZegoOrientationModeFixedResolutionRatio = 3
};

/// CDN network protocol types supported by ZEGO
typedef NS_ENUM(NSUInteger, ZegoCDNProtocol) {
    /// TCP protocol
    ZegoCDNProtocolTCP = 1,
    /// QUIC protocol
    ZegoCDNProtocolQUIC = 2
};

/// Supported httpDNS service types.
typedef NS_ENUM(NSUInteger, ZegoHttpDNSType) {
    /// None.
    ZegoHttpDNSTypeNone = 0,
    /// wangsu httpdns.
    ZegoHttpDNSTypeWangsu = 1,
    /// tencent httpdns.
    ZegoHttpDNSTypeTencent = 2,
    /// aliyun httpdns.
    ZegoHttpDNSTypeAliyun = 3
};

/// Publish or play stream event
typedef NS_ENUM(NSUInteger, ZegoStreamEvent) {
    /// Start publishing stream
    ZegoStreamEventPublishStart = 100,
    /// The first publish stream was successful
    ZegoStreamEventPublishSuccess = 101,
    /// Failed to publish stream for the first time
    ZegoStreamEventPublishFail = 102,
    /// Start retrying publishing stream
    ZegoStreamEventRetryPublishStart = 103,
    /// Retry publishing stream successfully
    ZegoStreamEventRetryPublishSuccess = 104,
    /// Failed to retry publishing stream
    ZegoStreamEventRetryPublishFail = 105,
    /// End of publishing stream
    ZegoStreamEventPublishEnd = 106,
    /// Start playing stream
    ZegoStreamEventPlayStart = 200,
    /// The first play stream was successful
    ZegoStreamEventPlaySuccess = 201,
    /// Failed to play stream for the first time
    ZegoStreamEventPlayFail = 202,
    /// Start retrying playing stream
    ZegoStreamEventRetryPlayStart = 203,
    /// Retry playing stream successfully
    ZegoStreamEventRetryPlaySuccess = 204,
    /// Failed to retry playing stream
    ZegoStreamEventRetryPlayFail = 205,
    /// End of playing stream
    ZegoStreamEventPlayEnd = 206
};

/// Type of capture target.
typedef NS_ENUM(NSUInteger, ZegoScreenCaptureSourceType) {
    /// Unknow.
    ZegoScreenCaptureSourceTypeUnknow = 0,
    /// The capture target is a certain window.
    ZegoScreenCaptureSourceTypeWindow = 1,
    /// The capture target is the screen of a certain monitor.
    ZegoScreenCaptureSourceTypeScreen = 2
};

/// Low light enhanced mode.
typedef NS_ENUM(NSUInteger, ZegoLowlightEnhancementMode) {
    /// Turn off the low-light enhancement, and the brightness of the captured image remains unchanged.
    ZegoLowlightEnhancementModeOff = 0,
    /// Turn on low-light enhancement to enhance the brightness of the captured image.
    ZegoLowlightEnhancementModeOn = 1,
    /// Automatic low-light enhancement, the brightness enhancement value of the captured picture is dynamically adjusted with the ambient brightness.
    ZegoLowlightEnhancementModeAuto = 2
};

/// Low light enhanced type.
typedef NS_ENUM(NSUInteger, ZegoExpLowlightEnhancementType) {
    /// Normal low light enhancement.
    ZegoExpLowlightEnhancementTypeNormal = 0,
    /// AI low light enhancement. If you want to use this function, contact ZEGO technical support.
    ZegoExpLowlightEnhancementTypeAI = 1
};

/// Video denoise mode.
typedef NS_ENUM(NSUInteger, ZegoVideoDenoiseMode) {
    /// Turn off video denoise.
    ZegoVideoDenoiseModeOff = 0,
    /// Turn on video denoise.
    ZegoVideoDenoiseModeOn = 1,
    /// Automatic video noise reduction, where the SDK internally determines whether to enable noise reduction based on the level of noise in the captured footage.
    ZegoVideoDenoiseModeAuto = 2
};

/// Video denoise strength.
typedef NS_ENUM(NSUInteger, ZegoVideoDenoiseStrength) {
    /// Light denoise strength.
    ZegoVideoDenoiseStrengthLight = 1,
    /// Medium denoise strength.
    ZegoVideoDenoiseStrengthMedium = 2,
    /// Heavy denoise strength.
    ZegoVideoDenoiseStrengthHeavy = 3
};

/// Range scene state change reason.
typedef NS_ENUM(NSUInteger, ZegoSceneState) {
    /// Logging in to the scene. When calling [loginScene] to log in to the scene, it will enter this state, indicating that it is requesting to connect to the server. The application interface is usually displayed through this state.
    ZegoSceneStateLogining = 0,
    /// Log in to the scene successfully. When the scene is successfully logged in, it will enter this state, indicating that the login to the scene has been successful, and users can normally use the range scene capabilities.
    ZegoSceneStateLogined = 1,
    /// Failed to log in to the scene. When the login fails, it will enter this state, indicating that the login scene has failed, for example, AppID or Token is incorrect, etc.
    ZegoSceneStateLoginFailed = 2,
    /// The scene connection is temporarily interrupted. If the interruption occurs due to poor network quality, the SDK will retry internally.
    ZegoSceneStateReconnecting = 3,
    /// The scene is successfully reconnected. If there is an interruption due to poor network quality, the SDK will retry internally, and enter this state after successful reconnection.
    ZegoSceneStateReconnected = 4,
    /// The scene fails to reconnect. If there is an interruption due to poor network quality, the SDK will retry internally, and enter this state after the reconnection fails.
    ZegoSceneStateReconnectFailed = 5,
    /// Kicked out of the scene by the server. For example, if you log in to the scene with the same user name in other places, and the local end is kicked out of the scene, it will enter this state.
    ZegoSceneStateKickOut = 6,
    /// Logout of the scene is successful. It is in this state by default before logging into the scene. When calling [logoutScene] to log out of the scene successfully, it will enter this state.
    ZegoSceneStateLogout = 7
};

/// Stream state.
typedef NS_ENUM(NSUInteger, ZegoStreamState) {
    /// Stop playing stream.
    ZegoStreamStateStoped = 0,
    /// Start request playing stream.
    ZegoStreamStateStarting = 1,
    /// Start playing stream.
    ZegoStreamStateStarted = 2
};

/// Device state.
typedef NS_ENUM(NSUInteger, ZegoDeviceState) {
    /// Device off.
    ZegoDeviceStateClose = 0,
    /// Device on.
    ZegoDeviceStateOpen = 1
};

/// Broadcast user's own information mode.
typedef NS_ENUM(NSUInteger, ZegoBroadcastMode) {
    /// Do not broadcast user's own information.
    ZegoBroadcastModeNone = 0,
    /// Broadcast user's own information.
    ZegoBroadcastModeAll = 1
};

/// Team state.
typedef NS_ENUM(NSUInteger, ZegoTeamState) {
    /// Joining the team. When calling [joinTeam] to join the team, it will enter this state, indicating that it is requesting to connect to the server. The application interface is usually displayed through this state.
    ZegoTeamStateLogining = 0,
    /// Join the room successfully. When joined the team successfully, it will enter this state.
    ZegoTeamStateLogined = 1,
    /// Failed to join the room. When the join team fails, it will enter this state.
    ZegoTeamStateLoginFailed = 2,
    /// The team connection is temporarily interrupted. If the interruption occurs due to poor network quality, the SDK will retry internally.
    ZegoTeamStateReconnecting = 3,
    /// The team connection is successfully reconnected. If there is an interruption due to poor network quality, the SDK will retry internally, and enter this state after successful reconnection.
    ZegoTeamStateReconnected = 4,
    /// The team connection fails to reconnect. If there is an interruption due to poor network quality, the SDK will retry internally, and enter this state after the reconnection fails.
    ZegoTeamStateReconnectFailed = 5,
    /// Kicked out of the team by the server. For example, if you join the team with the same user name in other places, and the local end is kicked out of the team, it will enter this state.
    ZegoTeamStateKickOut = 6,
    /// Leave the team is successful. It is in this state by default before join the team. When calling [leaveTeam] to leave the team successfully, it will enter this state.
    ZegoTeamStateLogout = 7
};

/// The mode of create item.
typedef NS_ENUM(NSUInteger, ZegoCreateItemMode) {
    /// Do not bind user after item is created.
    ZegoCreateItemModeNoBind = 0,
    /// Bind the user after creating the item.
    ZegoCreateItemModeBind = 1
};

/// Super resolution mode.
typedef NS_ENUM(NSUInteger, ZegoSuperResolutionState) {
    /// Super resolution turned off.
    ZegoSuperResolutionStateOff = 0,
    /// Super resolution turned on.
    ZegoSuperResolutionStateOn = 1
};

/// Video config preference.
typedef NS_ENUM(NSUInteger, ZegoVideoConfigPreference) {
    ZegoVideoConfigPreferenceSmooth = 0,
    ZegoVideoConfigPreferenceClear = 1,
    ZegoVideoConfigPreferenceBalanced = 2
};

/// video capture source.
typedef NS_ENUM(NSUInteger, ZegoVideoSourceType) {
    /// No capture, i.e. no video data.
    ZegoVideoSourceTypeNone = 1,
    /// The video source comes from the camera (main channel default, and front camera is captured by default). The default is front camera, which can be adjusted to rear via [useFrontCamera].
    ZegoVideoSourceTypeCamera = 2,
    /// Video source from custom capture.
    ZegoVideoSourceTypeCustom = 3,
    /// Video source from the main publish channel. When publishing the main channel, this value cannot be set.
    ZegoVideoSourceTypeMainPublishChannel = 4,
    /// Video source from media player.
    ZegoVideoSourceTypePlayer = 5,
    /// Video source from screen capture.
    ZegoVideoSourceTypeScreenCapture = 6,
    /// @deprecated Which video source to be used will determined by SDK
    ZegoVideoSourceDefault DEPRECATED_ATTRIBUTE = 7,
    /// @deprecated Same as [None], that is, no capture
    ZegoVideoSourceNone DEPRECATED_ATTRIBUTE = 8,
    /// @deprecated Same as [Camera], that is, video source from camera
    ZegoVideoSourceCamera DEPRECATED_ATTRIBUTE = 9,
    /// @deprecated Same as [Custom], that is, video source from custom capture
    ZegoVideoSourceExternalCapture DEPRECATED_ATTRIBUTE = 10,
    /// @deprecated Same as [MainPublishChannel], that is, video source from the main publish channel
    ZegoVideoSourceMainPublishChannel DEPRECATED_ATTRIBUTE = 11,
    /// @deprecated Same as [Player], that is, video source from media player
    ZegoVideoSourcePlayer DEPRECATED_ATTRIBUTE = 12,
    /// @deprecated Same as [ScreenCapture], that is, video source from screen capture
    ZegoVideoSourceScreenCapture DEPRECATED_ATTRIBUTE = 13,
    /// Video source from secondary camera, the rear camera when [useFrontCamera] is set to true, otherwise the front camera, only support iOS.
    ZegoVideoSourceTypeSecondaryCamera = 14
};

/// Screen capture source exception type.
typedef NS_ENUM(NSUInteger, ZegoScreenCaptureSourceExceptionType) {
    /// Unknown exception type.
    ZegoScreenCaptureSourceExceptionTypeUnknown = 0,
    /// The capture target fails, such as the monitor is unplugged and the window is closed.
    ZegoScreenCaptureSourceExceptionTypeInvalid = 1,
    /// Failed to collect target, internal reasons of the system.
    ZegoScreenCaptureSourceExceptionTypeFailed = 2,
    /// The set publish region is invalid, and the publish region is not within the capture screen region.
    ZegoScreenCaptureSourceExceptionTypePublishRegionInvalid = 3
};

/// Reasons attached to the end of the screen capture extension.
typedef NS_ENUM(NSUInteger, ZegoReplayKitExtReason) {
    /// The host application actively stops screen capture.
    ZegoReplayKitExtReasonHostStop = 0,
    /// The connection to the host application fails, and the screen capture function must be enabled by calling [startScreenCapture] in the host application.
    ZegoReplayKitExtReasonConnectFail = 1,
    /// Disconnect with the host application.
    ZegoReplayKitExtReasonDisconnect = 2
};

/// The state of the screen capture source window changes.
typedef NS_ENUM(NSUInteger, ZegoScreenCaptureWindowState) {
    /// The window is on the current screen, and the coordinate area changes.
    ZegoScreenCaptureWindowStateOnScreen = 0,
    /// The window leaves the current screen and pauses the capture.
    ZegoScreenCaptureWindowStateOffScreen = 1,
    /// The window is destroy.
    ZegoScreenCaptureWindowStateDestroy = 2
};

/// Multimedia load type.
typedef NS_ENUM(NSUInteger, ZegoMultimediaLoadType) {
    /// Load by file path.
    ZegoMultimediaLoadTypeFilePath = 0,
    /// Load by memory.
    ZegoMultimediaLoadTypeMemory = 1,
    /// Load by copyrighted music resource ID.
    ZegoMultimediaLoadTypeResourceID = 2
};

/// Alpha channel data layout.
typedef NS_ENUM(NSUInteger, ZegoAlphaLayoutType) {
    /// There is no alpha data.
    ZegoAlphaLayoutTypeNone = 0,
    /// Alpha channel data is to the left of RGB/YUV data.
    ZegoAlphaLayoutTypeLeft = 1,
    /// Alpha channel data is to the right of RGB/YUV data.
    ZegoAlphaLayoutTypeRight = 2,
    /// Alpha channel data is to the bottom of RGB/YUV data.
    ZegoAlphaLayoutTypeBottom = 3,
    /// Alpha channel data is to the upper right of RGB/YUV data.
    ZegoAlphaLayoutTypeRightTop = 4
};

/// Object segmentation type.
typedef NS_ENUM(NSUInteger, ZegoObjectSegmentationType) {
    /// Any background object segmentation.
    ZegoObjectSegmentationTypeAnyBackground = 0,
    /// Green screen background object segmentation.
    ZegoObjectSegmentationTypeGreenScreenBackground = 1
};

/// Object segmentation state.
typedef NS_ENUM(NSUInteger, ZegoObjectSegmentationState) {
    /// Object segmentation turned off.
    ZegoObjectSegmentationStateOff = 0,
    /// Object segmentation turned on.
    ZegoObjectSegmentationStateOn = 1
};

/// Video background process type.
typedef NS_ENUM(NSUInteger, ZegoBackgroundProcessType) {
    /// Background is transparent.
    ZegoBackgroundProcessTypeTransparent = 0,
    /// Fill the background with a solid color.
    ZegoBackgroundProcessTypeColor = 1,
    /// Blur background.
    ZegoBackgroundProcessTypeBlur = 2,
    /// The background is the specified image.
    ZegoBackgroundProcessTypeImage = 3,
    /// The background is the specified video.
    ZegoBackgroundProcessTypeVideo = 4
};

/// Background blur level.
typedef NS_ENUM(NSUInteger, ZegoBackgroundBlurLevel) {
    /// Background blur level low.
    ZegoBackgroundBlurLevelLow = 0,
    /// Background blur level medium.
    ZegoBackgroundBlurLevelMedium = 1,
    /// Background blur level high.
    ZegoBackgroundBlurLevelHigh = 2
};

/// The config of the media data publisher.
typedef NS_ENUM(NSUInteger, ZegoMediaDataPublisherMode) {
    /// Both publish video and audio.
    ZegoMediaDataPublisherModeBothVideoAndAudio = 0,
    /// Only publish video.
    ZegoMediaDataPublisherModeOnlyVideo = 1
};

/// Live audio effect mode.
typedef NS_ENUM(NSUInteger, ZegoLiveAudioEffectMode) {
    /// No audio effect.
    ZegoLiveAudioEffectModeNone = 0,
    /// Only local play.
    ZegoLiveAudioEffectModeLocal = 1,
    /// Only publish.
    ZegoLiveAudioEffectModePublish = 2,
    /// Publish and local play.
    ZegoLiveAudioEffectModeAll = 3
};

/// AI voice changer event.
typedef NS_ENUM(NSUInteger, ZegoAIVoiceChangerEvent) {
    /// Succeed.
    ZegoAIVoiceChangerEventSucceed = 0,
    /// Background performance insufficient.
    ZegoAIVoiceChangerEventBackgroundPerformanceInsufficient = 1,
    /// Foreground performance insufficient.
    ZegoAIVoiceChangerEventForegroundPerformanceInsufficient = 2,
    /// Others.
    ZegoAIVoiceChangerEventOthers = 3,
    /// Set speaker failed.
    ZegoAIVoiceChangerEventSetSpeakerFailed = 10
};

/// Media stream type.
typedef NS_ENUM(NSUInteger, ZegoMediaStreamType) {
    /// Media audio stream.
    ZegoMediaStreamTypeAudio = 0,
    /// Media video stream.
    ZegoMediaStreamTypeVideo = 1,
    /// Media audio and video stream.
    ZegoMediaStreamTypeAV = 2
};

/// Dump data type.
typedef NS_ENUM(NSUInteger, ZegoDumpDataType) {
    /// Audio.
    ZegoDumpDataTypeAudio = 0
};

/// Usage of Custom video processed data.
typedef NS_ENUM(NSUInteger, ZegoProcessedDataUsageType) {
    /// Preview.
    ZegoProcessedDataUsageTypePreview = 0,
    /// Publish.
    ZegoProcessedDataUsageTypePublish = 1,
    /// Both preview and publish.
    ZegoProcessedDataUsageTypeBoth = 2
};

/// Dummy capture image mode.
typedef NS_ENUM(NSUInteger, ZegoDummyCaptureImageMode) {
    /// Manual mode. The user needs to call the [EnableCamera] interface to turn off camera capture, and the SDK will use dummy capture image.
    ZegoDummyCaptureImageModeManual = 0,
    /// Auto mode. After the SDK detects that the camera is unavailable, it uses dummy capture image to puublish the stream.
    ZegoDummyCaptureImageModeAuto = 1
};

/// Log config.
///
/// Description: This parameter is required when calling [setlogconfig] to customize log configuration.
/// Use cases: This configuration is required when you need to customize the log storage path or the maximum log file size.
/// Caution: None.
@interface ZegoLogConfig : NSObject

/// The storage path of the log file. Description: Used to customize the storage path of the log file. Use cases: This configuration is required when you need to customize the log storage path. Required: False. Default value: The default path of each platform is different, please refer to the official website document https://docs.zegocloud.com/faq/express_sdkLog. Caution: Developers need to ensure read and write permissions for files under this path.
@property (nonatomic, copy) NSString *logPath;

/// Maximum log file size(Bytes). Description: Used to customize the maximum log file size. Use cases: This configuration is required when you need to customize the upper limit of the log file size. Required: False. Default value: 5MB (5 * 1024 * 1024 Bytes). Value range: Minimum 1MB (1 * 1024 * 1024 Bytes), maximum 100M (100 * 1024 * 1024 Bytes), 0 means no need to write logs. Caution: The larger the upper limit of the log file size, the more log information it carries, but the log upload time will be longer.
@property (nonatomic, assign) unsigned long long logSize;

/// Log files count. Default is 3. Value range is [3, 20].
@property (nonatomic, assign) unsigned int logCount;

@end

/// Custom video capture configuration.
///
/// Custom video capture, that is, the developer is responsible for collecting video data and sending the collected video data to SDK for video data encoding and publishing to the ZEGO RTC server. This feature is generally used by developers who use third-party beauty features or record game screen living.
/// When you need to use the custom video capture function, you need to set an instance of this class as a parameter to the [enableCustomVideoCapture] function.
/// Because when using custom video capture, SDK will no longer start the camera to capture video data. You need to collect video data from video sources by yourself.
@interface ZegoCustomVideoCaptureConfig : NSObject

/// Custom video capture video frame data type
@property (nonatomic, assign) ZegoVideoBufferType bufferType;

@end

/// Custom video process configuration.
@interface ZegoCustomVideoProcessConfig : NSObject

/// Custom video process video frame data type. The default value is [ZegoVideoBufferTypeCVPixelBuffer].
@property (nonatomic, assign) ZegoVideoBufferType bufferType;

@end

/// Custom video render configuration.
///
/// When you need to use the custom video render function, you need to set an instance of this class as a parameter to the [enableCustomVideoRender] function.
@interface ZegoCustomVideoRenderConfig : NSObject

/// Custom video capture video frame data type
@property (nonatomic, assign) ZegoVideoBufferType bufferType;

/// Custom video rendering video frame data format。Useless when set bufferType as [EncodedData]
@property (nonatomic, assign) ZegoVideoFrameFormatSeries frameFormatSeries;

/// Whether the engine also renders while customizing video rendering. The default value is [false]. Useless when set bufferType as [EncodedData]
@property (nonatomic, assign) BOOL enableEngineRender;

@end

/// Custom audio configuration.
@interface ZegoCustomAudioConfig : NSObject

/// Audio capture source type
@property (nonatomic, assign) ZegoAudioSourceType sourceType;

@end

/// Profile for create engine
///
/// Profile for create engine
@interface ZegoEngineProfile : NSObject

/// Application ID issued by ZEGO for developers, please apply from the ZEGO Admin Console https://console.zegocloud.com The value ranges from 0 to 4294967295.
@property (nonatomic, assign) unsigned int appID;

/// Application signature for each AppID, please apply from the ZEGO Admin Console. Application signature is a 64 character string. Each character has a range of '0' ~ '9', 'a' ~ 'z'. AppSign 2.17.0 and later allows null or no transmission. If the token is passed empty or not passed, the token must be entered in the [ZegoRoomConfig] parameter for authentication when the [loginRoom] interface is called to login to the room.
@property (nonatomic, strong) NSString *appSign;

/// The room scenario. the SDK will optimize the audio and video configuration for the specified scenario to achieve the best effect in this scenario. After specifying the scenario, you can call other APIs to adjusting the audio and video configuration. Differences between scenarios and how to choose a suitable scenario, please refer to https://docs.zegocloud.com/article/14940
@property (nonatomic, assign) ZegoScenario scenario;

@end

/// Advanced engine configuration.
@interface ZegoEngineConfig : NSObject

/// @deprecated This property has been deprecated since version 2.3.0, please use the [setLogConfig] function instead.
@property (nonatomic, strong, nullable) ZegoLogConfig *logConfig DEPRECATED_ATTRIBUTE;

/// Other special function switches, if not set, no special function will be used by default. Please contact ZEGO technical support before use.
@property (nonatomic, copy, nullable) NSDictionary<NSString *, NSString *> *advancedConfig;

@end

/// Proxy config.
///
/// Set proxy config.
@interface ZegoProxyInfo : NSObject

/// ip. Under local proxy: local proxy ip configured by the developer. under cloud proxy: proxy ip provided by ZEGO (either  hostname or ip will do).
@property (nonatomic, copy) NSString *ip;

/// port. Under Local Proxy: Local proxy port configured by the developer. Under Cloud Proxy: Proxy port provided by ZEGO.
@property (nonatomic, assign) unsigned int port;

/// hostname.Under Local Proxy: Local proxy hostname configured by the developer. Under cloud proxy: proxy hostname provided by ZEGO. proxy hostname (either hostname or ip will do).
@property (nonatomic, copy) NSString *hostName;

/// userName.Under Local Proxy: the authentication username of the local Proxy configured by the developer, can be ignored if there is none. Under cloud proxy: can be ignored.
@property (nonatomic, copy) NSString *userName;

/// password.Under local proxy: authentication password of local proxy configured by developer, can be ignored if there is none. Under cloud proxy: can be ignored.
@property (nonatomic, copy) NSString *password;

@end

/// Advanced room configuration.
///
/// Configure maximum number of users in the room and authentication token, etc.
@interface ZegoRoomConfig : NSObject

/// The maximum number of users in the room, Passing 0 means unlimited, the default is unlimited.
@property (nonatomic, assign) unsigned int maxMemberCount;

/// Whether to enable the user in and out of the room callback notification [onRoomUserUpdate], the default is off. If developers need to use ZEGO Room user notifications, make sure that each user who login sets this flag to true
@property (nonatomic, assign) BOOL isUserStatusNotify;

/// The token issued by the developer's business server is used to ensure security. For the generation rules, please refer to [Using Token Authentication](https://doc-zh.zego.im/article/10360), the default is an empty string, that is, no authentication. In versions 2.17.0 and above, if appSign is not passed in when calling the [createEngine] API to create an engine, or if appSign is empty, this parameter must be set for authentication when logging in to a room.
@property (nonatomic, copy) NSString *token;

/// The bitmask marker for capability negotiation, refer to enum [ZegoRoomCapabilityNegotiationTypesBitMask], when this param converted to binary, 0b01 that means 1 << 0 for enable the capability negotiation of all user in the room, 0x10 that means 1 << 1 for enable the capability negotiation of publisher in the room. The masks can be combined to allow different types of capability negotiation.
@property (nonatomic, assign) unsigned int capabilityNegotiationTypes;

/// Create a default room configuration
///
/// The default configuration parameters are: the maximum number of users in the room is unlimited, the user will not be notified when the user enters or leaves the room, no authentication.
///
/// @return ZegoRoomConfig instance
+ (instancetype)defaultConfig;

@end

/// Video config.
///
/// Configure parameters used for publishing stream, such as bitrate, frame rate, and resolution.
/// Developers should note that the width and height resolution of the mobile and desktop are opposite. For example, 360p, the resolution of the mobile is 360x640, and the desktop is 640x360.
/// When using external capture, the capture and encoding resolution of RTC cannot be set to 0*0, otherwise, there will be no video data in the publishing stream in the entire engine life cycle.
@interface ZegoVideoConfig : NSObject

/// Capture resolution, control the resolution of camera image acquisition. SDK requires the width and height to be set to even numbers. Only the camera is not started and the custom video capture is not used, the setting is effective. For performance reasons, the SDK scales the video frame to the encoding resolution after capturing from camera and before rendering to the preview view. Therefore, the resolution of the preview image is the encoding resolution. If you need the resolution of the preview image to be this value, Please call [setCapturePipelineScaleMode] first to change the capture pipeline scale mode to [Post]
@property (nonatomic, assign) CGSize captureResolution;

/// Encode resolution, control the image resolution of the encoder when publishing stream. SDK requires the width and height to be set to even numbers. The settings before and after publishing stream can be effective
@property (nonatomic, assign) CGSize encodeResolution;

/// Frame rate, control the frame rate of the camera and the frame rate of the encoder. Publishing stream set to 60 fps, playing stream to take effect need contact technical support
@property (nonatomic, assign) int fps;

/// Bit rate in kbps. The settings before and after publishing stream can be effective. The SDK will automatically set the bit rate suitable for the scenario selected by the developer. If the bit rate manually set by the developer exceeds the reasonable range, the SDK will automatically process the bit rate according to the reasonable range. If you need to configure a high bit rate due to business needs, please contact ZEGO Business.
@property (nonatomic, assign) int bitrate;

/// The codec id to be used, the default value is [default]. Settings only take effect before publishing stream
@property (nonatomic, assign) ZegoVideoCodecID codecID;

/// Video keyframe interval, in seconds. Description: Required: No. Default value: 2 seconds. Value range: [2, 5]. Caution: The setting is only valid before pushing.
@property (nonatomic, assign) int keyFrameInterval;

/// Create default video configuration(360p, 15fps, 600kbps)
///
/// 360p, 15fps, 600kbps
///
/// @return ZegoVideoConfig instance
+ (instancetype)defaultConfig;

/// Create video configuration with preset enumeration values
///
/// @return ZegoVideoConfig instance
+ (instancetype)configWithPreset:(ZegoVideoConfigPreset)preset;

/// Create video configuration with preset enumeration values
///
/// @return ZegoVideoConfig instance
- (instancetype)initWithPreset:(ZegoVideoConfigPreset)preset;

@end

/// Externally encoded data traffic control information.
@interface ZegoTrafficControlInfo : NSObject

/// Video FPS to be adjusted
@property (nonatomic, assign) int fps;

/// Video bitrate in kbps to be adjusted
@property (nonatomic, assign) int bitrate;

/// Video resolution to be adjusted
@property (nonatomic, assign) CGSize resolution;

@end

/// Dual stream info.
@interface ZegoPublishDualStreamConfig : NSObject

/// streamType
@property ZegoVideoStreamType streamType;

/// Video FPS to be adjusted
@property (nonatomic, assign) int fps;

/// Video bitrate in kbps to be adjusted
@property (nonatomic, assign) int bitrate;

/// Video resolution to be adjusted
@property (nonatomic, assign) CGSize encodeResolution;

@end

/// SEI configuration
///
/// Used to set the relevant configuration of the Supplemental Enhancement Information.
@interface ZegoSEIConfig : NSObject

/// SEI type
@property (nonatomic, assign) ZegoSEIType type;

/// Create a default SEI config object
///
/// @return ZegoSEIConfig instance
+ (instancetype)defaultConfig;

@end

/// Voice changer parameter.
///
/// Developer can use the built-in presets of the SDK to change the parameters of the voice changer.
@interface ZegoVoiceChangerParam : NSObject

/// Pitch parameter, value range [-12.0, 12.0], the larger the value, the sharper the sound, set it to 0.0 to turn off. Note that on v2.18.0 and older version, the value range is [-8.0, 8.0].
@property (nonatomic, assign) float pitch;

@end

/// Audio reverberation parameters.
///
/// Developers can use the SDK's built-in presets to change the parameters of the reverb.
@interface ZegoReverbParam : NSObject

/// Room size, in the range [0.0, 1.0], to control the size of the "room" in which the reverb is generated, the larger the room, the stronger the reverb.
@property (nonatomic, assign) float roomSize;

/// Echo, in the range [0.0, 0.5], to control the trailing length of the reverb.
@property (nonatomic, assign) float reverberance;

/// Reverb Damping, range [0.0, 2.0], controls the attenuation of the reverb, the higher the damping, the higher the attenuation.
@property (nonatomic, assign) float damping;

/// Dry/wet ratio, the range is greater than or equal to 0.0, to control the ratio between reverberation, direct sound and early reflections; dry part is set to 1 by default; the smaller the dry/wet ratio, the larger the wet ratio, the stronger the reverberation effect.
@property (nonatomic, assign) float dryWetRatio;

@end

/// Audio reverberation advanced parameters.
///
/// Developers can use the SDK's built-in presets to change the parameters of the reverb.
@interface ZegoReverbAdvancedParam : NSObject

/// Room size(%), in the range [0.0, 1.0], to control the size of the "room" in which the reverb is generated, the larger the room, the stronger the reverb.
@property (nonatomic, assign) float roomSize;

/// Echo(%), in the range [0.0, 100.0], to control the trailing length of the reverb.
@property (nonatomic, assign) float reverberance;

/// Reverb Damping(%), range [0.0, 100.0], controls the attenuation of the reverb, the higher the damping, the higher the attenuation.
@property (nonatomic, assign) float damping;

/// only wet
@property (nonatomic, assign) BOOL wetOnly;

/// wet gain(dB), range [-20.0, 10.0]
@property (nonatomic, assign) float wetGain;

/// dry gain(dB), range [-20.0, 10.0]
@property (nonatomic, assign) float dryGain;

/// Tone Low. 100% by default
@property (nonatomic, assign) float toneLow;

/// Tone High. 100% by default
@property (nonatomic, assign) float toneHigh;

/// PreDelay(ms), range [0.0, 200.0]
@property (nonatomic, assign) float preDelay;

/// Stereo Width(%). 0% by default
@property (nonatomic, assign) float stereoWidth;

@end

/// Audio reverberation echo parameters.
@interface ZegoReverbEchoParam : NSObject

/// Gain of input audio signal, in the range [0.0, 1.0]
@property (nonatomic, assign) float inGain;

/// Gain of output audio signal, in the range [0.0, 1.0]
@property (nonatomic, assign) float outGain;

/// Number of echos, in the range [0, 7]
@property (nonatomic, assign) int numDelays;

/// Respective delay of echo signal, in milliseconds, in the range [0, 5000] ms
@property (nonatomic, copy) NSArray<NSNumber *> *delay;

/// Respective decay coefficient of echo signal, in the range [0.0, 1.0]
@property (nonatomic, copy) NSArray<NSNumber *> *decay;

@end

/// User object.
///
/// Configure user ID and username to identify users in the room.
/// Note that the userID must be unique under the same appID, otherwise, there will be mutual kicks when logging in to the room.
/// It is strongly recommended that userID corresponds to the user ID of the business APP, that is, a userID and a real user are fixed and unique, and should not be passed to the SDK in a random userID. Because the unique and fixed userID allows ZEGO technicians to quickly locate online problems.
@interface ZegoUser : NSObject

/// User ID, a utf8 string with a maximum length of 64 bytes or less.Privacy reminder: Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.Caution: Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'. Do not use '%' if you need to communicate with the Web SDK.
@property (nonatomic, copy) NSString *userID;

/// User Name, a utf8 string with a maximum length of 256 bytes or less.Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.
@property (nonatomic, copy) NSString *userName;

/// Create a ZegoUser object
///
/// userName and userID are set to match
///
/// @return ZegoUser instance
+ (instancetype)userWithUserID:(NSString *)userID;

/// Create a ZegoUser object
///
/// userName and userID are set to match
- (instancetype)initWithUserID:(NSString *)userID;

/// Create a ZegoUser object
///
/// @return ZegoUser instance
+ (instancetype)userWithUserID:(NSString *)userID userName:(NSString *)userName;

/// Create a ZegoUser object
///
/// @return ZegoUser instance
- (instancetype)initWithUserID:(NSString *)userID userName:(NSString *)userName;

@end

/// Stream object.
///
/// Identify an stream object
@interface ZegoStream : NSObject

/// User object instance.Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.
@property (nonatomic, strong) ZegoUser *user;

/// Stream ID, a string of up to 256 characters. Caution: You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '-', '_'.
@property (nonatomic, copy) NSString *streamID;

/// Stream extra info
@property (nonatomic, copy) NSString *extraInfo;

@end

/// Room stream list.
///
/// Room stream list.
@interface ZegoRoomStreamList : NSObject

/// Publish stream list
@property (nonatomic, strong) NSArray<ZegoStream *> *publishStreamList;

/// Play stream list
@property (nonatomic, strong) NSArray<ZegoStream *> *playStreamList;

@end

/// Room extra information.
@interface ZegoRoomExtraInfo : NSObject

/// The key of the room extra information.
@property (nonatomic, copy) NSString *key;

/// The value of the room extra information.
@property (nonatomic, copy) NSString *value;

/// The user who update the room extra information.Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.
@property (nonatomic, strong) ZegoUser *updateUser;

/// Update time of the room extra information, UNIX timestamp, in milliseconds.
@property (nonatomic, assign) unsigned long long updateTime;

@end

/// Coordinates used by the ROI function.
@interface ZegoRoiRect : NSObject

/// The horizontal offset from the top-left corner
@property (nonatomic, assign) int x;

/// The vertical offset from the top-left corner
@property (nonatomic, assign) int y;

/// The width of the rectangle
@property (nonatomic, assign) int width;

/// The height of the rectangle
@property (nonatomic, assign) int height;

/// ROI strength, currently supported value range is [0, 4], 0 is no effect, 4 is the strongest.
@property (nonatomic, assign) int strength;

@end

/// Position information used by the face detection.
@interface ZegoFacePositionInfo : NSObject

/// Coordinates used by the face detection.
@property (nonatomic, assign) CGRect position;

@end

/// View object.
///
/// Configure view object, view Mode, background color
@interface ZegoCanvas : NSObject

/// View object
@property (nonatomic, strong) ZGView *view;

/// View mode, default is ZegoViewModeAspectFit
@property (nonatomic, assign) ZegoViewMode viewMode;

/// Background color, the format is 0xRRGGBB, default is black, which is 0x000000
@property (nonatomic, assign) int backgroundColor;

/// If enable alpha blend render, default is false.
@property (nonatomic, assign) BOOL alphaBlend;

/// Create a ZegoCanvas, default viewMode is ZegoViewModeAspectFit, default background color is black
///
/// @return ZegoCanvas instance
+ (instancetype)canvasWithView:(ZGView *)view;

/// Create a ZegoCanvas, default viewMode is ZegoViewModeAspectFit, default background color is black
///
/// @return ZegoCanvas instance
- (instancetype)initWithView:(ZGView *)view;

@end

/// Advanced publisher configuration.
///
/// Configure room id
@interface ZegoPublisherConfig : NSObject

/// The Room ID, It is not necessary to pass in single room mode, but the ID of the corresponding room must be passed in multi-room mode
@property (nonatomic, copy) NSString *roomID;

/// Whether to synchronize the network time when pushing streams. 1 is synchronized with 0 is not synchronized. And must be used with setStreamAlignmentProperty. It is used to align multiple streams at the mixed stream service or streaming end, such as the chorus scene of KTV.
@property (nonatomic, assign) int forceSynchronousNetworkTime;

/// When pushing a flow, review the pattern of the flow. By default, no audit is performed. If you want to use this function, contact ZEGO technical support.
@property (nonatomic, assign) ZegoStreamCensorshipMode streamCensorshipMode;

/// Inspect flag. If you want to use this function, contact ZEGO technical support.
@property (nonatomic, assign) int streamCensorFlag;

/// Codec capability negotiation type. By default, no reference to the outcome of the capability negotiation. If you want to use this function, contact ZEGO technical support.
@property (nonatomic, assign) ZegoCapabilityNegotiationType codecNegotiationType;

/// Stream title, a utf8 string with a maximum length of 255 bytes or less.
@property (nonatomic, copy) NSString *streamTitle;

@end

/// Advanced scene publisher configuration.
@interface ZegoScenePublisherConfig : NSObject

/// Range scene handle. Obtained through [getHandle] of ZegoRangeScene instance.
@property (nonatomic, assign) int rangeSceneHandle;

@end

/// Published stream quality information.
///
/// Audio and video parameters and network quality, etc.
@interface ZegoPublishStreamQuality : NSObject

/// Video capture frame rate. The unit of frame rate is f/s
@property (nonatomic, assign) double videoCaptureFPS;

/// Video encoding frame rate. The unit of frame rate is f/s
@property (nonatomic, assign) double videoEncodeFPS;

/// Video transmission frame rate. The unit of frame rate is f/s
@property (nonatomic, assign) double videoSendFPS;

/// Video bit rate in kbps
@property (nonatomic, assign) double videoKBPS;

/// Audio capture frame rate. The unit of frame rate is f/s
@property (nonatomic, assign) double audioCaptureFPS;

/// Audio transmission frame rate. The unit of frame rate is f/s
@property (nonatomic, assign) double audioSendFPS;

/// Audio bit rate in kbps
@property (nonatomic, assign) double audioKBPS;

/// Local to server delay, in milliseconds
@property (nonatomic, assign) int rtt;

/// Packet loss rate, in percentage, 0.0 ~ 1.0
@property (nonatomic, assign) double packetLostRate;

/// Published stream quality level
@property (nonatomic, assign) ZegoStreamQualityLevel level;

/// Whether to enable hardware encoding
@property (nonatomic, assign) BOOL isHardwareEncode;

/// Video codec ID (Available since 1.17.0)
@property (nonatomic, assign) ZegoVideoCodecID videoCodecID;

/// Total number of bytes sent, including audio, video, SEI
@property (nonatomic, assign) double totalSendBytes;

/// Number of audio bytes sent
@property (nonatomic, assign) double audioSendBytes;

/// Number of video bytes sent
@property (nonatomic, assign) double videoSendBytes;

@end

/// CDN config object.
///
/// Includes CDN URL and authentication parameter string
@interface ZegoCDNConfig : NSObject

/// CDN URL
@property (nonatomic, copy) NSString *url;

/// Auth param of URL. Please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored (set to null or empty string).
@property (nonatomic, copy) NSString *authParam;

/// URL supported protocols, candidate values are "tcp" and "quic". If there are more than one, separate them with English commas and try them in order. Please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored (set to null or empty string).
@property (nonatomic, copy) NSString *protocol;

/// QUIC version。 If [protocol] has the QUIC protocol, this information needs to be filled in. If there are multiple version numbers, separate them with commas. Please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored (set to null or empty string).
@property (nonatomic, copy) NSString *quicVersion;

/// customized httpdns service. This feature is only supported for playing stream currently.
@property ZegoHttpDNSType httpdns;

/// QUIC establishes link mode. If the value is 1, quic 0 rtt is used preferentially to establish link. Otherwise, the link is established normally. If [protocol] has the QUIC protocol, this value takes effect.
@property int quicConnectMode;

/// custom param of URL. Please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored (set to null or empty string).
@property (nonatomic, copy) NSString *customParams;

@end

/// Relay to CDN info.
///
/// Including the URL of the relaying CDN, relaying state, etc.
@interface ZegoStreamRelayCDNInfo : NSObject

/// URL of publishing stream to CDN
@property (nonatomic, copy) NSString *url;

/// State of relaying to CDN
@property (nonatomic, assign) ZegoStreamRelayCDNState state;

/// Reason for relay state changed
@property (nonatomic, assign) ZegoStreamRelayCDNUpdateReason updateReason;

/// The timestamp when the state changed, UNIX timestamp, in milliseconds.
@property (nonatomic, assign) unsigned long long stateTime;

@end

/// Face detection info.
///
/// Face detection info.
@interface ZegoFaceDetectionInfo : NSObject

/// The image width captured by the camera
@property (nonatomic, assign) int imageWidth;

/// The image height captured by the camera
@property (nonatomic, assign) int imageHeight;

/// Face position information list
@property (nonatomic, strong) NSArray<ZegoFacePositionInfo *> *facePositionList;

@end

/// Custom play stream resource type configuration.
///
/// Custom play stream resource type configuration.
@interface ZegoCustomPlayerResourceConfig : NSObject

/// The resource type selected by the play stream before starting the publish stream.
@property (nonatomic, assign) ZegoResourceType beforePublish;

/// The type of resource selected by the play stream in the publish stream.
@property (nonatomic, assign) ZegoResourceType publishing;

/// The type of resource selected by the play stream after stopping the publish stream.
@property (nonatomic, assign) ZegoResourceType afterPublish;

@end

/// Advanced player configuration.
///
/// Configure stream resource mode, CDN configuration and other advanced configurations.
@interface ZegoPlayerConfig : NSObject

/// Stream resource mode.
@property (nonatomic, assign) ZegoStreamResourceMode resourceMode;

/// The CDN configuration for playing stream. If set, the stream is play according to the URL instead of the streamID. After that, the streamID is only used as the ID of SDK internal callback.
@property (nonatomic, strong, nullable) ZegoCDNConfig *cdnConfig;

/// The Room ID. It only needs to be filled in the multi-room mode, which indicates which room this stream needs to be bound to. This parameter is ignored in single room mode.
@property (nonatomic, copy) NSString *roomID;

/// The video encoding type of the stream, please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored.
@property (nonatomic, assign) ZegoVideoCodecID videoCodecID;

/// The resource type of the source stream, please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored.
@property (nonatomic, assign) ZegoResourceType sourceResourceType;

/// Preconfigured codec template ID, please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored.
@property (nonatomic, assign) int codecTemplateID;

/// Play resource switching strategy mode, the default is ZegoStreamResourceSwitchModeDefault
@property (nonatomic, assign) ZegoStreamResourceSwitchMode resourceSwitchMode;

/// Play resource type when stop publish, the default is ZegoStreamResourceTypeDefault. This setting takes effect only if the user sets [resourceMode] to ZegoStreamResourceModeDefaut and [resourceSwitchMode] to ZegoStreamResourceSwitchModeDefault or ZegoStreamResourceSwitchModeSwitchToRTC.
@property (nonatomic, assign) ZegoStreamResourceType resourceWhenStopPublish;

/// Whether to enable adaptive switching of streams, 1 means on, 0 means off. Valid only if [resourceMode] is ZegoStreamResourceModeOnlyL3. Please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored.
@property (nonatomic, assign) int adaptiveSwitch;

/// Stream adaptive transcoding template ID list. Please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored.
@property (nonatomic, copy) NSArray<NSNumber *> *adaptiveTemplateIDList;

/// Play stream resource type configuration when [resourceMode] is ZegoStreamResourceModeCustom.
@property (nonatomic, strong, nullable) ZegoCustomPlayerResourceConfig *customResourceConfig;

@end

/// Advanced scene player configuration.
@interface ZegoScenePlayerConfig : NSObject

/// Range scene handle. Obtained through [getHandle] of ZegoRangeScene instance.
@property (nonatomic, assign) int rangeSceneHandle;

/// Stream resource mode.
@property (nonatomic, assign) ZegoStreamResourceMode resourceMode;

/// The CDN configuration for playing stream. If set, the stream is play according to the URL instead of the streamID. After that, the streamID is only used as the ID of SDK internal callback.
@property (nonatomic, strong, nullable) ZegoCDNConfig *cdnConfig;

@end

/// Played stream quality information.
///
/// Audio and video parameters and network quality, etc.
@interface ZegoPlayStreamQuality : NSObject

/// Video receiving frame rate. The unit of frame rate is f/s
@property (nonatomic, assign) double videoRecvFPS;

/// Video dejitter frame rate. The unit of frame rate is f/s (Available since 1.17.0)
@property (nonatomic, assign) double videoDejitterFPS;

/// Video decoding frame rate. The unit of frame rate is f/s
@property (nonatomic, assign) double videoDecodeFPS;

/// Video rendering frame rate. The unit of frame rate is f/s
@property (nonatomic, assign) double videoRenderFPS;

/// Video bit rate in kbps
@property (nonatomic, assign) double videoKBPS;

/// Video break rate, the unit is (number of breaks / every 10 seconds) (Available since 1.17.0)
@property (nonatomic, assign) double videoBreakRate;

/// Audio receiving frame rate. The unit of frame rate is f/s
@property (nonatomic, assign) double audioRecvFPS;

/// Audio dejitter frame rate. The unit of frame rate is f/s (Available since 1.17.0)
@property (nonatomic, assign) double audioDejitterFPS;

/// Audio decoding frame rate. The unit of frame rate is f/s
@property (nonatomic, assign) double audioDecodeFPS;

/// Audio rendering frame rate. The unit of frame rate is f/s
@property (nonatomic, assign) double audioRenderFPS;

/// Audio bit rate in kbps
@property (nonatomic, assign) double audioKBPS;

/// Audio break rate, the unit is (number of breaks / every 10 seconds) (Available since 1.17.0)
@property (nonatomic, assign) double audioBreakRate;

/// The audio quality of the playing stream determined by the audio MOS (Mean Opinion Score) measurement method, value range [-1, 5], where -1 means unknown, [0, 5] means valid score, the higher the score, the better the audio quality. For the subjective perception corresponding to the MOS value, please refer to https://docs.zegocloud.com/article/3720#4_4 (Available since 2.16.0)
@property (nonatomic, assign) double mos;

/// Server to local delay, in milliseconds
@property (nonatomic, assign) int rtt;

/// Packet loss rate, in percentage, 0.0 ~ 1.0
@property (nonatomic, assign) double packetLostRate;

/// Delay from peer to peer, in milliseconds
@property (nonatomic, assign) int peerToPeerDelay;

/// Packet loss rate from peer to peer, in percentage, 0.0 ~ 1.0
@property (nonatomic, assign) double peerToPeerPacketLostRate;

/// Published stream quality level
@property (nonatomic, assign) ZegoStreamQualityLevel level;

/// Delay after the data is received by the local end, in milliseconds
@property (nonatomic, assign) int delay;

/// The difference between the video timestamp and the audio timestamp, used to reflect the synchronization of audio and video, in milliseconds. This value is less than 0 means the number of milliseconds that the video leads the audio, greater than 0 means the number of milliseconds that the video lags the audio, and 0 means no difference. When the absolute value is less than 200, it can basically be regarded as synchronized audio and video, when the absolute value is greater than 200 for 10 consecutive seconds, it can be regarded as abnormal (Available since 1.19.0)
@property (nonatomic, assign) int avTimestampDiff;

/// Whether to enable hardware decoding
@property (nonatomic, assign) BOOL isHardwareDecode;

/// Video codec ID (Available since 1.17.0)
@property (nonatomic, assign) ZegoVideoCodecID videoCodecID;

/// Total number of bytes received, including audio, video, SEI
@property (nonatomic, assign) double totalRecvBytes;

/// Number of audio bytes received
@property (nonatomic, assign) double audioRecvBytes;

/// Number of video bytes received
@property (nonatomic, assign) double videoRecvBytes;

/// Accumulated audio break count (Available since 2.9.0)
@property (nonatomic, assign) unsigned int audioCumulativeBreakCount;

/// Accumulated audio break time, in milliseconds (Available since 2.9.0)
@property (nonatomic, assign) unsigned int audioCumulativeBreakTime;

/// Accumulated audio break rate, in percentage, 0.0 ~ 100.0 (Available since 2.9.0)
@property (nonatomic, assign) double audioCumulativeBreakRate;

/// Accumulated audio decode time, in milliseconds (Available since 2.9.0)
@property (nonatomic, assign) unsigned int audioCumulativeDecodeTime;

/// Accumulated video break count (Available since 2.9.0)
@property (nonatomic, assign) unsigned int videoCumulativeBreakCount;

/// Accumulated video break time, in milliseconds (Available since 2.9.0)
@property (nonatomic, assign) unsigned int videoCumulativeBreakTime;

/// Accumulated video break rate, in percentage, 0.0 ~ 1.0 (Available since 2.9.0)
@property (nonatomic, assign) double videoCumulativeBreakRate;

/// Accumulated video decode time, in milliseconds (Available since 2.9.0)
@property (nonatomic, assign) unsigned int videoCumulativeDecodeTime;

/// Mute video (Available since 3.13.0)
@property (nonatomic, assign) int muteVideo;

/// Mute audio (Available since 3.13.0)
@property (nonatomic, assign) int muteAudio;

@end

/// Cross APP playing stream configuration.
@interface ZegoCrossAppInfo : NSObject

/// AppID for playing streams across apps.
@property (nonatomic, assign) unsigned int appID;

/// The token that needs to be set.
@property (nonatomic, copy) NSString *token;

@end

/// SEI Callback info.
@interface ZegoMediaSideInfo : NSObject

/// Stream ID.
@property (nonatomic, copy) NSString *streamID;

/// SEI data
@property (nonatomic, strong) NSData *SEIData;

/// timestamp
@property (nonatomic, assign) long long timestampNs;

/// SEI source module. Please contact ZEGO technical support.
@property int moduleType;

@end

/// Device Info.
///
/// Including device ID and name
@interface ZegoDeviceInfo : NSObject

/// Device ID
@property (nonatomic, copy) NSString *deviceID;

/// Device name
@property (nonatomic, copy) NSString *deviceName;

/// Device extra info, Format: key="value"\nkey2="value2"..., use line break \n to separate key-value pairs, and use equal sign = to separate key and "value", and there are double quotes around the value
@property (nonatomic, copy) NSString *deviceExtraInfo;

@end

/// System performance monitoring status
@interface ZegoPerformanceStatus : NSObject

/// Current CPU usage of the app, value range [0, 1]
@property (nonatomic, assign) double cpuUsageApp;

/// Current CPU usage of the system, value range [0, 1]
@property (nonatomic, assign) double cpuUsageSystem;

/// Current memory usage of the app, value range [0, 1]
@property (nonatomic, assign) double memoryUsageApp;

/// Current memory usage of the system, value range [0, 1]
@property (nonatomic, assign) double memoryUsageSystem;

/// Current memory used of the app, in MB
@property (nonatomic, assign) double memoryUsedApp;

@end

/// Beauty configuration options.
///
/// Configure the parameters of skin peeling, whitening and sharpening
@interface ZegoBeautifyOption : NSObject

/// The sample step size of beauty peeling, the value range is [0,1], default 0.2
@property (nonatomic, assign) double polishStep;

/// Brightness parameter for beauty and whitening, the larger the value, the brighter the brightness, ranging from [0,1], default 0.5
@property (nonatomic, assign) double whitenFactor;

/// Beauty sharpening parameter, the larger the value, the stronger the sharpening, value range [0,1], default 0.1
@property (nonatomic, assign) double sharpenFactor;

/// Create a default beauty parameter object
///
/// @return ZegoBeautifyOption instance
+ (instancetype)defaultConfig;

@end

/// Beauty configuration param.
///
/// Configure the whiten, rosy, smooth, and sharpen parameters for beauty.
@interface ZegoEffectsBeautyParam : NSObject

/// The whiten intensity parameter, the value range is [0,100], and the default is 50.
@property (nonatomic, assign) int whitenIntensity;

/// the rosy intensity parameter, value range [0,100], and the default is 50.
@property (nonatomic, assign) int rosyIntensity;

/// the smooth intensity parameter, value range [0,100], and the default is 50.
@property (nonatomic, assign) int smoothIntensity;

/// the sharpen intensity parameter, value range [0,100], and the default is 50.
@property (nonatomic, assign) int sharpenIntensity;

@end

/// Mix stream audio configuration.
///
/// Configure video frame rate, bitrate, and resolution for mixer task
@interface ZegoMixerAudioConfig : NSObject

/// Audio bitrate in kbps, default is 48 kbps, cannot be modified after starting a mixer task
@property (nonatomic, assign) int bitrate;

/// Audio channel, default is Mono
@property (nonatomic, assign) ZegoAudioChannel channel;

/// codec ID, default is ZegoAudioCodecIDDefault
@property (nonatomic, assign) ZegoAudioCodecID codecID;

/// Multi-channel audio stream mixing mode. If [ZegoAudioMixMode] is selected as [Focused], the SDK will select 4 input streams with [isAudioFocus] set as the focus voice highlight. If it is not selected or less than 4 channels are selected, it will automatically fill in 4 channels. On web platforms, this property does not take effect.
@property (nonatomic, assign) ZegoAudioMixMode mixMode;

/// Create a default mix stream audio configuration
///
/// @return ZegoMixerAudioConfig instance
+ (instancetype)defaultConfig;

@end

/// Mix stream video config object.
///
/// Configure video frame rate, bitrate, and resolution for mixer task
@interface ZegoMixerVideoConfig : NSObject

/// Video FPS, cannot be modified after starting a mixer task
@property (nonatomic, assign) int fps;

/// Video bitrate in kbps
@property (nonatomic, assign) int bitrate;

/// Video quality, this value is valid when the video rate control mode parameter is set to constant quality. The valid value ranges from 0 to 51. The default value is 23. If you want better video quality, lower the quality value based on 23 to test the adjustment. If you want a smaller file size, test the adjustment by increasing the high quality value at the base of 23. Take the file size under the value x as an example. The file size under the value x + 6 is half the size of the file size under the value x, and the file size under the value x-6 is twice the size of the file size under the value x. On web platforms, this property does not take effect.
@property (nonatomic, assign) int quality;

/// Video bitrate control mode.  On web platforms, this property does not take effect.
@property (nonatomic, assign) ZegoVideoRateControlMode rateControlMode;

/// video resolution
@property (nonatomic, assign) CGSize resolution;

/// Create a mixer video configuration
///
/// @return ZegoMixerVideoConfig instance
+ (instancetype)configWithResolution:(CGSize)resolution fps:(int)fps bitrate:(int)bitrate;

/// Create a mixer video configuration
///
/// @return ZegoMixerVideoConfig instance
- (instancetype)initWithResolution:(CGSize)resolution fps:(int)fps bitrate:(int)bitrate;

@end

/// Mix stream output video config object.
///
/// Description: Configure the video parameters, coding format and bitrate of mix stream output.
/// Use cases: Manual mixed stream scenario, such as Co-hosting.
@interface ZegoMixerOutputVideoConfig : NSObject

/// Mix stream output video coding format, supporting H.264 and h.265 coding.
@property (nonatomic, assign) ZegoVideoCodecID videoCodecID;

/// Mix stream output video bitrate in kbps. The default value is the bitrate configured in [ZegoMixerVideoConfig].
@property (nonatomic, assign) int bitrate;

/// Mix stream video encode profile. Default value is [ZegoEncodeProfileDefault].
@property (nonatomic, assign) ZegoEncodeProfile encodeProfile;

/// The video encoding delay of mixed stream output, Valid value range [0, 2000], in milliseconds. The default value is 0.
@property (nonatomic, assign) int encodeLatency;

/// Enable high definition low bitrate. Default is false.
@property BOOL enableLowBitrateHD;

/// Set mix steram output video configuration.
- (void)configWithCodecID:(ZegoVideoCodecID)codecID bitrate:(int)bitrate;

/// Set mix steram output video configuration.
- (void)configWithCodecID:(ZegoVideoCodecID)codecID
                  bitrate:(int)bitrate
            encodeProfile:(ZegoEncodeProfile)profile
            encodeLatency:(int)latency;

/// Set mix steram output video configuration.
- (void)configWithCodecID:(ZegoVideoCodecID)codecID
                  bitrate:(int)bitrate
            encodeProfile:(ZegoEncodeProfile)profile
            encodeLatency:(int)latency
       enableLowBitrateHD:(BOOL)enableLowBitrateHD;

@end

/// Room information for the output stream in a mixed stream.
///
/// Available since: 3.18.0
/// Description: Setting room information for a single output stream; the mixed output stream can be added to the specified room, allowing users in the room to receive notifications of increased stream in the room.
/// Use cases: Manual mixed stream scenario, such as Co-hosting.
/// Restrictions: Dynamic updates during mixed stream are not supported.
@interface ZegoMixerOutputRoomInfo : NSObject

/// Specifies the room ID of the output stream. You need to ensure that the room is already present when mixing starts.
@property (nonatomic, copy) NSString *roomID;

/// Specifies the user ID of the output stream. It is not recommended to use the same userID as the actual user in the room to avoid conflicts with the SDK's stream addition behavior.
@property (nonatomic, copy) NSString *userID;

/// This function is unavaialble.
///
/// Please use [initWithRoomID:userID:] instead
+ (instancetype)new NS_UNAVAILABLE;

/// This function is unavaialble.
///
/// Please use [initWithRoomID:userID:] instead
- (instancetype)init NS_UNAVAILABLE;

/// Construct the room information of an output stream in a mixed stream by using roomID and userID.
///
/// @return ZegoMixerOutputRoomInfo instance.
- (instancetype)initWithRoomID:(NSString *)roomID userID:(NSString *)userID;

@end

/// Font style.
///
/// Description: Font style configuration, can be used to configure font type, font size, font color, font transparency.
/// Use cases: Set text watermark in manual stream mixing scene, such as Co-hosting.
@interface ZegoFontStyle : NSObject

/// Font type. Required: False. Default value: Source han sans [ZegoFontTypeSourceHanSans]
@property (nonatomic, assign) ZegoFontType type;

/// Font size in px. Required: False. Default value: 24. Value range: [12,100].
@property (nonatomic, assign) int size;

/// Font color, the calculation formula is: R + G x 256 + B x 65536, the value range of R (red), G (green), and B (blue) [0,255]. Required: False. Default value: 16777215(white). Value range: [0,16777215].
@property (nonatomic, assign) int color;

/// Font transparency. Required: False. Default value: 0. Value range: [0,100], 100 is completely opaque, 0 is completely transparent.
@property (nonatomic, assign) int transparency;

/// Whether the font has a border. Required: False. Default value: False. Value range: True/False.
@property (nonatomic, assign) BOOL border;

/// Font border color, the calculation formula is: R + G x 256 + B x 65536, the value range of R (red), G (green), and B (blue) [0,255]. Required: False. Default value: 0. Value range: [0,16777215].
@property (nonatomic, assign) int borderColor;

@end

/// Label info.
///
/// Description: Font style configuration, can be used to configure font type, font si-e, font color, font transparency.
/// Use cases: Set text watermark in manual stream mixing scene, such as Co-hosting.
@interface ZegoLabelInfo : NSObject

/// Text content, support for setting simplified Chinese, English, half-width, not full-width. Required: True.Value range: Maximum support for displaying 100 Chinese characters and 300 English characters.
@property (nonatomic, copy) NSString *text;

/// The distance between the font and the left border of the output canvas, in px. Required: False. Default value: 0.
@property (nonatomic, assign) int left;

/// The distance between the font and the top border of the output canvas, in px. Required: False. Default value: 0.
@property (nonatomic, assign) int top;

/// Font style. Required: False.
@property (nonatomic, strong) ZegoFontStyle *font;

/// This function is unavaialble.
///
/// Please use [initWithText:] instead
+ (instancetype)new NS_UNAVAILABLE;

/// This function is unavaialble.
///
/// Please use [initWithText:] instead
- (instancetype)init NS_UNAVAILABLE;

/// build a label info object with text.
///
/// @return ZegoLabelInfo instance.
- (instancetype)initWithText:(NSString *)text;

@end

/// Set the image information of a single input stream in the mux.
///
/// Available since: 2.19.0
/// Description: Sets a picture for the content of a single input stream, which is used in place of the video, i.e. the video is not displayed when the picture is used. The `layout` layout in [ZegoMixerInput] for image multiplexing.
/// Use case: The developer needs to temporarily turn off the camera to display the image during the video connection to the microphone, or display the picture when the audio is connected to the microphone.
/// Restrictions: Image size is limited to 1M.
@interface ZegoMixerImageInfo : NSObject

/// The image path, if not empty, the image will be displayed, otherwise, the video will be displayed. JPG and PNG formats are supported. There are 2 ways to use it: 1. URI: Provide the picture to ZEGO technical support for configuration. After the configuration is complete, the picture URI will be provided, for example: preset-id://xxx.jpg. 2. URL: Only HTTP protocol is supported.
@property (nonatomic, copy) NSString *url;

/// Image display mode. 0: Default. Use image to replace video content when url is not null. 1: Display image based on camera status. Display image when camera is turned off. Display video content when camera is turned on (no need to clear the url parameter). 2: Display image based on the input stream has video data or not. Display image when there is no video data in the input stream for 3 consecutive seconds. The default duration for judging that there is no video data is 3 seconds. If you need change this setting, please contact ZEGO technical support. Display video content when the input stream has video data.
@property (nonatomic, assign) int displayMode;

/// This function is unavaialble.
///
/// Please use [initWithURL:] instead
+ (instancetype)new NS_UNAVAILABLE;

/// This function is unavaialble.
///
/// Please use [initWithURL:] instead
- (instancetype)init NS_UNAVAILABLE;

/// build a image info object with url.
///
/// @return ZegoMixerImageInfo instance.
- (instancetype)initWithURL:(NSString *)url;

/// build a image info object with url and displayMode.
///
/// @return ZegoMixerImageInfo instance.
- (instancetype)initWithURLAndDisplayMode:(NSString *)url displayMode:(int)displayMode;

@end

/// Mixer input.
///
/// Configure the mix stream input stream ID, type, and the layout
@interface ZegoMixerInput : NSObject

/// Stream ID, a string of up to 256 characters. Caution: You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '-', '_'.
@property (nonatomic, copy) NSString *streamID;

/// Mix stream content type
@property (nonatomic, assign) ZegoMixerInputContentType contentType;

/// Stream layout. When the mixed stream is an audio stream (that is, the ContentType parameter is set to the audio mixed stream type). Developers do not need to assign a value to this field, just use the SDK default.
@property (nonatomic, assign) CGRect layout;

/// If enable soundLevel in mix stream task, an unique soundLevelID is need for every stream
@property (nonatomic, assign) unsigned int soundLevelID;

/// Input stream volume, valid range [0, 200], default is 100. On web platforms, this property does not take effect.
@property (nonatomic, assign) unsigned int volume;

/// Whether the focus voice is enabled in the current input stream, the sound of this stream will be highlighted if enabled. On web platforms, this property does not take effect.
@property (nonatomic, assign) BOOL isAudioFocus;

/// The direction of the audio. Valid direction is between 0 to 360. Set -1 means disable. Default value is -1. On web platforms, this property does not take effect.
@property (nonatomic, assign) int audioDirection;

/// Text watermark.
@property (nonatomic, strong) ZegoLabelInfo *label;

/// Video view render mode.
@property (nonatomic, assign) ZegoMixRenderMode renderMode;

/// User image information.
@property (nonatomic, strong) ZegoMixerImageInfo *imageInfo;

/// Description: Video frame corner radius, in px. Required: False. Value range: Does not exceed the width and height of the video screen set by the [layout] parameter. Default value: 0.
@property (nonatomic, assign) unsigned int cornerRadius;

/// Set advanced configuration. Please contact ZEGO technical support. On web platforms, this property does not take effect.
@property (nonatomic, strong, nullable) NSDictionary<NSString *, NSString *> *advancedConfig;

/// Create a mixed input object
///
/// @return ZegoMixerInput instance
- (instancetype)initWithStreamID:(NSString *)streamID
                     contentType:(ZegoMixerInputContentType)contentType
                          layout:(CGRect)layout;

/// Create a mixed input object
///
/// @return ZegoMixerInput instance
- (instancetype)initWithStreamID:(NSString *)streamID
                     contentType:(ZegoMixerInputContentType)contentType
                          layout:(CGRect)layout
                    soundLevelID:(unsigned int)soundLevelID;

@end

/// Mixer output object, currently, a mixed-stream task only supports a maximum of four video streams with different resolutions.
///
/// Configure mix stream output target URL or stream ID
@interface ZegoMixerOutput : NSObject

/// Mix stream output target, URL or stream ID, if set to be URL format, only RTMP URL surpported, for example rtmp://xxxxxxxx, addresses with two identical mixed-stream outputs cannot be passed in.
@property (nonatomic, copy) NSString *target;

/// Mix stream output video config. On web platforms, this property does not take effect.
@property (nonatomic, strong) ZegoMixerOutputVideoConfig *videoConfig;

/// Specifies the room information for the output stream.
@property (nonatomic, strong) ZegoMixerOutputRoomInfo *targetRoom;

/// Create a mix stream output object
///
/// @return ZegoMixerOutput instance
- (instancetype)initWithTarget:(NSString *)target;

/// Set the video configuration of the mix stream output
- (void)setVideoConfig:(ZegoMixerOutputVideoConfig *)videoConfig;

/// Set the room information of the mix stream output
- (void)setTargetRoom:(ZegoMixerOutputRoomInfo *)targetRoom;

@end

/// Watermark object.
///
/// Configure a watermark image URL and the layout of the watermark in the screen.
@interface ZegoWatermark : NSObject

/// The path of the watermark image. Support local file absolute path (file://xxx), Asset resource path (asset://xxx). The format supports png, jpg. The maximum length is less than 512 bytes.
@property (nonatomic, copy) NSString *imageURL;

/// Watermark image layout
@property (nonatomic, assign) CGRect layout;

/// Create a watermark object
///
/// @return ZegoWatermark instance
- (instancetype)initWithImageURL:(NSString *)imageURL layout:(CGRect)layout;

@end

/// Mixer whiteboard object.
///
/// Configure the mix whiteboard ID, aspect ratio and the layout.
@interface ZegoMixerWhiteboard : NSObject

/// Whiteboard ID.
@property unsigned long long whiteboardID;

/// Whiteboard aspect ratio(width), the default aspect ratio is 16:9.
@property int horizontalRatio;

/// Whiteboard aspect ratio(height), the default aspect ratio is 16:9.
@property int verticalRatio;

/// Whether the whiteboard will load dynamic PPT files or not, default value is false.
@property BOOL isPPTAnimation;

/// Whiteboard layout.
@property (nonatomic, assign) CGRect layout;

/// Whiteboard z-order.
@property int zOrder;

/// Whiteboard background color. Defaule is 0xF1F3F400 (gray). The color value corresponding to RGBA is 0xRRGGBBAA, and setting the transparency of the background color is currently not supported. The AA in 0xRRGGBBAA is 00. For example, select RGB as \#87CEFA as the background color, this parameter passes 0x87CEFA00.F
@property int backgroundColor;

/// Create a mixer whiteboard object
///
/// @return ZegoMixerWhiteboard instance
- (instancetype)initWithWhiteboardID:(unsigned long long)whiteboardID layout:(CGRect)layout;

@end

/// Mix stream task object.
///
/// This class is the configuration class of the stream mixing task. When a stream mixing task is requested to the ZEGO RTC server, the configuration of the stream mixing task is required.
/// This class describes the detailed configuration information of this stream mixing task.
@interface ZegoMixerTask : NSObject

/// Mix stream task ID, a string of up to 256 characters. You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '~', '!', '@', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
@property (nonatomic, copy, readonly) NSString *taskID;

/// This function is unavaialble
///
/// Please use [initWithTaskID:] instead
+ (instancetype)new NS_UNAVAILABLE;

/// This function is unavaialble
///
/// Please use [initWithTaskID:] instead
- (instancetype)init NS_UNAVAILABLE;

/// Create a mix stream task object with TaskID
///
/// @return ZegoMixerTask instance
- (instancetype)initWithTaskID:(NSString *)taskID;

/// Set the audio configuration of the mix stream task object
- (void)setAudioConfig:(ZegoMixerAudioConfig *)audioConfig;

/// Set the video configuration of the mix stream task object
- (void)setVideoConfig:(ZegoMixerVideoConfig *)videoConfig;

/// Set the input stream list for the mix stream task object
- (void)setInputList:(NSArray<ZegoMixerInput *> *)inputList;

/// Set the output list of the mix stream task object
- (void)setOutputList:(NSArray<ZegoMixerOutput *> *)outputList;

/// Set the watermark of the mix stream task object
- (void)setWatermark:(ZegoWatermark *)watermark;

/// Set the whiteboard of the mix stream task object
- (void)setWhiteboard:(ZegoMixerWhiteboard *)whiteboard;

/// Set the background color of the mix stream task object, The color value corresponding to RGBA is 0xRRGGBBAA, and setting the transparency of the background color is currently not supported. The AA in 0xRRGGBBAA is 00. For example, select RGB as \#87CEFA as the background color, this parameter passes 0x87CEFA00.
- (void)setBackgroundColor:(int)backgroundColor;

/// Set the background image of the mix stream task object
- (void)setBackgroundImageURL:(NSString *)backgroundImageURL;

/// Enable or disable sound level callback for the task. If enabled, then the remote player can get the soundLevel of every stream in the inputlist by [onMixerSoundLevelUpdate] callback.
- (void)enableSoundLevel:(BOOL)enable;

/// Setting the stream mixing alignment mode
- (void)setStreamAlignmentMode:(ZegoStreamAlignmentMode)mode;

/// Set custom user data, the length is no more than 1000.Note that only data with length will be read by SDK. If the length is greater than the actual length of data, the SDK will read the data according to the actual length of data.
- (void)setUserData:(NSData *)data length:(int)length;

/// Set advanced configuration, such as specifying video encoding and others. If you need to use it, contact ZEGO technical support.
- (void)setAdvancedConfig:(NSDictionary<NSString *, NSString *> *)config;

/// Description: Sets the lower limit of the interval range for the adaptive adjustment of the stream playing cache of the stream mixing server. In the real-time chorus KTV scenario, slight fluctuations in the network at the push end may cause the mixed stream to freeze. At this time, when the audience pulls the mixed stream, there is a high probability of the problem of freeze. By adjusting the lower limit of the interval range for the adaptive adjustment of the stream playing cache of the stream mixing server, it can optimize the freezing problem that occurs when playing mixing streams at the player end, but it will increase the delay. It is not set by default, that is, the server uses its own configuration values. It only takes effect for the new input stream setting, and does not take effect for the input stream that has already started mixing.Value Range: [0,10000], exceeding the maximum value will result in a failure of the stream mixing request. On web platforms, this property does not take effect.
- (void)setMinPlayStreamBufferLength:(int)minPlayStreamBufferLength;

/// Set the mixed stream image check mode.
- (void)setMixImageCheckMode:(ZegoMixImageCheckMode)mixImageCheckMode;

@end

/// Configuration for start sound level monitor.
@interface ZegoSoundLevelConfig : NSObject

/// Monitoring time period of the sound level, in milliseconds, has a value range of [100, 3000]. Default is 100 ms.
@property (nonatomic, assign) unsigned int millisecond;

/// Set whether the sound level callback includes the VAD detection result.
@property (nonatomic, assign) BOOL enableVAD;

@end

/// Sound level info object.
@interface ZegoSoundLevelInfo : NSObject

/// Sound level value.
@property (nonatomic, assign) float soundLevel;

/// Whether the stream corresponding to StreamID contains voice, 0 means noise, 1 means normal voice. This value is valid only when the [enableVAD] parameter in the [ZegoSoundLevelConfig] configuration is set to true when calling [startSoundLevelMonitor].
@property (nonatomic, assign) int vad;

@end

/// Auto mix stream task object.
///
/// Description: When using [StartAutoMixerTask] function to start an auto stream mixing task to the ZEGO RTC server, user need to set this parameter to configure the auto stream mixing task, including the task ID, room ID, audio configuration, output stream list, and whether to enable the sound level callback.
/// Use cases: This configuration is required when an auto stream mixing task is requested to the ZEGO RTC server.
/// Caution: As an argument passed when [StartAutoMixerTask] function is called.
@interface ZegoAutoMixerTask : NSObject

/// The taskID of the auto mixer task.Description: Auto stream mixing task id, must be unique in a room.Use cases: User need to set this parameter when initiating an auto stream mixing task.Required: Yes.Recommended value: Set this parameter based on requirements.Value range: A string up to 256 bytes.Caution: When starting a new auto stream mixing task, only one auto stream mixing task ID can exist in a room, that is, to ensure the uniqueness of task ID. You are advised to associate task ID with room ID. You can directly use the room ID as the task ID.Cannot include URL keywords, for example, 'http' and '?' etc, otherwise publishing stream and playing stream will fail. Only support numbers, English characters and '~', '!', '@', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
@property (nonatomic, copy) NSString *taskID;

/// The roomID of the auto mixer task.Description: Auto stream mixing task id.Use cases: User need to set this parameter when initiating an auto stream mixing task.Required: Yes.Recommended value: Set this parameter based on requirements.Value range: A string up to 128 bytes.Caution: Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.If you need to communicate with the Web SDK, please do not use '%'.
@property (nonatomic, copy) NSString *roomID;

/// The audio config of the auto mixer task.Description: The audio config of the auto mixer task.Use cases: If user needs special requirements for the audio config of the auto stream mixing task, such as adjusting the audio bitrate, user can set this parameter as required. Otherwise, user do not need to set this parameter.Required: No.Default value: The default audio bitrate is `48 kbps`, the default audio channel is `ZEGO_AUDIO_CHANNEL_MONO`, the default encoding ID is `ZEGO_AUDIO_CODEC_ID_DEFAULT`, and the default multi-channel audio stream mixing mode is `ZEGO_AUDIO_MIX_MODE_RAW`.Recommended value: Set this parameter based on requirements.
@property (nonatomic, strong) ZegoMixerAudioConfig *audioConfig;

/// The output list of the auto mixer task.Description: The output list of the auto stream mixing task, items in the list are URL or stream ID, if the item set to be URL format, only RTMP URL surpported, for example rtmp://xxxxxxxx.Use cases: User need to set this parameter to specify the mix stream output target when starting an auto stream mixing task.Required: Yes.
@property (nonatomic, strong) NSArray<ZegoMixerOutput *> *outputList;

/// Enable or disable sound level callback for the task. If enabled, then the remote player can get the sound level of every stream in the inputlist by [onAutoMixerSoundLevelUpdate] callback.Description: Enable or disable sound level callback for the task.If enabled, then the remote player can get the sound level of every stream in the inputlist by [onAutoMixerSoundLevelUpdate] callback.Use cases: This parameter needs to be configured if user need the sound level information of every stream when an auto stream mixing task started.Required: No.Default value: `false`.Recommended value: Set this parameter based on requirements.
@property (nonatomic, assign) BOOL enableSoundLevel;

/// Stream mixing alignment mode.
@property (nonatomic, assign) ZegoStreamAlignmentMode streamAlignmentMode;

/// Description: Sets the lower limit of the interval range for the adaptive adjustment of the stream playing cache of the stream mixing server. In the real-time chorus KTV scenario, slight fluctuations in the network at the push end may cause the mixed stream to freeze. At this time, when the audience pulls the mixed stream, there is a high probability of the problem of freeze. By adjusting the lower limit of the interval range for the adaptive adjustment of the stream playing cache of the stream mixing server, it can optimize the freezing problem that occurs when playing mixing streams at the player end, but it will increase the delay. It is not set by default, that is, the server uses its own configuration values. It only takes effect for the new input stream setting, and does not take effect for the input stream that has already started mixing.Value Range: [0,10000], exceeding the maximum value will result in a failure of the stream mixing request. On web platforms, this property does not take effect.
@property (nonatomic, assign) int minPlayStreamBufferLength;

@end

/// Broadcast message info.
///
/// The received object of the room broadcast message, including the message content, message ID, sender, sending time
@interface ZegoBroadcastMessageInfo : NSObject

/// message content
@property (nonatomic, copy) NSString *message;

/// message id
@property (nonatomic, assign) unsigned long long messageID;

/// Message send time, UNIX timestamp, in milliseconds.
@property (nonatomic, assign) unsigned long long sendTime;

/// Message sender.Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.
@property (nonatomic, strong) ZegoUser *fromUser;

@end

/// Barrage message info.
///
/// The received object of the room barrage message, including the message content, message ID, sender, sending time
@interface ZegoBarrageMessageInfo : NSObject

/// message content
@property (nonatomic, copy) NSString *message;

/// message id
@property (nonatomic, copy) NSString *messageID;

/// Message send time, UNIX timestamp, in milliseconds.
@property (nonatomic, assign) unsigned long long sendTime;

/// Message sender.Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.
@property (nonatomic, strong) ZegoUser *fromUser;

@end

/// Transparent message info.
///
/// Room transparent message, including room id, message content, sending user, sending type, sending mode, timeout period
@interface ZegoRoomSendTransparentMessage : NSObject

/// send mode
@property (nonatomic, assign) ZegoRoomTransparentMessageMode sendMode;

/// send type
@property (nonatomic, assign) ZegoRoomTransparentMessageType sendType;

/// Message send content.
@property (nonatomic, strong) NSData *content;

/// Message receiver list, when sendType appointed ZegoRoomTransparentMessageModeOnlyServer don't fill in. When appointed ZegoRoomTransparentMessageModeClientAndServer or ZegoRoomTransparentMessageModeOnlyClient, empty room will be sent to all online users.
@property (nonatomic, strong) NSArray<ZegoUser *> *recvUserList;

/// send message timeout, The default value is 10s.
@property (nonatomic, assign) int timeOut;

@end

/// Received a transparent message from the room.
///
/// Room transparent message, including message content, sending user, sending type, sending mode
@interface ZegoRoomRecvTransparentMessage : NSObject

/// send message user
@property (nonatomic, strong) ZegoUser *sendUser;

/// Message send content.
@property (nonatomic, strong) NSData *content;

@end

/// Object for video frame fieldeter.
///
/// Including video frame format, width and height, etc.
@interface ZegoVideoFrameParam : NSObject

/// Video frame format
@property (nonatomic, assign) ZegoVideoFrameFormat format;

/// Number of bytes per line (for example: BGRA only needs to consider strides [0], I420 needs to consider strides [0,1,2])
@property (nonatomic, assign) int *strides;

/// The rotation direction of the video frame, the SDK rotates clockwise
@property (nonatomic, assign) int rotation;

/// Video frame size
@property (nonatomic, assign) CGSize size;

@end

/// Object for video encoded frame fieldeter.
///
/// Including video encoded frame format, width and height, etc.
@interface ZegoVideoEncodedFrameParam : NSObject

/// Video encoded frame format
@property (nonatomic, assign) ZegoVideoEncodedFrameFormat format;

/// Whether it is a keyframe
@property (nonatomic, assign) BOOL isKeyFrame;

/// Video frame counterclockwise rotation angle
@property (nonatomic, assign) int rotation;

/// Video frame size
@property (nonatomic, assign) CGSize size;

/// SEI data (Optional, if you don't need to send SEI, set it to null. Deprecated, use [sendSEI] instead). Useful when set format as [AVCC] or [AnnexB]
@property (nonatomic, strong, nullable) NSData *SEIData;

/// Whether to use the external timestamp completely. The default is false. When set to false, the SDK will adjust based on the timestamps of the audio frame and video frame to ensure audio-video synchronization. When set to true, the SDK does not adjust the timestamp and uses the external timestamp completely.
@property (nonatomic, assign) BOOL isExternalClock;

@end

/// Parameter object for audio frame.
///
/// Including the sampling rate and channel of the audio frame
@interface ZegoAudioFrameParam : NSObject

/// Sampling Rate
@property (nonatomic, assign) ZegoAudioSampleRate sampleRate;

/// Audio channel, default is Mono
@property (nonatomic, assign) ZegoAudioChannel channel;

@end

/// Audio configuration.
///
/// Configure audio bitrate, audio channel, audio encoding for publishing stream
@interface ZegoAudioConfig : NSObject

/// Audio bitrate in kbps, default is 48 kbps. The settings before and after publishing stream can be effective
@property (nonatomic, assign) int bitrate;

/// Audio channel, default is Mono. The setting only take effect before publishing stream
@property (nonatomic, assign) ZegoAudioChannel channel;

/// codec ID, default is ZegoAudioCodecIDDefault. The setting only take effect before publishing stream
@property (nonatomic, assign) ZegoAudioCodecID codecID;

/// Create a default audio configuration
///
/// ZegoAudioConfigPresetStandardQuality (48 kbps, Mono, ZegoAudioCodecIDDefault)
///
/// @return ZegoAudioConfig instance
+ (instancetype)defaultConfig;

/// Create a audio configuration with preset enumeration values
///
/// @return ZegoAudioConfig instance
+ (instancetype)configWithPreset:(ZegoAudioConfigPreset)preset;

/// Create a audio configuration with preset enumeration values
///
/// @return ZegoAudioConfig instance
- (instancetype)initWithPreset:(ZegoAudioConfigPreset)preset;

@end

/// audio mixing data.
@interface ZegoAudioMixingData : NSObject

/// Audio PCM data that needs to be mixed into the stream
@property (nonatomic, strong, nullable) NSData *audioData;

/// Audio data attributes, including sample rate and number of channels. Currently supports 16k, 32k, 44.1k, 48k sampling rate, mono or stereo, 16-bit deep PCM data. Developers need to explicitly specify audio data attributes, otherwise mixing will not take effect.
@property (nonatomic, strong) ZegoAudioFrameParam *param;

/// SEI data, used to transfer custom data. When audioData is null, SEIData will not be sent
@property (nonatomic, strong, nullable) NSData *SEIData;

@end

/// Customize the audio processing configuration object.
///
/// Including custom audio acquisition type, sampling rate, channel number, sampling number and other parameters
@interface ZegoCustomAudioProcessConfig : NSObject

/// Sampling rate, the sampling rate of the input data expected by the audio pre-processing module in App. If 0, the default is the SDK internal sampling rate.
@property (nonatomic, assign) ZegoAudioSampleRate sampleRate;

/// Number of sound channels, the expected number of sound channels for input data of the audio pre-processing module in App. If 0, the default is the number of internal channels in the SDK
@property (nonatomic, assign) ZegoAudioChannel channel;

/// The number of samples required to encode a frame; if samples = 0, the SDK will use the internal sample number, and the SDK will pass the audio data to the external pre-processing module. If the samples! = 0 (the effective value of samples is between [160, 2048]), and the SDK will send audio data to the external preprocessing module that sets the length of sample number.
@property (nonatomic, assign) int samples;

@end

/// Record config.
@interface ZegoDataRecordConfig : NSObject

/// The path to save the recording file, absolute path, need to include the file name, the file name need to specify the suffix, currently supports .mp4/.flv/.aac format files, if multiple recording for the same path, will overwrite the file with the same name. The maximum length should be less than 1024 bytes.
@property (nonatomic, copy) NSString *filePath;

/// Type of recording media
@property (nonatomic, assign) ZegoDataRecordType recordType;

@end

/// File recording progress.
@interface ZegoDataRecordProgress : NSObject

/// Current recording duration in milliseconds
@property (nonatomic, assign) unsigned long long duration;

/// Current recording file size in byte
@property (nonatomic, assign) unsigned long long currentFileSize;

/// The quality of current recording file
@property (nonatomic, strong) ZegoPublishStreamQuality *quality;

@end

/// Network probe config
@interface ZegoNetworkProbeConfig : NSObject

/// Whether do traceroute, enabling tranceRoute will significantly increase network detection time
@property (nonatomic, assign) BOOL enableTraceroute;

@end

/// http probe result
@interface ZegoNetworkProbeHttpResult : NSObject

/// http probe errorCode, 0 means the connection is normal
@property (nonatomic, assign) int errorCode;

/// http request cost time, the unit is millisecond
@property (nonatomic, assign) unsigned int requestCostTime;

@end

/// tcp probe result
@interface ZegoNetworkProbeTcpResult : NSObject

/// tcp probe errorCode, 0 means the connection is normal
@property (nonatomic, assign) int errorCode;

/// tcp rtt, the unit is millisecond
@property (nonatomic, assign) unsigned int rtt;

/// tcp connection cost time, the unit is millisecond
@property (nonatomic, assign) unsigned int connectCostTime;

@end

/// udp probe result
@interface ZegoNetworkProbeUdpResult : NSObject

/// udp probe errorCode, 0 means the connection is normal
@property (nonatomic, assign) int errorCode;

/// The total time that the SDK send udp data to server and receive a reply, the unit is millisecond
@property (nonatomic, assign) unsigned int rtt;

@end

/// traceroute result
///
/// Jump up to 30 times. The traceroute result is for reference and does not represent the final network connection result. The priority is http, tcp, udp probe result.
@interface ZegoNetworkProbeTracerouteResult : NSObject

/// traceroute error code, 0 means normal
@property (nonatomic, assign) int errorCode;

/// Time consumed by trace route, the unit is millisecond
@property (nonatomic, assign) unsigned int tracerouteCostTime;

@end

/// Network probe result
@interface ZegoNetworkProbeResult : NSObject

/// http probe result
@property (nonatomic, strong, nullable) ZegoNetworkProbeHttpResult *httpProbeResult;

/// tcp probe result
@property (nonatomic, strong, nullable) ZegoNetworkProbeTcpResult *tcpProbeResult;

/// udp probe result
@property (nonatomic, strong, nullable) ZegoNetworkProbeUdpResult *udpProbeResult;

/// traceroute result
@property (nonatomic, strong, nullable) ZegoNetworkProbeTracerouteResult *tracerouteResult;

@end

/// Network speed test config
@interface ZegoNetworkSpeedTestConfig : NSObject

/// Test uplink or not
@property (nonatomic, assign) BOOL testUplink;

/// The unit is kbps. Recommended to use the bitrate in ZegoVideoConfig when call startPublishingStream to determine whether the network uplink environment is suitable.
@property (nonatomic, assign) int expectedUplinkBitrate;

/// Test downlink or not
@property (nonatomic, assign) BOOL testDownlink;

/// The unit is kbps. Recommended to use the bitrate in ZegoVideoConfig when call startPublishingStream to determine whether the network downlink environment is suitable.
@property (nonatomic, assign) int expectedDownlinkBitrate;

@end

/// test connectivity result
@interface ZegoTestNetworkConnectivityResult : NSObject

/// connect cost
@property (nonatomic, assign) unsigned int connectCost;

@end

/// network speed test quality
@interface ZegoNetworkSpeedTestQuality : NSObject

/// Time to connect to the server, in milliseconds. During the speed test, if the network connection is disconnected, it will automatically initiate a reconnection, and this variable will be updated accordingly.
@property (nonatomic, assign) unsigned int connectCost;

/// rtt, in milliseconds
@property (nonatomic, assign) unsigned int rtt;

/// packet lost rate. in percentage, 0.0 ~ 1.0
@property (nonatomic, assign) double packetLostRate;

/// network quality. excellent, good, medium and poor
@property (nonatomic, assign) ZegoStreamQualityLevel quality;

@end

/// RTC Network Statistics
@interface ZegoRtcStatsInfo : NSObject

/// total upstream bandwidth, in kbps
@property (nonatomic, assign) double totalTxBandwidth;

/// upstream average rtt, in milliseconds
@property (nonatomic, assign) unsigned int avgTxRtt;

/// upstream average packet lost rate. in percentage, 0.0 ~ 1.0
@property (nonatomic, assign) double avgTxPacketLostRate;

/// total downlink bandwidth, in kbps
@property (nonatomic, assign) double totalRxBandwidth;

/// downlink average rtt, in milliseconds
@property (nonatomic, assign) unsigned int avgRxRtt;

/// downlink average packet lost rate. in percentage, 0.0 ~ 1.0
@property (nonatomic, assign) double avgRxPacketLostRate;

/// average peer to peer delay, in milliseconds
@property (nonatomic, assign) unsigned int avgPeerToPeerDelay;

@end

/// The NTP info
@interface ZegoNetworkTimeInfo : NSObject

/// Network timestamp after synchronization, 0 indicates not yet synchronized
@property (nonatomic, assign) unsigned long long timestamp;

/// The max deviation
@property (nonatomic, assign) int maxDeviation;

@end

/// AudioEffectPlayer play configuration.
@interface ZegoAudioEffectPlayConfig : NSObject

/// The number of play counts. When set to 0, it will play in an infinite loop until the user invoke [stop]. The default is 1, which means it will play only once.
@property (nonatomic, assign) unsigned int playCount;

/// Whether to mix audio effects into the publishing stream, the default is false.
@property (nonatomic, assign) BOOL isPublishOut;

@end

/// Precise seek configuration
@interface ZegoAccurateSeekConfig : NSObject

/// The timeout time for precise search; if not set, the SDK internal default is set to 5000 milliseconds, the effective value range is [2000, 10000], the unit is ms
@property (nonatomic, assign) unsigned long long timeout;

@end

/// Media player network cache information
@interface ZegoNetWorkResourceCache : NSObject

/// Cached playable duration, unit ms
@property (nonatomic, assign) unsigned int time;

/// Cached playable size, unit byte
@property (nonatomic, assign) unsigned int size;

@end

/// CopyrightedMusic play configuration.
@interface ZegoCopyrightedMusicConfig : NSObject

/// User object instance, configure userID, userName. Note that the user ID set here needs to be consistent with the user ID set when logging in to the room, otherwise the request for the copyright music background service will fail.
@property (nonatomic, strong) ZegoUser *user;

@end

/// The configuration of getting lyric.
@interface ZegoCopyrightedMusicGetLyricConfig : NSObject

/// the ID of the song.
@property (nonatomic, copy) NSString *songID;

/// Copyright music resource song copyright provider.
@property (nonatomic, assign) int vendorID;

@end

/// The configuration of requesting resource.
@interface ZegoCopyrightedMusicRequestConfig : NSObject

/// the ID of the song.
@property (nonatomic, copy) NSString *songID;

/// VOD billing mode.
@property (nonatomic, assign) ZegoCopyrightedMusicBillingMode mode;

/// Copyright music resource song copyright provider.
@property (nonatomic, assign) ZegoCopyrightedMusicVendorID vendorID;

/// The room ID, the single-room mode can not be passed, and the corresponding room ID must be passed in the multi-room mode. Indicate in which room to order song/accompaniment/accompaniment clip/accompaniment segment.
@property (nonatomic, copy) NSString *roomID;

/// The master ID, which must be passed when the billing mode is billed by host. Indicate which homeowner to order song/accompaniment/accompaniment clip/accompaniment segment.
@property (nonatomic, copy) NSString *masterID;

/// The scene ID, indicate the actual business. For details, please consult ZEGO technical support.
@property (nonatomic, assign) int sceneID;

@end

/// The configuration of requesting resource.
@interface ZegoCopyrightedMusicRequestConfigV2 : NSObject

/// the ID of the song.
@property (nonatomic, copy) NSString *songID;

/// VOD billing mode. Refer to the value of [ZegoCopyrightedMusicBillingMode].
@property (nonatomic, assign) int mode;

/// Copyright music resource song copyright provider. Refer to the value of [ZegoCopyrightedMusicVendorID].
@property (nonatomic, assign) int vendorID;

/// The room ID, the single-room mode can not be passed, and the corresponding room ID must be passed in the multi-room mode. Indicate in which room to order song/accompaniment/accompaniment clip/accompaniment segment.
@property (nonatomic, copy) NSString *roomID;

/// The master ID, which must be passed when the billing mode is billed by host. Indicate which homeowner to order song/accompaniment/accompaniment clip/accompaniment segment.
@property (nonatomic, copy) NSString *masterID;

/// The scene ID, indicate the actual business. For details, please consult ZEGO technical support.
@property (nonatomic, assign) int sceneID;

/// The resource type of music. Refer to the value of [ZegoCopyrightedMusicResourceType].
@property (nonatomic, assign) int resourceType;

@end

/// The configuration of getting shared resource.
@interface ZegoCopyrightedMusicGetSharedConfig : NSObject

/// the ID of the song.
@property (nonatomic, copy) NSString *songID;

/// Copyright music resource song copyright provider.
@property (nonatomic, assign) ZegoCopyrightedMusicVendorID vendorID;

/// The room ID, the single-room mode can not be passed, and the corresponding room ID must be passed in the multi-room mode. Indicates which room to get resources from.
@property (nonatomic, copy) NSString *roomID;

@end

/// The configuration of getting shared resource.
@interface ZegoCopyrightedMusicGetSharedConfigV2 : NSObject

/// the ID of the song.
@property (nonatomic, copy) NSString *songID;

/// Copyright music resource song copyright provider. Refer to the value of [ZegoCopyrightedMusicVendorID].
@property (nonatomic, assign) int vendorID;

/// The room ID, the single-room mode can not be passed, and the corresponding room ID must be passed in the multi-room mode. Indicates which room to get resources from.
@property (nonatomic, copy) NSString *roomID;

/// The resource type of music.
@property (nonatomic, assign) int resourceType;

@end

/// The configuration of querying cache.
@interface ZegoCopyrightedMusicQueryCacheConfig : NSObject

/// the ID of the song.
@property (nonatomic, copy) NSString *songID;

/// The resource type of music.
@property (nonatomic, assign) ZegoCopyrightedMusicResourceType resourceType;

/// The resource quality type of music.
@property (nonatomic, assign) ZegoCopyrightedMusicResourceQualityType resourceQualityType;

/// Copyright music resource song copyright provider.
@property (nonatomic, assign) ZegoCopyrightedMusicVendorID vendorID;

@end

/// The configuration of querying cache.
@interface ZegoCopyrightedMusicQueryCacheConfigV2 : NSObject

/// the ID of the song.
@property (nonatomic, copy) NSString *songID;

/// The resource type of music. Refer to the value of [ZegoCopyrightedMusicResourceType].
@property (nonatomic, assign) int resourceType;

/// The resource quality type of music. Refer to the value of [ZegoCopyrightedMusicResourceQualityType].
@property (nonatomic, assign) int resourceQualityType;

/// Copyright music resource song copyright provider. Refer to the value of [ZegoCopyrightedMusicVendorID].
@property (nonatomic, assign) int vendorID;

@end

/// Orientation.
@interface ZegoPositionOrientation : NSObject

/// The unit vector of the front axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
@property (nonatomic, copy, nonnull) NSArray<NSNumber *> *axisForward;

/// The unit vector of the right axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
@property (nonatomic, copy, nonnull) NSArray<NSNumber *> *axisRight;

/// The unit vector of the up axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
@property (nonatomic, copy, nonnull) NSArray<NSNumber *> *axisUp;

@end

/// Request configuration of song or accompaniment.
@interface ZegoPosition : NSObject

/// The coordinates of the oneself in the world coordinate system. The parameter is a float array of length 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
@property (nonatomic, copy, nonnull) NSArray<NSNumber *> *coordinate;

/// Motion orientation.
@property (nonatomic, strong, nonnull) ZegoPositionOrientation *motionOrientation;

/// Camera orientation, for 3D range spatializer.
@property (nonatomic, strong, nonnull) ZegoPositionOrientation *cameraOrientation;

@end

/// Scene param.
@interface ZegoSceneParam : NSObject

/// User object instance, configure userID, userName. Note that the userID needs to be globally unique with the same appID, otherwise the user who logs in later will kick out the user who logged in first.
@property (nonatomic, strong, nonnull) ZegoUser *user;

/// Scene ID.
@property long long sceneID;

/// Scene template ID.
@property unsigned int templateID;

/// User position.
@property (nonatomic, strong, nonnull) ZegoPosition *position;

/// Broadcast user's own information mode, the default is ALL.
@property ZegoBroadcastMode broadcastMode;

/// Authentication token.
@property NSString *token;

@end

/// Scene config.
@interface ZegoSceneConfig : NSObject

/// State channel count.
@property unsigned int channelCount;

@end

/// Team param.
@interface ZegoTeamParam : NSObject

/// Team ID.
@property unsigned int teamID;

@end

/// Item param.
@interface ZegoItemParam : NSObject

/// Item ID.
@property long long itemID;

/// The number of users allowed to bind the item.
@property unsigned int capacity;

/// The initial position of the item in the range scene.
@property (nonatomic, strong, nonnull) ZegoPosition *position;

/// The mode of create item.
@property ZegoCreateItemMode createMode;

@end

/// Scene stream config.
@interface ZegoSceneStreamConfig : NSObject

/// Enable play stream in range or not.
@property BOOL enablePlayInRange;

/// Enable publish stream to world or not.
@property BOOL enablePublishToWorld;

@end

/// Screen capture configuration parameters.
@interface ZegoScreenCaptureConfig : NSObject

/// Whether to capture video when screen capture. The default is true.
@property (nonatomic, assign) BOOL captureVideo;

/// Whether to capture audio when screen capture. The default is true.
@property (nonatomic, assign) BOOL captureAudio;

/// Set Microphone audio volume for ReplayKit. The range is 0 ~ 200. The default is 100. (only for iOS)
@property (nonatomic, assign) unsigned int microphoneVolume;

/// Set Application audio volume for ReplayKit. The range is 0 ~ 200. The default is 100. (only for iOS)
@property (nonatomic, assign) unsigned int applicationVolume;

/// Set the crop rectangle during screen capture. The crop rectangle must be included in the rectangle of the original data, unit is pixel. (only for iOS/Android)
@property (nonatomic, assign) CGRect cropRect;

@end

/// The screen captures source information.
@interface ZegoScreenCaptureSourceInfo : NSObject

/// Target type for screen capture. (only for desktop)
@property (nonatomic, assign) ZegoScreenCaptureSourceType sourceType;

/// The ID of the capture source.
@property (nonatomic, assign) unsigned int sourceID;

/// Capture source name (in UTF8 encoding).
@property (nonatomic, copy) NSString *sourceName;

/// Thumbnail of the capture window.
@property (nonatomic, copy) ZGImage *_Nullable thumbnailImage;

/// The image content of the icon.
@property (nonatomic, copy) ZGImage *_Nullable iconImage;

@end

/// Layer border configuration.
///
/// Customize the size, color, etc. of the layer border.
@interface ZegoLayerBorderConfig : NSObject

/// Border size, default value 4, the maximum value is 100.
@property (nonatomic, assign) unsigned int width;

/// Background color, the format is 0xRRGGBB, default is green, which is 0x00FF00
@property (nonatomic, assign) int color;

@end

/// Audio source mix config
///
/// Used to config whether mix media player, audio effect player and captured system audio into publish stream or not when set audio source.
@interface ZegoAudioSourceMixConfig : NSObject

/// Media player instance index list.
@property (nonatomic, assign) int *mediaPlayerIndexList;

/// Media player instance count.
@property (nonatomic, assign) int mediaPlayerCount;

/// Audio effect player instance index list.
@property (nonatomic, assign) int *audioEffectPlayerIndexList;

/// Audio effect player instance count.
@property (nonatomic, assign) int audioEffectPlayerCount;

/// Create a default audio source mix config object
///
/// @return ZegoAudioSourceMixConfig instance
+ (instancetype)defaultConfig;

@end

/// Multimedia resource for ZEGO media player.
///
/// Used to configure loading parameters when loading multimedia resources.
@interface ZegoMediaPlayerResource : NSObject

/// Used to specify the loading type of multimedia resources.
@property (nonatomic, assign) ZegoMultimediaLoadType loadType;

/// The progress at which the plaback started.
@property (nonatomic, assign) long startPosition;

/// If the specified resource has a transparent effect, you need to specify the layout position of the alpha data.
@property (nonatomic, assign) ZegoAlphaLayoutType alphaLayout;

/// Common resource path.The absolute resource path or the URL of the network resource and cannot be null or "". Android can set this path string with Uri.
@property (nonatomic, copy) NSString *filePath;

/// binary data memory.
@property (nonatomic, strong, nullable) NSData *memory;

/// Binary data memory length.
@property (nonatomic, assign) int memoryLength;

/// The resource ID obtained from the copyrighted music module.
@property (nonatomic, copy) NSString *resourceID;

/// Online resource cache path, in utf8 encoding format.
@property (nonatomic, copy) NSString *onlineResourceCachePath;

/// The maximum length of online resource cache to be used, in bytes, with a minimum setting of 10M (10 * 1024 * 1024). The default value is 0 - no limit, and try to cache the entire file.
@property (nonatomic, assign) long long maxCachePendingLength;

@end

/// Background config.
///
/// It is used to configure background when the object segmentation is turned on.
@interface ZegoBackgroundConfig : NSObject

/// Background process type.
@property (nonatomic, assign) ZegoBackgroundProcessType processType;

/// Background color, the format is 0xRRGGBB, default is black, which is 0x000000
@property (nonatomic, assign) int color;

/// Background image URL. Support local file absolute path (file://xxx). The format supports png, jpg.
@property (nonatomic, copy) NSString *imageURL;

/// Background video URL. Caution: 1. The video will be played in a loop. 2. Support local file absolute path (file://xxx). 3. The format supports MP4, FLV, MKV, AVI. 4. The maximum resolution should not exceed 4096px, and it is recommended to be within 1920px. 5. The maximum video duration should not exceed 30 seconds, and it is recommended to be within 15 seconds. 6. The maximum video size should not exceed 50MB, and 10MB is recommended.
@property (nonatomic, copy) NSString *videoURL;

/// Background blur level.
@property (nonatomic, assign) ZegoBackgroundBlurLevel blurLevel;

@end

/// Object segmentation config.
///
/// It is used to configure parameters when the object segmentation is turned on.
@interface ZegoObjectSegmentationConfig : NSObject

/// The type of object segmentation.
@property (nonatomic, assign) ZegoObjectSegmentationType objectSegmentationType;

/// Background config.
@property (nonatomic, strong) ZegoBackgroundConfig *backgroundConfig;

@end

/// Media Infomration of media file.
///
/// Meida information such as video resolution of media file.
@interface ZegoMediaPlayerMediaInfo : NSObject

/// Video resolution width.
@property (nonatomic, assign) int width;

/// Video resolution height.
@property (nonatomic, assign) int height;

/// Video frame rate.
@property (nonatomic, assign) int frameRate;

@end

/// Used to config the media data publisher.
///
/// Used to config the media data publisher when creating it.
@interface ZegoMediaDataPublisherConfig : NSObject

/// Used to specify the publish channel index of publisher.
@property (nonatomic, assign) int channel;

/// Used to specify the mode of publisher.
@property (nonatomic, assign) ZegoMediaDataPublisherMode mode;

@end

/// Media player play statistics.
///
/// Media player play statistics.
@interface ZegoMediaPlayerStatisticsInfo : NSObject

/// Video source fps.
@property (nonatomic, assign) double videoSourceFps;

/// Video decode fps.
@property (nonatomic, assign) double videoDecodeFps;

/// Video render fps.
@property (nonatomic, assign) double videoRenderFps;

/// Audio source fps.
@property (nonatomic, assign) double audioSourceFps;

/// Audio decode fps.
@property (nonatomic, assign) double audioDecodeFps;

/// Audio render fps.
@property (nonatomic, assign) double audioRenderFps;

@end

/// Receive range configuration.
@interface ZegoReceiveRangeParam : NSObject

/// The minimum distance at which the 3D sound effect starts to have attenuation effect, the value needs to be >= 0 and <= max, the default value is 0.
@property (nonatomic, assign) float min;

/// The maximum range received, the value needs to be >= min, the default value is 0.
@property (nonatomic, assign) float max;

@end

/// Vocal range configuration.
@interface ZegoVocalRangeParam : NSObject

/// The minimum distance at which the 3D sound effect starts to have attenuation effect, the value needs to be >= 0 and <= max, the default value is 0.
@property (nonatomic, assign) float min;

/// The maximum range of the sound, the value needs to be >= min, the default value is 0.
@property (nonatomic, assign) float max;

@end

/// Dump data config.
@interface ZegoDumpDataConfig : NSObject

/// Data type.
@property (nonatomic, assign) ZegoDumpDataType dataType;

@end

/// AI voice changer speaker detail.
@interface ZegoAIVoiceChangerSpeakerInfo : NSObject

/// Speaker ID.
@property (nonatomic, assign) int id;

/// Speaker name.
@property (nonatomic, copy) NSString *name;

@end

/// Color enhancement params.
@interface ZegoColorEnhancementParams : NSObject

/// Description: color enhancement intensity. Value range: [0,1], the larger the value, the stronger the intensity of color enhancement. Default value: 0.
@property (nonatomic, assign) float intensity;

/// Description: Skin tone protection level. Value range: [0,1], the larger the value, the greater the level of skin protection. Default value: 1.
@property (nonatomic, assign) float skinToneProtectionLevel;

/// Description: Lip color protection level. Value range: [0,1], the larger the value, the greater the level of lip color protection. Default value: 0.
@property (nonatomic, assign) float lipColorProtectionLevel;

@end

/// Video denoise params.
@interface ZegoVideoDenoiseParams : NSObject

/// Description: Video denoise mode. Default value: Off.
@property (nonatomic, assign) ZegoVideoDenoiseMode mode;

/// Description: Video denoise strength. Default value: Light.
@property (nonatomic, assign) ZegoVideoDenoiseStrength strength;

@end

/// Dummy capture image params.
@interface ZegoDummyCaptureImageParams : NSObject

/// Picture file path.
@property (nonatomic, copy) NSString *path;

/// Dummy capture image mode.
@property (nonatomic, assign) ZegoDummyCaptureImageMode mode;

@end

/// Low light enhancement params.
@interface ZegoExpLowlightEnhancementParams : NSObject

/// Description: Low light enhancement mode. Default value: Off.
@property (nonatomic, assign) ZegoLowlightEnhancementMode mode;

/// Description: Low light enhancement type. Default value: Normal.
@property (nonatomic, assign) ZegoExpLowlightEnhancementType type;

@end

@interface ZegoRealTimeSequentialDataManager : NSObject

/// Sets up the real-time sequential data event handler.
///
/// Available since: 2.14.0
/// Description: Set up real-time sequential data callback to monitor callbacks such as sending data results， receiving data, etc.
/// When to call:After create the [ZegoRealTimeSequentialDataManager] instance.
/// Restrictions: None.
/// Caution: Calling this function will overwrite the callback set by the last call to this function.
///
/// @param handler Event handler for real-time sequential data
- (void)setEventHandler:(nullable id<ZegoRealTimeSequentialDataEventHandler>)handler;

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
- (void)startBroadcasting:(NSString *)streamID;

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
- (void)stopBroadcasting:(NSString *)streamID;

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
/// @param streamID The stream ID to which the real-time sequential data is sent.
/// @param callback Send real-time sequential data result callback.
- (void)sendRealTimeSequentialData:(NSData *)data
                          streamID:(NSString *)streamID
                          callback:(nullable ZegoRealTimeSequentialDataSentCallback)callback;

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
- (void)startSubscribing:(NSString *)streamID;

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
- (void)stopSubscribing:(NSString *)streamID;

/// Get real-time sequential data manager index.
///
/// @return Index of the real-time sequential data manager.
- (NSNumber *)getIndex;

/// This function is unavaialble.
///
/// Please use the [createRealTimeSequentialDataManager] function in ZegoExpressEngine class instead.
+ (instancetype)new NS_UNAVAILABLE;

/// This function is unavaialble.
///
/// Please use the [createRealTimeSequentialDataManager] function in ZegoExpressEngine class instead.
- (instancetype)init NS_UNAVAILABLE;

@end

@interface ZegoMediaPlayer : NSObject

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
- (void)setEventHandler:(nullable id<ZegoMediaPlayerEventHandler>)handler;

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
/// @param type Buffer type for video data
- (void)setVideoHandler:(nullable id<ZegoMediaPlayerVideoHandler>)handler
                 format:(ZegoVideoFrameFormat)format
                   type:(ZegoVideoBufferType)type;

/// Set audio data callback handler of the media player.
///
/// Available since: 2.1.0
/// Description: By setting this callback, the audio data of the media resource file played by the media player can be called back.
/// When to call: After the [ZegoMediaPlayer] instance created.
/// Restrictions: None.
/// Caution: When you no longer need to get the audio data, please call this function again to clear the handler to stop the audio data callback.
///
/// @param handler Audio data callback handler for media player
- (void)setAudioHandler:(nullable id<ZegoMediaPlayerAudioHandler>)handler;

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
- (void)setBlockDataHandler:(nullable id<ZegoMediaPlayerBlockDataHandler>)handler
                  blockSize:(unsigned int)blockSize;

/// Load local or network media resource.
///
/// Available: since 1.3.4
/// Description: Load media resources.
/// Use case: Developers can load the absolute path to the local resource or the URL of the network resource incoming.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Related APIs: Resources can be loaded through the [loadResourceWithPosition] or [loadResourceFromMediaData] function.
/// Caution: If the mediaplayer has already loaded resources or is in the process of playing, please first call the [stop] interface to halt the playback, and then proceed to call the interface to load the media resources; failure to do so will result in an unsuccessful load.
///
/// @param path The absolute resource path or the URL of the network resource and cannot be nil or "". Android can set this path string with Uri.
/// @param callback Notification of resource loading results
- (void)loadResource:(NSString *)path
            callback:(nullable ZegoMediaPlayerLoadResourceCallback)callback;

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
/// @param path The absolute resource path or the URL of the network resource and cannot be nil or "". Android can set this path string with Uri.
/// @param startPosition The progress at which the playback started.
/// @param callback Notification of resource loading results
- (void)loadResourceWithPosition:(NSString *)path
                   startPosition:(long)startPosition
                        callback:(nullable ZegoMediaPlayerLoadResourceCallback)callback;

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
/// @param startPosition Position of starting playback, in milliseconds.
/// @param callback Notification of resource loading results.
- (void)loadResourceFromMediaData:(NSData *)mediaData
                    startPosition:(long)startPosition
                         callback:(nullable ZegoMediaPlayerLoadResourceCallback)callback;

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
- (void)loadCopyrightedMusicResourceWithPosition:(NSString *)resourceID
                                   startPosition:(long)startPosition
                                        callback:
                                            (nullable ZegoMediaPlayerLoadResourceCallback)callback;

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
- (void)loadResourceWithConfig:(ZegoMediaPlayerResource *)resource
                      callback:(nullable ZegoMediaPlayerLoadResourceCallback)callback;

/// Start playing.
///
/// You need to load resources before playing
- (void)start;

/// Stop playing.
- (void)stop;

/// Pause playing.
- (void)pause;

/// Resume playing.
- (void)resume;

/// Set the specified playback progress.
///
/// Unit is millisecond
///
/// @param millisecond Point in time of specified playback progress
/// @param callback The result notification of set the specified playback progress
- (void)seekTo:(unsigned long long)millisecond
      callback:(nullable ZegoMediaPlayerSeekToCallback)callback;

/// Whether to repeat playback.
///
/// @param enable repeat playback flag. The default is NO.
- (void)enableRepeat:(BOOL)enable;

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
- (void)setPlaySpeed:(float)speed;

/// Whether to mix the player's sound into the stream being published.
///
/// This interface will only mix the media player sound into the main channel
///
/// @param enable Aux audio flag. The default is NO.
- (void)enableAux:(BOOL)enable;

/// Whether to play locally silently.
///
/// If [enableAux] switch is turned on, there is still sound in the publishing stream. The default is NO.
///
/// @param mute Mute local audio flag, The default is NO.
- (void)muteLocal:(BOOL)mute;

/// Set the view of the player playing video.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param canvas Video rendered canvas object
- (void)setPlayerCanvas:(nullable ZegoCanvas *)canvas;

/// Set mediaplayer volume. Both the local play volume and the publish volume are set.
///
/// @param volume The range is 0 ~ 200. The default is 60.
- (void)setVolume:(int)volume;

/// Set mediaplayer local playback volume.
///
/// @param volume The range is 0 ~ 200. The default is 60.
- (void)setPlayVolume:(int)volume;

/// Set mediaplayer publish volume.
///
/// @param volume The range is 0 ~ 200. The default is 60.
- (void)setPublishVolume:(int)volume;

/// Set playback progress callback interval.
///
/// This function can control the callback frequency of [onMediaPlayerPlayingProgress]. When the callback interval is set to 0, the callback is stopped. The default callback interval is 1s
/// This callback are not returned exactly at the set callback interval, but rather at the frequency at which the audio or video frames are processed to determine whether the callback is needed to call
///
/// @param millisecond Interval of playback progress callback in milliseconds
- (void)setProgressInterval:(unsigned long long)millisecond;

/// Gets the current local playback volume of the mediaplayer, the range is 0 ~ 200, with the default value of 60.
///
/// @return current volume
- (int)playVolume;

/// Gets the current publish volume of the mediaplayer, the range is 0 ~ 200, with the default value of 60.
///
/// @return current volume
- (int)publishVolume;

/// Get the total progress of your media resources.
///
/// You should load resource before invoking this function, otherwise the return value is 0
///
/// @return Unit is millisecond
- (unsigned long long)totalDuration;

/// Get current playing progress.
///
/// You should load resource before invoking this function, otherwise the return value is 0
///
/// @return current progress
- (unsigned long long)currentProgress;

/// Get current rendering progress.
///
/// You should load resource before invoking this function, otherwise the return value is 0
///
/// @return current rendering progress
- (unsigned long long)currentRenderingProgress;

/// Get the number of audio tracks of the playback file.
///
/// @return Number of audio tracks
- (unsigned int)audioTrackCount;

/// Set the audio track of the playback file.
///
/// @param index Audio track index, the number of audio tracks can be obtained through the [getAudioTrackCount] function.
- (void)setAudioTrackIndex:(unsigned int)index;

#if TARGET_OS_IPHONE
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
- (void)setAudioTrackMode:(ZegoMediaPlayerAudioTrackMode)mode;
#endif

#if TARGET_OS_IPHONE
/// Set the audio track for the media file to be publish.
///
/// Available since: 3.1.0
/// Description: Set the audio track for the media file to be publish.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Related APIs: The number of audio tracks can be obtained through the [getAudioTrackCount] function.
/// Caution: This call takes effect only after multitrack mode is enabled by calling the interface [setAudioTrackMode].
///
/// @param index Audio track index, the number of audio tracks can be obtained through the [getAudioTrackCount] function.
- (void)setAudioTrackPublishIndex:(unsigned int)index;
#endif

/// Enable voice changer, set up the specific voice changer parameters.
///
/// Available since: 3.15.0
/// Description: Enable voice changer, set up the specific voice changer parameters.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
///
/// @param enable Whether enable voice changer or not. True - enabled, NO - disabled, default value is NO.
/// @param param Voice changer parameters
/// @param audioChannel The audio channel to be voice changed
- (void)enableVoiceChanger:(BOOL)enable
                     param:(ZegoVoiceChangerParam *)param
              audioChannel:(ZegoMediaPlayerAudioChannel)audioChannel;

/// Get the current playback status.
///
/// @return current state
- (ZegoMediaPlayerState)currentState;

/// Get media player index.
///
/// Description: Get media player index.
/// When to call: It can be called after [createMediaPlayer].
/// Restrictions: None.
///
/// @return Media player index.
- (NSNumber *)index;

/// Take a screenshot of the current playing screen of the media player.
///
/// Only in the case of calling [setPlayerCanvas] to set the display controls and the playback state, can the screenshot be taken normally
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param callback The callback of the screenshot of the media player playing screen
- (void)takeSnapshot:(ZegoMediaPlayerTakeSnapshotCallback)callback;

/// Enable accurate seek and set relevant attributes.
///
/// Available since: 2.4.0
/// Description: The timestamp specified by normal seek may not an I frame, and then returns the I frame near the specified timestamp, which is not so accurate. But the accurate seek, when the specified timestamp is not an I frame, it will use the I frame near the specified timestamp to decode the frame of the specified timestamp.
/// Use cases: When user needs to seek to the specified timestamp accurately.
/// When to call: The setting must be called before [loadResource], and it will take effect during the entire life cycle of the media player.
///
/// @param enable Whether to enable accurate seek
/// @param config The property setting of accurate seek, only valid when enable is YES.
- (void)enableAccurateSeek:(BOOL)enable config:(ZegoAccurateSeekConfig *)config;

/// Set the maximum cache duration and cache data size of web materials.
///
/// The setting must be called before loading the resource, and it will take effect during the entire life cycle of the media player.
/// Time and size are not allowed to be 0 at the same time. The SDK internal default time is 5000, and the size is 15*1024*1024 byte.When one of time and size reaches the set value first, the cache will stop.
///
/// @param time The maximum length of the cache time, in ms, the SDK internal default is 5000; the effective value is greater than or equal to 2000; if you fill in 0, it means no limit.
/// @param size The maximum size of the cache, the unit is byte, the internal default size of the SDK is 15*1024*1024 byte; the effective value is greater than or equal to 5000000, if you fill in 0, it means no limit.
- (void)setNetWorkResourceMaxCache:(unsigned int)time size:(unsigned int)size;

/// Get the playable duration and size of the cached data of the current network material cache queue
///
/// @return Returns the current cached information, including the length of time the data can be played and the size of the cached data.
- (ZegoNetWorkResourceCache *)getNetWorkResourceCache;

/// Use this interface to set the cache threshold that the media player needs to resume playback. The SDK default value is 5000ms，The valid value is greater than or equal to 1000ms
///
/// The setting must be called before loading the resource, and it will take effect during the entire life cycle of the media player.
/// When the network status is poor and the media player has finished playing the cached network resources, it will stop playing, and notify the user through the `ZegoMediaPlayerNetworkEvent.BUFFER_BEGIN` state of the callback interface `onMediaPlayerNetworkEvent` that the network resources are being recached.
/// Only when the cached network resources are greater than the set threshold, the media player will automatically resume playback at the original paused position, and notify the user through the `ZegoMediaPlayerNetworkEvent.BUFFER_ENDED` of the callback interface `onMediaPlayerNetworkEvent` that the user has cached the network resources The threshold was reached and playback resumed.
///
/// @param threshold Threshold that needs to be reached to resume playback, unit ms.
- (void)setNetWorkBufferThreshold:(unsigned int)threshold;

/// Whether to enable sound level monitoring.
///
/// Available since: 2.15.0
/// Description: Whether to enable sound level monitoring.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Restrictions: None.
/// Related callbacks: After it is turned on, user can use the [onMediaPlayerSoundLevelUpdate] callback to monitor sound level updates.
///
/// @param enable Whether to enable monitoring, YES is enabled, NO is disabled.
/// @param millisecond Monitoring time period of the sound level, in milliseconds, has a value range of [100, 3000].
- (void)enableSoundLevelMonitor:(BOOL)enable millisecond:(unsigned int)millisecond;

/// Whether to enable frequency spectrum monitoring.
///
/// Available since: 2.15.0
/// Description: Whether to enable frequency spectrum monitoring.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Restrictions: None.
/// Related APIs: After it is turned on, user can use the [onMediaPlayerFrequencySpectrumUpdate] callback to monitor frequency spectrum updates.
///
/// @param enable Whether to enable monitoring, YES is enabled, NO is disabled.
/// @param millisecond Monitoring time period of the frequency spectrum, in milliseconds, has a value range of [10, 3000]. Note that on v3.19.0 and older version, the value range is [100, 3000].
- (void)enableFrequencySpectrumMonitor:(BOOL)enable millisecond:(unsigned int)millisecond;

/// Set the playback channel.
///
/// Available since: 2.20.0
/// Description: Set the playback channel.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Restrictions: None.
///
/// @param audioChannel Playback channel, the default is ZegoMediaPlayerAudioChannelAll.
- (void)setActiveAudioChannel:(ZegoMediaPlayerAudioChannel)audioChannel;

/// Clears the last frame of the playback control that remains on the control after playback ends.
///
/// Available since: 2.20.0
/// Description: Clears the last frame of the playback control that remains on the control after playback ends.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Restrictions: The interface call takes effect only when the media player ends playing.
- (void)clearView;

/// Get meida information such as video resolution from media file.
///
/// Available since: 3.6.0
/// Description: Get meida information such as video resolution from media file.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Restrictions: None.
- (ZegoMediaPlayerMediaInfo *)getMediaInfo;

/// Update the position of the media player (audio source).
///
/// Available since: 3.6.0
/// Description: Update the position of the media player (audio source).
/// Use cases: The media player also needs to have 3D spatial sound.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Restrictions: This interface needs to be used in conjunction with the RangeAudio/RangeScene module. This interface can only be called successfully after the RangeAudio/RangeScene module enables 3D sound effects.
///
/// @param position The unit vector of the front axis of its own coordinate system. The parameter is a float array with a length of 3.
- (void)updatePosition:(const float[_Nonnull 3])position;

/// Set http headers.
///
/// Available since: 3.8.0
/// Description: Call this function to set the http headers of the http network resource.
/// Use cases: When the network resource needs to set special header information.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Restrictions: Called before the corresponding network resource is loaded.
///
/// @param headers Headers info.
- (void)setHttpHeader:(NSDictionary<NSString *, NSString *> *)headers;

/// Set play media stream type.
///
/// Available since: 3.10.0
/// Description: Configure the media stream type to be played. You can only play video streams or audio streams. This will take effect during the life cycle of the media player.
/// Use cases: When only the video stream or audio stream needs to be played.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
/// Caution: Changing the media stream type during playing will take effect in the next playing.
///
/// @param streamType Stream type.
- (void)setPlayMediaStreamType:(ZegoMediaStreamType)streamType;

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
- (void)enableLiveAudioEffect:(BOOL)enable mode:(ZegoLiveAudioEffectMode)mode;

/// Enable local caching of http/https network resources.
///
/// Available since: 3.12.0
/// Description: When playing http/https network resources, when the local cache is enabled, the network resources will be saved locally and the cache information will be called back through [onMediaPlayerLocalCache].
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer]. Called after [loadResource] or during playback, it will take effect next playback.
/// Caution: Only http/https single file type network resources are supported. If there is a [seek] operation during playback, the cache will fail.
///
/// @param enable Whether to enable local caching.
/// @param cacheDir Cache dir. If left blank, the directory specified internally by SDK will be used.
- (void)enableLocalCache:(BOOL)enable cacheDir:(NSString *)cacheDir;

/// Enable the view mirror.
///
/// Available since: 3.14.0
/// Description: Enable view mirror.
/// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
///
/// @param enable Whether to enable view mirror.
- (void)enableViewMirror:(BOOL)enable;

/// Get playback statistics.
///
/// Available since: 3.12.0
/// Description: Get current playback statistics to monitor whether decoding and rendering anomalies occur in the player.
/// Use cases: Typically used in cloud-based media player scenarios.
/// When to call: Invoke after the [loadResource] callback succeeds.
- (ZegoMediaPlayerStatisticsInfo *)getPlaybackStatistics;

/// This function is unavaialble.
///
/// Please use the [createMediaPlayer] function in ZegoExpressEngine class instead.
+ (instancetype)new NS_UNAVAILABLE;

/// This function is unavaialble.
///
/// Please use the [createMediaPlayer] function in ZegoExpressEngine class instead.
- (instancetype)init NS_UNAVAILABLE;

/// [Deprecated] Setting up the specific voice changer parameters.
///
/// @deprecated Deprecated since 3.15.0, please use [enableVoiceChanger] instead.
/// @param param Voice changer parameters
/// @param audioChannel The audio channel to be voice changed
- (void)setVoiceChangerParam:(ZegoVoiceChangerParam *)param
                audioChannel:(ZegoMediaPlayerAudioChannel)audioChannel DEPRECATED_ATTRIBUTE;

@end

@interface ZegoAudioEffectPlayer : NSObject

/// Set audio effect player event handler.
///
/// Available since: 1.16.0
/// Description: Set audio effect player event handler.
/// When to call: It can be called after [createAudioEffectPlayer].
/// Restrictions: None.
/// Related APIs: [createAudioEffectPlayer].
///
/// @param handler event handler for audio effect player.
- (void)setEventHandler:(nullable id<ZegoAudioEffectPlayerEventHandler>)handler;

/// Start playing audio effect.
///
/// Available since: 1.16.0
/// Description: Start playing audio effect. The default is only played once and is not mixed into the publishing stream, if you want to change this please modify [config] param.
/// Use cases: When you need to play short sound effects, such as applause, cheers, etc., you can use this interface to achieve, and further configure the number of plays through the [config] parameter, and mix the sound effects into the push stream.
/// When to call: It can be called after [createAudioEffectPlayer].
/// Restrictions: None.
///
/// @param audioEffectID Description: ID for the audio effect. The SDK uses audioEffectID to control the playback of sound effects. The SDK does not force the user to pass in this parameter as a fixed value. It is best to ensure that each sound effect can have a unique ID. The recommended methods are static self-incrementing ID or the hash of the incoming sound effect file path.
/// @param path The absolute path of the local resource. <br>Value range: "assets://"、"ipod-library://" and network url are not supported. Set path as nil or "" if resource is loaded already using [loadResource].
/// @param config Audio effect playback configuration. <br>Default value: Set nil will only be played once, and will not be mixed into the publishing stream.
- (void)start:(unsigned int)audioEffectID
         path:(nullable NSString *)path
       config:(nullable ZegoAudioEffectPlayConfig *)config;

/// Stop playing audio effect.
///
/// Available since: 1.16.0
/// Description: Stop playing the specified audio effect [audioEffectID].
/// When to call: The specified [audioEffectID] is [start].
/// Restrictions: None.
///
/// @param audioEffectID ID for the audio effect.
- (void)stop:(unsigned int)audioEffectID;

/// Pause playing audio effect.
///
/// Available since: 1.16.0
/// Description: Pause playing the specified audio effect [audioEffectID].
/// When to call: The specified [audioEffectID] is [start].
/// Restrictions: None.
///
/// @param audioEffectID ID for the audio effect.
- (void)pause:(unsigned int)audioEffectID;

/// Resume playing audio effect.
///
/// Available since: 1.16.0
/// Description: Resume playing the specified audio effect [audioEffectID].
/// When to call: The specified [audioEffectID] is [pause].
/// Restrictions: None.
///
/// @param audioEffectID ID for the audio effect.
- (void)resume:(unsigned int)audioEffectID;

/// Stop playing all audio effect.
///
/// Available since: 1.16.0
/// Description: Stop playing all audio effect.
/// When to call: Some audio effects are Playing.
/// Restrictions: None.
- (void)stopAll;

/// Pause playing all audio effect.
///
/// Available since: 1.16.0
/// Description: Pause playing all audio effect.
/// When to call: It can be called after [createAudioEffectPlayer].
/// Restrictions: None.
- (void)pauseAll;

/// Resume playing all audio effect.
///
/// Available since: 1.16.0
/// Description: Resume playing all audio effect.
/// When to call: It can be called after [pauseAll].
/// Restrictions: None.
- (void)resumeAll;

/// Set the specified playback progress.
///
/// Available since: 1.16.0
/// Description: Set the specified audio effect playback progress. Unit is millisecond.
/// When to call: The specified [audioEffectID] is[start], and not finished.
/// Restrictions: None.
///
/// @param millisecond Point in time of specified playback progress.
/// @param audioEffectID ID for the audio effect.
/// @param callback The result of seek.
- (void)seekTo:(unsigned long long)millisecond
    audioEffectID:(unsigned int)audioEffectID
         callback:(nullable ZegoAudioEffectPlayerSeekToCallback)callback;

/// Set volume for a single audio effect. Both the local play volume and the publish volume are set.
///
/// Available since: 1.16.0
/// Description: Set volume for a single audio effect. Both the local play volume and the publish volume are set.
/// When to call: The specified [audioEffectID] is [start].
/// Restrictions: None.
///
/// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
/// @param audioEffectID ID for the audio effect.
- (void)setVolume:(int)volume audioEffectID:(unsigned int)audioEffectID;

/// Set local play volume for a single audio effect.
///
/// Available since: 3.11.0
/// Description: Set local play volume for a single audio effect.
/// When to call: The specified [audioEffectID] is [start].
/// Restrictions: None.
///
/// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
/// @param audioEffectID ID for the audio effect.
- (void)setPlayVolume:(int)volume audioEffectID:(unsigned int)audioEffectID;

/// Set publish volume for a single audio effect.
///
/// Available since: 3.11.0
/// Description: Set publish volume for a single audio effect.
/// When to call: The specified [audioEffectID] is [start].
/// Restrictions: None.
///
/// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
/// @param audioEffectID ID for the audio effect.
- (void)setPublishVolume:(int)volume audioEffectID:(unsigned int)audioEffectID;

/// Set volume for all audio effect. Both the local play volume and the publish volume are set.
///
/// Available since: 1.16.0
/// Description: Set volume for all audio effect. Both the local play volume and the publish volume are set.
/// When to call: It can be called after [createAudioEffectPlayer].
/// Restrictions: None.
///
/// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
- (void)setVolumeAll:(int)volume;

/// Set local play volume for all audio effect.
///
/// Available since: 3.11.0
/// Description: Set local play volume for all audio effect.
/// When to call: It can be called after [createAudioEffectPlayer].
/// Restrictions: None.
///
/// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
- (void)setPlayVolumeAll:(int)volume;

/// Set publish volume for all audio effect.
///
/// Available since: 3.11.0
/// Description: Set publish volume for all audio effect.
/// When to call: It can be called after [createAudioEffectPlayer].
/// Restrictions: None.
///
/// @param volume Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
- (void)setPublishVolumeAll:(int)volume;

/// Set the playback speed for a given audio effect. Both the local play speed and the publish speed are set. (separate settings are not supported).
///
/// Available since: 2.18.0
/// Description: Set the playback speed for a given audio effect. Both the local play speed and the publish speed are set. (separate settings are not supported).
/// When to call: The specified [audioEffectID] is [start].
/// Restrictions: None.
///
/// @param speed The speed of play. <br>Value range: The range is 0.5 ~ 2.0. <br>Default value: The default is 1.0.
/// @param audioEffectID ID for the audio effect.
- (void)setPlaySpeed:(float)speed audioEffectID:(unsigned int)audioEffectID;

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
- (unsigned long long)getTotalDuration:(unsigned int)audioEffectID;

/// Get current playback progress.
///
/// Available since: 1.16.0
/// Description: Get current playback progress of the specified audio effect. Unit is millisecond.
/// When to call: You should invoke this function after the audio effect resource already loaded, otherwise the return value is 0.
/// Restrictions: None.
/// Related APIs: [start], [loadResource].
///
/// @param audioEffectID ID for the audio effect.
- (unsigned long long)getCurrentProgress:(unsigned int)audioEffectID;

/// Load audio effect resource.
///
/// Available since: 1.16.0
/// Description: Load audio effect resource.
/// Use cases: In a scene where the same sound effect is played frequently, the SDK provides the function of preloading the sound effect file into the memory in order to optimize the performance of repeatedly reading and decoding the file.
/// When to call: It can be called after [createAudioEffectPlayer].
/// Restrictions: Preloading supports loading up to 15 sound effect files at the same time, and the duration of the sound effect files cannot exceed 30s, otherwise an error will be reported when loading.
///
/// @param path the absolute path of the audio effect resource and cannot be nil or "". <br>Value range: "assets://"、"ipod-library://" and network url are not supported.
/// @param audioEffectID ID for the audio effect.
/// @param callback load audio effect resource result.
- (void)loadResource:(NSString *)path
       audioEffectID:(unsigned int)audioEffectID
            callback:(nullable ZegoAudioEffectPlayerLoadResourceCallback)callback;

/// Unload audio effect resource.
///
/// Available since: 1.16.0
/// Description: Unload the specified audio effect resource.
/// When to call: After the sound effects are used up, related resources can be released through this function; otherwise, the SDK will release the loaded resources when the AudioEffectPlayer instance is destroyed.
/// Restrictions: None.
/// Related APIs: [loadResource].
///
/// @param audioEffectID ID for the audio effect loaded.
- (void)unloadResource:(unsigned int)audioEffectID;

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
- (void)updatePosition:(unsigned int)audioEffectID position:(const float[_Nonnull 3])position;

/// Get audio effect player index.
///
/// Available since: 1.16.0
/// Description: Get audio effect player index.
/// When to call: It can be called after [createAudioEffectPlayer].
/// Restrictions: None.
///
/// @return Audio effect player index.
- (NSNumber *)getIndex;

/// This function is unavaialble.
///
/// Please use the [createAudioEffectPlayer] function in ZegoExpressEngine class instead.
+ (instancetype)new NS_UNAVAILABLE;

/// This function is unavaialble
///
/// Please use the [createAudioEffectPlayer] function in ZegoExpressEngine class instead.
- (instancetype)init NS_UNAVAILABLE;

@end

@interface ZegoMediaDataPublisher : NSObject

/// Set event callback handler for the media data publisher.
///
/// Available since: 2.17.0
/// Description: Set event callback handler for media data publisher.
/// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
/// When to call: After calling the [createMediaDataPublisher] function to create a media data publisher.
/// Caution: This API used to be named [setMediaDataPublisherEventHandler] before version 3.8.0. Please refer to [Upgrade guide v3.8.0+](https://docs.zegocloud.com/en/16413.html).
///
/// @param handler The event handler
- (void)setEventHandler:(nullable id<ZegoMediaDataPublisherEventHandler>)handler;

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
- (void)addMediaFilePath:(NSString *)path isClear:(BOOL)isClear;

/// Clear all the status in this media data publisher, so that it can be restarted next time.
///
/// Available since: 2.17.0
/// Description: When you need to re-publish stream and do not need to continue publishing from the previous publish queue, you can call this function to reset this media data publisher's state.
/// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
/// Caution: When the developer calls [logoutRoom], the state is automatically reset.
- (void)reset;

/// Set the delay time of video playback.
///
/// Available since: 2.17.0
/// Description: When this value is set, when publishing video file stream, the SDK will permanently delay the video to the set time value before sending.
/// Use cases: Mainly used to correct the fixed audio and picture asynchronous phenomenon that occurs during streaming.
/// When to call: After calling the [createMediaDataPublisher] function to create a media data publisher.
///
/// @param delayTime Video playback time.Required: Yes.Value range: [0, 100] ms.
- (void)setVideoSendDelayTime:(int)delayTime;

/// Specify the starting point of the current video file publishing.
///
/// Available since: 2.17.0
/// Description: Specify the starting point of the current video file publishing.
/// When to call: Called at any point between [OnMediaDataPublisherFileOpen] and [OnMediaDataPublisherFileClose]. For example: this function can be called directly in the [OnMediaDataPublisherFileOpen] callback.
///
/// @param millisecond The timestamp of the start of streaming (relative to the timestamp of the file currently being published, the starting value is 0). the unit is milliseconds
- (void)seekTo:(unsigned long long)millisecond;

/// Get the total duration of the current file.
///
/// Available since: 2.17.0
/// Description: Get the total duration of the current file, in milliseconds.
/// When to call: After [onMediaDataPublisherFileDataBegin] callback.
///
/// @return The total duration of the current file.
- (unsigned long long)getTotalDuration;

/// Get the playing progress of the current file.
///
/// Available since: 2.17.0
/// Description: Get the playing progress of the current file, in milliseconds.
/// When to call: After received the [onMediaDataPublisherFileDataBegin] callback.
///
/// @return The playing progress of the current file.
- (unsigned long long)getCurrentDuration;

/// Get the channel index of the media data publisher.
///
/// Available since: 3.4.0
/// Description: Get the channel index of the media data publisher.
- (NSNumber *)getIndex;

@end

@interface ZegoRangeAudio : NSObject

/// set range audio event handler.
///
/// Available since: 2.11.0
/// Description: Set the callback function of the range audio module, which can receive the callback notification of the microphone on state [onRangeAudioMicrophoneStateUpdate].
/// Use case: Used to monitor the connection status of the current microphone.
/// When to call: After initializing the range audio [createRangeAudio].
///
/// @param handler The object used to receive range audio callbacks.
- (void)setEventHandler:(nullable id<ZegoRangeAudioEventHandler>)handler;

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
- (void)setAudioReceiveRange:(float)range;

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
- (int)setAudioReceiveRangeWithParam:(ZegoReceiveRangeParam *)param;

/// Set the frequency of real-time update locations within the SDK.
///
/// Available since: 2.21.0
/// Description: Set the frequency of real-time update locations within the SDK min 15 ms.
/// Use case: After setting the update position, the sensitivity of audio gradient is very high.
/// Default value: 100 ms.
/// When to call: After initializing the range audio [createRangeAudio].
///
/// @param frequency the frequency, the value must be greater than 15 ms.
- (void)setPositionUpdateFrequency:(int)frequency;

/// Set range voice volume.
///
/// Available since: 2.23.0
/// Description: Set range voice volume.
/// Use case: This interface allows you to increase or decrease the volume of a range voice stream when the user calls [startPlayingStream] and pulls another stream.
/// Default value: 100.
/// When to call: After initializing the range audio [createRangeAudio].
///
/// @param volume volume, [0,200].
- (void)setRangeAudioVolume:(int)volume;

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
- (void)setStreamVocalRange:(NSString *)streamID vocalRange:(float)vocalRange;

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
- (int)setStreamVocalRangeWithParam:(NSString *)streamID param:(ZegoVocalRangeParam *)param;

/// Update the location of the flow.
///
/// Available since: 2.23.0
/// Description: Set range voice volume.
/// Use case: When the user calls [startPlayingStream] to pull another stream, call [setStreamVocalRange] to set the stream's voice position, then call this interface to set the stream's position, so that the stream also has the range voice effect.
/// When to call: After initializing the range audio [createRangeAudio] and after [startPlayingStream].
///
/// @param streamID play stream id.
/// @param position The unit vector of the front axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
- (void)updateStreamPosition:(NSString *)streamID position:(float[_Nonnull 3])position;

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
- (void)updateSelfPosition:(float[_Nonnull 3])position
               axisForward:(float[_Nonnull 3])axisForward
                 axisRight:(float[_Nonnull 3])axisRight
                    axisUp:(float[_Nonnull 3])axisUp;

/// Add or update audio source position information.
///
/// Available since: 2.11.0
/// Description: Set the position of the audio source corresponding to the userID on the game map in the room, so that the SDK can calculate the distance and orientation of the listener to the audio source.
/// Use case: Update the position of the voice user in the game map coordinates.
/// When to call: Call [loginRoom] to call after logging in to the room, and the recorded audio source information will be cleared after logging out of the room.
///
/// @param userID The userID of the sound source.
/// @param position The coordinates of the speaker in the world coordinate system. The parameter is a float array of length 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
- (void)updateAudioSource:(NSString *)userID position:(float[_Nonnull 3])position;

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
- (void)enableSpatializer:(BOOL)enable;

/// Turn the microphone on or off.
///
/// Available since: 2.11.0
/// Description: When enable is `YES`, turn on the microphone and push audio stream; when it is `NO`, turn off the microphone and stop pushing audio stream.
/// Use case: The user turns on or off the microphone to communicate in the room.
/// Default value: When this function is not called, the microphone is turned off by default.
/// When to call: After initializing the range audio [createRangeAudio] and login room [loginRoom].
/// Caution: Turning on the microphone will automatically use the main channel to push the audio stream.
/// Related callbacks: Get the microphone switch state change through the callback [onRangeAudioMicrophoneStateUpdate].
///
/// @param enable Whether to turn on the microphone.
- (void)enableMicrophone:(BOOL)enable;

/// Turn the speaker on or off.
///
/// Available since: 2.11.0
/// Description: When enable is `YES`, turn on the speaker and play audio stream; when it is `NO`, turn off the speaker and stop playing audio stream.
/// Use case: The user turns on or off the speaker to communicate in the room.
/// Default value: When this function is not called, the speaker is turned off by default.
/// When to call: After initializing the range audio [createRangeAudio] and login room [loginRoom].
/// Caution: Turning on the speaker will automatically pull the audio stream in the room.
///
/// @param enable Whether to turn on the speaker.
- (void)enableSpeaker:(BOOL)enable;

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
- (void)setRangeAudioMode:(ZegoRangeAudioMode)mode;

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
- (void)setRangeAudioCustomMode:(ZegoRangeAudioSpeakMode)speakMode
                     listenMode:(ZegoRangeAudioListenMode)listenMode;

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
- (void)setTeamID:(NSString *)teamID;

/// Whether can receive the audio data of the specified user.
///
/// Available since: 2.16.0
/// Description: In the process of real-time audio and video interaction, local users can use this function to control whether to receive audio data from designated remote users when pulling streams as needed. When the developer does not receive the audio receipt, the hardware and network overhead can be reduced.
/// Use cases: When developers need to quickly close and restore remote audio, they can call this function to enhance the interactive experience.
/// Default value: The default is `NO`, which means to receive audio data from all users.
/// When to call: After initializing the range audio [createRangeAudio].
/// Caution: This function is valid only when the [muteAllPlayStreamAudio] function is set to `NO`.
/// Related APIs: You can call the [muteAllPlayStreamAudio] function to control whether to receive all audio data. 1. When the [muteAllPlayStreamAudio(YES)] function is called, it takes effect globally, that is, local users will be prohibited from receiving audio data from all remote users. At this time, the [muteUser] function will not take effect regardless of whether the [muteUser] function is called before or after [muteAllPlayStreamAudio]. 2. When the [muteAllPlayStreamAudio(NO)] function is called, the local user can receive the audio data of all remote users. At this time, the [muteUser] function can be used to control whether to receive the audio data of the specified user. Calling the [muteUser(userID, YES)] function allows the local user to receive audio data other than the `userID`; calling the [muteUser(userID, NO)] function allows the local user to receive the audio data of the `userID`.
///
/// @param userID User ID.
/// @param mute Whether it can receive the audio data of the specified remote user, "YES" means prohibition, "NO" means receiving, the default value is "NO".
- (void)muteUser:(NSString *)userID mute:(BOOL)mute;

/// This function is unavaialble.
///
/// Please use the [createRangeAudio] function in ZegoExpressEngine class instead.
+ (instancetype)new NS_UNAVAILABLE;

/// This function is unavaialble
///
/// Please use the [createRangeAudio] function in ZegoExpressEngine class instead.
- (instancetype)init NS_UNAVAILABLE;

@end

@interface ZegoCopyrightedMusic : NSObject

/// set copyrighted music event handler.
///
/// Available since: 2.13.0
/// Description: Set the callback function of the copyrighted music module, which can receive callback notifications related to song playback status .
/// When to call: After create the copyrighted music [createCopyrightedMusic].
///
/// @param handler The object used to receive copyrighted music callbacks.
- (void)setEventHandler:(nullable id<ZegoCopyrightedMusicEventHandler>)handler;

/// Initialize the copyrighted music module.
///
/// Available since: 2.13.0
/// Description: Initialize the copyrighted music so that you can use the function of the copyrighted music later.
/// When to call: After call [createCopyrightedMusic] and [loginRoom].
/// Caution: 1. The real user information must be passed in, otherwise the song resources cannot be obtained for playback. 2. The user ID set when initializing copyrighted music needs to be the same as the user ID set when logging in to the room.
///
/// @param config the copyrighted music configuration.
/// @param callback init result
- (void)initCopyrightedMusic:(ZegoCopyrightedMusicConfig *)config
                    callback:(nullable ZegoCopyrightedMusicInitCallback)callback;

/// Get cache size.
///
/// Available since: 2.13.0
/// Description: When using this module, some cache files may be generated, and the size of the cache file can be obtained through this interface.
/// Use case: Used to display the cache size of the App.
/// When to call: After initializing the copyrighted music [createCopyrightedMusic].
///
/// @return cache file size, in byte.
- (unsigned long long)getCacheSize;

/// Clear cache.
///
/// Available since: 2.13.0
/// Description: When using this module, some cache files may be generated, which can be cleared through this interface.
/// Use case: Used to clear the cache of the App.
/// When to call: After initializing the copyrighted music [createCopyrightedMusic].
- (void)clearCache;

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
- (void)sendExtendedRequest:(NSString *)command
                     params:(NSString *)params
                   callback:(nullable ZegoCopyrightedMusicSendExtendedRequestCallback)callback;

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
- (void)getLrcLyric:(NSString *)songID
           vendorID:(ZegoCopyrightedMusicVendorID)vendorID
           callback:(nullable ZegoCopyrightedMusicGetLrcLyricCallback)callback;

/// Get lyrics in lrc format.
///
/// Available since: 3.12.0
/// Description: Get lyrics in lrc format, support parsing lyrics line by line.
/// Use case: Used to display lyrics line by line.
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
///
/// @param config The configuration of get lyric.
/// @param callback get lyrics result
- (void)getLrcLyricWithConfig:(ZegoCopyrightedMusicGetLyricConfig *)config
                     callback:(nullable ZegoCopyrightedMusicGetLrcLyricCallback)callback;

/// Get lyrics in krc format.
///
/// Available since: 2.13.0
/// Description: Get verbatim lyrics, support parsing lyrics word by word.
/// Use case: Used to display lyrics word by word.
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
///
/// @param krcToken The krcToken obtained when calling [requestResource] for accompaniment or climax clips, or when obtaining shared resources through the [getSharedResource] interface. For more details, please refer to https://doc-zh.zego.im/article/15079#2_2
/// @param callback get lyrics result.
- (void)getKrcLyricByToken:(NSString *)krcToken
                  callback:(nullable ZegoCopyrightedMusicGetKrcLyricByTokenCallback)callback;

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
- (void)requestResource:(ZegoCopyrightedMusicRequestConfig *)config
                   type:(ZegoCopyrightedMusicResourceType)type
               callback:(nullable ZegoCopyrightedMusicRequestResourceCallback)callback;

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
- (void)requestResource:(ZegoCopyrightedMusicRequestConfigV2 *)config
               callback:(nullable ZegoCopyrightedMusicRequestResourceCallback)callback;

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
- (void)getSharedResource:(ZegoCopyrightedMusicGetSharedConfig *)config
                     type:(ZegoCopyrightedMusicResourceType)type
                 callback:(nullable ZegoCopyrightedMusicGetSharedResourceCallback)callback;

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
- (void)getSharedResource:(ZegoCopyrightedMusicGetSharedConfigV2 *)config
                 callback:(nullable ZegoCopyrightedMusicGetSharedResourceCallback)callback;

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
- (void)download:(NSString *)resourceID
        callback:(nullable ZegoCopyrightedMusicDownloadCallback)callback;

/// Cancel download of music resource.
///
/// Available since: 3.11.0
/// Description: Cancel download of music resource.
/// Use case: After starting to download music resources, use this interface to cancel the corresponding download task.
/// When to call: After starting the download [download].
/// Caution: When a valid resourceID is passed in, only unfinished download tasks will be cancelled. When empty is passed in, all unfinished download tasks will be canceled.
///
/// @param resourceID the resource ID corresponding to the song or accompaniment.
- (void)cancelDownload:(NSString *)resourceID;

/// Query the resource's cache is existed or not.
///
/// Available since: 3.9.0
/// Description: Query the resource is existed or not, query the Yinsuda resource cache by default
/// Use case: Can be used to check the resource's cache is existed or not
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
///
/// @param config The configuration of querying cache.
- (BOOL)queryCache:(ZegoCopyrightedMusicQueryCacheConfig *)config;

/// Query the resource's cache is existed or not.
///
/// Available since: 3.12.0
/// Description: Query the resource is existed or not, query the Yinsuda resource cache by default
/// Use case: Can be used to check the resource's cache is existed or not
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
///
/// @param config The configuration of querying cache.
- (BOOL)queryCacheV2:(ZegoCopyrightedMusicQueryCacheConfigV2 *)config;

/// Get the playing time of a song or accompaniment file.
///
/// Available since: 2.13.0
/// Description: Get the playing time of a song or accompaniment file.
/// Use case: Can be used to display the playing time information of the song or accompaniment on the view.
/// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
///
/// @param resourceID the resource ID corresponding to the song or accompaniment.
- (unsigned long long)getDuration:(NSString *)resourceID;

/// Set the difficulty level of scoring.
///
/// Available since: 2.22.0
/// Description: Users can set the scoring difficulty level through this function.
/// When to call: After calling [initCopyrightedMusic] to initialize copyrighted music successfully, call [startScore] to start scoring.
/// Default value: When this function is not called, the difficulty level of scoring is 4.
/// Restrictions: This function does not support dynamic settings. After calling this function successfully, the next call to [startScore] will take effect.
///
/// @param level The difficulty level of scoring. The level ranges from 0 to 4. The scoring difficulty decreases from 0 to 4.
- (void)setScoringLevel:(int)level;

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
- (int)startScore:(NSString *)resourceID pitchValueInterval:(int)pitchValueInterval;

/// Pause scoring.
///
/// Available since: 2.15.0
/// Description: Pause ongoing scoring,will stop the [OnCurrentPitchValueUpdate] callback.
/// Use case: You can call this interface to pause the scoring function while scoring.
/// When to call: It can be called while grading.
///
/// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
- (int)pauseScore:(NSString *)resourceID;

/// Resume scoring.
///
/// Available since: 2.15.0
/// Description: Resume currently paused scoring.
/// Use case: When there is currently paused scoring, this interface can be called to resume the scoring function.
/// When to call: It can be called when there is currently a paused scoring.
///
/// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
- (int)resumeScore:(NSString *)resourceID;

/// Stop scoring.
///
/// Available since: 2.15.0
/// Description: End the current rating.The [OnCurrentPitchValueUpdate] callback will be stopped, but the average or total score can still be obtained normally.
/// Use case: You can call this interface to end the scoring while scoring.
/// When to call: It can be called while grading.
///
/// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
- (int)stopScore:(NSString *)resourceID;

/// Reset scoring.
///
/// Available since: 2.15.0
/// Description: Reset the scores that have already been performed,The [OnCurrentPitchValueUpdate] callback will be stopped and the average or total score will be 0.
/// Use case: Often used in scenes where the same song is re-sung.
/// When to call: It can be called after scoring has been performed.
///
/// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
- (int)resetScore:(NSString *)resourceID;

/// Get the score of the previous sentence.
///
/// Available since: 2.15.0
/// Description: Get the score of the previous sentence.
/// Use case: Can be used to display the score of each sentence on the view.
/// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
///
/// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
- (int)getPreviousScore:(NSString *)resourceID;

/// Get average score.
///
/// Available since: 2.15.0
/// Description: Get the average score.
/// Use case: Can be used to display the average score on the view.
/// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
///
/// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
- (int)getAverageScore:(NSString *)resourceID;

/// Get total score .
///
/// Available since: 2.15.0
/// Description: Get the total score.
/// Use case: Can be used to display the total score on the view.
/// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
///
/// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
- (int)getTotalScore:(NSString *)resourceID;

/// Get full score .
///
/// Available since: 3.0.2
/// Description: Get the full score.
/// Use case: Can be used to display the full score on the view.
/// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
///
/// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
- (int)getFullScore:(NSString *)resourceID;

/// Get standard pitch data.
///
/// Available since: 2.15.0
/// Description: Get standard pitch data.
/// Use case: Can be used to display standard pitch lines on the view.
/// Caution: Only accompaniment or climactic clip assets have pitch lines.
///
/// @param resourceID the resource ID corresponding to the accompaniment or accompaniment clip.
/// @param callback get standard pitch data result.
- (void)getStandardPitch:(NSString *)resourceID
                callback:(nullable ZegoCopyrightedMusicGetStandardPitchCallback)callback;

/// Get real-time pitch data.
///
/// Available since: 2.15.0
/// Description: Get real-time pitch data.
/// Use case: Can be used to display real-time pitch lines on the view.
/// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
///
/// @param resourceID the resource ID corresponding to the song or accompaniment.
- (int)getCurrentPitch:(NSString *)resourceID;

/// This function is unavaialble.
///
/// Please use the [createCopyrightedMusic] function in ZegoExpressEngine class instead.
+ (instancetype)new NS_UNAVAILABLE;

/// This function is unavaialble
///
/// Please use the [createCopyrightedMusic] function in ZegoExpressEngine class instead.
- (instancetype)init NS_UNAVAILABLE;

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
- (void)requestSong:(ZegoCopyrightedMusicRequestConfig *)config
           callback:(nullable ZegoCopyrightedMusicRequestSongCallback)callback DEPRECATED_ATTRIBUTE;

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
- (void)requestAccompaniment:(ZegoCopyrightedMusicRequestConfig *)config
                    callback:(nullable ZegoCopyrightedMusicRequestAccompanimentCallback)callback
    DEPRECATED_ATTRIBUTE;

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
- (void)requestAccompanimentClip:(ZegoCopyrightedMusicRequestConfig *)config
                        callback:
                            (nullable ZegoCopyrightedMusicRequestAccompanimentClipCallback)callback
    DEPRECATED_ATTRIBUTE;

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
- (void)getMusicByToken:(NSString *)shareToken
               callback:(nullable ZegoCopyrightedMusicGetMusicByTokenCallback)callback
    DEPRECATED_ATTRIBUTE;

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
- (void)getLrcLyric:(NSString *)songID
           callback:(nullable ZegoCopyrightedMusicGetLrcLyricCallback)callback DEPRECATED_ATTRIBUTE;

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
- (BOOL)queryCache:(NSString *)songID type:(ZegoCopyrightedMusicType)type DEPRECATED_ATTRIBUTE;

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
- (BOOL)queryCache:(NSString *)songID
              type:(ZegoCopyrightedMusicType)type
          vendorID:(ZegoCopyrightedMusicVendorID)vendorID DEPRECATED_ATTRIBUTE;

@end

@interface ZegoAudioVADClient : NSObject

/// Check if an audio packet contains speech.
///
/// Available since: 2.21.0
/// Description: Check if an audio packet contains speech.
/// When to call: It can be called after [createAudioVADClient].
/// Caution: ZegoAudioVADTypeSpeech will also be returned when the voice data is abnormal to ensure that the voice will not be missed.
///
/// @param data audio data.
/// @param dataLength audio data length.
/// @param sampleRate sampling rate.
/// @param channels number of channels.
/// @return voice detection results.
- (ZegoAudioVADType)update:(unsigned char *)data
                dataLength:(int)dataLength
                sampleRate:(int)sampleRate
                  channels:(int)channels;

/// Reset audio vad client instance.
///
/// Available since: 2.21.0
/// Description: Reset audio vad client instance.
/// When to call: It can be called after [createAudioVADClient].
///
/// @return reset results.
- (BOOL)reset;

@end

@interface ZegoRangeSceneStream : NSObject

/// set range scene stream event handler.
///
/// Available since: 3.0.0
/// Description: Set the callback function of the range scene stream module.
/// When to call: After getting range scene stream instance by [getRangeSceneStream].
///
/// @param handler The object used to receive range scene stream callbacks.
/// @return The result of set range scene stream event handler, YES: success, NO: fail.
- (BOOL)setEventHandler:(nullable id<ZegoRangeSceneStreamEventHandler>)handler;

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
- (int)setReceiveRange:(float)range;

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
- (int)setReceiveRangeWithParam:(ZegoReceiveRangeParam *)param;

/// Turn 3D spatial sound on or off.
///
/// Available: since 3.0.0
/// Description: After it is turned on, the audio of non-team members in the world will change spatially with the distance and direction from the person.
/// When to call: After [getRangeSceneStream].
/// Default value: Disable.
/// Restrictions: Media volume is required to use 3D spatial sound.
/// Caution: None.
///
/// @param enable Whether to turn 3D spatial sound on, YES: enable, NO: disable.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
- (int)enableRangeSpatializer:(BOOL)enable;

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
/// @param mute YES: do not receive the user's audio stream, NO: receive the user's audio stream.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
- (int)mutePlayAudio:(NSString *)userID mute:(BOOL)mute;

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
/// @param mute YES: do not receive the user's video stream, NO: receive the user's video stream.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
- (int)mutePlayVideo:(NSString *)userID mute:(BOOL)mute;

/// This function is unavaialble.
///
/// Please use the [getRangeSceneStream] function in ZegoRangeScene class instead.
+ (instancetype)new NS_UNAVAILABLE;

/// This function is unavaialble
///
/// Please use the [getRangeSceneStream] function in ZegoRangeScene class instead.
- (instancetype)init NS_UNAVAILABLE;

@end

@interface ZegoRangeSceneTeam : NSObject

/// set range scene team event handler.
///
/// Available since: 3.1.0
/// Description: Set the callback function of the range scene team module.
/// When to call: After getting range scene team instance by [getRangeSceneTeam].
///
/// @param handler The object used to receive range scene team callbacks.
/// @return The result of set range scene team event handler, YES: success, NO: fail.
- (BOOL)setEventHandler:(nullable id<ZegoRangeSceneTeamEventHandler>)handler;

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
- (void)joinTeam:(ZegoTeamParam *)config callback:(nullable ZegoRangeSceneJoinTeamCallback)callback;

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
- (void)leaveTeam:(unsigned int)teamID callback:(nullable ZegoRangeSceneLeaveTeamCallback)callback;

/// This function is unavaialble.
///
/// Please use the [getRangeSceneTeam] function in ZegoRangeScene class instead.
+ (instancetype)new NS_UNAVAILABLE;

/// This function is unavaialble
///
/// Please use the [getRangeSceneTeam] function in ZegoRangeScene class instead.
- (instancetype)init NS_UNAVAILABLE;

@end

@interface ZegoRangeSceneItem : NSObject

/// set range scene item event handler.
///
/// Available since: 3.1.0
/// Description: Set the callback function of the range scene item module.
/// When to call: After getting range scene item instance by [getRangeSceneItem].
///
/// @param handler The object used to receive range scene item callbacks.
/// @return The result of set range scene item event handler, YES: success, NO: fail.
- (BOOL)setEventHandler:(nullable id<ZegoRangeSceneItemEventHandler>)handler;

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
- (void)createItem:(ZegoItemParam *)param
          callback:(nullable ZegoRangeSceneCreateItemCallback)callback;

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
- (void)destroyItem:(long long)itemID callback:(nullable ZegoRangeSceneDestroyItemCallback)callback;

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
- (void)bindItem:(long long)itemID callback:(nullable ZegoRangeSceneBindItemCallback)callback;

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
- (void)unbindItem:(long long)itemID callback:(nullable ZegoRangeSceneUnbindItemCallback)callback;

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
/// @param callback The callback of update item status.
- (void)updateItemStatus:(long long)itemID
                position:(ZegoPosition *)position
                 channel:(unsigned int)channel
                  status:(NSData *)status
                callback:(nullable ZegoRangeSceneUpdateItemStatusCallback)callback;

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
/// @param callback The callback of update item next command.
- (void)updateItemCommand:(long long)itemID
                 position:(ZegoPosition *)position
                  channel:(unsigned int)channel
                  command:(NSData *)command
                 callback:(nullable ZegoRangeSceneUpdateItemCommandCallback)callback;

/// This function is unavaialble.
///
/// Please use the [getRangeSceneItem] function in ZegoRangeScene class instead.
+ (instancetype)new NS_UNAVAILABLE;

/// This function is unavaialble
///
/// Please use the [getRangeSceneItem] function in ZegoRangeScene class instead.
- (instancetype)init NS_UNAVAILABLE;

@end

@interface ZegoRangeScene : NSObject

/// Get a range scene stream instance.
///
/// Available since: 3.0.0
/// Description: Get a range scene stream instance.
/// Use case: Often used in meta world scenarios, users can use related functions by getting range scene stream instance objects.
/// When to call: After creating the range scene via [createRangeScene], before destroying the range scene via [destroyRangeScene].
/// Restrictions: None.
///
/// @return range scene stream instance.
- (ZegoRangeSceneStream *)getRangeSceneStream;

/// Get a range scene team instance.
///
/// Available since: 3.1.0
/// Description: Get a range scene team instance.
/// Use case: Often used in meta world scenarios, users can use related functions by getting range scene team instance objects.
/// When to call: After creating the range scene via [createRangeScene], before destroying the range scene via [destroyRangeScene].
/// Restrictions: None.
///
/// @return range scene team instance.
- (ZegoRangeSceneTeam *)getRangeSceneTeam;

/// Get a range scene item instance.
///
/// Available since: 3.1.0
/// Description: Get a range scene item instance.
/// Use case: Often used in meta world scenarios, users can use related functions by getting range scene item instance objects.
/// When to call: After creating the range scene via [createRangeScene], before destroying the range scene via [destroyRangeScene].
/// Restrictions: None.
///
/// @return range scene item instance.
- (ZegoRangeSceneItem *)getRangeSceneItem;

/// Get range scene instance handle.
///
/// Available since: 3.0.0
/// Description: Get range scene instance handle.
/// Use case: Used to publish stream to the scene.
/// When to call: After creating the range scene via [createRangeScene], before destroying the range scene via [destroyRangeScene].
/// Restrictions: None.
///
/// @return Range scene instance handle.
- (int)getRangeSceneHandle;

/// set range scene event handler.
///
/// Available since: 3.0.0
/// Description: Set the callback function of the range scene module.
/// When to call: After creating the range scene via [createRangeScene], before destroying the range scene via [destroyRangeScene].
///
/// @param handler The object used to receive range scene callbacks.
/// @return The result of set range scene event handler, YES: success, NO: fail.
- (BOOL)setEventHandler:(nullable id<ZegoRangeSceneEventHandler>)handler;

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
- (void)loginScene:(ZegoSceneParam *)param
          callback:(nullable ZegoRangeSceneLoginSceneCallback)callback;

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
- (void)logoutScene:(nullable ZegoRangeSceneLogoutSceneCallback)callback;

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
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
- (int)updateUserStatus:(ZegoPosition *)position
                channel:(unsigned int)channel
                 status:(NSData *)status;

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
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
- (int)updateUserCommand:(ZegoPosition *)position
                 channel:(unsigned int)channel
                 command:(NSData *)command;

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
- (int)updateUserPosition:(ZegoPosition *)position;

/// Get user count in scene.
///
/// Available: since 3.0.0
/// Description: Developers can call this function to get user count in scene.
/// When to call: After [LoginScene], before [LogoutScene].
/// Default value: nil.
/// Restrictions: None.
/// Caution: None.
///
/// @param callback The callback of get user count in scene.
- (void)getUserCount:(nullable ZegoRangeSceneGetUserCountCallback)callback;

/// Get user list in view.
///
/// Available: since 3.0.0
/// Description: Developers can call this function to get user list in view.
/// When to call: After [LoginScene], before [LogoutScene].
/// Default value: nil.
/// Restrictions: None.
/// Caution: None.
///
/// @param callback The callback of get user list in view.
- (void)getUserListInView:(nullable ZegoRangeSceneGetUserListInViewCallback)callback;

/// Send custom command.
///
/// Available: since 3.0.0
/// Description: Developers can call this function to send custom command.
/// When to call: After [LoginScene], before [LogoutScene].
/// Default value: nil.
/// Restrictions: None.
/// Caution: None.
///
/// @param command Custom command.
/// @param callback The callback of send custom command.
- (void)sendCustomCommand:(NSData *)command
                 callback:(nullable ZegoRangeSceneSendCustomCommandCallback)callback;

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
- (int)renewToken:(NSString *)token;

/// Set range scene stream config.
///
/// Available: since 3.2.0
/// Description: Developers can call this function to set range scene stream config.
/// When to call: After [createRangeScene].
/// Default value: nil.
/// Restrictions: None.
/// Caution: None.
///
/// @param config Range scene stream config.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
- (int)setStreamConfig:(ZegoSceneStreamConfig *)config;

/// This function is unavaialble.
///
/// Please use the [createRangeScene] function in ZegoExpressEngine class instead.
+ (instancetype)new NS_UNAVAILABLE;

/// This function is unavaialble
///
/// Please use the [createRangeScene] function in ZegoExpressEngine class instead.
- (instancetype)init NS_UNAVAILABLE;

@end

@interface ZegoScreenCaptureSource : NSObject

#if TARGET_OS_OSX
/// Sets up the screen capture source event handler.
///
/// Available since: 3.1.0
/// Description: Set up screen capture source callback to monitor callbacks such as capture data, etc.
/// When to call:After create the [ZegoScreenCaptureSource] instance.
/// Restrictions: None.
/// Caution: Calling this function will overwrite the callback set by the last call to this function.
///
/// @param handler Event handler for real-time sequential data.
- (void)setEventHandler:(nullable id<ZegoScreenCaptureSourceEventHandler>)handler;
#endif

#if TARGET_OS_OSX
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
- (void)updateCaptureSource:(unsigned int)sourceId
                 sourceType:(ZegoScreenCaptureSourceType)sourceType;
#endif

#if TARGET_OS_OSX
/// Start screen capture.
///
/// Available since: 3.1.0
/// Description: Start screen capture.
/// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
- (void)startCapture;
#endif

#if TARGET_OS_OSX
/// Stop screen capture.
///
/// Available since: 3.1.0
/// Description: Stop screen capture.
- (void)stopCapture;
#endif

#if TARGET_OS_OSX
/// Gets the rectangle of the screen capture source.
///
/// Available since: 3.6.0
/// Description: Gets the rectangle of the screen capture source.
/// Restrictions: Called after starting [startScreenCapture] acquisition, only support in Windows/macOS.
///
/// @return Rect information about the capture resource.
- (CGRect)getCaptureSourceRect;
#endif

#if TARGET_OS_OSX
/// Update the area captured by the screen.
///
/// Available since: 3.1.0
/// Description: Update the area captured by the screen.
/// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
/// Restrictions: Only support in Windows/macOS.
///
/// @param rect The position of the area to be captured relative to the entire screen or window.
- (void)updateCaptureRegion:(CGRect)rect;
#endif

#if TARGET_OS_OSX
/// Update the streaming area of ​​the screen capture.
///
/// Available since: 3.8.0
/// Description: Update the streaming area of ​​the screen capture.
/// When to call: After updating the screen capture source [updateScreenCaptureSource].
/// Restrictions: Every time the screen capture source [updateScreenCaptureSource] is updated, the area will be cleared and needs to be reset, only support in Windows/macOS.
/// Caution: The set area cannot exceed the size returned by [onAvailableFrame], otherwise the setting will fail and the original frame will be pushed. The failure of the streaming area will be notified through [onExceptionOccurred].
///
/// @param rect The position of the area to be pushed relative to the upper left corner of the capture screen, the actual capture screen size can be obtained through [onAvailableFrame].
- (void)updatePublishRegion:(CGRect)rect;
#endif

#if TARGET_OS_OSX
/// Sets the filtered list of windows.
///
/// Available since: 3.1.0
/// Description: Specify a list of windows, and filter these windows when capturing the screen, and not display them on the screen.
/// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
/// Restrictions: Only available on Windows/macOS.
///
/// @param list List of IDs to filter windows.
- (void)setExcludeWindowList:(NSArray<NSNumber *> *)list;
#endif

#if TARGET_OS_OSX
/// Whether to activate the promotion of the window to the foreground.
///
/// Available since: 3.1.0
/// Description: When the capture target is a window, set whether to activate the window to be displayed in the foreground during the first capture.
/// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
/// Restrictions: Only available on Windows/macOS.
///
/// @param active Whether to activate the window. YES to activate the window, NO to not activate the window, the default is YES.
- (void)enableWindowActivate:(BOOL)active;
#endif

#if TARGET_OS_OSX
/// Set whether to show the cursor
///
/// Available since: 3.1.0
/// Description: Set whether to show the cursor.
/// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
/// Restrictions: Only available on Windows/macOS.
///
/// @param visible Whether to show the cursor. YES to show the cursor, NO to not show the cursor, the default is NO.
- (void)enableCursorVisible:(BOOL)visible;
#endif

#if TARGET_OS_OSX
/// Set whether to highlight the capture area
///
/// Available since: 3.21.0
/// Description: Set whether to highlight the capture area.
/// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
/// Restrictions: Only available on Windows/macOS.
///
/// @param enable Whether to highlight the capture area. YES to highlight, NO to not highlight, the default is NO.
/// @param config Highlight capture area border configuration.
- (void)enableHightLight:(BOOL)enable config:(ZegoLayerBorderConfig *)config;
#endif

#if TARGET_OS_OSX
/// Get screen capture source index.
///
/// @return Index of the screen capture source.
- (NSNumber *)getIndex;
#endif

/// This function is unavaialble.
///
/// Please use the [createScreenCaptureSource] function in ZegoExpressEngine class instead.
+ (instancetype)new NS_UNAVAILABLE;

/// This function is unavaialble.
///
/// Please use the [createScreenCaptureSource] function in ZegoExpressEngine class instead.
- (instancetype)init NS_UNAVAILABLE;

@end

@interface ZegoReplayKitExt : NSObject

#if defined(TARGET_OS_IOS) && TARGET_OS_IOS
/// Create a screen capture extension singleton.
///
/// Available since: 3.1.0
/// Description: Create a screen sharing extension singleton for Broadcast upload Extension to send the capture screen data to the SDK.
///
/// @return Extension singleton instance.
+ (ZegoReplayKitExt *)sharedInstance;
#endif

#if defined(TARGET_OS_IOS) && TARGET_OS_IOS
/// Initialize the extension app.
///
/// Available since: 3.1.0
/// Use cases: It needs to be called in the [broadcastStartedWithSetupInfo] method in the implementation class of [RPBroadcastSampleHandler].
/// Description: If the App Group configuration is not enabled, this interface can be used for initialization.
///
/// @param delegate Screen capture extension application event notification callback. Passing [nil] means no callback notification will be received.
- (void)setupWithDelegate:(id<ZegoReplayKitExtHandler>)delegate API_AVAILABLE(ios(12.0));
#endif

#if defined(TARGET_OS_IOS) && TARGET_OS_IOS
/// Use App Group to initialize the extension process, which can provide better performance and stability when sharing screens.
///
/// Available since: 3.3.0
/// Use cases: It needs to be called in the [broadcastStartedWithSetupInfo] method in the implementation class of [RPBroadcastSampleHandler].
/// Description: Setting AppGroupID is used to share memory between app, which can improve the performance and stability of data transmission. At the same time, it is necessary to call the [setAppGroupID:] method to set the AppGroupID before starting [startScreenCapture] in the host App.
///
/// @param delegate Screen capture extension application event notification callback. Passing [nil] means no callback notification will be received.
/// @param groupID The host app and the extension app should belong to the same App Group, and the AppGroupID needs to be passed in here.
- (void)setupWithDelegate:(id<ZegoReplayKitExtHandler>)delegate
                 appGroup:(NSString *)groupID API_AVAILABLE(ios(12.0));
#endif

#if defined(TARGET_OS_IOS) && TARGET_OS_IOS
/// Finish screen capture.
///
/// Available since: 3.1.0
/// Use cases: When the screen recording is stopped through the system control center, RPBroadcastSampleHandler.broadcastFinished will be called back and called in the broadcastFinished method.
- (void)finished API_AVAILABLE(ios(12.0));
#endif

#if defined(TARGET_OS_IOS) && TARGET_OS_IOS
/// Media data sending method
///
/// Available since: 3.1.0
/// Use cases: It needs to be called in the processSampleBuffer: method in the implementation class of RPBroadcastSampleHandler.
///
/// @param sampleBuffer CMSampleBuffer object which contains either video or audio data.
/// @param sampleBufferType Determine's the type of the sample buffer defined by the RPSampleBufferType enum.
- (void)sendSampleBuffer:(CMSampleBufferRef)sampleBuffer
                withType:(RPSampleBufferType)sampleBufferType API_AVAILABLE(ios(12.0));
#endif

@end

@interface ZegoAIVoiceChanger : NSObject

/// Get AI voice changer instance index.
///
/// @return AI voice changer instance index.
- (int)getIndex;

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
- (void)setEventHandler:(nullable id<ZegoAIVoiceChangerEventHandler>)handler;

/// Initialize AI voice changer engine.
- (void)initEngine;

/// Update AI voice changer engine models.
- (void)update;

/// Get AI voice changer speaker list.
- (void)getSpeakerList;

/// Set AI voice changer speaker.
///
/// @param speakerID Speaker ID.
- (void)setSpeaker:(int)speakerID;

/// This function is unavailable.
///
/// Please use the [createAIVoiceChanger] function in ZegoExpressEngine class instead.
+ (instancetype)new NS_UNAVAILABLE;

/// This function is unavailable.
///
/// Please use the [createAIVoiceChanger] function in ZegoExpressEngine class instead.
+ (instancetype)init NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
