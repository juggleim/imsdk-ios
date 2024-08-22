//
//  JWebSocketCommandManager.m
// JuggleIM
//
//  Created by Nathan on 2024/5/31.
//

#import "JWebSocketCommandManager.h"

#define jCommandTimeOutInterval 5
#define jCommandDetectInterval 5

@interface JBlockTimeObj : NSObject
@property (nonatomic, strong) JBlockObj *blockObj;
@property (nonatomic, assign) long long timestamp;
@end

@implementation JBlockTimeObj
@end

@interface JWebSocketCommandManager ()
@property (nonatomic, weak) id<JCommandTimeOutDelegate> delegate;
@property (nonatomic, strong) NSTimer *detectTimer;
@property (nonatomic, strong) NSMutableDictionary<NSNumber *, JBlockTimeObj *> *cmdBlockDic;
@end

@implementation JWebSocketCommandManager

- (instancetype)initWithDelegate:(id<JCommandTimeOutDelegate>)delegate {
    if (self = [super init]) {
        self.delegate = delegate;
        self.cmdBlockDic = [NSMutableDictionary dictionary];
    }
    return self;
}

- (void)start {
    [self stop];
    dispatch_async(dispatch_get_main_queue(), ^{
        self.detectTimer = [NSTimer scheduledTimerWithTimeInterval:jCommandDetectInterval
                                                            target:self
                                                          selector:@selector(detect)
                                                          userInfo:nil
                                                           repeats:YES];
    });
}

- (void)stop {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.detectTimer) {
            [self.detectTimer invalidate];
            self.detectTimer = nil;
        }
    });
}

- (void)setBlockObject:(JBlockObj *)obj forKey:(NSNumber *)index {
    @synchronized (self) {
        JBlockTimeObj *timeObj = [[JBlockTimeObj alloc] init];
        timeObj.blockObj = obj;
        timeObj.timestamp = [[NSDate date] timeIntervalSince1970];
        [self.cmdBlockDic setObject:timeObj forKey:index];
    }
}

- (JBlockObj *)removeBlockObjectForKey:(NSNumber *)index {
    @synchronized (self) {
        JBlockTimeObj *timeObj = [self.cmdBlockDic objectForKey:index];
        [self.cmdBlockDic removeObjectForKey:index];
        return timeObj.blockObj;
    }
}

- (NSArray<JBlockObj *> *)clearBlockObject {
    @synchronized (self) {
        NSMutableArray<JBlockObj *> *results = [[NSMutableArray alloc] init];
        for (JBlockTimeObj *timeObj in self.cmdBlockDic.allValues) {
            [results addObject:timeObj.blockObj];
        }
        [self.cmdBlockDic removeAllObjects];
        return [results copy];
    }
}

#pragma mark - internal
- (void)detect {
    NSArray <JBlockObj *> *timeOutMessages = [self doCommandDetection];
    if (timeOutMessages.count > 0) {
        [self afterCommandDetection:timeOutMessages];
    }
}

- (NSArray <JBlockObj *> *)doCommandDetection {
    NSMutableArray<NSNumber *> *timeOutKeys = [NSMutableArray array];
    NSMutableArray<JBlockObj *> *results = [NSMutableArray array];
    long long now = [[NSDate date] timeIntervalSince1970];
    @synchronized (self) {
        for (NSNumber *key in self.cmdBlockDic.allKeys) {
            JBlockTimeObj *timeObj = self.cmdBlockDic[key];
            long long delta = now - timeObj.timestamp;
            if (delta > jCommandTimeOutInterval) {
                [timeOutKeys addObject:key];
                [results addObject:timeObj.blockObj];
            }
        }
        for (NSNumber *key in timeOutKeys) {
            [self.cmdBlockDic removeObjectForKey:key];
        }
        return [results copy];
    }
}

- (void)afterCommandDetection:(NSArray <JBlockObj *> *)timeOutMessages {
    [self.delegate commandDidTimeOut:timeOutMessages];
}

@end
