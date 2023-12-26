//
//  JMessageDB.h
//  Juggle
//
//  Created by Nathan on 2023/12/8.
//

#import <Foundation/Foundation.h>
#import "JDBHelper.h"

NS_ASSUME_NONNULL_BEGIN

@interface JMessageDB : NSObject

- (void)createTables;

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper;

- (nullable JMessage *)getMessageWithMessageId:(NSString *)messageId;
@end

NS_ASSUME_NONNULL_END
