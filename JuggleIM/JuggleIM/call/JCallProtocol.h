//
//  JCallProtocol.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/21.
//

#import "JCallSession.h"

@protocol JCallReceiveDelegate <NSObject>

- (void)callDidReceive:(JCallSession *)callSession;

@end

@protocol JCallProtocol <NSObject>

- (JCallSession *)startSingleCall:(NSString *)userId
                         delegate:(id<JCallSessionDelegate>)delegate;

- (void)addReceiveDelegate:(id<JCallReceiveDelegate>)receiveDelegate;

@end
