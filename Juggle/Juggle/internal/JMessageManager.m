//
//  JMessageManager.m
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

#import "JMessageManager.h"
#import "JuggleConst.h"
#import "JTextMessage.h"
#import "JImageMessage.h"

@interface JMessageManager ()
@property (nonatomic, strong) JuggleCore *core;
@property (nonatomic, weak) id<JMessageDelegate> delegate;
@end

@implementation JMessageManager

- (instancetype)initWithCore:(JuggleCore *)core {
    JMessageManager *m = [[JMessageManager alloc] init];
    m.core = core;
    [m.core.webSocket registerMessageType:[JTextMessage class]];
    [m.core.webSocket registerMessageType:[JImageMessage class]];
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

- (void)sendMessage:(JMessageContent *)content
     inConversation:(JConversation *)conversation {
    [self.core.webSocket sendIMMessage:content
                        inConversation:conversation];
}

- (void)registerMessageType:(Class)messageClass {
    [self.core.webSocket registerMessageType:messageClass];
}

#pragma mark - internal
- (void)getRemoteMessagesFrom:(JConversation *)conversation
                    startTime:(long long)startTime
                        count:(int)count
                    direction:(JPullDirection)direction
                      success:(void (^)(NSArray *messages, BOOL isRemaining))successBlock
                        error:(void (^)(JErrorCode code))errorBlock {
    [self.core.webSocket queryHisMsgsFrom:conversation
                                startTime:startTime
                                    count:count
                                direction:direction
                                  success:successBlock
                                    error:errorBlock];
}

@end
