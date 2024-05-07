//
//  JPBData.h
//  JetIM
//
//  Created by Nathan on 2023/12/6.
//

#import <Foundation/Foundation.h>
#import "JConversation.h"
#import "JMessage.h"
#import "JConcreteConversationInfo.h"
#import "JetIMConst.h"
#import "JConcreteMessage.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, JPBRcvType) {
    JPBRcvTypeParseError,
    JPBRcvTypeCmdMatchError,
    JPBRcvTypeConnectAck,
    JPBRcvTypePublishMsgAck,
    JPBRcvTypeQryHisMsgsAck,
    JPBRcvTypeSyncConvsAck,
    JPBRcvTypeSyncMsgsAck,
    JPBRcvTypePublishMsg,
    JPBRcvTypePublishMsgNtf,
    JPBRcvTypePong,
    JPBRcvTypeDisconnectMsg,
    JPBRcvTypeQryReadDetailAck,
    JPBRcvTypeSimpleQryAck,
    JPBRcvTypeSimpleQryAckCallbackTimestamp,
    JPBRcvTypeConversationSetTopAck
};

typedef NS_ENUM(NSUInteger, JPBNotifyType) {
    JPBNotifyTypeDefault = 0,
    JPBNotifyTypeMsg
};

@interface JConnectAck : NSObject
@property (nonatomic, assign) int code;
@property (nonatomic, copy) NSString *userId;
@end

@interface JPublishMsgAck : NSObject
@property (nonatomic, assign) int index;
@property (nonatomic, assign) int code;
@property (nonatomic, copy) NSString *msgId;
@property (nonatomic, assign) long long timestamp;
@property (nonatomic, assign) long long seqNo;
@end

@interface JPublishMsgBody : NSObject
@property (nonatomic, strong) JConcreteMessage *rcvMessage;
@property (nonatomic, assign) int index;
@property (nonatomic, assign) int qos;
@end

@interface JQryAck : NSObject
@property (nonatomic, assign) int index;
@property (nonatomic, assign) int code;
@property (nonatomic, assign) long long timestamp;
@end

@interface JQryHisMsgsAck : JQryAck
@property (nonatomic, assign) BOOL isFinished;
@property (nonatomic, copy) NSArray<JConcreteMessage *> *msgs;
@end

@interface JSyncConvsAck : JQryAck
@property (nonatomic, assign) BOOL isFinished;
@property (nonatomic, copy) NSArray<JConcreteConversationInfo *> *convs;
@property (nonatomic, copy) NSArray<JConcreteConversationInfo *> *deletedConvs;
@end

@interface JQryReadDetailAck: JQryAck
@property (nonatomic, copy) NSArray<JUserInfo *> *readMembers;
@property (nonatomic, copy) NSArray<JUserInfo *> *unreadMembers;
@end

@interface JSimpleQryAck : JQryAck
@end

@interface JTimestampQryAck : JQryAck
@property (nonatomic, assign) long long operationTime;
@end

@interface JPublishMsgNtf : NSObject
@property (nonatomic, assign) long long syncTime;
@end

@interface JDisconnectMsg : NSObject
@property (nonatomic, assign) int code;
@property (nonatomic, assign) long long timestamp;
@end

@interface JPBRcvObj : NSObject
@property (nonatomic, assign) JPBRcvType rcvType;
@property (nonatomic, strong) JConnectAck *connectAck;
@property (nonatomic, strong) JPublishMsgAck *publishMsgAck;
@property (nonatomic, strong) JQryHisMsgsAck *qryHisMsgsAck;
@property (nonatomic, strong) JSyncConvsAck *syncConvsAck;
@property (nonatomic, strong) JPublishMsgBody *publishMsgBody;
@property (nonatomic, strong) JPublishMsgNtf *publishMsgNtf;
@property (nonatomic, strong) JDisconnectMsg *disconnectMsg;
@property (nonatomic, strong) JQryReadDetailAck *qryReadDetailAck;
@property (nonatomic, strong) JSimpleQryAck *simpleQryAck;
@property (nonatomic, strong) JTimestampQryAck *timestampQryAck;
@end

@interface JPBData : NSObject
- (NSData *)connectDataWithAppKey:(NSString *)appKey
                            token:(NSString *)token
                         deviceId:(NSString *)deviceId
                         platform:(NSString *)platform
                    deviceCompany:(NSString *)deviceCompany
                      deviceModel:(NSString *)deviceModel
                  deviceOsVersion:(NSString *)osVersion
                      packageName:(NSString *)packageName
                        pushToken:(NSString *)pushToken
                        networkId:(NSString *)networkId
                           ispNum:(NSString *)ispNum
                         clientIp:(NSString *)clientIp;

- (NSData *)sendMessageDataWithType:(NSString *)contentType
                            msgData:(NSData *)msgData
                              flags:(int)flags
                          clientUid:(NSString *)clientUid
                         mergedMsgs:(NSArray <JConcreteMessage *> *)mergedMsgs
                        isBroadcast:(BOOL)isBroadcast
                             userId:(NSString *)userId
                              index:(int)index
                   conversationType:(JConversationType)conversationType
                     conversationId:(NSString *)conversationId
                        mentionInfo:(nullable JMessageMentionInfo *)mentionInfo;

- (NSData *)recallMessageData:(NSString *)messageId
                 conversation:(JConversation *)conversation
                    timestamp:(long long)msgTime
                        index:(int)index;

- (NSData *)sendReadReceiptData:(NSArray <NSString *> *)messageIds
                 inConversation:(JConversation *)conversation
                          index:(int)index;

- (NSData *)getGroupMessageReadDetail:(NSString *)messageId
                       inConversation:(JConversation *)conversation
                                index:(int)index;

- (NSData *)syncMessagesDataWithReceiveTime:(long long)receiveTime
                                   sendTime:(long long)sendTime
                                     userId:(NSString *)userId
                                      index:(int)index;

- (NSData *)queryHisMsgsDataFrom:(JConversation *)conversation
                       startTime:(long long)startTime
                           count:(int)count
                       direction:(JPullDirection)direction
                           index:(int)index;

- (NSData *)queryHisMsgsDataByIds:(NSArray <NSString *> *)messageIds
                   inConversation:(JConversation *)conversation
                            index:(int)index;

- (NSData *)disconnectData:(BOOL)needPush;

- (NSData *)syncConversationsData:(long long)startTime
                            count:(int)count
                           userId:(NSString *)userId
                            index:(int)index;

- (NSData *)deleteConversationData:(JConversation *)conversation
                            userId:(NSString *)userId
                             index:(int)index;

- (NSData *)clearUnreadCountData:(JConversation *)conversation
                          userId:(NSString *)userId
                        msgIndex:(long long)msgIndex
                           index:(int)index;

- (NSData *)undisturbData:(JConversation *)conversation
                   userId:(NSString *)userId
                   isMute:(BOOL)isMute
                    index:(int)index;

- (NSData *)topConversationData:(JConversation *)conversation
                         userId:(NSString *)userId
                          isTop:(BOOL)isTop
                          index:(int)index;

- (NSData *)getMergedMessageList:(NSString *)messageId
                            time:(long long)timestamp
                           count:(int)count
                       direction:(JPullDirection)direction
                           index:(int)index;

- (NSData *)registerPushToken:(NSString *)token
                     deviceId:(NSString *)deviceId
                  packageName:(NSString *)packageName
                       userId:(NSString *)userId
                        index:(int)index;

- (NSData *)getMentionMessages:(JConversation *)conversation
                          time:(long long)timestamp
                         count:(int)count
                     direction:(JPullDirection)direction
                         index:(int)index;

- (NSData *)clearTotalUnreadCountMessages:(NSString *)userId
                                     time:(long long)time
                                    index:(int)index;

- (NSData *)pingData;

- (NSData *)publishAckData:(int)index;

- (JPBRcvObj *)rcvObjWithData:(NSData *)data;
@end

NS_ASSUME_NONNULL_END
