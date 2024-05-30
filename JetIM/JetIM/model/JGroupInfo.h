//
//  JGroupInfo.h
//  JetIM
//
//  Created by Nathan on 2024/3/12.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JGroupInfo : NSObject

@property (nonatomic, copy) NSString *groupId;
@property (nonatomic, copy) NSString *groupName;
@property (nonatomic, copy) NSString *portrait;
@property (nonatomic, strong) NSDictionary <NSString *, NSString *> *extraDic;

@end

NS_ASSUME_NONNULL_END
