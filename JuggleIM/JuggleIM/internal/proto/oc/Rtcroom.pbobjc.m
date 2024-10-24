// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// clang-format off
// source: rtcroom.proto

// This CPP symbol can be defined to use imports that match up to the framework
// imports needed when using CocoaPods.
#if !defined(GPB_USE_PROTOBUF_FRAMEWORK_IMPORTS)
 #define GPB_USE_PROTOBUF_FRAMEWORK_IMPORTS 0
#endif

#if GPB_USE_PROTOBUF_FRAMEWORK_IMPORTS
 #import <Protobuf/GPBProtocolBuffers_RuntimeSupport.h>
#else
 #import "GPBProtocolBuffers_RuntimeSupport.h"
#endif

#if GOOGLE_PROTOBUF_OBJC_VERSION < 30007
#error This file was generated by a newer version of protoc which is incompatible with your Protocol Buffer library sources.
#endif
#if 30007 < GOOGLE_PROTOBUF_OBJC_MIN_SUPPORTED_VERSION
#error This file was generated by an older version of protoc which is incompatible with your Protocol Buffer library sources.
#endif

#import <stdatomic.h>

#import "Rtcroom.pbobjc.h"
// @@protoc_insertion_point(imports)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"

#pragma mark - Objective-C Class declarations
// Forward declarations of Objective-C classes that we can use as
// static values in struct initializers.
// We don't use [Foo class] because it is not a static value.
GPBObjCClassDeclaration(RtcAnswerReq);
GPBObjCClassDeclaration(RtcInviteEvent);
GPBObjCClassDeclaration(RtcInviteReq);
GPBObjCClassDeclaration(RtcMember);
GPBObjCClassDeclaration(RtcMemberRoom);
GPBObjCClassDeclaration(RtcMemberRooms);
GPBObjCClassDeclaration(RtcRoom);
GPBObjCClassDeclaration(RtcRoomEvent);
GPBObjCClassDeclaration(RtcRoomReq);
GPBObjCClassDeclaration(UserInfo);

#pragma mark - RtcroomRoot

@implementation RtcroomRoot

// No extensions in the file and no imports or none of the imports (direct or
// indirect) defined extensions, so no need to generate +extensionRegistry.

@end

static GPBFileDescription RtcroomRoot_FileDescription = {
  .package = NULL,
  .prefix = NULL,
  .syntax = GPBFileSyntaxProto3
};

#pragma mark - Enum RtcRoomType

GPBEnumDescriptor *RtcRoomType_EnumDescriptor(void) {
  static _Atomic(GPBEnumDescriptor*) descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static const char *valueNames =
        "OneOne\000OneMore\000";
    static const int32_t values[] = {
        RtcRoomType_OneOne,
        RtcRoomType_OneMore,
    };
    static const char *extraTextFormatInfo = "\002\000\006\000\001\007\000";
    GPBEnumDescriptor *worker =
        [GPBEnumDescriptor allocDescriptorForName:GPBNSStringifySymbol(RtcRoomType)
                                       valueNames:valueNames
                                           values:values
                                            count:(uint32_t)(sizeof(values) / sizeof(int32_t))
                                     enumVerifier:RtcRoomType_IsValidValue
                                            flags:GPBEnumDescriptorInitializationFlag_None
                              extraTextFormatInfo:extraTextFormatInfo];
    GPBEnumDescriptor *expected = nil;
    if (!atomic_compare_exchange_strong(&descriptor, &expected, worker)) {
      [worker release];
    }
  }
  return descriptor;
}

BOOL RtcRoomType_IsValidValue(int32_t value__) {
  switch (value__) {
    case RtcRoomType_OneOne:
    case RtcRoomType_OneMore:
      return YES;
    default:
      return NO;
  }
}

#pragma mark - Enum RtcState

GPBEnumDescriptor *RtcState_EnumDescriptor(void) {
  static _Atomic(GPBEnumDescriptor*) descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static const char *valueNames =
        "RtcStateDefault\000RtcIncoming\000RtcOutgoing\000"
        "RtcConnecting\000RtcConnected\000";
    static const int32_t values[] = {
        RtcState_RtcStateDefault,
        RtcState_RtcIncoming,
        RtcState_RtcOutgoing,
        RtcState_RtcConnecting,
        RtcState_RtcConnected,
    };
    static const char *extraTextFormatInfo = "\005\000\017\000\001\013\000\002\013\000\003\r\000\004\014\000";
    GPBEnumDescriptor *worker =
        [GPBEnumDescriptor allocDescriptorForName:GPBNSStringifySymbol(RtcState)
                                       valueNames:valueNames
                                           values:values
                                            count:(uint32_t)(sizeof(values) / sizeof(int32_t))
                                     enumVerifier:RtcState_IsValidValue
                                            flags:GPBEnumDescriptorInitializationFlag_None
                              extraTextFormatInfo:extraTextFormatInfo];
    GPBEnumDescriptor *expected = nil;
    if (!atomic_compare_exchange_strong(&descriptor, &expected, worker)) {
      [worker release];
    }
  }
  return descriptor;
}

BOOL RtcState_IsValidValue(int32_t value__) {
  switch (value__) {
    case RtcState_RtcStateDefault:
    case RtcState_RtcIncoming:
    case RtcState_RtcOutgoing:
    case RtcState_RtcConnecting:
    case RtcState_RtcConnected:
      return YES;
    default:
      return NO;
  }
}

#pragma mark - Enum RtcRoomEventType

GPBEnumDescriptor *RtcRoomEventType_EnumDescriptor(void) {
  static _Atomic(GPBEnumDescriptor*) descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static const char *valueNames =
        "DefaultRtcRoomEvent\000RtcJoin\000RtcQuit\000RtcD"
        "estroy\000RtcStateChg\000";
    static const int32_t values[] = {
        RtcRoomEventType_DefaultRtcRoomEvent,
        RtcRoomEventType_RtcJoin,
        RtcRoomEventType_RtcQuit,
        RtcRoomEventType_RtcDestroy,
        RtcRoomEventType_RtcStateChg,
    };
    static const char *extraTextFormatInfo = "\005\000\023\000\001\007\000\002\007\000\003\n\000\004\013\000";
    GPBEnumDescriptor *worker =
        [GPBEnumDescriptor allocDescriptorForName:GPBNSStringifySymbol(RtcRoomEventType)
                                       valueNames:valueNames
                                           values:values
                                            count:(uint32_t)(sizeof(values) / sizeof(int32_t))
                                     enumVerifier:RtcRoomEventType_IsValidValue
                                            flags:GPBEnumDescriptorInitializationFlag_None
                              extraTextFormatInfo:extraTextFormatInfo];
    GPBEnumDescriptor *expected = nil;
    if (!atomic_compare_exchange_strong(&descriptor, &expected, worker)) {
      [worker release];
    }
  }
  return descriptor;
}

BOOL RtcRoomEventType_IsValidValue(int32_t value__) {
  switch (value__) {
    case RtcRoomEventType_DefaultRtcRoomEvent:
    case RtcRoomEventType_RtcJoin:
    case RtcRoomEventType_RtcQuit:
    case RtcRoomEventType_RtcDestroy:
    case RtcRoomEventType_RtcStateChg:
      return YES;
    default:
      return NO;
  }
}

#pragma mark - Enum InviteType

GPBEnumDescriptor *InviteType_EnumDescriptor(void) {
  static _Atomic(GPBEnumDescriptor*) descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static const char *valueNames =
        "DefaultInviteType\000RtcInvite\000RtcAccept\000Rt"
        "cDecline\000RtcCancel\000RtcTimeout\000";
    static const int32_t values[] = {
        InviteType_DefaultInviteType,
        InviteType_RtcInvite,
        InviteType_RtcAccept,
        InviteType_RtcDecline,
        InviteType_RtcCancel,
        InviteType_RtcTimeout,
    };
    static const char *extraTextFormatInfo = "\006\000\021\000\001\t\000\002\t\000\003\n\000\004\t\000\005\n\000";
    GPBEnumDescriptor *worker =
        [GPBEnumDescriptor allocDescriptorForName:GPBNSStringifySymbol(InviteType)
                                       valueNames:valueNames
                                           values:values
                                            count:(uint32_t)(sizeof(values) / sizeof(int32_t))
                                     enumVerifier:InviteType_IsValidValue
                                            flags:GPBEnumDescriptorInitializationFlag_None
                              extraTextFormatInfo:extraTextFormatInfo];
    GPBEnumDescriptor *expected = nil;
    if (!atomic_compare_exchange_strong(&descriptor, &expected, worker)) {
      [worker release];
    }
  }
  return descriptor;
}

BOOL InviteType_IsValidValue(int32_t value__) {
  switch (value__) {
    case InviteType_DefaultInviteType:
    case InviteType_RtcInvite:
    case InviteType_RtcAccept:
    case InviteType_RtcDecline:
    case InviteType_RtcCancel:
    case InviteType_RtcTimeout:
      return YES;
    default:
      return NO;
  }
}

#pragma mark - RtcRoomReq

@implementation RtcRoomReq

@dynamic roomType;
@dynamic roomId;
@dynamic hasJoinMember, joinMember;

typedef struct RtcRoomReq__storage_ {
  uint32_t _has_storage_[1];
  RtcRoomType roomType;
  NSString *roomId;
  RtcMember *joinMember;
} RtcRoomReq__storage_;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "roomType",
        .dataTypeSpecific.enumDescFunc = RtcRoomType_EnumDescriptor,
        .number = RtcRoomReq_FieldNumber_RoomType,
        .hasIndex = 0,
        .offset = (uint32_t)offsetof(RtcRoomReq__storage_, roomType),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldHasEnumDescriptor | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeEnum,
      },
      {
        .name = "roomId",
        .dataTypeSpecific.clazz = Nil,
        .number = RtcRoomReq_FieldNumber_RoomId,
        .hasIndex = 1,
        .offset = (uint32_t)offsetof(RtcRoomReq__storage_, roomId),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeString,
      },
      {
        .name = "joinMember",
        .dataTypeSpecific.clazz = GPBObjCClass(RtcMember),
        .number = RtcRoomReq_FieldNumber_JoinMember,
        .hasIndex = 2,
        .offset = (uint32_t)offsetof(RtcRoomReq__storage_, joinMember),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom),
        .dataType = GPBDataTypeMessage,
      },
    };
    GPBDescriptor *localDescriptor =
        [GPBDescriptor allocDescriptorForClass:GPBObjCClass(RtcRoomReq)
                                   messageName:@"RtcRoomReq"
                               fileDescription:&RtcroomRoot_FileDescription
                                        fields:fields
                                    fieldCount:(uint32_t)(sizeof(fields) / sizeof(GPBMessageFieldDescription))
                                   storageSize:sizeof(RtcRoomReq__storage_)
                                         flags:(GPBDescriptorInitializationFlags)(GPBDescriptorInitializationFlag_UsesClassRefs | GPBDescriptorInitializationFlag_Proto3OptionalKnown | GPBDescriptorInitializationFlag_ClosedEnumSupportKnown)];
    #if !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
      static const char *extraTextFormatInfo =
        "\003\001\010\000\002\006\000\003\n\000";
      [localDescriptor setupExtraTextInfo:extraTextFormatInfo];
    #endif  // !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
    #if defined(DEBUG) && DEBUG
      NSAssert(descriptor == nil, @"Startup recursed!");
    #endif  // DEBUG
    descriptor = localDescriptor;
  }
  return descriptor;
}

@end

int32_t RtcRoomReq_RoomType_RawValue(RtcRoomReq *message) {
  GPBDescriptor *descriptor = [RtcRoomReq descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RtcRoomReq_FieldNumber_RoomType];
  return GPBGetMessageRawEnumField(message, field);
}

void SetRtcRoomReq_RoomType_RawValue(RtcRoomReq *message, int32_t value) {
  GPBDescriptor *descriptor = [RtcRoomReq descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RtcRoomReq_FieldNumber_RoomType];
  GPBSetMessageRawEnumField(message, field, value);
}

#pragma mark - RtcRoom

@implementation RtcRoom

@dynamic roomType;
@dynamic roomId;
@dynamic hasOwner, owner;
@dynamic membersArray, membersArray_Count;

typedef struct RtcRoom__storage_ {
  uint32_t _has_storage_[1];
  RtcRoomType roomType;
  NSString *roomId;
  UserInfo *owner;
  NSMutableArray *membersArray;
} RtcRoom__storage_;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "roomType",
        .dataTypeSpecific.enumDescFunc = RtcRoomType_EnumDescriptor,
        .number = RtcRoom_FieldNumber_RoomType,
        .hasIndex = 0,
        .offset = (uint32_t)offsetof(RtcRoom__storage_, roomType),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldHasEnumDescriptor | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeEnum,
      },
      {
        .name = "roomId",
        .dataTypeSpecific.clazz = Nil,
        .number = RtcRoom_FieldNumber_RoomId,
        .hasIndex = 1,
        .offset = (uint32_t)offsetof(RtcRoom__storage_, roomId),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeString,
      },
      {
        .name = "owner",
        .dataTypeSpecific.clazz = GPBObjCClass(UserInfo),
        .number = RtcRoom_FieldNumber_Owner,
        .hasIndex = 2,
        .offset = (uint32_t)offsetof(RtcRoom__storage_, owner),
        .flags = GPBFieldOptional,
        .dataType = GPBDataTypeMessage,
      },
      {
        .name = "membersArray",
        .dataTypeSpecific.clazz = GPBObjCClass(RtcMember),
        .number = RtcRoom_FieldNumber_MembersArray,
        .hasIndex = GPBNoHasBit,
        .offset = (uint32_t)offsetof(RtcRoom__storage_, membersArray),
        .flags = GPBFieldRepeated,
        .dataType = GPBDataTypeMessage,
      },
    };
    GPBDescriptor *localDescriptor =
        [GPBDescriptor allocDescriptorForClass:GPBObjCClass(RtcRoom)
                                   messageName:@"RtcRoom"
                               fileDescription:&RtcroomRoot_FileDescription
                                        fields:fields
                                    fieldCount:(uint32_t)(sizeof(fields) / sizeof(GPBMessageFieldDescription))
                                   storageSize:sizeof(RtcRoom__storage_)
                                         flags:(GPBDescriptorInitializationFlags)(GPBDescriptorInitializationFlag_UsesClassRefs | GPBDescriptorInitializationFlag_Proto3OptionalKnown | GPBDescriptorInitializationFlag_ClosedEnumSupportKnown)];
    #if !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
      static const char *extraTextFormatInfo =
        "\002\001\010\000\002\006\000";
      [localDescriptor setupExtraTextInfo:extraTextFormatInfo];
    #endif  // !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
    #if defined(DEBUG) && DEBUG
      NSAssert(descriptor == nil, @"Startup recursed!");
    #endif  // DEBUG
    descriptor = localDescriptor;
  }
  return descriptor;
}

@end

int32_t RtcRoom_RoomType_RawValue(RtcRoom *message) {
  GPBDescriptor *descriptor = [RtcRoom descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RtcRoom_FieldNumber_RoomType];
  return GPBGetMessageRawEnumField(message, field);
}

void SetRtcRoom_RoomType_RawValue(RtcRoom *message, int32_t value) {
  GPBDescriptor *descriptor = [RtcRoom descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RtcRoom_FieldNumber_RoomType];
  GPBSetMessageRawEnumField(message, field, value);
}

#pragma mark - RtcMember

@implementation RtcMember

@dynamic hasMember, member;
@dynamic rtcState;
@dynamic callTime;
@dynamic connectTime;
@dynamic hangupTime;
@dynamic hasInviter, inviter;

typedef struct RtcMember__storage_ {
  uint32_t _has_storage_[1];
  RtcState rtcState;
  UserInfo *member;
  UserInfo *inviter;
  int64_t callTime;
  int64_t connectTime;
  int64_t hangupTime;
} RtcMember__storage_;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "member",
        .dataTypeSpecific.clazz = GPBObjCClass(UserInfo),
        .number = RtcMember_FieldNumber_Member,
        .hasIndex = 0,
        .offset = (uint32_t)offsetof(RtcMember__storage_, member),
        .flags = GPBFieldOptional,
        .dataType = GPBDataTypeMessage,
      },
      {
        .name = "rtcState",
        .dataTypeSpecific.enumDescFunc = RtcState_EnumDescriptor,
        .number = RtcMember_FieldNumber_RtcState,
        .hasIndex = 1,
        .offset = (uint32_t)offsetof(RtcMember__storage_, rtcState),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldHasEnumDescriptor | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeEnum,
      },
      {
        .name = "callTime",
        .dataTypeSpecific.clazz = Nil,
        .number = RtcMember_FieldNumber_CallTime,
        .hasIndex = 2,
        .offset = (uint32_t)offsetof(RtcMember__storage_, callTime),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeInt64,
      },
      {
        .name = "connectTime",
        .dataTypeSpecific.clazz = Nil,
        .number = RtcMember_FieldNumber_ConnectTime,
        .hasIndex = 3,
        .offset = (uint32_t)offsetof(RtcMember__storage_, connectTime),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeInt64,
      },
      {
        .name = "hangupTime",
        .dataTypeSpecific.clazz = Nil,
        .number = RtcMember_FieldNumber_HangupTime,
        .hasIndex = 4,
        .offset = (uint32_t)offsetof(RtcMember__storage_, hangupTime),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeInt64,
      },
      {
        .name = "inviter",
        .dataTypeSpecific.clazz = GPBObjCClass(UserInfo),
        .number = RtcMember_FieldNumber_Inviter,
        .hasIndex = 5,
        .offset = (uint32_t)offsetof(RtcMember__storage_, inviter),
        .flags = GPBFieldOptional,
        .dataType = GPBDataTypeMessage,
      },
    };
    GPBDescriptor *localDescriptor =
        [GPBDescriptor allocDescriptorForClass:GPBObjCClass(RtcMember)
                                   messageName:@"RtcMember"
                               fileDescription:&RtcroomRoot_FileDescription
                                        fields:fields
                                    fieldCount:(uint32_t)(sizeof(fields) / sizeof(GPBMessageFieldDescription))
                                   storageSize:sizeof(RtcMember__storage_)
                                         flags:(GPBDescriptorInitializationFlags)(GPBDescriptorInitializationFlag_UsesClassRefs | GPBDescriptorInitializationFlag_Proto3OptionalKnown | GPBDescriptorInitializationFlag_ClosedEnumSupportKnown)];
    #if !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
      static const char *extraTextFormatInfo =
        "\004\002\010\000\003\010\000\004\013\000\005\n\000";
      [localDescriptor setupExtraTextInfo:extraTextFormatInfo];
    #endif  // !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
    #if defined(DEBUG) && DEBUG
      NSAssert(descriptor == nil, @"Startup recursed!");
    #endif  // DEBUG
    descriptor = localDescriptor;
  }
  return descriptor;
}

@end

int32_t RtcMember_RtcState_RawValue(RtcMember *message) {
  GPBDescriptor *descriptor = [RtcMember descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RtcMember_FieldNumber_RtcState];
  return GPBGetMessageRawEnumField(message, field);
}

void SetRtcMember_RtcState_RawValue(RtcMember *message, int32_t value) {
  GPBDescriptor *descriptor = [RtcMember descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RtcMember_FieldNumber_RtcState];
  GPBSetMessageRawEnumField(message, field, value);
}

#pragma mark - RtcRoomEvent

@implementation RtcRoomEvent

@dynamic roomEventType;
@dynamic hasMember, member;

typedef struct RtcRoomEvent__storage_ {
  uint32_t _has_storage_[1];
  RtcRoomEventType roomEventType;
  RtcMember *member;
} RtcRoomEvent__storage_;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "roomEventType",
        .dataTypeSpecific.enumDescFunc = RtcRoomEventType_EnumDescriptor,
        .number = RtcRoomEvent_FieldNumber_RoomEventType,
        .hasIndex = 0,
        .offset = (uint32_t)offsetof(RtcRoomEvent__storage_, roomEventType),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldHasEnumDescriptor | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeEnum,
      },
      {
        .name = "member",
        .dataTypeSpecific.clazz = GPBObjCClass(RtcMember),
        .number = RtcRoomEvent_FieldNumber_Member,
        .hasIndex = 1,
        .offset = (uint32_t)offsetof(RtcRoomEvent__storage_, member),
        .flags = GPBFieldOptional,
        .dataType = GPBDataTypeMessage,
      },
    };
    GPBDescriptor *localDescriptor =
        [GPBDescriptor allocDescriptorForClass:GPBObjCClass(RtcRoomEvent)
                                   messageName:@"RtcRoomEvent"
                               fileDescription:&RtcroomRoot_FileDescription
                                        fields:fields
                                    fieldCount:(uint32_t)(sizeof(fields) / sizeof(GPBMessageFieldDescription))
                                   storageSize:sizeof(RtcRoomEvent__storage_)
                                         flags:(GPBDescriptorInitializationFlags)(GPBDescriptorInitializationFlag_UsesClassRefs | GPBDescriptorInitializationFlag_Proto3OptionalKnown | GPBDescriptorInitializationFlag_ClosedEnumSupportKnown)];
    #if !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
      static const char *extraTextFormatInfo =
        "\001\001\r\000";
      [localDescriptor setupExtraTextInfo:extraTextFormatInfo];
    #endif  // !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
    #if defined(DEBUG) && DEBUG
      NSAssert(descriptor == nil, @"Startup recursed!");
    #endif  // DEBUG
    descriptor = localDescriptor;
  }
  return descriptor;
}

@end

int32_t RtcRoomEvent_RoomEventType_RawValue(RtcRoomEvent *message) {
  GPBDescriptor *descriptor = [RtcRoomEvent descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RtcRoomEvent_FieldNumber_RoomEventType];
  return GPBGetMessageRawEnumField(message, field);
}

void SetRtcRoomEvent_RoomEventType_RawValue(RtcRoomEvent *message, int32_t value) {
  GPBDescriptor *descriptor = [RtcRoomEvent descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RtcRoomEvent_FieldNumber_RoomEventType];
  GPBSetMessageRawEnumField(message, field, value);
}

#pragma mark - RtcInviteReq

@implementation RtcInviteReq

@dynamic targetIdsArray, targetIdsArray_Count;
@dynamic roomType;
@dynamic roomId;

typedef struct RtcInviteReq__storage_ {
  uint32_t _has_storage_[1];
  RtcRoomType roomType;
  NSMutableArray *targetIdsArray;
  NSString *roomId;
} RtcInviteReq__storage_;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "targetIdsArray",
        .dataTypeSpecific.clazz = Nil,
        .number = RtcInviteReq_FieldNumber_TargetIdsArray,
        .hasIndex = GPBNoHasBit,
        .offset = (uint32_t)offsetof(RtcInviteReq__storage_, targetIdsArray),
        .flags = (GPBFieldFlags)(GPBFieldRepeated | GPBFieldTextFormatNameCustom),
        .dataType = GPBDataTypeString,
      },
      {
        .name = "roomType",
        .dataTypeSpecific.enumDescFunc = RtcRoomType_EnumDescriptor,
        .number = RtcInviteReq_FieldNumber_RoomType,
        .hasIndex = 0,
        .offset = (uint32_t)offsetof(RtcInviteReq__storage_, roomType),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldHasEnumDescriptor | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeEnum,
      },
      {
        .name = "roomId",
        .dataTypeSpecific.clazz = Nil,
        .number = RtcInviteReq_FieldNumber_RoomId,
        .hasIndex = 1,
        .offset = (uint32_t)offsetof(RtcInviteReq__storage_, roomId),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeString,
      },
    };
    GPBDescriptor *localDescriptor =
        [GPBDescriptor allocDescriptorForClass:GPBObjCClass(RtcInviteReq)
                                   messageName:@"RtcInviteReq"
                               fileDescription:&RtcroomRoot_FileDescription
                                        fields:fields
                                    fieldCount:(uint32_t)(sizeof(fields) / sizeof(GPBMessageFieldDescription))
                                   storageSize:sizeof(RtcInviteReq__storage_)
                                         flags:(GPBDescriptorInitializationFlags)(GPBDescriptorInitializationFlag_UsesClassRefs | GPBDescriptorInitializationFlag_Proto3OptionalKnown | GPBDescriptorInitializationFlag_ClosedEnumSupportKnown)];
    #if !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
      static const char *extraTextFormatInfo =
        "\003\001\000targetIds\000\002\010\000\003\006\000";
      [localDescriptor setupExtraTextInfo:extraTextFormatInfo];
    #endif  // !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
    #if defined(DEBUG) && DEBUG
      NSAssert(descriptor == nil, @"Startup recursed!");
    #endif  // DEBUG
    descriptor = localDescriptor;
  }
  return descriptor;
}

@end

int32_t RtcInviteReq_RoomType_RawValue(RtcInviteReq *message) {
  GPBDescriptor *descriptor = [RtcInviteReq descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RtcInviteReq_FieldNumber_RoomType];
  return GPBGetMessageRawEnumField(message, field);
}

void SetRtcInviteReq_RoomType_RawValue(RtcInviteReq *message, int32_t value) {
  GPBDescriptor *descriptor = [RtcInviteReq descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RtcInviteReq_FieldNumber_RoomType];
  GPBSetMessageRawEnumField(message, field, value);
}

#pragma mark - RtcAnswerReq

@implementation RtcAnswerReq

@dynamic targetId;
@dynamic roomId;

typedef struct RtcAnswerReq__storage_ {
  uint32_t _has_storage_[1];
  NSString *targetId;
  NSString *roomId;
} RtcAnswerReq__storage_;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "targetId",
        .dataTypeSpecific.clazz = Nil,
        .number = RtcAnswerReq_FieldNumber_TargetId,
        .hasIndex = 0,
        .offset = (uint32_t)offsetof(RtcAnswerReq__storage_, targetId),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeString,
      },
      {
        .name = "roomId",
        .dataTypeSpecific.clazz = Nil,
        .number = RtcAnswerReq_FieldNumber_RoomId,
        .hasIndex = 1,
        .offset = (uint32_t)offsetof(RtcAnswerReq__storage_, roomId),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeString,
      },
    };
    GPBDescriptor *localDescriptor =
        [GPBDescriptor allocDescriptorForClass:GPBObjCClass(RtcAnswerReq)
                                   messageName:@"RtcAnswerReq"
                               fileDescription:&RtcroomRoot_FileDescription
                                        fields:fields
                                    fieldCount:(uint32_t)(sizeof(fields) / sizeof(GPBMessageFieldDescription))
                                   storageSize:sizeof(RtcAnswerReq__storage_)
                                         flags:(GPBDescriptorInitializationFlags)(GPBDescriptorInitializationFlag_UsesClassRefs | GPBDescriptorInitializationFlag_Proto3OptionalKnown | GPBDescriptorInitializationFlag_ClosedEnumSupportKnown)];
    #if !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
      static const char *extraTextFormatInfo =
        "\002\001\010\000\002\006\000";
      [localDescriptor setupExtraTextInfo:extraTextFormatInfo];
    #endif  // !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
    #if defined(DEBUG) && DEBUG
      NSAssert(descriptor == nil, @"Startup recursed!");
    #endif  // DEBUG
    descriptor = localDescriptor;
  }
  return descriptor;
}

@end

#pragma mark - RtcInviteEvent

@implementation RtcInviteEvent

@dynamic inviteType;
@dynamic hasTargetUser, targetUser;
@dynamic hasRoom, room;

typedef struct RtcInviteEvent__storage_ {
  uint32_t _has_storage_[1];
  InviteType inviteType;
  UserInfo *targetUser;
  RtcRoom *room;
} RtcInviteEvent__storage_;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "inviteType",
        .dataTypeSpecific.enumDescFunc = InviteType_EnumDescriptor,
        .number = RtcInviteEvent_FieldNumber_InviteType,
        .hasIndex = 0,
        .offset = (uint32_t)offsetof(RtcInviteEvent__storage_, inviteType),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldHasEnumDescriptor | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeEnum,
      },
      {
        .name = "targetUser",
        .dataTypeSpecific.clazz = GPBObjCClass(UserInfo),
        .number = RtcInviteEvent_FieldNumber_TargetUser,
        .hasIndex = 1,
        .offset = (uint32_t)offsetof(RtcInviteEvent__storage_, targetUser),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom),
        .dataType = GPBDataTypeMessage,
      },
      {
        .name = "room",
        .dataTypeSpecific.clazz = GPBObjCClass(RtcRoom),
        .number = RtcInviteEvent_FieldNumber_Room,
        .hasIndex = 2,
        .offset = (uint32_t)offsetof(RtcInviteEvent__storage_, room),
        .flags = GPBFieldOptional,
        .dataType = GPBDataTypeMessage,
      },
    };
    GPBDescriptor *localDescriptor =
        [GPBDescriptor allocDescriptorForClass:GPBObjCClass(RtcInviteEvent)
                                   messageName:@"RtcInviteEvent"
                               fileDescription:&RtcroomRoot_FileDescription
                                        fields:fields
                                    fieldCount:(uint32_t)(sizeof(fields) / sizeof(GPBMessageFieldDescription))
                                   storageSize:sizeof(RtcInviteEvent__storage_)
                                         flags:(GPBDescriptorInitializationFlags)(GPBDescriptorInitializationFlag_UsesClassRefs | GPBDescriptorInitializationFlag_Proto3OptionalKnown | GPBDescriptorInitializationFlag_ClosedEnumSupportKnown)];
    #if !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
      static const char *extraTextFormatInfo =
        "\002\001\n\000\002\n\000";
      [localDescriptor setupExtraTextInfo:extraTextFormatInfo];
    #endif  // !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
    #if defined(DEBUG) && DEBUG
      NSAssert(descriptor == nil, @"Startup recursed!");
    #endif  // DEBUG
    descriptor = localDescriptor;
  }
  return descriptor;
}

@end

int32_t RtcInviteEvent_InviteType_RawValue(RtcInviteEvent *message) {
  GPBDescriptor *descriptor = [RtcInviteEvent descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RtcInviteEvent_FieldNumber_InviteType];
  return GPBGetMessageRawEnumField(message, field);
}

void SetRtcInviteEvent_InviteType_RawValue(RtcInviteEvent *message, int32_t value) {
  GPBDescriptor *descriptor = [RtcInviteEvent descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RtcInviteEvent_FieldNumber_InviteType];
  GPBSetMessageRawEnumField(message, field, value);
}

#pragma mark - RtcMemberRooms

@implementation RtcMemberRooms

@dynamic roomsArray, roomsArray_Count;

typedef struct RtcMemberRooms__storage_ {
  uint32_t _has_storage_[1];
  NSMutableArray *roomsArray;
} RtcMemberRooms__storage_;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "roomsArray",
        .dataTypeSpecific.clazz = GPBObjCClass(RtcMemberRoom),
        .number = RtcMemberRooms_FieldNumber_RoomsArray,
        .hasIndex = GPBNoHasBit,
        .offset = (uint32_t)offsetof(RtcMemberRooms__storage_, roomsArray),
        .flags = GPBFieldRepeated,
        .dataType = GPBDataTypeMessage,
      },
    };
    GPBDescriptor *localDescriptor =
        [GPBDescriptor allocDescriptorForClass:GPBObjCClass(RtcMemberRooms)
                                   messageName:@"RtcMemberRooms"
                               fileDescription:&RtcroomRoot_FileDescription
                                        fields:fields
                                    fieldCount:(uint32_t)(sizeof(fields) / sizeof(GPBMessageFieldDescription))
                                   storageSize:sizeof(RtcMemberRooms__storage_)
                                         flags:(GPBDescriptorInitializationFlags)(GPBDescriptorInitializationFlag_UsesClassRefs | GPBDescriptorInitializationFlag_Proto3OptionalKnown | GPBDescriptorInitializationFlag_ClosedEnumSupportKnown)];
    #if defined(DEBUG) && DEBUG
      NSAssert(descriptor == nil, @"Startup recursed!");
    #endif  // DEBUG
    descriptor = localDescriptor;
  }
  return descriptor;
}

@end

#pragma mark - RtcMemberRoom

@implementation RtcMemberRoom

@dynamic roomType;
@dynamic roomId;
@dynamic hasOwner, owner;
@dynamic rtcState;

typedef struct RtcMemberRoom__storage_ {
  uint32_t _has_storage_[1];
  RtcRoomType roomType;
  RtcState rtcState;
  NSString *roomId;
  UserInfo *owner;
} RtcMemberRoom__storage_;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "roomType",
        .dataTypeSpecific.enumDescFunc = RtcRoomType_EnumDescriptor,
        .number = RtcMemberRoom_FieldNumber_RoomType,
        .hasIndex = 0,
        .offset = (uint32_t)offsetof(RtcMemberRoom__storage_, roomType),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldHasEnumDescriptor | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeEnum,
      },
      {
        .name = "roomId",
        .dataTypeSpecific.clazz = Nil,
        .number = RtcMemberRoom_FieldNumber_RoomId,
        .hasIndex = 1,
        .offset = (uint32_t)offsetof(RtcMemberRoom__storage_, roomId),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeString,
      },
      {
        .name = "owner",
        .dataTypeSpecific.clazz = GPBObjCClass(UserInfo),
        .number = RtcMemberRoom_FieldNumber_Owner,
        .hasIndex = 2,
        .offset = (uint32_t)offsetof(RtcMemberRoom__storage_, owner),
        .flags = GPBFieldOptional,
        .dataType = GPBDataTypeMessage,
      },
      {
        .name = "rtcState",
        .dataTypeSpecific.enumDescFunc = RtcState_EnumDescriptor,
        .number = RtcMemberRoom_FieldNumber_RtcState,
        .hasIndex = 3,
        .offset = (uint32_t)offsetof(RtcMemberRoom__storage_, rtcState),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldHasEnumDescriptor | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeEnum,
      },
    };
    GPBDescriptor *localDescriptor =
        [GPBDescriptor allocDescriptorForClass:GPBObjCClass(RtcMemberRoom)
                                   messageName:@"RtcMemberRoom"
                               fileDescription:&RtcroomRoot_FileDescription
                                        fields:fields
                                    fieldCount:(uint32_t)(sizeof(fields) / sizeof(GPBMessageFieldDescription))
                                   storageSize:sizeof(RtcMemberRoom__storage_)
                                         flags:(GPBDescriptorInitializationFlags)(GPBDescriptorInitializationFlag_UsesClassRefs | GPBDescriptorInitializationFlag_Proto3OptionalKnown | GPBDescriptorInitializationFlag_ClosedEnumSupportKnown)];
    #if !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
      static const char *extraTextFormatInfo =
        "\003\001\010\000\002\006\000\004\010\000";
      [localDescriptor setupExtraTextInfo:extraTextFormatInfo];
    #endif  // !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
    #if defined(DEBUG) && DEBUG
      NSAssert(descriptor == nil, @"Startup recursed!");
    #endif  // DEBUG
    descriptor = localDescriptor;
  }
  return descriptor;
}

@end

int32_t RtcMemberRoom_RoomType_RawValue(RtcMemberRoom *message) {
  GPBDescriptor *descriptor = [RtcMemberRoom descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RtcMemberRoom_FieldNumber_RoomType];
  return GPBGetMessageRawEnumField(message, field);
}

void SetRtcMemberRoom_RoomType_RawValue(RtcMemberRoom *message, int32_t value) {
  GPBDescriptor *descriptor = [RtcMemberRoom descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RtcMemberRoom_FieldNumber_RoomType];
  GPBSetMessageRawEnumField(message, field, value);
}

int32_t RtcMemberRoom_RtcState_RawValue(RtcMemberRoom *message) {
  GPBDescriptor *descriptor = [RtcMemberRoom descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RtcMemberRoom_FieldNumber_RtcState];
  return GPBGetMessageRawEnumField(message, field);
}

void SetRtcMemberRoom_RtcState_RawValue(RtcMemberRoom *message, int32_t value) {
  GPBDescriptor *descriptor = [RtcMemberRoom descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RtcMemberRoom_FieldNumber_RtcState];
  GPBSetMessageRawEnumField(message, field, value);
}


#pragma clang diagnostic pop

// @@protoc_insertion_point(global_scope)

// clang-format on
