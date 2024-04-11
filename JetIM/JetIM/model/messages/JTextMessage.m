//
//  JTextMessage.m
//  JetIM
//
//  Created by Nathan on 2023/12/2.
//

#import "JTextMessage.h"


#define kTextType @"jg:text"
#define jContent @"content"
#define jTextExtra @"extra"

@implementation JTextMessage

- (instancetype)initWithContent:(NSString *)content {
    if (self = [super init]) {
        self.content = content;
    }
    return self;
}

+ (NSString *)contentType {
    return kTextType;
}

- (NSDictionary *)encodeToJson {
    return @{jContent:self.content?:@"",
             jTextExtra:self.extra?:@""
    };
}

- (void)decodeWithJson:(NSDictionary *)json {
    self.content = json[jContent]?:@"";
    self.extra = json[jTextExtra]?:@"";
}

- (NSString *)conversationDigest {
    return self.content?:@"";
}

@end
