#pragma once

#include "../ZegoExpressInterface.h"
#include "ZegoInternalAIVoiceChanger.hpp"
#include "ZegoInternalAudioEffectPlayer.hpp"
#include "ZegoInternalBase.h"
#include "ZegoInternalBridge.h"
#include "ZegoInternalCallbackImpl.hpp"
#include "ZegoInternalCopyrightedMusic.hpp"
#include "ZegoInternalExplicit.hpp"
#include "ZegoInternalMediaDataPublisher.hpp"
#include "ZegoInternalMediaPlayer.hpp"
#include "ZegoInternalRangeAudio.hpp"
#include "ZegoInternalRealTimeSequentialDataManager.hpp"
#include "ZegoInternalScreenCaptureSource.hpp"

ZEGO_DISABLE_DEPRECATION_WARNINGS

namespace ZEGO {
namespace EXPRESS {
class ZegoExpressEngineImp : public IZegoExpressEngine {
  public:
    using ZegoExpressEngineImpPtr = ZegoExpressEngineImp *;
    void setEventHandler(std::shared_ptr<IZegoEventHandler> eventHandler) override {
        if (oInternalCallbackCenter->getIZegoEventHandler() && eventHandler) {
            oInternalOriginBridge->apiCallResult(__func__,
                                                 ZEGO_ERROR_CODE_COMMON_EVENT_HANDLER_EXISTS);
        } else {
            oInternalCallbackCenter->setIZegoEventHandler(eventHandler);
        }
    }

    void setRoomScenario(ZegoScenario scenario) override {
        oInternalOriginBridge->setRoomScenario(zego_scenario(scenario));
    }

    void uploadLog() override { oInternalOriginBridge->uploadLog(); }

    void uploadLog(ZegoUploadLogResultCallback callback) override {
        int seq = oInternalOriginBridge->uploadLog();
        if (callback != nullptr) {
            oInternalCallbackCenter->insertZegoUploadLogResultCallback(seq, callback);
        }
    }

    static void submitLog() { oInternalOriginBridge->uploadLog(); }

    void enableDebugAssistant(bool enable) override {
        oInternalOriginBridge->enableDebugAssistant(enable);
    }

    std::string callExperimentalAPI(const std::string &params) override {
        return oInternalOriginBridge->callExperimentalAPI(params);
    }

    void setDummyCaptureImagePath(const std::string &filePath,
                                  ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override {
        return oInternalOriginBridge->setDummyCaptureImagePath(filePath, channel);
    }

    void
    setDummyCaptureImageParams(ZegoDummyCaptureImageParams params,
                               ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override {
        auto i_params = ZegoExpressConvert::O2IDummyCaptureImageParams(params);
        oInternalOriginBridge->setDummyCaptureImageParams(i_params, channel);
    }

    ///===================================================================================================
    void loginRoom(const std::string &roomID, ZegoUser user) override {
        ZegoRoomConfig config;
        loginRoom(roomID, user, config, nullptr);
    }

    void loginRoom(const std::string &roomID, ZegoUser user, ZegoRoomConfig config,
                   ZegoRoomLoginCallback callback) override {
        const char *_roomId = roomID.c_str();
        zego_user _user = ZegoExpressConvert::O2IUser(user);
        zego_room_config _config = ZegoExpressConvert::O2IRoomConfig(config);

        int seq = oInternalOriginBridge->loginRoomWithCallback(_roomId, _user, &_config);
        if (callback != nullptr) {
            oInternalCallbackCenter->insertZegoRoomLoginCallback(seq, callback);
        }
    }

    void logoutRoom(ZegoRoomLogoutCallback callback) override {
        if (callback != nullptr) {
            int seq = oInternalOriginBridge->logoutRoomWithCallback();
            oInternalCallbackCenter->insertZegoRoomLogoutCallback(seq, callback);
        } else {
            oInternalOriginBridge->logoutRoom();
        }
    }

    void logoutRoom(const std::string &roomID, ZegoRoomLogoutCallback callback) override {
        if (callback != nullptr) {
            int seq = oInternalOriginBridge->logoutRoomWithCallback(roomID.c_str());
            oInternalCallbackCenter->insertZegoRoomLogoutCallback(seq, callback);
        } else {
            const char *_roomId = roomID.c_str();
            oInternalOriginBridge->logoutRoom(_roomId);
        }
    }

    void switchRoom(const std::string &fromRoomID, const std::string &toRoomID,
                    ZegoRoomConfig *config) override {
        const char *from_room_id = fromRoomID.c_str();
        const char *to_room_id = toRoomID.c_str();
        if (config == nullptr) {
            oInternalOriginBridge->switchRoom(from_room_id, to_room_id, nullptr);
        } else {
            zego_room_config _config = ZegoExpressConvert::O2IRoomConfig(*config);
            oInternalOriginBridge->switchRoom(from_room_id, to_room_id, &_config);
        }
    }

    void renewToken(const std::string &roomID, const std::string &token) override {
        oInternalOriginBridge->renewToken(roomID.c_str(), token.c_str());
    }

    void setRoomExtraInfo(const std::string &roomID, const std::string &key,
                          const std::string &value,
                          ZegoRoomSetRoomExtraInfoCallback callback) override {
        int seq =
            oInternalOriginBridge->setRoomExtraInfo(roomID.c_str(), key.c_str(), value.c_str());
        if (callback != nullptr) {
            oInternalCallbackCenter->insertZegoRoomSetRoomExtraInfoCallback(seq, callback);
        }
    }

    ZegoRoomStreamList getRoomStreamList(const std::string &roomID,
                                         ZegoRoomStreamListType streamListType) override {
        zego_room_stream_list_type type = (zego_room_stream_list_type)streamListType;

        auto stream_list = oInternalOriginBridge->getRoomStreamList(roomID.c_str(), type);
        auto list = ZegoExpressConvert::I2ORoomStreamList(stream_list);

        oInternalOriginBridge->freeRoomStreamList(stream_list);

        return list;
    }

    ///===================================================================================================
    void startPublishingStream(const std::string &streamID, ZegoPublishChannel channel) override {
        const char *stream_id = streamID.c_str();
        oInternalOriginBridge->startPublishingStream(stream_id, zego_publish_channel(channel));
    }

    void startPublishingStream(const std::string &streamID, ZegoPublisherConfig config,
                               ZegoPublishChannel channel) override {
        const char *stream_id = streamID.c_str();
        zego_publisher_config _config = ZegoExpressConvert::O2IPublisherConfig(config);
        oInternalOriginBridge->startPublishingStreamWithConfig(
            stream_id, zego_publish_channel(channel), _config);
    }

    void startPublishingStreamInScene(const std::string &streamID, ZegoPublishChannel channel,
                                      ZegoScenePublisherConfig config) override {
        const char *stream_id = streamID.c_str();
        zego_scene_publisher_config _config =
            ZegoExpressConvert::O2IZegoScenePublisherConfig(config);
        oInternalOriginBridge->startPublishingStreamInScene(stream_id,
                                                            zego_publish_channel(channel), _config);
    }

    void stopPublishingStream(ZegoPublishChannel channel) override {
        oInternalOriginBridge->stopPublishingStream(zego_publish_channel(channel));
    }

    void setStreamExtraInfo(const std::string &extraInfo,
                            ZegoPublisherSetStreamExtraInfoCallback callback,
                            ZegoPublishChannel channel) override {
        const char *_extraInfo = extraInfo.c_str();
        int seq =
            oInternalOriginBridge->setStreamExtraInfo(_extraInfo, zego_publish_channel(channel));
        if (callback != nullptr) {
            oInternalCallbackCenter->insertZegoPublisherSetStreamExtraInfoCallback(seq, callback);
        }
    }

    void startPreview(ZegoCanvas *canvas, ZegoPublishChannel channel) override {
        if (canvas == nullptr) {
            oInternalOriginBridge->startPreview(nullptr, zego_publish_channel(channel));
        } else {
            zego_canvas _canvas;
            _canvas.view = canvas->view;
            _canvas.view_mode = zego_view_mode(canvas->viewMode);
            _canvas.background_color = canvas->backgroundColor;
            _canvas.alpha_blend = canvas->alphaBlend;
            oInternalOriginBridge->startPreview(&_canvas, zego_publish_channel(channel));
        }
    }

    void stopPreview(ZegoPublishChannel channel) override {
        oInternalOriginBridge->stopPreview(zego_publish_channel(channel));
    }

    void startPreview() override {
        oInternalOriginBridge->startPreview(nullptr, zego_publish_channel_main);
    }

    void stopPreview() override { oInternalOriginBridge->stopPreview(zego_publish_channel_main); }

    void setReverbPreset(ZegoReverbPreset preset) override {
        oInternalOriginBridge->setReverbPreset(zego_reverb_preset(preset));
    }

    void setReverbAdvancedParam(ZegoReverbAdvancedParam param) override {
        zego_reverb_advanced_param _param;

        _param.damping = param.damping;
        _param.reverberance = param.reverberance;
        _param.room_size = param.roomSize;
        _param.pre_delay = param.preDelay;
        _param.tone_low = param.toneLow;
        _param.tone_high = param.toneHigh;
        _param.wet_gain = param.wetGain;
        _param.dry_gain = param.dryGain;
        _param.stereo_width = param.stereoWidth;
        _param.wet_only = param.wetOnly;

        oInternalOriginBridge->setReverbAdvancedParam(_param);
    }

    void setReverbEchoParam(ZegoReverbEchoParam param) override {
        zego_reverb_echo_param _param;
        _param.in_gain = param.inGain;
        _param.out_gain = param.outGain;
        _param.num_delays = param.numDelays;
        for (int i = 0; i < 7; i++) {
            _param.delay[i] = param.delay[i];
            _param.decay[i] = param.decay[i];
        }
        oInternalOriginBridge->setReverbEchoParam(_param);
    }

    void enableVirtualStereo(bool enable, int angle) override {
        oInternalOriginBridge->enableVirtualStereo(enable, angle);
    }

    void enablePlayStreamVirtualStereo(bool enable, int angle,
                                       const std::string &streamID) override {
        oInternalOriginBridge->enablePlayStreamVirtualStereo(enable, angle, streamID.c_str());
    }

    void setElectronicEffects(bool enable, ZegoElectronicEffectsMode mode, int tonal) override {
        oInternalOriginBridge->setElectronicEffects(enable, zego_electronic_effects_mode(mode),
                                                    tonal);
    }

    void setVoiceChangerPreset(ZegoVoiceChangerPreset preset) override {
        oInternalOriginBridge->setVoiceChangerPreset(zego_voice_changer_preset(preset));
    }

    void setVoiceChangerParam(ZegoVoiceChangerParam param) override {
        oInternalOriginBridge->setVoiceChangerParam(param.pitch);
    }

    void setVideoConfig(ZegoVideoConfig videoConfig, ZegoPublishChannel channel) override {
        zego_video_config _config = ZegoExpressConvert::O2IVideoConfig(videoConfig);
        oInternalOriginBridge->setVideoConfig(_config, zego_publish_channel(channel));
    }

    ZegoVideoConfig getVideoConfig(ZegoPublishChannel channel) override {
        zego_video_config video_config =
            oInternalOriginBridge->getVideoConfig(zego_publish_channel(channel));
        auto videoConfig = ZegoExpressConvert::I2OVideoConfig(video_config);
        return videoConfig;
    }

    void setPublishDualStreamConfig(const std::vector<ZegoPublishDualStreamConfig> &configList,
                                    ZegoPublishChannel channel) override {
        std::vector<zego_publish_dual_stream_config> config_list;
        for (const auto &config : configList) {
            zego_publish_dual_stream_config stream_config;
            stream_config.stream_type = (zego_video_stream_type)config.streamType;
            stream_config.encode_height = config.encodeHeight;
            stream_config.encode_width = config.encodeWidth;
            stream_config.fps = config.fps;
            stream_config.bitrate = config.bitrate;

            config_list.push_back(stream_config);
        }

        zego_publish_dual_stream_config *sConfig = config_list.data();
        oInternalOriginBridge->setPublishDualStreamConfig(sConfig, ZegoUInt(config_list.size()),
                                                          zego_publish_channel(channel));
    }

    void setVideoMirrorMode(ZegoVideoMirrorMode mirrorMode, ZegoPublishChannel channel) override {
        oInternalOriginBridge->setVideoMirrorMode(zego_video_mirror_mode(mirrorMode),
                                                  zego_publish_channel(channel));
    }

#if (TARGET_OS_IPHONE || defined(ANDROID)) || defined(_OS_OHOS_)
    void setAppOrientation(ZegoOrientation orientation, ZegoPublishChannel channel) override {
        oInternalOriginBridge->setAppOrientation((zego_orientation)orientation,
                                                 zego_publish_channel(channel));
    }

    void setAppOrientationMode(ZegoOrientationMode mode) override {
        oInternalOriginBridge->setAppOrientationMode((zego_orientation_mode)mode);
    }
#endif

    void setAudioConfig(ZegoAudioConfig audioConfig, ZegoPublishChannel channel) override {
        zego_audio_config config = ZegoExpressConvert::O2IAudioConfig(audioConfig);
        oInternalOriginBridge->setAudioConfig(config, zego_publish_channel(channel));
    }

    ZegoAudioConfig getAudioConfig(ZegoPublishChannel channel) override {
        zego_audio_config _config =
            oInternalOriginBridge->getAudioConfig(zego_publish_channel(channel));
        ZegoAudioConfig config = ZegoExpressConvert::I2OAudioConfig(_config);
        return config;
    }

    void setPublishStreamEncryptionKey(const std::string &key,
                                       ZegoPublishChannel channel) override {
        const char *encryption_key = key.c_str();
        oInternalOriginBridge->setPublishStreamEncryptionKey(encryption_key,
                                                             zego_publish_channel(channel));
    }

    void
    takePublishStreamSnapshot(ZegoPublisherTakeSnapshotCallback callback,
                              ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override {
        oInternalOriginBridge->takePublishStreamSnapshot(zego_publish_channel(channel));
        if (callback != nullptr) {
            oInternalCallbackCenter->insertZegoPublisherTakeSnapshotCallback((int)channel,
                                                                             callback);
        }
    }

    void mutePublishStreamAudio(bool mute, ZegoPublishChannel channel) override {
        oInternalOriginBridge->mutePublishStreamAudio(mute, zego_publish_channel(channel));
    }

    void mutePublishStreamVideo(bool mute, ZegoPublishChannel channel) override {
        oInternalOriginBridge->mutePublishStreamVideo(mute, zego_publish_channel(channel));
    }

    void setStreamAlignmentProperty(int alignment, ZegoPublishChannel channel) override {
        oInternalOriginBridge->setStreamAlignmentProperty(alignment, zego_publish_channel(channel));
    }

    void enableTrafficControl(bool enable, int property, ZegoPublishChannel channel) override {
        oInternalOriginBridge->enableTrafficControl(enable, property,
                                                    zego_publish_channel(channel));
    }

    void setMinVideoBitrateForTrafficControl(int bitrate,
                                             ZegoTrafficControlMinVideoBitrateMode mode,
                                             ZegoPublishChannel channel) override {
        oInternalOriginBridge->setMinVideoBitrateForTrafficControl(
            bitrate, zego_traffic_control_min_video_bitrate_mode(mode),
            zego_publish_channel(channel));
    }

    void setMinVideoFpsForTrafficControl(int fps, ZegoPublishChannel channel) override {
        oInternalOriginBridge->setMinVideoFpsForTrafficControl(fps, zego_publish_channel(channel));
    }

    void setMinVideoResolutionForTrafficControl(int width, int height,
                                                ZegoPublishChannel channel) override {
        oInternalOriginBridge->setMinVideoResolutionForTrafficControl(
            width, height, zego_publish_channel(channel));
    }

    void setTrafficControlFocusOn(ZegoTrafficControlFocusOnMode mode,
                                  ZegoPublishChannel channel) override {
        oInternalOriginBridge->setTrafficControlFocusOn(zego_traffic_control_focus_on_mode(mode),
                                                        zego_publish_channel(channel));
    }

    void setCaptureVolume(int volume) override { oInternalOriginBridge->setCaptureVolume(volume); }

    int getCaptureVolume() override { return oInternalOriginBridge->getCaptureVolume(); }

    void setAudioCaptureStereoMode(ZegoAudioCaptureStereoMode mode) override {
        oInternalOriginBridge->setAudioCaptureStereoMode(zego_audio_capture_stereo_mode(mode));
    }

    void addPublishCdnUrl(const std::string &streamID, const std::string &targetURL,
                          ZegoPublisherUpdateCdnUrlCallback callback) override {
        addPublishCdnUrl(streamID, targetURL, 0, callback);
    }

    void addPublishCdnUrl(const std::string &streamID, const std::string &targetURL, int timeout,
                          ZegoPublisherUpdateCdnUrlCallback callback) override {
        const char *stream_id = streamID.c_str();
        const char *target_url = targetURL.c_str();

        int seq = oInternalOriginBridge->addPublishCdnUrl(stream_id, target_url, timeout);
        if (callback != nullptr) {
            oInternalCallbackCenter->insertZegoPublisherUpdateCdnUrlCallback(seq, callback);
        }
    }

    void removePublishCdnUrl(const std::string &streamID, const std::string &targetURL,
                             ZegoPublisherUpdateCdnUrlCallback callback) override {
        const char *stream_id = streamID.c_str();
        const char *target_url = targetURL.c_str();

        int seq = oInternalOriginBridge->removePublishCdnUrl(stream_id, target_url);
        if (callback != nullptr) {
            oInternalCallbackCenter->insertZegoPublisherUpdateCdnUrlCallback(seq, callback);
        }
    }

    void enablePublishDirectToCDN(bool enable, ZegoCDNConfig *config,
                                  ZegoPublishChannel channel) override {
        if (config == nullptr) {
            oInternalOriginBridge->enablePublishDirectToCDN(enable, nullptr,
                                                            zego_publish_channel(channel));
        } else {
            zego_cdn_config _config = ZegoExpressConvert::O2ICDNConfig(*config);
            oInternalOriginBridge->enablePublishDirectToCDN(enable, &_config,
                                                            zego_publish_channel(channel));
        }
    }

    void setPublishWatermark(ZegoWatermark *watermark, bool isPreviewVisible,
                             ZegoPublishChannel channel) override {
        if (watermark != nullptr) {
            zego_watermark _watermark = ZegoExpressConvert::O2IWatermark(*watermark);
            oInternalOriginBridge->setPublishWatermark(&_watermark, isPreviewVisible,
                                                       zego_publish_channel(channel));
        } else {
            oInternalOriginBridge->setPublishWatermark(nullptr, isPreviewVisible,
                                                       zego_publish_channel(channel));
        }
    }

    void setSEIConfig(ZegoSEIConfig config) override {
        zego_sei_config _config = ZegoExpressConvert::O2SEIConfig(config);
        oInternalOriginBridge->setSEIConfig(_config);
    }

    void sendSEI(const unsigned char *buffer, unsigned int buffer_length,
                 ZegoPublishChannel channel) override {
        oInternalOriginBridge->sendSEI(buffer, buffer_length, zego_publish_channel(channel));
    }

    void sendSEISyncWithCustomVideo(const unsigned char *data, unsigned int dataLength,
                                    unsigned long long timeStampNs,
                                    ZegoPublishChannel channel) override {
        oInternalOriginBridge->sendSEISyncWithCustomVideo(data, dataLength, timeStampNs,
                                                          (zego_publish_channel)channel);
    }

    void sendAudioSideInfo(const unsigned char *data, unsigned int dataLength, double timeStampMs,
                           ZegoPublishChannel channel) override {
        oInternalOriginBridge->sendAudioSideInfo(data, dataLength, timeStampMs,
                                                 (zego_publish_channel)channel);
    }

    void enableHardwareEncoder(bool enable) override {
        oInternalOriginBridge->enableHardwareEncoder(enable);
    }

    void setCapturePipelineScaleMode(ZegoCapturePipelineScaleMode mode) override {
        zego_capture_pipeline_scale_mode _mode = zego_capture_pipeline_scale_mode(mode);
        oInternalOriginBridge->setCapturePipelineScaleMode(_mode);
    }

    void enableH265EncodeFallback(bool enable) override {
        oInternalOriginBridge->enableH265EncodeFallback(enable);
    }

    bool isVideoEncoderSupported(ZegoVideoCodecID codecID) override {
        return isVideoEncoderSupported(codecID, ZEGO_VIDEO_CODEC_BACKEND_ANY) != 0;
    }

    int isVideoEncoderSupported(ZegoVideoCodecID codecID,
                                ZegoVideoCodecBackend codecBackend) override {
        return oInternalOriginBridge->isVideoEncoderSupported(
            static_cast<zego_video_codec_id>(codecID),
            static_cast<zego_video_codec_backend>(codecBackend));
    }

    void enableAuxBgmBalance(bool enable) override {
        oInternalOriginBridge->enableAuxBgmBalance(enable);
    }

    ///===================================================================================================
    void startPlayingStream(const std::string &streamID, ZegoCanvas *canvas) override {
        const char *stream_id = streamID.c_str();
        if (canvas == nullptr) {
            oInternalOriginBridge->startPlayingStream(stream_id, nullptr);
        } else {
            zego_canvas _canvas = ZegoExpressConvert::O2ICanvas(*canvas);
            oInternalOriginBridge->startPlayingStream(stream_id, &_canvas);
        }
    }

    void startPlayingStream(const std::string &streamID, ZegoCanvas *canvas,
                            ZegoPlayerConfig config) override {
        const char *stream_id = streamID.c_str();
        zego_canvas _canvas;
        if (canvas) {
            _canvas = ZegoExpressConvert::O2ICanvas(*canvas);
        }
        zego_player_config _config;
        memset(&_config, 0, sizeof(zego_player_config));
        zego_cdn_config cdn_cofig;
        memset(&cdn_cofig, 0, sizeof(zego_cdn_config));
        if (config.cdnConfig) {
            cdn_cofig = ZegoExpressConvert::O2ICDNConfig(*config.cdnConfig);
            _config.cdn_config = &cdn_cofig;
        } else {
            _config.cdn_config = nullptr;
        }
        _config.resource_mode = zego_stream_resource_mode(config.resourceMode);
        strncpy(_config.room_id, config.roomID.c_str(), ZEGO_EXPRESS_MAX_ROOMID_LEN);
        _config.video_codec_id = zego_video_codec_id(config.videoCodecID);
        _config.source_resource_type = zego_resource_type(config.sourceResourceType);
        _config.codec_template_id = config.codecTemplateID;
        _config.resource_switch_mode = zego_stream_resource_switch_mode(config.resourceSwitchMode);
        _config.resource_when_stop_publish =
            zego_stream_resource_type(config.resourceWhenStopPublish);

        _config.adaptive_switch = config.adaptiveSwitch;
        _config.adaptive_template_id_count = config.adaptiveTemplateIDList.size();
        _config.adaptive_template_id_list = nullptr;
        if (_config.adaptive_switch > 0 && _config.adaptive_template_id_count > 0) {
            _config.adaptive_template_id_list = new int[_config.adaptive_template_id_count];
            for (int i = 0; i < _config.adaptive_template_id_count; ++i) {
                _config.adaptive_template_id_list[i] = config.adaptiveTemplateIDList[i];
            }
        }

        struct zego_custom_player_resource_config customResourceConfig;
        customResourceConfig.before_publish =
            zego_resource_type(config.customResourceConfig.beforePublish);
        customResourceConfig.publishing =
            zego_resource_type(config.customResourceConfig.publishing);
        customResourceConfig.after_publish =
            zego_resource_type(config.customResourceConfig.afterPublish);
        _config.custom_resource_config = &customResourceConfig;

        oInternalOriginBridge->startPlayingStreamWithConfig(stream_id, canvas ? &_canvas : nullptr,
                                                            _config);

        if (_config.adaptive_template_id_list)
            delete[] _config.adaptive_template_id_list;
    }

    void startPlayingStream(const std::string &streamID) override {
        const char *stream_id = streamID.c_str();
        oInternalOriginBridge->startPlayingStream(stream_id, nullptr);
    }

    void startPlayingStream(const std::string &streamID, ZegoPlayerConfig config) override {
        startPlayingStream(streamID, nullptr, config);
    }

    virtual void startPlayingStreamInScene(const std::string &streamID, ZegoCanvas *canvas,
                                           ZegoScenePlayerConfig config) override {
        const char *stream_id = streamID.c_str();
        zego_canvas _canvas;
        if (canvas) {
            _canvas = ZegoExpressConvert::O2ICanvas(*canvas);
        }
        zego_scene_player_config _config;
        memset(&_config, 0, sizeof(zego_scene_player_config));
        zego_cdn_config cdn_cofig;
        memset(&cdn_cofig, 0, sizeof(zego_cdn_config));
        if (config.cdnConfig) {
            cdn_cofig = ZegoExpressConvert::O2ICDNConfig(*config.cdnConfig);
            _config.cdn_config = &cdn_cofig;
        } else {
            _config.cdn_config = nullptr;
        }
        _config.range_scene_handle = config.rangeSceneHandle;
        _config.resource_mode = zego_stream_resource_mode(config.resourceMode);
        oInternalOriginBridge->startPlayingStreamInScene(stream_id, canvas ? &_canvas : nullptr,
                                                         _config);
    }

    virtual void startPlayingStreamInScene(const std::string &streamID,
                                           ZegoScenePlayerConfig config) override {
        const char *stream_id = streamID.c_str();
        zego_scene_player_config _config;
        memset(&_config, 0, sizeof(zego_scene_player_config));
        zego_cdn_config cdn_cofig;
        memset(&cdn_cofig, 0, sizeof(zego_cdn_config));
        if (config.cdnConfig) {
            cdn_cofig = ZegoExpressConvert::O2ICDNConfig(*config.cdnConfig);
            _config.cdn_config = &cdn_cofig;
        } else {
            _config.cdn_config = nullptr;
        }
        _config.range_scene_handle = config.rangeSceneHandle;
        _config.resource_mode = zego_stream_resource_mode(config.resourceMode);
        oInternalOriginBridge->startPlayingStreamInScene(stream_id, nullptr, _config);
    }

    void switchPlayingStream(const std::string &fromStreamID, const std::string &toStreamID,
                             ZegoPlayerConfig config) override {
        zego_player_config _config;
        memset(&_config, 0, sizeof(zego_player_config));
        zego_cdn_config cdn_cofig;
        memset(&cdn_cofig, 0, sizeof(zego_cdn_config));
        if (config.cdnConfig) {
            cdn_cofig = ZegoExpressConvert::O2ICDNConfig(*config.cdnConfig);
            _config.cdn_config = &cdn_cofig;
        } else {
            _config.cdn_config = nullptr;
        }
        _config.resource_mode = zego_stream_resource_mode(config.resourceMode);
        strncpy(_config.room_id, config.roomID.c_str(), ZEGO_EXPRESS_MAX_ROOMID_LEN);
        _config.video_codec_id = zego_video_codec_id(config.videoCodecID);
        _config.source_resource_type = zego_resource_type(config.sourceResourceType);
        _config.codec_template_id = config.codecTemplateID;
        _config.resource_switch_mode = zego_stream_resource_switch_mode(config.resourceSwitchMode);

        struct zego_custom_player_resource_config customResourceConfig;
        customResourceConfig.before_publish =
            zego_resource_type(config.customResourceConfig.beforePublish);
        customResourceConfig.publishing =
            zego_resource_type(config.customResourceConfig.publishing);
        customResourceConfig.after_publish =
            zego_resource_type(config.customResourceConfig.afterPublish);
        _config.custom_resource_config = &customResourceConfig;

        oInternalOriginBridge->switchPlayingStream(fromStreamID.c_str(), toStreamID.c_str(),
                                                   &_config);
    }

    void stopPlayingStream(const std::string &streamID) override {
        const char *stream_id = streamID.c_str();
        oInternalOriginBridge->stopPlayingStream(stream_id);
    }

    void setPlayStreamDecryptionKey(const std::string &streamID, const std::string &key) override {
        const char *stream_id = streamID.c_str();
        const char *decryption_key = key.c_str();
        oInternalOriginBridge->setPlayStreamDecryptionKey(stream_id, decryption_key);
    }

    void setPlayStreamCrossAppInfo(const std::string &streamID, ZegoCrossAppInfo info) override {
        const char *stream_id = streamID.c_str();
        zego_cross_app_info config;
        config.app_id = info.appID;
        if (!info.token.empty()) {
            strncpy(config.token, info.token.c_str(), ZEGO_EXPRESS_MAX_TOKEN_LEN);
            config.token[ZEGO_EXPRESS_MAX_TOKEN_LEN - 1] = '\0';
        } else {
            memset(config.token, 0, ZEGO_EXPRESS_MAX_TOKEN_LEN);
        }
        oInternalOriginBridge->setPlayStreamCrossAppInfo(stream_id, config);
    }

    void takePlayStreamSnapshot(const std::string &streamID,
                                ZegoPlayerTakeSnapshotCallback callback) override {
        const char *stream_id = streamID.c_str();
        oInternalOriginBridge->takePlayStreamSnapshot(stream_id);
        if (callback != nullptr) {
            oInternalCallbackCenter->insertZegoPlayerTakeSnapshotCallback(streamID, callback);
        }
    }

    void setPlayVolume(const std::string &streamID, int volume) override {
        const char *stream_id = streamID.c_str();
        oInternalOriginBridge->setPlayVolume(stream_id, volume);
    }

    void setAllPlayStreamVolume(int volume) override {
        oInternalOriginBridge->setAllPlayStreamVolume(volume);
    }

    void setPlayStreamVideoType(const std::string &streamID, ZegoVideoStreamType type) override {
        const char *stream_id = streamID.c_str();
        oInternalOriginBridge->setPlayStreamVideoType(stream_id, zego_video_stream_type(type));
    }

    void setPlayStreamBufferIntervalRange(const std::string &streamID,
                                          unsigned int minBufferInterval,
                                          unsigned int maxBufferInterval) override {
        const char *stream_id = streamID.c_str();
        oInternalOriginBridge->setPlayStreamBufferIntervalRange(stream_id, minBufferInterval,
                                                                maxBufferInterval);
    }

    void setAudioMixMode(ZegoAudioMixMode mode,
                         const std::vector<std::string> &streamList) override {
        std::vector<const char *> stream_list_;
        for (auto &item : streamList) {
            stream_list_.push_back(item.c_str());
        }
        oInternalOriginBridge->setAudioMixMode(static_cast<zego_audio_mix_mode>(mode),
                                               stream_list_.data(), stream_list_.size());
    }

    void setPlayStreamFocusOn(const std::string &streamID) override {
        oInternalOriginBridge->setPlayStreamFocusOn(streamID.c_str());
    }

    void mutePlayStreamAudio(const std::string &streamID, bool mute) override {
        const char *stream_id = streamID.c_str();
        oInternalOriginBridge->mutePlayStreamAudio(stream_id, mute);
    }

    void muteAllPlayStreamAudio(bool mute) override {
        oInternalOriginBridge->muteAllPlayStreamAudio(mute);
    }

    void muteAllPlayAudioStreams(bool mute) override {
        oInternalOriginBridge->muteAllPlayAudioStreams(mute);
    }

    void mutePlayStreamVideo(const std::string &streamID, bool mute) override {
        const char *stream_id = streamID.c_str();
        oInternalOriginBridge->mutePlayStreamVideo(stream_id, mute);
    }

    void muteAllPlayStreamVideo(bool mute) override {
        oInternalOriginBridge->muteAllPlayStreamVideo(mute);
    }

    void muteAllPlayVideoStreams(bool mute) override {
        oInternalOriginBridge->muteAllPlayVideoStreams(mute);
    }

    void enableHardwareDecoder(bool enable) override {
        oInternalOriginBridge->enableHardwareDecoder(enable);
    }

    void enableCheckPoc(bool enable) override { oInternalOriginBridge->enableCheckPoc(enable); }

    bool isVideoDecoderSupported(ZegoVideoCodecID codecID) override {
        return isVideoDecoderSupported(codecID, ZEGO_VIDEO_CODEC_BACKEND_ANY) != 0;
    }

    int isVideoDecoderSupported(ZegoVideoCodecID codecID,
                                ZegoVideoCodecBackend codecBackend) override {
        return oInternalOriginBridge->isVideoDecoderSupported(
            static_cast<zego_video_codec_id>(codecID),
            static_cast<zego_video_codec_backend>(codecBackend));
    }

    void setLowlightEnhancement(ZegoLowlightEnhancementMode mode,
                                ZegoPublishChannel channel) override {
        oInternalOriginBridge->setLowlightEnhancement(
            static_cast<zego_low_light_enhancement_mode>(mode),
            static_cast<zego_publish_channel>(channel));
    }

    void
    setLowlightEnhancementParams(ZegoExpLowlightEnhancementParams params,
                                 ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override {
        struct zego_exp_low_light_enhancement_params p;
        memset(&p, 0, sizeof(struct zego_exp_low_light_enhancement_params));
        p.mode = static_cast<zego_low_light_enhancement_mode>(params.mode);
        p.type = static_cast<zego_exp_low_light_enhancement_type>(params.type);
        oInternalOriginBridge->setLowlightEnhancementParams(
            p, static_cast<zego_publish_channel>(channel));
    }

    int setVideoSource(ZegoVideoSourceType source) override { return setVideoSource(source, 0); }

    int setVideoSource(ZegoVideoSourceType source, unsigned int instanceID) override {
        return setVideoSource(source, instanceID, ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    int setVideoSource(ZegoVideoSourceType source, ZegoPublishChannel channel) override {
        return setVideoSource(source, 0, channel);
    }

    int setVideoSource(ZegoVideoSourceType source, unsigned int instanceID,
                       ZegoPublishChannel channel) override {
        return oInternalOriginBridge->setVideoSource(static_cast<zego_video_source_type>(source),
                                                     instanceID,
                                                     static_cast<zego_publish_channel>(channel));
    }

    int setAudioSource(ZegoAudioSourceType source) override {
        return setAudioSource(source, ZEGO_PUBLISH_CHANNEL_MAIN);
    }

    int setAudioSource(ZegoAudioSourceType source, ZegoPublishChannel channel) override {
        return oInternalOriginBridge->setAudioSource(static_cast<zego_audio_source_type>(source),
                                                     static_cast<zego_publish_channel>(channel));
    }

    int setAudioSource(ZegoAudioSourceType source, ZegoAudioSourceMixConfig config) override {
        struct zego_audio_source_mix_config audio_mix_config;
        memset(&audio_mix_config, 0, sizeof(struct zego_audio_source_mix_config));

        audio_mix_config.media_player_count = config.mediaPlayerCount;
        if (audio_mix_config.media_player_count > 0) {
            audio_mix_config.media_player_index_list =
                new zego_media_player_instance_index[audio_mix_config.media_player_count];
            for (int i = 0; i < audio_mix_config.media_player_count; i++) {
                audio_mix_config.media_player_index_list[i] =
                    (zego_media_player_instance_index)config.mediaPlayerIndexList[i];
            }
        }
        audio_mix_config.audio_effect_player_count = config.audioEffectPlayerCount;
        if (audio_mix_config.audio_effect_player_count > 0) {
            audio_mix_config.audio_effect_player_index_list =
                new zego_audio_effect_player_instance_index[audio_mix_config
                                                                .audio_effect_player_count];
            for (int i = 0; i < audio_mix_config.audio_effect_player_count; i++) {
                audio_mix_config.audio_effect_player_index_list[i] =
                    (zego_audio_effect_player_instance_index)config.audioEffectPlayerIndexList[i];
            }
        }
        audio_mix_config.enable_mix_system_playout = config.enableMixSystemPlayout;
        audio_mix_config.enable_mix_engine_playout = config.enableMixEnginePlayout;

        int ret = oInternalOriginBridge->setAudioSource(static_cast<zego_audio_source_type>(source),
                                                        audio_mix_config);

        if (audio_mix_config.media_player_count > 0) {
            delete[] audio_mix_config.media_player_index_list;
        }
        if (audio_mix_config.audio_effect_player_count > 0) {
            delete[] audio_mix_config.audio_effect_player_index_list;
        }

        return ret;
    }

    void setPlayStreamsAlignmentProperty(ZegoStreamAlignmentMode mode) override {
        oInternalOriginBridge->setPlayStreamsAlignmentProperty(zego_stream_alignment_mode(mode));
    }

    //===================================================================================================

    void muteMicrophone(bool enable) override { oInternalOriginBridge->muteMicrophone(enable); }

    bool isMicrophoneMuted() override { return oInternalOriginBridge->isMicrophoneMuted(); }

    void muteSpeaker(bool mute) override { oInternalOriginBridge->muteSpeaker(mute); }

    bool isSpeakerMuted() override { return oInternalOriginBridge->isSpeakerMuted(); }

    void enableCamera(bool enable, ZegoPublishChannel channel) override {
        oInternalOriginBridge->enableCamera(enable, zego_publish_channel(channel));
    }

    void enableAudioCaptureDevice(bool enable) override {
        oInternalOriginBridge->enableAudioCaptureDevice(enable);
    }

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    ZegoAudioRoute getAudioRouteType() override {
        return ZegoAudioRoute(oInternalOriginBridge->getAudioRouteType());
    }

    void setAudioRouteToSpeaker(bool defaultToSpeaker) override {
        oInternalOriginBridge->setAudioRouteToSpeaker(defaultToSpeaker);
    }
#endif

#if (TARGET_OS_IPHONE || defined(ANDROID)) || defined(_OS_OHOS_)
    void useFrontCamera(bool enable, ZegoPublishChannel channel) override {
        oInternalOriginBridge->useFrontCamera(enable, zego_publish_channel(channel));
    }

    bool isCameraFocusSupported(ZegoPublishChannel channel) override {
        return oInternalOriginBridge->isCameraFocusSupported(zego_publish_channel(channel));
    }

    void setCameraFocusMode(ZegoCameraFocusMode mode, ZegoPublishChannel channel) override {
        oInternalOriginBridge->setCameraFocusMode(zego_camera_focus_mode(mode),
                                                  zego_publish_channel(channel));
    }

    void setCameraFocusPointInPreview(float x, float y, ZegoPublishChannel channel) override {
        oInternalOriginBridge->setCameraFocusPointInPreview(x, y, zego_publish_channel(channel));
    }

    void setCameraExposureMode(ZegoCameraExposureMode mode, ZegoPublishChannel channel) override {
        oInternalOriginBridge->setCameraExposureMode(zego_camera_exposure_mode(mode),
                                                     zego_publish_channel(channel));
    }

    void setCameraExposurePointInPreview(float x, float y, ZegoPublishChannel channel) override {
        oInternalOriginBridge->setCameraExposurePointInPreview(x, y, zego_publish_channel(channel));
    }

    void setCameraExposureCompensation(float value, ZegoPublishChannel channel) override {
        oInternalOriginBridge->setCameraExposureCompensation(value, zego_publish_channel(channel));
    }

    void setCameraZoomFactor(float factor, ZegoPublishChannel channel) override {
        oInternalOriginBridge->setCameraZoomFactor(factor, zego_publish_channel(channel));
    }

    float getCameraMaxZoomFactor(ZegoPublishChannel channel) override {
        return oInternalOriginBridge->getCameraMaxZoomFactor(zego_publish_channel(channel));
    }
#endif

    void enableCameraAdaptiveFPS(bool enable, int minFPS, int maxFPS,
                                 ZegoPublishChannel channel) override {
        oInternalOriginBridge->enableCameraAdaptiveFPS(enable, minFPS, maxFPS,
                                                       zego_publish_channel(channel));
    }

    void startSoundLevelMonitor(unsigned int millisecond) override {
        struct zego_sound_level_config _config;
        _config.millisecond = millisecond;
        _config.enable_vad = false;
        oInternalOriginBridge->startSoundLevelMonitor(_config);
    }

    void startSoundLevelMonitor(ZegoSoundLevelConfig config) override {
        struct zego_sound_level_config _config;
        _config.millisecond = config.millisecond;
        _config.enable_vad = config.enableVAD;
        oInternalOriginBridge->startSoundLevelMonitor(_config);
    }

    void stopSoundLevelMonitor() override { oInternalOriginBridge->stopSoundLevelMonitor(); }

    void startAudioSpectrumMonitor(unsigned int millisecond) override {
        oInternalOriginBridge->startAudioSpectrumMonitor(millisecond);
    }

    void stopAudioSpectrumMonitor() override { oInternalOriginBridge->stopAudioSpectrumMonitor(); }

    void startPerformanceMonitor(unsigned int millisecond) override {
        oInternalOriginBridge->startPerformanceMonitor(millisecond);
    }

    void stopPerformanceMonitor() override { oInternalOriginBridge->stopPerformanceMonitor(); }

    void enableHeadphoneMonitor(bool enable) override {
        oInternalOriginBridge->enableHeadphoneMonitor(enable);
    }

    void setHeadphoneMonitorVolume(int volume) override {
        oInternalOriginBridge->setHeadphoneMonitorVolume(volume);
    }

    void enableMixSystemPlayout(bool enable) override {
        oInternalOriginBridge->enableMixSystemPlayout(enable);
    }

    void setMixSystemPlayoutVolume(int volume) override {
        oInternalOriginBridge->setMixSystemPlayoutVolume(volume);
    }

    void enableMixEnginePlayout(bool enable) override {
        oInternalOriginBridge->enableMixEnginePlayout(enable);
    }

    void startAudioVADStableStateMonitor(ZegoAudioVADStableStateMonitorType type) override {
        oInternalOriginBridge->startAudioVADStableStateMonitor(type, 3000);
    }

    void startAudioVADStableStateMonitor(ZegoAudioVADStableStateMonitorType type,
                                         int millisecond) override {
        oInternalOriginBridge->startAudioVADStableStateMonitor(type, millisecond);
    }

    void stopAudioVADStableStateMonitor(ZegoAudioVADStableStateMonitorType type) override {
        oInternalOriginBridge->stopAudioVADStableStateMonitor(type);
    }

    void useAudioDevice(ZegoAudioDeviceType deviceType, const std::string &deviceID) override {
        oInternalOriginBridge->useAudioDevice(zego_audio_device_type(deviceType), deviceID.c_str());
    }

    int getAudioDeviceVolume(ZegoAudioDeviceType deviceType, const std::string &deviceID) override {
        return oInternalOriginBridge->getAudioDeviceVolume(zego_audio_device_type(deviceType),
                                                           deviceID.c_str());
    }

    void setAudioDeviceVolume(ZegoAudioDeviceType deviceType, const std::string &deviceID,
                              int volume) override {
        oInternalOriginBridge->setAudioDeviceVolume(zego_audio_device_type(deviceType),
                                                    deviceID.c_str(), volume);
    }

    void setSpeakerVolumeInAPP(const std::string &deviceID, int volume) override {
        oInternalOriginBridge->setSpeakerVolumeInAPP(deviceID.c_str(), volume);
    }

    int getSpeakerVolumeInAPP(const std::string &deviceID) override {
        return oInternalOriginBridge->getSpeakerVolumeInAPP(deviceID.c_str());
    }

    void startAudioDeviceVolumeMonitor(ZegoAudioDeviceType deviceType,
                                       const std::string &deviceID) override {
        oInternalOriginBridge->startAudioDeviceVolumeMonitor(zego_audio_device_type(deviceType),
                                                             deviceID.c_str());
    }

    void stopAudioDeviceVolumeMonitor(ZegoAudioDeviceType deviceType,
                                      const std::string &deviceID) override {
        oInternalOriginBridge->stopAudioDeviceVolumeMonitor(zego_audio_device_type(deviceType),
                                                            deviceID.c_str());
    }

    void muteAudioDevice(ZegoAudioDeviceType deviceType, const std::string &deviceID,
                         bool mute) override {
        oInternalOriginBridge->muteAudioDevice(zego_audio_device_type(deviceType), deviceID.c_str(),
                                               mute);
    }

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    void setAudioDeviceMode(ZegoAudioDeviceMode deviceMode) override {
        oInternalOriginBridge->setAudioDeviceMode(zego_audio_device_mode(deviceMode));
    }
#endif

    bool isAudioDeviceMuted(ZegoAudioDeviceType deviceType, const std::string &deviceID) override {
        return oInternalOriginBridge->isAudioDeviceMuted(zego_audio_device_type(deviceType),
                                                         deviceID.c_str());
    }

    std::vector<ZegoDeviceInfo> getAudioDeviceList(ZegoAudioDeviceType deviceType) override {
        int count = 0;
        zego_device_info *_deviceInfoList =
            oInternalOriginBridge->getAudioDeviceList(zego_audio_device_type(deviceType), &count);

        std::vector<ZegoDeviceInfo> result;
        for (int i = 0; i < count; i++) {
            ZegoDeviceInfo deviceInfo = ZegoExpressConvert::I2ODeviceInfo(_deviceInfoList[i]);
            result.push_back(deviceInfo);
        }

        oInternalOriginBridge->freeAudioDeviceList(_deviceInfoList);
        return result;
    }

    virtual std::string getDefaultAudioDeviceID(ZegoAudioDeviceType deviceType) override {
        return oInternalOriginBridge->getDefaultAudioDeviceID(zego_audio_device_type(deviceType));
    }

    virtual ZegoDeviceInfo getCurrentAudioDevice(ZegoAudioDeviceType deviceType) override {
        zego_device_info device_info_src =
            oInternalOriginBridge->getCurrentAudioDevice(zego_audio_device_type(deviceType));
        ZegoDeviceInfo device_info = ZegoExpressConvert::I2ODeviceInfo(device_info_src);
        return device_info;
    }

    virtual std::string getDefaultVideoDeviceID() override {
        return oInternalOriginBridge->getDefaultVideoDeviceID();
    }

    void useVideoDevice(const std::string &deviceID, ZegoPublishChannel channel) override {
        const char *device_id = deviceID.c_str();
        oInternalOriginBridge->useVideoDevice(device_id, zego_publish_channel(channel));
    }

    std::vector<ZegoDeviceInfo> getVideoDeviceList() override {
        int count = 0;
        zego_device_info *_deviceInfoList = oInternalOriginBridge->getVideoDeviceList(&count);

        std::vector<ZegoDeviceInfo> result;
        for (int i = 0; i < count; i++) {
            ZegoDeviceInfo deviceInfo = ZegoExpressConvert::I2ODeviceInfo(_deviceInfoList[i]);
            result.push_back(deviceInfo);
        }

        oInternalOriginBridge->freeVideoDeviceList(_deviceInfoList);
        return result;
    }

    //===================================================================================================

    void enableAEC(bool enable) override { oInternalOriginBridge->enableAEC(enable); }

    // void enableAEC(bool enable, ZegoPublishChannel channel) override {
    //     oInternalOriginBridge->enableAEC(enable, zego_publish_channel(channel));
    // }

#if TARGET_OS_IPHONE || defined(ANDROID) || defined(_OS_OHOS_)
    void enableHeadphoneAEC(bool enable) override {
        oInternalOriginBridge->enableHeadphoneAEC(enable);
    }
#endif

    void setAECMode(ZegoAECMode mode) override {
        oInternalOriginBridge->setAECMode(zego_aec_mode(mode));
    }

    // void setAECMode(ZegoAECMode mode, ZegoPublishChannel channel) override {
    //     oInternalOriginBridge->setAECMode(zego_aec_mode(mode), zego_publish_channel(channel));
    // }

    void enableAGC(bool enable) override { oInternalOriginBridge->enableAGC(enable); }

    // void enableAGC(bool enable, ZegoPublishChannel channel) override {
    //     oInternalOriginBridge->enableAGC(enable, zego_publish_channel(channel));
    // }

    void setANSMode(ZegoANSMode mode) override {
        oInternalOriginBridge->setANSMode(zego_ans_mode(mode));
    }

    // void setANSMode(ZegoANSMode mode, ZegoPublishChannel channel) override {
    //     oInternalOriginBridge->setANSMode(zego_ans_mode(mode), zego_publish_channel(channel));
    // }

    void enableSpeechEnhance(bool enable, int level) override {
        oInternalOriginBridge->enableSpeechEnhance(enable, level);
    }

    // void enableSpeechEnhance(bool enable, int level, ZegoPublishChannel channel) override {
    //     oInternalOriginBridge->enableSpeechEnhance(enable, level, zego_publish_channel(channel));
    // }

    void enableANS(bool enable) override { oInternalOriginBridge->enableANS(enable); }

    // void enableANS(bool enable, ZegoPublishChannel channel) override {
    //     oInternalOriginBridge->enableANS(enable, zego_publish_channel(channel));
    // }

    void enableTransientANS(bool enable) override {
        oInternalOriginBridge->enableTransientANS(enable);
    }

    void enableAudioMixing(bool enable) override {
        oInternalOriginBridge->enableAudioMixing(enable);
    }

    void setAudioMixingHandler(std::shared_ptr<IZegoAudioMixingHandler> handler) override {
        oInternalCallbackCenter->setIZegoAudioMixingHandler(handler);
    }

    void muteLocalAudioMixing(bool mute) override {
        oInternalOriginBridge->muteLocalAudioMixing(mute);
    }

    void setAudioMixingVolume(int volume, ZegoVolumeType type) override {
        oInternalOriginBridge->setAudioMixingVolume(volume, zego_volume_type(type));
    }

    void setAudioEqualizerGain(int bandIndex, float bandGain) override {
        oInternalOriginBridge->setAudioEqualizerGain(bandIndex, bandGain);
    }

    void startEffectsEnv() override { oInternalOriginBridge->startEffectsEnv(); }

    void stopEffectsEnv() override { oInternalOriginBridge->stopEffectsEnv(); }

    void enableEffectsBeauty(bool enable) override {
        oInternalOriginBridge->enableEffectsBeauty(enable);
    }

    void setEffectsBeautyParam(ZegoEffectsBeautyParam param) override {
        zego_effects_beauty_param beauty_param;
        beauty_param.whiten_intensity = param.whitenIntensity;
        beauty_param.rosy_intensity = param.rosyIntensity;
        beauty_param.smooth_intensity = param.smoothIntensity;
        beauty_param.sharpen_intensity = param.sharpenIntensity;
        oInternalOriginBridge->setEffectsBeautyParam(beauty_param);
    }
    //===================================================================================================

    IZegoRealTimeSequentialDataManager *
    createRealTimeSequentialDataManager(const std::string &roomID) override {
        auto index = oInternalOriginBridge->createRealTimeSequentialDataManager(roomID.c_str());
        if (index == -1) {
            return nullptr;
        }

        auto manager = std::make_shared<ZegoExpressRealTimeSequentialDataManagerImp>(index);
        oInternalCallbackCenter->insertZegoExpressRealTimeSequentialDataManagerImp(index, manager);
        return manager.get();
    }

    void
    destroyRealTimeSequentialDataManager(IZegoRealTimeSequentialDataManager *&manager) override {
        if (manager && oInternalCallbackCenter->getZegoExpressRealTimeSequentialDataManagerImp(
                           manager->getIndex())) {
            oInternalOriginBridge->destroyRealTimeSequentialDataManager(manager->getIndex());
            oInternalCallbackCenter->eraseZegoExpressRealTimeSequentialDataManagerImp(
                manager->getIndex());
        }
    }

    void sendBroadcastMessage(const std::string &roomID, const std::string &message,
                              ZegoIMSendBroadcastMessageCallback callback) override {
        const char *_roomID = roomID.c_str();
        const char *_message = message.c_str();

        int seq = oInternalOriginBridge->sendBroadcastMessage(_roomID, _message);
        if (callback != nullptr) {
            oInternalCallbackCenter->insertZegoIMSendBroadcastMessageCallback(seq, callback);
        }
    }

    void sendBarrageMessage(const std::string &roomID, const std::string &message,
                            ZegoIMSendBarrageMessageCallback callback) override {
        const char *_roomID = roomID.c_str();
        const char *_message = message.c_str();

        int seq = oInternalOriginBridge->sendBarrageMessage(_roomID, _message);
        if (callback != nullptr) {
            oInternalCallbackCenter->insertZegoIMSendBarrageMessageCallback(seq, callback);
        }
    }

    void sendCustomCommand(const std::string &roomID, const std::string &command,
                           std::vector<ZegoUser> toUserList,
                           ZegoIMSendCustomCommandCallback callback) override {
        std::vector<zego_user> _userList;
        for (auto user : toUserList) {
            zego_user _user = ZegoExpressConvert::O2IUser(user);
            _userList.push_back(_user);
        }
        zego_user *users = _userList.data();
        const char *_roomID = roomID.c_str();
        const char *_command = command.c_str();

        int seq = oInternalOriginBridge->sendCustomCommand(_roomID, _command, users,
                                                           ZegoUInt(_userList.size()));
        if (callback != nullptr) {
            oInternalCallbackCenter->insertZegoIMSendCustomCommandCallback(seq, callback);
        }
    }

    void sendTransparentMessage(const std::string &roomID,
                                const ZegoRoomSendTransparentMessage &message,
                                ZegoRoomSendTransparentMessageCallback callback) override {
        zego_room_send_transparent_message message_;
        message_.content = (unsigned char *)message.content.data();
        message_.content_length = message.content.length();

        message_.send_mode = (zego_room_transparent_message_mode)message.sendMode;
        message_.send_type = (zego_room_transparent_message_type)message.sendType;
        message_.time_out = message.timeOut;

        std::vector<zego_user> _userList;
        for (auto user : message.recvUserList) {
            zego_user _user = ZegoExpressConvert::O2IUser(user);
            _userList.push_back(_user);
        }
        zego_user *users = _userList.data();
        message_.recv_user_list = users;
        message_.recv_user_list_count = ZegoUInt(_userList.size());

        int seq = oInternalOriginBridge->sendTransparentMessage(roomID.c_str(), &message_);
        if (callback != nullptr) {
            oInternalCallbackCenter->insertZegoRoomSendTransparentMessageCallback(seq, callback);
        }
    }

    //===================================================================================================
    void StartOrStopMixerTask(bool isStart, ZegoMixerTask task,
                              ZegoMixerStartCallback startCallback,
                              ZegoMixerStopCallback stopCallback) {
        zego_mixer_task _task;

        strncpy(_task.task_id, task.taskID.c_str(), ZEGO_EXPRESS_MAX_MIXER_TASK_LEN);

        std::vector<zego_mixer_input> input_list;
        {
            for (auto &input : task.inputList) {
                zego_mixer_input _input = ZegoExpressConvert::O2IMixerInput(input);
                input_list.push_back(_input);
            }
        }
        _task.input_list = input_list.data();
        _task.input_list_count = ZegoUInt(input_list.size());

        std::vector<zego_mixer_output> output_list;
        std::vector<std::shared_ptr<zego_mixer_output_video_config>> output_video_config_list;
        std::vector<std::shared_ptr<zego_mixer_output_room_info>> output_room_info_list;
        {
            unsigned int cnt = 0;
            for (auto &output : task.outputList) {
                std::shared_ptr<zego_mixer_output_video_config> videoConfig =
                    std::make_shared<zego_mixer_output_video_config>();
                ZegoMixerOutputVideoConfig default_video_config_;
                // if output video bitrate equal to default value, set value of bitrate to task.videoConfig.bitrate
                if (default_video_config_.bitrate == output.videoConfig.bitrate) {
                    videoConfig->bitrate = task.videoConfig.bitrate;
                } else {
                    videoConfig->bitrate = output.videoConfig.bitrate;
                }
                videoConfig->video_codec_id =
                    static_cast<zego_video_codec_id>(output.videoConfig.videoCodecID);
                videoConfig->encode_latency = output.videoConfig.encodeLatency;
                videoConfig->encode_profile =
                    static_cast<zego_encode_profile>(output.videoConfig.encodeProfile);
                videoConfig->enable_low_bitrate_hd = output.videoConfig.enableLowBitrateHD;

                zego_mixer_output _output = ZegoExpressConvert::O2IMixerOutput(output);
                output_video_config_list.push_back(videoConfig);
                _output.video_config = output_video_config_list.at(cnt).get();

                std::shared_ptr<zego_mixer_output_room_info> output_room_info =
                    std::make_shared<zego_mixer_output_room_info>();
                strncpy(output_room_info->room_id, output.targetRoom.roomID.c_str(),
                        sizeof(output_room_info->room_id) - 1);
                output_room_info->room_id[sizeof(output_room_info->room_id) - 1] = '\0';
                strncpy(output_room_info->userid, output.targetRoom.userID.c_str(),
                        sizeof(output_room_info->userid) - 1);
                output_room_info->userid[sizeof(output_room_info->userid) - 1] = '\0';
                output_room_info_list.push_back(output_room_info);
                _output.target_room = output_room_info.get();

                output_list.push_back(_output);
                cnt++;
            }
        }
        _task.output_list = output_list.data();
        _task.output_list_count = ZegoUInt(output_list.size());

        _task.audio_config = ZegoExpressConvert::O2IMixerAudioConfig(task.audioConfig);
        _task.video_config = ZegoExpressConvert::O2IMixerVideoConfig(task.videoConfig);

        zego_watermark _watermark;
        if (task.watermark != nullptr) {
            _watermark = ZegoExpressConvert::O2IWatermark(*task.watermark);
            _task.watermark = &_watermark;
        } else {
            _task.watermark = nullptr;
        }

        zego_mixer_whiteboard _whiteboard;
        if (task.whiteboard != nullptr) {
            _whiteboard = ZegoExpressConvert::O2IMixerWhiteboard(*task.whiteboard);
            ;
            _task.whiteboard = &_whiteboard;
        } else {
            _task.whiteboard = nullptr;
        }

        _task.background_color = task.backgroundColor;

        strncpy(_task.background_image_url, task.backgroundImageURL.c_str(),
                ZEGO_EXPRESS_MAX_URL_LEN);

        _task.enable_sound_level = task.enableSoundLevel;
        _task.stream_alignment_mode = (zego_stream_alignment_mode)task.streamAlignmentMode;
        _task.mix_image_check_mode = (zego_mix_image_check_mode)task.mixImageCheckMode;

        std::string advanceConfig;
        for (auto iter : task.advancedConfig) {
            advanceConfig += iter.first + "=" + iter.second + ";";
        }
        strncpy(_task.advanced_config, advanceConfig.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);

        if (task.userData && task.userDataLength > 0) {
            _task.user_data = task.userData;
            _task.user_data_length = task.userDataLength;
        } else {
            _task.user_data = nullptr;
            _task.user_data_length = 0;
        }

        _task.min_play_stream_buffer_length = task.minPlayStreamBufferLength;

        if (isStart) {
            int seq = oInternalOriginBridge->startMixerTask(_task);
            if (startCallback != nullptr) {
                oInternalCallbackCenter->insertZegoMixerStartCallback(seq, startCallback);
            }
        } else {
            int seq = oInternalOriginBridge->stopMixerTask(_task);
            if (stopCallback != nullptr) {
                oInternalCallbackCenter->insertZegoMixerStopCallback(seq, stopCallback);
            }
        }
    }

    void startMixerTask(ZegoMixerTask task, ZegoMixerStartCallback callback) override {
        StartOrStopMixerTask(true, task, callback, nullptr);
    }

    void stopMixerTask(ZegoMixerTask task, ZegoMixerStopCallback callback) override {
        StartOrStopMixerTask(false, task, nullptr, callback);
    }

    void StartOrStopAutoMixerTask(bool isStart, ZegoAutoMixerTask task,
                                  ZegoMixerStartCallback startCallback,
                                  ZegoMixerStopCallback stopCallback) {
        zego_auto_mixer_task _task;

        memset(_task.task_id, 0, sizeof(_task.task_id));
        memset(_task.room_id, 0, sizeof(task.roomID));
        strncpy(_task.task_id, task.taskID.c_str(), ZEGO_EXPRESS_MAX_MIXER_TASK_LEN);
        strncpy(_task.room_id, task.roomID.c_str(), ZEGO_EXPRESS_MAX_ROOMID_LEN);

        _task.audio_config = ZegoExpressConvert::O2IMixerAudioConfig(task.audioConfig);

        std::vector<zego_mixer_output> output_list;
        {
            for (auto &output : task.outputList) {
                zego_mixer_output _output = ZegoExpressConvert::O2IMixerOutput(output);
                output_list.push_back(_output);
            }
        }
        _task.output_list = output_list.data();
        _task.output_list_count = ZegoUInt(output_list.size());

        _task.enable_sound_level = task.enableSoundLevel;
        _task.stream_alignment_mode = (zego_stream_alignment_mode)task.streamAlignmentMode;
        _task.min_play_stream_buffer_length = task.minPlayStreamBufferLength;

        if (isStart) {
            int seq = oInternalOriginBridge->startAutoMixerTask(_task);
            if (startCallback != nullptr) {
                oInternalCallbackCenter->insertZegoMixerStartCallback(seq, startCallback);
            }
        } else {
            int seq = oInternalOriginBridge->stopAutoMixerTask(_task);
            if (stopCallback != nullptr) {
                oInternalCallbackCenter->insertZegoMixerStopCallback(seq, stopCallback);
            }
        }
    }

    void startAutoMixerTask(ZegoAutoMixerTask task, ZegoMixerStartCallback callback) override {
        StartOrStopAutoMixerTask(true, task, callback, nullptr);
    }

    void stopAutoMixerTask(ZegoAutoMixerTask task, ZegoMixerStopCallback callback) override {
        StartOrStopAutoMixerTask(false, task, nullptr, callback);
    }

    //===================================================================================================
    IZegoMediaPlayer *createMediaPlayer() override {
        auto index = oInternalOriginBridge->createMediaPlayer();
        if (index == zego_media_player_instance_index_null) {
            return nullptr;
        }

        auto mediaPlayer = std::make_shared<ZegoExpressMediaPlayerImp>(index);
        oInternalCallbackCenter->insertZegoExpressMediaPlayerImp(index, mediaPlayer);
        return mediaPlayer.get();
    }

    void destroyMediaPlayer(IZegoMediaPlayer *&mediaPlayer) override {
        if (mediaPlayer &&
            oInternalCallbackCenter->getZegoExpressMediaPlayerImp(mediaPlayer->getIndex())) {
            oInternalOriginBridge->destroyMediaPlayer(
                zego_media_player_instance_index(mediaPlayer->getIndex()));
            oInternalCallbackCenter->eraseZegoExpressMediaPlayerImp(mediaPlayer->getIndex());
        }
    }

    //===================================================================================================
    IZegoAudioEffectPlayer *createAudioEffectPlayer() override {
        auto index = oInternalOriginBridge->createAudioEffectPlayer();
        if (index == zego_audio_effect_player_instance_index_null) {
            return nullptr;
        }

        auto audioEffectPlayer = std::make_shared<ZegoExpressAudioEffectPlayerImp>(index);
        oInternalCallbackCenter->insertZegoExpressAudioEffectPlayerImp(index, audioEffectPlayer);
        return audioEffectPlayer.get();
    }

    void destroyAudioEffectPlayer(IZegoAudioEffectPlayer *&audioEffectPlayer) override {
        if (audioEffectPlayer && oInternalCallbackCenter->getZegoExpressAudioEffectPlayerImp(
                                     audioEffectPlayer->getIndex())) {
            oInternalOriginBridge->destroyAudioEffectPlayer(
                zego_audio_effect_player_instance_index(audioEffectPlayer->getIndex()));
            oInternalCallbackCenter->eraseZegoExpressAudioEffectPlayerImp(
                audioEffectPlayer->getIndex());
        }
    }

    //===================================================================================================
    IZegoRangeAudio *createRangeAudio() override {
        auto index = oInternalOriginBridge->createRangeAudio();
        if (index == zego_range_audio_instance_index_null) {
            return nullptr;
        }

        auto rangeAudio = std::make_shared<ZegoExpressRangeAudioImp>(index);
        oInternalCallbackCenter->insertZegoExpressRangeAudioImp(index, rangeAudio);
        return rangeAudio.get();
    }

    void destroyRangeAudio(IZegoRangeAudio *&rangeAudio) override {
        if (rangeAudio /*&& oInternalCallbackCenter->getZegoExpressRangeAudioImp(rangeAudio->getIndex()*/) {
            // oInternalOriginBridge->destroyRangeAudio(zego_range_audio_instance_index(rangeAudio->getIndex());
            oInternalOriginBridge->destroyRangeAudio(zego_range_audio_instance_index_first);
            // oInternalCallbackCenter->eraseZegoExpressRangeAudioImp(rangeAudio->getIndex());
            oInternalCallbackCenter->eraseZegoExpressRangeAudioImp(
                zego_range_audio_instance_index_first);
        }
    }

    //===================================================================================================
    IZegoCopyrightedMusic *createCopyrightedMusic() override {
        std::lock_guard<std::recursive_mutex> locker(
            oCopyrightedMusicContainer->copyrightedMusicMutex);
        if (oCopyrightedMusicContainer->copyrightedMusicInstance == nullptr) {
            oInternalOriginBridge->createCopyrightedMusic();
            auto newCopyrightedMusicInstance = std::make_shared<ZegoExpressCopyrightedMusicImp>(1);

            oInternalCallbackCenter->setZegoExpressCopyrightedMusicImp(newCopyrightedMusicInstance);
            oCopyrightedMusicContainer->copyrightedMusicInstance = newCopyrightedMusicInstance;
        }
        return oCopyrightedMusicContainer->copyrightedMusicInstance.get();
    }

    void destroyCopyrightedMusic(IZegoCopyrightedMusic *&copyrightedMusic) override {
        std::lock_guard<std::recursive_mutex> locker(
            oCopyrightedMusicContainer->copyrightedMusicMutex);
        if (copyrightedMusic != nullptr &&
            copyrightedMusic == oCopyrightedMusicContainer->copyrightedMusicInstance.get()) {
            oInternalOriginBridge->destroyCopyrightedMusic();
            oInternalCallbackCenter->setZegoExpressCopyrightedMusicImp(nullptr);
            oCopyrightedMusicContainer->copyrightedMusicInstance = nullptr;
            copyrightedMusic = nullptr;
        }
    }

    //===================================================================================================
    IZegoRangeScene *createRangeScene() override {
        int handle = oInternalOriginBridge->createRangeScene();
        if (handle == -1) {
            return nullptr;
        }

        auto rangeScene = std::make_shared<ZegoExpressRangeSceneImp>(handle);
        oInternalCallbackCenter->insertZegoExpressRangeSceneImp(handle, rangeScene);
        return rangeScene.get();
    }

    void destroyRangeScene(IZegoRangeScene *&rangeScene) override {
        if (rangeScene && oInternalCallbackCenter->getZegoExpressRangeSceneImp(
                              rangeScene->getRangeSceneHandle())) {
            int handle = rangeScene->getRangeSceneHandle();
            oInternalOriginBridge->destroyRangeScene(handle);
            oInternalCallbackCenter->eraseZegoExpressRangeSceneImp(handle);
        }
    }

    std::vector<ZegoScreenCaptureSourceInfo> getScreenCaptureSources(int thumbnailWidth,
                                                                     int thumbnailHeight,
                                                                     int iconWidth,
                                                                     int iconHeight) override {
        int count = 0;
        zego_screen_capture_source_info *source_info_list =
            oInternalOriginBridge->getScreenCaptureSources(thumbnailWidth, thumbnailHeight,
                                                           iconWidth, iconHeight, &count);

        std::vector<ZegoScreenCaptureSourceInfo> result;
        for (int i = 0; i < count; i++) {
            ZegoScreenCaptureSourceInfo sourceInfo =
                ZegoExpressConvert::I2OScreenCaptureSource(source_info_list[i]);
            result.push_back(sourceInfo);
        }

        oInternalOriginBridge->freeScreenCaptureSources(source_info_list, count);
        return result;
    }

    void freeScreenCaptureSources(std::vector<ZegoScreenCaptureSourceInfo> &sourceList) override {
        for (auto &source : sourceList) {
            if (source.iconImage.buffer) {
                delete[] source.iconImage.buffer;
                source.iconImage.buffer = nullptr;
            }

            if (source.thumbnailImage.buffer) {
                delete[] source.thumbnailImage.buffer;
                source.thumbnailImage.buffer = nullptr;
            }
        }
    }

    IZegoScreenCaptureSource *
    createScreenCaptureSource(void *sourceId, ZegoScreenCaptureSourceType sourceType) override {
        auto index = oInternalOriginBridge->createScreenCaptureSource(
            sourceId, (enum zego_screen_capture_source_type)sourceType);
        if (index == -1) {
            return nullptr;
        }

        auto source = std::make_shared<ZegoExpressScreenCaptureSourceImp>(index);
        oInternalCallbackCenter->insertZegoExpressScreenCaptureSourceImp(index, source);
        return source.get();
    }

    void destroyScreenCaptureSource(IZegoScreenCaptureSource *&source) override {
        if (source &&
            oInternalCallbackCenter->getZegoExpressScreenCaptureSourceImp(source->getIndex())) {
            oInternalOriginBridge->destroyScreenCaptureSource(source->getIndex());
            oInternalCallbackCenter->eraseZegoExpressScreenCaptureSourceImp(source->getIndex());
        }
    }

    //===================================================================================================
    IZegoMediaDataPublisher *createMediaDataPublisher() override {
        ZegoMediaDataPublisherConfig config;
        config.mode =
            ZegoMediaDataPublisherMode::ZEGO_MEDIA_DATA_PUBLISHER_MODE_BOTH_VIDEO_AND_AUDIO;
        config.channel = 0;
        return createMediaDataPublisher(config);
    }

    IZegoMediaDataPublisher *
    createMediaDataPublisher(ZegoMediaDataPublisherConfig config) override {
        auto index = oInternalOriginBridge->createMediaDataPublisher(config);
        if (index < 0) {
            return nullptr;
        }

        std::shared_ptr<ZegoExpressMediaDataPublisherImp> media_data_publisher_instance =
            oInternalCallbackCenter->getZegoExpressMediaDataPublisherImp(index);
        ;
        if (media_data_publisher_instance) {
            return media_data_publisher_instance.get();
        }

        media_data_publisher_instance = std::make_shared<ZegoExpressMediaDataPublisherImp>(index);
        oInternalCallbackCenter->insertZegoExpressMediaDataPublisherImp(
            index, media_data_publisher_instance);
        return media_data_publisher_instance.get();
    }

    void destroyMediaDataPublisher(IZegoMediaDataPublisher *&mediaDataPublisher) override {
        if (mediaDataPublisher) {
            int index = mediaDataPublisher->getIndex();
            oInternalOriginBridge->destroyMediaDataPublisher(index);
            oInternalCallbackCenter->eraseZegoExpressMediaDataPublisherImp(index);
        }
    }

    //===================================================================================================
    void enableCustomVideoCapture(bool enable, ZegoCustomVideoCaptureConfig *config,
                                  ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override {

        if (config == nullptr) {
            oInternalOriginBridge->enableCustomVideoCapture(enable, nullptr,
                                                            zego_publish_channel(channel));
        } else {
            zego_custom_video_capture_config _custom_video_capture_config;
            _custom_video_capture_config.buffer_type = zego_video_buffer_type(config->bufferType);
            oInternalOriginBridge->enableCustomVideoCapture(enable, &_custom_video_capture_config,
                                                            zego_publish_channel(channel));
        }
    }

    void
    setCustomVideoCaptureHandler(std::shared_ptr<IZegoCustomVideoCaptureHandler> handler) override {
        oInternalCallbackCenter->setIZegoCustomVideoCaptureHandler(handler);
    }

    void setCustomVideoCaptureFillMode(ZegoViewMode mode, ZegoPublishChannel channel) override {
        oInternalOriginBridge->customVideoCaptureSetFillMode(zego_view_mode(mode),
                                                             zego_publish_channel(channel));
    }

    void setCustomVideoCaptureDeviceState(
        bool isEnable, ZegoRemoteDeviceState state,
        ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override {
        oInternalOriginBridge->setCustomVideoCaptureDeviceState(
            isEnable, zego_remote_device_state(state), zego_publish_channel(channel));
    }

    void setCustomVideoCaptureRegionOfInterest(
        std::vector<ZegoRoiRect> rects,
        ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override {
        std::vector<zego_roi_rect> c_rects;
        for (auto &rect : rects) {
            zego_roi_rect c_rect;
            c_rect.x = rect.x;
            c_rect.y = rect.y;
            c_rect.width = rect.width;
            c_rect.height = rect.height;
            c_rect.strength = rect.strength;
            c_rects.emplace_back(std::move(c_rect));
        }
        oInternalOriginBridge->customVideoCaptureRegionOfInterest(c_rects.data(), c_rects.size(),
                                                                  zego_publish_channel(channel));
    }

    void sendCustomVideoCaptureRawData(const unsigned char *data, unsigned int dataLength,
                                       ZegoVideoFrameParam params,
                                       unsigned long long referenceTimeMillisecond,
                                       ZegoPublishChannel channel) override {
        auto _param = ZegoExpressConvert::O2IVideoFrameParam(params);
        oInternalOriginBridge->customVideoCaptureSendRawData(
            data, dataLength, _param, referenceTimeMillisecond, zego_publish_channel(channel));
    }

    int sendCustomVideoCaptureD3DTextureData(void *texture, int rotation,
                                             unsigned long long referenceTimeMillisecond,
                                             ZegoPublishChannel channel) override {
        return oInternalOriginBridge->customVideoCaptureSendD3DTextureData(
            texture, rotation, referenceTimeMillisecond, zego_publish_channel(channel));
    }

    void sendCustomVideoCaptureEncodedData(
        const unsigned char *data, unsigned int dataLength, ZegoVideoEncodedFrameParam params,
        unsigned long long referenceTimeMillisecond,
        ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override {
        auto _param = ZegoExpressConvert::O2IVideoEncodedFrameParam(params);
        oInternalOriginBridge->customVideoCaptureSendEncodedFrameData(
            data, dataLength, _param, referenceTimeMillisecond, zego_publish_channel(channel));
    }

    void
    enableCustomVideoProcessing(bool enable, ZegoCustomVideoProcessConfig *config,
                                ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override {

        if (config == nullptr) {
            oInternalOriginBridge->enableCustomVideoProcessing(enable, nullptr,
                                                               zego_publish_channel(channel));
        } else {
            zego_custom_video_process_config _custom_video_process_config;
            memset(&_custom_video_process_config, 0, sizeof(zego_custom_video_process_config));
            _custom_video_process_config.buffer_type = zego_video_buffer_type(config->bufferType);
            oInternalOriginBridge->enableCustomVideoProcessing(
                enable, &_custom_video_process_config, zego_publish_channel(channel));
        }
    }

    void
    setCustomVideoProcessHandler(std::shared_ptr<IZegoCustomVideoProcessHandler> handler) override {
        oInternalCallbackCenter->setIZegoCustomVideoProcessHandler(handler);
    }

    void sendCustomVideoProcessedRawData(const unsigned char **data, unsigned int *dataLength,
                                         ZegoVideoFrameParam params,
                                         unsigned long long referenceTimeMillisecond,
                                         ZegoPublishChannel channel) override {
        sendCustomVideoProcessedRawData(data, dataLength, params, referenceTimeMillisecond,
                                        ZEGO_PROCESSED_DATA_USAGE_TYPE_BOTH, channel);
    }

    void sendCustomVideoProcessedRawData(const unsigned char **data, unsigned int *dataLength,
                                         ZegoVideoFrameParam params,
                                         unsigned long long referenceTimeMillisecond,
                                         ZegoProcessedDataUsageType usage,
                                         ZegoPublishChannel channel) override {

        auto _param = ZegoExpressConvert::O2IVideoFrameParam(params);
        oInternalOriginBridge->sendCustomVideoProcessedRawData(
            data, dataLength, _param, referenceTimeMillisecond,
            zego_processed_data_usage_type(usage), zego_publish_channel(channel));
    }

    void sendCustomVideoProcessedCVPixelBuffer(void *buffer,
                                               unsigned long long referenceTimeMillisecond,
                                               ZegoPublishChannel channel) override {
        sendCustomVideoProcessedCVPixelBuffer(buffer, referenceTimeMillisecond,
                                              ZEGO_PROCESSED_DATA_USAGE_TYPE_BOTH, channel);
    }

    void sendCustomVideoProcessedCVPixelBuffer(void *buffer,
                                               unsigned long long referenceTimeMillisecond,
                                               ZegoProcessedDataUsageType usage,
                                               ZegoPublishChannel channel) override {

        oInternalOriginBridge->sendCustomVideoProcessedCVPixelBuffer(
            buffer, referenceTimeMillisecond, zego_processed_data_usage_type(usage),
            zego_publish_channel(channel));
    }

    void enableCustomAudioCaptureProcessing(bool enable,
                                            ZegoCustomAudioProcessConfig *config) override {
        if (config) {
            auto _config = ZegoExpressConvert::O2ICustomAudioProcessConfig(*config);
            oInternalOriginBridge->enableCustomAudioCaptureProcessing(enable, &_config);
        } else {
            oInternalOriginBridge->enableCustomAudioCaptureProcessing(enable, nullptr);
        }
    }

    void enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(
        bool enable, ZegoCustomAudioProcessConfig *config) override {
        if (config) {
            auto _config = ZegoExpressConvert::O2ICustomAudioProcessConfig(*config);
            oInternalOriginBridge->enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(
                enable, &_config);
        } else {
            oInternalOriginBridge->enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(enable,
                                                                                           nullptr);
        }
    }

    void enableBeforeAudioPrepAudioData(bool enable, ZegoAudioFrameParam param) override {
        zego_audio_frame_param _param = ZegoExpressConvert::O2IAudioFrameParam(param);
        oInternalOriginBridge->enableBeforeAudioPrepAudioData(enable, _param);
    }

    void enableCustomAudioRemoteProcessing(bool enable,
                                           ZegoCustomAudioProcessConfig *config) override {
        if (config) {
            auto _config = ZegoExpressConvert::O2ICustomAudioProcessConfig(*config);
            oInternalOriginBridge->enableCustomAudioRemoteProcessing(enable, &_config);
        } else {
            oInternalOriginBridge->enableCustomAudioRemoteProcessing(enable, nullptr);
        }
    }

    void enableCustomAudioPlaybackProcessing(bool enable,
                                             ZegoCustomAudioProcessConfig *config) override {
        if (config) {
            auto _config = ZegoExpressConvert::O2ICustomAudioProcessConfig(*config);
            oInternalOriginBridge->enableCustomAudioPlaybackProcessing(enable, &_config);
        } else {
            oInternalOriginBridge->enableCustomAudioPlaybackProcessing(enable, nullptr);
        }
    }

    void
    setCustomAudioProcessHandler(std::shared_ptr<IZegoCustomAudioProcessHandler> handler) override {
        oInternalCallbackCenter->setIZegoCustomAudioProcessHandler(handler);
    }

    //===================================================================================================
    void
    startRecordingCapturedData(ZegoDataRecordConfig config,
                               ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override {
        zego_data_record_config _config = ZegoExpressConvert::O2IDataRecordConfig(config);
        oInternalOriginBridge->startRecordingCapturedData(_config, zego_publish_channel(channel));
    }

    void
    stopRecordingCapturedData(ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override {
        oInternalOriginBridge->stopRecordingCapturedData(zego_publish_channel(channel));
    }

    void setDataRecordEventHandler(std::shared_ptr<IZegoDataRecordEventHandler> handler) override {
        oInternalCallbackCenter->setIZegoDataRecordEventHandler(handler);
    }

    //===================================================================================================
    void enableCustomVideoRender(bool enable, ZegoCustomVideoRenderConfig *config) override {
        if (config == nullptr) {
            oInternalOriginBridge->enableCustomVideoRender(enable, nullptr);
        } else {
            zego_custom_video_render_config _custom_video_render_config;
            _custom_video_render_config.buffer_type = zego_video_buffer_type(config->bufferType);
            _custom_video_render_config.frame_format_series =
                zego_video_frame_format_series(config->frameFormatSeries);
            _custom_video_render_config.enable_engine_render = config->enableEngineRender;
            oInternalOriginBridge->enableCustomVideoRender(enable, &_custom_video_render_config);
        }
    }

    void enableCapturedVideoCustomVideoRender(bool enable, ZegoPublishChannel channel) override {
        oInternalOriginBridge->enableCapturedVideoCustomVideoRender(enable,
                                                                    zego_publish_channel(channel));
    }

    void enableRemoteVideoCustomVideoRender(bool enable, const std::string &streamID) override {
        oInternalOriginBridge->enableRemoteVideoCustomVideoRender(enable, streamID.c_str());
    }

    void
    setCustomVideoRenderHandler(std::shared_ptr<IZegoCustomVideoRenderHandler> handler) override {
        oInternalCallbackCenter->setIZegoCustomVideoRenderHandler(handler);
    }

    //===================================================================================================
    void setAudioDataHandler(std::shared_ptr<IZegoAudioDataHandler> handler) override {
        oInternalCallbackCenter->setIZegoAudioDataHandler(handler);
    }

    void startAudioDataObserver(unsigned int observerBitMask, ZegoAudioFrameParam param) override {
        zego_audio_frame_param _param = ZegoExpressConvert::O2IAudioFrameParam(param);
        oInternalOriginBridge->startAudioDataObserver(observerBitMask, _param);
    }

    void stopAudioDataObserver() override { oInternalOriginBridge->stopAudioDataObserver(); }

    //===================================================================================================

    void enableCustomAudioIO(bool enable, ZegoCustomAudioConfig *config,
                             ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override {
        if (config == nullptr) {
            oInternalOriginBridge->enableCustomAudioIO(enable, nullptr,
                                                       zego_publish_channel(channel));
        } else {
            zego_custom_audio_config _config;
            _config.source_type = zego_audio_source_type(config->sourceType);
            oInternalOriginBridge->enableCustomAudioIO(enable, &_config,
                                                       zego_publish_channel(channel));
        }
    }

    void
    sendCustomAudioCaptureAACData(unsigned char *data, unsigned int dataLength,
                                  unsigned int configLength,
                                  unsigned long long referenceTimeMillisecond, unsigned int samples,
                                  ZegoAudioFrameParam param,
                                  ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override {
        zego_audio_frame_param _param = ZegoExpressConvert::O2IAudioFrameParam(param);
        oInternalOriginBridge->sendCustomAudioCaptureAACData(data, dataLength, configLength,
                                                             referenceTimeMillisecond, samples,
                                                             _param, zego_publish_channel(channel));
    }

    void
    sendCustomAudioCapturePCMData(unsigned char *data, unsigned int dataLength,
                                  ZegoAudioFrameParam param,
                                  ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override {
        zego_audio_frame_param _param = ZegoExpressConvert::O2IAudioFrameParam(param);
        oInternalOriginBridge->sendCustomAudioCapturePCMData(data, dataLength, _param,
                                                             zego_publish_channel(channel));
    }

    void fetchCustomAudioRenderPCMData(unsigned char *data, unsigned int dataLength,
                                       ZegoAudioFrameParam param) override {
        zego_audio_frame_param _param = ZegoExpressConvert::O2IAudioFrameParam(param);
        oInternalOriginBridge->fetchCustomAudioRenderPCMData(data, dataLength, _param);
    }

    void sendReferenceAudioPCMData(unsigned char *data, unsigned int dataLength,
                                   ZegoAudioFrameParam param) override {
        zego_audio_frame_param _param = ZegoExpressConvert::O2IAudioFrameParam(param);
        oInternalOriginBridge->sendReferenceAudioPCMData(data, dataLength, _param);
    }

    // void sendReferenceAudioPCMData(unsigned char *data, unsigned int dataLength,
    //                                ZegoAudioFrameParam param, ZegoPublishChannel channel) override {
    //     zego_audio_frame_param _param = ZegoExpressConvert::O2IAudioFrameParam(param);
    //     oInternalOriginBridge->sendReferenceAudioPCMData(data, dataLength, _param,
    //                                                      zego_publish_channel(channel));
    // }

    //===================================================================================================
    void startNetworkSpeedTest(ZegoNetworkSpeedTestConfig config, unsigned int interval) override {
        zego_network_speed_test_config _config;
        _config.test_uplink = config.testUplink;
        _config.expected_uplink_bitrate = config.expectedUplinkBitrate;
        _config.test_downlink = config.testDownlink;
        _config.expected_downlink_bitrate = config.expectedDownlinkBitrate;
        oInternalOriginBridge->startNetworkSpeedTest(_config, interval);
    }

    void stopNetworkSpeedTest() override { oInternalOriginBridge->stopNetworkSpeedTest(); }

    ZegoNetworkTimeInfo getNetworkTimeInfo() override {
        auto info = oInternalOriginBridge->getNetworkTimeInfo();
        return ZegoExpressConvert::I2ONetworkTimeInfo(info);
    }

    void startNetworkProbe(ZegoNetworkProbeConfig config,
                           ZegoNetworkProbeResultCallback callback) override {
        zego_network_probe_config _config;
        _config.enable_traceroute = config.enableTraceroute;
        int seq = oInternalOriginBridge->startNetworkProbe(_config);
        if (callback != nullptr) {
            oInternalCallbackCenter->insertZegoNetworkProbeResultCallback(seq, callback);
        }
    }

    void stopNetworkProbe() override { oInternalOriginBridge->stopNetworkProbe(); }

    void startDumpData(ZegoDumpDataConfig config) override {
        zego_dump_data_config _config;
        memset(&_config, 0, sizeof(zego_dump_data_config));
        _config.data_type = (zego_dump_data_type)config.dataType;
        oInternalOriginBridge->startDumpData(_config);
    }

    void stopDumpData() override { oInternalOriginBridge->stopDumpData(); }

    void uploadDumpData() override { oInternalOriginBridge->uploadDumpData(); }

    void removeDumpData() override { oInternalOriginBridge->removeDumpData(); }

#if TARGET_OS_IPHONE || defined(ANDROID)
    void initVideoSuperResolution() override { oInternalOriginBridge->initVideoSuperResolution(); }

    void uninitVideoSuperResolution() override {
        oInternalOriginBridge->uninitVideoSuperResolution();
    }

    void enableVideoSuperResolution(const std::string &streamID, bool enable) override {
        oInternalOriginBridge->enableVideoSuperResolution(streamID, enable);
    }
#endif
    void enableVideoObjectSegmentation(bool enable, ZegoObjectSegmentationType type,
                                       ZegoPublishChannel channel) override {
        oInternalOriginBridge->enableVideoObjectSegmentation(
            enable, (zego_object_segmentation_type)type, (zego_publish_channel)channel);
    }
    void enableVideoObjectSegmentation(bool enable, ZegoObjectSegmentationConfig config,
                                       ZegoPublishChannel channel) override {
        zego_object_segmentation_config config_c;
        config_c.object_segmentation_type =
            (zego_object_segmentation_type)config.objectSegmentationType;
        config_c.background_config.process_type =
            (zego_background_process_type)config.backgroundConfig.processType;
        config_c.background_config.color = config.backgroundConfig.color;
        strncpy(config_c.background_config.image_url, config.backgroundConfig.imageURL.c_str(),
                ZEGO_EXPRESS_MAX_URL_LEN);
        strncpy(config_c.background_config.video_url, config.backgroundConfig.videoURL.c_str(),
                ZEGO_EXPRESS_MAX_URL_LEN);
        config_c.background_config.blur_level =
            (zego_background_blur_level)config.backgroundConfig.blurLevel;

        oInternalOriginBridge->enableVideoObjectSegmentation(enable, config_c,
                                                             (zego_publish_channel)channel);
    }
    void enableAlphaChannelVideoEncoder(bool enable, ZegoAlphaLayoutType alphaLayout,
                                        ZegoPublishChannel channel) override {
        oInternalOriginBridge->enableAlphaChannelVideoEncoder(
            enable, (zego_alpha_layout_type)alphaLayout, (zego_publish_channel)channel);
    }

    int updatePlayingCanvas(const std::string &streamID, ZegoCanvas *canvas) override {
        int error = ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS;
        if (canvas == nullptr) {
            error = oInternalOriginBridge->updatePlayingCanvas(streamID.c_str(), nullptr);
        } else {
            zego_canvas _canvas = ZegoExpressConvert::O2ICanvas(*canvas);
            error = oInternalOriginBridge->updatePlayingCanvas(streamID.c_str(), &_canvas);
        }
        return error;
    }

    //===================================================================================================
    IZegoAIVoiceChanger *createAIVoiceChanger() override {
        int index = oInternalOriginBridge->createAIVoiceChanger();
        if (index == -1) {
            return nullptr;
        }

        auto ai_voice_changer = std::make_shared<ZegoExpressAIVoiceChangerImpl>(index);
        oInternalCallbackCenter->insertZegoExpressAIVoiceChangerImpl(index, ai_voice_changer);
        return ai_voice_changer.get();
    }

    void destroyAIVoiceChanger(IZegoAIVoiceChanger *&ai_voice_changer) override {
        if (ai_voice_changer) {
            int index = ai_voice_changer->getIndex();
            oInternalOriginBridge->destroyAIVoiceChanger(index);
            oInternalCallbackCenter->eraseZegoExpressAIVoiceChangerImpl(index);
        }
    }

    bool isAIVoiceChangerSupported() override {
        return oInternalOriginBridge->isAIVoiceChangerSupported();
    }

    void enableColorEnhancement(bool enable, ZegoColorEnhancementParams params,
                                ZegoPublishChannel channel) override {
        zego_color_enhancement_params p;
        p.intensity = params.intensity;
        p.skin_tone_protection_level = params.skinToneProtectionLevel;
        p.lip_color_protection_level = params.lipColorProtectionLevel;
        oInternalOriginBridge->enableColorEnhancement(enable, p,
                                                      static_cast<zego_publish_channel>(channel));
    }

  public:
    ///===================================================================================================
    static std::string getVersion() {
        if (!oInternalOriginBridge->getLibraryReady()) {
            return "unknow";
        }
        std::string version = oInternalOriginBridge->getVersion();
        return version;
    }

    static void setAndroidEnv(void *jvm, void *ctx) {
        if (!oInternalOriginBridge->getLibraryReady()) {
            return;
        }
        oInternalOriginBridge->setAndroidEnv(jvm, ctx);
    }

    static void setOhosEnv(void *env, void *exports) {
        oInternalOriginBridge->setOhosEnv(env, exports);
    }

    static void setEngineConfig(ZegoEngineConfig engineConfig) {
        if (!oInternalOriginBridge->getLibraryReady()) {
            return;
        }
        zego_engine_config _engineConfig;

        zego_log_config _logConfig;
        if (engineConfig.logConfig == nullptr) {
            _engineConfig.log_config = nullptr;
        } else {
            strncpy(_logConfig.log_path, engineConfig.logConfig->logPath.c_str(),
                    ZEGO_EXPRESS_MAX_COMMON_LEN);
            _logConfig.log_size = engineConfig.logConfig->logSize;
            _logConfig.log_count = engineConfig.logConfig->logCount;
            _engineConfig.log_config = &_logConfig;
        }

        std::string advanceConfig;
        for (auto iter : engineConfig.advancedConfig) {
            advanceConfig += iter.first + "=" + iter.second + ";";
        }
        strncpy(_engineConfig.advanced_config, advanceConfig.c_str(),
                ZEGO_EXPRESS_MAX_SET_CONFIG_VALUE_LEN);

        oInternalOriginBridge->setEngineConfig(_engineConfig);

        // Used to handle callback
        oInternalCallbackCenter->engineConfig = engineConfig;
    }

    static void setLogConfig(ZegoLogConfig config) {
        if (!oInternalOriginBridge->getLibraryReady()) {
            return;
        }
        zego_log_config _logConfig;
        strncpy(_logConfig.log_path, config.logPath.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
        _logConfig.log_size = config.logSize;
        _logConfig.log_count = config.logCount;
        oInternalOriginBridge->setLogConfig(_logConfig);
    }

    static void setApiCalledCallback(std::shared_ptr<IZegoApiCalledEventHandler> callback) {
        if (!oInternalOriginBridge->getLibraryReady()) {
            return;
        }
        oInternalCallbackCenter->registerApiCalledResultCallback();
        oInternalCallbackCenter->setIZegoApiCalledEventHandler(callback);
    }

    static bool isFeatureSupported(ZegoFeatureType featureType) {
        if (!oInternalOriginBridge->getLibraryReady()) {
            return false;
        }
        return oInternalOriginBridge->isFeatureSupported((zego_feature_type)featureType);
    }

    static void setRoomMode(ZegoRoomMode mode) {
        if (!oInternalOriginBridge->getLibraryReady()) {
            return;
        }
        oInternalOriginBridge->setRoomMode((zego_room_mode)mode);
    }

    static void setGeoFence(ZegoGeoFenceType type, std::vector<int> areaList) {
        if (!oInternalOriginBridge->getLibraryReady()) {
            return;
        }
        oInternalOriginBridge->setGeoFence(type, areaList);
    }

    static void setLocalProxyConfig(const std::vector<ZegoProxyInfo> &proxyList, bool enable) {
        if (!oInternalOriginBridge->getLibraryReady()) {
            return;
        }
        std::vector<zego_proxy_info> _proxyList;
        for (auto proxy : proxyList) {
            zego_proxy_info _proxy = ZegoExpressConvert::O2IProxyInfo(proxy);
            _proxyList.push_back(_proxy);
        }
        zego_proxy_info *proxy = _proxyList.data();
        oInternalOriginBridge->setLocalProxyConfig(proxy, ZegoUInt(_proxyList.size()), enable);
    }

    static void setCloudProxyConfig(const std::vector<ZegoProxyInfo> &proxyList,
                                    const std::string &token, bool enable) {
        if (!oInternalOriginBridge->getLibraryReady()) {
            return;
        }
        std::vector<zego_proxy_info> _proxyList;
        for (auto proxy : proxyList) {
            zego_proxy_info _proxy = ZegoExpressConvert::O2IProxyInfo(proxy);
            _proxyList.push_back(_proxy);
        }
        zego_proxy_info *proxy = _proxyList.data();
        oInternalOriginBridge->setCloudProxyConfig(proxy, ZegoUInt(_proxyList.size()),
                                                   token.c_str(), enable);
    }

    static void setLicense(const std::string &license) {
        if (!oInternalOriginBridge->getLibraryReady()) {
            return;
        }
        oInternalOriginBridge->setLicense(license.c_str());
    }

    static int loadLibrary(const std::string &sdk_library_full_path) {
        return oInternalOriginBridge->loadLibrary(sdk_library_full_path);
    }

    static void unLoadLibrary() { oInternalOriginBridge->unLoadLibrary(); }

    // static void setAdaptiveVideoConfig(const ZegoAdaptiveVideoConfig &config) {
    //     zego_adaptive_video_config _config = ZegoExpressConvert::O2IAdaptiveVideoConfig(config);
    //     oInternalOriginBridge->setAdaptiveVideoConfig(&_config);
    // }

    bool init(unsigned int appID, std::string appSign, bool isTestEnvironment,
              ZegoScenario scenario, std::shared_ptr<IZegoEventHandler> eventHandler) {
        if (!oInternalOriginBridge->getLibraryReady()) {
            return false;
        }
        oInternalCallbackCenter->registerCallback(true);
        oInternalCallbackCenter->setIZegoEventHandler(eventHandler);

        int errorCode = oInternalOriginBridge->init(appID, appSign.c_str(), isTestEnvironment,
                                                    zego_scenario(scenario));
        if (errorCode != ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS) {
            if (eventHandler) {
                eventHandler->onDebugError(errorCode, "CreateEngine", "CreateEngine failed");
            }

            oInternalCallbackCenter->unregisterCallback();
            oInternalCallbackCenter->clearHandlerData();
        }
        return errorCode == ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS;
    }

    bool init(const ZegoEngineProfile &profile, std::shared_ptr<IZegoEventHandler> eventHandler) {
        if (!oInternalOriginBridge->getLibraryReady()) {
            return false;
        }
        oInternalCallbackCenter->registerCallback(profile.callbackSwitchToMainThread);
        oInternalCallbackCenter->setIZegoEventHandler(eventHandler);

        int errorCode = oInternalOriginBridge->init(profile);
        if (errorCode != ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS) {
            if (eventHandler) {
                eventHandler->onDebugError(errorCode, "CreateEngine", "CreateEngine failed");
            }

            oInternalCallbackCenter->unregisterCallback();
            oInternalCallbackCenter->clearHandlerData();
        }
        return errorCode == ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS;
    }

    void uinitAsync(ZegoDestroyCompletionCallback afterDestroyed) {
        if (!oInternalOriginBridge->getLibraryReady()) {
            return;
        }
        oInternalCallbackCenter->setZegoDestroyCompletionCallback(afterDestroyed);
        oInternalCallbackCenter->unregisterCallback();
        oInternalCallbackCenter->clearHandlerData();
        oInternalCallbackCenter->clearContainerData();
        oInternalOriginBridge->uninitAsync();
        oInternalCallbackCenter->unRegisterApiCalledResultCallback();
    }
};

class ZegoExpressEngineContainer {
  public:
    std::recursive_mutex engineMutex;
    std::shared_ptr<ZegoExpressEngineImp> engineInstance = nullptr;
};
#define oEngineContainer ZegoSingleton<ZegoExpressEngineContainer>::CreateInstance()

class ZegoExpressSDKInternal {
  public:
    static void setEngineConfig(ZegoEngineConfig engineConfig) {
        std::lock_guard<std::recursive_mutex> locker(oEngineContainer->engineMutex);
        ZegoExpressEngineImp::setEngineConfig(engineConfig);
    }
    static void setLogConfig(ZegoLogConfig config) {
        std::lock_guard<std::recursive_mutex> locker(oEngineContainer->engineMutex);
        ZegoExpressEngineImp::setLogConfig(config);
    }

    static void setRoomMode(ZegoRoomMode mode) {
        std::lock_guard<std::recursive_mutex> locker(oEngineContainer->engineMutex);
        ZegoExpressEngineImp::setRoomMode(mode);
    }

    static void setGeoFence(ZegoGeoFenceType type, std::vector<int> areaList) {
        std::lock_guard<std::recursive_mutex> locker(oEngineContainer->engineMutex);
        ZegoExpressEngineImp::setGeoFence(type, areaList);
    }

    static IZegoExpressEngine *createEngine(ZegoEngineProfile profile,
                                            std::shared_ptr<IZegoEventHandler> eventHandler) {
        std::lock_guard<std::recursive_mutex> locker(oEngineContainer->engineMutex);
        if (oEngineContainer->engineInstance == nullptr) {
            auto newEngineInstance = std::make_shared<ZegoExpressEngineImp>();
            if (newEngineInstance->init(profile, eventHandler)) {
                oEngineContainer->engineInstance = newEngineInstance;
            }
        }
        return oEngineContainer->engineInstance.get();
    }

    static IZegoExpressEngine *createEngine(unsigned int appID, std::string appSign,
                                            bool isTestEnvironment, ZegoScenario scenario,
                                            std::shared_ptr<IZegoEventHandler> eventHandler) {
        std::lock_guard<std::recursive_mutex> locker(oEngineContainer->engineMutex);
        if (oEngineContainer->engineInstance == nullptr) {
            auto newEngineInstance = std::make_shared<ZegoExpressEngineImp>();
            if (newEngineInstance->init(appID, appSign, isTestEnvironment, scenario,
                                        eventHandler)) {
                oEngineContainer->engineInstance = newEngineInstance;
            }
        }
        return oEngineContainer->engineInstance.get();
    }

    static void destroyEngine(IZegoExpressEngine *&iEngine,
                              ZegoDestroyCompletionCallback afterDestroyed) {
        std::lock_guard<std::recursive_mutex> locker(oEngineContainer->engineMutex);
        if (iEngine != nullptr && iEngine == oEngineContainer->engineInstance.get()) {
            oEngineContainer->engineInstance->uinitAsync(afterDestroyed);
            oEngineContainer->engineInstance = nullptr;
            iEngine = nullptr;
        }
    }

    static IZegoExpressEngine *getEngine() {
        std::lock_guard<std::recursive_mutex> locker(oEngineContainer->engineMutex);
        return oEngineContainer->engineInstance.get();
    }

    static std::string getVersion() { return ZegoExpressEngineImp::getVersion(); }

    static void setAndroidEnv(void *jvm, void *ctx) {
        return ZegoExpressEngineImp::setAndroidEnv(jvm, ctx);
    }

    static void setOhosEnv(void *env, void *exports) {
        return ZegoExpressEngineImp::setOhosEnv(env, exports);
    }

    static void setApiCalledCallback(std::shared_ptr<IZegoApiCalledEventHandler> callback) {
        ZegoExpressEngineImp::setApiCalledCallback(callback);
    }

    static void setLocalProxyConfig(const std::vector<ZegoProxyInfo> &proxyList, bool enable) {
        std::lock_guard<std::recursive_mutex> locker(oEngineContainer->engineMutex);
        ZegoExpressEngineImp::setLocalProxyConfig(proxyList, enable);
    }

    static void setCloudProxyConfig(const std::vector<ZegoProxyInfo> &proxyList,
                                    const std::string &token, bool enable) {
        std::lock_guard<std::recursive_mutex> locker(oEngineContainer->engineMutex);
        ZegoExpressEngineImp::setCloudProxyConfig(proxyList, token, enable);
    }

    static void setLicense(const std::string &license) {
        ZegoExpressEngineImp::setLicense(license);
    }

    static bool isFeatureSupported(ZegoFeatureType featureType) {
        return ZegoExpressEngineImp::isFeatureSupported(featureType);
    }

    static void submitLog() { ZegoExpressEngineImp::submitLog(); }

    static int loadLibrary(const std::string &sdk_library_full_path) {
        return ZegoExpressEngineImp::loadLibrary(sdk_library_full_path);
    }

    static void unLoadLibrary() { ZegoExpressEngineImp::unLoadLibrary(); }

    // static void setAdaptiveVideoConfig(const ZegoAdaptiveVideoConfig &config) {
    //     ZegoExpressEngineImp::setAdaptiveVideoConfig(config);
    // }
};

} //namespace EXPRESS
} //namespace ZEGO

ZEGO_ENABLE_DEPRECATION_WARNINGS
