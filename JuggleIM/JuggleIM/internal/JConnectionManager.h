//
//  JConnectionManager.h
// JuggleIM
//
//  Created by Nathan on 2023/11/29.
//

#import <Foundation/Foundation.h>
#import <JConnectionProtocol.h>
#import "JIMCore.h"
#import "JConversationManager.h"
#import "JMessageManager.h"
#import "JChatroomManager.h"
#import "JCallManager.h"

NS_ASSUME_NONNULL_BEGIN

@interface JConnectionManager : NSObject <JConnectionProtocol>
- (instancetype)initWithCore:(JIMCore *)core
         conversationManager:(JConversationManager *)conversationManager
              messageManager:(JMessageManager *)messageManager
             chatroomManager:(JChatroomManager *)chatroomManager
                 callManager:(JCallManager *)callManager;

@end

NS_ASSUME_NONNULL_END
