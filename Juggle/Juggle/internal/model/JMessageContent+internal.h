//
//  JMessageContent+internal.h
//  Juggle
//
//  Created by Nathan on 2023/12/2.
//

#import <Foundation/Foundation.h>
#import "JMessageContent.h"

NS_ASSUME_NONNULL_BEGIN

@interface JMessageContent (internal)

- (NSData *)encode;
- (void)decode:(NSData *)data;

@end


NS_ASSUME_NONNULL_END
