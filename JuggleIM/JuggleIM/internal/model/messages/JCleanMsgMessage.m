//
//  JCleanMsgMessage.m
// JuggleIM
//
//  Created by 郑开 on 2024/5/21.
//

#import "JCleanMsgMessage.h"


#define jCleanMsgType            @"jg:cleanmsg"
#define jCleanMsgsCleanTime      @"clean_time"
#define jCleanMsgsSenderId       @"sender_id"

@implementation JCleanMsgMessage

+ (NSString *)contentType {
    return jCleanMsgType;
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    if ([json[jCleanMsgsCleanTime] isKindOfClass:[NSNumber class]]) {
        self.cleanTime = [json[jCleanMsgsCleanTime] longLongValue];
    }
    if ([json[jCleanMsgsSenderId] isKindOfClass:[NSString class]]) {
        self.senderId = json[jCleanMsgsSenderId];
    }
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

@end
