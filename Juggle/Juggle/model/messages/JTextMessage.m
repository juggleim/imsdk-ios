//
//  JTextMessage.m
//  Juggle
//
//  Created by Nathan on 2023/12/2.
//

#import "JTextMessage.h"


#define kTextType @"jg:text"

@implementation JTextMessage

+ (NSString *)contentType {
    return kTextType;
}

- (NSDictionary *)encodeToJson {
    return @{@"content":self.content?:@""};
}

- (void)decodeWithJson:(NSDictionary *)json {
    self.content = json[@"content"]?:@"";
}

- (NSString *)conversationDigest {
    return self.content?:@"";
}

@end
