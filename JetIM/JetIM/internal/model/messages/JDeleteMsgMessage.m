//
//  JDeleteMsgMessage.m
//  JetIM
//
//  Created by 郑开 on 2024/5/21.
//

#import "JDeleteMsgMessage.h"

#define jDeleteMsgType            @"jg:delmsgs"
#define jDeleteMsgs               @"msgs"
#define jDeleteMsgsMsgId          @"msg_id"


@implementation JDeleteMsgMessage

+ (NSString *)contentType {
    return jDeleteMsgType;
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    NSArray *msgs = json[jDeleteMsgs];
    NSMutableArray *msgIdList = [[NSMutableArray alloc] init];
    if ([msgs isKindOfClass:[NSArray class]]) {
        for (NSDictionary *itemDic in msgs) {
            if ([itemDic[jDeleteMsgsMsgId] isKindOfClass:[NSString class]]) {
                NSString * msgId = itemDic[jDeleteMsgsMsgId];
                [msgIdList addObject:msgId];
            }
            
        }
    }
    self.msgIdList = msgIdList;
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

@end
