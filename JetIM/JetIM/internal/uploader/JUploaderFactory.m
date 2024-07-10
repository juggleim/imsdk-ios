//
//  JUploaderFactory.m
//  JetIM
//
//  Created by 郑开 on 2024/7/10.
//

#import "JUploaderFactory.h"
#import "JPreSignUploader.h"
#import "JQiNiuUploader.h"


@implementation JUploaderFactory

+ (JBaseUploader *)getUpload:(NSString *)localPath
                     ossType:(JUploadOssType)ossType
                   qiNiuCred:(JUploadQiNiuCred *)qiNiuCred
                 preSignCred:(JUploadPreSignCred *)preSignCred{
    JBaseUploader * uploader = nil;
    switch (ossType) {
        case JUploadOssType_QiNiu:
            uploader = [[JQiNiuUploader alloc] initWith:localPath qiNiuCred:qiNiuCred];
            break;
        case JUploadOssType_S3:
        case JUploadOssType_MinIO:
        case JUploadOssType_OSS:
            uploader = [[JPreSignUploader alloc] initWith:localPath preSignCred:preSignCred];
            break;
        default:
            break;
    }
    return uploader;
    
    
    
}

@end
