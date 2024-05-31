//
//  JRecallInfoMessage.m
//  JetIM
//
//  Created by Nathan on 2024/2/3.
//

#define jRecallInfoType @"jg:recallinfo"
#define jRecallExtra @"extra"

#import "JRecallInfoMessage.h"

@implementation JRecallInfoMessage

+ (NSString *)contentType {
    return jRecallInfoType;
}

-(NSData *)encode{
    NSDictionary * dic = @{jRecallExtra:self.extra?:@{}};
    NSData *data = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    return data;
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    self.extra = json[jRecallExtra]?:@{};
}

@end
