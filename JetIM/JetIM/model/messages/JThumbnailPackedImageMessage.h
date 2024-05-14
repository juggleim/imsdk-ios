//
//  JThumbnailPackedImageMessage.h
//  JetIM
//
//  Created by Nathan on 2024/5/13.
//

#import <JetIM/JetIM.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface JThumbnailPackedImageMessage : JMediaMessageContent
/// 缩略图
@property (nonatomic, strong) UIImage *thumbnailImage;
/// 原图
@property (nonatomic, strong) UIImage *originalImage;
/// 图片高度
@property (nonatomic, assign) int height;
/// 图片宽度
@property (nonatomic, assign) int width;
/// 图片大小，单位：KB
@property (nonatomic, assign) long long size;
/// 扩展字段
@property (nonatomic, copy) NSString *extra;

+ (instancetype)messageWithImage:(UIImage *)image;

+ (instancetype)messageWithImageData:(NSData *)imageData;

@end

NS_ASSUME_NONNULL_END
