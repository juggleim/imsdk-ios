//
//  JGetMomentOption.h
//  JuggleIM
//
//  Created by Fei Li on 2025/11/13.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JuggleIMConst.h>

NS_ASSUME_NONNULL_BEGIN

@interface JGetMomentOption : NSObject

/// Starting timestamp for querying moments; 0 means the current time
@property (nonatomic, assign) long long startTime;
/// Fetch count，maximum 50
@property (nonatomic, assign) int count;
/// Fetch direction
@property (nonatomic, assign) JPullDirection direction;
/// Fetch the moment list published by the specified user. Empty means fetching the moment list for all friends. Only the getMomentList API supports this; the getCachedMomentList API does not support this parameter
@property (nonatomic, copy, nullable) NSString *userId;

- (NSDictionary *)toDictionary;

@end

NS_ASSUME_NONNULL_END
