//
//  JClearTotlaUnreadMessage.m
// JuggleIM
//
//  Created by 郑开 on 2024/6/28.
//

#import "JClearTotalUnreadMessage.h"

#define jClearTotalUnreadType            @"jg:cleartotalunread"
#define jClearTotalClearTime             @"clear_time"

@implementation JClearTotalUnreadMessage

+ (NSString *)contentType {
    return jClearTotalUnreadType;
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    NSNumber * time = [json objectForKey:jClearTotalClearTime];
    if(time){
        self.clearTime = [time longLongValue];
    }
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

@end
