//
//  JConnectionProtocol.h
//  Juggle
//
//  Created by Nathan on 2023/11/27.
//
#import "JuggleConst.h"

@protocol JConnectionDelegate <NSObject>
- (void)connectionStatusDidChange:(JConnectionStatus)status;
@end

@protocol JConnectionProtocol <NSObject>
- (void)connectWithToken:(NSString *)token;

- (void)disconnect:(BOOL)receivePush;

- (void)setDelegate:(id<JConnectionDelegate>) delegate;

@end
