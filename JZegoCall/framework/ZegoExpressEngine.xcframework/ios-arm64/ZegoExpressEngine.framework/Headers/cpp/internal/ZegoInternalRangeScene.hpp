#pragma once

#include "../ZegoExpressDefines.h"
#include "../ZegoExpressEventHandler.h"
#include "../ZegoExpressInterface.h"

#include "ZegoInternalBase.h"
#include "ZegoInternalBridge.h"

ZEGO_DISABLE_DEPRECATION_WARNINGS

namespace ZEGO {
namespace EXPRESS {

class ZegoExpressRangeSceneStreamImp : public IZegoRangeSceneStream {
  public:
    ZegoExpressRangeSceneStreamImp(int handle) : rangeSceneHandle(handle) {}

    bool setEventHandler(std::shared_ptr<IZegoRangeSceneStreamEventHandler> handler) override {
        std::lock_guard<std::mutex> lock(rangeSceneStreamMutex);
        streamEventHandler = handler;
        return true;
    }

    int setReceiveRange(float range) override {
        return oInternalOriginBridge->setReceiveRange(rangeSceneHandle, range);
    }

    int setReceiveRange(ZegoReceiveRangeParam param) override {
        zego_receive_range_param _param;
        _param.min = param.min;
        _param.max = param.max;
        return oInternalOriginBridge->setReceiveRange(rangeSceneHandle, _param);
    }

    int enableRangeSpatializer(bool enable) override {
        return oInternalOriginBridge->enableRangeSpatializer(rangeSceneHandle, enable);
    }

    int mutePlayAudio(const std::string &userID, bool mute) override {
        return oInternalOriginBridge->mutePlayAudio(rangeSceneHandle, userID.c_str(), mute);
    }

    int mutePlayVideo(const std::string &userID, bool mute) override {
        return oInternalOriginBridge->mutePlayVideo(rangeSceneHandle, userID.c_str(), mute);
    }

    void zego_on_range_scene_stream_user_stream_state_update(IZegoRangeScene *rangeScene,
                                                             const char *userid,
                                                             const char *stream_id,
                                                             enum zego_stream_state state) {
        std::string userID = userid;
        std::string streamID = stream_id;
        ZegoStreamState _state = static_cast<ZegoStreamState>(state);
        std::lock_guard<std::mutex> lock(rangeSceneStreamMutex);
        if (streamEventHandler) {
            streamEventHandler->onUserStreamStateUpdate(rangeScene, userID, streamID, _state);
        }
    }

    void zego_on_range_scene_stream_user_mic_update(IZegoRangeScene *rangeScene, const char *userid,
                                                    enum zego_device_state state) {
        std::string userID = userid;
        ZegoDeviceState _state = static_cast<ZegoDeviceState>(state);
        std::lock_guard<std::mutex> lock(rangeSceneStreamMutex);
        if (streamEventHandler) {
            streamEventHandler->onUserMicUpdate(rangeScene, userID, _state);
        }
    }

    void zego_on_range_scene_stream_user_camera_update(IZegoRangeScene *rangeScene,
                                                       const char *userid,
                                                       enum zego_device_state state) {
        std::string userID = userid;
        ZegoDeviceState _state = static_cast<ZegoDeviceState>(state);
        std::lock_guard<std::mutex> lock(rangeSceneStreamMutex);
        if (streamEventHandler) {
            streamEventHandler->onUserCameraUpdate(rangeScene, userID, _state);
        }
    }

    void zego_on_range_scene_stream_user_speaker_update(IZegoRangeScene *rangeScene,
                                                        const char *userid,
                                                        enum zego_device_state state) {
        std::string userID = userid;
        ZegoDeviceState _state = static_cast<ZegoDeviceState>(state);
        std::lock_guard<std::mutex> lock(rangeSceneStreamMutex);
        if (streamEventHandler) {
            streamEventHandler->onUserSpeakerUpdate(rangeScene, userID, _state);
        }
    }

  private:
    int rangeSceneHandle = -1;
    std::shared_ptr<IZegoRangeSceneStreamEventHandler> streamEventHandler;
    std::mutex rangeSceneStreamMutex;
};

class ZegoExpressRangeSceneTeamImp : public IZegoRangeSceneTeam {
  public:
    ZegoExpressRangeSceneTeamImp(int handle) : rangeSceneHandle(handle) {}

    bool setEventHandler(std::shared_ptr<IZegoRangeSceneTeamEventHandler> handler) override {
        std::lock_guard<std::mutex> lock(rangeSceneTeamMutex);
        teamEventHandler = handler;
        return true;
    }

    void joinTeam(ZegoTeamParam param, ZegoRangeSceneJoinTeamCallback callback) override {
        zego_team_param _param;
        memset(&_param, 0, sizeof(zego_team_param));
        _param.team_id = param.teamID;

        int seq = 0;
        int error = oInternalOriginBridge->joinTeam(rangeSceneHandle, &seq, _param);

        if (callback != nullptr) {
            if (error == ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS) {
                std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
                joinTeamCallbacks.insert({seq, callback});
            } else {
                ZEGO_SWITCH_THREAD_PRE
                callback(error, _param.team_id);
                ZEGO_SWITCH_THREAD_ING
            }
        }
    }

    void leaveTeam(unsigned int teamID, ZegoRangeSceneLeaveTeamCallback callback) override {
        int seq = 0;
        int error = oInternalOriginBridge->leaveTeam(rangeSceneHandle, &seq, teamID);

        if (callback != nullptr) {
            if (error == ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS) {
                std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
                leaveTeamCallbacks.insert({seq, callback});
            } else {
                ZEGO_SWITCH_THREAD_PRE
                callback(error, teamID);
                ZEGO_SWITCH_THREAD_ING
            }
        }
    }

    void zego_on_range_scene_team_team_state_update(IZegoRangeScene *rangeScene,
                                                    unsigned int team_id,
                                                    enum zego_team_state state,
                                                    zego_error error_code) {
        ZegoTeamState _state = static_cast<ZegoTeamState>(state);
        std::lock_guard<std::mutex> lock(rangeSceneTeamMutex);
        if (teamEventHandler) {
            teamEventHandler->onTeamStateUpdate(rangeScene, team_id, _state, error_code);
        }
    }

    void zego_on_range_scene_team_team_member_update(IZegoRangeScene *rangeScene,
                                                     unsigned int team_id,
                                                     enum zego_update_type update_type,
                                                     const struct zego_user *user_list,
                                                     unsigned int user_count) {
        ZegoUpdateType updateType = static_cast<ZegoUpdateType>(update_type);
        std::vector<ZegoUser> userList;
        for (unsigned int i = 0; i < user_count; ++i) {
            ZegoUser user = ZegoExpressConvert::I2OUser(user_list[i]);
            userList.push_back(user);
        }
        std::lock_guard<std::mutex> lock(rangeSceneTeamMutex);
        if (teamEventHandler) {
            teamEventHandler->onTeamMemberUpdate(rangeScene, team_id, updateType, userList);
        }
    }

    void zego_on_range_scene_team_join_team(zego_seq seq, int error_code, unsigned int team_id) {
        std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
        if (joinTeamCallbacks.count(seq) > 0) {
            auto cb = joinTeamCallbacks[seq];
            joinTeamCallbacks.erase(seq);
            if (cb) {
                cb(error_code, team_id);
            }
        }
    }

    void zego_on_range_scene_team_leave_team(zego_seq seq, int error_code, unsigned int team_id) {
        std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
        if (leaveTeamCallbacks.count(seq) > 0) {
            auto cb = leaveTeamCallbacks[seq];
            leaveTeamCallbacks.erase(seq);
            if (cb) {
                cb(error_code, team_id);
            }
        }
    }

  private:
    int rangeSceneHandle = -1;
    std::shared_ptr<IZegoRangeSceneTeamEventHandler> teamEventHandler;
    std::unordered_map<zego_seq, ZegoRangeSceneJoinTeamCallback> joinTeamCallbacks;
    std::unordered_map<zego_seq, ZegoRangeSceneLeaveTeamCallback> leaveTeamCallbacks;
    std::mutex rangeSceneTeamMutex;
    std::mutex lowFrequencyClosureMutex;
};

class ZegoExpressRangeSceneItemImp : public IZegoRangeSceneItem {
  public:
    ZegoExpressRangeSceneItemImp(int handle) : rangeSceneHandle(handle) {}

    bool setEventHandler(std::shared_ptr<IZegoRangeSceneItemEventHandler> handler) override {
        std::lock_guard<std::mutex> lock(rangeSceneItemMutex);
        itemEventHandler = handler;
        return true;
    }

    void createItem(ZegoItemParam param, ZegoRangeSceneCreateItemCallback callback) override {
        zego_item_param _param = ZegoExpressConvert::O2IZegoItemParam(param);

        int seq = 0;
        int error = oInternalOriginBridge->createItem(rangeSceneHandle, &seq, _param);

        if (callback != nullptr) {
            if (error == ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS) {
                std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
                createItemCallbacks.insert({seq, callback});
            } else {
                ZEGO_SWITCH_THREAD_PRE
                callback(error, param.itemID);
                ZEGO_SWITCH_THREAD_ING
            }
        }
    }

    void destroyItem(long long itemID, ZegoRangeSceneDestroyItemCallback callback) override {
        int seq = 0;
        int error = oInternalOriginBridge->destroyItem(rangeSceneHandle, &seq, itemID);

        if (callback != nullptr) {
            if (error == ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS) {
                std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
                destroyItemCallbacks.insert({seq, callback});
            } else {
                ZEGO_SWITCH_THREAD_PRE
                callback(error, itemID);
                ZEGO_SWITCH_THREAD_ING
            }
        }
    }

    void bindItem(long long itemID, ZegoRangeSceneBindItemCallback callback) override {
        int seq = 0;
        int error = oInternalOriginBridge->bindItem(rangeSceneHandle, &seq, itemID);

        if (callback != nullptr) {
            if (error == ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS) {
                std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
                bindItemCallbacks.insert({seq, callback});
            } else {
                ZEGO_SWITCH_THREAD_PRE
                callback(error, itemID);
                ZEGO_SWITCH_THREAD_ING
            }
        }
    }

    void unbindItem(long long itemID, ZegoRangeSceneUnbindItemCallback callback) override {
        int seq = 0;
        int error = oInternalOriginBridge->unbindItem(rangeSceneHandle, &seq, itemID);

        if (callback != nullptr) {
            if (error == ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS) {
                std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
                unbindItemCallbacks.insert({seq, callback});
            } else {
                ZEGO_SWITCH_THREAD_PRE
                callback(error, itemID);
                ZEGO_SWITCH_THREAD_ING
            }
        }
    }

    void updateItemStatus(long long itemID, ZegoPosition position, unsigned int channel,
                          const unsigned char *status, unsigned int statusLength,
                          ZegoRangeSceneUpdateItemStatusCallback callback) override {
        zego_position _position = ZegoExpressConvert::O2IZegoPosition(position);

        int seq = 0;
        int error = oInternalOriginBridge->updateItemStatus(
            rangeSceneHandle, &seq, itemID, _position, channel, status, statusLength);

        if (callback != nullptr) {
            if (error == ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS) {
                std::lock_guard<std::mutex> lock(highFrequencyClosureMutex);
                updateItemStatusCallbacks.insert({seq, callback});
            } else {
                ZEGO_SWITCH_THREAD_PRE
                callback(error, itemID);
                ZEGO_SWITCH_THREAD_ING
            }
        }
    }

    void updateItemCommand(long long itemID, ZegoPosition position, unsigned int channel,
                           const unsigned char *command, unsigned int commandLength,
                           ZegoRangeSceneUpdateItemCommandCallback callback) override {
        zego_position _position = ZegoExpressConvert::O2IZegoPosition(position);

        int seq = 0;
        int error = oInternalOriginBridge->updateItemCommand(
            rangeSceneHandle, &seq, itemID, _position, channel, command, commandLength);

        if (callback != nullptr) {
            if (error == ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS) {
                std::lock_guard<std::mutex> lock(highFrequencyClosureMutex);
                updateItemCommandCallbacks.insert({seq, callback});
            } else {
                ZEGO_SWITCH_THREAD_PRE
                callback(error, itemID);
                ZEGO_SWITCH_THREAD_ING
            }
        }
    }

    void zego_on_range_scene_item_item_enter_view(IZegoRangeScene *rangeScene, long long item_id,
                                                  unsigned int capacity,
                                                  struct zego_position position,
                                                  const char **user_list,
                                                  unsigned int user_list_size) {
        std::vector<std::string> userList;
        for (unsigned int i = 0; i < user_list_size; i++) {
            std::string userID = user_list[i];
            userList.push_back(userID);
        }
        ZegoPosition _position = ZegoExpressConvert::I2OZegoPosition(position);

        std::lock_guard<std::mutex> lock(rangeSceneItemMutex);
        if (itemEventHandler) {
            itemEventHandler->onItemEnterView(rangeScene, item_id, capacity, _position, userList);
        }
    }

    void zego_on_range_scene_item_item_leave_view(IZegoRangeScene *rangeScene, long long item_id) {
        std::lock_guard<std::mutex> lock(rangeSceneItemMutex);
        if (itemEventHandler) {
            itemEventHandler->onItemLeaveView(rangeScene, item_id);
        }
    }

    void zego_on_range_scene_item_item_bind_update(IZegoRangeScene *rangeScene, long long item_id,
                                                   const char *userid) {
        std::string userID = userid ? userid : "";

        std::lock_guard<std::mutex> lock(rangeSceneItemMutex);
        if (itemEventHandler) {
            itemEventHandler->onItemBindUpdate(rangeScene, item_id, userID);
        }
    }

    void zego_on_range_scene_item_item_unbind_update(IZegoRangeScene *rangeScene, long long item_id,
                                                     const char **user_list,
                                                     unsigned int user_list_size) {
        std::vector<std::string> userList;
        for (unsigned int i = 0; i < user_list_size; i++) {
            std::string userID = user_list[i];
            userList.push_back(userID);
        }

        std::lock_guard<std::mutex> lock(rangeSceneItemMutex);
        if (itemEventHandler) {
            itemEventHandler->onItemUnbindUpdate(rangeScene, item_id, userList);
        }
    }

    void zego_on_range_scene_item_item_status_update(IZegoRangeScene *rangeScene, long long item_id,
                                                     struct zego_position position,
                                                     unsigned int channel,
                                                     const unsigned char *status,
                                                     unsigned int status_length) {
        ZegoPosition _position = ZegoExpressConvert::I2OZegoPosition(position);

        std::lock_guard<std::mutex> lock(rangeSceneItemMutex);
        if (itemEventHandler) {
            itemEventHandler->onItemStatusUpdate(rangeScene, item_id, _position, channel, status,
                                                 status_length);
        }
    }

    void zego_on_range_scene_item_item_command_update(
        IZegoRangeScene *rangeScene, long long item_id, struct zego_position position,
        unsigned int channel, const unsigned char *command, unsigned int command_length) {
        ZegoPosition _position = ZegoExpressConvert::I2OZegoPosition(position);

        std::lock_guard<std::mutex> lock(rangeSceneItemMutex);
        if (itemEventHandler) {
            itemEventHandler->onItemCommandUpdate(rangeScene, item_id, _position, channel, command,
                                                  command_length);
        }
    }

    void zego_on_range_scene_item_create_item(zego_seq seq, int error_code, long long item_id) {
        std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
        if (createItemCallbacks.count(seq) > 0) {
            auto cb = createItemCallbacks[seq];
            createItemCallbacks.erase(seq);
            if (cb) {
                cb(error_code, item_id);
            }
        }
    }

    void zego_on_range_scene_item_destroy_item(zego_seq seq, int error_code, long long item_id) {
        std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
        if (destroyItemCallbacks.count(seq) > 0) {
            auto cb = destroyItemCallbacks[seq];
            destroyItemCallbacks.erase(seq);
            if (cb) {
                cb(error_code, item_id);
            }
        }
    }

    void zego_on_range_scene_item_bind_item(zego_seq seq, int error_code, long long item_id) {
        std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
        if (bindItemCallbacks.count(seq) > 0) {
            auto cb = bindItemCallbacks[seq];
            bindItemCallbacks.erase(seq);
            if (cb) {
                cb(error_code, item_id);
            }
        }
    }

    void zego_on_range_scene_item_unbind_item(zego_seq seq, int error_code, long long item_id) {
        std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
        if (unbindItemCallbacks.count(seq) > 0) {
            auto cb = unbindItemCallbacks[seq];
            unbindItemCallbacks.erase(seq);
            if (cb) {
                cb(error_code, item_id);
            }
        }
    }

    void zego_on_range_scene_item_update_item_status(zego_seq seq, int error_code,
                                                     long long item_id) {
        std::lock_guard<std::mutex> lock(highFrequencyClosureMutex);
        if (updateItemStatusCallbacks.count(seq) > 0) {
            auto cb = updateItemStatusCallbacks[seq];
            updateItemStatusCallbacks.erase(seq);
            if (cb) {
                cb(error_code, item_id);
            }
        }
    }

    void zego_on_range_scene_item_update_item_command(zego_seq seq, int error_code,
                                                      long long item_id) {
        std::lock_guard<std::mutex> lock(highFrequencyClosureMutex);
        if (updateItemCommandCallbacks.count(seq) > 0) {
            auto cb = updateItemCommandCallbacks[seq];
            updateItemCommandCallbacks.erase(seq);
            if (cb) {
                cb(error_code, item_id);
            }
        }
    }

  private:
    int rangeSceneHandle = -1;
    std::shared_ptr<IZegoRangeSceneItemEventHandler> itemEventHandler;
    std::unordered_map<zego_seq, ZegoRangeSceneCreateItemCallback> createItemCallbacks;
    std::unordered_map<zego_seq, ZegoRangeSceneDestroyItemCallback> destroyItemCallbacks;
    std::unordered_map<zego_seq, ZegoRangeSceneBindItemCallback> bindItemCallbacks;
    std::unordered_map<zego_seq, ZegoRangeSceneUnbindItemCallback> unbindItemCallbacks;
    std::unordered_map<zego_seq, ZegoRangeSceneUpdateItemStatusCallback> updateItemStatusCallbacks;
    std::unordered_map<zego_seq, ZegoRangeSceneUpdateItemCommandCallback>
        updateItemCommandCallbacks;
    std::mutex rangeSceneItemMutex;
    std::mutex lowFrequencyClosureMutex;
    std::mutex highFrequencyClosureMutex;
};

class ZegoExpressRangeSceneImp : public IZegoRangeScene {
  public:
    ZegoExpressRangeSceneImp(int handle) : rangeSceneHandle(handle) {}

    ~ZegoExpressRangeSceneImp() {
        if (rangeSceneStream != nullptr) {
            rangeSceneStream = nullptr;
        }
        if (rangeSceneTeam != nullptr) {
            rangeSceneTeam = nullptr;
        }
    }

    IZegoRangeSceneStream *getRangeSceneStream() override {
        if (rangeSceneStream == nullptr) {
            rangeSceneStream = std::make_shared<ZegoExpressRangeSceneStreamImp>(rangeSceneHandle);
        }
        return rangeSceneStream.get();
    }

    IZegoRangeSceneTeam *getRangeSceneTeam() override {
        if (rangeSceneTeam == nullptr) {
            rangeSceneTeam = std::make_shared<ZegoExpressRangeSceneTeamImp>(rangeSceneHandle);
        }
        return rangeSceneTeam.get();
    }

    IZegoRangeSceneItem *getRangeSceneItem() override {
        if (rangeSceneItem == nullptr) {
            rangeSceneItem = std::make_shared<ZegoExpressRangeSceneItemImp>(rangeSceneHandle);
        }
        return rangeSceneItem.get();
    }

    int getRangeSceneHandle() override { return rangeSceneHandle; }

    bool setEventHandler(std::shared_ptr<IZegoRangeSceneEventHandler> handler) override {
        std::lock_guard<std::mutex> lock(rangeSceneMutex);
        eventHandler = handler;
        return true;
    }

    void loginScene(ZegoSceneParam param, ZegoRangeSceneLoginSceneCallback callback) override {
        zego_scene_param _param = ZegoExpressConvert::O2IZegoSceneParam(param);

        int seq = 0;
        int error = oInternalOriginBridge->loginScene(rangeSceneHandle, &seq, _param);

        if (callback != nullptr) {
            if (error == ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS) {
                std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
                loginSceneCallbacks.insert({seq, callback});
            } else {
                ZEGO_SWITCH_THREAD_PRE
                ZegoSceneConfig config;
                callback(error, config);
                ZEGO_SWITCH_THREAD_ING
            }
        }
    }

    void logoutScene(ZegoRangeSceneLogoutSceneCallback callback) override {
        int seq = 0;
        int error = oInternalOriginBridge->logoutScene(rangeSceneHandle, &seq);

        if (callback != nullptr) {
            if (error == ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS) {
                std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
                logoutSceneCallbacks.insert({seq, callback});
            } else {
                ZEGO_SWITCH_THREAD_PRE
                callback(error);
                ZEGO_SWITCH_THREAD_ING
            }
        }
    }

    int updateUserStatus(ZegoPosition position, unsigned int channel, const unsigned char *status,
                         unsigned int statusLength) override {
        zego_position _position = ZegoExpressConvert::O2IZegoPosition(position);

        return oInternalOriginBridge->updateUserStatus(rangeSceneHandle, _position, channel, status,
                                                       statusLength);
    }

    int updateUserCommand(ZegoPosition position, unsigned int channel, const unsigned char *command,
                          unsigned int commandLength) override {
        zego_position _position = ZegoExpressConvert::O2IZegoPosition(position);

        return oInternalOriginBridge->updateUserCommand(rangeSceneHandle, _position, channel,
                                                        command, commandLength);
    }

    int updateUserPosition(ZegoPosition position) override {
        zego_position _position = ZegoExpressConvert::O2IZegoPosition(position);

        return oInternalOriginBridge->updateUserPosition(rangeSceneHandle, _position);
    }

    void getUserCount(ZegoRangeSceneGetUserCountCallback callback) override {
        int seq = 0;
        int error = oInternalOriginBridge->getUserCount(rangeSceneHandle, &seq);

        if (callback != nullptr) {
            if (error == ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS) {
                std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
                getUserCountCallbacks.insert({seq, callback});
            } else {
                ZEGO_SWITCH_THREAD_PRE
                callback(error, 0);
                ZEGO_SWITCH_THREAD_ING
            }
        }
    }

    void getUserListInView(ZegoRangeSceneGetUserListInViewCallback callback) override {
        int seq = 0;
        int error = oInternalOriginBridge->getUserListInView(rangeSceneHandle, &seq);

        if (callback != nullptr) {
            if (error == ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS) {
                std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
                getUserListInViewCallbacks.insert({seq, callback});
            } else {
                ZEGO_SWITCH_THREAD_PRE
                std::vector<std::string> userList;
                callback(error, userList);
                ZEGO_SWITCH_THREAD_ING
            }
        }
    }

    void sendCustomCommand(const unsigned char *command, unsigned int commandLength,
                           ZegoRangeSceneSendCustomCommandCallback callback) override {
        int seq = 0;
        int error = oInternalOriginBridge->sendCustomCommand(rangeSceneHandle, &seq, command,
                                                             commandLength);

        if (callback != nullptr) {
            if (error == ZegoErrorCode::ZEGO_ERROR_CODE_COMMON_SUCCESS) {
                std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
                sendCustomCommandCallbacks.insert({seq, callback});
            } else {
                ZEGO_SWITCH_THREAD_PRE
                callback(error);
                ZEGO_SWITCH_THREAD_ING
            }
        }
    }

    int renewToken(const std::string &token) override {
        return oInternalOriginBridge->renewToken(rangeSceneHandle, token.c_str());
    }

    int setStreamConfig(ZegoSceneStreamConfig config) override {
        zego_scene_stream_config _config = ZegoExpressConvert::O2IZegoSceneStreamConfig(config);
        return oInternalOriginBridge->setStreamConfig(rangeSceneHandle, _config);
    }

    void zego_on_range_scene_scene_state_update(enum zego_scene_state state, int error_code) {
        ZegoSceneState _state = static_cast<ZegoSceneState>(state);
        int errorCode = error_code;
        std::lock_guard<std::mutex> lock(rangeSceneMutex);
        if (eventHandler) {
            eventHandler->onSceneStateUpdate(this, _state, errorCode);
        }
    }

    void zego_on_range_scene_enter_view(struct zego_user user, struct zego_position position) {
        ZegoUser _user = ZegoExpressConvert::I2OUser(user);
        ZegoPosition _position = ZegoExpressConvert::I2OZegoPosition(position);
        std::lock_guard<std::mutex> lock(rangeSceneMutex);
        if (eventHandler) {
            eventHandler->onEnterView(this, _user, _position);
        }
    }

    void zego_on_range_scene_leave_view(const char *userid) {
        std::string userID = userid;
        std::lock_guard<std::mutex> lock(rangeSceneMutex);
        if (eventHandler) {
            eventHandler->onLeaveView(this, userID);
        }
    }

    void zego_on_range_scene_user_status_update(const char *userid, struct zego_position position,
                                                unsigned int channel, const unsigned char *status,
                                                unsigned int status_length) {
        std::string userID = userid;
        ZegoPosition _position = ZegoExpressConvert::I2OZegoPosition(position);
        std::lock_guard<std::mutex> lock(rangeSceneMutex);
        if (eventHandler) {
            eventHandler->onUserStatusUpdate(this, userID, _position, channel, status,
                                             status_length);
        }
    }

    void zego_on_range_scene_user_command_update(const char *userid, struct zego_position position,
                                                 unsigned int channel, const unsigned char *command,
                                                 unsigned int command_length) {
        std::string userID = userid;
        ZegoPosition _position = ZegoExpressConvert::I2OZegoPosition(position);
        std::lock_guard<std::mutex> lock(rangeSceneMutex);
        if (eventHandler) {
            eventHandler->onUserCommandUpdate(this, userID, _position, channel, command,
                                              command_length);
        }
    }

    void zego_on_range_scene_custom_command_update(const unsigned char *command,
                                                   unsigned int command_length) {
        std::lock_guard<std::mutex> lock(rangeSceneMutex);
        if (eventHandler) {
            eventHandler->onCustomCommandUpdate(this, command, command_length);
        }
    }

    void zego_on_range_scene_token_will_expire(int remain_time_in_second) {
        std::lock_guard<std::mutex> lock(rangeSceneMutex);
        if (eventHandler) {
            eventHandler->onSceneTokenWillExpire(this, remain_time_in_second);
        }
    }

    void zego_on_range_scene_login_scene(zego_seq seq, int error_code,
                                         struct zego_scene_config config) {
        ZegoSceneConfig _config = ZegoExpressConvert::I2OZegoSceneConfig(config);
        std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
        if (loginSceneCallbacks.count(seq) > 0) {
            auto cb = loginSceneCallbacks[seq];
            loginSceneCallbacks.erase(seq);
            if (cb) {
                cb(error_code, _config);
            }
        }
    }

    void zego_on_range_scene_logout_scene(zego_seq seq, int error_code) {
        std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
        if (logoutSceneCallbacks.count(seq) > 0) {
            auto cb = logoutSceneCallbacks[seq];
            logoutSceneCallbacks.erase(seq);
            if (cb) {
                cb(error_code);
            }
        }
    }

    void zego_on_range_scene_get_user_count(zego_seq seq, int error_code, unsigned int count) {
        std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
        if (getUserCountCallbacks.count(seq) > 0) {
            auto cb = getUserCountCallbacks[seq];
            getUserCountCallbacks.erase(seq);
            if (cb) {
                cb(error_code, count);
            }
        }
    }

    void zego_on_range_scene_get_user_list_in_view(zego_seq seq, int error_code,
                                                   const char **user_list,
                                                   unsigned int user_list_size) {
        std::vector<std::string> userList;
        for (unsigned int i = 0; i < user_list_size; i++) {
            std::string userID = user_list[i];
            userList.push_back(userID);
        }
        std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
        if (getUserListInViewCallbacks.count(seq) > 0) {
            auto cb = getUserListInViewCallbacks[seq];
            getUserListInViewCallbacks.erase(seq);
            if (cb) {
                cb(error_code, userList);
            }
        }
    }

    void zego_on_range_scene_send_custom_command(zego_seq seq, int error_code) {
        std::lock_guard<std::mutex> lock(lowFrequencyClosureMutex);
        if (sendCustomCommandCallbacks.count(seq) > 0) {
            auto cb = sendCustomCommandCallbacks[seq];
            sendCustomCommandCallbacks.erase(seq);
            if (cb) {
                cb(error_code);
            }
        }
    }

    void zego_on_range_scene_stream_user_stream_state_update(const char *userid,
                                                             const char *stream_id,
                                                             enum zego_stream_state state) {
        if (rangeSceneStream) {
            rangeSceneStream->zego_on_range_scene_stream_user_stream_state_update(this, userid,
                                                                                  stream_id, state);
        }
    }

    void zego_on_range_scene_stream_user_mic_update(const char *userid,
                                                    enum zego_device_state state) {
        if (rangeSceneStream) {
            rangeSceneStream->zego_on_range_scene_stream_user_mic_update(this, userid, state);
        }
    }

    void zego_on_range_scene_stream_user_camera_update(const char *userid,
                                                       enum zego_device_state state) {
        if (rangeSceneStream) {
            rangeSceneStream->zego_on_range_scene_stream_user_camera_update(this, userid, state);
        }
    }

    void zego_on_range_scene_stream_user_speaker_update(const char *userid,
                                                        enum zego_device_state state) {
        if (rangeSceneStream) {
            rangeSceneStream->zego_on_range_scene_stream_user_speaker_update(this, userid, state);
        }
    }

    void zego_on_range_scene_team_team_state_update(unsigned int team_id,
                                                    enum zego_team_state state,
                                                    zego_error error_code) {
        if (rangeSceneTeam) {
            rangeSceneTeam->zego_on_range_scene_team_team_state_update(this, team_id, state,
                                                                       error_code);
        }
    }

    void zego_on_range_scene_team_team_member_update(unsigned int team_id,
                                                     enum zego_update_type update_type,
                                                     const struct zego_user *user_list,
                                                     unsigned int user_count) {
        if (rangeSceneTeam) {
            rangeSceneTeam->zego_on_range_scene_team_team_member_update(this, team_id, update_type,
                                                                        user_list, user_count);
        }
    }

    void zego_on_range_scene_team_join_team(zego_seq seq, int error_code, unsigned int team_id) {
        if (rangeSceneTeam) {
            rangeSceneTeam->zego_on_range_scene_team_join_team(seq, error_code, team_id);
        }
    }

    void zego_on_range_scene_team_leave_team(zego_seq seq, int error_code, unsigned int team_id) {
        if (rangeSceneTeam) {
            rangeSceneTeam->zego_on_range_scene_team_leave_team(seq, error_code, team_id);
        }
    }

    void zego_on_range_scene_item_item_enter_view(long long item_id, unsigned int capacity,
                                                  struct zego_position position,
                                                  const char **user_list,
                                                  unsigned int user_list_size) {
        if (rangeSceneItem) {
            rangeSceneItem->zego_on_range_scene_item_item_enter_view(
                this, item_id, capacity, position, user_list, user_list_size);
        }
    }

    void zego_on_range_scene_item_item_leave_view(long long item_id) {
        if (rangeSceneItem) {
            rangeSceneItem->zego_on_range_scene_item_item_leave_view(this, item_id);
        }
    }

    void zego_on_range_scene_item_item_bind_update(long long item_id, const char *userid) {
        if (rangeSceneItem) {
            rangeSceneItem->zego_on_range_scene_item_item_bind_update(this, item_id, userid);
        }
    }

    void zego_on_range_scene_item_item_unbind_update(long long item_id, const char **user_list,
                                                     unsigned int user_list_size) {
        if (rangeSceneItem) {
            rangeSceneItem->zego_on_range_scene_item_item_unbind_update(this, item_id, user_list,
                                                                        user_list_size);
        }
    }

    void zego_on_range_scene_item_item_status_update(long long item_id,
                                                     struct zego_position position,
                                                     unsigned int channel,
                                                     const unsigned char *status,
                                                     unsigned int status_length) {
        if (rangeSceneItem) {
            rangeSceneItem->zego_on_range_scene_item_item_status_update(
                this, item_id, position, channel, status, status_length);
        }
    }

    void zego_on_range_scene_item_item_command_update(long long item_id,
                                                      struct zego_position position,
                                                      unsigned int channel,
                                                      const unsigned char *command,
                                                      unsigned int command_length) {
        if (rangeSceneItem) {
            rangeSceneItem->zego_on_range_scene_item_item_command_update(
                this, item_id, position, channel, command, command_length);
        }
    }

    void zego_on_range_scene_item_create_item(zego_seq seq, int error_code, long long item_id) {
        if (rangeSceneItem) {
            rangeSceneItem->zego_on_range_scene_item_create_item(seq, error_code, item_id);
        }
    }

    void zego_on_range_scene_item_destroy_item(zego_seq seq, int error_code, long long item_id) {
        if (rangeSceneItem) {
            rangeSceneItem->zego_on_range_scene_item_destroy_item(seq, error_code, item_id);
        }
    }

    void zego_on_range_scene_item_bind_item(zego_seq seq, int error_code, long long item_id) {
        if (rangeSceneItem) {
            rangeSceneItem->zego_on_range_scene_item_bind_item(seq, error_code, item_id);
        }
    }

    void zego_on_range_scene_item_unbind_item(zego_seq seq, int error_code, long long item_id) {
        if (rangeSceneItem) {
            rangeSceneItem->zego_on_range_scene_item_unbind_item(seq, error_code, item_id);
        }
    }

    void zego_on_range_scene_item_update_item_status(zego_seq seq, int error_code,
                                                     long long item_id) {
        if (rangeSceneItem) {
            rangeSceneItem->zego_on_range_scene_item_update_item_status(seq, error_code, item_id);
        }
    }

    void zego_on_range_scene_item_update_item_command(zego_seq seq, int error_code,
                                                      long long item_id) {
        if (rangeSceneItem) {
            rangeSceneItem->zego_on_range_scene_item_update_item_command(seq, error_code, item_id);
        }
    }

  private:
    int rangeSceneHandle = -1;
    std::shared_ptr<ZegoExpressRangeSceneStreamImp> rangeSceneStream;
    std::shared_ptr<ZegoExpressRangeSceneTeamImp> rangeSceneTeam;
    std::shared_ptr<ZegoExpressRangeSceneItemImp> rangeSceneItem;
    std::shared_ptr<IZegoRangeSceneEventHandler> eventHandler;
    std::unordered_map<zego_seq, ZegoRangeSceneLoginSceneCallback> loginSceneCallbacks;
    std::unordered_map<zego_seq, ZegoRangeSceneLogoutSceneCallback> logoutSceneCallbacks;
    std::unordered_map<zego_seq, ZegoRangeSceneGetUserCountCallback> getUserCountCallbacks;
    std::unordered_map<zego_seq, ZegoRangeSceneGetUserListInViewCallback>
        getUserListInViewCallbacks;
    std::unordered_map<zego_seq, ZegoRangeSceneSendCustomCommandCallback>
        sendCustomCommandCallbacks;
    std::mutex rangeSceneMutex;
    std::mutex lowFrequencyClosureMutex;
};

} // namespace EXPRESS
} // namespace ZEGO

ZEGO_ENABLE_DEPRECATION_WARNINGS
