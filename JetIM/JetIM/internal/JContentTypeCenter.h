//
//  JContentTypeCenter.h
//  JetIM
//
//  Created by Nathan on 2023/12/26.
//

#import <Foundation/Foundation.h>
#import "JMessageContent.h"

NS_ASSUME_NONNULL_BEGIN

@interface JContentTypeCenter : NSObject
+ (instancetype)shared;

- (void)registerContentType:(Class)messageClass;

- (JMessageContent *)contentWithData:(NSData *)data
                         contentType:(NSString *)type;

- (int)flagsWithType:(NSString *)type;
@end

NS_ASSUME_NONNULL_END
