//
//  JLRUCache.m
//  JuggleIM
//
//  Created by Fei Li on 2025/4/29.
//

#import "JLRUCache.h"
#import "JLRUCacheNode.h"

@interface JLRUCache ()
@property (nonatomic, assign) NSInteger capacity;
@property (nonatomic, assign) NSInteger count;
@property (nonatomic, strong) NSMutableDictionary *cache;
@property (nonatomic, strong) JLRUCacheNode *head;
@property (nonatomic, strong) JLRUCacheNode *tail;
@end

@implementation JLRUCache
- (instancetype)initWithCapacity:(NSInteger)capacity {
    self = [super init];
    if (self) {
        _capacity = capacity;
        _cache = [NSMutableDictionary dictionary];
        _head = [[JLRUCacheNode alloc] init];
        _tail = [[JLRUCacheNode alloc] init];
        _head.next = _tail;
        _tail.prev = _head;
        _count = 0;
    }
    return self;
}

- (void)clearCache {
    @synchronized (self) {
        [self.cache removeAllObjects];
        
        self.head.next = self.tail;
        self.tail.prev = self.head;
        
        self.count = 0;
    }
}

- (id)get:(id)key {
    @synchronized (self) {
        JLRUCacheNode *node = _cache[key];
        if (!node) {
            return nil;
        }
        [self moveNodeToHead:node];
        return node.value;
    }
}

- (void)put:(id)key value:(id)value {
    @synchronized (self) {
        JLRUCacheNode *node = _cache[key];
        if (node) {
            node.value = value;
            [self moveNodeToHead:node];
        } else {
            node = [[JLRUCacheNode alloc] init];
            node.key = key;
            node.value = value;
            _cache[key] = node;
            [self addNodeToHead:node];
            _count++;
            if (_count > _capacity) {
                JLRUCacheNode *tailNode = [self removeTailNode];
                [_cache removeObjectForKey:tailNode.key];
                _count--;
            }
        }
    }
}

#pragma mark - Private methods
- (void)moveNodeToHead:(JLRUCacheNode *)node {
    [self removeNode:node];
    [self addNodeToHead:node];
}

- (void)removeNode:(JLRUCacheNode *)node {
    node.prev.next = node.next;
    node.next.prev = node.prev;
}

- (void)addNodeToHead:(JLRUCacheNode *)node {
    node.prev = self.head;
    node.next = self.head.next;
    self.head.next.prev = node;
    self.head.next = node;
}

- (JLRUCacheNode *)removeTailNode {
    JLRUCacheNode *node = self.tail.prev;
    [self removeNode:node];
    return node;
}

@end
