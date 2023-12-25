//
//  JuggleIM.m
//  Juggle
//
//  Created by Nathan on 2023/11/27.
//

#import "JuggleIM.h"
#import "JConnectionManager.h"
#import "JMessageManager.h"
#import "JConversationManager.h"
#import "JuggleCore.h"

@interface JuggleIM ()
@property (nonatomic, strong) JuggleCore *core;
@end

@implementation JuggleIM

static JuggleIM *_instance;

+ (instancetype)shared {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [[self alloc] init];
        JuggleCore *core = [[JuggleCore alloc] init];
        _instance.core = core;
        _instance.messageManager = [[JMessageManager alloc] initWithCore:core];
        _instance.conversationManager = [[JConversationManager alloc] initWithCore:core];
        _instance.connectionManager = [[JConnectionManager alloc] initWithCore:core
                                                           conversationManager:_instance.conversationManager
                                                                messageManager:_instance.messageManager];
    });
    return _instance;
}

- (void)initWithAppKey:(NSString *)appKey {
    if ([self.core.appKey isEqualToString:appKey]) {
        return;
    }
    //appKey 更新了，则原来缓存的 userId 和 token 不再适用
    self.core.appKey = appKey;
    self.core.userId = @"";
    self.core.token = @"";
    NSLog(@"init appkey is %@", appKey);
}

- (void)setDelegateQueue:(dispatch_queue_t)delegateQueue {
    self.core.delegateQueue = delegateQueue;
}

@end
