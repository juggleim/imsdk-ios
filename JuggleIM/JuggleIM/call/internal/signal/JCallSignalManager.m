//
//  JCallSignalManager.m
//  JuggleIM
//
//  Created by Fei Li on 2024/10/21.
//

#import "JCallSignalManager.h"

@interface JCallSignalManager ()
@property (nonatomic, strong) JIMCore *core;
@end

@implementation JCallSignalManager

- (instancetype)initWithCore:(JIMCore *)core {
    if (self = [super init]) {
        self.core = core;
    }
    return self;
}

//- (JCallSession *)startSingleCall:(NSString *)userId
//                          success:(void (^)(JCallSession * _Nonnull))successBlock
//                            error:(void (^)(JErrorCode))errorBlock {
////    [self.core.webSocket ]
//}

@end
