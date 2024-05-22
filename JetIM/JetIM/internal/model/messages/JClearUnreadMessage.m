//
//  JClearUnreadMessage.m
//  JetIM
//
//  Created by 郑开 on 2024/5/21.
//

#import "JClearUnreadMessage.h"

#define jClearUnreadType            @"jg:clearunread"
#define jClearUnreads               @"conversations"
#define jClearUnreadsTargetId       @"target_id"
#define jClearUnreadsChannelType    @"channel_type"
#define jClearUnreadsLastReadIndex  @"latest_read_index"

@implementation JClearUnreadMessage

+ (NSString *)contentType {
    return jClearUnreadType;
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    NSArray *convsJson = json[jClearUnreads];
    NSMutableArray *conversations = [[NSMutableArray alloc] init];
    if ([convsJson isKindOfClass:[NSArray class]]) {
        for (NSDictionary *itemDic in convsJson) {
            id type = itemDic[jClearUnreadsChannelType];
            JConversationType conversationType =  JConversationTypeUnknown;
            if ([type isKindOfClass:[NSNumber class]]) {
                conversationType = [(NSNumber *)type intValue];
            }
            JConversation *c = [[JConversation alloc] initWithConversationType:conversationType conversationId:itemDic[jClearUnreadsTargetId]];
            JConcreteConversationInfo * conversationInfo = [[JConcreteConversationInfo alloc] init];
            conversationInfo.conversation = c;
            if ([itemDic[jClearUnreadsLastReadIndex] isKindOfClass:[NSNumber class]]) {
                conversationInfo.lastReadMessageIndex = [itemDic[jClearUnreadsLastReadIndex] longLongValue];
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
