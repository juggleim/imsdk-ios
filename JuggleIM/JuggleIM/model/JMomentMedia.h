//
//  JMomentMedia.h
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

@interface JMomentMedia : NSObject

/// 远端地址
@property (nonatomic, copy) NSString *url;
/// 类型
@property (nonatomic, assign) JPostMediaType type;

- (NSDictionary *)toDictionary;

@end

NS_ASSUME_NONNULL_END
