//
//  JPreSignUploader.m
//  JetIM
//
//  Created by 郑开 on 2024/7/10.
//

#import "JPreSignUploader.h"
#import "JLogger.h"

@interface JPreSignUploader ()<NSURLSessionTaskDelegate>

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

-(void)star{
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
    //上传URL
    NSURL *uploadURL = [NSURL URLWithString:self.preSignCred.url];
    // 创建请求
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:uploadURL];
    [request setHTTPMethod:@"POST"];
    // 创建上传任务
    NSURLSessionConfiguration *configuration = [NSURLSessionConfiguration defaultSessionConfiguration];
    NSURLSession *session = [NSURLSession sessionWithConfiguration:configuration delegate:self delegateQueue:[NSOperationQueue mainQueue]];
    NSURLSessionUploadTask *uploadTask = [session uploadTaskWithRequest:request 
                                                               fromFile:fileURL
                                                      completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
        if (error) {
            NSLog(@"Upload failed: %@", error.localizedDescription);
        } else {
            NSLog(@"Upload successful");
            // 可以根据需要处理响应数据
            NSString *responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
            NSLog(@"Response: %@", responseString);
        }
    }];
    
    // 启动任务
    [uploadTask resume];
}

//获取上传进度
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didSendBodyData:(int64_t)bytesSent totalBytesSent:(int64_t)totalBytesSent totalBytesExpectedToSend:(int64_t)totalBytesExpectedToSend {
    double progress = (double)totalBytesSent / (double)totalBytesExpectedToSend;
    NSLog(@"Upload progress: %.2f%%", progress * 100);
}

@end
