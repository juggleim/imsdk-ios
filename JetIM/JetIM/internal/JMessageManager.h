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
- (void)connectSuccess;

@property (nonatomic, weak) id<JMessageSendReceiveDelegate> sendReceiveDelegate;
@end

NS_ASSUME_NONNULL_END
