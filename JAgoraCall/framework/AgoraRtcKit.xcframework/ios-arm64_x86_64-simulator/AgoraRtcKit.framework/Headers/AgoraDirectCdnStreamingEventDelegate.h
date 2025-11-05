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

/**
 * The event handler for direct cdn streaming
 * 
 * @deprecated v4.6.0.
 *
 */
@protocol AgoraDirectCdnStreamingEventDelegate <NSObject>
@optional

/**
 * Event callback of direct cdn streaming
 * @param state Current status
 * @param reason Reason Code
 * @param message Message
 */
- (void)onDirectCdnStreamingStateChanged:(AgoraDirectCdnStreamingState)state
                                   reason:(AgoraDirectCdnStreamingReason)reason
                                 message:(NSString *_Nullable)message NS_SWIFT_NAME(onDirectCdnStreamingStateChanged(_:reason:message:));

- (void)onDirectCdnStreamingStats:(AgoraDirectCdnStreamingStats *_Nonnull)stats NS_SWIFT_NAME(onDirectCdnStreamingStats(_:));

@end