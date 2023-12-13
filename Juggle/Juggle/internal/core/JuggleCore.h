//
//  JuggleCore.h
//  Juggle
//
//  Created by Nathan on 2023/12/1.
//

#import <Foundation/Foundation.h>
#import "JWebSocket.h"
#import "JDBManager.h"

typedef NS_ENUM(NSUInteger, JConnectionStatusInternal) {
    //未连接
    JConnectionStatusInternalIdle = 0,
    //已连接
    JConnectionStatusInternalConnected = 1,
    //连接断开（用户主动断开）
    JConnectionStatusInternalDisconnected = 2,
    //连接中
    JConnectionStatusInternalConnecting = 3,
    //连接 token 错误
    JConnectionStatusInternalTokenIncorrect = 4,
    //
    JConnectionStatusInternalFailure
};

typedef NS_ENUM(NSUInteger, JDBStatus) {
    JDBStatusClosed,
    JDBStatusOpen
};

NS_ASSUME_NONNULL_BEGIN

@interface JuggleCore : NSObject
@property (nonatomic, copy) NSString *appKey;
@property (nonatomic, copy) NSString *token;
@property (nonatomic, copy) NSString *userId;
@property (nonatomic, strong) JWebSocket *webSocket;
@property (nonatomic, strong) dispatch_queue_t sendQueue;
@property (nonatomic, strong) dispatch_queue_t receiveQueue;
@property (nonatomic, strong) dispatch_queue_t delegateQueue;
@property (nonatomic, strong) JDBManager *dbManager;
@property (nonatomic, assign) JConnectionStatusInternal connectionStatus;
@property (nonatomic, assign) JDBStatus dbStatus;
@end

NS_ASSUME_NONNULL_END
