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


@end
