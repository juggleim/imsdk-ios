//
//  JWebSocket.h
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

#import <Foundation/Foundation.h>
#import "JuggleConst.h"

NS_ASSUME_NONNULL_BEGIN

@protocol JWebSocketConnectDelegate <NSObject>
- (void)connectCompleteWithCode:(JErrorCode)error
                         userId:(NSString *)userId;
@end

@interface JConnectInfo : NSObject
@property (nonatomic, copy) NSString *appKey;
@property (nonatomic, copy) NSString *token;
@end

@interface JWebSocket : NSObject

- (void)connect:(JConnectInfo *)info;
- (void)setConnectDelegate:(id<JWebSocketConnectDelegate>)delegate;

- (void)sendMessage:(NSData *)message;

@end



NS_ASSUME_NONNULL_END
