//
//  JConnectionManager.h
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

#import <Foundation/Foundation.h>
#import <JConnectionProtocol.h>
#import "JuggleCore.h"
#import "JConversationManager.h"
#import "JMessageManager.h"

NS_ASSUME_NONNULL_BEGIN

@interface JConnectionManager : NSObject <JConnectionProtocol>
- (instancetype)initWithCore:(JuggleCore *)core
         conversationManager:(JConversationManager *)conversationManager
              messageManager:(JMessageManager *)messageManager;
@end

NS_ASSUME_NONNULL_END
