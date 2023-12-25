//
//  JConversationDB.h
//  Juggle
//
//  Created by Nathan on 2023/12/8.
//

#import <Foundation/Foundation.h>
#import "JDBHelper.h"
#import "JConcreteConversationInfo.h"

NS_ASSUME_NONNULL_BEGIN

@interface JConversationDB : NSObject

- (void)createTables;

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper;

- (void)insertConversations:(NSArray<JConcreteConversationInfo *> *)conversations;


@end

NS_ASSUME_NONNULL_END
