//
//  JThumbnailPackedImageMessage.m
//  JetIM
//
//  Created by Nathan on 2024/5/13.
//

#import "JThumbnailPackedImageMessage.h"
#import "JUtility.h"
#import "JetIMConstInternal.h"
#import "objc/runtime.h"

#define jTPImageType @"jg:tpimg"
#define jTPDigest @"[Image]"
#define jLocalPath @"local"
#define jTPURL @"imageUri"
#define jTPThumbnailContent @"content"
#define jTPImageHeight @"height"
#define jTPImageWidth @"width"
#define jTPImageSize @"size"
#define jTPImageExtra @"extra"

@implementation JThumbnailPackedImageMessage

+ (NSString *)contentType {
    return jTPImageType;
}

+ (instancetype)messageWithImage:(UIImage *)image {
    JThumbnailPackedImageMessage *m = [[JThumbnailPackedImageMessage alloc] init];
    if (m) {
        m.originalImage = image;
        m.thumbnailImage = [self generateThumbnail:image];
    }
    return m;
}

+ (instancetype)messageWithImageData:(NSData *)imageData {
    JThumbnailPackedImageMessage *m = [[JThumbnailPackedImageMessage alloc] init];
    if (m) {
        m.originalImage = [UIImage imageWithData:imageData];
        m.thumbnailImage = [self generateThumbnail:m.originalImage];
    }
    return m;
}

- (NSData *)encode{
    NSData *thumbnailData = UIImageJPEGRepresentation(self.thumbnailImage, jThumbnailQuality);
    NSString *thumbnailString = nil;
    if ([thumbnailData respondsToSelector:@selector(base64EncodedStringWithOptions:)]) {
        thumbnailString = [thumbnailData base64EncodedStringWithOptions:kNilOptions];
    } else {
        thumbnailString = [JUtility base64EncodedStringFrom:thumbnailData];
    }
    NSDictionary * dic = @{jTPURL:self.url?:@"",
                           jLocalPath:self.localPath?:@"",
                           jTPThumbnailContent:thumbnailString?:@"",
                           jTPImageWidth:@(self.width),
                           jTPImageHeight:@(self.height),
                           jTPImageSize:@(self.size),
                           jTPImageExtra:self.extra?:@""};
    NSData *data = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    return data;
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    self.url = json[jTPURL]?:@"";
    self.localPath = json[jLocalPath]?:@"";
    NSString *thumbnailString = json[jTPThumbnailContent]?:@"";
    NSData *thumbnailData = nil;
    if (class_getInstanceMethod([NSData class], @selector(initWithBase64EncodedString:options:))) {
        thumbnailData = [[NSData alloc] initWithBase64EncodedString:thumbnailString
                                                        options:NSDataBase64DecodingIgnoreUnknownCharacters];
    } else {
        thumbnailData = [JUtility dataWithBase64EncodedString:thumbnailString];
    }
    self.thumbnailImage = [UIImage imageWithData:thumbnailData];
    id obj = json[jTPImageWidth];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.width = [(NSNumber *)obj intValue];
    }
    obj = json[jTPImageHeight];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.height = [(NSNumber *)obj intValue];
    }
    obj = json[jTPImageSize];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.size = [(NSNumber *)obj longLongValue];
    }
    self.extra = json[jTPImageExtra]?:@"";
}

- (NSString *)conversationDigest {
    return jTPDigest;
}

+ (UIImage *)generateThumbnail:(UIImage *)original {
    UIImage *thumbnail = [JUtility generateThumbnail:original
                                          targetSize:CGSizeMake(JThumbnailWidth, JThumbnailHeight)];
    return thumbnail;
}
@end
