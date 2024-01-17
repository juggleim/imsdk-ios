// Generated by the protocol buffer compiler.  DO NOT EDIT!
// clang-format off
// source: usermessages.proto

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

@class BanUser;
@class UserOnlineItem;

NS_ASSUME_NONNULL_BEGIN

#pragma mark - Enum BanType

typedef GPB_ENUM(BanType) {
  /**
   * Value used if any message's field encounters a value that is not defined
   * by this enum. The message will also have C functions to get/set the rawValue
   * of the field.
   **/
  BanType_GPBUnrecognizedEnumeratorValue = kGPBUnrecognizedEnumeratorValue,
  BanType_Permanent = 0,
  BanType_Temporary = 1,
};

GPBEnumDescriptor *BanType_EnumDescriptor(void);

/**
 * Checks to see if the given value is defined by the enum or was not known at
 * the time this source was generated.
 **/
BOOL BanType_IsValidValue(int32_t value);

#pragma mark - UsermessagesRoot

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
GPB_FINAL @interface UsermessagesRoot : GPBRootObject
@end

#pragma mark - UserOnlineStatusReq

typedef GPB_ENUM(UserOnlineStatusReq_FieldNumber) {
  UserOnlineStatusReq_FieldNumber_UserIdsArray = 1,
};

GPB_FINAL @interface UserOnlineStatusReq : GPBMessage

@property(nonatomic, readwrite, strong, null_resettable) NSMutableArray<NSString*> *userIdsArray;
/** The number of items in @c userIdsArray without causing the container to be created. */
@property(nonatomic, readonly) NSUInteger userIdsArray_Count;

@end

#pragma mark - UserOnlineStatusResp

typedef GPB_ENUM(UserOnlineStatusResp_FieldNumber) {
  UserOnlineStatusResp_FieldNumber_ItemsArray = 1,
};

GPB_FINAL @interface UserOnlineStatusResp : GPBMessage

@property(nonatomic, readwrite, strong, null_resettable) NSMutableArray<UserOnlineItem*> *itemsArray;
/** The number of items in @c itemsArray without causing the container to be created. */
@property(nonatomic, readonly) NSUInteger itemsArray_Count;

@end

#pragma mark - UserOnlineItem

typedef GPB_ENUM(UserOnlineItem_FieldNumber) {
  UserOnlineItem_FieldNumber_UserId = 1,
  UserOnlineItem_FieldNumber_IsOnline = 2,
};

GPB_FINAL @interface UserOnlineItem : GPBMessage

@property(nonatomic, readwrite, copy, null_resettable) NSString *userId;

@property(nonatomic, readwrite) BOOL isOnline;

@end

#pragma mark - BanUsersReq

typedef GPB_ENUM(BanUsersReq_FieldNumber) {
  BanUsersReq_FieldNumber_UserIdsArray = 1,
  BanUsersReq_FieldNumber_BanType = 2,
  BanUsersReq_FieldNumber_EndTime = 3,
  BanUsersReq_FieldNumber_IsAdd = 4,
};

GPB_FINAL @interface BanUsersReq : GPBMessage

@property(nonatomic, readwrite, strong, null_resettable) NSMutableArray<NSString*> *userIdsArray;
/** The number of items in @c userIdsArray without causing the container to be created. */
@property(nonatomic, readonly) NSUInteger userIdsArray_Count;

@property(nonatomic, readwrite) BanType banType;

@property(nonatomic, readwrite) int64_t endTime;

@property(nonatomic, readwrite) BOOL isAdd;

@end

/**
 * Fetches the raw value of a @c BanUsersReq's @c banType property, even
 * if the value was not defined by the enum at the time the code was generated.
 **/
int32_t BanUsersReq_BanType_RawValue(BanUsersReq *message);
/**
 * Sets the raw value of an @c BanUsersReq's @c banType property, allowing
 * it to be set to a value that was not defined by the enum at the time the code
 * was generated.
 **/
void SetBanUsersReq_BanType_RawValue(BanUsersReq *message, int32_t value);

#pragma mark - BanUser

typedef GPB_ENUM(BanUser_FieldNumber) {
  BanUser_FieldNumber_UserId = 1,
  BanUser_FieldNumber_BanType = 2,
  BanUser_FieldNumber_EndTime = 3,
  BanUser_FieldNumber_CreatedTime = 4,
};

GPB_FINAL @interface BanUser : GPBMessage

@property(nonatomic, readwrite, copy, null_resettable) NSString *userId;

@property(nonatomic, readwrite) BanType banType;

@property(nonatomic, readwrite) int64_t endTime;

@property(nonatomic, readwrite) int64_t createdTime;

@end

/**
 * Fetches the raw value of a @c BanUser's @c banType property, even
 * if the value was not defined by the enum at the time the code was generated.
 **/
int32_t BanUser_BanType_RawValue(BanUser *message);
/**
 * Sets the raw value of an @c BanUser's @c banType property, allowing
 * it to be set to a value that was not defined by the enum at the time the code
 * was generated.
 **/
void SetBanUser_BanType_RawValue(BanUser *message, int32_t value);

#pragma mark - QryBanUsersReq

typedef GPB_ENUM(QryBanUsersReq_FieldNumber) {
  QryBanUsersReq_FieldNumber_Limit = 1,
  QryBanUsersReq_FieldNumber_Offset = 2,
};

GPB_FINAL @interface QryBanUsersReq : GPBMessage

@property(nonatomic, readwrite) int64_t limit;

@property(nonatomic, readwrite, copy, null_resettable) NSString *offset;

@end

#pragma mark - QryBanUsersResp

typedef GPB_ENUM(QryBanUsersResp_FieldNumber) {
  QryBanUsersResp_FieldNumber_ItemsArray = 1,
  QryBanUsersResp_FieldNumber_Offset = 2,
};

GPB_FINAL @interface QryBanUsersResp : GPBMessage

@property(nonatomic, readwrite, strong, null_resettable) NSMutableArray<BanUser*> *itemsArray;
/** The number of items in @c itemsArray without causing the container to be created. */
@property(nonatomic, readonly) NSUInteger itemsArray_Count;

@property(nonatomic, readwrite, copy, null_resettable) NSString *offset;

@end

NS_ASSUME_NONNULL_END

CF_EXTERN_C_END

#pragma clang diagnostic pop

// @@protoc_insertion_point(global_scope)

// clang-format on