//
//  JNaviTask.m
//  JetIM
//
//  Created by Nathan on 2024/5/24.
//

#import "JNaviTask.h"

#define jMaxConcurrentCount 5
#define jNaviServerSuffix @"/navigator/general"
#define jNaviAppKey @"x-appkey"
#define jNaviToken @"x-token"
#define jNaviData @"data"
#define jNaviUserId @"user_id"
#define jNaviServers @"servers"

@interface JNaviTask ()
@property (nonatomic, strong) NSOperationQueue *requestQueue;
@property (nonatomic, strong) NSArray<NSString *> *urls;
@property (nonatomic, copy) NSString *appKey;
@property (nonatomic, copy) NSString *token;
@property (nonatomic, copy) void (^successBlock)(NSString * _Nonnull, NSArray<NSString *> * _Nonnull);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal);
@property (nonatomic, assign) BOOL isFinish;
@end

@implementation JNaviTask

+ (instancetype)taskWithUrls:(NSArray<NSString *> *)urls
                      appKey:(NSString *)appKey
                       token:(NSString *)token
                     success:(void (^)(NSString * _Nonnull, NSArray<NSString *> * _Nonnull))success
                     failure:(void (^)(JErrorCodeInternal))failure {
    JNaviTask *task = [[JNaviTask alloc] init];
    task.urls = urls;
    task.appKey = appKey;
    task.token = token;
    task.successBlock = success;
    task.errorBlock = failure;
    NSOperationQueue *queue = [[NSOperationQueue alloc] init];
    queue.maxConcurrentOperationCount = jMaxConcurrentCount;
    task.requestQueue = queue;
    
    return task;
}

- (void)start {
    for (NSString *url in self.urls) {
        NSInvocationOperation *operation = [[NSInvocationOperation alloc] initWithTarget:self
                                                                                selector:@selector(requestNavi:)
                                                                                  object:url];
        [self.requestQueue addOperation:operation];
    }
}

- (void)requestNavi:(NSString *)url {
    NSURL *u = [NSURL URLWithString:[NSString stringWithFormat:@"%@%@", url, jNaviServerSuffix]];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:u];
    [request setHTTPMethod:@"GET"];
    [request setValue:self.appKey forHTTPHeaderField:jNaviAppKey];
    [request setValue:self.token forHTTPHeaderField:jNaviToken];
    NSURLSessionDataTask *task = [[NSURLSession sharedSession] dataTaskWithRequest:request
                                                                 completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        @synchronized (self) {
            if (self.isFinish) {
                return;
            }
            self.isFinish = YES;
            if (error) {
                NSLog(@"[JetIM] request navi error, description is %@", error.localizedDescription);
                if (self.errorBlock) {
                    self.errorBlock(JErrorCodeInternalNaviFailure);
                }
                return;
            }
            NSInteger statusCode = ((NSHTTPURLResponse *)response).statusCode;
            if (statusCode != 200) {
                NSLog(@"[JetIM] request navi error, http code is %ld", (long)statusCode);
                if (self.errorBlock) {
                    if (statusCode == 401) {
                        self.errorBlock(JErrorCodeInternalTokenIllegal);
                    } else {
                        self.errorBlock(JErrorCodeInternalNaviFailure);
                    }
                }
                return;
            }
            NSError *e = nil;
            NSDictionary *responseDic = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&e];
            if (e) {
                NSLog(@"[JetIM] request navi json error, description is %@", e.localizedDescription);
                if (self.errorBlock) {
                    self.errorBlock(JErrorCodeInternalNaviFailure);
                }
                return;
            }
            if (responseDic) {
                NSDictionary *dataDic = responseDic[jNaviData];
                if (dataDic) {
                    NSString *userId = dataDic[jNaviUserId]?:@"";
                    NSArray *servers = dataDic[jNaviServers];
                    if (self.successBlock) {
                        self.successBlock(userId, servers);
                        return;
                    }
                }
            }
            NSLog(@"[JetIM] request navi unknown error");
            if (self.errorBlock) {
                self.errorBlock(JErrorCodeInternalNaviFailure);
            }
        }
    }];
    
    [task resume];
}

@end
