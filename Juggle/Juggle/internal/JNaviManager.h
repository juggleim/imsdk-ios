//
//  JNaviManager.h
//  Juggle
//
//  Created by Nathan on 2023/12/29.
//

#import <Foundation/Foundation.h>
#import "JuggleConstInternal.h"

NS_ASSUME_NONNULL_BEGIN

@interface JNaviManager : NSObject

+ (void)requestNavi:(NSURL *)url
             appKey:(NSString *)appKey
              token:(NSString *)token
            success:(void (^)(NSString *userId, NSArray<NSString *> *servers))success
            failure:(void (^)(JErrorCodeInternal errorCode))failure;

@end

NS_ASSUME_NONNULL_END
