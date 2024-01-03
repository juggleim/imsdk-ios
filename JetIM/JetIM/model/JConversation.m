//
//  JConversation.m
//  JetIM
//
//  Created by Nathan on 2023/11/27.
//

#import "JConversation.h"

@implementation JConversation

- (instancetype)initWithConversationType:(JConversationType)type conversationId:(NSString *)conversationId {
    JConversation *c = [[JConversation alloc] init];
    c.conversationType = type;
    c.conversationId = conversationId;
    return c;
}
@end
