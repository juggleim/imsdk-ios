//
//  JCreateConversationTagMessage.m
//  JuggleIM
//
//  Created by Fei Li on 2026/4/28.
//

#import "JCreateConversationTagMessage.h"

@implementation JCreateConversationTagMessage

+ (NSString *)contentType {
    return @"jg:createconvertags";
}

- (void)decode:(NSData *)data {
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    NSArray *tagListJson = json[@"tags"];
    NSMutableArray *tagList = [NSMutableArray array];
    if ([tagListJson isKindOfClass:[NSArray class]]) {
        for (NSDictionary *itemDic in tagListJson) {
            if (![itemDic isKindOfClass:[NSDictionary class]]) {
                continue;
            }
            JConversationTagInfo *tagInfo = [JConversationTagInfo new];
            tagInfo.tagId = itemDic[@"tag"] ?: @"";
            tagInfo.name = itemDic[@"tag_name"] ?: @"";
            tagInfo.type = JConversationTagTypeUser;
            [tagList addObject:tagInfo];
        }
    }
    self.tagList = tagList;
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

@end
