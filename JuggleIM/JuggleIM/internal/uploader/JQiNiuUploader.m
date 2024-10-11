//
//  JQiNiuUploader.m
// JuggleIM
//
//  Created by 郑开 on 2024/7/10.
//

#import "JQiNiuUploader.h"

@implementation JQiNiuUploader


-(instancetype)initWith:(NSString *)localPath
              qiNiuCred:(JUploadQiNiuCred *)qiNiuCred {
    if(self = [super init]){
        self.localPath = localPath;
        self.qiNiuCred = qiNiuCred;
    }
    return self;
}

@end
