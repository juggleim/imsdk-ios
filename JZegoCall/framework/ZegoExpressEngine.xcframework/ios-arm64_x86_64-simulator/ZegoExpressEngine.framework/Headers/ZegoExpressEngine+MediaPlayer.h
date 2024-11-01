//
//  ZegoExpressEngine+MediaPlayer.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (MediaPlayer)

/// Creates a media player instance.
///
/// Available since: 2.1.0
/// Description: Creates a media player instance.
/// Use case: It is often used to play media resource scenes, For example, play video files, push the video of media resources in combination with custom video acquisition, and the remote end can pull the stream for viewing.
/// When to call: It can be called after the SDK by [createEngine] has been initialized.
/// Restrictions: Currently, a maximum of 4 instances can be created, after which it will return nil.
/// Caution: The more instances of a media player, the greater the performance overhead on the device.
/// Related APIs: User can call [destroyMediaPlayer] function to destroy a media player instance.
///
/// @return Media player instance, nil will be returned when the maximum number is exceeded.
- (nullable ZegoMediaPlayer *)createMediaPlayer;

/// Destroys a media player instance.
///
/// Available since: 2.1.0
/// Description: Destroys a media player instance.
/// Related APIs: User can call [createMediaPlayer] function to create a media player instance.
///
/// @param mediaPlayer The media player instance object to be destroyed.
- (void)destroyMediaPlayer:(ZegoMediaPlayer *)mediaPlayer;

@end

NS_ASSUME_NONNULL_END
