#pragma once

#include "../ZegoExpressDefines.h"
#include "../ZegoExpressEventHandler.h"
#include "../ZegoExpressInterface.h"

#include "ZegoInternalBase.h"
#include "ZegoInternalBridge.h"

ZEGO_DISABLE_DEPRECATION_WARNINGS

namespace ZEGO {
namespace EXPRESS {

class ZegoExpressRealTimeSequentialDataManagerImp : public IZegoRealTimeSequentialDataManager {
  public:
    ZegoExpressRealTimeSequentialDataManagerImp(int index) : instance_index_(index) {}

    void
    setEventHandler(std::shared_ptr<IZegoRealTimeSequentialDataEventHandler> handler) override {
        std::lock_guard<std::mutex> lock(event_handler_mutex_);
        event_handler_ = handler;
    }

    void startBroadcasting(const std::string &streamID) override {
        oInternalOriginBridge->realTimeSequentialDataStartBroadcasting(streamID.c_str(),
                                                                       instance_index_);
    }

    void stopBroadcasting(const std::string &streamID) override {
        oInternalOriginBridge->realTimeSequentialDataStopBroadcasting(streamID.c_str(),
                                                                      instance_index_);
    }

    void sendRealTimeSequentialData(const unsigned char *data, unsigned int dataLength,
                                    const std::string &streamID,
                                    ZegoRealTimeSequentialDataSentCallback callback) override {
        int seq = oInternalOriginBridge->realTimeSequentialDataSendData(
            data, dataLength, streamID.c_str(), instance_index_);
        if (callback != nullptr) {
            std::lock_guard<std::mutex> lock(event_handler_mutex_);
            data_sent_callbacks_.insert({seq, callback});
        }
    }

    void startSubscribing(const std::string &streamID) override {
        oInternalOriginBridge->realTimeSequentialDataStartSubscribing(streamID.c_str(),
                                                                      instance_index_);
    }

    void stopSubscribing(const std::string &streamID) override {
        oInternalOriginBridge->realTimeSequentialDataStopSubscribing(streamID.c_str(),
                                                                     instance_index_);
    }

    int getIndex() override { return instance_index_; }

    void zego_on_receive_real_time_sequential_data(const unsigned char *data,
                                                   unsigned int data_length,
                                                   const char *stream_id) {
        std::lock_guard<std::mutex> lock(event_handler_mutex_);
        if (event_handler_) {
            auto weak_event_handler =
                std::weak_ptr<IZegoRealTimeSequentialDataEventHandler>(event_handler_);

            std::string streamID = stream_id;
            std::string buffer((const char *)data, data_length);

            auto handler_in_main = weak_event_handler.lock();
            if (handler_in_main) {
                handler_in_main->onReceiveRealTimeSequentialData(
                    this, (const unsigned char *)buffer.data(), ZegoUInt(buffer.size()), streamID);
            }
        }
    }

    void zego_on_real_time_sequential_data_sent(zego_error error_code, zego_seq seq) {
        std::lock_guard<std::mutex> lock(event_handler_mutex_);
        if (data_sent_callbacks_.count(seq) > 0) {
            auto cb = data_sent_callbacks_[seq];
            data_sent_callbacks_.erase(seq);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

  private:
    std::shared_ptr<IZegoRealTimeSequentialDataEventHandler> event_handler_;
    std::unordered_map<zego_seq, ZegoRealTimeSequentialDataSentCallback> data_sent_callbacks_;
    std::mutex event_handler_mutex_;
    int instance_index_ = -1;
};
} // namespace EXPRESS
} // namespace ZEGO

ZEGO_ENABLE_DEPRECATION_WARNINGS