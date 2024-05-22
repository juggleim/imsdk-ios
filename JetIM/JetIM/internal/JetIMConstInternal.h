//
//  JetIMConstInternal.h
//  JetIM
//
//  Created by Nathan on 2023/12/1.
//

#define JWebSocketURL @"120.48.178.248:9002"
#define JNaviURL @"http://120.48.178.248:8083"
#define JPlatform @"iOS"
#define JDeviceCompany @"Apple"

#define JThumbnailWidth 240
#define JThumbnailHeight 240
#define jThumbnailQuality 0.3

typedef NS_ENUM(NSUInteger, JErrorCodeInternal) {
    JErrorCodeInternalNone = 0,
    //未传 AppKey
    JErrorCodeInternalAppKeyEmpty = 11001,
    //未传 Token
    JErrorCodeInternalTokenEmpty = 11002,
    //AppKey 不存在
    JErrorCodeInternalAppKeyInvalid = 11003,
    //Token 不合法
    JErrorCodeInternalTokenIllegal = 11004,
    //Token 未授权
    JErrorCodeInternalTokenUnauthorized = 11005,
    //Token 已过期
    JErrorCodeInternalTokenExpired = 11006,
    //App 已封禁
    JErrorCodeInternalAppProhibited = 11009,
    //用户被封禁
    JErrorCodeInternalUserProhibited = 11010,
    //用户被踢下线
    JErrorCodeInternalUserKickedByOtherClient = 11011,
    //用户注销下线
    JErrorCodeInternalUserLogOut = 11012,
    
    //群组不存在
    JErrorCodeInternalGroupNotExist = 13001,
    //不是群成员
    JErrorCodeInternalNotGroupMember = 13002,
    
    JErrorCodeInternalWebSocketFailure = 21001,
    JErrorCodeInternalNaviFailure = 21002,
    JErrorCodeInternalInvalidParam = 21003,
    //操作超时
    JErrorCodeInternalOperationTimeOut = 21004,
    //连接不可用
    JErrorCodeInternalConnectionUnavailable = 21005,
    
    //消息不存在
    JErrorCodeInternalMessageNotExist = 22001,
    //消息已被撤回
    JErrorCodeInternalMessageAlreadyRecalled = 22002,
    //消息上传失败
    JErrorCodeInternalMessageUploadError = 22003,
    //日志不存在
    JErrorCodeInternalLogNotExist = 22004
};
