//
//  JDBManager.h
//  Juggle
//
//  Created by Nathan on 2023/12/8.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JDBManager : NSObject

- (void)openIMDB:(NSString *)appKey
          userId:(NSString *)userId;

@end

NS_ASSUME_NONNULL_END
