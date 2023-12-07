//
//  JMessage+internal.m
//  Juggle
//
//  Created by Nathan on 2023/12/7.
//

#import "JMessage+internal.h"
#import <objc/runtime.h>

static const void *Key = &Key;

@implementation JMessage (internal)
@dynamic msgIndex;

- (int64_t)msgIndex {
    NSNumber *num = objc_getAssociatedObject(self, Key);
    return num.longLongValue;
}

- (void)setMsgIndex:(int64_t)msgIndex {
    objc_setAssociatedObject(self, Key, @(msgIndex), OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

@end
