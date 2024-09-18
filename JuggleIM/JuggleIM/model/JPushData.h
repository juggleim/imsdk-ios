//
//  JPushData.h
//  JuggleIM
//
//  Created by Fei Li on 2024/9/18.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JPushData : NSObject

/// 推送内容，用于通知栏的展示
@property (nonatomic, copy) NSString *content;

/// 扩展字段，可以携带自定义 json
@property (nonatomic, copy) NSString *extra;

@end

NS_ASSUME_NONNULL_END
