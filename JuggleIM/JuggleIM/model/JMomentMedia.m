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
    if (self.snapshotUrl) {
        dict[@"snapshot_url"] = self.snapshotUrl;
    }
    dict[@"type"] = @(self.type);
    dict[@"height"] = @(self.height);
    dict[@"width"] = @(self.width);
    dict[@"duration"] = @(self.duration);
    return [dict copy];
}

@end
