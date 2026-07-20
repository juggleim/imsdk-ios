//
//  ZegoExpressEngine+PictureCapturer.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (PictureCapturer)

/// Create picture capturer instance.
///
/// Available since: 3.22.0
/// Description: Creates a picture capturer instance.
/// Use case: Often used in pushing static images.
/// When to call: It can be called after the SDK by [createEngine] has been initialized.
/// Restrictions: None.
/// Related APIs: User can call [destroyPictureCapturer] function to destroy a picture capturer instance. Use [setVideoSource] to set the picture capturer as the push stream video source.
///
/// @return Picture capturer instance.
- (nullable ZegoPictureCapturer *)createPictureCapturer;

/// Destroys a picture capturer instance.
///
/// Available since: 3.22.0
/// Description: Destroys the picture capturer instance.
/// Related APIs: User can call [createPictureCapturer] function to create a picture capturer instance.
///
/// @param pictureCapturer The picture capturer instance to be destroyed.
- (void)destroyPictureCapturer:(ZegoPictureCapturer *)pictureCapturer;

@end

NS_ASSUME_NONNULL_END
