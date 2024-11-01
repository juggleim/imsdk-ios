#pragma once

#include "../ZegoExpressDefines.h"
#include "../ZegoExpressErrorCode.h"
#include "../ZegoExpressEventHandler.h"
#include "ZegoInternalBridge.h"

namespace ZEGO {
namespace EXPRESS {

#define declearSingleRawMember(MemberType)                                                         \
    MemberType m##MemberType = nullptr;                                                            \
    std::recursive_mutex m##MemberType##Mutex;                                                     \
                                                                                                   \
    void set##MemberType(MemberType member) {                                                      \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex);                          \
        m##MemberType = member;                                                                    \
    }                                                                                              \
                                                                                                   \
    MemberType get##MemberType() {                                                                 \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex);                          \
        return m##MemberType;                                                                      \
    }

#define declearSingleShareMember(MemberType)                                                       \
    std::shared_ptr<MemberType> m##MemberType;                                                     \
    std::recursive_mutex m##MemberType##Mutex;                                                     \
                                                                                                   \
    void set##MemberType(std::shared_ptr<MemberType> member) {                                     \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex);                          \
        m##MemberType = member;                                                                    \
    }                                                                                              \
                                                                                                   \
    std::shared_ptr<MemberType> get##MemberType() {                                                \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex);                          \
        return m##MemberType;                                                                      \
    }

#define declearMultiRawMember(KeyType, MemberType)                                                 \
    std::unordered_map<KeyType, MemberType> m##MemberType;                                         \
    std::recursive_mutex m##MemberType##Mutex;                                                     \
                                                                                                   \
    void insert##MemberType(KeyType seq, MemberType member) {                                      \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex);                          \
        m##MemberType.insert({seq, member});                                                       \
    }                                                                                              \
                                                                                                   \
    MemberType get##MemberType(KeyType seq) {                                                      \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex);                          \
        MemberType member = nullptr;                                                               \
        if (m##MemberType.count(seq) > 0) {                                                        \
            member = m##MemberType[seq];                                                           \
        }                                                                                          \
        return member;                                                                             \
    }                                                                                              \
                                                                                                   \
    MemberType erase##MemberType(KeyType seq) {                                                    \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex);                          \
        MemberType member = get##MemberType(seq);                                                  \
        m##MemberType.erase(seq);                                                                  \
        return member;                                                                             \
    }

#define declearMultiShareMember(MemberType)                                                        \
    std::unordered_map<zego_seq, std::shared_ptr<MemberType>> m##MemberType;                       \
    std::recursive_mutex m##MemberType##Mutex;                                                     \
                                                                                                   \
    void insert##MemberType(zego_seq seq, std::shared_ptr<MemberType> member) {                    \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex);                          \
        m##MemberType.insert({seq, member});                                                       \
    }                                                                                              \
                                                                                                   \
    std::shared_ptr<MemberType> get##MemberType(zego_seq seq) {                                    \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex);                          \
        std::shared_ptr<MemberType> member = nullptr;                                              \
        if (m##MemberType.count(seq) > 0) {                                                        \
            member = m##MemberType[seq];                                                           \
        }                                                                                          \
        return member;                                                                             \
    }                                                                                              \
                                                                                                   \
    std::shared_ptr<MemberType> erase##MemberType(zego_seq seq) {                                  \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex);                          \
        std::shared_ptr<MemberType> member = get##MemberType(seq);                                 \
        m##MemberType.erase(seq);                                                                  \
        return member;                                                                             \
    }

using ZegoVoidPtr = void *;
using ZegoUInt = unsigned int;

template <typename T> class ZegoSingleton {
  public:
    template <typename... Args> static T *CreateInstance(Args &&... args) {
        if (m_pInstance == nullptr) {
            m_pInstance = new T(std::forward<Args>(args)...);
        }
        return m_pInstance;
    }

    static T *GetInstance() { return m_pInstance; }

    static void DestroyInstance() {
        if (m_pInstance) {
            delete m_pInstance;
            m_pInstance = nullptr;
        }
    }

  private:
    ZegoSingleton(void) = default;
    virtual ~ZegoSingleton(void) = default;

  private:
    static T *m_pInstance;
};
template <class T> T *ZegoSingleton<T>::m_pInstance = nullptr;

class ZegoExpressConvert {
  public:
    static ZegoUser I2OUser(const zego_user &_user) {
        ZegoUser user;
        user.userID = _user.user_id;
        user.userName = _user.user_name;
        return user;
    }

    static zego_user O2IUser(const ZegoUser &user) {
        zego_user _user;
        strncpy(_user.user_id, user.userID.c_str(), ZEGO_EXPRESS_MAX_USERID_LEN);
        strncpy(_user.user_name, user.userName.c_str(), ZEGO_EXPRESS_MAX_USERNAME_LEN);
        return _user;
    }

    static zego_proxy_info O2IProxyInfo(const ZegoProxyInfo &info) {
        zego_proxy_info _info;
        strncpy(_info.ip, info.ip.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
        strncpy(_info.hostname, info.hostName.c_str(), ZEGO_EXPRESS_MAX_URL_LEN);
        strncpy(_info.username, info.userName.c_str(), ZEGO_EXPRESS_MAX_USERNAME_LEN);
        strncpy(_info.password, info.password.c_str(), ZEGO_EXPRESS_MAX_USERNAME_LEN);
        _info.port = info.port;
        return _info;
    }

    // static zego_adaptive_video_config
    // O2IAdaptiveVideoConfig(const ZegoAdaptiveVideoConfig &config) {
    //     zego_adaptive_video_config _config;
    //     memset(&_config, 0, sizeof(zego_adaptive_video_config));
    //     _config.preference = static_cast<zego_video_config_preference>(config.preference);
    //     _config.min_encode_width = config.minEncodeWidth;
    //     _config.min_encode_height = config.minEncodeHeight;
    //     _config.min_fps = config.minFPS;
    //     return _config;
    // }

    static zego_room_config O2IRoomConfig(const ZegoRoomConfig &config) {
        zego_room_config _config;
        _config.max_member_count = config.maxMemberCount;
        _config.is_user_status_notify = config.isUserStatusNotify;
        strncpy(_config.token, config.token.c_str(), ZEGO_EXPRESS_MAX_ROOM_TOKEN_VALUE_LEN);
        _config.capability_negotiation_types = config.capabilityNegotiationTypes;
        return _config;
    }

    static ZegoStream I2OStream(const zego_stream &_stream) {
        ZegoStream stream;
        stream.user.userID = _stream.user.user_id;
        stream.user.userName = _stream.user.user_name;
        stream.streamID = _stream.stream_id;
        stream.extraInfo = _stream.extra_info;
        return stream;
    }

    static ZegoRoomExtraInfo I2ORoomExtraInfo(const zego_room_extra_info &_info) {
        ZegoRoomExtraInfo info;
        info.key = _info.key;
        info.value = _info.value;
        info.updateUser.userID = _info.update_user.user_id;
        info.updateUser.userName = _info.update_user.user_name;
        info.updateTime = _info.update_time;
        return info;
    }

    static zego_canvas O2ICanvas(const ZegoCanvas &canvas) {
        zego_canvas _canvas;
        _canvas.view = canvas.view;
        _canvas.view_mode = zego_view_mode(canvas.viewMode);
        _canvas.background_color = canvas.backgroundColor;
        _canvas.alpha_blend = canvas.alphaBlend;
        return _canvas;
    }

    static ZegoPublishStreamQuality
    I2OPushlishQuality(const zego_publish_stream_quality &_quality) {
        ZegoPublishStreamQuality quality;

        quality.videoCaptureFPS = _quality.video_capture_fps;
        quality.videoEncodeFPS = _quality.video_encode_fps;
        quality.videoSendFPS = _quality.video_send_fps;
        quality.videoKBPS = _quality.video_kbps;

        quality.audioCaptureFPS = _quality.audio_capture_fps;
        quality.audioSendFPS = _quality.audio_send_fps;
        quality.audioKBPS = _quality.audio_kbps;

        quality.rtt = _quality.rtt;
        quality.packetLostRate = _quality.packet_lost_rate;
        quality.level = ZegoStreamQualityLevel(_quality.level);
        quality.isHardwareEncode = _quality.is_hardware_encode;
        quality.videoCodecID = ZegoVideoCodecID(_quality.video_codec_id);

        quality.totalSendBytes = _quality.total_send_bytes;
        quality.audioSendBytes = _quality.audio_send_bytes;
        quality.videoSendBytes = _quality.video_send_bytes;

        return quality;
    }

    static ZegoPlayStreamQuality I2OPlayQuality(const zego_play_stream_quality &_quality) {
        ZegoPlayStreamQuality quality;
        quality.videoRecvFPS = _quality.video_recv_fps;
        quality.videoDejitterFPS = _quality.video_dejitter_fps;
        quality.videoDecodeFPS = _quality.video_decode_fps;
        quality.videoRenderFPS = _quality.video_render_fps;
        quality.videoKBPS = _quality.video_kbps;
        quality.videoBreakRate = _quality.video_break_rate;

        quality.audioRecvFPS = _quality.audio_recv_fps;
        quality.audioDejitterFPS = _quality.audio_dejitter_fps;
        quality.audioDecodeFPS = _quality.audio_decode_fps;
        quality.audioRenderFPS = _quality.audio_render_fps;
        quality.audioKBPS = _quality.audio_kbps;
        quality.audioBreakRate = _quality.audio_break_rate;

        quality.mos = _quality.mos;
        quality.rtt = _quality.rtt;
        quality.packetLostRate = _quality.packet_lost_rate;
        quality.peerToPeerDelay = _quality.peer_to_peer_delay;
        quality.peerToPeerPacketLostRate = _quality.peer_to_peer_packet_lost_rate;
        quality.level = ZegoStreamQualityLevel(_quality.level);
        quality.delay = _quality.delay;
        quality.avTimestampDiff = _quality.av_timestamp_diff;

        quality.isHardwareDecode = _quality.is_hardware_decode;
        quality.videoCodecID = ZegoVideoCodecID(_quality.video_codec_id);

        quality.totalRecvBytes = _quality.total_recv_bytes;
        quality.audioRecvBytes = _quality.audio_recv_bytes;
        quality.videoRecvBytes = _quality.video_recv_bytes;

        quality.audioCumulativeBreakCount = _quality.audio_cumulative_break_count;
        quality.audioCumulativeBreakTime = _quality.audio_cumulative_break_time;
        quality.audioCumulativeBreakRate = _quality.audio_cumulative_break_rate;
        quality.audioCumulativeDecodeTime = _quality.audio_cumulative_decode_time;

        quality.videoCumulativeBreakCount = _quality.video_cumulative_break_count;
        quality.videoCumulativeBreakTime = _quality.video_cumulative_break_time;
        quality.videoCumulativeBreakRate = _quality.video_cumulative_break_rate;
        quality.videoCumulativeDecodeTime = _quality.video_cumulative_decode_time;

        quality.muteVideo = _quality.mute_video;
        quality.muteAudio = _quality.mute_audio;

        return quality;
    }

    static ZegoDeviceInfo I2ODeviceInfo(const zego_device_info &_deviceInfo) {
        ZegoDeviceInfo deviceInfo;
        deviceInfo.deviceID = _deviceInfo.device_id;
        deviceInfo.deviceName = _deviceInfo.device_name;
        return deviceInfo;
    }

    static ZegoPerformanceStatus I2OPerformanceStatus(const zego_performance_status &_status) {
        ZegoPerformanceStatus status;
        status.cpuUsageApp = _status.cpu_usage_app;
        status.cpuUsageSystem = _status.cpu_usage_system;
        status.memoryUsageApp = _status.memory_usage_app;
        status.memoryUsageSystem = _status.memory_usage_system;
        status.memoryUsedApp = _status.memory_used_app;
        return status;
    }

    static zego_audio_config O2IAudioConfig(const ZegoAudioConfig &config) {
        zego_audio_config _config;
        _config.bitrate = config.bitrate;
        _config.channel = zego_audio_channel(config.channel);
        _config.codec_id = zego_audio_codec_id(config.codecID);
        return _config;
    }

    static ZegoAudioConfig I2OAudioConfig(const zego_audio_config &_config) {
        ZegoAudioConfig config;
        config.bitrate = _config.bitrate;
        config.channel = ZegoAudioChannel(_config.channel);
        config.codecID = ZegoAudioCodecID(_config.codec_id);
        return config;
    }

    static zego_video_config O2IVideoConfig(const ZegoVideoConfig &config) {
        zego_video_config _config;
        _config.bitrate = config.bitrate;
        _config.fps = config.fps;
        _config.capture_height = config.captureHeight;
        _config.capture_width = config.captureWidth;
        _config.encode_height = config.encodeHeight;
        _config.encode_width = config.encodeWidth;
        _config.codec_id = zego_video_codec_id(config.codecID);
        _config.key_frame_interval = config.keyFrameInterval;
        return _config;
    }

    static ZegoVideoConfig I2OVideoConfig(const zego_video_config &_config) {
        ZegoVideoConfig config;
        config.bitrate = _config.bitrate;
        config.fps = _config.fps;
        config.captureHeight = _config.capture_height;
        config.captureWidth = _config.capture_width;
        config.encodeHeight = _config.encode_height;
        config.encodeWidth = _config.encode_width;
        config.codecID = ZegoVideoCodecID(_config.codec_id);
        return config;
    }

    static ZegoRoomStreamList I2ORoomStreamList(const zego_room_stream_list *room_stream_list) {
        ZegoRoomStreamList streamList;

        if (nullptr == room_stream_list)
            return streamList;

        if (nullptr != room_stream_list->publish_stream_list &&
            0 != room_stream_list->publish_stream_list_count) {
            for (unsigned int i = 0; i < room_stream_list->publish_stream_list_count; i++) {
                ZegoStream stream;
                stream.streamID = room_stream_list->publish_stream_list[i].stream_id;
                stream.extraInfo = room_stream_list->publish_stream_list[i].extra_info;
                stream.user = I2OUser(room_stream_list->publish_stream_list[i].user);

                streamList.publishStreamList.emplace_back(std::move(stream));
            }
        }

        if (nullptr != room_stream_list->play_stream_list &&
            0 != room_stream_list->play_stream_list_count) {
            for (unsigned int i = 0; i < room_stream_list->play_stream_list_count; i++) {
                ZegoStream stream;
                stream.streamID = room_stream_list->play_stream_list[i].stream_id;
                stream.extraInfo = room_stream_list->play_stream_list[i].extra_info;
                stream.user = I2OUser(room_stream_list->play_stream_list[i].user);

                streamList.playStreamList.emplace_back(std::move(stream));
            }
        }

        return streamList;
    }

    static zego_video_frame_param O2IVideoFrameParam(const ZegoVideoFrameParam &param) {
        zego_video_frame_param _param;
        _param.format = zego_video_frame_format(param.format);
        memcpy(_param.strides, param.strides, sizeof(_param.strides));
        _param.height = param.height;
        _param.width = param.width;
        _param.rotation = param.rotation;
        return _param;
    }

    static ZegoVideoFrameParam I2OVideoFrameParam(const zego_video_frame_param &_param) {
        ZegoVideoFrameParam param;
        param.format = ZegoVideoFrameFormat(_param.format);
        memcpy(param.strides, _param.strides, sizeof(param.strides));
        param.height = _param.height;
        param.width = _param.width;
        param.rotation = _param.rotation;
        return param;
    }

    static ZegoRect I2ORect(const zego_rect &_rect) {
        ZegoRect rect;
        rect.x = _rect.left;
        rect.y = _rect.top;
        rect.width = _rect.right - _rect.left;
        rect.height = _rect.bottom - _rect.top;
        return rect;
    }

    static zego_video_encoded_frame_param
    O2IVideoEncodedFrameParam(const ZegoVideoEncodedFrameParam &param) {
        zego_video_encoded_frame_param _param;
        _param.format = zego_video_encoded_frame_format(param.format);
        _param.is_key_frame = param.isKeyFrame;
        _param.height = param.height;
        _param.width = param.width;
        _param.rotation = param.rotation;
        _param.sei_data = param.SEIData;
        _param.sei_data_length = param.SEIDataLength;
        return _param;
    }

    static ZegoVideoEncodedFrameParam
    I2OVideoEncodedFrameParam(const zego_video_encoded_frame_param &_param) {
        ZegoVideoEncodedFrameParam param;
        param.format = ZegoVideoEncodedFrameFormat(_param.format);
        param.isKeyFrame = _param.is_key_frame;
        param.height = _param.height;
        param.width = _param.width;
        param.rotation = _param.rotation;
        param.SEIData = _param.sei_data;
        param.SEIDataLength = _param.sei_data_length;
        return param;
    }

    static ZegoAudioFrameParam I2OAudioFrameParam(const zego_audio_frame_param &_param) {
        ZegoAudioFrameParam param;
        param.channel = ZegoAudioChannel(_param.channel);
        param.sampleRate = ZegoAudioSampleRate(_param.sample_rate);
        return param;
    }

    static zego_audio_frame_param O2IAudioFrameParam(const ZegoAudioFrameParam &param) {
        zego_audio_frame_param _param;
        _param.channel = zego_audio_channel(param.channel);
        _param.sample_rate = zego_audio_sample_rate(param.sampleRate);
        return _param;
    }

    static ZegoDataRecordConfig I2ODataRecordConfig(const zego_data_record_config &_config) {
        ZegoDataRecordConfig config;
        config.filePath = _config.file_path;
        config.recordType = ZegoDataRecordType(_config.record_type);
        return config;
    }

    static zego_data_record_config O2IDataRecordConfig(const ZegoDataRecordConfig &config) {
        zego_data_record_config _config;
        strncpy(_config.file_path, config.filePath.c_str(), ZEGO_EXPRESS_MAX_URL_LEN);
        _config.record_type = zego_data_record_type(config.recordType);
        return _config;
    }

    static zego_cdn_config O2ICDNConfig(const ZegoCDNConfig &config) {
        zego_cdn_config _config;
        strncpy(_config.url, config.url.c_str(), ZEGO_EXPRESS_MAX_URL_LEN);
        strncpy(_config.auth_param, config.authParam.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
        strncpy(_config.protocol, config.protocol.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
        strncpy(_config.quic_version, config.quicVersion.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
        _config.http_dns = static_cast<zego_http_dns_type>(config.httpdns);
        _config.quic_connect_mode = config.quicConnectMode;
        return _config;
    }

    static zego_mixer_input O2IMixerInput(const ZegoMixerInput &input) {
        zego_mixer_input _input;
        _input.content_type = zego_mixer_input_content_type(input.contentType);
        _input.layout.left = input.layout.x;
        _input.layout.right = input.layout.x + input.layout.width;
        _input.layout.top = input.layout.y;
        _input.layout.bottom = input.layout.y + input.layout.height;
        _input.sound_level_id = input.soundLevelID;
        _input.volume = input.volume;
        _input.is_audio_focus = input.isAudioFocus;
        if (input.audioDirection >= 0 && input.audioDirection <= 360) {
            _input.enable_audio_direction = true;
            _input.audio_direction = input.audioDirection;
        } else {
            _input.enable_audio_direction = false;
            _input.audio_direction = -1;
        }
        strncpy(_input.stream_id, input.streamID.c_str(), ZEGO_EXPRESS_MAX_URL_LEN);
        _input.label.left = input.label.left;
        _input.label.top = input.label.top;
        strncpy(_input.label.text, input.label.text.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
        _input.label.font.color = input.label.font.color;
        _input.label.font.size = input.label.font.size;
        _input.label.font.type = static_cast<zego_font_type>(input.label.font.type);
        _input.label.font.transparency = input.label.font.transparency;
        _input.label.font.border = input.label.font.border;
        _input.label.font.bordercolor = input.label.font.borderColor;
        _input.render_mode = static_cast<zego_mix_render_mode>(input.renderMode);
        _input.corner_radius = input.cornerRadius;

        strncpy(_input.image_info.url, input.imageInfo.url.c_str(), ZEGO_EXPRESS_MAX_URL_LEN);
        // _input.image_info.mix_mode = static_cast<zego_mixer_image_mode>(input.imageInfo.mixMode);
        _input.image_info.display_mode = input.imageInfo.displayMode;

        // advanced config
        std::string advanceConfig;
        for (auto iter : input.advancedConfig) {
            advanceConfig += iter.first + "=" + iter.second + ";";
        }
        strncpy(_input.advanced_config, advanceConfig.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);

        return _input;
    }

    static zego_mixer_output O2IMixerOutput(const ZegoMixerOutput &output) {
        zego_mixer_output _output;
        strncpy(_output.target, output.target.c_str(), ZEGO_EXPRESS_MAX_URL_LEN);
        return _output;
    }

    static zego_mixer_video_config O2IMixerVideoConfig(const ZegoMixerVideoConfig &config) {
        zego_mixer_video_config _config;
        _config.bitrate = config.bitrate;
        _config.fps = config.fps;
        _config.width = config.width;
        _config.height = config.height;
        //新增对齐
        _config.quality = config.quality;
        _config.rate_control_mode =
            static_cast<zego_video_rate_control_mode>(config.rateControlMode);
        return _config;
    }

    static zego_mixer_audio_config O2IMixerAudioConfig(const ZegoMixerAudioConfig &config) {
        zego_mixer_audio_config _config;
        _config.bitrate = config.bitrate;
        _config.codec_id = zego_audio_codec_id(config.codecID);
        _config.channel = zego_audio_channel(config.channel);
        _config.mix_mode = zego_audio_mix_mode(config.mixMode);
        return _config;
    }

    static zego_watermark O2IWatermark(const ZegoWatermark &watermark) {
        zego_watermark _watermark;
        _watermark.layout.left = watermark.layout.x;
        _watermark.layout.right = watermark.layout.x + watermark.layout.width;
        _watermark.layout.top = watermark.layout.y;
        _watermark.layout.bottom = watermark.layout.y + watermark.layout.height;
        strncpy(_watermark.image_url, watermark.imageURL.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
        return _watermark;
    }

    static zego_mixer_whiteboard O2IMixerWhiteboard(const ZegoMixerWhiteboard &whiteboard) {
        zego_mixer_whiteboard _whiteboard;
        _whiteboard.whiteboard_id = whiteboard.whiteboardID;
        _whiteboard.horizontal_ratio = whiteboard.horizontalRatio;
        _whiteboard.vertical_ratio = whiteboard.verticalRatio;
        _whiteboard.is_ppt_animation = whiteboard.isPPTAnimation;
        _whiteboard.layout.left = whiteboard.layout.x;
        _whiteboard.layout.right = whiteboard.layout.x + whiteboard.layout.width;
        _whiteboard.layout.top = whiteboard.layout.y;
        _whiteboard.layout.bottom = whiteboard.layout.y + whiteboard.layout.height;
        _whiteboard.z_order = whiteboard.zOrder;
        _whiteboard.background_color = whiteboard.backgroundColor;
        return _whiteboard;
    }

    static zego_custom_audio_process_config
    O2ICustomAudioProcessConfig(const ZegoCustomAudioProcessConfig &config) {
        zego_custom_audio_process_config _config;
        _config.channel = zego_audio_channel(config.channel);
        _config.sample_rate = zego_audio_sample_rate(config.sampleRate);
        _config.samples = config.samples;
        return _config;
    }

    static zego_sei_config O2SEIConfig(const ZegoSEIConfig &config) {
        zego_sei_config _config;
        _config.type = zego_sei_type(config.type);
        return _config;
    }

    static ZegoNetworkTimeInfo I2ONetworkTimeInfo(const zego_network_time_info &info) {
        ZegoNetworkTimeInfo oInfo;
        oInfo.timestamp = info.timestamp;
        oInfo.maxDeviation = info.max_deviation;

        return oInfo;
    }

    static zego_publisher_config O2IPublisherConfig(const ZegoPublisherConfig &config) {
        zego_publisher_config _config = {};
        strncpy(_config.room_id, config.roomID.c_str(), ZEGO_EXPRESS_MAX_ROOMID_LEN);
        _config.force_synchronous_network_time = config.forceSynchronousNetworkTime;
        int censorshipMode = (int)config.streamCensorshipMode;
        _config.stream_censorship_mode = (zego_stream_censorship_mode)censorshipMode;
        _config.stream_censor_flag = config.streamCensorFlag;
        _config.codec_negotiation_type =
            (zego_capability_negotiation_type)config.codecNegotiationType;
        return _config;
    }

    static zego_scene_publisher_config
    O2IZegoScenePublisherConfig(const ZegoScenePublisherConfig &config) {
        zego_scene_publisher_config _config;
        _config.range_scene_handle = config.rangeSceneHandle;
        return _config;
    }

    static ZegoPosition I2OZegoPosition(const zego_position &position) {
        ZegoPosition _position;
        for (int i = 0; i < 3; ++i) {
            _position.coordinate[i] = position.coordinate[i];
            _position.motionOrientation.axisForward[i] =
                position.motion_orientation.axis_forward[i];
            _position.motionOrientation.axisRight[i] = position.motion_orientation.axis_right[i];
            _position.motionOrientation.axisUp[i] = position.motion_orientation.axis_up[i];
            _position.cameraOrientation.axisForward[i] =
                position.camera_orientation.axis_forward[i];
            _position.cameraOrientation.axisRight[i] = position.camera_orientation.axis_right[i];
            _position.cameraOrientation.axisUp[i] = position.camera_orientation.axis_up[i];
        }
        return _position;
    }

    static zego_position O2IZegoPosition(const ZegoPosition &position) {
        zego_position _position;
        for (int i = 0; i < 3; ++i) {
            _position.coordinate[i] = position.coordinate[i];
            _position.motion_orientation.axis_forward[i] =
                position.motionOrientation.axisForward[i];
            _position.motion_orientation.axis_right[i] = position.motionOrientation.axisRight[i];
            _position.motion_orientation.axis_up[i] = position.motionOrientation.axisUp[i];
            _position.camera_orientation.axis_forward[i] =
                position.cameraOrientation.axisForward[i];
            _position.camera_orientation.axis_right[i] = position.cameraOrientation.axisRight[i];
            _position.camera_orientation.axis_up[i] = position.cameraOrientation.axisUp[i];
        }
        return _position;
    }

    static zego_scene_param O2IZegoSceneParam(const ZegoSceneParam &param) {
        zego_scene_param _param;
        memset(&_param, 0, sizeof(zego_scene_param));
        ZegoStrncpy(_param.user.user_id, param.user.userID.c_str(), ZEGO_EXPRESS_MAX_USERID_LEN);
        ZegoStrncpy(_param.user.user_name, param.user.userName.c_str(),
                    ZEGO_EXPRESS_MAX_USERNAME_LEN);
        ZegoStrncpy(_param.token, param.token.c_str(), ZEGO_EXPRESS_MAX_TOKEN_LEN);
        _param.scene_id = param.sceneID;
        _param.template_id = param.templateID;
        _param.broadcast_mode = static_cast<zego_broadcast_mode>(param.broadcastMode);
        _param.position = ZegoExpressConvert::O2IZegoPosition(param.position);
        return _param;
    }

    static ZegoSceneConfig I2OZegoSceneConfig(const zego_scene_config &config) {
        ZegoSceneConfig _config;
        _config.channelCount = config.channel_count;
        return _config;
    }

    static zego_item_param O2IZegoItemParam(const ZegoItemParam &param) {
        zego_item_param _param;
        memset(&_param, 0, sizeof(zego_item_param));
        _param.item_id = param.itemID;
        _param.capacity = param.capacity;
        _param.position = ZegoExpressConvert::O2IZegoPosition(param.position);
        _param.create_mode = static_cast<zego_create_item_mode>(param.createMode);
        return _param;
    }

    static zego_scene_stream_config O2IZegoSceneStreamConfig(const ZegoSceneStreamConfig &config) {
        zego_scene_stream_config _config;
        memset(&_config, 0, sizeof(zego_scene_stream_config));
        _config.enable_play_in_range = config.enablePlayInRange;
        _config.enable_publish_to_world = config.enablePublishToWorld;
        return _config;
    }

    static ZegoImageBuffer I2OImageBuffer(const zego_image_buffer &buffer) {
        ZegoImageBuffer oBuffer = {};
        oBuffer.width = buffer.width;
        oBuffer.height = buffer.height;
        oBuffer.length = buffer.length;
        if (buffer.length > 0) {
            oBuffer.buffer = new unsigned char[buffer.length]{0};
            memcpy(oBuffer.buffer, buffer.buffer, buffer.length);
        }
        return oBuffer;
    }

    static ZegoScreenCaptureSourceInfo
    I2OScreenCaptureSource(const zego_screen_capture_source_info &info) {
        ZegoScreenCaptureSourceInfo oInfo = {};
        oInfo.sourceType = (ZegoScreenCaptureSourceType)info.source_type;
        oInfo.sourceID = info.source_id;
        oInfo.sourceName = info.source_name;
        oInfo.thumbnailImage = I2OImageBuffer(info.thumbnail_image);
        oInfo.iconImage = I2OImageBuffer(info.icon_image);
        return oInfo;
    }

    static zego_media_player_resource
    O2IMediaPlayerResource(const ZegoMediaPlayerResource &resource) {
        zego_media_player_resource oResource;
        memset(&oResource, 0, sizeof(zego_media_player_resource));
        oResource.load_type = static_cast<zego_multimedia_load_type>(resource.loadType);
        oResource.start_position = resource.startPosition;
        oResource.alpha_layout = static_cast<zego_alpha_layout_type>(resource.alphaLayout);
        ZegoStrncpy(oResource.file_path, resource.filePath.c_str(), ZEGO_EXPRESS_MAX_URL_LEN);
        oResource.memory = resource.memory;
        oResource.memory_length = resource.memoryLength;
        ZegoStrncpy(oResource.resource_id, resource.resourceID.c_str(),
                    ZEGO_EXPRESS_MAX_COMMON_LEN);

        return oResource;
    }

    static ZegoMediaSideInfo I2OMediaSideInfo(const zego_media_side_info &_info) {
        ZegoMediaSideInfo info;
        info.streamID = _info.stream_id;
        info.SEIData = _info.sei_data;
        info.SEIDataLength = _info.sei_data_length;
        info.timestampNs = _info.timestamp_ns;
        return info;
    }
};

} // namespace EXPRESS
} // namespace ZEGO
