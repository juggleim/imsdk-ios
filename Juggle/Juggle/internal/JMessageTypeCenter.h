//
//  JMessageTypeCenter.h
//  Juggle
//
//  Created by Nathan on 2023/12/26.
//

#import <Foundation/Foundation.h>
#import "JMessageContent.h"

NS_ASSUME_NONNULL_BEGIN

@interface JMessageTypeCenter : NSObject
+ (instancetype)shared;

- (void)registerMessageType:(Class)messageClass;

- (JMessageContent *)contentWithData:(NSData *)data
                         contentType:(NSString *)type;
@end

NS_ASSUME_NONNULL_END
