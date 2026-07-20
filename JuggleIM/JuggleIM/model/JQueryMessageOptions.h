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
//Specified search content
@property (nonatomic, copy) NSString *searchContent;
//Specified message sender id
@property (nonatomic, copy) NSArray <NSString *> *senderUserIds;
//Specified message type
@property (nonatomic, copy) NSArray <NSString *> *contentTypes;
//Specified conversation
@property (nonatomic, copy) NSArray <JConversation *> *conversations;
//Specified message status
@property (nonatomic, copy) NSArray <NSNumber *> *states;
//Specified conversation type
@property (nonatomic, copy) NSArray <NSNumber *> *conversationTypes;

@end
