//
//  JMarkUnreadMessage.m
//  JuggleIM
//
//  Created by Fei Li on 2024/8/23.
//

#import "JMarkUnreadMessage.h"

#define jMarkUnreadType @"jg:markunread"
#define jConversations @"conversations"
#define jTargetId @"target_id"
#define jChannelType @"channel_type"

@implementation JMarkUnreadMessage

+ (NSString *)contentType {
    return jMarkUnreadType;
}

- (void)decode:(NSData *)data {
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    NSArray *convsJson = json[jConversations];
    NSMutableArray *conversations = [[NSMutableArray alloc] init];
    if ([convsJson isKindOfClass:[NSArray class]]) {
        for (NSDictionary *itemDic in convsJson) {
            id type = itemDic[jChannelType];
            JConversationType conversationType =  JConversationTypeUnknown;
            if ([type isKindOfClass:[NSNumber class]]) {
                conversationType = [(NSNumber *)type intValue];
            }
            JConversation *c = [[JConversation alloc] initWithConversationType:conversationType conversationId:itemDic[jTargetId]];
            [conversations addObject:c];
        }
    }
    self.conversations = conversations;
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

@end
