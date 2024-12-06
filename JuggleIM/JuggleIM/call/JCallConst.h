//
//  JCallConst.h
//  JuggleCall
//
//  Created by Fei Li on 2024/10/9.
//

#ifndef JCallConst_h
#define JCallConst_h

/// 通话状态
typedef NS_ENUM(NSInteger, JCallStatus) {
    /// 无通话
    JCallStatusIdle = 0,
    /// 被呼叫
    JCallStatusIncoming = 1,
    /// 呼出
    JCallStatusOutgoing = 2,
    /// 连接中
    JCallStatusConnecting = 3,
    /// 连接成功
    JCallStatusConnected = 4
};

typedef NS_ENUM(NSInteger, JCallMediaType) {
    /// 语音通话
    JCallMediaTypeVoice = 0,
    /// 视频通话
    JCallMediaTypeVideo = 1
};

/// 通话结束原因
typedef NS_ENUM(NSInteger, JCallFinishReason) {
    /// 未知原因
    JCallFinishReasonUnknown = 0,
    /// 当前用户挂断已接通的来电
    JCallFinishReasonHangup = 1,
    /// 当前用户拒接来电
    JCallFinishReasonDecline = 2,
    /// 当前用户忙线
    JCallFinishReasonBusy = 3,
    /// 当前用户未接听
    JCallFinishReasonNoResponse = 4,
    /// 当前用户取消呼叫
    JCallFinishReasonCancel = 5,
    /// 对端用户挂断已接通的来电
    JCallFinishReasonOtherSideHangup = 6,
    /// 对端用户拒接来电
    JCallFinishReasonOtherSideDecline = 7,
    /// 对端用户忙线
    JCallFinishReasonOtherSideBusy = 8,
    /// 对端用户未接听
    JCallFinishReasonOtherSideNoResponse = 9,
    /// 对端用户取消呼叫
    JCallFinishReasonOtherSideCancel = 10,
    /// 房间被销毁
    JCallFinishReasonRoomDestroy = 11,
    /// 网络出错
    JCallFinishReasonNetworkError = 12,
    /// 当前用户在其它端接听来电
    JCallFinishReasonAcceptOnOtherClient = 13,
    /// 当前用户在其它端挂断来电
    JCallFinishReasonHangupOnOtherClient = 14
};

typedef NS_ENUM(NSInteger, JCallErrorCode) {
    /// 成功
    JCallSuccess = 0,
    /// 已在通话中
    JCallErrorCodeCallExist = 1,
    /// 没被邀请时不能接听通话
    JCallErrorCodeCantAcceptWhileNotInvited = 2,
    /// 接听失败
    JCallErrorCodeAcceptFail = 3,
    /// 加入 Media 房间失败
    JCallErrorCodeJoinMediaRoomFail = 4,
    /// 参数错误
    JCallErrorCodeInvalidParameter = 5
};

#endif /* JCallConst_h */
