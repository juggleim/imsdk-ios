//
//  JImageMessage.m
//  Juggle
//
//  Created by Nathan on 2023/12/3.
//

#import "JImageMessage.h"

#define kImageType @"jg:img"
#define kDigest @"[Image]"

@implementation JImageMessage

+ (NSString *)contentType {
    return kImageType;
}

- (NSDictionary *)encodeToJson {
    return @{@"origin":self.imageURL?:@"",
             @"thumbnail":self.thumbnailURL?:@""};
}

- (void)decodeWithJson:(NSDictionary *)json {
    self.imageURL = json[@"origin"]?:@"";
    self.thumbnailURL = json[@"thumbnail"]?:@"";
}

- (NSString *)conversationDigest {
    return kDigest;
}

@end
