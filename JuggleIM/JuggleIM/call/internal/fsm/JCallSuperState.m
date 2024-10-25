//
//  JBaseState.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JCallSuperState.h"
#import "JCallEvent.h"

@implementation JCallSuperState

- (BOOL)stateDidEnter {
    return YES;
}

- (BOOL)stateDidLeave {
    return YES;
}

- (BOOL)event:(NSInteger)event
     userInfo:(id)userInfo {
    
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
            [self.callSessionImpl mediaQuit];
            [self.callSessionImpl transitionToIdleState];
            
        default:
            break;
    }
    
    return YES;
}

@end
