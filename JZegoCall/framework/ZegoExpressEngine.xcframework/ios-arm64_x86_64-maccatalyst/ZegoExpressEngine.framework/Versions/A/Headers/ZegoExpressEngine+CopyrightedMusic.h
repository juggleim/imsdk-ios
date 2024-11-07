//
//  ZegoExpressEngine+CopyrightedMusic.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (CopyrightedMusic)

/// Creates a copyrighted music instance.
///
/// Available since: 2.13.0
/// Description: Creates a copyrighted music instance.
/// Use case: Often used in online KTV chorus scenarios, users can use related functions by creating copyrighted music instance objects.
/// When to call: It can be called after the engine by [createEngine] has been initialized.
/// Restrictions: The SDK only supports the creation of one instance of CopyrightedMusic. Multiple calls to this function return the same object.
///
/// @return copyrighted music instance, multiple calls to this function return the same object.
- (nullable ZegoCopyrightedMusic *)createCopyrightedMusic;

/// Destroys a copyrighted music instance.
///
/// Available since: 2.13.0
/// Description: Destroys a copyrighted music instance.
/// When to call: It can be called before the engine by [destroyEngine]
///
/// @param copyrightedMusic The copyrighted music instance object to be destroyed.
- (void)destroyCopyrightedMusic:(ZegoCopyrightedMusic *)copyrightedMusic;

@end

NS_ASSUME_NONNULL_END
