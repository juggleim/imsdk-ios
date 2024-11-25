//
//  JConnEventUtil.m
//  JuggleIM
//
//  Created by Fei Li on 2024/11/12.
//

#import "JConnEventUtil.h"
#import "JConnEvent.h"

@implementation JConnEventUtil

static NSString *JConnEventNameMap[] = {
    [JConnEventUserConnect] = @"user connect",
    [JConnEventUserDisconnect] = @"user disconnect",
    
    [JConnEventConnectDone] = @"connect done",
    [JConnEventConnectFailure] = @"connect failure",
    
    [JConnEventReconnectTimerFire] = @"reconnect timer fire",
    [JConnEventNetworkAvailable] = @"network available",
    [JConnEventEnterForground] = @"enter foreground",
    [JConnEventEnterBackground] = @"enter background",
    [JConnEventWebsocketFail] = @"websocket fail",
    [JConnEventRemoteDisconnect] = @"remote disconnect",
    
    [20] = @"unknown"
};

+ (NSString *)nameOfEvent:(NSInteger)event {
    return JConnEventNameMap[event];
}

@end
