//
//  JMessageProtocol.h
//  JetIM
//
//  Created by Nathan on 2023/11/27.
//

#import <JetIM/JMessage.h>
#import <JetIM/JConversation.h>
#import <JetIM/JetIMConst.h>

@protocol JMessageDelegate <NSObject>

- (void)messageDidReceive:(JMessage *)message;

@end

@protocol JMessageProtocol <NSObject>

/// 发送消息
/// - Parameters:
///   - content: 消息实体
///   - conversation: 会话
- (JMessage *)sendMessage:(JMessageContent *)content
           inConversation:(JConversation *)conversation
                  success:(void (^)(JMessage *message))successBlock
                    error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock;

/// 从本地获取消息
/// 获取消息列表
/// - Parameters:
///   - conversation: 会话对象
///   - count: 拉取消息条数
///   - time: 消息时间戳，如果传 0 表示当前时间
///   - direction: 拉取方向
- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation 
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction;

/// 从本地获取消息
/// - Parameters:
///   - conversation: 会话对象
///   - count: 拉取消息条数
///   - time: 消息时间戳，如果传 0 表示当前时间
///   - direction: 拉取方向
///   - contentTypes: 拉取的消息类型列表，消息类型获取举例 " [JTextMessage contentType] "
- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation
                                   count:(int)count
                                    time:(long long)time
                               direction:(JPullDirection)direction
                            contentTypes:(NSArray <NSNumber *> *)contentTypes;

/// 根据 clientMsgNo 删除本地消息（只删除本端消息，服务端消息不受影响）
/// - Parameter clientMsgNo: 本端消息唯一编号
- (void)deleteMessageByClientMsgNo:(long long)clientMsgNo;

/// 根据 messageId 删除本地消息（只删除本端消息，服务端消息不受影响）
/// - Parameter messageId: 消息 id
- (void)deleteMessageByMessageId:(NSString *)messageId;


/// 根据 messageId 数组获取对应的本地消息
/// - Parameter messageIds: messageId 数组
- (NSArray<JMessage *> *)getMessagesByMessageIds:(NSArray<NSString *> *)messageIds;


/// 根据 clientMsgNo 数组获取对应的本地消息
/// - Parameter clientMsgNos: clientMsgNo 数组
- (NSArray<JMessage *> *)getMessagesByClientMsgNos:(NSArray<NSNumber *> *)clientMsgNos;

/*!
 注册自定义的消息类型

 @param messageClass    自定义消息的类，该自定义消息需要继承于 RCMessageContent

 @discussion
 如果您需要自定义消息，必须调用此方法注册该自定义消息的消息类型，否则 SDK 将无法识别和解析该类型消息。
 @discussion 请在初始化 appkey 之后，token 连接之前调用该方法注册自定义消息

 @warning 如果您使用 IMLib，请使用此方法注册自定义的消息类型；
 如果您使用 IMKit，请使用 RCIM 中的同名方法注册自定义的消息类型，而不要使用此方法。
 
 @remarks 消息操作
 */

/// 注册自定义消息，没有注册的自定义消息 SDK 无法正常进行解析
/// - Parameter messageClass: 自定义消息的类，需要继承 JMessageContent
- (void)registerContentType:(Class)messageClass;

- (void)setDelegate:(id<JMessageDelegate>)delegate;




- (void)getRemoteMessagesFrom:(JConversation *)conversation
                    startTime:(long long)startTime
                        count:(int)count
                    direction:(JPullDirection)direction
                      success:(void (^)(NSArray *messages, BOOL isFinished))successBlock
                        error:(void (^)(JErrorCode code))errorBlock;
@end
