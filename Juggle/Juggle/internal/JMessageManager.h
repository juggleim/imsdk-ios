//
//  JMessageManager.h
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

#import <Foundation/Foundation.h>
#import "JMessageProtocol.h"
#import "JuggleCore.h"

NS_ASSUME_NONNULL_BEGIN

@interface JMessageManager : NSObject <JMessageProtocol>
- (instancetype)initWithCore:(JuggleCore *)core;
@end

NS_ASSUME_NONNULL_END
