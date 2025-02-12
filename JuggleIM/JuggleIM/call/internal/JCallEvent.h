//
//  JCallEvent.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/23.
//

#ifndef JCallEvent_h
#define JCallEvent_h

typedef NS_ENUM(NSInteger, JCallEvent) {
    /// Signal outgoing
    JCallEventInvite,
    JCallEventAccept,
    JCallEventHangup, //失败暂不处理，对端会收到 media 的退出或者超时
    JCallEventAcceptAfterHangupOther,
    //JCallEventAddParticipants,
    
    /// Signal outgoing callback
    JCallEventInviteDone,
    JCallEventInviteFail,
    JCallEventInviteTimeOut,
    //JCallEventAddParticipantsFail,
    JCallEventAcceptDone,
    JCallEventAcceptFail,
    JCallEventIncomingTimeOut,
    
    /// Signal incoming
    JCallEventReceiveInvite,
    JCallEventReceiveInviteOthers,
    JCallEventReceiveAccept,
    JCallEventReceiveHangup,
    JCallEventReceiveSelfQuit,
    JCallEventReceiveQuit,
    JCallEventRoomDestroy,
    
    /// Media outgoing
    
    /// Media outgoing callback
    JCallEventJoinChannelDone, //join channel 暂不需要产生事件，只是进入某一个状态时的一个动作
    JCallEventJoinChannelFail,
    
    /// Media incoming
    JCallEventParticipantJoinChannel,
    JCallEventParticipantLeaveChannel,
    JCallEventParticipantEnableCamera,
    JCallEventParticipantEnableMic,
    
    /// Error
    
};


#endif /* JCallEvent_h */
