//
//  JRecallInfoMessage.m
// JuggleIM
//
//  Created by Nathan on 2024/2/3.
//

#define jRecallInfoType @"jg:recallinfo"
#define jRecallExtra @"exts"

#import "JRecallInfoMessage.h"

@implementation JRecallInfoMessage

+ (NSString *)contentType {
    return jRecallInfoType;
}

-(NSData *)encode{
    NSDictionary * dic = @{jRecallExtra:self.exts?:@{}};
    NSData *data = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    return data;
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    self.exts = json[jRecallExtra]?:@{};
}

@end
