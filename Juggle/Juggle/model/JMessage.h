//
//  JMessage.h
//  Juggle
//
//  Created by Nathan on 2023/11/27.
//

#import <Foundation/Foundation.h>

#import <Juggle/JConversation.h>
#import <Juggle/JMessageContent.h>

NS_ASSUME_NONNULL_BEGIN

/// 消息方向，发送/接收
typedef NS_ENUM(NSUInteger, JMessageDirection) {
    JMessageDirectionSend = 1,
    JMessageDirectionReceive = 2
};

/// 消息状态
typedef NS_ENUM(NSUInteger, JMessageState) {
    JMessageStateUnknown = 0,
    JMessageStateSending,
    JMessageStateSent,
    JMessageStateFail
};

@interface JMessage : NSObject
@property (nonatomic, strong) JConversation *conversation;
/// 消息类型
@property (nonatomic, copy) NSString *messageType;
/// 本端消息唯一编号（只对当前设备生效）
@property (nonatomic, assign) long long clientMsgNo;
/// 消息 id，全局唯一
@property (nonatomic, copy) NSString *messageId;
/// 消息方向，发送/接收
@property (nonatomic, assign) JMessageDirection direction;
/// 消息状态
@property (nonatomic, assign) JMessageState messageState;
/// 是否已读
@property (nonatomic, assign) BOOL hasRead;
/// 消息发送的时间戳（服务端时间，全局唯一）
@property (nonatomic, assign) long long timestamp;
/// 发送者 userId
@property (nonatomic, copy) NSString *senderUserId;
/// 消息内容
@property (nonatomic, strong) JMessageContent *content;
@end

NS_ASSUME_NONNULL_END
