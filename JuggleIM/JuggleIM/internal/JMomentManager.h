//
//  JMomentManager.h
//  JuggleIM
//
//  Created by Fei Li on 2025/11/13.
//

#import <Foundation/Foundation.h>
#import "JMomentProtocol.h"
#import "JIMCore.h"

NS_ASSUME_NONNULL_BEGIN

@interface JMomentManager : NSObject <JMomentProtocol>
- (instancetype)initWithCore:(JIMCore *)core;
@end

NS_ASSUME_NONNULL_END
