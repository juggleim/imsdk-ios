//
// JIM.m
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import "JIM.h"
#import "JConnectionManager.h"
#import "JMessageManager.h"
#import "JConversationManager.h"
#import "JUserInfoManager.h"
#import "JChatroomManager.h"
#import "JIMCore.h"
#import "JLogger.h"
#import "JUtility.h"
#import "JCallManager.h"

@interface JIM ()
@property (nonatomic, strong) JIMCore *core;
@end

@implementation JIM

static JIM *_instance;

+ (instancetype)shared {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [[self alloc] init];
        JIMCore *core = [[JIMCore alloc] init];
        _instance.core = core;
        JLogger.shared.core = core;
        JChatroomManager *chatroomManager = [[JChatroomManager alloc] initWithCore:core];
        _instance.chatroomManager = chatroomManager;
        JUserInfoManager *userInfoManager = [[JUserInfoManager alloc] initWithCore:core];
        JMessageManager *messageManager = [[JMessageManager alloc] initWithCore:core chatroomManager:chatroomManager userInfoManager:userInfoManager];
        JConversationManager *conversationManager = [[JConversationManager alloc] initWithCore:core messageManager:messageManager userInfoManager:userInfoManager];
        messageManager.sendReceiveDelegate = conversationManager;
        JCallManager *callManager = [[JCallManager alloc] initWithCore:core userInfoManager:userInfoManager];
        _instance.conversationManager = conversationManager;
        _instance.messageManager = messageManager;
        _instance.userInfoManager = userInfoManager;
        _instance.callManager = callManager;
        _instance.connectionManager = [[JConnectionManager alloc] initWithCore:core
                                                           conversationManager:conversationManager
                                                                messageManager:messageManager
                                                               chatroomManager:chatroomManager
                                                                   callManager:callManager
                                                               userInfoManager:userInfoManager];
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

- (NSString *)getSDKVersion {
    return JIMVersion;
}

- (void)setDelegateQueue:(dispatch_queue_t)delegateQueue {
    self.core.delegateQueue = delegateQueue;
}

- (void)setServerUrls:(NSArray<NSString *> *)serverUrls {
    [self.core setServers:serverUrls];
}

- (void)setConsoleLogLevel:(JLogLevel)level {
    JLogger.shared.consoleLogLevel = level;
}

- (NSString *)appKey {
    return self.core.appKey;
}

- (NSString *)currentUserId {
    return self.core.userId;
}

-(NSString *)getDeviceId{
    return [JUtility getDeviceId];
}

@end
