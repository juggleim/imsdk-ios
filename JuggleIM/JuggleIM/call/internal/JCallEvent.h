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
    JCallEventHangup, // Failures are not handled for now. The remote side will receive the media exit or timeout.
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
    
    /// Signal join
    JCallEventJoin,
    JCallEventJoinDone,
    JCallEventJoinFail,
    JCallEventReceiveJoin,
    
    /// Media outgoing
    
    /// Media outgoing callback
    JCallEventJoinChannelDone, // Joining the channel does not need to produce an event for now; it is only an action when entering a state.
    JCallEventJoinChannelFail,
    
    /// Media incoming
    JCallEventParticipantJoinChannel,
    JCallEventParticipantLeaveChannel,
    JCallEventParticipantEnableCamera,
    JCallEventParticipantEnableMic,
    
    JCallEventSoundLevelUpdate,
    JCallEventVideoFirstFrameRender
    
    /// Error
    
};


#endif /* JCallEvent_h */
