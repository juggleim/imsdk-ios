//
//  JConversationDB.h
//  JetIM
//
//  Created by Nathan on 2023/12/8.
//

#import <Foundation/Foundation.h>
#import "JDBHelper.h"
#import "JConcreteConversationInfo.h"
#import "JMessageDB.h"

NS_ASSUME_NONNULL_BEGIN

@interface JConversationDB : NSObject
@property (nonatomic, strong) JMessageDB *messageDB;

- (void)createTables;
- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper;
- (void)insertConversations:(NSArray<JConcreteConversationInfo *> *)conversations;
- (nullable JConcreteConversationInfo *)getConversationInfo:(JConversation *)conversation;
- (NSArray<JConcreteConversationInfo *> *)getConversationInfoList;
@end

NS_ASSUME_NONNULL_END
