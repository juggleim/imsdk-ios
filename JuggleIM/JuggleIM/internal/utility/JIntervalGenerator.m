//
//  JIntervalGenerator.m
//  JuggleIM
//
//  Created by Fei Li on 2024/9/11.
//

#import "JIntervalGenerator.h"
#import "JLogger.h"

@interface JIntervalGenerator ()
@property (nonatomic, assign) float interval;
@end

@implementation JIntervalGenerator

- (float)getNextInterval {
    float result = self.interval;
    if (result < 1) {
        self.interval = 1;
    } else if (result >= 32) {
        
    } else {
        self.interval *= 2;
    }
    JLogI(@"J-Itvl", @"interval is %f", result);
    return result;
}

- (void)reset {
    self.interval = 0.3;
}

@end
