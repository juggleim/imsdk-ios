//
//  JUploadManager.m
// JuggleIM
//
//  Created by 郑开 on 2024/7/9.
//

#import "JUploadManager.h"
#import "JLogger.h"
#import "JUploadEnum.h"
#import "JUploadQiNiuCred.h"
#import "JUploadPreSignCred.h"
#import "JMediaMessageContent.h"
#import "JImageMessage.h"
#import "JFileMessage.h"
#import "JVoiceMessage.h"
#import "JThumbnailPackedImageMessage.h"
#import "JSnapshotPackedVideoMessage.h"
#import "JUploaderFactory.h"
#import "JUtility.h"

#define jLogUploadSuccess 3
#define jLogUploadFail 4
#define jLogNotExist 5

@interface JUploadManager ()

@property (nonatomic, strong) JIMCore *core;

@end

@implementation JUploadManager

- (instancetype)initWithCore:(JIMCore *)core{
    if (self = [super init]) {
        self.core = core;
    }
    return self;
}

- (void)uploadImage:(UIImage *)image success:(void (^)(NSString *))successBlock error:(void (^)(JErrorCode))errorBlock {
    if (self.core.webSocket == nil) {
        JLogE(@"J-Uploader", @"upload image fail, webSocket is null");
        if(errorBlock) {
            errorBlock(JErrorCodeConnectionUnavailable);
        }
        return;
    }
    if (!image) {
        JLogE(@"J-Uploader", @"upload image fail, image is nil");
        if (errorBlock) {
            errorBlock(JErrorCodeInvalidParam);
        }
        return;
    }
    NSString *path = [self pathOfImage:image];
    JUploadFileType uploadFileType = JUploadFileType_Image;
    [self requestUploadFileCred:uploadFileType
                       filePath:path
                        success:^(JUploadOssType ossType, JUploadQiNiuCred * _Nonnull qiNiuCred, JUploadPreSignCred * _Nonnull preSignCred) {
        [self uploadFile:path
                 ossType:ossType
               qiNiuCred:qiNiuCred
             preSignCred:preSignCred
                 success:successBlock
                   error:^{
            if (errorBlock) {
                errorBlock(JErrorCodeMessageUploadError);
            }
        }];
    } error:^(JErrorCodeInternal code) {
        if (errorBlock) {
            errorBlock((JErrorCode)code);
        }
    }];
}

- (void)uploadLog:(NSString *)filePath
        messageId:(NSString *)messageId
         complete:(void (^)(void))completeBlock {
    if (self.core.webSocket == nil) {
        JLogE(@"J-Uploader", @"upload log fail, webSocket is null");
        if(completeBlock) {
            completeBlock();
        }
        return;
    }
    if (filePath.length == 0) {
        JLogE(@"J-Uploader", @"log zip file not exist");
        [self notifyLogResult:jLogNotExist messageId:messageId url:nil];
        if (completeBlock) {
            completeBlock();
        }
        return;
    }
    JUploadFileType uploadFileType = JUploadFileType_Log;
    [self requestUploadFileCred:uploadFileType
                       filePath:filePath
                        success:^(JUploadOssType ossType, JUploadQiNiuCred * _Nonnull qiNiuCred, JUploadPreSignCred * _Nonnull preSignCred) {
        [self uploadFile:filePath
                 ossType:ossType
               qiNiuCred:qiNiuCred
             preSignCred:preSignCred
                 success:^(NSString *url) {
            [self notifyLogResult:jLogUploadSuccess messageId:messageId url:url];
            if (completeBlock) {
                completeBlock();
            }
        } error:^{
            [self notifyLogResult:jLogUploadFail messageId:messageId url:nil];
            if (completeBlock) {
                completeBlock();
            }
        }];
    } error:^(JErrorCodeInternal code) {
        [self notifyLogResult:jLogUploadFail messageId:messageId url:nil];
        if (completeBlock) {
            completeBlock();
        }
    }];
}

- (void)uploadMessage:(JMessage *)message
             progress:(void (^)(int progress))progressBlock
              success:(void (^)(JMessage * jmessage))successBlock
                error:(void (^)(void))errorBlock
               cancel:(void (^)(void))cancelBlock {
    if(self.core.webSocket == nil) {
        JLogE(@"J-Uploader", @"uploadMessage fail, webSocket is null, message = %lld", message.clientMsgNo);
        if(errorBlock){
            errorBlock();
        }
        return;
    }
    if(message.content == nil || ![message.content isKindOfClass:[JMediaMessageContent class]]){
        JLogE(@"J-Uploader", @"uploadMessage fail, message content is null, message = %lld", message.clientMsgNo);
        if(errorBlock){
            errorBlock();
        }
        return;
    }
    JMediaMessageContent * content = (JMediaMessageContent *) message.content;
    if(content.localPath == nil || content.localPath.length == 0){
        JLogE(@"J-Uploader", @"uploadMessage fail, local path is null, message = %lld", message.clientMsgNo);
        if(errorBlock){
            errorBlock();
        }
        return;
    }
    JUploadFileType uploadFileType;
    if ([content isKindOfClass:[JImageMessage class]] || [content isKindOfClass:[JThumbnailPackedImageMessage class]]) {
        uploadFileType = JUploadFileType_Image;
    } else if ([content isKindOfClass:[JVideoMessage class]] || [content isKindOfClass:[JSnapshotPackedVideoMessage class]]) {
        uploadFileType = JUploadFileType_Video;
    } else if ([content isKindOfClass:[JFileMessage class]]) {
        uploadFileType = JUploadFileType_File;
    } else if ([content isKindOfClass:[JVoiceMessage class]]) {
        uploadFileType = JUploadFileType_Audio;
    } else {
        uploadFileType = JUploadFileType_Default;
    }
    //获取封面或缩略图
    BOOL needPreUpload = NO;
    NSString * preUploadLocalPath = @"";
    if ([content isKindOfClass:[JImageMessage class]]) {
        preUploadLocalPath = ((JImageMessage *) content).thumbnailLocalPath;
        if (preUploadLocalPath.length > 0) {
            needPreUpload = YES;
        }
    } else if ([content isKindOfClass:[JVideoMessage class]]) {
        preUploadLocalPath = ((JVideoMessage *) content).snapshotLocalPath;
        if (preUploadLocalPath.length > 0) {
            needPreUpload = YES;
        }
    }
    //有缩略图的情况下先上传缩略图
    if (needPreUpload) {
        float preProgressPercent = 0.2f;
        [self doUploadMessage:message
                     fileType:uploadFileType
                    localPath:preUploadLocalPath
                  isPreUpload:YES
                     progress:^(int progress) {
            int realProgress = progress * preProgressPercent;
            progressBlock(realProgress);
        } success:^(JMessage *jmessage) {
            [self doUploadMessage:jmessage
                         fileType:uploadFileType
                        localPath:content.localPath
                      isPreUpload:NO
                         progress:^(int progress) {
                int realProgress = (progress * (1 - preProgressPercent)) + (100 * preProgressPercent);
                progressBlock(realProgress);
            } success:successBlock error:errorBlock cancel:cancelBlock];
            
        } error:errorBlock cancel:cancelBlock];
    } else {
        [self doUploadMessage:message
                     fileType:uploadFileType
                    localPath:content.localPath
                  isPreUpload:NO
                     progress:progressBlock
                      success:successBlock
                        error:errorBlock
                       cancel:cancelBlock];
    }
}

- (void)doUploadMessage:(JMessage *)message
               fileType:(JUploadFileType)fileType
              localPath:(NSString *)localPath
            isPreUpload:(BOOL)isPreUpload
               progress:(void (^)(int progress))progressBlock
                success:(void (^)(JMessage * jmessage))successBlock
                  error:(void (^)(void))errorBlock
                 cancel:(void (^)(void))cancelBlock {
    __weak typeof(self) weakSelf = self;
    [self requestUploadFileCred:fileType
                       filePath:localPath
                        success:^(JUploadOssType ossType, JUploadQiNiuCred * _Nonnull qiNiuCred, JUploadPreSignCred * _Nonnull preSignCred) {
        [weakSelf realUpload:message localPath:localPath ossType:ossType qiNiuCred:qiNiuCred preSignCred:preSignCred progress:progressBlock success:successBlock error:errorBlock cancel:cancelBlock isPreUpload:isPreUpload];
    } error:^(JErrorCodeInternal code) {
        JLogE(@"J-Uploader", @"getUploadFileCred failed, localPath = %@ ,message = %lld, errorCode = %li ", localPath, message.clientMsgNo, code);
        if(errorBlock){
            errorBlock();
        }
    }];
}

- (void)realUpload:(JMessage *)message
         localPath:(NSString *)localPath
           ossType:(JUploadOssType)ossType
         qiNiuCred:(JUploadQiNiuCred *)qiNiuCred
       preSignCred:(JUploadPreSignCred *)preSignCred
          progress:(void (^)(int progress))progressBlock
           success:(void (^)(JMessage * jmessage))successBlock
             error:(void (^)(void))errorBlock
            cancel:(void (^)(void))cancelBlock
       isPreUpload:(BOOL)isPreUpload {
    JBaseUploader * uploader = [JUploaderFactory getUpload:localPath ossType:ossType qiNiuCred:qiNiuCred preSignCred:preSignCred];
    if(uploader == nil){
        JLogE(@"J-Uploader", @"doRealUpload failed, uploader is null, localPath = %@ , message = %lld", localPath,message.clientMsgNo);
        if (errorBlock) {
            errorBlock();
        }
        return;
    }
    uploader.JUploadProgress = ^(int progress) {
        if(progressBlock){
            progressBlock(progress);
        }
    };
    uploader.JUploadSuccess = ^(NSString * _Nonnull url) {
        JMediaMessageContent * content = (JMediaMessageContent *) message.content;
        if(isPreUpload == NO){
            content.url = url;
        }else{
            if ([content isKindOfClass:[JImageMessage class]]){
                JImageMessage * messageContent = (JImageMessage *)content;
                messageContent.thumbnailUrl = url;
            }else if ([content isKindOfClass:[JVideoMessage class]]) {
                JVideoMessage * messageContent = (JVideoMessage *)content;
                messageContent.snapshotUrl = url;
            }
        }
        successBlock(message);
    };
    uploader.JUploadError = ^{
        if(errorBlock){
            errorBlock();
        }
    };
    uploader.JUploadCancel = ^{
        if(cancelBlock){
            cancelBlock();
        }
    };
    [uploader start];
}

#pragma mark - internal
- (void)uploadFile:(NSString *)localPath
           ossType:(JUploadOssType)ossType
         qiNiuCred:(JUploadQiNiuCred *)qiNiuCred
       preSignCred:(JUploadPreSignCred *)preSignCred
           success:(void (^)(NSString *url))successBlock
             error:(void (^)(void))errorBlock {
    JBaseUploader * uploader = [JUploaderFactory getUpload:localPath ossType:ossType qiNiuCred:qiNiuCred preSignCred:preSignCred];
    if(uploader == nil) {
        JLogE(@"J-Uploader", @"upload file failed, uploader is null, localPath = %@", localPath);
        if (errorBlock) {
            errorBlock();
        }
        return;
    }
    uploader.JUploadSuccess = successBlock;
    uploader.JUploadError = errorBlock;
    [uploader start];
}

- (void)requestUploadFileCred:(JUploadFileType)fileType
                     filePath:(NSString *)filePath
                      success:(void (^)(JUploadOssType ossType, JUploadQiNiuCred * _Nonnull qiNiuCred, JUploadPreSignCred * _Nonnull preSignCred))successBlock
                        error:(void (^)(JErrorCodeInternal code))errorBlock {
    NSString * ext = [filePath pathExtension];
    if (ext == nil || ext.length == 0) {
        JLogE(@"J-Uploader", @"requestUploadFileCred fail, ext is null, path = %@", filePath);
        if (errorBlock) {
            errorBlock(JErrorCodeInternalInvalidParam);
        }
        return;
    }
    if (self.core.webSocket == nil) {
        JLogE(@"J-Uploader", @"requestUploadFileCred fail, webSocket is null, path is %@", filePath);
        if (errorBlock) {
            errorBlock(JErrorCodeInternalConnectionUnavailable);
        }
        return;
    }
    [self.core.webSocket getUploadFileCred:self.core.userId
                                  fileType:fileType
                                       ext:ext
                                   success:successBlock
                                     error:^(JErrorCodeInternal code) {
        JLogE(@"J-Uploader", @"getUploadFileCred failed, localPath = %@, errorCode = %ld", filePath, code);
        if (errorBlock) {
            errorBlock(code);
        }
    }];
}

- (void)notifyLogResult:(int)result
              messageId:(NSString *)messageId
                    url:(NSString *)url {
    [self.core.webSocket uploadLogStatus:result
                                  userId:self.core.userId
                               messageId:messageId
                                     url:url];
}

- (NSString *)pathOfImage:(UIImage *)image {
    NSData *imgData = UIImageJPEGRepresentation(image, 0.5);
    
    NSString *mediaPath = [JUtility mediaPath:JMediaTypeImage];
    if (![[NSFileManager defaultManager] fileExistsAtPath:mediaPath]) {
        [[NSFileManager defaultManager] createDirectoryAtPath:mediaPath
                                  withIntermediateDirectories:YES
                                                   attributes:nil
                                                        error:nil];
    }
    NSString *fileName = [NSString stringWithFormat:@"%lld.jpg", (long long)[NSDate date].timeIntervalSince1970];
    NSString *localPath = [mediaPath stringByAppendingPathComponent:fileName];
    [imgData writeToFile:localPath atomically:YES];
    return localPath;
}
@end
