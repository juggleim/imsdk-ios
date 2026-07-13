//
//  JPreSignUploader.m
// JuggleIM
//
//  Created by Zheng Kai on 2024/7/10.
//

#import "JPreSignUploader.h"
#import "JLogger.h"

@interface JPreSignUploader ()<NSURLSessionTaskDelegate>
@property (nonatomic, strong) NSURLSessionUploadTask * uploadTask;
@property (nonatomic, assign) JUploadOssType ossType;
@end


@implementation JPreSignUploader

-(instancetype)initWith:(NSString *)localPath
            preSignCred:(JUploadPreSignCred *)preSignCred
                ossType:(JUploadOssType)ossType {
    if(self = [super init]){
        self.localPath = localPath;
        self.preSignCred = preSignCred;
        self.ossType = ossType;
    }
    return self;
}

-(void)start{
    if(self.localPath == nil || self.localPath.length == 0){
        JLogE(@"J-Uploader", @"PreSignUploader error, localPath is empty");
        [self notifyFail];
        return;
    }
    if(self.preSignCred == nil || self.preSignCred.url == nil || self.preSignCred.url.length == 0){
        JLogE(@"J-Uploader", @"PreSignUploader error, mPreSignCred is null or empty");
        [self notifyFail];
        return;
    }
    
    //Filename with extension.
    NSString *fileName = [self.localPath lastPathComponent];
    
    if(fileName == nil || fileName.length == 0){
        JLogE(@"J-Uploader", @"PreSignUploader error, fileName is empty");
        [self notifyFail];
        return;
    }
    
    //File URL.
    NSURL *fileURL = [NSURL fileURLWithPath:self.localPath];
    NSData * data = [NSData dataWithContentsOfURL:fileURL];
    if(data == nil || data.length == 0){
        JLogE(@"J-Uploader", @"PreSignUploader error, update File is empty");
        [self notifyFail];
        return;
    }
    //Upload URL.
    NSURL *uploadURL = [NSURL URLWithString:self.preSignCred.url];
    // Create request.
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:uploadURL];
    [request setHTTPMethod:@"PUT"];
    if (self.ossType == JUploadOssType_S3) {
        [request setValue:@"public-read" forHTTPHeaderField:@"x-amz-acl"];
    }
    // Create upload task.
    NSURLSessionConfiguration *configuration = [NSURLSessionConfiguration defaultSessionConfiguration];
    NSURLSession * session = [NSURLSession sessionWithConfiguration:configuration delegate:self delegateQueue:[NSOperationQueue mainQueue]];
    self.uploadTask = [session uploadTaskWithRequest:request
                                            fromData:data
                                   completionHandler:^(NSData * _Nullable responseData, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (error == nil && ((NSHTTPURLResponse *)response).statusCode == 200) {
            NSString *mediaUrl = self.preSignCred.downloadUrl;
            if (mediaUrl.length == 0) {
                NSArray * array = [self.preSignCred.url componentsSeparatedByString:@"?"];
                if(array.count >= 2){
                    mediaUrl = array.firstObject;
                    JLogE(@"J-Uploader", @"PreSignUploader success, url is %@", mediaUrl);
                }
            }
            [self notifySuccess:mediaUrl];
        } else {
            JLogE(@"J-Uploader", @"PreSignUploader error, responseCode is %li, error is %@",((NSHTTPURLResponse *)response).statusCode,  error.localizedDescription);
            [self notifyFail];
        }
        [session invalidateAndCancel];
    }];
    
    // Start task.
    [self.uploadTask resume];
    
}

-(void)cancel{
    [self.uploadTask cancel];
    JLogE(@"J-Uploader", @"PreSignUploader canceled");
    [self notifyCancel];
}

//Get upload progress.
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didSendBodyData:(int64_t)bytesSent totalBytesSent:(int64_t)totalBytesSent totalBytesExpectedToSend:(int64_t)totalBytesExpectedToSend {
    double progress = (double)totalBytesSent / (double)totalBytesExpectedToSend;
    int IntProgress = progress * 100;
    [self notifyProgress:IntProgress];
}

@end
