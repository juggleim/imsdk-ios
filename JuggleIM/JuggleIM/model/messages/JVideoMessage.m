//
//  JVideoMessage.m
// JuggleIM
//
//  Created by Nathan on 2024/1/25.
//

#import "JVideoMessage.h"
#import <AVKit/AVKit.h>
#import "JUtility.h"

#define jVideoType @"jg:video"
#define jLocalPath @"local"
#define jSnapshotLocalPath  @"snapshotLocalPath"
#define jVideoUrl @"url"
#define jSnapshotUrl @"poster"
#define jVideoHeight @"height"
#define jVideoWidth @"width"
#define jVideoSize @"size"
#define jVideoDuration @"duration"
#define jVideoExtra @"extra"
#define jVideoDigest @"[Video]"

@implementation JVideoMessage

+ (instancetype)videoWithData:(NSData *)videoFileData {
    JVideoMessage *videoMessage = [[JVideoMessage alloc] init];
    
    NSString *mediaPath = [JUtility mediaPath:JMediaTypeVideo];
    if (![[NSFileManager defaultManager] fileExistsAtPath:mediaPath]) {
        [[NSFileManager defaultManager] createDirectoryAtPath:mediaPath
                                  withIntermediateDirectories:YES
                                                   attributes:nil
                                                        error:nil];
    }
    NSString *fileName = [NSString stringWithFormat:@"%lld", (long long)[NSDate date].timeIntervalSince1970];
    NSString *movExtend = @".mov";
    NSString *fullName = [fileName stringByAppendingString:movExtend];
    NSString *localPath = [mediaPath stringByAppendingPathComponent:fullName];
    [videoFileData writeToFile:localPath atomically:YES];
    videoMessage.localPath = localPath;
    
    AVAsset *asset = [AVAsset assetWithURL:[NSURL fileURLWithPath:localPath]];
    videoMessage.duration = CMTimeGetSeconds(asset.duration);
    AVAssetImageGenerator *generator = [AVAssetImageGenerator assetImageGeneratorWithAsset:asset];
    generator.appliesPreferredTrackTransform = YES;
    CMTime cmTime = CMTimeMake(2, 1);
    CGImageRef cgImage = [generator copyCGImageAtTime:cmTime
                                            actualTime:nil
                                                 error:nil];
    UIImage *image = [UIImage imageWithCGImage:cgImage];
    videoMessage.width = image.size.width;
    videoMessage.height = image.size.height;
    
    UIImage *snapshotImage = [JUtility generateThumbnail:image targetSize:CGSizeMake(JThumbnailWidth, JThumbnailHeight)];
    NSData *snapshotData = UIImageJPEGRepresentation(snapshotImage, jThumbnailQuality);
    NSString *snapshotName = [NSString stringWithFormat:@"snapshot_%@", fileName];
    NSString *jpgExtend = @".jpg";
    NSString *snapshotFullName = [snapshotName stringByAppendingString:jpgExtend];
    NSString *snapshotPath = [mediaPath stringByAppendingPathComponent:snapshotFullName];
    [snapshotData writeToFile:snapshotPath atomically:YES];
    videoMessage.snapshotLocalPath = snapshotPath;
    
    return videoMessage;
}

+ (NSString *)contentType {
    return jVideoType;
}

-(NSData *)encode{
    NSDictionary * dic = @{jVideoUrl:self.url?:@"",
                           jLocalPath:self.localPath?:@"",
                           jSnapshotUrl:self.snapshotUrl?:@"",
                           jVideoHeight:@(self.height),
                           jVideoWidth:@(self.width),
                           jVideoSize:@(self.size),
                           jVideoDuration:@(self.duration),
                           jVideoExtra:self.extra?:@"",
                           jSnapshotLocalPath:self.snapshotLocalPath?:@""
                  };
    NSData *data = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    return data;
}

- (void)decode:(NSData *)data {
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    self.url = json[jVideoUrl]?:@"";
    self.localPath = json[jLocalPath]?:@"";
    self.snapshotLocalPath = json[jSnapshotLocalPath]?:@"";
    self.snapshotUrl = json[jSnapshotUrl]?:@"";
    id obj = json[jVideoHeight];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.height = [(NSNumber *)obj intValue];
    }
    obj = json[jVideoWidth];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.width = [(NSNumber *)obj intValue];
    }
    obj = json[jVideoSize];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.size = [(NSNumber *)obj longLongValue];
    }
    obj = json[jVideoDuration];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.duration = [(NSNumber *)obj intValue];
    }
    self.extra = json[jVideoExtra]?:@"";
}

- (NSString *)conversationDigest {
    return jVideoDigest;
}

@end
