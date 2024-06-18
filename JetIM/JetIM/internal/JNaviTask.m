//
//  JNaviTask.m
//  JetIM
//
//  Created by Nathan on 2024/5/24.
//

#import "JNaviTask.h"
#import "JLogger.h"

#define jMaxConcurrentCount 5
#define jNaviServerSuffix @"/navigator/general"
#define jNaviData @"data"
#define jNaviUserId @"user_id"
#define jNaviServers @"servers"

typedef NS_ENUM(NSUInteger, JTaskStatus) {
    JTaskStatusIdle,
    JTaskStatusFailure,
    JTaskStatusSuccess
};

@interface JNaviTask ()
@property (nonatomic, strong) NSOperationQueue *requestQueue;
@property (nonatomic, strong) NSMutableDictionary <NSString *, NSNumber *>*requestDic;
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
    if (urls.count > jMaxConcurrentCount) {
        urls = [urls subarrayWithRange:NSMakeRange(0, jMaxConcurrentCount)];
    }
    NSMutableDictionary *dic = [NSMutableDictionary dictionary];
    for (NSString *url in urls) {
        [dic setObject:@(JTaskStatusIdle) forKey:url];
    }
    task.requestDic = dic;
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
    JLogI(@"NAV-Start", @"urls is %@", self.requestDic.allKeys);
    if (self.requestDic.count == 0 && self.errorBlock) {
        self.errorBlock(JErrorCodeInternalServerSetError);
        return;
    }
    for (NSString *url in self.requestDic.allKeys) {
        NSInvocationOperation *operation = [[NSInvocationOperation alloc] initWithTarget:self
                                                                                selector:@selector(requestNavi:)
                                                                                  object:url];
        [self.requestQueue addOperation:operation];
    }
}

- (void)requestNavi:(NSString *)url {
    JLogI(@"NAV-Request", @"url is %@", url);
    NSURL *u = [NSURL URLWithString:[NSString stringWithFormat:@"%@%@", url, jNaviServerSuffix]];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:u];
    [request setHTTPMethod:@"GET"];
    [request setValue:self.appKey forHTTPHeaderField:jNaviAppKey];
    [request setValue:self.token forHTTPHeaderField:jNaviToken];
    NSURLSessionDataTask *task = [[NSURLSession sharedSession] dataTaskWithRequest:request
                                                                 completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (error) {
            JLogE(@"NAV-Request", @"error description is %@, url is %@", error.localizedDescription, url);
            [self responseError:JErrorCodeInternalNaviFailure url:url];
            return;
        }
        NSInteger statusCode = ((NSHTTPURLResponse *)response).statusCode;
        if (statusCode != 200) {
            JLogE(@"NAV-Request", @"error http code is %ld", (long)statusCode);
            if (statusCode == 401) {
                [self responseError:JErrorCodeInternalTokenIllegal url:url];
            } else {
                [self responseError:JErrorCodeInternalNaviFailure url:url];
            }
            return;
        }
        NSError *e = nil;
        NSDictionary *responseDic = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&e];
        if (e) {
            JLogE(@"NAV-Request", @"json error description is %@", e.localizedDescription);
            [self responseError:JErrorCodeInternalNaviFailure url:url];
            return;
        }
        if (responseDic) {
            NSDictionary *dataDic = responseDic[jNaviData];
            if (dataDic) {
                NSString *userId = dataDic[jNaviUserId]?:@"";
                NSArray *servers = dataDic[jNaviServers];
                [self responseSuccess:url userId:userId servers:servers];
                return;
            }
        }
        JLogE(@"NAV-Request", @"unknown error");
        [self responseError:JErrorCodeInternalNaviFailure url:url];
    }];
    
    [task resume];
}

- (void)responseError:(JErrorCodeInternal)code
                  url:(NSString *)url {
    BOOL allFailed = YES;
    @synchronized (self) {
        if (self.isFinish) {
            return;
        }
        [self.requestDic setObject:@(JTaskStatusFailure) forKey:url];
        for (NSNumber *status in self.requestDic.allValues) {
            if (status.intValue != JTaskStatusFailure) {
                allFailed = NO;
                break;
            }
        }
    }
    if (allFailed && self.errorBlock) {
        self.errorBlock(code);
    }
}

- (void)responseSuccess:(NSString *)url
                 userId:(NSString *)userId
                servers:(NSArray<NSString *> *)servers {
    @synchronized (self) {
        if (self.isFinish) {
            JLogI(@"NAV-Request", @"compete fail, url is %@", url);
            return;
        }
        self.isFinish = YES;
        [self.requestDic setObject:@(JTaskStatusSuccess) forKey:url];
    }
    JLogI(@"NAV-Request", @"compete success url is %@", url);
    if (self.successBlock) {
        self.successBlock(userId, servers);
    }
}

@end
