//
//  JMessageManager.h
//  JetIM
//
//  Created by Nathan on 2023/11/29.
//

#import <Foundation/Foundation.h>
#import "JMessageProtocol.h"
#import "JetIMCore.h"
#import "JMessageSendReceiveDelegate.h"

NS_ASSUME_NONNULL_BEGIN

@interface JMessageManager : NSObject <JMessageProtocol>
- (instancetype)initWithCore:(JetIMCore *)core;
- (void)syncMessages;

@property (nonatomic, weak) id<JMessageSendReceiveDelegate> sendReceiveDelegate;

- (void)updateSendSyncTime:(long long)timestamp;

@end

NS_ASSUME_NONNULL_END
