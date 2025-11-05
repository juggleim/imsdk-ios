//
//  JGetFavoriteMessageOption.h
//  JuggleIM
//
//  Created by Fei Li on 2025/8/13.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JGetFavoriteMessageOption : NSObject
/// 查询的起始位置，第一次可以传空，后续可以从成功回调里获取下一次的 offset
@property (nonatomic, copy) NSString *offset;
/// 查询数量
@property (nonatomic, assign) int count;
@end

NS_ASSUME_NONNULL_END
