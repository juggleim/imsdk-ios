//
//  JDownloadManager.m
//  JuggleIM
//
//  Created by Nathan on 2024/7/23.
//

#import "JDownloadManager.h"
#import "JDownloader.h"

@interface JDownloadManager ()
@property (nonatomic, strong) NSMutableDictionary <NSString *, JDownloader *> *downloadDic;
@property (nonatomic, strong) dispatch_queue_t downloadQueue;
@end

@implementation JDownloadManager

- (void)downloadWithMessageId:(NSString *)messageId
                          Url:(NSString *)url
                         path:(NSString *)localPath
                     progress:(void (^)(int))progressBlock
                      success:(void (^)(NSString * _Nonnull))successBlock
                        error:(void (^)(JErrorCode))errorBlock {
    dispatch_async(self.downloadQueue, ^{
        JDownloader *downloader = [self.downloadDic objectForKey:messageId];
        if (downloader) {
            if (errorBlock) {
                errorBlock(JErrorCodeDownloadAlreadyExist);
            }
            return;
        }
        downloader = [[JDownloader alloc] initWithUrl:url
                                                 path:localPath
                                             progress:progressBlock
                                              success:^(NSString * _Nonnull localPath) {
            dispatch_async(self.downloadQueue, ^{
                [self.downloadDic removeObjectForKey:messageId];
            });
            if (successBlock) {
                successBlock(localPath);
            }
        } error:^(JErrorCode errorCode) {
            dispatch_async(self.downloadQueue, ^{
                [self.downloadDic removeObjectForKey:messageId];
            });
            if (errorBlock) {
                errorBlock(errorCode);
            }
        }];
        [self.downloadDic setObject:downloader forKey:messageId];
        [downloader start];
    });
}

- (void)cancelDownload:(NSString *)messageId {
    dispatch_async(self.downloadQueue, ^{
        JDownloader *downloader = [self.downloadDic objectForKey:messageId];
        [downloader cancel];
    });
}

- (NSMutableDictionary<NSString *,JDownloader *> *)downloadDic {
    if (!_downloadDic) {
        _downloadDic = [NSMutableDictionary dictionary];
    }
    return _downloadDic;
}

- (dispatch_queue_t)downloadQueue {
    if (!_downloadQueue) {
        _downloadQueue = dispatch_queue_create("com.JIM.im.logqueue", DISPATCH_QUEUE_SERIAL);
    }
    return _downloadQueue;
}
@end
