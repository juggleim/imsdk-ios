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
            // Each state handles this itself.
            break;
            
        case JConnEventUserDisconnect:
            // Each state handles this itself.
            break;
            
        case JConnEventConnectDone:
            // do nothing
            // Handled by the connecting state.
            // Ignored in other states.
            break;
            
        case JConnEventNetworkAvailable:
            // do nothing
            // Handled by the waiting and connecting states.
            // Ignored in other states; the connected state automatically triggers websocketFail.
            break;
            
        default:
            break;
    }
    return result;
}

@end
