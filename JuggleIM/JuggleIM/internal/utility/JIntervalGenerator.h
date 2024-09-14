//
//  JIntervalGenerator.h
//  JuggleIM
//
//  Created by Fei Li on 2024/9/11.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JIntervalGenerator : NSObject

- (int)getNextInterval;

- (void)reset;

@end

NS_ASSUME_NONNULL_END
