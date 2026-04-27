//
//  JDeleteConversationTagMessage.m
//  JuggleIM
//
//  Created by Fei Li on 2026/4/23.
//

#import "JDeleteConversationTagMessage.h"


@implementation JDeleteConversationTagMessage

+ (NSString *)contentType {
    return @"jg:delconvertags";
}

- (void)decode:(NSData *)data {
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    NSArray *tagIdListJson = json[@"tags"];
    NSMutableArray *tagIdList = [NSMutableArray array];
    if ([tagIdListJson isKindOfClass:[NSArray class]]) {
        for (NSDictionary *itemDic in tagIdListJson) {
            NSString *tagId = itemDic[@"tag"];
            [tagIdList addObject:tagId];
        }
    }
    self.tagIdList = tagIdList;
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

@end
