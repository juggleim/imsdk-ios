//
//  JLRUCache.h
//  JuggleIM
//
//  Created by Fei Li on 2025/4/29.
//

#import <Foundation/Foundation.h>

@interface JLRUCache : NSObject
- (instancetype)initWithCapacity:(NSInteger)capacity;
- (id)get:(id)key;
- (void)put:(id)key value:(id)value;
- (void)clearCache;
@end

