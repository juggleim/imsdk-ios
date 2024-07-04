//
//  JConversation.m
//  JetIM
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
        return c.conversationType == self.conversationType && [c.conversationId isEqualToString:self.conversationId];
    }
    return NO;
}
@end
