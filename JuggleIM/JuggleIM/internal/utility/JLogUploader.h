//
//  JLogUploader.h
// JuggleIM
//
//  Created by Nathan on 2024/6/5.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JLogUploader : NSObject

- (void)upload:(NSString *)filePath
        appKey:(NSString *)appKey
         token:(NSString *)token
      complete:(void(^)(void))completion;

@end

NS_ASSUME_NONNULL_END
