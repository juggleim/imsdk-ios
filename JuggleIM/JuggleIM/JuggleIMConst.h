//
//  JuggleIMConst.h
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

/// 拉取消息的方向
typedef NS_ENUM(NSUInteger, JPullDirection) {
    JPullDirectionNewer = 0,
    JPullDirectionOlder = 1,
};

//聊天室成员排列顺序
typedef NS_ENUM(NSUInteger, JChatroomMemberOrder) {
    //返回最早加入的成员
    JChatroomMemberOrderAscending = 0,
    //返回最晚加入的成员
    JChatroomMemberOrderDescending = 1
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
    
    //聊天室默认错误
    JErrorCodeChatroomUnknownError = 14000,
    //非聊天室成员
    JErrorCodeNotChatroomMember = 14001,
    //聊天室属性已满（最多 100 个）
    JErrorCodeChatroomAttributeCountExceed = 14002,
    //无权限操作聊天室属性（非当前用户设置的 key）
    JErrorCodeChatroomKeyUnauthorized = 14003,
    //聊天室属性不存在
    JErrorCodeChatroomAttributeNotExist = 14004,
    //聊天室不存在
    JErrorCodeChatroomNotExist = 14005,
    //聊天室已销毁
    JErrorCodeChatroomDestroyed = 14006,
    
    //非法参数
    JErrorCodeInvalidParam = 21003,
    //操作超时
    JErrorCodeOperationTimeOut = 21004,
    //连接不可用
    JErrorCodeConnectionUnavailable = 21005,
    //服务器设置错误
    JErrorCodeServerSetError = 21006,
    //连接已存在
    JErrorCodeConnectionAlreadyExist = 21007,
    
    //消息不存在
    JErrorCodeMessageNotExist = 22001,
    //消息已被撤回
    JErrorCodeMessageAlreadyRecalled = 22002,
    //消息上传失败
    JErrorCodeMessageUploadError = 22003,
    //撤回消息扩展信息（extras）中的 key 和 value 不是 NSString
    JErrorCodeRecallExtrasTypeNotString = 22004,
    //下载的不是媒体消息
    JErrorCodeDownloadNotMediaMessage = 23001,
    //消息下载失败
    JErrorCodeMessageDownloadError = 23006,
    //消息下载已存在
    JErrorCodeDownloadAlreadyExist = 23008,
    //消息下载被取消
    JErrorCodeDownloadCanceled = 23009,
    
    //批量设置聊天室属性失败
    JErrorCodeChatroomBatchSetAttributeFail = 24001
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

typedef NS_ENUM(NSUInteger, JLogLevel) {
    JLogLevelNone = 0,
    JLogLevelFatal = 1,
    JLogLevelError = 2,
    JLogLevelWarning = 3,
    JLogLevelInfo = 4,
    JLogLevelDebug = 5,
    JLogLevelVerbose = 6
};

// 置顶会话排序规则
typedef NS_ENUM(NSUInteger, JTopConversationsOrderType) {
    // 按照置顶时间排序
    JTopConversationsOrderByTopTime,
    // 按照会话的消息时间排序
    JTopConversationsOrderByMessageTime
};
