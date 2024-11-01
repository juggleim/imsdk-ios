//
//  JCallSession.h
//  JuggleCall
//
//  Created by Fei Li on 2024/10/9.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <JuggleIM/JCallConst.h>
#import <JuggleIM/JuggleIMConst.h>
#import <JuggleIM/JCallMember.h>

@protocol JCallSessionDelegate <NSObject>

@optional

/// 通话已接通
- (void)callDidConnect;

/// 通话已结束
/// - Parameter finishReason: 结束原因
- (void)callDidFinish:(JCallFinishReason)finishReason;

/// 用户被邀请
/// - Parameter userId: 被邀请的用户 id
- (void)userDidInvite:(NSString *)userId;

/// 用户加入通话
/// - Parameter userId: 用户 id
- (void)userDidConnect:(NSString *)userId;

/// 用户退出通话
/// - Parameter userId: 用户 id
- (void)userDidLeave:(NSString *)userId;

/// 用户开启/关闭摄像头
/// - Parameters:
///   - enable: 是否开启
///   - userId: 用户 id
- (void)userCamaraDidChange:(BOOL)enable
                     userId:(NSString *)userId;

/// 用户开启/关闭麦克风
/// - Parameters:
///   - enable: 是否开启
///   - userId: 用户 id
- (void)userMicrophoneDidChange:(BOOL)enable
                         userId:(NSString *)userId;

/// 通话中的错误回调
/// - Parameter errorCode: 错误码
- (void)errorDidOccur:(JCallErrorCode)errorCode;

@end

@protocol JCallSession <NSObject>
/// 通话 id
@property (nonatomic, copy) NSString *callId;
/// 是否多人通话，NO 表示一对一通话
@property (nonatomic, assign) BOOL isMultiCall;
/// 通话状态
@property (nonatomic, assign) JCallStatus callStatus;
/// 是否开启视频，默认 NO 关闭
@property (nonatomic, assign) BOOL cameraEnable;
/// 是否开启麦克风，默认 YES 开启
@property (nonatomic, assign) BOOL microphoneEnable;
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
@property (nonatomic, copy) NSArray <JCallMember *> *members;

- (void)addDelegate:(id<JCallSessionDelegate>)delegate;

/// 接听来电
- (void)accept;

/// 挂断电话
- (void)hangup;

/// 设置麦克风静音
/// - Parameter isMute: 是否静音
- (void)setMicrophoneMute:(BOOL)isMute;

#pragma mark - TODO:
/// 呼叫用户加入通话（isMultiCall 为 NO 时不支持该功能）
/// - Parameter userIdList: 呼叫的用户 id 列表
- (void)inviteUsers:(NSArray <NSString *> *)userIdList;

/// 设置用户对应的视频 view
/// - Parameters:
///   - view: 视频 view
///   - userId: 用户 id
- (void)setVideoView:(UIView *)view
           forUserId:(NSString *)userId;

@end
