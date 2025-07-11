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

/// 用户被邀请（多人通话中使用）
/// - Parameter userId: 被邀请的用户 id
- (void)usersDidInvite:(NSArray <NSString *> *)userIdList
             inviterId:(NSString *)inviterId;

/// 用户加入通话（多人通话中使用）
/// - Parameter userId: 用户 id
- (void)usersDidConnect:(NSArray <NSString *> *)userIdList;

/// 用户退出通话（多人通话中使用）
/// - Parameter userId: 用户 id
- (void)usersDidLeave:(NSArray <NSString *> *)userIdList;

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

/// 用户声音大小变化回调
/// - Parameter soundLevels: 由 userId 为 key，声音大小为 value 的字典
- (void)soundLevelDidUpdate:(NSDictionary<NSString *,NSNumber *> *)soundLevels;

/// 通话中的错误回调
/// - Parameter errorCode: 错误码
- (void)errorDidOccur:(JCallErrorCode)errorCode;

@end

@protocol JCallSession <NSObject>
/// 通话 id
@property (nonatomic, copy) NSString *callId;
/// 是否多人通话，NO 表示一对一通话
@property (nonatomic, assign) BOOL isMultiCall;
/// 媒体类型（语音/视频）
@property (nonatomic, assign) JCallMediaType mediaType;
/// 通话状态
@property (nonatomic, assign) JCallStatus callStatus;
/// 呼叫开始时间（多人会话中当前用户被呼叫的时间，不一定等于整个通话开始的时间）
@property (nonatomic, assign) long long startTime;
/// 当前用户加入通话的时间
@property (nonatomic, assign) long long connectTime;
/// 当前用户结束通话的时间
@property (nonatomic, assign) long long finishTime;
/// 通话的发起人 id
@property (nonatomic, copy) NSString *owner;
/// 邀请当前用户加入通话的用户 id
@property (nonatomic, copy) NSString *inviterId;
/// 通话结束原因
@property (nonatomic, assign) JCallFinishReason finishReason;
/// 通话参与者（除当前用户外的其他参与者）
@property (nonatomic, copy, readonly) NSArray <JCallMember *> *members;
/// 当前用户
@property (nonatomic, strong, readonly) JCallMember *currentCallMember;

- (void)addDelegate:(id<JCallSessionDelegate>)delegate;

/// 接听来电
- (void)accept;

/// 挂断电话
- (void)hangup;

/// 开启摄像头
/// - Parameter isEnable: 是否开启
- (void)enableCamera:(BOOL)isEnable;

/// 设置用户的视频 view
/// - Parameters:
///   - view: 视频 view
///   - userId: 用户 id（当前用户或者会话中的其他用户）
- (void)setVideoView:(UIView *)view
           forUserId:(NSString *)userId;

/// 开始预览
/// - Parameter view: 预览的视频 view
- (void)startPreview:(UIView *)view;

/// 设置麦克风静音
/// - Parameter isMute: 是否静音
- (void)muteMicrophone:(BOOL)isMute;

/// 设置扬声器静音
/// - Parameter isMute: 是否静音
- (void)muteSpeaker:(BOOL)isMute;

/// 设置外放声音
/// - Parameter isEnable: YES 使用外放扬声器；NO 使用听筒
- (void)setSpeakerEnable:(BOOL)isEnable;

/// 切换摄像头，默认 YES 使用前置摄像头
/// - Parameter isEnable: YES 使用前置摄像头；NO 使用后置摄像头
- (void)useFrontCamera:(BOOL)isEnable;

#pragma mark -
/// 呼叫用户加入通话（isMultiCall 为 NO 时不支持该功能）
/// - Parameter userIdList: 呼叫的用户 id 列表
- (void)inviteUsers:(NSArray <NSString *> *)userIdList;

@end
