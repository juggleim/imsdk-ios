//
//  JMessageTypeCenter.m
//  JetIM
//
//  Created by Nathan on 2023/12/26.
//

#import "JMessageTypeCenter.h"
#import <objc/runtime.h>
#import "JMessageContent+internal.h"

@interface JMessageTypeCenter ()
@property (nonatomic, strong) NSMutableDictionary *messageTypeDic;
@end

@implementation JMessageTypeCenter

static JMessageTypeCenter *_instance;

+ (instancetype)shared {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [[self alloc] init];
        _instance.messageTypeDic = [[NSMutableDictionary alloc] init];
    });
    return _instance;
}

- (void)registerMessageType:(Class)messageClass {
    NSString *contentType = nil;
    if (class_getClassMethod(messageClass, @selector(contentType))) {
        contentType = [messageClass contentType];
        @synchronized (self) {
            [self.messageTypeDic setObject:messageClass forKey:contentType];
        }
    }
}

- (JMessageContent *)contentWithData:(NSData *)data
                         contentType:(NSString *)type {
    id content = nil;
    @synchronized (self) {
        Class cls = self.messageTypeDic[type];
        content = [[cls alloc] init];
    }
    [content decode:data];
    return content;
}
@end
