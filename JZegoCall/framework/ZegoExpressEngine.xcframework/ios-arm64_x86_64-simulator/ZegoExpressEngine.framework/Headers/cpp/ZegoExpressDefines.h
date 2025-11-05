
#ifndef __ZEGOEXPRESSDEFINES_H__
#define __ZEGOEXPRESSDEFINES_H__

#include "internal/ZegoInternalDefines.hpp"

namespace ZEGO {
namespace EXPRESS {

/// Room scenario.
enum ZegoScenario {
    /// @deprecated Legacy general scenario
    ZEGO_SCENARIO_GENERAL ZEGO_DEPRECATED_ENUM = 0,

    /// @deprecated Legacy communication scenario
    ZEGO_SCENARIO_COMMUNICATION ZEGO_DEPRECATED_ENUM = 1,

    /// @deprecated Legacy live broadcast scenario
    ZEGO_SCENARIO_LIVE ZEGO_DEPRECATED_ENUM = 2,

    /// Available since: 3.0.0. Description: The default (generic) scenario. If none of the following scenarios conform to your actual application scenario, this default scenario can be used.
    ZEGO_SCENARIO_DEFAULT = 3,

    /// Available since: 3.0.0. Description: Standard video call scenario, it is suitable for one-to-one video call scenarios.
    ZEGO_SCENARIO_STANDARD_VIDEO_CALL = 4,

    /// Available since: 3.0.0. Description: High quality video call scenario, it is similar to the standard video call scenario, but this scenario uses a higher video frame rate, bit rate, and resolution (540p) by default, which is suitable for video call scenario with high image quality requirements.
    ZEGO_SCENARIO_HIGH_QUALITY_VIDEO_CALL = 5,

    /// Available since: 3.0.0. Description: Standard chatroom scenario, suitable for multi-person pure voice calls (low data usage). Note: On the ExpressVideo SDK, the camera is not enabled by default in this scenario.
    ZEGO_SCENARIO_STANDARD_CHATROOM = 6,

    /// Available since: 3.0.0. Description: High quality chatroom scenario, it is similar to the standard chatroom scenario, but this scenario uses a higher audio bit rate than the standard chatroom scenario by default. It is suitable for multi-person pure voice call scenarios with high requirements on sound quality. Note: On the ExpressVideo SDK, the camera is not enabled by default in this scenario.
    ZEGO_SCENARIO_HIGH_QUALITY_CHATROOM = 7,

    /// Available since: 3.0.0. Description: Live broadcast scenario, it is suitable for one-to-many live broadcast scenarios such as shows, games, e-commerce, and large educational classes. The audio and video quality, fluency, and compatibility have been optimized. Note: Even in live broadcast scenarios, the SDK has no business "roles" (such as anchors and viewers), and all users in the room can publish and play streams.
    ZEGO_SCENARIO_BROADCAST = 8,

    /// Available since: 3.0.0. Description: Karaoke (KTV) scenario, it is suitable for real-time chorus and online karaoke scenarios, and has optimized delay, sound quality, ear return, echo cancellation, etc., and also ensures accurate alignment and ultra-low delay when multiple people chorus.
    ZEGO_SCENARIO_KARAOKE = 9,

    /// Available since: 3.3.0. Description: Standard voice call scenario, it is suitable for one-to-one video or voice call scenarios. Note: On the ExpressVideo SDK, the camera is not enabled by default in this scenario.
    ZEGO_SCENARIO_STANDARD_VOICE_CALL = 10

};

/// SDK feature type.
enum ZegoFeatureType {
    /// Basic audio feature.
    ZEGO_FEATURE_TYPE_AUDIO = 0,

    /// Basic video feature.
    ZEGO_FEATURE_TYPE_VIDEO = 1,

    /// Media player feature.
    ZEGO_FEATURE_TYPE_MEDIA_PLAYER = 2,

    /// Local media data recorder feature.
    ZEGO_FEATURE_TYPE_MEDIA_DATA_RECORDER = 3,

    /// Media data publisher feature.
    ZEGO_FEATURE_TYPE_MEDIA_DATA_PUBLISHER = 4,

    /// Supplemental Enhancement Information (media side info) feature.
    ZEGO_FEATURE_TYPE_SEI = 5,

    /// SDK video capture feature.
    ZEGO_FEATURE_TYPE_SDK_VIDEO_CAPTURE = 6,

    /// Custom video capture feature.
    ZEGO_FEATURE_TYPE_CUSTOM_VIDEO_CAPTURE = 7,

    /// SDK video rendering feature.
    ZEGO_FEATURE_TYPE_SDK_VIDEO_RENDER = 8,

    /// Custom video rendering feature.
    ZEGO_FEATURE_TYPE_CUSTOM_VIDEO_RENDER = 9,

    /// SDK video processing feature (including low-light enhancement feature).
    ZEGO_FEATURE_TYPE_SDK_VIDEO_PROCESSING = 10,

    /// Custom video processing feature.
    ZEGO_FEATURE_TYPE_CUSTOM_VIDEO_PROCESSING = 11,

    /// Streaming encryption / decryption feature.
    ZEGO_FEATURE_TYPE_STREAM_ENCRYPTION = 12,

    /// RTMP streaming feature.
    ZEGO_FEATURE_TYPE_RTMP = 13,

    /// RTMPS streaming feature.
    ZEGO_FEATURE_TYPE_RTMPS = 14,

    /// RTMP over QUIC streaming feature.
    ZEGO_FEATURE_TYPE_RTMP_OVER_QUIC = 15,

    /// RTMP streaming feature.
    ZEGO_FEATURE_TYPE_HTTP_FLV = 16,

    /// HTTPS-FLV streaming feature.
    ZEGO_FEATURE_TYPE_HTTPS_FLV = 17,

    /// HTTP-FLV over QUIC streaming feature.
    ZEGO_FEATURE_TYPE_HTTP_FLV_OVER_QUIC = 18,

    /// Super resolution imaging feature.
    ZEGO_FEATURE_TYPE_SUPER_RESOLUTION = 19,

    /// Effects beauty feature.
    ZEGO_FEATURE_TYPE_EFFECTS_BEAUTY = 20,

    /// Whiteboard beauty feature.
    ZEGO_FEATURE_TYPE_WHITEBOARD = 21,

    /// Range audio feature.
    ZEGO_FEATURE_TYPE_RANGE_AUDIO = 22,

    /// Copy righted music feature.
    ZEGO_FEATURE_TYPE_COPY_RIGHTED_MUSIC = 23,

    /// Video object segmentation feature.
    ZEGO_FEATURE_TYPE_VIDEO_OBJECT_SEGMENTATION = 24,

    /// Range scene feature. (3.0.0 and above support)
    ZEGO_FEATURE_TYPE_RANGE_SCENE = 25,

    /// Screen capture feature. (3.1.0 and above support)
    ZEGO_FEATURE_TYPE_SCREEN_CAPTURE = 26,

    /// AI voice changer feature. (3.8.0 and above support)
    ZEGO_FEATURE_TYPE_AI_VOICE_CHANGER = 27

};

/// Language.
enum ZegoLanguage {
    /// English
    ZEGO_LANGUAGE_ENGLISH = 0,

    /// Chinese
    ZEGO_LANGUAGE_CHINESE = 1

};

/// Room mode.
enum ZegoRoomMode {
    /// Single room mode.
    ZEGO_ROOM_MODE_SINGLE_ROOM = 0,

    /// Multiple room mode.
    ZEGO_ROOM_MODE_MULTI_ROOM = 1

};

/// Geo fence type.
enum ZegoGeoFenceType {
    /// Not use geo fence.
    ZEGO_GEO_FENCE_TYPE_NONE = 0,

    /// Include the specified geo fence areas.
    ZEGO_GEO_FENCE_TYPE_INCLUDE = 1,

    /// Exclude the specified geo fence areas.
    ZEGO_GEO_FENCE_TYPE_EXCLUDE = 2

};

/// Geo fence area code.
enum ZegoGeoFenceAreaCode {
    /// Chinese mainland (excluding Hong Kong, Macao and Taiwan).
    ZEGO_GEO_FENCE_AREA_CODE_CN = 2,

    /// North America.
    ZEGO_GEO_FENCE_AREA_CODE_NA = 3,

    /// Europe, including the UK.
    ZEGO_GEO_FENCE_AREA_CODE_EU = 4,

    /// Asia, excluding Chinese mainland and India.
    ZEGO_GEO_FENCE_AREA_CODE_AS = 5,

    /// India.
    ZEGO_GEO_FENCE_AREA_CODE_IN = 6

};

/// engine state.
enum ZegoEngineState {
    /// The engine has started
    ZEGO_ENGINE_STATE_START = 0,

    /// The engine has stoped
    ZEGO_ENGINE_STATE_STOP = 1

};

/// Room state.
enum ZegoRoomState {
    /// Unconnected state, enter this state before logging in and after exiting the room. If there is a steady state abnormality in the process of logging in to the room, such as AppID or Token are incorrect, or if the same user name is logged in elsewhere and the local end is KickOut, it will enter this state.
    ZEGO_ROOM_STATE_DISCONNECTED = 0,

    /// The state that the connection is being requested. It will enter this state after successful execution login room function. The display of the UI is usually performed using this state. If the connection is interrupted due to poor network quality, the SDK will perform an internal retry and will return to the requesting connection status.
    ZEGO_ROOM_STATE_CONNECTING = 1,

    /// The status that is successfully connected. Entering this status indicates that the login to the room has been successful. The user can receive the callback notification of the user and the stream information in the room.
    ZEGO_ROOM_STATE_CONNECTED = 2

};

/// Room state change reason.
enum ZegoRoomStateChangedReason {
    /// Logging in to the room. When calling [loginRoom] to log in to the room or [switchRoom] to switch to the target room, it will enter this state, indicating that it is requesting to connect to the server. The application interface is usually displayed through this state.
    ZEGO_ROOM_STATE_CHANGED_REASON_LOGINING = 0,

    /// Log in to the room successfully. When the room is successfully logged in or switched, it will enter this state, indicating that the login to the room has been successful, and users can normally receive callback notifications of other users in the room and all stream information additions and deletions.
    ZEGO_ROOM_STATE_CHANGED_REASON_LOGINED = 1,

    /// Failed to log in to the room. When the login or switch room fails, it will enter this state, indicating that the login or switch room has failed, for example, AppID or Token is incorrect, etc.
    ZEGO_ROOM_STATE_CHANGED_REASON_LOGIN_FAILED = 2,

    /// The room connection is temporarily interrupted. If the interruption occurs due to poor network quality, the SDK will retry internally.
    ZEGO_ROOM_STATE_CHANGED_REASON_RECONNECTING = 3,

    /// The room is successfully reconnected. If there is an interruption due to poor network quality, the SDK will retry internally, and enter this state after successful reconnection.
    ZEGO_ROOM_STATE_CHANGED_REASON_RECONNECTED = 4,

    /// The room fails to reconnect. If there is an interruption due to poor network quality, the SDK will retry internally, and enter this state after the reconnection fails.
    ZEGO_ROOM_STATE_CHANGED_REASON_RECONNECT_FAILED = 5,

    /// Kicked out of the room by the server. For example, if you log in to the room with the same user name in other places, and the local end is kicked out of the room, it will enter this state.
    ZEGO_ROOM_STATE_CHANGED_REASON_KICK_OUT = 6,

    /// Logout of the room is successful. It is in this state by default before logging into the room. When calling [logoutRoom] to log out of the room successfully or [switchRoom] to log out of the current room successfully, it will enter this state.
    ZEGO_ROOM_STATE_CHANGED_REASON_LOGOUT = 7,

    /// Failed to log out of the room. Enter this state when calling [logoutRoom] fails to log out of the room or [switchRoom] fails to log out of the current room internally.
    ZEGO_ROOM_STATE_CHANGED_REASON_LOGOUT_FAILED = 8

};

/// Room mode.
enum ZegoRoomTransparentMessageMode {
    /// Single room mode.
    ZEGO_ROOM_TRANSPARENT_MESSAGE_MODE_ONLY_CLIENT = 0,

    /// Multiple room mode.
    ZEGO_ROOM_TRANSPARENT_MESSAGE_MODE_ONLY_SERVER = 1,

    /// Multiple room mode.
    ZEGO_ROOM_TRANSPARENT_MESSAGE_MODE_CLIENT_AND_SERVER = 2

};

/// Room mode.
enum ZegoRoomTransparentMessageType {
    /// Single room mode.
    ZEGO_ROOM_TRANSPARENT_MESSAGE_NORMAL = 0,

    /// Multiple room mode.
    ZEGO_ROOM_TRANSPARENT_MESSAGE_SEQUENCE = 1

};

/// Publish channel.
enum ZegoPublishChannel {
    /// The main (default/first) publish channel.
    ZEGO_PUBLISH_CHANNEL_MAIN = 0,

    /// The auxiliary (second) publish channel
    ZEGO_PUBLISH_CHANNEL_AUX = 1,

    /// The third publish channel
    ZEGO_PUBLISH_CHANNEL_THIRD = 2,

    /// The fourth publish channel
    ZEGO_PUBLISH_CHANNEL_FOURTH = 3

};

/// Publish CensorshipMode.
enum ZegoStreamCensorshipMode {
    /// no censorship.
    ZEGO_STREAM_CENSORSHIP_MODE_NONE = 0,

    /// only censorship stream audio.
    ZEGO_STREAM_CENSORSHIP_MODE_AUDIO = 1,

    /// only censorship stream video.
    ZEGO_STREAM_CENSORSHIP_MODE_VIDEO = 2,

    /// censorship stream audio and video.
    ZEGO_STREAM_CENSORSHIP_MODE_AUDIO_AND_VIDEO = 3

};

/// Type of capability negotiation for publish stream references.
enum ZegoCapabilityNegotiationType {
    /// no reference to the outcome of the capability negotiation.
    ZEGO_CAPABILITY_NEGOTIATION_TYPE_NONE = 0,

    /// refer to the outcome of the capability negotiation of all user in the room.
    ZEGO_CAPABILITY_NEGOTIATION_TYPE_ALL = 1,

    /// refer to the outcome of the capability negotiation of publisher in the room.
    ZEGO_CAPABILITY_NEGOTIATION_TYPE_PUBLISHER = 2

};

/// Video rendering fill mode.
enum ZegoViewMode {
    /// The proportional scaling up, there may be black borders
    ZEGO_VIEW_MODE_ASPECT_FIT = 0,

    /// The proportional zoom fills the entire View and may be partially cut
    ZEGO_VIEW_MODE_ASPECT_FILL = 1,

    /// Fill the entire view, the image may be stretched
    ZEGO_VIEW_MODE_SCALE_TO_FILL = 2

};

/// Mirror mode for previewing or playing the of the stream.
enum ZegoVideoMirrorMode {
    /// The mirror image only for previewing locally. This mode is used by default. When the mobile terminal uses a rear camera, this mode is still used by default, but it does not work. Local preview does not set mirroring.
    ZEGO_VIDEO_MIRROR_MODE_ONLY_PREVIEW_MIRROR = 0,

    /// Both the video previewed locally and the far end playing the stream will see mirror image.
    ZEGO_VIDEO_MIRROR_MODE_BOTH_MIRROR = 1,

    /// Both the video previewed locally and the far end playing the stream will not see mirror image.
    ZEGO_VIDEO_MIRROR_MODE_NO_MIRROR = 2,

    /// The mirror image only for far end playing the stream.
    ZEGO_VIDEO_MIRROR_MODE_ONLY_PUBLISH_MIRROR = 3

};

/// SEI type
enum ZegoSEIType {
    /// Using H.264 SEI (nalu type = 6, payload type = 243) type packaging, this type is not specified by the SEI standard, there is no conflict with the video encoder or the SEI in the video file, users do not need to follow the SEI content Do filtering, SDK uses this type by default.
    ZEGO_SEI_TYPE_ZEGO_DEFINED = 0,

    /// SEI (nalu type = 6, payload type = 5) of H.264 is used for packaging. The H.264 standard has a prescribed format for this type: startcode + nalu type (6) + payload type (5) + len + payload (uuid + content) + trailing bits. Because the video encoder itself generates an SEI with a payload type of 5, or when a video file is used for streaming, such SEI may also exist in the video file, so when using this type, the user needs to use uuid + context as a buffer sending SEI. At this time, in order to distinguish the SEI generated by the video encoder itself, when the App sends this type of SEI, it can fill in the service-specific uuid (uuid length is 16 bytes). When the receiver uses the SDK to parse the SEI of the payload type 5, it will set filter string filters out the SEI matching the uuid and throws it to the business. If the filter string is not set, the SDK will throw all received SEI to the developer. uuid filter string setting function, [ZegoEngineConfig.advancedConfig("unregister_sei_filter","XXXXXX")], where unregister_sei_filter is the key, and XXXXX is the uuid filter string to be set.
    ZEGO_SEI_TYPE_USER_UNREGISTER = 1

};

/// Publish stream status.
enum ZegoPublisherState {
    /// The state is not published, and it is in this state before publishing the stream. If a steady-state exception occurs in the publish process, such as AppID or Token are incorrect, or if other users are already publishing the stream, there will be a failure and enter this state.
    ZEGO_PUBLISHER_STATE_NO_PUBLISH = 0,

    /// The state that it is requesting to publish the stream after the [startPublishingStream] function is successfully called. The UI is usually displayed through this state. If the connection is interrupted due to poor network quality, the SDK will perform an internal retry and will return to the requesting state.
    ZEGO_PUBLISHER_STATE_PUBLISH_REQUESTING = 1,

    /// The state that the stream is being published, entering the state indicates that the stream has been successfully published, and the user can communicate normally.
    ZEGO_PUBLISHER_STATE_PUBLISHING = 2

};

/// Voice changer preset value.
enum ZegoVoiceChangerPreset {
    /// No Voice changer
    ZEGO_VOICE_CHANGER_PRESET_NONE = 0,

    /// Male to child voice (loli voice effect)
    ZEGO_VOICE_CHANGER_PRESET_MEN_TO_CHILD = 1,

    /// Male to female voice (kindergarten voice effect)
    ZEGO_VOICE_CHANGER_PRESET_MEN_TO_WOMEN = 2,

    /// Female to child voice
    ZEGO_VOICE_CHANGER_PRESET_WOMEN_TO_CHILD = 3,

    /// Female to male voice
    ZEGO_VOICE_CHANGER_PRESET_WOMEN_TO_MEN = 4,

    /// Foreigner voice effect
    ZEGO_VOICE_CHANGER_PRESET_FOREIGNER = 5,

    /// Autobot Optimus Prime voice effect
    ZEGO_VOICE_CHANGER_PRESET_OPTIMUS_PRIME = 6,

    /// Android robot voice effect
    ZEGO_VOICE_CHANGER_PRESET_ANDROID = 7,

    /// Ethereal voice effect
    ZEGO_VOICE_CHANGER_PRESET_ETHEREAL = 8,

    /// Magnetic(Male) voice effect
    ZEGO_VOICE_CHANGER_PRESET_MALE_MAGNETIC = 9,

    /// Fresh(Female) voice effect
    ZEGO_VOICE_CHANGER_PRESET_FEMALE_FRESH = 10,

    /// Electronic effects in C major voice effect
    ZEGO_VOICE_CHANGER_PRESET_MAJOR_C = 11,

    /// Electronic effects in A minor voice effect
    ZEGO_VOICE_CHANGER_PRESET_MINOR_A = 12,

    /// Electronic effects in harmonic minor voice effect
    ZEGO_VOICE_CHANGER_PRESET_HARMONIC_MINOR = 13,

    /// Female Vitality Sound effect
    ZEGO_VOICE_CHANGER_PRESET_FEMALE_ENERGETIC = 14,

    /// Richness effect
    ZEGO_VOICE_CHANGER_PRESET_RICH_NESS = 15,

    /// Muffled effect
    ZEGO_VOICE_CHANGER_PRESET_MUFFLED = 16,

    /// Roundness effect
    ZEGO_VOICE_CHANGER_PRESET_ROUNDNESS = 17,

    /// Falsetto effect
    ZEGO_VOICE_CHANGER_PRESET_FALSETTO = 18,

    /// Fullness effect
    ZEGO_VOICE_CHANGER_PRESET_FULLNESS = 19,

    /// Clear effect
    ZEGO_VOICE_CHANGER_PRESET_CLEAR = 20,

    /// Hight effect
    ZEGO_VOICE_CHANGER_PRESET_HIGHLY_RESONANT = 21,

    /// Loud clear effect
    ZEGO_VOICE_CHANGER_PRESET_LOUD_CLEAR = 22,

    /// Minions effect
    ZEGO_VOICE_CHANGER_PRESET_MINIONS = 23,

    /// Sunshine effect
    ZEGO_VOICE_CHANGER_PRESET_SUNSHINE = 24,

    /// Gentle effect
    ZEGO_VOICE_CHANGER_PRESET_GENTLE = 25,

    /// Sweet effect
    ZEGO_VOICE_CHANGER_PRESET_SWEET = 26,

    /// Sweet male effect
    ZEGO_VOICE_CHANGER_PRESET_SWEET_MALE = 27,

    /// Sweet female effec
    ZEGO_VOICE_CHANGER_PRESET_SWEET_FEMALE = 28,

    /// Bright effect
    ZEGO_VOICE_CHANGER_PRESET_BRIGHT = 29,

    /// Autobot effect
    ZEGO_VOICE_CHANGER_PRESET_AUTOBOT = 30,

    /// Out of power effect
    ZEGO_VOICE_CHANGER_PRESET_OUT_OF_POWER = 31

};

/// Reverberation preset value.
enum ZegoReverbPreset {
    /// No Reverberation
    ZEGO_REVERB_PRESET_NONE = 0,

    /// Soft room reverb effect
    ZEGO_REVERB_PRESET_SOFT_ROOM = 1,

    /// Large room reverb effect
    ZEGO_REVERB_PRESET_LARGE_ROOM = 2,

    /// Concert hall reverb effect
    ZEGO_REVERB_PRESET_CONCERT_HALL = 3,

    /// Valley reverb effect
    ZEGO_REVERB_PRESET_VALLEY = 4,

    /// Recording studio reverb effect
    ZEGO_REVERB_PRESET_RECORDING_STUDIO = 5,

    /// Basement reverb effect
    ZEGO_REVERB_PRESET_BASEMENT = 6,

    /// KTV reverb effect
    ZEGO_REVERB_PRESET_KTV = 7,

    /// Popular reverb effect
    ZEGO_REVERB_PRESET_POPULAR = 8,

    /// Rock reverb effect
    ZEGO_REVERB_PRESET_ROCK = 9,

    /// Vocal concert reverb effect
    ZEGO_REVERB_PRESET_VOCAL_CONCERT = 10,

    /// Gramophone reverb effect
    ZEGO_REVERB_PRESET_GRAMO_PHONE = 11,

    /// Enhanced KTV reverb effect. Provide KTV effect with more concentrated voice and better brightness. Compared with the original KTV reverb effect, the reverberation time is shortened and the dry-wet ratio is increased.
    ZEGO_REVERB_PRESET_ENHANCED_KTV = 12,

    /// Enhanced Rock reverb effect
    ZEGO_REVERB_PRESET_ENHANCED_ROCK = 13,

    /// Enhanced misty reverb effect
    ZEGO_REVERB_PRESET_ENHANCED_MISTY = 14,

    /// Hip Hop reverb effect
    ZEGO_REVERB_PRESET_HIP_HOP = 15,

    /// Misty reverb effect
    ZEGO_REVERB_PRESET_MISTY = 16,

    /// 3D voice reverb effect
    ZEGO_REVERB_PRESET_THREE_DIMENSIONAL_VOICE = 17

};

/// Mode of Electronic Effects.
enum ZegoElectronicEffectsMode {
    /// Major
    ZEGO_ELECTRONIC_EFFECTS_MODE_MAJOR = 0,

    /// Minor
    ZEGO_ELECTRONIC_EFFECTS_MODE_MINOR = 1,

    /// Harmonic Minor
    ZEGO_ELECTRONIC_EFFECTS_MODE_HARMONIC_MINOR = 2

};

/// Video configuration resolution and bitrate preset enumeration. The preset resolutions are adapted for mobile and desktop. On mobile, height is longer than width, and desktop is the opposite. For example, 1080p is actually 1080(w) x 1920(h) on mobile and 1920(w) x 1080(h) on desktop.
enum ZegoVideoConfigPreset {
    /// Set the resolution to 320x180, the default is 15 fps, the code rate is 300 kbps
    ZEGO_VIDEO_CONFIG_PRESET_180P = 0,

    /// Set the resolution to 480x270, the default is 15 fps, the code rate is 400 kbps
    ZEGO_VIDEO_CONFIG_PRESET_270P = 1,

    /// Set the resolution to 640x360, the default is 15 fps, the code rate is 600 kbps
    ZEGO_VIDEO_CONFIG_PRESET_360P = 2,

    /// Set the resolution to 960x540, the default is 15 fps, the code rate is 1200 kbps
    ZEGO_VIDEO_CONFIG_PRESET_540P = 3,

    /// Set the resolution to 1280x720, the default is 15 fps, the code rate is 1500 kbps
    ZEGO_VIDEO_CONFIG_PRESET_720P = 4,

    /// Set the resolution to 1920x1080, the default is 15 fps, the code rate is 3000 kbps
    ZEGO_VIDEO_CONFIG_PRESET_1080P = 5

};

/// Stream quality level.
enum ZegoStreamQualityLevel {
    /// Excellent
    ZEGO_STREAM_QUALITY_LEVEL_EXCELLENT = 0,

    /// Good
    ZEGO_STREAM_QUALITY_LEVEL_GOOD = 1,

    /// Normal
    ZEGO_STREAM_QUALITY_LEVEL_MEDIUM = 2,

    /// Bad
    ZEGO_STREAM_QUALITY_LEVEL_BAD = 3,

    /// Failed
    ZEGO_STREAM_QUALITY_LEVEL_DIE = 4,

    /// Unknown
    ZEGO_STREAM_QUALITY_LEVEL_UNKNOWN = 5

};

/// Audio channel type.
enum ZegoAudioChannel {
    /// Unknown
    ZEGO_AUDIO_CHANNEL_UNKNOWN = 0,

    /// Mono
    ZEGO_AUDIO_CHANNEL_MONO = 1,

    /// Stereo
    ZEGO_AUDIO_CHANNEL_STEREO = 2

};

/// Audio capture stereo mode.
enum ZegoAudioCaptureStereoMode {
    /// Disable stereo capture, that is, mono.
    ZEGO_AUDIO_CAPTURE_STEREO_MODE_NONE = 0,

    /// Always enable stereo capture.
    ZEGO_AUDIO_CAPTURE_STEREO_MODE_ALWAYS = 1,

    /// @deprecated Same as [Always], that is, always enable stereo capture
    ZEGO_AUDIO_CAPTURE_STEREO_MODE_ADAPTIVE ZEGO_DEPRECATED_ENUM = 2

};

/// Audio mix mode.
enum ZegoAudioMixMode {
    /// Default mode, no special behavior
    ZEGO_AUDIO_MIX_MODE_RAW = 0,

    /// Audio focus mode, which can highlight the sound of a certain stream in multiple audio streams
    ZEGO_AUDIO_MIX_MODE_FOCUSED = 1

};

/// Audio codec ID.
enum ZegoAudioCodecID {
    /// Default, determined by the [scenario] when calling [createEngine].
    ZEGO_AUDIO_CODEC_ID_DEFAULT = 0,

    /// Can be used for RTC and CDN streaming; bitrate range from 10kbps to 128kbps; supports stereo; latency is around 500ms. Server cloud transcoding is required when communicating with the Web SDK, and it is not required when relaying to CDN.
    ZEGO_AUDIO_CODEC_ID_NORMAL = 1,

    /// Can be used for RTC and CDN streaming; good compatibility; bitrate range from 16kbps to 192kbps; supports stereo; latency is around 350ms; the sound quality is worse than [Normal] in the same (low) bitrate. Server cloud transcoding is required when communicating with the Web SDK, and it is not required when relaying to CDN.
    ZEGO_AUDIO_CODEC_ID_NORMAL2 = 2,

    /// Not recommended; if you need to use it, please contact ZEGO technical support. Can only be used for RTC streaming.
    ZEGO_AUDIO_CODEC_ID_NORMAL3 = 3,

    /// Not recommended; if you need to use it, please contact ZEGO technical support. Can only be used for RTC streaming.
    ZEGO_AUDIO_CODEC_ID_LOW = 4,

    /// Not recommended; if you need to use it, please contact ZEGO technical support. Can only be used for RTC streaming; maximum bitrate is 16kbps.
    ZEGO_AUDIO_CODEC_ID_LOW2 = 5,

    /// Can only be used for RTC streaming; bitrate range from 6kbps to 192kbps; supports stereo; latency is around 200ms; Under the same bitrate (low bitrate), the sound quality is significantly better than [Normal] and [Normal2]; low CPU overhead. Server cloud transcoding is not required when communicating with the Web SDK, and it is required when relaying to CDN.
    ZEGO_AUDIO_CODEC_ID_LOW3 = 6

};

/// Video codec ID.
enum ZegoVideoCodecID {
    /// Default (H.264)
    ZEGO_VIDEO_CODEC_ID_DEFAULT = 0,

    /// Scalable Video Coding (H.264 SVC)
    ZEGO_VIDEO_CODEC_ID_SVC = 1,

    /// VP8
    ZEGO_VIDEO_CODEC_ID_VP8 = 2,

    /// H.265
    ZEGO_VIDEO_CODEC_ID_H265 = 3,

    /// Dualstream Scalable Video Coding
    ZEGO_VIDEO_CODEC_ID_H264_DUAL_STREAM = 4,

    /// Unknown Video Coding
    ZEGO_VIDEO_CODEC_ID_UNKNOWN = 100

};

/// Backend implementation of video codec.
enum ZegoVideoCodecBackend {
    /// Software or Hardware
    ZEGO_VIDEO_CODEC_BACKEND_ANY = 0,

    /// Software
    ZEGO_VIDEO_CODEC_BACKEND_SOFTWARE = 1,

    /// Hardware
    ZEGO_VIDEO_CODEC_BACKEND_HARDWARE = 2

};

/// Video screen rotation direction.
enum ZegoOrientation {
    /// Not rotate
    ZEGO_ORIENTATION_0 = 0,

    /// Rotate 90 degrees counterclockwise
    ZEGO_ORIENTATION_90 = 1,

    /// Rotate 180 degrees counterclockwise
    ZEGO_ORIENTATION_180 = 2,

    /// Rotate 270 degrees counterclockwise
    ZEGO_ORIENTATION_270 = 3

};

/// Video stream type
enum ZegoVideoStreamType {
    /// The type to be played depends on the network status
    ZEGO_VIDEO_STREAM_TYPE_DEFAULT = 0,

    /// small resolution type
    ZEGO_VIDEO_STREAM_TYPE_SMALL = 1,

    /// big resolution type
    ZEGO_VIDEO_STREAM_TYPE_BIG = 2

};

/// Audio echo cancellation mode.
enum ZegoAECMode {
    /// Aggressive echo cancellation may affect the sound quality slightly, but the echo will be very clean.
    ZEGO_AEC_MODE_AGGRESSIVE = 0,

    /// Moderate echo cancellation, which may slightly affect a little bit of sound, but the residual echo will be less.
    ZEGO_AEC_MODE_MEDIUM = 1,

    /// Comfortable echo cancellation, that is, echo cancellation does not affect the sound quality of the sound, and sometimes there may be a little echo, but it will not affect the normal listening.
    ZEGO_AEC_MODE_SOFT = 2,

    /// AI echo cancellation. Supports intelligent recognition and elimination of echo, with a significant improvement in vocal fidelity compared to traditional AEC algorithms, without additional delay or power consumption increase.
    ZEGO_AEC_MODE_AI = 3

};

/// Active Noise Suppression mode.
enum ZegoANSMode {
    /// Soft ANS. In most instances, the sound quality will not be damaged, but some noise will remain.
    ZEGO_ANS_MODE_SOFT = 0,

    /// Medium ANS. It may damage some sound quality, but it has a good noise reduction effect.
    ZEGO_ANS_MODE_MEDIUM = 1,

    /// Aggressive ANS. It may significantly impair the sound quality, but it has a good noise reduction effect.
    ZEGO_ANS_MODE_AGGRESSIVE = 2,

    /// AI mode ANS. It will cause great damage to music, so it can not be used for noise suppression of sound sources that need to collect background sound. Please contact ZEGO technical support before use.
    ZEGO_ANS_MODE_AI = 3,

    /// Balanced AI mode ANS. It will cause great damage to music, so it can not be used for noise suppression of sound sources that need to collect background sound. Please contact ZEGO technical support before use.
    ZEGO_ANS_MODE_AI_BALANCED = 4,

    /// Low latency AI mode ANS. It will cause great damage to music, so it can not be used for noise suppression of sound sources that need to collect background sound. Please contact ZEGO technical support before use.
    ZEGO_ANS_MODE_AI_LOW_LATENCY = 5,

    /// Aggressive AI mode ANS. It will cause great damage to music, so it can not be used for noise suppression of sound sources that need to collect background sound. Please contact ZEGO technical support before use.
    ZEGO_ANS_MODE_AI_AGGRESSIVE = 6

};

/// video encode profile.
enum ZegoEncodeProfile {
    /// The default video encode specifications, The default value is the video encoding specification at the Main level.
    ZEGO_ENCODE_PROFILE_DEFAULT = 0,

    /// Baseline-level video encode specifications have low decoding consumption and poor picture effects. They are generally used for low-level applications or applications that require additional fault tolerance.
    ZEGO_ENCODE_PROFILE_BASELINE = 1,

    /// Main-level video encode specifications, decoding consumption is slightly higher than Baseline, the picture effect is also better, generally used in mainstream consumer electronic products.
    ZEGO_ENCODE_PROFILE_MAIN = 2,

    /// High-level video encode specifications, decoding consumption is higher than Main, the picture effect is better, generally used for broadcasting and video disc storage, high-definition TV.
    ZEGO_ENCODE_PROFILE_HIGH = 3

};

/// Video rate control mode, the default mode is constant video rate.
enum ZegoVideoRateControlMode {
    /// Constant rate.
    ZEGO_VIDEO_RATE_CONTROL_MODE_CONSTANT_RATE = 0,

    /// Constant quality, if this mode is used, the video rate fluctuates according to the network speed. For example, in the live broadcast of games, the constant quality mode will be used to improve the video quality in order to let the audience see smooth operation pictures.
    ZEGO_VIDEO_RATE_CONTROL_MODE_CONSTANT_QUALITY = 1

};

/// Stream alignment mode.
enum ZegoStreamAlignmentMode {
    /// Disable stream alignment.
    ZEGO_STREAM_ALIGNMENT_MODE_NONE = 0,

    /// Streams should be aligned as much as possible, call the [setStreamAlignmentProperty] function to enable the stream alignment of the push stream network time alignment of the specified channel.
    ZEGO_STREAM_ALIGNMENT_MODE_TRY = 1

};

/// Mixed stream sets the image parameter check mode.
enum ZegoMixImageCheckMode {
    /// Strictly perform image verification, set the background image, watermark will verify the image path, the image occupy set in the mixed flow input parameter will also verify whether the set image resource request is successful, in order to normally initiate mixed flow, otherwise fail to initiate mixed flow.
    ZEGO_MIX_IMAGE_CHECK_MODE_NORMAL = 0,

    /// Only verify image path URI/URL As long as the path is correct, the mixed flow is successfully initiated.
    ZEGO_MIX_IMAGE_CHECK_MODE_PATH = 1,

    /// The mixed flow can be initiated successfully without checking the related parameters of the picture.
    ZEGO_MIX_IMAGE_CHECK_MODE_NOTHING = 2

};

/// Traffic control property (bitmask enumeration).
enum ZegoTrafficControlProperty {
    /// Basic (Adaptive (reduce) video bitrate)
    ZEGO_TRAFFIC_CONTROL_PROPERTY_BASIC = 0,

    /// Adaptive (reduce) video FPS
    ZEGO_TRAFFIC_CONTROL_PROPERTY_ADAPTIVE_FPS = 1,

    /// Adaptive (reduce) video resolution
    ZEGO_TRAFFIC_CONTROL_PROPERTY_ADAPTIVE_RESOLUTION = 2,

    /// Adaptive (reduce) audio bitrate
    ZEGO_TRAFFIC_CONTROL_PROPERTY_ADAPTIVE_AUDIO_BITRATE = 4

};

/// Video transmission mode when current bitrate is lower than the set minimum bitrate.
enum ZegoTrafficControlMinVideoBitrateMode {
    /// Stop video transmission when current bitrate is lower than the set minimum bitrate
    ZEGO_TRAFFIC_CONTROL_MIN_VIDEO_BITRATE_MODE_NO_VIDEO = 0,

    /// Video is sent at a very low frequency (no more than 2fps) which is lower than the set minimum bitrate
    ZEGO_TRAFFIC_CONTROL_MIN_VIDEO_BITRATE_MODE_ULTRA_LOW_FPS = 1

};

/// Factors that trigger traffic control
enum ZegoTrafficControlFocusOnMode {
    /// Focus only on the local network
    ZEGO_TRAFFIC_CONTROL_FOUNS_ON_LOCAL_ONLY = 0,

    /// Pay attention to the local network, but also take into account the remote network, currently only effective in the 1v1 scenario
    ZEGO_TRAFFIC_CONTROL_FOUNS_ON_REMOTE = 1

};

/// Playing stream status.
enum ZegoPlayerState {
    /// The state of the flow is not played, and it is in this state before the stream is played. If the steady flow anomaly occurs during the playing process, such as AppID or Token are incorrect, it will enter this state.
    ZEGO_PLAYER_STATE_NO_PLAY = 0,

    /// The state that the stream is being requested for playing. After the [startPlayingStream] function is successfully called, it will enter the state. The UI is usually displayed through this state. If the connection is interrupted due to poor network quality, the SDK will perform an internal retry and will return to the requesting state.
    ZEGO_PLAYER_STATE_PLAY_REQUESTING = 1,

    /// The state that the stream is being playing, entering the state indicates that the stream has been successfully played, and the user can communicate normally.
    ZEGO_PLAYER_STATE_PLAYING = 2

};

/// Media event when playing.
enum ZegoPlayerMediaEvent {
    /// Audio stuck event when playing
    ZEGO_PLAYER_MEDIA_EVENT_AUDIO_BREAK_OCCUR = 0,

    /// Audio stuck event recovery when playing
    ZEGO_PLAYER_MEDIA_EVENT_AUDIO_BREAK_RESUME = 1,

    /// Video stuck event when playing
    ZEGO_PLAYER_MEDIA_EVENT_VIDEO_BREAK_OCCUR = 2,

    /// Video stuck event recovery when playing
    ZEGO_PLAYER_MEDIA_EVENT_VIDEO_BREAK_RESUME = 3

};

/// Resource Type.
enum ZegoResourceType {
    /// CDN
    ZEGO_RESOURCE_TYPE_CDN = 0,

    /// RTC
    ZEGO_RESOURCE_TYPE_RTC = 1,

    /// L3
    ZEGO_RESOURCE_TYPE_L3 = 2

};

/// Stream Resource Mode
enum ZegoStreamResourceMode {
    /// Default mode. The SDK will automatically select the streaming resource according to the cdnConfig parameters set by the player config and the ready-made background configuration.
    ZEGO_STREAM_RESOURCE_MODE_DEFAULT = 0,

    /// Playing stream only from CDN.
    ZEGO_STREAM_RESOURCE_MODE_ONLY_CDN = 1,

    /// Playing stream only from L3.
    ZEGO_STREAM_RESOURCE_MODE_ONLY_L3 = 2,

    /// Playing stream only from RTC.
    ZEGO_STREAM_RESOURCE_MODE_ONLY_RTC = 3,

    /// @deprecated Legacy CDN Plus
    ZEGO_STREAM_RESOURCE_MODE_CDN_PLUS ZEGO_DEPRECATED_ENUM = 4,

    /// Custom mode. The SDK selects the streaming resource based on the customResourceConfig parameter of the streaming settings.
    ZEGO_STREAM_RESOURCE_MODE_CUSTOM = 5

};

/// Stream Switch Resource Mode
enum ZegoStreamResourceSwitchMode {
    /// Default mode. The SDK will automatically select the streaming resource according to the parameters set by the player config and the ready-made background configuration.
    ZEGO_STREAM_RESOURCE_SWITCH_MODE_DEFAULT = 0,

    /// Auto switch to RTC resource when publishing.
    ZEGO_STREAM_RESOURCE_SWITCH_MODE_SWITCH_TO_RTC = 1,

    /// Keep using original resource when publishing, not switch to RTC resource.
    ZEGO_STREAM_RESOURCE_SWITCH_MODE_KEEP_ORIGINAL = 2

};

/// Stream Resource Type
enum ZegoStreamResourceType {
    /// Default mode. The SDK will automatically select the streaming resource according to the parameters set by the player config and the ready-made background configuration.
    ZEGO_STREAM_RESOURCE_TYPE_DEFAULT = 0,

    /// CDN resource.
    ZEGO_STREAM_RESOURCE_TYPE_CDN = 1,

    /// L3 resource.
    ZEGO_STREAM_RESOURCE_TYPE_L3 = 2

};

/// Update type.
enum ZegoUpdateType {
    /// Add
    ZEGO_UPDATE_TYPE_ADD = 0,

    /// Delete
    ZEGO_UPDATE_TYPE_DELETE = 1

};

/// Get room stream list type.
enum ZegoRoomStreamListType {
    /// List of all online streams in the current room, excluding your own streams
    ZEGO_ROOM_STREAM_LIST_TYPE_PLAY = 0,

    /// List of all online streams in the current room, including your own streams
    ZEGO_ROOM_STREAM_LIST_TYPE_ALL = 1

};

/// Capability negotiation enable bitmask enumeration.
enum ZegoRoomCapabilityNegotiationTypesBitMask {
    /// The mask bit of this field corresponds to enable the capability negotiation of all user in the room.
    ZEGO_ROOM_CAPABILITY_NEGOTIATION_TYPES_BIT_MASK_ALL = 1 << 0,

    /// The mask bit of this field corresponds to enable the capability negotiation of publisher in the room.
    ZEGO_ROOM_CAPABILITY_NEGOTIATION_TYPES_PUBLISHER = 1 << 1

};

/// State of CDN relay.
enum ZegoStreamRelayCDNState {
    /// The state indicates that there is no CDN relay
    ZEGO_STREAM_RELAY_CDN_STATE_NO_RELAY = 0,

    /// The CDN relay is being requested
    ZEGO_STREAM_RELAY_CDN_STATE_RELAY_REQUESTING = 1,

    /// Entering this status indicates that the CDN relay has been successful
    ZEGO_STREAM_RELAY_CDN_STATE_RELAYING = 2

};

/// Reason for state of CDN relay changed.
enum ZegoStreamRelayCDNUpdateReason {
    /// No error
    ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_NONE = 0,

    /// Server error
    ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_SERVER_ERROR = 1,

    /// Handshake error
    ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_HANDSHAKE_FAILED = 2,

    /// Access point error
    ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_ACCESS_POINT_ERROR = 3,

    /// Stream create failure
    ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_CREATE_STREAM_FAILED = 4,

    /// Bad stream ID
    ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_BAD_NAME = 5,

    /// CDN server actively disconnected
    ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_CDN_SERVER_DISCONNECTED = 6,

    /// Active disconnect
    ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_DISCONNECTED = 7,

    /// All mixer input streams sessions closed
    ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_MIX_STREAM_ALL_INPUT_STREAM_CLOSED = 8,

    /// All mixer input streams have no data
    ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_MIX_STREAM_ALL_INPUT_STREAM_NO_DATA = 9,

    /// Internal error of stream mixer server
    ZEGO_STREAM_RELAY_CDN_UPDATE_REASON_MIX_STREAM_SERVER_INTERNAL_ERROR = 10

};

/// Device type.
enum ZegoDeviceType {
    /// Unknown device type.
    ZEGO_DEVICE_TYPE_UNKNOWN = 0,

    /// Camera device.
    ZEGO_DEVICE_TYPE_CAMERA = 1,

    /// Microphone device.
    ZEGO_DEVICE_TYPE_MICROPHONE = 2,

    /// Speaker device.
    ZEGO_DEVICE_TYPE_SPEAKER = 3,

    /// Audio device. (Other audio device that cannot be accurately classified into microphones or speakers.)
    ZEGO_DEVICE_TYPE_AUDIO_DEVICE = 4,

    /// Audio Session.
    ZEGO_DEVICE_TYPE_AUDIO_SESSION = 5

};

/// The exception type for the device.
enum ZegoDeviceExceptionType {
    /// Unknown device exception.
    ZEGO_DEVICE_EXCEPTION_TYPE_UNKNOWN = 0,

    /// Generic device exception.
    ZEGO_DEVICE_EXCEPTION_TYPE_GENERIC = 1,

    /// Invalid device ID exception.
    ZEGO_DEVICE_EXCEPTION_TYPE_INVALID_ID = 2,

    /// Device permission is not granted.
    ZEGO_DEVICE_EXCEPTION_TYPE_PERMISSION_NOT_GRANTED = 3,

    /// The capture frame rate of the device is 0.
    ZEGO_DEVICE_EXCEPTION_TYPE_ZERO_CAPTURE_FPS = 4,

    /// The device is being occupied.
    ZEGO_DEVICE_EXCEPTION_TYPE_DEVICE_OCCUPIED = 5,

    /// The device is unplugged (not plugged in).
    ZEGO_DEVICE_EXCEPTION_TYPE_DEVICE_UNPLUGGED = 6,

    /// The device requires the system to restart before it can work (Windows platform only).
    ZEGO_DEVICE_EXCEPTION_TYPE_REBOOT_REQUIRED = 7,

    /// The system media service is unavailable, e.g. when the iOS system detects that the current pressure is huge (such as playing a lot of animation), it is possible to disable all media related services (Apple platform only).
    ZEGO_DEVICE_EXCEPTION_TYPE_MEDIA_SERVICES_WERE_LOST = 8,

    /// The device is being occupied by Siri (Apple platform only).
    ZEGO_DEVICE_EXCEPTION_TYPE_SIRI_IS_RECORDING = 9,

    /// The device captured sound level is too low (Windows platform only).
    ZEGO_DEVICE_EXCEPTION_TYPE_SOUND_LEVEL_TOO_LOW = 10,

    /// The device is being occupied, and maybe cause by iPad magnetic case (Apple platform only).
    ZEGO_DEVICE_EXCEPTION_TYPE_MAGNETIC_CASE = 11,

    /// Audio session deactive (Apple platform only).
    ZEGO_DEVICE_EXCEPTION_TYPE_AUDIO_SESSION_DEACTIVE = 12,

    /// Audio session category change (Apple platform only).
    ZEGO_DEVICE_EXCEPTION_TYPE_AUDIO_SESSION_CATEGORY_CHANGE = 13

};

/// Remote device status.
enum ZegoRemoteDeviceState {
    /// Device on
    ZEGO_REMOTE_DEVICE_STATE_OPEN = 0,

    /// General device error
    ZEGO_REMOTE_DEVICE_STATE_GENERIC_ERROR = 1,

    /// Invalid device ID
    ZEGO_REMOTE_DEVICE_STATE_INVALID_ID = 2,

    /// No permission
    ZEGO_REMOTE_DEVICE_STATE_NO_AUTHORIZATION = 3,

    /// Captured frame rate is 0
    ZEGO_REMOTE_DEVICE_STATE_ZERO_FPS = 4,

    /// The device is occupied
    ZEGO_REMOTE_DEVICE_STATE_IN_USE_BY_OTHER = 5,

    /// The device is not plugged in or unplugged
    ZEGO_REMOTE_DEVICE_STATE_UNPLUGGED = 6,

    /// The system needs to be restarted
    ZEGO_REMOTE_DEVICE_STATE_REBOOT_REQUIRED = 7,

    /// System media services stop, such as under the iOS platform, when the system detects that the current pressure is huge (such as playing a lot of animation), it is possible to disable all media related services.
    ZEGO_REMOTE_DEVICE_STATE_SYSTEM_MEDIA_SERVICES_LOST = 8,

    /// The remote user calls [enableCamera] or [enableAudioCaptureDevice] to disable the camera or microphone.
    ZEGO_REMOTE_DEVICE_STATE_DISABLE = 9,

    /// The remote user actively calls [muteMicrophone] or [mutePublishStreamAudio] or [mutePublishStreamVideo] to stop publish the audio or video stream.
    ZEGO_REMOTE_DEVICE_STATE_MUTE = 10,

    /// The device is interrupted, such as a phone call interruption, etc.
    ZEGO_REMOTE_DEVICE_STATE_INTERRUPTION = 11,

    /// There are multiple apps at the same time in the foreground, such as the iPad app split screen, the system will prohibit all apps from using the camera.
    ZEGO_REMOTE_DEVICE_STATE_IN_BACKGROUND = 12,

    /// CDN server actively disconnected
    ZEGO_REMOTE_DEVICE_STATE_MULTI_FOREGROUND_APP = 13,

    /// The system is under high load pressure and may cause abnormal equipment.
    ZEGO_REMOTE_DEVICE_STATE_BY_SYSTEM_PRESSURE = 14,

    /// The remote device is not supported to publish the device state.
    ZEGO_REMOTE_DEVICE_STATE_NOT_SUPPORT = 15

};

/// Audio device type.
enum ZegoAudioDeviceType {
    /// Audio input type
    ZEGO_AUDIO_DEVICE_TYPE_INPUT = 0,

    /// Audio output type
    ZEGO_AUDIO_DEVICE_TYPE_OUTPUT = 1

};

/// Audio device mode.
enum ZegoAudioDeviceMode {
    /// Enable system echo cancellation.
    ZEGO_AUDIO_DEVICE_MODE_COMMUNICATION = 1,

    /// The system echo cancellation function is disabled.
    ZEGO_AUDIO_DEVICE_MODE_GENERAL = 2,

    /// Automatically select whether to enable system echo cancellation based on the scenario.
    ZEGO_AUDIO_DEVICE_MODE_AUTO = 3,

    /// Enable system echo cancellation. Compared with Communication, this mode always occupies the microphone device.
    ZEGO_AUDIO_DEVICE_MODE_COMMUNICATION2 = 4,

    /// Enable system echo cancellation. Compared with Communication, in this mode, the microphone is released and the media volume is reduced.
    ZEGO_AUDIO_DEVICE_MODE_COMMUNICATION3 = 5,

    /// Disable system echo cancellation. Compared with General, this mode is not released when a microphone device is used.
    ZEGO_AUDIO_DEVICE_MODE_GENERAL2 = 6,

    /// Disable system echo cancellation. Compared with General, this mode will always occupy the microphone device.
    ZEGO_AUDIO_DEVICE_MODE_GENERAL3 = 7,

    /// Enable system echo cancellation. Compared with Communication, this mode of wheat after releasing the microphone, maintain the call volume.
    ZEGO_AUDIO_DEVICE_MODE_COMMUNICATION4 = 8

};

/// Audio route
enum ZegoAudioRoute {
    /// Speaker
    ZEGO_AUDIO_ROUTE_SPEAKER = 0,

    /// Headphone
    ZEGO_AUDIO_ROUTE_HEADPHONE = 1,

    /// Bluetooth device
    ZEGO_AUDIO_ROUTE_BLUETOOTH = 2,

    /// Receiver
    ZEGO_AUDIO_ROUTE_RECEIVER = 3,

    /// External USB audio device
    ZEGO_AUDIO_ROUTE_EXTERNAL_USB = 4,

    /// Apple AirPlay
    ZEGO_AUDIO_ROUTE_AIR_PLAY = 5

};

/// Mix stream content type.
enum ZegoMixerInputContentType {
    /// Mix stream for audio only
    ZEGO_MIXER_INPUT_CONTENT_TYPE_AUDIO = 0,

    /// Mix stream for both audio and video
    ZEGO_MIXER_INPUT_CONTENT_TYPE_VIDEO = 1,

    /// Mix stream for video only. On web platforms, this property does not take effect
    ZEGO_MIXER_INPUT_CONTENT_TYPE_VIDEO_ONLY = 2

};

/// Capture pipeline scale mode.
enum ZegoCapturePipelineScaleMode {
    /// Zoom immediately after acquisition, default
    ZEGO_CAPTURE_PIPELINE_SCALE_MODE_PRE = 0,

    /// Scaling while encoding
    ZEGO_CAPTURE_PIPELINE_SCALE_MODE_POST = 1

};

/// Video frame format.
enum ZegoVideoFrameFormat {
    /// Unknown format, will take platform default
    ZEGO_VIDEO_FRAME_FORMAT_UNKNOWN = 0,

    /// I420 (YUV420Planar) format
    ZEGO_VIDEO_FRAME_FORMAT_I420 = 1,

    /// NV12 (YUV420SemiPlanar) format
    ZEGO_VIDEO_FRAME_FORMAT_NV12 = 2,

    /// NV21 (YUV420SemiPlanar) format
    ZEGO_VIDEO_FRAME_FORMAT_NV21 = 3,

    /// BGRA32 format
    ZEGO_VIDEO_FRAME_FORMAT_BGRA32 = 4,

    /// RGBA32 format
    ZEGO_VIDEO_FRAME_FORMAT_RGBA32 = 5,

    /// ARGB32 format
    ZEGO_VIDEO_FRAME_FORMAT_ARGB32 = 6,

    /// ABGR32 format
    ZEGO_VIDEO_FRAME_FORMAT_ABGR32 = 7,

    /// I422 (YUV422Planar) format
    ZEGO_VIDEO_FRAME_FORMAT_I422 = 8,

    /// BGR24 format
    ZEGO_VIDEO_FRAME_FORMAT_BGR24 = 9,

    /// RGB24 format
    ZEGO_VIDEO_FRAME_FORMAT_RGB24 = 10

};

/// Video encoded frame format.
enum ZegoVideoEncodedFrameFormat {
    /// AVC AVCC format
    ZEGO_VIDEO_ENCODED_FRAME_FORMAT_AVCC = 0,

    /// AVC Annex-B format
    ZEGO_VIDEO_ENCODED_FRAME_FORMAT_ANNEXB = 1,

    /// VP8 format
    ZEGO_VIDEO_ENCODED_FRAME_FORMAT_VP8 = 2,

    /// HEVC AVCC format
    ZEGO_VIDEO_ENCODED_FRAME_FORMAT_HEVC_AVCC = 3,

    /// HEVC Annex-B format
    ZEGO_VIDEO_ENCODED_FRAME_FORMAT_HEVC_ANNEXB = 4

};

/// Video frame buffer type.
enum ZegoVideoBufferType {
    /// Raw data type video frame
    ZEGO_VIDEO_BUFFER_TYPE_UNKNOWN = 0,

    /// Raw data type video frame
    ZEGO_VIDEO_BUFFER_TYPE_RAW_DATA = 1,

    /// Encoded data type video frame
    ZEGO_VIDEO_BUFFER_TYPE_ENCODED_DATA = 2,

    /// CVPixelBuffer type video frame
    ZEGO_VIDEO_BUFFER_TYPE_CV_PIXEL_BUFFER = 4,

    /// D3D Texture2D type video frame
    ZEGO_VIDEO_BUFFER_TYPE_D3D_TEXTURE_2D = 8

};

/// Video frame format series.
enum ZegoVideoFrameFormatSeries {
    /// RGB series
    ZEGO_VIDEO_FRAME_FORMAT_SERIES_RGB = 0,

    /// YUV series
    ZEGO_VIDEO_FRAME_FORMAT_SERIES_YUV = 1

};

/// Video frame flip mode.
enum ZegoVideoFlipMode {
    /// No flip
    ZEGO_VIDEO_FLIP_MODE_NONE = 0,

    /// X-axis flip
    ZEGO_VIDEO_FLIP_MODE_X = 1,

    /// Y-axis flip
    ZEGO_VIDEO_FLIP_MODE_Y = 2,

    /// X-Y-axis flip
    ZEGO_VIDEO_FLIP_MODE_XY = 3

};

/// Customize the audio processing configuration type.
enum ZegoCustomAudioProcessType {
    /// Remote audio processing
    ZEGO_CUSTOM_AUDIO_PROCESS_TYPE_REMOTE = 0,

    /// Capture audio processing
    ZEGO_CUSTOM_AUDIO_PROCESS_TYPE_CAPTURE = 1,

    /// Remote audio and capture audio processing
    ZEGO_CUSTOM_AUDIO_PROCESS_TYPE_CAPTURE_AND_REMOTE = 2

};

/// Audio Config Preset.
enum ZegoAudioConfigPreset {
    /// Basic sound quality (16 kbps, Mono, ZegoAudioCodecIDDefault)
    ZEGO_AUDIO_CONFIG_PRESET_BASIC_QUALITY = 0,

    /// Standard sound quality (48 kbps, Mono, ZegoAudioCodecIDDefault)
    ZEGO_AUDIO_CONFIG_PRESET_STANDARD_QUALITY = 1,

    /// Standard sound quality (56 kbps, Stereo, ZegoAudioCodecIDDefault)
    ZEGO_AUDIO_CONFIG_PRESET_STANDARD_QUALITY_STEREO = 2,

    /// High sound quality (128 kbps, Mono, ZegoAudioCodecIDDefault)
    ZEGO_AUDIO_CONFIG_PRESET_HIGH_QUALITY = 3,

    /// High sound quality (192 kbps, Stereo, ZegoAudioCodecIDDefault)
    ZEGO_AUDIO_CONFIG_PRESET_HIGH_QUALITY_STEREO = 4

};

/// Range audio mode
enum ZegoRangeAudioMode {
    /// World mode, you can communicate with everyone in the room.
    ZEGO_RANGE_AUDIO_MODE_WORLD = 0,

    /// Team mode, only communicate with members of the team.
    ZEGO_RANGE_AUDIO_MODE_TEAM = 1,

    /// Secret team mode, communicate with members of the team (with team members), can hear the voices of members who within the audio receive range (except the team members).
    ZEGO_RANGE_AUDIO_MODE_SECRET_TEAM = 2

};

/// Range audio speak mode
enum ZegoRangeAudioSpeakMode {
    /// All mode, his voice can be heard by members of the team and by those within range.
    ZEGO_RANGE_AUDIO_SPEAK_MODE_ALL = 0,

    /// Only world mode, only those within range can hear his voice.
    ZEGO_RANGE_AUDIO_SPEAK_MODE_WORLD = 1,

    /// Only team mode, only members of the team can hear his voice (not restricted by range).
    ZEGO_RANGE_AUDIO_SPEAK_MODE_TEAM = 2

};

/// Range audio listening mode
enum ZegoRangeAudioListenMode {
    /// All mode, can hear everyone in the room.
    ZEGO_RANGE_AUDIO_LISTEN_MODE_ALL = 0,

    /// Only world mode, only listen to people within range.
    ZEGO_RANGE_AUDIO_LISTEN_MODE_WORLD = 1,

    /// Only team mode, only listen to the voices of members of the team you belong to (not restricted by range).
    ZEGO_RANGE_AUDIO_LISTEN_MODE_TEAM = 2

};

/// Range audio microphone state.
enum ZegoRangeAudioMicrophoneState {
    /// The range audio microphone is off.
    ZEGO_RANGE_AUDIO_MICROPHONE_STATE_OFF = 0,

    /// The range audio microphone is turning on.
    ZEGO_RANGE_AUDIO_MICROPHONE_STATE_TURNING_ON = 1,

    /// The range audio microphone is on.
    ZEGO_RANGE_AUDIO_MICROPHONE_STATE_ON = 2

};

/// Player state.
enum ZegoMediaPlayerState {
    /// Not playing
    ZEGO_MEDIA_PLAYER_STATE_NO_PLAY = 0,

    /// Playing
    ZEGO_MEDIA_PLAYER_STATE_PLAYING = 1,

    /// Pausing
    ZEGO_MEDIA_PLAYER_STATE_PAUSING = 2,

    /// End of play
    ZEGO_MEDIA_PLAYER_STATE_PLAY_ENDED = 3

};

/// Player audio track mode.
enum ZegoMediaPlayerAudioTrackMode {
    /// Normal audio track mode
    ZEGO_MEDIA_PLAYER_AUDIO_TRACK_MODE_NORMAL = 0,

    /// Multiple audio track mode
    ZEGO_MEDIA_PLAYER_AUDIO_TRACK_MODE_MULTIPLE = 1

};

/// Player network event.
enum ZegoMediaPlayerNetworkEvent {
    /// Network resources are not playing well, and start trying to cache data
    ZEGO_MEDIA_PLAYER_NETWORK_EVENT_BUFFER_BEGIN = 0,

    /// Network resources can be played smoothly
    ZEGO_MEDIA_PLAYER_NETWORK_EVENT_BUFFER_ENDED = 1

};

/// Audio channel.
enum ZegoMediaPlayerAudioChannel {
    /// Audio channel left
    ZEGO_MEDIA_PLAYER_AUDIO_CHANNEL_LEFT = 0,

    /// Audio channel right
    ZEGO_MEDIA_PLAYER_AUDIO_CHANNEL_RIGHT = 1,

    /// Audio channel all
    ZEGO_MEDIA_PLAYER_AUDIO_CHANNEL_ALL = 2

};

/// Media player first frame event type.
enum ZegoMediaPlayerFirstFrameEvent {
    /// The first video frame is rendered event.
    ZEGO_MEDIA_PLAYER_FIRST_FRAME_EVENT_VIDEO_RENDERED = 0,

    /// The first audio frame is rendered event.
    ZEGO_MEDIA_PLAYER_FIRST_FRAME_EVENT_AUDIO_RENDERED = 1

};

/// AudioEffectPlayer state.
enum ZegoAudioEffectPlayState {
    /// Not playing
    ZEGO_AUDIO_EFFECT_PLAY_STATE_NO_PLAY = 0,

    /// Playing
    ZEGO_AUDIO_EFFECT_PLAY_STATE_PLAYING = 1,

    /// Pausing
    ZEGO_AUDIO_EFFECT_PLAY_STATE_PAUSING = 2,

    /// End of play
    ZEGO_AUDIO_EFFECT_PLAY_STATE_PLAY_ENDED = 3

};

/// volume type.
enum ZegoVolumeType {
    /// volume local
    ZEGO_VOLUME_TYPE_LOCAL = 0,

    /// volume remote
    ZEGO_VOLUME_TYPE_REMOTE = 1

};

/// audio sample rate.
enum ZegoAudioSampleRate {
    /// Unknown
    ZEGO_AUDIO_SAMPLE_RATE_UNKNOWN = 0,

    /// 8K
    ZEGO_AUDIO_SAMPLE_RATE_8K = 8000,

    /// 16K
    ZEGO_AUDIO_SAMPLE_RATE_16K = 16000,

    /// 22.05K
    ZEGO_AUDIO_SAMPLE_RATE_22K = 22050,

    /// 24K
    ZEGO_AUDIO_SAMPLE_RATE_24K = 24000,

    /// 32K
    ZEGO_AUDIO_SAMPLE_RATE_32K = 32000,

    /// 44.1K
    ZEGO_AUDIO_SAMPLE_RATE_44K = 44100,

    /// 48K
    ZEGO_AUDIO_SAMPLE_RATE_48K = 48000

};

/// Audio capture source type.
enum ZegoAudioSourceType {
    /// Default audio capture source (the main channel uses custom audio capture by default; the aux channel uses the same sound as main channel by default).
    ZEGO_AUDIO_SOURCE_TYPE_DEFAULT = 0,

    /// Use custom audio capture, refer to [enableCustomAudioIO] or [setAudioSource].
    ZEGO_AUDIO_SOURCE_TYPE_CUSTOM = 1,

    /// Use media player as audio source, only support aux channel.
    ZEGO_AUDIO_SOURCE_TYPE_MEDIA_PLAYER = 2,

    /// No audio source. This audio source type can only be used in [setAudioSource] interface, has no effect when used in [enableCustomAudioIO] interface.
    ZEGO_AUDIO_SOURCE_TYPE_NONE = 3,

    /// Using microphone as audio source. This audio source type can only be used in [setAudioSource] interface, has no effect when used in [enableCustomAudioIO] interface.
    ZEGO_AUDIO_SOURCE_TYPE_MICROPHONE = 4,

    /// Using main channel as audio source. Ineffective when used in main channel. This audio source type can only be used in [setAudioSource] interface, has no effect when used in [enableCustomAudioIO] interface.
    ZEGO_AUDIO_SOURCE_TYPE_MAIN_PUBLISH_CHANNEL = 5

};

/// Record type.
enum ZegoDataRecordType {
    /// This field indicates that the Express-Audio SDK records audio by default, and the Express-Video SDK records audio and video by default. When recording files in .aac format, audio is also recorded by default.
    ZEGO_DATA_RECORD_TYPE_DEFAULT = 0,

    /// only record audio
    ZEGO_DATA_RECORD_TYPE_ONLY_AUDIO = 1,

    /// only record video, Audio SDK and recording .aac format files are invalid.
    ZEGO_DATA_RECORD_TYPE_ONLY_VIDEO = 2,

    /// record audio and video. Express-Audio SDK and .aac format files are recorded only audio.
    ZEGO_DATA_RECORD_TYPE_AUDIO_AND_VIDEO = 3

};

/// Record state.
enum ZegoDataRecordState {
    /// Unrecorded state, which is the state when a recording error occurs or before recording starts.
    ZEGO_DATA_RECORD_STATE_NO_RECORD = 0,

    /// Recording in progress, in this state after successfully call [startRecordingCapturedData] function
    ZEGO_DATA_RECORD_STATE_RECORDING = 1,

    /// Record successs
    ZEGO_DATA_RECORD_STATE_SUCCESS = 2

};

/// Audio data callback function enable bitmask enumeration.
enum ZegoAudioDataCallbackBitMask {
    /// The mask bit of this field corresponds to the enable [onCapturedAudioData] callback function
    ZEGO_AUDIO_DATA_CALLBACK_BIT_MASK_CAPTURED = 1 << 0,

    /// The mask bit of this field corresponds to the enable [onPlaybackAudioData] callback function
    ZEGO_AUDIO_DATA_CALLBACK_BIT_MASK_PLAYBACK = 1 << 1,

    /// The mask bit of this field corresponds to the enable [onMixedAudioData] callback function
    ZEGO_AUDIO_DATA_CALLBACK_BIT_MASK_MIXED = 1 << 2,

    /// The mask bit of this field corresponds to the enable [onPlayerAudioData] callback function
    ZEGO_AUDIO_DATA_CALLBACK_BIT_MASK_PLAYER = 1 << 3

};

/// Network mode
enum ZegoNetworkMode {
    /// Offline (No network)
    ZEGO_NETWORK_MODE_OFFLINE = 0,

    /// Unknown network mode
    ZEGO_NETWORK_MODE_UNKNOWN = 1,

    /// Wired Ethernet (LAN)
    ZEGO_NETWORK_MODE_ETHERNET = 2,

    /// Wi-Fi (WLAN)
    ZEGO_NETWORK_MODE_WIFI = 3,

    /// 2G Network (GPRS/EDGE/CDMA1x/etc.)
    ZEGO_NETWORK_MODE_2G = 4,

    /// 3G Network (WCDMA/HSDPA/EVDO/etc.)
    ZEGO_NETWORK_MODE_3G = 5,

    /// 4G Network (LTE)
    ZEGO_NETWORK_MODE_4G = 6,

    /// 5G Network (NR (NSA/SA))
    ZEGO_NETWORK_MODE_5G = 7

};

/// network speed test type
enum ZegoNetworkSpeedTestType {
    /// uplink
    ZEGO_NETWORK_SPEED_TEST_TYPE_UPLINK = 0,

    /// downlink
    ZEGO_NETWORK_SPEED_TEST_TYPE_DOWNLINK = 1

};

/// VOD billing mode.
enum ZegoCopyrightedMusicBillingMode {
    /// Pay-per-use.Each time a user obtains a song resource, a charge is required, that is, the user will be charged for each time based on the actual call to obtain the song resource interface (such as [requestResource] etc.).
    ZEGO_COPYRIGHTED_MUSIC_BILLING_MODE_COUNT = 0,

    /// Monthly billing by user.Billing for a single user is based on the monthly dimension, that is, the statistics call to obtain song resources (such as [requestResource], etc.) and the parameters are the user ID of the monthly subscription, and the charging is based on the monthly dimension.
    ZEGO_COPYRIGHTED_MUSIC_BILLING_MODE_USER = 1,

    /// Monthly billing by room.The room users are billed on a monthly basis, that is, statistical calls to obtain song resources (such as [requestResource], etc.) are passed as Roomid for a monthly subscription of the room, and fees are charged on a monthly basis.
    ZEGO_COPYRIGHTED_MUSIC_BILLING_MODE_ROOM = 2,

    /// Monthly billing by master. Every time a user obtains a resource, it is counted as the owner’s acquisition of resources, that is, according to the actual call to obtain the song resource interface (such as [requestResource], etc.), the parameters are passed as the Roomid of the room and the Masterid of the owner, and the fee is charged according to the owner.
    ZEGO_COPYRIGHTED_MUSIC_BILLING_MODE_MASTER = 3

};

/// The music resource type. For [querycache] interface.
enum ZegoCopyrightedMusicType {
    /// @deprecated Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.
    ZEGO_COPYRIGHTED_MUSIC_SONG ZEGO_DEPRECATED_ENUM = 0,

    /// @deprecated Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.
    ZEGO_COPYRIGHTED_MUSIC_SONG_HQ ZEGO_DEPRECATED_ENUM = 1,

    /// @deprecated Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.
    ZEGO_COPYRIGHTED_MUSIC_SONG_SQ ZEGO_DEPRECATED_ENUM = 2,

    /// @deprecated Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.
    ZEGO_COPYRIGHTED_MUSIC_ACCOMPANIMENT ZEGO_DEPRECATED_ENUM = 3,

    /// @deprecated Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.
    ZEGO_COPYRIGHTED_MUSIC_ACCOMPANIMENT_CLIP ZEGO_DEPRECATED_ENUM = 4,

    /// @deprecated Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.
    ZEGO_COPYRIGHTED_MUSIC_ACCOMPANIMENT_SEGMENT ZEGO_DEPRECATED_ENUM = 5

};

/// The music resource type. For [ZegoCopyrightedMusicRequestConfig], [ZegoCopyrightedMusicGetSharedConfig] and [ZegoCopyrightedMusicQueryCacheConfig].
enum ZegoCopyrightedMusicResourceType {
    /// Song.
    ZEGO_COPYRIGHTED_MUSIC_RESOURCE_SONG = 0,

    /// Song accompaniment.
    ZEGO_COPYRIGHTED_MUSIC_RESOURCE_ACCOMPANIMENT = 1,

    /// Song accompaniment clip.
    ZEGO_COPYRIGHTED_MUSIC_RESOURCE_ACCOMPANIMENT_CLIP = 2,

    /// Song accompaniment segment.
    ZEGO_COPYRIGHTED_MUSIC_RESOURCE_ACCOMPANIMENT_SEGMENT = 3

};

/// Copyright music resource song copyright provider. For more information about the copyright owner, please contact ZEGO business personnel.
enum ZegoCopyrightedMusicVendorID {
    /// Default copyright provider.
    ZEGO_COPYRIGHTED_MUSIC_VENDOR_DEFAULT = 0,

    /// First copyright provider.
    ZEGO_COPYRIGHTED_MUSIC_VENDOR1 = 1,

    /// Second copyright provider.
    ZEGO_COPYRIGHTED_MUSIC_VENDOR2 = 2,

    /// Third copyright provider.
    ZEGO_COPYRIGHTED_MUSIC_VENDOR3 = 4

};

/// The music resource quality type. For [ZegoCopyrightedMusicQueryCacheConfig].
enum ZegoCopyrightedMusicResourceQualityType {
    /// Standard Definition Audio.
    ZEGO_COPYRIGHTED_MUSIC_RESOURCE_QUALITY_NORMAL = 0,

    /// High Definition Audio.
    ZEGO_COPYRIGHTED_MUSIC_RESOURCE_QUALITY_HQ = 1,

    /// Lossless Audio Quality.
    ZEGO_COPYRIGHTED_MUSIC_RESOURCE_QUALITY_SQ = 2

};

/// Font type.
enum ZegoFontType {
    /// Source han sans.
    ZEGO_FONT_TYPE_SOURCE_HAN_SANS = 0,

    /// Alibaba sans.
    ZEGO_FONT_TYPE_ALIBABA_SANS = 1,

    /// Pang men zheng dao title.
    ZEGO_FONT_TYPE_PANG_MEN_ZHENG_DAO_TITLE = 2,

    /// HappyZcool.
    ZEGO_FONT_TYPE_HAPPY_ZCOOL = 3

};

/// Mixing stream video view render mode.
enum ZegoMixRenderMode {
    /// The proportional zoom fills the entire area and may be partially cut.
    ZEGO_MIX_RENDER_MODE_FILL = 0,

    /// Scale the filled area proportionally. If the scale does not match the set size after scaling, the extra part will be displayed as transparent.
    ZEGO_MIX_RENDER_MODE_FIT = 1

};

/// Camera focus mode.
enum ZegoCameraFocusMode {
    /// Auto focus.
    ZEGO_CAMERA_FOCUS_MODE_AUTO_FOCUS = 0,

    /// Continuous auto focus.
    ZEGO_CAMERA_FOCUS_MODE_CONTINUOUS_AUTO_FOCUS = 1

};

/// Camera exposure mode.
enum ZegoCameraExposureMode {
    /// Auto exposure.
    ZEGO_CAMERA_EXPOSURE_MODE_AUTO_EXPOSURE = 0,

    /// Continuous auto exposure.
    ZEGO_CAMERA_EXPOSURE_MODE_CONTINUOUS_AUTO_EXPOSURE = 1

};

/// voice activity detection type
enum ZegoAudioVADType {
    /// noise
    ZEGO_AUDIO_VAD_TYPE_NOISE = 0,

    /// speech
    ZEGO_AUDIO_VAD_TYPE_SPEECH = 1

};

/// stable voice activity detection type
enum ZegoAudioVADStableStateMonitorType {
    /// captured
    ZEGO_AUDIO_VAD_STABLE_STATE_MONITOR_TYPE_CAPTURED = 0,

    /// custom processed
    ZEGO_AUDIO_VAD_STABLE_STATE_MONITOR_TYPE_CUSTOM_PROCESSED = 1

};

/// Orientation mode of the video.
enum ZegoOrientationMode {
    /// Custom mode. The default is the custom mode. In this mode, the user needs to set the orientation through [SetAppOrientation], and set the video resolution through [SetVideoConfig] to control the video ratio. The SDK rotates the video at the stream publishing end.
    ZEGO_ORIENTATION_MODE_CUSTOM = 0,

    /// Description: Player self adaption mode. The video orientation of the stream playing end is automatically vertically upward, and the user of the stream publishing end no longer needs to set the orientation through [SetAppOrientation], and no longer need to set the video resolution to control the video ratio through [SetVideoConfig]. Caution: 1. Both the stream publishing end and the stream playing end need to be set to [ZegoOrientationModeAdaption] mode. 2. Media players, cloud recording, local recording, and publish or play streaming scenarios via CDN are not supported.  3. In this mode, the SDK will automatically swap the width and height of the encoding resolution according to the actual orientation of the device.
    ZEGO_ORIENTATION_MODE_ADAPTION = 1,

    /// Description: Player adapt to pulisher mode. Taking the Status Bar as a reference, the video direction of the stream playing end is the same as the preview video direction of the stream publishing end. The SDK will use the Status Bar as a reference to rotate the image on the stream playing end, and the rotation angle is the same as the rotation angle of the preview on the stream publishing end. Stream publishing end users no longer need to set the orientation through [SetAppOrientation], and no longer need to set the video resolution to control the video ratio through [SetVideoConfig]. Caution: 1. Media players, cloud recording, local recording, and publish or play streaming scenarios via CDN are not supported.2. In this mode, the SDK will automatically swap the width and height of the encoding resolution according to the actual position of the Status Bar.
    ZEGO_ORIENTATION_MODE_ALIGNMENT = 2,

    /// Description: Fixed resolution ratio mode. Taking the Status Bar as a reference, the video orientation of the stream playing end is the same as the previewed video direction of the stream publishing end, and the video resolution is the same as the encoding resolution. Users of the streaming end no longer need to set the orientation through [SetAppOrientation].
    ZEGO_ORIENTATION_MODE_FIXED_RESOLUTION_RATIO = 3

};

/// CDN network protocol types supported by ZEGO
enum ZegoCDNProtocol {
    /// TCP protocol
    ZEGO_CDN_PROTOCOL_TCP = 1,

    /// QUIC protocol
    ZEGO_CDN_PROTOCOL_QUIC = 2

};

/// Supported httpDNS service types.
enum ZegoHttpDNSType {
    /// None.
    ZEGO_HTTP_DNS_TYPE_NONE = 0,

    /// wangsu httpdns.
    ZEGO_HTTP_DNS_TYPE_WANGSU = 1,

    /// tencent httpdns.
    ZEGO_HTTP_DNS_TYPE_TENCENT = 2,

    /// aliyun httpdns.
    ZEGO_HTTP_DNS_TYPE_ALIYUN = 3

};

/// Publish or play stream event
enum ZegoStreamEvent {
    /// Start publishing stream
    ZEGO_STREAM_EVENT_PUBLISH_START = 100,

    /// The first publish stream was successful
    ZEGO_STREAM_EVENT_PUBLISH_SUCCESS = 101,

    /// Failed to publish stream for the first time
    ZEGO_STREAM_EVENT_PUBLISH_FAIL = 102,

    /// Start retrying publishing stream
    ZEGO_STREAM_EVENT_RETRY_PUBLISH_START = 103,

    /// Retry publishing stream successfully
    ZEGO_STREAM_EVENT_RETRY_PUBLISH_SUCCESS = 104,

    /// Failed to retry publishing stream
    ZEGO_STREAM_EVENT_RETRY_PUBLISH_FAIL = 105,

    /// End of publishing stream
    ZEGO_STREAM_EVENT_PUBLISH_END = 106,

    /// Start playing stream
    ZEGO_STREAM_EVENT_PLAY_START = 200,

    /// The first play stream was successful
    ZEGO_STREAM_EVENT_PLAY_SUCCESS = 201,

    /// Failed to play stream for the first time
    ZEGO_STREAM_EVENT_PLAY_FAIL = 202,

    /// Start retrying playing stream
    ZEGO_STREAM_EVENT_RETRY_PLAY_START = 203,

    /// Retry playing stream successfully
    ZEGO_STREAM_EVENT_RETRY_PLAY_SUCCESS = 204,

    /// Failed to retry playing stream
    ZEGO_STREAM_EVENT_RETRY_PLAY_FAIL = 205,

    /// End of playing stream
    ZEGO_STREAM_EVENT_PLAY_END = 206

};

/// Type of capture target.
enum ZegoScreenCaptureSourceType {
    /// Unknow.
    ZEGO_SCREEN_CAPTURE_SOURCE_TYPE_UNKNOW = 0,

    /// The capture target is a certain window.
    ZEGO_SCREEN_CAPTURE_SOURCE_TYPE_WINDOW = 1,

    /// The capture target is the screen of a certain monitor.
    ZEGO_SCREEN_CAPTURE_SOURCE_TYPE_SCREEN = 2

};

/// Low light enhanced mode.
enum ZegoLowlightEnhancementMode {
    /// Turn off the low-light enhancement, and the brightness of the captured image remains unchanged.
    ZEGO_LOWLIGHT_ENHANCEMENT_MODE_OFF = 0,

    /// Turn on low-light enhancement to enhance the brightness of the captured image.
    ZEGO_LOWLIGHT_ENHANCEMENT_MODE_ON = 1,

    /// Automatic low-light enhancement, the brightness enhancement value of the captured picture is dynamically adjusted with the ambient brightness.
    ZEGO_LOWLIGHT_ENHANCEMENT_MODE_AUTO = 2

};

/// Low light enhanced type.
enum ZegoExpLowlightEnhancementType {
    /// Normal low light enhancement.
    ZEGO_EXP_LOWLIGHT_ENHANCEMENT_TYPE_NORMAL = 0,

    /// AI low light enhancement. If you want to use this function, contact ZEGO technical support.
    ZEGO_EXP_LOWLIGHT_ENHANCEMENT_TYPE_AI = 1

};

/// Range scene state change reason.
enum ZegoSceneState {
    /// Logging in to the scene. When calling [loginScene] to log in to the scene, it will enter this state, indicating that it is requesting to connect to the server. The application interface is usually displayed through this state.
    ZEGO_SCENE_STATE_LOGINING = 0,

    /// Log in to the scene successfully. When the scene is successfully logged in, it will enter this state, indicating that the login to the scene has been successful, and users can normally use the range scene capabilities.
    ZEGO_SCENE_STATE_LOGINED = 1,

    /// Failed to log in to the scene. When the login fails, it will enter this state, indicating that the login scene has failed, for example, AppID or Token is incorrect, etc.
    ZEGO_SCENE_STATE_LOGIN_FAILED = 2,

    /// The scene connection is temporarily interrupted. If the interruption occurs due to poor network quality, the SDK will retry internally.
    ZEGO_SCENE_STATE_RECONNECTING = 3,

    /// The scene is successfully reconnected. If there is an interruption due to poor network quality, the SDK will retry internally, and enter this state after successful reconnection.
    ZEGO_SCENE_STATE_RECONNECTED = 4,

    /// The scene fails to reconnect. If there is an interruption due to poor network quality, the SDK will retry internally, and enter this state after the reconnection fails.
    ZEGO_SCENE_STATE_RECONNECT_FAILED = 5,

    /// Kicked out of the scene by the server. For example, if you log in to the scene with the same user name in other places, and the local end is kicked out of the scene, it will enter this state.
    ZEGO_SCENE_STATE_KICK_OUT = 6,

    /// Logout of the scene is successful. It is in this state by default before logging into the scene. When calling [logoutScene] to log out of the scene successfully, it will enter this state.
    ZEGO_SCENE_STATE_LOGOUT = 7

};

/// Stream state.
enum ZegoStreamState {
    /// Stop playing stream.
    ZEGO_STREAM_STATE_STOPED = 0,

    /// Start request playing stream.
    ZEGO_STREAM_STATE_STARTING = 1,

    /// Start playing stream.
    ZEGO_STREAM_STATE_STARTED = 2

};

/// Device state.
enum ZegoDeviceState {
    /// Device off.
    ZEGO_DEVICE_STATE_CLOSE = 0,

    /// Device on.
    ZEGO_DEVICE_STATE_OPEN = 1

};

/// Broadcast user's own information mode.
enum ZegoBroadcastMode {
    /// Do not broadcast user's own information.
    ZEGO_BROADCAST_MODE_NONE = 0,

    /// Broadcast user's own information.
    ZEGO_BROADCAST_MODE_ALL = 1

};

/// Team state.
enum ZegoTeamState {
    /// Joining the team. When calling [joinTeam] to join the team, it will enter this state, indicating that it is requesting to connect to the server. The application interface is usually displayed through this state.
    ZEGO_TEAM_STATE_LOGINING = 0,

    /// Join the room successfully. When joined the team successfully, it will enter this state.
    ZEGO_TEAM_STATE_LOGINED = 1,

    /// Failed to join the room. When the join team fails, it will enter this state.
    ZEGO_TEAM_STATE_LOGIN_FAILED = 2,

    /// The team connection is temporarily interrupted. If the interruption occurs due to poor network quality, the SDK will retry internally.
    ZEGO_TEAM_STATE_RECONNECTING = 3,

    /// The team connection is successfully reconnected. If there is an interruption due to poor network quality, the SDK will retry internally, and enter this state after successful reconnection.
    ZEGO_TEAM_STATE_RECONNECTED = 4,

    /// The team connection fails to reconnect. If there is an interruption due to poor network quality, the SDK will retry internally, and enter this state after the reconnection fails.
    ZEGO_TEAM_STATE_RECONNECT_FAILED = 5,

    /// Kicked out of the team by the server. For example, if you join the team with the same user name in other places, and the local end is kicked out of the team, it will enter this state.
    ZEGO_TEAM_STATE_KICK_OUT = 6,

    /// Leave the team is successful. It is in this state by default before join the team. When calling [leaveTeam] to leave the team successfully, it will enter this state.
    ZEGO_TEAM_STATE_LOGOUT = 7

};

/// The mode of create item.
enum ZegoCreateItemMode {
    /// Do not bind user after item is created.
    ZEGO_CREATE_ITEM_MODE_NO_BIND = 0,

    /// Bind the user after creating the item.
    ZEGO_CREATE_ITEM_MODE_BIND = 1

};

/// Super resolution mode.
enum ZegoSuperResolutionState {
    /// Super resolution turned off.
    ZEGO_SUPER_RESOLUTION_STATE_OFF = 0,

    /// Super resolution turned on.
    ZEGO_SUPER_RESOLUTION_STATE_ON = 1

};

/// video capture source.
enum ZegoVideoSourceType {
    /// No capture, i.e. no video data.
    ZEGO_VIDEO_SOURCE_TYPE_NONE = 1,

    /// The video source comes from the camera (main channel default, and front camera is captured by default). The default is front camera, which can be adjusted to rear via [useFrontCamera].
    ZEGO_VIDEO_SOURCE_TYPE_CAMERA = 2,

    /// Video source from custom capture.
    ZEGO_VIDEO_SOURCE_TYPE_CUSTOM = 3,

    /// Video source from the main publish channel. When publishing the main channel, this value cannot be set.
    ZEGO_VIDEO_SOURCE_TYPE_MAIN_PUBLISH_CHANNEL = 4,

    /// Video source from media player.
    ZEGO_VIDEO_SOURCE_TYPE_PLAYER = 5,

    /// Video source from screen capture.
    ZEGO_VIDEO_SOURCE_TYPE_SCREEN_CAPTURE = 6,

    /// @deprecated Which video source to be used will determined by SDK
    ZEGO_VIDEO_SOURCE_DEFAULT ZEGO_DEPRECATED_ENUM = 7,

    /// @deprecated Same as [None], that is, no capture
    ZEGO_VIDEO_SOURCE_NONE ZEGO_DEPRECATED_ENUM = 8,

    /// @deprecated Same as [Camera], that is, video source from camera
    ZEGO_VIDEO_SOURCE_CAMERA ZEGO_DEPRECATED_ENUM = 9,

    /// @deprecated Same as [Custom], that is, video source from custom capture
    ZEGO_VIDEO_SOURCE_EXTERNAL_CAPTURE ZEGO_DEPRECATED_ENUM = 10,

    /// @deprecated Same as [MainPublishChannel], that is, video source from the main publish channel
    ZEGO_VIDEO_SOURCE_MAIN_PUBLISH_CHANNEL ZEGO_DEPRECATED_ENUM = 11,

    /// @deprecated Same as [Player], that is, video source from media player
    ZEGO_VIDEO_SOURCE_PLAYER ZEGO_DEPRECATED_ENUM = 12,

    /// @deprecated Same as [ScreenCapture], that is, video source from screen capture
    ZEGO_VIDEO_SOURCE_SCREEN_CAPTURE ZEGO_DEPRECATED_ENUM = 13,

    /// Video source from secondary camera, the rear camera when [useFrontCamera] is set to true, otherwise the front camera, only support iOS.
    ZEGO_VIDEO_SOURCE_TYPE_SECONDARY_CAMERA = 14

};

/// Screen capture source exception type.
enum ZegoScreenCaptureSourceExceptionType {
    /// Unknown exception type.
    ZEGO_SCREEN_CAPTURE_SOURCE_EXCEPTION_TYPE_UNKNOWN = 0,

    /// The capture target fails, such as the monitor is unplugged and the window is closed.
    ZEGO_SCREEN_CAPTURE_SOURCE_EXCEPTION_TYPE_INVALID = 1,

    /// Failed to collect target, internal reasons of the system.
    ZEGO_SCREEN_CAPTURE_SOURCE_EXCEPTION_TYPE_FAILED = 2,

    /// The set publish region is invalid, and the publish region is not within the capture screen region.
    ZEGO_SCREEN_CAPTURE_SOURCE_EXCEPTION_TYPE_PUBLISH_REGION_INVALID = 3

};

/// The state of the screen capture source window changes.
enum ZegoScreenCaptureWindowState {
    /// The window is on the current screen, and the coordinate area changes.
    ZEGO_SCREEN_CAPTURE_WINDOW_STATE_ON_SCREEN = 0,

    /// The window leaves the current screen and pauses the capture.
    ZEGO_SCREEN_CAPTURE_WINDOW_STATE_OFF_SCREEN = 1,

    /// The window is destroy.
    ZEGO_SCREEN_CAPTURE_WINDOW_STATE_DESTROY = 2

};

/// Multimedia load type.
enum ZegoMultimediaLoadType {
    /// Load by file path.
    ZEGO_MULTIMEDIA_LOAD_TYPE_FILE_PATH = 0,

    /// Load by memory.
    ZEGO_MULTIMEDIA_LOAD_TYPE_MEMORY = 1,

    /// Load by copyrighted music resource ID.
    ZEGO_MULTIMEDIA_LOAD_TYPE_RESOURCE_ID = 2

};

/// Alpha channel data layout.
enum ZegoAlphaLayoutType {
    /// There is no alpha data.
    ZEGO_ALPHA_LAYOUT_TYPE_NONE = 0,

    /// Alpha channel data is to the left of RGB/YUV data.
    ZEGO_ALPHA_LAYOUT_TYPE_LEFT = 1,

    /// Alpha channel data is to the right of RGB/YUV data.
    ZEGO_ALPHA_LAYOUT_TYPE_RIGHT = 2,

    /// Alpha channel data is to the bottom of RGB/YUV data.
    ZEGO_ALPHA_LAYOUT_TYPE_BOTTOM = 3,

    /// Alpha channel data is to the upper right of RGB/YUV data.
    ZEGO_ALPHA_LAYOUT_TYPE_RIGHT_TOP = 4

};

/// Object segmentation type.
enum ZegoObjectSegmentationType {
    /// Any background object segmentation.
    ZEGO_OBJECT_SEGMENTATION_TYPE_ANY_BACKGROUND = 0,

    /// Green screen background object segmentation.
    ZEGO_OBJECT_SEGMENTATION_TYPE_GREEN_SCREEN_BACKGROUND = 1

};

/// Object segmentation state.
enum ZegoObjectSegmentationState {
    /// Object segmentation turned off.
    ZEGO_OBJECT_SEGMENTATION_STATE_OFF = 0,

    /// Object segmentation turned on.
    ZEGO_OBJECT_SEGMENTATION_STATE_ON = 1

};

/// Video background process type.
enum ZegoBackgroundProcessType {
    /// Background is transparent.
    ZEGO_BACKGROUND_PROCESS_TYPE_TRANSPARENT = 0,

    /// Fill the background with a solid color.
    ZEGO_BACKGROUND_PROCESS_TYPE_COLOR = 1,

    /// Blur background.
    ZEGO_BACKGROUND_PROCESS_TYPE_BLUR = 2,

    /// The background is the specified image.
    ZEGO_BACKGROUND_PROCESS_TYPE_IMAGE = 3,

    /// The background is the specified video.
    ZEGO_BACKGROUND_PROCESS_TYPE_VIDEO = 4

};

/// Background blur level.
enum ZegoBackgroundBlurLevel {
    /// Background blur level low.
    ZEGO_BACKGROUND_BLUR_LEVEL_LOW = 0,

    /// Background blur level medium.
    ZEGO_BACKGROUND_BLUR_LEVEL_MEDIUM = 1,

    /// Background blur level high.
    ZEGO_BACKGROUND_BLUR_LEVEL_HIGH = 2

};

/// The config of the media data publisher.
enum ZegoMediaDataPublisherMode {
    /// Both publish video and audio.
    ZEGO_MEDIA_DATA_PUBLISHER_MODE_BOTH_VIDEO_AND_AUDIO = 0,

    /// Only publish video.
    ZEGO_MEDIA_DATA_PUBLISHER_MODE_ONLY_VIDEO = 1

};

/// Live audio effect mode.
enum ZegoLiveAudioEffectMode {
    /// No audio effect.
    ZEGO_LIVE_AUDIO_EFFECT_MODE_NONE = 0,

    /// Only local play.
    ZEGO_LIVE_AUDIO_EFFECT_MODE_LOCAL = 1,

    /// Only publish.
    ZEGO_LIVE_AUDIO_EFFECT_MODE_PUBLISH = 2,

    /// Publish and local play.
    ZEGO_LIVE_AUDIO_EFFECT_MODE_ALL = 3

};

/// AI voice changer event.
enum ZegoAIVoiceChangerEvent {
    /// Succeed.
    ZEGO_AI_VOICE_CHANGER_EVENT_SUCCEED = 0,

    /// Background performance insufficient.
    ZEGO_AI_VOICE_CHANGER_EVENT_BACKGROUND_PERFORMANCE_INSUFFICIENT = 1,

    /// Foreground performance insufficient.
    ZEGO_AI_VOICE_CHANGER_EVENT_FOREGROUND_PERFORMANCE_INSUFFICIENT = 2,

    /// Others.
    ZEGO_AI_VOICE_CHANGER_EVENT_OTHERS = 3,

    /// Set speaker failed.
    ZEGO_AI_VOICE_CHANGER_EVENT_SET_SPEAKER_FAILED = 10

};

/// Media stream type.
enum ZegoMediaStreamType {
    /// Media audio stream.
    ZEGO_MEDIA_STREAM_TYPE_AUDIO = 0,

    /// Media video stream.
    ZEGO_MEDIA_STREAM_TYPE_VIDEO = 1,

    /// Media audio and video stream.
    ZEGO_MEDIA_STREAM_TYPE_AV = 2

};

/// Dump data type.
enum ZegoDumpDataType {
    /// Audio.
    ZEGO_DUMP_DATA_TYPE_AUDIO = 0

};

/// Usage of Custom video processed data.
enum ZegoProcessedDataUsageType {
    /// Preview.
    ZEGO_PROCESSED_DATA_USAGE_TYPE_PREVIEW = 0,

    /// Publish.
    ZEGO_PROCESSED_DATA_USAGE_TYPE_PUBLISH = 1,

    /// Both preview and publish.
    ZEGO_PROCESSED_DATA_USAGE_TYPE_BOTH = 2

};

/// Dummy capture image mode.
enum ZegoDummyCaptureImageMode {
    /// Manual mode. The user needs to call the [EnableCamera] interface to turn off camera capture, and the SDK will use dummy capture image.
    ZEGO_DUMMY_CAPTURE_IMAGE_MODE_MANUAL = 0,

    /// Auto mode. After the SDK detects that the camera is unavailable, it uses dummy capture image to puublish the stream.
    ZEGO_DUMMY_CAPTURE_IMAGE_MODE_AUTO = 1

};

/// Log config.
///
/// Description: This parameter is required when calling [setlogconfig] to customize log configuration.
/// Use cases: This configuration is required when you need to customize the log storage path or the maximum log file size.
/// Caution: None.
struct ZegoLogConfig {
    /// The storage path of the log file. Description: Used to customize the storage path of the log file. Use cases: This configuration is required when you need to customize the log storage path. Required: False. Default value: The default path of each platform is different, please refer to the official website document https://docs.zegocloud.com/faq/express_sdkLog. Caution: Developers need to ensure read and write permissions for files under this path.
    std::string logPath;

    /// Maximum log file size(Bytes). Description: Used to customize the maximum log file size. Use cases: This configuration is required when you need to customize the upper limit of the log file size. Required: False. Default value: 5MB (5 * 1024 * 1024 Bytes). Value range: Minimum 1MB (1 * 1024 * 1024 Bytes), maximum 100M (100 * 1024 * 1024 Bytes), 0 means no need to write logs. Caution: The larger the upper limit of the log file size, the more log information it carries, but the log upload time will be longer.
    unsigned long long logSize;

    /// Log files count. Default is 3. Value range is [3, 20].
    unsigned int logCount;

    ZegoLogConfig() {
        logSize = 5 * 1024 * 1024;
        logCount = 3;
    }
};

/// Custom video capture configuration.
///
/// Custom video capture, that is, the developer is responsible for collecting video data and sending the collected video data to SDK for video data encoding and publishing to the ZEGO RTC server. This feature is generally used by developers who use third-party beauty features or record game screen living.
/// When you need to use the custom video capture function, you need to set an instance of this class as a parameter to the [enableCustomVideoCapture] function.
/// Because when using custom video capture, SDK will no longer start the camera to capture video data. You need to collect video data from video sources by yourself.
struct ZegoCustomVideoCaptureConfig {
    /// Custom video capture video frame data type
    ZegoVideoBufferType bufferType;
};

/// Custom video process configuration.
struct ZegoCustomVideoProcessConfig {
    /// Custom video process video frame data type
    ZegoVideoBufferType bufferType;
};

/// Custom video render configuration.
///
/// When you need to use the custom video render function, you need to set an instance of this class as a parameter to the [enableCustomVideoRender] function.
struct ZegoCustomVideoRenderConfig {
    /// Custom video capture video frame data type
    ZegoVideoBufferType bufferType;

    /// Custom video rendering video frame data format。Useless when set bufferType as [EncodedData]
    ZegoVideoFrameFormatSeries frameFormatSeries;

    /// Whether the engine also renders while customizing video rendering. The default value is [false]. Useless when set bufferType as [EncodedData]
    bool enableEngineRender;
};

/// Custom audio configuration.
struct ZegoCustomAudioConfig {
    /// Audio capture source type
    ZegoAudioSourceType sourceType;
};

/// Profile for create engine
///
/// Profile for create engine
struct ZegoEngineProfile {
    /// Application ID issued by ZEGO for developers, please apply from the ZEGO Admin Console https://console.zegocloud.com The value ranges from 0 to 4294967295.
    unsigned int appID;

    /// Application signature for each AppID, please apply from the ZEGO Admin Console. Application signature is a 64 character string. Each character has a range of '0' ~ '9', 'a' ~ 'z'. AppSign 2.17.0 and later allows null or no transmission. If the token is passed empty or not passed, the token must be entered in the [ZegoRoomConfig] parameter for authentication when the [loginRoom] interface is called to login to the room.
    std::string appSign;

    /// The room scenario. the SDK will optimize the audio and video configuration for the specified scenario to achieve the best effect in this scenario. After specifying the scenario, you can call other APIs to adjusting the audio and video configuration. Differences between scenarios and how to choose a suitable scenario, please refer to https://docs.zegocloud.com/article/14940
    ZegoScenario scenario;

    /// only windows and mac need callback switch to ui thread
    bool callbackSwitchToMainThread;

    ZegoEngineProfile() : appID(0), appSign(""), callbackSwitchToMainThread(true) {}
};

/// Advanced engine configuration.
struct ZegoEngineConfig {
    /// @deprecated This property has been deprecated since version 2.3.0, please use the [setLogConfig] function instead.
    ZegoLogConfig *logConfig;

    /// Other special function switches, if not set, no special function will be used by default. Please contact ZEGO technical support before use.
    std::unordered_map<std::string, std::string> advancedConfig;

    ZegoEngineConfig() { logConfig = nullptr; }
};

/// Proxy config.
///
/// Set proxy config.
struct ZegoProxyInfo {
    /// ip. Under local proxy: local proxy ip configured by the developer. under cloud proxy: proxy ip provided by ZEGO (either  hostname or ip will do).
    std::string ip;

    /// port. Under Local Proxy: Local proxy port configured by the developer. Under Cloud Proxy: Proxy port provided by ZEGO.
    unsigned int port;

    /// hostname.Under Local Proxy: Local proxy hostname configured by the developer. Under cloud proxy: proxy hostname provided by ZEGO. proxy hostname (either hostname or ip will do).
    std::string hostName;

    /// userName.Under Local Proxy: the authentication username of the local Proxy configured by the developer, can be ignored if there is none. Under cloud proxy: can be ignored.
    std::string userName;

    /// password.Under local proxy: authentication password of local proxy configured by developer, can be ignored if there is none. Under cloud proxy: can be ignored.
    std::string password;

    ZegoProxyInfo() : ip(""), port(0), hostName(""), userName(""), password("") {}
};

/// Advanced room configuration.
///
/// Configure maximum number of users in the room and authentication token, etc.
struct ZegoRoomConfig {
    /// The maximum number of users in the room, Passing 0 means unlimited, the default is unlimited.
    unsigned int maxMemberCount;

    /// Whether to enable the user in and out of the room callback notification [onRoomUserUpdate], the default is off. If developers need to use ZEGO Room user notifications, make sure that each user who login sets this flag to true
    bool isUserStatusNotify;

    /// The token issued by the developer's business server is used to ensure security. For the generation rules, please refer to [Using Token Authentication](https://doc-zh.zego.im/article/10360), the default is an empty string, that is, no authentication. In versions 2.17.0 and above, if appSign is not passed in when calling the [createEngine] API to create an engine, or if appSign is empty, this parameter must be set for authentication when logging in to a room.
    std::string token;

    /// The bitmask marker for capability negotiation, refer to enum [ZegoRoomCapabilityNegotiationTypesBitMask], when this param converted to binary, 0b01 that means 1 << 0 for enable the capability negotiation of all user in the room, 0x10 that means 1 << 1 for enable the capability negotiation of publisher in the room. The masks can be combined to allow different types of capability negotiation.
    unsigned int capabilityNegotiationTypes;

    ZegoRoomConfig()
        : maxMemberCount(0), isUserStatusNotify(false), token(""), capabilityNegotiationTypes(0) {}
};

/// Video config.
///
/// Configure parameters used for publishing stream, such as bitrate, frame rate, and resolution.
/// Developers should note that the width and height resolution of the mobile and desktop are opposite. For example, 360p, the resolution of the mobile is 360x640, and the desktop is 640x360.
/// When using external capture, the capture and encoding resolution of RTC cannot be set to 0*0, otherwise, there will be no video data in the publishing stream in the entire engine life cycle.
struct ZegoVideoConfig {
    /// Capture resolution width, control the width of camera image acquisition. SDK requires this member to be set to an even number. Only the camera is not started and the custom video capture is not used, the setting is effective. For performance reasons, the SDK scales the video frame to the encoding resolution after capturing from camera and before rendering to the preview view. Therefore, the resolution of the preview image is the encoding resolution. If you need the resolution of the preview image to be this value, Please call [setCapturePipelineScaleMode] first to change the capture pipeline scale mode to [Post]
    int captureWidth;

    /// Capture resolution height, control the height of camera image acquisition. SDK requires this member to be set to an even number. Only the camera is not started and the custom video capture is not used, the setting is effective. For performance reasons, the SDK scales the video frame to the encoding resolution after capturing from camera and before rendering to the preview view. Therefore, the resolution of the preview image is the encoding resolution. If you need the resolution of the preview image to be this value, Please call [setCapturePipelineScaleMode] first to change the capture pipeline scale mode to [Post]
    int captureHeight;

    /// Encode resolution width, control the image width of the encoder when publishing stream. SDK requires this member to be set to an even number. The settings before and after publishing stream can be effective
    int encodeWidth;

    /// Encode resolution height, control the image height of the encoder when publishing stream. SDK requires this member to be set to an even number. The settings before and after publishing stream can be effective
    int encodeHeight;

    /// Frame rate, control the frame rate of the camera and the frame rate of the encoder. Publishing stream set to 60 fps, playing stream to take effect need contact technical support
    int fps;

    /// Bit rate in kbps. The settings before and after publishing stream can be effective. The SDK will automatically set the bit rate suitable for the scenario selected by the developer. If the bit rate manually set by the developer exceeds the reasonable range, the SDK will automatically process the bit rate according to the reasonable range. If you need to configure a high bit rate due to business needs, please contact ZEGO Business.
    int bitrate;

    /// The codec id to be used, the default value is [default]. Settings only take effect before publishing stream
    ZegoVideoCodecID codecID;

    /// Video keyframe interval, in seconds. Description: Required: No. Default value: 2 seconds. Value range: [2, 5]. Caution: The setting is only valid before pushing.
    int keyFrameInterval;

    ZegoVideoConfig(ZegoVideoConfigPreset preset = ZEGO_VIDEO_CONFIG_PRESET_360P) {
        codecID = ZEGO_VIDEO_CODEC_ID_DEFAULT;
        keyFrameInterval = 2;
        switch (preset) {
        case ZEGO_VIDEO_CONFIG_PRESET_180P:
            captureWidth = 320;
            captureHeight = 180;
            encodeWidth = 320;
            encodeHeight = 180;
            bitrate = 300;
            fps = 15;
            break;
        case ZEGO_VIDEO_CONFIG_PRESET_270P:
            captureWidth = 480;
            captureHeight = 270;
            encodeWidth = 480;
            encodeHeight = 270;
            bitrate = 400;
            fps = 15;
            break;
        case ZEGO_VIDEO_CONFIG_PRESET_360P:
            captureWidth = 640;
            captureHeight = 360;
            encodeWidth = 640;
            encodeHeight = 360;
            bitrate = 600;
            fps = 15;
            break;
        case ZEGO_VIDEO_CONFIG_PRESET_540P:
            captureWidth = 960;
            captureHeight = 540;
            encodeWidth = 960;
            encodeHeight = 540;
            bitrate = 1200;
            fps = 15;
            break;
        case ZEGO_VIDEO_CONFIG_PRESET_720P:
            captureWidth = 1280;
            captureHeight = 720;
            encodeWidth = 1280;
            encodeHeight = 720;
            bitrate = 1500;
            fps = 15;
            break;
        case ZEGO_VIDEO_CONFIG_PRESET_1080P:
            captureWidth = 1920;
            captureHeight = 1080;
            encodeWidth = 1920;
            encodeHeight = 1080;
            bitrate = 3000;
            fps = 15;
            break;
        }
    }
};

/// Externally encoded data traffic control information.
struct ZegoTrafficControlInfo {
    /// Video resolution width to be adjusted
    int width;

    /// Video resolution height to be adjusted
    int height;

    /// Video FPS to be adjusted
    int fps;

    /// Video bitrate in kbps to be adjusted
    int bitrate;
};

/// Dual stream info.
struct ZegoPublishDualStreamConfig {
    /// streamType
    ZegoVideoStreamType streamType;

    /// Video resolution width to be adjusted
    int encodeWidth;

    /// Video resolution height to be adjusted
    int encodeHeight;

    /// Video FPS to be adjusted
    int fps;

    /// Video bitrate in kbps to be adjusted
    int bitrate;
};

/// SEI configuration
///
/// Used to set the relevant configuration of the Supplemental Enhancement Information.
struct ZegoSEIConfig {
    /// SEI type
    ZegoSEIType type;

    ZegoSEIConfig(ZegoSEIType type = ZEGO_SEI_TYPE_ZEGO_DEFINED) : type(type) {}
};

/// Voice changer parameter.
///
/// Developer can use the built-in presets of the SDK to change the parameters of the voice changer.
struct ZegoVoiceChangerParam {
    /// Pitch parameter, value range [-12.0, 12.0], the larger the value, the sharper the sound, set it to 0.0 to turn off. Note that on v2.18.0 and older version, the value range is [-8.0, 8.0].
    float pitch;
};

/// Audio reverberation parameters.
///
/// Developers can use the SDK's built-in presets to change the parameters of the reverb.
struct ZegoReverbParam {
    /// Room size, in the range [0.0, 1.0], to control the size of the "room" in which the reverb is generated, the larger the room, the stronger the reverb.
    float roomSize;

    /// Echo, in the range [0.0, 0.5], to control the trailing length of the reverb.
    float reverberance;

    /// Reverb Damping, range [0.0, 2.0], controls the attenuation of the reverb, the higher the damping, the higher the attenuation.
    float damping;

    /// Dry/wet ratio, the range is greater than or equal to 0.0, to control the ratio between reverberation, direct sound and early reflections; dry part is set to 1 by default; the smaller the dry/wet ratio, the larger the wet ratio, the stronger the reverberation effect.
    float dryWetRatio;
};

/// Audio reverberation advanced parameters.
///
/// Developers can use the SDK's built-in presets to change the parameters of the reverb.
struct ZegoReverbAdvancedParam {
    /// Room size(%), in the range [0.0, 1.0], to control the size of the "room" in which the reverb is generated, the larger the room, the stronger the reverb.
    float roomSize;

    /// Echo(%), in the range [0.0, 100.0], to control the trailing length of the reverb.
    float reverberance;

    /// Reverb Damping(%), range [0.0, 100.0], controls the attenuation of the reverb, the higher the damping, the higher the attenuation.
    float damping;

    /// only wet
    bool wetOnly;

    /// wet gain(dB), range [-20.0, 10.0]
    float wetGain;

    /// dry gain(dB), range [-20.0, 10.0]
    float dryGain;

    /// Tone Low. 100% by default
    float toneLow;

    /// Tone High. 100% by default
    float toneHigh;

    /// PreDelay(ms), range [0.0, 200.0]
    float preDelay;

    /// Stereo Width(%). 0% by default
    float stereoWidth;

    ZegoReverbAdvancedParam()
        : roomSize(.0f), reverberance(.0f), damping(.0f), wetOnly(false), wetGain(.0f),
          dryGain(.0f), toneLow(100.0f), toneHigh(100.0f), preDelay(.0f), stereoWidth(.0f) {}
};

/// Audio reverberation echo parameters.
struct ZegoReverbEchoParam {
    /// Gain of input audio signal, in the range [0.0, 1.0]
    float inGain;

    /// Gain of output audio signal, in the range [0.0, 1.0]
    float outGain;

    /// Number of echos, in the range [0, 7]
    int numDelays;

    /// Respective delay of echo signal, in milliseconds, in the range [0, 5000] ms
    int delay[7];

    /// Respective decay coefficient of echo signal, in the range [0.0, 1.0]
    float decay[7];
};

/// User object.
///
/// Configure user ID and username to identify users in the room.
/// Note that the userID must be unique under the same appID, otherwise, there will be mutual kicks when logging in to the room.
/// It is strongly recommended that userID corresponds to the user ID of the business APP, that is, a userID and a real user are fixed and unique, and should not be passed to the SDK in a random userID. Because the unique and fixed userID allows ZEGO technicians to quickly locate online problems.
struct ZegoUser {
    /// User ID, a utf8 string with a maximum length of 64 bytes or less.Privacy reminder: Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.Caution: Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'. Do not use '%' if you need to communicate with the Web SDK.
    std::string userID;

    /// User Name, a utf8 string with a maximum length of 256 bytes or less.Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.
    std::string userName;

    ZegoUser() {}
    ZegoUser(std::string userID, std::string userName) : userID(userID), userName(userName) {}
};

/// Stream object.
///
/// Identify an stream object
struct ZegoStream {
    /// User object instance.Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.
    ZegoUser user;

    /// Stream ID, a string of up to 256 characters. Caution: You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '-', '_'.
    std::string streamID;

    /// Stream extra info
    std::string extraInfo;
};

/// Room stream list.
///
/// Room stream list.
struct ZegoRoomStreamList {
    /// Publish stream list
    std::vector<ZegoStream> publishStreamList;

    /// Play stream list
    std::vector<ZegoStream> playStreamList;
};

/// Room extra information.
struct ZegoRoomExtraInfo {
    /// The key of the room extra information.
    std::string key;

    /// The value of the room extra information.
    std::string value;

    /// The user who update the room extra information.Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.
    ZegoUser updateUser;

    /// Update time of the room extra information, UNIX timestamp, in milliseconds.
    unsigned long long updateTime;
};

/// View related coordinates.
struct ZegoRect {
    /// The horizontal offset from the top-left corner
    int x;

    /// The vertical offset from the top-left corner
    int y;

    /// The width of the rectangle
    int width;

    /// The height of the rectangle
    int height;

    ZegoRect() : x(0), y(0), width(0), height(0) {}
    ZegoRect(int x, int y, int w, int h) : x(x), y(y), width(w), height(h) {}
};

/// Coordinates used by the ROI function.
struct ZegoRoiRect {
    /// The horizontal offset from the top-left corner
    int x;

    /// The vertical offset from the top-left corner
    int y;

    /// The width of the rectangle
    int width;

    /// The height of the rectangle
    int height;

    /// ROI strength, currently supported value range is [0, 4], 0 is no effect, 4 is the strongest.
    int strength;
};

/// View object.
///
/// Configure view object, view Mode, background color
struct ZegoCanvas {
    /// View object
    void *view;

    /// View mode, default is ZegoViewModeAspectFit
    ZegoViewMode viewMode;

    /// Background color, the format is 0xRRGGBB, default is black, which is 0x000000
    int backgroundColor;

    /// If enable alpha blend render, default is false.
    bool alphaBlend;

    ZegoCanvas(void *view = nullptr, ZegoViewMode viewMode = ZEGO_VIEW_MODE_ASPECT_FIT,
               int backgroundColor = 0x000000)
        : view(view), viewMode(viewMode), backgroundColor(backgroundColor), alphaBlend(false) {}
};

/// Advanced publisher configuration.
///
/// Configure room id
struct ZegoPublisherConfig {
    /// The Room ID, It is not necessary to pass in single room mode, but the ID of the corresponding room must be passed in multi-room mode
    std::string roomID;

    /// Whether to synchronize the network time when pushing streams. 1 is synchronized with 0 is not synchronized. And must be used with setStreamAlignmentProperty. It is used to align multiple streams at the mixed stream service or streaming end, such as the chorus scene of KTV.
    int forceSynchronousNetworkTime;

    /// When pushing a flow, review the pattern of the flow. By default, no audit is performed. If you want to use this function, contact ZEGO technical support.
    ZegoStreamCensorshipMode streamCensorshipMode;

    /// Inspect flag. If you want to use this function, contact ZEGO technical support.
    int streamCensorFlag;

    /// Codec capability negotiation type. By default, no reference to the outcome of the capability negotiation. If you want to use this function, contact ZEGO technical support.
    ZegoCapabilityNegotiationType codecNegotiationType;

    /// Stream title, a utf8 string with a maximum length of 255 bytes or less.
    std::string streamTitle;

    ZegoPublisherConfig()
        : roomID(""), forceSynchronousNetworkTime(0),
          streamCensorshipMode(ZEGO_STREAM_CENSORSHIP_MODE_NONE), streamCensorFlag(0),
          codecNegotiationType(ZEGO_CAPABILITY_NEGOTIATION_TYPE_NONE) {}
};

/// Advanced scene publisher configuration.
struct ZegoScenePublisherConfig {
    /// Range scene handle. Obtained through [getHandle] of ZegoRangeScene instance.
    int rangeSceneHandle;

    ZegoScenePublisherConfig() : rangeSceneHandle(-1) {}
};

/// Published stream quality information.
///
/// Audio and video parameters and network quality, etc.
struct ZegoPublishStreamQuality {
    /// Video capture frame rate. The unit of frame rate is f/s
    double videoCaptureFPS;

    /// Video encoding frame rate. The unit of frame rate is f/s
    double videoEncodeFPS;

    /// Video transmission frame rate. The unit of frame rate is f/s
    double videoSendFPS;

    /// Video bit rate in kbps
    double videoKBPS;

    /// Audio capture frame rate. The unit of frame rate is f/s
    double audioCaptureFPS;

    /// Audio transmission frame rate. The unit of frame rate is f/s
    double audioSendFPS;

    /// Audio bit rate in kbps
    double audioKBPS;

    /// Local to server delay, in milliseconds
    int rtt;

    /// Packet loss rate, in percentage, 0.0 ~ 1.0
    double packetLostRate;

    /// Published stream quality level
    ZegoStreamQualityLevel level;

    /// Whether to enable hardware encoding
    bool isHardwareEncode;

    /// Video codec ID (Available since 1.17.0)
    ZegoVideoCodecID videoCodecID;

    /// Total number of bytes sent, including audio, video, SEI
    double totalSendBytes;

    /// Number of audio bytes sent
    double audioSendBytes;

    /// Number of video bytes sent
    double videoSendBytes;
};

/// CDN config object.
///
/// Includes CDN URL and authentication parameter string
struct ZegoCDNConfig {
    /// CDN URL
    std::string url;

    /// Auth param of URL. Please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored (set to null or empty string).
    std::string authParam;

    /// URL supported protocols, candidate values are "tcp" and "quic". If there are more than one, separate them with English commas and try them in order. Please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored (set to null or empty string).
    std::string protocol;

    /// QUIC version。 If [protocol] has the QUIC protocol, this information needs to be filled in. If there are multiple version numbers, separate them with commas. Please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored (set to null or empty string).
    std::string quicVersion;

    /// customized httpdns service. This feature is only supported for playing stream currently.
    ZegoHttpDNSType httpdns;

    /// QUIC establishes link mode. If the value is 1, quic 0 rtt is used preferentially to establish link. Otherwise, the link is established normally. If [protocol] has the QUIC protocol, this value takes effect.
    int quicConnectMode;

    /// custom param of URL. Please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored (set to null or empty string).
    std::string customParams;

    ZegoCDNConfig() : quicConnectMode(0) {}
};

/// Relay to CDN info.
///
/// Including the URL of the relaying CDN, relaying state, etc.
struct ZegoStreamRelayCDNInfo {
    /// URL of publishing stream to CDN
    std::string url;

    /// State of relaying to CDN
    ZegoStreamRelayCDNState state;

    /// Reason for relay state changed
    ZegoStreamRelayCDNUpdateReason updateReason;

    /// The timestamp when the state changed, UNIX timestamp, in milliseconds.
    unsigned long long stateTime;
};

/// Custom play stream resource type configuration.
///
/// Custom play stream resource type configuration.
struct ZegoCustomPlayerResourceConfig {
    /// The resource type selected by the play stream before starting the publish stream.
    ZegoResourceType beforePublish;

    /// The type of resource selected by the play stream in the publish stream.
    ZegoResourceType publishing;

    /// The type of resource selected by the play stream after stopping the publish stream.
    ZegoResourceType afterPublish;

    ZegoCustomPlayerResourceConfig()
        : beforePublish(ZEGO_RESOURCE_TYPE_RTC), publishing(ZEGO_RESOURCE_TYPE_RTC),
          afterPublish(ZEGO_RESOURCE_TYPE_RTC) {}
};

/// Advanced player configuration.
///
/// Configure stream resource mode, CDN configuration and other advanced configurations.
struct ZegoPlayerConfig {
    /// Stream resource mode.
    ZegoStreamResourceMode resourceMode;

    /// The CDN configuration for playing stream. If set, the stream is play according to the URL instead of the streamID. After that, the streamID is only used as the ID of SDK internal callback.
    ZegoCDNConfig *cdnConfig;

    /// The Room ID. It only needs to be filled in the multi-room mode, which indicates which room this stream needs to be bound to. This parameter is ignored in single room mode.
    std::string roomID;

    /// The video encoding type of the stream, please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored.
    ZegoVideoCodecID videoCodecID;

    /// The resource type of the source stream, please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored.
    ZegoResourceType sourceResourceType;

    /// Preconfigured codec template ID, please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored.
    int codecTemplateID;

    /// Play resource switching strategy mode, the default is ZegoStreamResourceSwitchModeDefault
    ZegoStreamResourceSwitchMode resourceSwitchMode;

    /// Play resource type when stop publish, the default is ZegoStreamResourceTypeDefault. This setting takes effect only if the user sets [resourceMode] to ZegoStreamResourceModeDefaut and [resourceSwitchMode] to ZegoStreamResourceSwitchModeDefault or ZegoStreamResourceSwitchModeSwitchToRTC.
    ZegoStreamResourceType resourceWhenStopPublish;

    /// Whether to enable adaptive switching of streams, 1 means on, 0 means off. Valid only if [resourceMode] is ZegoStreamResourceModeOnlyL3. Please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored.
    int adaptiveSwitch;

    /// Stream adaptive transcoding template ID list. Please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored.
    std::vector<int> adaptiveTemplateIDList;

    /// Play stream resource type configuration when [resourceMode] is ZegoStreamResourceModeCustom.
    ZegoCustomPlayerResourceConfig customResourceConfig;

    ZegoPlayerConfig()
        : resourceMode(ZEGO_STREAM_RESOURCE_MODE_DEFAULT), cdnConfig(nullptr), roomID(""),
          videoCodecID(ZEGO_VIDEO_CODEC_ID_UNKNOWN), sourceResourceType(ZEGO_RESOURCE_TYPE_RTC),
          codecTemplateID(0), resourceSwitchMode(ZEGO_STREAM_RESOURCE_SWITCH_MODE_DEFAULT),
          resourceWhenStopPublish(ZEGO_STREAM_RESOURCE_TYPE_DEFAULT), adaptiveSwitch(0) {}
};

/// Advanced scene player configuration.
struct ZegoScenePlayerConfig {
    /// Range scene handle. Obtained through [getHandle] of ZegoRangeScene instance.
    int rangeSceneHandle;

    /// Stream resource mode.
    ZegoStreamResourceMode resourceMode;

    /// The CDN configuration for playing stream. If set, the stream is play according to the URL instead of the streamID. After that, the streamID is only used as the ID of SDK internal callback.
    ZegoCDNConfig *cdnConfig;

    ZegoScenePlayerConfig()
        : rangeSceneHandle(-1), resourceMode(ZEGO_STREAM_RESOURCE_MODE_DEFAULT),
          cdnConfig(nullptr) {}
};

/// Played stream quality information.
///
/// Audio and video parameters and network quality, etc.
struct ZegoPlayStreamQuality {
    /// Video receiving frame rate. The unit of frame rate is f/s
    double videoRecvFPS;

    /// Video dejitter frame rate. The unit of frame rate is f/s (Available since 1.17.0)
    double videoDejitterFPS;

    /// Video decoding frame rate. The unit of frame rate is f/s
    double videoDecodeFPS;

    /// Video rendering frame rate. The unit of frame rate is f/s
    double videoRenderFPS;

    /// Video bit rate in kbps
    double videoKBPS;

    /// Video break rate, the unit is (number of breaks / every 10 seconds) (Available since 1.17.0)
    double videoBreakRate;

    /// Audio receiving frame rate. The unit of frame rate is f/s
    double audioRecvFPS;

    /// Audio dejitter frame rate. The unit of frame rate is f/s (Available since 1.17.0)
    double audioDejitterFPS;

    /// Audio decoding frame rate. The unit of frame rate is f/s
    double audioDecodeFPS;

    /// Audio rendering frame rate. The unit of frame rate is f/s
    double audioRenderFPS;

    /// Audio bit rate in kbps
    double audioKBPS;

    /// Audio break rate, the unit is (number of breaks / every 10 seconds) (Available since 1.17.0)
    double audioBreakRate;

    /// The audio quality of the playing stream determined by the audio MOS (Mean Opinion Score) measurement method, value range [-1, 5], where -1 means unknown, [0, 5] means valid score, the higher the score, the better the audio quality. For the subjective perception corresponding to the MOS value, please refer to https://docs.zegocloud.com/article/3720#4_4 (Available since 2.16.0)
    double mos;

    /// Server to local delay, in milliseconds
    int rtt;

    /// Packet loss rate, in percentage, 0.0 ~ 1.0
    double packetLostRate;

    /// Delay from peer to peer, in milliseconds
    int peerToPeerDelay;

    /// Packet loss rate from peer to peer, in percentage, 0.0 ~ 1.0
    double peerToPeerPacketLostRate;

    /// Published stream quality level
    ZegoStreamQualityLevel level;

    /// Delay after the data is received by the local end, in milliseconds
    int delay;

    /// The difference between the video timestamp and the audio timestamp, used to reflect the synchronization of audio and video, in milliseconds. This value is less than 0 means the number of milliseconds that the video leads the audio, greater than 0 means the number of milliseconds that the video lags the audio, and 0 means no difference. When the absolute value is less than 200, it can basically be regarded as synchronized audio and video, when the absolute value is greater than 200 for 10 consecutive seconds, it can be regarded as abnormal (Available since 1.19.0)
    int avTimestampDiff;

    /// Whether to enable hardware decoding
    bool isHardwareDecode;

    /// Video codec ID (Available since 1.17.0)
    ZegoVideoCodecID videoCodecID;

    /// Total number of bytes received, including audio, video, SEI
    double totalRecvBytes;

    /// Number of audio bytes received
    double audioRecvBytes;

    /// Number of video bytes received
    double videoRecvBytes;

    /// Accumulated audio break count (Available since 2.9.0)
    unsigned int audioCumulativeBreakCount;

    /// Accumulated audio break time, in milliseconds (Available since 2.9.0)
    unsigned int audioCumulativeBreakTime;

    /// Accumulated audio break rate, in percentage, 0.0 ~ 100.0 (Available since 2.9.0)
    double audioCumulativeBreakRate;

    /// Accumulated audio decode time, in milliseconds (Available since 2.9.0)
    unsigned int audioCumulativeDecodeTime;

    /// Accumulated video break count (Available since 2.9.0)
    unsigned int videoCumulativeBreakCount;

    /// Accumulated video break time, in milliseconds (Available since 2.9.0)
    unsigned int videoCumulativeBreakTime;

    /// Accumulated video break rate, in percentage, 0.0 ~ 1.0 (Available since 2.9.0)
    double videoCumulativeBreakRate;

    /// Accumulated video decode time, in milliseconds (Available since 2.9.0)
    unsigned int videoCumulativeDecodeTime;

    /// Mute video (Available since 3.13.0)
    int muteVideo;

    /// Mute audio (Available since 3.13.0)
    int muteAudio;
};

/// Cross APP playing stream configuration.
struct ZegoCrossAppInfo {
    /// AppID for playing streams across apps.
    unsigned int appID;

    /// The token that needs to be set.
    std::string token;

    ZegoCrossAppInfo() : appID(0), token("") {}
};

/// SEI Callback info.
struct ZegoMediaSideInfo {
    /// Stream ID.
    std::string streamID;

    /// SEI data
    const unsigned char *SEIData;

    /// Length of the SEI data
    unsigned int SEIDataLength;

    /// timestamp
    long long timestampNs;

    /// SEI source module. Please contact ZEGO technical support.
    int moduleType;
};

/// Device Info.
///
/// Including device ID and name
struct ZegoDeviceInfo {
    /// Device ID
    std::string deviceID;

    /// Device name
    std::string deviceName;

    /// Device extra info, Format: key="value"\nkey2="value2"..., use line break \n to separate key-value pairs, and use equal sign = to separate key and "value", and there are double quotes around the value
    std::string deviceExtraInfo;
};

/// System performance monitoring status
struct ZegoPerformanceStatus {
    /// Current CPU usage of the app, value range [0, 1]
    double cpuUsageApp;

    /// Current CPU usage of the system, value range [0, 1]
    double cpuUsageSystem;

    /// Current memory usage of the app, value range [0, 1]
    double memoryUsageApp;

    /// Current memory usage of the system, value range [0, 1]
    double memoryUsageSystem;

    /// Current memory used of the app, in MB
    double memoryUsedApp;
};

/// Beauty configuration param.
///
/// Configure the whiten, rosy, smooth, and sharpen parameters for beauty.
struct ZegoEffectsBeautyParam {
    /// The whiten intensity parameter, the value range is [0,100], and the default is 50.
    int whitenIntensity;

    /// the rosy intensity parameter, value range [0,100], and the default is 50.
    int rosyIntensity;

    /// the smooth intensity parameter, value range [0,100], and the default is 50.
    int smoothIntensity;

    /// the sharpen intensity parameter, value range [0,100], and the default is 50.
    int sharpenIntensity;
};

/// Mix stream audio configuration.
///
/// Configure video frame rate, bitrate, and resolution for mixer task
struct ZegoMixerAudioConfig {
    /// Audio bitrate in kbps, default is 48 kbps, cannot be modified after starting a mixer task
    int bitrate;

    /// Audio channel, default is Mono
    ZegoAudioChannel channel;

    /// codec ID, default is ZegoAudioCodecIDDefault
    ZegoAudioCodecID codecID;

    /// Multi-channel audio stream mixing mode. If [ZegoAudioMixMode] is selected as [Focused], the SDK will select 4 input streams with [isAudioFocus] set as the focus voice highlight. If it is not selected or less than 4 channels are selected, it will automatically fill in 4 channels. On web platforms, this property does not take effect.
    ZegoAudioMixMode mixMode;

    ZegoMixerAudioConfig()
        : bitrate(48), channel(ZEGO_AUDIO_CHANNEL_MONO), codecID(ZEGO_AUDIO_CODEC_ID_DEFAULT),
          mixMode(ZEGO_AUDIO_MIX_MODE_RAW) {}
    ZegoMixerAudioConfig(int bitrate, ZegoAudioChannel channel, ZegoAudioCodecID codecID)
        : bitrate(bitrate), channel(channel), codecID(codecID), mixMode(ZEGO_AUDIO_MIX_MODE_RAW) {}
};

/// Mix stream video config object.
///
/// Configure video frame rate, bitrate, and resolution for mixer task
struct ZegoMixerVideoConfig {
    /// Video resolution width
    int width;

    /// Video resolution height
    int height;

    /// Video FPS, cannot be modified after starting a mixer task
    int fps;

    /// Video bitrate in kbps
    int bitrate;

    /// Video quality, this value is valid when the video rate control mode parameter is set to constant quality. The valid value ranges from 0 to 51. The default value is 23. If you want better video quality, lower the quality value based on 23 to test the adjustment. If you want a smaller file size, test the adjustment by increasing the high quality value at the base of 23. Take the file size under the value x as an example. The file size under the value x + 6 is half the size of the file size under the value x, and the file size under the value x-6 is twice the size of the file size under the value x. On web platforms, this property does not take effect.
    int quality;

    /// Video bitrate control mode.  On web platforms, this property does not take effect.
    ZegoVideoRateControlMode rateControlMode;

    ZegoMixerVideoConfig()
        : width(640), height(360), fps(15), bitrate(600), quality(23),
          rateControlMode(ZEGO_VIDEO_RATE_CONTROL_MODE_CONSTANT_RATE) {}
    ZegoMixerVideoConfig(int width, int height, int fps, int bitrate)
        : width(width), height(height), fps(fps), bitrate(bitrate), quality(23),
          rateControlMode(ZEGO_VIDEO_RATE_CONTROL_MODE_CONSTANT_RATE) {}
};

/// Mix stream output video config object.
///
/// Description: Configure the video parameters, coding format and bitrate of mix stream output.
/// Use cases: Manual mixed stream scenario, such as Co-hosting.
struct ZegoMixerOutputVideoConfig {
    /// Mix stream output video coding format, supporting H.264 and h.265 coding.
    ZegoVideoCodecID videoCodecID;

    /// Mix stream output video bitrate in kbps. The default value is the bitrate configured in [ZegoMixerVideoConfig].
    int bitrate;

    /// Mix stream video encode profile. Default value is [ZegoEncodeProfileDefault].
    ZegoEncodeProfile encodeProfile;

    /// The video encoding delay of mixed stream output, Valid value range [0, 2000], in milliseconds. The default value is 0.
    int encodeLatency;

    /// Enable high definition low bitrate. Default is false.
    bool enableLowBitrateHD;

    ZegoMixerOutputVideoConfig()
        : videoCodecID(ZEGO_VIDEO_CODEC_ID_DEFAULT), bitrate(0),
          encodeProfile(ZEGO_ENCODE_PROFILE_DEFAULT), encodeLatency(0), enableLowBitrateHD(false) {}
    ZegoMixerOutputVideoConfig(ZegoVideoCodecID codecID, int bitrate)
        : videoCodecID(codecID), bitrate(bitrate), encodeProfile(ZEGO_ENCODE_PROFILE_DEFAULT),
          encodeLatency(0), enableLowBitrateHD(false) {}
    ZegoMixerOutputVideoConfig(ZegoVideoCodecID codecID, int bitrate, ZegoEncodeProfile profile,
                               int encodeLatency)
        : videoCodecID(codecID), bitrate(bitrate), encodeProfile(profile),
          encodeLatency(encodeLatency), enableLowBitrateHD(false) {}
};

/// Room information for the output stream in a mixed stream.
///
/// Available since: 3.18.0
/// Description: Setting room information for a single output stream; the mixed output stream can be added to the specified room, allowing users in the room to receive notifications of increased stream in the room.
/// Use cases: Manual mixed stream scenario, such as Co-hosting.
/// Restrictions: Dynamic updates during mixed stream are not supported.
struct ZegoMixerOutputRoomInfo {
    /// Specifies the room ID of the output stream. You need to ensure that the room is already present when mixing starts.
    std::string roomID;

    /// Specifies the user ID of the output stream. It is not recommended to use the same userID as the actual user in the room to avoid conflicts with the SDK's stream addition behavior.
    std::string userID;

    /// Construct the room information of an output stream in a mixed stream by using roomID and userID.
    ZegoMixerOutputRoomInfo(const std::string &roomID, const std::string &userID)
        : roomID(roomID), userID(userID) {}
};

/// Font style.
///
/// Description: Font style configuration, can be used to configure font type, font size, font color, font transparency.
/// Use cases: Set text watermark in manual stream mixing scene, such as Co-hosting.
struct ZegoFontStyle {
    /// Font type. Required: False. Default value: Source han sans [ZegoFontTypeSourceHanSans]
    ZegoFontType type;

    /// Font size in px. Required: False. Default value: 24. Value range: [12,100].
    int size;

    /// Font color, the calculation formula is: R + G x 256 + B x 65536, the value range of R (red), G (green), and B (blue) [0,255]. Required: False. Default value: 16777215(white). Value range: [0,16777215].
    int color;

    /// Font transparency. Required: False. Default value: 0. Value range: [0,100], 100 is completely opaque, 0 is completely transparent.
    int transparency;

    /// Whether the font has a border. Required: False. Default value: False. Value range: True/False.
    bool border;

    /// Font border color, the calculation formula is: R + G x 256 + B x 65536, the value range of R (red), G (green), and B (blue) [0,255]. Required: False. Default value: 0. Value range: [0,16777215].
    int borderColor;

    /// Create a default font style object.
    ZegoFontStyle()
        : type(ZEGO_FONT_TYPE_SOURCE_HAN_SANS), size(24), color(16777215), transparency(0),
          border(false), borderColor(0) {}
};

/// Label info.
///
/// Description: Font style configuration, can be used to configure font type, font si-e, font color, font transparency.
/// Use cases: Set text watermark in manual stream mixing scene, such as Co-hosting.
struct ZegoLabelInfo {
    /// Text content, support for setting simplified Chinese, English, half-width, not full-width. Required: True.Value range: Maximum support for displaying 100 Chinese characters and 300 English characters.
    std::string text;

    /// The distance between the font and the left border of the output canvas, in px. Required: False. Default value: 0.
    int left;

    /// The distance between the font and the top border of the output canvas, in px. Required: False. Default value: 0.
    int top;

    /// Font style. Required: False.
    ZegoFontStyle font;

    /// Build a label info object with text.
    ZegoLabelInfo(std::string text) : text(text), left(0), top(0), font() {}
};

/// Set the image information of a single input stream in the mux.
///
/// Available since: 2.19.0
/// Description: Sets a picture for the content of a single input stream, which is used in place of the video, i.e. the video is not displayed when the picture is used. The `layout` layout in [ZegoMixerInput] for image multiplexing.
/// Use case: The developer needs to temporarily turn off the camera to display the image during the video connection to the microphone, or display the picture when the audio is connected to the microphone.
/// Restrictions: Image size is limited to 1M.
struct ZegoMixerImageInfo {
    /// The image path, if not empty, the image will be displayed, otherwise, the video will be displayed. JPG and PNG formats are supported. There are 2 ways to use it: 1. URI: Provide the picture to ZEGO technical support for configuration. After the configuration is complete, the picture URI will be provided, for example: preset-id://xxx.jpg. 2. URL: Only HTTP protocol is supported.
    std::string url;

    /// Image display mode. 0: Default. Use image to replace video content when url is not null. 1: Display image based on camera status. Display image when camera is turned off. Display video content when camera is turned on (no need to clear the url parameter). 2: Display image based on the input stream has video data or not. Display image when there is no video data in the input stream for 3 consecutive seconds. The default duration for judging that there is no video data is 3 seconds. If you need change this setting, please contact ZEGO technical support. Display video content when the input stream has video data.
    int displayMode;

    /// Build a image info object with url.
    ZegoMixerImageInfo(std::string url) : url(url), displayMode(0) {}
    /// Build a image info object with url and displayMode.
    ZegoMixerImageInfo(std::string url, int displayMode) : url(url), displayMode(displayMode) {}
};

/// Mixer input.
///
/// Configure the mix stream input stream ID, type, and the layout
struct ZegoMixerInput {
    /// Stream ID, a string of up to 256 characters. Caution: You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '-', '_'.
    std::string streamID;

    /// Mix stream content type
    ZegoMixerInputContentType contentType;

    /// Stream layout. When the mixed stream is an audio stream (that is, the ContentType parameter is set to the audio mixed stream type). Developers do not need to assign a value to this field, just use the SDK default.
    ZegoRect layout;

    /// If enable soundLevel in mix stream task, an unique soundLevelID is need for every stream
    unsigned int soundLevelID;

    /// Input stream volume, valid range [0, 200], default is 100. On web platforms, this property does not take effect.
    unsigned int volume;

    /// Whether the focus voice is enabled in the current input stream, the sound of this stream will be highlighted if enabled. On web platforms, this property does not take effect.
    bool isAudioFocus;

    /// The direction of the audio. Valid direction is between 0 to 360. Set -1 means disable. Default value is -1. On web platforms, this property does not take effect.
    int audioDirection;

    /// Text watermark.
    ZegoLabelInfo label;

    /// Video view render mode.
    ZegoMixRenderMode renderMode;

    /// User image information.
    ZegoMixerImageInfo imageInfo;

    /// Description: Video frame corner radius, in px. Required: False. Value range: Does not exceed the width and height of the video screen set by the [layout] parameter. Default value: 0.
    unsigned int cornerRadius;

    /// Set advanced configuration. Please contact ZEGO technical support. On web platforms, this property does not take effect.
    std::map<std::string, std::string> advancedConfig;

    ZegoMixerInput()
        : streamID(), contentType(ZEGO_MIXER_INPUT_CONTENT_TYPE_VIDEO), layout(), soundLevelID(0),
          volume(100), isAudioFocus(false), audioDirection(-1), label(""),
          renderMode(ZEGO_MIX_RENDER_MODE_FILL), imageInfo(""), cornerRadius(0) {}
    ZegoMixerInput(std::string streamID, ZegoMixerInputContentType contentType, ZegoRect layout,
                   unsigned int soundLevelID)
        : streamID(streamID), contentType(contentType), layout(layout), soundLevelID(soundLevelID),
          volume(100), isAudioFocus(false), audioDirection(-1), label(""),
          renderMode(ZEGO_MIX_RENDER_MODE_FILL), imageInfo(""), cornerRadius(0) {}
};

/// Mixer output object, currently, a mixed-stream task only supports a maximum of four video streams with different resolutions.
///
/// Configure mix stream output target URL or stream ID
struct ZegoMixerOutput {
    /// Mix stream output target, URL or stream ID, if set to be URL format, only RTMP URL surpported, for example rtmp://xxxxxxxx, addresses with two identical mixed-stream outputs cannot be passed in.
    std::string target;

    /// Mix stream output video config. On web platforms, this property does not take effect.
    ZegoMixerOutputVideoConfig videoConfig;

    /// Specifies the room information for the output stream.
    ZegoMixerOutputRoomInfo targetRoom;

    ZegoMixerOutput() : videoConfig(), targetRoom("", "") {}
    ZegoMixerOutput(std::string target) : target(target), videoConfig(), targetRoom("", "") {}
};

/// Watermark object.
///
/// Configure a watermark image URL and the layout of the watermark in the screen.
struct ZegoWatermark {
    /// The path of the watermark image. Support local file absolute path (file://xxx). The format supports png, jpg. The maximum length is less than 512 bytes.
    std::string imageURL;

    /// Watermark image layout
    ZegoRect layout;

    ZegoWatermark() {}
    ZegoWatermark(std::string imageURL, ZegoRect layout) : imageURL(imageURL), layout(layout) {}
};

/// Mixer whiteboard object.
///
/// Configure the mix whiteboard ID, aspect ratio and the layout.
struct ZegoMixerWhiteboard {
    /// Whiteboard ID.
    unsigned long long whiteboardID;

    /// Whiteboard aspect ratio(width), the default aspect ratio is 16:9.
    int horizontalRatio;

    /// Whiteboard aspect ratio(height), the default aspect ratio is 16:9.
    int verticalRatio;

    /// Whether the whiteboard will load dynamic PPT files or not, default value is false.
    bool isPPTAnimation;

    /// Whiteboard layout.
    ZegoRect layout;

    /// Whiteboard z-order.
    int zOrder;

    /// Whiteboard background color. Defaule is 0xF1F3F400 (gray). The color value corresponding to RGBA is 0xRRGGBBAA, and setting the transparency of the background color is currently not supported. The AA in 0xRRGGBBAA is 00. For example, select RGB as \#87CEFA as the background color, this parameter passes 0x87CEFA00.F
    int backgroundColor;

    ZegoMixerWhiteboard() {
        whiteboardID = 0;
        horizontalRatio = 16;
        verticalRatio = 9;
        isPPTAnimation = false;
        zOrder = 0;
        backgroundColor = 0xF1F3F400;
    }
};

/// Mix stream task object.
///
/// This class is the configuration class of the stream mixing task. When a stream mixing task is requested to the ZEGO RTC server, the configuration of the stream mixing task is required.
/// This class describes the detailed configuration information of this stream mixing task.
struct ZegoMixerTask {
    /// The task ID of the task
    std::string taskID;

    /// The audio config of the task
    ZegoMixerAudioConfig audioConfig;

    /// The audio config of the task
    ZegoMixerVideoConfig videoConfig;

    /// The input list of the task
    std::vector<ZegoMixerInput> inputList;

    /// The output list of the task
    std::vector<ZegoMixerOutput> outputList;

    /// The watermark of the task
    ZegoWatermark *watermark;

    /// The whiteboard of the task
    ZegoMixerWhiteboard *whiteboard;

    /// The background color of the task, The color value corresponding to RGBA is 0xRRGGBBAA, and setting the transparency of the background color is currently not supported. The AA in 0xRRGGBBAA is 00. For example, select RGB as \#87CEFA as the background color, this parameter passes 0x87CEFA00.
    int backgroundColor;

    /// The background image URL of the task
    std::string backgroundImageURL;

    /// Enable or disable sound level callback for the task. If enabled, then the remote player can get the soundLevel of every stream in the inputlist by [onMixerSoundLevelUpdate] callback.
    bool enableSoundLevel;

    /// The stream mixing alignment mode
    ZegoStreamAlignmentMode streamAlignmentMode;

    /// User data, the length of user data should not be more than 1000 bytes,After setting, the streaming party can obtain the SEI content by listening to the callback of [onPlayerRecvSEI].
    const unsigned char *userData;

    /// User data length, not greater than 1000.Note that only data with length will be read by SDK. If the length is greater than the actual length of data, the SDK will read the data according to the actual length of data.
    unsigned int userDataLength;

    /// Set advanced configuration, such as specifying video encoding and others. If you need to use it, contact ZEGO technical support.
    std::unordered_map<std::string, std::string> advancedConfig;

    /// Description: Sets the lower limit of the interval range for the adaptive adjustment of the stream playing cache of the stream mixing server. In the real-time chorus KTV scenario, slight fluctuations in the network at the push end may cause the mixed stream to freeze. At this time, when the audience pulls the mixed stream, there is a high probability of the problem of freeze. By adjusting the lower limit of the interval range for the adaptive adjustment of the stream playing cache of the stream mixing server, it can optimize the freezing problem that occurs when playing mixing streams at the player end, but it will increase the delay. It is not set by default, that is, the server uses its own configuration values. It only takes effect for the new input stream setting, and does not take effect for the input stream that has already started mixing.Value Range: [0,10000], exceeding the maximum value will result in a failure of the stream mixing request. On web platforms, this property does not take effect.
    int minPlayStreamBufferLength;

    /// Set the mixed stream image check mode.
    ZegoMixImageCheckMode mixImageCheckMode;

    ZegoMixerTask(std::string taskID = "")
        : taskID(taskID), watermark(nullptr), whiteboard(nullptr), backgroundColor(0),
          enableSoundLevel(false), streamAlignmentMode(ZEGO_STREAM_ALIGNMENT_MODE_NONE),
          userData(nullptr), userDataLength(0), minPlayStreamBufferLength(-1),
          mixImageCheckMode(ZEGO_MIX_IMAGE_CHECK_MODE_NORMAL) {}
};

/// Configuration for start sound level monitor.
struct ZegoSoundLevelConfig {
    /// Monitoring time period of the sound level, in milliseconds, has a value range of [100, 3000]. Default is 100 ms.
    unsigned int millisecond;

    /// Set whether the sound level callback includes the VAD detection result.
    bool enableVAD;
};

/// Sound level info object.
struct ZegoSoundLevelInfo {
    /// Sound level value.
    float soundLevel;

    /// Whether the stream corresponding to StreamID contains voice, 0 means noise, 1 means normal voice. This value is valid only when the [enableVAD] parameter in the [ZegoSoundLevelConfig] configuration is set to true when calling [startSoundLevelMonitor].
    int vad;
};

/// Auto mix stream task object.
///
/// Description: When using [StartAutoMixerTask] function to start an auto stream mixing task to the ZEGO RTC server, user need to set this parameter to configure the auto stream mixing task, including the task ID, room ID, audio configuration, output stream list, and whether to enable the sound level callback.
/// Use cases: This configuration is required when an auto stream mixing task is requested to the ZEGO RTC server.
/// Caution: As an argument passed when [StartAutoMixerTask] function is called.
struct ZegoAutoMixerTask {
    /// The taskID of the auto mixer task.Description: Auto stream mixing task id, must be unique in a room.Use cases: User need to set this parameter when initiating an auto stream mixing task.Required: Yes.Recommended value: Set this parameter based on requirements.Value range: A string up to 256 bytes.Caution: When starting a new auto stream mixing task, only one auto stream mixing task ID can exist in a room, that is, to ensure the uniqueness of task ID. You are advised to associate task ID with room ID. You can directly use the room ID as the task ID.Cannot include URL keywords, for example, 'http' and '?' etc, otherwise publishing stream and playing stream will fail. Only support numbers, English characters and '~', '!', '@', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
    std::string taskID;

    /// The roomID of the auto mixer task.Description: Auto stream mixing task id.Use cases: User need to set this parameter when initiating an auto stream mixing task.Required: Yes.Recommended value: Set this parameter based on requirements.Value range: A string up to 128 bytes.Caution: Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.If you need to communicate with the Web SDK, please do not use '%'.
    std::string roomID;

    /// The audio config of the auto mixer task.Description: The audio config of the auto mixer task.Use cases: If user needs special requirements for the audio config of the auto stream mixing task, such as adjusting the audio bitrate, user can set this parameter as required. Otherwise, user do not need to set this parameter.Required: No.Default value: The default audio bitrate is `48 kbps`, the default audio channel is `ZEGO_AUDIO_CHANNEL_MONO`, the default encoding ID is `ZEGO_AUDIO_CODEC_ID_DEFAULT`, and the default multi-channel audio stream mixing mode is `ZEGO_AUDIO_MIX_MODE_RAW`.Recommended value: Set this parameter based on requirements.
    ZegoMixerAudioConfig audioConfig;

    /// The output list of the auto mixer task.Description: The output list of the auto stream mixing task, items in the list are URL or stream ID, if the item set to be URL format, only RTMP URL surpported, for example rtmp://xxxxxxxx.Use cases: User need to set this parameter to specify the mix stream output target when starting an auto stream mixing task.Required: Yes.
    std::vector<ZegoMixerOutput> outputList;

    /// Enable or disable sound level callback for the task. If enabled, then the remote player can get the sound level of every stream in the inputlist by [onAutoMixerSoundLevelUpdate] callback.Description: Enable or disable sound level callback for the task.If enabled, then the remote player can get the sound level of every stream in the inputlist by [onAutoMixerSoundLevelUpdate] callback.Use cases: This parameter needs to be configured if user need the sound level information of every stream when an auto stream mixing task started.Required: No.Default value: `false`.Recommended value: Set this parameter based on requirements.
    bool enableSoundLevel;

    /// Stream mixing alignment mode.
    ZegoStreamAlignmentMode streamAlignmentMode;

    /// Description: Sets the lower limit of the interval range for the adaptive adjustment of the stream playing cache of the stream mixing server. In the real-time chorus KTV scenario, slight fluctuations in the network at the push end may cause the mixed stream to freeze. At this time, when the audience pulls the mixed stream, there is a high probability of the problem of freeze. By adjusting the lower limit of the interval range for the adaptive adjustment of the stream playing cache of the stream mixing server, it can optimize the freezing problem that occurs when playing mixing streams at the player end, but it will increase the delay. It is not set by default, that is, the server uses its own configuration values. It only takes effect for the new input stream setting, and does not take effect for the input stream that has already started mixing.Value Range: [0,10000], exceeding the maximum value will result in a failure of the stream mixing request. On web platforms, this property does not take effect.
    int minPlayStreamBufferLength;

    /// Create a auto mix stream task object
    ZegoAutoMixerTask()
        : taskID(), roomID(), audioConfig(), outputList(), enableSoundLevel(false),
          streamAlignmentMode(ZEGO_STREAM_ALIGNMENT_MODE_NONE), minPlayStreamBufferLength(-1) {}
};

/// Broadcast message info.
///
/// The received object of the room broadcast message, including the message content, message ID, sender, sending time
struct ZegoBroadcastMessageInfo {
    /// message content
    std::string message;

    /// message id
    unsigned long long messageID;

    /// Message send time, UNIX timestamp, in milliseconds.
    unsigned long long sendTime;

    /// Message sender.Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.
    ZegoUser fromUser;
};

/// Barrage message info.
///
/// The received object of the room barrage message, including the message content, message ID, sender, sending time
struct ZegoBarrageMessageInfo {
    /// message content
    std::string message;

    /// message id
    std::string messageID;

    /// Message send time, UNIX timestamp, in milliseconds.
    unsigned long long sendTime;

    /// Message sender.Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.
    ZegoUser fromUser;
};

/// Transparent message info.
///
/// Room transparent message, including room id, message content, sending user, sending type, sending mode, timeout period
struct ZegoRoomSendTransparentMessage {
    /// send mode
    ZegoRoomTransparentMessageMode sendMode;

    /// send type
    ZegoRoomTransparentMessageType sendType;

    /// Message send content.
    std::string content;

    /// Message receiver list, when sendType appointed ZegoRoomTransparentMessageModeOnlyServer don't fill in. When appointed ZegoRoomTransparentMessageModeClientAndServer or ZegoRoomTransparentMessageModeOnlyClient, empty room will be sent to all online users.
    std::vector<ZegoUser> recvUserList;

    /// send message timeout, The default value is 10s.
    int timeOut;

    ZegoRoomSendTransparentMessage()
        : sendMode(ZEGO_ROOM_TRANSPARENT_MESSAGE_MODE_ONLY_CLIENT),
          sendType(ZEGO_ROOM_TRANSPARENT_MESSAGE_NORMAL), timeOut(10) {}
};

/// Received a transparent message from the room.
///
/// Room transparent message, including message content, sending user, sending type, sending mode
struct ZegoRoomRecvTransparentMessage {
    /// send message user
    ZegoUser sendUser;

    /// Message send content.
    std::string content;
};

/// Object for video frame fieldeter.
///
/// Including video frame format, width and height, etc.
struct ZegoVideoFrameParam {
    /// Video frame format
    ZegoVideoFrameFormat format;

    /// Number of bytes per line (for example: BGRA only needs to consider strides [0], I420 needs to consider strides [0,1,2])
    int strides[4];

    /// Video frame width. When use custom video capture, the video data meeting the 32-bit alignment can obtain the maximum performance. Taking BGRA as an example, width * 4 is expected to be multiple of 32.
    int width;

    /// Video frame height
    int height;

    /// The rotation direction of the video frame, the SDK rotates clockwise
    int rotation;
};

/// Object for video encoded frame fieldeter.
///
/// Including video encoded frame format, width and height, etc.
struct ZegoVideoEncodedFrameParam {
    /// Video encoded frame format
    ZegoVideoEncodedFrameFormat format;

    /// Whether it is a keyframe
    bool isKeyFrame;

    /// Video frame counterclockwise rotation angle
    int rotation;

    /// Video frame width
    int width;

    /// Video frame height
    int height;

    /// SEI data (Optional, if you don't need to send SEI, set it to null. Deprecated, use [sendSEI] instead). Useful when set format as [AVCC] or [AnnexB]
    const unsigned char *SEIData;

    /// Length of the SEI data (Optional, if you don't need to send SEI, set it to 0. Deprecated, use [sendSEI] instead). Useful when set format as [AVCC] or [AnnexB]
    unsigned int SEIDataLength;

    /// Whether to use the external timestamp completely. The default is false. When set to false, the SDK will adjust based on the timestamps of the audio frame and video frame to ensure audio-video synchronization. When set to true, the SDK does not adjust the timestamp and uses the external timestamp completely.
    bool isExternalClock;

    ZegoVideoEncodedFrameParam()
        : format(ZEGO_VIDEO_ENCODED_FRAME_FORMAT_AVCC), isKeyFrame(false), rotation(0), width(0),
          height(0), SEIData(nullptr), SEIDataLength(0), isExternalClock(false) {}
};

/// Parameter object for audio frame.
///
/// Including the sampling rate and channel of the audio frame
struct ZegoAudioFrameParam {
    /// Sampling Rate
    ZegoAudioSampleRate sampleRate;

    /// Audio channel, default is Mono
    ZegoAudioChannel channel;

    ZegoAudioFrameParam()
        : sampleRate(ZEGO_AUDIO_SAMPLE_RATE_UNKNOWN), channel(ZEGO_AUDIO_CHANNEL_MONO) {}
};

/// Audio configuration.
///
/// Configure audio bitrate, audio channel, audio encoding for publishing stream
struct ZegoAudioConfig {
    /// Audio bitrate in kbps, default is 48 kbps. The settings before and after publishing stream can be effective
    int bitrate;

    /// Audio channel, default is Mono. The setting only take effect before publishing stream
    ZegoAudioChannel channel;

    /// codec ID, default is ZegoAudioCodecIDDefault. The setting only take effect before publishing stream
    ZegoAudioCodecID codecID;

    ZegoAudioConfig(ZegoAudioConfigPreset preset = ZEGO_AUDIO_CONFIG_PRESET_STANDARD_QUALITY) {
        codecID = ZEGO_AUDIO_CODEC_ID_DEFAULT;
        switch (preset) {
        case ZEGO_AUDIO_CONFIG_PRESET_BASIC_QUALITY:
            bitrate = 16;
            channel = ZEGO_AUDIO_CHANNEL_MONO;
            break;
        case ZEGO_AUDIO_CONFIG_PRESET_STANDARD_QUALITY:
            bitrate = 48;
            channel = ZEGO_AUDIO_CHANNEL_MONO;
            break;
        case ZEGO_AUDIO_CONFIG_PRESET_STANDARD_QUALITY_STEREO:
            bitrate = 56;
            channel = ZEGO_AUDIO_CHANNEL_STEREO;
            break;
        case ZEGO_AUDIO_CONFIG_PRESET_HIGH_QUALITY:
            bitrate = 128;
            channel = ZEGO_AUDIO_CHANNEL_MONO;
            break;
        case ZEGO_AUDIO_CONFIG_PRESET_HIGH_QUALITY_STEREO:
            bitrate = 192;
            channel = ZEGO_AUDIO_CHANNEL_STEREO;
            break;
        }
    }
};

/// audio mixing data.
struct ZegoAudioMixingData {
    /// Audio PCM data that needs to be mixed into the stream
    unsigned char *audioData;

    /// the length of the audio PCM data that needs to be mixed into the stream. If this is no enough data left to mix as audioDataLength, then you can set audioDataLength = 0.
    unsigned int audioDataLength;

    /// Audio data attributes, including sample rate and number of channels. Currently supports 16k, 32k, 44.1k, 48k sampling rate, mono or stereo, 16-bit deep PCM data. Developers need to explicitly specify audio data attributes, otherwise mixing will not take effect.
    ZegoAudioFrameParam param;

    /// SEI data, used to transfer custom data. When audioData is null, SEIData will not be sent
    unsigned char *SEIData;

    /// SEI data length
    unsigned int SEIDataLength;
};

/// Customize the audio processing configuration object.
///
/// Including custom audio acquisition type, sampling rate, channel number, sampling number and other parameters
struct ZegoCustomAudioProcessConfig {
    /// Sampling rate, the sampling rate of the input data expected by the audio pre-processing module in App. If 0, the default is the SDK internal sampling rate.
    ZegoAudioSampleRate sampleRate;

    /// Number of sound channels, the expected number of sound channels for input data of the audio pre-processing module in App. If 0, the default is the number of internal channels in the SDK
    ZegoAudioChannel channel;

    /// The number of samples required to encode a frame; if samples = 0, the SDK will use the internal sample number, and the SDK will pass the audio data to the external pre-processing module. If the samples! = 0 (the effective value of samples is between [160, 2048]), and the SDK will send audio data to the external preprocessing module that sets the length of sample number.
    int samples;
};

/// Record config.
struct ZegoDataRecordConfig {
    /// The path to save the recording file, absolute path, need to include the file name, the file name need to specify the suffix, currently supports .mp4/.flv/.aac format files, if multiple recording for the same path, will overwrite the file with the same name. The maximum length should be less than 1024 bytes.
    std::string filePath;

    /// Type of recording media
    ZegoDataRecordType recordType;
};

/// File recording progress.
struct ZegoDataRecordProgress {
    /// Current recording duration in milliseconds
    unsigned long long duration;

    /// Current recording file size in byte
    unsigned long long currentFileSize;

    /// The quality of current recording file
    ZegoPublishStreamQuality quality;
};

/// Network probe config
struct ZegoNetworkProbeConfig {
    /// Whether do traceroute, enabling tranceRoute will significantly increase network detection time
    bool enableTraceroute;
};

/// http probe result
struct ZegoNetworkProbeHttpResult {
    /// http probe errorCode, 0 means the connection is normal
    int errorCode;

    /// http request cost time, the unit is millisecond
    unsigned int requestCostTime;
};

/// tcp probe result
struct ZegoNetworkProbeTcpResult {
    /// tcp probe errorCode, 0 means the connection is normal
    int errorCode;

    /// tcp rtt, the unit is millisecond
    unsigned int rtt;

    /// tcp connection cost time, the unit is millisecond
    unsigned int connectCostTime;
};

/// udp probe result
struct ZegoNetworkProbeUdpResult {
    /// udp probe errorCode, 0 means the connection is normal
    int errorCode;

    /// The total time that the SDK send udp data to server and receive a reply, the unit is millisecond
    unsigned int rtt;
};

/// traceroute result
///
/// Jump up to 30 times. The traceroute result is for reference and does not represent the final network connection result. The priority is http, tcp, udp probe result.
struct ZegoNetworkProbeTracerouteResult {
    /// traceroute error code, 0 means normal
    int errorCode;

    /// Time consumed by trace route, the unit is millisecond
    unsigned int tracerouteCostTime;
};

/// Network probe result
struct ZegoNetworkProbeResult {
    /// http probe result
    ZegoNetworkProbeHttpResult *httpProbeResult;

    /// tcp probe result
    ZegoNetworkProbeTcpResult *tcpProbeResult;

    /// udp probe result
    ZegoNetworkProbeUdpResult *udpProbeResult;

    /// traceroute result
    ZegoNetworkProbeTracerouteResult *tracerouteResult;
};

/// Network speed test config
struct ZegoNetworkSpeedTestConfig {
    /// Test uplink or not
    bool testUplink;

    /// The unit is kbps. Recommended to use the bitrate in ZegoVideoConfig when call startPublishingStream to determine whether the network uplink environment is suitable.
    int expectedUplinkBitrate;

    /// Test downlink or not
    bool testDownlink;

    /// The unit is kbps. Recommended to use the bitrate in ZegoVideoConfig when call startPublishingStream to determine whether the network downlink environment is suitable.
    int expectedDownlinkBitrate;
};

/// test connectivity result
struct ZegoTestNetworkConnectivityResult {
    /// connect cost
    unsigned int connectCost;
};

/// network speed test quality
struct ZegoNetworkSpeedTestQuality {
    /// Time to connect to the server, in milliseconds. During the speed test, if the network connection is disconnected, it will automatically initiate a reconnection, and this variable will be updated accordingly.
    unsigned int connectCost;

    /// rtt, in milliseconds
    unsigned int rtt;

    /// packet lost rate. in percentage, 0.0 ~ 1.0
    double packetLostRate;

    /// network quality. excellent, good, medium and poor
    ZegoStreamQualityLevel quality;
};

/// RTC Network Statistics
struct ZegoRtcStatsInfo {
    /// total upstream bandwidth, in kbps
    double totalTxBandwidth;

    /// upstream average rtt, in milliseconds
    unsigned int avgTxRtt;

    /// upstream average packet lost rate. in percentage, 0.0 ~ 1.0
    double avgTxPacketLostRate;

    /// total downlink bandwidth, in kbps
    double totalRxBandwidth;

    /// downlink average rtt, in milliseconds
    unsigned int avgRxRtt;

    /// downlink average packet lost rate. in percentage, 0.0 ~ 1.0
    double avgRxPacketLostRate;

    /// average peer to peer delay, in milliseconds
    unsigned int avgPeerToPeerDelay;
};

/// The NTP info
struct ZegoNetworkTimeInfo {
    /// Network timestamp after synchronization, 0 indicates not yet synchronized
    unsigned long long timestamp;

    /// The max deviation
    int maxDeviation;
};

/// AudioEffectPlayer play configuration.
struct ZegoAudioEffectPlayConfig {
    /// The number of play counts. When set to 0, it will play in an infinite loop until the user invoke [stop]. The default is 1, which means it will play only once.
    unsigned int playCount;

    /// Whether to mix audio effects into the publishing stream, the default is false.
    bool isPublishOut;
};

/// Precise seek configuration
struct ZegoAccurateSeekConfig {
    /// The timeout time for precise search; if not set, the SDK internal default is set to 5000 milliseconds, the effective value range is [2000, 10000], the unit is ms
    unsigned long long timeout;

    ZegoAccurateSeekConfig() { timeout = 5000; }
};

/// Media player network cache information
struct ZegoNetWorkResourceCache {
    /// Cached playable duration, unit ms
    unsigned int time;

    /// Cached playable size, unit byte
    unsigned int size;
};

/// CopyrightedMusic play configuration.
struct ZegoCopyrightedMusicConfig {
    /// User object instance, configure userID, userName. Note that the user ID set here needs to be consistent with the user ID set when logging in to the room, otherwise the request for the copyright music background service will fail.
    ZegoUser user;
};

/// The configuration of getting lyric.
struct ZegoCopyrightedMusicGetLyricConfig {
    /// the ID of the song.
    std::string songID;

    /// Copyright music resource song copyright provider.
    int vendorID;

    ZegoCopyrightedMusicGetLyricConfig() { vendorID = 0; }
};

/// The configuration of requesting resource.
struct ZegoCopyrightedMusicRequestConfig {
    /// the ID of the song.
    std::string songID;

    /// VOD billing mode.
    ZegoCopyrightedMusicBillingMode mode;

    /// Copyright music resource song copyright provider.
    ZegoCopyrightedMusicVendorID vendorID;

    /// The room ID, the single-room mode can not be passed, and the corresponding room ID must be passed in the multi-room mode. Indicate in which room to order song/accompaniment/accompaniment clip/accompaniment segment.
    std::string roomID;

    /// The master ID, which must be passed when the billing mode is billed by host. Indicate which homeowner to order song/accompaniment/accompaniment clip/accompaniment segment.
    std::string masterID;

    /// The scene ID, indicate the actual business. For details, please consult ZEGO technical support.
    int sceneID;

    ZegoCopyrightedMusicRequestConfig() {
        mode = ZEGO_COPYRIGHTED_MUSIC_BILLING_MODE_COUNT;
        vendorID = ZEGO_COPYRIGHTED_MUSIC_VENDOR_DEFAULT;
        sceneID = 0;
    }
};

/// The configuration of requesting resource.
struct ZegoCopyrightedMusicRequestConfigV2 {
    /// the ID of the song.
    std::string songID;

    /// VOD billing mode. Refer to the value of [ZegoCopyrightedMusicBillingMode].
    int mode;

    /// Copyright music resource song copyright provider. Refer to the value of [ZegoCopyrightedMusicVendorID].
    int vendorID;

    /// The room ID, the single-room mode can not be passed, and the corresponding room ID must be passed in the multi-room mode. Indicate in which room to order song/accompaniment/accompaniment clip/accompaniment segment.
    std::string roomID;

    /// The master ID, which must be passed when the billing mode is billed by host. Indicate which homeowner to order song/accompaniment/accompaniment clip/accompaniment segment.
    std::string masterID;

    /// The scene ID, indicate the actual business. For details, please consult ZEGO technical support.
    int sceneID;

    /// The resource type of music. Refer to the value of [ZegoCopyrightedMusicResourceType].
    int resourceType;

    ZegoCopyrightedMusicRequestConfigV2() {
        mode = 0;
        vendorID = 0;
        sceneID = 0;
        resourceType = 0;
    }
};

/// The configuration of getting shared resource.
struct ZegoCopyrightedMusicGetSharedConfig {
    /// the ID of the song.
    std::string songID;

    /// Copyright music resource song copyright provider.
    ZegoCopyrightedMusicVendorID vendorID;

    /// The room ID, the single-room mode can not be passed, and the corresponding room ID must be passed in the multi-room mode. Indicates which room to get resources from.
    std::string roomID;

    ZegoCopyrightedMusicGetSharedConfig() { vendorID = ZEGO_COPYRIGHTED_MUSIC_VENDOR_DEFAULT; }
};

/// The configuration of getting shared resource.
struct ZegoCopyrightedMusicGetSharedConfigV2 {
    /// the ID of the song.
    std::string songID;

    /// Copyright music resource song copyright provider. Refer to the value of [ZegoCopyrightedMusicVendorID].
    int vendorID;

    /// The room ID, the single-room mode can not be passed, and the corresponding room ID must be passed in the multi-room mode. Indicates which room to get resources from.
    std::string roomID;

    /// The resource type of music.
    int resourceType;

    ZegoCopyrightedMusicGetSharedConfigV2() {
        vendorID = 0;
        resourceType = 0;
    }
};

/// The configuration of querying cache.
struct ZegoCopyrightedMusicQueryCacheConfig {
    /// the ID of the song.
    std::string songID;

    /// The resource type of music.
    ZegoCopyrightedMusicResourceType resourceType;

    /// The resource quality type of music.
    ZegoCopyrightedMusicResourceQualityType resourceQualityType;

    /// Copyright music resource song copyright provider.
    ZegoCopyrightedMusicVendorID vendorID;

    ZegoCopyrightedMusicQueryCacheConfig() {
        resourceType = ZEGO_COPYRIGHTED_MUSIC_RESOURCE_SONG;
        resourceQualityType = ZEGO_COPYRIGHTED_MUSIC_RESOURCE_QUALITY_NORMAL;
        vendorID = ZEGO_COPYRIGHTED_MUSIC_VENDOR_DEFAULT;
    }
};

/// The configuration of querying cache.
struct ZegoCopyrightedMusicQueryCacheConfigV2 {
    /// the ID of the song.
    std::string songID;

    /// The resource type of music. Refer to the value of [ZegoCopyrightedMusicResourceType].
    int resourceType;

    /// The resource quality type of music. Refer to the value of [ZegoCopyrightedMusicResourceQualityType].
    int resourceQualityType;

    /// Copyright music resource song copyright provider. Refer to the value of [ZegoCopyrightedMusicVendorID].
    int vendorID;

    ZegoCopyrightedMusicQueryCacheConfigV2() {
        resourceType = 0;
        resourceQualityType = 0;
        vendorID = 0;
    }
};

/// Orientation.
struct ZegoPositionOrientation {
    /// The unit vector of the front axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
    float axisForward[3];

    /// The unit vector of the right axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
    float axisRight[3];

    /// The unit vector of the up axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
    float axisUp[3];
};

/// Request configuration of song or accompaniment.
struct ZegoPosition {
    /// The coordinates of the oneself in the world coordinate system. The parameter is a float array of length 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
    float coordinate[3];

    /// Motion orientation.
    ZegoPositionOrientation motionOrientation;

    /// Camera orientation, for 3D range spatializer.
    ZegoPositionOrientation cameraOrientation;
};

/// Scene param.
struct ZegoSceneParam {
    /// User object instance, configure userID, userName. Note that the userID needs to be globally unique with the same appID, otherwise the user who logs in later will kick out the user who logged in first.
    ZegoUser user;

    /// Scene ID.
    long long sceneID;

    /// Scene template ID.
    unsigned int templateID;

    /// User position.
    ZegoPosition position;

    /// Broadcast user's own information mode, the default is ALL.
    ZegoBroadcastMode broadcastMode;

    /// Authentication token.
    std::string token;

    ZegoSceneParam() {
        sceneID = 0;
        templateID = 0;
        broadcastMode = ZEGO_BROADCAST_MODE_ALL;
        token = "";
    }
};

/// Scene config.
struct ZegoSceneConfig {
    /// State channel count.
    unsigned int channelCount;

    ZegoSceneConfig() { channelCount = 0; }
};

/// Team param.
struct ZegoTeamParam {
    /// Team ID.
    unsigned int teamID;

    ZegoTeamParam() { teamID = 0; }
};

/// Item param.
struct ZegoItemParam {
    /// Item ID.
    long long itemID;

    /// The number of users allowed to bind the item.
    unsigned int capacity;

    /// The initial position of the item in the range scene.
    ZegoPosition position;

    /// The mode of create item.
    ZegoCreateItemMode createMode;

    ZegoItemParam() {
        itemID = 0;
        capacity = 1;
        createMode = ZEGO_CREATE_ITEM_MODE_NO_BIND;
    }
};

/// Scene stream config.
struct ZegoSceneStreamConfig {
    /// Enable play stream in range or not.
    bool enablePlayInRange;

    /// Enable publish stream to world or not.
    bool enablePublishToWorld;

    ZegoSceneStreamConfig() {
        enablePlayInRange = true;
        enablePublishToWorld = true;
    }
};

/// image data content
struct ZegoImageBuffer {
    /// image content in BGRA32 format.
    unsigned char *buffer;

    /// buffer size.
    unsigned int length;

    /// buffer width.
    unsigned int width;

    /// buffer height.
    unsigned int height;
};

/// The screen captures source information.
struct ZegoScreenCaptureSourceInfo {
    /// Target type for screen capture. (only for desktop)
    ZegoScreenCaptureSourceType sourceType;

    /// The ID of the capture source.
    void *sourceID;

    /// Capture source name (in UTF8 encoding).
    std::string sourceName;

    /// Thumbnail of the capture window.
    ZegoImageBuffer thumbnailImage;

    /// The image content of the icon.
    ZegoImageBuffer iconImage;
};

/// Layer border configuration.
///
/// Customize the size, color, etc. of the layer border.
struct ZegoLayerBorderConfig {
    /// Border size, default value 4, the maximum value is 100.
    unsigned int width;

    /// Background color, the format is 0xRRGGBB, default is green, which is 0x00FF00
    int color;

    ZegoLayerBorderConfig() {
        width = 4;
        color = 0x00FF00;
    }
};

/// Audio source mix config
///
/// Used to config whether mix media player, audio effect player and captured system audio into publish stream or not when set audio source.
struct ZegoAudioSourceMixConfig {
    /// Media player instance index list.
    int *mediaPlayerIndexList;

    /// Media player instance count.
    int mediaPlayerCount;

    /// Audio effect player instance index list.
    int *audioEffectPlayerIndexList;

    /// Audio effect player instance count.
    int audioEffectPlayerCount;

    /// Enable or disable mix captured system audio into publish stream.
    bool enableMixSystemPlayout;

    /// Enable or disable mix SDK playout into publish stream.
    bool enableMixEnginePlayout;

    ZegoAudioSourceMixConfig() {
        mediaPlayerIndexList = nullptr;
        mediaPlayerCount = 0;
        audioEffectPlayerIndexList = nullptr;
        audioEffectPlayerCount = 0;
        enableMixSystemPlayout = false;
        enableMixEnginePlayout = false;
    }
};

/// Multimedia resource for ZEGO media player.
///
/// Used to configure loading parameters when loading multimedia resources.
struct ZegoMediaPlayerResource {
    /// Used to specify the loading type of multimedia resources.
    ZegoMultimediaLoadType loadType;

    /// The progress at which the plaback started.
    unsigned long long startPosition;

    /// If the specified resource has a transparent effect, you need to specify the layout position of the alpha data.
    ZegoAlphaLayoutType alphaLayout;

    /// Common resource path.The absolute resource path or the URL of the network resource and cannot be null or "". Android can set this path string with Uri.
    std::string filePath;

    /// binary data memory.
    unsigned char *memory;

    /// Binary data memory length.
    int memoryLength;

    /// The resource ID obtained from the copyrighted music module.
    std::string resourceID;

    /// Online resource cache path, in utf8 encoding format.
    std::string onlineResourceCachePath;

    /// The maximum length of online resource cache to be used, in bytes, with a minimum setting of 10M (10 * 1024 * 1024). The default value is 0 - no limit, and try to cache the entire file.
    long long maxCachePendingLength;

    ZegoMediaPlayerResource() {
        loadType = ZEGO_MULTIMEDIA_LOAD_TYPE_FILE_PATH;
        startPosition = 0;
        alphaLayout = ZEGO_ALPHA_LAYOUT_TYPE_NONE;
        filePath = "";
        memory = nullptr;
        memoryLength = 0;
        resourceID = "";
        onlineResourceCachePath = "";
        maxCachePendingLength = 0;
    }
};

/// Background config.
///
/// It is used to configure background when the object segmentation is turned on.
struct ZegoBackgroundConfig {
    /// Background process type.
    ZegoBackgroundProcessType processType;

    /// Background color, the format is 0xRRGGBB, default is black, which is 0x000000
    int color;

    /// Background image URL. Support local file absolute path (file://xxx). The format supports png, jpg.
    std::string imageURL;

    /// Background video URL. Caution: 1. The video will be played in a loop. 2. Support local file absolute path (file://xxx). 3. The format supports MP4, FLV, MKV, AVI. 4. The maximum resolution should not exceed 4096px, and it is recommended to be within 1920px. 5. The maximum video duration should not exceed 30 seconds, and it is recommended to be within 15 seconds. 6. The maximum video size should not exceed 50MB, and 10MB is recommended.
    std::string videoURL;

    /// Background blur level.
    ZegoBackgroundBlurLevel blurLevel;

    ZegoBackgroundConfig()
        : processType(ZEGO_BACKGROUND_PROCESS_TYPE_TRANSPARENT), color(0), imageURL(""),
          videoURL(""), blurLevel(ZEGO_BACKGROUND_BLUR_LEVEL_MEDIUM) {}
};

/// Object segmentation config.
///
/// It is used to configure parameters when the object segmentation is turned on.
struct ZegoObjectSegmentationConfig {
    /// The type of object segmentation.
    ZegoObjectSegmentationType objectSegmentationType;

    /// Background config.
    ZegoBackgroundConfig backgroundConfig;

    ZegoObjectSegmentationConfig()
        : objectSegmentationType(ZEGO_OBJECT_SEGMENTATION_TYPE_ANY_BACKGROUND) {}
};

/// Media Infomration of media file.
///
/// Meida information such as video resolution of media file.
struct ZegoMediaPlayerMediaInfo {
    /// Video resolution width.
    int width;

    /// Video resolution height.
    int height;

    /// Video frame rate.
    int frameRate;

    ZegoMediaPlayerMediaInfo() {
        width = 0;
        height = 0;
        frameRate = 0;
    }
};

/// Used to config the media data publisher.
///
/// Used to config the media data publisher when creating it.
struct ZegoMediaDataPublisherConfig {
    /// Used to specify the publish channel index of publisher.
    int channel;

    /// Used to specify the mode of publisher.
    ZegoMediaDataPublisherMode mode;
};

/// Media player play statistics.
///
/// Media player play statistics.
struct ZegoMediaPlayerStatisticsInfo {
    /// Video source fps.
    double videoSourceFps;

    /// Video decode fps.
    double videoDecodeFps;

    /// Video render fps.
    double videoRenderFps;

    /// Audio source fps.
    double audioSourceFps;

    /// Audio decode fps.
    double audioDecodeFps;

    /// Audio render fps.
    double audioRenderFps;

    ZegoMediaPlayerStatisticsInfo() {
        videoSourceFps = 0;
        videoDecodeFps = 0;
        videoRenderFps = 0;
        audioSourceFps = 0;
        audioDecodeFps = 0;
        audioRenderFps = 0;
    }
};

/// Receive range configuration.
struct ZegoReceiveRangeParam {
    /// The minimum distance at which the 3D sound effect starts to have attenuation effect, the value needs to be >= 0 and <= max, the default value is 0.
    float min;

    /// The maximum range received, the value needs to be >= min, the default value is 0.
    float max;

    ZegoReceiveRangeParam() {
        min = 0;
        max = 0;
    }
};

/// Vocal range configuration.
struct ZegoVocalRangeParam {
    /// The minimum distance at which the 3D sound effect starts to have attenuation effect, the value needs to be >= 0 and <= max, the default value is 0.
    float min;

    /// The maximum range of the sound, the value needs to be >= min, the default value is 0.
    float max;

    ZegoVocalRangeParam() {
        min = 0;
        max = 0;
    }
};

/// Dump data config.
struct ZegoDumpDataConfig {
    /// Data type.
    ZegoDumpDataType dataType;
};

/// AI voice changer speaker detail.
struct ZegoAIVoiceChangerSpeakerInfo {
    /// Speaker ID.
    int id;

    /// Speaker name.
    std::string name;

    ZegoAIVoiceChangerSpeakerInfo() { id = 0; }
};

/// Color enhancement params.
struct ZegoColorEnhancementParams {
    /// Description: color enhancement intensity. Value range: [0,1], the larger the value, the stronger the intensity of color enhancement. Default value: 0.
    float intensity;

    /// Description: Skin tone protection level. Value range: [0,1], the larger the value, the greater the level of skin protection. Default value: 1.
    float skinToneProtectionLevel;

    /// Description: Lip color protection level. Value range: [0,1], the larger the value, the greater the level of lip color protection. Default value: 0.
    float lipColorProtectionLevel;

    ZegoColorEnhancementParams() {
        intensity = 0;
        skinToneProtectionLevel = 1;
        lipColorProtectionLevel = 0;
    }
};

/// Dummy capture image params.
struct ZegoDummyCaptureImageParams {
    /// Picture file path.
    std::string path;

    /// Dummy capture image mode.
    ZegoDummyCaptureImageMode mode;

    ZegoDummyCaptureImageParams() {
        path = "";
        mode = ZEGO_DUMMY_CAPTURE_IMAGE_MODE_MANUAL;
    }
};

/// Low light enhancement params.
struct ZegoExpLowlightEnhancementParams {
    /// Description: Low light enhancement mode. Default value: Off.
    ZegoLowlightEnhancementMode mode;

    /// Description: Low light enhancement type. Default value: Normal.
    ZegoExpLowlightEnhancementType type;

    ZegoExpLowlightEnhancementParams() {
        mode = ZEGO_LOWLIGHT_ENHANCEMENT_MODE_OFF;
        type = ZEGO_EXP_LOWLIGHT_ENHANCEMENT_TYPE_NORMAL;
    }
};

/// Callback for asynchronous destruction completion.
///
/// In general, developers do not need to listen to this callback.
using ZegoDestroyCompletionCallback = std::function<void()>;

/// Callback for setting room extra information.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
using ZegoRoomSetRoomExtraInfoCallback = std::function<void(int errorCode)>;

/// Login room result callback.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param extendedData Extended Information
using ZegoRoomLoginCallback = std::function<void(int errorCode, std::string extendedData)>;

/// Logout room result callback.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param extendedData Extended Information
using ZegoRoomLogoutCallback = std::function<void(int errorCode, std::string extendedData)>;

/// Log upload result callback.
///
/// Description: After calling [uploadLog] to upload the log, get the upload result through this callback.
/// Use cases: When uploading logs, in order to determine whether the logs are uploaded successfully, you can get them through this callback.
/// Caution: In the case of poor network, the return time of this callback may be longer.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
using ZegoUploadLogResultCallback = std::function<void(int errorCode)>;

/// Callback for setting stream extra information.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
using ZegoPublisherSetStreamExtraInfoCallback = std::function<void(int errorCode)>;

/// Callback for add/remove CDN URL.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
using ZegoPublisherUpdateCdnUrlCallback = std::function<void(int errorCode)>;

/// Results of take publish stream snapshot.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param image Snapshot image (Windows: HBITMAP; macOS/iOS: CGImageRef; Linux: QImage; Android: Bitmap)
using ZegoPublisherTakeSnapshotCallback = std::function<void(int errorCode, void *image)>;

/// Results of take play stream snapshot.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param image Snapshot image (Windows: HBITMAP; macOS/iOS: CGImageRef; Linux: QImage; Android: Bitmap)
using ZegoPlayerTakeSnapshotCallback = std::function<void(int errorCode, void *image)>;

/// Results of starting a mixer task.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param extendedData Extended Information
using ZegoMixerStartCallback = std::function<void(int errorCode, std::string extendedData)>;

/// Results of stoping a mixer task.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
using ZegoMixerStopCallback = std::function<void(int errorCode)>;

/// Callback for sending real-time sequential data.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
using ZegoRealTimeSequentialDataSentCallback = std::function<void(int errorCode)>;

/// Callback for sending broadcast messages.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param messageID ID of this message
using ZegoIMSendBroadcastMessageCallback =
    std::function<void(int errorCode, unsigned long long messageID)>;

/// Callback for sending barrage message.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param messageID ID of this message
using ZegoIMSendBarrageMessageCallback = std::function<void(int errorCode, std::string messageID)>;

/// Callback for sending custom command.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
using ZegoIMSendCustomCommandCallback = std::function<void(int errorCode)>;

/// Callback for sending custom command.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
using ZegoRoomSendTransparentMessageCallback = std::function<void(int errorCode)>;

/// Callback for test network connectivity.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param result Network connectivity test results
using ZegoTestNetworkConnectivityCallback =
    std::function<void(int errorCode, ZegoTestNetworkConnectivityResult result)>;

/// Callback for network probe.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param result Network probe result
using ZegoNetworkProbeResultCallback =
    std::function<void(int errorCode, const ZegoNetworkProbeResult &result)>;

/// Callback for media player loads resources.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
using ZegoMediaPlayerLoadResourceCallback = std::function<void(int errorCode)>;

/// Callback for media player seek to playback progress.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
using ZegoMediaPlayerSeekToCallback = std::function<void(int errorCode)>;

/// The callback of the screenshot of the media player playing screen
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param image Snapshot image (Windows: HBITMAP; macOS/iOS: CGImageRef; Linux: QImage; Android: Bitmap)
using ZegoMediaPlayerTakeSnapshotCallback = std::function<void(int errorCode, void *image)>;

/// Callback for audio effect player loads resources.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
using ZegoAudioEffectPlayerLoadResourceCallback = std::function<void(int errorCode)>;

/// Callback for audio effect player seek to playback progress.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
using ZegoAudioEffectPlayerSeekToCallback = std::function<void(int errorCode)>;

/// Callback for copyrighted music init.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
using ZegoCopyrightedMusicInitCallback = std::function<void(int errorCode)>;

/// Callback of sending extended feature request.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param command request command.
/// @param result request result, each request command has corresponding request result.
using ZegoCopyrightedMusicSendExtendedRequestCallback =
    std::function<void(int errorCode, std::string command, std::string result)>;

/// Get lrc format lyrics complete callback.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param lyrics lrc format lyrics.
using ZegoCopyrightedMusicGetLrcLyricCallback =
    std::function<void(int errorCode, std::string lyrics)>;

/// Get verbatim lyrics complete callback.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param lyrics verbatim lyrics.
using ZegoCopyrightedMusicGetKrcLyricByTokenCallback =
    std::function<void(int errorCode, std::string lyrics)>;

/// Callback for request song.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource The JSON string returned by the song ordering service, including song resource information.
using ZegoCopyrightedMusicRequestSongCallback =
    std::function<void(int errorCode, std::string resource)>;

/// Callback for request accompaniment.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource accompany resource information.
using ZegoCopyrightedMusicRequestAccompanimentCallback =
    std::function<void(int errorCode, std::string resource)>;

/// Callback for request accompaniment clip.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource accompany clip resource information.
using ZegoCopyrightedMusicRequestAccompanimentClipCallback =
    std::function<void(int errorCode, std::string resource)>;

/// Callback for acquire songs or accompaniment through authorization token.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource song or accompany resource information.
using ZegoCopyrightedMusicGetMusicByTokenCallback =
    std::function<void(int errorCode, std::string resource)>;

/// Callback of requesting music resource.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource The JSON string returned by the song ordering service, including music resource information.
using ZegoCopyrightedMusicRequestResourceCallback =
    std::function<void(int errorCode, std::string resource)>;

/// Callback of getting shared music resource.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param resource The JSON string returned by the song ordering service, including music resource information.
using ZegoCopyrightedMusicGetSharedResourceCallback =
    std::function<void(int errorCode, std::string resource)>;

/// Callback for download song or accompaniment.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
using ZegoCopyrightedMusicDownloadCallback = std::function<void(int errorCode)>;

/// Get standard pitch data complete callback.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param pitch Standard pitch data.
using ZegoCopyrightedMusicGetStandardPitchCallback =
    std::function<void(int errorCode, std::string pitch)>;

/// Login scene result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param config State channel count.
using ZegoRangeSceneLoginSceneCallback =
    std::function<void(int errorCode, const ZegoSceneConfig &config)>;

/// Logout scene result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
using ZegoRangeSceneLogoutSceneCallback = std::function<void(int errorCode)>;

/// The callback of getting user count in scene. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param count User count in scene.
using ZegoRangeSceneGetUserCountCallback = std::function<void(int errorCode, unsigned int count)>;

/// The callback of getting user list in view. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param userList User list in view.
using ZegoRangeSceneGetUserListInViewCallback =
    std::function<void(int errorCode, const std::vector<std::string> &userList)>;

/// The callback of send custom command. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
using ZegoRangeSceneSendCustomCommandCallback = std::function<void(int errorCode)>;

/// Join team result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param teamID Team ID.
using ZegoRangeSceneJoinTeamCallback = std::function<void(int errorCode, unsigned int teamID)>;

/// Leave team result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param teamID Team ID.
using ZegoRangeSceneLeaveTeamCallback = std::function<void(int errorCode, unsigned int teamID)>;

/// Create item result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param itemID Item ID.
using ZegoRangeSceneCreateItemCallback = std::function<void(int errorCode, long long itemID)>;

/// Destroy item result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param itemID Item ID.
using ZegoRangeSceneDestroyItemCallback = std::function<void(int errorCode, long long itemID)>;

/// Bind item result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param itemID Item ID.
using ZegoRangeSceneBindItemCallback = std::function<void(int errorCode, long long itemID)>;

/// Unbind item result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param itemID Item ID.
using ZegoRangeSceneUnbindItemCallback = std::function<void(int errorCode, long long itemID)>;

/// Update item status result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param itemID Item ID.
using ZegoRangeSceneUpdateItemStatusCallback = std::function<void(int errorCode, long long itemID)>;

/// Update item command result callback. Do not call the SDK interface in the callback thread.
///
/// @param errorCode Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param itemID Item ID.
using ZegoRangeSceneUpdateItemCommandCallback =
    std::function<void(int errorCode, long long itemID)>;

using ZegoAudioSpectrum = std::vector<float>;
using ZegoView = void *;

} // namespace EXPRESS
} // namespace ZEGO

#endif /* __ZEGOEXPRESSDEFINES_H__ */
