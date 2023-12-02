//
//  JMessageContent+internal.m
//  Juggle
//
//  Created by Nathan on 2023/12/2.
//

#import "JMessageContent+internal.h"

@implementation JMessageContent (internal)

- (NSData *)encode {
    NSDictionary *dic = [self encodeToJson];
    NSData *data = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    return data;
}

@end
