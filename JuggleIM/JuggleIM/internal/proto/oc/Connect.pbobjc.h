// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// clang-format off
// source: connect.proto

// This CPP symbol can be defined to use imports that match up to the framework
// imports needed when using CocoaPods.
#if !defined(GPB_USE_PROTOBUF_FRAMEWORK_IMPORTS)
 #define GPB_USE_PROTOBUF_FRAMEWORK_IMPORTS 0
#endif

#if GPB_USE_PROTOBUF_FRAMEWORK_IMPORTS
 #import <Protobuf/GPBProtocolBuffers.h>
#else
 #import "GPBProtocolBuffers.h"
#endif

#if GOOGLE_PROTOBUF_OBJC_VERSION < 30007
#error This file was generated by a newer version of protoc which is incompatible with your Protocol Buffer library sources.
#endif
#if 30007 < GOOGLE_PROTOBUF_OBJC_MIN_SUPPORTED_VERSION
#error This file was generated by an older version of protoc which is incompatible with your Protocol Buffer library sources.
#endif

// @@protoc_insertion_point(imports)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

CF_EXTERN_C_BEGIN

@class ConnectAckMsgBody;
@class ConnectMsgBody;
@class DisconnectMsgBody;
@class PublishAckMsgBody;
@class PublishMsgBody;
@class QueryAckMsgBody;
@class QueryConfirmMsgBody;
@class QueryMsgBody;

NS_ASSUME_NONNULL_BEGIN

#pragma mark - ConnectRoot

/**
 * Exposes the extension registry for this file.
 *
 * The base class provides:
 * @code
 *   + (GPBExtensionRegistry *)extensionRegistry;
 * @endcode
 * which is a @c GPBExtensionRegistry that includes all the extensions defined by
 * this file and all files that it depends on.
 **/
GPB_FINAL @interface ConnectRoot : GPBRootObject
@end

#pragma mark - ConnectMsgBody

typedef GPB_ENUM(ConnectMsgBody_FieldNumber) {
  ConnectMsgBody_FieldNumber_ProtoId = 1,
  ConnectMsgBody_FieldNumber_SdkVersion = 2,
  ConnectMsgBody_FieldNumber_Appkey = 3,
  ConnectMsgBody_FieldNumber_Token = 4,
  ConnectMsgBody_FieldNumber_DeviceId = 5,
  ConnectMsgBody_FieldNumber_Platform = 6,
  ConnectMsgBody_FieldNumber_DeviceCompany = 7,
  ConnectMsgBody_FieldNumber_DeviceModel = 8,
  ConnectMsgBody_FieldNumber_DeviceOsVersion = 9,
  ConnectMsgBody_FieldNumber_PushToken = 10,
  ConnectMsgBody_FieldNumber_NetworkId = 11,
  ConnectMsgBody_FieldNumber_IspNum = 12,
  ConnectMsgBody_FieldNumber_ClientIp = 13,
  ConnectMsgBody_FieldNumber_PackageName = 14,
  ConnectMsgBody_FieldNumber_PushChannel = 15,
};

GPB_FINAL @interface ConnectMsgBody : GPBMessage

@property(nonatomic, readwrite, copy, null_resettable) NSString *protoId;

/** sdk 版本 */
@property(nonatomic, readwrite, copy, null_resettable) NSString *sdkVersion;

@property(nonatomic, readwrite, copy, null_resettable) NSString *appkey;

@property(nonatomic, readwrite, copy, null_resettable) NSString *token;

/** 设备信息 */
@property(nonatomic, readwrite, copy, null_resettable) NSString *deviceId;

/** iOS/Android/iPad */
@property(nonatomic, readwrite, copy, null_resettable) NSString *platform;

/** 设备厂商, huawei/apple */
@property(nonatomic, readwrite, copy, null_resettable) NSString *deviceCompany;

/** 设备型号  例如: iPhone8 */
@property(nonatomic, readwrite, copy, null_resettable) NSString *deviceModel;

/** 手机操作系统版本 */
@property(nonatomic, readwrite, copy, null_resettable) NSString *deviceOsVersion;

/** 厂商推送Token */
@property(nonatomic, readwrite, copy, null_resettable) NSString *pushToken;

/** 网络 */
@property(nonatomic, readwrite, copy, null_resettable) NSString *networkId;

/** 网络运营商编号 */
@property(nonatomic, readwrite, copy, null_resettable) NSString *ispNum;

@property(nonatomic, readwrite, copy, null_resettable) NSString *clientIp;

@property(nonatomic, readwrite, copy, null_resettable) NSString *packageName;

@property(nonatomic, readwrite, copy, null_resettable) NSString *pushChannel;

@end

#pragma mark - ConnectAckMsgBody

typedef GPB_ENUM(ConnectAckMsgBody_FieldNumber) {
  ConnectAckMsgBody_FieldNumber_Code = 1,
  ConnectAckMsgBody_FieldNumber_UserId = 2,
  ConnectAckMsgBody_FieldNumber_Session = 3,
  ConnectAckMsgBody_FieldNumber_Timestamp = 4,
  ConnectAckMsgBody_FieldNumber_Ext = 5,
};

GPB_FINAL @interface ConnectAckMsgBody : GPBMessage

@property(nonatomic, readwrite) int32_t code;

@property(nonatomic, readwrite, copy, null_resettable) NSString *userId;

@property(nonatomic, readwrite, copy, null_resettable) NSString *session;

@property(nonatomic, readwrite) int64_t timestamp;

@property(nonatomic, readwrite, copy, null_resettable) NSString *ext;

@end

#pragma mark - DisconnectMsgBody

typedef GPB_ENUM(DisconnectMsgBody_FieldNumber) {
  DisconnectMsgBody_FieldNumber_Code = 1,
  DisconnectMsgBody_FieldNumber_Timestamp = 2,
  DisconnectMsgBody_FieldNumber_Ext = 3,
};

GPB_FINAL @interface DisconnectMsgBody : GPBMessage

@property(nonatomic, readwrite) int32_t code;

@property(nonatomic, readwrite) int64_t timestamp;

@property(nonatomic, readwrite, copy, null_resettable) NSString *ext;

@end

#pragma mark - PublishMsgBody

typedef GPB_ENUM(PublishMsgBody_FieldNumber) {
  PublishMsgBody_FieldNumber_Index = 1,
  PublishMsgBody_FieldNumber_Topic = 2,
  PublishMsgBody_FieldNumber_TargetId = 3,
  PublishMsgBody_FieldNumber_Timestamp = 4,
  PublishMsgBody_FieldNumber_Data_p = 5,
};

GPB_FINAL @interface PublishMsgBody : GPBMessage

@property(nonatomic, readwrite) int32_t index;

@property(nonatomic, readwrite, copy, null_resettable) NSString *topic;

@property(nonatomic, readwrite, copy, null_resettable) NSString *targetId;

@property(nonatomic, readwrite) int64_t timestamp;

@property(nonatomic, readwrite, copy, null_resettable) NSData *data_p;

@end

#pragma mark - PublishAckMsgBody

typedef GPB_ENUM(PublishAckMsgBody_FieldNumber) {
  PublishAckMsgBody_FieldNumber_Index = 1,
  PublishAckMsgBody_FieldNumber_Code = 2,
  PublishAckMsgBody_FieldNumber_MsgId = 3,
  PublishAckMsgBody_FieldNumber_Timestamp = 4,
  PublishAckMsgBody_FieldNumber_MsgSeqNo = 5,
  PublishAckMsgBody_FieldNumber_MemberCount = 6,
  PublishAckMsgBody_FieldNumber_ClientMsgId = 7,
};

GPB_FINAL @interface PublishAckMsgBody : GPBMessage

@property(nonatomic, readwrite) int32_t index;

@property(nonatomic, readwrite) int32_t code;

@property(nonatomic, readwrite, copy, null_resettable) NSString *msgId;

@property(nonatomic, readwrite) int64_t timestamp;

@property(nonatomic, readwrite) int64_t msgSeqNo;

@property(nonatomic, readwrite) int32_t memberCount;

@property(nonatomic, readwrite, copy, null_resettable) NSString *clientMsgId;

@end

#pragma mark - QueryMsgBody

typedef GPB_ENUM(QueryMsgBody_FieldNumber) {
  QueryMsgBody_FieldNumber_Index = 1,
  QueryMsgBody_FieldNumber_Topic = 2,
  QueryMsgBody_FieldNumber_TargetId = 3,
  QueryMsgBody_FieldNumber_Timestamp = 4,
  QueryMsgBody_FieldNumber_Data_p = 5,
};

GPB_FINAL @interface QueryMsgBody : GPBMessage

@property(nonatomic, readwrite) int32_t index;

@property(nonatomic, readwrite, copy, null_resettable) NSString *topic;

@property(nonatomic, readwrite, copy, null_resettable) NSString *targetId;

@property(nonatomic, readwrite) int64_t timestamp;

@property(nonatomic, readwrite, copy, null_resettable) NSData *data_p;

@end

#pragma mark - QueryAckMsgBody

typedef GPB_ENUM(QueryAckMsgBody_FieldNumber) {
  QueryAckMsgBody_FieldNumber_Index = 1,
  QueryAckMsgBody_FieldNumber_Code = 2,
  QueryAckMsgBody_FieldNumber_Timestamp = 3,
  QueryAckMsgBody_FieldNumber_Data_p = 4,
};

GPB_FINAL @interface QueryAckMsgBody : GPBMessage

@property(nonatomic, readwrite) int32_t index;

@property(nonatomic, readwrite) int32_t code;

@property(nonatomic, readwrite) int64_t timestamp;

@property(nonatomic, readwrite, copy, null_resettable) NSData *data_p;

@end

#pragma mark - QueryConfirmMsgBody

typedef GPB_ENUM(QueryConfirmMsgBody_FieldNumber) {
  QueryConfirmMsgBody_FieldNumber_Index = 1,
};

GPB_FINAL @interface QueryConfirmMsgBody : GPBMessage

@property(nonatomic, readwrite) int32_t index;

@end

#pragma mark - ImWebsocketMsg

typedef GPB_ENUM(ImWebsocketMsg_FieldNumber) {
  ImWebsocketMsg_FieldNumber_Version = 1,
  ImWebsocketMsg_FieldNumber_Cmd = 2,
  ImWebsocketMsg_FieldNumber_Qos = 3,
  ImWebsocketMsg_FieldNumber_Payload = 4,
  ImWebsocketMsg_FieldNumber_ConnectMsgBody = 11,
  ImWebsocketMsg_FieldNumber_ConnectAckMsgBody = 12,
  ImWebsocketMsg_FieldNumber_DisconnectMsgBody = 13,
  ImWebsocketMsg_FieldNumber_PublishMsgBody = 14,
  ImWebsocketMsg_FieldNumber_PubAckMsgBody = 15,
  ImWebsocketMsg_FieldNumber_QryMsgBody = 16,
  ImWebsocketMsg_FieldNumber_QryAckMsgBody = 17,
  ImWebsocketMsg_FieldNumber_QryConfirmMsgBody = 18,
};

typedef GPB_ENUM(ImWebsocketMsg_Testof_OneOfCase) {
  ImWebsocketMsg_Testof_OneOfCase_GPBUnsetOneOfCase = 0,
  ImWebsocketMsg_Testof_OneOfCase_ConnectMsgBody = 11,
  ImWebsocketMsg_Testof_OneOfCase_ConnectAckMsgBody = 12,
  ImWebsocketMsg_Testof_OneOfCase_DisconnectMsgBody = 13,
  ImWebsocketMsg_Testof_OneOfCase_PublishMsgBody = 14,
  ImWebsocketMsg_Testof_OneOfCase_PubAckMsgBody = 15,
  ImWebsocketMsg_Testof_OneOfCase_QryMsgBody = 16,
  ImWebsocketMsg_Testof_OneOfCase_QryAckMsgBody = 17,
  ImWebsocketMsg_Testof_OneOfCase_QryConfirmMsgBody = 18,
};

GPB_FINAL @interface ImWebsocketMsg : GPBMessage

@property(nonatomic, readwrite) int32_t version;

@property(nonatomic, readwrite) int32_t cmd;

@property(nonatomic, readwrite) int32_t qos;

@property(nonatomic, readwrite, copy, null_resettable) NSData *payload;

@property(nonatomic, readonly) ImWebsocketMsg_Testof_OneOfCase testofOneOfCase;

@property(nonatomic, readwrite, strong, null_resettable) ConnectMsgBody *connectMsgBody;

@property(nonatomic, readwrite, strong, null_resettable) ConnectAckMsgBody *connectAckMsgBody;

@property(nonatomic, readwrite, strong, null_resettable) DisconnectMsgBody *disconnectMsgBody;

@property(nonatomic, readwrite, strong, null_resettable) PublishMsgBody *publishMsgBody;

@property(nonatomic, readwrite, strong, null_resettable) PublishAckMsgBody *pubAckMsgBody;

@property(nonatomic, readwrite, strong, null_resettable) QueryMsgBody *qryMsgBody;

@property(nonatomic, readwrite, strong, null_resettable) QueryAckMsgBody *qryAckMsgBody;

@property(nonatomic, readwrite, strong, null_resettable) QueryConfirmMsgBody *qryConfirmMsgBody;

@end

/**
 * Clears whatever value was set for the oneof 'testof'.
 **/
void ImWebsocketMsg_ClearTestofOneOfCase(ImWebsocketMsg *message);

NS_ASSUME_NONNULL_END

CF_EXTERN_C_END

#pragma clang diagnostic pop

// @@protoc_insertion_point(global_scope)

// clang-format on
