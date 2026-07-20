//
//  JCallConst.h
//  JuggleCall
//
//  Created by Fei Li on 2024/10/9.
//

#ifndef JCallConst_h
#define JCallConst_h

/// Call status.
typedef NS_ENUM(NSInteger, JCallStatus) {
    /// No active call.
    JCallStatusIdle = 0,
    /// Incoming status.
    JCallStatusIncoming = 1,
    /// Outgoing status.
    JCallStatusOutgoing = 2,
    /// Connecting status.
    JCallStatusConnecting = 3,
    /// Connected status.
    JCallStatusConnected = 4,
    /// Joining proactively.
    JCallStatusJoin = 5
};

typedef NS_ENUM(NSInteger, JCallMediaType) {
    /// Audio call.
    JCallMediaTypeVoice = 0,
    /// Video call.
    JCallMediaTypeVideo = 1
};

/// Call finish reason.
typedef NS_ENUM(NSInteger, JCallFinishReason) {
    /// Unknown reason.
    JCallFinishReasonUnknown = 0,
    /// Current user hung up a connected call.
    JCallFinishReasonHangup = 1,
    /// Current user declined the incoming call.
    JCallFinishReasonDecline = 2,
    /// Current user was busy.
    JCallFinishReasonBusy = 3,
    /// Current user did not answer.
    JCallFinishReasonNoResponse = 4,
    /// Current user canceled the call.
    JCallFinishReasonCancel = 5,
    /// The remote user hung up a connected call.
    JCallFinishReasonOtherSideHangup = 6,
    /// The remote user declined the incoming call.
    JCallFinishReasonOtherSideDecline = 7,
    /// The remote user was busy.
    JCallFinishReasonOtherSideBusy = 8,
    /// The remote user did not answer.
    JCallFinishReasonOtherSideNoResponse = 9,
    /// The remote user canceled the call.
    JCallFinishReasonOtherSideCancel = 10,
    /// The room was destroyed.
    JCallFinishReasonRoomDestroy = 11,
    /// Network error.
    JCallFinishReasonNetworkError = 12,
    /// Current user accepted the incoming call on another client.
    JCallFinishReasonAcceptOnOtherClient = 13,
    /// Current user hung up the call on another client.
    JCallFinishReasonHangupOnOtherClient = 14
};

typedef NS_ENUM(NSInteger, JCallErrorCode) {
    /// Success.
    JCallSuccess = 0,
    /// Already in a call.
    JCallErrorCodeCallExist = 1,
    /// Cannot accept a call when not invited.
    JCallErrorCodeCantAcceptWhileNotInvited = 2,
    /// Failed to accept the call.
    JCallErrorCodeAcceptFail = 3,
    /// Failed to join the media room.
    JCallErrorCodeJoinMediaRoomFail = 4,
    /// Invalid parameter.
    JCallErrorCodeInvalidParameter = 5,
    /// Failed to invite users.
    JCallErrorCodeInviteFail = 6,
    /// Failed to join the room.
    JCallErrorCodeJoinRoomFail = 7
};

#endif /* JCallConst_h */
