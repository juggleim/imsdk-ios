//
//  JHeartBeatManager.m
//  JetIM
//
//  Created by Nathan on 2023/12/26.
//

#import "JHeartBeatManager.h"

#define kPingInterval 180

@interface JHeartBeatManager ()
@property (nonatomic, strong) JetIMCore *core;
@property (nonatomic, strong) NSTimer *pingTimer;
@end

@implementation JHeartBeatManager

- (void)start {
    NSLog(@"[JetIM] start ping");
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
    NSLog(@"[JetIM] stop ping");
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.pingTimer) {
            [self.pingTimer invalidate];
            self.pingTimer = nil;
        }
    });
}

- (instancetype)initWithCore:(JetIMCore *)core {
    JHeartBeatManager *m = [[JHeartBeatManager alloc] init];
    m.core = core;
    return m;
}

#pragma mark - internal
- (void)sendPing {
    NSLog(@"[JetIM] send ping");
    [self.core.webSocket sendPing];
}

@end
