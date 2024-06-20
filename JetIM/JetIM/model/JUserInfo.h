//
//  JUserInfo.h
//  JetIM
//
//  Created by Nathan on 2024/3/9.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JUserInfo : NSObject

@property (nonatomic, copy) NSString *userId;
@property (nullable, nonatomic, copy) NSString *userName;
@property (nullable, nonatomic, copy) NSString *portrait;
@property (nullable, nonatomic, strong) NSDictionary <NSString *, NSString *> *extraDic;

@end

NS_ASSUME_NONNULL_END
