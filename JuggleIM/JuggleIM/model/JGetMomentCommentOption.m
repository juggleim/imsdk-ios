//
//  JGetMomentCommentOption.m
//  JuggleIM
//
//  Created by Fei Li on 2025/11/13.
//

#import "JGetMomentCommentOption.h"

@implementation JGetMomentCommentOption

- (NSDictionary *)toDictionary {
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    if (self.momentId) {
        dict[@"moment_id"] = self.momentId;
    }
    dict[@"start"] = @(self.startTime);
    dict[@"limit"] = @(self.count);
    int order = self.direction == 1 ? 0 : 1;
    dict[@"order"] = @(order);
    return [dict copy];
}

@end
