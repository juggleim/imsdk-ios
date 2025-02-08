//
//  JReactionDB.h
//  JuggleIM
//
//  Created by Fei Li on 2025/2/7.
//

#import <Foundation/Foundation.h>
#import "JDBHelper.h"
#import "JMessageReaction.h"

NS_ASSUME_NONNULL_BEGIN

@interface JReactionDB : NSObject

- (void)createTables;
- (void)updateTables;
- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper;

- (NSArray <JMessageReaction *> *)getMessageReactions:(NSArray <NSString *> *)messageIds;
- (void)setMessageReactions:(NSArray <JMessageReaction *> *)reactions;

@end

NS_ASSUME_NONNULL_END
