//
//  JMessageManager.m
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

#import "JMessageManager.h"

@interface JMessageManager ()
@property (nonatomic, strong) JuggleCore *core;
@property (nonatomic, weak) id<JMessageDelegate> delegate;
@end

@implementation JMessageManager

- (instancetype)initWithCore:(JuggleCore *)core {
    JMessageManager *m = [[JMessageManager alloc] init];
    m.core = core;
    return m;
}

- (void)setDelegate:(id<JMessageDelegate>)delegate {
    _delegate = delegate;
}

//
//- (void)deleteMessageByClientId:(long)clientMsgNo {
//    <#code#>
//}
//
//- (void)deleteMessageByMessageId:(NSString *)messageId {
//    <#code#>
//}
//
//- (NSArray<JMessage *> *)getMessagesFrom:(JConversation *)conversation count:(int)count time:(long long)time direction:(JPullDirection)direction {
//    <#code#>
//}
//
//- (void)sendMessage:(JMessageContent *)content conversation:(JConversation *)conversation {
//    <#code#>
//}

@end
