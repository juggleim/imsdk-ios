//
//  JFileMessage.m
//  JetIM
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
#define kFileExtra @"extra"

@implementation JFileMessage

+ (NSString *)contentType {
    return kFileType;
}

- (NSData *)encode{
    NSDictionary * dic = @{kURL:self.url?:@"",
                           kName:self.name?:@"",
                           kSize:@(self.size),
                           kType:self.type?:@"",
                           kFileExtra:self.extra?:@""};
    NSData *data = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    return data;
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    self.url = json[kURL]?:@"";
    self.name = json[kName]?:@"";
    id obj = json[kSize];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.size = [(NSNumber *)obj longLongValue];
    }
    self.type = json[kType]?:@"";
    self.extra = json[kFileExtra]?:@"";
}

- (NSString *)conversationDigest {
    return kDigest;
}

- (NSString *)searchContent{
    return self.name?:@"";
}
@end
