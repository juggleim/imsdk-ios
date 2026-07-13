//
//  JMessage.h
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import <Foundation/Foundation.h>

#import <JuggleIM/JConversation.h>
#import <JuggleIM/JMessageContent.h>
#import <JuggleIM/JGroupMessageReadInfo.h>
#import <JuggleIM/JUserInfo.h>
#import <JuggleIM/JMessageOptions.h>

/// Message direction, send/receive
typedef NS_ENUM(NSUInteger, JMessageDirection) {
    JMessageDirectionSend = 1,
    JMessageDirectionReceive = 2
};

/// Message status
typedef NS_ENUM(NSUInteger, JMessageState) {
    JMessageStateUnknown = 0,
    JMessageStateSending,
    JMessageStateSent,
    JMessageStateFail,
    JMessageStateUploading
};

@interface JMessage : NSObject
@property (nonatomic, strong) JConversation *conversation;
/// Message type
@property (nonatomic, copy) NSString *contentType;
/// Local unique message number (valid only on the current device)
@property (nonatomic, assign) long long clientMsgNo;
/// Message id, globally unique
@property (nonatomic, copy) NSString *messageId;
/// Message direction, send/receive
@property (nonatomic, assign) JMessageDirection direction;
/// Message status
@property (nonatomic, assign) JMessageState messageState;
/// Whether the message has been read
/// For the receiver, indicates whether they have read this message
/// For the sender, indicates whether the other party has read this message (only valid for one-to-one chats)
@property (nonatomic, assign) BOOL hasRead;
/// Timestamp when the message was sent (server time, in milliseconds)
@property (nonatomic, assign) long long timestamp;
/// Sender userId
@property (nonatomic, copy) NSString *senderUserId;
/// Message content
@property (nonatomic, strong) JMessageContent *content;
/// Group message read info (only valid for group messages)
@property (nonatomic, strong) JGroupMessageReadInfo *groupReadInfo;
/// Mention info (nil for non-mention messages)
@property (nonatomic, strong) JMessageMentionInfo *mentionInfo;
/// Referenced message
@property (nonatomic, strong) JMessage *referredMsg;
/// Local message attributes (valid only locally and not synced to the server)
@property (nonatomic, copy) NSString *localAttribute;
/// Whether the message has been edited
@property (nonatomic, assign) BOOL isEdit;
/// Whether the message has been deleted
@property (nonatomic, assign) BOOL isDeleted;
/// Message destruction timestamp (server time, in milliseconds).
/// Determined by the message sent time plus the lifeTime and lifeTimeAfterRead in JMessageOptions when sending; the earlier timestamp is used.
/// The default value is 0, which means no automatic destruction.
@property (nonatomic, assign) long long destroyTime;
/// Message lifetime after being read, in milliseconds.
/// The default value is 0, which means no automatic destruction after read.
@property (nonatomic, assign) long long lifeTimeAfterRead;
/// Sender name (displayed according to the default rules)
@property (nonatomic, copy) NSString *senderDisplayName;
/// Sender friend remark
@property (nonatomic, copy) NSString *friendAlias;
/// Sender group member remark
@property (nonatomic, copy) NSString *groupMemberAlias;
/// Sender real name
@property (nonatomic, copy) NSString *senderName;
/// Sender avatar URL
@property (nonatomic, copy) NSString *senderPortrait;
@end
