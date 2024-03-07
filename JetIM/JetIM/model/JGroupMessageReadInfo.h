//
//  JGroupMessageReadInfo.h
//  JetIM
//
//  Created by Nathan on 2024/3/6.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JGroupMessageReadInfo : NSObject

/// 已读人数
@property (nonatomic, assign) int readCount;

/// 群内总人数
@property (nonatomic, assign) int memberCount;

@end

NS_ASSUME_NONNULL_END
