//
//  JDeleteConvMessage.m
// JuggleIM
//
//  Created by Nathan on 2024/2/23.
//

#import "JDeleteConvMessage.h"

#define jDelConvType @"jg:delconvers"
#define jDelConvConversations @"conversations"
#define jDelConvTargetId @"target_id"
#define jDelConvChannelType @"channel_type"
#define jDelConvSubChannel @"sub_channel"

@implementation JDeleteConvMessage

+ (NSString *)contentType {
    return jDelConvType;
}

- (void)decode:(NSData *)data {
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    NSArray *convsJson = json[jDelConvConversations];
    NSMutableArray *conversations = [[NSMutableArray alloc] init];
    if ([convsJson isKindOfClass:[NSArray class]]) {
        for (NSDictionary *itemDic in convsJson) {
            id type = itemDic[jDelConvChannelType];
            JConversationType conversationType =  JConversationTypeUnknown;
            if ([type isKindOfClass:[NSNumber class]]) {
                conversationType = [(NSNumber *)type intValue];
            }
            JConversation *c = [[JConversation alloc] initWithConversationType:conversationType conversationId:itemDic[jDelConvTargetId]];
            NSString *subChannel = itemDic[jDelConvSubChannel];
            if (subChannel.length > 0) {
                c.subChannel = subChannel;
            }
            [conversations addObject:c];
        }
    }
    self.conversations = conversations;
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

@end
