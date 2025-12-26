//
//  JGetMomentOption.m
//  JuggleIM
//
//  Created by Fei Li on 2025/11/13.
//

#import "JGetMomentOption.h"

@implementation JGetMomentOption

- (instancetype)init {
    self = [super init];
    if (self) {
        self.direction = JPullDirectionOlder;
    }
    return self;
}

- (NSDictionary *)toDictionary {
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    dict[@"start"] = @(self.startTime);
    dict[@"limit"] = @(self.count);
    int order = self.direction == 1 ? 0 : 1;
    dict[@"order"] = @(order);
    return [dict copy];
}

@end
