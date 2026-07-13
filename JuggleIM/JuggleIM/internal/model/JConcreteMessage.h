//
//  JConcreteMessage.h
// JuggleIM
//
//  Created by Nathan on 2023/12/10.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JConcreteMessage : JMessage
@property (nonatomic, assign) long long seqNo;
@property (nonatomic, assign) long long msgIndex;
@property (nonatomic, copy) NSString *clientUid;
@property (nonatomic, assign) int flags;
@property (nonatomic, assign) BOOL existed;
@property (nonatomic, strong) JGroupInfo *groupInfo;
@property (nonatomic, strong) JUserInfo *targetUserInfo;
@property (nonatomic, strong) JUserInfo *senderUserInfo;
@property (nonatomic, strong) JGroupMember *groupMemberInfo;
@property (nonatomic, strong) JFriendInfo *friendInfo;
@property (nonatomic, copy) NSString *referMsgId;
@property (nonatomic, strong) JPushData *pushData;
@property (nonatomic, assign) long long lifeTime;
/// Message read time (only effective for one-to-one chats).
@property (nonatomic, assign) long long readTime;
@property (nonatomic, assign) BOOL isMute;
/// Encrypted message hash retry count.
@property (nonatomic, assign) int hashCount;
@end

NS_ASSUME_NONNULL_END
