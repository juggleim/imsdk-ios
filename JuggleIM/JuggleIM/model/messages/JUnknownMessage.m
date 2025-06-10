//
//  JUnknownMessage.m
//  JuggleIM
//
//  Created by Fei Li on 2025/6/10.
//

#import "JUnknownMessage.h"

#define jUnknownType @"jg:unknown"
#define jType @"type"
#define jContent @"content"

@implementation JUnknownMessage

+ (NSString *)contentType {
    return jUnknownType;
}

- (NSData *)encode {
    NSDictionary *dic = @{jContent:self.content?:@""};
    NSData *data = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    return data;
}

- (void)decode:(NSData *)data {
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    id content = json[jContent];
    if ([content isKindOfClass:[NSString class]]) {
        self.content = content;
    } else {
        self.content = @"";
    }
}


@end
