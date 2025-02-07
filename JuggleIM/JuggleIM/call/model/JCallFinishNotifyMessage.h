//
//  JCallFinishNotifyMessage.h
//  JuggleIM
//
//  Created by Fei Li on 2024/11/27.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, JCallFinishNotifyType) {
    // 主叫取消
    JCallFinishNotifyTypeCancel = 0,
    // 被叫拒绝
    JCallFinishNotifyTypeReject = 1,
    // 被叫无应答
    JCallFinishNotifyTypeNoResponse = 2,
    //通话结束
    JCallFinishNotifyTypeComplete = 3
};

@interface JCallFinishNotifyMessage : JMessageContent
// 结束类型
@property (nonatomic, assign) JCallFinishNotifyType finishType;
// 通话时长，单位（ms）
@property (nonatomic, assign) long long duration;
// 通话类型
@property (nonatomic, assign) JCallMediaType mediaType;

@end

NS_ASSUME_NONNULL_END
