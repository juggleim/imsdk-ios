//
//  JFileMessage.m
//  Juggle
//
//  Created by Nathan on 2023/12/26.
//

#import "JFileMessage.h"

#define kFileType @"jg:file"
#define kDigest @"[File]"
#define kName @"name"
#define kURL @"url"
#define kSize @"size"
#define kType @"type"

@implementation JFileMessage

+ (NSString *)contentType {
    return kFileType;
}

- (NSDictionary *)encodeToJson {
    return @{kURL:self.url?:@"",
             kName:self.name?:@"",
             kSize:@(self.size),
             kType:self.type?:@""
    };
}

- (void)decodeWithJson:(NSDictionary *)json {
    self.url = json[kURL]?:@"";
    self.name = json[kName]?:@"";
    id obj = json[kSize];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.size = [(NSNumber *)obj longLongValue];
    }
    self.type = json[kType]?:@"";
}

- (NSString *)conversationDigest {
    return kDigest;
}

@end
