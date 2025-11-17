//
//  JMomentManager.m
//  JuggleIM
//
//  Created by Fei Li on 2025/11/13.
//

#import "JMomentManager.h"
#import "JLogger.h"

#define jAppKey @"appkey"
#define jAuthorization @"Authorization"

@interface JMomentManager ()
@property (nonatomic, strong) JIMCore *core;
@property (nonatomic, strong) NSString *baseUrl;
@end

@implementation JMomentManager

- (instancetype)initWithCore:(JIMCore *)core {
    if (self = [super init]) {
        self.core = core;
    }
    return self;
}

- (void)addComment:(nonnull NSString *)momentId
   parentCommentId:(nullable NSString *)parentCommentId
           content:(nonnull NSString *)content
          complete:(nullable void (^)(JErrorCode, JMomentComment * _Nullable))completeBlock {
    NSDictionary *dic = @{
        @"post_id": momentId,
        @"parent_comment_id": parentCommentId,
        @"text": content
    };
    [self requestBySubUrl:@"/jim/posts/comments/add"
                   method:@"POST"
                   params:dic
                 complete:^(int errorCode, NSDictionary *dic) {
        
    }];
}

- (void)addMoment:(nonnull NSString *)content
        mediaList:(nullable NSArray<JMomentMedia *> *)mediaList
         complete:(nullable void (^)(JErrorCode, JMoment * _Nullable))completeBlock {
    NSMutableDictionary *contentDic = [NSMutableDictionary dictionary];
    if (mediaList.count > 0) {
        NSMutableArray *mediaListArray = [NSMutableArray array];
        for (JMomentMedia *media in mediaList) {
            NSDictionary *mediaDic = [media toDictionary];
            [mediaListArray addObject:mediaDic];
        }
        contentDic[@"medias"] = mediaListArray;
    }
    contentDic[@"text"] = content;
    NSDictionary *params = @{@"content": contentDic};
    
    [self requestBySubUrl:@"/jim/posts/add"
                   method:@"POST"
                   params:params
                 complete:^(int errorCode, NSDictionary *dic) {
        if (errorCode == JErrorCodeNone) {
            JMoment *moment = [JMoment new];
            moment.content = content;
            moment.mediaArray = mediaList;
//            moment.momentId = ;
//            moment.userInfo = ;
//            moment.createTime = ;
            if (completeBlock) {
                completeBlock(JErrorCodeNone, moment);
            }
        } else {
            if (completeBlock) {
                completeBlock(errorCode, nil);
            }
        }
    }];
}

- (void)addReaction:(nonnull NSString *)momentId
                key:(nonnull NSString *)key
           complete:(nullable void (^)(JErrorCode))completeBlock {
    [self requestBySubUrl:@"/jim/posts/reactions/add"
                   method:@"POST"
                   params:@{@"post_id": momentId, @"key": key}
                 complete:^(int errorCode, NSDictionary *dic) {
        
    }];
}

- (NSArray<JMoment *> * _Nonnull)getCachedMomentList:(nonnull JGetMomentOption *)option {
    NSMutableArray *result = [NSMutableArray array];
    return result;
}

- (void)getCommentList:(nonnull JGetMomentCommentOption *)option
              complete:(nullable void (^)(JErrorCode, NSArray<JMomentComment *> * _Nullable))completeBlock { 
    [self requestBySubUrl:@"/jim/posts/comments/list"
                   method:@"GET"
                   params:[option toDictionary]
                 complete:^(int errorCode, NSDictionary *dic) {
        
    }];
}

- (void)getMoment:(nonnull NSString *)momentId
         complete:(nullable void (^)(JErrorCode, JMoment * _Nullable))completeBlock {
    [self requestBySubUrl:@"/jim/posts/info"
                   method:@"GET"
                   params:@{@"post_id": momentId}
                 complete:^(int errorCode, NSDictionary *dic) {
        
    }];
}

- (void)getMomentList:(nonnull JGetMomentOption *)option
             complete:(nullable void (^)(JErrorCode, NSArray<JMoment *> * _Nullable))completeBlock {
    [self requestBySubUrl:@"/jim/posts/list"
                   method:@"GET"
                   params:[option toDictionary]
                 complete:^(int errorCode, NSDictionary *dic) {
        
    }];
}

- (void)getReactionList:(nonnull NSString *)momentId
               complete:(nullable void (^)(JErrorCode, NSArray<JMomentReaction *> * _Nullable))completeBlock {
    [self requestBySubUrl:@"/jim/posts/reactions/list"
                   method:@"GET"
                   params:@{@"post_id": momentId}
                 complete:^(int errorCode, NSDictionary *dic) {
        
    }];
}

- (void)removeComment:(nonnull NSString *)commentId
             complete:(nullable void (^)(JErrorCode))completeBlock {
    [self requestBySubUrl:@"/jim/posts/comments/del"
                   method:@"POST"
                   params:@{@"comment_ids": @[commentId]}
                 complete:^(int errorCode, NSDictionary *dic) {
        
    }];
}

- (void)removeMoment:(nonnull NSString *)momentId
            complete:(nullable void (^)(JErrorCode))completeBlock {
    [self requestBySubUrl:@"/jim/posts/del"
                   method:@"POST"
                   params:@{@"post_ids":@[momentId]}
                 complete:^(int errorCode, NSDictionary *dic) {
        if (completeBlock) {
            completeBlock(errorCode);
        }
    }];
}

- (void)removeReaction:(nonnull NSString *)momentId
                   key:(nonnull NSString *)key
              complete:(nullable void (^)(JErrorCode))completeBlock {
    [self requestBySubUrl:@"/jim/posts/reactions/del"
                   method:@"POST"
                   params:@{@"post_id": momentId, @"key": key}
                 complete:^(int errorCode, NSDictionary *dic) {
        
    }];
}

- (void)requestBySubUrl:(NSString *)subUrl
                 method:(NSString *)method
                 params:(NSDictionary *)params
               complete:(void (^)(int errorCode, NSDictionary *dic))completeBlock {
    NSString *urlString = [self.baseUrl stringByAppendingString:subUrl];
    if ([method isEqualToString:@"GET"]) {
        urlString = [self fetchGetUrl:urlString
                               params:params];
    }
    NSURL *url = [NSURL URLWithString:urlString];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    [request setHTTPMethod:method];
    [request setValue:self.core.appKey forHTTPHeaderField:jAppKey];
    [request setValue:self.core.token forHTTPHeaderField:jAuthorization];
    if ([method isEqualToString:@"POST"] && params.count > 0) {
        NSData *jsonData = [NSJSONSerialization dataWithJSONObject:params
                                                           options:0
                                                             error:NULL];
        if (jsonData) {
            request.HTTPBody = jsonData;
            [request setValue:@"application/json; charset=UTF-8" forHTTPHeaderField:@"Content-Type"];
        }
    }
    NSURLSessionDataTask *task = [[NSURLSession sharedSession] dataTaskWithRequest:request
                                                                 completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (error) {
            JLogE(@"Mmt-Request", @"error description is %@, url is %@", error.localizedDescription, url);
            if (completeBlock) {
                completeBlock(JErrorCodeMomentRequestError, nil);
            }
            return;
        }
        NSInteger statusCode = ((NSHTTPURLResponse *)response).statusCode;
        if (statusCode != 200) {
            JLogE(@"Mmt-Request", @"error http code is %ld, url is %@", (long)statusCode, url);
            if (completeBlock) {
                completeBlock(JErrorCodeMomentRequestError, nil);
            }
            return;
        }
        NSError *e = nil;
        NSDictionary *responseDic = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&e];
        if (e) {
            JLogE(@"Mmt-Request", @"json error description is %@, url is %@", e.localizedDescription, url);
            if (completeBlock) {
                completeBlock(JErrorCodeMomentRequestError, nil);
            }
            return;
        }
        if (responseDic) {
            id codeId = responseDic[@"code"];
            if ([codeId isKindOfClass:[NSNumber class]]) {
                int code = [(NSNumber *)codeId intValue];
                if (code != JErrorCodeNone) {
                    JLogE(@"Mmt-Request", @"response error, code is %i, url is %@", code, url);
                    if (completeBlock) {
                        completeBlock(code, nil);
                    }
                    return;
                }
            } else {
                JLogE(@"Mmt-Request", @"doesn't response a code");
                if (completeBlock) {
                    completeBlock(JErrorCodeMomentRequestError, nil);
                }
                return;
            }
            NSDictionary *dataDic = responseDic[@"data"];
            if (completeBlock) {
                completeBlock(JErrorCodeNone, dataDic);
            }
            return;
        }
        JLogE(@"Mmt-Request", @"unknown error");
        if (completeBlock) {
            completeBlock(JErrorCodeMomentRequestError, nil);
        }
    }];
    [task resume];
}

- (NSString *)fetchGetUrl:(NSString *)url
                   params:(NSDictionary *)params {
    // 1. 将原始 URL 转为 NSURLComponents（方便处理参数）
    NSURLComponents *components = [NSURLComponents componentsWithString:url];
    if (!components) {
        JLogE(@"Mmt-Request", @"fetch url error");
        return url;
    }

    // 2. 初始化参数数组（如果已有参数，先保留）
    NSMutableArray *queryItems = [NSMutableArray arrayWithArray:components.queryItems ?: @[]];

    // 3. 遍历字典，添加参数到数组
    [params enumerateKeysAndObjectsUsingBlock:^(NSString *key, id value, BOOL *stop) {
        // 将参数值转为字符串（支持数字、布尔值、字符串等）
        NSString *valueString = [NSString stringWithFormat:@"%@", value];
        // 创建查询参数项（会自动处理编码）
        NSURLQueryItem *item = [NSURLQueryItem queryItemWithName:key value:valueString];
        [queryItems addObject:item];
    }];

    // 4. 将参数数组赋值给 components
    components.queryItems = queryItems;

    // 5. 生成最终的 URL 字符串
    NSString *finalURLString = components.string;
    return finalURLString;
}

- (NSString *)baseUrl {
    if (!_baseUrl) {
        NSString *server = @"";
        if (self.core.servers.count > 0) {
            server = self.core.servers[0];
        }
        if ([server hasPrefix:@"ws://"]) {
            server = [server stringByReplacingOccurrencesOfString:@"ws://" withString:@"http://" options:NSAnchoredSearch range:NSMakeRange(0, 5)];
        } else if ([server hasPrefix:@"wss://"]) {
            server = [server stringByReplacingOccurrencesOfString:@"wss://" withString:@"http://" options:NSAnchoredSearch range:NSMakeRange(0, 6)];
        }
        _baseUrl = server;
    }
    return _baseUrl;
}

@end
