//
//  JSnapshotPackedVideoMessage.m
//  JetIM
//
//  Created by Nathan on 2024/5/14.
//

#import "JSnapshotPackedVideoMessage.h"
#import "JetIMConstInternal.h"
#import "JUtility.h"
#import "objc/runtime.h"

#define jSPVideoType @"jg:spvideo"
#define jSPVideoUrl @"url"
#define jSPSnapshotImage @"snapshot"
#define jSPVideoHeight @"height"
#define jSPVideoWidth @"width"
#define jSPVideoSize @"size"
#define jSPVideoDuration @"duration"
#define jSPVideoExtra @"extra"
#define jSPVideoDigest @"[Video]"

@implementation JSnapshotPackedVideoMessage
+ (NSString *)contentType {
    return jSPVideoType;
}


-(NSData *)encode{
    NSData *snapshotData = UIImageJPEGRepresentation(self.snapshotImage, jThumbnailQuality);
    NSString *snapshotString = nil;
    if ([snapshotData respondsToSelector:@selector(base64EncodedStringWithOptions:)]) {
        snapshotString = [snapshotData base64EncodedStringWithOptions:kNilOptions];
    } else {
        snapshotString = [JUtility base64EncodedStringFrom:snapshotData];
    }
    NSDictionary * dic = @{jSPVideoUrl:self.url?:@"",
                           jSPSnapshotImage:snapshotString?:@"",
                           jSPVideoHeight:@(self.height),
                           jSPVideoWidth:@(self.width),
                           jSPVideoSize:@(self.size),
                           jSPVideoDuration:@(self.duration),
                           jSPVideoExtra:self.extra?:@""
                  };
    NSData *data = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    return data;
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    self.url = json[jSPVideoUrl]?:@"";
    NSString *snapshotString = json[jSPSnapshotImage]?:@"";
    NSData *snapshotData = nil;
    if (class_getInstanceMethod([NSData class], @selector(initWithBase64EncodedString:options:))) {
        snapshotData = [[NSData alloc] initWithBase64EncodedString:snapshotString
                                                        options:NSDataBase64DecodingIgnoreUnknownCharacters];
    } else {
        snapshotData = [JUtility dataWithBase64EncodedString:snapshotString];
    }
    self.snapshotImage = [UIImage imageWithData:snapshotData];
    id obj = json[jSPVideoHeight];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.height = [(NSNumber *)obj intValue];
    }
    obj = json[jSPVideoWidth];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.width = [(NSNumber *)obj intValue];
    }
    obj = json[jSPVideoSize];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.size = [(NSNumber *)obj longLongValue];
    }
    obj = json[jSPVideoDuration];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.duration = [(NSNumber *)obj intValue];
    }
    self.extra = json[jSPVideoExtra]?:@"";
}

- (NSString *)conversationDigest {
    return jSPVideoDigest;
}
@end
