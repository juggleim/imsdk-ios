//
//  AgoraRtcEngineKit.h
//  AgoraRtcEngineKit
//
//  Copyright (c) 2018 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AgoraConstants.h"
#import "AgoraObjects.h"

/** Agora provides ensured quality of experience (QoE) for worldwide Internet-based voice and video communications through a virtual global network that is especially optimized for real-time web and mobile-to-mobile applications.

 The AgoraRtcEngineKit class is the entry point of the Agora SDK that provides simple APIs for applications to easily start voice and video communication.
 */
@class AgoraRtcEngineKit;
@class AgoraMediaRecorder;

@protocol AgoraAudioEncodedFrameDelegate <NSObject>
@required

/**
 * Occurs when the record audio data is received.
 */
- (void)onRecordEncodedAudioFrame:(NSData* _Nonnull)frameData info:(AgoraEncodedAudioFrameInfo* _Nonnull)info NS_SWIFT_NAME(onRecordEncodedAudioFrame(_:info:));

/**
 * Occurs when the playback audio data is received.
 */
- (void)onPlaybackEncodedAudioFrame:(NSData* _Nonnull)frameData info:(AgoraEncodedAudioFrameInfo* _Nonnull)info NS_SWIFT_NAME(onPlaybackEncodedAudioFrame(_:info:));

/**
 * Occurs when the mixed audio data is received.
 */
- (void)onMixedEncodedAudioFrame:(NSData* _Nonnull)frameData info:(AgoraEncodedAudioFrameInfo* _Nonnull)info NS_SWIFT_NAME(onMixedEncodedAudioFrame(_:info:));
@end
