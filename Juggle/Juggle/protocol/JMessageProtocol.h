//
//  JMessageProtocol.h
//  Juggle
//
//  Created by Nathan on 2023/11/27.
//

#import "JMessage.h"
#import "JConversation.h"
#import "JuggleConst.h"

@protocol JMessageDelegate <NSObject>

- (void)messageDidReceive:(JMessage *)message;
- (void)messageDidUpdate:(JMessage *)message;

@end

@protocol JMessageProtocol <NSObject>

/// 发送消息
/// - Parameters:
///   - content: 消息实体
///   - conversation: 会话
- (void)sendMessage:(JMessageContent *)content
     inConversation:(JConversation *)conversation;

//从本地获取消息，如果本地消息存在缺失，会尝试从远端把缺失的消息拉取回来。
//TODO: 走的回调待补充，回调中需要标识是否还存在没拉完的消息
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

/// 根据 clientMsgNo 删除本地消息（只删除本端消息，服务端消息不受影响）
/// - Parameter clientMsgNo: 本端消息唯一编号
- (void)deleteMessageByClientId:(long)clientMsgNo;

/// 根据 messageId 删除本地消息（只删除本端消息，服务端消息不受影响）
/// - Parameter messageId: 消息 id
- (void)deleteMessageByMessageId:(NSString *)messageId;

- (void)setDelegate:(id<JMessageDelegate>)delegate;
@end
