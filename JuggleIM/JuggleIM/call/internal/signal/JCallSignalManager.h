//
//  JCallSignalManager.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/21.
//

#import <Foundation/Foundation.h>
#import "JCallSession.h"
#import "JuggleIMConst.h"
#import "JIMCore.h"

NS_ASSUME_NONNULL_BEGIN

@interface JCallSignalManager : NSObject

- (instancetype)initWithCore:(JIMCore *)core;

- (JCallSession *)startSingleCall:(NSString *)userId
                          success:(void (^)(JCallSession *callSession))successBlock
                            error:(void (^)(JErrorCode code))errorBlock;

@end

NS_ASSUME_NONNULL_END
