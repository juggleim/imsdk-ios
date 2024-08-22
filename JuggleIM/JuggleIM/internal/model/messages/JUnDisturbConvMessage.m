//
//  JUnDisturbConvMessage.m
// JuggleIM
//
//  Created by 郑开 on 2024/5/21.
//

#import "JUnDisturbConvMessage.h"


#define jUnDisturbConvType               @"jg:undisturb"
#define jUnDisturbConvs                  @"conversations"
#define jUnDisturbConvTargetId           @"target_id"
#define jUnDisturbConvChannelType        @"channel_type"
#define jUnDisturbConvUndisturbType      @"undisturb_type"

@implementation JUnDisturbConvMessage

+ (NSString *)contentType {
    return jUnDisturbConvType;
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    NSArray *convsJson = json[jUnDisturbConvs];
    NSMutableArray *conversations = [[NSMutableArray alloc] init];
    if ([convsJson isKindOfClass:[NSArray class]]) {
        for (NSDictionary *itemDic in convsJson) {
            id type = itemDic[jUnDisturbConvChannelType];
            JConversationType conversationType =  JConversationTypeUnknown;
            if ([type isKindOfClass:[NSNumber class]]) {
                conversationType = [(NSNumber *)type intValue];
            }
            JConversation *c = [[JConversation alloc] initWithConversationType:conversationType conversationId:itemDic[jUnDisturbConvTargetId]];
            JConcreteConversationInfo * conversationInfo = [[JConcreteConversationInfo alloc] init];
            conversationInfo.conversation = c;
            if ([itemDic[jUnDisturbConvUndisturbType] isKindOfClass:[NSNumber class]]) {
                int conversationType = [itemDic[jUnDisturbConvUndisturbType] intValue];
                conversationInfo.mute = (conversationType == 1);
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
