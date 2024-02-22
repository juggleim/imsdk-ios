//
//  JConversationManager.m
//  JetIM
//
//  Created by Nathan on 2023/11/29.
//

#import "JConversationManager.h"

#define kConversationSyncCount 100

@interface JConversationManager ()
@property (nonatomic, strong) JetIMCore *core;
@property (nonatomic, weak) id<JConversationSyncDelegate> syncDelegate;
@end

@implementation JConversationManager

- (instancetype)initWithCore:(JetIMCore *)core {
    JConversationManager *m = [[JConversationManager alloc] init];
    m.core = core;
    return m;
}

- (void)syncConversations:(void (^)(void))completeBlock {
    [self.core.webSocket syncConversations:self.core.conversationSyncTime
                                     count:kConversationSyncCount
                                    userId:self.core.userId
                                   success:^(NSArray * _Nonnull conversations, NSArray * _Nonnull deletedConversations, BOOL isFinished) {
        long long updateTime = 0;
        if (conversations.lastObject) {
            JConcreteConversationInfo *last = conversations.lastObject;
            if (last.updateTime > 0) {
                updateTime = last.updateTime;
            }
            [self.core.dbManager insertConversations:conversations];
        }
        if (deletedConversations.lastObject) {
            JConcreteConversationInfo *last = deletedConversations.lastObject;
            if (last.updateTime > updateTime) {
                updateTime = last.updateTime;
            }
            [deletedConversations enumerateObjectsUsingBlock:^(JConcreteConversationInfo *  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                [self.core.dbManager deleteConversationInfoBy:obj.conversation];
            }];
        }
        if (updateTime > 0) {
            self.core.conversationSyncTime = updateTime;
        }
        if (!isFinished) {
            [self syncConversations:completeBlock];
        } else {
            dispatch_async(self.core.delegateQueue, ^{
                if ([self.syncDelegate respondsToSelector:@selector(conversationSyncDidComplete)]) {
                    [self.syncDelegate conversationSyncDidComplete];
                }
            });
            if (completeBlock) {
                completeBlock();
            }
        }
    } error:^(JErrorCodeInternal code) {
        NSLog(@"[JetIM] sync conversation fail, code is %lu", (unsigned long)code);
        if (completeBlock) {
            completeBlock();
        }
    }];
}

//- (void)clearUnreadCountByConversation:(JConversation *)conversation { 
//    <#code#>
//}

- (JConversationInfo *)getConversationInfo:(JConversation *)conversation {
    return [self.core.dbManager getConversationInfo:conversation];
}

- (NSArray<JConversationInfo *> *)getConversationInfoListWithTypes:(NSArray<NSNumber *> *)conversationTypes
                                                             count:(int)count
                                                         timestamp:(long long)ts
                                                         direction:(JPullDirection)direction {
    return [self.core.dbManager getConversationInfoListWithTypes:conversationTypes
                                                           count:count
                                                       timestamp:ts
                                                       direction:direction];
}

- (NSArray<JConversationInfo *> *)getConversationInfoListByCount:(int)count
                                                       timestamp:(long long)ts
                                                       direction:(JPullDirection)direction {
    NSArray *array = [[NSArray alloc] init];
    return [self getConversationInfoListWithTypes:array
                                            count:count
                                        timestamp:ts
                                        direction:direction];
}

- (void)deleteConversationInfoBy:(JConversation *)conversation {
    [self.core.dbManager deleteConversationInfoBy:conversation];
    [self.core.webSocket deleteConversationInfo:conversation
                                         userId:self.core.userId
                                        success:^{
        NSLog(@"[JetIM] delete conversation success");
    } error:^(JErrorCodeInternal code) {
        NSLog(@"[JetIM] delete conversation error, code is %lu", code);
    }];
}

- (NSArray<JConversationInfo *> *)getConversationInfoList {
    return [self.core.dbManager getConversationInfoList];
}
//
//- (int)getTotalUnreadCount { 
//    <#code#>
//}
//
//- (int)getUnreadCountByConversation:(JConversation *)conversation { 
//    <#code#>
//}

- (void)setDraft:(NSString *)draft inConversation:(JConversation *)conversation {
    [self.core.dbManager setDraft:draft inConversation:conversation];
}

- (void)clearDraftInConversation:(JConversation *)conversation {
    [self.core.dbManager clearDraftInConversation:conversation];
}

- (void)setSyncDelegate:(id<JConversationSyncDelegate>)delegate {
    _syncDelegate = delegate;
}

#pragma mark - internal


@end
