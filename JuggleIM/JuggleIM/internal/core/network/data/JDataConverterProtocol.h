//
//  JDataConverterProtocol.h
//  JuggleIM
//
//  Created by Fei Li on 2024/8/20.
//

@protocol JDataConverterProtocol <NSObject>

- (NSData *)encode:(NSData *)data;

- (NSData *)decode:(NSData *)data;

@end
