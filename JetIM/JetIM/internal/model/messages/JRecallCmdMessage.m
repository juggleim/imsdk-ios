//
//  JRecallCmdMessage.m
//  JetIM
//
//  Created by Nathan on 2024/2/2.
//

#define jRecallCmdType @"jg:recall"
#define jRecallMsgTime @"msg_time"
#define jRecallMsgId @"msg_id"
#define jRecallSenderId @"sender_id"
#define jRecallReceiverId @"receiver_id"
#define jRecallChannelType @"channel_type"

#import "JRecallCmdMessage.h"

@implementation JRecallCmdMessage

+ (NSString *)contentType {
    return jRecallCmdType;
}

- (void)decodeWithJson:(NSDictionary *)json {
    self.originalMessageId = json[jRecallMsgId]?:@"";
    id time = json[jRecallMsgTime];
    if ([time isKindOfClass:[NSNumber class]]) {
        self.originalMessageTime = [(NSNumber *)time longLongValue];
    }
    self.senderId = json[jRecallSenderId]?:@"";
    self.receiverId = json[jRecallReceiverId]?:@"";
    id type = json[jRecallChannelType];
    if ([type isKindOfClass:[NSNumber class]]) {
        self.conversationType = [(NSNumber *)type intValue];
    }
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

@end
