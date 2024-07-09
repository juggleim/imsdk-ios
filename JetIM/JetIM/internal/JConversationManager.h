//
//  JConversationManager.h
//  JetIM
//
//  Created by Nathan on 2023/11/29.
//

#import <Foundation/Foundation.h>
#import "JConversationProtocol.h"
#import "JetIMCore.h"
#import "JMessageSendReceiveDelegate.h"
#import "JMessageManager.h"
NS_ASSUME_NONNULL_BEGIN

@interface JConversationManager : NSObject <JConversationProtocol, JMessageSendReceiveDelegate>
- (instancetype)initWithCore:(JetIMCore *)core messageManager:(JMessageManager *)messageManager;
- (void)syncConversations:(void(^)(void))completeBlock;
- (void)connectSuccess;
@end

NS_ASSUME_NONNULL_END
