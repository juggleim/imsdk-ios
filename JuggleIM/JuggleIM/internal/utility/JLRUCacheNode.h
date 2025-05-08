//
//  JLRUCacheNode.h
//  JuggleIM
//
//  Created by Fei Li on 2025/4/29.
//

#import <Foundation/Foundation.h>

@interface JLRUCacheNode : NSObject
@property (nonatomic, strong) id key;
@property (nonatomic, strong) id value;
@property (nonatomic, weak) JLRUCacheNode *prev;
@property (nonatomic, weak) JLRUCacheNode *next;
@end

