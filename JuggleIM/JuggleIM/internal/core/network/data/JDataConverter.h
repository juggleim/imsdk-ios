//
//  JDataConverter.h
//  JuggleIM
//
//  Created by Fei Li on 2025/8/8.
//

#import <Foundation/Foundation.h>
#import "JDataConverterProtocol.h"

NS_ASSUME_NONNULL_BEGIN

@interface JDataConverter : NSObject <JDataConverterProtocol>

+ (instancetype)converter;

- (NSData *)getPubKey;
- (void)storeSharedKey:(NSData *)key;

@end

NS_ASSUME_NONNULL_END
