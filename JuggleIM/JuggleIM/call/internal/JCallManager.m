//
//  JCallManager.m
//  JuggleCall
//
//  Created by Fei Li on 2024/10/10.
//

#import "JCallManager.h"
#import "JCallSignalManager.h"
#import "JCallMediaEngine.h"

@interface JCallManager ()
@property (nonatomic, strong) JIMCore *core;
@property (nonatomic, strong) JCallSignalManager *signalManager;
@property (nonatomic, strong) JCallMediaEngine *mediaEngine;
@end

@implementation JCallManager
- (instancetype)initWithCore:(JIMCore *)core {
    if (self = [super init]) {
        self.core = core;
    }
    return self;
}

- (JCallSignalManager *)signalManager {
    if (!_signalManager) {
        _signalManager = [[JCallSignalManager alloc] initWithCore:self.core];
    }
    return _signalManager;
}
@end
