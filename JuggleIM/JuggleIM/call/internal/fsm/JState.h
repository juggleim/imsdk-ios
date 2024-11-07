//
//  JState.h
//  JuggleCall
//
//  Created by Fei Li on 2024/10/9.
//

#import <Foundation/Foundation.h>

@interface JState : NSObject

- (instancetype)initWithName:(NSString *)name
                  superState:(JState *)superState;
- (BOOL)stateDidEnter;
- (BOOL)stateDidLeave;
- (BOOL)event:(NSInteger)event
     userInfo:(NSDictionary *)userInfo;

@property (nonatomic, copy) NSString *name;
@property (nonatomic, strong) JState *superState;

@end
