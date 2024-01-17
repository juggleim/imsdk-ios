//
//  JContentTypeCenter.m
//  JetIM
//
//  Created by Nathan on 2023/12/26.
//

#import "JContentTypeCenter.h"
#import <objc/runtime.h>
#import "JMessageContent+internal.h"

@interface JContentTypeCenter ()
@property (nonatomic, strong) NSMutableDictionary *contentTypeDic;
@end

@implementation JContentTypeCenter

static JContentTypeCenter *_instance;

+ (instancetype)shared {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [[self alloc] init];
        _instance.contentTypeDic = [[NSMutableDictionary alloc] init];
    });
    return _instance;
}

- (void)registerContentType:(Class)messageClass {
    NSString *contentType = nil;
    if (class_getClassMethod(messageClass, @selector(contentType))) {
        contentType = [messageClass contentType];
        @synchronized (self) {
            [self.contentTypeDic setObject:messageClass forKey:contentType];
        }
    }
}

- (JMessageContent *)contentWithData:(NSData *)data
                         contentType:(NSString *)type {
    id content = nil;
    @synchronized (self) {
        Class cls = self.contentTypeDic[type];
        content = [[cls alloc] init];
    }
    [content decode:data];
    return content;
}
@end