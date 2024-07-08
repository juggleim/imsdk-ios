//
//  JMergeInfo.h
//  JetIM
//
//  Created by 郑开 on 2024/6/26.
//

#import <Foundation/Foundation.h>
#import "JConcreteMessage.h"

@interface JMergeInfo : NSObject

/// 被合并消息的会话标识
@property (nonatomic, strong) JConversation * conversation;

@property (nonatomic, copy) NSArray <JConcreteMessage *> * messages;

@property (nonatomic, copy) NSString * containerMsgId;

@end

