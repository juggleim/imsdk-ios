//
//  JSimpleDataConverter.h
//  JuggleIM
//
//  Created by Fei Li on 2024/8/20.
//

#import <Foundation/Foundation.h>
#import "JDataConverterProtocol.h"

NS_ASSUME_NONNULL_BEGIN

@interface JSimpleDataConverter : NSObject <JDataConverterProtocol>
+ (instancetype)converter;
@end

NS_ASSUME_NONNULL_END
