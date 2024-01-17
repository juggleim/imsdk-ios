//
//  JConnectionProtocol.h
//  JetIM
//
//  Created by Nathan on 2023/11/27.
//
#import <JetIM/JetIMConst.h>

@protocol JConnectionDelegate <NSObject>
/// 连接状态变化的回调
/// - Parameters:
///   - status: 变化后的状态
///   - code: 连接错误码，在 JConnectionStatusFailure 状态时有效，其它状态均为 0。
- (void)connectionStatusDidChange:(JConnectionStatus)status
                        errorCode:(JErrorCode)code;
/// 数据库打开的回调，设备离线的情况下也可以打开本地数据库查看历史消息
- (void)dbDidOpen;
- (void)dbDidClose;
@end

@protocol JConnectionProtocol <NSObject>
- (void)connectWithToken:(NSString *)token;

- (void)disconnect:(BOOL)receivePush;

- (void)setDelegate:(id<JConnectionDelegate>) delegate;

@end