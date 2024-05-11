//
//  JReadNtfMessage.m
//  JetIM
//
//  Created by Nathan on 2024/3/4.
//

#import "JReadNtfMessage.h"

#define jReadNtfType @"jg:readntf"
#define jReadNtfMsgs @"msgs"
#define jReadNtfMsgId @"msg_id"

@implementation JReadNtfMessage

+ (NSString *)contentType {
    return jReadNtfType;
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    NSArray *msgs = json[jReadNtfMsgs];
    NSMutableArray *messageIds = [[NSMutableArray alloc] init];
    if ([msgs isKindOfClass:[NSArray class]]) {
        for (NSDictionary *msgDic in msgs) {
            NSString *messageId = msgDic[jReadNtfMsgId];
            [messageIds addObject:messageId];
        }
    }
    self.messageIds = messageIds;
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

@end
