//
//  ZegoExpressEngine+RangeScene.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (RangeScene)

/// Create a range scene instance object.
///
/// Available since: 3.0.0
/// Description: Create a range scene instance object.
/// Use case: Often used in meta world scenarios, users can use related functions by range scene instance objects.
/// When to call: After initializing the engine by [createEngine].
/// Restrictions: None.
///
/// @return range scene instance object.
- (nullable ZegoRangeScene *)createRangeScene;

/// Destroy a range scene instance object.
///
/// Available since: 3.0.0
/// Description: Destroy a range scene instance.
/// When to call: After create the range scene by [createRangeScene].
/// Restrictions: None.
///
/// @param rangeScene The range scene instance object to be destroyed.
- (void)destroyRangeScene:(ZegoRangeScene *)rangeScene;

@end

NS_ASSUME_NONNULL_END
