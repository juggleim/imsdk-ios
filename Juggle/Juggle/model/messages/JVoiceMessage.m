//
//  JVoiceMessage.m
//  Juggle
//
//  Created by Nathan on 2023/12/26.
//

#import "JVoiceMessage.h"

#define kVoiceType @"jg:voice"
#define kDigest @"[voice]"
#define kURL @"url"
#define kDuration @"duration"

@implementation JVoiceMessage

+ (NSString *)contentType {
    return kVoiceType;
}

- (NSDictionary *)encodeToJson {
    return @{kURL:self.url?:@"",
             kDuration:@(self.duration)
    };
}

- (void)decodeWithJson:(NSDictionary *)json {
    self.url = json[kURL]?:@"";
    id obj = json[kDuration];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.duration = [(NSNumber *)obj longValue];
    }
}

- (NSString *)conversationDigest {
    return kDigest;
}
@end