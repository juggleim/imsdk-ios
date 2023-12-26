//
//  JHeartBeatManager.h
//  Juggle
//
//  Created by Nathan on 2023/12/26.
//

#import <Foundation/Foundation.h>
#import "JuggleCore.h"

NS_ASSUME_NONNULL_BEGIN

@interface JHeartBeatManager : NSObject
- (instancetype)initWithCore:(JuggleCore *)core;
- (void)start;
- (void)stop;
@end

NS_ASSUME_NONNULL_END
