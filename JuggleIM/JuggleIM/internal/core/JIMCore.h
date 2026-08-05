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
    //Not connected
    JConnectionStatusInternalIdle = 0,
    //Connected
    JConnectionStatusInternalConnected = 1,
    //Connection disconnected (user actively disconnects or is kicked by the server; SDK will not reconnect)
    JConnectionStatusInternalDisconnected = 2,
    //Connecting
    JConnectionStatusInternalConnecting = 3,
    //Connection failed
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
@property (nonatomic, copy) NSString *session;
@property (nonatomic, assign) BOOL enableE2EE;
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
- (long long)getTimeDifference;
- (long long)getCurrentTime;

@end

NS_ASSUME_NONNULL_END
