//
//  JUltEncryptProtocol.h
//  JuggleIM
//
//  Created by Fei Li on 2025/8/20.
//

#ifndef JUltDataConverterProtocol_h
#define JUltDataConverterProtocol_h

#import <Foundation/Foundation.h>

@protocol JUltEncryptProtocol <NSObject>

- (NSData *)getPubKey;
- (void)storeSharedKey:(NSData *)key;
- (NSData *)encrypt:(NSData *)data;
- (NSData *)decrypt:(NSData *)data;

@end


#endif /* JUltDataConverterProtocol_h */
