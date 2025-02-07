//
//  JConnectionProtocol.h
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//
#import <JuggleIM/JuggleIMConst.h>

@protocol JConnectionDelegate <NSObject>
@optional
/// 连接状态变化的回调
/// - Parameters:
///   - status: 变化后的状态
///   - code: 连接错误码，在 JConnectionStatusFailure 状态时有效，其它状态均为 0。
///   - extra: 附加信息
- (void)connectionStatusDidChange:(JConnectionStatus)status
                        errorCode:(JErrorCode)code
                            extra:(NSString *)extra;
/// 数据库打开的回调，设备离线的情况下也可以打开本地数据库查看历史消息
- (void)dbDidOpen;
- (void)dbDidClose;
@end

@protocol JConnectionProtocol <NSObject>
- (void)connectWithToken:(NSString *)token;

- (void)disconnect:(BOOL)receivePush;

- (void)registerDeviceToken:(NSData *)tokenData;

- (void)registerVoIPToken:(NSData *)tokenData;

/// 设置语言（主要影响内置消息的推送语言，目前支持中/英文）
/// - Parameters:
///   - language: 语言，例 “en-US”
///   - completeBlock: 结果回调，返回 JErrorCodeNone 表示成功
- (void)setLanguage:(NSString *)language
           complete:(void (^)(JErrorCode code))completeBlock;

/// 获取语言（主要影响内置消息的推送语言，目前支持中/英文）
/// - Parameter completeBlock: 结果回调，返回 JErrorCodeNone 表示成功
- (void)getLanguage:(void (^)(JErrorCode code, NSString *language))completeBlock;

- (JConnectionStatus)getConnectionStatus;

- (void)addDelegate:(id<JConnectionDelegate>) delegate;

@end
