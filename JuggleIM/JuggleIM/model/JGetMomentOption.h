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

/// 查询朋友圈的起始时间戳，0 表示当前时间
@property (nonatomic, assign) long long startTime;
/// 拉取数量，最大不超过 50
@property (nonatomic, assign) int count;
/// 拉取方向
@property (nonatomic, assign) JPullDirection direction;
/// 获取指定用户发布的朋友圈列表，为空表示获取所有好友的朋友圈列表。只支持 getMomentList 接口；getCachedMomentList 接口不支持该参数
@property (nonatomic, copy, nullable) NSString *userId;

- (NSDictionary *)toDictionary;

@end

NS_ASSUME_NONNULL_END
