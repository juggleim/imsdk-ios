//
//  JPBData.h
//  Juggle
//
//  Created by Nathan on 2023/12/6.
//

#import <Foundation/Foundation.h>
#import "JConversation.h"
#import "JMessage.h"
#import "JConcreteConversationInfo.h"
#import "JuggleConst.h"
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
    JPBRcvTypePong
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
@end

@interface JPublishMsgNtf : NSObject
@property (nonatomic, assign) long long syncTime;
@end

@interface JPBRcvObj : NSObject
@property (nonatomic, assign) JPBRcvType rcvType;
@property (nonatomic, strong) JConnectAck *connectAck;
@property (nonatomic, strong) JPublishMsgAck *publishMsgAck;
@property (nonatomic, strong) JQryHisMsgsAck *qryHisMsgsAck;
@property (nonatomic, strong) JSyncConvsAck *syncConvsAck;
@property (nonatomic, strong) JConcreteMessage *rcvMessage;
@property (nonatomic, strong) JPublishMsgNtf *publishMsgNtf;
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

- (NSData *)pingData;

- (JPBRcvObj *)rcvObjWithData:(NSData *)data;
@end

NS_ASSUME_NONNULL_END
