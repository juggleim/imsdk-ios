//
//  JCallEventUtil.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/9.
//

#import "JCallEventUtil.h"

@implementation JCallEventUtil

static NSDictionary *eventNameMap;

+ (NSString *)nameOfEvent:(NSInteger)event {
    return [[self getNameMap] objectForKey:@(event)];
}

+ (NSDictionary *)getNameMap {
    if (!eventNameMap) {
        eventNameMap = @{
            @(0): @"test"
        };
    }
    return eventNameMap;
}

@end
