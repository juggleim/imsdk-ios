//
//  JCall.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/9.
//

#import "JCall.h"
#import "JStateMachine.h"
#import "JIdleState.h"
#import <JuggleIM/JuggleIM.h>

@implementation JCall

static JCall *_instance;

+ (instancetype)shared {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [[self alloc] init];
    });
    return _instance;
}

- (void)startCall {
//    JStateMachine *stateMachine = [[JStateMachine alloc] initWithName:@"JCall"];
//    JIdleState *idleState = [[JIdleState alloc] initWithName:@"idle" superState:nil];
//    [stateMachine setInitialState:idleState];
//    [stateMachine start];
//    [stateMachine event:0 userInfo:nil];
    
    
}

@end
