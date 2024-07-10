//
//  JQiNiuUploader.h
//  JetIM
//
//  Created by 郑开 on 2024/7/10.
//

#import "JBaseUploader.h"
#import "JUploadQiNiuCred.h"
NS_ASSUME_NONNULL_BEGIN

@interface JQiNiuUploader : JBaseUploader

@property (nonatomic, strong) JUploadQiNiuCred * qiNiuCred;

-(instancetype)initWith:(NSString *)localPath
              qiNiuCred:(JUploadQiNiuCred *)qiNiuCred;

@end

NS_ASSUME_NONNULL_END
