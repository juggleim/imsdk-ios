#pragma once

#if defined(__APPLE_OS__) || defined(__APPLE__)
#include "TargetConditionals.h"
#else
// Fix "-Wundef" warnings
#define TARGET_OS_IPHONE 0
#define TARGET_OS_OSX 0
#endif

#if defined(_WIN32)
#include <codecvt>
#include <locale>
#include <sstream>
#include <wchar.h>
#include <windows.h>
#define ZegoStrncpy strncpy_s
#endif

#if TARGET_OS_OSX || TARGET_OS_IPHONE
#define ZegoStrncpy strncpy
#endif

#if defined(ANDROID) || defined(_OS_OHOS_)
#define ZegoStrncpy strncpy
#endif

#if defined(__unix__)
#define ZegoStrncpy strncpy
#include <string.h>
#endif

#ifdef ZEGO_DISABLE_SWTICH_THREAD
#define ZEGO_SWITCH_THREAD_PRE_STATIC {
#define ZEGO_SWITCH_THREAD_PRE ZEGO_SWITCH_THREAD_PRE_STATIC
#define ZEGO_SWITCH_THREAD_ING }
#else
#if defined(_WIN32)
#define ZEGO_SWITCH_THREAD_PRE_STATIC                                                              \
    std::function<void(void)> *pFunc = new std::function<void(void)>;                              \
    *pFunc = [=](void) {
#if defined(_MSVC_LANG) && _MSVC_LANG >= 202002L
#define ZEGO_SWITCH_THREAD_PRE                                                                     \
    std::function<void(void)> *pFunc = new std::function<void(void)>;                              \
    *pFunc = [=, this](void) {                                                                     \
    (void)(this);
#else
#define ZEGO_SWITCH_THREAD_PRE ZEGO_SWITCH_THREAD_PRE_STATIC
#endif
#define ZEGO_SWITCH_THREAD_ING                                                                     \
    }                                                                                              \
    ;                                                                                              \
    if (oInternalOriginBridge->isCallbackSwitchToMainThread()) {                                   \
        oInternalOriginBridge->postWindowsMessage(pFunc);                                          \
    } else {                                                                                       \
        oInternalOriginBridge->postZegoCallbackTask(pFunc);                                        \
    }
#elif TARGET_OS_IPHONE
#import <Foundation/Foundation.h>
#define ZEGO_SWITCH_THREAD_PRE_STATIC dispatch_async(dispatch_get_main_queue(), ^{
#define ZEGO_SWITCH_THREAD_PRE ZEGO_SWITCH_THREAD_PRE_STATIC
#define ZEGO_SWITCH_THREAD_ING                                                                     \
    });
#elif TARGET_OS_OSX
#import <Foundation/Foundation.h>
#include <functional>
#define ZEGO_SWITCH_THREAD_PRE_STATIC auto task = [=]() {
#define ZEGO_SWITCH_THREAD_PRE ZEGO_SWITCH_THREAD_PRE_STATIC
#define ZEGO_SWITCH_THREAD_ING                                                                     \
    }                                                                                              \
    ;                                                                                              \
    if (oInternalOriginBridge->isCallbackSwitchToMainThread()) {                                   \
        dispatch_async(dispatch_get_main_queue(), ^{                                               \
          task();                                                                                  \
        });                                                                                        \
    } else {                                                                                       \
        oInternalOriginBridge->postZegoCallbackTask(new std::function<void(void)>(task));          \
    }
#else
#define ZEGO_SWITCH_THREAD_PRE_STATIC {
#define ZEGO_SWITCH_THREAD_PRE ZEGO_SWITCH_THREAD_PRE_STATIC
#define ZEGO_SWITCH_THREAD_ING }
#endif
#endif

#ifdef __GNUC__
#define GCC_VERSION_AT_LEAST(x, y) (__GNUC__ > (x) || __GNUC__ == (x) && __GNUC_MINOR__ >= (y))
#else
#define GCC_VERSION_AT_LEAST(x, y) 0
#endif

#if GCC_VERSION_AT_LEAST(3, 1)
#define ZEGO_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define ZEGO_DEPRECATED __declspec(deprecated)
#else
#define ZEGO_DEPRECATED
#endif

#if __cplusplus >= 201703L
#define ZEGO_DEPRECATED_ENUM [[deprecated]]
#elif defined(__clang__) || GCC_VERSION_AT_LEAST(6, 0)
#define ZEGO_DEPRECATED_ENUM __attribute__((deprecated))
#else
#define ZEGO_DEPRECATED_ENUM
#endif

#if defined(__ICL) || defined(__INTEL_COMPILER)
#define ZEGO_DISABLE_DEPRECATION_WARNINGS __pragma(warning(push)) __pragma(warning(disable : 1478))
#define ZEGO_ENABLE_DEPRECATION_WARNINGS __pragma(warning(pop))
#elif defined(_MSC_VER)
#define ZEGO_DISABLE_DEPRECATION_WARNINGS __pragma(warning(push)) __pragma(warning(disable : 4996))
#define ZEGO_ENABLE_DEPRECATION_WARNINGS __pragma(warning(pop))
#else
#define ZEGO_DISABLE_DEPRECATION_WARNINGS                                                          \
    _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#define ZEGO_ENABLE_DEPRECATION_WARNINGS _Pragma("GCC diagnostic pop")
#endif

#define ZEGO_UNUSED_VARIABLE(x) ((void)(x))

#include <chrono>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
