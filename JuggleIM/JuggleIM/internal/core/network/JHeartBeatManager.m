//
//  JHeartBeatManager.m
// JuggleIM
//
//  Created by Nathan on 2023/12/26.
//

#import "JHeartBeatManager.h"
#import "JLogger.h"

#define jPingInterval 15
#define jDetectInterval 10
#define jTimeOutInterval (jPingInterval*3)

@interface JHeartBeatManager ()
@property (nonatomic, weak) JWebSocket *ws;
@property (nonatomic, strong) NSTimer *pingTimer;
@property (nonatomic, strong) NSTimer *detectTimer;
@property (nonatomic, assign) long long lastMessageReceiveTime;//秒
@end

@implementation JHeartBeatManager

- (void)start {
    JLogI(@"HB-Start", @"");
    [self stop];
    dispatch_async(dispatch_get_main_queue(), ^{
        self.pingTimer = [NSTimer scheduledTimerWithTimeInterval:jPingInterval
                                                          target:self
                                                        selector:@selector(sendPing)
                                                        userInfo:nil
                                                         repeats:YES];
        self.detectTimer = [NSTimer scheduledTimerWithTimeInterval:jDetectInterval
                                                            target:self
                                                          selector:@selector(detect)
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
        if (self.detectTimer) {
            [self.detectTimer invalidate];
            self.detectTimer = nil;
        }
    });
}

- (void)updateLastMessageReceiveTime {
    self.lastMessageReceiveTime = [[NSDate date] timeIntervalSince1970];
}

- (instancetype)initWithWebSocket:(JWebSocket *)ws {
    if (self = [super init]) {
        self.ws = ws;
    }
    return self;
}

#pragma mark - internal
- (void)sendPing {
    [self.ws sendPing];
}

//run in main queue
- (void)detect {
    long long now = [[NSDate date] timeIntervalSince1970];
    if (now - self.lastMessageReceiveTime >= jTimeOutInterval) {
        JLogE(@"HB-TimeOut", @"");
        [self notifyTimeOut];
    }
}

//run in main queue
- (void)notifyTimeOut {
    [self stop];
    [self.ws heartbeatTimeOut];
}

@end
