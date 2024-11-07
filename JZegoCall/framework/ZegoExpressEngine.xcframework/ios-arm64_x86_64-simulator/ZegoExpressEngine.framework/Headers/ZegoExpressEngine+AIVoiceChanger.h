//
//  ZegoExpressEngine+AIVoiceChanger.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (AIVoiceChanger)

/// Creates an AI voice changer instance.
///
/// Available since: 3.10.0
/// Description: Creates an AI voice changer instance.
/// Use case: Often used in live broadcasting, voice chatroom and KTV.
/// When to call: It can be called after the SDK by [createEngine] has been initialized.
/// Restrictions: Currently, a maximum of 1 instance can be created, after which it will return nil.
/// Related APIs: User can call [destroyAIVoiceChanger] function to destroy an AI voice changer instance.
///
/// @return AI voice changer instance.
- (nullable ZegoAIVoiceChanger *)createAIVoiceChanger;

/// Destroys an AI voice changer instance.
///
/// Available since: 3.10.0
/// Description: Destroys the AI voice changer instance.
/// Related APIs: User can call [createAIVoiceChanger] function to create an AI voice changer instance.
///
/// @param aiVoiceChanger The AI voice changer instance object to be destroyed.
- (void)destroyAIVoiceChanger:(ZegoAIVoiceChanger *)aiVoiceChanger;

/// Check the device can run AI voice changer or not.
///
/// Available since: 3.14.0
/// Description: Check the device can run AI voice changer or not.
/// Use case: Often used in live broadcasting, voice chatroom and KTV.
/// When to call: It can be called after the SDK by [createEngine] has been initialized.
///
/// @return Return YES if the device can run AI voice changer, otherwise return NO.
- (BOOL)isAIVoiceChangerSupported;

@end

NS_ASSUME_NONNULL_END
