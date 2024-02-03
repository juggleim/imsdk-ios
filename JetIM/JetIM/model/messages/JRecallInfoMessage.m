//
//  JRecallInfoMessage.m
//  JetIM
//
//  Created by Nathan on 2024/2/3.
//

#define jRecallInfoType @"jg:recallInfo"
#define jRecallExtra @"extra"

#import "JRecallInfoMessage.h"

@implementation JRecallInfoMessage

+ (NSString *)contentType {
    return jRecallInfoType;
}

- (NSDictionary *)encodeToJson {
    return @{jRecallExtra:self.extra?:@""};
}

- (void)decodeWithJson:(NSDictionary *)json {
    self.extra = json[jRecallExtra]?:@"";
}

@end
