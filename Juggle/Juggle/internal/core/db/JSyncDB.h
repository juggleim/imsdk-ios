//
//  JSyncDB.h
//  Juggle
//
//  Created by Nathan on 2023/12/22.
//

#import <Foundation/Foundation.h>
#import "JDBHelper.h"

NS_ASSUME_NONNULL_BEGIN

@interface JSyncDB : NSObject

- (void)createTables;

- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper;
@end

NS_ASSUME_NONNULL_END
