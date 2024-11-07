#pragma once

#include "../ZegoExpressDefines.h"
#include "../ZegoExpressEventHandler.h"
#include "../ZegoExpressInterface.h"

#include "ZegoInternalBase.h"
#include "ZegoInternalBridge.h"

ZEGO_DISABLE_DEPRECATION_WARNINGS

namespace ZEGO {
namespace EXPRESS {

class ZegoExpressAudioEffectPlayerImp : public IZegoAudioEffectPlayer {
  public:
    ZegoExpressAudioEffectPlayerImp(int index) : instanceIndex(index) {}

    void setEventHandler(std::shared_ptr<IZegoAudioEffectPlayerEventHandler> handler) override {
        std::lock_guard<std::mutex> lock(audioEffectEventMutex);
        aepEventHandler = handler;
    }

    void loadResource(unsigned int audioEffectID, const std::string &path,
                      ZegoAudioEffectPlayerLoadResourceCallback callback) override {
        int seq = oInternalOriginBridge->audioEffectPlayerLoadResource(
            audioEffectID, path.c_str(), zego_audio_effect_player_instance_index(instanceIndex));
        if (callback != nullptr) {
            std::lock_guard<std::mutex> lock(audioEffectEventMutex);
            aepLoadResourceCallbacks.insert({seq, callback});
        }
    }

    void unloadResource(unsigned int audioEffectID) override {
        oInternalOriginBridge->audioEffectPlayerUnloadResource(
            audioEffectID, zego_audio_effect_player_instance_index(instanceIndex));
    }

    void start(unsigned int audioEffectID, const std::string &path,
               ZegoAudioEffectPlayConfig *config = nullptr) override {
        const char *_path = path.empty() ? nullptr : path.c_str();
        std::shared_ptr<zego_audio_effect_play_config> _config;
        if (config) {
            _config = std::make_shared<zego_audio_effect_play_config>();
            _config->is_publish_out = config->isPublishOut;
            _config->play_count = config->playCount;
        }
        oInternalOriginBridge->audioEffectPlayerStart(
            audioEffectID, _path, _config.get(),
            zego_audio_effect_player_instance_index(instanceIndex));
    }

    void stop(unsigned int audioEffectID) override {
        oInternalOriginBridge->audioEffectPlayerStop(
            audioEffectID, zego_audio_effect_player_instance_index(instanceIndex));
    }

    void pause(unsigned int audioEffectID) override {
        oInternalOriginBridge->audioEffectPlayerPause(
            audioEffectID, zego_audio_effect_player_instance_index(instanceIndex));
    }

    void resume(unsigned int audioEffectID) override {
        oInternalOriginBridge->audioEffectPlayerResume(
            audioEffectID, zego_audio_effect_player_instance_index(instanceIndex));
    }

    void stopAll() override {
        oInternalOriginBridge->audioEffectPlayerStopAll(
            zego_audio_effect_player_instance_index(instanceIndex));
    }

    void pauseAll() override {
        oInternalOriginBridge->audioEffectPlayerPauseAll(
            zego_audio_effect_player_instance_index(instanceIndex));
    }

    void resumeAll() override {
        oInternalOriginBridge->audioEffectPlayerResumeAll(
            zego_audio_effect_player_instance_index(instanceIndex));
    }

    void seekTo(unsigned int audioEffectID, unsigned long long millisecond,
                ZegoAudioEffectPlayerSeekToCallback callback) override {
        int seq = oInternalOriginBridge->audioEffectPlayerSeekTo(
            audioEffectID, millisecond, zego_audio_effect_player_instance_index(instanceIndex));
        if (callback != nullptr) {
            std::lock_guard<std::mutex> lock(audioEffectEventMutex);
            aepSeekToCallbacks.insert({seq, callback});
        }
    }

    void setVolume(unsigned int audioEffectID, int volume) override {
        oInternalOriginBridge->audioEffectPlayerSetVolume(
            audioEffectID, volume, zego_audio_effect_player_instance_index(instanceIndex));
    }

    void setPlayVolume(unsigned int audioEffectID, int volume) override {
        oInternalOriginBridge->audioEffectPlayerSetPlayVolume(
            audioEffectID, volume, zego_audio_effect_player_instance_index(instanceIndex));
    }

    void setPublishVolume(unsigned int audioEffectID, int volume) override {
        oInternalOriginBridge->audioEffectPlayerSetPublishVolume(
            audioEffectID, volume, zego_audio_effect_player_instance_index(instanceIndex));
    }

    void setVolumeAll(int volume) override {
        oInternalOriginBridge->audioEffectPlayerSetVolumeAll(
            volume, zego_audio_effect_player_instance_index(instanceIndex));
    }

    void setPlayVolumeAll(int volume) override {
        oInternalOriginBridge->audioEffectPlayerSetPlayVolumeAll(
            volume, zego_audio_effect_player_instance_index(instanceIndex));
    }

    void setPublishVolumeAll(int volume) override {
        oInternalOriginBridge->audioEffectPlayerSetPublishVolumeAll(
            volume, zego_audio_effect_player_instance_index(instanceIndex));
    }

    void setPlaySpeed(unsigned int audioEffectID, float speed) override {
        oInternalOriginBridge->audioEffectPlayerSetPlaySpeed(
            audioEffectID, speed, zego_audio_effect_player_instance_index(instanceIndex));
    }

    unsigned long long getTotalDuration(unsigned int audioEffectID) override {
        return oInternalOriginBridge->audioEffectPlayerGetTotalDuration(
            audioEffectID, zego_audio_effect_player_instance_index(instanceIndex));
    }

    unsigned long long getCurrentProgress(unsigned int audioEffectID) override {
        return oInternalOriginBridge->audioEffectPlayerGetCurrentProgress(
            audioEffectID, zego_audio_effect_player_instance_index(instanceIndex));
    }

    void updatePosition(unsigned int audioEffectID, const float position[3]) override {
        oInternalOriginBridge->audioEffectPlayerUpdatePosition(
            audioEffectID, position, zego_audio_effect_player_instance_index(instanceIndex));
    }

    int getIndex() override { return instanceIndex; }

    void zego_on_audio_effect_play_state_update(unsigned int audio_effect_id,
                                                enum zego_audio_effect_play_state state,
                                                zego_error error_code) {
        std::lock_guard<std::mutex> lock(audioEffectEventMutex);
        if (aepEventHandler) {
            auto weakAEPEventHandler =
                std::weak_ptr<IZegoAudioEffectPlayerEventHandler>(aepEventHandler);
            ZEGO_SWITCH_THREAD_PRE
            auto handlerInMain = weakAEPEventHandler.lock();
            if (handlerInMain) {
                handlerInMain->onAudioEffectPlayStateUpdate(
                    this, audio_effect_id, ZegoAudioEffectPlayState(state), error_code);
            }
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_audio_effect_player_load_resource(zego_seq seq, zego_error error_code) {
        std::lock_guard<std::mutex> lock(audioEffectEventMutex);
        if (aepLoadResourceCallbacks.count(seq) > 0) {
            auto cb = aepLoadResourceCallbacks[seq];
            aepLoadResourceCallbacks.erase(seq);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_audio_effect_player_seek_to(zego_seq seq, zego_error error_code) {
        std::lock_guard<std::mutex> lock(audioEffectEventMutex);
        if (aepSeekToCallbacks.count(seq) > 0) {
            auto cb = aepSeekToCallbacks[seq];
            aepSeekToCallbacks.erase(seq);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

  private:
    std::shared_ptr<IZegoAudioEffectPlayerEventHandler> aepEventHandler;
    std::unordered_map<zego_seq, ZegoAudioEffectPlayerLoadResourceCallback>
        aepLoadResourceCallbacks;
    std::unordered_map<zego_seq, ZegoAudioEffectPlayerSeekToCallback> aepSeekToCallbacks;
    std::mutex audioEffectEventMutex;
    int instanceIndex = -1;
};
} // namespace EXPRESS
} // namespace ZEGO

ZEGO_ENABLE_DEPRECATION_WARNINGS
