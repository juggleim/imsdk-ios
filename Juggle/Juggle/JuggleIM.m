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
        _instance.connectionManager = [[JConnectionManager alloc] initWithCore:core];
        _instance.messageManager = [[JMessageManager alloc] initWithCore:core];
        _instance.conversationManager = [[JConversationManager alloc] initWithCore:core];
    });
    return _instance;
}

- (void)initWithAppKey:(NSString *)appKey {
    self.core.appKey = appKey;
    NSLog(@"init appkey is %@", appKey);
}


@end
