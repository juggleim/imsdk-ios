//
//  ZegoExpressEngine+MediaDataPublisher.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (MediaDataPublisher)

/// Creates a media data publisher instance.
///
/// Available since: 3.4.0
/// Description: Developers can use this interface to create ZegoMediaDataPublisher objects to push local media resource files to implement server-side streaming scenarios, such as AI classrooms.
/// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
/// When to call: After calling the [createEngine] function to create an engine.
/// Caution: It returns the instance object of the specified publish channel. It will return the instance object if it already exists.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param config Config the media data publisher.
/// @return Media data publisher instance.
- (nullable ZegoMediaDataPublisher *)createMediaDataPublisher:
    (ZegoMediaDataPublisherConfig *)config;

/// Destroys a media data publisher instance.
///
/// Available since: 2.17.0
/// Description: Destroys a media data publisher instance.
/// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
/// When to call: After calling the [createEngine] function to create an engine.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param mediaDataPublisher The media data publisher instance object to be destroyed
- (void)destroyMediaDataPublisher:(ZegoMediaDataPublisher *)mediaDataPublisher;

@end

NS_ASSUME_NONNULL_END
