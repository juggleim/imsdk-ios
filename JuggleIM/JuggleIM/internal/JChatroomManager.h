//
//  JChatroomManager.h
//  JuggleIM
//
//  Created by Fei Li on 2024/8/13.
//

#import <Foundation/Foundation.h>
#import "JChatroomProtocol.h"
#import "JIMCore.h"
#import "JCachedChatroomStatus.h"

NS_ASSUME_NONNULL_BEGIN

@interface JChatroomManager : NSObject <JChatroomProtocol>
- (instancetype)initWithCore:(JIMCore *)core;
- (void)connectSuccess;
- (void)userDisconnect;

- (long long)getSyncTimeForChatroom:(NSString *)chatroomId;
- (void)setSyncTime:(long long)syncTime
        forChatroom:(NSString *)chatroomId;

- (void)setAttrSyncTime:(long long)syncTime
            forChatroom:(NSString *)chatroomId;
@end

NS_ASSUME_NONNULL_END
