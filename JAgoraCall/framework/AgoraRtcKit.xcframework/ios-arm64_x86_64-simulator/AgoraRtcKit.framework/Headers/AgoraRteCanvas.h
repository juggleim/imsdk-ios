/**
 *
 * Agora Real Time Engagement
 * Copyright (c) 2024 Agora IO. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>

#import "AgoraRteEnumerates.h"

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
typedef UIView AgoraRteView;
#elif TARGET_OS_MAC
#import <AppKit/AppKit.h>
typedef NSView AgoraRteView;
#endif

@class AgoraRteError;
@class AgoraRte;

/**
 * The Rect class is used to manage the rect.
 * @since v4.4.0
 * @technical preview
 */
__attribute__((visibility("default"))) @interface AgoraRteRect : NSObject
- (instancetype _Nonnull)init;

- (void)setX:(int32_t)x;
- (int32_t)x;

- (void)setY:(int32_t)y;
- (int32_t)y;

- (void)setWidth:(int32_t)width;
- (int32_t)width;

- (void)setHeight:(int32_t)height;
- (int32_t)height;
@end

/**
 * The ViewConfig class is used to manage the view config.
 * @since v4.4.0
 * @technical preview
 */
__attribute__((visibility("default"))) @interface AgoraRteViewConfig : NSObject
- (instancetype _Nonnull)init;

- (void)setCropArea:(AgoraRteRect* _Nullable)cropArea error:(AgoraRteError* _Nullable)error;

- (AgoraRteRect* _Nullable)cropArea:(AgoraRteError* _Nullable)error;
@end

/**
 * The CanvasInitialConfig class is used to initialize the Canvas object.
 * @since v4.4.0
 * @technical preview
 */
__attribute__((visibility("default"))) @interface AgoraRteCanvasInitialConfig : NSObject
- (instancetype _Nonnull)init;
@end


/**
 * The AgoraRteCanvasConfig class is used to configure the AgoraRteCanvas object.
 * @since v4.4.0
 */
__attribute__((visibility("default"))) @interface AgoraRteCanvasConfig : NSObject
- (instancetype _Nonnull)init;

/**
 * Set the video render mode.
 * @since v4.4.0
 * @param mode The render mode to set. Refer to AgoraRteVideoRenderMode type, default is AgoraRteVideoRenderModeHidden.
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 * - AgoraRteOk: Success
 * - AgoraRteErrorInvalidArgument: The mode parameter is set to an illegal value.
 * @return void
 */
- (void)setVideoRenderMode:(AgoraRteVideoRenderMode)mode error:(AgoraRteError * _Nullable)error;

/**
 * Get the render mode.
 * @since v4.4.0
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 *  - AgoraRteOk: Success
 * @return AgoraRteVideoRenderMode
 */
- (AgoraRteVideoRenderMode)videoRenderMode:(AgoraRteError * _Nullable)error;

/**
 * Set the video mirror mode.
 * @since v4.4.0
 * @param mode The mirror mode to set. Refer to AgoraRteVideoMirrorMode type, default is AgoraRteVideoMirrorModeAuto.
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 * - AgoraRteOk: Success
 * - AgoraRteErrorInvalidArgument: The mode parameter is set to an illegal value.
 * @return void
 */
- (void)setVideoMirrorMode:(AgoraRteVideoMirrorMode)mode error:(AgoraRteError* _Nullable)error;

/**
 * Get the video mirror mode.
 * @since v4.4.0
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 *  - AgoraRteOk: Success
 * @return AgoraRteVideoMirrorMode
 */
- (AgoraRteVideoMirrorMode)videoMirrorMode:(AgoraRteError * _Nullable)error;

- (void)setCropArea:(AgoraRteRect* _Nonnull)cropArea error:(AgoraRteError * _Nullable)error;

- (AgoraRteRect* _Nonnull)cropArea:(AgoraRteError* _Nullable)error;

@end


/**
 * AgoraRteCanvas interface, used to set the video rendering view.
 * @since v4.4.0
 */
__attribute__((visibility("default"))) @interface AgoraRteCanvas : NSObject

/**
 * Construct an AgoraRteCanvas object.
 * @since v4.4.0
 * @param rte AgoraRte object.
 * @param config CanvasInitialConfig initialization configuration object. Currently, a null pointer can be passed.
 */
- (instancetype _Nonnull)initWithRte:(AgoraRte* _Nonnull)rte initialConfig:(AgoraRteCanvasInitialConfig * _Nullable)config;

/**
 * Get the configuration of AgoraRteCanvas object.
 * @since v4.4.0
 * @param config CanvasConfig object
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 *   - AgoraRteOk: Success
 *   - AgoraRteErrorInvalidOperation: The corresponding internal Canvas object has been destroyed or is invalid.
 *   - AgoraRteErrorInvalidArgument: The passed config object is null.
 * @return BOOL
 *   - YES: Successfully retrieved.
 *   - NO: Failed to retrieve.
 */
- (BOOL)getConfigs:(AgoraRteCanvasConfig* _Nonnull)config error:(AgoraRteError* _Nullable)error;

/**
 * Configure the AgoraRteCanvas object.
 * @since v4.4.0
 * @param config CanvasConfig object
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 *   - AgoraRteOk: Success
 *   - AgoraRteErrorInvalidOperation: The corresponding internal Canvas object has been destroyed or is invalid.
 *   - AgoraRteErrorInvalidArgument: The passed config object is null.
 * @return BOOL
 *   - YES: Successfully set the configuration.
 *   - NO: Failed to set the configuration.
 */
- (BOOL)setConfigs:(AgoraRteCanvasConfig* _Nonnull)config error:(AgoraRteError* _Nullable)error;

/**
 * Add a rendering view. Currently, only one view is supported.
 * @since v4.4.0
 * @param view Pointer to the View object. On the iOS platform, you can assign an UIView object(NSView Object on mac) to a View type variable and pass it to the interface.
 * @param config View-related configuration. Currently, nullptr can be passed.
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 *   - AgoraRteOk: Success
 *   - AgoraRteErrorInvalidOperation: The corresponding internal Canvas object has been destroyed or is invalid.
 *   - AgoraRteErrorInvalidArgument: The passed view is null.
 * @return BOOL
 *   - YES: Successfully add the View.
 *   - NO: Failed to add the View.
 */
- (BOOL)addView:(AgoraRteView * _Nonnull)view config:(AgoraRteViewConfig* _Nullable)config error:(AgoraRteError* _Nullable)error;

/** 
 * Remove a rendering view.
 * @since v4.4.0
 * @param view Pointer to the View object.
 * @param config View-related configuration. Currently, nullptr can be passed.
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 *   - AgoraRteOk: Success
 *   - AgoraRteErrorInvalidOperation: The corresponding internal Canvas object has been destroyed or is invalid.
 *   - AgoraRteErrorInvalidArgument: The passed view is null.
 * @return BOOL
 *   - YES: Successfully removed the View.
 *   - NO: Failed to remove the View.
 */
- (BOOL)removeView:(AgoraRteView * _Nonnull)view config:(AgoraRteViewConfig* _Nullable)config error:(AgoraRteError* _Nullable)error;

@end
