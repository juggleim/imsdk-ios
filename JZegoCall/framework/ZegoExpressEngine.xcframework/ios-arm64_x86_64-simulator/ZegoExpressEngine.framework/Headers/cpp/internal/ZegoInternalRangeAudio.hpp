#pragma once

#include "../ZegoExpressDefines.h"
#include "../ZegoExpressEventHandler.h"
#include "../ZegoExpressInterface.h"

#include "ZegoInternalBase.h"
#include "ZegoInternalBridge.h"

ZEGO_DISABLE_DEPRECATION_WARNINGS

namespace ZEGO {
namespace EXPRESS {

class ZegoExpressRangeAudioImp : public IZegoRangeAudio {
  public:
    ZegoExpressRangeAudioImp(int index) : instanceIndex(index) {}

    void setEventHandler(std::shared_ptr<IZegoRangeAudioEventHandler> handler) override {
        std::lock_guard<std::mutex> lock(rangeAudioEventMutex);
        raEventHandler = handler;
    }

    void setRangeAudioMode(ZegoRangeAudioMode mode) override {
        oInternalOriginBridge->setRangeAudioMode((zego_range_audio_mode)mode,
                                                 (zego_range_audio_instance_index)instanceIndex);
    }

    void setRangeAudioCustomMode(ZegoRangeAudioSpeakMode speak_mode,
                                 ZegoRangeAudioListenMode listen_mode) override {
        oInternalOriginBridge->setRangeAudioCustomMode(
            (zego_range_audio_speak_mode)speak_mode, (zego_range_audio_listen_mode)listen_mode,
            (zego_range_audio_instance_index)instanceIndex);
    }

    void setTeamID(const std::string &teamID) override {
        const char *team_id = teamID.c_str();
        oInternalOriginBridge->rangeAudioSetTeamID(team_id,
                                                   (zego_range_audio_instance_index)instanceIndex);
    }

    void setAudioReceiveRange(float range) override {
        oInternalOriginBridge->rangeAudioSetAudioReceiveRange(
            range, (zego_range_audio_instance_index)instanceIndex);
    }

    int setAudioReceiveRange(ZegoReceiveRangeParam param) override {
        zego_receive_range_param _param;
        _param.min = param.min;
        _param.max = param.max;
        return oInternalOriginBridge->rangeAudioSetAudioReceiveRange(
            _param, (zego_range_audio_instance_index)instanceIndex);
    }

    void setPositionUpdateFrequency(int frequency) override {
        oInternalOriginBridge->rangeAudiosetPositionUpdateFrequency(
            frequency, (zego_range_audio_instance_index)instanceIndex);
    }

    void setRangeAudioVolume(int volume) override {
        oInternalOriginBridge->rangeAudioSetAudioVolume(
            volume, (zego_range_audio_instance_index)instanceIndex);
    }

    void setStreamVocalRange(const std::string &streamID, float vocalRange) override {
        oInternalOriginBridge->rangeAudioSetStreamVocalRange(
            streamID.c_str(), vocalRange, (zego_range_audio_instance_index)instanceIndex);
    }

    int setStreamVocalRange(const std::string &streamID, ZegoVocalRangeParam param) override {
        zego_vocal_range_param _param;
        _param.min = param.min;
        _param.max = param.max;
        return oInternalOriginBridge->rangeAudioSetStreamVocalRange(
            streamID.c_str(), _param, (zego_range_audio_instance_index)instanceIndex);
    }

    void updateStreamPosition(const std::string &streamID, float position[3]) override {
        oInternalOriginBridge->rangeAudioUpdateStreamPosition(
            streamID.c_str(), position, (zego_range_audio_instance_index)instanceIndex);
    }

    void enableMicrophone(bool enable) override {
        oInternalOriginBridge->rangeAudioEnableMicrophone(
            enable, (zego_range_audio_instance_index)instanceIndex);
    }

    void enableSpeaker(bool enable) override {
        oInternalOriginBridge->rangeAudioEnableSpeaker(
            enable, (zego_range_audio_instance_index)instanceIndex);
    }

    void enableSpatializer(bool enable) override {
        oInternalOriginBridge->rangeAudioEnableSpatializer(
            enable, (zego_range_audio_instance_index)instanceIndex);
    }

    void updateAudioSource(const std::string &userID, float position[3]) override {
        oInternalOriginBridge->rangeAudioUpdateAudioSource(
            userID.c_str(), position, (zego_range_audio_instance_index)instanceIndex);
    }

    void updateSelfPosition(float position[3], float axisForward[3], float axisRight[3],
                            float axisUp[3]) override {
        oInternalOriginBridge->rangeAudioUpdateSelfPosition(
            position, axisForward, axisRight, axisUp,
            (zego_range_audio_instance_index)instanceIndex);
    }

    void muteUser(const std::string &userID, bool mute) override {
        oInternalOriginBridge->rangeAudioMuteUser(userID, mute,
                                                  (zego_range_audio_instance_index)instanceIndex);
    }

    int getIndex() { return instanceIndex; }

    void zego_on_range_audio_microphone_state_update(enum zego_range_audio_microphone_state state,
                                                     zego_error error_code) {
        std::lock_guard<std::mutex> lock(rangeAudioEventMutex);
        if (raEventHandler) {
            auto weakRAEventHandler = std::weak_ptr<IZegoRangeAudioEventHandler>(raEventHandler);
            ZEGO_SWITCH_THREAD_PRE
            auto handlerInMain = weakRAEventHandler.lock();
            if (handlerInMain) {
                handlerInMain->onRangeAudioMicrophoneStateUpdate(
                    this, ZegoRangeAudioMicrophoneState(state), error_code);
            }
            ZEGO_SWITCH_THREAD_ING
        }
    }

  private:
    std::shared_ptr<IZegoRangeAudioEventHandler> raEventHandler;
    std::mutex rangeAudioEventMutex;
    int instanceIndex = -1;
};
} // namespace EXPRESS
} // namespace ZEGO

ZEGO_ENABLE_DEPRECATION_WARNINGS
