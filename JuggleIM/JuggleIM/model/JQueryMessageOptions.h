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

@property (nonatomic, copy) NSString *searchContent;
@property (nonatomic, copy) NSArray <NSString *> *senderUserIds;
@property (nonatomic, copy) NSArray <NSString *> *contentTypes;
@property (nonatomic, copy) NSArray <JConversation *> *conversations;
@property (nonatomic, copy) NSArray <NSNumber *> *states;
@property (nonatomic, copy) NSArray <NSNumber *> *conversationTypes;

@end
