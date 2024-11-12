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
    [JConnEventReconnectTimerFire] = @"reconnect timer fire",
    [JConnEventNetworkChange] = @"network change",
    [JConnEventEnterForground] = @"enter foreground",
    [JConnEventEnterBackground] = @"enter background",
    [JConnEventWebsocketFail] = @"websocket fail",
    
    [100] = @"unknown"
};

+ (NSString *)nameOfEvent:(NSInteger)event {
    return JConnEventNameMap[event];
}

@end
