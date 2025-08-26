#pragma once
#include <atomic>
#include <string>

#include "./ZegoInternalPrivate.h"
#include "./include/zego-express-ai-voice-changer.h"
#include "./include/zego-express-audio-effect-player.h"
#include "./include/zego-express-audio-vad-client.h"
#include "./include/zego-express-copyrighted-music.h"
#include "./include/zego-express-custom-audio-io.h"
#include "./include/zego-express-custom-video-io.h"
#include "./include/zego-express-device.h"
#include "./include/zego-express-engine.h"
#include "./include/zego-express-errcode.h"
#include "./include/zego-express-im.h"
#include "./include/zego-express-media-data-publisher.h"
#include "./include/zego-express-mediaplayer.h"
#include "./include/zego-express-mixer.h"
#include "./include/zego-express-player.h"
#include "./include/zego-express-preprocess.h"
#include "./include/zego-express-publisher.h"
#include "./include/zego-express-range-audio.h"
#include "./include/zego-express-range-scene-item.h"
#include "./include/zego-express-range-scene-stream.h"
#include "./include/zego-express-range-scene-team.h"
#include "./include/zego-express-range-scene.h"
#include "./include/zego-express-record.h"
#include "./include/zego-express-room.h"
#include "./include/zego-express-screen-capture.h"
#include "./include/zego-express-utilities.h"

#include "./ZegoInternalExplicit.hpp"

ZEGO_DISABLE_DEPRECATION_WARNINGS

namespace ZEGO {
namespace EXPRESS {

class ZegoExpressEngineBridge {
    class ZegoExpressEngineBridgePri {
      public:
        void init() {}
    };

  public:
    static ZegoExpressEngineBridge *GetInstance() {
        static ZegoExpressEngineBridge oInstance;
        return &oInstance;
    }

    const char *getVersion() {
        const char *version;
        zego_express_get_version(&version);
        return version;
    }

    bool getLibraryReady() { return library_ready; }

    void setAndroidEnv(void *jvm, void *ctx) { zego_express_set_android_env(jvm, ctx); }

    void setOhosEnv(void *env, void *exports) { zego_express_set_ohos_env(env, exports); }

    void setEngineConfig(zego_engine_config config) { zego_express_set_engine_config(config); }

    void setRoomMode(zego_room_mode mode) { zego_express_set_room_mode(mode); }

    void setGeoFence(ZegoGeoFenceType type, std::vector<int> areaList) {
        zego_geo_fence_type geo_fence_type = static_cast<zego_geo_fence_type>(type);
        zego_express_set_geo_fence(geo_fence_type, areaList.data(), areaList.size());
    }

    bool isFeatureSupported(zego_feature_type featureType) {
        bool supported = false;
        zego_express_is_feature_supported(featureType, &supported);
        return supported;
    }

    void setLogConfig(zego_log_config config) { zego_express_set_log_config(config); }

    void setLocalProxyConfig(zego_proxy_info *proxyList, int count, bool enable) {
        zego_express_set_local_proxy_config(proxyList, count, enable);
    }

    void setCloudProxyConfig(zego_proxy_info *proxyList, int count, const char *token,
                             bool enable) {
        zego_express_set_cloud_proxy_config(proxyList, count, token, enable);
    }

    void setLicense(const char *license) { zego_express_set_license(license); }

    int loadLibrary(const std::string &sdk_library_full_path) {
        int error_code = 0;
#ifdef ZEGOEXP_EXPLICIT
        error_code = loadLibraryInternal(sdk_library_full_path);
        library_ready = (error_code == 0);
#else
        error_code = ZEGO_ERRCODE_COMMON_LOAD_LIBRARY_NOT_SUPPORT;
#endif
        return error_code;
    }

    void unLoadLibrary() {
#ifdef ZEGOEXP_EXPLICIT
        unLoadLibraryInternal();
        library_ready = false;
#endif
    }

    // void setAdaptiveVideoConfig(zego_adaptive_video_config *config) {
    //     zego_express_set_adaptive_video_config(config);
    // }

    void apiCallResult(const char *func, int error_code) {
        zego_express_handle_api_call_result(func, error_code);
    }

    int init(unsigned int appID, const char *appSign, bool isTestEnvironment,
             zego_scenario scenario) {
        // reset the locals
        pri->init();

        // set platform
        zego_express_set_platform_language(zego_platform_language_cpp);

        int result = zego_express_engine_init(appID, appSign, isTestEnvironment, scenario);
        return result;
    }

#if defined(_WIN32) || TARGET_OS_OSX
    bool isCallbackSwitchToMainThread() { return callback_switch_to_main_thread_; }
#endif

    int init(const ZegoEngineProfile &profile) {
        // reset the locals
        pri->init();

        // set platform
        zego_express_set_platform_language(zego_platform_language_cpp);

        // do init
        zego_engine_profile config;
        config.app_id = profile.appID;
        if (!profile.appSign.empty()) {
            strncpy(config.app_sign, profile.appSign.c_str(), ZEGO_EXPRESS_MAX_APPSIGN_LEN);
        } else {
            memset(config.app_sign, 0, ZEGO_EXPRESS_MAX_APPSIGN_LEN);
        }
        config.scenario = static_cast<zego_scenario>(profile.scenario);

        int result = zego_express_engine_init_with_profile(config);

        if (result == 0) {
#if defined(_WIN32) || TARGET_OS_OSX
            callback_switch_to_main_thread_ = profile.callbackSwitchToMainThread;
#endif
        }

        return result;
    }

    void uninitAsync() { zego_express_engine_uninit_async(); }

    std::string callExperimentalAPI(const std::string &params) {
        char *temp_result = nullptr;
        zego_express_call_experimental_api(params.c_str(), &temp_result);
        std::string result = temp_result ? temp_result : "";
        zego_express_free_call_experimental_api_result(temp_result);
        return result;
    }

    zego_seq getIncreaseSeq() { return zego_express_get_increase_seq(); }

    void setDummyCaptureImagePath(const std::string &filePath,
                                  ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) {
        zego_express_set_dummy_capture_image_path(filePath.c_str(), (zego_publish_channel)channel);
    }

    void setDummyCaptureImageParams(const zego_dummy_capture_image_params &params,
                                    ZegoPublishChannel channel) {
        zego_express_set_dummy_capture_image_params(params, (zego_publish_channel)channel);
    }

    int uploadLog() {
        int seq = 0;
        zego_express_upload_log(&seq);
        return seq;
    }

    void enableDebugAssistant(bool enable) { zego_express_enable_debug_assistant(enable); }

    void setRoomScenario(zego_scenario scenario) { zego_express_set_room_scenario(scenario); }

    void loginRoom(const char *room_id, zego_user user, zego_room_config *room_config) {
        zego_express_login_room(room_id, user, room_config);
    }

    int loginRoomWithCallback(const char *room_id, zego_user user, zego_room_config *room_config) {
        int seq = 0;
        zego_express_login_room_with_callback(room_id, user, room_config, &seq);
        return seq;
    }

    void logoutRoom() { zego_express_logout_all_room(); }

    void logoutRoom(const char *room_id) { zego_express_logout_room(room_id); }

    int logoutRoomWithCallback() {
        int seq = 0;
        zego_express_logout_all_room_with_callback(&seq);
        return seq;
    }

    int logoutRoomWithCallback(const char *room_id) {
        int seq = 0;
        zego_express_logout_room_with_callback(room_id, &seq);
        return seq;
    }

    void switchRoom(const char *from_room_id, const char *to_room_id,
                    zego_room_config *room_config) {
        zego_express_switch_room(from_room_id, to_room_id, room_config);
    }

    void renewToken(const char *room_id, const char *token) {
        zego_express_renew_token(room_id, token);
    }

    struct zego_room_stream_list *getRoomStreamList(const char *room_id,
                                                    zego_room_stream_list_type stream_list_type) {

        struct zego_room_stream_list *stream_list = nullptr;
        zego_express_get_room_stream_list(room_id, stream_list_type, &stream_list);
        return stream_list;
    }

    void freeRoomStreamList(struct zego_room_stream_list *stream_list) {
        if (nullptr == stream_list)
            return;

        zego_express_free_room_stream_list(stream_list);
    }

    int setRoomExtraInfo(const char *extraInfo, const char *key, const char *value) {
        int seq = 0;
        zego_express_set_room_extra_info(extraInfo, key, value, &seq);
        return seq;
    }

    void setVideoMirrorMode(zego_video_mirror_mode mirrorMode, zego_publish_channel channel) {
        zego_express_set_video_mirror_mode(mirrorMode, channel);
    }

    void startPreview(zego_canvas *canvas, zego_publish_channel channel) {
        zego_express_start_preview(canvas, channel);
    }

    void stopPreview(zego_publish_channel channel) { zego_express_stop_preview(channel); }

    void setAudioConfig(zego_audio_config audioConfig, zego_publish_channel channel) {
        zego_express_set_audio_config_by_channel(audioConfig, channel);
    }

    zego_audio_config getAudioConfig(zego_publish_channel channel) {
        zego_audio_config config;
        zego_express_get_audio_config_by_channel(channel, &config);
        return config;
    }

    void setVideoConfig(zego_video_config videoConfig, zego_publish_channel channel) {
        zego_express_set_video_config(videoConfig, channel);
    }

    zego_video_config getVideoConfig(zego_publish_channel channel) {
        zego_video_config config;
        zego_express_get_video_config(channel, &config);
        return config;
    }

    void setPublishDualStreamConfig(zego_publish_dual_stream_config *configList,
                                    int configListCount, zego_publish_channel channel) {
        zego_express_set_publish_dual_stream_config(configList, configListCount, channel);
    }

    void startPublishingStream(const char *streamID, zego_publish_channel channel) {
        zego_express_start_publishing_stream(streamID, channel);
    }

    void startPublishingStreamWithConfig(const char *streamID, zego_publish_channel channel,
                                         zego_publisher_config config) {
        zego_express_start_publishing_stream_with_config(streamID, config, channel);
    }

    void startPublishingStreamInScene(const char *streamID, zego_publish_channel channel,
                                      zego_scene_publisher_config config) {
        zego_express_start_publishing_stream_in_scene(streamID, channel, config);
    }

    void stopPublishingStream(zego_publish_channel channel) {
        zego_express_stop_publishing_stream(channel);
    }

    int setStreamExtraInfo(const char *extraInfo, zego_publish_channel channel) {
        int seq = 0;
        zego_express_set_stream_extra_info(extraInfo, channel, &seq);
        return seq;
    }

    void setPublishStreamEncryptionKey(const char *key, zego_publish_channel channel) {
        zego_express_set_publish_stream_encryption_key(key, channel);
    }

    void takePublishStreamSnapshot(zego_publish_channel channel) {
        zego_express_take_publish_stream_snapshot(channel);
    }

    void mutePublishStreamAudio(bool mute, zego_publish_channel channel) {
        zego_express_mute_publish_stream_audio(mute, channel);
    }

    void mutePublishStreamVideo(bool mute, zego_publish_channel channel) {
        zego_express_mute_publish_stream_video(mute, channel);
    }

    void setStreamAlignmentProperty(int alignment, zego_publish_channel channel) {
        zego_express_set_stream_alignment_property(alignment, channel);
    }

    void setCaptureVolume(int volume) { zego_express_set_capture_volume(volume); }

    int getCaptureVolume() {
        int volume = 0;
        zego_express_get_capture_volume(&volume);
        return volume;
    }

    void setAudioCaptureStereoMode(zego_audio_capture_stereo_mode mode) {
        zego_express_set_audio_capture_stereo_mode(mode);
    }

    int addPublishCdnUrl(const char *streamID, const char *targetURL, int timeout) {
        int seq = 0;
        zego_express_add_publish_cdn_url_v2(streamID, targetURL, timeout, &seq);
        return seq;
    }

    int removePublishCdnUrl(const char *streamID, const char *targetURL) {
        int seq = 0;
        zego_express_remove_publish_cdn_url(streamID, targetURL, &seq);
        return seq;
    }

    void enablePublishDirectToCDN(bool enable, zego_cdn_config *config,
                                  zego_publish_channel channel) {
        zego_express_enable_publish_direct_to_cdn(enable, config, channel);
    }

    void setPublishWatermark(zego_watermark *watermark, bool isPreviewVisible,
                             zego_publish_channel channel) {
        zego_express_set_publish_watermark(isPreviewVisible, watermark, channel);
    }

    void enableHardwareEncoder(bool enable) { zego_express_enable_hardware_encoder(enable); }

    void setCapturePipelineScaleMode(zego_capture_pipeline_scale_mode mode) {
        zego_express_set_capture_pipeline_scale_mode(mode);
    }

    void enableH265EncodeFallback(bool enable) {
        zego_express_enable_h_265_encode_fallback(enable);
    }

    int isVideoEncoderSupported(zego_video_codec_id codecID,
                                zego_video_codec_backend codecBackend) {
        int ret = 0;
        zego_express_is_video_encoder_supported(codecID, codecBackend, &ret);
        return ret;
    }

    void enableAuxBgmBalance(bool enable) { zego_express_enable_aux_bgm_balance(enable); }

    void setSEIConfig(zego_sei_config config) { zego_express_set_sei_config(config); }

    void sendSEI(const unsigned char *buffer, unsigned int buffer_length,
                 zego_publish_channel channel) {
        zego_express_send_sei(buffer, buffer_length, channel);
    }

    void sendSEISyncWithCustomVideo(const unsigned char *data, unsigned int dataLength,
                                    unsigned long long timeStampNs, zego_publish_channel channel) {
        zego_express_send_sei_sync_with_custom_video(data, dataLength, timeStampNs, channel);
    }

    void sendAudioSideInfo(const unsigned char *data, unsigned int dataLength, double timeStampMs,
                           zego_publish_channel channel) {
        zego_express_send_audio_side_info(data, dataLength, timeStampMs, channel);
    }

    void enableTrafficControl(bool enable, int property, zego_publish_channel channel) {
        zego_express_enable_traffic_control_by_channel(enable, property, channel);
    }

    void setMinVideoBitrateForTrafficControl(int bitrate,
                                             zego_traffic_control_min_video_bitrate_mode mode,
                                             zego_publish_channel channel) {
        zego_express_set_min_video_bitrate_for_traffic_control_by_channel(bitrate, mode, channel);
    }

    void setMinVideoFpsForTrafficControl(int fps, zego_publish_channel channel) {
        zego_express_set_min_video_fps_for_traffic_control_by_channel(fps, channel);
    }

    void setMinVideoResolutionForTrafficControl(int width, int height,
                                                zego_publish_channel channel) {
        zego_express_set_min_video_resolution_for_traffic_control_by_channel(width, height,
                                                                             channel);
    }

    void setTrafficControlFocusOn(zego_traffic_control_focus_on_mode mode,
                                  zego_publish_channel channel) {
        zego_express_set_traffic_control_focus_on_by_channel(mode, channel);
    }

    void setAppOrientation(zego_orientation orientation, zego_publish_channel channel) {
        zego_express_set_app_orientation(orientation, channel);
    }

    void setAppOrientationMode(zego_orientation_mode mode) {
        zego_express_set_app_orientation_mode(mode);
    }

    void startPlayingStream(const char *streamID, zego_canvas *canvas) {
        zego_express_start_playing_stream(streamID, canvas);
    }

    void startPlayingStreamWithConfig(const char *streamID, zego_canvas *canvas,
                                      zego_player_config config) {
        zego_express_start_playing_stream_with_config(streamID, canvas, config);
    }

    void startPlayingStreamInScene(const char *streamID, zego_canvas *canvas,
                                   zego_scene_player_config config) {
        zego_express_start_playing_stream_in_scene(streamID, canvas, config);
    }

    void switchPlayingStream(const char *fromStreamID, const char *toStreamID,
                             const zego_player_config *config) {
        zego_express_switch_playing_stream(fromStreamID, toStreamID, *config);
    }

    void stopPlayingStream(const char *streamID) { zego_express_stop_playing_stream(streamID); }

    void setPlayStreamDecryptionKey(const char *streamID, const char *key) {
        zego_express_set_play_stream_decryption_key(streamID, key);
    }

    void setPlayStreamCrossAppInfo(const char *streamID, struct zego_cross_app_info info) {
        zego_express_set_play_stream_cross_app_info(streamID, info);
    }

    void takePlayStreamSnapshot(const char *streamID) {
        zego_express_take_play_stream_snapshot(streamID);
    }

    void setPlayVolume(const char *streamID, int volume) {
        zego_express_set_play_volume(streamID, volume);
    }

    void setAllPlayStreamVolume(int volume) { zego_express_set_all_play_stream_volume(volume); }

    void setPlayStreamVideoType(const char *streamID, zego_video_stream_type type) {
        zego_express_set_play_stream_video_type(streamID, type);
    }

    void setPlayStreamBufferIntervalRange(const char *streamID, unsigned int minBufferInterval,
                                          unsigned int maxBufferInterval) {
        zego_express_set_play_stream_buffer_interval_range(streamID, minBufferInterval,
                                                           maxBufferInterval);
    }

    void setAudioMixMode(zego_audio_mix_mode mode, const char **streamList, int num) {
        zego_express_set_audio_mix_mode(mode, streamList, num);
    }

    void setPlayStreamFocusOn(const char *streamID) {
        zego_express_set_play_stream_focus_on(streamID);
    }

    void mutePlayStreamAudio(const char *streamID, bool mute) {
        zego_express_mute_play_stream_audio(streamID, mute);
    }

    void mutePlayStreamVideo(const char *streamID, bool mute) {
        zego_express_mute_play_stream_video(streamID, mute);
    }

    void muteAllPlayStreamAudio(bool mute) { zego_express_mute_all_play_stream_audio(mute); }

    void muteAllPlayAudioStreams(bool mute) { zego_express_mute_all_play_audio_streams(mute); }

    void muteAllPlayStreamVideo(bool mute) { zego_express_mute_all_play_stream_video(mute); }

    void muteAllPlayVideoStreams(bool mute) { zego_express_mute_all_play_video_streams(mute); }

    void enableHardwareDecoder(bool enable) { zego_express_enable_hardware_decoder(enable); }

    void enableCheckPoc(bool enable) { zego_express_enable_check_poc(enable); }

    int isVideoDecoderSupported(zego_video_codec_id codecID,
                                zego_video_codec_backend codec_backend) {
        int seq = 0;
        zego_express_is_video_decoder_supported(codecID, codec_backend, &seq);
        return seq;
    }

    void setLowlightEnhancement(zego_low_light_enhancement_mode mode,
                                zego_publish_channel channel) {
        zego_express_set_low_light_enhancement(mode, channel);
    }

    void setLowlightEnhancementParams(const zego_exp_low_light_enhancement_params &params,
                                      zego_publish_channel channel) {
        zego_express_set_low_light_enhancement_params(params, channel);
    }

    zego_error setVideoSource(zego_video_source_type source, unsigned int instanceID,
                              zego_publish_channel channel) {
        return zego_express_set_video_source(source, instanceID, channel);
    }

    zego_error setAudioSource(zego_audio_source_type source, zego_publish_channel channel) {
        return zego_express_set_audio_source(source, channel);
    }

    zego_error setAudioSource(zego_audio_source_type source,
                              struct zego_audio_source_mix_config config) {
        return zego_express_set_audio_source_with_config(source, config);
    }

    void setPlayStreamsAlignmentProperty(zego_stream_alignment_mode mode) {
        zego_express_set_play_streams_alignment_property(mode);
    }

    void muteMicrophone(bool mute) { zego_express_mute_microphone(mute); }

    bool isMicrophoneMuted() {
        bool is_mute = false;
        zego_express_is_microphone_muted(&is_mute);
        return is_mute;
    }

    void muteSpeaker(bool mute) { zego_express_mute_speaker(mute); }

    bool isSpeakerMuted() {
        bool is_mute = false;
        zego_express_is_speaker_muted(&is_mute);
        return is_mute;
    }

    void enableCamera(bool enable, zego_publish_channel channel) {
        zego_express_enable_camera(enable, zego_exp_notify_device_state_mode_open, channel);
    }

    void enableAudioCaptureDevice(bool enable) { zego_express_enable_audio_capture_device(enable); }

    zego_audio_route getAudioRouteType() {
        zego_audio_route route;
        zego_express_get_audio_route_type(&route);
        return route;
    }

    void setAudioRouteToSpeaker(bool defaultToSpeaker) {
        zego_express_set_audio_route_to_speaker(defaultToSpeaker);
    }

    void useFrontCamera(bool enable, zego_publish_channel channel) {
        zego_express_use_front_camera(enable, channel);
    }

    bool isCameraFocusSupported(zego_publish_channel channel) {
        bool ret = false;
        zego_express_is_camera_focus_supported(channel, &ret);
        return ret;
    }

    void setCameraFocusMode(zego_camera_focus_mode mode, zego_publish_channel channel) {
        zego_express_set_camera_focus_mode(mode, channel);
    }

    void setCameraFocusPointInPreview(float x, float y, zego_publish_channel channel) {
        zego_express_set_camera_focus_point_in_preview(x, y, channel);
    }

    void setCameraExposureMode(zego_camera_exposure_mode mode, zego_publish_channel channel) {
        zego_express_set_camera_exposure_mode(mode, channel);
    }

    void setCameraExposurePointInPreview(float x, float y, zego_publish_channel channel) {
        zego_express_set_camera_exposure_point_in_preview(x, y, channel);
    }

    void setCameraExposureCompensation(float value, zego_publish_channel channel) {
        zego_express_set_camera_exposure_compensation(value, channel);
    }

    void setCameraZoomFactor(float factor, zego_publish_channel channel) {
        zego_express_set_camera_zoom_factor(factor, channel);
    }

    float getCameraMaxZoomFactor(zego_publish_channel channel) {
        float factor = 0.0;
        zego_express_get_camera_max_zoom_factor(channel, &factor);
        return factor;
    }

    void enableCameraAdaptiveFPS(bool enable, int minFPS, int maxFPS,
                                 zego_publish_channel channel) {
        zego_express_enable_camera_adaptive_fps(enable, minFPS, maxFPS, channel);
    }

    void startSoundLevelMonitor(struct zego_sound_level_config config) {
        zego_express_start_sound_level_monitor_with_config(config);
    }

    void stopSoundLevelMonitor() { zego_express_stop_sound_level_monitor(); }

    void startAudioSpectrumMonitor(unsigned int millisecond) {
        zego_express_start_audio_spectrum_monitor(millisecond);
    }

    void stopAudioSpectrumMonitor() { zego_express_stop_audio_spectrum_monitor(); }

    void startPerformanceMonitor(unsigned int millisecond) {
        zego_express_start_performance_monitor(millisecond);
    }

    void stopPerformanceMonitor() { zego_express_stop_performance_monitor(); }

    void enableHeadphoneMonitor(bool enable) { zego_express_enable_headphone_monitor(enable); }

    void setHeadphoneMonitorVolume(int volume) {
        zego_express_set_headphone_monitor_volume(volume);
    }

    void enableMixSystemPlayout(bool enable) { zego_express_enable_mix_system_playout(enable); }

    void setMixSystemPlayoutVolume(int volume) {
        zego_express_set_mix_system_playout_volume(volume);
    }

    void startAudioVADStableStateMonitor(ZegoAudioVADStableStateMonitorType type, int millisecond) {
        zego_audio_vad_stable_state_monitor_type monitor_type =
            static_cast<zego_audio_vad_stable_state_monitor_type>(type);
        zego_express_start_audio_vad_stable_state_monitor(monitor_type, millisecond);
    }

    void stopAudioVADStableStateMonitor(ZegoAudioVADStableStateMonitorType type) {
        zego_audio_vad_stable_state_monitor_type monitor_type =
            static_cast<zego_audio_vad_stable_state_monitor_type>(type);
        zego_express_stop_audio_vad_stable_state_monitor(monitor_type);
    }

    void enableMixEnginePlayout(bool enable) { zego_express_enable_mix_engine_playout(enable); }

    void useAudioDevice(zego_audio_device_type deviceType, const char *deviceID) {
        zego_express_use_audio_device(deviceType, deviceID);
    }

    int getAudioDeviceVolume(zego_audio_device_type deviceType, const char *deviceID) {
        int volume = 0;
        zego_express_get_audio_device_volume(deviceType, deviceID, &volume);
        return volume;
    }

    void setAudioDeviceVolume(zego_audio_device_type deviceType, const char *deviceID, int volume) {
        zego_express_set_audio_device_volume(deviceType, deviceID, volume);
    }

    void setSpeakerVolumeInAPP(const char *deviceID, int volume) {
        zego_express_set_speaker_volume_in_app(deviceID, volume);
    }

    int getSpeakerVolumeInAPP(const char *deviceID) {
        return zego_express_get_speaker_volume_in_app(deviceID);
    }

    void startAudioDeviceVolumeMonitor(enum zego_audio_device_type device_type,
                                       const char *device_id) {
        zego_express_start_audio_device_volume_monitor(device_type, device_id);
    }

    void stopAudioDeviceVolumeMonitor(enum zego_audio_device_type device_type,
                                      const char *device_id) {
        zego_express_stop_audio_device_volume_monitor(device_type, device_id);
    }

    void muteAudioDevice(enum zego_audio_device_type device_type, const char *device_id,
                         bool mute) {
        zego_express_mute_audio_device(device_type, device_id, mute);
    }

    void setAudioDeviceMode(enum zego_audio_device_mode mode) {
        zego_express_set_audio_device_mode(mode);
    }

    bool isAudioDeviceMuted(enum zego_audio_device_type device_type, const char *device_id) {
        bool muted = false;
        zego_express_is_audio_device_muted(device_type, device_id, &muted);
        return muted;
    }

    zego_device_info *getAudioDeviceList(zego_audio_device_type device_type, int *device_count) {
        zego_device_info *result = nullptr;
        zego_express_get_audio_device_list(device_type, device_count, &result);
        return result;
    }

    void freeAudioDeviceList(zego_device_info *device_list) {
        zego_express_free_audio_device_list(device_list);
    }

    const char *getDefaultAudioDeviceID(zego_audio_device_type device_type) {
        const char *device_id;
        zego_express_get_default_audio_device_id(device_type, &device_id);
        return device_id;
    }

    const char *getDefaultVideoDeviceID() {
        const char *device_id;
        zego_express_get_default_video_device_id(&device_id);
        return device_id;
    }

    zego_device_info getCurrentAudioDevice(zego_audio_device_type deviceType) {
        zego_device_info info;
        zego_express_get_current_audio_device(deviceType, &info);
        return info;
    }

    void useVideoDevice(const char *deviceID, zego_publish_channel channel) {
        zego_express_use_video_device(deviceID, channel);
    }

    zego_device_info *getVideoDeviceList(int *device_count) {
        zego_device_info *result = nullptr;
        zego_express_get_video_device_list(device_count, &result);
        return result;
    }

    void freeVideoDeviceList(zego_device_info *device_list) {
        zego_express_free_video_device_list(device_list);
    }

    void enableAEC(bool enable) { zego_express_enable_aec(enable); }

    // void enableAEC(bool enable, zego_publish_channel channel) {
    //     zego_express_enable_aec_with_channel(enable, channel);
    // }

    void enableHeadphoneAEC(bool enable) { zego_express_enable_headphone_aec(enable); }

    void setAECMode(zego_aec_mode mode) { zego_express_set_aec_mode(mode); }

    // void setAECMode(zego_aec_mode mode, zego_publish_channel channel) {
    //     zego_express_set_aec_mode_with_channel(mode, channel);
    // }

    void enableAGC(bool enable) { zego_express_enable_agc(enable); }

    // void enableAGC(bool enable, zego_publish_channel channel) {
    //     zego_express_enable_agc_with_channel(enable, channel);
    // }

    void setANSMode(zego_ans_mode mode) { zego_express_set_ans_mode(mode); }

    // void setANSMode(zego_ans_mode mode, zego_publish_channel channel) {
    //     zego_express_set_ans_mode_with_channel(mode, channel);
    // }

    void enableSpeechEnhance(bool enable, int level) {
        zego_express_enable_speech_enhance(enable, level);
    }

    // void enableSpeechEnhance(bool enable, int level, zego_publish_channel channel) {
    //     zego_express_enable_speech_enhance_with_channel(enable, level, channel);
    // }

    void enableANS(bool enable) { zego_express_enable_ans(enable); }

    // void enableANS(bool enable, zego_publish_channel channel) {
    //     zego_express_enable_ans_with_channel(enable, channel);
    // }

    void enableTransientANS(bool enable) { zego_express_enable_transient_ans(enable); }

    void enableAudioMixing(bool enable) { zego_express_enable_audio_mixing(enable); }

    void muteLocalAudioMixing(bool mute) { zego_express_mute_local_audio_mixing(mute); }

    void setAudioMixingVolume(int volume, zego_volume_type type) {
        zego_express_set_audio_mixing_volume_with_type(volume, type);
    }

    void setAudioEqualizerGain(int bandIndex, float bandGain) {
        zego_express_set_audio_equalizer_gain(bandIndex, bandGain);
    }

    void enableBeautify(int feature, zego_publish_channel channel) {
        zego_express_enable_beautify(feature, channel);
    }

    void setBeautifyOption(zego_beautify_option option, zego_publish_channel channel) {
        zego_express_set_beautify_option(option, channel);
    }

    void startEffectsEnv() { zego_express_start_effects_env(); }

    void stopEffectsEnv() { zego_express_stop_effects_env(); }

    void enableEffectsBeauty(bool enable) { zego_express_enable_effects_beauty(enable); }

    void setEffectsBeautyParam(zego_effects_beauty_param param) {
        zego_express_set_effects_beauty_param(param);
    }

    void setVoiceChangerPreset(zego_voice_changer_preset preset) {
        zego_express_set_voice_changer_preset(preset);
    }

    void setVoiceChangerParam(float param) { zego_express_set_voice_changer_param(param); }

    void enableVirtualStereo(bool enable, int angle) {
        zego_express_enable_virtual_stereo(enable, angle);
    }

    void enablePlayStreamVirtualStereo(bool enable, int angle, const char *stream_id) {
        zego_express_enable_play_stream_virtual_stereo(enable, angle, stream_id);
    }

    void setElectronicEffects(bool enable, zego_electronic_effects_mode mode, int tonal) {
        zego_express_set_electronic_effects(enable, mode, tonal);
    }

    void setReverbPreset(zego_reverb_preset preset) { zego_express_set_reverb_preset(preset); }

    void setReverbAdvancedParam(struct zego_reverb_advanced_param param) {
        zego_express_set_reverb_advanced_param(param);
    }

    void setReverbEchoParam(struct zego_reverb_echo_param param) {
        zego_express_set_reverb_echo_param(param);
    }

    int createRealTimeSequentialDataManager(const char *room_id) {
        int instanceIndex = -1;
        zego_express_create_real_time_sequential_data_manager(room_id, &instanceIndex);
        return instanceIndex;
    }

    int destroyRealTimeSequentialDataManager(int instance_index) {
        int result = zego_express_destroy_real_time_sequential_data_manager(instance_index);
        return result;
    }

    int realTimeSequentialDataStartBroadcasting(const char *stream_id, int instance_index) {
        int result =
            zego_express_real_time_sequential_data_start_broadcasting(stream_id, instance_index);
        return result;
    }

    int realTimeSequentialDataStopBroadcasting(const char *stream_id, int instance_index) {
        int result =
            zego_express_real_time_sequential_data_stop_broadcasting(stream_id, instance_index);
        return result;
    }

    int realTimeSequentialDataSendData(const unsigned char *data, unsigned int data_length,
                                       const char *stream_id, int instance_index) {
        int seq = 0;
        zego_express_send_real_time_sequential_data(data, data_length, stream_id, instance_index,
                                                    &seq);
        return seq;
    }

    int realTimeSequentialDataStartSubscribing(const char *stream_id, int_fast16_t instance_index) {
        int result =
            zego_express_real_time_sequential_data_start_subscribing(stream_id, instance_index);
        return result;
    }

    int realTimeSequentialDataStopSubscribing(const char *stream_id, int instance_index) {
        int result =
            zego_express_real_time_sequential_data_stop_subscribing(stream_id, instance_index);
        return result;
    }

    int sendBroadcastMessage(const char *room_id, const char *content) {
        int seq = 0;
        zego_express_send_broadcast_message(room_id, content, &seq);
        return seq;
    }

    int sendBarrageMessage(const char *room_id, const char *content) {
        int seq = 0;
        zego_express_send_barrage_message(room_id, content, &seq);
        return seq;
    }

    int sendCustomCommand(const char *room_id, const char *content, struct zego_user *to_user_list,
                          unsigned int to_user_count) {
        int seq = 0;
        zego_express_send_custom_command(room_id, content, to_user_list, to_user_count, &seq);
        return seq;
    }

    int sendTransparentMessage(const char *room_id,
                               struct zego_room_send_transparent_message *message) {
        int seq = 0;
        zego_express_send_transparent_message(room_id, message, &seq);
        return seq;
    }

    int startMixerTask(zego_mixer_task task) {
        int seq = 0;
        zego_express_start_mixer_task(task, &seq);
        return seq;
    }

    int stopMixerTask(zego_mixer_task task) {
        int seq = 0;
        zego_express_stop_mixer_task(task, &seq);
        return seq;
    }

    int startAutoMixerTask(zego_auto_mixer_task task) {
        int seq = 0;
        zego_express_start_auto_mixer_task(task, &seq);
        return seq;
    }

    int stopAutoMixerTask(zego_auto_mixer_task task) {
        int seq = 0;
        zego_express_stop_auto_mixer_task(task, &seq);
        return seq;
    }

    zego_media_player_instance_index createMediaPlayer() {
        zego_media_player_instance_index instanceIndex = zego_media_player_instance_index_null;
        zego_express_create_media_player(&instanceIndex);
        return instanceIndex;
    }

    int destroyMediaPlayer(zego_media_player_instance_index instance_index) {
        int result = zego_express_destroy_media_player(instance_index);
        return result;
    }

    int mediaPlayerStart(zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_start(instance_index);
        return result;
    }

    int mediaPlayerStop(zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_stop(instance_index);
        return result;
    }

    int mediaPlayerPause(zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_pause(instance_index);
        return result;
    }

    int mediaPlayerResume(zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_resume(instance_index);
        return result;
    }

    int mediaPlayerPreload(const char *path, zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_load_resource(path, instance_index);
        return result;
    }

    int mediaPlayerPreloadWithPosition(const char *path, unsigned long long start_position,
                                       zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_load_resource_with_position(path, start_position,
                                                                           instance_index);
        return result;
    }

    int mediaPlayerPreload(unsigned char *media_data, int media_data_length,
                           unsigned long long start_position,
                           zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_load_resource_from_media_data(
            media_data, media_data_length, start_position, instance_index);
        return result;
    }

    int mediaPlayerPreloadCopyrightedMusicResourceWithPosition(
        const char *recource_id, unsigned long long start_position,
        zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_load_copyrighted_music_resource_with_position(
            recource_id, start_position, instance_index);
        return result;
    }

    int mediaPlayerPreloadResourceWithConfig(struct zego_media_player_resource *resource,
                                             zego_media_player_instance_index instance_index) {
        int seq = zego_express_media_player_load_resource_with_config(resource, instance_index);
        return seq;
    }

    int mediaPlayerSeekTo(unsigned long long millisecond,
                          zego_media_player_instance_index instance_index, zego_seq seq) {
        return zego_express_media_player_seek_to(millisecond, instance_index, &seq);
    }

    int mediaPlayerSetVolume(int volume, zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_set_volume(volume, instance_index);
        return result;
    }

    int mediaPlayerSetPlayVolume(int volume, zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_set_play_volume(volume, instance_index);
        return result;
    }

    int mediaPlayerSetPublishVolume(int volume, zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_set_publish_volume(volume, instance_index);
        return result;
    }

    unsigned long long
    mediaPlayerGetTotalDuration(zego_media_player_instance_index instance_index) {
        unsigned long long totalDuration = 0;
        zego_express_media_player_get_total_duration(instance_index, &totalDuration);
        return totalDuration;
    }

    unsigned long long
    mediaPlayerGetCurrentProgress(zego_media_player_instance_index instance_index) {
        unsigned long long currentProgress = 0;
        zego_express_media_player_get_current_progress(instance_index, &currentProgress);
        return currentProgress;
    }

    unsigned long long
    mediaPlayerGetCurrentRenderingProgress(zego_media_player_instance_index instance_index) {
        unsigned long long currentProgress = 0;
        zego_express_media_player_get_current_rendering_progress(instance_index, &currentProgress);
        return currentProgress;
    }

    int mediaPlayerGetPublishVolume(zego_media_player_instance_index instance_index) {
        int volume = 0;
        zego_express_media_player_get_publish_volume(instance_index, &volume);
        return volume;
    }

    int mediaPlayerGetPlayVolume(zego_media_player_instance_index instance_index) {
        int volume = 0;
        zego_express_media_player_get_play_volume(instance_index, &volume);
        return volume;
    }

    zego_media_player_state
    mediaPlayerGetCurrentState(zego_media_player_instance_index instance_index) {
        zego_media_player_state state;
        zego_express_media_player_get_current_state(instance_index, &state);
        return state;
    }

    unsigned int mediaPlayerGetAudioTrackCount(zego_media_player_instance_index instance_index) {
        unsigned int count = 0;
        zego_express_media_player_get_audio_track_count(instance_index, &count);
        return count;
    }

    int mediaPlayerSetAudioTrackIndex(unsigned int index,
                                      zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_set_audio_track_index(index, instance_index);
        return result;
    }

    int mediaPlayerSetAudioTrackMode(zego_media_player_audio_track_mode mode,
                                     zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_set_audio_track_mode(mode, instance_index);
        return result;
    }

    int mediaPlayerSetAudioTrackPublishIndex(unsigned int index,
                                             zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_set_audio_track_publish_index(index, instance_index);
        return result;
    }

    int mediaPlayerSetVoiceChangerParam(zego_media_player_audio_channel audio_channel, float pitch,
                                        zego_media_player_instance_index instance_index) {
        int result =
            zego_express_media_player_set_voice_changer_param(audio_channel, pitch, instance_index);
        return result;
    }

    int mediaPlayerEnableVoiceChanger(zego_media_player_audio_channel audio_channel, bool enable,
                                      float pitch,
                                      zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_enable_voice_changer(audio_channel, enable, pitch,
                                                                    instance_index);
        return result;
    }

    int takeSnapshot(zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_take_snapshot(instance_index);
        return result;
    }

    int mediaPlayerEnableAccurateSeek(bool enable, struct zego_accurate_seek_config *config,
                                      zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_enable_accurate_seek(enable, config, instance_index);
        return result;
    }

    int mediaPlayerSetNetWorkResourceMaxCache(unsigned int time, unsigned int size,
                                              zego_media_player_instance_index instance_index) {
        int result =
            zego_express_media_player_set_network_resource_max_cache(time, size, instance_index);
        return result;
    }
    int mediaPlayerGetNetWorkResourceCache(struct zego_network_resource_cache *cache,
                                           zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_get_network_resource_cache(cache, instance_index);
        return result;
    }

    int mediaPlayerSetNetWorkBufferThreshold(unsigned int threshold,
                                             zego_media_player_instance_index instance_index) {
        int result =
            zego_express_media_player_set_network_buffer_threshold(threshold, instance_index);
        return result;
    }

    int mediaPlayerMuteLocal(bool mute, zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_mute_local_audio(mute, instance_index);
        return result;
    }

    int mediaPlayerEnableViewMirror(bool enable, zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_enable_view_mirror(enable, instance_index);
        return result;
    }

    int mediaPlayerSetPlayerCanvas(struct zego_canvas *canvas,
                                   zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_set_player_canvas(canvas, instance_index);
        return result;
    }

    int mediaPlayerEnableAUX(bool enable, zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_enable_aux(enable, instance_index);
        return result;
    }

    int mediaPlayerSetPlayLoopCount(int count, zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_set_play_loop_count(count, instance_index);
        return result;
    }

    int mediaPlayerSetPlaySpeed(float speed, zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_set_play_speed(speed, instance_index);
        return result;
    }

    int mediaPlayerEnableRepeat(bool enable, zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_enable_repeat(enable, instance_index);
        return result;
    }

    int mediaPlayerSetProgressInterval(unsigned long long millisecond,
                                       zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_set_progress_interval(millisecond, instance_index);
        return result;
    }

    int mediaPlayerEnableAudioData(bool enable, zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_enable_audio_data(enable, instance_index);
        return result;
    }

    int mediaPlayerEnableVideoData(bool enable, zego_video_frame_format format,
                                   zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_enable_video_data(enable, format, instance_index);
        return result;
    }

    int mediaPlayerEnableSoundLevelMonitor(bool enable, unsigned int millisecond,
                                           zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_enable_sound_level_monitor(enable, millisecond,
                                                                          instance_index);
        return result;
    }

    int mediaPlayerEnableFrequencySpectrumMonitor(bool enable, unsigned int millisecond,
                                                  zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_enable_frequency_spectrum_monitor(
            enable, millisecond, instance_index);
        return result;
    }

    int mediaPlayerEnableBlockData(bool enable, unsigned int blockSize,
                                   zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_enable_block_data(enable, blockSize, instance_index);
        return result;
    }

    int mediaPlayerSetActiveAudioChannel(zego_media_player_audio_channel audio_channel,
                                         zego_media_player_instance_index instance_index) {
        int result =
            zego_express_media_player_set_active_audio_channel(audio_channel, instance_index);
        return result;
    }

    int mediaPlayerClearView(zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_clear_view(instance_index);
        return result;
    }

    int mediaPlayerGetMediaInfo(struct zego_media_player_media_info *media_info,
                                zego_media_player_instance_index instance_index) {
        int result = zego_express_media_player_get_media_info(media_info, instance_index);
        return result;
    }

    void mediaPlayerUpdatePosition(const float position[3],
                                   zego_media_player_instance_index instance_index) {
        zego_express_media_player_update_position(position, instance_index);
    }

    void mediaPlayerSetHttpHeader(std::unordered_map<std::string, std::string> headers,
                                  zego_media_player_instance_index instance_index) {
        int headers_size = headers.size();
        if (headers_size == 0) {
            return;
        }

        zego_key_value_pair *headers_list = new zego_key_value_pair[headers_size]();
        int index = 0;
        for (auto it = headers.begin(); it != headers.end(); it++) {
            zego_key_value_pair *header_item = headers_list + index;
            index++;
            memset(header_item, 0, sizeof(zego_key_value_pair));
            int key_size = it->first.size();
            if (key_size > ZEGO_EXPRESS_MAX_COMMON_LEN) {
                key_size = ZEGO_EXPRESS_MAX_COMMON_LEN;
            }
            memmove(header_item->key, it->first.c_str(), key_size);
            int value_size = it->second.size();
            if (value_size > ZEGO_EXPRESS_MAX_COMMON_LEN) {
                value_size = ZEGO_EXPRESS_MAX_COMMON_LEN;
            }
            memmove(header_item->value, it->second.c_str(), value_size);
        }

        zego_express_media_player_set_http_header(headers_list, headers_size, instance_index);
        delete[] headers_list;
    }
    void mediaPlayerEnableLiveAudioEffect(bool enable, zego_live_audio_effect_mode mode,
                                          zego_media_player_instance_index instance_index) {
        zego_express_media_player_enable_live_audio_effect(enable, mode, instance_index);
    }

    void mediaPlayerEnableLocalCache(bool enable, const char *cached_dir,
                                     zego_media_player_instance_index instance_index) {
        zego_express_media_player_enable_local_cache(enable, cached_dir, instance_index);
    }

    zego_audio_effect_player_instance_index createAudioEffectPlayer() {
        zego_audio_effect_player_instance_index instanceIndex =
            zego_audio_effect_player_instance_index_null;
        zego_express_create_audio_effect_player(&instanceIndex);
        return instanceIndex;
    }

    int destroyAudioEffectPlayer(zego_audio_effect_player_instance_index instance_index) {
        int result = zego_express_destroy_audio_effect_player(instance_index);
        return result;
    }

    int audioEffectPlayerLoadResource(unsigned int audio_effect_id, const char *path,
                                      zego_audio_effect_player_instance_index instance_index) {
        int seq = 0;
        zego_express_audio_effect_player_load_resource(audio_effect_id, path, instance_index, &seq);
        return seq;
    }

    int audioEffectPlayerUnloadResource(unsigned int audio_effect_id,
                                        zego_audio_effect_player_instance_index instance_index) {
        int result =
            zego_express_audio_effect_player_unload_resource(audio_effect_id, instance_index);
        return result;
    }

    int audioEffectPlayerStart(unsigned int audio_effect_id, const char *path,
                               struct zego_audio_effect_play_config *config,
                               zego_audio_effect_player_instance_index instance_index) {
        int result =
            zego_express_audio_effect_player_start(audio_effect_id, path, config, instance_index);
        return result;
    };

    int audioEffectPlayerStop(unsigned int audio_effect_id,
                              zego_audio_effect_player_instance_index instance_index) {
        int result = zego_express_audio_effect_player_stop(audio_effect_id, instance_index);
        return result;
    }

    int audioEffectPlayerPause(unsigned int audio_effect_id,
                               zego_audio_effect_player_instance_index instance_index) {
        int result = zego_express_audio_effect_player_pause(audio_effect_id, instance_index);
        return result;
    }

    int audioEffectPlayerResume(unsigned int audio_effect_id,
                                zego_audio_effect_player_instance_index instance_index) {
        int result = zego_express_audio_effect_player_resume(audio_effect_id, instance_index);
        return result;
    }

    int audioEffectPlayerStopAll(zego_audio_effect_player_instance_index instance_index) {
        int result = zego_express_audio_effect_player_stop_all(instance_index);
        return result;
    }

    int audioEffectPlayerPauseAll(zego_audio_effect_player_instance_index instance_index) {
        int result = zego_express_audio_effect_player_pause_all(instance_index);
        return result;
    }

    int audioEffectPlayerResumeAll(zego_audio_effect_player_instance_index instance_index) {
        int result = zego_express_audio_effect_player_resume_all(instance_index);
        return result;
    }

    int audioEffectPlayerSeekTo(unsigned int audio_effect_id, unsigned long long millisecond,
                                zego_audio_effect_player_instance_index instance_index) {
        int seq = 0;
        zego_express_audio_effect_player_seek_to(audio_effect_id, millisecond, instance_index,
                                                 &seq);
        return seq;
    }

    int audioEffectPlayerSetVolume(unsigned int audio_effect_id, int volume,
                                   zego_audio_effect_player_instance_index instance_index) {
        int result =
            zego_express_audio_effect_player_set_volume(audio_effect_id, volume, instance_index);
        return result;
    }

    int audioEffectPlayerSetPlayVolume(unsigned int audio_effect_id, int volume,
                                       zego_audio_effect_player_instance_index instance_index) {
        int result = zego_express_audio_effect_player_set_play_volume(audio_effect_id, volume,
                                                                      instance_index);
        return result;
    }

    int audioEffectPlayerSetPublishVolume(unsigned int audio_effect_id, int volume,
                                          zego_audio_effect_player_instance_index instance_index) {
        int result = zego_express_audio_effect_player_set_publish_volume(audio_effect_id, volume,
                                                                         instance_index);
        return result;
    }

    int audioEffectPlayerSetVolumeAll(int volume,
                                      zego_audio_effect_player_instance_index instance_index) {
        int result = zego_express_audio_effect_player_set_volume_all(volume, instance_index);
        return result;
    }

    int audioEffectPlayerSetPlayVolumeAll(int volume,
                                          zego_audio_effect_player_instance_index instance_index) {
        int result = zego_express_audio_effect_player_set_play_volume_all(volume, instance_index);
        return result;
    }

    int
    audioEffectPlayerSetPublishVolumeAll(int volume,
                                         zego_audio_effect_player_instance_index instance_index) {
        int result =
            zego_express_audio_effect_player_set_publish_volume_all(volume, instance_index);
        return result;
    }

    int audioEffectPlayerSetPlaySpeed(unsigned int audio_effect_id, float speed,
                                      zego_audio_effect_player_instance_index instance_index) {
        int result =
            zego_express_audio_effect_player_set_play_speed(audio_effect_id, speed, instance_index);
        return result;
    }

    unsigned long long
    audioEffectPlayerGetTotalDuration(unsigned int audio_effect_id,
                                      zego_audio_effect_player_instance_index instance_index) {
        unsigned long long result = 0;
        zego_express_audio_effect_player_get_total_duration(audio_effect_id, instance_index,
                                                            &result);
        return result;
    }

    unsigned long long
    audioEffectPlayerGetCurrentProgress(unsigned int audio_effect_id,
                                        zego_audio_effect_player_instance_index instance_index) {
        unsigned long long result = 0;
        zego_express_audio_effect_player_get_current_progress(audio_effect_id, instance_index,
                                                              &result);
        return result;
    }

    void audioEffectPlayerUpdatePosition(unsigned int audio_effect_id, const float position[3],
                                         zego_audio_effect_player_instance_index instance_index) {
        zego_express_audio_effect_player_update_position(audio_effect_id, position, instance_index);
    }

    void enableCustomVideoRender(bool enable, struct zego_custom_video_render_config *config) {
        zego_express_enable_custom_video_render(enable, config);
    }

    void enableCapturedVideoCustomVideoRender(bool enable, zego_publish_channel channel) {
        zego_express_enable_captured_video_custom_video_render(enable, channel);
    }

    void enableRemoteVideoCustomVideoRender(bool enable, const char *stream_id) {
        zego_express_enable_remote_video_custom_video_render(enable, stream_id);
    }

    void enableCustomVideoCapture(bool enable, zego_custom_video_capture_config *config,
                                  zego_publish_channel channel) {
        zego_express_enable_custom_video_capture(enable, config, channel);
    }

    void customVideoCaptureSetFillMode(zego_view_mode mode, zego_publish_channel channel) {
        zego_express_set_custom_video_capture_fill_mode(mode, channel);
    }

    void setCustomVideoCaptureDeviceState(bool isEnable, zego_remote_device_state state,
                                          zego_publish_channel channel) {
        zego_express_set_custom_video_capture_device_state(isEnable, state, channel);
    }

    void customVideoCaptureRegionOfInterest(zego_roi_rect *rects, unsigned int rects_count,
                                            zego_publish_channel channel) {
        zego_express_set_custom_video_capture_region_of_interest(rects, rects_count, channel);
    }

    void customVideoCaptureSendRawData(const unsigned char *data, unsigned int data_length,
                                       const struct zego_video_frame_param param,
                                       unsigned long long reference_time_millisecond,
                                       zego_publish_channel channel) {
        zego_express_send_custom_video_capture_raw_data(data, data_length, param,
                                                        reference_time_millisecond, 1000, channel);
    }

    int customVideoCaptureSendD3DTextureData(void *texture, int rotation,
                                             unsigned long long reference_time_millisecond,
                                             zego_publish_channel channel) {
        return zego_express_send_custom_video_capture_d3d_texture_data(
            texture, rotation, reference_time_millisecond, 1000, channel);
    }

    void customVideoCaptureSendEncodedFrameData(const unsigned char *data, unsigned int data_length,
                                                const struct zego_video_encoded_frame_param param,
                                                unsigned long long reference_time_millisecond,
                                                zego_publish_channel channel) {
        zego_express_send_custom_video_capture_encoded_data(
            data, data_length, param, double(reference_time_millisecond), channel);
    }

    void enableCustomVideoProcessing(bool enable, zego_custom_video_process_config *config,
                                     zego_publish_channel channel) {
        zego_express_enable_custom_video_processing(enable, config, channel);
    }

    void sendCustomVideoProcessedRawData(const unsigned char **data, unsigned int *data_length,
                                         const struct zego_video_frame_param param,
                                         unsigned long long reference_time_millisecond,
                                         enum zego_processed_data_usage_type usage,
                                         zego_publish_channel channel) {
        zego_express_send_custom_video_processed_raw_data_v2(
            data, data_length, param, reference_time_millisecond, usage, channel);
    }

    void sendCustomVideoProcessedCVPixelBuffer(void *buffer,
                                               unsigned long long reference_time_millisecond,
                                               enum zego_processed_data_usage_type usage,
                                               zego_publish_channel channel) {
        zego_express_send_custom_video_processed_cv_pixel_buffer_v2(
            buffer, reference_time_millisecond, usage, channel);
    }

    void enableCustomAudioCaptureProcessing(bool enable, zego_custom_audio_process_config *config) {
        zego_express_enable_custom_audio_capture_processing(enable, config);
    }

    void enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(
        bool enable, zego_custom_audio_process_config *config) {
        zego_express_enable_custom_audio_capture_processing_after_headphone_monitor(enable, config);
    }

    void enableBeforeAudioPrepAudioData(bool enable, zego_audio_frame_param param) {
        zego_express_enable_before_audio_prep_audio_data(enable, param);
    }

    void enableCustomAudioRemoteProcessing(bool enable, zego_custom_audio_process_config *config) {
        zego_express_enable_custom_audio_remote_processing(enable, config);
    }

    void enableCustomAudioPlaybackProcessing(bool enable,
                                             zego_custom_audio_process_config *config) {
        zego_express_enable_custom_audio_playback_processing(enable, config);
    }

    void startAudioDataObserver(unsigned int observer_bitmask, zego_audio_frame_param param) {
        zego_express_start_audio_data_observer(observer_bitmask, param);
    }

    void stopAudioDataObserver() { zego_express_stop_audio_data_observer(); }

    void startRecordingCapturedData(zego_data_record_config config, zego_publish_channel channel) {
        zego_express_start_recording_captured_data(config, channel);
    }

    void stopRecordingCapturedData(zego_publish_channel channel) {
        zego_express_stop_recording_captured_data(channel);
    }

    void enableCustomAudioIO(bool enable, zego_custom_audio_config *config,
                             zego_publish_channel channel) {
        zego_express_enable_custom_audio_io(enable, config, channel);
    }

    void sendCustomAudioCaptureAACData(unsigned char *data, unsigned int dataLength,
                                       unsigned int configLength,
                                       unsigned long long referenceTimeMillisecond,
                                       unsigned int samples, zego_audio_frame_param param,
                                       zego_publish_channel channel) {
        zego_express_send_custom_audio_capture_aac_data(
            data, dataLength, configLength, referenceTimeMillisecond, samples, param, channel);
    }

    void sendCustomAudioCapturePCMData(unsigned char *data, unsigned int dataLength,
                                       zego_audio_frame_param param, zego_publish_channel channel) {
        zego_express_send_custom_audio_capture_pcm_data(data, dataLength, param, channel);
    }

    void fetchCustomAudioRenderPCMData(unsigned char *data, unsigned int dataLength,
                                       zego_audio_frame_param param) {
        zego_express_fetch_custom_audio_render_pcm_data(data, dataLength, param);
    }

    void sendReferenceAudioPCMData(unsigned char *data, unsigned int dataLength,
                                   zego_audio_frame_param param) {
        zego_express_send_reference_audio_pcm_data(data, dataLength, param);
    }

    // void sendReferenceAudioPCMData(unsigned char *data, unsigned int dataLength,
    //                                zego_audio_frame_param param, zego_publish_channel channel) {
    //     zego_express_send_reference_audio_pcm_data_with_channel(data, dataLength, param, channel);
    // }

    int testNetworkConnectivity() {
        int seq = 0;
        zego_express_test_network_connectivity(&seq);
        return seq;
    }

    void startNetworkSpeedTest(zego_network_speed_test_config config, unsigned int interval) {
        zego_express_start_network_speed_test(config, interval);
    }

    void stopNetworkSpeedTest() { zego_express_stop_network_speed_test(); }

    zego_network_time_info getNetworkTimeInfo() {
        zego_network_time_info timeInfo;
        zego_express_get_network_time_info(&timeInfo);
        return timeInfo;
    }

    int startNetworkProbe(zego_network_probe_config config) {
        int seq = 0;
        zego_express_start_network_probe(config, &seq);
        return seq;
    }

    void stopNetworkProbe() { zego_express_stop_network_probe(); }

    void startDumpData(zego_dump_data_config config) { zego_express_start_dump_data(config); }

    void stopDumpData() { zego_express_stop_dump_data(); }

    void uploadDumpData() { zego_express_upload_dump_data(); }

    void removeDumpData() { zego_express_remove_dump_data(); }

    zego_range_audio_instance_index createRangeAudio() {
        zego_range_audio_instance_index instanceIndex = zego_range_audio_instance_index_null;
        zego_express_create_range_audio(&instanceIndex);
        return instanceIndex;
    }

    int destroyRangeAudio(zego_range_audio_instance_index instance_index) {
        int result = zego_express_destroy_range_audio(instance_index);
        return result;
    }

    void setRangeAudioMode(zego_range_audio_mode mode,
                           zego_range_audio_instance_index instance_index) {
        zego_express_set_range_audio_mode(mode, instance_index);
    }

    void setRangeAudioCustomMode(zego_range_audio_speak_mode speak_mode,
                                 zego_range_audio_listen_mode listen_mode,
                                 zego_range_audio_instance_index instance_index) {
        zego_express_set_range_audio_custom_mode(speak_mode, listen_mode, instance_index);
    }

    void rangeAudioSetTeamID(const char *team_id, zego_range_audio_instance_index instance_index) {
        zego_express_range_audio_set_team_id(team_id, instance_index);
    }

    void rangeAudioSetAudioReceiveRange(float range,
                                        zego_range_audio_instance_index instance_index) {
        zego_express_range_audio_set_audio_receive_range(range, instance_index);
    }

    int rangeAudioSetAudioReceiveRange(zego_receive_range_param param,
                                       zego_range_audio_instance_index instance_index) {
        return zego_express_range_audio_set_audio_receive_range_with_param(param, instance_index);
    }

    void rangeAudiosetPositionUpdateFrequency(int frequency,
                                              zego_range_audio_instance_index instance_index) {
        zego_express_range_audio_set_position_update_frequency(frequency, instance_index);
    }

    void rangeAudioSetAudioVolume(int volume, zego_range_audio_instance_index instance_index) {
        zego_express_range_audio_set_audio_volume(volume, instance_index);
    }

    void rangeAudioSetStreamVocalRange(const char *streamID, float vocalRange,
                                       zego_range_audio_instance_index instance_index) {
        zego_express_range_audio_set_stream_vocal_range(streamID, vocalRange, instance_index);
    }

    int rangeAudioSetStreamVocalRange(const char *streamID, zego_vocal_range_param param,
                                      zego_range_audio_instance_index instance_index) {
        return zego_express_range_audio_set_stream_vocal_range_with_param(streamID, param,
                                                                          instance_index);
    }

    void rangeAudioUpdateStreamPosition(const char *streamID, float position[3],
                                        zego_range_audio_instance_index instance_index) {
        zego_express_range_audio_update_stream_position(streamID, position, instance_index);
    }

    void rangeAudioEnableMicrophone(bool enable, zego_range_audio_instance_index instance_index) {
        zego_express_range_audio_enable_microphone(enable, instance_index);
    }

    void rangeAudioEnableSpeaker(bool enable, zego_range_audio_instance_index instance_index) {
        zego_express_range_audio_enable_speaker(enable, instance_index);
    }

    void rangeAudioEnableSpatializer(bool enable, zego_range_audio_instance_index instance_index) {
        zego_express_range_audio_enable_spatializer(enable, instance_index);
    }

    void rangeAudioUpdateAudioSource(const char *user_id, float position[3],
                                     zego_range_audio_instance_index instance_index) {
        zego_express_range_audio_update_audio_source(user_id, position, instance_index);
    }

    void rangeAudioUpdateSelfPosition(float position[3], float axisForward[3], float axisRight[3],
                                      float axisUp[3],
                                      zego_range_audio_instance_index instance_index) {
        zego_express_range_audio_update_self_position(position, axisForward, axisRight, axisUp,
                                                      instance_index);
    }

    void rangeAudioMuteUser(const std::string &userID, bool mute,
                            zego_range_audio_instance_index instance_index) {
        zego_express_range_audio_mute_user(userID.c_str(), mute, instance_index);
    }

    void createCopyrightedMusic() { zego_express_create_copyrighted_music(); }

    void destroyCopyrightedMusic() { zego_express_destroy_copyrighted_music(); }

    int copyrightedMusicInitCopyrightedMusic(zego_copyrighted_music_config config) {
        int seq = 0;
        zego_express_copyrighted_music_init(config, &seq);
        return seq;
    }

    unsigned long long copyrightedMusicGetCacheSize() {
        unsigned long long size = 0;
        zego_express_copyrighted_music_get_cache_size(&size);
        return size;
    }

    void copyrightedMusicClearCache() { zego_express_copyrighted_music_clear_cache(); }

    int copyrightedMusicSendExtendedRequest(const char *command, const char *params) {
        int seq = 0;
        zego_express_copyrighted_music_send_extended_request(command, params, &seq);
        return seq;
    }

    int copyrightedMusicGetLrcLyric(const char *song_id) {
        int seq = 0;
        zego_express_copyrighted_music_get_lrc_lyric(song_id, &seq);
        return seq;
    }

    int copyrightedMusicGetLrcLyric(const char *song_id,
                                    zego_copyrighted_music_vendor_id vendor_id) {
        int seq = 0;
        zego_express_copyrighted_music_get_lrc_lyric_with_vendor(song_id, vendor_id, &seq);
        return seq;
    }

    int copyrightedMusicGetLrcLyric(zego_copyrighted_music_get_lyric_config config) {
        int seq = 0;
        zego_express_copyrighted_music_get_lrc_lyric_with_config(config, &seq);
        return seq;
    }

    int copyrightedMusicGetKrcLyricByToken(const char *krc_token) {
        int seq = 0;
        zego_express_copyrighted_music_get_krc_lyric_by_token(krc_token, &seq);
        return seq;
    }

    int copyrightedMusicRequestSong(zego_copyrighted_music_request_config config) {
        int seq = 0;
        zego_express_copyrighted_music_request_song(config, &seq);
        return seq;
    }

    int copyrightedMusicRequestAccompaniment(zego_copyrighted_music_request_config config) {
        int seq = 0;
        zego_express_copyrighted_music_request_accompaniment(config, &seq);
        return seq;
    }

    int copyrightedMusicRequestAccompanimentClip(zego_copyrighted_music_request_config config) {
        int seq = 0;
        zego_express_copyrighted_music_request_accompaniment_clip(config, &seq);
        return seq;
    }

    int copyrightedMusicGetMusicByToken(const char *song_token) {
        int seq = 0;
        zego_express_copyrighted_music_get_music_by_token(song_token, &seq);
        return seq;
    }

    int copyrightedMusicRequestResource(zego_copyrighted_music_request_config config,
                                        zego_copyrighted_music_resource_type type) {
        int seq = 0;
        zego_express_copyrighted_music_request_resource(config, type, &seq);
        return seq;
    }

    int copyrightedMusicRequestResource(zego_copyrighted_music_request_config_v2 config) {
        int seq = 0;
        zego_express_copyrighted_music_request_resource_v2(config, &seq);
        return seq;
    }

    int copyrightedMusicGetSharedResource(zego_copyrighted_music_get_shared_config config,
                                          zego_copyrighted_music_resource_type type) {
        int seq = 0;
        zego_express_copyrighted_music_get_shared_resource(config, type, &seq);
        return seq;
    }

    int copyrightedMusicGetSharedResource(zego_copyrighted_music_get_shared_config_v2 config) {
        int seq = 0;
        zego_express_copyrighted_music_get_shared_resource_v2(config, &seq);
        return seq;
    }

    int copyrightedMusicDownload(const char *resource_id) {
        int seq = 0;
        zego_express_copyrighted_music_download(resource_id, &seq);
        return seq;
    }

    void copyrightedMusicCancelDownload(const char *resource_id) {
        zego_express_copyrighted_music_cancel_download(resource_id);
    }

    bool copyrightedMusicQueryCache(const char *song_id, zego_copyrighted_music_type type) {
        bool result = false;
        zego_express_copyrighted_music_query_cache(song_id, type, &result);
        return result;
    }

    bool copyrightedMusicQueryCache(const char *song_id, zego_copyrighted_music_type type,
                                    zego_copyrighted_music_vendor_id vendor_id) {
        bool result = false;
        zego_express_copyrighted_music_query_cache_with_vendor(song_id, type, vendor_id, &result);
        return result;
    }

    bool copyrightedMusicQueryCache(zego_copyrighted_music_query_cache_config config) {
        bool result = false;
        zego_express_copyrighted_music_query_cache_with_config(config, &result);
        return result;
    }

    bool copyrightedMusicQueryCache(zego_copyrighted_music_query_cache_config_v2 config) {
        bool result = false;
        zego_express_copyrighted_music_query_cache_with_config_v2(config, &result);
        return result;
    }

    unsigned long long copyrightedMusicGetDuration(const char *resource_id) {
        unsigned long long totalDuration = 0;
        zego_express_copyrighted_music_get_duration(resource_id, &totalDuration);
        return totalDuration;
    }

    void copyrightedMusicSetScoringLevel(int level) {
        zego_express_copyrighted_music_set_scoring_level(level);
    }

    int copyrightedMusicStartScore(const char *resource_id, int pitchValueInterval) {
        int ret = zego_express_copyrighted_music_start_score(resource_id, pitchValueInterval);
        return ret;
    }

    int copyrightedMusicPauseScore(const char *resource_id) {
        int ret = zego_express_copyrighted_music_pause_score(resource_id);
        return ret;
    }

    int copyrightedMusicResumeScore(const char *resource_id) {
        int ret = zego_express_copyrighted_music_resume_score(resource_id);
        return ret;
    }

    int copyrightedMusicStopScore(const char *resource_id) {
        int ret = zego_express_copyrighted_music_stop_score(resource_id);
        return ret;
    }

    int copyrightedMusicResetScore(const char *resource_id) {
        int ret = zego_express_copyrighted_music_reset_score(resource_id);
        return ret;
    }

    int copyrightedMusicGetPreviousScore(const char *resource_id) {
        int score = 0;
        zego_express_copyrighted_music_get_previous_score(resource_id, &score);
        return score;
    }

    int copyrightedMusicGetAverageScore(const char *resource_id) {
        int score = 0;
        zego_express_copyrighted_music_get_average_score(resource_id, &score);
        return score;
    }

    int copyrightedMusicGetTotalScore(const char *resource_id) {
        int score = 0;
        zego_express_copyrighted_music_get_total_score(resource_id, &score);
        return score;
    }

    int copyrightedMusicGetFullScore(const char *resource_id) {
        int score = 0;
        zego_express_copyrighted_music_get_full_score(resource_id, &score);
        return score;
    }

    int copyrightedMusicGetCurrentPitch(const char *resource_id) {
        int score = 0;
        zego_express_copyrighted_music_get_current_pitch(resource_id, &score);
        return score;
    }

    int copyrightedMusicGetStandardPitch(const char *resource_id) {
        int seq = 0;
        zego_express_copyrighted_music_get_standard_pitch(resource_id, &seq);
        return seq;
    }

    struct zego_screen_capture_source_info *getScreenCaptureSources(int thumbnail_width,
                                                                    int thumbnail_height,
                                                                    int icon_width, int icon_height,
                                                                    int *source_count) {
        zego_screen_capture_source_info *result = zego_express_get_screen_capture_sources(
            thumbnail_width, thumbnail_height, icon_width, icon_height, source_count);
        return result;
    }

    void freeScreenCaptureSources(struct zego_screen_capture_source_info *list, int source_count) {
        zego_express_free_screen_capture_source_list(list, source_count);
    }

    int createScreenCaptureSource(void *source_id,
                                  enum zego_screen_capture_source_type source_type) {
        int instanceIndex = zego_express_create_screen_capture_source(source_id, source_type);
        return instanceIndex;
    }

    int destroyScreenCaptureSource(int instance_index) {
        int result = zego_express_destroy_screen_capture_source(instance_index);
        return result;
    }

    int screenCaptureUpdateCaptureSource(void *source_id,
                                         enum zego_screen_capture_source_type source_type,
                                         int instance_index) {
        int result = zego_express_screen_capture_update_capture_source(source_id, source_type,
                                                                       instance_index);
        return result;
    }

    int screenCaptureStartCapture(int instance_index) {
        int result = zego_express_screen_capture_start_capture(instance_index);
        return result;
    }

    int screenCaptureStopCapture(int instance_index) {
        int result = zego_express_screen_capture_stop_capture(instance_index);
        return result;
    }
    struct zego_rect screenCaptureGetCaptureSourceRect(int instance_index) {
        return zego_express_screen_capture_get_capture_source_rect(instance_index);
    }

    int screenCaptureUpdateCaptureRegion(struct zego_rect rect, int instance_index) {
        int result = zego_express_screen_capture_update_capture_region(rect, instance_index);
        return result;
    }

    int screenCaptureUpdatePublishRegion(struct zego_rect rect, int instance_index) {
        int result = zego_express_screen_capture_update_publish_region(rect, instance_index);
        return result;
    }

    int screenCaptureSetExcludeWindowList(void **list, int count, int instance_index) {
        int result =
            zego_express_screen_capture_set_exclude_window_list(list, count, instance_index);
        return result;
    }

    int screenCaptureEnableWindowActivate(bool active, int instance_index) {
        int result = zego_express_screen_capture_enable_window_activate(active, instance_index);
        return result;
    }

    int screenCaptureEnableCursorVisible(bool visible, int instance_index) {
        int result = zego_express_screen_capture_enable_cursor_visible(visible, instance_index);
        return result;
    }

    int screenCaptureEnableHightLight(bool enable, ZegoLayerBorderConfig config,
                                      int instance_index) {
        zego_layer_border_config border_config;
        border_config.color = config.color;
        border_config.width = config.width;
        int result =
            zego_express_screen_capture_enable_hight_light(enable, border_config, instance_index);
        return result;
    }

    int screenCaptureEnableAudioCapture(bool enable, ZegoAudioFrameParam audioParam,
                                        int instance_index) {
        zego_audio_frame_param audio_param;
        audio_param.channel = (zego_audio_channel)audioParam.channel;
        audio_param.sample_rate = (zego_audio_sample_rate)audioParam.sampleRate;
        int result =
            zego_express_screen_capture_enable_audio_capture(enable, audio_param, instance_index);
        return result;
    }

    zego_instance createMediaDataPublisher(ZegoMediaDataPublisherConfig config) {
        zego_media_data_publisher_config publisher_config;
        publisher_config.mode = (zego_media_data_publisher_mode)config.mode;
        publisher_config.channel = config.channel;

        zego_instance instanceIndex = -1;
        zego_express_create_media_data_publisher(publisher_config, &instanceIndex);
        return instanceIndex;
    }

    void destroyMediaDataPublisher(zego_instance instance_index) {
        zego_express_destroy_media_data_publisher(instance_index);
    }

    void mediaDataPublisherAddMediaFilePath(const std::string &path, bool is_clear,
                                            zego_instance instance_index) {
        zego_express_media_data_publisher_add_media_file_path(path.c_str(), is_clear,
                                                              instance_index);
    }

    void mediaDataPublisherReset(zego_instance instance_index) {
        zego_express_media_data_publisher_reset(instance_index);
    }

    void mediaDataPublisherSetVideoSendDelayTime(int delayTime, zego_instance instance_index) {
        zego_express_media_data_publisher_set_video_send_delay_time(delayTime, instance_index);
    }

    void mediaDataPublisherSeekTo(unsigned long long millisecond, zego_instance instance_index) {
        zego_express_media_data_publisher_seek_to(millisecond, instance_index);
    }

    unsigned long long mediaDataPublisherGetTotalDuration(zego_instance instance_index) {
        unsigned long long duration = 0;
        zego_express_media_data_publisher_get_total_duration(instance_index, &duration);
        return duration;
    }

    unsigned long long mediaDataPublisherGetCurrentDuration(zego_instance instance_index) {
        unsigned long long duration = 0;
        zego_express_media_data_publisher_get_current_duration(instance_index, &duration);
        return duration;
    }

    int createRangeScene() {
        int range_scene_handle = -1;
        zego_express_create_range_scene(&range_scene_handle);
        return range_scene_handle;
    }

    void destroyRangeScene(int range_scene_handle) {
        zego_express_destroy_range_scene(range_scene_handle);
    }

    int loginScene(int range_scene_handle, int *seq, zego_scene_param param) {
        return zego_express_range_scene_login_scene(range_scene_handle, seq, param);
    }

    int logoutScene(int range_scene_handle, int *seq) {
        return zego_express_range_scene_logout_scene(range_scene_handle, seq);
    }

    int updateUserStatus(int range_scene_handle, zego_position position, unsigned int channel,
                         const unsigned char *status, unsigned int status_length) {
        return zego_express_range_scene_update_user_status(range_scene_handle, position, channel,
                                                           status, status_length);
    }

    int updateUserCommand(int range_scene_handle, zego_position position, unsigned int channel,
                          const unsigned char *command, unsigned int command_length) {
        return zego_express_range_scene_update_user_command(range_scene_handle, position, channel,
                                                            command, command_length);
    }

    int updateUserPosition(int range_scene_handle, zego_position position) {
        return zego_express_range_scene_update_user_position(range_scene_handle, position);
    }

    int getUserCount(int range_scene_handle, int *seq) {
        return zego_express_range_scene_get_user_count(range_scene_handle, seq);
    }

    int getUserListInView(int range_scene_handle, int *seq) {
        return zego_express_range_scene_get_user_list_in_view(range_scene_handle, seq);
    }

    int sendCustomCommand(int range_scene_handle, int *seq, const unsigned char *command,
                          unsigned int command_length) {
        return zego_express_range_scene_send_custom_command(range_scene_handle, seq, command,
                                                            command_length);
    }

    int renewToken(int range_scene_handle, const char *token) {
        return zego_express_range_scene_renew_token(range_scene_handle, token);
    }

    int setStreamConfig(int range_scene_handle, zego_scene_stream_config config) {
        return zego_express_range_scene_set_stream_config(range_scene_handle, config);
    }

    int setReceiveRange(int range_scene_handle, float range) {
        return zego_express_range_scene_stream_set_receive_range(range_scene_handle, range);
    }

    int setReceiveRange(int range_scene_handle, zego_receive_range_param param) {
        return zego_express_range_scene_stream_set_receive_range_with_param(range_scene_handle,
                                                                            param);
    }

    int enableRangeSpatializer(int range_scene_handle, bool enable) {
        return zego_express_range_scene_stream_enable_range_spatializer(range_scene_handle, enable);
    }

    int mutePlayAudio(int range_scene_handle, const char *user_id, bool mute) {
        return zego_express_range_scene_stream_mute_play_audio(range_scene_handle, user_id, mute);
    }

    int mutePlayVideo(int range_scene_handle, const char *user_id, bool mute) {
        return zego_express_range_scene_stream_mute_play_video(range_scene_handle, user_id, mute);
    }

    int joinTeam(int range_scene_handle, int *seq, zego_team_param config) {
        return zego_express_range_scene_team_join_team(range_scene_handle, seq, config);
    }

    int leaveTeam(int range_scene_handle, int *seq, unsigned int team_id) {
        return zego_express_range_scene_team_leave_team(range_scene_handle, seq, team_id);
    }

    int createItem(int range_scene_handle, zego_seq *seq, zego_item_param param) {
        return zego_express_range_scene_item_create_item(range_scene_handle, seq, param);
    }

    int destroyItem(int range_scene_handle, zego_seq *seq, long long item_id) {
        return zego_express_range_scene_item_destroy_item(range_scene_handle, seq, item_id);
    }

    int bindItem(int range_scene_handle, zego_seq *seq, long long item_id) {
        return zego_express_range_scene_item_bind_item(range_scene_handle, seq, item_id);
    }

    int unbindItem(int range_scene_handle, zego_seq *seq, long long item_id) {
        return zego_express_range_scene_item_unbind_item(range_scene_handle, seq, item_id);
    }

    int updateItemStatus(int range_scene_handle, zego_seq *seq, long long item_id,
                         zego_position position, unsigned int channel, const unsigned char *status,
                         unsigned int status_length) {
        return zego_express_range_scene_item_update_item_status(
            range_scene_handle, seq, item_id, position, channel, status, status_length);
    }

    int updateItemCommand(int range_scene_handle, zego_seq *seq, long long item_id,
                          zego_position position, unsigned int channel,
                          const unsigned char *command, unsigned int command_length) {
        return zego_express_range_scene_item_update_item_command(
            range_scene_handle, seq, item_id, position, channel, command, command_length);
    }

    int initVideoSuperResolution() { return zego_express_init_video_super_resolution(); }

    int uninitVideoSuperResolution() { return zego_express_uninit_video_super_resolution(); }

    int enableVideoSuperResolution(const std::string &streamID, bool enable) {
        return zego_express_enable_video_super_resolution(streamID.c_str(), enable);
    }

    int enableVideoObjectSegmentation(bool enable, zego_object_segmentation_type type,
                                      zego_publish_channel channel) {
        return zego_express_enable_video_object_segmentation(enable, type, channel);
    }
    int enableVideoObjectSegmentation(bool enable, zego_object_segmentation_config config,
                                      zego_publish_channel channel) {
        return zego_express_enable_video_object_segmentation_with_config(enable, config, channel);
    }
    int enableAlphaChannelVideoEncoder(bool enable, zego_alpha_layout_type alpha_layout,
                                       zego_publish_channel channel) {
        return zego_express_enable_alpha_channel_video_encoder(enable, alpha_layout, channel);
    }
    int updatePlayingCanvas(const char *stream_id, zego_canvas *canvas) {
        return zego_express_update_playing_canvas(stream_id, canvas);
    }

    int createAIVoiceChanger() {
        int index = -1;
        zego_express_create_ai_voice_changer(&index);
        return index;
    }

    void destroyAIVoiceChanger(int index) { zego_express_destroy_ai_voice_changer(index); }

    bool isAIVoiceChangerSupported() {
        int is_supported = 0;
        zego_express_is_ai_voice_changer_supported(&is_supported);
        return is_supported != 0;
    }

    int aiVoiceChangerInitEngine(int index) { return zego_express_ai_voice_changer_init(index); }

    int aiVoiceChangerUpdate(int index) { return zego_express_ai_voice_changer_update(index); }

    int aiVoiceChangerGetSpeakerList(int index) {
        return zego_express_ai_voice_changer_get_speaker_list(index);
    }

    int aiVoiceChangerSetSpeaker(int index, int speaker_id) {
        return zego_express_ai_voice_changer_set_speaker(index, speaker_id);
    }

    int mediaPlayerSetPlayMediaStreamType(zego_media_stream_type stream_type,
                                          zego_media_player_instance_index instance_index) {
        return zego_express_media_player_set_play_media_stream_type(stream_type, instance_index);
    }
    int mediaPlayerGetPlaybackStatistics(zego_media_player_statistics_info *stat,
                                         zego_media_player_instance_index instance_index) {
        return zego_express_media_player_get_playback_statistics(stat, instance_index);
    }
    int enableColorEnhancement(bool enable, zego_color_enhancement_params params,
                               zego_publish_channel channel) {
        return zego_express_enable_color_enhancement(enable, params, channel);
    }

    void postWindowsMessage(void *message) { zego_express_post_windows_message(message); }

    // eventHandler
    void registerRecvWindowsMessageCallback(void *callback, void *user_context) {
        zego_register_recv_windows_message_callback(callback, user_context);
    }

    void postZegoCallbackTask(void *callback_task) {
        zego_express_post_zego_callback_task(callback_task);
    }

    //win and mac
    void registerRecvZegoCallbackTask(void *callback, void *user_context) {

        zego_register_recv_zego_callback_task(callback, user_context);
    }

    void registerEngineStateCallback(void *callback_func, void *user_context) {
        zego_register_engine_state_update_callback(zego_on_engine_state_update(callback_func),
                                                   user_context);
    }

    void registerUploadLogResultCallback(void *callback_func, void *user_context) {
        zego_register_upload_log_result_callback(zego_on_upload_log_result(callback_func),
                                                 user_context);
    }

    void registerEngineUninitCallback(void *callback_func, void *user_context) {
        zego_register_engine_uninit_callback(zego_on_engine_uninit(callback_func), user_context);
    }

    void registerFatalErrorCallback(void *callback_func, void *user_context) {
        zego_register_fatal_error_callback(zego_on_fatal_error(callback_func), user_context);
    }

    void registerOnCallErrorCallback(void *callback, void *context) {
        zego_register_debug_error_callback(zego_on_debug_error(callback), context);
    }
    void registerOnApiCalledResultCallback(void *callback, void *context) {
        zego_register_api_called_result_callback(zego_on_api_called_result(callback), context);
    }

    void registerRoomStreamUpdateCallback(void *callback_func, void *user_context) {
        zego_register_room_stream_update_callback(zego_on_room_stream_update(callback_func),
                                                  user_context);
    }

    void registerRoomStreamExtraInfoUpdateCallback(void *callback_func, void *user_context) {
        zego_register_room_stream_extra_info_update_callback(
            zego_on_room_stream_extra_info_update(callback_func), user_context);
    }

    void registerRoomStateUpdateCallback(void *callback_func, void *user_context) {
        zego_register_room_state_update_callback(zego_on_room_state_update(callback_func),
                                                 user_context);
    }

    void registerRoomUserStateUpdateCallback(void *callback_func, void *user_context) {
        zego_register_room_user_update_callback(zego_on_room_user_update(callback_func),
                                                user_context);
    }

    void registerRoomOnlineUserCountUpdateCallback(void *callback_func, void *user_context) {
        zego_register_room_online_user_count_update_callback(
            zego_on_room_online_user_count_update(callback_func), user_context);
    }

    void registerRoomExtraInfoUpdateCallback(void *callback_func, void *user_context) {
        zego_register_room_extra_info_update_callback(zego_on_room_extra_info_update(callback_func),
                                                      user_context);
    }

    void registerRoomSetRoomExtraInfoResultCallback(void *callback_func, void *user_context) {
        zego_register_room_set_room_extra_info_result_callback(
            zego_on_room_set_room_extra_info_result(callback_func), user_context);
    }

    void registerRoomTokenWillExpireCallback(void *callback_func, void *user_context) {
        zego_register_room_token_will_expire_callback(zego_on_room_token_will_expire(callback_func),
                                                      user_context);
    }

    void registerRoomLoginCallback(void *callback_func, void *user_context) {
        zego_register_room_login_result_callback(zego_on_room_login_result(callback_func),
                                                 user_context);
    }

    void registerRoomLogoutCallback(void *callback_func, void *user_context) {
        zego_register_room_logout_result_callback(zego_on_room_logout_result(callback_func),
                                                  user_context);
    }

    void registerRoomStateChangedCallback(void *callback_func, void *user_context) {
        zego_register_room_state_changed_callback(zego_on_room_state_changed(callback_func),
                                                  user_context);
    }

    void registerPublisherStateUpdateCallback(void *callback_func, void *user_context) {
        zego_register_publisher_state_update_callback(zego_on_publisher_state_update(callback_func),
                                                      user_context);
    }

    void registerPublisherQualityUpdateCallback(void *callback_func, void *user_context) {
        zego_register_publisher_quality_update_callback(
            zego_on_publisher_quality_update(callback_func), user_context);
    }

    void registerPublisherRecvAudioCapturedFirstFrameCallback(void *callback_func,
                                                              void *user_context) {
        zego_register_publisher_captured_audio_first_frame_callback(
            zego_on_publisher_captured_audio_first_frame(callback_func), user_context);
    }

    void registerPublisherRecvVideoCapturedFirstFrameCallback(void *callback_func,
                                                              void *user_context) {
        zego_register_publisher_captured_video_first_frame_callback(
            zego_on_publisher_captured_video_first_frame(callback_func), user_context);
    }

    void registerPublisherRecvAudioSendFirstFrameCallback(void *callback_func, void *user_context) {
        zego_register_publisher_send_audio_first_frame_callback(
            zego_on_publisher_send_audio_first_frame(callback_func), user_context);
    }

    void registerPublisherRecvVideoSendFirstFrameCallback(void *callback_func, void *user_context) {
        zego_register_publisher_send_video_first_frame_callback(
            zego_on_publisher_send_video_first_frame(callback_func), user_context);
    }

    void registerPublisherRenderVideoFirstFrameCallback(void *callback_func, void *user_context) {
        zego_register_publisher_render_video_first_frame_callback(
            zego_on_publisher_render_video_first_frame(callback_func), user_context);
    }

    void registerPublisherVideoSizeChangedCallback(void *callback_func, void *user_context) {
        zego_register_publisher_video_size_changed_callback(
            zego_on_publisher_video_size_changed(callback_func), user_context);
    }

    void registerPublisherRelayCDNStateUpdateCallback(void *callback_func, void *user_context) {
        zego_register_publisher_relay_cdn_state_update_callback(
            zego_on_publisher_relay_cdn_state_update(callback_func), user_context);
    }

    void registerPublisherVideoEncoderChangedCallback(void *callback_func, void *user_context) {
        zego_register_publisher_video_encoder_changed_callback(
            zego_on_publisher_video_encoder_changed(callback_func), user_context);
    }

    void registerPublisherUpdatePublishCdnUrlCallback(void *callback_func, void *user_context) {
        zego_register_publisher_update_cdn_url_result_callback(
            zego_on_publisher_update_cdn_url_result(callback_func), user_context);
    }

    void registerPublisherUpdateStreamExtraInfoResultCallback(void *callback_func,
                                                              void *user_context) {
        zego_register_publisher_update_stream_extra_info_result_callback(
            zego_on_publisher_update_stream_extra_info_result(callback_func), user_context);
    }

    void registerPublisherTakeSnapshotResultCallback(void *callback_func, void *user_context) {
        zego_register_publisher_take_snapshot_result_callback(
            zego_on_publisher_take_snapshot_result(callback_func), user_context);
    }

    void registerPublisherStreamEventCallback(void *callback_func, void *user_context) {
        zego_register_publisher_stream_event_callback(zego_on_publisher_stream_event(callback_func),
                                                      user_context);
    }

    void registerPublisherLowFpsWarningCallback(void *callback_func, void *user_context) {
        zego_register_publisher_low_fps_warning_callback(
            (zego_on_publisher_low_fps_warning)callback_func, user_context);
    }

    void registerPublisherDummyCaptureImagePathErrorCallback(void *callback_func,
                                                             void *user_context) {
        zego_register_publisher_dummy_capture_image_path_error_callback(
            (zego_on_publisher_dummy_capture_image_path_error)callback_func, user_context);
    }

    void registerPlayerStreamEventCallback(void *callback_func, void *user_context) {
        zego_register_player_stream_event_callback(zego_on_player_stream_event(callback_func),
                                                   user_context);
    }

    void registerPlayerStateUpdateCallback(void *callback_func, void *user_context) {
        zego_register_player_state_update_callback(zego_on_player_state_update(callback_func),
                                                   user_context);
    }

    void registerPlayerSwitchedCallback(void *callback_func, void *user_context) {
        zego_register_player_switched_callback(zego_on_player_switched(callback_func),
                                               user_context);
    }

    void registerPlayerQualityUpdateCallback(void *callback_func, void *user_context) {
        zego_register_player_quality_update_callback(zego_on_player_quality_update(callback_func),
                                                     user_context);
    }

    void registerPlayerMediaEventCallback(void *callback_func, void *user_context) {
        zego_register_player_media_event_callback(zego_on_player_media_event(callback_func),
                                                  user_context);
    }

    void registerPlayerRecvAudioFirstFrameCallback(void *callback_func, void *user_context) {
        zego_register_player_recv_audio_first_frame_callback(
            zego_on_player_recv_audio_first_frame(callback_func), user_context);
    }

    void registerPlayerRecvVideoFirstFrameCallback(void *callback_func, void *user_context) {
        zego_register_player_recv_video_first_frame_callback(
            zego_on_player_recv_video_first_frame(callback_func), user_context);
    }

    void registerPlayerRenderVideoFirstFrameCallback(void *callback_func, void *user_context) {
        zego_register_player_render_video_first_frame_callback(
            zego_on_player_render_video_first_frame(callback_func), user_context);
    }

    void registerPlayerRenderCameraVideoFirstFrameCallback(void *callback_func,
                                                           void *user_context) {
        zego_register_player_render_camera_video_first_frame_callback(
            zego_on_player_render_camera_video_first_frame(callback_func), user_context);
    }

    void registerPlayerVideoSizeChangedCallback(void *callback_func, void *user_context) {
        zego_register_player_video_size_changed_callback(
            zego_on_player_video_size_changed(callback_func), user_context);
    }

    void registerPlayerRecvSEICallback(void *callback_func, void *user_context) {
        zego_register_player_recv_media_side_info_callback(
            zego_on_player_recv_media_side_info(callback_func), user_context);
    }

    void registerPlayerRecvAudioSideInfoCallback(void *callback_func, void *user_context) {
        zego_register_player_recv_audio_side_info_callback(
            zego_on_player_recv_audio_side_info(callback_func), user_context);
    }

    void registerPlayerTakeSnapshotResultCallback(void *callback_func, void *user_context) {
        zego_register_player_take_snapshot_result_callback(
            zego_on_player_take_snapshot_result(callback_func), user_context);
    }

    void registerPlayerLowFpsWarningCallback(void *callback_func, void *user_context) {
        zego_register_player_low_fps_warning_callback(zego_on_player_low_fps_warning(callback_func),
                                                      user_context);
    }

    void registerPlayerVideoSuperResolutionUpdate(void *callback_func, void *user_context) {
        zego_register_player_video_super_resolution_update_callback(
            zego_on_player_video_super_resolution_update(callback_func), user_context);
    }

    void registerCapturedSoundLevelUpdateCallback(void *callback_func, void *user_context) {
        zego_register_captured_sound_level_update_callback(
            zego_on_captured_sound_level_update(callback_func), user_context);
    }

    void registerRemoteSoundLevelUpdateCallback(void *callback_func, void *user_context) {
        zego_register_remote_sound_level_update_callback(
            zego_on_remote_sound_level_update(callback_func), user_context);
    }

    void registerCapturedSoundLevelInfoUpdateCallback(void *callback_func, void *user_context) {
        zego_register_captured_sound_level_info_update_callback(
            zego_on_captured_sound_level_info_update(callback_func), user_context);
    }

    void registerRemoteSoundLevelInfoUpdateCallback(void *callback_func, void *user_context) {
        zego_register_remote_sound_level_info_update_callback(
            zego_on_remote_sound_level_info_update(callback_func), user_context);
    }

    void registerAudioVADStateUpdateCallback(void *callback_func, void *user_context) {
        zego_register_audio_vad_state_update_callback(zego_on_audio_vad_state_update(callback_func),
                                                      user_context);
    }

    void registerCapturedAudioSpectrumUpdateCallback(void *callback_func, void *user_context) {
        zego_register_captured_audio_spectrum_update_callback(
            zego_on_captured_audio_spectrum_update(callback_func), user_context);
    }

    void registerRemoteAudioSpectrumUpdateCallback(void *callback_func, void *user_context) {
        zego_register_remote_audio_spectrum_update_callback(
            zego_on_remote_audio_spectrum_update(callback_func), user_context);
    }

    void registerAudioDeviceStateChangedCallback(void *callback_func, void *user_context) {
        zego_register_audio_device_state_changed_callback(
            zego_on_audio_device_state_changed(callback_func), user_context);
    }

    void registerAudioDeviceVolumeChangedCallback(void *callback_func, void *user_context) {
        zego_register_audio_device_volume_changed_callback(
            zego_on_audio_device_volume_changed(callback_func), user_context);
    }

    void registerVideoDeviceStateChangedCallback(void *callback_func, void *user_context) {
        zego_register_video_device_state_changed_callback(
            zego_on_video_device_state_changed(callback_func), user_context);
    }

    void registerLocalDeviceExceptionOccurredCallback(void *callback_func, void *user_context) {
        zego_register_local_device_exception_occurred_callback(
            zego_on_local_device_exception_occurred(callback_func), user_context);
    }

    void registerAudioRouteChangeCallback(void *callback_func, void *user_context) {
        zego_register_audio_route_change_callback(zego_on_audio_route_change(callback_func),
                                                  user_context);
    }

    void registerRemoteCameraStateUpdateCallback(void *callback_func, void *user_context) {
        zego_register_remote_camera_state_update_callback(
            zego_on_remote_camera_state_update(callback_func), user_context);
    }

    void registerRemoteMicStateUpdateCallback(void *callback_func, void *user_context) {
        zego_register_remote_mic_state_update_callback(
            zego_on_remote_mic_state_update(callback_func), user_context);
    }

    void registerRemoteSpeakerStateUpdateCallback(void *callback_func, void *user_context) {
        zego_register_remote_speaker_state_update_callback(
            zego_on_remote_speaker_state_update(callback_func), user_context);
    }

    void registerRealTimeSequentialDataSentCallback(void *callback_func, void *user_context) {
        zego_register_real_time_sequential_data_sent_callback(
            zego_on_real_time_sequential_data_sent(callback_func), user_context);
    }

    void registerReceiveRealTimeSequentialDataCallback(void *callback_func, void *user_context) {
        zego_register_receive_real_time_sequential_data_callback(
            zego_on_receive_real_time_sequential_data(callback_func), user_context);
    }

    void registerIMSendBroadcastMessageResultCallback(void *callback_func, void *user_context) {
        zego_register_im_send_broadcast_message_result_callback(
            zego_on_im_send_broadcast_message_result(callback_func), user_context);
    }

    void registerIMRecvBroadcasMessageCallback(void *callback_func, void *user_context) {
        zego_register_im_recv_broadcast_message_callback(
            zego_on_im_recv_broadcast_message(callback_func), user_context);
    }

    void registerIMSendBarrageMessageResultCallback(void *callback_func, void *user_context) {
        zego_register_im_send_barrage_message_result_callback(
            zego_on_im_send_barrage_message_result(callback_func), user_context);
    }

    void registerIMRecvBarrageMessageResultCallback(void *callback_func, void *user_context) {
        zego_register_im_recv_barrage_message_callback(
            zego_on_im_recv_barrage_message(callback_func), user_context);
    }

    void registerIMSendCustomCommandResultCallback(void *callback_func, void *user_context) {
        zego_register_im_send_custom_command_result_callback(
            zego_on_im_send_custom_command_result(callback_func), user_context);
    }

    void registerIMRecvCustomCommandCallback(void *callback_func, void *user_context) {
        zego_register_im_recv_custom_command_callback(zego_on_im_recv_custom_command(callback_func),
                                                      user_context);
    }

    void registerIMSendTransparentMessageResultCallback(void *callback_func, void *user_context) {
        zego_register_send_transparent_message_result_callback(
            zego_on_send_transparent_message_result(callback_func), user_context);
    }

    void registerIMRecvRoomTransparentMessageCallback(void *callback_func, void *user_context) {
        zego_register_recv_room_transparent_message_callback(
            zego_on_recv_room_transparent_message(callback_func), user_context);
    }

    void registerMixerStartResultCallback(void *callback_func, void *user_context) {
        zego_register_mixer_start_result_callback(zego_on_mixer_start_result(callback_func),
                                                  user_context);
        zego_register_auto_mixer_start_result_callback(
            zego_on_auto_mixer_start_result(callback_func), user_context);
    }

    void registerMixerStopResultCallback(void *callback_func, void *user_context) {
        zego_register_mixer_stop_result_callback(zego_on_mixer_stop_result(callback_func),
                                                 user_context);
        zego_register_auto_mixer_stop_result_callback(zego_on_auto_mixer_stop_result(callback_func),
                                                      user_context);
    }

    void registerMixerSoundLevelUpdateCallback(void *callback_func, void *user_context) {
        zego_register_mixer_sound_level_update_callback(
            zego_on_mixer_sound_level_update(callback_func), user_context);
    }

    void registerAutoMixerSoundLevelUpdateCallback(void *callback_func, void *user_context) {
        zego_register_auto_mixer_sound_level_update_callback(
            zego_on_auto_mixer_sound_level_update(callback_func), user_context);
    }

    void registerMixerRelayCDNStateUpdateCallback(void *callback_func, void *user_context) {
        zego_register_mixer_relay_cdn_state_update_callback(
            zego_on_mixer_relay_cdn_state_update(callback_func), user_context);
    }

    void registerMediaPlayerStateUpdateCallback(void *callback_func, void *user_context) {
        zego_register_media_player_state_update_callback(
            zego_on_media_player_state_update(callback_func), user_context);
    }

    void registerMediaPlayerNetworkEventCallback(void *callback_func, void *user_context) {
        zego_register_media_player_network_event_callback(
            zego_on_media_player_network_event(callback_func), user_context);
    }

    void registerMediaPlayerPlayingProgressCallback(void *callback_func, void *user_context) {
        zego_register_media_player_playing_progress_callback(
            zego_on_media_player_playing_progress(callback_func), user_context);
    }

    void registerMediaPlayerRenderingProgressCallback(void *callback_func, void *user_context) {
        zego_register_media_player_rendering_progress_callback(
            zego_on_media_player_rendering_progress(callback_func), user_context);
    }

    void registerMediaPlayerVideoSizeChangedCallback(void *callback_func, void *user_context) {
        zego_register_media_player_video_size_changed_callback(
            zego_on_media_player_video_size_changed(callback_func), user_context);
    }

    void registerMediaPlayerRecvSEICallback(void *callback_func, void *user_context) {
        zego_register_media_player_recv_sei_callback(zego_on_media_player_recv_sei(callback_func),
                                                     user_context);
    }

    void registerMediaPlayerSoundLevelUpdateCallback(void *callback_func, void *user_context) {
        zego_register_media_player_sound_level_update_callback(
            zego_on_media_player_sound_level_update(callback_func), user_context);
    }

    void registerMediaPlayerFrequencySpectrumUpdateCallback(void *callback_func,
                                                            void *user_context) {
        zego_register_media_player_frequency_spectrum_update_callback(
            zego_on_media_player_frequency_spectrum_update(callback_func), user_context);
    }

    void registerMediaPlayerSeekToResult(void *callback_func, void *user_context) {
        zego_register_media_player_seek_to_callback(zego_on_media_player_seek_to(callback_func),
                                                    user_context);
    }

    void registerMediaPlayerLoadFileResult(void *callback_func, void *user_context) {
        zego_register_media_player_load_resource_callback(
            zego_on_media_player_load_resource(callback_func), user_context);
    }

    void registerMediaPlayerAudioDataCallback(void *callback_func, void *user_context) {
        zego_register_media_player_audio_frame_callback(
            zego_on_media_player_audio_frame(callback_func), user_context);
    }

    void registerMediaPlayerFirstFrameEventCallback(void *callback_func, void *user_context) {
        zego_register_media_player_first_frame_event_callback(
            zego_on_media_player_first_frame_event(callback_func), user_context);
    }

    void registerMediaPlayerLocalCacheCallback(void *callback_func, void *user_context) {
        zego_register_media_player_local_cache_callback(
            zego_on_media_player_local_cache(callback_func), user_context);
    }

    void registerMediaPlayerVideoDataCallback(void *callback_func, void *user_context) {
        zego_register_media_player_video_frame_callback(
            zego_on_media_player_video_frame(callback_func), user_context);
    }

    void registerMediaPlayerTakeSnapshotResultCallback(void *callback_func, void *user_context) {
        zego_register_media_player_take_snapshot_result_callback(
            zego_on_media_player_take_snapshot_result(callback_func), user_context);
    }

    void registerMediaPlayerBlockBeginCallback(void *callback_func, void *user_context) {
        zego_register_media_player_block_begin_callback(
            zego_on_media_player_block_begin(callback_func), user_context);
    }

    void registerMediaPlayerBlockDataCallback(void *callback_func, void *user_context) {
        zego_register_media_player_block_data_callback(
            zego_on_media_player_block_data(callback_func), user_context);
    }

    void registerAudioEffectPlayStateUpdateCallback(void *callback_func, void *user_context) {
        zego_register_audio_effect_play_state_update_callback(
            zego_on_audio_effect_play_state_update(callback_func), user_context);
    }

    void registerAudioEffectPlayerLoadResourceCallback(void *callback_func, void *user_context) {
        zego_register_audio_effect_player_load_resource_callback(
            zego_on_audio_effect_player_load_resource(callback_func), user_context);
    }

    void registerAudioEffectPlayerSeekToCallback(void *callback_func, void *user_context) {
        zego_register_audio_effect_player_seek_to_callback(
            zego_on_audio_effect_player_seek_to(callback_func), user_context);
    }

    void registerRangeAudioMicrophoneStateUpdate(void *callback_func, void *user_context) {
        zego_register_range_audio_microphone_state_update_callback(
            zego_on_range_audio_microphone_state_update(callback_func), user_context);
    }

    void registerCustomVideoRenderLocalFrameDataCallback(void *callback_func, void *user_context) {
        zego_register_custom_video_render_captured_frame_data_callback(
            zego_on_custom_video_render_captured_frame_data(callback_func), user_context);
    }

    void registerCustomVideoRenderRemoteFrameDataCallback(void *callback_func, void *user_context) {
        zego_register_custom_video_render_remote_frame_data_callback(
            zego_on_custom_video_render_remote_frame_data(callback_func), user_context);
    }

    void registerCustomVideoRenderRemoteFrameEncodedDataCallback(void *callback_func,
                                                                 void *user_context) {
        zego_register_custom_video_render_remote_frame_encoded_data_callback(
            zego_on_custom_video_render_remote_frame_encoded_data(callback_func), user_context);
    }

    void registerCustomVideoCaptureStartCallback(void *callback_func, void *user_context) {
        zego_register_custom_video_capture_start_callback(
            zego_on_custom_video_capture_start(callback_func), user_context);
    }

    void registerCustomVideoCaptureStopCallback(void *callback_func, void *user_context) {
        zego_register_custom_video_capture_stop_callback(
            zego_on_custom_video_capture_stop(callback_func), user_context);
    }

    void registerCustomVideoCaptureEncodedDataTrafficControlCallback(void *callback_func,
                                                                     void *user_context) {
        zego_register_custom_video_capture_encoded_data_traffic_control_callback(
            zego_on_custom_video_capture_encoded_data_traffic_control(callback_func), user_context);
    }

    void registerCustomVideoProcessCapturedRawDataCallback(void *callback_func,
                                                           void *user_context) {
        zego_register_custom_video_process_captured_unprocessed_raw_data_callback(
            zego_on_custom_video_process_captured_unprocessed_raw_data(callback_func),
            user_context);
    }

    void registerCustomVideoProcessCapturedCVPixelBufferCallback(void *callback_func,
                                                                 void *user_context) {
        zego_register_custom_video_process_captured_unprocessed_cvpixelbuffer_callback(
            zego_on_custom_video_process_captured_unprocessed_cvpixelbuffer(callback_func),
            user_context);
    }

    void registerAudioMixingCopyDataCallback(void *callback_func, void *user_context) {
        zego_register_copy_audio_mixing_data_callback(zego_on_copy_audio_mixing_data(callback_func),
                                                      user_context);
    }

    void registerOnCapturedAudioDataCallback(void *callback_func, void *user_context) {
        zego_register_captured_audio_data_callback(zego_on_captured_audio_data(callback_func),
                                                   user_context);
    }

    void registerOnPlaybackAudioDataCallback(void *callback_func, void *user_context) {
        zego_register_playback_audio_data_callback(zego_on_playback_audio_data(callback_func),
                                                   user_context);
    }

    void registerOnMixedAudioDataCallback(void *callback_func, void *user_context) {
        zego_register_mixed_audio_data_callback(zego_on_mixed_audio_data(callback_func),
                                                user_context);
    }

    void registerOnPlayerAudioDataCallback(void *callback_func, void *user_context) {
        zego_register_player_audio_data_callback(zego_on_player_audio_data(callback_func),
                                                 user_context);
    }

    void registerRecordingCapturedDataStateUpdateCallback(void *callback_func, void *user_context) {
        zego_register_captured_data_record_state_update_callback(
            zego_on_captured_data_record_state_update(callback_func), user_context);
    }

    void registerRecordingCapturedDataProgressUpdateCallback(void *callback_func,
                                                             void *user_context) {
        zego_register_captured_data_record_progress_update_callback(
            zego_on_captured_data_record_progress_update(callback_func), user_context);
    }

    void registerProcessCapturedAudioDataCallback(void *callback_func, void *user_context) {
        zego_register_process_captured_audio_data_callback(
            zego_on_process_captured_audio_data(callback_func), user_context);
    }
    void registerProcessCapturedAudioDataAfterUsedHeadphoneMonitorCallback(void *callback_func,
                                                                           void *user_context) {
        zego_register_process_captured_audio_data_after_used_headphone_monitor_callback(
            zego_on_process_captured_audio_data_after_used_headphone_monitor(callback_func),
            user_context);
    }
    void registerBeforeAudioPrepAudioDataCallback(void *callback_func, void *user_context) {
        zego_register_before_audio_prep_audio_data_callback(
            zego_on_before_audio_prep_audio_data(callback_func), user_context);
    }
    void registerProcessRemoteAudioDataCallback(void *callback_func, void *user_context) {
        zego_register_process_remote_audio_data_callback(
            zego_on_process_remote_audio_data(callback_func), user_context);
    }

    void registerProcessPlaybackAudioDataCallback(void *callback_func, void *user_context) {
        zego_register_process_playback_audio_data_callback(
            zego_on_process_playback_audio_data(callback_func), user_context);
    }

    void registerPerformanceStatueUpdateCallback(void *callback_func, void *user_context) {
        zego_register_performance_status_update_callback(
            zego_on_performance_status_update(callback_func), user_context);
    }

    void registerNetworkModeChangedCallback(void *callback_func, void *user_context) {
        zego_register_network_mode_changed_callback(zego_on_network_mode_changed(callback_func),
                                                    user_context);
    }

    void registerTestNetworkConnectivityCallback(void *callback_func, void *user_context) {
        zego_register_test_network_connectivity_callback(
            zego_on_test_network_connectivity(callback_func), user_context);
    }

    void registerNetworkSpeedTestQualityUpdateCallback(void *callback_func, void *user_context) {
        zego_register_network_speed_test_quality_update_callback(
            zego_on_network_speed_test_quality_update(callback_func), user_context);
    }

    void registerNetworkSpeedTestErrorCallback(void *callback_func, void *user_context) {
        zego_register_network_speed_test_error_callback(
            zego_on_network_speed_test_error(callback_func), user_context);
    }

    void registerNetworkProbeResultCallback(void *callback_func, void *user_context) {
        zego_register_network_probe_result_callback(zego_on_network_probe_result(callback_func),
                                                    user_context);
    }

    void registerRequestDumpDataCallback(void *callback_func, void *user_context) {
        zego_register_request_dump_data_callback(zego_on_request_dump_data(callback_func),
                                                 user_context);
    }

    void registerRequestUploadDumpDataCallback(void *callback_func, void *user_context) {
        zego_register_request_upload_dump_data_callback(
            zego_on_request_upload_dump_data(callback_func), user_context);
    }

    void registerStartDumpDataCallback(void *callback_func, void *user_context) {
        zego_register_start_dump_data_callback(zego_on_start_dump_data(callback_func),
                                               user_context);
    }

    void registerStopDumpDataCallback(void *callback_func, void *user_context) {
        zego_register_stop_dump_data_callback(zego_on_stop_dump_data(callback_func), user_context);
    }

    void registerUploadDumpDataCallback(void *callback_func, void *user_context) {
        zego_register_upload_dump_data_callback(zego_on_upload_dump_data(callback_func),
                                                user_context);
    }

    void registerRecvExperimentalAPICallback(void *callback_func, void *user_context) {
        zego_register_recv_experimental_api_callback(zego_on_recv_experimental_api(callback_func),
                                                     user_context);
    }

    void registerNetworkQualityCallback(void *callback_func, void *user_context) {
        zego_register_network_quality_callback(zego_on_network_quality(callback_func),
                                               user_context);
    }

    void registerRtcStatsCallback(void *callback_func, void *user_context) {
        zego_register_rtc_stats_callback(zego_on_rtc_stats(callback_func), user_context);
    }

    void registerCopyrightedMusicDownloadProcessUpdateCallback(void *callback_func,
                                                               void *user_context) {
        zego_register_copyrighted_music_download_progress_update_callback(
            zego_on_copyrighted_music_download_progress_update(callback_func), user_context);
    }

    void registerCopyrightedMusicCurrentPitchValueUpdateCallback(void *callback_func,
                                                                 void *user_context) {
        zego_register_copyrighted_music_current_pitch_value_update_callback(
            zego_on_copyrighted_music_current_pitch_value_update(callback_func), user_context);
    }

    void registerCopyrightedMusicInitCallback(void *callback_func, void *user_context) {
        zego_register_copyrighted_music_init_callback(zego_on_copyrighted_music_init(callback_func),
                                                      user_context);
    }

    void registerCopyrightedMusicSendExtendedRequestCallback(void *callback_func,
                                                             void *user_context) {
        zego_register_copyrighted_music_send_extended_request_callback(
            zego_on_copyrighted_music_send_extended_request(callback_func), user_context);
    }

    void registerCopyrightedMusicGetLrcLyricCallback(void *callback_func, void *user_context) {
        zego_register_copyrighted_music_get_lrc_lyric_callback(
            zego_on_copyrighted_music_get_lrc_lyric(callback_func), user_context);
    }

    void registerCopyrightedMusicGetKrcLyricByTokenCallback(void *callback_func,
                                                            void *user_context) {
        zego_register_copyrighted_music_get_krc_lyric_by_token_callback(
            zego_on_copyrighted_music_get_krc_lyric_by_token(callback_func), user_context);
    }

    void registerCopyrightedMusicRequestSongCallback(void *callback_func, void *user_context) {
        zego_register_copyrighted_music_request_song_callback(
            zego_on_copyrighted_music_request_song(callback_func), user_context);
    }

    void registerCopyrightedMusicRequestAccompanimentCallback(void *callback_func,
                                                              void *user_context) {
        zego_register_copyrighted_music_request_accompaniment_callback(
            zego_on_copyrighted_music_request_accompaniment(callback_func), user_context);
    }

    void registerCopyrightedMusicRequestAccompanimentClipCallback(void *callback_func,
                                                                  void *user_context) {
        zego_register_copyrighted_music_request_accompaniment_clip_callback(
            zego_on_copyrighted_music_request_accompaniment(callback_func), user_context);
    }

    void registerCopyrightedMusicGetMusicByTokenCallback(void *callback_func, void *user_context) {
        zego_register_copyrighted_music_get_music_by_token_callback(
            zego_on_copyrighted_music_get_music_by_token(callback_func), user_context);
    }

    void registerCopyrightedMusicRequestResourceCallback(void *callback_func, void *user_context) {
        zego_register_copyrighted_music_request_resource_callback(
            zego_on_copyrighted_music_request_resource(callback_func), user_context);
    }

    void registerCopyrightedMusicGetSharedResourceCallback(void *callback_func,
                                                           void *user_context) {
        zego_register_copyrighted_music_get_shared_resource_callback(
            zego_on_copyrighted_music_get_shared_resource(callback_func), user_context);
    }

    void registerCopyrightedMusicDownloadCallback(void *callback_func, void *user_context) {
        zego_register_copyrighted_music_download_callback(
            zego_on_copyrighted_music_download(callback_func), user_context);
    }

    void registerCopyrightedMusicGetStandardPitchCallback(void *callback_func, void *user_context) {
        zego_register_copyrighted_music_get_standard_pitch_callback(
            zego_on_copyrighted_music_get_standard_pitch(callback_func), user_context);
    }

    void registerScreenCaptureSourceAvailableFrameCallback(void *callback_func,
                                                           void *user_context) {
        zego_register_screen_capture_source_available_frame_callback(
            zego_on_screen_capture_source_available_frame(callback_func), user_context);
    }

    void registerScreenCaptureSourceExceptionOccurredCallback(void *callback_func,
                                                              void *user_context) {
        zego_register_screen_capture_source_exception_occurred_callback(
            zego_on_screen_capture_source_exception_occurred(callback_func), user_context);
    }

    void registerScreenCaptureSourceCaptureTypeExceptionOccurredCallback(void *callback_func,
                                                                         void *user_context) {
        zego_register_screen_capture_source_capture_type_exception_occurred_callback(
            zego_on_screen_capture_source_capture_type_exception_occurred(callback_func),
            user_context);
    }

    void registerScreenCaptureSourceWindowStateCallback(void *callback_func, void *user_context) {
        zego_register_screen_capture_window_state_changed_callback(
            zego_on_screen_capture_window_state_changed(callback_func), user_context);
    }

    void registerScreenCaptureSourceCaptureRectCallback(void *callback_func, void *user_context) {
        zego_register_screen_capture_rect_changed_callback(
            zego_on_screen_capture_rect_changed(callback_func), user_context);
    }

    void registerNetworkTimeSynchronizedCallback(void *callback_func, void *user_context) {
        zego_register_network_time_synchronized_callback(
            zego_on_network_time_synchronized(callback_func), user_context);
    }

    void registerMediaDataPublisherFileOpenCallback(void *callback_func, void *user_context) {
        zego_register_media_data_publisher_file_open_callback(
            zego_on_media_data_publisher_file_open(callback_func), user_context);
    }

    void registerMediaDataPublisherFileCloseCallback(void *callback_func, void *user_context) {
        zego_register_media_data_publisher_file_close_callback(
            zego_on_media_data_publisher_file_close(callback_func), user_context);
    }

    void registerMediaDataPublisherFileDataBeginCallback(void *callback_func, void *user_context) {
        zego_register_media_data_publisher_file_data_begin_callback(
            zego_on_media_data_publisher_file_data_begin(callback_func), user_context);
    }

    void registerMediaDataPublisherFileDataEndCallback(void *callback_func, void *user_context) {
        zego_register_media_data_publisher_file_data_end_callback(
            zego_on_media_data_publisher_file_data_end(callback_func), user_context);
    }

    void registerRangeSceneSceneStateUpdateCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_scene_state_update_callback(
            zego_on_range_scene_scene_state_update(callback_func), user_context);
    }
    void registerRangeSceneEnterViewCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_enter_view_callback(zego_on_range_scene_enter_view(callback_func),
                                                      user_context);
    }
    void registerRangeSceneLeaveViewCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_leave_view_callback(zego_on_range_scene_leave_view(callback_func),
                                                      user_context);
    }
    void registerRangeSceneUserStatusUpdateCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_user_status_update_callback(
            zego_on_range_scene_user_status_update(callback_func), user_context);
    }
    void registerRangeSceneUserCommandUpdateCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_user_command_update_callback(
            zego_on_range_scene_user_command_update(callback_func), user_context);
    }
    void registerRangeSceneCustomCommandUpdateCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_custom_command_update_callback(
            zego_on_range_scene_custom_command_update(callback_func), user_context);
    }
    void registerRangeSceneTokenWillExpireCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_token_will_expire_callback(
            zego_on_range_scene_token_will_expire(callback_func), user_context);
    }
    void registerRangeSceneLoginSceneCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_login_scene_callback(
            zego_on_range_scene_login_scene(callback_func), user_context);
    }
    void registerRangeSceneLogoutSceneCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_logout_scene_callback(
            zego_on_range_scene_logout_scene(callback_func), user_context);
    }
    void registerRangeSceneGetUserCountCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_get_user_count_callback(
            zego_on_range_scene_get_user_count(callback_func), user_context);
    }
    void registerRangeSceneGetUserListInViewCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_get_user_list_in_view_callback(
            zego_on_range_scene_get_user_list_in_view(callback_func), user_context);
    }
    void registerRangeSceneSendCustomCommandCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_send_custom_command_callback(
            zego_on_range_scene_send_custom_command(callback_func), user_context);
    }
    void registerRangeSceneStreamUserStreamStateUpdateCallback(void *callback_func,
                                                               void *user_context) {
        zego_register_range_scene_stream_user_stream_state_update_callback(
            zego_on_range_scene_stream_user_stream_state_update(callback_func), user_context);
    }
    void registerRangeSceneStreamUserMicUpdateCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_stream_user_mic_update_callback(
            zego_on_range_scene_stream_user_mic_update(callback_func), user_context);
    }

    void registerRangeSceneStreamUserCameraUpdateCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_stream_user_camera_update_callback(
            zego_on_range_scene_stream_user_camera_update(callback_func), user_context);
    }
    void registerRangeSceneStreamUserSpeakerUpdateCallback(void *callback_func,
                                                           void *user_context) {
        zego_register_range_scene_stream_user_speaker_update_callback(
            zego_on_range_scene_stream_user_speaker_update(callback_func), user_context);
    }
    void registerRangeSceneTeamTeamStateUpdateCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_team_team_state_update_callback(
            zego_on_range_scene_team_team_state_update(callback_func), user_context);
    }
    void registerRangeSceneTeamTeamMemberUpdateCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_team_team_member_update_callback(
            zego_on_range_scene_team_team_member_update(callback_func), user_context);
    }
    void registerRangeSceneTeamJoinTeamCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_team_join_team_callback(
            zego_on_range_scene_team_join_team(callback_func), user_context);
    }
    void registerRangeSceneTeamLeaveTeamCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_team_leave_team_callback(
            zego_on_range_scene_team_leave_team(callback_func), user_context);
    }

    void registerRangeSceneItemItemEnterViewCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_item_item_enter_view_callback(
            zego_on_range_scene_item_item_enter_view(callback_func), user_context);
    }

    void registerRangeSceneItemItemLeaveViewCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_item_item_leave_view_callback(
            zego_on_range_scene_item_item_leave_view(callback_func), user_context);
    }

    void registerRangeSceneItemItemBindUpdateCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_item_item_bind_update_callback(
            zego_on_range_scene_item_item_bind_update(callback_func), user_context);
    }

    void registerRangeSceneItemItemUnbindUpdateCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_item_item_unbind_update_callback(
            zego_on_range_scene_item_item_unbind_update(callback_func), user_context);
    }

    void registerRangeSceneItemItemStatusUpdateCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_item_item_status_update_callback(
            zego_on_range_scene_item_item_status_update(callback_func), user_context);
    }

    void registerRangeSceneItemItemCommandUpdateCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_item_item_command_update_callback(
            zego_on_range_scene_item_item_command_update(callback_func), user_context);
    }

    void registerRangeSceneItemCreateItemCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_item_create_item_callback(
            zego_on_range_scene_item_create_item(callback_func), user_context);
    }

    void registerRangeSceneItemDestroyItemCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_item_destroy_item_callback(
            zego_on_range_scene_item_destroy_item(callback_func), user_context);
    }

    void registerRangeSceneItemBindItemCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_item_bind_item_callback(
            zego_on_range_scene_item_bind_item(callback_func), user_context);
    }

    void registerRangeSceneItemUnbindItemCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_item_unbind_item_callback(
            zego_on_range_scene_item_unbind_item(callback_func), user_context);
    }

    void registerRangeSceneItemUpdateItemStatusCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_item_update_item_status_callback(
            zego_on_range_scene_item_update_item_status(callback_func), user_context);
    }

    void registerRangeSceneItemUpdateItemCommandCallback(void *callback_func, void *user_context) {
        zego_register_range_scene_item_update_item_command_callback(
            zego_on_range_scene_item_update_item_command(callback_func), user_context);
    }

    void registerVideoObjectSegmentationStateChangedCallback(void *callback_func,
                                                             void *user_context) {
        zego_register_video_object_segmentation_state_changed_callback(
            (zego_on_video_object_segmentation_state_changed)callback_func, user_context);
    }

    void registerAIVoiceChangerInitCallback(void *callback_func, void *user_context) {
        zego_register_ai_voice_changer_init_callback((zego_on_ai_voice_changer_init)callback_func,
                                                     user_context);
    }

    void registerAIVoiceChangerUpdateProgressCallback(void *callback_func, void *user_context) {
        zego_register_ai_voice_changer_update_progress_callback(
            (zego_on_ai_voice_changer_update_progress)callback_func, user_context);
    }

    void registerAIVoiceChangerUpdateCallback(void *callback_func, void *user_context) {
        zego_register_ai_voice_changer_update_callback(
            (zego_on_ai_voice_changer_update)callback_func, user_context);
    }

    void registerAIVoiceChangerGetSpeakerListCallback(void *callback_func, void *user_context) {
        zego_register_ai_voice_changer_get_speaker_list_callback(
            (zego_on_ai_voice_changer_get_speaker_list)callback_func, user_context);
    }

    void registerAIVoiceChangerEventCallback(void *callback_func, void *user_context) {
        zego_register_ai_voice_changer_event_callback((zego_on_ai_voice_changer_event)callback_func,
                                                      user_context);
    }

    void registerAIVoiceChangerSetSpeakerCallback(void *callback_func, void *user_context) {
        zego_register_ai_voice_changer_set_speaker_callback(
            (zego_on_ai_voice_changer_set_speaker)callback_func, user_context);
    }

  private:
    ZegoExpressEngineBridge() { pri = new ZegoExpressEngineBridgePri(); }

    ~ZegoExpressEngineBridge() { delete pri; }

    ZegoExpressEngineBridgePri *pri = nullptr;

#ifdef ZEGOEXP_EXPLICIT
    bool library_ready = false;
    ZEGOEXP_DECLARE_FUNC
#else
    bool library_ready = true;
#endif

#if defined(_WIN32) || TARGET_OS_OSX
    std::atomic<bool> callback_switch_to_main_thread_ = {true}; //only win and mac
#endif
};
#define oInternalOriginBridge ZegoExpressEngineBridge::GetInstance()

} // namespace EXPRESS
} // namespace ZEGO

ZEGO_ENABLE_DEPRECATION_WARNINGS
