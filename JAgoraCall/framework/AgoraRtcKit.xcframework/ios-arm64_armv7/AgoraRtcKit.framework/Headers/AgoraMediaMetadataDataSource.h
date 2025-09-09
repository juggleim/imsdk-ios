//
//  AgoraRtcEngineKit.h
//  AgoraRtcEngineKit
//
//  Copyright (c) 2018 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AgoraConstants.h"
#import "AgoraObjects.h"
#import "AgoraRtcMediaPlayerProtocol.h"
#import "AgoraH265TranscoderProtocol.h"

/** Agora provides ensured quality of experience (QoE) for worldwide Internet-based voice and video communications through a virtual global network that is especially optimized for real-time web and mobile-to-mobile applications.

 The AgoraRtcEngineKit class is the entry point of the Agora SDK that provides simple APIs for applications to easily start voice and video communication.
 */
@class AgoraRtcEngineKit;
@class AgoraMediaRecorder;

/** The definition of the AgoraMediaMetadataDataSource protocol.
 * @note Implement all the callbacks in this protocol in the critical thread. We recommend avoiding any time-consuming
 * operation in the critical thread.
*/
@protocol AgoraMediaMetadataDataSource <NSObject>
@required

/** Occurs when the SDK requests the maximum size of the metadata.
*
* After calling the \ref AgoraRtcEngineKit.setMediaMetadataDataSource:withType: setMediaMetadataDataSource method,
* the SDK triggers this callback to query the maximum size of your metadata.
* You must specify the maximum size in the return value and then pass it to the SDK.
*
* @return The maximum size (bytes) of the buffer of the metadata. See \ref AgoraMediaMetadataDataSource.readyToSendMetadataAtTimestamp: readyToSendMetadataAtTimestamp. The value must not exceed 1024 bytes.
* You must specify the maximum size in this return value.
 */
- (NSInteger)metadataMaxSize NS_SWIFT_NAME(metadataMaxSize());

/** Occurs when the SDK is ready to send metadata.

You need to specify the metadata in the return value of this method.

 @note  Ensure that the size of the metadata that you specify in this callback does not exceed the value set in the \ref AgoraMediaMetadataDataSource.metadataMaxSize metadataMaxSize callback.
 @param timestamp The timestamp (ms) of the current metadata.
 @return The metadata that you want to send in the format of NSData, including the following parameters:
 - `uid`: ID of the user who sends the metadata.
 - `size`: The size of the sent metadata.
 - `buffer`: The sent metadata.
 - `timeStampMs`: The NTP timestamp (ms) when the metadata is sent.
 */
- (NSData * _Nullable)readyToSendMetadataAtTimestamp:(NSTimeInterval)timestamp sourceType:(AgoraVideoSourceType)sourceType NS_SWIFT_NAME(readyToSendMetadata(atTimestamp:sourceType:));

@end
