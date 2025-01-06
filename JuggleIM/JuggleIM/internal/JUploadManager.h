//
//  JUploadManager.h
// JuggleIM
//
//  Created by 郑开 on 2024/7/9.
//

#import <Foundation/Foundation.h>
#import "JMessageUploadProvider.h"
#import "JIMCore.h"

@interface JUploadManager : NSObject<JMessageUploadProvider>

- (instancetype)initWithCore:(JIMCore *)core;

- (void)uploadLog:(NSString *)filePath
        messageId:(NSString *)messageId
         complete:(void (^)(void))completeBlock;

- (void)uploadImage:(UIImage *)image
            success:(void (^)(NSString * url))successBlock
              error:(void (^)(JErrorCode code))errorBlock;

@end

