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
    if (self.type == JMomentMediaTypeImage) {
        dict[@"type"] = @"image";
    } else {
        dict[@"type"] = @"video";
    }
    dict[@"height"] = @(self.height);
    dict[@"width"] = @(self.width);
    dict[@"duration"] = @(self.duration);
    return [dict copy];
}

+ (JMomentMedia *)mediaWith:(NSDictionary *)dictionary {
    if (!dictionary) return nil;
    JMomentMedia *media = [[self alloc] init];
    
    media.url = dictionary[@"url"];
    media.snapshotUrl = dictionary[@"snapshot_url"];
    media.type = [dictionary[@"type"] isEqualToString:@"image"] ? JMomentMediaTypeImage : JMomentMediaTypeVideo;
    media.height = [dictionary[@"height"] intValue];
    media.width = [dictionary[@"width"] intValue];
    media.duration = [dictionary[@"duration"] intValue];
    
    return media;
}

@end
