//
//  JGroupReadNtfMessage.m
// JuggleIM
//
//  Created by Nathan on 2024/3/6.
//

#import "JGroupReadNtfMessage.h"

#define jGroupReadNtfType @"jg:grpreadntf"
#define jGroupReadNtfMsgs @"msgs"
#define jGroupReadNtfMsgId @"msg_id"
#define jGroupReadNtfReadCount @"read_count"
#define jGroupReadNtfMemberCount @"member_count"

@implementation JGroupReadNtfMessage

+ (NSString *)contentType {
    return jGroupReadNtfType;
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    NSArray *jsonMsgs = json[jGroupReadNtfMsgs];
    NSMutableDictionary *msgs = [[NSMutableDictionary alloc] init];
    if ([jsonMsgs isKindOfClass:[NSArray class]]) {
        for (NSDictionary *msgDic in jsonMsgs) {
            NSString *messageId = msgDic[jGroupReadNtfMsgId];
            JGroupMessageReadInfo *readInfo = [[JGroupMessageReadInfo alloc] init];

            id readCountId = msgDic[jGroupReadNtfReadCount];
            if ([readCountId isKindOfClass:[NSNumber class]]) {
                int readCount = [(NSNumber *)readCountId intValue];
                readInfo.readCount = readCount;
            }
            id memberCountId = msgDic[jGroupReadNtfMemberCount];
            if ([memberCountId isKindOfClass:[NSNumber class]]) {
                int memberCount = [(NSNumber *)memberCountId intValue];
                readInfo.memberCount = memberCount;
            }
            [msgs setValue:readInfo forKey:messageId];
        }
    }
    self.msgs = msgs;
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

@end
