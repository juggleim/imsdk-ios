//
//  JConversationManager.m
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

#import "JConversationManager.h"

#define kConversationSyncCount 100

@interface JConversationManager ()
@property (nonatomic, strong) JuggleCore *core;
@end

@implementation JConversationManager

- (instancetype)initWithCore:(JuggleCore *)core {
    JConversationManager *m = [[JConversationManager alloc] init];
    m.core = core;
    return m;
}

- (void)syncConversations {
//    dispatch_async(self.core.sendQueue, ^{
        [self.core.webSocket syncConversations:self.core.conversationSyncTime
                                         count:kConversationSyncCount
                                        userId:self.core.userId
                                       success:^(NSArray * _Nonnull conversations, BOOL isFinished) {
            if (conversations.lastObject) {
                JConcreteConversationInfo *last = conversations.lastObject;
                if (last.updateTime > 0) {
                    self.core.conversationSyncTime = last.updateTime;
                }
                [self.core.dbManager insertConversations:conversations];
            }
            if (!isFinished) {
                [self syncConversations];
            }
        } error:^(JErrorCode code) {
            
        }];
//    });
}

//- (void)clearUnreadCountByConversation:(JConversation *)conversation { 
//    <#code#>
//}
//
//- (JConversationInfo *)getConversationInfo:(JConversation *)conversation { 
//    <#code#>
//}
//
//- (NSArray<JConversationInfo *> *)getConversationInfoList { 
//    <#code#>
//}
//
//- (int)getTotalUnreadCount { 
//    <#code#>
//}
//
//- (int)getUnreadCountByConversation:(JConversation *)conversation { 
//    <#code#>
//}

#pragma mark - internal


@end
