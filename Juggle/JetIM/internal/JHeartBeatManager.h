//
//  JHeartBeatManager.h
//  JetIM
//
//  Created by Nathan on 2023/12/26.
//

#import <Foundation/Foundation.h>
#import "JetIMCore.h"

NS_ASSUME_NONNULL_BEGIN

@interface JHeartBeatManager : NSObject
- (instancetype)initWithCore:(JetIMCore *)core;
- (void)start;
- (void)stop;
@end

NS_ASSUME_NONNULL_END
