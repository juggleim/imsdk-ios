//
//  JBaseUploader.h
// JuggleIM
//
//  Created by 郑开 on 2024/7/10.
//

#import <Foundation/Foundation.h>
#import "JUploaderProtocol.h"

NS_ASSUME_NONNULL_BEGIN

@interface JBaseUploader : NSObject

@property (nonatomic, copy) NSString * localPath;

@property (nonatomic, assign) long long progressCallBackInterval;

@property (nonatomic, assign) long long lastProgressCallBackTime;

@property (nonatomic, copy) void (^JUploadProgress)(int progress);

@property (nonatomic, copy) void (^JUploadSuccess)(NSString * url);

@property (nonatomic, copy) void (^JUploadError)(void);

@property (nonatomic, copy) void (^JUploadCancel)(void);

-(void)start;

-(void)cancel;

-(void)notifyProgress:(int)progress;

-(void)notifySuccess:(NSString *)url;

-(void)notifyFail;

-(void)notifyCancel;



@end

NS_ASSUME_NONNULL_END
