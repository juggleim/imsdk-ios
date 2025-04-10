//
//  JCallMediaZegoEngine.h
//  JZegoCall
//
//  Created by Fei Li on 2024/10/31.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JCallMediaEngineProtocol.h>
#import <ZegoExpressEngine/ZegoExpressEngine.h>

NS_ASSUME_NONNULL_BEGIN

@interface JCallMediaZegoEngine : NSObject <JCallMediaEngineProtocol>

- (void)createEngineWith:(NSNumber *)appId appSign:(NSString *)appSign;

+ (void)setEventHandler:(id<ZegoEventHandler>)handler;

@end

NS_ASSUME_NONNULL_END
