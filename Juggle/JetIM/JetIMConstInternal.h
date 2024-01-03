//
//  JetIMConstInternal.h
//  JetIM
//
//  Created by Nathan on 2023/12/1.
//

#define JWebSocketURL @"120.48.178.248:9002"
#define JNaviURL @"http://120.48.178.248:8083/navigator/general"
#define JPlatform @"iOS"
#define JDeviceCompany @"Apple"

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
    
    JErrorCodeInternalWebSocketFailure = 21001,
    JErrorCodeInternalNaviFailure = 21002
};
