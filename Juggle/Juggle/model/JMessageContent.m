//
//  JMessageContent.m
//  Juggle
//
//  Created by Nathan on 2023/11/28.
//

#import "JMessageContent.h"

#define kUnknownType @"jg:unknown"

@implementation JMessageContent

+ (NSString *)contentType {
    return kUnknownType;
}

- (NSDictionary *)encodeToJson {
    NSDictionary *dic = [[NSDictionary alloc] init];
    return dic;
}

- (void)decodeWithJson:(NSDictionary *)json {
    
}

- (NSString *)conversationDigest {
    return @"";
}

@end
