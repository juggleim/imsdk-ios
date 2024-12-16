//
//  JCallManager.h
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import <Foundation/Foundation.h>
#import "JCallProtocol.h"
#import "JIMCore.h"

NS_ASSUME_NONNULL_BEGIN

@interface JCallManager : NSObject <JCallProtocol>
- (instancetype)initWithCore:(JIMCore *)core;
- (void)connectSuccess;
// IM 被踢下线
- (void)imKick;
@end

NS_ASSUME_NONNULL_END
