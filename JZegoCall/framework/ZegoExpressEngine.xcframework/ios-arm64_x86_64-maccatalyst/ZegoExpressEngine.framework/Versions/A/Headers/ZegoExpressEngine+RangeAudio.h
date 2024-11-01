//
//  ZegoExpressEngine+RangeAudio.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (RangeAudio)

/// Creates a range audio instance.
///
/// Available since: 2.11.0
/// Description: Creates a range audio instance.
/// Use case: Often used in game voice scenarios, users can use related functions by creating range audio instance objects.
/// When to call: It can be called after the engine by [createEngine] has been initialized.
/// Restrictions: Currently, a maximum of 1 instances can be created, after which it will return nil.
/// Impacts on other APIs: If you use the range audio module, you cannot use the basic push-pull stream [startPublishingStream], [startPlayingStream] interfaces and related callbacks.
///
/// @return range audio instance, nil will be returned when the maximum number is exceeded.
- (nullable ZegoRangeAudio *)createRangeAudio;

/// Destroys a range audio instance.
///
/// Available since: 2.11.0
/// Description: Destroys a range audio instance.
///
/// @param rangeAudio The range audio instance object to be destroyed.
- (void)destroyRangeAudio:(ZegoRangeAudio *)rangeAudio;

@end

NS_ASSUME_NONNULL_END
