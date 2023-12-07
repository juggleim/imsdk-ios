//
//  JPBData.h
//  Juggle
//
//  Created by Nathan on 2023/12/6.
//

#import <Foundation/Foundation.h>
#import "JConversation.h"
#import "JMessage.h"
#import "JuggleConst.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, JAckType) {
    JAckTypeParseError,
    JAckTypeConnect,
    JAckTypePublishMsg,
    JAckTypeQryMsg
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
@end

@interface JQryMsgAck : NSObject
@property (nonatomic, assign) int index;
@property (nonatomic, assign) int code;
@property (nonatomic, assign) long long timestamp;
@property (nonatomic, assign) long long syncTime;
@property (nonatomic, assign) BOOL isFinished;
@property (nonatomic, strong) NSArray *msgs;
@end

@interface JAck : NSObject
@property (nonatomic, assign) JAckType ackType;
@property (nonatomic, strong) JConnectAck *connectAck;
@property (nonatomic, strong) JPublishMsgAck *publishMsgAck;
@property (nonatomic, strong) JQryMsgAck *qryMsgAck;
@end



@interface JPBData : NSObject
+ (NSData *)connectDataWithAppKey:(NSString *)appKey
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

+ (NSData *)sendMessageDataWithType:(NSString *)contentType
                            msgData:(NSData *)msgData
                              flags:(int)flags
                          clientUid:(NSString *)clientUid
                              index:(int)index
                   conversationType:(JConversationType)conversationType
                     conversationId:(NSString *)conversationId;

+ (NSData *)queryHisMsgsFrom:(JConversation *)conversation
                   startTime:(long long)startTime
                       count:(int)count
                   direction:(JPullDirection)direction
                       index:(int)index;

+ (NSData *)disconnectData:(BOOL)needPush;

+ (void)registerMessageType:(Class)messageClass;

+ (JAck *)ackWithData:(NSData *)data;
@end

NS_ASSUME_NONNULL_END
