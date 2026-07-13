//
//  JImageMessage.h
// JuggleIM
//
//  Created by Nathan on 2023/12/3.
//

#import <JuggleIM/JuggleIM.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface JImageMessage : JMediaMessageContent

- (instancetype)initWithImage:(UIImage *)image;

- (instancetype)initWithImage:(UIImage *)image
                     fileName:(NSString *)fileName;

///Thumbnail local path
@property (nonatomic, copy) NSString *thumbnailLocalPath;
/// Remote address of the thumbnail
@property (nonatomic, copy) NSString *thumbnailUrl;
/// Image height
@property (nonatomic, assign) int height;
/// Image width
@property (nonatomic, assign) int width;
/// Image size, in bytes
@property (nonatomic, assign) long long size;
/// Extra field
@property (nonatomic, copy) NSString *extra;
@end

NS_ASSUME_NONNULL_END
