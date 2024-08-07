//
//  JMediaMessageContent.m
// JuggleIM
//
//  Created by Nathan on 2024/5/9.
//

#import "JMediaMessageContent.h"

#define kMediaType @"jg:media"

@implementation JMediaMessageContent

+ (NSString *)contentType {
    return kMediaType;
}

@end
