//
//  JState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/9.
//

#import "JState.h"
#import "JLogger.h"

@implementation JState

- (instancetype)initWithName:(NSString *)name
                  superState:(nonnull JState *)superState {
    self = [super init];
    if (self) {
        self.name = name;
        if (superState) {
            self.superState = superState;
        }
    }
    return self;
}

- (BOOL)stateDidEnter {
//    JLogI(@"CALL-Fsm", @"enter state %@", self.name);
    return NO;
}

- (BOOL)stateDidLeave {
//    JLogI(@"CALL-Fsm", @"leave state %@", self.name);
    return NO;
}

- (BOOL)event:(NSInteger)event userInfo:(nonnull id)userInfo {
//    JLogI(@"CALL-Fsm", @"%@ handle event %ld", self.name, event);
    return NO;
}

@end
