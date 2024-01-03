//
//  JImageMessage.m
//  JetIM
//
//  Created by Nathan on 2023/12/3.
//

#import "JImageMessage.h"

#define kImageType @"jg:img"
#define kDigest @"[Image]"
#define jURL @"url"
#define jThumbnail @"thumbnail"
#define jImageHeight @"height"
#define jImageWidth @"width"

@implementation JImageMessage

+ (NSString *)contentType {
    return kImageType;
}

- (NSDictionary *)encodeToJson {
    return @{jURL:self.url?:@"",
             jThumbnail:self.thumbnailUrl?:@"",
             jImageWidth:@(self.width),
             jImageHeight:@(self.height)
    };
}

- (void)decodeWithJson:(NSDictionary *)json {
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
}

- (NSString *)conversationDigest {
    return kDigest;
}

@end
