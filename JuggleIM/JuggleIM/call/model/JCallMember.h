//
//  JCallMember.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/23.
//

#import <Foundation/Foundation.h>
#import "JCallConst.h"

NS_ASSUME_NONNULL_BEGIN

@interface JCallMember : NSObject
@property (nonatomic, copy) NSString *userId;
@property (nonatomic, assign) JCallStatus callStatus;
@end

NS_ASSUME_NONNULL_END
