//
//  JGetMessageOptions.h
//  JuggleIM
//
//  Created by Fei Li on 2024/8/16.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JGetMessageOptions : NSObject
/// 消息时间戳，传 0 或者不设置时，默认为当前时间
@property (nonatomic, assign) long long startTime;
/// 拉取数量，默认为 100 条，超过 100 时按 100 返回
@property (nonatomic, assign) int count;
/// 拉取的消息类型列表，消息类型获取示例：[JTextMessage contentType]，如果为空则拉取所有类型的消息
@property (nonatomic, strong) NSArray <NSString *> *contentTypes;
@end

NS_ASSUME_NONNULL_END
