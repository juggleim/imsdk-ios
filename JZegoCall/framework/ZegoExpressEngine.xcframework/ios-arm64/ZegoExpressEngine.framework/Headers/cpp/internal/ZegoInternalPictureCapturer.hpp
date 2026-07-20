#pragma once

#include "../ZegoExpressDefines.h"
#include "../ZegoExpressEventHandler.h"
#include "../ZegoExpressInterface.h"

#include "ZegoInternalBase.h"
#include "ZegoInternalBridge.h"

ZEGO_DISABLE_DEPRECATION_WARNINGS

namespace ZEGO {
namespace EXPRESS {

class ZegoExpressPictureCapturerImpl : public IZegoPictureCapturer {
  public:
    ZegoExpressPictureCapturerImpl(int index) : index_(index) {}
    ~ZegoExpressPictureCapturerImpl() {}

    // IZegoPictureCapturer
    virtual int getIndex() override { return index_; }

    virtual void setPath(const std::string &path) override {
        oInternalOriginBridge->setPictureCapturerPath(index_, path);
    }

  private:
    int index_ = -1;
};

} // namespace EXPRESS
} // namespace ZEGO
