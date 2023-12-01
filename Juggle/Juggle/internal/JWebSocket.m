//
//  JWebSocket.m
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

#import "JWebSocket.h"
#import "SRWebSocket.h"


//TODO: remove this class
@interface JWebSocket ()
@property(nonatomic, weak) id<JWebSocketDelegate> delegate;
@property(nonatomic, strong) NSURLRequest *request;
@end

@implementation JWebSocket

- (instancetype)initWithURLRequest:(NSURLRequest *)request {
    JWebSocket *w = [[JWebSocket alloc] init];
    w.request = request;
    return w;
}


- (void)open {
    
}

- (void)sendData:(NSData *)data completion:(void (^)(NSError * _Nullable))completion {
    
}

@end
