//
//  JMessageProtocol.h
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import <JuggleIM/JMessage.h>
#import <JuggleIM/JConversation.h>
#import <JuggleIM/JuggleIMConst.h>
#import <JuggleIM/JMessageUploadProvider.h>
#import <JuggleIM/JMediaMessageContent.h>

@class JMergeMessage;

@protocol JMessageDelegate <NSObject>
/// 接收消息的回调
- (void)messageDidReceive:(JMessage *)message;

@optional
/// 消息撤回的回调
- (void)messageDidRecall:(JMessage *)message;
/// 消息删除的回调
- (void)messageDidDelete:(JConversation *)conversation
            clientMsgNos:(NSArray <NSNumber *> *)clientMsgNos;
/// 消息清除回调，表示清除特定会话中某个时间点之前的所有消息
/// - Parameters:
///   - conversation: 被清除消息所属的会话标识
///   - timestamp: 时间戳（毫秒），timestamp 之前的消息被清除
///   - senderId: 若不为空，表示只清除发送者 id 为 senderId 的消息
- (void)messageDidClear:(JConversation *)conversation
              timestamp:(long long)timestamp
               senderId:(NSString *)senderId;

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

/// 发送消息
/// - Parameters:
///   - content: 消息实体
///   - messageOption: 消息扩展选项
///   - conversation: 会话
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (JMessage *)sendMessage:(JMessageContent *)content
            messageOption:(JMessageOptions *)messageOption
           inConversation:(JConversation *)conversation
                  success:(void (^)(JMessage *message))successBlock
                    error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock;

/// 发送媒体消息（先上传媒体，再发送消息）
/// - Parameters:
///   - content: 媒体消息实体
///   - conversation: 会话
///   - progressBlock: 上传进度回调
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
///   - cancelBlock: 用户取消上传回调
- (JMessage *)sendMediaMessage:(JMediaMessageContent *)content
                inConversation:(JConversation *)conversation
                      progress:(void (^)(int progress, JMessage *message))progressBlock
                       success:(void (^)(JMessage *message))successBlock
                         error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock
                        cancel:(void (^)(JMessage *message))cancelBlock;

/// 发送媒体消息（先上传媒体，再发送消息）
/// - Parameters:
///   - content: 媒体消息实体
///   - messageOption: 消息扩展选项
///   - conversation: 会话
///   - progressBlock: 上传进度回调
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
///   - cancelBlock: 用户取消上传回调
- (JMessage *)sendMediaMessage:(JMediaMessageContent *)content
                 messageOption:(JMessageOptions *)messageOption
                inConversation:(JConversation *)conversation
                      progress:(void (^)(int progress, JMessage *message))progressBlock
                       success:(void (^)(JMessage *message))successBlock
                         error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock
                        cancel:(void (^)(JMessage *message))cancelBlock;

/// 重发消息，用于发送失败后进行重发
/// - Parameters:
///   - messsage: 消息对象
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (JMessage *)resend:(JMessage *)messsage
             success:(void (^)(JMessage *message))successBlock
               error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock;


/// 重发消息，用于媒体类型消息重发
/// - Parameters:
///   - message: 消息对象
///   - progressBlock: 上传进度回调
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
///   - cancelBlock: 取消回调
- (JMessage *)resendMediaMessage:(JMessage *)message
                        progress:(void (^)(int progress, JMessage *message))progressBlock
                         success:(void (^)(JMessage *message))successBlock
                           error:(void (^)(JErrorCode errorCode, JMessage *message))errorBlock
                          cancel:(void (^)(JMessage *message))cancelBlock;
/// 保存消息
/// - Parameters:
///   - content: 消息实体
///   - messageOption: 消息扩展选项
///   - conversation: 会话
///   - direction: 消息方向
- (JMessage *)saveMessage:(JMessageContent *)content
           inConversation:(JConversation *)conversation
                direction:(JMessageDirection)direction;

/// 保存消息
/// - Parameters:
///   - content: 消息实体
///   - messageOption: 消息扩展选项
///   - conversation: 会话
///   - direction: 消息方向
- (JMessage *)saveMessage:(JMessageContent *)content
            messageOption:(JMessageOptions *)messageOption
           inConversation:(JConversation *)conversation
                direction:(JMessageDirection)direction;

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

/// 在同一个会话里，根据本端消息唯一编号批量删除消息
/// - Parameters:
///   - clientMsgNos: 本端消息唯一编号列表
///   - conversation: 会话标识
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)deleteMessagesByClientMsgNoList:(NSArray<NSNumber *> *)clientMsgNos
                           conversation:(JConversation *)conversation
                                success:(void (^)(void))successBlock
                                  error:(void (^)(JErrorCode errorCode))errorBlock;

/// 在同一个会话里，根据消息 id 批量删除消息
/// - Parameters:
///   - messageIds: 消息 id 列表
///   - conversation: 会话标识
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)deleteMessagesByMessageIds:(NSArray<NSString *> *)messageIds
                      conversation:(JConversation *)conversation
                           success:(void (^)(void))successBlock
                             error:(void (^)(JErrorCode errorCode))errorBlock;

/// 撤回消息（撤回后会话中的所有人都看不到原消息）
/// - Parameters:
///   - messageId: 被撤回的消息 id
///   - extras: 扩展信息，key 和 value 都只能是 NSString 类型
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)recallMessage:(NSString *)messageId
               extras:(NSDictionary <NSString *, NSString *> *)extras
              success:(void (^)(JMessage *message))successBlock
                error:(void (^)(JErrorCode errorCode))errorBlock;

/// 清空会话中指定时间之前的所有消息，startTime 传 0 表示当前时间
/// - Parameters:
///   - conversation: 会话标识
///   - startTime: 开始时间，传 0 表示当前时间
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)clearMessagesIn:(JConversation *)conversation
              startTime:(long long)startTime
                success:(void (^)(void))successBlock
                  error:(void (^)(JErrorCode errorCode))errorBlock;

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

/// 注册自定义消息，没有注册的自定义消息 SDK 无法正常进行解析
/// - Parameter messageClass: 自定义消息的类，需要继承 JMessageContent
- (void)registerContentType:(Class)messageClass;

- (void)addDelegate:(id<JMessageDelegate>)delegate;

- (void)addSyncDelegate:(id<JMessageSyncDelegate>)delegate;

- (void)addReadReceiptDelegate:(id<JMessageReadReceiptDelegate>)delegate;

- (void)setMessageUploadProvider:(id<JMessageUploadProvider>)uploadProvider;

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

/// 先从本地获取消息，如果中间存在缺失，则从远端补齐。如果本地消息已经是完备的，将不会再走 remoteMessageBlock 回调。
/// - Parameters:
///   - conversation: 会话对象
///   - startTime: 消息时间戳，如果传 0 为当前时间
///   - count: 拉取数量，超过 100 条按 100 返回
///   - direction: 拉取方向
///   - localMessageBlock: 本地消息回调
///   - remoteMessageBlock: 远端消息回调（可能包含已返回的本地消息）
///   - errorBlock: 失败回调
- (void)getLocalAndRemoteMessagesFrom:(JConversation *)conversation
                            startTime:(long long)startTime
                                count:(int)count
                            direction:(JPullDirection)direction
                    localMessageBlock:(void (^)(NSArray <JMessage *> *messages,BOOL needRemote))localMessageBlock
                   remoteMessageBlock:(void (^)(NSArray <JMessage *> *messages))remoteMessageBlock
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

/// 消息本地检索
/// - Parameters:
///   - searchContent: 查询内容
///   - conversation: 要查询的会话，传空从所有会话中查询
///   - count:拉取数量，超过 100 条按 100 返回
///   - time: 消息时间戳，如果传 0 为当前时间
///   - direction: 查询方向
///   - contentTypes: 内容类型，传空返回所有类型
- (NSArray <JMessage *> *)searchMessagesWithContent:(NSString *)searchContent
                                     inConversation:(JConversation *)conversation
                                             count:(int)count
                                              time:(long long)time
                                         direction:(JPullDirection)direction
                                      contentTypes:(NSArray<NSString *> *)contentTypes;

/// 获取指定会话中未读的 @ 消息
/// - Parameters:
///   - conversation: 会话标识
///   - count: 获取数量，超过 100 条按 100 返回
///   - time: 消息时间戳，传 0 为当前时间
///   - direction: 查询方向
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)getMentionMessages:(JConversation *)conversation
                     count:(int)count
                      time:(long long)time
                 direction:(JPullDirection)direction
                   success:(void (^)(NSArray<JMessage *> *messages, BOOL isFinished))successBlock
                     error:(void (^)(JErrorCode code))errorBlock;

/// 获取消息本地属性
/// - Parameter messageId: 消息 id
- (NSString *)getLocalAttributeByMessageId:(NSString *)messageId;


/// 设置消息本地属性（只在本地生效，不会同步到远端）
/// - Parameters:
///   - attribute: 本地属性（可以使用 JSON 以满足复杂的业务场景）
///   - messageId: 消息 id
- (void)setLocalAttribute:(NSString *)attribute forMessage:(NSString *)messageId;


/// 获取消息本地属性
/// - Parameter clientMsgNo: 本端消息唯一编号
- (NSString *)getLocalAttributeByClientMsgNo:(long long)clientMsgNo;

/// 设置消息本地属性（只在本地生效，不会同步到远端）
/// - Parameters:
///   - attribute: 本地属性（可以使用 JSON 以满足复杂的业务场景）
///   - clientMsgNo: 本端消息唯一编号
- (void)setLocalAttribute:(NSString *)attribute forClientMsgNo:(long long)clientMsgNo;

/// 消息广播。同时向批量会话中发送消息，该消息在发送方不影响会话的排序
/// - Parameters:
///   - content: 消息实体
///   - conversations: 目标会话列表
///   - progressBlock: 进度回调
///   - completeBlock: 完成回调
- (void)broadcastMessage:(JMessageContent *)content
         inConversations:(NSArray <JConversation *> *)conversations
                progress:(void (^)(JMessage *sentMessage, JErrorCode code, int processCount, int totalCount))progressBlock
                complete:(void (^)(void))completeBlock;

/// 下载消息中的媒体文件
/// - Parameters:
///   - messageId: 消息 id
///   - progressBlock: 进度更新的回调 [progress: 当前的下载进度，0 <= progress <= 100]
///   - successBlock: 成功的回调 [message: 下载完成的本地路径已写入 message]
///   - errorBlock: 失败的回调 [errorCode: 错误码]
- (void)downloadMediaMessage:(NSString *)messageId
                    progress:(void (^)(JMessage *message, int progress))progressBlock
                     success:(void (^)(JMessage *message))successBlock
                       error:(void (^)(JErrorCode errorCode))errorBlock;


/// 取消下载消息
/// - Parameter messageId: 消息 id
- (void)cancelDownloadMediaMessage:(NSString *)messageId;

// TODO: 上传做完后删除
- (void)setMessageState:(JMessageState)state
        withClientMsgNo:(long long)clientMsgNo;

@end
