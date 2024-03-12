//
//  JUserInfoManager.h
//  JetIM
//
//  Created by Nathan on 2024/3/12.
//

#import <Foundation/Foundation.h>
#import "JUserInfoProtocol.h"
#import "JetIMCore.h"

NS_ASSUME_NONNULL_BEGIN

@interface JUserInfoManager : NSObject <JUserInfoProtocol>

- (instancetype)initWithCore:(JetIMCore *)core;

@end

NS_ASSUME_NONNULL_END
