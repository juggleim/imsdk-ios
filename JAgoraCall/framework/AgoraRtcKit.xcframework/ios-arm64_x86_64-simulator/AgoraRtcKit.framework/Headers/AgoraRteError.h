/**
 *
 * Agora Real Time Engagement
 * Copyright (c) 2024 Agora IO. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>

#import "AgoraRteEnumerates.h"

/**
 * AgoraRteError class. Used to record the execution result of an interface call.
 * @since v4.4.0
 * @technical preview
 */
__attribute__((visibility("default"))) @interface AgoraRteError : NSObject

- (instancetype _Nonnull)init;

- (void)setErrorWithCode:(AgoraRteErrorCode)code message:(NSString * _Nullable)message;

/**
 * This interface is used to get the specific error code.
 * @since v4.4.0
 * @return AgoraRteErrorCode Error code, Refer to the AgoraRteErrorCode type for details.
 */
- (AgoraRteErrorCode)code;

/**
 * This interface is used to get the specific error description.
 * @since v4.4.0
 * @return NSString Error description.
 */
- (NSString * _Nullable)message;

@end
