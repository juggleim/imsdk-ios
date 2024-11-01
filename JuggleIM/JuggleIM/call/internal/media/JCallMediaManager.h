//
//  JCallMediaManager.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/31.
//

#import <Foundation/Foundation.h>
#import "JCallSession.h"

@interface JCallMediaManager : NSObject

+ (instancetype)shared;

- (void)initZegoEngineWith:(int)appId
                   appSign:(NSString *)appSign;

- (void)joinRoom:(id<JCallSession>)callSession
        complete:(void (^)(int, NSDictionary *))completeBlock;

@end

