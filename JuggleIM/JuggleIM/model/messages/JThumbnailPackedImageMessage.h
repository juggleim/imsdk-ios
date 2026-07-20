//
//  JThumbnailPackedImageMessage.h
// JuggleIM
//
//  Created by Nathan on 2024/5/13.
//

#import <JuggleIM/JuggleIM.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface JThumbnailPackedImageMessage : JMediaMessageContent
/// Thumbnail
@property (nonatomic, strong) UIImage *thumbnailImage;
/// Original image
@property (nonatomic, strong) UIImage *originalImage;
/// Image height
@property (nonatomic, assign) int height;
/// Image width
@property (nonatomic, assign) int width;
/// Image size, in KB
@property (nonatomic, assign) long long size;
/// Extra field
@property (nonatomic, copy) NSString *extra;

+ (instancetype)messageWithImage:(UIImage *)image;

+ (instancetype)messageWithImageData:(NSData *)imageData;

@end

NS_ASSUME_NONNULL_END
