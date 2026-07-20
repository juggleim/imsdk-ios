
#ifndef __ZEGO_EXPRESS_PICTURE_CAPTURER_H__
#define __ZEGO_EXPRESS_PICTURE_CAPTURER_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Create picture capturer instance.
///
/// Available since: 3.22.0
/// Description: Creates a picture capturer instance.
/// Use case: Often used in pushing static images.
/// When to call: It can be called after the SDK by [createEngine] has been initialized.
/// Restrictions: None.
/// Related APIs: User can call [destroyPictureCapturer] function to destroy a picture capturer instance. Use [setVideoSource] to set the picture capturer as the push stream video source.
///
/// @param instance_index [in/out] Picture capturer instance, -1 will be returned when the maximum number is exceeded.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_create_picture_capturer(int *instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_create_picture_capturer)(int *instance_index);
#endif

/// Destroys a picture capturer instance.
///
/// Available since: 3.22.0
/// Description: Destroys the picture capturer instance.
/// Related APIs: User can call [createPictureCapturer] function to create a picture capturer instance.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_destroy_picture_capturer(int instance_index);
#else
typedef zego_error(EXP_CALL *pfnzego_express_destroy_picture_capturer)(int instance_index);
#endif

/// Set the path of the picture capturer source.
///
/// Available since: 3.22.0
/// Description: Set the path of the picture capturer source.
/// Related APIs: User can call [createPictureCapturer] function to create a picture capturer instance.
///
/// @param path The path of the picture. Support local picture file path (file://xxx), Android URI path (uri://xxx), asset resource path (asset://xxx). The URL length cannot exceed 512 characters.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_picture_capturer_set_path(int instance_index,
                                                                       const char *path);
#else
typedef zego_error(EXP_CALL *pfnzego_express_picture_capturer_set_path)(int instance_index,
                                                                        const char *path);
#endif

ZEGO_END_DECLS

#endif
