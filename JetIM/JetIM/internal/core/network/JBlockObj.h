//
//  JBlockObj.h
//  JetIM
//
//  Created by Nathan on 2024/5/31.
//

#import <Foundation/Foundation.h>
#import "JetIMConstInternal.h"
#import "JUserInfo.h"
#import "JConcreteConversationInfo.h"

NS_ASSUME_NONNULL_BEGIN

@interface JBlockObj : NSObject
@end

@interface JSendMessageObj : JBlockObj
@property (nonatomic, assign) long long clientMsgNo;
@property (nonatomic, copy) void (^successBlock)(long long clientMsgNo, NSString *msgId, long long timestamp, long long seqNo);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode, long long clientMsgNo);
@end

@interface JQryHisMsgsObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(NSArray * _Nonnull msgs, BOOL isFinished);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@interface JSyncConvsObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(NSArray * _Nonnull convs, NSArray * _Nonnull deletedConvs, BOOL isFinished);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@interface JTimestampBlockObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(long long timestamp);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@interface JQryReadDetailObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(NSArray<JUserInfo *> *readMembers, NSArray<JUserInfo *> *unreadMembers);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@interface JSimpleBlockObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(void);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@interface JConversationObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(JConcreteConversationInfo *conversationInfo, long long timestamp);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

NS_ASSUME_NONNULL_END
