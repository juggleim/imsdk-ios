//
//  JMomentReaction.h
//  JuggleIM
//
//  Created by Fei Li on 2025/11/11.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JUserInfo.h>

NS_ASSUME_NONNULL_BEGIN

@interface JMomentReaction : NSObject

@property (nonatomic, copy) NSString *key;
@property (nonatomic, strong) NSArray <JUserInfo *> *userArray;

- (NSDictionary *)toDictionary;
@end

NS_ASSUME_NONNULL_END
