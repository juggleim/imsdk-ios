//
//  JMessageContent.m
// JuggleIM
//
//  Created by Nathan on 2023/11/28.
//

#import "JMessageContent.h"

#define kUnknownType @"jg:unknown"

@implementation JMessageContent

+ (NSString *)contentType {
    return kUnknownType;
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCountable|JMessageFlagIsSave;
}

/// Data serialization method
-(NSData *)encode{
    return nil;
}

/// Data deserialization method
/// - Parameter date: Data content
-(void)decode:(NSData *)data{
    
}

- (NSString *)conversationDigest {
    return @"";
}

/// Local fuzzy search content
- (NSString *)searchContent{
    return @"";
}
@end
