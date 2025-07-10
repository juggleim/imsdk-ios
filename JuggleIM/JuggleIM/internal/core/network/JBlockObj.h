//
//  JBlockObj.h
// JuggleIM
//
//  Created by Nathan on 2024/5/31.
//

#import <Foundation/Foundation.h>
#import "JuggleIMConstInternal.h"
#import "JUserInfo.h"
#import "JConcreteConversationInfo.h"
#import "JUploadEnum.h"
#import "JUploadQiNiuCred.h"
#import "JUploadPreSignCred.h"
#import "JChatroomAttributeItem.h"
#import "JRtcRoom.h"

NS_ASSUME_NONNULL_BEGIN

@interface JBlockObj : NSObject
@end

@interface JSendMessageObj : JBlockObj
@property (nonatomic, assign) long long clientMsgNo;
@property (nonatomic, copy) void (^successBlock)(long long clientMsgNo, NSString *msgId, long long timestamp, long long seqNo,  NSString * _Nullable contentType,  JMessageContent * _Nullable content, int groupMemberCount);
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

@interface JUploadFileCredBlockObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(JUploadOssType ossType, JUploadQiNiuCred *qiNiuCred, JUploadPreSignCred *preSignCred);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@interface JGlobalMuteObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(BOOL isMute, NSString * _Nonnull timezone, NSArray<JTimePeriod *> * _Nonnull periods);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@interface JUpdateChatroomAttrObj : JBlockObj
@property (nonatomic, copy) void (^completeBlock)(JErrorCodeInternal code,  NSArray<JChatroomAttributeItem *> * _Nullable items);
@end

@interface JChatroomObj : JBlockObj
@property (nonatomic, copy) NSString *chatroomId;
@end

@interface JStringObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(NSString *);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@interface JMessageReactionObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(NSArray <JMessageReaction *> *);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@interface JRtcRoomArrayObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(NSArray <JRtcRoom *> *);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

@interface JRtcAuthObj : JBlockObj
@property (nonatomic, copy) void (^successBlock)(NSString *token, NSString *url);
@property (nonatomic, copy) void (^errorBlock)(JErrorCodeInternal errorCode);
@end

NS_ASSUME_NONNULL_END
