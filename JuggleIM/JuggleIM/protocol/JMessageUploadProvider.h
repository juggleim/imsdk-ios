//
//  JMessageUploadProvider.h
// JuggleIM
//
//  Created by Nathan on 2024/5/9.
//

#import <Foundation/Foundation.h>

@class JMessage;

NS_ASSUME_NONNULL_BEGIN

@protocol JMessageUploadProvider <NSObject>

/// Implement this method and return the result to the SDK through the callbacks.
/// - Parameters:
///   - message: Message waiting to be uploaded by the developer.
///   - progressBlock: Upload progress callback, 0 <= progress <= 100.
///   - successBlock: Upload success callback. After a successful upload, set the remote URL in JMediaMessageContent's url and pass the message back.
///   - errorBlock: Failure callback.
///   - cancelBlock: User cancellation callback.
- (void)uploadMessage:(JMessage *)message
             progress:(void (^)(int progress))progressBlock
              success:(void (^)(JMessage *message))successBlock
                error:(void (^)(void))errorBlock
               cancel:(void (^)(void))cancelBlock;

@end

NS_ASSUME_NONNULL_END
