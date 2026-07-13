//
//  JGroupMessageReadInfo.h
// JuggleIM
//
//  Created by Nathan on 2024/3/6.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JGroupMessageReadInfo : NSObject

/// Read user count
@property (nonatomic, assign) int readCount;

/// Total user count in the group
@property (nonatomic, assign) int memberCount;

@end

NS_ASSUME_NONNULL_END
