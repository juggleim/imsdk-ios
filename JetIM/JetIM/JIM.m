//
//  JetIM.m
//  JetIM
//
//  Created by Nathan on 2023/11/27.
//

#import "JetIM.h"
#import "JConnectionManager.h"
#import "JMessageManager.h"
#import "JConversationManager.h"
#import "JUserInfoManager.h"
#import "JetIMCore.h"
#import "JLogger.h"

@interface JIM ()
@property (nonatomic, strong) JetIMCore *core;
@end

@implementation JIM

static JIM *_instance;

+ (instancetype)shared {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [[self alloc] init];
        JetIMCore *core = [[JetIMCore alloc] init];
        _instance.core = core;
        JMessageManager *messageManager = [[JMessageManager alloc] initWithCore:core];
        JConversationManager *conversationManager = [[JConversationManager alloc] initWithCore:core];
        JUserInfoManager *userInfoManager = [[JUserInfoManager alloc] initWithCore:core];
        messageManager.sendReceiveDelegate = conversationManager;
        _instance.conversationManager = conversationManager;
        _instance.messageManager = messageManager;
        _instance.userInfoManager = userInfoManager;
        _instance.connectionManager = [[JConnectionManager alloc] initWithCore:core
                                                           conversationManager:conversationManager
                                                                messageManager:messageManager];
    });
    return _instance;
}

- (void)initWithAppKey:(NSString *)appKey {
    JLogI(@"J-Init", @"appKey is %@", appKey);
    if ([self.core.appKey isEqualToString:appKey]) {
        return;
    }
    //appKey 更新了，则原来缓存的 userId 和 token 不再适用
    self.core.appKey = appKey;
    self.core.userId = @"";
    self.core.token = @"";
}

- (void)setDelegateQueue:(dispatch_queue_t)delegateQueue {
    self.core.delegateQueue = delegateQueue;
}

- (void)setServer:(NSArray<NSString *> *)serverUrls {
    [self.core setNaviUrls:serverUrls];
}

- (void)setConsoleLogLevel:(JLogLevel)level {
    JLogger.shared.consoleLogLevel = level;
}

- (NSString *)currentUserId {
    return self.core.userId;
}

@end
