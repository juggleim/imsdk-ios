//
//  JDownloader.m
//  JetIM
//
//  Created by Nathan on 2024/7/17.
//

#import "JDownloader.h"
#import "JLogger.h"

@interface JDownloader ()<NSURLSessionDownloadDelegate>
@property (nonatomic, copy) NSString *url;
@property (nonatomic, copy) NSString *localPath;
@property (nonatomic, copy) void (^progressBlock)(int progress);
@property (nonatomic, copy) void (^successBlock)(NSString *localPath);
@property (nonatomic, copy) void (^errorBlock)(JErrorCode errorCode);
@end

@implementation JDownloader

- (instancetype)initWithUrl:(NSString *)url
                       path:(NSString *)localPath
                   progress:(void (^)(int))progressBlock
                    success:(void (^)(NSString * _Nonnull))successBlock
                      error:(void (^)(JErrorCode))errorBlock {
    self = [super init];
    if (self) {
        self.url = url;
        self.localPath = localPath;
        self.progressBlock = progressBlock;
        self.successBlock = successBlock;
        self.errorBlock = errorBlock;
    }
    return self;
}

- (void)start {
    NSURLSession *session = [NSURLSession sessionWithConfiguration:[NSURLSessionConfiguration defaultSessionConfiguration]
                                                          delegate:self
                                                     delegateQueue:nil];
    NSURLSessionDownloadTask *task = [session downloadTaskWithURL:[NSURL URLWithString:self.url]];
    [task resume];
}

- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didFinishDownloadingToURL:(NSURL *)location {
    NSError *err = nil;
    [[NSFileManager defaultManager] moveItemAtURL:location toURL:[NSURL fileURLWithPath:self.localPath isDirectory:NO] error:&err];
    if (err) {
        JLogE(@"MSG-Downloader", @"downloadTask error, description is %@", err.localizedDescription);
        if (self.errorBlock) {
            self.errorBlock(JErrorCodeMessageDownloadError);
        }
        [session invalidateAndCancel];
        return;
    }
    if (self.successBlock) {
        self.successBlock(self.localPath);
    }
    [session invalidateAndCancel];
}

- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didWriteData:(int64_t)bytesWritten totalBytesWritten:(int64_t)totalBytesWritten totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite {
    int progress = (int)(100*totalBytesWritten/totalBytesExpectedToWrite);
    if (self.progressBlock) {
        self.progressBlock(progress);
    }
}

@end
