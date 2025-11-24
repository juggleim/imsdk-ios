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
/// 缩略图远端地址
@property (nonatomic, copy) NSString *snapshotUrl;
/// 类型
@property (nonatomic, assign) JMomentMediaType type;
/// 高度
@property (nonatomic, assign) int height;
/// 宽度
@property (nonatomic, assign) int width;
/// 时长（用于视频）
@property (nonatomic, assign) int duration;

- (NSDictionary *)toDictionary;

+ (JMomentMedia *)mediaWith:(NSDictionary *)dictionary;

@end

NS_ASSUME_NONNULL_END
