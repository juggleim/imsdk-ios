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
@property (nonatomic, strong) NSArray <NSString *> *senderUserIds;
@property (nonatomic, strong) NSArray <NSString *> *contentTypes;
@property (nonatomic, strong) NSArray <JConversation *> *conversations;
@property (nonatomic, strong) NSArray <NSNumber *> *states;

@end
