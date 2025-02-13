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
@property (nonatomic, strong) JGroupMember *groupMemberInfo;
@property (nonatomic, copy) NSString *referMsgId;
@property (nonatomic, strong) JPushData *pushData;
@end

NS_ASSUME_NONNULL_END
