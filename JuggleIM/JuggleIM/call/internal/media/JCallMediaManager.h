//
//  JCallMediaManager.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/31.
//

#import <Foundation/Foundation.h>
#import "JCallSessionImpl.h"

@interface JCallMediaManager : NSObject

+ (instancetype)shared;

- (void)initZegoEngineWith:(int)appId
                   appSign:(NSString *)appSign;

- (void)joinRoom:(JCallSessionImpl *)callSession
        complete:(void (^)(int, NSDictionary *))completeBlock;

@end

