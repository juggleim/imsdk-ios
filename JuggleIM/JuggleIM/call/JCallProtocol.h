//
//  JCallProtocol.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/21.
//

#import <JuggleIM/JCallSession.h>

@protocol JCallReceiveDelegate <NSObject>

/// 接听到通话
/// - Parameter callSession: 通话实例
- (void)callDidReceive:(id<JCallSession>)callSession;

@end

@protocol JCallProtocol <NSObject>

/// 提供 Zego 初始化必需的参数
/// - Parameters:
///   - appId: Zego AppId
///   - appSign: Zego AppSign
- (void)initZegoEngineWith:(int)appId
                   appSign:(NSString *)appSign;

/// 初始化 LiveKit
- (void)initLiveKitEngine;

/// 发起 1v1 语音通话
/// - Parameters:
///   - userId: 对方的 userId
///   - delegate: 设置代理
- (id<JCallSession>)startSingleCall:(NSString *)userId
                           delegate:(id<JCallSessionDelegate>)delegate;

/// 发起 1v1 通话
/// - Parameters:
///   - userId: 对方的 userId
///   - mediaType: 语音/视频
///   - delegate: 设置代理
- (id<JCallSession>)startSingleCall:(NSString *)userId
                          mediaType:(JCallMediaType)mediaType
                           delegate:(id<JCallSessionDelegate>)delegate;

/// 发起多人通话
/// - Parameters:
///   - userIdList: userId 列表
///   - mediaType: 语音/视频
///   - delegate: 设置代理
- (id<JCallSession>)startMultiCall:(NSArray <NSString *> *)userIdList
                         mediaType:(JCallMediaType)mediaType
                          delegate:(id<JCallSessionDelegate>)delegate;

/// 根据 callId 获取 CallSession
/// - Parameter callId: 通话 id
- (id<JCallSession>)getCallSession:(NSString *)callId;

- (void)addReceiveDelegate:(id<JCallReceiveDelegate>)receiveDelegate;

@end
