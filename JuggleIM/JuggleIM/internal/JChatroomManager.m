//
//  JChatroomManager.m
//  JuggleIM
//
//  Created by Fei Li on 2024/8/13.
//

#import "JChatroomManager.h"
#import "JLogger.h"

@interface JChatroomManager ()
@property (nonatomic, strong) JIMCore *core;
@property (nonatomic, strong) NSHashTable <id<JChatroomDelegate>> *delegates;

@end

@implementation JChatroomManager

- (instancetype)initWithCore:(JIMCore *)core {
    self = [super init];
    if (self) {
        self.core = core;
    }
    return self;
}

- (void)addDelegate:(id<JChatroomDelegate>)delegate {
    dispatch_async(self.core.delegateQueue, ^{
        if (!delegate) {
            return;
        }
        [self.delegates addObject:delegate];
    });
}

- (void)joinChatroom:(NSString *)chatroomId {
    [self.core.webSocket joinChatroom:chatroomId
                              success:^(long long timestamp) {
        JLogI(@"CHRM-Join", @"success");
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JChatroomDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(chatroomDidJoin:)]) {
                    [obj chatroomDidJoin:chatroomId];
                }
            }];
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"CHRM-Join", @"error code is %ld", code);
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JChatroomDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(chatroomJoinFail:errorCode:)]) {
                    [obj chatroomJoinFail:chatroomId errorCode:(JErrorCode)code];
                }
            }];
        });
    }];
}

- (void)getChatroomInfo:(NSString *)chatroomId option:(JChatroomInfoOption *)option success:(void (^)(JChatroomInfo *))successBlock error:(void (^)(JErrorCode))errorBlock { 
    
}


- (void)quitChatroom:(NSString *)chatroomId { 
    [self.core.webSocket quitChatroom:chatroomId
                              success:^(long long timestamp) {
        JLogI(@"CHRM-quit", @"success");
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JChatroomDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(chatroomDidQuit:)]) {
                    [obj chatroomDidQuit:chatroomId];
                }
            }];
        });
    } error:^(JErrorCodeInternal code) {
        JLogE(@"CHRM-quit", @"error code is %ld", code);
        dispatch_async(self.core.delegateQueue, ^{
            [self.delegates.allObjects enumerateObjectsUsingBlock:^(id<JChatroomDelegate>  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj respondsToSelector:@selector(chatroomQuitFail:errorCode:)]) {
                    [obj chatroomQuitFail:chatroomId errorCode:(JErrorCode)code];
                }
            }];
        });
    }];
}

#pragma mark -- internal
- (NSHashTable<id<JChatroomDelegate>> *)delegates {
    if (!_delegates) {
        _delegates = [NSHashTable weakObjectsHashTable];
    }
    return _delegates;
}

@end
