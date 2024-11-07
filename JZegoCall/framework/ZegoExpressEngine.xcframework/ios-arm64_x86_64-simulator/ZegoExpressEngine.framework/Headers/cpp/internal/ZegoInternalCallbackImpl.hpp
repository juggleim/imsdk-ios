
#pragma once

#include "../ZegoExpressDefines.h"
#include "../ZegoExpressEventHandler.h"
#include "../ZegoExpressInterface.h"

#include "ZegoInternalAIVoiceChanger.hpp"
#include "ZegoInternalAudioEffectPlayer.hpp"
#include "ZegoInternalBase.h"
#include "ZegoInternalBridge.h"
#include "ZegoInternalCopyrightedMusic.hpp"
#include "ZegoInternalMediaDataPublisher.hpp"
#include "ZegoInternalMediaPlayer.hpp"
#include "ZegoInternalRangeAudio.hpp"
#include "ZegoInternalRangeScene.hpp"
#include "ZegoInternalRealTimeSequentialDataManager.hpp"
#include "ZegoInternalScreenCaptureSource.hpp"

ZEGO_DISABLE_DEPRECATION_WARNINGS

namespace ZEGO {
namespace EXPRESS {

#define oInternalCallbackCenter ZegoSingleton<ZegoInternalCallbackCenter>::CreateInstance()
class ZegoInternalCallbackCenter {
  public:
    declearSingleShareMember(IZegoEventHandler);
    declearSingleShareMember(IZegoApiCalledEventHandler);
    declearMultiRawMember(zego_seq, ZegoRoomSetRoomExtraInfoCallback);
    declearMultiRawMember(zego_seq, ZegoRoomLoginCallback);
    declearMultiRawMember(zego_seq, ZegoRoomLogoutCallback);
    declearMultiRawMember(zego_seq, ZegoPublisherSetStreamExtraInfoCallback);
    declearMultiRawMember(zego_seq, ZegoPublisherUpdateCdnUrlCallback);
    declearMultiRawMember(zego_seq, ZegoPublisherTakeSnapshotCallback);
    declearMultiRawMember(std::string, ZegoPlayerTakeSnapshotCallback);
    declearMultiRawMember(zego_seq, ZegoRealTimeSequentialDataSentCallback);
    declearMultiRawMember(zego_seq, ZegoIMSendBroadcastMessageCallback);
    declearMultiRawMember(zego_seq, ZegoIMSendBarrageMessageCallback);
    declearMultiRawMember(zego_seq, ZegoIMSendCustomCommandCallback);
    declearMultiRawMember(zego_seq, ZegoRoomSendTransparentMessageCallback);
    declearMultiRawMember(zego_seq, ZegoMixerStartCallback);
    declearMultiRawMember(zego_seq, ZegoMixerStopCallback);
    declearMultiShareMember(ZegoExpressMediaPlayerImp);
    declearMultiShareMember(ZegoExpressAudioEffectPlayerImp);
    declearMultiShareMember(ZegoExpressRangeAudioImp);
    declearMultiShareMember(ZegoExpressRangeSceneImp);
    declearMultiShareMember(ZegoExpressRealTimeSequentialDataManagerImp);
    declearMultiShareMember(ZegoExpressScreenCaptureSourceImp);
    declearMultiShareMember(ZegoExpressMediaDataPublisherImp);
    declearSingleShareMember(IZegoCustomVideoRenderHandler);
    declearSingleShareMember(IZegoCustomVideoProcessHandler);
    declearSingleShareMember(IZegoCustomVideoCaptureHandler);
    declearSingleShareMember(IZegoAudioDataHandler);
    declearSingleShareMember(IZegoAudioMixingHandler);
    declearSingleShareMember(IZegoDataRecordEventHandler);
    declearSingleShareMember(IZegoCustomAudioProcessHandler);
    declearSingleRawMember(ZegoDestroyCompletionCallback);
    declearMultiRawMember(zego_seq, ZegoTestNetworkConnectivityCallback);
    declearMultiRawMember(zego_seq, ZegoNetworkProbeResultCallback);
    declearMultiRawMember(zego_seq, ZegoUploadLogResultCallback);
    declearSingleShareMember(ZegoExpressCopyrightedMusicImp);
    declearMultiShareMember(ZegoExpressAIVoiceChangerImpl);

    void clearHandlerData() {
        mIZegoEventHandler = nullptr;
        mIZegoApiCalledEventHandler = nullptr;
        mZegoRoomSetRoomExtraInfoCallback.clear();
        mZegoRoomLoginCallback.clear();
        mZegoRoomLogoutCallback.clear();
        mZegoPublisherSetStreamExtraInfoCallback.clear();
        mZegoPublisherUpdateCdnUrlCallback.clear();
        mZegoPublisherTakeSnapshotCallback.clear();
        mZegoPlayerTakeSnapshotCallback.clear();
        mZegoIMSendBroadcastMessageCallback.clear();
        mZegoIMSendBarrageMessageCallback.clear();
        mZegoIMSendCustomCommandCallback.clear();
        mZegoRoomSendTransparentMessageCallback.clear();
        mZegoMixerStartCallback.clear();
        mZegoMixerStopCallback.clear();
        mZegoExpressMediaPlayerImp.clear();
        mZegoExpressAudioEffectPlayerImp.clear();
        mZegoExpressRangeSceneImp.clear();
        mIZegoCustomVideoRenderHandler = nullptr;
        mIZegoCustomVideoProcessHandler = nullptr;
        mIZegoCustomVideoCaptureHandler = nullptr;
        mIZegoAudioDataHandler = nullptr;
        mIZegoAudioMixingHandler = nullptr;
        mIZegoDataRecordEventHandler = nullptr;
        mIZegoCustomAudioProcessHandler = nullptr;
        mZegoTestNetworkConnectivityCallback.clear();
        mZegoNetworkProbeResultCallback.clear();
        mZegoUploadLogResultCallback.clear();

        mZegoExpressCopyrightedMusicImp = nullptr;
        mZegoExpressMediaDataPublisherImp.clear();
        mZegoExpressScreenCaptureSourceImp.clear();
        mZegoExpressRangeAudioImp.clear();
        mZegoExpressRealTimeSequentialDataManagerImp.clear();
        mZegoExpressAIVoiceChangerImpl.clear();
    }

    void clearContainerData() {
        std::lock_guard<std::recursive_mutex> locker(
            oCopyrightedMusicContainer->copyrightedMusicMutex);

        oCopyrightedMusicContainer->copyrightedMusicInstance = nullptr;
    }

    void registerApiCalledResultCallback() {
        oInternalOriginBridge->registerOnApiCalledResultCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_api_called_result), ZegoVoidPtr(this));
    }
    void unRegisterApiCalledResultCallback() {
        oInternalOriginBridge->registerOnApiCalledResultCallback(nullptr, nullptr);
    }

    void registerCallback() {
#if defined(_WIN32)
        oInternalOriginBridge->registerRecvWindowsMessageCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_recv_win_massage), nullptr);
#endif
        oInternalOriginBridge->registerEngineUninitCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_engine_uninit), nullptr);

        oInternalOriginBridge->registerFatalErrorCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_fatal_error), nullptr);

        oInternalOriginBridge->registerOnCallErrorCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_debug_error), ZegoVoidPtr(this));
        oInternalOriginBridge->registerEngineStateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_engine_state_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerUploadLogResultCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_upload_log_result), ZegoVoidPtr(this));

        oInternalOriginBridge->registerRoomStreamUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_room_stream_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRoomStreamExtraInfoUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_room_stream_extra_info_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRoomStateUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_room_state_update), ZegoVoidPtr(this));
        oInternalOriginBridge->registerRoomUserStateUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_user_update), ZegoVoidPtr(this));
        oInternalOriginBridge->registerRoomOnlineUserCountUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_room_online_user_count_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRoomExtraInfoUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_room_extra_info_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRoomSetRoomExtraInfoResultCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_room_set_room_extra_info_result),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRoomTokenWillExpireCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_room_token_will_expire),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRoomLoginCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_room_login_callback),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRoomLogoutCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_room_logout_callback),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRoomStateChangedCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_room_state_changed),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerPublisherStateUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_state_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPublisherQualityUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_quality_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPublisherRecvAudioCapturedFirstFrameCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_captured_audio_first_frame),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPublisherStreamEventCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_stream_event),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPlayerStreamEventCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_stream_event),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerPublisherRecvVideoCapturedFirstFrameCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_captured_video_first_frame),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPublisherRecvAudioSendFirstFrameCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_send_audio_first_frame),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPublisherRecvVideoSendFirstFrameCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_send_video_first_frame),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPublisherRenderVideoFirstFrameCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_render_video_first_frame),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPublisherVideoSizeChangedCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_video_size_changed),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPublisherUpdatePublishCdnUrlCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_update_cdn_url_result),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPublisherRelayCDNStateUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_relay_cdn_state_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPublisherVideoEncoderChangedCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_video_encoder_changed),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPublisherUpdateStreamExtraInfoResultCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_publisher_update_stream_extra_info_result),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPublisherTakeSnapshotResultCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_take_snapshot_result),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPublisherLowFpsWarningCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_low_fps_warning),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPublisherDummyCaptureImagePathErrorCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_publisher_dummy_capture_image_path_error),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerPlayerStateUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_state_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPlayerSwitchedCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_switched), ZegoVoidPtr(this));
        oInternalOriginBridge->registerPlayerQualityUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_quality_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPlayerMediaEventCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_media_event),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPlayerRecvAudioFirstFrameCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_recv_audio_first_frame),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPlayerRecvVideoFirstFrameCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_recv_video_first_frame),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPlayerRenderVideoFirstFrameCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_render_video_first_frame),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPlayerRenderCameraVideoFirstFrameCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_player_render_camera_video_first_frame),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPlayerVideoSizeChangedCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_video_size_changed),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPlayerRecvSEICallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_recv_sei), ZegoVoidPtr(this));
        oInternalOriginBridge->registerPlayerRecvAudioSideInfoCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_recv_audio_side_info),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerPlayerTakeSnapshotResultCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_take_snapshot_result),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerPlayerLowFpsWarningCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_low_fps_warning),
            ZegoVoidPtr(this));
#if TARGET_OS_IPHONE || defined(ANDROID)
        oInternalOriginBridge->registerPlayerVideoSuperResolutionUpdate(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_video_super_resolution_update),
            ZegoVoidPtr(this));
#endif

        oInternalOriginBridge->registerLocalDeviceExceptionOccurredCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_local_device_exception_occurred),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerAudioDeviceStateChangedCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_audio_device_state_changed),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerAudioDeviceVolumeChangedCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_audio_device_volume_changed),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerVideoDeviceStateChangedCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_video_device_state_changed),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRemoteCameraStateUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_remote_camera_state_update),
            ZegoVoidPtr(this));

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
        oInternalOriginBridge->registerAudioRouteChangeCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_audio_route_change),
            ZegoVoidPtr(this));
#endif
        oInternalOriginBridge->registerRemoteMicStateUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_remote_mic_state_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRemoteSpeakerStateUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_remote_speaker_state_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCapturedSoundLevelUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_captured_sound_level_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRemoteSoundLevelUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_remote_sound_level_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCapturedSoundLevelInfoUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_captured_sound_level_info_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRemoteSoundLevelInfoUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_remote_sound_level_info_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCapturedAudioSpectrumUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_captured_audio_spectrum_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRemoteAudioSpectrumUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_remote_audio_spectrum_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerAudioVADStateUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_audio_vad_state_update),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerRealTimeSequentialDataSentCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_real_time_sequential_data_sent),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerReceiveRealTimeSequentialDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_receive_real_time_sequential_data),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerIMSendBroadcastMessageResultCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_im_send_broadcast_message_result),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerIMRecvBroadcasMessageCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_im_recv_broadcast_message),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerIMSendBarrageMessageResultCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_im_send_barrage_message_result),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerIMRecvBarrageMessageResultCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_im_recv_barrage_message),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerIMSendCustomCommandResultCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_im_send_custom_command_result),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerIMRecvCustomCommandCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_im_recv_custom_command),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerIMSendTransparentMessageResultCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_im_send_transparent_message_result),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerIMRecvRoomTransparentMessageCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_im_recv_transparent_message),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerMixerStartResultCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_mixer_start_result),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMixerStopResultCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_mixer_stop_result), ZegoVoidPtr(this));
        oInternalOriginBridge->registerMixerSoundLevelUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_mixer_sound_level_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerAutoMixerSoundLevelUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_auto_mixer_sound_level_update),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerMixerRelayCDNStateUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_mixer_relay_cdn_state_update),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerMediaPlayerStateUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_state_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaPlayerNetworkEventCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_network_event),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaPlayerPlayingProgressCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_playing_progress),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaPlayerRenderingProgressCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_rendering_progress),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaPlayerVideoSizeChangedCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_video_size_changed),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaPlayerRecvSEICallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_recv_sei),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaPlayerSoundLevelUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_sound_level_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaPlayerFrequencySpectrumUpdateCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_media_player_frequency_spectrum_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaPlayerSeekToResult(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_seek_to),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaPlayerLoadFileResult(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_mediaplayer_load_file_result),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaPlayerAudioDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_audio_frame),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaPlayerFirstFrameEventCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_first_frame_event),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaPlayerLocalCacheCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_local_cache),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaPlayerVideoDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_video_frame),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaPlayerTakeSnapshotResultCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_take_snapshot_result),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaPlayerBlockBeginCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_block_begin),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaPlayerBlockDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_block_data),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerAudioEffectPlayStateUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_audio_effect_play_state_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerAudioEffectPlayerLoadResourceCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_audio_effect_player_load_resource),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerAudioEffectPlayerSeekToCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_audio_effect_player_seek_to),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerRangeAudioMicrophoneStateUpdate(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_audio_microphone_state_update),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerCustomVideoRenderLocalFrameDataCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_custom_video_render_captured_frame_data),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCustomVideoRenderRemoteFrameDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_custom_video_render_remote_frame_data),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCustomVideoRenderRemoteFrameEncodedDataCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_custom_video_render_remote_frame_encoded_data),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerCustomVideoCaptureStartCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_custom_video_capture_start),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCustomVideoCaptureStopCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_custom_video_capture_stop),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCustomVideoCaptureEncodedDataTrafficControlCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::
                            zego_on_custom_video_capture_encoded_data_traffic_control),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerCustomVideoProcessCapturedRawDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::
                            zego_on_custom_video_process_captured_unprocessed_raw_data),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCustomVideoProcessCapturedCVPixelBufferCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::
                            zego_on_custom_video_process_captured_unprocessed_cvpixelbuffer),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerAudioMixingCopyDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_audio_mixing_copy_data),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerOnCapturedAudioDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_captured_audio_data),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerOnPlaybackAudioDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_playback_audio_data),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerOnMixedAudioDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_mixed_audio_data), ZegoVoidPtr(this));
        oInternalOriginBridge->registerOnPlayerAudioDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_audio_data), ZegoVoidPtr(this));

        oInternalOriginBridge->registerRecordingCapturedDataStateUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_captured_data_record_state_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRecordingCapturedDataProgressUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_captured_data_record_progress_update),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerProcessCapturedAudioDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_process_captured_audio_data),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerProcessCapturedAudioDataAfterUsedHeadphoneMonitorCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::
                            zego_on_process_captured_audio_data_after_used_headphone_monitor),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerBeforeAudioPrepAudioDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_before_audio_prep_audio_data),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerProcessRemoteAudioDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_process_remote_audio_data),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerProcessPlaybackAudioDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_process_playback_audio_data),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerPerformanceStatueUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_performance_status_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerNetworkModeChangedCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_network_mode_changed),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerTestNetworkConnectivityCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_test_network_connectivity),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerNetworkSpeedTestQualityUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_network_speed_test_quality_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerNetworkSpeedTestErrorCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_network_speed_test_error),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerNetworkProbeResultCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_network_probe_result),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRequestDumpDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_request_dump_data), ZegoVoidPtr(this));
        oInternalOriginBridge->registerRequestUploadDumpDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_request_upload_dump_data),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerStartDumpDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_start_dump_data), ZegoVoidPtr(this));
        oInternalOriginBridge->registerStopDumpDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_stop_dump_data), ZegoVoidPtr(this));
        oInternalOriginBridge->registerUploadDumpDataCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_upload_dump_data), ZegoVoidPtr(this));
        oInternalOriginBridge->registerRecvExperimentalAPICallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_recv_experimental_api),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerNetworkQualityCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_network_quality), ZegoVoidPtr(this));

        oInternalOriginBridge->registerCopyrightedMusicDownloadProcessUpdateCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_copyrighted_music_download_progress_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCopyrightedMusicCurrentPitchValueUpdateCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_copyrighted_music_current_pitch_value_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCopyrightedMusicInitCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_copyrighted_music_init),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCopyrightedMusicSendExtendedRequestCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_copyrighted_music_send_extended_request),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCopyrightedMusicGetLrcLyricCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_copyrighted_music_get_lrc_lyric),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCopyrightedMusicGetKrcLyricByTokenCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_copyrighted_music_get_krc_lyric_by_token),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCopyrightedMusicRequestSongCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_copyrighted_music_request_song),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCopyrightedMusicRequestAccompanimentCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_copyrighted_music_request_accompaniment),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCopyrightedMusicRequestAccompanimentClipCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_copyrighted_music_request_accompaniment_clip),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCopyrightedMusicGetMusicByTokenCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_copyrighted_music_get_music_by_token),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCopyrightedMusicRequestResourceCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_copyrighted_music_request_resource),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCopyrightedMusicGetSharedResourceCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_copyrighted_music_get_shared_resource),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCopyrightedMusicGetStandardPitchCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_copyrighted_music_get_standard_pitch),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerCopyrightedMusicDownloadCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_copyrighted_music_download),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerScreenCaptureSourceAvailableFrameCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_screen_capture_source_available_frame),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerScreenCaptureSourceExceptionOccurredCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_screen_capture_source_exception_occurred),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerScreenCaptureSourceWindowStateCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_screen_capture_source_window_state_changed),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerScreenCaptureSourceCaptureRectCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_screen_capture_source_rect_changed),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerNetworkTimeSynchronizedCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_network_time_synchronized),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaDataPublisherFileCloseCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_data_publisher_file_close),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaDataPublisherFileOpenCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_data_publisher_file_open),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaDataPublisherFileDataBeginCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_data_publisher_file_data_begin),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerMediaDataPublisherFileDataEndCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_data_publisher_file_data_end),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerRangeSceneSceneStateUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_scene_state_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneEnterViewCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_enter_view),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneLeaveViewCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_leave_view),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneUserStatusUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_user_status_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneUserCommandUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_user_command_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneCustomCommandUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_custom_command_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneTokenWillExpireCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_token_will_expire),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneLoginSceneCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_login_scene),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneLogoutSceneCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_logout_scene),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneGetUserCountCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_get_user_count),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneGetUserListInViewCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_get_user_list_in_view),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneSendCustomCommandCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_send_custom_command),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneStreamUserStreamStateUpdateCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_range_scene_stream_user_stream_state_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneStreamUserMicUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_stream_user_mic_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneStreamUserCameraUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_stream_user_camera_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneStreamUserSpeakerUpdateCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_range_scene_stream_user_speaker_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneTeamTeamStateUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_team_team_state_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneTeamTeamMemberUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_team_team_member_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneTeamJoinTeamCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_team_join_team),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneTeamLeaveTeamCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_team_leave_team),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneItemItemEnterViewCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_item_item_enter_view),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneItemItemLeaveViewCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_item_item_leave_view),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneItemItemBindUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_item_item_bind_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneItemItemUnbindUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_item_item_unbind_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneItemItemStatusUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_item_item_status_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneItemItemCommandUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_item_item_command_update),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneItemCreateItemCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_item_create_item),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneItemDestroyItemCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_item_destroy_item),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneItemBindItemCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_item_bind_item),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneItemUnbindItemCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_item_unbind_item),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneItemUpdateItemStatusCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_item_update_item_status),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerRangeSceneItemUpdateItemCommandCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_range_scene_item_update_item_command),
            ZegoVoidPtr(this));
        oInternalOriginBridge->registerVideoObjectSegmentationStateChangedCallback(
            ZegoVoidPtr(
                &ZegoInternalCallbackCenter::zego_on_video_object_segmentation_state_changed),
            ZegoVoidPtr(this));

        oInternalOriginBridge->registerAIVoiceChangerInitCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_ai_voice_changer_init), nullptr);
        oInternalOriginBridge->registerAIVoiceChangerUpdateProgressCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_ai_voice_changer_update_progress),
            nullptr);
        oInternalOriginBridge->registerAIVoiceChangerUpdateCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_ai_voice_changer_update), nullptr);
        oInternalOriginBridge->registerAIVoiceChangerGetSpeakerListCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_ai_voice_changer_get_speaker_list),
            nullptr);
        oInternalOriginBridge->registerAIVoiceChangerEventCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_ai_voice_changer_event), nullptr);
        oInternalOriginBridge->registerAIVoiceChangerSetSpeakerCallback(
            ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_ai_voice_changer_set_speaker),
            nullptr);
    }

    void unregisterCallback() {
        oInternalOriginBridge->registerOnCallErrorCallback(nullptr, nullptr);
        oInternalOriginBridge->registerEngineStateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerUploadLogResultCallback(nullptr, nullptr);
        oInternalOriginBridge->registerFatalErrorCallback(nullptr, nullptr);

        oInternalOriginBridge->registerRoomStreamUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRoomStreamExtraInfoUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRoomStateUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRoomUserStateUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRoomOnlineUserCountUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRoomExtraInfoUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRoomSetRoomExtraInfoResultCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRoomLoginCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRoomLogoutCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRoomStateChangedCallback(nullptr, nullptr);

        oInternalOriginBridge->registerIMSendTransparentMessageResultCallback(nullptr, nullptr);
        oInternalOriginBridge->registerIMRecvRoomTransparentMessageCallback(nullptr, nullptr);

        oInternalOriginBridge->registerPublisherStateUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPublisherQualityUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPublisherRecvAudioCapturedFirstFrameCallback(nullptr,
                                                                                    nullptr);
        oInternalOriginBridge->registerPublisherRecvVideoCapturedFirstFrameCallback(nullptr,
                                                                                    nullptr);
        oInternalOriginBridge->registerPublisherRecvAudioSendFirstFrameCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPublisherRecvVideoSendFirstFrameCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPublisherRenderVideoFirstFrameCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPublisherVideoSizeChangedCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPublisherUpdatePublishCdnUrlCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPublisherRelayCDNStateUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPublisherVideoEncoderChangedCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPublisherUpdateStreamExtraInfoResultCallback(nullptr,
                                                                                    nullptr);
        oInternalOriginBridge->registerPublisherTakeSnapshotResultCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPublisherLowFpsWarningCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPublisherDummyCaptureImagePathErrorCallback(nullptr,
                                                                                   nullptr);

        oInternalOriginBridge->registerPlayerStateUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPlayerSwitchedCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPlayerQualityUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPlayerMediaEventCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPlayerRecvAudioFirstFrameCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPlayerRecvVideoFirstFrameCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPlayerRenderVideoFirstFrameCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPlayerRenderCameraVideoFirstFrameCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPlayerVideoSizeChangedCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPlayerRecvSEICallback(nullptr, nullptr);
        oInternalOriginBridge->registerPlayerTakeSnapshotResultCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPlayerLowFpsWarningCallback(nullptr, nullptr);
        oInternalOriginBridge->registerPlayerVideoSuperResolutionUpdate(nullptr, nullptr);

        oInternalOriginBridge->registerAudioRouteChangeCallback(nullptr, nullptr);
        oInternalOriginBridge->registerAudioDeviceStateChangedCallback(nullptr, nullptr);
        oInternalOriginBridge->registerAudioDeviceVolumeChangedCallback(nullptr, nullptr);
        oInternalOriginBridge->registerVideoDeviceStateChangedCallback(nullptr, nullptr);
        oInternalOriginBridge->registerLocalDeviceExceptionOccurredCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRemoteCameraStateUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRemoteMicStateUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRemoteSpeakerStateUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerCapturedSoundLevelUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRemoteSoundLevelUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerCapturedAudioSpectrumUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRemoteAudioSpectrumUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerAudioVADStateUpdateCallback(nullptr, nullptr);

        oInternalOriginBridge->registerRealTimeSequentialDataSentCallback(nullptr, nullptr);
        oInternalOriginBridge->registerReceiveRealTimeSequentialDataCallback(nullptr, nullptr);

        oInternalOriginBridge->registerIMSendBroadcastMessageResultCallback(nullptr, nullptr);
        oInternalOriginBridge->registerIMRecvBroadcasMessageCallback(nullptr, nullptr);
        oInternalOriginBridge->registerIMSendBarrageMessageResultCallback(nullptr, nullptr);
        oInternalOriginBridge->registerIMRecvBarrageMessageResultCallback(nullptr, nullptr);
        oInternalOriginBridge->registerIMSendCustomCommandResultCallback(nullptr, nullptr);
        oInternalOriginBridge->registerIMRecvCustomCommandCallback(nullptr, nullptr);

        oInternalOriginBridge->registerMixerStartResultCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMixerStopResultCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMixerSoundLevelUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerAutoMixerSoundLevelUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMixerRelayCDNStateUpdateCallback(nullptr, nullptr);

        oInternalOriginBridge->registerMediaPlayerStateUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMediaPlayerNetworkEventCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMediaPlayerPlayingProgressCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMediaPlayerRecvSEICallback(nullptr, nullptr);
        oInternalOriginBridge->registerMediaPlayerSoundLevelUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMediaPlayerFrequencySpectrumUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMediaPlayerSeekToResult(nullptr, nullptr);
        oInternalOriginBridge->registerMediaPlayerLoadFileResult(nullptr, nullptr);
        oInternalOriginBridge->registerMediaPlayerAudioDataCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMediaPlayerFirstFrameEventCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMediaPlayerLocalCacheCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMediaPlayerVideoDataCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMediaPlayerTakeSnapshotResultCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMediaPlayerBlockBeginCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMediaPlayerBlockDataCallback(nullptr, nullptr);

        oInternalOriginBridge->registerAudioEffectPlayStateUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerAudioEffectPlayerLoadResourceCallback(nullptr, nullptr);
        oInternalOriginBridge->registerAudioEffectPlayerSeekToCallback(nullptr, nullptr);

        oInternalOriginBridge->registerRangeAudioMicrophoneStateUpdate(nullptr, nullptr);

        oInternalOriginBridge->registerCustomVideoRenderLocalFrameDataCallback(nullptr, nullptr);
        oInternalOriginBridge->registerCustomVideoRenderRemoteFrameDataCallback(nullptr, nullptr);
        oInternalOriginBridge->registerCustomVideoRenderRemoteFrameEncodedDataCallback(nullptr,
                                                                                       nullptr);

        oInternalOriginBridge->registerCustomVideoProcessCapturedRawDataCallback(nullptr, nullptr);
        oInternalOriginBridge->registerCustomVideoProcessCapturedCVPixelBufferCallback(nullptr,
                                                                                       nullptr);

        oInternalOriginBridge->registerCustomVideoCaptureStartCallback(nullptr, nullptr);
        oInternalOriginBridge->registerCustomVideoCaptureStopCallback(nullptr, nullptr);
        oInternalOriginBridge->registerCustomVideoCaptureEncodedDataTrafficControlCallback(nullptr,
                                                                                           nullptr);

        oInternalOriginBridge->registerAudioMixingCopyDataCallback(nullptr, nullptr);

        oInternalOriginBridge->registerOnCapturedAudioDataCallback(nullptr, nullptr);
        oInternalOriginBridge->registerOnPlaybackAudioDataCallback(nullptr, nullptr);
        oInternalOriginBridge->registerOnMixedAudioDataCallback(nullptr, nullptr);
        oInternalOriginBridge->registerOnPlayerAudioDataCallback(nullptr, nullptr);

        oInternalOriginBridge->registerRecordingCapturedDataStateUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRecordingCapturedDataProgressUpdateCallback(nullptr,
                                                                                   nullptr);

        oInternalOriginBridge->registerProcessCapturedAudioDataCallback(nullptr, nullptr);
        oInternalOriginBridge->registerProcessCapturedAudioDataAfterUsedHeadphoneMonitorCallback(
            nullptr, nullptr);
        oInternalOriginBridge->registerBeforeAudioPrepAudioDataCallback(nullptr, nullptr);
        oInternalOriginBridge->registerProcessRemoteAudioDataCallback(nullptr, nullptr);
        oInternalOriginBridge->registerProcessPlaybackAudioDataCallback(nullptr, nullptr);

        oInternalOriginBridge->registerPerformanceStatueUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerNetworkModeChangedCallback(nullptr, nullptr);
        oInternalOriginBridge->registerTestNetworkConnectivityCallback(nullptr, nullptr);
        oInternalOriginBridge->registerNetworkSpeedTestQualityUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerNetworkSpeedTestErrorCallback(nullptr, nullptr);
        oInternalOriginBridge->registerNetworkProbeResultCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRequestDumpDataCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRequestUploadDumpDataCallback(nullptr, nullptr);
        oInternalOriginBridge->registerStartDumpDataCallback(nullptr, nullptr);
        oInternalOriginBridge->registerStopDumpDataCallback(nullptr, nullptr);
        oInternalOriginBridge->registerUploadDumpDataCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRecvExperimentalAPICallback(nullptr, nullptr);
        oInternalOriginBridge->registerNetworkQualityCallback(nullptr, nullptr);

        oInternalOriginBridge->registerCopyrightedMusicDownloadProcessUpdateCallback(nullptr,
                                                                                     nullptr);
        oInternalOriginBridge->registerCopyrightedMusicCurrentPitchValueUpdateCallback(nullptr,
                                                                                       nullptr);
        oInternalOriginBridge->registerCopyrightedMusicInitCallback(nullptr, nullptr);
        oInternalOriginBridge->registerCopyrightedMusicSendExtendedRequestCallback(nullptr,
                                                                                   nullptr);
        oInternalOriginBridge->registerCopyrightedMusicGetLrcLyricCallback(nullptr, nullptr);
        oInternalOriginBridge->registerCopyrightedMusicGetKrcLyricByTokenCallback(nullptr, nullptr);
        oInternalOriginBridge->registerCopyrightedMusicRequestSongCallback(nullptr, nullptr);
        oInternalOriginBridge->registerCopyrightedMusicRequestAccompanimentCallback(nullptr,
                                                                                    nullptr);
        oInternalOriginBridge->registerCopyrightedMusicRequestAccompanimentClipCallback(nullptr,
                                                                                        nullptr);
        oInternalOriginBridge->registerCopyrightedMusicGetMusicByTokenCallback(nullptr, nullptr);
        oInternalOriginBridge->registerCopyrightedMusicRequestResourceCallback(nullptr, nullptr);
        oInternalOriginBridge->registerCopyrightedMusicGetSharedResourceCallback(nullptr, nullptr);
        oInternalOriginBridge->registerCopyrightedMusicDownloadCallback(nullptr, nullptr);
        oInternalOriginBridge->registerCopyrightedMusicGetStandardPitchCallback(nullptr, nullptr);

        oInternalOriginBridge->registerScreenCaptureSourceAvailableFrameCallback(nullptr, nullptr);
        oInternalOriginBridge->registerScreenCaptureSourceExceptionOccurredCallback(nullptr,
                                                                                    nullptr);
        oInternalOriginBridge->registerScreenCaptureSourceWindowStateCallback(nullptr, nullptr);

        oInternalOriginBridge->registerNetworkTimeSynchronizedCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMediaDataPublisherFileCloseCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMediaDataPublisherFileOpenCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMediaDataPublisherFileDataBeginCallback(nullptr, nullptr);
        oInternalOriginBridge->registerMediaDataPublisherFileDataEndCallback(nullptr, nullptr);

        oInternalOriginBridge->registerRangeSceneSceneStateUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneEnterViewCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneLeaveViewCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneUserStatusUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneUserCommandUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneCustomCommandUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneTokenWillExpireCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneLoginSceneCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneLogoutSceneCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneGetUserCountCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneGetUserListInViewCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneSendCustomCommandCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneStreamUserStreamStateUpdateCallback(nullptr,
                                                                                     nullptr);
        oInternalOriginBridge->registerRangeSceneStreamUserMicUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneStreamUserCameraUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneStreamUserSpeakerUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneTeamTeamStateUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneTeamTeamMemberUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneTeamJoinTeamCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneTeamLeaveTeamCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneItemItemEnterViewCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneItemItemLeaveViewCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneItemItemBindUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneItemItemUnbindUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneItemItemStatusUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneItemItemCommandUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneItemCreateItemCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneItemDestroyItemCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneItemBindItemCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneItemUnbindItemCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneItemUpdateItemStatusCallback(nullptr, nullptr);
        oInternalOriginBridge->registerRangeSceneItemUpdateItemCommandCallback(nullptr, nullptr);
        oInternalOriginBridge->registerVideoObjectSegmentationStateChangedCallback(nullptr,
                                                                                   nullptr);

        oInternalOriginBridge->registerAIVoiceChangerInitCallback(nullptr, nullptr);
        oInternalOriginBridge->registerAIVoiceChangerUpdateProgressCallback(nullptr, nullptr);
        oInternalOriginBridge->registerAIVoiceChangerUpdateCallback(nullptr, nullptr);
        oInternalOriginBridge->registerAIVoiceChangerGetSpeakerListCallback(nullptr, nullptr);
        oInternalOriginBridge->registerAIVoiceChangerEventCallback(nullptr, nullptr);
    }

    static void zego_on_engine_uninit(void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto afterDestroy = oInternalCallbackCenter->getZegoDestroyCompletionCallback();
        if (afterDestroy) {
            afterDestroy();
            oInternalCallbackCenter->setZegoDestroyCompletionCallback(nullptr);
        }
    }

    static void zego_on_fatal_error(int error_code, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onFatalError(error_code);
        ZEGO_SWITCH_THREAD_ING
    }

#if defined(_WIN32)
    static void zego_on_recv_win_massage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        ZEGO_UNUSED_VARIABLE(hWnd);
        ZEGO_UNUSED_VARIABLE(lParam);
        if (uMsg == (WM_APP + 10086)) {
            std::function<void(void)> *pFunc = (std::function<void(void)> *)wParam;
            (*pFunc)();
            delete pFunc;
        }
    }
#endif

    // DEBUG CALLBACK
    static void zego_on_debug_error(int error_code, const char *func_name, const char *_info,
                                    void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string funcName = func_name;
        std::string info = _info;
        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onDebugError(error_code, funcName, info);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_api_called_result(int error_code, const char *func_name, const char *_info,
                                          void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoApiCalledEventHandler();
        std::string funcName = func_name;
        std::string info = _info;
        auto weakHandler = std::weak_ptr<IZegoApiCalledEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onApiCalledResult(error_code, funcName, info);
        ZEGO_SWITCH_THREAD_ING
    }

    // ENEING CALLBACK
    static void zego_on_engine_state_update(zego_engine_state state, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onEngineStateUpdate(ZegoEngineState(state));
        ZEGO_SWITCH_THREAD_ING
    }

    // ROOM CALLBACK
    static void zego_on_room_state_update(const char *room_id, zego_room_state state,
                                          zego_error error_code, const char *extend_data,
                                          void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string roomID = room_id;
        std::string extendData = extend_data;
        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onRoomStateUpdate(roomID, ZegoRoomState(state), error_code, extendData);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_room_stream_update(const char *room_id, enum zego_update_type update_type,
                                           const struct zego_stream *stream_info_list,
                                           unsigned int stream_info_count,
                                           const char *extended_data, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string roomID = room_id;
        std::string extendedData = extended_data;
        std::vector<ZegoStream> streamList;
        for (unsigned int i = 0; i < stream_info_count; i++) {
            ZegoStream stream = ZegoExpressConvert::I2OStream(stream_info_list[i]);
            streamList.push_back(stream);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain) {
            handlerInMain->onRoomStreamUpdate(roomID, ZegoUpdateType(update_type), streamList,
                                              extendedData);
        }

        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_room_stream_extra_info_update(const char *room_id,
                                                      const struct zego_stream *stream_info_list,
                                                      unsigned int stream_info_count,
                                                      void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string roomID = room_id;
        std::vector<ZegoStream> streamList;
        for (unsigned int i = 0; i < stream_info_count; i++) {
            ZegoStream stream = ZegoExpressConvert::I2OStream(stream_info_list[i]);
            streamList.push_back(stream);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onRoomStreamExtraInfoUpdate(roomID, streamList);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_user_update(const char *room_id, zego_update_type update_type,
                                    const struct zego_user *user_list, unsigned int user_count,
                                    void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string roomID = room_id;
        std::vector<ZegoUser> userList;
        for (unsigned int i = 0; i < user_count; i++) {
            ZegoUser user = ZegoExpressConvert::I2OUser(user_list[i]);
            userList.push_back(user);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onRoomUserUpdate(roomID, ZegoUpdateType(update_type), userList);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_room_online_user_count_update(const char *room_id, int online_user_count,
                                                      void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string roomID = room_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onRoomOnlineUserCountUpdate(roomID, online_user_count);
        ZEGO_SWITCH_THREAD_ING
    }

    static void
    zego_on_room_extra_info_update(const char *room_id,
                                   const struct zego_room_extra_info *room_extra_info_list,
                                   unsigned int room_extra_info_count, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string roomID = room_id;
        std::vector<ZegoRoomExtraInfo> infoList;
        for (unsigned int i = 0; i < room_extra_info_count; i++) {
            ZegoRoomExtraInfo info = ZegoExpressConvert::I2ORoomExtraInfo(room_extra_info_list[i]);
            infoList.push_back(info);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onRoomExtraInfoUpdate(roomID, infoList);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_upload_log_result(zego_seq seq, zego_error error_code, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto callback = oInternalCallbackCenter->eraseZegoUploadLogResultCallback(seq);
        if (callback) {
            ZEGO_SWITCH_THREAD_PRE_STATIC
            callback(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void zego_on_room_set_room_extra_info_result(zego_error error_code, const char *room_id,
                                                        const char *key, zego_seq seq,
                                                        void *user_context) {
        ZEGO_UNUSED_VARIABLE(room_id);
        ZEGO_UNUSED_VARIABLE(key);
        ZEGO_UNUSED_VARIABLE(user_context);
        auto callback = oInternalCallbackCenter->eraseZegoRoomSetRoomExtraInfoCallback(seq);
        if (callback) {
            ZEGO_SWITCH_THREAD_PRE_STATIC
            callback(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void zego_on_room_token_will_expire(const char *room_id, int remain_time_in_second,
                                               void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string roomID = room_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onRoomTokenWillExpire(roomID, remain_time_in_second);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_room_login_callback(zego_error error_code, const char *extended_data,
                                            const char *room_id, zego_seq seq, void *user_context) {
        ZEGO_UNUSED_VARIABLE(room_id);
        ZEGO_UNUSED_VARIABLE(user_context);
        std::string extendedData = extended_data ? extended_data : "";
        auto callback = oInternalCallbackCenter->eraseZegoRoomLoginCallback(seq);
        if (callback) {
            ZEGO_SWITCH_THREAD_PRE_STATIC
            callback(error_code, extendedData);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void zego_on_room_logout_callback(zego_error error_code, const char *extended_data,
                                             const char *room_id, zego_seq seq,
                                             void *user_context) {
        ZEGO_UNUSED_VARIABLE(room_id);
        ZEGO_UNUSED_VARIABLE(user_context);
        std::string extendedData = extended_data ? extended_data : "";
        auto callback = oInternalCallbackCenter->eraseZegoRoomLogoutCallback(seq);
        if (callback) {
            ZEGO_SWITCH_THREAD_PRE_STATIC
            callback(error_code, extendedData);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void zego_on_room_state_changed(const char *room_id,
                                           enum zego_room_state_changed_reason reason,
                                           zego_error error_code, const char *extended_data,
                                           void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string roomID = room_id ? room_id : "";
        std::string extendData = extended_data ? extended_data : "";
        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onRoomStateChanged(roomID, ZegoRoomStateChangedReason(reason),
                                              error_code, extendData);
        ZEGO_SWITCH_THREAD_ING
    }

    // PUBLISHER CALLBACK
    static void zego_on_publisher_state_update(const char *stream_id, zego_publisher_state state,
                                               zego_error error_code, const char *extend_data,
                                               void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;
        std::string extendData = extend_data;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPublisherStateUpdate(streamID, ZegoPublisherState(state), error_code,
                                                  extendData);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_quality_update(const char *stream_id,
                                                 zego_publish_stream_quality quality,
                                                 void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        ZegoPublishStreamQuality pushlishQuality = ZegoExpressConvert::I2OPushlishQuality(quality);
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPublisherQualityUpdate(streamID, pushlishQuality);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_captured_audio_first_frame(void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPublisherCapturedAudioFirstFrame();
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_stream_event(enum zego_stream_event event_id,
                                               const char *stream_id, const char *extra_info,
                                               void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;
        std::string extraInfo = extra_info;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPublisherStreamEvent((ZegoStreamEvent)event_id, streamID, extraInfo);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_stream_event(enum zego_stream_event event_id, const char *stream_id,
                                            const char *extra_info, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;
        std::string extraInfo = extra_info;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPlayerStreamEvent((ZegoStreamEvent)event_id, streamID, extraInfo);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_captured_video_first_frame(enum zego_publish_channel channel,
                                                             void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPublisherCapturedVideoFirstFrame(ZegoPublishChannel(channel));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_send_audio_first_frame(enum zego_publish_channel channel,
                                                         void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPublisherSendAudioFirstFrame(ZegoPublishChannel(channel));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_send_video_first_frame(enum zego_publish_channel channel,
                                                         void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPublisherSendVideoFirstFrame(ZegoPublishChannel(channel));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_render_video_first_frame(enum zego_publish_channel channel,
                                                           void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPublisherRenderVideoFirstFrame(ZegoPublishChannel(channel));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_video_size_changed(int width, int height,
                                                     enum zego_publish_channel channel,
                                                     void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPublisherVideoSizeChanged(width, height, ZegoPublishChannel(channel));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_update_cdn_url_result(const char *stream_id,
                                                        zego_error error_code, zego_seq seq,
                                                        void *user_context) {
        ZEGO_UNUSED_VARIABLE(stream_id);
        ZEGO_UNUSED_VARIABLE(user_context);
        auto callback = oInternalCallbackCenter->eraseZegoPublisherUpdateCdnUrlCallback(seq);
        if (callback) {
            ZEGO_SWITCH_THREAD_PRE_STATIC
            callback(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void
    zego_on_publisher_relay_cdn_state_update(const char *stream_id,
                                             const zego_stream_relay_cdn_info *state_info_list,
                                             unsigned int info_count, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;
        std::vector<ZegoStreamRelayCDNInfo> streamInfoList;

        for (unsigned int i = 0; i < info_count; i++) {
            zego_stream_relay_cdn_info _info = state_info_list[i];
            ZegoStreamRelayCDNInfo info;
            info.url = _info.url;
            info.stateTime = _info.state_time;
            info.state = ZegoStreamRelayCDNState(_info.state);
            info.updateReason = ZegoStreamRelayCDNUpdateReason(_info.update_reason);
            streamInfoList.push_back(info);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPublisherRelayCDNStateUpdate(streamID, streamInfoList);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_video_encoder_changed(enum zego_video_codec_id from_codec_id,
                                                        enum zego_video_codec_id to_codec_id,
                                                        enum zego_publish_channel channel,
                                                        void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPublisherVideoEncoderChanged(
                static_cast<ZegoVideoCodecID>(from_codec_id),
                static_cast<ZegoVideoCodecID>(to_codec_id),
                static_cast<ZegoPublishChannel>(channel));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_update_stream_extra_info_result(zego_error error_code,
                                                                  zego_seq seq,
                                                                  void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto callback = oInternalCallbackCenter->eraseZegoPublisherSetStreamExtraInfoCallback(seq);
        if (callback) {
            ZEGO_SWITCH_THREAD_PRE_STATIC
            callback(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void zego_on_publisher_take_snapshot_result(zego_error error_code,
                                                       enum zego_publish_channel channel,
                                                       void *image, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto callback =
            oInternalCallbackCenter->eraseZegoPublisherTakeSnapshotCallback((int)channel);
        if (callback) {
            callback(error_code, image);
        }
    }

    static void zego_on_publisher_low_fps_warning(enum zego_video_codec_id codec_id,
                                                  enum zego_publish_channel channel,
                                                  void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPublisherLowFpsWarning((ZegoVideoCodecID)codec_id,
                                                    (ZegoPublishChannel)channel);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_publisher_dummy_capture_image_path_error(zego_error error_code,
                                                                 const char *path,
                                                                 enum zego_publish_channel channel,
                                                                 void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string resource_path = path ? path : "";
        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPublisherDummyCaptureImagePathError(error_code, resource_path,
                                                                 (ZegoPublishChannel)channel);
        ZEGO_SWITCH_THREAD_ING
    }

    // PLAYER CALLBACK
    static void zego_on_player_state_update(const char *stream_id, zego_player_state state,
                                            zego_error error_code, const char *extend_data,
                                            void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;
        std::string extendData = extend_data;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPlayerStateUpdate(streamID, ZegoPlayerState(state), error_code,
                                               extendData);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_switched(const char *stream_id, zego_error error_code,
                                        void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPlayerSwitched(streamID, error_code);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_quality_update(const char *stream_id,
                                              zego_play_stream_quality quality,
                                              void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;
        ZegoPlayStreamQuality playQuality = ZegoExpressConvert::I2OPlayQuality(quality);

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPlayerQualityUpdate(streamID, playQuality);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_media_event(const char *stream_id,
                                           zego_player_media_event media_event,
                                           void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPlayerMediaEvent(streamID, ZegoPlayerMediaEvent(media_event));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_recv_audio_first_frame(const char *stream_id, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPlayerRecvAudioFirstFrame(streamID);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_recv_video_first_frame(const char *stream_id, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;

        // sync callback
        if (handler) {
            handler->onPlayerSyncRecvVideoFirstFrame(streamID);
        }
        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPlayerRecvVideoFirstFrame(streamID);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_render_video_first_frame(const char *stream_id, void *user_context) {

        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPlayerRenderVideoFirstFrame(streamID);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_render_camera_video_first_frame(const char *stream_id,
                                                               void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id ? stream_id : "";

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);

        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPlayerRenderCameraVideoFirstFrame(streamID);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_video_size_changed(const char *stream_id, int width, int height,
                                                  void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPlayerVideoSizeChanged(streamID, width, height);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_recv_sei(struct zego_media_side_info info, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        ZegoMediaSideInfo mediaSideInfo = ZegoExpressConvert::I2OMediaSideInfo(info);

        if (handler) {
            handler->onPlayerSyncRecvSEI(mediaSideInfo.streamID, mediaSideInfo.SEIData,
                                         mediaSideInfo.SEIDataLength);
            handler->onPlayerRecvMediaSideInfo(mediaSideInfo);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        std::vector<unsigned char> seiData;
        seiData.reserve(info.sei_data_length);
        for (unsigned int i = 0; i < info.sei_data_length; i++) {
            seiData.push_back(info.sei_data[i]);
        }
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPlayerRecvSEI(mediaSideInfo.streamID, seiData.data(),
                                           ZegoUInt(seiData.size()));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_player_recv_audio_side_info(const char *stream_id,
                                                    const unsigned char *data,
                                                    unsigned int data_length, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;
        std::vector<unsigned char> audioSideData;
        audioSideData.reserve(data_length);
        for (unsigned int i = 0; i < data_length; i++) {
            audioSideData.push_back(data[i]);
        }

        if (handler) {
            handler->onPlayerRecvAudioSideInfo(streamID, audioSideData.data(),
                                               ZegoUInt(audioSideData.size()));
        }
    }
    //#endif

    static void zego_on_player_take_snapshot_result(zego_error error_code, const char *stream_id,
                                                    void *image, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        std::string streamID = stream_id;
        auto callback = oInternalCallbackCenter->eraseZegoPlayerTakeSnapshotCallback(streamID);
        if (callback) {
            callback(error_code, image);
        }
    }

    static void zego_on_player_low_fps_warning(zego_video_codec_id codec_id, const char *stream_id,
                                               void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPlayerLowFpsWarning(static_cast<ZegoVideoCodecID>(codec_id), streamID);
        ZEGO_SWITCH_THREAD_ING
    }

#if TARGET_OS_IPHONE || defined(ANDROID)
    static void zego_on_player_video_super_resolution_update(const char *stream_id,
                                                             zego_super_resolution_state state,
                                                             int error_code, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id ? stream_id : "";

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPlayerVideoSuperResolutionUpdate(
                streamID, static_cast<ZegoSuperResolutionState>(state), error_code);
        ZEGO_SWITCH_THREAD_ING
    }
#endif

    // DEVICE CALLBACK
    static void zego_on_audio_device_state_changed(enum zego_update_type update_type,
                                                   enum zego_audio_device_type device_type,
                                                   struct zego_device_info _device,
                                                   void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        ZegoDeviceInfo device = ZegoExpressConvert::I2ODeviceInfo(_device);

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onAudioDeviceStateChanged(ZegoUpdateType(update_type),
                                                     ZegoAudioDeviceType(device_type), device);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_audio_device_volume_changed(enum zego_audio_device_type device_type,
                                                    const char *_device_id, int volume,
                                                    void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string deviceID = _device_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onAudioDeviceVolumeChanged(ZegoAudioDeviceType(device_type), deviceID,
                                                      volume);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_video_device_state_changed(enum zego_update_type update_type,
                                                   struct zego_device_info _device,
                                                   void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        ZegoDeviceInfo device = ZegoExpressConvert::I2ODeviceInfo(_device);

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onVideoDeviceStateChanged(ZegoUpdateType(update_type), device);
        ZEGO_SWITCH_THREAD_ING
    }

    static void
    zego_on_local_device_exception_occurred(enum zego_device_exception_type exception_type,
                                            enum zego_device_type device_type,
                                            const char *device_id, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string deviceId = device_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain) {
            handlerInMain->onLocalDeviceExceptionOccurred(ZegoDeviceExceptionType(exception_type),
                                                          ZegoDeviceType(device_type), deviceId);
        }
        ZEGO_SWITCH_THREAD_ING
    }

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    static void zego_on_audio_route_change(zego_audio_route audio_route, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onAudioRouteChange(ZegoAudioRoute(audio_route));
        ZEGO_SWITCH_THREAD_ING
    }
#endif

    static void zego_on_remote_mic_state_update(const char *stream_id,
                                                zego_remote_device_state state,
                                                void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onRemoteMicStateUpdate(streamID, ZegoRemoteDeviceState(state));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_remote_speaker_state_update(const char *stream_id,
                                                    zego_remote_device_state state,
                                                    void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onRemoteSpeakerStateUpdate(streamID, ZegoRemoteDeviceState(state));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_remote_camera_state_update(const char *stream_id,
                                                   zego_remote_device_state state,
                                                   void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string streamID = stream_id;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onRemoteCameraStateUpdate(streamID, ZegoRemoteDeviceState(state));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_captured_sound_level_update(const zego_sound_level_info *sound_level_info,
                                                    void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        float soundLevel = sound_level_info->sound_level;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onCapturedSoundLevelUpdate(soundLevel);
        ZEGO_SWITCH_THREAD_ING
    }

    static void
    zego_on_captured_sound_level_info_update(const zego_sound_level_info *sound_level_info,
                                             void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        ZegoSoundLevelInfo info;

        info.soundLevel = sound_level_info->sound_level;
        info.vad = sound_level_info->vad;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onCapturedSoundLevelInfoUpdate(info);
        ZEGO_SWITCH_THREAD_ING
    }

    static void
    zego_on_remote_sound_level_update(const zego_sound_level_info *sound_level_info_list,
                                      unsigned int info_count, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        std::unordered_map<std::string, float> soundLevels;
        for (unsigned int i = 0; i < info_count; i++) {
            zego_sound_level_info sound_level_info = sound_level_info_list[i];
            std::string streamID = sound_level_info.stream_id;
            float soundLevel = sound_level_info.sound_level;
            soundLevels.insert({streamID, soundLevel});
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onRemoteSoundLevelUpdate(soundLevels);
        ZEGO_SWITCH_THREAD_ING
    }

    static void
    zego_on_remote_sound_level_info_update(const zego_sound_level_info *sound_level_info_list,
                                           unsigned int info_count, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        std::unordered_map<std::string, ZegoSoundLevelInfo> info;
        for (unsigned int i = 0; i < info_count; i++) {
            ZegoSoundLevelInfo soundLevelInfo;

            zego_sound_level_info sound_level_info = sound_level_info_list[i];
            soundLevelInfo.soundLevel = sound_level_info.sound_level;
            soundLevelInfo.vad = sound_level_info.vad;
            info.insert({sound_level_info.stream_id, soundLevelInfo});
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onRemoteSoundLevelInfoUpdate(info);
        ZEGO_SWITCH_THREAD_ING
    }

    static void
    zego_on_audio_vad_state_update(zego_audio_vad_stable_state_monitor_type monitor_type,
                                   zego_audio_vad_type type, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onAudioVADStateUpdate((ZegoAudioVADStableStateMonitorType)monitor_type,
                                                 (ZegoAudioVADType)type);
        ZEGO_SWITCH_THREAD_ING
    }

    static void
    zego_on_captured_audio_spectrum_update(const zego_audio_spectrum_info *audio_spectrum_info,
                                           void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        ZegoAudioSpectrum audioSpectrum;
        for (unsigned int i = 0; i < audio_spectrum_info->spectrum_count; i++) {
            audioSpectrum.push_back(audio_spectrum_info->spectrum_list[i]);
        }
        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onCapturedAudioSpectrumUpdate(audioSpectrum);
        ZEGO_SWITCH_THREAD_ING
    }

    static void
    zego_on_remote_audio_spectrum_update(const zego_audio_spectrum_info *audio_spectrum_info_list,
                                         unsigned int info_count, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::unordered_map<std::string, ZegoAudioSpectrum> audioSpectrums;
        for (unsigned int j = 0; j < info_count; j++) {
            zego_audio_spectrum_info frequency_spectrum_info = audio_spectrum_info_list[j];

            std::string streamID = frequency_spectrum_info.stream_id;
            ZegoAudioSpectrum frequencySpectrum;
            for (unsigned int i = 0; i < frequency_spectrum_info.spectrum_count; i++) {
                frequencySpectrum.push_back(frequency_spectrum_info.spectrum_list[i]);
            }
            audioSpectrums.insert({streamID, frequencySpectrum});
        }
        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onRemoteAudioSpectrumUpdate(audioSpectrums);
        ZEGO_SWITCH_THREAD_ING
    }

    // IM CALLBACK

    static void zego_on_receive_real_time_sequential_data(int index, const unsigned char *data,
                                                          unsigned int data_length,
                                                          const char *stream_id,
                                                          void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto manager =
            oInternalCallbackCenter->getZegoExpressRealTimeSequentialDataManagerImp(index);
        if (manager) {
            manager->zego_on_receive_real_time_sequential_data(data, data_length, stream_id);
        }
    }

    static void zego_on_real_time_sequential_data_sent(zego_error error_code, int index,
                                                       zego_seq seq, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto manager =
            oInternalCallbackCenter->getZegoExpressRealTimeSequentialDataManagerImp(index);
        if (manager) {
            manager->zego_on_real_time_sequential_data_sent(error_code, seq);
        }
    }

    static void zego_on_im_send_broadcast_message_result(const char *room_id,
                                                         unsigned long long message_id,
                                                         zego_error error_code, zego_seq seq,
                                                         void *user_context) {
        ZEGO_UNUSED_VARIABLE(room_id);
        ZEGO_UNUSED_VARIABLE(user_context);
        auto callback = oInternalCallbackCenter->eraseZegoIMSendBroadcastMessageCallback(seq);
        if (callback) {
            unsigned long long messageID = message_id;
            ZEGO_SWITCH_THREAD_PRE_STATIC
            callback(error_code, messageID);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void
    zego_on_im_recv_broadcast_message(const char *room_id,
                                      const struct zego_broadcast_message_info *message_info_list,
                                      unsigned int message_count, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string roomID = room_id;
        std::vector<ZegoBroadcastMessageInfo> messageInfoList;
        for (unsigned int i = 0; i < message_count; i++) {
            zego_broadcast_message_info message_info = message_info_list[i];
            ZegoBroadcastMessageInfo messageInfo;
            messageInfo.messageID = message_info.message_id;
            messageInfo.sendTime = message_info.send_time;
            messageInfo.message = message_info.message;
            messageInfo.fromUser = ZegoExpressConvert::I2OUser(message_info.from_user);
            messageInfoList.push_back(messageInfo);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onIMRecvBroadcastMessage(roomID, messageInfoList);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_im_send_barrage_message_result(const char *room_id, const char *message_id,
                                                       zego_error error_code, zego_seq seq,
                                                       void *user_context) {
        ZEGO_UNUSED_VARIABLE(room_id);
        ZEGO_UNUSED_VARIABLE(user_context);
        auto callback = oInternalCallbackCenter->eraseZegoIMSendBarrageMessageCallback(seq);
        if (callback) {
            std::string messageID = message_id;
            ZEGO_SWITCH_THREAD_PRE_STATIC
            callback(error_code, messageID);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void
    zego_on_im_recv_barrage_message(const char *room_id,
                                    const struct zego_barrage_message_info *message_info_list,
                                    unsigned int message_count, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        std::string roomID = room_id;
        std::vector<ZegoBarrageMessageInfo> messageInfoList;
        for (unsigned int i = 0; i < message_count; i++) {
            zego_barrage_message_info message_info = message_info_list[i];
            ZegoBarrageMessageInfo messageInfo;
            messageInfo.messageID = message_info.message_id;
            messageInfo.sendTime = message_info.send_time;
            messageInfo.message = message_info.message;
            messageInfo.fromUser = ZegoExpressConvert::I2OUser(message_info.from_user);
            messageInfoList.push_back(messageInfo);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onIMRecvBarrageMessage(roomID, messageInfoList);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_im_send_custom_command_result(const char *room_id, zego_error error_code,
                                                      zego_seq seq, void *user_context) {
        ZEGO_UNUSED_VARIABLE(room_id);
        ZEGO_UNUSED_VARIABLE(user_context);
        auto callback = oInternalCallbackCenter->eraseZegoIMSendCustomCommandCallback(seq);
        if (callback) {
            ZEGO_SWITCH_THREAD_PRE_STATIC
            callback(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void zego_on_im_recv_custom_command(const char *room_id, struct zego_user from_user,
                                               const char *_content, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        std::string roomID = room_id;
        ZegoUser user = ZegoExpressConvert::I2OUser(from_user);
        std::string content = _content;

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onIMRecvCustomCommand(roomID, user, content);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_im_send_transparent_message_result(const char *room_id,
                                                           zego_error error_code, zego_seq seq,
                                                           void *user_context) {
        ZEGO_UNUSED_VARIABLE(room_id);
        ZEGO_UNUSED_VARIABLE(user_context);
        auto callback = oInternalCallbackCenter->eraseZegoRoomSendTransparentMessageCallback(seq);
        if (callback) {
            ZEGO_SWITCH_THREAD_PRE_STATIC
            callback(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void
    zego_on_im_recv_transparent_message(const char *room_id,
                                        const struct zego_room_recv_transparent_message *message,
                                        const char *_content, void *user_context) {
        ZEGO_UNUSED_VARIABLE(_content);
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        std::string roomID = room_id;
        ZegoRoomRecvTransparentMessage message_;
        message_.content.assign((const char *)message->content, message->content_length);
        message_.sendUser = ZegoExpressConvert::I2OUser(message->send_user);

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onRecvRoomTransparentMessage(roomID, message_);
        ZEGO_SWITCH_THREAD_ING
    }

    // MIXER CALLBACK
    static void zego_on_mixer_start_result(zego_error error_code, zego_seq seq,
                                           const char *extended_data, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto callback = oInternalCallbackCenter->eraseZegoMixerStartCallback(seq);
        if (callback) {
            int errorCode = error_code;
            std::string extendData = extended_data;
            ZEGO_SWITCH_THREAD_PRE_STATIC
            callback(errorCode, extendData);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void zego_on_mixer_stop_result(zego_error error_code, zego_seq seq, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto callback = oInternalCallbackCenter->eraseZegoMixerStopCallback(seq);
        if (callback) {
            int errorCode = error_code;
            ZEGO_SWITCH_THREAD_PRE_STATIC
            callback(errorCode);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void zego_on_mixer_sound_level_update(
        const struct zego_mixer_sound_level_info *sound_level_info_list, unsigned int info_count,
        void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        std::unordered_map<unsigned int, float> soundLevels;
        for (unsigned int i = 0; i < info_count; i++) {
            zego_mixer_sound_level_info sound_level_info = sound_level_info_list[i];
            unsigned int soundLevelID = sound_level_info.sound_level_id;
            float soundLevel = sound_level_info.sound_level;
            soundLevels.insert({soundLevelID, soundLevel});
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onMixerSoundLevelUpdate(soundLevels);
        ZEGO_SWITCH_THREAD_ING
    }

    static void
    zego_on_auto_mixer_sound_level_update(const struct zego_sound_level_info *sound_levels,
                                          unsigned int info_count, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();

        std::unordered_map<std::string, float> soundLevels;
        for (unsigned int i = 0; i < info_count; i++) {
            zego_sound_level_info sound_level_info = sound_levels[i];
            std::string streamID = sound_level_info.stream_id;
            float soundLevel = sound_level_info.sound_level;
            soundLevels.insert({streamID, soundLevel});
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onAutoMixerSoundLevelUpdate(soundLevels);
        ZEGO_SWITCH_THREAD_ING
    }

    static void
    zego_on_mixer_relay_cdn_state_update(const char *task_id,
                                         const struct zego_stream_relay_cdn_info *cdn_info_list,
                                         unsigned int info_count, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        std::string taskID = task_id;
        std::vector<ZegoStreamRelayCDNInfo> streamInfoList;

        for (unsigned int i = 0; i < info_count; i++) {
            zego_stream_relay_cdn_info _info = cdn_info_list[i];
            ZegoStreamRelayCDNInfo info;
            info.url = _info.url;
            info.stateTime = _info.state_time;
            info.state = ZegoStreamRelayCDNState(_info.state);
            info.updateReason = ZegoStreamRelayCDNUpdateReason(_info.update_reason);
            streamInfoList.push_back(info);
        }

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onMixerRelayCDNStateUpdate(taskID, streamInfoList);
        ZEGO_SWITCH_THREAD_ING
    }

    // MediaPlayer Callback
    static void zego_on_media_player_state_update(zego_media_player_state state,
                                                  zego_error error_code,
                                                  zego_media_player_instance_index instance_index,
                                                  void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            mediaPlayer->zego_on_media_player_state_update(state, error_code);
        }
    }

    static void zego_on_media_player_network_event(zego_media_player_network_event event,
                                                   zego_media_player_instance_index instance_index,
                                                   void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            mediaPlayer->zego_on_media_player_network_event(event);
        }
    }

    static void
    zego_on_media_player_playing_progress(unsigned long long millisecond,
                                          zego_media_player_instance_index instance_index,
                                          void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            mediaPlayer->zego_on_media_player_playing_progress(millisecond);
        }
    }

    static void
    zego_on_media_player_rendering_progress(unsigned long long millisecond,
                                            zego_media_player_instance_index instance_index,
                                            void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            mediaPlayer->zego_on_media_player_rendering_progress(millisecond);
        }
    }

    static void
    zego_on_media_player_video_size_changed(int width, int height,
                                            zego_media_player_instance_index instance_index,
                                            void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            mediaPlayer->zego_on_media_player_video_size_changed(width, height);
        }
    }

    static void zego_on_media_player_recv_sei(const unsigned char *data, unsigned int data_length,
                                              enum zego_media_player_instance_index instance_index,
                                              void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            mediaPlayer->zego_on_media_player_recv_sei(data, data_length);
        }
    }

    static void
    zego_on_media_player_sound_level_update(float sound_level,
                                            enum zego_media_player_instance_index instance_index,
                                            void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            mediaPlayer->zego_on_media_player_sound_level_update(sound_level);
        }
    }

    static void zego_on_media_player_frequency_spectrum_update(
        float *spectrum_list, unsigned int spectrum_count,
        enum zego_media_player_instance_index instance_index, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            mediaPlayer->zego_on_media_player_frequency_spectrum_update(spectrum_list,
                                                                        spectrum_count);
        }
    }

    static void zego_on_media_player_seek_to(zego_seq seq, zego_error error_code,
                                             enum zego_media_player_instance_index instance_index,
                                             void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            mediaPlayer->zego_on_mediaplayer_seek_to_result(seq, error_code);
        }
    }

    static void
    zego_on_mediaplayer_load_file_result(zego_error error_code,
                                         zego_media_player_instance_index instance_index,
                                         void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            mediaPlayer->zego_on_mediaplayer_load_file_result(error_code);
        }
    }

    static void
    zego_on_media_player_audio_frame(const unsigned char *data, unsigned int data_length,
                                     const struct zego_audio_frame_param param,
                                     enum zego_media_player_instance_index instance_index,
                                     void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            mediaPlayer->zego_on_media_player_audio_frame(data, data_length, param);
        }
    }

    static void
    zego_on_media_player_first_frame_event(enum zego_media_player_first_frame_event event,
                                           enum zego_media_player_instance_index instance_index,
                                           void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            mediaPlayer->zego_on_media_player_first_frame_event(event);
        }
    }

    static void zego_on_media_player_local_cache(
        zego_error error, const char *resource, const char *cached_file,
        enum zego_media_player_instance_index instance_index, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            mediaPlayer->zego_on_media_player_local_cache(error, resource, cached_file);
        }
    }

    static void zego_on_media_player_video_frame(
        const unsigned char **data, unsigned int *data_length,
        const struct zego_video_frame_param param, const char *extra_info,
        enum zego_media_player_instance_index instance_index, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            mediaPlayer->zego_on_media_player_video_frame(data, data_length, param, extra_info);
        }
    }

    static void
    zego_on_media_player_take_snapshot_result(zego_error error_code, void *image,
                                              enum zego_media_player_instance_index instance_index,
                                              void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            mediaPlayer->zego_on_media_player_take_snapshot_result(error_code, image,
                                                                   instance_index);
        }
    }

    static void
    zego_on_media_player_block_begin(const char *path,
                                     enum zego_media_player_instance_index instance_index,
                                     void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            mediaPlayer->zego_on_media_player_block_begin(path);
        }
    }

    static unsigned int
    zego_on_media_player_block_data(unsigned char *const buffer, unsigned int buffer_size,
                                    enum zego_media_player_instance_index instance_index,
                                    void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
        if (mediaPlayer) {
            return mediaPlayer->zego_on_media_player_block_data(buffer, buffer_size);
        }
        return -1;
    }

    static void zego_on_audio_effect_play_state_update(
        unsigned int audio_effect_id, enum zego_audio_effect_play_state state,
        zego_error error_code, enum zego_audio_effect_player_instance_index instance_index,
        void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto audioEffectPlayer =
            oInternalCallbackCenter->getZegoExpressAudioEffectPlayerImp(instance_index);
        if (audioEffectPlayer) {
            audioEffectPlayer->zego_on_audio_effect_play_state_update(audio_effect_id, state,
                                                                      error_code);
        }
    }

    static void zego_on_audio_effect_player_load_resource(
        zego_seq seq, zego_error error_code,
        enum zego_audio_effect_player_instance_index instance_index, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto audioEffectPlayer =
            oInternalCallbackCenter->getZegoExpressAudioEffectPlayerImp(instance_index);
        if (audioEffectPlayer) {
            audioEffectPlayer->zego_on_audio_effect_player_load_resource(seq, error_code);
        }
    }

    static void
    zego_on_audio_effect_player_seek_to(zego_seq seq, zego_error error_code,
                                        enum zego_audio_effect_player_instance_index instance_index,
                                        void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto audioEffectPlayer =
            oInternalCallbackCenter->getZegoExpressAudioEffectPlayerImp(instance_index);
        if (audioEffectPlayer) {
            audioEffectPlayer->zego_on_audio_effect_player_seek_to(seq, error_code);
        }
    }

    static void zego_on_range_audio_microphone_state_update(
        enum zego_range_audio_microphone_state state, zego_error error_code,
        enum zego_range_audio_instance_index instance_index, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto rangeAudio = oInternalCallbackCenter->getZegoExpressRangeAudioImp(instance_index);
        if (rangeAudio) {
            rangeAudio->zego_on_range_audio_microphone_state_update(state, error_code);
        }
    }

    // CustomVideoRender
    static void zego_on_custom_video_render_captured_frame_data(
        unsigned char **data, unsigned int *data_length, const struct zego_video_frame_param _param,
        enum zego_video_flip_mode flip_mode, enum zego_publish_channel channel,
        void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoCustomVideoRenderHandler();
        if (handler) {
            auto param = ZegoExpressConvert::I2OVideoFrameParam(_param);
            ZegoVideoFlipMode flipMode = ZegoVideoFlipMode(flip_mode);
            handler->onCapturedVideoFrameRawData(data, data_length, param, flipMode,
                                                 ZegoPublishChannel(channel));
        }
    }

    static void zego_on_custom_video_render_remote_frame_data(
        const char *stream_id, unsigned char **data, unsigned int *data_length,
        const struct zego_video_frame_param _param, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoCustomVideoRenderHandler();
        if (handler) {
            std::string streamID = stream_id;
            auto param = ZegoExpressConvert::I2OVideoFrameParam(_param);
            handler->onRemoteVideoFrameRawData(data, data_length, param, streamID);
        }
    }

    static void zego_on_custom_video_render_remote_frame_encoded_data(
        const unsigned char *data, unsigned int data_length,
        const struct zego_video_encoded_frame_param _param,
        unsigned long long reference_time_millisecond, const char *stream_id, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoCustomVideoRenderHandler();
        if (handler) {
            std::string streamID = stream_id;
            auto param = ZegoExpressConvert::I2OVideoEncodedFrameParam(_param);
            handler->onRemoteVideoFrameEncodedData(data, data_length, param,
                                                   reference_time_millisecond, streamID);
        }
    }

    static void zego_on_custom_video_process_captured_unprocessed_raw_data(
        const unsigned char **data, unsigned int *data_length,
        const struct zego_video_frame_param _param, unsigned long long reference_time_millisecond,
        enum zego_publish_channel channel, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoCustomVideoProcessHandler();
        if (handler) {
            auto param = ZegoExpressConvert::I2OVideoFrameParam(_param);
            handler->onCapturedUnprocessedRawData(
                data, data_length, param, reference_time_millisecond, ZegoPublishChannel(channel));
        }
    }

    static void zego_on_custom_video_process_captured_unprocessed_cvpixelbuffer(
        void *buffer, unsigned long long reference_time_millisecond,
        enum zego_publish_channel channel, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoCustomVideoProcessHandler();
        if (handler) {
            handler->onCapturedUnprocessedCVPixelBuffer(buffer, reference_time_millisecond,
                                                        ZegoPublishChannel(channel));
        }
    }

    // CustomVideoCapture
    static void zego_on_custom_video_capture_start(enum zego_publish_channel channel,
                                                   void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto weakHandler = std::weak_ptr<IZegoCustomVideoCaptureHandler>(
            oInternalCallbackCenter->getIZegoCustomVideoCaptureHandler());
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler)
            handler->onStart(ZegoPublishChannel(channel));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_custom_video_capture_stop(enum zego_publish_channel channel,
                                                  void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto weakHandler = std::weak_ptr<IZegoCustomVideoCaptureHandler>(
            oInternalCallbackCenter->getIZegoCustomVideoCaptureHandler());
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler)
            handler->onStop(ZegoPublishChannel(channel));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_custom_video_capture_encoded_data_traffic_control(
        struct zego_traffic_control_info traffic_control_info, enum zego_publish_channel channel,
        void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        ;
        auto weakHandler = std::weak_ptr<IZegoCustomVideoCaptureHandler>(
            oInternalCallbackCenter->getIZegoCustomVideoCaptureHandler());
        ZegoTrafficControlInfo trafficControlInfo;
        trafficControlInfo.bitrate = traffic_control_info.bitrate;
        trafficControlInfo.fps = traffic_control_info.fps;
        trafficControlInfo.height = traffic_control_info.height;
        trafficControlInfo.width = traffic_control_info.width;
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler)
            handler->onEncodedDataTrafficControl(trafficControlInfo, ZegoPublishChannel(channel));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_audio_mixing_copy_data(struct zego_audio_mixing_data *data,
                                               void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoAudioMixingHandler();
        if (handler) {
            ZegoAudioMixingData audioMixingData;
            audioMixingData.audioData = data->audio_data;
            audioMixingData.audioDataLength = data->audio_data_length;
            audioMixingData.param.channel = ZegoAudioChannel(data->param.channel);
            audioMixingData.param.sampleRate = ZegoAudioSampleRate(data->param.sample_rate);
            audioMixingData.SEIData = data->sei_data;
            audioMixingData.SEIDataLength = data->sei_data_length;
            handler->onAudioMixingCopyData(&audioMixingData);
            data->audio_data_length = audioMixingData.audioDataLength;
            data->param.channel = zego_audio_channel(audioMixingData.param.channel);
            data->param.sample_rate = zego_audio_sample_rate(audioMixingData.param.sampleRate);
            data->sei_data_length = audioMixingData.SEIDataLength;
        }
    }

    static void zego_on_captured_audio_data(const unsigned char *data, unsigned int data_length,
                                            zego_audio_frame_param _param, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoAudioDataHandler();
        if (handler) {
            ZegoAudioFrameParam param = ZegoExpressConvert::I2OAudioFrameParam(_param);
            handler->onCapturedAudioData(data, data_length, param);
        }
    }

    static void zego_on_playback_audio_data(const unsigned char *data, unsigned int data_length,
                                            zego_audio_frame_param _param, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoAudioDataHandler();
        if (handler) {
            ZegoAudioFrameParam param = ZegoExpressConvert::I2OAudioFrameParam(_param);
            handler->onPlaybackAudioData(data, data_length, param);
        }
    }

    static void zego_on_mixed_audio_data(const unsigned char *data, unsigned int data_length,
                                         zego_audio_frame_param _param, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoAudioDataHandler();
        if (handler) {
            ZegoAudioFrameParam param = ZegoExpressConvert::I2OAudioFrameParam(_param);
            handler->onMixedAudioData(data, data_length, param);
        }
    }

    static void zego_on_player_audio_data(const unsigned char *data, unsigned int data_length,
                                          struct zego_audio_frame_param _param,
                                          const char *stream_id, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoAudioDataHandler();
        if (handler) {
            ZegoAudioFrameParam param = ZegoExpressConvert::I2OAudioFrameParam(_param);
            handler->onPlayerAudioData(data, data_length, param, stream_id);
        }
    }

    static void zego_on_captured_data_record_state_update(zego_data_record_state state,
                                                          zego_error error_code,
                                                          zego_data_record_config _config,
                                                          zego_publish_channel channel,
                                                          void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto weakHandler = std::weak_ptr<IZegoDataRecordEventHandler>(
            oInternalCallbackCenter->getIZegoDataRecordEventHandler());
        ZegoDataRecordConfig config = ZegoExpressConvert::I2ODataRecordConfig(_config);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler)
            handler->onCapturedDataRecordStateUpdate(ZegoDataRecordState(state), error_code, config,
                                                     ZegoPublishChannel(channel));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_captured_data_record_progress_update(zego_data_record_progress _progress,
                                                             zego_data_record_config _config,
                                                             zego_publish_channel channel,
                                                             void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto weakHandler = std::weak_ptr<IZegoDataRecordEventHandler>(
            oInternalCallbackCenter->getIZegoDataRecordEventHandler());
        ZegoDataRecordConfig config = ZegoExpressConvert::I2ODataRecordConfig(_config);
        ZegoDataRecordProgress progress;
        progress.duration = _progress.duration;
        progress.currentFileSize = _progress.current_file_size;
        progress.quality = ZegoExpressConvert::I2OPushlishQuality(_progress.quality);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler)
            handler->onCapturedDataRecordProgressUpdate(progress, config,
                                                        ZegoPublishChannel(channel));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_process_captured_audio_data(unsigned char *data, unsigned int data_length,
                                                    struct zego_audio_frame_param *_param,
                                                    double timestamp, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoCustomAudioProcessHandler();
        if (handler) {
            ZegoAudioFrameParam param = ZegoExpressConvert::I2OAudioFrameParam(*_param);
            handler->onProcessCapturedAudioData(data, data_length, &param, timestamp);
            *_param = ZegoExpressConvert::O2IAudioFrameParam(param);
        }
    }

    static void zego_on_process_captured_audio_data_after_used_headphone_monitor(
        unsigned char *data, unsigned int data_length, struct zego_audio_frame_param *_param,
        double timestamp, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoCustomAudioProcessHandler();
        if (handler) {
            ZegoAudioFrameParam param = ZegoExpressConvert::I2OAudioFrameParam(*_param);
            handler->onProcessCapturedAudioDataAfterUsedHeadphoneMonitor(data, data_length, &param,
                                                                         timestamp);
            *_param = ZegoExpressConvert::O2IAudioFrameParam(param);
        }
    }

    static void zego_on_before_audio_prep_audio_data(const unsigned char *data,
                                                     unsigned int data_length,
                                                     struct zego_audio_frame_param _param,
                                                     void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoCustomAudioProcessHandler();
        if (handler) {
            ZegoAudioFrameParam param = ZegoExpressConvert::I2OAudioFrameParam(_param);
            handler->onBeforeAudioPrepAudioData(data, data_length, param);
        }
    }

    static void zego_on_process_remote_audio_data(unsigned char *data, unsigned int data_length,
                                                  struct zego_audio_frame_param *_param,
                                                  const char *stream_id, double timestamp,
                                                  void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoCustomAudioProcessHandler();
        if (handler) {
            ZegoAudioFrameParam param = ZegoExpressConvert::I2OAudioFrameParam(*_param);
            handler->onProcessRemoteAudioData(data, data_length, &param, stream_id, timestamp);
            *_param = ZegoExpressConvert::O2IAudioFrameParam(param);
        }
    }

    static void zego_on_process_playback_audio_data(unsigned char *data, unsigned int data_length,
                                                    struct zego_audio_frame_param *_param,
                                                    double timestamp, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoCustomAudioProcessHandler();
        if (handler) {
            ZegoAudioFrameParam param = ZegoExpressConvert::I2OAudioFrameParam(*_param);
            handler->onProcessPlaybackAudioData(data, data_length, &param, timestamp);
            *_param = ZegoExpressConvert::O2IAudioFrameParam(param);
        }
    }

    // Utilities callback
    static void zego_on_performance_status_update(zego_performance_status _status,
                                                  void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto handler = oInternalCallbackCenter->getIZegoEventHandler();
        ZegoPerformanceStatus status = ZegoExpressConvert::I2OPerformanceStatus(_status);

        auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handlerInMain = weakHandler.lock();
        if (handlerInMain)
            handlerInMain->onPerformanceStatusUpdate(status);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_network_mode_changed(enum zego_network_mode mode, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto weakHandler =
            std::weak_ptr<IZegoEventHandler>(oInternalCallbackCenter->getIZegoEventHandler());
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler)
            handler->onNetworkModeChanged(ZegoNetworkMode(mode));
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_network_speed_test_error(int error_code,
                                                 enum zego_network_speed_test_type type,
                                                 void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto weakHandler =
            std::weak_ptr<IZegoEventHandler>(oInternalCallbackCenter->getIZegoEventHandler());
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler)
            handler->onNetworkSpeedTestError(error_code, ZegoNetworkSpeedTestType(type));
        ZEGO_SWITCH_THREAD_ING
    }

    static void
    zego_on_network_speed_test_quality_update(const struct zego_network_speed_test_quality _quality,
                                              enum zego_network_speed_test_type type,
                                              void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto weakHandler =
            std::weak_ptr<IZegoEventHandler>(oInternalCallbackCenter->getIZegoEventHandler());
        ZegoNetworkSpeedTestQuality quality;
        quality.connectCost = _quality.connect_cost;
        quality.packetLostRate = _quality.packet_lost_rate;
        quality.rtt = _quality.rtt;
        quality.quality = (ZegoStreamQualityLevel)_quality.quality;
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler)
            handler->onNetworkSpeedTestQualityUpdate(quality, ZegoNetworkSpeedTestType(type));
        ZEGO_SWITCH_THREAD_ING
    }

    static void
    zego_on_test_network_connectivity(zego_seq seq, zego_error error_code,
                                      const struct zego_test_network_connectivity_result _result,
                                      void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        ZegoTestNetworkConnectivityResult result;
        result.connectCost = _result.connect_cost;
        auto callback = oInternalCallbackCenter->eraseZegoTestNetworkConnectivityCallback(seq);
        if (callback) {
            ZEGO_SWITCH_THREAD_PRE_STATIC
            callback(error_code, result);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void zego_on_network_probe_result(zego_seq seq, zego_error error_code,
                                             const struct zego_network_probe_result _result,
                                             void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        ZegoNetworkProbeHttpResult httpResult;
        ZegoNetworkProbeTcpResult tcpResult;
        ZegoNetworkProbeUdpResult udpResult;
        ZegoNetworkProbeTracerouteResult tracerouteResult;
        bool httpResultValid = false;
        bool tcpResultValid = false;
        bool udpResultValid = false;
        bool tracerouteResultValid = false;
        if (_result.http_probe_result) {
            httpResultValid = true;
            httpResult.errorCode = _result.http_probe_result->error_code;
            httpResult.requestCostTime = _result.http_probe_result->request_cost_time;
        }
        if (_result.tcp_probe_result) {
            tcpResultValid = true;
            tcpResult.connectCostTime = _result.tcp_probe_result->connect_cost_time;
            tcpResult.rtt = _result.tcp_probe_result->rtt;
            tcpResult.errorCode = _result.tcp_probe_result->error_code;
        }
        if (_result.udp_probe_result) {
            udpResultValid = true;
            udpResult.errorCode = _result.udp_probe_result->error_code;
            udpResult.rtt = _result.udp_probe_result->rtt;
        }
        if (_result.traceroute_result) {
            tracerouteResultValid = true;
            tracerouteResult.errorCode = _result.traceroute_result->error_code;
            tracerouteResult.tracerouteCostTime = _result.traceroute_result->traceroute_cost_time;
        }

        auto callback = oInternalCallbackCenter->eraseZegoNetworkProbeResultCallback(seq);
        if (callback) {
            ZEGO_SWITCH_THREAD_PRE_STATIC
            auto httpResultTemp = httpResult;
            auto tcpResultTemp = tcpResult;
            auto udpResultTemp = udpResult;
            auto traceRouteResultTemp = tracerouteResult;
            ZegoNetworkProbeResult result;
            result.httpProbeResult = httpResultValid ? &httpResultTemp : nullptr;
            result.tcpProbeResult = tcpResultValid ? &tcpResultTemp : nullptr;
            result.udpProbeResult = udpResultValid ? &udpResultTemp : nullptr;
            result.tracerouteResult = tracerouteResultValid ? &traceRouteResultTemp : nullptr;
            callback(error_code, result);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    static void zego_on_request_dump_data(void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto weakHandler =
            std::weak_ptr<IZegoEventHandler>(oInternalCallbackCenter->getIZegoEventHandler());
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler)
            handler->onRequestDumpData();
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_request_upload_dump_data(const char *dump_dir, bool take_photo,
                                                 void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto weakHandler =
            std::weak_ptr<IZegoEventHandler>(oInternalCallbackCenter->getIZegoEventHandler());
        std::string dir = dump_dir ? dump_dir : "";
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler) {
            handler->onRequestUploadDumpData(dir, take_photo);
        }
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_start_dump_data(int error_code, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto weakHandler =
            std::weak_ptr<IZegoEventHandler>(oInternalCallbackCenter->getIZegoEventHandler());
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler)
            handler->onStartDumpData(error_code);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_stop_dump_data(int error_code, const char *dump_dir, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto weakHandler =
            std::weak_ptr<IZegoEventHandler>(oInternalCallbackCenter->getIZegoEventHandler());
        std::string dir = dump_dir ? dump_dir : "";
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler) {
            handler->onStopDumpData(error_code, dir);
        }
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_upload_dump_data(int error_code, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto weakHandler =
            std::weak_ptr<IZegoEventHandler>(oInternalCallbackCenter->getIZegoEventHandler());
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler)
            handler->onUploadDumpData(error_code);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_recv_experimental_api(const char *json, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto weakHandler =
            std::weak_ptr<IZegoEventHandler>(oInternalCallbackCenter->getIZegoEventHandler());
        std::string jsonStr = json;
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler)
            handler->onRecvExperimentalAPI(jsonStr);
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_network_quality(const char *user_id,
                                        enum zego_stream_quality_level upstream_quality,
                                        enum zego_stream_quality_level downstream_quality,
                                        void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto weakHandler =
            std::weak_ptr<IZegoEventHandler>(oInternalCallbackCenter->getIZegoEventHandler());
        std::string userID = "";
        if (user_id) {
            userID = user_id;
        }
        ZegoStreamQualityLevel upstreamQuality = (ZegoStreamQualityLevel)upstream_quality;
        ZegoStreamQualityLevel downstreamQuality = (ZegoStreamQualityLevel)downstream_quality;
        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler) {
            handler->onNetworkQuality(userID, upstreamQuality, downstreamQuality);
        }
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_copyrighted_music_download_progress_update(zego_seq seq,
                                                                   const char *resource_id,
                                                                   float progress_rate,
                                                                   void *user_context) {
        ZEGO_UNUSED_VARIABLE(seq);
        ZEGO_UNUSED_VARIABLE(user_context);
        auto copyrightedMusic = oInternalCallbackCenter->getZegoExpressCopyrightedMusicImp();

        if (copyrightedMusic) {
            copyrightedMusic->zego_on_copyrighted_music_download_progress_update(resource_id,
                                                                                 progress_rate);
        }
    }

    static void zego_on_copyrighted_music_current_pitch_value_update(const char *resource_id,
                                                                     int current_duration,
                                                                     int pitch_value,
                                                                     void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto copyrightedMusic = oInternalCallbackCenter->getZegoExpressCopyrightedMusicImp();

        if (copyrightedMusic) {
            copyrightedMusic->zego_on_copyrighted_music_current_pitch_value_update(
                resource_id, current_duration, pitch_value);
        }
    }

    static void zego_on_copyrighted_music_init(zego_seq seq, zego_error error_code,
                                               void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto copyrightedMusic = oInternalCallbackCenter->getZegoExpressCopyrightedMusicImp();

        if (copyrightedMusic) {
            copyrightedMusic->zego_on_copyrighted_music_init(seq, error_code);
        }
    }

    static void zego_on_copyrighted_music_send_extended_request(zego_seq seq, zego_error error_code,
                                                                const char *command,
                                                                const char *result,
                                                                void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto copyrightedMusic = oInternalCallbackCenter->getZegoExpressCopyrightedMusicImp();

        if (copyrightedMusic) {
            copyrightedMusic->zego_on_copyrighted_music_send_extended_request(seq, error_code,
                                                                              command, result);
        }
    }

    static void zego_on_copyrighted_music_get_lrc_lyric(zego_seq seq, zego_error error_code,
                                                        const char *lyrics, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto copyrightedMusic = oInternalCallbackCenter->getZegoExpressCopyrightedMusicImp();

        if (copyrightedMusic) {
            copyrightedMusic->zego_on_copyrighted_music_get_lrc_lyric(seq, error_code, lyrics);
        }
    }

    static void zego_on_copyrighted_music_get_krc_lyric_by_token(zego_seq seq,
                                                                 zego_error error_code,
                                                                 const char *lyrics,
                                                                 void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto copyrightedMusic = oInternalCallbackCenter->getZegoExpressCopyrightedMusicImp();

        if (copyrightedMusic) {
            copyrightedMusic->zego_on_copyrighted_music_get_krc_lyric_by_token(seq, error_code,
                                                                               lyrics);
        }
    }

    static void zego_on_copyrighted_music_request_song(zego_seq seq, zego_error error_code,
                                                       const char *resource, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto copyrightedMusic = oInternalCallbackCenter->getZegoExpressCopyrightedMusicImp();

        if (copyrightedMusic) {
            copyrightedMusic->zego_on_copyrighted_music_request_song(seq, error_code, resource);
        }
    }

    static void zego_on_copyrighted_music_request_accompaniment(zego_seq seq, zego_error error_code,
                                                                const char *resource,
                                                                void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto copyrightedMusic = oInternalCallbackCenter->getZegoExpressCopyrightedMusicImp();

        if (copyrightedMusic) {
            copyrightedMusic->zego_on_copyrighted_music_request_accompaniment(seq, error_code,
                                                                              resource);
        }
    }

    static void zego_on_copyrighted_music_request_accompaniment_clip(zego_seq seq,
                                                                     zego_error error_code,
                                                                     const char *resource,
                                                                     void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto copyrightedMusic = oInternalCallbackCenter->getZegoExpressCopyrightedMusicImp();

        if (copyrightedMusic) {
            copyrightedMusic->zego_on_copyrighted_music_request_accompaniment_clip(seq, error_code,
                                                                                   resource);
        }
    }

    static void zego_on_copyrighted_music_get_music_by_token(zego_seq seq, zego_error error_code,
                                                             const char *resource,
                                                             void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto copyrightedMusic = oInternalCallbackCenter->getZegoExpressCopyrightedMusicImp();

        if (copyrightedMusic) {
            copyrightedMusic->zego_on_copyrighted_music_get_music_by_token(seq, error_code,
                                                                           resource);
        }
    }

    static void zego_on_copyrighted_music_request_resource(zego_seq seq, zego_error error_code,
                                                           const char *resource,
                                                           void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto copyrightedMusic = oInternalCallbackCenter->getZegoExpressCopyrightedMusicImp();

        if (copyrightedMusic) {
            copyrightedMusic->zego_on_copyrighted_music_request_resource(seq, error_code, resource);
        }
    }

    static void zego_on_copyrighted_music_get_shared_resource(zego_seq seq, zego_error error_code,
                                                              const char *resource,
                                                              void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto copyrightedMusic = oInternalCallbackCenter->getZegoExpressCopyrightedMusicImp();

        if (copyrightedMusic) {
            copyrightedMusic->zego_on_copyrighted_music_get_shared_resource(seq, error_code,
                                                                            resource);
        }
    }

    static void zego_on_copyrighted_music_download(zego_seq seq, zego_error error_code,
                                                   void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto copyrightedMusic = oInternalCallbackCenter->getZegoExpressCopyrightedMusicImp();

        if (copyrightedMusic) {
            copyrightedMusic->zego_on_copyrighted_music_download(seq, error_code);
        }
    }

    static void zego_on_copyrighted_music_get_standard_pitch(zego_seq seq, zego_error error_code,
                                                             const char *pitch,
                                                             void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto copyrightedMusic = oInternalCallbackCenter->getZegoExpressCopyrightedMusicImp();

        if (copyrightedMusic) {
            copyrightedMusic->zego_on_copyrighted_music_get_standard_pitch(seq, error_code, pitch);
        }
    }

    static void
    zego_on_screen_capture_source_available_frame(const void *data, unsigned int data_length,
                                                  const struct zego_video_frame_param param,
                                                  int instance_index, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto screenCaptureSource =
            oInternalCallbackCenter->getZegoExpressScreenCaptureSourceImp(instance_index);

        if (screenCaptureSource) {
            screenCaptureSource->zego_on_screen_capture_source_available_frame(data, data_length,
                                                                               param);
        }
    }

    static void zego_on_screen_capture_source_exception_occurred(
        enum zego_screen_capture_source_exception_type exception_type, int instance_index,
        void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto screenCaptureSource =
            oInternalCallbackCenter->getZegoExpressScreenCaptureSourceImp(instance_index);

        if (screenCaptureSource) {
            screenCaptureSource->zego_on_screen_capture_source_exception_occurred(exception_type);
        }
    }

    static void zego_on_screen_capture_source_window_state_changed(
        enum zego_screen_capture_window_state window_state, zego_rect rect, int instance_index,
        void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto screenCaptureSource =
            oInternalCallbackCenter->getZegoExpressScreenCaptureSourceImp(instance_index);

        if (screenCaptureSource) {
            screenCaptureSource->zego_on_screen_capture_source_window_state_changed(window_state,
                                                                                    rect);
        }
    }

    static void zego_on_screen_capture_source_rect_changed(zego_rect rect, int instance_index,
                                                           void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto screenCaptureSource =
            oInternalCallbackCenter->getZegoExpressScreenCaptureSourceImp(instance_index);

        if (screenCaptureSource) {
            screenCaptureSource->zego_on_screen_capture_source_rect_changed(rect);
        }
    }

    static void zego_on_network_time_synchronized(void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);
        auto weakHandler =
            std::weak_ptr<IZegoEventHandler>(oInternalCallbackCenter->getIZegoEventHandler());

        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler) {
            handler->onNetworkTimeSynchronized();
        }
        ZEGO_SWITCH_THREAD_ING
    }
    static void zego_on_media_data_publisher_file_open(const char *path,
                                                       zego_instance instance_index,
                                                       void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto media_data_publisher_ins =
            oInternalCallbackCenter->getZegoExpressMediaDataPublisherImp(instance_index);

        if (media_data_publisher_ins) {
            media_data_publisher_ins->zego_on_media_data_publisher_file_open(path, instance_index);
        }
    }

    static void zego_on_media_data_publisher_file_close(zego_error error_code, const char *path,
                                                        zego_instance instance_index,
                                                        void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto media_data_publisher_ins =
            oInternalCallbackCenter->getZegoExpressMediaDataPublisherImp(instance_index);
        if (media_data_publisher_ins) {
            media_data_publisher_ins->zego_on_media_data_publisher_file_close(error_code, path,
                                                                              instance_index);
        }
    }

    static void zego_on_media_data_publisher_file_data_begin(const char *path,
                                                             zego_instance instance_index,
                                                             void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto media_data_publisher_ins =
            oInternalCallbackCenter->getZegoExpressMediaDataPublisherImp(instance_index);
        if (media_data_publisher_ins) {
            media_data_publisher_ins->zego_on_media_data_publisher_file_data_begin(path,
                                                                                   instance_index);
        }
    }

    static void zego_on_media_data_publisher_file_data_end(const char *path,
                                                           zego_instance instance_index,
                                                           void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto media_data_publisher_ins =
            oInternalCallbackCenter->getZegoExpressMediaDataPublisherImp(instance_index);
        if (media_data_publisher_ins) {
            media_data_publisher_ins->zego_on_media_data_publisher_file_data_end(path,
                                                                                 instance_index);
        }
    }

    static void zego_on_range_scene_scene_state_update(int range_scene_handle,
                                                       enum zego_scene_state state, int error_code,
                                                       void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_scene_state_update(state, error_code);
        }
    }

    static void zego_on_range_scene_enter_view(int range_scene_handle, struct zego_user user,
                                               struct zego_position position, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_enter_view(user, position);
        }
    }

    static void zego_on_range_scene_leave_view(int range_scene_handle, const char *userid,
                                               void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_leave_view(userid);
        }
    }

    static void zego_on_range_scene_user_status_update(int range_scene_handle, const char *userid,
                                                       struct zego_position position,
                                                       unsigned int channel,
                                                       const unsigned char *status,
                                                       unsigned int status_length,
                                                       void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_user_status_update(userid, position, channel, status,
                                                                status_length);
        }
    }

    static void zego_on_range_scene_user_command_update(int range_scene_handle, const char *userid,
                                                        struct zego_position position,
                                                        unsigned int channel,
                                                        const unsigned char *command,
                                                        unsigned int command_length,
                                                        void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_user_command_update(userid, position, channel, command,
                                                                 command_length);
        }
    }

    static void zego_on_range_scene_custom_command_update(int range_scene_handle,
                                                          const unsigned char *command,
                                                          unsigned int command_length,
                                                          void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_custom_command_update(command, command_length);
        }
    }

    static void zego_on_range_scene_token_will_expire(int range_scene_handle,
                                                      int remain_time_in_second,
                                                      void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_token_will_expire(remain_time_in_second);
        }
    }

    static void zego_on_range_scene_login_scene(int range_scene_handle, zego_seq seq,
                                                int error_code, struct zego_scene_config config,
                                                void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_login_scene(seq, error_code, config);
        }
    }

    static void zego_on_range_scene_logout_scene(int range_scene_handle, zego_seq seq,
                                                 int error_code, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_logout_scene(seq, error_code);
        }
    }

    static void zego_on_range_scene_get_user_count(int range_scene_handle, zego_seq seq,
                                                   int error_code, unsigned int count,
                                                   void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_get_user_count(seq, error_code, count);
        }
    }

    static void zego_on_range_scene_get_user_list_in_view(int range_scene_handle, zego_seq seq,
                                                          int error_code, const char **user_list,
                                                          unsigned int user_list_size,
                                                          void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_get_user_list_in_view(seq, error_code, user_list,
                                                                   user_list_size);
        }
    }

    static void zego_on_range_scene_send_custom_command(int range_scene_handle, zego_seq seq,
                                                        int error_code, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_send_custom_command(seq, error_code);
        }
    }

    static void zego_on_range_scene_stream_user_stream_state_update(int range_scene_handle,
                                                                    const char *userid,
                                                                    const char *stream_id,
                                                                    enum zego_stream_state state,
                                                                    void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_stream_user_stream_state_update(userid, stream_id,
                                                                             state);
        }
    }

    static void zego_on_range_scene_stream_user_mic_update(int range_scene_handle,
                                                           const char *userid,
                                                           enum zego_device_state state,
                                                           void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_stream_user_mic_update(userid, state);
        }
    }

    static void zego_on_range_scene_stream_user_camera_update(int range_scene_handle,
                                                              const char *userid,
                                                              enum zego_device_state state,
                                                              void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_stream_user_camera_update(userid, state);
        }
    }

    static void zego_on_range_scene_stream_user_speaker_update(int range_scene_handle,
                                                               const char *userid,
                                                               enum zego_device_state state,
                                                               void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_stream_user_speaker_update(userid, state);
        }
    }

    static void zego_on_range_scene_team_team_state_update(int range_scene_handle,
                                                           unsigned int team_id,
                                                           enum zego_team_state state,
                                                           zego_error error_code,
                                                           void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_team_team_state_update(team_id, state, error_code);
        }
    }

    static void zego_on_range_scene_team_team_member_update(
        int range_scene_handle, unsigned int team_id, enum zego_update_type update_type,
        const struct zego_user *user_list, unsigned int user_count, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_team_team_member_update(team_id, update_type,
                                                                     user_list, user_count);
        }
    }

    static void zego_on_range_scene_team_join_team(int range_scene_handle, zego_seq seq,
                                                   int error_code, unsigned int team_id,
                                                   void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_team_join_team(seq, error_code, team_id);
        }
    }

    static void zego_on_range_scene_team_leave_team(int range_scene_handle, zego_seq seq,
                                                    int error_code, unsigned int team_id,
                                                    void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_team_leave_team(seq, error_code, team_id);
        }
    }

    static void zego_on_range_scene_item_item_enter_view(int range_scene_handle, long long item_id,
                                                         unsigned int capacity,
                                                         struct zego_position position,
                                                         const char **user_list,
                                                         unsigned int user_list_size,
                                                         void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_item_item_enter_view(item_id, capacity, position,
                                                                  user_list, user_list_size);
        }
    }

    static void zego_on_range_scene_item_item_leave_view(int range_scene_handle, long long item_id,
                                                         void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_item_item_leave_view(item_id);
        }
    }

    static void zego_on_range_scene_item_item_bind_update(int range_scene_handle, long long item_id,
                                                          const char *userid, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_item_item_bind_update(item_id, userid);
        }
    }

    static void zego_on_range_scene_item_item_unbind_update(int range_scene_handle,
                                                            long long item_id,
                                                            const char **user_list,
                                                            unsigned int user_list_size,
                                                            void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_item_item_unbind_update(item_id, user_list,
                                                                     user_list_size);
        }
    }

    static void
    zego_on_range_scene_item_item_status_update(int range_scene_handle, long long item_id,
                                                struct zego_position position, unsigned int channel,
                                                const unsigned char *status,
                                                unsigned int status_length, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_item_item_status_update(item_id, position, channel,
                                                                     status, status_length);
        }
    }

    static void
    zego_on_range_scene_item_item_command_update(int range_scene_handle, long long item_id,
                                                 struct zego_position position,
                                                 unsigned int channel, const unsigned char *command,
                                                 unsigned int command_length, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_item_item_command_update(item_id, position, channel,
                                                                      command, command_length);
        }
    }

    static void zego_on_range_scene_item_create_item(int range_scene_handle, zego_seq seq,
                                                     int error_code, long long item_id,
                                                     void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_item_create_item(seq, error_code, item_id);
        }
    }

    static void zego_on_range_scene_item_destroy_item(int range_scene_handle, zego_seq seq,
                                                      int error_code, long long item_id,
                                                      void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_item_destroy_item(seq, error_code, item_id);
        }
    }

    static void zego_on_range_scene_item_bind_item(int range_scene_handle, zego_seq seq,
                                                   int error_code, long long item_id,
                                                   void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_item_bind_item(seq, error_code, item_id);
        }
    }

    static void zego_on_range_scene_item_unbind_item(int range_scene_handle, zego_seq seq,
                                                     int error_code, long long item_id,
                                                     void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_item_unbind_item(seq, error_code, item_id);
        }
    }

    static void zego_on_range_scene_item_update_item_status(int range_scene_handle, zego_seq seq,
                                                            int error_code, long long item_id,
                                                            void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_item_update_item_status(seq, error_code, item_id);
        }
    }

    static void zego_on_range_scene_item_update_item_command(int range_scene_handle, zego_seq seq,
                                                             int error_code, long long item_id,
                                                             void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto range_scene = oInternalCallbackCenter->getZegoExpressRangeSceneImp(range_scene_handle);
        if (range_scene) {
            range_scene->zego_on_range_scene_item_update_item_command(seq, error_code, item_id);
        }
    }

    static void
    zego_on_video_object_segmentation_state_changed(enum zego_object_segmentation_state state,
                                                    enum zego_publish_channel channel,
                                                    int error_code, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto eventHandler = oInternalCallbackCenter->getIZegoEventHandler();
        auto weakHandler = std::weak_ptr<IZegoEventHandler>(eventHandler);

        ZEGO_SWITCH_THREAD_PRE_STATIC
        auto handler = weakHandler.lock();
        if (handler) {
            handler->onVideoObjectSegmentationStateChanged((ZegoObjectSegmentationState)state,
                                                           ZegoPublishChannel(channel), error_code);
        }
        ZEGO_SWITCH_THREAD_ING
    }

    static void zego_on_ai_voice_changer_init(zego_error error_code, int instance_index,
                                              void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto ai_voice_changer =
            oInternalCallbackCenter->getZegoExpressAIVoiceChangerImpl(instance_index);
        if (ai_voice_changer) {
            ai_voice_changer->zego_on_ai_voice_changer_init(error_code);
        }
    }

    static void zego_on_ai_voice_changer_update_progress(double percent, int file_index,
                                                         int file_count, int instance_index,
                                                         void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto ai_voice_changer =
            oInternalCallbackCenter->getZegoExpressAIVoiceChangerImpl(instance_index);
        if (ai_voice_changer) {
            ai_voice_changer->zego_on_ai_voice_changer_update_progress(percent, file_index,
                                                                       file_count);
        }
    }

    static void zego_on_ai_voice_changer_update(zego_error error_code, int instance_index,
                                                void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto ai_voice_changer =
            oInternalCallbackCenter->getZegoExpressAIVoiceChangerImpl(instance_index);
        if (ai_voice_changer) {
            ai_voice_changer->zego_on_ai_voice_changer_update(error_code);
        }
    }

    static void zego_on_ai_voice_changer_get_speaker_list(
        zego_error error_code, const struct zego_ai_voice_changer_speaker_info *speaker_list,
        unsigned int speaker_count, int instance_index, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto ai_voice_changer =
            oInternalCallbackCenter->getZegoExpressAIVoiceChangerImpl(instance_index);
        if (ai_voice_changer) {
            ai_voice_changer->zego_on_ai_voice_changer_get_speaker_list(error_code, speaker_list,
                                                                        speaker_count);
        }
    }

    static void zego_on_ai_voice_changer_event(enum zego_ai_voice_changer_event event,
                                               int instance_index, void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto ai_voice_changer =
            oInternalCallbackCenter->getZegoExpressAIVoiceChangerImpl(instance_index);
        if (ai_voice_changer) {
            ai_voice_changer->zego_on_ai_voice_changer_event((ZegoAIVoiceChangerEvent)event);
        }
    }

    static void zego_on_ai_voice_changer_set_speaker(zego_error error_code, int instance_index,
                                                     void *user_context) {
        ZEGO_UNUSED_VARIABLE(user_context);

        auto ai_voice_changer =
            oInternalCallbackCenter->getZegoExpressAIVoiceChangerImpl(instance_index);
        if (ai_voice_changer) {
            ai_voice_changer->zego_on_ai_voice_changer_set_speaker(error_code);
        }
    }
};

} // namespace EXPRESS
} // namespace ZEGO

ZEGO_ENABLE_DEPRECATION_WARNINGS
