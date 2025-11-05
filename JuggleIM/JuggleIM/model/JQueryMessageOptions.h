//
//  JQueryMessageOptions.h
//  JuggleIM
//
//  Created by Fei Li on 2024/8/22.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JConversation.h>
#import <JuggleIM/JMessage.h>

@interface JQueryMessageOptions : NSObject
//指定搜索内容
@property (nonatomic, copy) NSString *searchContent;
//指定消息发送者 id
@property (nonatomic, copy) NSArray <NSString *> *senderUserIds;
//指定消息类型
@property (nonatomic, copy) NSArray <NSString *> *contentTypes;
//指定会话
@property (nonatomic, copy) NSArray <JConversation *> *conversations;
//指定消息状态
@property (nonatomic, copy) NSArray <NSNumber *> *states;
//指定会话类型
@property (nonatomic, copy) NSArray <NSNumber *> *conversationTypes;

@end
