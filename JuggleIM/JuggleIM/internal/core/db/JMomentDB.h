//
//  JMomentDB.h
//  JuggleIM
//
//  Created by Fei Li on 2025/11/17.
//

#import <Foundation/Foundation.h>
#import "JDBHelper.h"
#import "JMoment.h"
#import "JGetMomentOption.h"

NS_ASSUME_NONNULL_BEGIN

@interface JMomentDB : NSObject

- (void)createTables;
- (instancetype)initWithDBHelper:(JDBHelper *)dbHelper;

- (NSArray<JMoment *> *)getCachedMomentList:(JGetMomentOption *)option;
- (void)updateMomentList:(NSArray <JMoment *> *)momentList;
+ (NSString *)createMomentTable;
@end

NS_ASSUME_NONNULL_END
