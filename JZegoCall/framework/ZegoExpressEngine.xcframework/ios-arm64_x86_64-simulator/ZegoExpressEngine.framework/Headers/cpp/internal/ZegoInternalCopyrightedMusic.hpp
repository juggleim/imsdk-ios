#pragma once

#include "../ZegoExpressDefines.h"
#include "../ZegoExpressEventHandler.h"
#include "../ZegoExpressInterface.h"

#include "ZegoInternalBase.h"
#include "ZegoInternalBridge.h"

#define COPYRIGHTEDMUSIC_COPY_CHAR_TO_STD(param) std::string std##param = param;
namespace ZEGO {
namespace EXPRESS {

class ZegoExpressCopyrightedMusicImp : public IZegoCopyrightedMusic {
  public:
    ZegoExpressCopyrightedMusicImp(int index) { ZEGO_UNUSED_VARIABLE(index); }

    void setEventHandler(std::shared_ptr<IZegoCopyrightedMusicEventHandler> handler) override {
        std::lock_guard<std::mutex> loc(copyrightedMusicEventMutex);
        eventHandler = handler;
    }

    void initCopyrightedMusic(ZegoCopyrightedMusicConfig config,
                              ZegoCopyrightedMusicInitCallback callback) override {
        zego_copyrighted_music_config _config;
        memset(&_config, 0, sizeof(_config));

        ZegoStrncpy(_config.user.user_id, config.user.userID.c_str(), ZEGO_EXPRESS_MAX_USERID_LEN);
        ZegoStrncpy(_config.user.user_name, config.user.userName.c_str(),
                    ZEGO_EXPRESS_MAX_USERNAME_LEN);

        int seq = oInternalOriginBridge->copyrightedMusicInitCopyrightedMusic(_config);
        if (callback != nullptr) {
            std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
            initCallbacks.insert({seq, callback});
        }
    }

    unsigned long long getCacheSize() override {
        return oInternalOriginBridge->copyrightedMusicGetCacheSize();
    }

    void clearCache() override { oInternalOriginBridge->copyrightedMusicClearCache(); }

    void sendExtendedRequest(const std::string &command, const std::string &params,
                             ZegoCopyrightedMusicSendExtendedRequestCallback callback) override {
        const char *_command = command.empty() ? nullptr : command.c_str();
        const char *_params = params.empty() ? nullptr : params.c_str();

        int seq = oInternalOriginBridge->copyrightedMusicSendExtendedRequest(_command, _params);
        if (callback != nullptr) {
            std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
            sendExtendedRequestCallbacks.insert({seq, callback});
        }
    }

    void getLrcLyric(const std::string &songID,
                     ZegoCopyrightedMusicGetLrcLyricCallback callback) override {
        const char *_songID = songID.empty() ? nullptr : songID.c_str();

        int seq = oInternalOriginBridge->copyrightedMusicGetLrcLyric(_songID);
        if (callback) {
            std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
            getLrcLyricCallbacks.insert({seq, callback});
        }
    }

    void getLrcLyric(const std::string &songID, ZegoCopyrightedMusicVendorID vendorID,
                     ZegoCopyrightedMusicGetLrcLyricCallback callback) override {
        const char *_songID = songID.empty() ? nullptr : songID.c_str();

        int seq = oInternalOriginBridge->copyrightedMusicGetLrcLyric(
            _songID, (zego_copyrighted_music_vendor_id)vendorID);
        if (callback) {
            std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
            getLrcLyricCallbacks.insert({seq, callback});
        }
    }

    void getLrcLyric(ZegoCopyrightedMusicGetLyricConfig config,
                     ZegoCopyrightedMusicGetLrcLyricCallback callback) override {
        zego_copyrighted_music_get_lyric_config _config;
        toGetLyricConfig(_config, config);

        int seq = oInternalOriginBridge->copyrightedMusicGetLrcLyric(_config);
        if (callback) {
            std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
            getLrcLyricCallbacks.insert({seq, callback});
        }
    }

    void getKrcLyricByToken(const std::string &krcToken,
                            ZegoCopyrightedMusicGetKrcLyricByTokenCallback callback) override {
        const char *_krcToken = krcToken.empty() ? nullptr : krcToken.c_str();

        int seq = oInternalOriginBridge->copyrightedMusicGetKrcLyricByToken(_krcToken);
        if (callback) {
            std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
            getKrcLyricByTokenCallbacks.insert({seq, callback});
        }
    }

    void requestSong(ZegoCopyrightedMusicRequestConfig config,
                     ZegoCopyrightedMusicRequestSongCallback callback) override {
        zego_copyrighted_music_request_config _config;
        toRequestConfig(_config, config);

        int seq = oInternalOriginBridge->copyrightedMusicRequestSong(_config);
        if (callback) {
            std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
            requestSongCallbacks.insert({seq, callback});
        }
    }

    void requestAccompaniment(ZegoCopyrightedMusicRequestConfig config,
                              ZegoCopyrightedMusicRequestAccompanimentCallback callback) override {
        zego_copyrighted_music_request_config _config;
        toRequestConfig(_config, config);

        int seq = oInternalOriginBridge->copyrightedMusicRequestAccompaniment(_config);
        if (callback) {
            std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
            requestAccompanimentCallbacks.insert({seq, callback});
        }
    }

    void requestAccompanimentClip(
        ZegoCopyrightedMusicRequestConfig config,
        ZegoCopyrightedMusicRequestAccompanimentClipCallback callback) override {
        zego_copyrighted_music_request_config _config;
        toRequestConfig(_config, config);

        int seq = oInternalOriginBridge->copyrightedMusicRequestAccompanimentClip(_config);
        if (callback) {
            std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
            requestAccompanimentClipCallbacks.insert({seq, callback});
        }
    }

    void getMusicByToken(const std::string &songToken,
                         ZegoCopyrightedMusicGetMusicByTokenCallback callback) override {
        const char *_songToken = songToken.empty() ? nullptr : songToken.c_str();

        int seq = oInternalOriginBridge->copyrightedMusicGetMusicByToken(_songToken);
        if (callback) {
            std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
            getMusicByTokenCallbacks.insert({seq, callback});
        }
    }

    void requestResource(ZegoCopyrightedMusicRequestConfig config,
                         ZegoCopyrightedMusicResourceType type,
                         ZegoCopyrightedMusicRequestResourceCallback callback) override {
        zego_copyrighted_music_request_config _config;
        zego_copyrighted_music_resource_type _type;

        toRequestConfig(_config, config);
        _type = (zego_copyrighted_music_resource_type)type;

        int seq = oInternalOriginBridge->copyrightedMusicRequestResource(_config, _type);
        if (callback) {
            std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
            requestResourceCallbacks.insert({seq, callback});
        }
    }

    void requestResource(ZegoCopyrightedMusicRequestConfigV2 config,
                         ZegoCopyrightedMusicRequestResourceCallback callback) override {
        zego_copyrighted_music_request_config_v2 _config;

        toRequestConfigV2(_config, config);

        int seq = oInternalOriginBridge->copyrightedMusicRequestResource(_config);
        if (callback) {
            std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
            requestResourceCallbacks.insert({seq, callback});
        }
    }

    void getSharedResource(ZegoCopyrightedMusicGetSharedConfig config,
                           ZegoCopyrightedMusicResourceType type,
                           ZegoCopyrightedMusicGetSharedResourceCallback callback) override {
        zego_copyrighted_music_get_shared_config _config;
        zego_copyrighted_music_resource_type _type;

        toGetSharedConfig(_config, config);
        _type = (zego_copyrighted_music_resource_type)type;

        int seq = oInternalOriginBridge->copyrightedMusicGetSharedResource(_config, _type);
        if (callback) {
            std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
            getSharedResourceCallbacks.insert({seq, callback});
        }
    }

    void getSharedResource(ZegoCopyrightedMusicGetSharedConfigV2 config,
                           ZegoCopyrightedMusicGetSharedResourceCallback callback) override {
        zego_copyrighted_music_get_shared_config_v2 _config;

        toGetSharedConfigV2(_config, config);

        int seq = oInternalOriginBridge->copyrightedMusicGetSharedResource(_config);
        if (callback) {
            std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
            getSharedResourceCallbacks.insert({seq, callback});
        }
    }

    void download(const std::string &resourceID,
                  ZegoCopyrightedMusicDownloadCallback callback) override {
        const char *_resourceID = resourceID.empty() ? nullptr : resourceID.c_str();

        int seq = oInternalOriginBridge->copyrightedMusicDownload(_resourceID);
        if (callback) {
            std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
            loadCallbacks.insert({seq, callback});
        }
    }

    void cancelDownload(const std::string &resourceID) override {
        const char *_resourceID = resourceID.empty() ? nullptr : resourceID.c_str();

        oInternalOriginBridge->copyrightedMusicCancelDownload(_resourceID);
    }

    bool queryCache(const std::string &songID, ZegoCopyrightedMusicType type) override {
        const char *_songID = songID.empty() ? nullptr : songID.c_str();

        bool result = oInternalOriginBridge->copyrightedMusicQueryCache(
            _songID, (zego_copyrighted_music_type)type);
        return result;
    }

    bool queryCache(const std::string &songID, ZegoCopyrightedMusicType type,
                    ZegoCopyrightedMusicVendorID vendorID) override {
        const char *_songID = songID.empty() ? nullptr : songID.c_str();

        bool result = oInternalOriginBridge->copyrightedMusicQueryCache(
            _songID, (zego_copyrighted_music_type)type, (zego_copyrighted_music_vendor_id)vendorID);
        return result;
    }

    bool queryCache(ZegoCopyrightedMusicQueryCacheConfig config) override {
        zego_copyrighted_music_query_cache_config _config;
        toQueryCacheConfig(_config, config);

        bool result = oInternalOriginBridge->copyrightedMusicQueryCache(_config);
        return result;
    }

    bool queryCache(ZegoCopyrightedMusicQueryCacheConfigV2 config) override {
        zego_copyrighted_music_query_cache_config_v2 _config;
        toQueryCacheConfigV2(_config, config);

        bool result = oInternalOriginBridge->copyrightedMusicQueryCache(_config);
        return result;
    }

    unsigned long long getDuration(const std::string &resourceID) override {
        const char *_resourceID = resourceID.empty() ? nullptr : resourceID.c_str();

        unsigned long long totalDuration =
            oInternalOriginBridge->copyrightedMusicGetDuration(_resourceID);
        return totalDuration;
    }

    void setScoringLevel(int level) override {
        oInternalOriginBridge->copyrightedMusicSetScoringLevel(level);
    }

    int startScore(const std::string &resourceID, int pitchValueInterval) override {
        const char *_resourceID = resourceID.empty() ? nullptr : resourceID.c_str();

        int ret =
            oInternalOriginBridge->copyrightedMusicStartScore(_resourceID, pitchValueInterval);
        return ret;
    }

    int pauseScore(const std::string &resourceID) override {
        const char *_resourceID = resourceID.empty() ? nullptr : resourceID.c_str();

        int ret = oInternalOriginBridge->copyrightedMusicPauseScore(_resourceID);
        return ret;
    }

    int resumeScore(const std::string &resourceID) override {
        const char *_resourceID = resourceID.empty() ? nullptr : resourceID.c_str();

        int ret = oInternalOriginBridge->copyrightedMusicResumeScore(_resourceID);
        return ret;
    }

    int stopScore(const std::string &resourceID) override {
        const char *_resourceID = resourceID.empty() ? nullptr : resourceID.c_str();

        int ret = oInternalOriginBridge->copyrightedMusicStopScore(_resourceID);
        return ret;
    }

    int resetScore(const std::string &resourceID) override {
        const char *_resourceID = resourceID.empty() ? nullptr : resourceID.c_str();

        int ret = oInternalOriginBridge->copyrightedMusicResetScore(_resourceID);
        return ret;
    }

    int getPreviousScore(const std::string &resourceID) override {
        const char *_resourceID = resourceID.empty() ? nullptr : resourceID.c_str();

        int ret = oInternalOriginBridge->copyrightedMusicGetPreviousScore(_resourceID);
        return ret;
    }

    int getAverageScore(const std::string &resourceID) override {
        const char *_resourceID = resourceID.empty() ? nullptr : resourceID.c_str();

        int ret = oInternalOriginBridge->copyrightedMusicGetAverageScore(_resourceID);
        return ret;
    }

    int getTotalScore(const std::string &resourceID) override {
        const char *_resourceID = resourceID.empty() ? nullptr : resourceID.c_str();

        int ret = oInternalOriginBridge->copyrightedMusicGetTotalScore(_resourceID);
        return ret;
    }

    int getFullScore(const std::string &resourceID) override {
        const char *_resourceID = resourceID.empty() ? nullptr : resourceID.c_str();

        int ret = oInternalOriginBridge->copyrightedMusicGetFullScore(_resourceID);
        return ret;
    }

    int getCurrentPitch(const std::string &resourceID) override {
        const char *_resourceID = resourceID.empty() ? nullptr : resourceID.c_str();

        int ret = oInternalOriginBridge->copyrightedMusicGetCurrentPitch(_resourceID);
        return ret;
    }

    void getStandardPitch(const std::string &resourceID,
                          ZegoCopyrightedMusicGetStandardPitchCallback callback) override {
        const char *_resourceID = resourceID.empty() ? nullptr : resourceID.c_str();

        int seq = oInternalOriginBridge->copyrightedMusicGetStandardPitch(_resourceID);
        if (callback) {
            std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
            getStandardPitchCallbacks.insert({seq, callback});
        }
    }

    void zego_on_copyrighted_music_download_progress_update(const char *resource_id,
                                                            float progress_rate) {
        std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
        if (eventHandler) {
            auto weakEventHandler = std::weak_ptr<IZegoCopyrightedMusicEventHandler>(eventHandler);
            COPYRIGHTEDMUSIC_COPY_CHAR_TO_STD(resource_id);
            ZEGO_SWITCH_THREAD_PRE
            auto handlerInMain = weakEventHandler.lock();
            if (handlerInMain) {
                handlerInMain->onDownloadProgressUpdate(this, stdresource_id, progress_rate);
            }
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_copyrighted_music_current_pitch_value_update(const char *resource_id,
                                                              int current_duration,
                                                              int pitch_value) {
        std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
        if (eventHandler) {
            auto weakEventHandler = std::weak_ptr<IZegoCopyrightedMusicEventHandler>(eventHandler);
            COPYRIGHTEDMUSIC_COPY_CHAR_TO_STD(resource_id);
            ZEGO_SWITCH_THREAD_PRE
            auto handlerInMain = weakEventHandler.lock();
            if (handlerInMain) {
                handlerInMain->onCurrentPitchValueUpdate(this, stdresource_id, current_duration,
                                                         pitch_value);
            }
            ZEGO_SWITCH_THREAD_ING
        }
    }

    // seq
    void zego_on_copyrighted_music_init(zego_seq seq, zego_error error_code) {
        std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
        if (initCallbacks.count(seq) > 0) {
            auto cb = initCallbacks[seq];
            initCallbacks.erase(seq);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_copyrighted_music_send_extended_request(zego_seq seq, zego_error error_code,
                                                         const char *command, const char *result) {
        std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
        if (sendExtendedRequestCallbacks.count(seq) > 0) {
            auto cb = sendExtendedRequestCallbacks[seq];
            sendExtendedRequestCallbacks.erase(seq);
            COPYRIGHTEDMUSIC_COPY_CHAR_TO_STD(command);
            COPYRIGHTEDMUSIC_COPY_CHAR_TO_STD(result);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code, stdcommand, stdresult);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_copyrighted_music_get_lrc_lyric(zego_seq seq, zego_error error_code,
                                                 const char *lyrics) {
        std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
        if (getLrcLyricCallbacks.count(seq) > 0) {
            auto cb = getLrcLyricCallbacks[seq];
            getLrcLyricCallbacks.erase(seq);
            COPYRIGHTEDMUSIC_COPY_CHAR_TO_STD(lyrics);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code, stdlyrics);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_copyrighted_music_get_krc_lyric_by_token(zego_seq seq, zego_error error_code,
                                                          const char *lyrics) {
        std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
        if (getKrcLyricByTokenCallbacks.count(seq) > 0) {
            auto cb = getKrcLyricByTokenCallbacks[seq];
            getKrcLyricByTokenCallbacks.erase(seq);
            COPYRIGHTEDMUSIC_COPY_CHAR_TO_STD(lyrics);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code, stdlyrics);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_copyrighted_music_request_song(zego_seq seq, zego_error error_code,
                                                const char *resource) {
        std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
        if (requestSongCallbacks.count(seq) > 0) {
            auto cb = requestSongCallbacks[seq];
            requestSongCallbacks.erase(seq);
            COPYRIGHTEDMUSIC_COPY_CHAR_TO_STD(resource);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code, stdresource);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_copyrighted_music_request_accompaniment(zego_seq seq, zego_error error_code,
                                                         const char *resource) {
        std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
        if (requestAccompanimentCallbacks.count(seq) > 0) {
            auto cb = requestAccompanimentCallbacks[seq];
            requestAccompanimentCallbacks.erase(seq);
            COPYRIGHTEDMUSIC_COPY_CHAR_TO_STD(resource);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code, stdresource);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_copyrighted_music_request_accompaniment_clip(zego_seq seq, zego_error error_code,
                                                              const char *resource) {
        std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
        if (requestAccompanimentClipCallbacks.count(seq) > 0) {
            auto cb = requestAccompanimentClipCallbacks[seq];
            requestAccompanimentClipCallbacks.erase(seq);
            COPYRIGHTEDMUSIC_COPY_CHAR_TO_STD(resource);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code, stdresource);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_copyrighted_music_get_music_by_token(zego_seq seq, zego_error error_code,
                                                      const char *resource) {
        std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
        if (getMusicByTokenCallbacks.count(seq) > 0) {
            auto cb = getMusicByTokenCallbacks[seq];
            getMusicByTokenCallbacks.erase(seq);
            COPYRIGHTEDMUSIC_COPY_CHAR_TO_STD(resource);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code, stdresource);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_copyrighted_music_request_resource(zego_seq seq, zego_error error_code,
                                                    const char *resource) {
        std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
        if (requestResourceCallbacks.count(seq) > 0) {
            auto cb = requestResourceCallbacks[seq];
            requestResourceCallbacks.erase(seq);
            COPYRIGHTEDMUSIC_COPY_CHAR_TO_STD(resource);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code, stdresource);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_copyrighted_music_get_shared_resource(zego_seq seq, zego_error error_code,
                                                       const char *resource) {
        std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
        if (getSharedResourceCallbacks.count(seq) > 0) {
            auto cb = getSharedResourceCallbacks[seq];
            getSharedResourceCallbacks.erase(seq);
            COPYRIGHTEDMUSIC_COPY_CHAR_TO_STD(resource);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code, stdresource);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_copyrighted_music_download(zego_seq seq, zego_error error_code) {
        std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
        if (loadCallbacks.count(seq) > 0) {
            auto cb = loadCallbacks[seq];
            loadCallbacks.erase(seq);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code);
            ZEGO_SWITCH_THREAD_ING
        }
    }

    void zego_on_copyrighted_music_get_standard_pitch(zego_seq seq, zego_error error_code,
                                                      const char *pitch) {
        std::lock_guard<std::mutex> lock(copyrightedMusicEventMutex);
        if (getStandardPitchCallbacks.count(seq) > 0) {
            auto cb = getStandardPitchCallbacks[seq];
            getStandardPitchCallbacks.erase(seq);
            COPYRIGHTEDMUSIC_COPY_CHAR_TO_STD(pitch);
            ZEGO_SWITCH_THREAD_PRE
            cb(error_code, stdpitch);
            ZEGO_SWITCH_THREAD_ING
        }
    }

  private:
    void toGetLyricConfig(zego_copyrighted_music_get_lyric_config &oconfig,
                          const ZegoCopyrightedMusicGetLyricConfig &config) {
        memset(&oconfig, 0, sizeof(oconfig));
        ZegoStrncpy(oconfig.song_id, config.songID.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN - 1);
        oconfig.vendor_id = config.vendorID;
    }

    void toRequestConfig(zego_copyrighted_music_request_config &oconfig,
                         const ZegoCopyrightedMusicRequestConfig &config) {
        memset(&oconfig, 0, sizeof(oconfig));
        ZegoStrncpy(oconfig.song_id, config.songID.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN - 1);
        oconfig.mode = (zego_copyrighted_music_billing_mode)config.mode;
        oconfig.vendor_id = (zego_copyrighted_music_vendor_id)config.vendorID;
        ZegoStrncpy(oconfig.room_id, config.roomID.c_str(), ZEGO_EXPRESS_MAX_ROOMID_LEN - 1);
        ZegoStrncpy(oconfig.master_id, config.masterID.c_str(), ZEGO_EXPRESS_MAX_USERID_LEN - 1);
        oconfig.scene_id = config.sceneID;
    }

    void toRequestConfigV2(zego_copyrighted_music_request_config_v2 &oconfig,
                           const ZegoCopyrightedMusicRequestConfigV2 &config) {
        memset(&oconfig, 0, sizeof(oconfig));
        ZegoStrncpy(oconfig.song_id, config.songID.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN - 1);
        oconfig.mode = config.mode;
        oconfig.vendor_id = config.vendorID;
        ZegoStrncpy(oconfig.room_id, config.roomID.c_str(), ZEGO_EXPRESS_MAX_ROOMID_LEN - 1);
        ZegoStrncpy(oconfig.master_id, config.masterID.c_str(), ZEGO_EXPRESS_MAX_USERID_LEN - 1);
        oconfig.scene_id = config.sceneID;
        oconfig.resource_type = config.resourceType;
    }

    void toGetSharedConfig(zego_copyrighted_music_get_shared_config &oconfig,
                           const ZegoCopyrightedMusicGetSharedConfig &config) {
        memset(&oconfig, 0, sizeof(oconfig));
        ZegoStrncpy(oconfig.song_id, config.songID.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN - 1);
        oconfig.vendor_id = (zego_copyrighted_music_vendor_id)config.vendorID;
        ZegoStrncpy(oconfig.room_id, config.roomID.c_str(), ZEGO_EXPRESS_MAX_ROOMID_LEN - 1);
    }

    void toGetSharedConfigV2(zego_copyrighted_music_get_shared_config_v2 &oconfig,
                             const ZegoCopyrightedMusicGetSharedConfigV2 &config) {
        memset(&oconfig, 0, sizeof(oconfig));
        ZegoStrncpy(oconfig.song_id, config.songID.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN - 1);
        oconfig.vendor_id = config.vendorID;
        ZegoStrncpy(oconfig.room_id, config.roomID.c_str(), ZEGO_EXPRESS_MAX_ROOMID_LEN - 1);
        oconfig.resource_type = config.resourceType;
    }

    void toQueryCacheConfig(zego_copyrighted_music_query_cache_config &oconfig,
                            const ZegoCopyrightedMusicQueryCacheConfig &config) {
        memset(&oconfig, 0, sizeof(oconfig));
        ZegoStrncpy(oconfig.song_id, config.songID.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN - 1);
        oconfig.resource_type = (zego_copyrighted_music_resource_type)config.resourceType;
        oconfig.resource_quality_type =
            (zego_copyrighted_music_resource_quality_type)config.resourceQualityType;
        oconfig.vendor_id = (zego_copyrighted_music_vendor_id)config.vendorID;
    }

    void toQueryCacheConfigV2(zego_copyrighted_music_query_cache_config_v2 &oconfig,
                              const ZegoCopyrightedMusicQueryCacheConfigV2 &config) {
        memset(&oconfig, 0, sizeof(oconfig));
        ZegoStrncpy(oconfig.song_id, config.songID.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN - 1);
        oconfig.resource_type = config.resourceType;
        oconfig.resource_quality_type = config.resourceQualityType;
        oconfig.vendor_id = config.vendorID;
    }

  private:
    std::shared_ptr<IZegoCopyrightedMusicEventHandler> eventHandler;
    std::mutex copyrightedMusicEventMutex;
    std::unordered_map<zego_seq, ZegoCopyrightedMusicInitCallback> initCallbacks;
    std::unordered_map<zego_seq, ZegoCopyrightedMusicSendExtendedRequestCallback>
        sendExtendedRequestCallbacks;
    std::unordered_map<zego_seq, ZegoCopyrightedMusicGetLrcLyricCallback> getLrcLyricCallbacks;
    std::unordered_map<zego_seq, ZegoCopyrightedMusicGetKrcLyricByTokenCallback>
        getKrcLyricByTokenCallbacks;
    std::unordered_map<zego_seq, ZegoCopyrightedMusicRequestSongCallback> requestSongCallbacks;
    std::unordered_map<zego_seq, ZegoCopyrightedMusicRequestAccompanimentCallback>
        requestAccompanimentCallbacks;
    std::unordered_map<zego_seq, ZegoCopyrightedMusicRequestAccompanimentClipCallback>
        requestAccompanimentClipCallbacks;
    std::unordered_map<zego_seq, ZegoCopyrightedMusicGetMusicByTokenCallback>
        getMusicByTokenCallbacks;
    std::unordered_map<zego_seq, ZegoCopyrightedMusicRequestResourceCallback>
        requestResourceCallbacks;
    std::unordered_map<zego_seq, ZegoCopyrightedMusicGetSharedResourceCallback>
        getSharedResourceCallbacks;
    std::unordered_map<zego_seq, ZegoCopyrightedMusicDownloadCallback> loadCallbacks;
    std::unordered_map<zego_seq, ZegoCopyrightedMusicGetStandardPitchCallback>
        getStandardPitchCallbacks;
};

class ZegoExpressCopyrightedMusicContainer {
  public:
    std::recursive_mutex copyrightedMusicMutex;
    std::shared_ptr<ZegoExpressCopyrightedMusicImp> copyrightedMusicInstance = nullptr;
};
#define oCopyrightedMusicContainer                                                                 \
    ZegoSingleton<ZegoExpressCopyrightedMusicContainer>::CreateInstance()
} // namespace EXPRESS
} // namespace ZEGO
