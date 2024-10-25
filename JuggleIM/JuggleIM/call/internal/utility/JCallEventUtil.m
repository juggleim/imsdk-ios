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
    [JCallEventInviteFail] = @"invite fail",
    [JCallEventInviteTimeOut] = @"invite time out",
    [JCallEventAcceptDone] = @"accept done",
    [JCallEventAcceptFail] = @"accept fail",
    [JCallEventReceiveInvite] = @"receive invite",
    [JCallEventReceiveAccept] = @"receive accept",
    [JCallEventReceiveHangup] = @"receive hang up",
    [JCallEventJoinChannelDone] = @"join channel done",
    [JCallEventJoinChannelFail] = @"join channel fail",
    [JCallEventParticipantJoinChannel] = @"participant join channel",
    [JCallEventParticipantLeaveChannel] = @"participant leave channel",
    [JCallEventParticipantEnableCamera] = @"participant enable camera",
    [JCallEventParticipantEnableMic] = @"participant enable mic"
};

+ (NSString *)nameOfEvent:(NSInteger)event {
    return JCallEventNameMap[event];
}

@end
