//
//  JStateMachine.h
//  JuggleCall
//
//  Created by Fei Li on 2024/10/9.
//

#import <Foundation/Foundation.h>
#import "JState.h"

@interface JStateMachine : NSObject

- (instancetype)initWithName:(NSString *)name;
- (void)setInitialState:(JState *)state;
- (void)start;
- (void)event:(NSInteger)event
     userInfo:(NSDictionary *)userInfo;
- (void)transitionTo:(JState *)state;

@end
