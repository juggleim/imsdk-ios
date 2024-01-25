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
#define jVideoExtra @"extra"
#define jVideoDigest @"[Video]"

@implementation JVideoMessage

+ (NSString *)contentType {
    return jVideoType;
}

- (NSDictionary *)encodeToJson {
    return @{jVideoUrl:self.url?:@"",
             jSnapshotUrl:self.snapshotUrl?:@"",
             jVideoHeight:@(self.height),
             jVideoWidth:@(self.width),
             jVideoExtra:self.extra?:@""
    };
}

- (void)decodeWithJson:(NSDictionary *)json {
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
    self.extra = json[jVideoExtra]?:@"";
}

- (NSString *)conversationDigest {
    return jVideoDigest;
}

@end
