//
//  JConnectionManager.h
//  JetIM
//
//  Created by Nathan on 2023/11/29.
//

#import <Foundation/Foundation.h>
#import <JConnectionProtocol.h>
#import "JetIMCore.h"
#import "JConversationManager.h"
#import "JMessageManager.h"

NS_ASSUME_NONNULL_BEGIN

@interface JConnectionManager : NSObject <JConnectionProtocol>
- (instancetype)initWithCore:(JetIMCore *)core
         conversationManager:(JConversationManager *)conversationManager
              messageManager:(JMessageManager *)messageManager;
@end

NS_ASSUME_NONNULL_END
