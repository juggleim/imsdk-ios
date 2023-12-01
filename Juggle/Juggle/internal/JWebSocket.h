//
//  JWebSocket.h
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

//TODO: remove this class


#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class JWebSocket;

@protocol JWebSocketDelegate <NSObject>
- (void)webSocket:(JWebSocket *)webSocket didReceiveMessageWithData:(NSData *)data;
@end

@interface JWebSocket : NSObject

- (instancetype)initWithURLRequest:(NSURLRequest *)request;
- (void)open;
- (void)close;
- (void)sendData:(NSData *)data
      completion:(void (^)(NSError *_Nullable error))completion;
- (void)setDelegate:(id<JWebSocketDelegate>)delegate;

@end



NS_ASSUME_NONNULL_END
