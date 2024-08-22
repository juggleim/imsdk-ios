//
//  JHeartBeatManager.h
// JuggleIM
//
//  Created by Nathan on 2023/12/26.
//

#import <Foundation/Foundation.h>
#import "JWebSocket.h"

NS_ASSUME_NONNULL_BEGIN

@interface JHeartBeatManager : NSObject
- (instancetype)initWithWebSocket:(JWebSocket *)ws;
- (void)start;
- (void)stop;
- (void)updateLastMessageReceiveTime;
@end

NS_ASSUME_NONNULL_END
