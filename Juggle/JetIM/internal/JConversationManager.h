//
//  JConversationManager.h
//  JetIM
//
//  Created by Nathan on 2023/11/29.
//

#import <Foundation/Foundation.h>
#import "JConversationProtocol.h"
#import "JetIMCore.h"

NS_ASSUME_NONNULL_BEGIN

@interface JConversationManager : NSObject <JConversationProtocol>
- (instancetype)initWithCore:(JetIMCore *)core;
- (void)syncConversations:(void(^)(void))completeBlock;
@end

NS_ASSUME_NONNULL_END
