//
//  Copyright (c) 2018 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AgoraRtcMediaPlayerProtocol.h"

@protocol AgoraRtcMediaPlayerProtocol;
@class AgoraOutputVideoFrame;

@protocol AgoraRtcMediaPlayerVideoFrameDelegate <NSObject>
@optional
/** Occurs when each time the player receives a video frame.

 After registering the video frame observer, the callback occurs when each
 time the player receives a video frame, reporting the detailed
 information of the video frame.

 @param playerKit AgoraRtcMediaPlayer

 @param videoFrame The detailed information of the video frame.
 */
- (void)AgoraRtcMediaPlayer:(id<AgoraRtcMediaPlayerProtocol> _Nonnull)playerKit
       didReceiveVideoFrame:(AgoraOutputVideoFrame * _Nonnull)videoFrame NS_SWIFT_NAME(AgoraRtcMediaPlayer(_:didReceiveVideoFrame:));

/** Occurs when each time the player receives a video frame.

 After registering the video frame observer, the callback occurs when each
 time the player receives a video frame, reporting the detailed
 information of the CVPixelBufferRef.

 @param playerKit AgoraRtcMediaPlayer

 @param pixelBuffer The detailed information of the CVPixelBufferRef. Format define by AgoraRtcMediaPlayerGetVideoPixelFormat.
 */
- (void)AgoraRtcMediaPlayer:(id<AgoraRtcMediaPlayerProtocol> _Nonnull)playerKit
       didReceivePixelBuffer:(CVPixelBufferRef _Nonnull)pixelBuffer NS_SWIFT_NAME(AgoraRtcMediaPlayer(_:didReceivePixelBuffer:));

/**
 * Occurs each time needs to get preference video frame type.
 * @return AgoraVideoFormat.
 */
- (AgoraVideoFormat)AgoraRtcMediaPlayerGetVideoPixelFormat NS_SWIFT_NAME(AgoraRtcMediaPlayerGetVideoPixelFormat());

@end