//
//  JStreamTextMessage.m
//  JuggleIM
//
//  Created by Fei Li on 2026/1/29.
//

#import "JStreamTextMessage.h"

#define jSTContent @"content"
#define jSTIsFinished @"is_finished"
#define jSTSeq @"seq"

@implementation JStreamTextMessage

+ (NSString *)contentType {
    return @"jg:streamtext";
}

- (NSData *)encode {
    NSDictionary *dic = @{jSTContent:self.content?:@"",
                          jSTIsFinished:@(self.isFinished),
                          jSTSeq:@(self.seq)};
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
    id seq = json[jSTSeq];
    if ([seq isKindOfClass:[NSNumber class]]) {
        self.seq = [(NSNumber *)seq intValue];
    }
}

- (NSString *)conversationDigest {
    return self.content?:@"";
}

- (NSString *)searchContent{
    return self.content?:@"";
}
@end
