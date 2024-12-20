// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// clang-format off
// source: pushtoken.proto

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

#import "Pushtoken.pbobjc.h"
// @@protoc_insertion_point(imports)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"

#pragma mark - Objective-C Class declarations
// Forward declarations of Objective-C classes that we can use as
// static values in struct initializers.
// We don't use [Foo class] because it is not a static value.
GPBObjCClassDeclaration(RegPushTokenReq);

#pragma mark - PushtokenRoot

@implementation PushtokenRoot

// No extensions in the file and no imports or none of the imports (direct or
// indirect) defined extensions, so no need to generate +extensionRegistry.

@end

static GPBFileDescription PushtokenRoot_FileDescription = {
  .package = NULL,
  .prefix = NULL,
  .syntax = GPBFileSyntaxProto3
};

#pragma mark - Enum Platform

GPBEnumDescriptor *Platform_EnumDescriptor(void) {
  static _Atomic(GPBEnumDescriptor*) descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static const char *valueNames =
        "DefaultPlatform\000Android\000IOs\000Web\000Pc\000";
    static const int32_t values[] = {
        Platform_DefaultPlatform,
        Platform_Android,
        Platform_IOs,
        Platform_Web,
        Platform_Pc,
    };
    static const char *extraTextFormatInfo = "\004\000\017\000\001\007\000\002\"A\000\003\003\000";
    GPBEnumDescriptor *worker =
        [GPBEnumDescriptor allocDescriptorForName:GPBNSStringifySymbol(Platform)
                                       valueNames:valueNames
                                           values:values
                                            count:(uint32_t)(sizeof(values) / sizeof(int32_t))
                                     enumVerifier:Platform_IsValidValue
                                            flags:GPBEnumDescriptorInitializationFlag_None
                              extraTextFormatInfo:extraTextFormatInfo];
    GPBEnumDescriptor *expected = nil;
    if (!atomic_compare_exchange_strong(&descriptor, &expected, worker)) {
      [worker release];
    }
  }
  return descriptor;
}

BOOL Platform_IsValidValue(int32_t value__) {
  switch (value__) {
    case Platform_DefaultPlatform:
    case Platform_Android:
    case Platform_IOs:
    case Platform_Web:
    case Platform_Pc:
      return YES;
    default:
      return NO;
  }
}

#pragma mark - Enum PushChannel

GPBEnumDescriptor *PushChannel_EnumDescriptor(void) {
  static _Atomic(GPBEnumDescriptor*) descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static const char *valueNames =
        "DefaultChannel\000Apple\000Huawei\000Xiaomi\000";
    static const int32_t values[] = {
        PushChannel_DefaultChannel,
        PushChannel_Apple,
        PushChannel_Huawei,
        PushChannel_Xiaomi,
    };
    static const char *extraTextFormatInfo = "\004\000\016\000\001\005\000\002\006\000\003\006\000";
    GPBEnumDescriptor *worker =
        [GPBEnumDescriptor allocDescriptorForName:GPBNSStringifySymbol(PushChannel)
                                       valueNames:valueNames
                                           values:values
                                            count:(uint32_t)(sizeof(values) / sizeof(int32_t))
                                     enumVerifier:PushChannel_IsValidValue
                                            flags:GPBEnumDescriptorInitializationFlag_None
                              extraTextFormatInfo:extraTextFormatInfo];
    GPBEnumDescriptor *expected = nil;
    if (!atomic_compare_exchange_strong(&descriptor, &expected, worker)) {
      [worker release];
    }
  }
  return descriptor;
}

BOOL PushChannel_IsValidValue(int32_t value__) {
  switch (value__) {
    case PushChannel_DefaultChannel:
    case PushChannel_Apple:
    case PushChannel_Huawei:
    case PushChannel_Xiaomi:
      return YES;
    default:
      return NO;
  }
}

#pragma mark - RegPushTokenReq

@implementation RegPushTokenReq

@dynamic deviceId;
@dynamic platform;
@dynamic pushChannel;
@dynamic pushToken;
@dynamic packageName;
@dynamic voipToken;

typedef struct RegPushTokenReq__storage_ {
  uint32_t _has_storage_[1];
  Platform platform;
  PushChannel pushChannel;
  NSString *deviceId;
  NSString *pushToken;
  NSString *packageName;
  NSString *voipToken;
} RegPushTokenReq__storage_;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "deviceId",
        .dataTypeSpecific.clazz = Nil,
        .number = RegPushTokenReq_FieldNumber_DeviceId,
        .hasIndex = 0,
        .offset = (uint32_t)offsetof(RegPushTokenReq__storage_, deviceId),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeString,
      },
      {
        .name = "platform",
        .dataTypeSpecific.enumDescFunc = Platform_EnumDescriptor,
        .number = RegPushTokenReq_FieldNumber_Platform,
        .hasIndex = 1,
        .offset = (uint32_t)offsetof(RegPushTokenReq__storage_, platform),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldHasEnumDescriptor | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeEnum,
      },
      {
        .name = "pushChannel",
        .dataTypeSpecific.enumDescFunc = PushChannel_EnumDescriptor,
        .number = RegPushTokenReq_FieldNumber_PushChannel,
        .hasIndex = 2,
        .offset = (uint32_t)offsetof(RegPushTokenReq__storage_, pushChannel),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldHasEnumDescriptor | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeEnum,
      },
      {
        .name = "pushToken",
        .dataTypeSpecific.clazz = Nil,
        .number = RegPushTokenReq_FieldNumber_PushToken,
        .hasIndex = 3,
        .offset = (uint32_t)offsetof(RegPushTokenReq__storage_, pushToken),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeString,
      },
      {
        .name = "packageName",
        .dataTypeSpecific.clazz = Nil,
        .number = RegPushTokenReq_FieldNumber_PackageName,
        .hasIndex = 4,
        .offset = (uint32_t)offsetof(RegPushTokenReq__storage_, packageName),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeString,
      },
      {
        .name = "voipToken",
        .dataTypeSpecific.clazz = Nil,
        .number = RegPushTokenReq_FieldNumber_VoipToken,
        .hasIndex = 5,
        .offset = (uint32_t)offsetof(RegPushTokenReq__storage_, voipToken),
        .flags = (GPBFieldFlags)(GPBFieldOptional | GPBFieldTextFormatNameCustom | GPBFieldClearHasIvarOnZero),
        .dataType = GPBDataTypeString,
      },
    };
    GPBDescriptor *localDescriptor =
        [GPBDescriptor allocDescriptorForClass:GPBObjCClass(RegPushTokenReq)
                                   messageName:@"RegPushTokenReq"
                               fileDescription:&PushtokenRoot_FileDescription
                                        fields:fields
                                    fieldCount:(uint32_t)(sizeof(fields) / sizeof(GPBMessageFieldDescription))
                                   storageSize:sizeof(RegPushTokenReq__storage_)
                                         flags:(GPBDescriptorInitializationFlags)(GPBDescriptorInitializationFlag_UsesClassRefs | GPBDescriptorInitializationFlag_Proto3OptionalKnown | GPBDescriptorInitializationFlag_ClosedEnumSupportKnown)];
    #if !GPBOBJC_SKIP_MESSAGE_TEXTFORMAT_EXTRAS
      static const char *extraTextFormatInfo =
        "\005\001\010\000\003\013\000\004\t\000\005\013\000\006\t\000";
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

int32_t RegPushTokenReq_Platform_RawValue(RegPushTokenReq *message) {
  GPBDescriptor *descriptor = [RegPushTokenReq descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RegPushTokenReq_FieldNumber_Platform];
  return GPBGetMessageRawEnumField(message, field);
}

void SetRegPushTokenReq_Platform_RawValue(RegPushTokenReq *message, int32_t value) {
  GPBDescriptor *descriptor = [RegPushTokenReq descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RegPushTokenReq_FieldNumber_Platform];
  GPBSetMessageRawEnumField(message, field, value);
}

int32_t RegPushTokenReq_PushChannel_RawValue(RegPushTokenReq *message) {
  GPBDescriptor *descriptor = [RegPushTokenReq descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RegPushTokenReq_FieldNumber_PushChannel];
  return GPBGetMessageRawEnumField(message, field);
}

void SetRegPushTokenReq_PushChannel_RawValue(RegPushTokenReq *message, int32_t value) {
  GPBDescriptor *descriptor = [RegPushTokenReq descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:RegPushTokenReq_FieldNumber_PushChannel];
  GPBSetMessageRawEnumField(message, field, value);
}


#pragma clang diagnostic pop

// @@protoc_insertion_point(global_scope)

// clang-format on
