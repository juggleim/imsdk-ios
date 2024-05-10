//
//  JetIMConst.h
//  JetIM
//
//  Created by Nathan on 2023/11/27.
//

/// 拉取消息的方向
typedef NS_ENUM(NSUInteger, JPullDirection) {
    JPullDirectionNewer = 0,
    JPullDirectionOlder = 1,
};

typedef NS_ENUM(NSUInteger, JConnectionStatus) {
    //未连接
    JConnectionStatusIdle = 0,
    //已连接
    JConnectionStatusConnected = 1,
    //（用户主动断开或者被服务端踢掉，SDK 不会进行重连）
    JConnectionStatusDisconnected = 2,
    //连接中
    JConnectionStatusConnecting = 3,
    //连接失败，用户可根据 JErrorCode 来决定相应的处理
    JConnectionStatusFailure
};

typedef NS_ENUM(NSUInteger, JErrorCode) {
    JErrorCodeNone = 0,
    //未传 AppKey
    JErrorCodeAppKeyEmpty = 11001,
    //未传 Token
    JErrorCodeTokenEmpty = 11002,
    //AppKey 不存在
    JErrorCodeAppKeyInvalid = 11003,
    //Token 不合法
    JErrorCodeTokenIllegal = 11004,
    //Token 未授权
    JErrorCodeTokenUnauthorized = 11005,
    //Token 已过期
    JErrorCodeTokenExpired = 11006,
    //App 已封禁
    JErrorCodeAppProhibited = 11009,
    //用户被封禁
    JErrorCodeUserProhibited = 11010,
    //用户被踢下线
    JErrorCodeUserKickedByOtherClient = 11011,
    //用户注销下线
    JErrorCodeUserLogOut = 11012,
    
    //群组不存在
    JErrorCodeGroupNotExist = 13001,
    //不是群成员
    JErrorCodeNotGroupMember = 13002,
    
    //非法参数
    JErrorCodeInvalidParam = 21003,
    //操作超时
    JErrorCodeOperationTimeOut = 21004,
    //连接不可用
    JErrorCodeConnectionUnavailable = 21005,
    
    //消息不存在
    JErrorCodeMessageNotExist = 22001,
    //消息已被撤回
    JErrorCodeMessageAlreadyRecalled = 22002,
    //消息上传失败
    JErrorCodeMessageUploadError = 22003
};

/*!
 当前所处的网络
 */
typedef NS_ENUM(NSUInteger, JNetworkStatus) {
    /*!
     当前网络不可用
     */
    JNetworkStatusNotReachable = 0,

    /*!
     当前处于 WiFi 网络
     */
    JNetworkStatusReachableViaWiFi = 1,

    /*!
     移动网络
     */
    JNetworkStatusReachableViaWWAN = 2,
};
