#import <UIKit/UIKit.h>

@interface UIImage (JImage)

/**
 *  Resize image
 *
 *  @param image      original image
 *  @param targetSize target size
 *
 *  @return resized image
 */
+ (UIImage *)image:(UIImage *)image byScalingToSize:(CGSize)targetSize;

+ (UIImage *)getSubImage:(UIImage *)originImage Rect:(CGRect)rect imageOrientation:(UIImageOrientation)imageOrientation;

+ (UIImage *)scaleImage:(UIImage *)Image toScale:(float)scaleSize;
@end
