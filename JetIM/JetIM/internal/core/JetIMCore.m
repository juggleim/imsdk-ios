//
//  JetIMCore.m
//  JetIM
//
//  Created by Nathan on 2023/12/1.
//

#import "JetIMCore.h"
#import "JetIMConstInternal.h"

#define kSendQueue "com.JetIM.im.wssendqueue"
#define kReceiveQueue "com.JetIM.im.wsreceivequeue"

#define jAppKey @"JetIMAppKey"
#define jUserId @"JetIMUserId"
#define jToken @"JetIMToken"

@implementation JetIMCore

- (instancetype)init {
    self = [super init];
    if (self) {
        self.sendQueue = dispatch_queue_create(kSendQueue, DISPATCH_QUEUE_SERIAL);
        self.receiveQueue = dispatch_queue_create(kReceiveQueue, DISPATCH_QUEUE_SERIAL);
        self.delegateQueue = dispatch_get_main_queue();
        self.webSocket = [[JWebSocket alloc] initWithSendQueque:self.sendQueue
                                                   receiveQueue:self.receiveQueue];
        self.dbManager = [[JDBManager alloc] init];
        _appKey = [[NSUserDefaults standardUserDefaults] objectForKey:jAppKey];
        _userId = [[NSUserDefaults standardUserDefaults] objectForKey:jUserId];
        _token = [[NSUserDefaults standardUserDefaults] objectForKey:jToken];
    }
    return self;
}

- (void)setAppKey:(NSString *)appKey {
    if ([_appKey isEqualToString:appKey]) {
        return;
    }
    _appKey = appKey;
    [[NSUserDefaults standardUserDefaults] setObject:appKey forKey:jAppKey];
}

- (void)setUserId:(NSString *)userId {
    if ([_userId isEqualToString:userId]) {
        return;
    }
    _userId = userId;
    [[NSUserDefaults standardUserDefaults] setObject:userId forKey:jUserId];
}

- (void)setToken:(NSString *)token {
    if ([_token isEqualToString:token]) {
        return;
    }
    _token = token;
    [[NSUserDefaults standardUserDefaults] setObject:token forKey:jToken];
}

- (void)getSyncTimeFromDB {
    _conversationSyncTime = [self.dbManager getConversationSyncTime];
    _messageSendSyncTime = [self.dbManager getMessageSendSyncTime];
    _messageReceiveSyncTime = [self.dbManager getMessageReceiveSyncTime];
}

- (void)setConversationSyncTime:(long long)conversationSyncTime {
    if (conversationSyncTime > _conversationSyncTime) {
        _conversationSyncTime = conversationSyncTime;
        [self.dbManager setConversationSyncTime:conversationSyncTime];
    }
}

- (void)setMessageSendSyncTime:(long long)messageSendSyncTime {
    if (messageSendSyncTime > _messageSendSyncTime) {
        _messageSendSyncTime = messageSendSyncTime;
        [self.dbManager setMessageSendSyncTime:messageSendSyncTime];
    }
}

- (void)setMessageReceiveSyncTime:(long long)messageReceiveSyncTime {
    if (messageReceiveSyncTime > _messageReceiveSyncTime) {
        _messageReceiveSyncTime = messageReceiveSyncTime;
        [self.dbManager setMessageReceiveSyncTime:messageReceiveSyncTime];
    }
}

- (NSArray<NSString *> *)naviUrls {
    if (!_naviUrls) {
        _naviUrls = @[JNaviURL];
    }
    return _naviUrls;
}

- (NSArray *)servers {
    if (!_servers) {
        _servers = @[JWebSocketURL];
    }
    return _servers;
}

@end
