//
//  JImageMessage.m
//  Juggle
//
//  Created by Nathan on 2023/12/3.
//

#import "JImageMessage.h"

#define kImageType @"jg:img"
#define kDigest @"[Image]"
#define kURL @"url"
#define kThumbnail @"thumbnail"

@implementation JImageMessage

+ (NSString *)contentType {
    return kImageType;
}

- (NSDictionary *)encodeToJson {
    return @{kURL:self.url?:@"",
             kThumbnail:self.thumbnailUrl?:@""};
}

- (void)decodeWithJson:(NSDictionary *)json {
    self.url = json[kURL]?:@"";
    self.thumbnailUrl = json[kThumbnail]?:@"";
}

- (NSString *)conversationDigest {
    return kDigest;
}

@end
