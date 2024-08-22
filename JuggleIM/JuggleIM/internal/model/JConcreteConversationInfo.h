//
//  JConcreteConversationInfo.h
// JuggleIM
//
//  Created by Nathan on 2023/12/25.
//

#import <JuggleIM/JuggleIM.h>
#import "JGroupInfo.h"
#import "JUserInfo.h"

NS_ASSUME_NONNULL_BEGIN

@interface JConcreteConversationInfo : JConversationInfo
@property (nonatomic, assign) long long lastReadMessageIndex;//最后一条读过的消息的未读数 index
@property (nonatomic, assign) long long lastMessageIndex;//最后一条消息的未读数 index
@property (nonatomic, assign) long long syncTime;
@property (nonatomic, strong) JGroupInfo *groupInfo;
@property (nonatomic, strong) JUserInfo *targetUserInfo;
@property (nonatomic, strong) NSArray <JUserInfo *> * mentionUserList;
@end

NS_ASSUME_NONNULL_END
