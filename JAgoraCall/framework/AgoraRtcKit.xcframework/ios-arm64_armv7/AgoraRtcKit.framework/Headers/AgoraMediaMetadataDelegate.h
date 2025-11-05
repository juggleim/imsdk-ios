//
//  AgoraRtcEngineKit.h
//  AgoraRtcEngineKit
//
//  Copyright (c) 2018 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AgoraObjects.h"

/** The definition of AgoraMediaMetadataDelegate.
@note  Implement the callback in this protocol in the critical thread. We recommend avoiding any time-consuming operation in the critical thread.
*/
@protocol AgoraMediaMetadataDelegate <NSObject>
@required

/** Occurs when the local user receives the metadata.
 * 
 * @param metadata The received metadata. See \ref AgoraMetadata.
 */
- (void)didMetadataReceived:(AgoraMetadata * _Nonnull)metadata NS_SWIFT_NAME(didMetadataReceived(_:));

@optional
/* Unavailable Delegate Methods */
#if TARGET_OS_IPHONE
- (void)receiveMetadata:(NSData * _Nonnull)data fromUser:(NSInteger)uid atTimestamp:(NSTimeInterval)timestamp NS_SWIFT_NAME(receiveMetadata(_:fromUser:atTimestamp:)) __attribute__((availability(ios,deprecated=7_0,message="Use didMetadataReceived: instead.")));
#endif
#if (!(TARGET_OS_IPHONE) && (TARGET_OS_MAC))
- (void)receiveMetadata:(NSData * _Nonnull)data fromUser:(NSInteger)uid atTimestamp:(NSTimeInterval)timestamp NS_SWIFT_NAME(receiveMetadata(_:fromUser:atTimestamp:)) __attribute__((availability(macos,deprecated=10_9,message="Use didMetadataReceived: instead.")));
#endif
@end
