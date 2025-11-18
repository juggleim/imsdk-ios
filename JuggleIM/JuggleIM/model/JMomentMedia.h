//
//  JMomentMedia.h
//  JuggleIM
//
//  Created by Fei Li on 2025/11/11.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, JMomentMediaType) {
    /*
     图片
     */
    JMomentMediaTypeImage,
    
    /*
     视频
     */
    JMomentMediaTypeVideo
};

@interface JMomentMedia : NSObject

/// 远端地址
@property (nonatomic, copy) NSString *url;
/// 类型
@property (nonatomic, assign) JMomentMediaType type;

- (NSDictionary *)toDictionary;

@end

NS_ASSUME_NONNULL_END
