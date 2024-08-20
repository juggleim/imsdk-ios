//
//  JSimpleDataConverter.m
//  JuggleIM
//
//  Created by Fei Li on 2024/8/20.
//

#import "JSimpleDataConverter.h"

@interface JSimpleDataConverter ()
@property (nonatomic, strong) NSData *key;
@end

@implementation JSimpleDataConverter

+ (instancetype)converter {
    uint32_t random1 = arc4random();
    uint32_t random2 = arc4random();
    uint64_t random8Bytes = ((uint64_t)random1 << 32) | random2;
    NSData *key = [NSData dataWithBytes:&random8Bytes length:sizeof(random8Bytes)];
    
    JSimpleDataConverter *converter = [[JSimpleDataConverter alloc] init];
    converter.key = key;
    return converter;
}

- (NSData *)encode:(NSData *)data {
    return [self xorData:data withKey:self.key];
}

- (NSData *)decode:(NSData *)data {
    return [self xorData:data withKey:self.key];
}

- (NSData *)xorData:(NSData *)sourceData
            withKey:(NSData *)keyData {
    Byte *keyBytes = (Byte *)[keyData bytes];
    NSUInteger keyLength = [keyData length];
    Byte *sourceBytes = (Byte *)[sourceData bytes];
    for (long i = 0; i < [sourceData length]; i++) {
        sourceBytes[i] = sourceBytes[i] ^ keyBytes[i%keyLength];
    }
    return sourceData;
}

@end
