//
//  JRecallCmdMessage.m
// JuggleIM
//
//  Created by Nathan on 2024/2/2.
//

#define jRecallCmdType @"jg:recall"
#define jRecallMsgTime @"msg_time"
#define jRecallMsgId @"msg_id"
#define jRecallExts @"exts"

#import "JRecallCmdMessage.h"

@implementation JRecallCmdMessage

+ (NSString *)contentType {
    return jRecallCmdType; 
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    self.originalMessageId = json[jRecallMsgId]?:@"";
    id time = json[jRecallMsgTime];
    if ([time isKindOfClass:[NSNumber class]]) {
        self.originalMessageTime = [(NSNumber *)time longLongValue];
    }
    self.extra = json[jRecallExts];
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

@end
