//
//  JWebSocketCommandManager.h
//  JetIM
//
//  Created by Nathan on 2024/5/31.
//

#import <Foundation/Foundation.h>
#import "JBlockObj.h"

NS_ASSUME_NONNULL_BEGIN

@protocol JCommandTimeOutDelegate <NSObject>

- (void)commandDidTimeOut:(NSArray<JBlockObj *> *)objs;

@end

@interface JWebSocketCommandManager : NSObject

- (instancetype)initWithDelegate:(id<JCommandTimeOutDelegate>)delegate;

- (void)start;

- (void)stop;

- (void)setBlockObject:(JBlockObj *)obj
                forKey:(NSNumber *)index;

- (JBlockObj *)removeBlockObjectForKey:(NSNumber *)index;

- (NSArray <JBlockObj *> *)clearBlockObject;


@end

NS_ASSUME_NONNULL_END
