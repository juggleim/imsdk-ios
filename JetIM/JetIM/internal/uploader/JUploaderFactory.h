//
//  JUploaderFactory.h
//  JetIM
//
//  Created by 郑开 on 2024/7/10.
//

#import <Foundation/Foundation.h>
#import "JUploadEnum.h"
#import "JUploadQiNiuCred.h"
#import "JUploadPreSignCred.h"
#import "JBaseUploader.h"

NS_ASSUME_NONNULL_BEGIN

@interface JUploaderFactory : NSObject

+ (JBaseUploader *)getUpload:(NSString *)localPath
                     ossType:(JUploadOssType)ossType
                   qiNiuCred:(JUploadQiNiuCred *)qiNiuCred
                 preSignCred:(JUploadPreSignCred *)preSignCred;

@end

NS_ASSUME_NONNULL_END
