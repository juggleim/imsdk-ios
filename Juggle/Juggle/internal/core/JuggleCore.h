//
//  JuggleCore.h
//  Juggle
//
//  Created by Nathan on 2023/12/1.
//

#import <Foundation/Foundation.h>
#import "JWebSocket.h"

NS_ASSUME_NONNULL_BEGIN

@interface JuggleCore : NSObject
@property (nonatomic, copy) NSString *appKey;
@property (nonatomic, copy) NSString *token;
@property (nonatomic, copy) NSString *userId;
@property (nonatomic, strong) JWebSocket *webSocket;
@property (nonatomic, strong) dispatch_queue_t sendQueue;
@property (nonatomic, strong) dispatch_queue_t receiveQueue;
@property (nonatomic, strong) dispatch_queue_t delegateQueue;
@end

NS_ASSUME_NONNULL_END
