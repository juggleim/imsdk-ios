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
    //未连接
    JConnectionStatusIdle = 0,
    //已连接
    JConnectionStatusConnected = 1,
    //连接断开（用户主动断开）
    JConnectionStatusDisconnected = 2,
    //连接中
    JConnectionStatusConnecting = 3,
    //连接 token 错误
    JConnectionStatusTokenIncorrect = 4
};

typedef NS_ENUM(NSUInteger, JErrorCode) {
    JErrorCodeNone = 0,
    
    JErrorCodeWebSocketFailure = 21001,
};
