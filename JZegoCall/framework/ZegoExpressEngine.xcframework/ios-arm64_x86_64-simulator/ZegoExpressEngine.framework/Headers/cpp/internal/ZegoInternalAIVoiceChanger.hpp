#pragma once

#include "../ZegoExpressDefines.h"
#include "../ZegoExpressEventHandler.h"
#include "../ZegoExpressInterface.h"

#include "ZegoInternalBase.h"
#include "ZegoInternalBridge.h"

ZEGO_DISABLE_DEPRECATION_WARNINGS

namespace ZEGO {
namespace EXPRESS {

class ZegoExpressAIVoiceChangerImpl : public IZegoAIVoiceChanger {
  public:
    ZegoExpressAIVoiceChangerImpl(int index) : index_(index) {}
    ~ZegoExpressAIVoiceChangerImpl() {}

    // IZegoAIVoiceChanger
    virtual int getIndex() override { return index_; }

    virtual void
    setEventHandler(std::shared_ptr<IZegoAIVoiceChangerEventHandler> handler) override {
        std::lock_guard<std::mutex> lock(mtx_);
        event_handler_ = handler;
    }

    virtual void initEngine() override { oInternalOriginBridge->aiVoiceChangerInitEngine(index_); }

    virtual void update() override { oInternalOriginBridge->aiVoiceChangerUpdate(index_); }

    virtual void getSpeakerList() override {
        oInternalOriginBridge->aiVoiceChangerGetSpeakerList(index_);
    }

    virtual void setSpeaker(int speaker_id) override {
        oInternalOriginBridge->aiVoiceChangerSetSpeaker(index_, speaker_id);
    }

    void zego_on_ai_voice_changer_init(int error_code) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (event_handler_) {
            auto weak_event_handler =
                std::weak_ptr<IZegoAIVoiceChangerEventHandler>(event_handler_);
            ZEGO_SWITCH_THREAD_PRE
            auto handler_in_main = weak_event_handler.lock();
            if (handler_in_main) {
                handler_in_main->onInit(this, error_code);
            }
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_ai_voice_changer_update_progress(double percent, int fileIndex, int fileCount) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (event_handler_) {
            auto weak_event_handler =
                std::weak_ptr<IZegoAIVoiceChangerEventHandler>(event_handler_);
            ZEGO_SWITCH_THREAD_PRE
            auto handler_in_main = weak_event_handler.lock();
            if (handler_in_main) {
                handler_in_main->onUpdateProgress(this, percent, fileIndex, fileCount);
            }
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_ai_voice_changer_update(int error_code) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (event_handler_) {
            auto weak_event_handler =
                std::weak_ptr<IZegoAIVoiceChangerEventHandler>(event_handler_);
            ZEGO_SWITCH_THREAD_PRE
            auto handler_in_main = weak_event_handler.lock();
            if (handler_in_main) {
                handler_in_main->onUpdate(this, error_code);
            }
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_ai_voice_changer_get_speaker_list(
        int error_code, const struct zego_ai_voice_changer_speaker_info *speaker_list,
        unsigned int speaker_count) {
        std::vector<ZegoAIVoiceChangerSpeakerInfo> speaker_info_list;
        for (unsigned int i = 0; i < speaker_count; i++) {
            auto &speaker = speaker_list[i];

            ZegoAIVoiceChangerSpeakerInfo speaker_info;
            speaker_info.id = speaker.id;
            speaker_info.name = speaker.name;
            speaker_info_list.push_back(std::move(speaker_info));
        }

        std::lock_guard<std::mutex> lock(mtx_);
        if (event_handler_) {
            auto weak_event_handler =
                std::weak_ptr<IZegoAIVoiceChangerEventHandler>(event_handler_);
            ZEGO_SWITCH_THREAD_PRE
            auto handler_in_main = weak_event_handler.lock();
            if (handler_in_main) {
                handler_in_main->onGetSpeakerList(this, error_code, speaker_info_list);
            }
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_ai_voice_changer_event(ZegoAIVoiceChangerEvent event) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (event_handler_) {
            auto weak_event_handler =
                std::weak_ptr<IZegoAIVoiceChangerEventHandler>(event_handler_);
            ZEGO_SWITCH_THREAD_PRE
            auto handler_in_main = weak_event_handler.lock();
            if (handler_in_main) {
                handler_in_main->onEvent(this, event);
            }
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_ai_voice_changer_set_speaker(int error_code) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (event_handler_) {
            auto weak_event_handler =
                std::weak_ptr<IZegoAIVoiceChangerEventHandler>(event_handler_);
            ZEGO_SWITCH_THREAD_PRE
            auto handler_in_main = weak_event_handler.lock();
            if (handler_in_main) {
                handler_in_main->onSetSpeaker(this, error_code);
            }
            ZEGO_SWITCH_THREAD_ING
        }
    }

  private:
    std::mutex mtx_;
    std::shared_ptr<IZegoAIVoiceChangerEventHandler> event_handler_;
    int index_ = -1;
};

} // namespace EXPRESS
} // namespace ZEGO
