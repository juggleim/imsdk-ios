//
//  JPBData.h
//  Juggle
//
//  Created by Nathan on 2023/12/6.
//

#import <Foundation/Foundation.h>
#import "JConversation.h"

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


+ (JAck *)ackWithData:(NSData *)data;
@end

NS_ASSUME_NONNULL_END
