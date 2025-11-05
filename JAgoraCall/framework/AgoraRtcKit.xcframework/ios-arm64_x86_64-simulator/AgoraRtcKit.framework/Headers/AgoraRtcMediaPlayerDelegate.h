//
//  Copyright (c) 2018 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AgoraConstants.h"
#import "AgoraObjects.h"

@protocol AgoraRtcMediaPlayerProtocol;

@protocol AgoraRtcMediaPlayerDelegate <NSObject>
@optional

/** Reports the playback state change.

 @param playerKit AgoraRtcMediaPlayer

 @param state The new playback state after change. See AgoraMediaPlayerState.

 @param reason The player's error code. See AgoraMediaPlayerReason.
 */
- (void)AgoraRtcMediaPlayer:(id<AgoraRtcMediaPlayerProtocol> _Nonnull)playerKit
          didChangedToState:(AgoraMediaPlayerState)state
                      reason:(AgoraMediaPlayerReason)reason NS_SWIFT_NAME(AgoraRtcMediaPlayer(_:didChangedTo:reason:));

/** Reports current playback progress.

 The callback occurs once every one second during the playback and reports
 current playback progress.

 @param playerKit AgoraMediaPlayer

@param positionMs Current playback progress (ms).
 @param timestampMs The NTP timestamp (ms) when the position is sent.
 */
- (void)AgoraRtcMediaPlayer:(id<AgoraRtcMediaPlayerProtocol> _Nonnull)playerKit
       didChangedToPosition:(NSInteger)positionMs
                atTimestamp:(NSTimeInterval)timestampMs NS_SWIFT_NAME(AgoraRtcMediaPlayer(_:didChangedTo:atTimestamp:));

/** Reports the result of the seek operation.

 @param playerKit AgoraRtcMediaPlayer
 @param eventCode AgoraMediaPlayerEvent
 @param elapsedTime The playback elapsed time.
 @param message NSString
 */
- (void)AgoraRtcMediaPlayer:(id<AgoraRtcMediaPlayerProtocol> _Nonnull)playerKit
              didOccurEvent:(AgoraMediaPlayerEvent)eventCode
                elapsedTime:(NSInteger)elapsedTime
                    message:(NSString *_Nullable)message NS_SWIFT_NAME(AgoraRtcMediaPlayer(_:didOccur:elapsedTime:message:));

/** Reports the reception of the media metadata.

 The callback occurs when the player receivers the media metadata and reports
 the detailed information of the media metadata.

 @param playerKit AgoraRtcMediaPlayer

 @param data The detailed data of the media metadata.

 @param length The length (byte) of the data.
 */
- (void)AgoraRtcMediaPlayer:(id<AgoraRtcMediaPlayerProtocol> _Nonnull)playerKit
             didReceiveData:(NSString *_Nullable)data
                     length:(NSInteger)length NS_SWIFT_NAME(AgoraRtcMediaPlayer(_:didReceiveData:length:));

/**
 * @brief Triggered when play buffer updated, once every 1 second
 *
 * @param playerKit AgoraRtcMediaPlayer
 * @param playCachedBuffer NSInteger
 */
- (void)AgoraRtcMediaPlayer:(id<AgoraRtcMediaPlayerProtocol> _Nonnull)playerKit
             didPlayBufferUpdated:(NSInteger)playCachedBuffer NS_SWIFT_NAME(AgoraRtcMediaPlayer(_:didPlayBufferUpdated:));

/**
 * @brief Triggered when the player preloadSrc
 *
 * @param playerKit AgoraRtcMediaPlayer
 * @param event AgoraMediaPlayerPreloadEvent
 */
- (void)AgoraRtcMediaPlayer:(id<AgoraRtcMediaPlayerProtocol> _Nonnull)playerKit
            didPreloadEvent:(AgoraMediaPlayerPreloadEvent)event NS_SWIFT_NAME(AgoraRtcMediaPlayer(_:didPreloadEvent:));

/**
 * @brief Reports current playback source bitrate changed.
 *
 * @param to Streaming media information after the change.
 * @param from Streaming media information before the change.
 */
- (void)AgoraRtcMediaPlayer:(id<AgoraRtcMediaPlayerProtocol> _Nonnull)playerKit playerSrcInfoDidChange:(AgoraMediaPlayerSrcInfo *_Nonnull)to from:(AgoraMediaPlayerSrcInfo *_Nonnull)from NS_SWIFT_NAME(AgoraRtcMediaPlayer(_:playerSrcInfoDidChange:from:));

/**
 * @brief Triggered when  media player information updated.
 *
 * @param info Include information of media player.
 */
- (void)AgoraRtcMediaPlayer:(id<AgoraRtcMediaPlayerProtocol> _Nonnull)playerKit infoUpdated:(AgoraMediaPlayerUpdatedInfo *_Nonnull)info NS_SWIFT_NAME(AgoraRtcMediaPlayer(_:infoUpdated:));

/**
 * @brief Triggered every 1 second, reports the statistics of the files being cached.
 *
 * @param stats Cached file statistics.
 */
- (void)AgoraRtcMediaPlayer:(id<AgoraRtcMediaPlayerProtocol> _Nonnull)playerKit cacheStats:(AgoraMediaPlayerCacheStatistics *_Nonnull)info NS_SWIFT_NAME(AgoraRtcMediaPlayer(_:cacheStats:));

/**
 * @brief Triggered every 1 second, reports the statistics of the media stream being played.
 *
 * @param stats The statistics of the media stream.
 */
- (void)AgoraRtcMediaPlayer:(id<AgoraRtcMediaPlayerProtocol> _Nonnull)playerKit playbackStats:(AgoraMediaPlayerPlaybackStats *_Nonnull)info NS_SWIFT_NAME(AgoraRtcMediaPlayer(_:playbackStats:));

/**
 * @brief AgoraCDN Token has expired and needs to be set up with renewAgoraCDNSrcToken(const char*
 * src).
 * 
 * @deprecated 4.6.0
 * 
 */
- (void)onAgoraCDNTokenWillExpire NS_SWIFT_NAME(onAgoraCDNTokenWillExpire());

/**
 * @brief Triggered when play volume updated, once every 200 millisecond
 *
 * @param volume volume of current player.
 */
- (void)AgoraRtcMediaPlayer:(id<AgoraRtcMediaPlayerProtocol> _Nonnull)playerKit
             volumeIndicationDidReceive:(NSInteger)volume NS_SWIFT_NAME(AgoraRtcMediaPlayer(_:volumeIndicationDidReceive:));
@end
