//
//  JCallEventUtil.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/9.
//

#import "JCallEventUtil.h"

@implementation JCallEventUtil

static NSString *JCallEventNameMap[] = {
    [JCallEventInvite] = @"invite",
    [JCallEventAccept] = @"accept",
    [JCallEventHangup] = @"hang up",
    [JCallEventAcceptAfterHangupOther] = @"accept after hangup other",
    
    [JCallEventInviteDone] = @"invite done",
    [JCallEventInviteFail] = @"invite fail",
    [JCallEventInviteTimeOut] = @"invite time out",
    [JCallEventAcceptDone] = @"accept done",
    [JCallEventAcceptFail] = @"accept fail",
    [JCallEventIncomingTimeOut] = @"incoming time out",
    
    [JCallEventReceiveInvite] = @"receive invite",
    [JCallEventReceiveInviteOthers] = @"receive invite others",
    [JCallEventReceiveAccept] = @"receive accept",
    [JCallEventReceiveHangup] = @"receive hang up",
    [JCallEventReceiveSelfQuit] = @"receive self quit",
    [JCallEventReceiveQuit] = @"receive quit",
    [JCallEventRoomDestroy] = @"room destroy",
    
    [JCallEventJoin] = @"join",
    [JCallEventJoinChannelDone] = @"join channel done",
    [JCallEventJoinChannelFail] = @"join channel fail",
    [JCallEventReceiveJoin] = @"receive join",
    
    [JCallEventParticipantJoinChannel] = @"participant join channel",
    [JCallEventParticipantLeaveChannel] = @"participant leave channel",
    [JCallEventParticipantEnableCamera] = @"participant enable camera",
    [JCallEventParticipantEnableMic] = @"participant enable mic",
    
    [JCallEventSoundLevelUpdate] = @"sound level update",
    [JCallEventVideoFirstFrameRender] = @"video first frame render",
    
    [40] = @"unknown"
};

+ (NSString *)nameOfEvent:(NSInteger)event {
    return JCallEventNameMap[event];
}

@end
