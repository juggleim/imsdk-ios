//
//  JConversation.m
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import "JConversation.h"

@implementation JConversation

- (instancetype)initWithConversationType:(JConversationType)type conversationId:(NSString *)conversationId {
    if (self = [super init]) {
        self.conversationType = type;
        self.conversationId = conversationId;
    }
    return self;
}

- (BOOL)isEqual:(id)object {
    if (self == object) {
        return YES;
    }
    if ([object isKindOfClass:[self class]]) {
        JConversation *c = (JConversation *)object;
        return c.conversationType == self.conversationType && [c.conversationId isEqualToString:self.conversationId] && [c.subChannel isEqualToString:self.subChannel];
    }
    return NO;
}

- (NSUInteger)hash {
    NSUInteger hash = self.conversationType;
    hash = hash * 31u + self.conversationId.hash;
    hash = hash * 31u + self.subChannel.hash;
    return hash;
}

- (id)copyWithZone:(NSZone *)zone {
    JConversation *conversation = [[[self class] allocWithZone:zone] initWithConversationType:self.conversationType
                                                                               conversationId:self.conversationId];
    conversation.subChannel = self.subChannel;
    return conversation;
}

- (NSString *)subChannel {
    if (!_subChannel) {
        _subChannel = @"";
    }
    return _subChannel;
}

@end
