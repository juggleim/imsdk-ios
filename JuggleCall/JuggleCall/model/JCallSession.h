//
//  JCallSession.h
//  JuggleCall
//
//  Created by Fei Li on 2024/10/9.
//

#import <Foundation/Foundation.h>
#import <JuggleCall/JCallConst.h>

@interface JCallSession : NSObject
/// 通话 id
@property (nonatomic, copy) NSString *callId;
/// 通话状态
@property (nonatomic, assign) JCallStatus callStatus;
/// 是否开启视频，默认 NO 关闭
@property (nonatomic, assign) BOOL cameraEnable;
/// 是否开启麦克风，默认 YES kaiq
@property (nonatomic, assign) BOOL micEnable;
/// 呼叫开始时间（多人会话中当前用户被呼叫的时间，不一定等于整个通话开始的时间）
@property (nonatomic, assign) long long startTime;
/// 当前用户加入通话的时间
@property (nonatomic, assign) long long connectTime;
/// 当前用户结束通话的时间
@property (nonatomic, assign) long long finishTime;
/// 通话的发起人 id
@property (nonatomic, copy) NSString *owner;
/// 邀请当前用户加入通话的用户 id
@property (nonatomic, copy) NSString *inviter;
/// 通话结束原因
@property (nonatomic, assign) JCallFinishReason finishReason;
/// 通话参与者（除当前用户外的其他参与者）
@property (nonatomic, copy) NSArray *participants;
@end
