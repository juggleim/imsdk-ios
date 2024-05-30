//
//  JHeartBeatManager.m
//  JetIM
//
//  Created by Nathan on 2023/12/26.
//

#import "JHeartBeatManager.h"
#import "JLogger.h"

#define kPingInterval 30

@interface JHeartBeatManager ()
@property (nonatomic, strong) JetIMCore *core;
@property (nonatomic, strong) NSTimer *pingTimer;
@end

@implementation JHeartBeatManager

- (void)start {
    JLogI(@"HB-Start", @"");
    [self stop];
    dispatch_async(dispatch_get_main_queue(), ^{
        self.pingTimer = [NSTimer scheduledTimerWithTimeInterval:kPingInterval
                                                          target:self
                                                        selector:@selector(sendPing)
                                                        userInfo:nil
                                                         repeats:YES];
    });
}

- (void)stop {
    JLogI(@"HB-Stop", @"");
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.pingTimer) {
            [self.pingTimer invalidate];
            self.pingTimer = nil;
        }
    });
}

- (instancetype)initWithCore:(JetIMCore *)core {
    if (self = [super init]) {
        self.core = core;
    }
    return self;
}

#pragma mark - internal
- (void)sendPing {
    [self.core.webSocket sendPing];
}

@end
