//
//  JImageMessage.m
// JuggleIM
//
//  Created by Nathan on 2023/12/3.
//

#import "JImageMessage.h"
#import <UIKit/UIKit.h>
#import "JUtility.h"

#define kImageType @"jg:img"
#define kDigest @"[Image]"
#define jLocalPath @"local"
#define jThumbnailLocalPath @"thumbnailLocalPath"
#define jURL @"url"
#define jThumbnail @"thumbnail"
#define jImageHeight @"height"
#define jImageWidth @"width"
#define jImageSize @"size"
#define jImageExtra @"extra"

@implementation JImageMessage

- (instancetype)initWithImage:(UIImage *)image {
    NSString *fileName = [NSString stringWithFormat:@"%lld.jpg", (long long)[NSDate date].timeIntervalSince1970];
    return [self initWithImage:image fileName:fileName];
}

- (instancetype)initWithImage:(UIImage *)image fileName:(NSString *)fileName {
    if (self = [super init]) {
        NSData *imgData = UIImageJPEGRepresentation(image, 0.85);
        
        NSString *mediaPath = [JUtility mediaPath:JMediaTypeImage];
        if (![[NSFileManager defaultManager] fileExistsAtPath:mediaPath]) {
            [[NSFileManager defaultManager] createDirectoryAtPath:mediaPath
                                      withIntermediateDirectories:YES
                                                       attributes:nil
                                                            error:nil];
        }
        if (fileName.length == 0) {
            fileName = [NSString stringWithFormat:@"%lld.jpg", (long long)[NSDate date].timeIntervalSince1970];
        }
        NSString *localPath = [mediaPath stringByAppendingPathComponent:fileName];
        while ([[NSFileManager defaultManager] fileExistsAtPath:localPath]) {
            localPath = [self appendFileName:localPath];
        }
        [imgData writeToFile:localPath atomically:YES];
        self.localPath = localPath;
        UIImage *thumbnail = [JUtility generateThumbnail:image targetSize:CGSizeMake(JThumbnailWidth, JThumbnailHeight)];
        NSData *thumbData = UIImageJPEGRepresentation(thumbnail, jThumbnailQuality);
        NSString *thumbName = [NSString stringWithFormat:@"thumb_%@", fileName];
        NSString *thumbPath = [mediaPath stringByAppendingPathComponent:thumbName];
        while ([[NSFileManager defaultManager] fileExistsAtPath:thumbPath]) {
            thumbPath = [self appendFileName:thumbPath];
        }
        [thumbData writeToFile:thumbPath atomically:YES];
        self.thumbnailLocalPath = thumbPath;
        self.width = image.size.width;
        self.height = image.size.height;
    }
    return self;
}

+ (NSString *)contentType {
    return kImageType;
}

- (NSData *)encode{
    //绝对路径转换成相对路径
    NSString * localPath = [self.localPath stringByAbbreviatingWithTildeInPath];
    
    NSString * thumbnailLocalPath = [self.thumbnailLocalPath stringByAbbreviatingWithTildeInPath];

    
    NSDictionary * dic = @{jURL:self.url?:@"",
                           jThumbnail:self.thumbnailUrl?:@"",
                           jLocalPath:localPath?:@"",
                           jImageWidth:@(self.width),
                           jImageHeight:@(self.height),
                           jImageSize:@(self.size),
                           jImageExtra:self.extra?:@"",
                           jThumbnailLocalPath:thumbnailLocalPath?:@""};
    NSData *data = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    return data;
}

- (void)decode:(NSData *)data{
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    
    //相对路径转换成绝对路径

    NSString * localPath = json[jLocalPath]?:@"";
    if(localPath.length > 0){
        self.localPath = [localPath stringByExpandingTildeInPath];
    }
    NSString * thumbnailLocalPath = json[jThumbnailLocalPath]?:@"";
    if(thumbnailLocalPath.length > 0){
        self.thumbnailLocalPath = [thumbnailLocalPath stringByExpandingTildeInPath];
    }
    
    self.url = json[jURL]?:@"";
    self.thumbnailUrl = json[jThumbnail]?:@"";
    id obj = json[jImageWidth];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.width = [(NSNumber *)obj intValue];
    }
    obj = json[jImageHeight];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.height = [(NSNumber *)obj intValue];
    }
    obj = json[jImageSize];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.size = [(NSNumber *)obj longLongValue];
    }
    self.extra = json[jImageExtra]?:@"";
}

- (NSString *)conversationDigest {
    return kDigest;
}

- (NSString *)appendFileName:(NSString *)filePath {
    // 获取文件扩展名
    NSString *extension = [filePath pathExtension];
    
    if (extension.length == 0) {
        // 如果没有扩展名，直接在末尾添加"1"
        return [filePath stringByAppendingString:@"1"];
    } else {
        // 去掉扩展名的文件路径
        NSString *pathWithoutExtension = [filePath stringByDeletingPathExtension];
        // 在文件名后添加"1"，再加上原来的扩展名
        return [pathWithoutExtension stringByAppendingFormat:@"1.%@", extension];
    }
}

@end
