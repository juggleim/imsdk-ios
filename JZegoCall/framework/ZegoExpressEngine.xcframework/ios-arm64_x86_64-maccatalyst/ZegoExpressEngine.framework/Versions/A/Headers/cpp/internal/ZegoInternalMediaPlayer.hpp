#pragma once

#include "../ZegoExpressDefines.h"
#include "../ZegoExpressEventHandler.h"
#include "../ZegoExpressInterface.h"

#include "ZegoInternalBase.h"
#include "ZegoInternalBridge.h"

ZEGO_DISABLE_DEPRECATION_WARNINGS

namespace ZEGO {
namespace EXPRESS {

class ZegoExpressMediaPlayerImp : public IZegoMediaPlayer,
                                  public std::enable_shared_from_this<ZegoExpressMediaPlayerImp> {
  public:
    ZegoExpressMediaPlayerImp(int index) : instanceIndex(index) {}

    void setEventHandler(std::shared_ptr<IZegoMediaPlayerEventHandler> handler) override {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        mpEventHandler = handler;
    }

    void setVideoHandler(std::shared_ptr<IZegoMediaPlayerVideoHandler> handler,
                         ZegoVideoFrameFormat format) override {
        {
            std::lock_guard<std::mutex> lock(mediaEventMutex);
            mpVideoHandler = handler;
        }
        if (handler) {
            oInternalOriginBridge->mediaPlayerEnableVideoData(
                true, zego_video_frame_format(format),
                zego_media_player_instance_index(instanceIndex));
        } else {
            oInternalOriginBridge->mediaPlayerEnableVideoData(
                false, zego_video_frame_format(format),
                zego_media_player_instance_index(instanceIndex));
        }
    }

    void setAudioHandler(std::shared_ptr<IZegoMediaPlayerAudioHandler> handler) override {
        {
            std::lock_guard<std::mutex> lock(mediaEventMutex);
            mpAudioHandler = handler;
        }
        if (handler) {
            oInternalOriginBridge->mediaPlayerEnableAudioData(
                true, zego_media_player_instance_index(instanceIndex));
        } else {
            oInternalOriginBridge->mediaPlayerEnableAudioData(
                false, zego_media_player_instance_index(instanceIndex));
        }
    }

    void setBlockDataHandler(std::shared_ptr<IZegoMediaPlayerBlockDataHandler> handler,
                             unsigned int blockSize) override {
        {
            std::lock_guard<std::mutex> lock(mediaEventMutex);
            mpBlockDataHandler = handler;
        }
        if (handler) {
            oInternalOriginBridge->mediaPlayerEnableBlockData(
                true, blockSize, zego_media_player_instance_index(instanceIndex));
        } else {
            oInternalOriginBridge->mediaPlayerEnableBlockData(
                false, blockSize, zego_media_player_instance_index(instanceIndex));
        }
    }

    void loadResource(const std::string &sourcePath,
                      ZegoMediaPlayerLoadResourceCallback callback) override {
        int errorCode = oInternalOriginBridge->mediaPlayerPreload(
            sourcePath.c_str(), zego_media_player_instance_index(instanceIndex));

        handleLoadResourceCallback(errorCode, callback);
    }

    void loadResourceWithPosition(const std::string &sourcePath, unsigned long long startPosition,
                                  ZegoMediaPlayerLoadResourceCallback callback) override {
        int errorCode = oInternalOriginBridge->mediaPlayerPreloadWithPosition(
            sourcePath.c_str(), startPosition, zego_media_player_instance_index(instanceIndex));

        handleLoadResourceCallback(errorCode, callback);
    }

    void loadResourceFromMediaData(unsigned char *media_data, int media_data_length,
                                   unsigned long long start_position,
                                   ZegoMediaPlayerLoadResourceCallback callback) override {
        int errorCode = oInternalOriginBridge->mediaPlayerPreload(
            media_data, media_data_length, start_position,
            zego_media_player_instance_index(instanceIndex));

        handleLoadResourceCallback(errorCode, callback);
    }

    void loadCopyrightedMusicResourceWithPosition(
        const std::string &resourceId, unsigned long long startPosition,
        ZegoMediaPlayerLoadResourceCallback callback) override {
        int errorCode =
            oInternalOriginBridge->mediaPlayerPreloadCopyrightedMusicResourceWithPosition(
                resourceId.c_str(), startPosition, zego_media_player_instance_index(instanceIndex));

        handleLoadResourceCallback(errorCode, callback);
    }

    void loadResourceWithConfig(ZegoMediaPlayerResource resource,
                                ZegoMediaPlayerLoadResourceCallback callback) override {
        auto resource_c = ZegoExpressConvert::O2IMediaPlayerResource(resource);
        int errorCode = oInternalOriginBridge->mediaPlayerPreloadResourceWithConfig(
            &resource_c, zego_media_player_instance_index(instanceIndex));

        handleLoadResourceCallback(errorCode, callback);
    }

    void start() override {
        oInternalOriginBridge->mediaPlayerStart(zego_media_player_instance_index(instanceIndex));
    }

    void stop() override {
        oInternalOriginBridge->mediaPlayerStop(zego_media_player_instance_index(instanceIndex));
    }

    void pause() override {
        oInternalOriginBridge->mediaPlayerPause(zego_media_player_instance_index(instanceIndex));
    }

    void resume() override {
        oInternalOriginBridge->mediaPlayerResume(zego_media_player_instance_index(instanceIndex));
    }

    void seekTo(unsigned long long millisecond, ZegoMediaPlayerSeekToCallback callback) override {
        auto seq = oInternalOriginBridge->getIncreaseSeq();
        if (callback != nullptr) {
            std::lock_guard<std::mutex> lock(mediaEventMutex);
            mpSeekToCallbacks.insert({seq, callback});
        }

        oInternalOriginBridge->mediaPlayerSeekTo(
            millisecond, zego_media_player_instance_index(instanceIndex), seq);
    }

    void enableRepeat(bool enable) override {
        oInternalOriginBridge->mediaPlayerEnableRepeat(
            enable, zego_media_player_instance_index(instanceIndex));
    }

    void setPlayLoopCount(int count) override {
        oInternalOriginBridge->mediaPlayerSetPlayLoopCount(
            count, zego_media_player_instance_index(instanceIndex));
    }

    void setPlaySpeed(float speed) override {
        oInternalOriginBridge->mediaPlayerSetPlaySpeed(
            speed, zego_media_player_instance_index(instanceIndex));
    }

    void enableAux(bool enable) override {
        oInternalOriginBridge->mediaPlayerEnableAUX(
            enable, zego_media_player_instance_index(instanceIndex));
    }

    void muteLocal(bool mute) override {
        oInternalOriginBridge->mediaPlayerMuteLocal(
            mute, zego_media_player_instance_index(instanceIndex));
    }

    void enableViewMirror(bool enable) override {
        oInternalOriginBridge->mediaPlayerEnableViewMirror(
            enable, zego_media_player_instance_index(instanceIndex));
    }

    void setPlayerCanvas(ZegoCanvas *canvas) override {
        if (canvas == nullptr) {
            oInternalOriginBridge->mediaPlayerSetPlayerCanvas(
                nullptr, zego_media_player_instance_index(instanceIndex));
        } else {
            zego_canvas _canvas;
            _canvas.view = canvas->view;
            _canvas.view_mode = zego_view_mode(canvas->viewMode);
            _canvas.background_color = canvas->backgroundColor;
            _canvas.alpha_blend = canvas->alphaBlend;
            oInternalOriginBridge->mediaPlayerSetPlayerCanvas(
                &_canvas, zego_media_player_instance_index(instanceIndex));
        }
    }

    void setVolume(int volume) override {
        oInternalOriginBridge->mediaPlayerSetVolume(
            volume, zego_media_player_instance_index(instanceIndex));
    }

    void setPlayVolume(int volume) override {
        oInternalOriginBridge->mediaPlayerSetPlayVolume(
            volume, zego_media_player_instance_index(instanceIndex));
    }

    void setPublishVolume(int volume) override {
        oInternalOriginBridge->mediaPlayerSetPublishVolume(
            volume, zego_media_player_instance_index(instanceIndex));
    }

    void setProgressInterval(unsigned long long millisecond) override {
        oInternalOriginBridge->mediaPlayerSetProgressInterval(
            millisecond, zego_media_player_instance_index(instanceIndex));
    }

    unsigned long long getTotalDuration() override {
        return oInternalOriginBridge->mediaPlayerGetTotalDuration(
            zego_media_player_instance_index(instanceIndex));
    }

    unsigned long long getCurrentProgress() override {
        return oInternalOriginBridge->mediaPlayerGetCurrentProgress(
            zego_media_player_instance_index(instanceIndex));
    }

    unsigned long long getCurrentRenderingProgress() override {
        return oInternalOriginBridge->mediaPlayerGetCurrentRenderingProgress(
            zego_media_player_instance_index(instanceIndex));
    }

    int getPlayVolume() override {
        return oInternalOriginBridge->mediaPlayerGetPlayVolume(
            zego_media_player_instance_index(instanceIndex));
    }

    int getPublishVolume() override {
        return oInternalOriginBridge->mediaPlayerGetPublishVolume(
            zego_media_player_instance_index(instanceIndex));
    }

    ZegoMediaPlayerState getCurrentState() override {
        zego_media_player_state state = oInternalOriginBridge->mediaPlayerGetCurrentState(
            zego_media_player_instance_index(instanceIndex));
        return ZegoMediaPlayerState(state);
    }

    unsigned int getAudioTrackCount() override {
        unsigned int count = oInternalOriginBridge->mediaPlayerGetAudioTrackCount(
            zego_media_player_instance_index(instanceIndex));
        return count;
    }

    void setAudioTrackIndex(unsigned int index) override {
        oInternalOriginBridge->mediaPlayerSetAudioTrackIndex(
            index, zego_media_player_instance_index(instanceIndex));
    }

    void setAudioTrackMode(ZegoMediaPlayerAudioTrackMode mode) override {
        oInternalOriginBridge->mediaPlayerSetAudioTrackMode(
            zego_media_player_audio_track_mode(mode),
            zego_media_player_instance_index(instanceIndex));
    }

    void setAudioTrackPublishIndex(unsigned int index) override {
        oInternalOriginBridge->mediaPlayerSetAudioTrackPublishIndex(
            index, zego_media_player_instance_index(instanceIndex));
    }

    void setVoiceChangerParam(ZegoMediaPlayerAudioChannel audioChannel,
                              ZegoVoiceChangerParam param) override {
        oInternalOriginBridge->mediaPlayerSetVoiceChangerParam(
            zego_media_player_audio_channel(audioChannel), param.pitch,
            zego_media_player_instance_index(instanceIndex));
    }

    void enableVoiceChanger(ZegoMediaPlayerAudioChannel audioChannel, bool enable,
                            ZegoVoiceChangerParam param) override {
        oInternalOriginBridge->mediaPlayerEnableVoiceChanger(
            zego_media_player_audio_channel(audioChannel), enable, param.pitch,
            zego_media_player_instance_index(instanceIndex));
    }

    int getIndex() override { return instanceIndex; }

    void takeSnapshot(ZegoMediaPlayerTakeSnapshotCallback callback) override {
        oInternalOriginBridge->takeSnapshot(zego_media_player_instance_index(instanceIndex));
        if (callback != nullptr) {
            std::lock_guard<std::mutex> lock(mediaEventMutex);
            mpTakeSnapshotCallback = callback;
        }
    }

    void enableAccurateSeek(bool enable, ZegoAccurateSeekConfig *config) override {
        zego_accurate_seek_config _zego_accurate_seek_config;
        memset(&_zego_accurate_seek_config, 0, sizeof(zego_accurate_seek_config));
        if (config == nullptr) {
            _zego_accurate_seek_config.time_out = 5000;

        } else {
            _zego_accurate_seek_config.time_out = config->timeout;
        }
        oInternalOriginBridge->mediaPlayerEnableAccurateSeek(
            enable, &_zego_accurate_seek_config, zego_media_player_instance_index(instanceIndex));
    }

    void setNetWorkResourceMaxCache(unsigned int time, unsigned int size) override {
        oInternalOriginBridge->mediaPlayerSetNetWorkResourceMaxCache(
            time, size, zego_media_player_instance_index(instanceIndex));
    };

    ZegoNetWorkResourceCache *getNetWorkResourceCache() override {
        zego_network_resource_cache network_resource_cache;
        memset(&network_resource_cache, 0, sizeof(zego_network_resource_cache));
        oInternalOriginBridge->mediaPlayerGetNetWorkResourceCache(
            &network_resource_cache, zego_media_player_instance_index(instanceIndex));
        ZegoNetWorkResourceCache *cache_result = new ZegoNetWorkResourceCache();
        cache_result->size = network_resource_cache.size;
        cache_result->time = network_resource_cache.time;
        return cache_result;
    };

    void setNetWorkBufferThreshold(unsigned int threshold) override {
        oInternalOriginBridge->mediaPlayerSetNetWorkBufferThreshold(
            threshold, zego_media_player_instance_index(instanceIndex));
    }

    void enableSoundLevelMonitor(bool enable, unsigned int millisecond) override {
        oInternalOriginBridge->mediaPlayerEnableSoundLevelMonitor(
            enable, millisecond, zego_media_player_instance_index(instanceIndex));
    }

    void enableFrequencySpectrumMonitor(bool enable, unsigned int millisecond) override {
        oInternalOriginBridge->mediaPlayerEnableFrequencySpectrumMonitor(
            enable, millisecond, zego_media_player_instance_index(instanceIndex));
    }

    void setActiveAudioChannel(ZegoMediaPlayerAudioChannel audioChannel) override {
        oInternalOriginBridge->mediaPlayerSetActiveAudioChannel(
            zego_media_player_audio_channel(audioChannel),
            zego_media_player_instance_index(instanceIndex));
    }

    void clearView() override {
        oInternalOriginBridge->mediaPlayerClearView(
            zego_media_player_instance_index(instanceIndex));
    }

    ZegoMediaPlayerMediaInfo getMediaInfo() override {
        zego_media_player_media_info media_info;
        memset(&media_info, 0, sizeof(zego_media_player_media_info));
        oInternalOriginBridge->mediaPlayerGetMediaInfo(
            &media_info, zego_media_player_instance_index(instanceIndex));

        ZegoMediaPlayerMediaInfo media_info_result;
        media_info_result.width = media_info.width;
        media_info_result.height = media_info.height;
        media_info_result.frameRate = media_info.frame_rate;
        return media_info_result;
    }

    void updatePosition(const float position[3]) override {
        oInternalOriginBridge->mediaPlayerUpdatePosition(
            position, zego_media_player_instance_index(instanceIndex));
    }

    void setHttpHeader(std::unordered_map<std::string, std::string> headers) override {
        oInternalOriginBridge->mediaPlayerSetHttpHeader(
            headers, zego_media_player_instance_index(instanceIndex));
    }
    void enableLiveAudioEffect(bool enable, ZegoLiveAudioEffectMode mode) override {
        oInternalOriginBridge->mediaPlayerEnableLiveAudioEffect(
            enable, zego_live_audio_effect_mode(mode),
            zego_media_player_instance_index(instanceIndex));
    }

    void enableLocalCache(bool enable, const std::string &cacheDir) override {
        oInternalOriginBridge->mediaPlayerEnableLocalCache(
            enable, cacheDir.c_str(), zego_media_player_instance_index(instanceIndex));
    }

    void setPlayMediaStreamType(ZegoMediaStreamType streamType) override {
        oInternalOriginBridge->mediaPlayerSetPlayMediaStreamType(
            static_cast<zego_media_stream_type>(streamType),
            zego_media_player_instance_index(instanceIndex));
    }
    ZegoMediaPlayerStatisticsInfo getPlaybackStatistics() override {
        zego_media_player_statistics_info stat_c;
        memset(&stat_c, 0, sizeof(zego_media_player_statistics_info));
        oInternalOriginBridge->mediaPlayerGetPlaybackStatistics(
            &stat_c, zego_media_player_instance_index(instanceIndex));

        ZegoMediaPlayerStatisticsInfo stat;
        stat.videoSourceFps = stat_c.video_source_fps;
        stat.videoDecodeFps = stat_c.video_decode_fps;
        stat.videoRenderFps = stat_c.video_render_fps;
        stat.audioSourceFps = stat_c.audio_source_fps;
        stat.audioDecodeFps = stat_c.audio_decode_fps;
        stat.audioRenderFps = stat_c.audio_render_fps;

        return stat;
    }

    void zego_on_media_player_state_update(zego_media_player_state state, zego_error error_code) {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        if (mpEventHandler) {
            auto weakMPEventHandler = std::weak_ptr<IZegoMediaPlayerEventHandler>(mpEventHandler);
            auto weak_self = std::weak_ptr<ZegoExpressMediaPlayerImp>(shared_from_this());
            ZEGO_SWITCH_THREAD_PRE
            auto handlerInMain = weakMPEventHandler.lock();
            auto self = weak_self.lock();
            if (handlerInMain && self) {
                handlerInMain->onMediaPlayerStateUpdate(this, ZegoMediaPlayerState(state),
                                                        error_code);
            }
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_media_player_network_event(zego_media_player_network_event event) {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        auto weakMPEventHandler = std::weak_ptr<IZegoMediaPlayerEventHandler>(mpEventHandler);
        auto weak_self = std::weak_ptr<ZegoExpressMediaPlayerImp>(shared_from_this());
        ZEGO_SWITCH_THREAD_PRE
        auto handlerInMain = weakMPEventHandler.lock();
        auto self = weak_self.lock();
        if (handlerInMain && self) {
            handlerInMain->onMediaPlayerNetworkEvent(this, ZegoMediaPlayerNetworkEvent(event));
        }
        ZEGO_SWITCH_THREAD_ING
    }

    void zego_on_media_player_playing_progress(unsigned long long millisecond) {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        auto weakMPEventHandler = std::weak_ptr<IZegoMediaPlayerEventHandler>(mpEventHandler);
        auto weak_self = std::weak_ptr<ZegoExpressMediaPlayerImp>(shared_from_this());
        ZEGO_SWITCH_THREAD_PRE
        auto handlerInMain = weakMPEventHandler.lock();
        auto self = weak_self.lock();
        if (handlerInMain && self) {
            handlerInMain->onMediaPlayerPlayingProgress(this, millisecond);
        }
        ZEGO_SWITCH_THREAD_ING
    }

    void zego_on_media_player_rendering_progress(unsigned long long millisecond) {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        auto weakMPEventHandler = std::weak_ptr<IZegoMediaPlayerEventHandler>(mpEventHandler);
        auto weak_self = std::weak_ptr<ZegoExpressMediaPlayerImp>(shared_from_this());
        ZEGO_SWITCH_THREAD_PRE
        auto handlerInMain = weakMPEventHandler.lock();
        auto self = weak_self.lock();
        if (handlerInMain && self) {
            handlerInMain->onMediaPlayerRenderingProgress(this, millisecond);
        }
        ZEGO_SWITCH_THREAD_ING
    }

    void zego_on_media_player_video_size_changed(int width, int height) {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        auto weakMPEventHandler = std::weak_ptr<IZegoMediaPlayerEventHandler>(mpEventHandler);
        auto weak_self = std::weak_ptr<ZegoExpressMediaPlayerImp>(shared_from_this());
        ZEGO_SWITCH_THREAD_PRE
        auto handlerInMain = weakMPEventHandler.lock();
        auto self = weak_self.lock();
        if (handlerInMain && self) {
            handlerInMain->onMediaPlayerVideoSizeChanged(this, width, height);
        }
        ZEGO_SWITCH_THREAD_ING
    }

    void zego_on_media_player_recv_sei(const unsigned char *data, unsigned int data_length) {
        std::vector<unsigned char> seiData;
        seiData.reserve(data_length);
        for (unsigned int i = 0; i < data_length; i++) {
            seiData.push_back(data[i]);
        }

        std::lock_guard<std::mutex> lock(mediaEventMutex);
        auto weakMPEventHandler = std::weak_ptr<IZegoMediaPlayerEventHandler>(mpEventHandler);
        auto weak_self = std::weak_ptr<ZegoExpressMediaPlayerImp>(shared_from_this());
        ZEGO_SWITCH_THREAD_PRE
        auto handlerInMain = weakMPEventHandler.lock();
        auto self = weak_self.lock();
        if (handlerInMain && self) {
            handlerInMain->onMediaPlayerRecvSEI(this, seiData.data(), data_length);
        }
        ZEGO_SWITCH_THREAD_ING
    }

    void zego_on_media_player_sound_level_update(float sound_level) {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        auto weakMPEventHandler = std::weak_ptr<IZegoMediaPlayerEventHandler>(mpEventHandler);
        auto weak_self = std::weak_ptr<ZegoExpressMediaPlayerImp>(shared_from_this());
        ZEGO_SWITCH_THREAD_PRE
        auto handlerInMain = weakMPEventHandler.lock();
        auto self = weak_self.lock();
        if (handlerInMain && self) {
            handlerInMain->onMediaPlayerSoundLevelUpdate(this, sound_level);
        }
        ZEGO_SWITCH_THREAD_ING
    }

    void zego_on_media_player_frequency_spectrum_update(float *spectrum_list,
                                                        unsigned int spectrum_count) {
        ZegoAudioSpectrum spectrumList;
        for (unsigned int i = 0; i < spectrum_count; i++) {
            spectrumList.push_back(spectrum_list[i]);
        }
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        auto weakMPEventHandler = std::weak_ptr<IZegoMediaPlayerEventHandler>(mpEventHandler);
        auto weak_self = std::weak_ptr<ZegoExpressMediaPlayerImp>(shared_from_this());
        ZEGO_SWITCH_THREAD_PRE
        auto handlerInMain = weakMPEventHandler.lock();
        auto self = weak_self.lock();
        if (handlerInMain && self) {
            handlerInMain->onMediaPlayerFrequencySpectrumUpdate(this, spectrumList);
        }
        ZEGO_SWITCH_THREAD_ING
    }

    void zego_on_mediaplayer_seek_to_result(zego_seq seq, zego_error error_code) {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        if (mpSeekToCallbacks.count(seq) > 0) {
            auto cb = mpSeekToCallbacks[seq];
            mpSeekToCallbacks.erase(seq);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_mediaplayer_load_file_result(zego_error error_code) {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        if (mpLoadResourceCallbacks.empty()) {
            return;
        }

        ZegoMediaPlayerLoadResourceCallback callback = mpLoadResourceCallbacks.front();
        mpLoadResourceCallbacks.pop_front();
        if (callback) {
            ZEGO_SWITCH_THREAD_PRE
            callback(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_media_player_first_frame_event(enum zego_media_player_first_frame_event event) {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        auto weakMPEventHandler = std::weak_ptr<IZegoMediaPlayerEventHandler>(mpEventHandler);
        auto weak_self = std::weak_ptr<ZegoExpressMediaPlayerImp>(shared_from_this());
        ZEGO_SWITCH_THREAD_PRE
        auto handlerInMain = weakMPEventHandler.lock();
        auto self = weak_self.lock();
        if (handlerInMain && self) {
            handlerInMain->onMediaPlayerFirstFrameEvent(this,
                                                        ZegoMediaPlayerFirstFrameEvent(event));
        }
        ZEGO_SWITCH_THREAD_ING
    }

    void zego_on_media_player_local_cache(zego_error error_code, const char *resource,
                                          const char *cached_file) {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        std::string strResource = resource ? resource : "";
        std::string strCachedFile = cached_file ? cached_file : "";
        auto weakMPEventHandler = std::weak_ptr<IZegoMediaPlayerEventHandler>(mpEventHandler);
        auto weak_self = std::weak_ptr<ZegoExpressMediaPlayerImp>(shared_from_this());
        ZEGO_SWITCH_THREAD_PRE
        auto handlerInMain = weakMPEventHandler.lock();
        auto self = weak_self.lock();
        if (handlerInMain && self) {
            handlerInMain->onMediaPlayerLocalCache(this, error_code, strResource, strCachedFile);
        }
        ZEGO_SWITCH_THREAD_ING
    }

    void zego_on_media_player_video_frame(const unsigned char **data, unsigned int *data_length,
                                          const struct zego_video_frame_param _param,
                                          const char *extra_info) {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        if (mpVideoHandler) {
            auto param = ZegoExpressConvert::I2OVideoFrameParam(_param);
            mpVideoHandler->onVideoFrame(this, data, data_length, param);
            mpVideoHandler->onVideoFrame(this, data, data_length, param, extra_info);
        }
    }

    void zego_on_media_player_take_snapshot_result(
        zego_error error_code, void *image, enum zego_media_player_instance_index instance_index) {
        ZEGO_UNUSED_VARIABLE(instance_index);
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        if (mpTakeSnapshotCallback) {
            mpTakeSnapshotCallback(error_code, image);
        }
    }

    void zego_on_media_player_audio_frame(const unsigned char *data, unsigned int data_length,
                                          const struct zego_audio_frame_param _param) {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        if (mpAudioHandler) {
            ZegoAudioFrameParam param;
            param.channel = ZegoAudioChannel(_param.channel);
            param.sampleRate = ZegoAudioSampleRate(_param.sample_rate);
            mpAudioHandler->onAudioFrame(this, data, data_length, param);
        }
    }

    void zego_on_media_player_block_begin(const char *path) {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        if (mpBlockDataHandler) {
            std::string strPath = path ? path : "";
            mpBlockDataHandler->onBlockBegin(this, path);
        }
    }

    unsigned int zego_on_media_player_block_data(unsigned char *const buffer,
                                                 unsigned int buffer_size) {
        std::lock_guard<std::mutex> lock(mediaEventMutex);
        if (mpBlockDataHandler) {
            return mpBlockDataHandler->onBlockData(this, buffer, buffer_size);
        }
        return -1;
    }

  private:
    void handleLoadResourceCallback(int errorCode, ZegoMediaPlayerLoadResourceCallback callback) {
        if (errorCode == ZEGO_ERRCODE_COMMON_SUCCESS) {
            std::lock_guard<std::mutex> lock(mediaEventMutex);
            if (callback) {
                mpLoadResourceCallbacks.push_back(callback);
            } else {
                mpLoadResourceCallbacks.push_back([](int errorCode) {});
            }
        } else {
            ZEGO_SWITCH_THREAD_PRE
            if (callback) {
                callback(errorCode);
            }
            ZEGO_SWITCH_THREAD_ING
        }
    }

  private:
    std::shared_ptr<IZegoMediaPlayerVideoHandler> mpVideoHandler;
    std::shared_ptr<IZegoMediaPlayerEventHandler> mpEventHandler;
    std::shared_ptr<IZegoMediaPlayerAudioHandler> mpAudioHandler;
    std::shared_ptr<IZegoMediaPlayerBlockDataHandler> mpBlockDataHandler;
    std::list<ZegoMediaPlayerLoadResourceCallback> mpLoadResourceCallbacks;
    ZegoMediaPlayerTakeSnapshotCallback mpTakeSnapshotCallback;
    std::unordered_map<zego_seq, ZegoMediaPlayerSeekToCallback> mpSeekToCallbacks;
    std::mutex mediaEventMutex;
    int instanceIndex = -1;
};

} // namespace EXPRESS
} // namespace ZEGO

ZEGO_ENABLE_DEPRECATION_WARNINGS
