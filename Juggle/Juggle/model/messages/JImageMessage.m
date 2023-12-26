//
//  JImageMessage.m
//  Juggle
//
//  Created by Nathan on 2023/12/3.
//

#import "JImageMessage.h"

#define kImageType @"jg:img"
#define kDigest @"[Image]"
#define jURL @"url"
#define jThumbnail @"thumbnail"

@implementation JImageMessage

+ (NSString *)contentType {
    return kImageType;
}

- (NSDictionary *)encodeToJson {
    return @{jURL:self.url?:@"",
             jThumbnail:self.thumbnailUrl?:@""};
}

- (void)decodeWithJson:(NSDictionary *)json {
    self.url = json[jURL]?:@"";
    self.thumbnailUrl = json[jThumbnail]?:@"";
}

- (NSString *)conversationDigest {
    return kDigest;
}

@end
