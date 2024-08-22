//
//  JDownloadManager.h
//  JuggleIM
//
//  Created by Nathan on 2024/7/23.
//

#import <Foundation/Foundation.h>
#import "JuggleIMConst.h"

NS_ASSUME_NONNULL_BEGIN

@interface JDownloadManager : NSObject

- (void)downloadWithMessageId:(NSString *)messageId
                          Url:(NSString *)url
                         path:(NSString *)localPath
                     progress:(void (^)(int progress))progressBlock
                      success:(void (^)(NSString *localPath))successBlock
                        error:(void (^)(JErrorCode errorCode))errorBlock;

- (void)cancelDownload:(NSString *)messageId;

@end

NS_ASSUME_NONNULL_END
