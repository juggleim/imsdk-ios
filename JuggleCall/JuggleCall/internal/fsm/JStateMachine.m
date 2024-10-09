//
//  JStateMachine.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/9.
//

#import "JStateMachine.h"
#import "JState.h"
#import "JLogger.h"
#import "JCallEventUtil.h"

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

- (void)event:(NSInteger)event userInfo:(id)userInfo {
    dispatch_async(self.fsmQueue, ^{
        JLogI(@"CALL-Fsm", @"state %@, event %@", self.currentState.name, [JCallEventUtil nameOfEvent:event]);
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
}

@end
