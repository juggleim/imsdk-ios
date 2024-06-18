//
//  JTopConvMessage.m
//  JetIM
//
//  Created by 郑开 on 2024/5/21.
//

#import "JTopConvMessage.h"

#define jTopConvType            @"jg:topconvers"
#define jTopConvs               @"conversations"
#define jTopConvsTargetId       @"target_id"
#define jTopConvsChannelType    @"channel_type"
#define jTopConvsIsTop          @"is_top"
#define jTopConvsTopUpdateTime  @"top_update_time"

@implementation JTopConvMessage

+ (NSString *)contentType {
    return jTopConvType;
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    NSArray *convsJson = json[jTopConvs];
    NSMutableArray *conversations = [[NSMutableArray alloc] init];
    if ([convsJson isKindOfClass:[NSArray class]]) {
        for (NSDictionary *itemDic in convsJson) {
            id type = itemDic[jTopConvsChannelType];
            JConversationType conversationType =  JConversationTypeUnknown;
            if ([type isKindOfClass:[NSNumber class]]) {
                conversationType = [(NSNumber *)type intValue];
            }
            JConversation *c = [[JConversation alloc] initWithConversationType:conversationType conversationId:itemDic[jTopConvsTargetId]];
            JConcreteConversationInfo * conversationInfo = [[JConcreteConversationInfo alloc] init];
            conversationInfo.conversation = c;
            if ([itemDic[jTopConvsIsTop] isKindOfClass:[NSNumber class]]) {
                conversationInfo.isTop = [itemDic[jTopConvsIsTop] boolValue];
            }
            if ([itemDic[jTopConvsTopUpdateTime] isKindOfClass:[NSNumber class]]) {
                conversationInfo.topTime = [itemDic[jTopConvsTopUpdateTime] longLongValue];
            }
            [conversations addObject:conversationInfo];
        }
    }
    self.conversations = conversations;
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

@end
