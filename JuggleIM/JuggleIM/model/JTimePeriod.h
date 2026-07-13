//
//  JTimePeriod.h
//  JuggleIM
//
//  Created by Nathan on 2024/8/9.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JTimePeriod : NSObject

/// Start time, in "HH:mm" format
@property (nonatomic, copy) NSString *startTime;

/// End time, in "HH:mm" format
@property (nonatomic, copy) NSString *endTime;


@end

NS_ASSUME_NONNULL_END
