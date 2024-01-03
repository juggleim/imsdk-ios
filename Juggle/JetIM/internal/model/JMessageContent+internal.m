//
//  JMessageContent+internal.m
//  JetIM
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

- (void)decode:(NSData *)data {
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    [self decodeWithJson:dic];
}

@end
