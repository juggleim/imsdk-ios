//
//  JUserStatusChangeMessage.m
//  JuggleIM
//
//  Created by Fei Li on 2026/5/11.
//

#import "JUserStatusChangeMessage.h"

@implementation JUserStatusChangeMessage

+ (NSString *)contentType {
    return @"jg:onlinechg";
}

- (void)decode:(NSData *)data {
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    self.isOnline = NO;
    id isOnlineId = json[@"is_online"];
    if ([isOnlineId isKindOfClass:[NSNumber class]]) {
        BOOL isOnline = [(NSNumber *)isOnlineId boolValue];
        self.isOnline = isOnline;
    }
}

+ (JMessageFlag)flags {
    return JMessageFlagIsStatus;
}

@end
