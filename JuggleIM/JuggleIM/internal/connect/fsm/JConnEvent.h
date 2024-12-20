//
//  JConnEvent.h
//  JuggleIM
//
//  Created by Fei Li on 2024/11/12.
//

#ifndef JConnEvent_h
#define JConnEvent_h

typedef NS_ENUM(NSInteger, JConnEvent) {
    //user operation
    JConnEventUserConnect,
    JConnEventUserDisconnect,
    
    //callback
    JConnEventConnectDone,
    JConnEventConnectFailure,
    
    //
    JConnEventReconnectTimerFire,
    JConnEventNetworkAvailable,
    JConnEventEnterForground,
    JConnEventEnterBackground,
    JConnEventWebsocketFail,
    JConnEventRemoteDisconnect,
    JConnEventConnectingTimeOut
};

#endif /* JConnEvent_h */
