//
//  JVersionDB.h
//  JuggleIM
//
//  Created by Fei Li on 2025/2/21.
//

#import <Foundation/Foundation.h>
#import "JDBHelper.h"

NS_ASSUME_NONNULL_BEGIN

@interface JVersionDB : NSObject

- (void)createTables;
- (void)updateTables;
- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper;

@end

NS_ASSUME_NONNULL_END
