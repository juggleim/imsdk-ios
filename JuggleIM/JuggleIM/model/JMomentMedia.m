//
//  JMomentMedia.m
//  JuggleIM
//
//  Created by Fei Li on 2025/11/11.
//

#import "JMomentMedia.h"

@implementation JMomentMedia

- (NSDictionary *)toDictionary {
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    if (self.url) {
        dict[@"url"] = self.url;
    }
    dict[@"type"] = @(self.type);
    return [dict copy];
}

@end
