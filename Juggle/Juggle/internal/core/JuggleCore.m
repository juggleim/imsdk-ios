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
        self.sendQueue = dispatch_queue_create("com.juggle.im.wssendqueue", DISPATCH_QUEUE_SERIAL);
        self.receiveQueue = dispatch_queue_create("com.juggle.im.wsreceivequeue", DISPATCH_QUEUE_SERIAL);
        self.webSocket = [[JWebSocket alloc] initWithSendQueque:self.sendQueue
                                                   receiveQueue:self.receiveQueue];
    }
    return self;
}

@end
