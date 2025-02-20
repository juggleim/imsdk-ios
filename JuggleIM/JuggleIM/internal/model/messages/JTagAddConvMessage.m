//
//  JTagAddConvMessage.m
//  JuggleIM
//
//  Created by Fei Li on 2025/2/20.
//

#import "JTagAddConvMessage.h"

@implementation JTagAddConvMessage

+ (NSString *)contentType {
    return @"jg:tagaddconvers";
}

- (void)decode:(NSData *)data {
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    self.tagId = json[@"tag"];
    self.tagName = json[@"tag_name"];
    NSArray *convsJson = json[@"convers"];
    NSMutableArray *conversations = [[NSMutableArray alloc] init];
    if ([convsJson isKindOfClass:[NSArray class]]) {
        for (NSDictionary *itemDic in convsJson) {
            id type = itemDic[@"channel_type"];
            JConversationType conversationType =  JConversationTypeUnknown;
            if ([type isKindOfClass:[NSNumber class]]) {
                conversationType = [(NSNumber *)type intValue];
            }
            JConversation *c = [[JConversation alloc] initWithConversationType:conversationType conversationId:itemDic[@"target_id"]];
            [conversations addObject:c];
        }
    }
    self.conversations = conversations;
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

@end
