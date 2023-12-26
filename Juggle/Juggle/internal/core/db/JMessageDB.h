//
//  JMessageDB.h
//  Juggle
//
//  Created by Nathan on 2023/12/8.
//

#import <Foundation/Foundation.h>
#import "JDBHelper.h"
#import "JConcreteMessage.h"

NS_ASSUME_NONNULL_BEGIN

@interface JMessageDB : NSObject

- (void)createTables;

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper;

- (nullable JConcreteMessage *)getMessageWithMessageId:(NSString *)messageId;

- (void)insertMessage:(JMessage *)message
                 inDb:(JFMDatabase *)db;
@end

NS_ASSUME_NONNULL_END
