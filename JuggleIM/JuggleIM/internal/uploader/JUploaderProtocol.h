//
//  JUploaderProtocol.h
// JuggleIM
//
//  Created by 郑开 on 2024/7/10.
//

#import <Foundation/Foundation.h>

@protocol JUploaderProtocol <NSObject>

@property (nonatomic, copy) void (^JUploadProgress)(int progress);

@property (nonatomic, copy) void (^JUploadSuccess)(NSString * url);

@property (nonatomic, copy) void (^JUploadError)(void);

@property (nonatomic, copy) void (^JUploadCancel)(void);

-(void)star;

-(void)cancel;

@end
