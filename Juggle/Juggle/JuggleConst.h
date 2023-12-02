//
//  JuggleConst.h
//  Juggle
//
//  Created by Nathan on 2023/11/27.
//

/// 拉取消息的方向
typedef NS_ENUM(NSUInteger, JPullDirection) {
    JPullDirectionOldToNew = 0,
    JPullDirectionNewToOld = 1,
};

typedef NS_ENUM(NSUInteger, JConnectionStatus) {
    JConnectionStatusUnknown = 0,
    JConnectionStatusConnected = 1,
    JConnectionStatusConnecting = 2,
    JConnectionStatusTokenIncorrect = 3,
    JConnectionStatusDisconnected = 4
};

typedef NS_ENUM(NSUInteger, JErrorCode) {
    JErrorCodeNone = 0,
    
    JErrorCodeWebSocketFailure = 21001,
};
