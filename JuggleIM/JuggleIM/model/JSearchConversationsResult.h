//
//  JSearchConversationsResult.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/28.
//

#import <Foundation/Foundation.h>
#import "JConversationInfo.h"

@interface JSearchConversationsResult : NSObject
@property (nonatomic, strong) JConversationInfo *conversationInfo;
@property (nonatomic, assign) int matchedCount;
@end
