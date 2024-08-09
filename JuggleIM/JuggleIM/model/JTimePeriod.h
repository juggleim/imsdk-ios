//
//  JTimePeriod.h
//  JuggleIM
//
//  Created by Nathan on 2024/8/9.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JTimePeriod : NSObject

/// 开始时间，格式为 "HH:mm"
@property (nonatomic, copy) NSString *startTime;

/// 结束时间，格式为 "HH:mm"
@property (nonatomic, copy) NSString *endTime;


@end

NS_ASSUME_NONNULL_END
