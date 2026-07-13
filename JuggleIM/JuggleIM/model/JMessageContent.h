//
//  JMessageContent.h
// JuggleIM
//
//  Created by Nathan on 2023/11/28.
//

#import <Foundation/Foundation.h>

typedef NS_OPTIONS(int32_t, JMessageFlag) {
    JMessageFlagNone = 0,
    //Whether this is a command message
    JMessageFlagIsCmd = 1 << 0,
    //Whether this is countable message
    JMessageFlagIsCountable = 1 << 1,
    //Whether this is a status message
    JMessageFlagIsStatus = 1 << 2,
    //Whether this message is saved
    JMessageFlagIsSave = 1 << 3,
    //Whether this has been modified
    JMessageFlagIsModified = 1 << 4,
    //Whether this is a merged message
    JMessageFlagIsMerged = 1 << 5,
    //Whether this is a Do Not Disturb message
    JMessageFlagIsMute = 1 << 6,
    //Whether this is a broadcast message (these messages update the sender conversation list but not the conversation sort order; receivers treat them like normal messages)
    JMessageFlagIsBroadcast = 1 << 7,
    //Whether this is an encrypted message
    JMessageFlagIsE2EE = 1 << 11
};

NS_ASSUME_NONNULL_BEGIN
@interface JMessageContent : NSObject

/// Message type
+ (NSString *)contentType;

/// Message attributes; if unset, defaults to JMessageFlagIsCountable | JMessageFlagIsSave
+ (JMessageFlag)flags;

/// Data serialization method
- (NSData *)encode;

/// Data deserialization method, generating usable message content
/// - Parameter date: Data content
- (void)decode:(NSData *)data;

/// Message summary displayed in the conversation list
- (NSString *)conversationDigest;

/// Local fuzzy search content
- (NSString *)searchContent;

@end

NS_ASSUME_NONNULL_END
