/**
 *
 * Agora Real Time Engagement
 * Copyright (c) 2024 Agora IO. All rights reserved.
 *
 */


#import <Foundation/Foundation.h>
#import "AgoraRtePlayer.h"
#import "AgoraRteEnumerates.h"

/**
 * Player Observer, an interface for receiving player event callbacks.
 * @since v4.4.0
 */
__attribute__((visibility("default"))) @interface AgoraRtePlayerObserver : NSObject

- (instancetype _Nonnull)init;

/**
 * Player state callback. This function is called when the player state changes.
 * @since v4.4.0
 * @param oldState The previous state.
 * @param newState The new state.
 * @param error Possible ErrorCode returns. Only when the new_state value corresponds to AgoraRtePlayerStateFailed, you need to check the value of this parameter.
 *   - AgoraRteErrorDefault. For specific reasons, see Error.message, including the following situations:
 *     - Failed to connect to the channel.
 *   - AgoraRteErrorInvalidArgument.
 *     - Invalid appid.
 *     - Invalid channelid.
 *     - Invalid uid.
 *   - AgoraRteErrorAuthenticationFailed.
 *     - Invalid token.
 *     - Token expired.
 *   - AgoraRteErrorStreamNotFound. After entering the channel, no stream was received from the broadcaster for more than 10 seconds.
 * @return void
 */
- (void)onStateChanged:(AgoraRtePlayerState)oldState newState:(AgoraRtePlayerState)newState error:(AgoraRteError * _Nullable)error;

/** 
 * Playback position change callback.
 * @since v4.4.0
 * @param currentTime Current playback progress (milisecond).
 * @param utcTime Current playback progress (milisecond).
 * @return void
 */
- (void)onPositionChanged:(uint64_t)currentTime utcTime:(uint64_t)utcTime;

/**
 * Video resolution change callback.
 * @since v4.4.0
 * @param width The width of the video frame.
 * @param height The height of the video frame.
 * @return void
 */
- (void)onResolutionChanged:(int)width height:(int)height;

/**
 * Event callback.
 * @since v4.4.0
 * @param event The event notified by the callback. Refer to AgoraRtePlayerEvent type. Currently, the following events can be handled accordingly:
 *  - AgoraRtePlayerEventFreezeStart: Indicates that stuttering has occurred or shows a loading animation.
 *  - AgoraRtePlayerEventFreezeStop: Indicates that stuttering has ended or stops the loading animation.
 *  - AgoraRtePlayerEventAuthenticationWillExpire: Regenerate the token, use the new token to construct the rte URL, and call Player::OpenWithUrl to refresh the token.
 *  - AgoraRtePlayerEventAbrFallbackToAudioOnlyLayer: Indicates that due to network reasons, it has fallen back to audio-only mode.
 *  - AgoraRtePlayerEventAbrRecoverFromAudioOnlyLayer: Indicates that it has recovered from audio-only mode to video mode.
 * @return void
 */
- (void)onEvent:(AgoraRtePlayerEvent)event;

/**
 * Metadata callback.
 * @since v4.4.0
 * @param type The type of metadata.
 * @param data The metadata buffer.
 * @return void
 */
- (void)onMetadata:(AgoraRtePlayerMetadataType)type data:(NSData * _Nonnull)data;

/**
 * Player information update callback. This is called when fields in AgoraRtePlayerInfo are updated.
 * @since v4.4.0
 * @param info The current AgoraRtePlayerInfo information.
 * @return void 
 */
- (void)onPlayerInfoUpdated:(AgoraRtePlayerInfo * _Nonnull)info;

/**
 * Update player current volume.
 * @since v4.4.0
 * @param volume The current volume of the player. The value range is [0, 255].
 * @return void
 */
- (void)onAudioVolumeIndication:(int32_t)volume;

@end
