//
//  ZegoExpressEngine+ScreenCapture.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (ScreenCapture)

#if TARGET_OS_OSX
/// Get a list of screens or windows in a screen.
///
/// Available since: 3.1.0
/// Description: Get a list of screens or windows in a screen.
/// Restrictions: Only support in Windows/macOS.
///
/// @param thumbnailSize Get the thumbnail size corresponding to the window, the thumbnail can be used to draw on the window selection interface. (unit is pixel)
/// @param iconSize Get the size of the icon corresponding to the program. (unit is pixel)
/// @return List of capture source info objects.
- (NSArray<ZegoScreenCaptureSourceInfo *> *)
    getScreenCaptureSourcesWithThumbnailSize:(CGSize)thumbnailSize
                                    iconSize:(CGSize)iconSize;
#endif

#if TARGET_OS_OSX
/// Create the screen capture source
///
/// Available: since 3.1.0
/// Description: Creates a screen capture source object based on the provided source ID and source type.
/// Use cases: It is used when you need to record and share the screen or window.
/// When to call: It needs to be called after [createEngine].
/// Platform differences: Only supports Windows and macOS.
///
/// @param sourceId The specified screen ID or window ID.
/// @param sourceType The specified screen source type.
/// @return The screen capture instance, nil will be returned when the maximum number is exceeded.
- (nullable ZegoScreenCaptureSource *)createScreenCaptureSource:(unsigned int)sourceId
                                                     sourceType:
                                                         (ZegoScreenCaptureSourceType)sourceType;
#endif

#if TARGET_OS_OSX
/// Destroy the screen capture source instance
///
/// Available: since 3.1.0
/// Description: Destroy the [ZegoScreenCaptureSource] instance object.
/// Use cases: When you no longer need to use the screen capture function, you can use this function to destroy the instance object created by the [createScreenCaptureSource] function
/// When to call: When you need to the screen capture source object needs to be destroyed
/// Restrictions: After destroy the instance, you need to release the [ZegoScreenCaptureSource] instance object you hold by yourself, and don’t call the function of this instance object after the destruction.
/// Platform differences: Only supports Windows and macOS.
///
/// @param source The screen capture source instance to be destroyed.
- (void)destroyScreenCaptureSource:(ZegoScreenCaptureSource *)source;
#endif

#if TARGET_OS_IPHONE
/// Set the App Group configuration item.
///
/// Available since: 3.3.0
/// Use cases: You need to use the iOS cross-process screen sharing function, and you need to start the App Group, which can provide better performance and stability. Must be used with [setupWithAppGroupID:] in the `ZegoReplayKit` extension class.
/// When to call: Called after [createEngine], before calling [startScreenCapture].
/// Restrictions: Only available on iOS platform.
///
/// @param groupID The host app and the extension app should belong to the same App Group, and the AppGroupID needs to be passed in here.
- (void)setAppGroupID:(NSString *)groupID API_AVAILABLE(ios(12.0));
#endif

#if TARGET_OS_IPHONE
/// Start screen capture, in-app capture only.
///
/// Available since: 3.1.0
/// Description: Start screen capture.
/// When to call: After calling the [setVideoSource]、[setAudioSource] function to set the capture source to `ScreenCapture`.
/// Restrictions: Only valid for iOS system
///
/// @param config Screen capture parameter configuration.
- (void)startScreenCaptureInApp:(ZegoScreenCaptureConfig *)config API_AVAILABLE(ios(12.0));
#endif

#if TARGET_OS_IPHONE
/// Start screen capture.
///
/// Available since: 3.1.0
/// Description: Start screen capture.
/// When to call: After calling the [setVideoSource]、[setAudioSource] function to set the capture source to `ScreenCapture`.
///
/// @param config Screen capture parameter configuration.
- (void)startScreenCapture:(ZegoScreenCaptureConfig *)config API_AVAILABLE(ios(12.0));
#endif

#if TARGET_OS_IPHONE
/// Stop screen capture.
///
/// Available since: 3.1.0
/// Description: Stop screen capture.
- (void)stopScreenCapture API_AVAILABLE(ios(12.0));
#endif

#if TARGET_OS_IPHONE
/// Update screen capture parameter configuration.
///
/// Available since: 3.1.0
/// Description: Update screen capture parameter configuration.
/// When to call: After calling [startScreenCapture] to start capturing.
/// Restrictions: Only valid for iOS system. Only available on iOS 12.0 or newer
///
/// @param config Screen capture parameter configuration.
- (void)updateScreenCaptureConfig:(ZegoScreenCaptureConfig *)config API_AVAILABLE(ios(12.0));
#endif

@end

NS_ASSUME_NONNULL_END
