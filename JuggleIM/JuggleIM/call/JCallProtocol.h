//
//  JCallProtocol.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/21.
//

#import <JuggleIM/JCallSession.h>
#import <JuggleIM/JConversation.h>
#import <JuggleIM/JCallInfo.h>

@protocol JCallReceiveDelegate <NSObject>

/// 接听到通话
/// - Parameter callSession: 通话实例
- (void)callDidReceive:(id<JCallSession>)callSession;

@end

@protocol JConversationCallDelegate <NSObject>

/// 会话中的通话信息变更
/// - Parameters:
///   - callInfo: 变更后的通话信息
///   - conversation: 所属会话
///   - isFinished: 是否已结束
- (void)callInfoDidUpdate:(JCallInfo *)callInfo
           inConversation:(JConversation *)conversation
               isFinished:(BOOL)isFinished;

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

/// 提供 Agora 初始化必需的参数
/// - Parameter appId: Agora APP ID
- (void)initAgoraEngineWith:(NSString *)appId;

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

/// 发起 1v1 通话
/// - Parameters:
///   - userId: 对方的 userId
///   - mediaType: 语音/视频
///   - extra: 扩展字段
///   - delegate: 设置代理
- (id<JCallSession>)startSingleCall:(NSString *)userId
                          mediaType:(JCallMediaType)mediaType
                              extra:(NSString *)extra
                           delegate:(id<JCallSessionDelegate>)delegate;

/// 发起多人通话
/// - Parameters:
///   - userIdList: userId 列表
///   - mediaType: 语音/视频
///   - delegate: 设置代理
- (id<JCallSession>)startMultiCall:(NSArray <NSString *> *)userIdList
                         mediaType:(JCallMediaType)mediaType
                          delegate:(id<JCallSessionDelegate>)delegate;

/// 发起多人通话
/// - Parameters:
///   - userIdList: userId 列表
///   - mediaType: 语音/视频
///   - extra: 扩展字段
///   - delegate: 设置代理
- (id<JCallSession>)startMultiCall:(NSArray <NSString *> *)userIdList
                         mediaType:(JCallMediaType)mediaType
                             extra:(NSString *)extra
                          delegate:(id<JCallSessionDelegate>)delegate;

/// 发起多人通话
/// - Parameters:
///   - userIdList: userId 列表
///   - mediaType: 语音/视频
///   - conversation: 所属会话
///   - extra: 扩展字段
///   - delegate: 设置代理
- (id<JCallSession>)startMultiCall:(NSArray <NSString *> *)userIdList
                         mediaType:(JCallMediaType)mediaType
                      conversation:(JConversation *)conversation
                             extra:(NSString *)extra
                          delegate:(id<JCallSessionDelegate>)delegate;

/// 主动加入通话
/// - Parameters:
///   - callId: 通话 id
///   - delegate: 设置代理
- (id<JCallSession>)joinCall:(NSString *)callId
                    delegate:(id<JCallSessionDelegate>)delegate;

/// 根据 callId 获取 CallSession
/// - Parameter callId: 通话 id
- (id<JCallSession>)getCallSession:(NSString *)callId;

/// 获取会话中的通话信息
/// - Parameters:
///   - conversation: 会话标识
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)getConversationCallInfo:(JConversation *)conversation
                        success:(void (^)(JCallInfo *callInfo))successBlock
                          error:(void (^)(JErrorCode errorCode))errorBlock;

- (void)addReceiveDelegate:(id<JCallReceiveDelegate>)receiveDelegate;

- (void)addConversationCallDelegate:(id<JConversationCallDelegate>)conversationCallDelegate;

@end
