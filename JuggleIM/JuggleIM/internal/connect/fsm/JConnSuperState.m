//
//  JConnSuperState.m
//  JuggleIM
//
//  Created by Fei Li on 2024/11/12.
//

#import "JConnSuperState.h"
#import "JConnEvent.h"

@implementation JConnSuperState

- (BOOL)event:(NSInteger)event userInfo:(NSDictionary *)userInfo {
    BOOL result = YES;
    switch (event) {
        case JConnEventUserConnect:
            // 各状态自行处理
            break;
            
        case JConnEventUserDisconnect:
            // 各状态自行处理
            break;
            
        case JConnEventConnectDone:
            // do nothing
            // connecting 状态处理
            // 其它状态下忽略
            break;
            
        default:
            break;
    }
    return result;
}

@end
