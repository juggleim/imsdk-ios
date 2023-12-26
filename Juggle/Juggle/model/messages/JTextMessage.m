//
//  JTextMessage.m
//  Juggle
//
//  Created by Nathan on 2023/12/2.
//

#import "JTextMessage.h"


#define kTextType @"jg:text"
#define kContent @"content"

@implementation JTextMessage

- (instancetype)initWithContent:(NSString *)content {
    JTextMessage *text = [[JTextMessage alloc] init];
    text.content = content;
    return text;
}

+ (NSString *)contentType {
    return kTextType;
}

- (NSDictionary *)encodeToJson {
    return @{kContent:self.content?:@""};
}

- (void)decodeWithJson:(NSDictionary *)json {
    self.content = json[kContent]?:@"";
}

- (NSString *)conversationDigest {
    return self.content?:@"";
}

@end
