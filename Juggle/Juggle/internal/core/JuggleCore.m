//
//  JuggleCore.m
//  Juggle
//
//  Created by Nathan on 2023/12/1.
//

#import "JuggleCore.h"

#define kSendQueue "com.juggle.im.wssendqueue"
#define kReceiveQueue "com.juggle.im.wsreceivequeue"

#define kAppKey @"JuggleAppKey"
#define kUserId @"JuggleUserId"
#define kToken @"JuggleToken"

@implementation JuggleCore

- (instancetype)init {
    self = [super init];
    if (self) {
        self.sendQueue = dispatch_queue_create(kSendQueue, DISPATCH_QUEUE_SERIAL);
        self.receiveQueue = dispatch_queue_create(kReceiveQueue, DISPATCH_QUEUE_SERIAL);
        self.delegateQueue = dispatch_get_main_queue();
        self.webSocket = [[JWebSocket alloc] initWithSendQueque:self.sendQueue
                                                   receiveQueue:self.receiveQueue];
        self.dbManager = [[JDBManager alloc] init];
        _appKey = [[NSUserDefaults standardUserDefaults] objectForKey:kAppKey];
        _userId = [[NSUserDefaults standardUserDefaults] objectForKey:kUserId];
        _token = [[NSUserDefaults standardUserDefaults] objectForKey:kToken];
    }
    return self;
}

- (void)setAppKey:(NSString *)appKey {
    if ([_appKey isEqualToString:appKey]) {
        return;
    }
    _appKey = appKey;
    [[NSUserDefaults standardUserDefaults] setObject:appKey forKey:kAppKey];
}

- (void)setUserId:(NSString *)userId {
    if ([_userId isEqualToString:userId]) {
        return;
    }
    _userId = userId;
    [[NSUserDefaults standardUserDefaults] setObject:userId forKey:kUserId];
}

- (void)setToken:(NSString *)token {
    if ([_token isEqualToString:token]) {
        return;
    }
    _token = token;
    [[NSUserDefaults standardUserDefaults] setObject:token forKey:kToken];
}
@end
