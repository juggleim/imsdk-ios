//
//  JPBData.h
// JuggleIM
//
//  Created by Nathan on 2023/12/6.
//

#import <Foundation/Foundation.h>
#import "JConversation.h"
#import "JMessage.h"
#import "JConcreteConversationInfo.h"
#import "JuggleIMConst.h"
#import "JConcreteMessage.h"
#import "JMergeInfo.h"
#import "JUploadEnum.h"
#import "JUploadQiNiuCred.h"
#import "JUploadPreSignCred.h"
#import "JChatroomAttributeItem.h"
#import "JPushData.h"
#import "JRtcRoom.h"

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
    JPBRcvTypeConversationSetTopAck,
    JPBRcvTypeAddConversation,
    JPBRcvTypeFileCredMsgAck,
    JPBRcvTypeGlobalMuteAck,
    JPBRcvTypePublishChatroomMsgNtf,
    JPBRcvTypeSyncChatroomMsgsAck,
    JPBRcvTypeQryFirstUnreadMsgAck,
    JPBRcvTypeSetChatroomAttrAck,
    JPBRcvTypePublishChatroomAttrNtf,
    JPBRcvTypeSyncChatroomAttrsAck,
    JPBRcvTypeRemoveChatroomAttrAck,
    JPBRcvTypeChatroomDestroyNtf,
    JPBRcvTypeChatroomEventNtf,
    JPBRcvTypeRtcRoomEventNtf,
    JPBRcvTypeRtcInviteEventNtf,
    JPBRcvTypeCallAuthAck,
    JPBRcvTypeRtcPingAck,
    JPBRcvTypeQryCallRoomsAck,
    JPBRcvTypeQryCallRoomAck,
    JPBRcvTypeGetUserSettingAck,
    JPBRcvTypeQryMsgExtAck,
    JPBRcvTypeGetTopMsgAck,
    JPBRcvTypeGetFavoriteMsgAck,
    JPBRcvTypeGetConversationConfAck,
    JPBRcvTypeGetUserInfoAck,
    JPBRcvTypeGetGroupInfoAck
};

typedef NS_ENUM(NSUInteger, JPBChrmEventType) {
    JPBChrmEventTypeJoin = 0,
    JPBChrmEventTypeQuit = 1,
    JPBChrmEventTypeKick = 2,
    JPBChrmEventTypeFallout = 3
};

typedef NS_ENUM(NSUInteger, JPBRtcInviteType) {
    JPBRtcInviteTypeInvite = 0,
    JPBRtcInviteTypeAccept = 1,
    JPBRtcInviteTypeHangup = 2
};

typedef NS_ENUM(NSUInteger, JPBRtcRoomEventType) {
    JPBRtcRoomEventTypeDefault = 0,
    JPBRtcRoomEventTypeJoin = 1,
    JPBRtcRoomEventTypeQuit = 2,
    JPBRtcRoomEventTypeDestroy = 3,
    JPBRtcInviteTypeStateChange = 4,
};

@interface JConnectAck : NSObject
@property (nonatomic, assign) int code;
@property (nonatomic, copy) NSString *userId;
@property (nonatomic, copy) NSString *session;
@property (nonatomic, copy) NSString *extra;
@end

@interface JPublishMsgAck : NSObject
@property (nonatomic, assign) int index;
@property (nonatomic, assign) int code;
@property (nonatomic, copy) NSString *msgId;
@property (nonatomic, assign) long long timestamp;
@property (nonatomic, assign) long long seqNo;
@property (nonatomic, copy) NSString *clientUid;
@property (nonatomic, copy) NSString *contentType;
@property (nonatomic, strong) JMessageContent *content;
@property (nonatomic, assign) int groupMemberCount;
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

@interface JConversationInfoAck : JQryAck
@property (nonatomic, strong) JConcreteConversationInfo *conversationInfo;
@end

@interface JSimpleQryAck : JQryAck
@end

@interface JTimestampQryAck : JQryAck
@property (nonatomic, assign) long long operationTime;
@end

@interface JPublishMsgNtf : NSObject
@property (nonatomic, assign) long long syncTime;
@property (nonatomic, copy) NSString *chatroomId;
@property (nonatomic, assign) JPBChrmEventType type;
@end

@interface JRtcInviteEventNtf : NSObject
@property (nonatomic, assign) JPBRtcInviteType type;
@property (nonatomic, strong) JUserInfo *user;
@property (nonatomic, strong) JRtcRoom *room;
@property (nonatomic, strong) NSArray <JUserInfo *> *targetUsers;
@end

@interface JRtcRoomEventNtf : NSObject
@property (nonatomic, assign) JPBRtcRoomEventType eventType;
@property (nonatomic, strong) NSArray <JCallMember *> *members;
@property (nonatomic, strong) JRtcRoom *room;
@end

@interface JDisconnectMsg : NSObject
@property (nonatomic, assign) int code;
@property (nonatomic, assign) long long timestamp;
@property (nonatomic, copy) NSString *extra;
@end

@interface JQryFileCredAck : JQryAck
@property (nonatomic, assign) JUploadOssType ossType;
@property (nonatomic, strong) JUploadQiNiuCred *qiNiuCred;
@property (nonatomic, strong) JUploadPreSignCred *preSignCred;
@end

@interface JGlobalMuteAck : JQryAck
@property (nonatomic, assign) BOOL isMute;
@property (nonatomic, copy) NSString *timezone;
@property (nonatomic, copy) NSArray <JTimePeriod *> *periods;
@end

@interface JChatroomAttrsAck : JQryAck
@property (nonatomic, copy) NSArray <JChatroomAttributeItem *> *items;
@end

@interface JStringAck : JQryAck
@property (nonatomic, copy) NSString *str;
@end

@interface JRtcQryCallRoomsAck : JQryAck
@property (nonatomic, copy) NSArray <JRtcRoom *> *rooms;
@end

@interface JQryMsgExtAck : JQryAck
@property (nonatomic, copy) NSArray <JMessageReaction *> *reactionList;
@end

@interface JRtcAuthAck : JQryAck
@property (nonatomic, copy) NSString *token;
@property (nonatomic, copy) NSString *url;
@end

@interface JGetTopMsgAck : JQryAck
@property (nonatomic, strong) JConcreteMessage *message;
@property (nonatomic, strong) JUserInfo *userInfo;
@property (nonatomic, assign) long long createdTime;
@end

@interface JGetFavoriteMsgAck : JQryAck
@property (nonatomic, copy) NSArray <JFavoriteMessage *> *favoriteMsgs;
@property (nonatomic, copy) NSString *offset;
@end

@interface JTemplateAck<T> : JQryAck
@property (nonatomic, strong) T t;
@end

@interface JPBRcvObj : NSObject
@property (nonatomic, assign) long long timestamp;
@property (nonatomic, assign) JPBRcvType rcvType;
@property (nonatomic, strong) JConnectAck *connectAck;
@property (nonatomic, strong) JPublishMsgAck *publishMsgAck;
@property (nonatomic, strong) JQryHisMsgsAck *qryHisMsgsAck;
@property (nonatomic, strong) JSyncConvsAck *syncConvsAck;
@property (nonatomic, strong) JPublishMsgBody *publishMsgBody;
@property (nonatomic, strong) JPublishMsgNtf *publishMsgNtf;
@property (nonatomic, strong) JDisconnectMsg *disconnectMsg;
@property (nonatomic, strong) JSimpleQryAck *simpleQryAck;
@property (nonatomic, strong) JTimestampQryAck *timestampQryAck;
@property (nonatomic, strong) JConversationInfoAck *conversationInfoAck;
@property (nonatomic, strong) JQryFileCredAck *qryFileCredAck;
@property (nonatomic, strong) JGlobalMuteAck *globalMuteAck;
@property (nonatomic, strong) JChatroomAttrsAck *chatroomAttrsAck;
@property (nonatomic, strong) JRtcRoomEventNtf *rtcRoomEventNtf;
@property (nonatomic, strong) JRtcInviteEventNtf *rtcInviteEventNtf;
@property (nonatomic, strong) JStringAck *stringAck;
@property (nonatomic, strong) JRtcQryCallRoomsAck *rtcQryCallRoomsAck;
@property (nonatomic, strong) JQryMsgExtAck *qryMsgExtAck;
@property (nonatomic, strong) JRtcAuthAck *rtcAuthAck;
@property (nonatomic, strong) JGetTopMsgAck *getTopMsgAck;
@property (nonatomic, strong) JGetFavoriteMsgAck *getFavoriteMsgAck;
@property (nonatomic, strong) JTemplateAck *templateAck;
@end

@interface JPBData : NSObject
- (void)resetDataConverter;

- (void)setMessagePreprocessor:(id<JMessagePreprocessor>)preprocessor;

- (NSData *)connectDataWithAppKey:(NSString *)appKey
                            token:(NSString *)token
                         deviceId:(NSString *)deviceId
                         platform:(NSString *)platform
                    deviceCompany:(NSString *)deviceCompany
                      deviceModel:(NSString *)deviceModel
                  deviceOsVersion:(NSString *)osVersion
                      packageName:(NSString *)packageName
                        pushToken:(NSString *)pushToken
                        voipToken:(NSString *)voipToken
                        networkId:(NSString *)networkId
                           ispNum:(NSString *)ispNum
                         clientIp:(NSString *)clientIp
                         language:(NSString *)language;

- (NSData *)sendMessageDataWithType:(NSString *)contentType
                            msgData:(NSData *)msgData
                              flags:(int)flags
                          clientUid:(NSString *)clientUid
                          mergeInfo:(JMergeInfo *)mergeInfo
                        isBroadcast:(BOOL)isBroadcast
                             userId:(NSString *)userId
                              index:(int)index
                       conversation:(JConversation *)conversation
                        mentionInfo:(JMessageMentionInfo *)mentionInfo
                    referredMessage:(JConcreteMessage *)referredMessage
                           pushData:(JPushData *)pushData
                           lifeTime:(long long)lifeTime
                  lifeTimeAfterRead:(long long)lifeTimeAfterRead;

- (NSData *)recallMessageData:(NSString *)messageId
                       extras:(NSDictionary *)extras
                 conversation:(JConversation *)conversation
                    timestamp:(long long)msgTime
                        index:(int)index;

- (NSData *)updateMessageData:(NSString *)messageId
                      msgType:(NSString *)contentType
                      msgData:(NSData *)msgData
                 conversation:(JConversation *)conversation
                    timestamp:(long long)timestamp
                     msgSeqNo:(long long)msgSeqNo
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
                    contentTypes:(NSArray <NSString *> *)contentTypes
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
                           msgId:(NSString *)msgId
                       timestamp:(long long)timestamp
                           index:(int)index;

- (NSData *)undisturbData:(JConversation *)conversation
                   userId:(NSString *)userId
                   isMute:(BOOL)isMute
                    index:(int)index;

- (NSData *)topConversationData:(JConversation *)conversation
                         userId:(NSString *)userId
                          isTop:(BOOL)isTop
                          index:(int)index;

- (NSData *)topMessageData:(NSString *)messageId
              conversation:(JConversation *)conversation
                     isTop:(BOOL)isTop
                     index:(int)index;

- (NSData *)getTopMessageData:(JConversation *)conversation
                        index:(int)index;

- (NSData *)favoriteMessagesData:(NSArray <JMessage *> *)messages
                           isAdd:(BOOL)isAdd
                          userId:(NSString *)userId
                           index:(int)index;

- (NSData *)getFavoriteMessagesData:(NSString *)userId
                              limit:(int)limit
                             offset:(NSString *)offset
                              index:(int)index;

- (NSData *)markUnread:(JConversation *)conversation
                userId:(NSString *)userId
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

- (NSData *)registerVoIPToken:(NSString *)token
                     deviceId:(NSString *)deviceId
                  packageName:(NSString *)packageName
                       userId:(NSString *)userId
                        index:(int)index;

- (NSData *)getMentionMessages:(JConversation *)conversation
                          time:(long long)timestamp
                         count:(int)count
                     direction:(JPullDirection)direction
                 lastReadIndex:(long long)lastReadIndex
                         index:(int)index;

- (NSData *)clearTotalUnreadCountMessages:(NSString *)userId
                                     time:(long long)time
                                    index:(int)index;

- (NSData *)deleteMessage:(JConversation *)conversation
                  msgList:(NSArray <JConcreteMessage *> *)msgList
              forAllUsers:(BOOL)forAllUsers
                    index:(int)index;

- (NSData *)clearHistoryMessage:(JConversation *)conversation
                           time:(long long)time
                          scope:(int)scope
                          index:(int)index;

- (NSData *)createConversationInfo:(JConversation *)conversation
                            userId:(NSString *)userId
                             index:(int)index;

- (NSData *)getUploadFileCred:(NSString *)userId
                     fileType:(JUploadFileType)fileType
                          ext:(NSString *)ext
                        index:(int)index;

- (NSData *)setGlobalMute:(BOOL)isMute
                   userId:(NSString *)userId
                 timezone:(NSString *)timezone
                  periods:(NSArray<JTimePeriod *> *)periods
                    index:(int)index;

- (NSData *)getGlobalMute:(NSString *)userId
                    index:(int)index;

- (NSData *)joinChatroom:(NSString *)chatroomId
            isAutoCreate:(BOOL)isAutoCreate
                   index:(int)index;

- (NSData *)quitChatroom:(NSString *)chatroomId
                   index:(int)index;

- (NSData *)pushSwitch:(BOOL)enablePush
                userId:(NSString *)userId
                 index:(int)index;

- (NSData *)uploadLogStatus:(int)result
                     userId:(NSString *)userId
                  messageId:(NSString *)messageId
                        url:(NSString *)url
                      index:(int)index;

- (NSData *)syncChatroomMessages:(long long)syncTime
                      chatroomId:(NSString *)chatroomId
                          userId:(NSString *)userId
                prevMessageCount:(int)count
                           index:(int)index;

- (NSData *)syncChatroomAttributes:(long long)syncTime
                        chatroomId:(NSString *)chatroomId
                            userId:(NSString *)userId
                             index:(int)index;

- (NSData *)qryFirstUnreadMessage:(JConversation *)conversation
                            index:(int)index;

- (NSData *)setAttributes:(NSDictionary<NSString *,NSString *> *)attributes
              forChatroom:(NSString *)chatroomId
                    index:(int)index;

- (NSData *)removeAttributes:(NSArray <NSString *> *)keys
                 forChatroom:(NSString *)chatroomId
                       index:(int)index;

- (NSData *)addMsgSet:(NSString *)messageId
         conversation:(JConversation *)conversation
                  key:(NSString *)key
               userId:(NSString *)userId
                index:(int)index;

- (NSData *)removeMsgSet:(NSString *)messageId
            conversation:(JConversation *)conversation
                     key:(NSString *)key
                  userId:(NSString *)userId
                   index:(int)index;

- (NSData *)queryMsgExSet:(NSArray <NSString *> *)messageIdList
             conversation:(JConversation *)conversation
                    index:(int)index;

- (NSData *)addConversations:(NSArray <JConversation *> *)conversations
                       toTag:(NSString *)tagId
                      userId:(NSString *)userId
                       index:(int)index;

- (NSData *)removeConversations:(NSArray <JConversation *> *)conversations
                        fromTag:(NSString *)tagId
                         userId:(NSString *)userId
                          index:(int)index;

- (NSData *)fetchUserInfo:(NSString *)userId
                    index:(int)index;

- (NSData *)fetchGroupInfo:(NSString *)groupId
                     index:(int)index;

- (NSData *)pingData;

- (NSData *)publishAckData:(int)index;

#pragma mark - Call
- (NSData *)callInvite:(NSString *)callId
           isMultiCall:(BOOL)isMultiCall
             mediaType:(JCallMediaType)mediaType
          conversation:(JConversation *)conversation
          targetIdList:(NSArray <NSString *>*)userIdList
            engineType:(NSUInteger)engineType
                 extra:(NSString *)extra
                 index:(int)index;

- (NSData *)callHangup:(NSString *)callId
                 index:(int)index;

- (NSData *)callAccept:(NSString *)callId
                 index:(int)index;

- (NSData *)callConnected:(NSString *)callId
                    index:(int)index;

- (NSData *)callJoin:(NSString *)callId
               index:(int)index;

- (NSData *)getConversationCallInfo:(JConversation *)conversation
                             userId:(NSString *)userId
                              index:(int)index;

- (NSData *)queryCallRooms:(NSString *)userId
                     index:(int)index;

- (NSData *)queryCallRoom:(NSString *)roomId
                    index:(int)index;

- (NSData *)setLanguage:(NSString *)language
                 userId:(NSString *)userId
                  index:(int)index;

- (NSData *)getLanguage:(NSString *)userId
                  index:(int)index;

- (NSData *)rtcPingData:(NSString *)callId
                  index:(int)index;

- (JPBRcvObj *)rcvObjWithData:(NSData *)data;
@end

NS_ASSUME_NONNULL_END
