//
//  JVoiceMessage.m
//  JetIM
//
//  Created by Nathan on 2023/12/26.
//

#import "JVoiceMessage.h"

#define kVoiceType @"jg:voice"
#define kDigest @"[voice]"
#define kURL @"url"
#define kDuration @"duration"
#define kVoiceExtra @"extra"

@implementation JVoiceMessage

+ (NSString *)contentType {
    return kVoiceType;
}

- (NSDictionary *)encodeToJson {
    return @{kURL:self.url?:@"",
             kDuration:@(self.duration),
             kVoiceExtra:self.extra?:@""
    };
}

- (void)decodeWithJson:(NSDictionary *)json {
    self.url = json[kURL]?:@"";
    id obj = json[kDuration];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.duration = [(NSNumber *)obj longValue];
    }
    self.extra = json[kVoiceExtra]?:@"";
}

- (NSString *)conversationDigest {
    return kDigest;
}
@end
