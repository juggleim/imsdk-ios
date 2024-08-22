//
//  JMessageManager.h
// JuggleIM
//
//  Created by Nathan on 2023/11/29.
//

#import <Foundation/Foundation.h>
#import "JMessageProtocol.h"
#import "JIMCore.h"
#import "JMessageSendReceiveDelegate.h"

NS_ASSUME_NONNULL_BEGIN

@interface JMessageManager : NSObject <JMessageProtocol>
- (instancetype)initWithCore:(JIMCore *)core;
- (void)syncMessages;
- (void)updateSendSyncTime:(long long)timestamp;
- (void)connectSuccess;

@property (nonatomic, weak) id<JMessageSendReceiveDelegate> sendReceiveDelegate;

@end

NS_ASSUME_NONNULL_END
