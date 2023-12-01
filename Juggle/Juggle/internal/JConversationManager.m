//
//  JConversationManager.m
//  Juggle
//
//  Created by Nathan on 2023/11/29.
//

#import "JConversationManager.h"

@interface JConversationManager ()
@property (nonatomic, strong) JuggleCore *core;
@end

@implementation JConversationManager

- (instancetype)initWithCore:(JuggleCore *)core {
    JConversationManager *m = [[JConversationManager alloc] init];
    m.core = core;
    return m;
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

@end
