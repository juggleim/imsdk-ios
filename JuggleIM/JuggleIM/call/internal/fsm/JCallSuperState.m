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
            // idle 状态处理
            // 其它状态下 invite 两次不会是同一个 callSession
            break;
            
        case JCallEventInviteFail:
            // do nothing
            // outgoing 状态处理
            // 其它状态下忽略
            break;
            
        case JCallEventReceiveInvite:
            // do nothing
            // idle 状态处理
            // 其它状态下忽略（服务端不会给已在房间内的用户发送同一个 callId 的 invite）
            break;
            
        case JCallEventHangup:
            self.callSessionImpl.finishReason = JCallFinishReasonHangup;
            [self.callSessionImpl signalHangup];
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
            // incoming 状态处理
            // 其它状态忽略
            break;
            
        case JCallEventAcceptFail:
            // do nothing
            // incoming 状态处理
            // 其它状态忽略
            break;
            
        case JCallEventReceiveAccept:
            // TODO: 更新 member 状态
            // outgoing 状态处理
            // 其它状态忽略
            break;
            
        case JCallEventReceiveHangup:
        {
            NSString *userId = userInfo[@"userId"];
            [self.callSessionImpl memberHangup:userId];
            if (!self.callSessionImpl.isMultiCall) {
                [self.callSessionImpl transitionToIdleState];
            }
        }
            break;
            
        case JCallEventJoinChannelDone:
            // do nothing
            // connecting 状态处理
            // 其它状态忽略
            break;
            
        case JCallEventJoinChannelFail:
            // do nothing
            // connecting 状态处理
            // 其它状态忽略
            break;
            
        case JCallEventParticipantJoinChannel:
            break;
            
        case JCallEventParticipantLeaveChannel:
            break;
            
        case JCallEventParticipantEnableCamera:
            break;
            
        case JCallEventParticipantEnableMic:
            break;
            
        default:
            break;
    }
    
    return YES;
}

@end
