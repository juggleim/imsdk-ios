//
//  JDownloader.h
// JuggleIM
//
//  Created by Nathan on 2024/7/17.
//

#import <Foundation/Foundation.h>
#import "JuggleIMConst.h"

NS_ASSUME_NONNULL_BEGIN

@interface JDownloader : NSObject

- (instancetype)initWithUrl:(NSString *)url
                       path:(NSString *)localPath
                   progress:(void (^)(int progress))progressBlock
                    success:(void (^)(NSString *localPath))successBlock
                      error:(void (^)(JErrorCode errorCode))errorBlock;

- (void)start;
@end

NS_ASSUME_NONNULL_END
