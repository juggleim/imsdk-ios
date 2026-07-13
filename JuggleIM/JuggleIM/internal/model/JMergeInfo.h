//
//  JMergeInfo.h
// JuggleIM
//
//  Created by Zheng Kai on 2024/6/26.
//

#import <Foundation/Foundation.h>
#import "JConcreteMessage.h"

@interface JMergeInfo : NSObject

/// Conversation identifier of the merged message.
@property (nonatomic, strong) JConversation * conversation;

@property (nonatomic, copy) NSArray <JConcreteMessage *> * messages;

@property (nonatomic, copy) NSString * containerMsgId;

@end
