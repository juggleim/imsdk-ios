//
//  JLogCommandMessage.m
// JuggleIM
//
//  Created by Nathan on 2024/5/21.
//

#import "JLogCommandMessage.h"

#define jLogCommandType @"jg:logcmd"
#define jLogStartTime @"start"
#define jLogEndTime @"end"
#define jLogPlatform @"platform"

@implementation JLogCommandMessage

+ (NSString *)contentType {
    return jLogCommandType;
}

- (void)decode:(NSData *)data {
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    
    id startTime = json[jLogStartTime];
    if ([startTime isKindOfClass:[NSNumber class]]) {
        self.startTime = [(NSNumber *)startTime longLongValue];
    }
    id endTime = json[jLogEndTime];
    if ([endTime isKindOfClass:[NSNumber class]]) {
        self.endTime = [(NSNumber *)endTime longLongValue];
    }
    self.platform = json[jLogPlatform];
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

@end
