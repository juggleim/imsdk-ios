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

- (void)uploadMessage:(JMessage *)message
             progress:(void (^)(int progress))progressBlock
              success:(void (^)(JMessage * jmessage))successBlock
                error:(void (^)(void))errorBlock
               cancel:(void (^)(void))cancelBlock{
    
    if(self.core.webSocket == nil){
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
        [self doRequestUploadFileCred:message
                             fileType:uploadFileType
                            localPath:preUploadLocalPath
                          isPreUpload:YES
                             progress:^(int progress) {
            int realProgress = progress * preProgressPercent;
            progressBlock(realProgress);
        } success:^(JMessage *jmessage) {
            [self doRequestUploadFileCred:jmessage
                                 fileType:uploadFileType
                                localPath:content.localPath
                              isPreUpload:NO
                                 progress:^(int progress) {
                int realProgress = (progress * (1 - preProgressPercent)) + (100 * preProgressPercent);
                progressBlock(realProgress);
            }
                                  success:successBlock
                                    error:errorBlock
                                   cancel:cancelBlock];
            
        }error:errorBlock cancel:cancelBlock];
    }else{
        [self doRequestUploadFileCred:message
                             fileType:uploadFileType
                            localPath:content.localPath
                          isPreUpload:NO
                             progress:progressBlock
                              success:successBlock
                                error:errorBlock
                               cancel:cancelBlock];
    }
    
}

-(void)doRequestUploadFileCred:(JMessage *)message
                      fileType:(JUploadFileType)fileType
                     localPath:(NSString *)localPath
                   isPreUpload:(BOOL)isPreUpload
                      progress:(void (^)(int progress))progressBlock
                       success:(void (^)(JMessage * jmessage))successBlock
                         error:(void (^)(void))errorBlock
                        cancel:(void (^)(void))cancelBlock{
    NSString * ext = [localPath pathExtension];
    if(ext == nil || ext.length == 0){
        JLogE(@"J-Uploader", @"doRequestUploadFileCred fail, ext is null, localPath = %@", localPath);
        if(errorBlock){
            errorBlock();
        }
        return;
    }
    if(self.core.webSocket == nil){
        JLogE(@"J-Uploader", @"uploadMessage fail, webSocket is null, message = %lld", message.clientMsgNo);
        if(errorBlock){
            errorBlock();
        }
        return;
    }
    __weak typeof(self) weakSelf = self;
    [self.core.webSocket getUploadFileCred:self.core.userId fileType:fileType ext:ext success:^(JUploadOssType ossType, JUploadQiNiuCred * _Nonnull qiNiuCred, JUploadPreSignCred * _Nonnull preSignCred) {
        [weakSelf realUpload:message localPath:localPath ossType:ossType qiNiuCred:qiNiuCred preSignCred:preSignCred progress:progressBlock success:successBlock error:errorBlock cancel:cancelBlock isPreUpload:isPreUpload];
    } error:^(JErrorCodeInternal code) {
        JLogE(@"J-Uploader", @"getUploadFileCred failed, localPath = %@ ,message = %lld, errorCode = %li ", localPath,message.clientMsgNo,code);
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
       isPreUpload:(BOOL)isPreUpload{
    JBaseUploader * uploader = [JUploaderFactory getUpload:localPath ossType:ossType qiNiuCred:qiNiuCred preSignCred:preSignCred];
    if(uploader == nil){
        JLogE(@"J-Uploader", @"doRealUpload failed, uploader is null, localPath = %@ , message = %lld", localPath,message.clientMsgNo);
        if (errorBlock) {
            errorBlock();
        }
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
@end
