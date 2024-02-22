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
    JPBRcvTypeRecall,
    JPBRcvTypeDelConvsAck
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
@property (nonatomic, assign) long long msgIndex;
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
@property (nonatomic, strong) NSArray<JConcreteMessage *> *msgs;
@end

@interface JSyncConvsAck : JQryAck
@property (nonatomic, assign) BOOL isFinished;
@property (nonatomic, strong) NSArray<JConcreteConversationInfo *> *convs;
@property (nonatomic, strong) NSArray<JConcreteConversationInfo *> *deletedConvs;
@end

@interface JSimpleQryAck : JQryAck
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
@property (nonatomic, strong) JSimpleQryAck *simpleQryAck;
@end

@interface JPBData : NSObject
- (NSData *)connectDataWithAppKey:(NSString *)appKey
                            token:(NSString *)token
                         deviceId:(NSString *)deviceId
                         platform:(NSString *)platform
                    deviceCompany:(NSString *)deviceCompany
                      deviceModel:(NSString *)deviceModel
                  deviceOsVersion:(NSString *)osVersion
                        pushToken:(NSString *)pushToken
                        networkId:(NSString *)networkId
                           ispNum:(NSString *)ispNum
                         clientIp:(NSString *)clientIp;

- (NSData *)sendMessageDataWithType:(NSString *)contentType
                            msgData:(NSData *)msgData
                              flags:(int)flags
                          clientUid:(NSString *)clientUid
                              index:(int)index
                   conversationType:(JConversationType)conversationType
                     conversationId:(NSString *)conversationId;

- (NSData *)recallMessageData:(NSString *)messageId
                 conversation:(JConversation *)conversation
                    timestamp:(long long)msgTime
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

- (NSData *)disconnectData:(BOOL)needPush;

- (NSData *)syncConversationsData:(long long)startTime
                            count:(int)count
                           userId:(NSString *)userId
                            index:(int)index;

- (NSData *)deleteConversationData:(JConversation *)conversation
                            userId:(NSString *)userId
                             index:(int)index;

- (NSData *)pingData;

- (NSData *)publishAckData:(int)index;

- (JPBRcvObj *)rcvObjWithData:(NSData *)data;
@end

NS_ASSUME_NONNULL_END
