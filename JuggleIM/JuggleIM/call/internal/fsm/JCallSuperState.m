//
//  JBaseState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JCallSuperState.h"
#import "JCallEvent.h"
#import "JUserInfo.h"

@implementation JCallSuperState

- (BOOL)stateDidEnter {
    return YES;
}

- (BOOL)stateDidLeave {
    return YES;
}

- (BOOL)event:(NSInteger)event
     userInfo:(NSDictionary *)userInfo {
    
    switch (event) {
        case JCallEventInvite:
//            [self.callSessionImpl error:JCallErrorCodeCallExist];
            // do nothing
            // Handled in the idle state.
            // Handled in the connected state.
            // In other states, two invite events will not be for the same callSession.
            break;
            
        case JCallEventInviteDone:
            // do nothing
            // Handled in the outgoing state.
            // Handled in the connected state.
            // Ignored in other states.
            break;
            
        case JCallEventInviteFail:
            // do nothing
            // Handled in the outgoing state.
            // Handled in the connected state.
            // Ignored in other states.
            break;
            
        case JCallEventReceiveInvite:
            // do nothing
            // Handled in the idle state.
            // Ignored in other states. Server will not send an invite with the same callId to a user already in the room.
            break;
            
        case JCallEventReceiveInviteOthers:
            // Do nothing in the idle state.
            // Other states are handled here.
        {
            JUserInfo *inviter = userInfo[@"inviter"];
            NSArray<JUserInfo *> *targetUsers = userInfo[@"targetUsers"];
            [self.callSessionImpl addInviteMembers:targetUsers inviter:inviter];
            break;
        }
            
        case JCallEventHangup:
            if (self.callSessionImpl.callStatus == JCallStatusIncoming) {
                self.callSessionImpl.finishReason = JCallFinishReasonDecline;
            } else if (self.callSessionImpl.callStatus == JCallStatusOutgoing) {
                self.callSessionImpl.finishReason = JCallFinishReasonCancel;
            } else {
                self.callSessionImpl.finishReason = JCallFinishReasonHangup;
            }
            [self.callSessionImpl signalHangup];
            if (self.callSessionImpl.callStatus == JCallStatusIncoming) {
                [self.callSessionImpl transitionToIdleStateWithoutMediaQuit];
            } else {
                [self.callSessionImpl transitionToIdleState];
            }
            break;
            
        case JCallEventReceiveSelfQuit:
            if (self.callSessionImpl.callStatus == JCallStatusConnected) {
                self.callSessionImpl.finishReason = JCallFinishReasonNetworkError;
            } else if (self.callSessionImpl.callStatus == JCallStatusIncoming) {
                self.callSessionImpl.finishReason = JCallFinishReasonNoResponse;
            }
            [self.callSessionImpl transitionToIdleState];
            break;
            
        case JCallEventRoomDestroy:
            self.callSessionImpl.finishReason = JCallFinishReasonRoomDestroy;
            [self.callSessionImpl transitionToIdleState];
            break;
            
        case JCallEventAccept:
            [self.callSessionImpl error:JCallErrorCodeCantAcceptWhileNotInvited];
            break;
            
        case JCallEventAcceptDone:
            // do nothing
            // Handled in the incoming state.
            // Ignored in other states.
            break;
            
        case JCallEventAcceptFail:
            // do nothing
            // Handled in the incoming state.
            // Ignored in other states.
            break;
            
        case JCallEventReceiveAccept:
        {
            // Handled in the outgoing state when another user accepts.
            // Handled in the incoming state when current user accepts on another client.
            NSString *userId = userInfo[@"userId"];
            [self.callSessionImpl memberAccept:userId];
            break;
        }
            
        case JCallEventReceiveHangup:
        {
            // Handled in the incoming state when the current user hangs up on another client.
            NSString *userId = userInfo[@"userId"];
            [self.callSessionImpl memberHangup:userId];
            if (!self.callSessionImpl.isMultiCall) {
                [self.callSessionImpl transitionToIdleState];
            }
            break;
        }
            
        case JCallEventReceiveQuit:
        {
            // Unlike JCallEventReceiveHangup, the incoming state will not receive quit events from the current user on another client.
            NSArray <NSString *> *userIdList = userInfo[@"userIdList"];
            [self.callSessionImpl membersQuit:userIdList];
            if (!self.callSessionImpl.isMultiCall) {
                [self.callSessionImpl transitionToIdleState];
            }
            break;
        }
            
        case JCallEventJoinChannelDone:
            // do nothing
            // Handled in the connecting state.
            // Ignored in other states.
            break;
            
        case JCallEventJoinChannelFail:
            // do nothing
            // Handled in the connecting state.
            // Ignored in other states.
            break;
            
        case JCallEventParticipantJoinChannel:
        {
            NSArray <NSString *> *userIdList = userInfo[@"userIdList"];
            [self.callSessionImpl membersConnected:userIdList];
            break;
        }
            
        case JCallEventParticipantLeaveChannel:
            break;
            
        case JCallEventParticipantEnableCamera:
        {
            BOOL enable = [(NSNumber *)userInfo[@"enable"] boolValue];
            NSString *userId = userInfo[@"userId"];
            [self.callSessionImpl cameraEnable:enable userId:userId];
            break;
        }
            
        case JCallEventParticipantEnableMic:
        {
            BOOL enable = [(NSNumber *)userInfo[@"enable"] boolValue];
            NSString *userId = userInfo[@"userId"];
            [self.callSessionImpl microphoneEnable:enable userId:userId];
            break;
        }
            
        case JCallEventSoundLevelUpdate:
        {
            NSDictionary *soundLevelDic = userInfo;
            [self.callSessionImpl soundLevelUpdate:soundLevelDic];
            break;
        }
            
        case JCallEventVideoFirstFrameRender:
        {
            NSString *userId = userInfo[@"userId"];
            [self.callSessionImpl videoFirstFrameRender:userId];
            break;
        }
            
        case JCallEventJoin:
        {
            // do nothing
            // Handled in the idle state.
            // Ignored in other states.
            break;
        }
            
        case JCallEventJoinDone:
            // do nothing
            // Handled in the join state.
            // Ignored in other states.
            break;
            
        case JCallEventJoinFail:
            // do nothing
            // Handled in the join state.
            // Ignored in other states.
            break;
            
        case JCallEventReceiveJoin:
        {
            NSArray <JUserInfo *> *userList = userInfo[@"users"];
            [self.callSessionImpl membersJoin:userList];
            break;
        }
            
        default:
            break;
    }
    
    return YES;
}

@end
