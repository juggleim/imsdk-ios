//
//  AgoraRtcEngineKit.h
//  AgoraRtcEngineKit
//
//  Copyright (c) 2018 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AgoraEnumerates.h"

/**
 * The AgoraFaceInfoDelegate protocol enables face info callback event notifications to your application.
 */
@protocol AgoraFaceInfoDelegate <NSObject>
@optional

/**
 * Occurs when the face info is received.
 * @param outFaceInfo A pointer to the face info: NSString.
 * @return
 * - true: The face info is valid and sent.
 * - false: The face info is invalid or sent.
 */
- (BOOL)onFaceInfo:(NSString* _Nonnull)outFaceInfo NS_SWIFT_NAME(onFaceInfo(_:));

@end
