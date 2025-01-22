#import <UIKit/UIKit.h>

@interface UIImage (JImage)

/**
 *  修改图片size
 *
 *  @param image      原图片
 *  @param targetSize 要修改的size
 *
 *  @return 修改后的图片
 */
+ (UIImage *)image:(UIImage *)image byScalingToSize:(CGSize)targetSize;

+ (UIImage *)getSubImage:(UIImage *)originImage Rect:(CGRect)rect imageOrientation:(UIImageOrientation)imageOrientation;

+ (UIImage *)scaleImage:(UIImage *)Image toScale:(float)scaleSize;
@end
