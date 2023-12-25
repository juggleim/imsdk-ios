//
//  JConversationInfo.h
//  Juggle
//
//  Created by Nathan on 2023/11/27.
//

#import <Foundation/Foundation.h>

#import <Juggle/JConversation.h>
#import <Juggle/JMessage.h>

NS_ASSUME_NONNULL_BEGIN

@interface JConversationInfo : NSObject

@property (nonatomic, strong) JConversation *conversation;
@property (nonatomic, assign) int unreadCount;
@property (nonatomic, assign) long long updateTime;
@property (nonatomic, strong) JMessage *lastMessage;
@property (nonatomic, assign) BOOL isTop;
@property (nonatomic, assign) long long topTime;
@property (nonatomic, assign) BOOL mute;
@property (nonatomic, copy) NSString *draft;
//TODO: mention
//@property (nonatomic, strong) JMessage *lastMentionMessage;

@end

NS_ASSUME_NONNULL_END
