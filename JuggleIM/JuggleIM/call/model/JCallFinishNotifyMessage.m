//
//  JCallFinishNotifyMessage.m
//  JuggleIM
//
//  Created by Fei Li on 2024/11/27.
//

#import "JCallFinishNotifyMessage.h"

#define jCallFinishNtf @"jg:callfinishntf"
#define jReason @"reason"
#define jDuration @"duration"
#define jCall @"[Call]"

@implementation JCallFinishNotifyMessage

+ (NSString *)contentType {
    return jCallFinishNtf;
}

+ (JMessageFlag)flags {
    return JMessageFlagIsSave;
}

- (NSData *)encode {
    NSDictionary *dic = @{jReason:@(self.finishType),
                          jDuration:@(self.duration)};
    NSData *data = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    return data;
}

- (void)decode:(NSData *)data {
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    id obj = json[jReason];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.finishType = [(NSNumber *)obj intValue];
    }
    obj = json[jDuration];
    if ([obj isKindOfClass:[NSNumber class]]) {
        self.duration = [(NSNumber *)obj longLongValue];
    }
}

- (NSString *)conversationDigest {
    return jCall;
}

@end
