//
//  JuggleCore.m
//  Juggle
//
//  Created by Nathan on 2023/12/1.
//

#import "JuggleCore.h"

@implementation JuggleCore

- (instancetype)init {
    self = [super init];
    if (self) {
        self.webSocket = [[JWebSocket alloc] init];
    }
    return self;
}

@end
