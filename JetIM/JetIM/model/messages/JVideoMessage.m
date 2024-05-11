//
//  JVideoMessage.m
//  JetIM
//
//  Created by Nathan on 2024/1/25.
//

#import "JVideoMessage.h"

#define jVideoType @"jg:video"
#define jVideoUrl @"url"
#define jSnapshotUrl @"poster"
#define jVideoHeight @"height"
#define jVideoWidth @"width"
#define jVideoSize @"size"
#define jVideoDuration @"duration"
#define jVideoExtra @"extra"
#define jVideoDigest @"[Video]"

@implementation JVideoMessage

+ (NSString *)contentType {
    return jVideoType;
}


-(NSData *)encode{
    NSDictionary * dic = @{jVideoUrl:self.url?:@"",
                           jSnapshotUrl:self.snapshotUrl?:@"",
                           jVideoHeight:@(self.height),
                           jVideoWidth:@(self.width),
                           jVideoSize:@(self.size),
                           jVideoDuration:@(self.duration),
                           jVideoExtra:self.extra?:@""
                  };
    NSData *data = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    return data;
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    self.url = json[jVideoUrl]?:@"";
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
