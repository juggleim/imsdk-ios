//
//  JPostMedia.h
//  JuggleIM
//
//  Created by Fei Li on 2025/11/11.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, JPostMediaType) {
    /*
     图片
     */
    JPostMediaTypeImage,
    
    /*
     视频
     */
    JPostMediaTypeVideo
};

@interface JPostMedia : NSObject

@property (nonatomic, copy) NSString *url;
@property (nonatomic, assign) JPostMediaType type;

@end

NS_ASSUME_NONNULL_END
