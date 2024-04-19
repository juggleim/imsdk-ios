//
//  JMessageProtocol.h
//  JetIM
//
//  Created by Nathan on 2023/11/27.
//

#import <JetIM/JMessage.h>
#import <JetIM/JConversation.h>
#import <JetIM/JetIMConst.h>

@class JMergeMessage;

@protocol JMessageDelegate <NSObject>

- (void)messageDidReceive:(JMessage *)message;

- (void)messageDidRecall:(JMessage *)message;

@end

@protocol JMessageSyncDelegate <NSObject>

/// 消息同步完成的回调
- (void)messageSyncDidComplete;

@end

@protocol JMessageReadReceiptDelegate <NSObject>

/// 单聊消息阅读回调
/// - Parameters:
///   - messageIds: 消息 id 数组
///   - conversation: 所在会话
- (void)messagesDidRead:(NSArray <NSString *> *)messageIds
         inConversation:(JConversation *)conversation;

/// 群消息阅读回调
/// - Parameters:
///   - msgs: key 为 messageId
///   - conversation: 所在会话
- (void)groupMessagesDidRead:(NSDictionary <NSString *, JGroupMessageReadInfo *> *)msgs
              inConversation:(JConversation *)conversation;

@end

@protocol JMessageProtocol <NSObject>

/// 发送消息
/// - Parameters:
///   - content: 消息实体
///   - conversation: 会话
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (JMessage *)sendMessage:(JMessageContent *)content
           inConversation:(JConversation *)conversation
                  success:(void (^)(JMessage *message))successBlock
                    error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock;

/// 重发消息，用于发送失败后进行重发
/// - Parameters:
///   - messsage: 消息对象
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (JMessage *)resend:(JMessage *)messsage
             success:(void (^)(JMessage *message))successBlock
               error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock;


/// 保存消息
/// - Parameters:
///   - content: 消息实体
///   - conversation: 会话
- (JMessage *)saveMessage:(JMessageContent *)content
           inConversation:(JConversation *)conversation;

/// 从本地获取消息，结果按照消息时间正序排列（旧的在前，新的在后）
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

/// 从本地获取消息，结果按照消息时间正序排列（旧的在前，新的在后）
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
                            contentTypes:(NSArray <NSString *> *)contentTypes;

/// 根据 clientMsgNo 删除本地消息（只删除本端消息，服务端消息不受影响）
/// - Parameter clientMsgNo: 本端消息唯一编号
- (void)deleteMessageByClientMsgNo:(long long)clientMsgNo;

/// 根据 messageId 删除本地消息（只删除本端消息，服务端消息不受影响）
/// - Parameter messageId: 消息 id
- (void)deleteMessageByMessageId:(NSString *)messageId;

/// 撤回消息（撤回后会话中的所有人都看不到原消息）
- (void)recallMessage:(NSString *)messageId
              success:(void (^)(JMessage *message))successBlock
                error:(void (^)(JErrorCode errorCode))errorBlock;

/// 清除会话内所有消息
/// - Parameter conversation: 会话标识
- (void)clearMessagesIn:(JConversation *)conversation;

/// 根据 messageId 数组获取对应的本地消息
/// - Parameter messageIds: messageId 数组
- (NSArray<JMessage *> *)getMessagesByMessageIds:(NSArray<NSString *> *)messageIds;


/// 根据 messageId 数组获取对应的消息，如果本地有则优先取本地消息，否则去服务端获取
/// - Parameters:
///   - messageIds: messageId 数组
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)getMessagesByMessageIds:(NSArray<NSString *> *)messageIds
                 inConversation:(JConversation *)conversation
                        success:(void (^)(NSArray <JMessage *> *messages))successBlock
                          error:(void (^)(JErrorCode errorCode))errorBlock;

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

- (void)setSyncDelegate:(id<JMessageSyncDelegate>)delegate;

- (void)setReadReceiptDelegate:(id<JMessageReadReceiptDelegate>)delegate;

/// 从远端拉取历史消息
/// - Parameters:
///   - conversation: 会话对象
///   - startTime: 消息时间戳，如果传 0 为当前时间
///   - count: 拉取数量，超过 100 条按 100 返回
///   - direction: 拉取方向
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)getRemoteMessagesFrom:(JConversation *)conversation
                    startTime:(long long)startTime
                        count:(int)count
                    direction:(JPullDirection)direction
                      success:(void (^)(NSArray *messages))successBlock
                        error:(void (^)(JErrorCode code))errorBlock;


/// 先从本地获取消息，如果中间存在缺失，则从远端补齐
/// - Parameters:
///   - conversation: 会话对象
///   - startTime: 消息时间戳，如果传 0 为当前时间
///   - count: 拉取数量，超过 100 条按 100 返回
///   - direction: 拉取方向
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)getLocalAndRemoteMessagesFrom:(JConversation *)conversation
                            startTime:(long long)startTime
                                count:(int)count
                            direction:(JPullDirection)direction
                              success:(void (^)(NSArray *messages))successBlock
                                error:(void (^)(JErrorCode code))errorBlock;

/// 发送阅读回执
/// - Parameters:
///   - messageIds: 需要发送阅读回执的消息 id 列表
///   - conversation: 消息所在会话
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)sendReadReceipt:(NSArray <NSString *> *)messageIds
         inConversation:(JConversation *)conversation
                success:(void (^)(void))successBlock
                  error:(void (^)(JErrorCode code))errorBlock;

/// 获取群消息阅读状态
/// - Parameters:
///   - messageId: 需要查询的群消息 id
///   - conversation: 消息所在会话
///   - successBlock: 成功回调，readMemberIds 存放已读用户 id 列表，unreadMemberIds 存放未读用户 id 列表
///   - errorBlock: 失败回调
- (void)getGroupMessageReadDetail:(NSString *)messageId
                   inConversation:(JConversation *)conversation
                          success:(void (^)(NSArray<JUserInfo *> *readMembers, NSArray<JUserInfo *> *unreadMembers))successBlock
                            error:(void (^)(JErrorCode code))errorBlock;

/// 获取被合并的消息列表
/// - Parameters:
///   - messageId: 合并消息 id
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)getMergedMessageList:(NSString *)messageId
                     success:(void (^)(NSArray<JMessage *> *mergedMessages))successBlock
                       error:(void (^)(JErrorCode code))errorBlock;

// TODO: 上传做完后删除
- (void)setMessageState:(JMessageState)state
        withClientMsgNo:(long long)clientMsgNo;



/// 消息本地检索
/// - Parameters:
///   - searchContent: 查询内容
///   - count:拉取数量，超过 100 条按 100 返回
///   - time: 消息时间戳，如果传 0 为当前时间
///   - direction: 查询方向
///   - contentTypes: 内容类型 ，传空 返回所有类型
- (NSArray <JMessage *> *)searchMessagesWithContent:(NSString *)searchContent
                                             count:(int)count
                                              time:(long long)time
                                         direction:(JPullDirection)direction
                                      contentTypes:(NSArray<NSString *> *)contentTypes;
@end
