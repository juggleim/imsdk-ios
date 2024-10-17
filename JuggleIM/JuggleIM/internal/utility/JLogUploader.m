//
//  JLogUploader.m
// JuggleIM
//
//  Created by Nathan on 2024/6/5.
//

#import "JLogUploader.h"
#import "JLogger.h"
#import "JuggleIMConstInternal.h"

#define jUploadUrl @""
#define jCode @"code"
#define jBoundary @"aoangindighy-gne"

@implementation JLogUploader

- (void)upload:(NSString *)filePath
        appKey:(nonnull NSString *)appKey
         token:(nonnull NSString *)token
      complete:(nonnull void (^)(void))completion {
    NSURL *u = [NSURL URLWithString:jUploadUrl];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:u];
    [request setHTTPMethod:@"POST"];
    [request setValue:[NSString stringWithFormat:@"multipart/form-data; boundary=%@", jBoundary] forHTTPHeaderField:@"Content-Type"];
    [request setValue:appKey forHTTPHeaderField:jNaviAppKey];
    [request setValue:token forHTTPHeaderField:jNaviToken];
    
    NSURLSessionUploadTask *task = [[NSURLSession sharedSession] uploadTaskWithRequest:request
                                                                              fromData:[self getBodyData:filePath]
                                                                     completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (error) {
            JLogE(@"J-Logger", @"error description is %@", error.localizedDescription);
            completion();
            return;
        }
        NSInteger statusCode = ((NSHTTPURLResponse *)response).statusCode;
        if (statusCode != 200) {
            JLogE(@"J-Logger", @"error http code is %ld", (long)statusCode);
            completion();
            return;
        }
        NSError *e = nil;
        NSDictionary *responseDic = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&e];
        if (e) {
            JLogE(@"J-Logger", @"json error description is %@", e.localizedDescription);
            completion();
            return;
        }
        if (responseDic) {
            NSNumber *code = responseDic[jCode];
            int c = [code intValue];
            if (c != 0) {
                completion();
                JLogE(@"J-Logger", @"error code is %d", c);
            } else {
                completion();
                JLogI(@"J-Logger", @"upload success");
            }
        } else {
            completion();
            JLogE(@"J-Logger", @"unknown error");
        }
    }];
    
    [task resume];
}

- (NSData *)getBodyData:(NSString *)filePath {
    NSMutableString *startStr = [[NSMutableString alloc] init];
    [startStr appendFormat:@"--%@\r\n", jBoundary];
    [startStr appendFormat:@"Content-Disposition: form-data; name=\"log\"; filename=\"fileLog.gzip\"\r\n"];
    [startStr appendFormat:@"Content-Type: application/octet-stream\r\n\r\n"];

    NSMutableString *endStr = [[NSMutableString alloc] init];
    [endStr appendFormat:@"\r\n--%@--\r\n", jBoundary];

    NSMutableData *uploadData = [NSMutableData data];
    [uploadData appendData:[startStr dataUsingEncoding:NSUTF8StringEncoding]];
    [uploadData appendData:[NSData dataWithContentsOfFile:filePath]];
    [uploadData appendData:[endStr dataUsingEncoding:NSUTF8StringEncoding]];
    return uploadData;
}

@end
