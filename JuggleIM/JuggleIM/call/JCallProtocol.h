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

/// Received an incoming call.
/// - Parameter callSession: Call session.
- (void)callDidReceive:(id<JCallSession>)callSession;

@end

@protocol JConversationCallDelegate <NSObject>

/// Call information in the conversation changed.
/// - Parameters:
///   - callInfo: Updated call information.
///   - conversation: Associated conversation.
///   - isFinished: Whether the call has finished.
- (void)callInfoDidUpdate:(JCallInfo *)callInfo
           inConversation:(JConversation *)conversation
               isFinished:(BOOL)isFinished;

@end

@protocol JCallProtocol <NSObject>

/// Provide the parameters required to initialize Zego.
/// - Parameters:
///   - appId: Zego AppId
///   - appSign: Zego AppSign
- (void)initZegoEngineWith:(int)appId
                   appSign:(NSString *)appSign;

/// Initialize LiveKit.
- (void)initLiveKitEngine;

/// Provide the parameters required to initialize Agora.
/// - Parameter appId: Agora APP ID
- (void)initAgoraEngineWith:(NSString *)appId;

/// Start a one-to-one audio call.
/// - Parameters:
///   - userId: Remote user's userId.
///   - delegate: Delegate to set.
- (id<JCallSession>)startSingleCall:(NSString *)userId
                           delegate:(id<JCallSessionDelegate>)delegate;

/// Start a one-to-one call.
/// - Parameters:
///   - userId: Remote user's userId.
///   - mediaType: Audio/video.
///   - delegate: Delegate to set.
- (id<JCallSession>)startSingleCall:(NSString *)userId
                          mediaType:(JCallMediaType)mediaType
                           delegate:(id<JCallSessionDelegate>)delegate;

/// Start a one-to-one call.
/// - Parameters:
///   - userId: Remote user's userId.
///   - mediaType: Audio/video.
///   - extra: Extra field.
///   - delegate: Delegate to set.
- (id<JCallSession>)startSingleCall:(NSString *)userId
                          mediaType:(JCallMediaType)mediaType
                              extra:(NSString *)extra
                           delegate:(id<JCallSessionDelegate>)delegate;

/// Start a multi-party call.
/// - Parameters:
///   - userIdList: List of userIds.
///   - mediaType: Audio/video.
///   - delegate: Delegate to set.
- (id<JCallSession>)startMultiCall:(NSArray <NSString *> *)userIdList
                         mediaType:(JCallMediaType)mediaType
                          delegate:(id<JCallSessionDelegate>)delegate;

/// Start a multi-party call.
/// - Parameters:
///   - userIdList: List of userIds.
///   - mediaType: Audio/video.
///   - extra: Extra field.
///   - delegate: Delegate to set.
- (id<JCallSession>)startMultiCall:(NSArray <NSString *> *)userIdList
                         mediaType:(JCallMediaType)mediaType
                             extra:(NSString *)extra
                          delegate:(id<JCallSessionDelegate>)delegate;

/// Start a multi-party call.
/// - Parameters:
///   - userIdList: List of userIds.
///   - mediaType: Audio/video.
///   - conversation: Associated conversation.
///   - extra: Extra field.
///   - delegate: Delegate to set.
- (id<JCallSession>)startMultiCall:(NSArray <NSString *> *)userIdList
                         mediaType:(JCallMediaType)mediaType
                      conversation:(JConversation *)conversation
                             extra:(NSString *)extra
                          delegate:(id<JCallSessionDelegate>)delegate;

/// Proactively join a call.
/// - Parameters:
///   - callId: Call ID.
///   - delegate: Delegate to set.
- (id<JCallSession>)joinCall:(NSString *)callId
                    delegate:(id<JCallSessionDelegate>)delegate;

/// Get the CallSession by callId.
/// - Parameter callId: Call ID.
- (id<JCallSession>)getCallSession:(NSString *)callId;

/// Get call information in a conversation.
/// - Parameters:
///   - conversation: Conversation identifier.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
- (void)getConversationCallInfo:(JConversation *)conversation
                        success:(void (^)(JCallInfo *callInfo))successBlock
                          error:(void (^)(JErrorCode errorCode))errorBlock;

- (void)addReceiveDelegate:(id<JCallReceiveDelegate>)receiveDelegate;

- (void)addConversationCallDelegate:(id<JConversationCallDelegate>)conversationCallDelegate;

@end
