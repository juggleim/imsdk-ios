#pragma once

#include "../ZegoExpressDefines.h"
#include "../ZegoExpressEventHandler.h"
#include "../ZegoExpressInterface.h"

#include "ZegoInternalBase.h"
#include "ZegoInternalBridge.h"

ZEGO_DISABLE_DEPRECATION_WARNINGS

namespace ZEGO {
namespace EXPRESS {

class ZegoExpressScreenCaptureSourceImp : public IZegoScreenCaptureSource {
  public:
    ZegoExpressScreenCaptureSourceImp(int index) : instance_index_(index) {}

    void setEventHandler(std::shared_ptr<IZegoScreenCaptureSourceEventHandler> handler) override {
        std::lock_guard<std::mutex> lock(event_handler_mutex_);
        event_handler_ = handler;
    }

    void updateCaptureSource(void *sourceId, ZegoScreenCaptureSourceType sourceType) override {
        oInternalOriginBridge->screenCaptureUpdateCaptureSource(
            sourceId, (enum zego_screen_capture_source_type)sourceType, instance_index_);
    }

    void startCapture() override {
        oInternalOriginBridge->screenCaptureStartCapture(instance_index_);
    }

    void stopCapture() override {
        oInternalOriginBridge->screenCaptureStopCapture(instance_index_);
    }

    ZegoRect getCaptureSourceRect() override {
        struct zego_rect s_rect =
            oInternalOriginBridge->screenCaptureGetCaptureSourceRect(instance_index_);
        return ZegoRect(s_rect.left, s_rect.top, s_rect.right - s_rect.left,
                        s_rect.bottom - s_rect.top);
    }

    void updateCaptureRegion(ZegoRect rect) override {
        struct zego_rect s_rect = {rect.x, rect.y, rect.x + rect.width, rect.y + rect.height};
        oInternalOriginBridge->screenCaptureUpdateCaptureRegion(s_rect, instance_index_);
    }

    void updatePublishRegion(ZegoRect rect) override {
        struct zego_rect s_rect = {rect.x, rect.y, rect.x + rect.width, rect.y + rect.height};
        oInternalOriginBridge->screenCaptureUpdatePublishRegion(s_rect, instance_index_);
    }

    void setExcludeWindowList(void **list, int count) override {
        oInternalOriginBridge->screenCaptureSetExcludeWindowList(list, count, instance_index_);
    }

    void enableWindowActivate(bool active) override {
        oInternalOriginBridge->screenCaptureEnableWindowActivate(active, instance_index_);
    }

    void enableCursorVisible(bool visible) override {
        oInternalOriginBridge->screenCaptureEnableCursorVisible(visible, instance_index_);
    }

    void enableAudioCapture(bool enable, ZegoAudioFrameParam audioParam) override {
        oInternalOriginBridge->screenCaptureEnableAudioCapture(enable, audioParam, instance_index_);
    }

    int getIndex() override { return instance_index_; }

    void zego_on_screen_capture_source_available_frame(const void *data, unsigned int data_length,
                                                       const struct zego_video_frame_param param) {
        std::lock_guard<std::mutex> lock(event_handler_mutex_);
        if (event_handler_) {
            event_handler_->onAvailableFrame(this, data, data_length,
                                             ZegoExpressConvert::I2OVideoFrameParam(param));
        }
    }

    void zego_on_screen_capture_source_exception_occurred(
        enum zego_screen_capture_source_exception_type exception_type) {
        std::lock_guard<std::mutex> lock(event_handler_mutex_);
        if (event_handler_) {
            auto weakEventHandler =
                std::weak_ptr<IZegoScreenCaptureSourceEventHandler>(event_handler_);
            ZEGO_SWITCH_THREAD_PRE
            auto handlerInMain = weakEventHandler.lock();
            if (handlerInMain) {
                handlerInMain->onExceptionOccurred(
                    this, (ZegoScreenCaptureSourceExceptionType)exception_type);
            }
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_screen_capture_source_window_state_changed(
        enum zego_screen_capture_window_state window_state, zego_rect rect) {
        std::lock_guard<std::mutex> lock(event_handler_mutex_);
        if (event_handler_) {
            auto weakEventHandler =
                std::weak_ptr<IZegoScreenCaptureSourceEventHandler>(event_handler_);
            ZEGO_SWITCH_THREAD_PRE
            auto handlerInMain = weakEventHandler.lock();
            if (handlerInMain) {
                handlerInMain->onWindowStateChanged(this,
                                                    (ZegoScreenCaptureWindowState)window_state,
                                                    ZegoExpressConvert::I2ORect(rect));
            }
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_screen_capture_source_rect_changed(zego_rect rect) {
        std::lock_guard<std::mutex> lock(event_handler_mutex_);
        if (event_handler_) {
            auto weakEventHandler =
                std::weak_ptr<IZegoScreenCaptureSourceEventHandler>(event_handler_);
            ZEGO_SWITCH_THREAD_PRE
            auto handlerInMain = weakEventHandler.lock();
            if (handlerInMain) {
                handlerInMain->onRectChanged(this, ZegoExpressConvert::I2ORect(rect));
            }
            ZEGO_SWITCH_THREAD_ING
        }
    }

  private:
    std::shared_ptr<IZegoScreenCaptureSourceEventHandler> event_handler_;
    std::mutex event_handler_mutex_;
    int instance_index_ = -1;
};
} // namespace EXPRESS
} // namespace ZEGO

ZEGO_ENABLE_DEPRECATION_WARNINGS
