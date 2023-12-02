//
//  JConnectionManager.m
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

#import "JConnectionManager.h"
#import "JWebSocket.h"

@interface JConnectionManager () <JWebSocketConnectDelegate>
@property (nonatomic, strong) JuggleCore *core;
@property (nonatomic, weak) id<JConnectionDelegate> delegate;
@end

@implementation JConnectionManager

- (instancetype)initWithCore:(JuggleCore *)core {
    JConnectionManager *m = [[JConnectionManager alloc] init];
    [core.webSocket setConnectDelegate:m];
    m.core = core;
    return m;
}

- (void)connectWithToken:(NSString *)token {
    self.core.token = token;
    
    //TODO: navi
    
    JConnectInfo *info = [[JConnectInfo alloc] init];
    info.appKey = self.core.appKey;
    info.token = token;
    [self.core.webSocket connect:info];
    
}

- (void)disconnect:(BOOL)receivePush {
    
}

- (void)setDelegate:(id<JConnectionDelegate>)delegate {
    _delegate = delegate;
}

- (void)connectCompleteWithCode:(JErrorCode)error
                         userId:(NSString *)userId {
    NSLog(@"[Juggle] connect complete, error code is %d", error);
}

@end
