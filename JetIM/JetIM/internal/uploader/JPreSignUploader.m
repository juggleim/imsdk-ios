//
//  JPreSignUploader.m
//  JetIM
//
//  Created by 郑开 on 2024/7/10.
//

#import "JPreSignUploader.h"
#import "JLogger.h"

@interface JPreSignUploader ()<NSURLSessionTaskDelegate>

@property (nonatomic, strong) NSURLSessionUploadTask * uploadTask;
@end


@implementation JPreSignUploader

-(instancetype)initWith:(NSString *)localPath
            preSignCred:(JUploadPreSignCred *)preSignCred{
    if(self = [super init]){
        self.localPath = localPath;
        self.preSignCred = preSignCred;
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
    
    //带扩展名的文件名
    NSString *fileName = [self.localPath lastPathComponent];
    
    if(fileName == nil || fileName.length == 0){
        JLogE(@"J-Uploader", @"PreSignUploader error, fileName is empty");
        [self notifyFail];
        return;
    }
    
    //文件URL
    NSURL *fileURL = [NSURL fileURLWithPath:self.localPath];
    NSData * data = [NSData dataWithContentsOfURL:fileURL];
    if(data == nil || data.length == 0){
        JLogE(@"J-Uploader", @"PreSignUploader error, update File is empty");
        [self notifyFail];
        return;
    }
    //上传URL
    NSURL *uploadURL = [NSURL URLWithString:self.preSignCred.url];
    // 创建请求
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:uploadURL];
    [request setHTTPMethod:@"PUT"];
    // 创建上传任务
    NSURLSessionConfiguration *configuration = [NSURLSessionConfiguration defaultSessionConfiguration];
    NSURLSession * session = [NSURLSession sessionWithConfiguration:configuration delegate:self delegateQueue:[NSOperationQueue mainQueue]];
    self.uploadTask = [session uploadTaskWithRequest:request
                                            fromData:data
                                   completionHandler:^(NSData * _Nullable responseData, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (error == nil && ((NSHTTPURLResponse *)response).statusCode == 200) {
            NSArray * array = [self.preSignCred.url componentsSeparatedByString:@"?"];
            if(array.count >= 2){
                NSString * modifiedUrl = array.firstObject;
                JLogE(@"J-Uploader", @"PreSignUploader success, url is %@",modifiedUrl);
                [self notifySuccess:modifiedUrl];
            }
        } else {
            JLogE(@"J-Uploader", @"PreSignUploader error, responseCode is %li, error is %@",((NSHTTPURLResponse *)response).statusCode,  error.localizedDescription);
            [self notifyFail];
        }
        [session invalidateAndCancel];
    }];
    
    // 启动任务
    [self.uploadTask resume];
    
}

-(void)cancel{
    [self.uploadTask cancel];
    JLogE(@"J-Uploader", @"PreSignUploader canceled");
    [self notifyCancel];
}

//获取上传进度
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didSendBodyData:(int64_t)bytesSent totalBytesSent:(int64_t)totalBytesSent totalBytesExpectedToSend:(int64_t)totalBytesExpectedToSend {
    double progress = (double)totalBytesSent / (double)totalBytesExpectedToSend;
    int IntProgress = progress * 100;
    [self notifyProgress:IntProgress];
}

@end
