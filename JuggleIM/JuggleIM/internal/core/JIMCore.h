//
//  JIMCore.h
// JuggleIM
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
    //连接断开（用户主动断开或者被服务端踢掉，SDK 不会进行重连）
    JConnectionStatusInternalDisconnected = 2,
    //连接中
    JConnectionStatusInternalConnecting = 3,
    //连接失败
    JConnectionStatusInternalFailure,
    //
    JConnectionStatusInternalWaitingForConnecting
};

NS_ASSUME_NONNULL_BEGIN

@interface JIMCore : NSObject
@property (nonatomic, copy) NSArray <NSString *> *naviUrls;
@property (nonatomic, copy) NSArray *servers;
@property (nonatomic, copy) NSString *appKey;
@property (nonatomic, copy) NSString *token;
@property (nonatomic, copy) NSString *userId;
@property (nonatomic, strong) JWebSocket *webSocket;
@property (nonatomic, strong) dispatch_queue_t sendQueue;
@property (nonatomic, strong) dispatch_queue_t receiveQueue;
@property (nonatomic, strong) dispatch_queue_t delegateQueue;
@property (nonatomic, strong) JDBManager *dbManager;
//send queue
@property (nonatomic, assign) JConnectionStatusInternal connectionStatus;
//receive queue
@property (nonatomic, assign) long long conversationSyncTime;
@property (nonatomic, assign) long long messageSendSyncTime;
@property (nonatomic, assign) long long messageReceiveSyncTime;

- (void)getSyncTimeFromDB;

@end

NS_ASSUME_NONNULL_END
