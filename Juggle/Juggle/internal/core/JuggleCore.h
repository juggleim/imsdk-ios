//
//  JuggleCore.h
//  Juggle
//
//  Created by Nathan on 2023/12/1.
//

#import <Foundation/Foundation.h>
#import "JWebSocket.h"

NS_ASSUME_NONNULL_BEGIN

@interface JuggleCore : NSObject
@property (nonatomic, copy) NSString *appKey;
@property (nonatomic, copy) NSString *token;
@property (nonatomic, strong) JWebSocket *webSocket;
@end

NS_ASSUME_NONNULL_END
