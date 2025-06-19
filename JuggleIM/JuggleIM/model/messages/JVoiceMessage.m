//
//  JVoiceMessage.m
// JuggleIM
//
//  Created by Nathan on 2023/12/26.
//

#import "JVoiceMessage.h"

#define kVoiceType @"jg:voice"
#define kDigest @"[Voice]"
#define jLocalPath @"local"
#define kURL @"url"
#define kDuration @"duration"
#define kVoiceExtra @"extra"

@implementation JVoiceMessage

+ (NSString *)contentType {
    return kVoiceType;
}

-(NSData *)encode{
    NSDictionary * dic = @{kURL:self.url?:@"",
                           jLocalPath:self.localPath?:@"",
                           kDuration:@(self.duration),
                           kVoiceExtra:self.extra?:@""};
    NSData *data = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    return data;
}

- (void)decode:(NSData *)data {
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    
    self.localPath = json[jLocalPath]?:@"";
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
