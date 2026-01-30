//
//  JStreamTextMessage.m
//  JuggleIM
//
//  Created by Fei Li on 2026/1/29.
//

#import "JStreamTextMessage.h"

#define jSTContent @"content"
#define jSTIsFinished @"is_finished"

@implementation JStreamTextMessage

+ (NSString *)contentType {
    return @"jg:streamtext";
}

- (NSData *)encode {
    NSDictionary *dic = @{jSTContent:self.content?:@"",
                           jSTIsFinished:@(self.isFinished)};
    NSData *data = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    return data;
}

- (void)decode:(NSData *)data {
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    id content = json[jSTContent];
    if ([content isKindOfClass:[NSString class]]) {
        self.content = content;
    } else {
        self.content = @"";
    }
    id isFinished = json[jSTIsFinished];
    if ([isFinished isKindOfClass:[NSNumber class]]) {
        self.isFinished = [(NSNumber *)isFinished boolValue];
    }
}

- (NSString *)conversationDigest {
    return self.content?:@"";
}

- (NSString *)searchContent{
    return self.content?:@"";
}
@end
