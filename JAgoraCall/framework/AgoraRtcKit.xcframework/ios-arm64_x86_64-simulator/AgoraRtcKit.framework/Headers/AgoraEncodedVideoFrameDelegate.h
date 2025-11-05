//
//  AgoraRtcEngineKit.h
//  AgoraRtcEngineKit
//
//  Copyright (c) 2018 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>

@class AgoraEncodedVideoFrameInfo;

@protocol AgoraEncodedVideoFrameDelegate <NSObject>
@optional

/**
 * Occurs when  get H264 video data interface before decoding
 */
- (BOOL)onEncodedVideoFrameReceived:(NSData * _Nonnull )videoData length:(size_t)length info:(AgoraEncodedVideoFrameInfo * _Nonnull)videoFrameInfo NS_SWIFT_NAME(onEncodedVideoFrameReceived(_:length:info:));
@end
