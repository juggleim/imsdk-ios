//
//  JNaviTask.h
// JuggleIM
//
//  Created by Nathan on 2024/5/24.
//

#import <Foundation/Foundation.h>
#import "JuggleIMConstInternal.h"

NS_ASSUME_NONNULL_BEGIN

@interface JNaviTask : NSObject

+ (instancetype)taskWithUrls:(NSArray <NSString *> *)urls
                      appKey:(NSString *)appKey
                       token:(NSString *)token
                     success:(void (^)(NSString *userId, NSArray<NSString *> *servers))success
                     failure:(void (^)(JErrorCodeInternal errorCode))failure;

- (void)start;

@end

NS_ASSUME_NONNULL_END
