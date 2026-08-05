#import "UIImage+JImage.h"

@implementation UIImage (JImage)

/**
 *  Resize image
 *
 *  @param image      original image
 *  @param targetSize target size
 *
 *  @return resized image
 */
+ (UIImage *)image:(UIImage *)image byScalingToSize:(CGSize)targetSize {
    UIImage *sourceImage = image;
    UIImage *newImage = nil;

    UIGraphicsBeginImageContext(targetSize);

    CGRect thumbnailRect = CGRectZero;
    thumbnailRect.origin = CGPointZero;
    thumbnailRect.size.width = targetSize.width;
    thumbnailRect.size.height = targetSize.height;

    [sourceImage drawInRect:thumbnailRect];

    newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();

    return newImage;
}

+ (UIImage *)getSubImage:(UIImage *)originImage
                    Rect:(CGRect)rect
        imageOrientation:(UIImageOrientation)imageOrientation {
    CGImageRef subImageRef = CGImageCreateWithImageInRect(originImage.CGImage, rect);
    CGRect smallBounds = CGRectMake(0, 0, CGImageGetWidth(subImageRef), CGImageGetHeight(subImageRef));

    UIGraphicsBeginImageContext(smallBounds.size);
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextDrawImage(context, smallBounds, subImageRef);
    UIImage *smallImage = [UIImage imageWithCGImage:subImageRef scale:1.f orientation:imageOrientation];
    CGImageRelease(subImageRef);
    UIGraphicsEndImageContext();
    return smallImage;
}

+ (UIImage *)scaleImage:(UIImage *)Image toScale:(float)scaleSize {
    UIGraphicsBeginImageContext(CGSizeMake(Image.size.width * scaleSize, Image.size.height * scaleSize));
    [Image drawInRect:CGRectMake(0, 0, Image.size.width * scaleSize, Image.size.height * scaleSize)];
    UIImage *scaledImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return scaledImage;
}
@end
