//
//  JPreSignUploader.h
// JuggleIM
//
//  Created by 郑开 on 2024/7/10.
//

#import "JBaseUploader.h"
#import "JUploadPreSignCred.h"
#import "JUploadEnum.h"

NS_ASSUME_NONNULL_BEGIN

@interface JPreSignUploader : JBaseUploader

@property (nonatomic, strong) JUploadPreSignCred * preSignCred;

-(instancetype)initWith:(NSString *)localPath 
            preSignCred:(JUploadPreSignCred *)preSignCred
                ossType:(JUploadOssType)ossType;

@end

NS_ASSUME_NONNULL_END
