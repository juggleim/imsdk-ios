//
//  JMessageSendReceiveDelegate.h
//  JetIM
//
//  Created by Nathan on 2024/2/27.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol JMessageSendReceiveDelegate <NSObject>
- (void)messageDidSave:(JConcreteMessage *)message;
- (void)messageDidSend:(JConcreteMessage *)message;
- (void)messageDidReceive:(JConcreteMessage *)message;
@end

NS_ASSUME_NONNULL_END
