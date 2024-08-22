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

/// 开发者实现此方法，并将结果回调给 SDK
/// - Parameters:
///   - message: 等待开发者上传的消息
///   - progressBlock: 上传进度回调，0 <= progress <= 100
///   - successBlock: 上传成功回调，开发者上传成功后需自行将远端地址置入 JMediaMessageContent 的 url 内再把 message 回调上来
///   - errorBlock: 失败回调
///   - cancelBlock: 用户取消回调
- (void)uploadMessage:(JMessage *)message
             progress:(void (^)(int progress))progressBlock
              success:(void (^)(JMessage *message))successBlock
                error:(void (^)(void))errorBlock
               cancel:(void (^)(void))cancelBlock;

@end

NS_ASSUME_NONNULL_END
