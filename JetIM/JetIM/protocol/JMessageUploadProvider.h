//
//  JMessageUploadProvider.h
//  JetIM
//
//  Created by Nathan on 2024/5/9.
//

#import <Foundation/Foundation.h>

@class JMessage;

NS_ASSUME_NONNULL_BEGIN

@protocol JMessageUploadProvider <NSObject>

- (void)uploadMessage:(JMessage *)message
             progress:(void (^)(int progress))progressBlock
              success:(void (^)(JMessage *message))successBlock
                error:(void (^)(void))errorBlock
               cancel:(void (^)(void))cancelBlock;

@end

NS_ASSUME_NONNULL_END
