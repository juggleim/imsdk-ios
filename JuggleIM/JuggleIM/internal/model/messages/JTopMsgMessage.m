//
//  JTopMsgMessage.m
//  JuggleIM
//
//  Created by Fei Li on 2025/7/15.
//

#import "JTopMsgMessage.h"

#define jTopMsgType @"jg:topmsg"
#define jAction     @"action"
#define jMessageId  @"msg_id"

@implementation JTopMsgMessage

+ (NSString *)contentType {
    return jTopMsgType;
}

- (void)decode:(NSData *)data {
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    id actionId = json[jAction];
    if ([actionId isKindOfClass:[NSNumber class]]) {
        int action = [actionId intValue];
        self.isTop = action == 0;
    }
    self.messageId = json[jMessageId];
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

@end
