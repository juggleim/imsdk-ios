//
//  JStateMachine.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/9.
//

#import "JStateMachine.h"
#import "JState.h"
#import "JLogger.h"

@interface JStateMachine ()
@property (nonatomic, strong) JState *currentState;
@property (nonatomic, strong) dispatch_queue_t fsmQueue;
@end

@implementation JStateMachine

- (instancetype)initWithName:(NSString *)name {
    self = [super init];
    if (self) {
        self.fsmQueue = dispatch_queue_create([name UTF8String], NULL);
    }
    return self;
}

- (void)event:(NSInteger)event
         name:(NSString *)eventName
     userInfo:(NSDictionary *)userInfo {
    dispatch_async(self.fsmQueue, ^{
        JLogI(@"FSM-Sm", @"state %@, event %@", self.currentState.name, eventName);
        JState *state = self.currentState;
        while (state) {
            if ([state event:event userInfo:userInfo]) {
                break;
            } else {
                state = state.superState;
            }
        }
    });
}

- (void)setInitialState:(JState *)state {
    self.currentState = state;
}

- (void)start {
    JState *state = self.currentState;
    while (state) {
        if ([state stateDidEnter]) {
            break;
        } else {
            state = state.superState;
        }
    }
}

- (void)transitionTo:(JState *)state {
    JLogI(@"FSM-Sm", @"leave state %@", self.currentState.name);
    JState *current = self.currentState;
    while (current) {
        if ([current stateDidLeave]) {
            break;
        } else {
            current = current.superState;
        }
    }
    
    self.currentState = state;
    
    JLogI(@"FSM-Sm", @"enter state %@", state.name);
    current = self.currentState;
    while (current) {
        if ([current stateDidEnter]) {
            break;
        } else {
            current = current.superState;
        }
    }
}

@end
