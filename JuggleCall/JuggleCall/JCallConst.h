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
    JCallStatusIdle,
    /// 呼出
    JCallStatusOutgoing,
    /// 被呼叫
    JCallStatusIncoming,
    /// 连接中
    JCallStatusConnecting,
    /// 连接成功
    JCallStatusConnected
};

/// 通话结束原因
typedef NS_ENUM(NSInteger, JCallFinishReason) {
    /// 当前用户挂断已接通的来电
    JCallFinishReasonHangUp,
    /// 当前用户拒接来电
    JCallFinishReasonDecline,
    /// 当前用户忙线
    JCallFinishReasonBusy,
    /// 当前用户未接听
    JCallFinishReasonNoResponse,
    /// 当前用户取消呼叫
    JCallFinishReasonCancel,
    /// 对端用户挂断已接通的来电
    JCallFinishReasonOtherSideHangUp,
    /// 对端用户拒接来电
    JCallFinishReasonOtherSideDecline,
    /// 对端用户忙线
    JCallFinishReasonOtherSideBusy,
    /// 对端用户未接听
    JCallFinishReasonOtherSideNoResponse,
    /// 对端用户取消呼叫
    JCallFinishReasonOtherSideCancel,
    /// 网络出错
    JCallFinishReasonNetworkError
    
};

#endif /* JCallConst_h */
