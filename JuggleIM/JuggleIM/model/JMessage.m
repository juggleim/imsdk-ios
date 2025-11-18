//
//  JMessage.m
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import "JMessage.h"

@implementation JMessage

- (BOOL)isEqual:(id)object {
    if ([object isKindOfClass:self.class]) {
        JMessage * it = (JMessage *)object;
        return (self.clientMsgNo == it.clientMsgNo);
    } else {
        return [super isEqual:object];
    }
}

@end
