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

-(NSData *)encode{
    NSDictionary * dic = @{jContent:self.content?:@"",
                           jTextExtra:self.extra?:@""};
    NSData *data = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    return data;
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    self.content = json[jContent]?:@"";
    self.extra = json[jTextExtra]?:@"";
}

- (NSString *)conversationDigest {
    return self.content?:@"";
}

- (NSString *)searchContent{
    return self.content?:@"";
}
@end
