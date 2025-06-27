//
//  JUnknownMessage.m
//  JuggleIM
//
//  Created by Fei Li on 2025/6/10.
//

#import "JUnknownMessage.h"

#define jUnknownType @"jg:unknown"

@implementation JUnknownMessage

+ (NSString *)contentType {
    return jUnknownType;
}

- (NSData *)encode {
    return [self.content dataUsingEncoding:NSUTF8StringEncoding];
}

- (void)decode:(NSData *)data {
    self.content = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
}


@end
